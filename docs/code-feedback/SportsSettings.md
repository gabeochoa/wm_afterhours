# Code Review: SportsSettings

**File:** `src/systems/screens/SportsSettings.h`
**Original lines:** 769 | **Current lines:** 662
**Lines saved:** 107

## Completed

- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `.with_absolute_position(x, y)` combined call
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- `with_720p_size()` migration
- `set_default_font()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- Bottom button prompts → data-driven loop
- LB/RB buttons → data-driven loop
- `get_current_settings()` / `get_section_header()` → unified `TabInfo` struct
- `format_slider_value()` simplified to use `get_current_settings()`
- Help panel 4 divs → data-driven loop

## Remaining (low ROI)

1. Setting row data uses verbose struct initialization (~0 lines — inherent to the data)
2. Per-row 5+ entities with verbose configs (~0 lines — elements are structurally different: label, arrows, value, slider, segments)
3. Left/right arrow buttons duplicated per row (~0 lines — already in the row loop)
4. `get_setting_description` + `_line2` split (~5 lines) — could return a struct but minimal gain
5. Keyboard input handling duplicates click logic (~5 lines) — different code paths (pressed vs button click)

**Verdict:** Screen is well-refactored. The main settings loop is necessarily complex due to the variety of control types (sliders, selectors, segmented indicators).
