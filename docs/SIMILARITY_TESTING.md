# UI Similarity Testing Guide

This document explains how to compare our UI screens against inspiration images to measure visual accuracy.

## Overview

We use image diffing to calculate how closely our rendered UI screens match the inspiration mockups. The goal is to prove the UI library can replicate complex game UI designs.

## Current Scores

| Screen | Inspiration File | Screen File | Similarity |
|--------|-----------------|-------------|------------|
| Neon Strike | `example_shooter_game.png` | `NeonStrike.h` | **91.9%** ✅ |
| Ace Combat | `example_ace_combat.jpg` | `FlightOptions.h` | **79.8%** ✅ |
| Mini Motorways | `example_mini_motorways.jpg` | `MiniMotorwaysSettings.h` | **77.7%** ✅ |
| Dead Space | `example_deadspace.jpg` | `DeadSpaceSettings.h` | 69.6% |
| Kirby Air Riders | `example_kirby_airriders.jpg` | `KirbyOptions.h` | 60.2% |
| Rubber Bandits | `example_rubber_bandits.jpg` | `RubberBanditsMenu.h` | 56.4% |
| Cozy Cafe | `example_cozy_game.png` | `CozyCafe.h` | 55.5% |
| PowerWash Sim | `example_powerwash.jpg` | `PowerWashSettings.h` | 54.8% |
| Rematch/Sports | `example_rematch.jpg` | `SportsSettings.h` | 52.4% |
| Empire Tycoon | `example_tycoon_game.png` | `EmpireTycoon.h` | 51.3% |
| Angry Birds | `example_angry_birds.jpg` | `AngryBirdsSettings.h` | 35.0% |
| Cross Tag | `example_cross_tag.jpg` | `FighterMenu.h` | 24.5% |
| Islands & Trains | `example_islands_trains.jpg` | `IslandsTrainsSettings.h` | Missing |
| Parcel Corps | `example_parcel_coro.jpg` | `ParcelCorpsSettings.h` | Missing |

## How to Run the Comparison

### Step 1: Build the Application
```bash
cd /Users/gabeochoa/p/wm_afterhours
make
```

### Step 2: Take Screenshots of All Screens
```bash
python3 screenshot_all_screens.py
```
This runs the app, cycles through all screens using keyboard simulation, and saves screenshots to `/tmp/ui_showcase_screenshots/`.

### Step 3: Copy Screenshots to Output Directory
```bash
cp /tmp/ui_showcase_screenshots/cozy_cafe.png \
   /tmp/ui_showcase_screenshots/empire_tycoon.png \
   /tmp/ui_showcase_screenshots/neon_strike.png \
   /Users/gabeochoa/p/wm_afterhours/output/
```

### Step 4: Run the Image Diff Script
```bash
python3 scripts/image_diff.py
```

This compares each screenshot against its inspiration image and outputs:
- Similarity percentages
- Diff images saved to `output/diffs/` (highlighting pixel differences)

## Understanding the Results

### What the Similarity Score Means
- **90%+**: Excellent match, minor differences only
- **70-90%**: Good match, some layout or styling differences
- **50-70%**: Recognizable but significant differences
- **<50%**: Major layout/style mismatches

### Why Scores May Be Lower Than Expected

1. **Aspect Ratio Mismatch**
   - Inspiration images: 1024×1024 (square)
   - Our app: 1280×720 (widescreen 16:9)
   - This causes layouts to be positioned differently even when proportions are correct

2. **Missing Background Images**
   - Inspiration has illustrated backgrounds (theme parks, cafes, etc.)
   - Our implementation uses gradients/solid colors

3. **Missing Advanced Text Effects**
   - 3D/puffy text, complex outlines
   - See `LIBRARY_GAPS.md` for documented limitations

4. **Font Differences**
   - Even similar fonts render slightly differently

## File Locations

| Purpose | Path |
|---------|------|
| Inspiration images | `inspiration/` |
| Generated screenshots | `output/` |
| Diff images | `output/diffs/` |
| Diff script | `scripts/image_diff.py` |
| Screenshot automation | `screenshot_all_screens.py` |

## Improving Similarity Scores

### Layout Adjustments
1. Center content horizontally when aspect ratios differ
2. Use proportional sizing (`percent()`) instead of fixed pixels
3. Match panel widths/heights to inspiration proportions

### Visual Polish
1. Match exact colors using eyedropper tools
2. Add shadows, borders, and rounded corners
3. Use workarounds for missing features (see `src/ui_workarounds/`)

### Known Limitations (Document in LIBRARY_GAPS.md)
- Gradient backgrounds → use `GradientBackground.h` workaround
- Text outlines → use `TextOutline.h` workaround  
- Notification badges → use `NotificationBadge.h` workaround
- Decorative frames → use `DecorativeFrame.h` workaround

## The Image Diff Algorithm

The `scripts/image_diff.py` script:

1. Loads both images (inspiration and screenshot)
2. Resizes the screenshot to match inspiration dimensions (for comparison)
3. Calculates pixel-by-pixel difference using PIL's `ImageChops.difference()`
4. Counts pixels that differ by more than 0 in any RGB channel
5. Calculates similarity as: `100% - (different_pixels / total_pixels) * 100%`
6. Saves a visual diff image highlighting the differences

### Diff Image Color Key
- **Black areas**: Pixels match exactly
- **Colored areas**: Pixels differ (brighter = bigger difference)

## Quick One-Liner

To rebuild, screenshot, and check similarity in one command:

```bash
make && python3 screenshot_all_screens.py && \
cp /tmp/ui_showcase_screenshots/*.png output/ && \
python3 scripts/image_diff.py
```

## Adding New Screen Comparisons

1. Add inspiration image to `inspiration/` directory
2. Create matching screen in `src/systems/screens/`
3. Update `scripts/image_diff.py` to include the new screen mapping:

```python
screens = {
    "your_screen": ("inspiration_file.png", "your_screen.png"),
    # ... existing screens
}
```

4. Run the comparison workflow above

