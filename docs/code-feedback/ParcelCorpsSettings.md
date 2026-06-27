# Code Review: ParcelCorpsSettings

**File:** `src/systems/screens/ParcelCorpsSettings.h`
**Original lines:** 1204 | **Current lines:** 760
**Lines saved:** 444

## Completed

- Legend items → data-driven loop
- Merge `render_toggle_row_rainbow` → `render_toggle_row_with_icon`
- Merge `render_display_row_rainbow` → `render_display_row`
- `std::bitset<4>(0b1111)` → `RoundedCorners()`
- `pixels(static_cast<int>(...))` → `pxf()` lambda
- Extract `render_row_bg()` helper
- Extract `render_icon()` helper
- Extract `render_chevron()` helper
- `.with_absolute_position(x, y)` combined call
- `with_720p_size()` migration
- `set_default_font()` migration
- `.with_debug_name()` removal (auto-derived from labels)
- Quest items → data-driven loop
- HUD elements → data-driven loop
- Speedometer labels → data-driven loop

## Remaining (low ROI)

1. Chat messages: already a data-driven loop (~0 lines)
2. Status bar elements (~5 lines) — only 3 elements, each distinct
3. Chevron background color repeated inline (~0 lines — defined in helper)

**Verdict:** Screen went from 1204 → 760 lines (37% reduction). Fully refactored; remaining items are marginal.
