# afterhours layout issues found by the screen mock sweep

Generated from all 92 screens via `./mocks/build.sh --no-serve && ./mocks/analyze.py`.
Ranked by **how many screens a fix would touch**, which is what makes one worth
doing before another.

Every item below survived a manual check against the dump. That mattered more
than expected: **5 of the 7 automated checks were wrong on first run** and their
findings evaporated once the checker accounted for what the layout was actually
asked to do. Those are recorded in "Ruled out" so nobody re-files them.

| # | issue | screens | nodes |
|---|-------|--------:|------:|
| 1 | Children that do not fit are silently overflowed | 6 | 20 |
| 2 | Padding applied inconsistently to a container vs its children | 1 | 28 |
| 3 | `expand()` resolving to 0 on a separator | 1 | 1 |
| 4 | 8 screenshot baselines are stale (not a library bug) | 8 | — |

---

## 1. Children that do not fit are silently overflowed — 6 screens

**Screens:** `powerwash_settings` (9), `flex_alignment` (5), `layout` (3),
`deadspace_settings` (1), `meters_gauges` (1), `rubber_bandits_menu` (1)

When a container's resolved size is smaller than its children need, afterhours
lays the children out at their full size anyway and lets them hang outside the
parent box. It does not shrink them, does not wrap them, and does not warn.

Two representative cases:

- `powerwash_settings`, a `Row` of `Percent(1.0)` → 440px holding
  `170 + (20 margin + 44) + 170 + 44 = 448px` of fixed children. The last
  button ends 8px past the parent's right edge. Repeats on all 9 setting rows.
- `layout`'s `description_panel` is `Pixels(44)` with 14.4px padding top and
  bottom — 28.8px of padding around a `Pixels(32)` child, so 60.8px of content
  in a 44px box.

**Why it is worth researching rather than dismissing as caller error.** It is
caller error in each individual case, and probably the right fix for most of
these screens is to change the screen. But three separate mechanisms that
should have caught it did nothing:

- `Size::strictness` exists and is `1.0` on these children. If strictness is
  meant to gate shrinking, a strict child overflowing its parent is the case it
  exists to describe, and nothing consults it here.
- `flex_wrap` defaults to `Wrap`, and CSS flexbox would have wrapped
  `flex_alignment`'s three 26px chips inside their 49.8px content box. It does
  not wrap.
- `collect_layout_problems` (wm-side) already detects this shape and reports it
  for `flex_alignment` — but only that one, so its coverage does not match.

Decide which of shrink / wrap / warn is intended, then make it happen in one
place. Until then this failure is invisible at authoring time.

## 2. Padding applied inconsistently to a container vs its children — 1 screen, 28 nodes

**Screen:** `forms`

In `forms`, a child is positioned *up and to the left of its own parent's
origin* — outside the box on two sides at once, while comfortably fitting
inside it by size.

```
grandparent (unnamed Column)  x=653.4  padding left=25.6 top=14.4
  checkbox_row                x=704.6  = 653.4 + 25.6 + 25.6   <- 2x the padding
    "checkbox label"          x=691.8  = 653.4 + 12.8          <- 0.5x the padding
```

The child sits at exactly `-12.8, -7.2` from `checkbox_row`, which is precisely
**half the grandparent's padding, negated** — while `checkbox_row` itself is
offset by **twice** that padding. One padding value, applied at two different
scales one level apart.

Only one screen, but 28 nodes and a clean arithmetic signature, so it is likely
a single bug in how padding is folded into relative position. Worth doing early
despite the screen count: it is the most likely of these to be a genuine solver
defect rather than a screen over-specifying itself.

## 3. `expand()` resolving to 0 — 1 screen

**Screen:** `deadspace_settings`, node `sep_sidebar`, `x=Expand(1) -> 0px`.

A separator asking to expand got no width. This is the same *family* as the
absolute-subtree collapse fixed in afterhours `48f808d` (expand is only ever
resolved inside `solve_violations`, so anything that skips that pass collapses),
but `sep_sidebar` is not under an absolutely-positioned parent, so that fix does
not explain it. Some other path is skipping the expand pass. Small and isolated,
but the shared root cause makes it cheap to look at alongside anything else in
`solve_violations`.

## 4. Eight screenshot baselines are stale — 8 screens

Not a library bug, listed so it stops reading as one. `make validate-screenshots`
fails on `auto_text_color` (19.3%), `parcel_corps_settings` (2.2%), `forms`
(2.0%), `flex_alignment` (1.8%), `hstack_showcase` (1.4%), `circular_progress`
(1.3%), `cards` (1.2%), `islands_trains_settings` (1.0%).

These reproduce with **identical percentages at the old submodule pointer**
(`9594e20`), so they predate the afterhours bump and the split-pane work. Spot
checking `auto_text_color`, the *current* render is the correct one — the
baseline has content clipped off the right edge and its entire fourth section
missing. The baselines were captured before a layout fix and never refreshed.

11 screens have no baseline at all (`composer_lab`, `split_pane_lab`, the six
`dialog_*`, `hover_lab`, `multiline_text_lab`, `styled_text_lab`,
`text_input_lab`).

`make update-baselines` resolves both, but it should be a deliberate commit of
its own — it would also bake in whatever issues 1–3 currently produce.

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
