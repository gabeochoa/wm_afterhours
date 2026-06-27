#!/usr/bin/env python3
"""Take screenshots of all UI showcase screens via MCP - uses key navigation instead of restarting."""

import json
import subprocess
import sys
import time
import base64
import os
import signal

class TimeoutError(Exception):
    pass

def timeout_handler(signum, frame):
    raise TimeoutError("Operation timed out")

class MCPClient:
    def __init__(self, executable, args):
        self.proc = subprocess.Popen(
            [executable] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True,
            bufsize=1
        )
        self.request_id = 0
        
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
        self.proc.stdin.write(request_str + "\n")
        self.proc.stdin.flush()
        
        # Read response - skip non-JSON lines
        while True:
            response_str = self.proc.stdout.readline()
            if not response_str:
                return None
            
            response_str = response_str.strip()
            if response_str.startswith('{'):
                try:
                    return json.loads(response_str)
                except json.JSONDecodeError:
                    continue
    
    def initialize(self):
        return self.send_request("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "screenshot_test", "version": "1.0"}
        })
    
    def call_tool(self, name, arguments=None):
        params = {"name": name}
        if arguments:
            params["arguments"] = arguments
        return self.send_request("tools/call", params)
    
    def ping(self):
        response = self.call_tool("ping")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content.get("text") == "pong":
                return True
        return False
    
    def screenshot(self, save_path):
        response = self.call_tool("screenshot")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content["type"] == "image":
                data = base64.b64decode(content["data"])
                with open(save_path, "wb") as f:
                    f.write(data)
                return True
        return False
    
    def key_press(self, key):
        """Press a key (e.g., '.' for next screen, ',' for prev)"""
        return self.call_tool("key_press", {"key": key})
    
    def exit_app(self):
        return self.call_tool("exit")
    
    def close(self):
        try:
            self.exit_app()
            time.sleep(0.2)
        except:
            pass
        try:
            self.proc.stdin.close()
            self.proc.terminate()
            self.proc.wait(timeout=2)
        except:
            self.proc.kill()

def main():
    # Screen order matches what the app cycles through with '.' key
    # Use exact names from --list-screens
    screens = ["buttons", "cards", "colors", "forms", "layout", "simple_button", "tabbing", "text"]
    output_dir = "/tmp/ui_showcase_screenshots"
    os.makedirs(output_dir, exist_ok=True)
    
    exe = "./output/ui_tester.exe"
    
    # Set up overall timeout
    signal.signal(signal.SIGALRM, timeout_handler)
    signal.alarm(120)  # 2 minute total timeout
    
    print(f"Capturing {len(screens)} screens to {output_dir}", file=sys.stderr)
    print("=" * 50, file=sys.stderr)
    
    try:
        # Start app on first screen
        print(f"Starting app on screen: {screens[0]}", file=sys.stderr)
        client = MCPClient(exe, ["--mcp", f"--screen={screens[0]}"])
        
        # Wait for app to start
        time.sleep(0.8)
        
        # Initialize and verify connection
        client.initialize()
        time.sleep(0.2)
        
        if not client.ping():
            print("ERROR: Failed to connect to MCP server", file=sys.stderr)
            client.close()
            return
        
        print("Connected to MCP server", file=sys.stderr)
        
        results = {}
        for i, screen in enumerate(screens):
            print(f"Capturing: {screen}", file=sys.stderr)
            
            # Wait for screen to render
            time.sleep(0.3)
            
            # Take screenshot
            screenshot_path = os.path.join(output_dir, f"{screen}.png")
            if client.screenshot(screenshot_path):
                print(f"  OK: {screenshot_path}", file=sys.stderr)
                results[screen] = True
            else:
                print(f"  FAILED: {screen}", file=sys.stderr)
                results[screen] = False
            
            # Switch to next screen (unless this is the last one)
            if i < len(screens) - 1:
                client.key_press(".")
                time.sleep(0.2)  # Brief pause for screen transition
        
        # Clean exit
        client.close()
        
        print("=" * 50, file=sys.stderr)
        print(f"Results: {sum(results.values())}/{len(results)} succeeded", file=sys.stderr)
        
        for screen, success in results.items():
            status = "✓" if success else "✗"
            print(f"  {status} {screen}", file=sys.stderr)
            
    except TimeoutError:
        print("ERROR: Overall timeout exceeded", file=sys.stderr)
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
    finally:
        signal.alarm(0)

if __name__ == "__main__":
    main()
