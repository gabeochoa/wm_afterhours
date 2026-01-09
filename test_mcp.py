#!/usr/bin/env python3
"""
MCP test script - starts the game, takes screenshots, and clicks buttons.
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
        print(f">>> Sending: {method}", file=sys.stderr)
        self.proc.stdin.write(request_str + "\n")
        self.proc.stdin.flush()
        
        # Read response - skip non-JSON lines
        while True:
            response_str = self.proc.stdout.readline()
            if not response_str:
                print("No response received!", file=sys.stderr)
                return None
            
            response_str = response_str.strip()
            if response_str.startswith('{'):
                try:
                    response = json.loads(response_str)
                    return response
                except json.JSONDecodeError:
                    continue
            # Skip non-JSON output
    
    def initialize(self):
        return self.send_request("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "test", "version": "1.0"}
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
                    print(f"Screenshot saved to {save_path}", file=sys.stderr)
                return data
        return None
    
    def get_screen_size(self):
        response = self.call_tool("get_screen_size")
        print(f"get_screen_size response: {json.dumps(response, indent=2)}", file=sys.stderr)
        if response and "result" in response:
            content = response["result"]["content"][0]
            if "text" in content:
                text = content["text"]
                if text:
                    return json.loads(text)
            # Maybe it's directly in content
            if "width" in content:
                return content
        return None
    
    def mouse_click(self, x, y):
        return self.call_tool("mouse_click", {"x": x, "y": y})
    
    def mouse_move(self, x, y):
        return self.call_tool("mouse_move", {"x": x, "y": y})
    
    def key_press(self, key):
        return self.call_tool("key_press", {"key": key})
    
    def close(self):
        self.proc.stdin.close()
        self.proc.terminate()
        self.proc.wait()


def main():
    print("Starting UI Tester with MCP...", file=sys.stderr)
    
    client = MCPClient("./output/ui_tester.exe", ["--mcp", "--simple_button"])
    
    try:
        # Initialize
        print("\n=== Initializing MCP ===", file=sys.stderr)
        init_response = client.initialize()
        print(f"Init response: {json.dumps(init_response, indent=2)}", file=sys.stderr)
        
        # Give the game a moment to render
        time.sleep(0.5)
        
        # Get screen size
        print("\n=== Getting screen size ===", file=sys.stderr)
        size = client.get_screen_size()
        print(f"Screen size: {size}", file=sys.stderr)
        
        # Take initial screenshot
        print("\n=== Taking initial screenshot ===", file=sys.stderr)
        client.screenshot("/tmp/mcp_before_click.png")
        
        # Click the button (center of screen based on the simple_button layout)
        # The button appears to be centered, so let's click at approximately center
        if size:
            # Button appears to be around center vertically, maybe slightly up
            center_x = size["width"] // 2
            button_y = 360  # Approximate button position from screenshot
            
            print(f"\n=== Clicking at ({center_x}, {button_y}) ===", file=sys.stderr)
            
            # Need to move mouse first, then click
            move_response = client.mouse_move(center_x, button_y)
            print(f"Move response: {json.dumps(move_response, indent=2)}", file=sys.stderr)
            time.sleep(0.1)
            
            click_response = client.mouse_click(center_x, button_y)
            print(f"Click response: {json.dumps(click_response, indent=2)}", file=sys.stderr)
            time.sleep(0.2)
            
            # Click again to make sure
            click_response = client.mouse_click(center_x, button_y)
            print(f"Click 2 response: {json.dumps(click_response, indent=2)}", file=sys.stderr)
        
        # Wait for the click to process and UI to update
        time.sleep(0.5)
        
        # Take screenshot after click
        print("\n=== Taking screenshot after click ===", file=sys.stderr)
        client.screenshot("/tmp/mcp_after_click.png")
        
        print("\n=== Test complete! ===", file=sys.stderr)
        print("Screenshots saved to /tmp/mcp_before_click.png and /tmp/mcp_after_click.png", file=sys.stderr)
        
    finally:
        client.close()


if __name__ == "__main__":
    main()

