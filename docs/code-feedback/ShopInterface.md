# Code Review: ShopInterface

**File:** `src/systems/screens/ShopInterface.h`
**Original lines:** 658 | **Current lines:** 582
**Lines saved:** 76

## Completed

- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `.with_absolute_position(x, y)` combined call
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- `with_720p_size()` migration
- `set_default_font()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- Column headers → data-driven loop

## Remaining (low ROI)

1. Item row loop: 7 entities per row, configs are all distinct (~0 lines — already looped)
2. `format_gold()` duplicated from EmpireTycoon (~0 lines — could extract to shared utility but screens are standalone)
3. Cart items loop: already looped (~0 lines)
4. Detail panel: 6 entities for selected item (~10 lines) — elements differ in position/size/font
5. Panel creation pattern (list + detail + cart) (~5 lines) — differ in border/roundness

**Verdict:** Screen is well-structured. The item row loop already exists; remaining items are structurally distinct elements.
