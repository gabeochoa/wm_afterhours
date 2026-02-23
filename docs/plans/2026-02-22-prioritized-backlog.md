# Prioritized Backlog

All work items extracted from `docs/` and `vendor/afterhours/docs/`. Deduplicated
across overlapping documents.

**Size:** S (< 1 day) / M (1-3 days) / L (3-5 days) / XL (1-2 weeks)
**Priority:** H (high) / M (medium) / L (low)

---

## Summary Table

| # | Item | Size | Pri | Source |
|---|------|------|-----|--------|
| 1 | Layout perf Phase A: flat vector + SmallVector + template callbacks | S | H | layout-perf-design |
| 2 | Layout perf Phase C: skip layout on scroll-only frames | M | H | layout-perf-design |
| 3 | Layout perf Phase B: single-pass violation solving | M | H | layout-perf-design |
| 4 | Dirty-flag guard on merge_entity_arrays | S | H | perf-optimization 2025 |
| 5 | Skip systems with zero matching entities | S | H | perf-optimization 2025 |
| 6 | std::set -> unordered_set (permanent_ids + collision ids) | S | H | perf-optimization 2025 |
| 7 | Singleton map -> flat array | S | H | perf-optimization 2025 |
| 8 | Cache collidable entity list in once() | S | H | perf-optimization 2025 |
| 9 | Singleton lookup for input collector | S | H | perf-optimization 2025 |
| 10 | Cache mouse position per frame | S | H | perf-optimization 2025 |
| 11 | StylePreset struct + Builder API | S | H | styling-presets-design |
| 12 | StyleRegistry (named styles) | S | H | styling-presets-design |
| 13 | Auto-interpolating visual transitions (HasStyleTransition) | M | H | styling-presets-design |
| 14 | Virtualized scroll (lazy lists / show_rows) | L | H | egui + swiftui gap |
| 15 | Toast notifications | S | H | fabula gap |
| 16 | Alert dialog | S | H | swiftui gap |
| 17 | Scroll offset API (read offset, stick-to-bottom) | M | H | egui + fabula gap |
| 18 | Scroll-to API (programmatic scroll) | M | H | egui gap |
| 19 | Accordion / collapsible sections | S | H | egui + fabula gap |
| 20 | Focus ring P0: fix stuck/trapped focus (flex_alignment, islands_trains) | S | H | focus-ring-audit |
| 21 | E2E coverage expansion (7 new test scripts) | M | M | e2e-coverage-design |
| 22 | Bottom/side sheet (drawer panel) | M | M | fabula + swiftui gap |
| 23 | Navigation stack (push/pop with back button) | M | M | swiftui gap |
| 24 | Grid layout (auto-columns, striped rows) | M | M | egui + swiftui gap |
| 25 | Per-widget tooltips | M | M | egui gap |
| 26 | Segmented control | S | M | swiftui gap |
| 27 | Action sheet (bottom action list) | S | M | swiftui gap |
| 28 | Secure text field (password input) | S | M | swiftui gap |
| 29 | Page view (swipeable pages) | M | M | swiftui gap |
| 30 | Virtual joystick | M | M | fabula gap |
| 31 | Animated tab indicator | S | M | fabula gap |
| 32 | List with sections (grouped list) | S | M | swiftui gap |
| 33 | Collapsible text (line-limited expand/collapse) | S | M | fabula gap |
| 34 | Style inheritance / extend() | S | M | styling-presets-design |
| 35 | Icon Registry + icon_button composite | M | M | doc 18 |
| 36 | Layout QA pipeline (collect + audit scripts) | M | M | layout-qa-pipeline-design |
| 37 | Unified Widget Gallery screen | M | M | egui showcase ideas |
| 38 | Focus ring P1: add focus to 11 interactive screens | M | M | focus-ring-audit |
| 39 | Focus ring P2: improve ring contrast on 14 screens | M | M | focus-ring-audit |
| 40 | Auto-snapshot all screens in CI | M | M | egui showcase ideas |
| 41 | Template-based query filters (replace std::function) | M | M | perf-optimization 2025 |
| 42 | gen_for_each(callback) to avoid materializing vector | S | M | perf-optimization 2025 |
| 43 | Dirty flag for animation manager | S | M | perf-optimization 2025 |
| 44 | Spatial hash grid for collision broad-phase | L | M | perf-optimization 2025 |
| 45 | Replace shared_ptr<Entity> with arena/pool allocation | XL | M | perf-optimization 2025 |
| 46 | SoA component storage / archetype tables | XL | M | perf-optimization 2025 |
| 47 | Per-component-type entity index lists | L | M | perf-optimization 2025 |
| 48 | Form container | S | L | swiftui gap |
| 49 | Resizable panels | M | L | egui + fabula gap |
| 50 | Popup alignment API | S | L | egui gap |
| 51 | Undo/redo utility | S | L | egui gap |
| 52 | DatePicker | M | L | swiftui gap |
| 53 | Image comparison slider | S | L | fabula gap |
| 54 | 3D fold animation | M | L | fabula gap |
| 55 | Code snippets in showcases | M | L | egui showcase ideas |
| 56 | Widget state controls in showcases | S | L | egui showcase ideas |
| 57 | Source code links in HUD | S | L | egui showcase ideas |
| 58 | Web demo via Emscripten | XL | L | egui showcase ideas |
| 59 | Rounded corner stencil/shader masking | L | L | layout-qa framework |
| 60 | Focus ring P3: images screen limited tab coverage | S | L | focus-ring-audit |

---

## High Priority Items (20)

### Performance — Layout Engine

Items 1-3 from `vendor/afterhours/docs/plans/2026-02-22-layout-perf-design.md`.

**1. Phase A: Eliminate per-frame allocations and map lookups** — Size: S, Pri: H

Replace `std::map<EntityID, RefEntity>` with flat vector (O(1) lookup), add
SmallVector for layout_children (avoid heap allocs), template the spacing
callbacks (eliminate std::function). Mechanical changes, no behavior change.
Validate with existing E2E suite — zero diffs expected.

**2. Phase C: Skip layout on scroll-only frames** — Size: M, Pri: H

Add UILayoutFingerprint (entity count + structure hash) to detect identical
tree across frames. When fingerprint matches, skip RunAutoLayout entirely.
Biggest user-visible win for scroll smoothness. Moderate risk (hash
correctness). Validate by toggling debug mode for layout-every-frame comparison.

**3. Phase B: Single-pass violation solving** — Size: M, Pri: H

Merge solve_violations + tax_refund into distribute_space. Proportional
shrink distribution (RAD-style) instead of iterative even distribution.
Highest risk — layout output will shift by a few pixels. Requires E2E
baseline regeneration after validation.

### Performance — Core ECS

Items 4-10 from `vendor/afterhours/docs/plans/2025-02-15-performance-optimization-design.md`.

**4. Dirty-flag guard on merge_entity_arrays** — Size: S, Pri: H

Add `std::atomic<bool> temp_dirty` to EntityCollection. Skip O(N) merge when
no entities were created. ~29 unnecessary merges eliminated per frame with 30+
systems. Full design in `2025-02-15-item-01-dirty-flag-merge.md`.

**5. Skip systems with zero matching entities** — Size: S, Pri: H

Maintain population counts per component type. Check before system iteration.
Zero-entity systems become a single array lookup + branch.

**6. std::set -> unordered_set** — Size: S, Pri: H

Two one-line type changes: `permanant_ids` and collision `ids`. O(1) vs O(log N).

**7. Singleton map -> flat array** — Size: S, Pri: H

Replace `unordered_map<ComponentID, Entity*>` with `std::array<Entity*, MAX>`.
ComponentID is a small integer. Zero hash overhead.

**8. Cache collidable entity list in once()** — Size: S, Pri: H

Move EntityQuery from per-entity into system's once(). Eliminates N full scans.

**9. Singleton lookup for input collector** — Size: S, Pri: H

Replace full EntityQuery with `get_singleton_cmp<InputCollector>()`. One-line fix.

**10. Cache mouse position per frame** — Size: S, Pri: H

Store computed mouse position in frame-local variable. Scaling math runs once.

### Styling

Items 11-13 from `vendor/afterhours/docs/plans/2026-02-22-styling-presets-design.md`.

**11. StylePreset struct + Builder API** — Size: S, Pri: H

Value type bundling base ComponentConfig with per-state overrides (hover,
pressed, focused, disabled) and transition parameters. Builder pattern for
ergonomic construction. Foundation for all other styling improvements.

**12. StyleRegistry (named styles)** — Size: S, Pri: H

Global `unordered_map<string, StylePreset>` for looking up styles by name.
Decouples style definition from style usage. Additive — no existing code changes.

**13. Auto-interpolating visual transitions** — Size: M, Pri: H

HasStyleTransition component tracking current/target values for bg color,
border, shadow, opacity, scale, roundness. System lerps between states on
hover/focus/press changes. Requires render system integration to read from
transition state instead of raw config.

### New Components — Core Gaps

Items 14-19 from `docs/egui-comparison.md` and `docs/fabula-comparison.md`.

**14. Virtualized scroll (lazy lists)** — Size: L, Pri: H

Only lay out visible children in scroll containers. Three tiers: simple (all
children), uniform rows (O(visible) via row height), full manual (viewport
rect query). Single biggest performance gap for long lists. Both egui and
SwiftUI have this.

**15. Toast notifications** — Size: S, Pri: H

Auto-dismissing notifications that slide in from top/bottom. Configurable
position, duration, tap-to-dismiss. Common game UI need (achievements, status,
errors). API: `imm::toast(ctx, "message", ToastConfig{})`.

**16. Alert dialog** — Size: S, Pri: H

Modal dialog with title, message, and 1-3 action buttons (OK, Cancel,
Destructive). Standard UI pattern for confirm quit, overwrite save, delete
character. Distinct from the existing modal system which is content-agnostic.

**17. Scroll offset API** — Size: M, Pri: H

Expose scroll offset for programmatic use: `get_scroll_offset()`,
`stick_to_bottom(true)`, `scroll_with_delta(Vec2)`. Enables chat windows,
log viewers, parallax, snap-to-item, custom scroll indicators.

**18. Scroll-to API** — Size: M, Pri: H

Programmatic scroll control: `scroll_to_row(index, alignment)`,
`scroll_to_cursor(Align::TOP)`, `vertical_scroll_offset(pixels)`. Jump-to-item
for long lists, auto-scroll for chat/logs.

**19. Accordion / collapsible sections** — Size: S, Pri: H

Expand/collapse sections with header + content. Similar to tree_view but for
flat content grouping (settings categories, quest logs). egui has
CollapsingHeader, Fabula has CollapsibleView. Simple wrapper around existing
tree_view mechanics.

### Quality

**20. Focus ring P0: fix stuck/trapped focus** — Size: S, Pri: H

Two screens broken: flex_alignment (Tab never advances past "All" tab),
islands_trains_settings (focus trapped in slider notches, consumes all 16 tabs
on one control). These completely block keyboard navigation.

---

## Medium Priority Items (27)

### Testing & Quality

**21. E2E coverage expansion (7 new scripts)** — Size: M, Pri: M

Add tests for potion_crafting, shop_interface, kart_select, toasts, file_tree,
horizontal_drag, stepper_showcase. Design in
`docs/plans/2026-02-18-e2e-coverage-expansion-design.md`.

**36. Layout QA pipeline** — Size: M, Pri: M

Python scripts for automated screenshot collection + AI-driven layout audit.
Two phases: collect_qa_screenshots.py, run_layout_audit.py. Supports Claude
CLI and Cursor backends. Design in
`docs/plans/2026-02-22-layout-qa-pipeline-design.md`.

**38. Focus ring P1: add focus to 11 screens** — Size: M, Pri: M

radio_buttons, scroll_view, neon_strike, absolute_positioning, layout,
self_align, text_overflow, meters_gauges, nine_slice_borders, example_borders,
horizontal_drag all have interactive elements with no focus ring.

**39. Focus ring P2: improve ring contrast on 14 screens** — Size: M, Pri: M

tab_container, cozy_cafe, rubber_bandits_menu, deadspace_settings, and 10
others have low-visibility focus rings. Need per-theme focus color overrides
or thicker rings.

**40. Auto-snapshot all screens in CI** — Size: M, Pri: M

run_headless_screenshots() already exists. Add CI job with per-screen
regression comparison against baselines.

### New Components — Polish

**22. Bottom/side sheet (drawer panel)** — Size: M, Pri: M

Edge-anchored panels with drag-to-open, spring animation, background dimming.
Portrait = bottom sheet, landscape = side sheet. Inventory, chat, settings tray.

**23. Navigation stack (push/pop)** — Size: M, Pri: M

Screen flow with automatic back button. Push screens onto a stack, pop to go
back. navigation_bar exists but doesn't manage a screen stack.

**24. Grid layout** — Size: M, Pri: M

Auto-wrapping grid container with column count or min-column-width. Striped
rows optional. Needed for item inventories, icon grids, data tables.

**25. Per-widget tooltips** — Size: M, Pri: M

Hover-triggered popup on any widget via chained API:
`.on_hover_text("tooltip")`. Interactive tooltips with rich content. Positioned
at cursor, auto-flips to stay on screen.

**26. Segmented control** — Size: S, Pri: M

Inline horizontal option selector (like iOS segmented control). Map/transit/
satellite style. Distinct from tabs — segmented controls change a value, tabs
change content.

**27. Action sheet** — Size: S, Pri: M

Bottom-anchored action list (iOS-style). Slide up with options + cancel button.
Context actions, share menus.

**28. Secure text field** — Size: S, Pri: M

Password input with show/hide toggle. Renders dots/asterisks by default. Uses
existing text_area internals with masking overlay.

**29. Page view (swipeable pages)** — Size: M, Pri: M

Horizontal swipe to switch between full-screen pages. Onboarding, tutorials,
character selection. Includes dot indicator for current page.

**30. Virtual joystick** — Size: M, Pri: M

On-screen joystick with circular boundary constraint. Reports angle + distance.
Springs back to center on release. Mobile/touch game UIs.

**31. Animated tab indicator** — Size: S, Pri: M

Smooth sliding underline/highlight between tab buttons. Current tab_container
has instant switching. Animated indicator improves perceived quality.

**32. List with sections** — Size: S, Pri: M

Grouped list with section headers, like UITableView with sections. Settings
categories, categorized inventory items.

**33. Collapsible text** — Size: S, Pri: M

Truncate text to N lines, show More/Less button only when text exceeds limit.
Quest descriptions, item tooltips, dialog logs.

**34. Style inheritance / extend()** — Size: S, Pri: M

`StylePreset::extend(parent)` deep-copies parent and applies overrides.
`danger_btn = extend(toolbar_btn).base(red_bg)`. Simple composition.

**35. Icon Registry + icon_button** — Size: M, Pri: M

Central registry mapping action IDs to icon textures + fallback text symbols.
`icon_button()` composite: registry lookup + sprite/text + label + click.
Design in `docs/18_icon_registry.md`.

**37. Unified Widget Gallery screen** — Size: M, Pri: M

Single showcase rendering every imm:: primitive and composite in a grid (label |
widget). Everything-at-a-glance view inspired by egui's Widget Gallery.

### Performance — Deeper

**41. Template-based query filters** — Size: M, Pri: M

Replace std::function filter storage with template chain. Eliminates virtual
dispatch and heap allocation per filter.

**42. gen_for_each(callback)** — Size: S, Pri: M

Callback-based iteration that avoids materializing RefEntities vector. Additive
API alongside existing gen().

**43. Dirty flag for animation manager** — Size: S, Pri: M

Track has_active_tracks. Skip entire hash map iteration when no tracks active.

**44. Spatial hash grid for collision** — Size: L, Pri: M

Replace O(N^2) all-pairs with spatial hash. O(N) for uniformly distributed
entities. Huge impact for games with many colliders.

**45. Replace shared_ptr<Entity> with arena/pool** — Size: XL, Pri: M

Eliminate refcount traffic and heap fragmentation. Entities in contiguous pool.
Touches every system that stores entity references. Massive impact but high
effort and API-breaking.

**46. SoA component storage** — Size: XL, Pri: M

Per-archetype dense arrays instead of per-entity ComponentArray[128]. Cache-
friendly iteration. Needs investigation (type-erased placement-new vs explicit
registration vs hybrid). API-breaking.

**47. Per-component-type entity index lists** — Size: L, Pri: M

vector<Entity*> per component type. Systems iterate only their component's
list. Huge impact — needs investigation on whether ComponentBitSet can drive
index maintenance without API changes.

---

## Low Priority Items (13)

### New Components — Nice-to-Have

**48. Form container** — Size: S, Pri: L

Grouped control layout with section headers/footers. Styling wrapper.

**49. Resizable panels** — Size: M, Pri: L

User-draggable split panes (left/right or top/bottom). Editor/tool UIs.

**50. Popup alignment API** — Size: S, Pri: L

General-purpose popup with anchor point + alignment control (RectAlign).

**51. Undo/redo utility** — Size: S, Pri: L

Generic stack for any Clone+PartialEq state. Settings, editors, forms.

**52. DatePicker** — Size: M, Pri: L

Calendar date selection widget. Niche for games.

**53. Image comparison slider** — Size: S, Pri: L

Before/after image overlay with draggable divider. Graphics settings preview.

**54. 3D fold animation** — Size: M, Pri: L

Paper-fold effect using rotation3D. Page transitions, card flips. Requires 3D
perspective transforms on UI elements.

### Showcase & Documentation

**55. Code snippets in showcases** — Size: M, Pri: L

Show ComponentConfig builder code next to rendered widget. Self-documenting
showcases.

**56. Widget state controls** — Size: S, Pri: L

Visible/disabled/opacity toggles in showcase screens for testing states.

**57. Source code links in HUD** — Size: S, Pri: L

Show file path of current screen in HUD overlay.

**58. Web demo via Emscripten** — Size: XL, Pri: L

Compile to WebGL/WebAssembly for live browser demo. High effort, high marketing
impact.

### Framework

**59. Rounded corner stencil/shader masking** — Size: L, Pri: L

Children with rectangular backgrounds poke past parent rounded corners.
Requires stencil buffer or shader masking; scissor only clips rectangularly.
16 screens affected (framework limitation in layout QA).

**60. Focus ring P3: images screen** — Size: S, Pri: L

Only 3 of 8+ interactive elements in tab order on the images screen.

---

## Completed Items (for reference)

| Item | Source |
|------|--------|
| UI Debug Mode (GridOverlay, RenderFlash, InlineDimensionLabels, HoverInspector) | ui-debug-mode-design |
| Nested Clip Scissor Fix (compute_intersected_clip_rect) | nested-clip-scissor-fix |
| Adaptive Scaling (all 5 phases, 42 screens converted) | doc 30 |
| Color System (named colors, WCAG, auto_text_color) | doc 05 |
| Button Variants (Filled, Outline, Ghost) | doc 03 |
| Stepper/Selector (string, int, float variants) | doc 08 |
| Tree Node / Collapsible Header (tree_view) | doc 11 |
| Layout QA audit (89 issues fixed, 0 remaining open) | layout_qa_todo |

---

## Suggested Implementation Order

Work in parallel tracks. Each track can be done independently.

### Track 1: Performance (makes everything feel better)

1. Items 4-10 — Core ECS quick wins (all S, ~1 day total)
2. Item 1 — Layout perf Phase A (S, ~half day)
3. Item 2 — Layout perf Phase C (M, scroll feel)
4. Item 3 — Layout perf Phase B (M, then baseline regen)

### Track 2: Styling (makes building UIs faster)

1. Item 11 — StylePreset struct
2. Item 12 — StyleRegistry
3. Item 34 — Style extend()
4. Item 13 — Auto-interpolating transitions

### Track 3: Missing Components (fills the biggest gaps)

1. Item 20 — Fix P0 focus bugs (blocker)
2. Item 15 — Toast notifications (quick win)
3. Item 16 — Alert dialog (quick win)
4. Item 19 — Accordion sections (quick win)
5. Item 17 + 18 — Scroll APIs (unlocks many patterns)
6. Item 14 — Virtualized scroll (biggest perf gap for content)

### Track 4: Testing & Quality

1. Item 21 — E2E coverage expansion
2. Item 38 — Focus ring P1
3. Item 40 — Auto-snapshot CI
4. Item 36 — Layout QA pipeline

---

## Source Documents

| Document | Location |
|----------|----------|
| Layout Engine Performance | `vendor/afterhours/docs/plans/2026-02-22-layout-perf-design.md` |
| Styling Presets | `vendor/afterhours/docs/plans/2026-02-22-styling-presets-design.md` |
| UI Debug Mode | `vendor/afterhours/docs/plans/2026-02-22-ui-debug-mode-design.md` |
| Nested Clip Fix | `docs/plans/2026-02-22-nested-clip-scissor-fix-design.md` |
| Layout QA Pipeline | `docs/plans/2026-02-22-layout-qa-pipeline-design.md` |
| E2E Coverage Expansion | `docs/plans/2026-02-18-e2e-coverage-expansion-design.md` |
| Core ECS Performance | `vendor/afterhours/docs/plans/2025-02-15-performance-optimization-design.md` |
| Dirty Flag Merge | `vendor/afterhours/docs/plans/2025-02-15-item-01-dirty-flag-merge.md` |
| Performance Brainstorm (350+ items) | `vendor/afterhours/docs/speed.md` |
| egui Comparison | `docs/egui-comparison.md` |
| Fabula + SwiftUI Comparison | `docs/fabula-comparison.md` |
| Focus Ring Audit | `docs/focus_ring_audit_consolidated.md` |
| Layout QA Results | `docs/layout_qa_todo.md` |
| Icon Registry | `docs/18_icon_registry.md` |
| Adaptive Scaling | `docs/30_adaptive_scaling.md` |
