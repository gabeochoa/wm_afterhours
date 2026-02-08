# Code Review: ParcelCorpsSettings

**File:** `src/systems/screens/ParcelCorpsSettings.h`
**Original lines:** 1204 | **Current lines:** 819
**Lines saved so far:** ~385
**Estimated remaining saveable lines:** ~80

## Completed
- ~~Legend items data-driven loop (~100 lines)~~
- ~~Merge render_toggle_row_rainbow into render_toggle_row_with_icon (~50 lines)~~
- ~~Merge render_display_row_rainbow into render_display_row (~40 lines)~~
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~20 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda (~30 lines)~~
- ~~Extract render_row_bg() helper (~25 lines)~~
- ~~Extract render_icon() helper (~20 lines)~~
- ~~Extract render_chevron() helper (~20 lines)~~
- ~~.with_absolute_position(x, y) combined call (vendor V4, ~100 lines)~~

## Remaining Screen-Level Suggestions

### 1. Chat messages loop could use a simpler struct (~15 lines saved)
**Current:** Two `div` calls per message with entity ID arithmetic.
**Suggested:** Use a struct with computed positions and a single helper lambda.

### 2. HUD elements share identical styling (~15 lines saved)
**Current:** Five separate `div` calls for LIVE, eye, diamond, score, username.
**Suggested:** Data-driven loop with `struct HudElement { label, size, x, y, font_size }`.

### 3. Status bar elements could be a data loop (~10 lines saved)
**Current:** 4 separate `div` calls for time, signal, battery.
**Suggested:** Group into a data array with position/label/style.

### 4. Quest panel items repeat the same pattern (~10 lines saved)
**Current:** Checkbox + level badge + text repeated for each quest item.
**Suggested:** Use a `struct QuestItem` array and loop.

### 5. Speedometer section has 3 divs that could be nested (~5 lines saved)
**Current:** 3 absolute-positioned elements for speed/unit/vehicle.
**Suggested:** Use a parent container with column layout.

### 6. Chevron background color repeated inline (~3 lines saved)
**Current:** `afterhours::Color{55, 60, 70, 180}` repeated.
**Suggested:** Define as `chevron_bg` member.

### 7. Unnecessary `.with_debug_name()` on decorative elements (~15 lines saved)
**Current:** Every single element has a debug name.
**Suggested:** Remove from separators, battery, decorative elements.

## Remaining Vendor-Level Suggestions

### V1. `.with_pill()` convenience for rounded pill shapes (~60 lines across screens)
`.with_rounded_corners(RoundedCorners()).with_roundness(X)` is a 2-line pattern.

### V2. `.with_circle()` for fully round elements (~30 lines across screens)
`.with_rounded_corners(RoundedCorners()).with_roundness(1.0f)` for circles.

### V3. `ComponentSize` from two floats shorthand (~50 lines across screens)
`.with_size(ComponentSize{pixels(w), pixels(h)})` could be `.with_size(pixels(w), pixels(h))`.

### V4. `ComponentConfig::inherit_style()` for copying visual style (~40 lines across screens)
Allow inheriting visual style (font, colors, alignment) from another config.

## Summary
- Remaining screen-level suggestions: 7
- Remaining vendor-level suggestions: 4
- Estimated remaining lines saveable: ~80
