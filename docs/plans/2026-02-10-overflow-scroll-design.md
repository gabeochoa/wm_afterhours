# Overflow & Scroll Unification Design

## Problem

Scrolling requires a dedicated `scroll_view()` widget and clipping requires
adding `HasClipChildren`. These are separate concepts that can't compose with
other widgets like `drag_group()`. A scrollable drag group requires nesting
widgets in a specific order and the drag system doesn't account for scroll
offsets during hit testing.

## Goal

Make overflow behavior (visible, hidden, scroll) a property of any box via
`ComponentConfig`. Remove the special-case widgets. Make the drag system
scroll-aware so `drag_group(..., config.with_overflow(Overflow::Scroll, Axis::Y))`
just works.

## API

```cpp
enum class Overflow { Visible, Hidden, Scroll };

// Axis::Both is new; Axis::X and Axis::Y already exist.
config.with_overflow(Overflow::Scroll, Axis::Y)     // scroll vertical only
config.with_overflow(Overflow::Scroll, Axis::Both)   // scroll both axes
config.with_overflow(Overflow::Hidden, Axis::X)      // clip horizontal only
config.with_overflow(Overflow::Hidden, Axis::Both)   // clip both
```

`ComponentConfig` stores `overflow_x` and `overflow_y`, each defaulting to
`Overflow::Visible`. `Axis::Both` sets both fields. `_init_component` reads
these and adds/configures `HasScrollView` (for `Scroll`) or `HasClipChildren`
(for `Hidden`).

## Architecture

Three layers:

### Config layer

`ComponentConfig::with_overflow(Overflow, Axis)` stores `overflow_x` and
`overflow_y`. `_init_component` translates these into ECS components:
- `Scroll` on either axis -> add `HasScrollView` with the corresponding
  `vertical_enabled` / `horizontal_enabled` flags.
- `Hidden` on either axis -> add `HasClipChildren` (or fold into a unified
  component later).

### Input system

A new `HandleScrollInput<InputAction>` system replaces the inline mouse wheel
handling currently in `scroll_view()` (imm_components.h lines 2068-2101). It
runs on all entities with `HasScrollView`, checks if the mouse is inside the
entity's rect, and applies wheel deltas to `scroll_offset`. Registered in
`UIPluginPostUpdateBridge` alongside other input handlers.

### Drag system awareness

`HandleDragGroupsPostLayout` reads `child_cmp.rect()` for hit testing. When a
drag group entity has `HasScrollView`, the system subtracts `scroll_offset`
from layout-computed rects before comparing against mouse position. A helper
like `scroll_adjusted_rect(entity)` centralizes this — the same offset the
renderer already applies.

### Data flow

1. Immediate mode: `div(ctx, mk(...), config.with_overflow(Scroll, Y))` ->
   `_init_component` adds `HasScrollView{vertical=true}`
2. `HandleScrollInput` reads wheel input, updates `scroll_offset`
3. Layout runs (unaware of scroll)
4. Drag system hit-tests using scroll-adjusted rects
5. Renderer applies `scroll_offset` + scissor clipping

## Migration Plan

Five steps. Each step is independently committable and testable — all existing
E2E tests must pass before committing.

### Step 1: Extract scroll input into a system

Move the mouse wheel handling out of `scroll_view()` into a new
`HandleScrollInput<InputAction>` system. Register it in
`UIPluginPostUpdateBridge`. `scroll_view()` still adds `HasScrollView` — the
system picks it up automatically. All existing scroll_view screens behave
identically.

**Test:** Run all E2E tests + manually verify scroll screens.
**Commit.**

### Step 2: Add `with_overflow(Overflow, Axis)` to ComponentConfig

Add `Overflow` enum, `Axis::Both`, and `overflow_x` / `overflow_y` fields to
`ComponentConfig`. Update `_init_component` to add `HasScrollView` or
`HasClipChildren` based on these values. Update `scroll_view()` internally to
use `config.with_overflow(Overflow::Scroll, ...)` instead of manually adding
`HasScrollView`. `scroll_view()` still exists as a function but is now a thin
wrapper around `div()` + config.

**Test:** Run all E2E tests + manually verify scroll screens.
**Commit.**

### Step 3: Migrate callers

Replace `scroll_view(ctx, ep, config)` calls in all screens with
`div(ctx, ep, config.with_overflow(Overflow::Scroll, Axis::Y))` (or whichever
axis). Replace direct `HasClipChildren` usage with
`config.with_overflow(Overflow::Hidden, ...)`. This is a rename-only step —
no behavior change.

**Test:** Run all E2E tests.
**Commit.**

### Step 4: Delete `scroll_view()` and clean up

Remove the `scroll_view()` function. Remove `HasClipChildren` if fully
subsumed by the overflow config. Remove any dead code.

**Test:** Build succeeds, all E2E tests pass.
**Commit.**

### Step 5: Add tests

E2E tests verifying:
- `Overflow::Hidden` clips content (children outside bounds not visible).
- `Overflow::Scroll` allows wheel input and scrolls content.
- `drag_group` with `Overflow::Scroll` supports drag reorder in a scrollable
  list.

**Test:** New tests pass.
**Commit.**

## Risks & Open Questions

### Scroll-adjusted hit testing

The drag system and potentially other systems (dropdowns, tooltips) use
`rect()` which returns layout-space positions. `scroll_adjusted_rect()` fixes
it for the drag system, but other hit-test call sites may also need
adjustment. Out of scope for this plan but worth auditing.

### `Axis::Both` addition

The existing `Axis` enum has `X`, `Y`, `left`, `right`, `top`, `bottom`.
Adding `Both` could affect switch statements that enumerate `Axis` values.
Audit exhaustive switches before merging.

### Component overlap during migration

During steps 2-3, both old and new code paths coexist. `_init_component` must
not double-add `HasScrollView` if `scroll_view()` also adds it. The existing
`addComponentIfMissing` pattern handles this.

### Drag-near-edge auto-scroll (future)

Not in scope. The `HandleScrollInput` system from step 1 sets the foundation.
A future system can check "is a drag active and is the mouse near the edge of
a scrollable ancestor?" and nudge `scroll_offset` accordingly.
