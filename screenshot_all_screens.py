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
        
        # Start a thread to drain stderr to prevent blocking
        import threading
        def drain_stderr():
            try:
                for line in self.proc.stderr:
                    log_debug(f"[stderr] {line.rstrip()[:60]}")
            except:
                pass
        self.stderr_thread = threading.Thread(target=drain_stderr, daemon=True)
        self.stderr_thread.start()
        self.request_id = 0
        self.current_screen_index = 0
        self.screen_names = []
        
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
    
    log(f"Found {count} screens")
    return count, screen_names


def main():
    executable = "./output/ui_tester.exe"
    output_dir = "/tmp/ui_showcase_screenshots"
    os.makedirs(output_dir, exist_ok=True)
    
    log("=" * 60)
    log("Screenshot All Screens - Starting")
    log("=" * 60)
    
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
    signal.alarm(180)  # 3 minute total timeout
    
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
        start_time = time.time()
        
        for i in range(screen_count):
            screen_name = client.get_current_screen_name()
            log(f"[{i+1}/{screen_count}] Capturing: {screen_name}")
            
            # Wait for screen to render
            time.sleep(0.4)
            
            # Take screenshot
            screenshot_path = os.path.join(output_dir, f"{screen_name}.png")
            if client.screenshot(screenshot_path):
                results[screen_name] = True
            else:
                log(f"  FAILED to capture: {screen_name}")
                results[screen_name] = False
            
            # Navigate to next screen (unless this is the last one)
            if i < screen_count - 1:
                client.next_screen()
                time.sleep(0.3)  # Brief pause for screen transition
        
        elapsed = time.time() - start_time
        
        # Clean exit
        client.close()
        client = None
        
        log("=" * 60)
        log(f"Completed in {elapsed:.1f}s")
        log(f"Results: {sum(results.values())}/{len(results)} succeeded")
        
        for screen, success in results.items():
            status = "✓" if success else "✗"
            print(f"  {status} {screen}", file=sys.stderr)
        
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
