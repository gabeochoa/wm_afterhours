# Afterhours Library Gaps & Workarounds

wm_afterhours vendors afterhours (git submodule) **and** maintains it, so gaps
found during UI work get fixed upstream, merged, and pulled in via a submodule
pin bump — not worked around in wm source. This file tracks only what's still
**open**; resolved gaps are listed compactly for the record (full detail lives in
git history and the afterhours PRs).

See also: `docs/vendor_ui_sizing_issues.md`

---

## Blocking another project's bump

Two regressions from this repo's own work stop floatinghotel bumping past
`0c67090`. Both were diagnosed there with a verified fix; wm cannot see either
because it is raylib and its baselines happen not to land on the bad case.

- **sokol backend does not compile since `b3f8cef`** — `backend.h`'s
  `MetalPlatformAPI::get_mouse_position` calls `window_manager::window_to_content`,
  but `window_manager.h` includes `graphics.h` (and so `backend.h`) before it
  declares that struct. Every sokol/Metal build fails; wm is raylib so upstream
  never sees it. The fix is what the raylib branch already does: leave the
  backend at the DPI divide and apply the letterbox in `input_system.h`'s Metal
  branch, which can see `window_manager`.

- **flex solver budgets raw child sizes while placement uses snapped ones**
  (since `7a56f60`) — placement reads `snapped_extent`, but `_total_child`,
  `_max_child` (`autolayout.h` ~1040) and the justify-content pass (~1412) still
  sum `child.computed`. A row of eight `w1280()` buttons each round up a pixel,
  so an `expand()` spacer is handed pre-snap slack and the last button lands
  15px past its row. Same cause as the `FlexEnd` row starting 11px too far
  right. floatinghotel measured 0 → 25 overflow warnings against `0c67090`, back
  to 0 with the three sites summing `snapped_extent`. A regression test is
  written and sitting in their doc, ready for `sizing_repro_test.cpp`.

This is the parent-measures-child-before-child-snaps root cause again, in the
one pass that was missed.

## Open, asked for by other projects

Collected from every gap doc across the 19 projects that vendor afterhours,
checked against current `main` rather than the pin each is stuck on. Nine of
wordproc's fifteen, all ten of break-ross's, four of kart's and one of
cartographer's turned out to be already in; these are what is left.

### Components

- **access-key underlines** (wordproc) — per-character decoration, so `&File`
  can underline the F. Needs the renderer to decorate one glyph in a run, which
  the run machinery from the wrap work could carry. Filed as accessibility.

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

### Harness

- **`--headless` is parsed but unreachable** (endless-dance-chaos) — the flag
  is read and a working `RaylibHeadless` backend exists, but `RunConfig` has no
  `display` field and `run()` calls `init_window()` unconditionally. So the flag
  silently does nothing and every run opens a window. They measured the headless
  path at 11.7x and had to work around it with their own `--no-render`. The fix
  is an additive `DisplayMode display` on `RunConfig`.

- **no fixed timestep** (endless-dance-chaos) — the sim is framerate-dependent,
  so a bot playtest is not reproducible run to run. They added ~8 lines in their
  own `main.cpp`; every consumer wanting a deterministic harness will.

- **`wait` resolution is quantised by the substep batch**
  (endless-dance-chaos, minor) — documented there, no workaround needed beyond
  keeping `sim_steps` small.

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

- **tooltip** — `plugins/ui/tooltip.h`, with `TooltipLab` and a baseline
- **table / grid layout** — `plugins/ui/grid.h`; `RaceResults` and
  `MinesweeperLab` converted to it, plus `GridLab`
- **scrollbar colour and style** — track/thumb usages and explicit colour
  overrides on `HasScrollView`, with `ScrollbarStyleLab`
- **singleton-only systems skip the entity scan** — a system whose components
  are all registered singletons resolves through the singleton map instead of
  walking every entity (puzzle profiled that scan at 37% of a frame)
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
