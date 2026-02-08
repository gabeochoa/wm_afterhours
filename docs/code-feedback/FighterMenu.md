# Code Review: FighterMenu

**File:** `src/systems/screens/FighterMenu.h`
**Original lines:** 641 | **Current lines:** ~565
**Lines saved so far:** ~76 (batch refactor)

## Completed

- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~4 lines)~~
- ~~.with_absolute_position(x, y) combined call (~30 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Bottom button prompts are 10 entities with repeated patterns (~40 lines saved)
### 2. L and R bumper indicators are nearly identical (~15 lines saved)
### 3. Menu option loop could share base configs for icon/item (~15 lines saved)
### 4. Card panel has 8 entities for what could be structured (~15 lines saved)
### 5. Shadow colors repeated inline (~3 lines saved)
### 6. Selected menu item shadow applied twice (~5 lines saved)
### 7. `.with_debug_name()` on every element (~10 lines saved)

## Vendor-Level Suggestions -- Filed

- V1 `.with_pill()` / `.with_circle()` → doc 47
- V2 `ComponentSize` float shorthand → doc 56 (maybe, as `with_720p_size`)

## Summary
- Remaining screen-level: 7 (~103 lines saveable)
- Remaining vendor-level: 0 (all filed)
