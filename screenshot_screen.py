#!/usr/bin/env python3
"""
Quick screenshot utility for a single screen using MCP.

Usage:
    python3 screenshot_screen.py <screen_name> [output_path]
    
Examples:
    python3 screenshot_screen.py flex_alignment
    python3 screenshot_screen.py flex_alignment /tmp/test.png
"""

import sys
import os
import subprocess
import json
import time
import threading
import base64

class MCPClient:
    def __init__(self, executable, args):
        self.proc = subprocess.Popen(
            [executable] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        self.request_id = 0
        
        # Drain stderr in background
        def drain_stderr():
            try:
                for line in self.proc.stderr:
                    pass  # Silently consume
            except:
                pass
        self.stderr_thread = threading.Thread(target=drain_stderr, daemon=True)
        self.stderr_thread.start()
    
    def send_request(self, method, params=None, timeout=10):
        self.request_id += 1
        request = {
            "jsonrpc": "2.0",
            "id": self.request_id,
            "method": method,
            "params": params or {}
        }
        msg = json.dumps(request) + "\n"
        
        try:
            self.proc.stdin.write(msg)
            self.proc.stdin.flush()
            
            # Read response
            response_line = self.proc.stdout.readline()
            if response_line:
                return json.loads(response_line)
            return None
        except Exception as e:
            print(f"Error: {e}")
            return None
    
    def initialize(self):
        return self.send_request("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "screenshot_screen", "version": "1.0"}
        })
    
    def call_tool(self, name, arguments=None):
        params = {"name": name}
        if arguments:
            params["arguments"] = arguments
        return self.send_request("tools/call", params)
    
    def screenshot(self, save_path):
        response = self.call_tool("screenshot")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content["type"] == "image":
                data = base64.b64decode(content["data"])
                with open(save_path, "wb") as f:
                    f.write(data)
                return len(data)
        return 0
    
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
    if len(sys.argv) < 2:
        print("Usage: python3 screenshot_screen.py <screen_name> [output_path]")
        print("Example: python3 screenshot_screen.py flex_alignment")
        sys.exit(1)
    
    screen_name = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else f"/tmp/{screen_name}.png"
    
    # Find executable
    script_dir = os.path.dirname(os.path.abspath(__file__))
    executable = os.path.join(script_dir, "output", "ui_tester.exe")
    
    if not os.path.exists(executable):
        print(f"Error: {executable} not found. Run 'make' first.")
        sys.exit(1)
    
    print(f"Starting with screen: {screen_name}")
    # Start with --mcp AND --screen to go directly to that screen
    client = MCPClient(executable, ["--mcp", f"--screen={screen_name}"])
    
    # Wait for app to initialize and navigate to screen
    time.sleep(2.0)
    
    try:
        # Initialize MCP
        resp = client.initialize()
        if not resp:
            print("Failed to initialize MCP")
            sys.exit(1)
        
        # Wait for render
        time.sleep(0.3)
        
        # Take screenshot
        print(f"Taking screenshot...")
        size = client.screenshot(output_path)
        
        if size > 0:
            print(f"✓ Saved: {output_path} ({size:,} bytes)")
        else:
            print(f"Error: Screenshot failed")
            
    finally:
        client.close()

if __name__ == "__main__":
    main()
