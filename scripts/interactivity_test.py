#!/usr/bin/env python3
"""
Generic interactivity testing for UI screens via MCP.

This script tests interactive elements (sliders, toggles, buttons, tabs) 
by finding them in the UI tree and simulating user interactions.

Usage:
    python scripts/interactivity_test.py                      # Test all screens
    python scripts/interactivity_test.py --screen=X           # Test specific screen
    python scripts/interactivity_test.py --list               # List available screens
    python scripts/interactivity_test.py --verbose            # Show detailed output
    python scripts/interactivity_test.py --save-screenshots   # Save before/after images

Screen test definitions are in SCREEN_TESTS dictionary.
"""

import argparse
import base64
import json
import os
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Dict, List, Optional, Tuple

EXECUTABLE = "./output/ui_tester.exe"
SCREENSHOT_DIR = Path("/tmp/interactivity_tests")
TIMEOUT = 30


@dataclass
class UIElement:
    """Represents a UI element from the tree dump."""
    id: int
    name: str
    rect: dict  # {x, y, width, height}
    visible: bool
    children: List['UIElement']
    
    @property
    def center(self) -> Tuple[int, int]:
        """Get center point of element."""
        return (
            int(self.rect['x'] + self.rect['width'] / 2),
            int(self.rect['y'] + self.rect['height'] / 2)
        )
    
    @property
    def left_edge(self) -> Tuple[int, int]:
        """Get left edge point for slider drag start."""
        return (
            int(self.rect['x'] + 5),
            int(self.rect['y'] + self.rect['height'] / 2)
        )
    
    @property
    def right_edge(self) -> Tuple[int, int]:
        """Get right edge point for slider drag end."""
        return (
            int(self.rect['x'] + self.rect['width'] - 5),
            int(self.rect['y'] + self.rect['height'] / 2)
        )


class MCPClient:
    """Client for communicating with the MCP-enabled app."""
    
    def __init__(self, executable: str, args: List[str], verbose: bool = False):
        self.verbose = verbose
        self.proc = subprocess.Popen(
            [executable] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True,
            bufsize=1
        )
        self.request_id = 0
        
    def _log(self, msg: str):
        if self.verbose:
            print(f"  [MCP] {msg}", file=sys.stderr)
    
    def send_request(self, method: str, params: dict = None) -> Optional[dict]:
        """Send JSON-RPC request and get response."""
        self.request_id += 1
        request = {
            "jsonrpc": "2.0",
            "id": self.request_id,
            "method": method,
        }
        if params:
            request["params"] = params
        
        request_str = json.dumps(request)
        self._log(f">>> {method}")
        self.proc.stdin.write(request_str + "\n")
        self.proc.stdin.flush()
        
        # Read response - skip non-JSON lines
        while True:
            response_str = self.proc.stdout.readline()
            if not response_str:
                self._log("No response!")
                return None
            response_str = response_str.strip()
            if response_str.startswith('{'):
                try:
                    response = json.loads(response_str)
                    return response
                except json.JSONDecodeError:
                    continue
    
    def initialize(self) -> dict:
        """Initialize MCP connection."""
        return self.send_request("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "interactivity_test", "version": "1.0"}
        })
    
    def call_tool(self, name: str, arguments: dict = None) -> Optional[dict]:
        """Call an MCP tool."""
        params = {"name": name}
        if arguments:
            params["arguments"] = arguments
        return self.send_request("tools/call", params)
    
    def ping(self) -> bool:
        """Check if server is responding."""
        response = self.call_tool("ping")
        if response and "result" in response:
            content = response["result"]["content"][0]
            return content.get("text") == "pong"
        return False
    
    def screenshot(self, save_path: str = None) -> Optional[bytes]:
        """Take a screenshot, optionally save to file."""
        response = self.call_tool("screenshot")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if content["type"] == "image":
                data = base64.b64decode(content["data"])
                if save_path:
                    Path(save_path).parent.mkdir(parents=True, exist_ok=True)
                    with open(save_path, "wb") as f:
                        f.write(data)
                return data
        return None
    
    def get_ui_tree(self) -> Optional[dict]:
        """Get the UI component tree."""
        response = self.call_tool("dump_ui_tree")
        if response and "result" in response:
            content = response["result"]["content"][0]
            if "text" in content:
                return json.loads(content["text"])
        return None
    
    def mouse_move(self, x: int, y: int) -> dict:
        """Move mouse to position."""
        return self.call_tool("mouse_move", {"x": x, "y": y})
    
    def mouse_click(self, x: int, y: int, button: str = "left") -> dict:
        """Click at position."""
        return self.call_tool("mouse_click", {"x": x, "y": y, "button": button})
    
    def mouse_drag(self, start: Tuple[int, int], end: Tuple[int, int], steps: int = 10):
        """Simulate mouse drag from start to end."""
        # Move to start
        self.mouse_move(start[0], start[1])
        time.sleep(0.05)
        
        # Press mouse button (simulate with key_down doesn't work for mouse, 
        # so we'll do multiple clicks along the path)
        dx = (end[0] - start[0]) / steps
        dy = (end[1] - start[1]) / steps
        
        for i in range(steps + 1):
            x = int(start[0] + dx * i)
            y = int(start[1] + dy * i)
            self.mouse_click(x, y)
            time.sleep(0.02)
    
    def key_press(self, key: str) -> dict:
        """Press a key."""
        return self.call_tool("key_press", {"key": key})
    
    def close(self):
        """Close the connection."""
        try:
            self.call_tool("exit")
            time.sleep(0.2)
        except:
            pass
        self.proc.terminate()
        try:
            self.proc.wait(timeout=5)
        except:
            self.proc.kill()


def parse_ui_tree(tree_data: dict) -> List[UIElement]:
    """Parse UI tree JSON into UIElement objects."""
    elements = []
    
    def parse_node(node: dict) -> UIElement:
        children = [parse_node(c) for c in node.get("children", [])]
        return UIElement(
            id=node.get("id", 0),
            name=node.get("name", ""),
            rect=node.get("rect", {"x": 0, "y": 0, "width": 0, "height": 0}),
            visible=node.get("visible", False),
            children=children
        )
    
    for root in tree_data.get("tree", []):
        elements.append(parse_node(root))
    
    return elements


def find_elements_by_pattern(elements: List[UIElement], pattern: str) -> List[UIElement]:
    """Find all elements whose name matches pattern (supports * wildcard)."""
    results = []
    
    def matches(name: str, pat: str) -> bool:
        if '*' in pat:
            prefix, suffix = pat.split('*', 1)
            return name.startswith(prefix) and name.endswith(suffix)
        return name == pat
    
    def search(el: UIElement):
        if el.name and matches(el.name, pattern):
            results.append(el)
        for child in el.children:
            search(child)
    
    for el in elements:
        search(el)
    
    return results


def find_element_by_name(elements: List[UIElement], name: str) -> Optional[UIElement]:
    """Find first element with exact name match."""
    matches = find_elements_by_pattern(elements, name)
    return matches[0] if matches else None


@dataclass
class InteractionTest:
    """Definition of an interaction test."""
    name: str
    element_pattern: str  # Pattern to find element(s) to interact with
    action: str  # "click", "drag_left", "drag_right", "drag_full"
    description: str
    verify: Optional[Callable] = None  # Optional verification function


@dataclass
class ScreenTestConfig:
    """Configuration for testing a specific screen."""
    screen_name: str
    interactions: List[InteractionTest]
    setup_delay: float = 0.5  # Delay after screen loads
    between_delay: float = 0.2  # Delay between interactions


# ============================================================================
# SCREEN TEST DEFINITIONS
# ============================================================================

SCREEN_TESTS: Dict[str, ScreenTestConfig] = {
    "sports_settings": ScreenTestConfig(
        screen_name="sports_settings",
        interactions=[
            InteractionTest(
                name="LB Tab Button",
                element_pattern="lb_btn",
                action="click",
                description="Click LB to cycle tabs left"
            ),
            InteractionTest(
                name="RB Tab Button", 
                element_pattern="rb_btn",
                action="click",
                description="Click RB to cycle tabs right"
            ),
            InteractionTest(
                name="Tab Click",
                element_pattern="tab_0",
                action="click",
                description="Click first tab directly"
            ),
            InteractionTest(
                name="Slider Drag",
                element_pattern="slider_*",
                action="drag_full",
                description="Drag any slider from left to right"
            ),
            InteractionTest(
                name="Value Left Arrow",
                element_pattern="value_left_0",
                action="click",
                description="Click < to decrease Window mode value"
            ),
            InteractionTest(
                name="Value Right Arrow",
                element_pattern="value_right_0",
                action="click",
                description="Click > to increase Window mode value"
            ),
            InteractionTest(
                name="Max FPS Left Arrow",
                element_pattern="value_left_4",
                action="click",
                description="Click < to decrease Max FPS"
            ),
            InteractionTest(
                name="Max FPS Right Arrow",
                element_pattern="value_right_4",
                action="click",
                description="Click > to increase Max FPS"
            ),
        ]
    ),
    
    "parcel_corps_settings": ScreenTestConfig(
        screen_name="parcel_corps_settings",
        interactions=[
            InteractionTest(
                name="Subtitles Toggle",
                element_pattern="toggle_row_110",
                action="click",
                description="Toggle subtitles on/off"
            ),
            InteractionTest(
                name="Full Screen Toggle",
                element_pattern="toggle_row_130",
                action="click", 
                description="Toggle fullscreen on/off"
            ),
            InteractionTest(
                name="Motion Blur Toggle",
                element_pattern="toggle_row_160",
                action="click",
                description="Toggle motion blur on/off"
            ),
            InteractionTest(
                name="VSync Toggle",
                element_pattern="toggle_row_170",
                action="click",
                description="Toggle VSync on/off"
            ),
            InteractionTest(
                name="Audio Volume Slider",
                element_pattern="volume_slider_200",
                action="drag_full",
                description="Drag audio volume slider"
            ),
            InteractionTest(
                name="Music Volume Slider",
                element_pattern="volume_slider_210",
                action="drag_full",
                description="Drag music volume slider"
            ),
        ]
    ),
    
    "angry_birds_settings": ScreenTestConfig(
        screen_name="angry_birds_settings",
        interactions=[
            InteractionTest(
                name="Music Toggle",
                element_pattern="toggle_0",
                action="click",
                description="Toggle music on/off"
            ),
            InteractionTest(
                name="Sound Toggle",
                element_pattern="toggle_1",
                action="click",
                description="Toggle sound on/off"
            ),
            InteractionTest(
                name="Vibration Toggle",
                element_pattern="toggle_2",
                action="click",
                description="Toggle vibration on/off"
            ),
            InteractionTest(
                name="Notifications Button",
                element_pattern="notifications_btn",
                action="click",
                description="Toggle notifications on/off"
            ),
        ]
    ),
    
    "empire_tycoon": ScreenTestConfig(
        screen_name="empire_tycoon",
        interactions=[
            InteractionTest(
                name="Tab 0",
                element_pattern="tab_0",
                action="click",
                description="Select first navigation tab"
            ),
            InteractionTest(
                name="Tab 1",
                element_pattern="tab_1",
                action="click",
                description="Select second navigation tab"
            ),
            InteractionTest(
                name="Tab 2",
                element_pattern="tab_2",
                action="click",
                description="Select third navigation tab"
            ),
        ]
    ),
    
    "islands_trains_settings": ScreenTestConfig(
        screen_name="islands_trains_settings",
        interactions=[
            InteractionTest(
                name="Mode Left Arrow",
                element_pattern="chevron_l_110",
                action="click",
                description="Cycle display mode left"
            ),
            InteractionTest(
                name="Mode Right Arrow",
                element_pattern="chevron_r_110",
                action="click",
                description="Cycle display mode right"
            ),
            InteractionTest(
                name="Cam Pan Left",
                element_pattern="chevron_l_140",
                action="click",
                description="Decrease cam panning speed"
            ),
            InteractionTest(
                name="Cam Pan Right",
                element_pattern="chevron_r_140",
                action="click",
                description="Increase cam panning speed"
            ),
        ]
    ),
    
    "dead_space_settings": ScreenTestConfig(
        screen_name="dead_space_settings",
        interactions=[
            InteractionTest(
                name="Initial Menu Item",
                element_pattern="initial_0",
                action="click",
                description="Select first menu item"
            ),
            InteractionTest(
                name="Settings Menu Item",
                element_pattern="setting_0",
                action="click",
                description="Select Controls setting"
            ),
        ]
    ),
    
    "flight_options": ScreenTestConfig(
        screen_name="flight_options",
        interactions=[
            InteractionTest(
                name="Category Selection",
                element_pattern="tab_*",
                action="click",
                description="Select a category tab"
            ),
        ]
    ),
    
    "cozy_cafe": ScreenTestConfig(
        screen_name="cozy_cafe",
        interactions=[
            InteractionTest(
                name="Menu Item",
                element_pattern="*_110",
                action="click",
                description="Select first daily special"
            ),
            InteractionTest(
                name="Inventory Button",
                element_pattern="inventory_btn",
                action="click",
                description="Click inventory button"
            ),
        ]
    ),
    
    "fighter_menu": ScreenTestConfig(
        screen_name="fighter_menu",
        interactions=[
            InteractionTest(
                name="Tab Selection",
                element_pattern="tab_*",
                action="click",
                description="Select a tab"
            ),
            InteractionTest(
                name="Menu Item Selection",
                element_pattern="menu_*",
                action="click",
                description="Select a menu item"
            ),
        ]
    ),
}


def run_interaction_test(
    client: MCPClient,
    test: InteractionTest,
    elements: List[UIElement],
    save_screenshots: bool,
    screen_name: str
) -> Tuple[bool, str]:
    """
    Run a single interaction test.
    Returns (success, message).
    """
    # Find target element(s)
    targets = find_elements_by_pattern(elements, test.element_pattern)
    
    if not targets:
        return False, f"Element '{test.element_pattern}' not found in UI tree"
    
    target = targets[0]  # Use first match
    
    if not target.visible:
        return False, f"Element '{target.name}' exists but is not visible"
    
    # Take before screenshot if requested
    if save_screenshots:
        client.screenshot(str(SCREENSHOT_DIR / f"{screen_name}_{test.name}_before.png"))
    
    # Perform action
    if test.action == "click":
        cx, cy = target.center
        client.mouse_move(cx, cy)
        time.sleep(0.05)
        client.mouse_click(cx, cy)
        
    elif test.action == "drag_left":
        client.mouse_drag(target.right_edge, target.left_edge)
        
    elif test.action == "drag_right":
        client.mouse_drag(target.left_edge, target.right_edge)
        
    elif test.action == "drag_full":
        # Drag right then back left
        client.mouse_drag(target.left_edge, target.right_edge)
        time.sleep(0.1)
        client.mouse_drag(target.right_edge, target.center)
        
    else:
        return False, f"Unknown action: {test.action}"
    
    time.sleep(0.1)
    
    # Take after screenshot if requested
    if save_screenshots:
        client.screenshot(str(SCREENSHOT_DIR / f"{screen_name}_{test.name}_after.png"))
    
    # Run verification if provided
    if test.verify:
        # Get updated UI tree for verification
        new_tree = client.get_ui_tree()
        if new_tree:
            new_elements = parse_ui_tree(new_tree)
            if not test.verify(elements, new_elements):
                return False, "Verification failed"
    
    return True, f"Interacted with '{target.name}' at ({target.center[0]}, {target.center[1]})"


def run_screen_tests(
    config: ScreenTestConfig,
    verbose: bool = False,
    save_screenshots: bool = False
) -> Tuple[int, int]:
    """
    Run all interaction tests for a screen.
    Returns (passed_count, total_count).
    """
    print(f"\n{'='*60}")
    print(f"Testing: {config.screen_name}")
    print(f"{'='*60}")
    
    # Start app with this screen
    client = MCPClient(
        EXECUTABLE,
        ["--mcp", f"--screen={config.screen_name}"],
        verbose=verbose
    )
    
    passed = 0
    total = len(config.interactions)
    
    try:
        # Initialize
        client.initialize()
        time.sleep(config.setup_delay)
        
        if not client.ping():
            print("  ERROR: Server not responding")
            return 0, total
        
        # Get initial UI tree
        tree = client.get_ui_tree()
        if not tree:
            print("  ERROR: Could not get UI tree")
            return 0, total
        
        elements = parse_ui_tree(tree)
        
        if verbose:
            # Print element summary
            all_elements = []
            def collect(el):
                if el.name:
                    all_elements.append(el.name)
                for c in el.children:
                    collect(c)
            for e in elements:
                collect(e)
            print(f"  Found {len(all_elements)} named elements")
        
        # Run each test
        for test in config.interactions:
            success, message = run_interaction_test(
                client, test, elements, save_screenshots, config.screen_name
            )
            
            status = "PASS" if success else "FAIL"
            print(f"  [{status}] {test.name}: {test.description}")
            if verbose or not success:
                print(f"         {message}")
            
            if success:
                passed += 1
            
            time.sleep(config.between_delay)
            
            # Refresh UI tree after each interaction
            tree = client.get_ui_tree()
            if tree:
                elements = parse_ui_tree(tree)
    
    except Exception as e:
        print(f"  ERROR: {e}")
        import traceback
        if verbose:
            traceback.print_exc()
    
    finally:
        client.close()
    
    return passed, total


def list_screens():
    """List all available screen tests."""
    print("\nAvailable screen tests:")
    print("-" * 40)
    for name, config in SCREEN_TESTS.items():
        print(f"  {name}")
        for test in config.interactions:
            print(f"    - {test.name}: {test.description}")
    print()


def main():
    parser = argparse.ArgumentParser(
        description="Generic interactivity testing for UI screens via MCP"
    )
    parser.add_argument(
        "--screen", 
        type=str, 
        help="Test specific screen only"
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="List available screen tests"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Show detailed output"
    )
    parser.add_argument(
        "--save-screenshots",
        action="store_true",
        help=f"Save before/after screenshots to {SCREENSHOT_DIR}"
    )
    args = parser.parse_args()
    
    if args.list:
        list_screens()
        return
    
    # Determine which screens to test
    if args.screen:
        if args.screen not in SCREEN_TESTS:
            print(f"Unknown screen: {args.screen}")
            print("Use --list to see available screens")
            sys.exit(1)
        screens = [args.screen]
    else:
        screens = list(SCREEN_TESTS.keys())
    
    # Create screenshot directory if needed
    if args.save_screenshots:
        SCREENSHOT_DIR.mkdir(parents=True, exist_ok=True)
    
    # Run tests
    total_passed = 0
    total_tests = 0
    
    for screen in screens:
        config = SCREEN_TESTS[screen]
        passed, total = run_screen_tests(
            config,
            verbose=args.verbose,
            save_screenshots=args.save_screenshots
        )
        total_passed += passed
        total_tests += total
    
    # Summary
    print("\n" + "=" * 60)
    print(f"SUMMARY: {total_passed}/{total_tests} tests passed")
    print("=" * 60)
    
    if args.save_screenshots:
        print(f"Screenshots saved to: {SCREENSHOT_DIR}")
    
    if total_passed < total_tests:
        sys.exit(1)


if __name__ == "__main__":
    main()

