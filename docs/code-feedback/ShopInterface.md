# Code Review: ShopInterface

**File:** `src/systems/screens/ShopInterface.h`
**Original lines:** 658 | **Current lines:** ~570
**Lines saved so far:** ~88 (batch refactor)
**Estimated remaining saveable lines:** ~90

## Completed
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~5 lines)~~
- ~~.with_absolute_position(x, y) combined call (~30 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Item row loop creates 7 entities with verbose configs (~40 lines saved)
**Suggested:** Extract `render_item_row(item, index, is_selected, y_pos)` helper.

### 2. Column header divs repeat the same pattern 4 times (~15 lines saved)
**Suggested:** Loop over `{{"Item", col_name_x}, {"Type", col_cat_x}, ...}`.

### 3. `format_gold()` duplicated from EmpireTycoon (~10 lines saved)
**Suggested:** Extract to a shared utility header.

### 4. Cart items loop is verbose (~10 lines saved)
**Suggested:** Reduce config chain by extracting shared text style.

### 5. Detail panel creates 6 entities that could share a base config (~15 lines saved)
**Suggested:** Define a `detail_text_config` base and customize per element.

### 6. Panel creation pattern repeated 3 times (~8 lines saved)
**Suggested:** Helper or shared base config for panel bg + border + rounded + shadow.

### 7. `.with_debug_name()` on every element (~10 lines saved)
**Suggested:** Remove from separators, alt row bgs, static text.

## Remaining Vendor-Level Suggestions

### V1. `.with_pill()` convenience (~10 lines saved)

### V2. `ComponentSize` float pair shorthand (~10 lines saved)

## Summary
- Remaining screen-level suggestions: 7
- Remaining vendor-level suggestions: 2
- Estimated remaining lines saveable: ~90
