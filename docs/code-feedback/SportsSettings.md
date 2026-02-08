# Code Review: SportsSettings

**File:** `src/systems/screens/SportsSettings.h`
**Original lines:** 769 | **Current lines:** ~700
**Lines saved so far:** ~69 (batch refactor)

## Completed

- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~2 lines)~~
- ~~.with_absolute_position(x, y) combined call (~40 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Setting row data uses verbose struct initialization (~30 lines saved)
### 2. `get_current_settings()` and `format_slider_value()` both switch on `selected_tab` (~15 lines saved)
### 3. LB and RB buttons are near-identical (~15 lines saved)
### 4. Each settings row creates 5+ entities with verbose configs (~40 lines saved)
### 5. Left/right arrow buttons are duplicated per row (~20 lines saved)
### 6. Help/info panel creates 4 nearly identical divs (~10 lines saved)
### 7. Bottom button prompts repeat the same pattern (~10 lines saved)
### 8. `get_setting_description` and `_line2` are split unnecessarily (~10 lines saved)
### 9. Keyboard input handling duplicates click logic (~10 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `.with_transparent_bg()` → doc 55 (YES)
- V2 `ComponentSize` float shorthand → doc 56 (maybe, as `with_720p_size`)

## Summary
- Remaining screen-level: 9 (~160 lines saveable)
- Remaining vendor-level: 0 (all filed)
