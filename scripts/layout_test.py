#!/usr/bin/env python3
"""
Layout regression testing for UI screens.

Takes screenshots of all registered screens and compares against baselines.
Detects layout regressions after code changes.

Usage:
    python scripts/layout_test.py --baseline     # Generate baseline screenshots
    python scripts/layout_test.py                # Compare against baselines
    python scripts/layout_test.py --screen=X    # Test specific screen only
    python scripts/layout_test.py --threshold=5 # Set diff threshold (default: 1%)

Exit codes:
    0 - All tests passed
    1 - Some tests failed
    2 - Missing baselines (run with --baseline first)
"""

import argparse
import base64
import json
import os
import subprocess
import sys
import time
from pathlib import Path

# Try to import PIL for proper image comparison
try:
    from PIL import Image, ImageChops
    HAS_PIL = True
except ImportError:
    HAS_PIL = False
    print("Warning: PIL not found. Using byte-by-byte comparison (less accurate).")

BASELINE_DIR = Path("tests/baselines/layout")
CURRENT_DIR = Path("tests/current/layout")
DIFF_DIR = Path("tests/diffs/layout")
EXECUTABLE = "./output/ui_tester.exe"
TIMEOUT = 120
DEFAULT_THRESHOLD = 1.0  # 1% difference allowed


def send_request(proc, req_id, method, params=None):
    """Send a JSON-RPC request to the MCP server."""
    request = {"jsonrpc": "2.0", "id": req_id, "method": method}
    if params:
        request["params"] = params
    proc.stdin.write(json.dumps(request) + "\n")
    proc.stdin.flush()
    
    start = time.time()
    while time.time() - start < TIMEOUT:
        line = proc.stdout.readline()
        if not line:
            time.sleep(0.1)
            continue
        line = line.strip()
        if line.startswith("{"):
            try:
                return json.loads(line)
            except json.JSONDecodeError:
                continue
    return None


def take_screenshot(proc, req_id):
    """Take a screenshot and return the image data."""
    resp = send_request(proc, req_id, "tools/call", {"name": "screenshot"})
    if resp and "result" in resp:
        content = resp["result"].get("content", [])
        if content and content[0].get("type") == "image":
            return base64.b64decode(content[0]["data"])
    return None


def list_screens(proc, req_id):
    """Get list of available screens."""
    resp = send_request(proc, req_id, "tools/call", {"name": "list_screens"})
    if resp and "result" in resp:
        content = resp["result"].get("content", [])
        if content and content[0].get("type") == "text":
            # Parse screen list from text
            text = content[0]["text"]
            screens = []
            for line in text.split("\n"):
                line = line.strip()
                if line and not line.startswith("Available") and not line.startswith("---"):
                    # Handle format like "1. ScreenName" or just "ScreenName"
                    if ". " in line:
                        screens.append(line.split(". ", 1)[1])
                    else:
                        screens.append(line)
            return screens
    return []


def goto_screen(proc, req_id, screen_name):
    """Navigate to a specific screen."""
    resp = send_request(proc, req_id, "tools/call", 
                        {"name": "goto_screen", "arguments": {"screen": screen_name}})
    return resp is not None


def compare_images_pil(img1_data, img2_data):
    """Compare two images using PIL. Returns difference percentage."""
    import io
    img1 = Image.open(io.BytesIO(img1_data)).convert("RGB")
    img2 = Image.open(io.BytesIO(img2_data)).convert("RGB")
    
    # Resize if needed
    if img1.size != img2.size:
        img2 = img2.resize(img1.size, Image.Resampling.LANCZOS)
    
    diff = ImageChops.difference(img1, img2)
    diff_pixels = list(diff.getdata())
    total_diff = sum(sum(p) for p in diff_pixels)
    max_diff = len(diff_pixels) * 255 * 3
    
    return (total_diff / max_diff) * 100


def compare_images_bytes(img1_data, img2_data):
    """Compare two images byte-by-byte. Returns difference percentage."""
    if len(img1_data) != len(img2_data):
        return 100.0  # Completely different if sizes differ
    
    diff_bytes = sum(1 for a, b in zip(img1_data, img2_data) if a != b)
    return (diff_bytes / len(img1_data)) * 100


def compare_images(img1_data, img2_data):
    """Compare two images. Returns difference percentage."""
    if HAS_PIL:
        return compare_images_pil(img1_data, img2_data)
    return compare_images_bytes(img1_data, img2_data)


def create_diff_image(baseline_data, current_data, output_path):
    """Create a visual diff image if PIL is available."""
    if not HAS_PIL:
        return
    
    import io
    baseline = Image.open(io.BytesIO(baseline_data)).convert("RGB")
    current = Image.open(io.BytesIO(current_data)).convert("RGB")
    
    if baseline.size != current.size:
        current = current.resize(baseline.size, Image.Resampling.LANCZOS)
    
    diff = ImageChops.difference(baseline, current)
    # Amplify differences
    diff = diff.point(lambda x: min(255, x * 5))
    diff.save(output_path, "PNG")


def run_layout_tests(baseline_mode=False, screen_filter=None, threshold=DEFAULT_THRESHOLD):
    """Run layout tests for all screens."""
    print(f"\n{'Generating baselines' if baseline_mode else 'Running layout tests'}...")
    print("=" * 60)
    
    # Ensure directories exist
    BASELINE_DIR.mkdir(parents=True, exist_ok=True)
    CURRENT_DIR.mkdir(parents=True, exist_ok=True)
    DIFF_DIR.mkdir(parents=True, exist_ok=True)
    
    # Start the app in MCP mode
    proc = subprocess.Popen(
        [EXECUTABLE, "--mcp"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
        bufsize=1,
    )
    
    results = {"passed": [], "failed": [], "missing": [], "error": []}
    req_id = 1
    
    try:
        time.sleep(2.0)
        
        # Initialize MCP
        send_request(proc, req_id, "initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "layout_test", "version": "1.0"},
        })
        req_id += 1
        time.sleep(0.5)
        
        # Get list of screens
        screens = list_screens(proc, req_id)
        req_id += 1
        
        if not screens:
            print("ERROR: Could not get screen list")
            return False
        
        # Filter screens if specified
        if screen_filter:
            screens = [s for s in screens if screen_filter.lower() in s.lower()]
        
        print(f"Testing {len(screens)} screens...")
        print("-" * 60)
        
        for screen_name in screens:
            # Navigate to screen
            if not goto_screen(proc, req_id, screen_name):
                print(f"  ERROR: {screen_name} - Failed to navigate")
                results["error"].append(screen_name)
                req_id += 1
                continue
            req_id += 1
            time.sleep(0.5)  # Wait for screen to render
            
            # Take screenshot
            screenshot_data = take_screenshot(proc, req_id)
            req_id += 1
            
            if not screenshot_data:
                print(f"  ERROR: {screen_name} - Failed to capture screenshot")
                results["error"].append(screen_name)
                continue
            
            # Sanitize screen name for filename
            safe_name = screen_name.replace(" ", "_").replace("/", "_")
            baseline_path = BASELINE_DIR / f"{safe_name}.png"
            current_path = CURRENT_DIR / f"{safe_name}.png"
            diff_path = DIFF_DIR / f"{safe_name}_diff.png"
            
            if baseline_mode:
                # Save as baseline
                with open(baseline_path, "wb") as f:
                    f.write(screenshot_data)
                print(f"  BASELINE: {screen_name}")
                results["passed"].append(screen_name)
            else:
                # Save current screenshot
                with open(current_path, "wb") as f:
                    f.write(screenshot_data)
                
                # Compare against baseline
                if not baseline_path.exists():
                    print(f"  MISSING:  {screen_name} - No baseline (run with --baseline)")
                    results["missing"].append(screen_name)
                    continue
                
                with open(baseline_path, "rb") as f:
                    baseline_data = f.read()
                
                diff_pct = compare_images(baseline_data, screenshot_data)
                
                if diff_pct <= threshold:
                    print(f"  PASS:     {screen_name} ({diff_pct:.2f}% diff)")
                    results["passed"].append(screen_name)
                    # Clean up passing diff files
                    if diff_path.exists():
                        diff_path.unlink()
                else:
                    print(f"  FAIL:     {screen_name} ({diff_pct:.2f}% diff > {threshold}%)")
                    results["failed"].append(screen_name)
                    # Create diff image
                    create_diff_image(baseline_data, screenshot_data, diff_path)
        
        return len(results["failed"]) == 0 and len(results["missing"]) == 0
    
    finally:
        # Clean exit
        send_request(proc, req_id, "tools/call", {"name": "exit"})
        time.sleep(0.3)
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
        
        # Print summary
        print("\n" + "=" * 60)
        print("SUMMARY")
        print("=" * 60)
        print(f"  Passed:  {len(results['passed'])}")
        print(f"  Failed:  {len(results['failed'])}")
        print(f"  Missing: {len(results['missing'])}")
        print(f"  Errors:  {len(results['error'])}")
        
        if results["failed"]:
            print(f"\nFailed screens:")
            for s in results["failed"]:
                print(f"  - {s}")
            print(f"\nDiff images saved to: {DIFF_DIR}")
        
        if results["missing"]:
            print(f"\nMissing baselines (run with --baseline):")
            for s in results["missing"]:
                print(f"  - {s}")


def main():
    parser = argparse.ArgumentParser(description="Layout regression testing for UI screens")
    parser.add_argument("--baseline", action="store_true", 
                        help="Generate baseline screenshots instead of comparing")
    parser.add_argument("--screen", type=str, 
                        help="Test specific screen only (partial match)")
    parser.add_argument("--threshold", type=float, default=DEFAULT_THRESHOLD,
                        help=f"Difference threshold percentage (default: {DEFAULT_THRESHOLD})")
    args = parser.parse_args()
    
    success = run_layout_tests(
        baseline_mode=args.baseline,
        screen_filter=args.screen,
        threshold=args.threshold
    )
    
    if not args.baseline:
        sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()

