# MCP (Model Context Protocol) Integration Guide

This project includes an MCP server that enables AI assistants and automated testing tools to interact with the UI application programmatically. The MCP server follows the [Model Context Protocol](https://modelcontextprotocol.io/) specification.

## Quick Start

### Starting the App in MCP Mode

```bash
./output/ui_tester.exe --mcp
```

You can also specify a starting screen:

```bash
./output/ui_tester.exe --mcp --screen=simple_button
```

### Basic Python Client

```python
import subprocess
import json

# Start the app with MCP enabled
proc = subprocess.Popen(
    ["./output/ui_tester.exe", "--mcp"],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.DEVNULL,
    text=True,
    bufsize=1
)

# Send initialize request
request = {
    "jsonrpc": "2.0",
    "id": 1,
    "method": "initialize",
    "params": {
        "protocolVersion": "2024-11-05",
        "capabilities": {},
        "clientInfo": {"name": "my-client", "version": "1.0"}
    }
}
proc.stdin.write(json.dumps(request) + "\n")
proc.stdin.flush()

# Read response
response = json.loads(proc.stdout.readline())
print(response)
```

## Available Tools

The MCP server exposes the following tools:

### `ping`
Check if the MCP server is running and responsive.

```json
{"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {"name": "ping"}}
```

Response: `{"text": "pong"}`

### `screenshot`
Capture a screenshot of the current frame as a base64-encoded PNG.

```json
{"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {"name": "screenshot"}}
```

Response contains `{"type": "image", "data": "<base64>", "mimeType": "image/png"}`

### `get_screen_size`
Get the current window dimensions.

```json
{"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {"name": "get_screen_size"}}
```

Response: `{"width": 1280, "height": 720}`

### `mouse_move`
Move the mouse cursor to screen coordinates.

```json
{
  "jsonrpc": "2.0", "id": 1, 
  "method": "tools/call", 
  "params": {
    "name": "mouse_move",
    "arguments": {"x": 640, "y": 360}
  }
}
```

### `mouse_click`
Click at the specified screen coordinates.

```json
{
  "jsonrpc": "2.0", "id": 1,
  "method": "tools/call",
  "params": {
    "name": "mouse_click",
    "arguments": {"x": 640, "y": 360, "button": "left"}
  }
}
```

- `button`: `"left"` (default) or `"right"`

### `key_press`
Press and release a key.

```json
{
  "jsonrpc": "2.0", "id": 1,
  "method": "tools/call",
  "params": {
    "name": "key_press",
    "arguments": {"key": "enter"}
  }
}
```

### `key_down` / `key_up`
Hold or release a key (for key combinations or held inputs).

```json
{"method": "tools/call", "params": {"name": "key_down", "arguments": {"key": "shift"}}}
{"method": "tools/call", "params": {"name": "key_press", "arguments": {"key": "a"}}}
{"method": "tools/call", "params": {"name": "key_up", "arguments": {"key": "shift"}}}
```

### `dump_ui_tree`
Dump the UI component tree showing positions, sizes, and hierarchy.

```json
{"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {"name": "dump_ui_tree"}}
```

### `exit`
Request the application to close gracefully.

```json
{"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {"name": "exit"}}
```

## Supported Key Names

The following key names are supported (case-insensitive):

### Letters & Numbers
- `a` through `z`
- `0` through `9`

### Punctuation
- `space`, `apostrophe` (`'`), `comma` (`,`), `minus` (`-`)
- `period` (`.`), `slash` (`/`), `semicolon` (`;`), `equal` (`=`)
- `left_bracket` (`[`), `right_bracket` (`]`), `backslash` (`\`), `grave` (`` ` ``)

### Navigation & Control
- `escape` / `esc`
- `enter` / `return`
- `tab`, `backspace`
- `insert`, `delete`
- `right`, `left`, `up`, `down`
- `page_up` / `pageup`, `page_down` / `pagedown`
- `home`, `end`

### Function Keys
- `f1` through `f25`

### Modifier Keys
- `shift` / `left_shift` / `lshift`
- `ctrl` / `control` / `left_control` / `lctrl`
- `alt` / `left_alt` / `lalt`
- `super` / `cmd` / `command` / `win` / `windows`

### Numpad
- `kp_0` through `kp_9` / `numpad0` through `numpad9`
- `kp_decimal`, `kp_divide`, `kp_multiply`, `kp_subtract`, `kp_add`, `kp_enter`

## Complete Python Client Example

See `test_mcp.py` for a complete working example:

```python
#!/usr/bin/env python3
import subprocess
import json
import base64
import time

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
        
        self.proc.stdin.write(json.dumps(request) + "\n")
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
                return data
        return None
    
    def key_press(self, key):
        return self.call_tool("key_press", {"key": key})
    
    def mouse_click(self, x, y, button="left"):
        return self.call_tool("mouse_click", {"x": x, "y": y, "button": button})
    
    def close(self):
        self.call_tool("exit")
        self.proc.stdin.close()
        self.proc.wait()

# Usage
client = MCPClient("./output/ui_tester.exe", ["--mcp", "--screen=simple_button"])
client.initialize()
time.sleep(0.3)

client.screenshot("/tmp/before.png")
client.mouse_click(640, 360)
time.sleep(0.2)
client.screenshot("/tmp/after.png")

client.close()
```

## Protocol Details

### Communication
- All communication is over stdin/stdout using newline-delimited JSON
- Follows JSON-RPC 2.0 protocol
- stderr is used for application logs (filtered to errors only in MCP mode)

### Initialization Sequence
1. Client sends `initialize` request
2. Server responds with capabilities
3. Client can optionally send `notifications/initialized`
4. Client can now call tools

### Build Configuration
MCP support is enabled by the `AFTER_HOURS_ENABLE_MCP` define (set in the Makefile). If disabled, `--mcp` flag will not be available.

## Use Cases

### Automated UI Testing
```python
# Click button, take screenshot, verify state
client.mouse_click(640, 360)
time.sleep(0.1)
screenshot_data = client.screenshot()
# Compare with expected image...
```

### Screen Navigation Testing
```python
# Cycle through all screens
for _ in range(14):
    client.key_press(",")  # Previous screen
    time.sleep(0.2)
    client.screenshot(f"/tmp/screen_{_}.png")
```

### AI Assistant Integration
The MCP protocol allows AI assistants to:
- Take screenshots to understand UI state
- Click buttons and interact with controls
- Navigate between screens
- Verify UI behavior programmatically






