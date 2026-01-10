#!/usr/bin/env python3
"""
Screenshot all screens in the UI showcase.
"""

import subprocess
import json
import base64
import time
import sys
import os

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
                    response = json.loads(response_str)
                    return response
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
    
    def screenshot(self, save_path=None):
        response = self.call_tool("screenshot")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content["type"] == "image":
                data = base64.b64decode(content["data"])
                if save_path:
                    with open(save_path, "wb") as f:
                        f.write(data)
                    print(f"  Saved: {save_path}")
                return data
        return None
    
    def key_press(self, key):
        return self.call_tool("key_press", {"key": key})
    
    def ping(self):
        """Check if the MCP server is responding."""
        response = self.call_tool("ping")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content.get("text") == "pong":
                return True
        return False
    
    def close(self):
        self.proc.stdin.close()
        self.proc.terminate()
        self.proc.wait()


SCREENS = [
    "buttons",
    "cards", 
    "colors",
    "forms",
    "layout",
    "simple_button",
    "tabbing",
    "text",
]


def main():
    # Create output directory
    output_dir = "/tmp/ui_showcase_screenshots"
    os.makedirs(output_dir, exist_ok=True)
    
    print(f"Screenshots will be saved to: {output_dir}")
    print("=" * 50)
    
    # Start with the first screen
    client = MCPClient("./output/ui_tester.exe", ["--mcp", f"--screen={SCREENS[0]}"])
    
    try:
        # Initialize
        print("Initializing MCP...")
        init_response = client.initialize()
        if not init_response:
            print("Failed to initialize MCP!")
            return
        
        # Give the game a moment to render
        time.sleep(0.3)
        
        # Ping to verify server is ready
        print("Pinging MCP server...")
        if client.ping():
            print("  Server is ready!")
        else:
            print("  Warning: Ping failed, continuing anyway...")
        
        time.sleep(0.2)
        
        for i, screen_name in enumerate(SCREENS):
            print(f"\nScreen {i+1}/{len(SCREENS)}: {screen_name}")
            
            # Take screenshot
            screenshot_path = os.path.join(output_dir, f"{screen_name}.png")
            client.screenshot(screenshot_path)
            
            # Navigate to next screen (press '.' key = period = KEY_PERIOD = 46)
            if i < len(SCREENS) - 1:
                # Key code for '.' is 46
                client.key_press({"key": "period"})
                time.sleep(0.3)  # Wait for screen transition
        
        print("\n" + "=" * 50)
        print(f"Done! {len(SCREENS)} screenshots saved to {output_dir}")
        print("\nScreenshots:")
        for screen in SCREENS:
            path = os.path.join(output_dir, f"{screen}.png")
            if os.path.exists(path):
                size = os.path.getsize(path)
                print(f"  {screen}.png ({size:,} bytes)")
        
    finally:
        client.close()


if __name__ == "__main__":
    main()

