#!/usr/bin/env python3
"""
Compare current screenshots against baseline to detect regressions.
"""

import sys
sys.path.insert(0, '/Users/gabeochoa/Library/Python/3.9/lib/python/site-packages')

from PIL import Image, ImageChops
import os

def images_are_identical(img1_path, img2_path):
    """Check if two images are pixel-identical."""
    try:
        img1 = Image.open(img1_path).convert("RGB")
        img2 = Image.open(img2_path).convert("RGB")
        
        if img1.size != img2.size:
            return False, "Size mismatch"
        
        diff = ImageChops.difference(img1, img2)
        if diff.getbbox() is None:
            return True, "Identical"
        
        # Calculate difference percentage
        diff_pixels = list(diff.getdata())
        total_diff = sum(sum(p) for p in diff_pixels)
        max_diff = len(diff_pixels) * 255 * 3
        diff_pct = (total_diff / max_diff * 100)
        return False, f"Differs by {diff_pct:.4f}%"
    except Exception as e:
        return False, f"Error: {e}"

def main():
    baseline_dir = "/Users/gabeochoa/p/wm_afterhours/baseline_screenshots"
    current_dir = "/tmp/ui_showcase_screenshots"
    
    if not os.path.exists(baseline_dir):
        print(f"ERROR: Baseline directory not found: {baseline_dir}")
        return 1
    
    if not os.path.exists(current_dir):
        print(f"ERROR: Current screenshots directory not found: {current_dir}")
        print("Run screenshot_all_screens.py first")
        return 1
    
    baseline_files = sorted([f for f in os.listdir(baseline_dir) if f.endswith('.png')])
    
    if not baseline_files:
        print("ERROR: No baseline screenshots found")
        return 1
    
    print(f"Comparing {len(baseline_files)} screenshots...")
    print("=" * 60)
    
    identical_count = 0
    different_count = 0
    missing_count = 0
    
    for filename in baseline_files:
        baseline_path = os.path.join(baseline_dir, filename)
        current_path = os.path.join(current_dir, filename)
        
        if not os.path.exists(current_path):
            print(f"MISSING: {filename}")
            missing_count += 1
            continue
        
        is_identical, message = images_are_identical(baseline_path, current_path)
        
        if is_identical:
            print(f"✓ {filename}")
            identical_count += 1
        else:
            print(f"✗ {filename} - {message}")
            different_count += 1
    
    print("=" * 60)
    print(f"Results: {identical_count} identical, {different_count} different, {missing_count} missing")
    
    if different_count == 0 and missing_count == 0:
        print("\n✅ All screenshots match baseline - no regressions!")
        return 0
    else:
        print(f"\n❌ {different_count + missing_count} regressions detected")
        return 1

if __name__ == "__main__":
    sys.exit(main())

