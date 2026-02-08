#!/usr/bin/env python3
"""
Screenshot all screens in the UI showcase.
Dynamically detects available screens by running --list-screens.
"""

import subprocess
import json
import base64
import time
import sys
import os
import re
import signal

# Verbose logging flag
VERBOSE = True

def log(msg):
    """Print a log message with timestamp."""
    if VERBOSE:
        timestamp = time.strftime("%H:%M:%S")
        print(f"[{timestamp}] {msg}", file=sys.stderr)

def log_debug(msg):
    """Print a debug message."""
    if VERBOSE:
        print(f"  DEBUG: {msg}", file=sys.stderr)


class TimeoutError(Exception):
    pass


def timeout_handler(signum, frame):
    raise TimeoutError("Operation timed out")


class MCPClient:
    def __init__(self, executable, args):
        log(f"Starting process: {executable} {' '.join(args)}")
        # NOTE: stderr is set to PIPE to capture it, but we must drain it to avoid blocking
        # The C++ MCP code redirects stdout to stderr for logs, and writes JSON to a saved stdout fd
        self.proc = subprocess.Popen(
            [executable] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,  # Capture stderr separately
            text=True,
            bufsize=1
        )
        log(f"Process started with PID: {self.proc.pid}")
        
        # Collect validation warnings and errors
        self.validation_warnings = []
        self.validation_errors = []
        
        # Start a thread to drain stderr to prevent blocking
        import threading
        def drain_stderr():
            try:
                for line in self.proc.stderr:
                    stripped = line.rstrip()
                    # Remove ANSI color codes for cleaner matching
                    clean_line = re.sub(r'\x1b\[[0-9;]*m', '', stripped)
                    
                    # Capture UI validation messages (look for validation keywords)
                    is_validation = any(kw in clean_line for kw in [
                        '[UI Validation]', 'ContrastRatio', 'ScreenBounds', 
                        'ChildContainment', 'MinFontSize', 'validation'
                    ])
                    
                    if is_validation:
                        if 'LOG_WARN' in clean_line or 'WARN' in clean_line:
                            self.validation_warnings.append(clean_line)
                        elif 'LOG_ERROR' in clean_line or 'ERROR' in clean_line:
                            self.validation_errors.append(clean_line)
                        else:
                            self.validation_warnings.append(clean_line)
                    # Capture general errors (not validation-specific)
                    elif 'LOG_ERROR' in clean_line or '[ERROR]' in clean_line:
                        self.validation_errors.append(clean_line)
                    # Show truncated debug output for other lines
                    log_debug(f"[stderr] {stripped[:60]}")
            except:
                pass
        self.stderr_thread = threading.Thread(target=drain_stderr, daemon=True)
        self.stderr_thread.start()
        self.request_id = 0
        self.current_screen_index = 0
        self.screen_names = []
    
    def get_validation_summary(self):
        """Return collected validation warnings and errors."""
        return self.validation_warnings, self.validation_errors
        
    def set_screen_names(self, names):
        self.screen_names = names
        
    def get_current_screen_name(self):
        if self.screen_names and 0 <= self.current_screen_index < len(self.screen_names):
            return self.screen_names[self.current_screen_index]
        return f"screen_{self.current_screen_index}"
        
    def send_request(self, method, params=None):
        self.request_id += 1
        request = {
            "jsonrpc": "2.0",
            "id": self.request_id,
            "method": method,
        }
        if params:
            request["params"] = params
        
        request_str = json.dumps(request)
        log_debug(f"Sending request #{self.request_id}: {method}")
        
        try:
            self.proc.stdin.write(request_str + "\n")
            self.proc.stdin.flush()
            log_debug(f"Request sent, waiting for response...")
        except Exception as e:
            log(f"ERROR writing to stdin: {e}")
            return None
        
        # Read response - skip non-JSON lines with timeout tracking
        start_time = time.time()
        lines_read = 0
        max_wait = 15  # Max seconds to wait for a response
        
        while True:
            # Check if we've been waiting too long for a single response
            elapsed = time.time() - start_time
            if elapsed > max_wait:
                log(f"ERROR: Timeout waiting for response to {method} after {elapsed:.1f}s and {lines_read} lines")
                return None
            if elapsed > 5 and lines_read % 10 == 0:
                log(f"WARNING: Still waiting for {method} ({elapsed:.1f}s, {lines_read} lines)")
            
            try:
                response_str = self.proc.stdout.readline()
            except Exception as e:
                log(f"ERROR reading from stdout: {e}")
                return None
                
            if not response_str:
                log(f"ERROR: Got empty response (EOF) after {lines_read} lines")
                return None
            
            lines_read += 1
            response_str = response_str.strip()
            
            if not response_str:
                continue  # Skip empty lines
                
            if response_str.startswith('{'):
                try:
                    response = json.loads(response_str)
                    elapsed = time.time() - start_time
                    log_debug(f"Got response in {elapsed:.2f}s after {lines_read} lines")
                    return response
                except json.JSONDecodeError as e:
                    log_debug(f"JSON decode error on line {lines_read}: {e}")
                    log_debug(f"  Content: {response_str[:100]}...")
                    continue
            else:
                # Non-JSON line, skip it but log occasionally
                if lines_read <= 5 or lines_read % 20 == 0:
                    log_debug(f"Skipping non-JSON line {lines_read}: {response_str[:60]}...")
    
    def initialize(self):
        log("Initializing MCP connection...")
        result = self.send_request("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "screenshot_all", "version": "1.0"}
        })
        if result:
            log("MCP initialized successfully")
        else:
            log("ERROR: MCP initialization failed")
        return result
    
    def call_tool(self, name, arguments=None):
        params = {"name": name}
        if arguments:
            params["arguments"] = arguments
        return self.send_request("tools/call", params)
    
    def screenshot(self, save_path=None):
        log_debug(f"Taking screenshot...")
        response = self.call_tool("screenshot")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content["type"] == "image":
                data = base64.b64decode(content["data"])
                if save_path:
                    with open(save_path, "wb") as f:
                        f.write(data)
                    log(f"  Saved: {save_path} ({len(data):,} bytes)")
                return data
        log("  WARNING: Screenshot returned no data")
        return None
    
    def mouse_move(self, x, y):
        log_debug(f"Moving mouse to ({x}, {y})")
        result = self.call_tool("mouse_move", {"x": x, "y": y})
        return result
    
    def dump_ui_tree(self):
        """Get the UI component tree with interactivity info."""
        log_debug("Dumping UI tree...")
        response = self.call_tool("dump_ui_tree")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content.get("type") == "text":
                try:
                    return json.loads(content["text"])
                except json.JSONDecodeError:
                    log("WARNING: Failed to parse UI tree JSON")
                    return None
        log("WARNING: dump_ui_tree returned no data")
        return None

    def key_press(self, key):
        log_debug(f"Pressing key: {key}")
        result = self.call_tool("key_press", {"key": key})
        return result
    
    def next_screen(self):
        """Navigate to next screen and update index."""
        log_debug(f"Navigating to next screen (currently at index {self.current_screen_index})")
        result = self.key_press("page_down")
        self.current_screen_index += 1
        return result
    
    def ping(self):
        """Check if the MCP server is responding."""
        log("Pinging MCP server...")
        response = self.call_tool("ping")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content.get("text") == "pong":
                log("Ping successful: pong")
                return True
        log("Ping failed!")
        return False
    
    def exit_app(self):
        log("Sending exit command...")
        return self.call_tool("exit")
    
    def close(self):
        log("Closing MCP client...")
        try:
            self.exit_app()
            time.sleep(0.2)
        except Exception as e:
            log(f"Error during exit: {e}")
        try:
            self.proc.stdin.close()
            self.proc.terminate()
            log("Waiting for process to terminate...")
            self.proc.wait(timeout=2)
            log("Process terminated")
        except subprocess.TimeoutExpired:
            log("Process didn't terminate, killing...")
            self.proc.kill()
        except Exception as e:
            log(f"Error during close: {e}")


def find_interactive_elements(tree_data, max_elements=5):
    """Walk the UI tree and find visible, clickable elements.
    Returns list of (label, center_x, center_y, width, height) tuples."""
    results = []
    
    def walk(node):
        if len(results) >= max_elements:
            return
        
        # Check if this node is visible and clickable
        is_visible = node.get("visible", False)
        is_clickable = node.get("clickable", False)
        rect = node.get("rect", {})
        w = rect.get("width", 0)
        h = rect.get("height", 0)
        
        if is_visible and is_clickable and w > 10 and h > 10:
            x = rect.get("x", 0)
            y = rect.get("y", 0)
            label = node.get("label", node.get("name", f"id:{node.get('id', '?')}"))
            center_x = int(x + w / 2)
            center_y = int(y + h / 2)
            results.append((label, center_x, center_y, int(w), int(h)))
        
        # Recurse into children
        for child in node.get("children", []):
            walk(child)
    
    if tree_data and "tree" in tree_data:
        for root in tree_data["tree"]:
            walk(root)
    
    return results


def get_screen_count(executable):
    """Run --list-screens and count the number of available screens."""
    log(f"Running: {executable} --list-screens")
    result = subprocess.run(
        [executable, "--list-screens"],
        capture_output=True,
        text=True,
        timeout=10
    )
    
    # Count lines that start with "--screen="
    count = 0
    screen_names = []
    for line in result.stdout.split('\n'):
        match = re.search(r'--screen=(\S+)', line)
        if match:
            count += 1
            screen_names.append(match.group(1))
    
    # IMPORTANT: Sort alphabetically - this matches the internal order
    # used by page_down cycling (std::map<std::string> is alphabetically ordered)
    screen_names.sort()
    
    log(f"Found {count} screens")
    return count, screen_names


def ensure_build():
    """Run make to ensure executable is up-to-date."""
    executable = "./output/ui_tester.exe"
    
    # Check if executable exists
    if not os.path.exists(executable):
        log("Executable not found, running make...")
        need_build = True
    else:
        # Check if any source file is newer than executable
        exe_mtime = os.path.getmtime(executable)
        need_build = False
        for root, dirs, files in os.walk("src"):
            for f in files:
                if f.endswith(('.cpp', '.h')):
                    src_path = os.path.join(root, f)
                    if os.path.getmtime(src_path) > exe_mtime:
                        log(f"Source file {src_path} is newer than executable, rebuilding...")
                        need_build = True
                        break
            if need_build:
                break
    
    if need_build:
        log("Running make...")
        result = subprocess.run(["make"], capture_output=True, text=True, timeout=300)
        if result.returncode != 0:
            log(f"ERROR: make failed with code {result.returncode}")
            log(result.stderr)
            return False
        log("Build successful")
    else:
        log("Executable is up-to-date")
    
    return True


def main():
    executable = "./output/ui_tester.exe"
    output_dir = "/tmp/ui_showcase_screenshots"
    
    # Parse args
    with_hover = "--with-hover" in sys.argv
    hover_count = 3  # Number of interactive elements to hover per screen
    for arg in sys.argv:
        if arg.startswith("--hover-count="):
            hover_count = int(arg.split("=")[1])
    
    os.makedirs(output_dir, exist_ok=True)
    if with_hover:
        hover_dir = os.path.join(output_dir, "hover")
        os.makedirs(hover_dir, exist_ok=True)
    
    log("=" * 60)
    log("Screenshot All Screens - Starting")
    if with_hover:
        log(f"  Hover mode: ON (capturing up to {hover_count} hover states per screen)")
    log("=" * 60)
    
    # Ensure build is up-to-date
    if not ensure_build():
        return 1
    
    # Get screen count dynamically
    log("Detecting available screens...")
    try:
        screen_count, screen_names = get_screen_count(executable)
    except subprocess.TimeoutExpired:
        log("ERROR: --list-screens timed out!")
        return 1
    except FileNotFoundError:
        log(f"ERROR: Executable not found: {executable}")
        return 1
    
    if screen_count == 0:
        log("ERROR: No screens found!")
        return 1
    
    log(f"Screens: {', '.join(screen_names)}")
    log(f"Output directory: {output_dir}")
    log("=" * 60)
    
    # Set up overall timeout
    signal.signal(signal.SIGALRM, timeout_handler)
    timeout_secs = 600 if with_hover else 180  # 10 min with hover, 3 min without
    signal.alarm(timeout_secs)
    
    client = None
    try:
        # Start app on first screen (alphabetical order)
        first_screen = screen_names[0] if screen_names else "buttons"
        log(f"Starting app on screen: {first_screen}")
        client = MCPClient(executable, ["--mcp", f"--screen={first_screen}"])
        client.set_screen_names(screen_names)
        
        # Wait for app to start
        log("Waiting for app to start...")
        time.sleep(1.0)
        
        # Check if process is still running
        if client.proc.poll() is not None:
            log(f"ERROR: Process exited with code {client.proc.returncode}")
            return 1
        
        # Initialize and verify connection
        init_result = client.initialize()
        if not init_result:
            log("ERROR: Initialize returned None")
            return 1
            
        time.sleep(0.3)
        
        if not client.ping():
            log("ERROR: Failed to connect to MCP server")
            client.close()
            return 1
        
        log("Connected to MCP server!")
        log("=" * 60)
        
        results = {}
        hover_results = {}
        start_time = time.time()
        
        for i in range(screen_count):
            screen_name = client.get_current_screen_name()
            log(f"[{i+1}/{screen_count}] Capturing: {screen_name}")
            
            # Wait for screen to render fully
            time.sleep(0.6)
            
            # Take default screenshot (no hover)
            # First move mouse off-screen to ensure clean default state
            if with_hover:
                client.mouse_move(0, 0)
                time.sleep(0.2)
            
            screenshot_path = os.path.join(output_dir, f"{screen_name}.png")
            if client.screenshot(screenshot_path):
                results[screen_name] = True
            else:
                log(f"  FAILED to capture: {screen_name}")
                results[screen_name] = False
            
            # Capture hover-state screenshots if requested
            if with_hover and results.get(screen_name):
                tree = client.dump_ui_tree()
                interactive = find_interactive_elements(tree, max_elements=hover_count)
                
                if interactive:
                    log(f"  Found {len(interactive)} interactive elements")
                    hover_results[screen_name] = []
                    
                    for idx, (label, cx, cy, w, h) in enumerate(interactive):
                        # Move mouse to center of element
                        client.mouse_move(cx, cy)
                        time.sleep(0.15)  # Wait for hover state to render
                        
                        # Take hover screenshot
                        safe_label = re.sub(r'[^\w\-]', '_', label[:30])
                        hover_path = os.path.join(
                            hover_dir, f"{screen_name}_hover_{idx}_{safe_label}.png"
                        )
                        if client.screenshot(hover_path):
                            hover_results[screen_name].append(
                                (label, hover_path)
                            )
                            log(f"    hover[{idx}]: '{label}' at ({cx},{cy})")
                        else:
                            log(f"    FAILED hover for '{label}'")
                    
                    # Move mouse off-screen to reset for next screen
                    client.mouse_move(0, 0)
                    time.sleep(0.1)
                else:
                    log(f"  No interactive elements found")
            
            # Navigate to next screen (unless this is the last one)
            if i < screen_count - 1:
                client.next_screen()
                time.sleep(1.2)  # Longer pause for screen transition to complete
        
        elapsed = time.time() - start_time
        
        # Get validation summary before closing
        warnings, errors = client.get_validation_summary()
        
        # Clean exit
        client.close()
        client = None
        
        log("=" * 60)
        log(f"Completed in {elapsed:.1f}s")
        log(f"Results: {sum(results.values())}/{len(results)} succeeded")
        
        for screen, success in results.items():
            status = "✓" if success else "✗"
            hover_info = ""
            if with_hover and screen in hover_results:
                hover_info = f" (+{len(hover_results[screen])} hover)"
            print(f"  {status} {screen}{hover_info}", file=sys.stderr)
        
        if with_hover:
            total_hovers = sum(len(v) for v in hover_results.values())
            screens_with_hovers = len(hover_results)
            print(f"\n  Hover screenshots: {total_hovers} across {screens_with_hovers} screens", file=sys.stderr)
            print(f"  Hover output: {hover_dir}", file=sys.stderr)
        
        # Print validation summary
        if warnings or errors:
            print("\n" + "=" * 60, file=sys.stderr)
            print("VALIDATION SUMMARY", file=sys.stderr)
            print("=" * 60, file=sys.stderr)
            
            if errors:
                print(f"\n❌ ERRORS ({len(errors)}):", file=sys.stderr)
                # Deduplicate similar errors
                unique_errors = list(dict.fromkeys(errors))
                for err in unique_errors[:20]:  # Show max 20
                    print(f"  {err}", file=sys.stderr)
                if len(unique_errors) > 20:
                    print(f"  ... and {len(unique_errors) - 20} more", file=sys.stderr)
            
            if warnings:
                print(f"\n⚠️  WARNINGS ({len(warnings)}):", file=sys.stderr)
                # Deduplicate similar warnings
                unique_warnings = list(dict.fromkeys(warnings))
                for warn in unique_warnings[:20]:  # Show max 20
                    print(f"  {warn}", file=sys.stderr)
                if len(unique_warnings) > 20:
                    print(f"  ... and {len(unique_warnings) - 20} more", file=sys.stderr)
            
            print("=" * 60, file=sys.stderr)
        else:
            print("\n✅ No validation warnings or errors detected!", file=sys.stderr)
        
        return 0 if all(results.values()) else 1
            
    except TimeoutError:
        log("ERROR: Overall timeout exceeded (180s)")
        return 1
    except KeyboardInterrupt:
        log("Interrupted by user")
        return 1
    except Exception as e:
        log(f"ERROR: {e}")
        import traceback
        traceback.print_exc()
        return 1
    finally:
        signal.alarm(0)
        if client:
            try:
                client.close()
            except:
                pass


if __name__ == "__main__":
    sys.exit(main())
