# Code Review: SportsSettings

**File:** `src/systems/screens/SportsSettings.h`
**Original lines:** 769 | **Current lines:** ~700
**Lines saved so far:** ~69 (batch refactor)
**Estimated remaining saveable lines:** ~130

## Completed
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~2 lines)~~
- ~~.with_absolute_position(x, y) combined call (~40 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Setting row data uses verbose struct initialization (~30 lines saved)
**Suggested:** Use two separate structs or named initializers.

### 2. `get_current_settings()` and `format_slider_value()` both switch on `selected_tab` (~15 lines saved)
**Suggested:** Store a `std::vector<SettingRow>*` pointer once at the top.

### 3. LB and RB buttons are near-identical (~15 lines saved)
**Suggested:** Loop over `{{"LB", x1, -1}, {"RB", x2, +1}}`.

### 4. Each settings row creates 5+ entities with verbose configs (~40 lines saved)
**Suggested:** Extract a `render_setting_row()` helper.

### 5. Left/right arrow buttons are duplicated per row (~20 lines saved)
**Suggested:** Extract `render_arrow_button()`.

### 6. Help/info panel creates 4 nearly identical divs (~10 lines saved)
**Suggested:** Data-drive with `struct { label, y_offset, color }` array.

### 7. Bottom button prompts repeat the same pattern (~10 lines saved)
**Suggested:** Loop over `{{"Y", "Reset to default", yellow}, {"B", "Back", red}}`.

### 8. `get_setting_description` and `_line2` are split unnecessarily (~10 lines saved)
**Suggested:** Return a `std::pair<string, string>` from one function.

### 9. Keyboard input handling duplicates click logic (~10 lines saved)
**Suggested:** Extract `adjust_setting_value(setting, direction)`.

## Remaining Vendor-Level Suggestions

### V1. `.with_transparent_bg()` convenience (~5 lines saved)
`.with_custom_background(afterhours::Color{0, 0, 0, 0})` -> `.with_transparent_bg()`.

### V2. `ComponentSize` float shorthand (~10 lines saved)

## Summary
- Remaining screen-level suggestions: 9
- Remaining vendor-level suggestions: 2
- Estimated remaining lines saveable: ~130
