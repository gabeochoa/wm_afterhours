# Multi-Resolution Screenshot Validation

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Need to verify resolution-independent layouts

---

## Problem

The `screenshot_all_screens.py` tool currently captures screenshots at a single fixed resolution. This means we can't catch layout bugs that only appear at different resolutions — elements overflowing at 720p, text truncation at smaller sizes, or scaling issues when going from 720p to 1080p.

Since all our screens are meant to serve as role models for the library, they should look correct at multiple resolutions.

## Proposed Changes

### 1. Add `--resolution` flag to headless runner

Support passing resolution to `screenshot_all_screens.py` and the underlying `ui_tester.exe`:

```bash
# Single resolution (current behavior)
python3 screenshot_all_screens.py --validate

# Specific resolution
python3 screenshot_all_screens.py --validate --resolution=1280x720

# Multiple resolutions in one run
python3 screenshot_all_screens.py --validate --resolution=1280x720,1920x1080
```

### 2. Generate side-by-side output per screen

For each screen, produce one screenshot per resolution with a clear naming convention:

```
/tmp/ui_showcase_screenshots/
  cozy_cafe_720p.png
  cozy_cafe_1080p.png
  angry_birds_settings_720p.png
  angry_birds_settings_1080p.png
  ...
```

### 3. Pass resolution to `ui_tester.exe`

Add a `--resolution=WxH` argument to the headless binary so it creates a window of the specified size:

```bash
output/ui_tester.exe --screen=cozy_cafe --headless --frames=3 --resolution=1280x720
output/ui_tester.exe --screen=cozy_cafe --headless --frames=3 --resolution=1920x1080
```

### 4. Default resolutions

When `--resolution` is not specified, default to both 720p and 1080p:

| Name | Width | Height |
|------|-------|--------|
| 720p | 1280 | 720 |
| 1080p | 1920 | 1080 |

### 5. Validation at each resolution

Run the existing validation systems (screen bounds, child containment, contrast, font size, config conflict detection) at each resolution. Report which resolution triggered which warnings:

```
[720p]  cozy_cafe: 0 warnings
[1080p] cozy_cafe: 0 warnings
[720p]  angry_birds_settings: 0 warnings
[1080p] angry_birds_settings: 2 warnings
  - MinFontSize: Font size 11px below minimum 12px (entity: 203)
  - ScreenBounds: Element extends beyond screen (entity: 451)
```

---

## What This Catches

- Elements using `pixels()` that look fine at 720p but overflow at 1080p (or vice versa)
- Font sizes that resolve below the minimum at one resolution but not another
- Layouts that depend on exact pixel positions rather than relative units
- Screens that don't scale proportionally (validates `h720()`, `screen_pct()`, `percent()` usage)
- The `enforce_resolution_independence` validator can cross-check against both outputs

---

## Implementation Notes

- The headless runner already uses `SetWindowSize` / `SetTargetFPS` — just parameterize the width/height
- `screenshot_all_screens.py` already iterates screens in a loop — add an outer resolution loop
- Consider a `--quick` flag that only runs the default resolution (for fast iteration)
- Snapshot comparison (existing `--validate` mode) should compare against resolution-specific baselines
