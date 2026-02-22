# Nested Clip Container Scissor Fix

**Date:** 2026-02-22
**Status:** Implemented & verified

## Problem

Children with `Overflow::Hidden` inside a scroll view escape the scroll view's scissor clipping when scrolled. They render on top of other UI elements outside the scroll viewport.

**Root cause:** In `rendering.h`, the rendering path checks:

```cpp
bool is_clip_container = entity.has<HasScrollView>() || entity.has<HasClipChildren>();
bool needs_scissor = clip_ancestor.valid() && !is_clip_container;
```

If an entity has `HasClipChildren` (added by `Overflow::Hidden`), it's treated as a clip container and skips its parent's scissor entirely. This is correct for scroll views (they define their own viewport) but wrong for `HasClipChildren` entities nested inside scroll views.

## Solution

Replace the single-ancestor scissor lookup with `compute_intersected_clip_rect()`, which walks the **full** ancestor chain and intersects ALL clip rects from `HasScrollView` and `HasClipChildren` ancestors.

**Key changes:**
- Only `HasScrollView` entities skip parent scissoring (they define the viewport)
- `HasClipChildren` entities are now clipped by the intersection of all ancestor clip rects
- Auto-overflow scroll views that don't need scrolling are excluded from the intersection
- Both immediate-mode `render()` and buffered `collect()` paths updated

**Alternatives rejected:**
- (A) Only fix HasClipChildren-in-scroll-view — doesn't handle deeper nesting
- (C) Manually track scissor stack during render traversal — too invasive, render order is flat not recursive

## Files Changed

- `vendor/afterhours/src/plugins/ui/rendering.h` — Added `compute_intersected_clip_rect()`, updated `render()` and `collect()`
- `src/systems/screens/ScrollClipBugRepro.h` — 4-panel regression test screen
- `tests/e2e_scripts/54_scroll_clip_bug.e2e` — E2E test covering all 4 cases

## Test Cases

| Panel | Setup | Verifies |
|-------|-------|----------|
| 1 | Normal rows in scroll view | Baseline — no regression |
| 2 | `Overflow::Hidden` rows in scroll view | Original bug fix |
| 3 | `Overflow::Hidden` wrapper around rows in scroll view | Nested HasClipChildren → scroll |
| 4 | Scroll view inside scroll view | Nested scroll views |

All 4 panels clip identically when scrolled. Existing scroll view and scroll click e2e tests pass without regression.
