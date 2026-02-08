# Code Review: ParcelCorpsSettings

**File:** `src/systems/screens/ParcelCorpsSettings.h`
**Original lines:** 1204 | **Current lines:** 819
**Lines saved so far:** ~385

## Completed

- ~~Legend items data-driven loop (~100 lines)~~
- ~~Merge render_toggle_row_rainbow into render_toggle_row_with_icon (~50 lines)~~
- ~~Merge render_display_row_rainbow into render_display_row (~40 lines)~~
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~20 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda (~30 lines)~~
- ~~Extract render_row_bg() helper (~25 lines)~~
- ~~Extract render_icon() helper (~20 lines)~~
- ~~Extract render_chevron() helper (~20 lines)~~
- ~~.with_absolute_position(x, y) combined call (~100 lines)~~

## Remaining Screen-Level Suggestions

### 1. Chat messages loop could use a simpler struct (~15 lines saved)
### 2. HUD elements share identical styling (~15 lines saved)
### 3. Status bar elements could be a data loop (~10 lines saved)
### 4. Quest panel items repeat the same pattern (~10 lines saved)
### 5. Speedometer section has 3 divs that could be nested (~5 lines saved)
### 6. Chevron background color repeated inline (~3 lines saved)
### 7. Unnecessary `.with_debug_name()` on decorative elements (~15 lines saved)

## Vendor-Level Suggestions -- Filed

All vendor-level suggestions have been filed as standalone docs:
- V1 `.with_pill()` → doc 47
- V2 `.with_circle()` → doc 47
- V3 `ComponentSize` float shorthand → doc 56 (maybe, as `with_720p_size`)
- V4 `ComponentConfig::inherit_style()` → not yet filed

## Summary
- Remaining screen-level: 7 (~73 lines saveable)
- Remaining vendor-level: 1 unfiled (inherit_style)
