# Afterhours Library Gaps & Workarounds

wm_afterhours vendors afterhours (git submodule) **and** maintains it, so gaps
found during UI work get fixed upstream, merged, and pulled in via a submodule
pin bump — not worked around in wm source. This file tracks only what's still
**open**; resolved gaps are listed compactly for the record (full detail lives in
git history and the afterhours PRs).

See also: `docs/vendor_ui_sizing_issues.md`

---

## Known limitations (open, low priority)

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
