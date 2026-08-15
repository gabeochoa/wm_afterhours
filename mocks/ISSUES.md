# afterhours layout issues found by the screen mock sweep

Generated from all 92 screens via `./mocks/build.sh --no-serve && ./mocks/analyze.py`.
Ranked by **how many screens a fix would touch**, which is what makes one worth
doing before another.

Every item below survived a manual check against the dump. That mattered more
than expected: **5 of the 7 automated checks were wrong on first run** and their
findings evaporated once the checker accounted for what the layout was actually
asked to do. Those are recorded in "Ruled out" so nobody re-files them.

| # | issue | screens | nodes | status |
|---|-------|--------:|------:|---|
| 1 | Children that do not fit are silently overflowed | 6 | 21 | open |
| 2 | Scroll-view children positioned one margin off | 1 | 27 | **fixed** |
| 3 | `expand()` resolving to 0 on the cross axis | 1 | 1 | **fixed** |
| 4 | Screenshot baselines were stale (not a library bug) | 19 | — | **fixed** |

---

## 1. Children that do not fit are silently overflowed — 6 screens, 21 nodes

**Screens:** `powerwash_settings` (9), `flex_alignment` (5), `layout` (3),
`deadspace_settings` (1), `meters_gauges` (1), `rubber_bandits_menu` (1),
`forms` (1)

When a container's resolved size is smaller than its children need, afterhours
lays the children out at their full size anyway and lets them hang outside the
parent box. It does not shrink them and does not wrap them.

Two representative cases:

- `powerwash_settings`, a `Row` of `Percent(1.0)` → 440px holding
  `170 + (20 margin + 44) + 170 + 44 = 448px` of fixed children. The last
  button ends 8px past the parent's right edge. Repeats on all 9 setting rows.
- `layout`'s `description_panel` is `Pixels(44)` with 14.4px padding top and
  bottom — 28.8px of padding around a `Pixels(32)` child, so 60.8px of content
  in a 44px box.

**This is caller error in every one of these cases**, and the right fix is to
change the screens. The library question is only whether it should be *visible*
at authoring time, and there the picture is narrower than it first looked:

- `Size::strictness` is `1.0` on all of these children, and `strictness == 1`
  means "do not shrink me". `compute_error` honours that: it only redistributes
  among children with `strictness < 1`. Working as designed.
- `flex_wrap` is **NoWrap** on 3860 of the 3863 nodes in the sweep, because
  NoWrap is the default (`UIComponent` and `ComponentConfig` both start there),
  not `Wrap`. The `layout_types.h` comment claimed the opposite; corrected.
- What is left is that the overflow warning does not fire. Its tolerance is
  `sx + child_margin + 4px`, and `powerwash_settings` overflows by 8px with a
  20px margin, so the margin allowance swallows it. That allowance was added
  deliberately (a `percent(1)` child plus margin overflows by exactly its
  margin under the content-box model, which the caller cannot act on), so
  tightening it needs a way to tell that idiom apart from real overflow rather
  than just a smaller number.

Not urgent, and not a solver bug. Left open as a diagnostics question.

## 2. Scroll-view children positioned one margin off — FIXED

**Screen:** `forms` (27 of its 28 flagged nodes; the 28th is issue 1).

`computed_rel` is the *pre-margin* origin — `UIComponent::rect()` adds
`margin_left/top` back on. The scroll-view repositioning pass added the margin
itself as well, so every margined direct child of a scroll view got its box
placed one margin down and right of the widgets inside it. In `forms` that drew
each checkbox row as a visible staircase: the row's fill offset from its own
label and checkmark, and the rows spilling past the panel's right edge.

The reason it survived this long is that the pass existed **twice**, once in
`systems.h` (tick, for hit-testing) and once in `rendering.h` (render), and the
copies had drifted: only one knew about `gap`, only one propagated the shift to
grandchildren, and whichever ran last silently won. Fixing one copy alone made
things worse, not better — the surviving copy's delta then dragged the children
off in the other direction.

Fixed by deleting the render-side copy and keeping one
`detail::reposition_scroll_view_children` in `systems.h` that both call, with
the margin double-count removed. Verified against `forms_720p.png` (staircase
gone) and `scroll_click_bug_720p.png` (rows no longer double-offset).

## 3. `expand()` resolving to 0 on the cross axis — FIXED

**Screen:** `deadspace_settings`, node `sep_sidebar`, `x=Expand(1) -> 0px`.

Both axes went through `tax_refund`, which resolves expand as *a share of the
leftover space*. That is right on the main axis, where children stack, and
wrong on the cross axis, where they overlap: `sep_sidebar`'s `percent(1)`
siblings had already claimed the sidebar's full 240px, so the leftover was 0
and the separator got nothing.

Cross-axis `expand()` now means stretch — the whole content box minus the
child's own margins — which is what `align-self: stretch` does in CSS. The
separator under "PAUSE MENU" renders again. Regression test:
`cross_axis_expand_stretches` in `tests/autolayout_test.cpp`.

## 4. Screenshot baselines were stale — FIXED

Not a library bug, listed because it read as one. `make validate-screenshots`
was failing on 8 screens and 11 more had no baseline at all.

The 8 reproduced with **identical percentages at the old submodule pointer**
(`9594e20`), so they predated the afterhours bump and the split-pane work. Spot
checking `auto_text_color`, the *current* render was the correct one — the
baseline had content clipped off the right edge and its entire fourth section
missing. Refreshed in its own commit after per-screen visual review, then
refreshed again for the four screens issues 2 and 3 changed
(`forms`, `deadspace_settings`, `scroll_click_bug`, `scroll_view`).

Now 92 passed, 0 failed, 0 missing.

---

## Ruled out

Each of these was a bug in `analyze.py`, not in afterhours. Kept so the same
false leads do not get re-filed.

| reported | screens | why it was wrong |
|---|--:|---|
| `justify-content-ignored` | 12 | The checker counted child margins and gap as free slack. The dialog button rows are flush right to the pixel; `FlexEnd` works. |
| `laid-out-offscreen` | 4 | All of it was content inside scroll views. Scrolled-away content is supposed to be outside the viewport. |
| `child-escapes-parent` (scroll) | 3 | `scroll_view`, `scroll_click_bug`, `scroll_clip_bug` — a clipping container is meant to hold oversized children. |
| `flow-siblings-overlap` | 4 | Toggle knobs and their labels use a deliberate `margin.top: -20` to pull one over the other. Overlap is the request. |
| `gap-not-applied` | 1 | `toasts` sets `gap: 8` *and* `margin.right: 8` per child. 16px is correct — gap stacks with margins in afterhours as in CSS. |

The general lesson for the checker: a raw geometry assertion has to subtract
everything the caller deliberately asked for — margins (including negative
ones), gap, padding, and clipping — before a difference means anything.
