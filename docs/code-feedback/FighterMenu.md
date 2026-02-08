# Code Review: FighterMenu

**File:** `src/systems/screens/FighterMenu.h`
**Original lines:** 641 | **Current lines:** ~565
**Lines saved so far:** ~76 (batch refactor)
**Estimated remaining saveable lines:** ~100

## Completed
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~4 lines)~~
- ~~.with_absolute_position(x, y) combined call (~30 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Bottom button prompts are 10 entities with repeated patterns (~40 lines saved)
**Suggested:** Data-driven loop with `struct Prompt { btn, label, color, x }`.

### 2. L and R bumper indicators are nearly identical (~15 lines saved)
**Suggested:** Loop over `{{"L", x_left}, {"R", x_right}}`.

### 3. Menu option loop could share base configs for icon/item (~15 lines saved)
**Suggested:** Share common attributes via a base config.

### 4. Card panel has 8 entities for what could be structured (~15 lines saved)
**Suggested:** Use a column container with children instead of absolute positioning.

### 5. `afterhours::Color{0, 0, 0, X}` shadow colors repeated inline (~3 lines saved)
**Suggested:** Use `shadow_color` member for all shadows.

### 6. Selected menu item shadow applied twice (icon + item) (~5 lines saved)
**Suggested:** Compute shadow once, apply to both.

### 7. `.with_debug_name()` on every element (~10 lines saved)
**Suggested:** Remove from decorative elements.

## Remaining Vendor-Level Suggestions

### V1. `.with_pill()` / `.with_circle()` convenience (~8 lines saved)
For bumper buttons.

### V2. `ComponentSize` float shorthand (~8 lines saved)

## Summary
- Remaining screen-level suggestions: 7
- Remaining vendor-level suggestions: 2
- Estimated remaining lines saveable: ~100
