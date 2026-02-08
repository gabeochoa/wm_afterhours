# Code Review: ShopInterface

**File:** `src/systems/screens/ShopInterface.h`
**Original lines:** 658 | **Current lines:** ~570
**Lines saved so far:** ~88 (batch refactor)

## Completed

- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~5 lines)~~
- ~~.with_absolute_position(x, y) combined call (~30 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Item row loop creates 7 entities with verbose configs (~40 lines saved)
### 2. Column header divs repeat the same pattern 4 times (~15 lines saved)
### 3. `format_gold()` duplicated from EmpireTycoon (~10 lines saved)
### 4. Cart items loop is verbose (~10 lines saved)
### 5. Detail panel creates 6 entities that could share a base config (~15 lines saved)
### 6. Panel creation pattern repeated 3 times (~8 lines saved)
### 7. `.with_debug_name()` on every element (~10 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `.with_pill()` → doc 47
- V2 `ComponentSize` float pair shorthand → doc 56 (maybe, as `with_720p_size`)

## Summary
- Remaining screen-level: 7 (~108 lines saveable)
- Remaining vendor-level: 0 (all filed)
