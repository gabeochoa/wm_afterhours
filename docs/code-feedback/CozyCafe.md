# Code Review: CozyCafe

**File:** `src/systems/screens/CozyCafe.h`
**Original lines:** 800 | **Current lines:** ~695
**Lines saved so far:** ~105 (batch refactor)

## Completed

- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~8 lines)~~
- ~~.with_absolute_position(x, y) combined call (~45 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Texture loading is 9 near-identical lines (~12 lines saved)
### 2. Star rating rendering has triple-branching for each star (~20 lines saved)
### 3. Chat avatars duplicated for 2 messages (~25 lines saved)
### 4. Bottom-right icon buttons share identical structure (~40 lines saved)
### 5. Customer row rendering repeats per customer (~15 lines saved)
### 6. Menu item buttons share identical sizing/styling (~10 lines saved)
### 7. Music slider is manual (track + fill + handle) (~15 lines saved)
### 8. Panel creation pattern repeated for left and right (~10 lines saved)
### 9. Repeated `.with_font("Gaegu-Bold", h720(X))` pattern (~5 lines saved)
### 10. `.with_debug_name()` on decorative elements (~10 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `sprite_or_fallback()` → doc 56 (maybe, as generic `fallback()`)
- V2 `.with_pill()` → doc 47
- V3 Default font per-screen → not yet filed

## Summary
- Remaining screen-level: 10 (~162 lines saveable)
- Remaining vendor-level: 1 unfiled (default font per-screen)
