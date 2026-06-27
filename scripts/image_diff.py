#!/usr/bin/env python3
"""
Image diffing script to compare inspiration images with our current screenshots.
Generates side-by-side comparisons and difference maps.
"""

import sys
sys.path.insert(0, '/Users/gabeochoa/Library/Python/3.9/lib/python/site-packages')

from PIL import Image, ImageChops, ImageDraw, ImageFont
import os

def resize_to_match(img1, img2):
    """Resize img2 to match img1's dimensions."""
    return img2.resize(img1.size, Image.Resampling.LANCZOS)

def create_diff_image(img1, img2):
    """Create a difference image highlighting changes."""
    diff = ImageChops.difference(img1, img2)
    # Amplify differences for visibility
    diff = diff.point(lambda x: min(255, x * 3))
    return diff

def create_comparison(inspiration_path, current_path, output_path, name):
    """Create a side-by-side comparison with diff."""
    try:
        inspiration = Image.open(inspiration_path).convert("RGB")
        current = Image.open(current_path).convert("RGB")
    except Exception as e:
        print(f"Error loading images for {name}: {e}")
        return
    
    # Resize current to match inspiration
    current_resized = resize_to_match(inspiration, current)
    
    # Create diff
    diff = create_diff_image(inspiration, current_resized)
    
    # Calculate dimensions for output
    w, h = inspiration.size
    margin = 20
    label_height = 40
    total_width = w * 3 + margin * 4
    total_height = h + margin * 2 + label_height
    
    # Create output image
    output = Image.new("RGB", (total_width, total_height), (40, 40, 40))
    draw = ImageDraw.Draw(output)
    
    # Paste images
    output.paste(inspiration, (margin, margin + label_height))
    output.paste(current_resized, (w + margin * 2, margin + label_height))
    output.paste(diff, (w * 2 + margin * 3, margin + label_height))
    
    # Add labels
    try:
        font = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 24)
    except:
        font = ImageFont.load_default()
    
    draw.text((margin + w//2 - 80, margin//2), "INSPIRATION", fill=(255, 255, 255), font=font)
    draw.text((w + margin * 2 + w//2 - 50, margin//2), "CURRENT", fill=(255, 255, 255), font=font)
    draw.text((w * 2 + margin * 3 + w//2 - 80, margin//2), "DIFFERENCE", fill=(255, 100, 100), font=font)
    
    # Add title
    draw.text((total_width//2 - 100, 5), name.upper(), fill=(255, 200, 100), font=font)
    
    output.save(output_path, "PNG")
    print(f"Created: {output_path}")
    
    # Calculate difference percentage
    diff_pixels = list(diff.getdata())
    total_diff = sum(sum(p) for p in diff_pixels)
    max_diff = len(diff_pixels) * 255 * 3
    similarity = 100 - (total_diff / max_diff * 100)
    print(f"  Similarity: {similarity:.1f}%")
    return similarity

def main():
    base_path = "/Users/gabeochoa/p/wm_afterhours"
    inspiration_dir = os.path.join(base_path, "inspiration")
    output_dir = os.path.join(base_path, "output")
    diff_dir = os.path.join(base_path, "output", "diffs")
    
    os.makedirs(diff_dir, exist_ok=True)
    
    comparisons = [
        # Main game screens (high priority)
        ("example_cozy_game.png", "cozy_cafe.png", "Cozy Cafe"),
        ("example_tycoon_game.png", "empire_tycoon.png", "Empire Tycoon"),
        ("example_shooter_game.png", "neon_strike.png", "Neon Strike"),
        # Settings screens
        ("example_ace_combat.jpg", "flight_options.png", "Ace Combat"),
        ("example_angry_birds.jpg", "angry_birds_settings.png", "Angry Birds"),
        ("example_cross_tag.jpg", "fighter_menu.png", "Cross Tag"),
        ("example_deadspace.jpg", "deadspace_settings.png", "Dead Space"),
        ("example_islands_trains.jpg", "islands_trains_settings.png", "Islands Trains"),
        ("example_kirby_airriders.jpg", "kirby_options.png", "Kirby Air Riders"),
        ("example_mini_motorways.jpg", "mini_motorways_settings.png", "Mini Motorways"),
        ("example_parcel_coro.jpg", "parcel_corps_settings.png", "Parcel Corps"),
        ("example_powerwash.jpg", "powerwash_settings.png", "PowerWash Sim"),
        ("example_rematch.jpg", "sports_settings.png", "Rematch Sports"),
        ("example_rubber_bandits.jpg", "rubber_bandits_menu.png", "Rubber Bandits"),
    ]
    
    results = []
    for insp_file, curr_file, name in comparisons:
        insp_path = os.path.join(inspiration_dir, insp_file)
        curr_path = os.path.join(output_dir, curr_file)
        diff_path = os.path.join(diff_dir, f"diff_{curr_file}")
        
        if os.path.exists(insp_path) and os.path.exists(curr_path):
            similarity = create_comparison(insp_path, curr_path, diff_path, name)
            if similarity:
                results.append((name, similarity))
        else:
            print(f"Missing files for {name}:")
            if not os.path.exists(insp_path):
                print(f"  Missing: {insp_path}")
            if not os.path.exists(curr_path):
                print(f"  Missing: {curr_path}")
    
    if results:
        print("\n" + "="*50)
        print("SIMILARITY SUMMARY")
        print("="*50)
        for name, sim in results:
            bar_len = int(sim / 2)
            bar = "█" * bar_len + "░" * (50 - bar_len)
            print(f"{name:15} [{bar}] {sim:.1f}%")

if __name__ == "__main__":
    main()

