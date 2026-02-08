# Code Review: EmpireTycoon

**File:** `src/systems/screens/EmpireTycoon.h`
**Original lines:** 944 | **Current lines:** 760
**Lines saved:** 184

## Completed

- Texture loading lambda
- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `.with_absolute_position(x, y)` combined call
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- `with_720p_size()` migration
- `set_default_font()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- Title text shadow + main → data-driven loop
- Chat box lines → loop
- Status icons (happiness + resources) → data-driven loop
- Happiness/resources meters → data-driven loop (5 shared elements)
- Cloud sprites → data-driven loop
- Production/Projects panels → shared title+box pattern

## Remaining (low ROI)

1. Tab loop icon fallback (~10 lines) — needs vendor `sprite_or_fallback` (decided NO)
2. Bottom-right icons verbose configs (~10 lines) — elements are structurally different
3. Action buttons config verbose (~5 lines)
4. Shadow color repeated inline (~3 lines) — could extract variable
5. Bottom milestone bar manual (~5 lines)

**Verdict:** Screen went from 944 → 760 lines (19% reduction). Major duplicated patterns all merged. Remaining items are marginal.
