# Code Review: EmpireTycoon

**File:** `src/systems/screens/EmpireTycoon.h`
**Original lines:** 944 | **Current lines:** 865
**Lines saved so far:** ~79

## Completed

- ~~Texture loading lambda (~20 lines)~~
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~8 lines)~~
- ~~.with_absolute_position(x, y) combined call (~50 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~
- ~~Title text shadow+main data-driven loop (~15 lines)~~
- ~~Chat box lines loop (~5 lines)~~

## Remaining Screen-Level Suggestions

### 1. Status icon rendering is duplicated for happiness/resources (~30 lines saved)
### 2. Meter rendering (happiness + resources) is duplicated (~50 lines saved)
### 3. Tab loop icon fallback could be simplified (~15 lines saved)
### 4. Bottom-right icons verbose config chains (~25 lines saved)
### 5. Action buttons config is verbose (~10 lines saved)
### 6. Shadow color repeated ~5 times (~3 lines saved)
### 7. Cloud sprites duplicate source rect calculation (~5 lines saved)
### 8. `format_money` duplicated in ShopInterface (~0 lines but better reuse)
### 9. Bottom milestone bar is a manual progress bar (~10 lines saved)
### 10. `.with_debug_name()` on every element (~15 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `.with_pill()` / `.with_circle()` → doc 47
- V2 `sprite_or_fallback()` → doc 56 (maybe, as generic `fallback()`)
- V3 `ComponentSize` float pair → doc 56 (maybe, as `with_720p_size`)

## Summary
- Remaining screen-level: 10 (~163 lines saveable)
- Remaining vendor-level: 0 (all filed)
