# Code Review: NeonStrike

**File:** `src/systems/screens/NeonStrike.h`
**Original lines:** 745 | **Current lines:** ~655
**Lines saved so far:** ~90 (batch refactor)

## Completed

- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~4 lines)~~
- ~~.with_absolute_position(x, y) combined call (~50 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Texture loading is 11 near-identical lines (~15 lines saved)
### 2. Compass cardinal directions are 4 identical divs (~20 lines saved)
### 3. Killstreak sidebar icons have 4 entities per icon (~15 lines saved)
### 4. Map grid lines: vertical and horizontal loops are near-identical (~10 lines saved)
### 5. Health panel: manual progress bar (~15 lines saved)
### 6. Armor bar also manual bg + fill (~10 lines saved)
### 7. Equipment boxes share identical structure (~15 lines saved)
### 8. `icon_or_fallback` pattern for killstreak/equipment (~10 lines saved)
### 9. `.with_debug_name()` on decorative elements (~10 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `sprite_or_fallback()` → doc 56 (maybe, as generic `fallback()`)
- V2 `.with_circle()` → doc 47

## Summary
- Remaining screen-level: 9 (~120 lines saveable)
- Remaining vendor-level: 0 (all filed)
