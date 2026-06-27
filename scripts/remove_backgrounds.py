#!/usr/bin/env python3
"""
Script to remove backgrounds from icon PNGs and make them transparent.
Uses PIL/Pillow to detect the background color (from corner pixels) and make it transparent.
"""

import sys
sys.path.insert(0, '/Users/gabeochoa/Library/Python/3.9/lib/python/site-packages')

from PIL import Image
import os

def remove_background(input_path, output_path=None, tolerance=30):
    """Remove the background color from an image and make it transparent."""
    if output_path is None:
        output_path = input_path
    
    img = Image.open(input_path).convert("RGBA")
    data = img.getdata()
    
    # Get background color from top-left corner
    bg_color = data[0][:3]  # RGB only
    
    new_data = []
    for item in data:
        # Check if this pixel is close to the background color
        r, g, b, a = item
        if (abs(r - bg_color[0]) < tolerance and 
            abs(g - bg_color[1]) < tolerance and 
            abs(b - bg_color[2]) < tolerance):
            # Make it transparent
            new_data.append((r, g, b, 0))
        else:
            new_data.append(item)
    
    img.putdata(new_data)
    img.save(output_path, "PNG")
    print(f"Processed: {input_path}")

def main():
    images_dir = "/Users/gabeochoa/p/wm_afterhours/resources/images"
    
    # List of icons to process
    icons_to_process = [
        # CozyCafe icons
        "icon_inventory.png",
        "icon_research.png", 
        "icon_crafting.png",
        "flower_blossom.png",
        "avatar_guildmate.png",
        "avatar_devteam.png",
        # EmpireTycoon icons
        "icon_rides.png",
        "icon_food.png",
        "icon_upgrades.png",
        "icon_finance.png",
        "icon_shop.png",
        "icon_settings.png",
        "icon_happiness.png",
        "icon_resources.png",
        "icon_coin_small.png",
        "icon_star_trophy.png",
        "mascot_business.png",
        "cloud_white.png",
        # NeonStrike icons
        "icon_uav.png",
        "icon_recon.png",
        "icon_shield_tactical.png",
        "icon_strike.png",
        "icon_danger.png",
        "icon_skull.png",
        "icon_grenade.png",
        "icon_melee.png",
        "crosshair_neon.png",
    ]
    
    for icon in icons_to_process:
        path = os.path.join(images_dir, icon)
        if os.path.exists(path):
            try:
                remove_background(path, tolerance=40)
            except Exception as e:
                print(f"Error processing {icon}: {e}")
        else:
            print(f"Not found: {icon}")

if __name__ == "__main__":
    main()

