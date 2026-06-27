# Code Review: FighterMenu

**File:** `src/systems/screens/FighterMenu.h`
**Original lines:** 641 | **Current lines:** 527
**Lines saved:** 114

## Completed

- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `.with_absolute_position(x, y)` combined call
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- `with_720p_size()` migration
- `set_default_font()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- L/R bumper indicators → data-driven loop
- Bottom button prompts → data-driven loop

## Remaining (low ROI)

1. Menu option loop: icon + item bar share base config (~10 lines) — configs differ enough that merging adds complexity
2. Card panel: 8 entities forming info card (~10 lines) — elements are structurally different
3. Shadow colors repeated inline (~3 lines) — already uses `shadow_color` variable
4. Selected item shadow applied in two places (~3 lines)

**Verdict:** Screen is in good shape. Remaining items are <10 lines each.
