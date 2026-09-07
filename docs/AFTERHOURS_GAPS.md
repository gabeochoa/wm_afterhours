# Afterhours Library Gaps & Workarounds

wm_afterhours vendors afterhours (git submodule) **and** maintains it, so gaps
found during UI work get fixed upstream, merged, and pulled in via a submodule
pin bump — not worked around in wm source. This file tracks only what's still
**open**; resolved gaps are listed compactly for the record (full detail lives in
git history and the afterhours PRs).

See also: `docs/vendor_ui_sizing_issues.md`

---

## Open, asked for by other projects

Collected from every gap doc across the 19 projects that vendor afterhours,
checked against current `main` rather than the pin each is stuck on. Nine of
wordproc's fifteen, all ten of break-ross's, four of kart's and one of
cartographer's turned out to be already in; these are what is left.

### Components

- **tooltip** (wordproc) — nothing in the library. Every consumer that wants
  hover text builds a popover by hand and positions it themselves.

- **table / grid layout** (wordproc) — no `table()` or `grid()`. Column
  alignment across rows is done today by giving every cell the same fixed
  width, which is what the resolution work above keeps finding broken.

- **access-key underlines** (wordproc) — per-character decoration, so `&File`
  can underline the F. Needs the renderer to decorate one glyph in a run, which
  the run machinery from the wrap work could carry. Filed as accessibility.

- **scrollbar colour and style** (wordproc) — `show_scrollbar` and
  `scrollbar_thickness` exist and nothing else, so a scrollbar cannot be made
  to match a theme.

### Input

- **mouse delta through the action mapping** (cartographer) — `GetMouseDelta()`
  for camera look cannot go through actions, since it is a continuous 2D delta
  rather than a button or an axis with a direction. So mouse sensitivity and
  rebinding live outside the system every other input goes through.

- **`synthetic_press_delay` is undocumented and load-bearing** (cartographer) —
  a 1-frame delay in `consume_press` before an injected key registers. Tests
  need a `wait` between `key_down` and anything depending on it, and nothing
  says so.

### E2E

- **command handlers are registered per SystemManager** (cartographer) — with
  several managers (game, pause, transition) each needs `HandlePressKeyCommand`
  and friends registered by hand. Miss one and commands silently do nothing
  while that manager is active. A global registry, or propagation, would end a
  class of bug rather than a bug.

### Diagnostics

- **no lint for custom colours bypassing the theme** (cartographer) — theme
  usage is easy to skip by accident and nothing catches it in shared UI.

### Core

- **EntityQuery has no index** (MyNameChef, puzzle) — every terminal is a
  linear scan. puzzle hit the quadratic case on a five-node graph and
  hand-wrote an index; their point was not speed but that an indexed
  relationship "cannot be half-written in the first place", after a node-delete
  sweep reaped ports and wires and forgot knobs and sliders. There is a written
  plan for this.

- **kart's remaining component extractions** — `HasLabels`, `CanWrapAround`,
  `TeamID`, `ManagesAvailableColors`. Arguably game-specific; listed for
  completeness, not recommended.

---

## Known limitations (open, low priority)

- **tab order is allocation order, not tree order** — `process_tabbing` moves
  focus by setting it to `ROOT` and letting the next entity the iteration
  reaches grab it, so Tab follows the entity collection's storage order rather
  than the widget tree. Two widgets side by side tab in whatever order they
  were allocated, which is build order today and need not stay that way; there
  is no tab index and no way for a caller to state the order it wants.
  Order-preserving `cleanup()` (`58e1613`) stops widget retirement from
  reshuffling it, which **hides this rather than fixing it** — the order is
  stable now, but it is still the wrong order to be deriving from. A real fix
  sorts focusables by tree position, with an explicit override.

- **a virtualized list still builds a real div for the rows above the fold** —
  `virtual_list` folds the rows *below* the window straight into
  `content_size` via `HasScrollView::unbuilt_content_size`, but the rows above
  it are a `vlist_skipped_above` div of the right height. It cannot be folded
  the same way because the scroll offset is applied when drawing rather than
  when laying out: with nothing in front of it the first built row lays out at
  the top of the content and is then drawn off screen. Folding it needs
  children offset during the layout pass. One div, so the cost is negligible;
  it is the asymmetry that will confuse the next reader.

- **`virtual_list` requires a uniform row height** — finding the window and
  the height of the skipped rows is `offset / row_height`, which is why it is
  cheap. Variable heights need a prefix sum over every item to answer either
  question, which is O(n) per frame and gives back exactly what the
  virtualization was for. Doing it properly means a cumulative-height table
  cached on the component and invalidated when an item resizes. Until then a
  list of mixed-height rows has to pick a single pitch or not virtualize.

- **slider handle 0.75 compression** — the knob's center never quite reaches the
  value position at 100% (cosmetic). `imm_components.h` `slider`. Revisit the
  handle width/position model only if it becomes visible.
- **crowded tab bars still need a smaller font at the call site** — `tab_container`
  content-fit (`expand()` + `min_width = Dim::Text`) removes truncation only when
  the labels *can* fit. A bar with many long labels (e.g. `flight_options`' 9 tabs
  at the default font) still overflows; set a smaller font on the tab_container
  config for those.
- **word-wrap has no hard character break** — with `TextOverflow::Wrap`, a single
  word wider than its box goes on its own line (not split), and wrap requires an
  explicit font size (auto-fit + wrap is ambiguous).

Batch/headless screenshot determinism is a wm-side tooling concern, not an
afterhours gap — tracked in `docs/LAYOUT_AUDIT.md`.

---

## Resolved & upstreamed (merged into afterhours main, pinned `e348efb`)

- tab_container tab strip bounds under `with_absolute_position`
- **render-command sort recycled-id tiebreak** (SEVERE — fixed 76/79 screens;
  root cause behind the modals breakage, first-child-missing-control, and
  cross-rebuild screenshot non-determinism)
- progress_bar fill/label percent compounding, and track percent compounding
- slider handle position on percent-sized tracks
- stepper multi-visible label separation
- tab_container equal-width → long-label ellipsize (now `expand()` + `Dim::Text`
  min width; added `Dim::Text` to `resolve_constraint`)
- static-label word-wrap — new `TextOverflow::Wrap` feature
- `children()` sizing now includes `flex_gap`
- checkbox/toggle focus ring — confirmed **handled**, not a bug (checkbox/toggle
  are focus clusters via `FocusClusterRoot`/`InFocusCluster`; the ring render is
  type-agnostic with an explicit cluster branch)
- toggle_switch "sibling entities consume layout space" — **not a bug** (internal
  entities are children, not siblings)

Regression tests for these live in the afterhours `tests/` suite: `autolayout_test`,
`progress_bar_test`, `slider_test`, `stepper_test`, `tab_container_test`,
`text_wrap_test`, `render_order_test`.
