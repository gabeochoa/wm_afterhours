# Code Review: NeonStrike

**File:** `src/systems/screens/NeonStrike.h`
**Original lines:** 745 | **Current lines:** 614
**Lines saved:** 131

## Completed

- Texture loading lambda
- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `.with_absolute_position(x, y)` combined call
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- `with_720p_size()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- Compass cardinal directions → data-driven loop
- Equipment boxes → data-driven loop
- Map grid lines (vertical + horizontal) merged into single loop
- Health + armor bars → data-driven struct loop

## Remaining (low ROI)

1. Killstreak sidebar icons: 4 entities per icon, already looped (~0 lines — loop exists)
2. `icon_or_fallback` pattern for textures (~10 lines) — would need vendor `sprite_or_fallback`
3. Remaining `.with_debug_name()` on label-less decorative elements (~5 lines)

**Verdict:** Screen is well-refactored. Remaining items are marginal or need vendor support.
