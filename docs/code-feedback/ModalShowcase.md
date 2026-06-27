# Code Review: ModalShowcase

**File:** `src/systems/screens/ModalShowcase.h`
**Original lines:** 607 | **Current lines:** 515
**Lines saved:** 92

## Completed

- `.with_debug_name()` removal (auto-derived from labels)
- Section creation pattern (5 sections) → `make_section` lambda (4 of 5 — section 5 kept inline due to different styling)

## Remaining (low ROI)

1. Button config pattern repeated 8+ times (~15 lines) — buttons differ in size, color, margin; a helper would need many params
2. `.with_render_layer(MODAL_CONTENT_LAYER)` on every modal child (~15 lines) — needs vendor feature (modal render layer inheritance, doc 49)
3. Button row pattern inside modals repeated 3 times (~10 lines) — each has different justify/content
4. Result string if/else chain (~3 lines)
5. `static bool` inside `for_each_with` (~0 lines, code smell)

**Verdict:** The big win (`make_section`) is done. Remaining items are modal-specific and mostly blocked on vendor feature doc 49 (render layer inheritance for modal children).
