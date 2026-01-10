#!/usr/bin/env python3
"""
Snapshot diff testing for UI screens.

Usage:
    python scripts/snapshot_test.py              # Compare against baselines
    python scripts/snapshot_test.py --update     # Update baselines with current screenshots
    python scripts/snapshot_test.py --screen=X   # Test specific screen only

Naming convention: <screen_name>_<language>_<specific_check>.png
"""

import argparse
import json
import os
import subprocess
import sys
import time
import base64
from pathlib import Path

SNAPSHOT_DIR = Path("test_snapshots")
EXECUTABLE = "./output/ui_tester.exe"
TIMEOUT = 60


def send_request(proc, req_id, method, params=None):
    """Send a JSON-RPC request to the MCP server."""
    request = {"jsonrpc": "2.0", "id": req_id, "method": method}
    if params:
        request["params"] = params
    proc.stdin.write(json.dumps(request) + "\n")
    proc.stdin.flush()
    while True:
        line = proc.stdout.readline()
        if not line:
            return None
        line = line.strip()
        if line.startswith("{"):
            try:
                return json.loads(line)
            except json.JSONDecodeError:
                continue


def take_screenshot(proc, req_id):
    """Take a screenshot and return the image data."""
    resp = send_request(proc, req_id, "tools/call", {"name": "screenshot"})
    if resp and "result" in resp:
        content = resp["result"]["content"][0]
        if content["type"] == "image":
            return base64.b64decode(content["data"])
    return None


def compare_images(img1_data, img2_data):
    """Compare two images byte-by-byte. Returns True if identical."""
    return img1_data == img2_data


def get_language_demo_snapshots(proc):
    """Generate snapshots for the language_demo screen."""
    snapshots = {}
    req_id = 10

    # English
    send_request(proc, req_id, "tools/call", {"name": "key_press", "arguments": {"key": "1"}})
    time.sleep(0.3)
    req_id += 1
    snapshots["language_demo_english_layout.png"] = take_screenshot(proc, req_id)
    req_id += 1

    # Korean
    send_request(proc, req_id, "tools/call", {"name": "key_press", "arguments": {"key": "2"}})
    time.sleep(0.3)
    req_id += 1
    snapshots["language_demo_korean_layout.png"] = take_screenshot(proc, req_id)
    req_id += 1

    # Japanese
    send_request(proc, req_id, "tools/call", {"name": "key_press", "arguments": {"key": "3"}})
    time.sleep(0.3)
    req_id += 1
    snapshots["language_demo_japanese_layout.png"] = take_screenshot(proc, req_id)
    req_id += 1

    # Round-trip back to English
    send_request(proc, req_id, "tools/call", {"name": "key_press", "arguments": {"key": "1"}})
    time.sleep(0.3)
    req_id += 1
    snapshots["language_demo_english_roundtrip.png"] = take_screenshot(proc, req_id)

    return snapshots


SCREEN_SNAPSHOT_GENERATORS = {
    "language_demo": get_language_demo_snapshots,
}


def run_snapshot_test(screen_name, update_mode=False):
    """Run snapshot tests for a specific screen."""
    if screen_name not in SCREEN_SNAPSHOT_GENERATORS:
        print(f"Unknown screen: {screen_name}")
        return False

    print(f"\n{'Updating' if update_mode else 'Testing'} snapshots for: {screen_name}")
    print("=" * 50)

    # Start the app
    proc = subprocess.Popen(
        [EXECUTABLE, "--mcp", f"--screen={screen_name}"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
        bufsize=1,
    )

    try:
        time.sleep(2.0)

        # Initialize MCP
        send_request(
            proc,
            1,
            "initialize",
            {
                "protocolVersion": "2024-11-05",
                "capabilities": {},
                "clientInfo": {"name": "snapshot_test", "version": "1.0"},
            },
        )
        time.sleep(0.5)

        # Generate snapshots
        generator = SCREEN_SNAPSHOT_GENERATORS[screen_name]
        current_snapshots = generator(proc)

        # Compare or update
        all_passed = True
        for filename, current_data in current_snapshots.items():
            if current_data is None:
                print(f"  FAIL: {filename} - Could not capture screenshot")
                all_passed = False
                continue

            baseline_path = SNAPSHOT_DIR / filename

            if update_mode:
                # Update baseline
                with open(baseline_path, "wb") as f:
                    f.write(current_data)
                print(f"  UPDATED: {filename}")
            else:
                # Compare against baseline
                if not baseline_path.exists():
                    print(f"  MISSING: {filename} - No baseline exists (run with --update)")
                    all_passed = False
                    continue

                with open(baseline_path, "rb") as f:
                    baseline_data = f.read()

                if compare_images(current_data, baseline_data):
                    print(f"  PASS: {filename}")
                else:
                    print(f"  FAIL: {filename} - Screenshot differs from baseline")
                    # Save the diff for inspection
                    diff_path = SNAPSHOT_DIR / f"{filename.replace('.png', '_DIFF.png')}"
                    with open(diff_path, "wb") as f:
                        f.write(current_data)
                    print(f"         Saved current screenshot to: {diff_path}")
                    all_passed = False

        return all_passed

    finally:
        # Clean exit
        send_request(proc, 999, "tools/call", {"name": "exit"})
        time.sleep(0.3)
        proc.terminate()
        proc.wait(timeout=5)


def main():
    parser = argparse.ArgumentParser(description="Snapshot diff testing for UI screens")
    parser.add_argument("--update", action="store_true", help="Update baselines instead of comparing")
    parser.add_argument("--screen", type=str, help="Test specific screen only")
    args = parser.parse_args()

    # Ensure snapshot directory exists
    SNAPSHOT_DIR.mkdir(exist_ok=True)

    # Determine which screens to test
    if args.screen:
        screens = [args.screen]
    else:
        screens = list(SCREEN_SNAPSHOT_GENERATORS.keys())

    # Run tests
    all_passed = True
    for screen in screens:
        if not run_snapshot_test(screen, update_mode=args.update):
            all_passed = False

    # Summary
    print("\n" + "=" * 50)
    if args.update:
        print("Baselines updated successfully!")
    elif all_passed:
        print("All snapshot tests PASSED!")
    else:
        print("Some snapshot tests FAILED!")
        sys.exit(1)


if __name__ == "__main__":
    main()

