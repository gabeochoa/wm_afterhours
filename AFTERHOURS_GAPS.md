# Afterhours Library Gaps & Workarounds

Issues encountered in the `vendor/afterhours` library during UI work. We do NOT modify vendor code — instead we document gaps here and add workarounds in our source.

---

## Format

Each entry should include:
- **Issue**: What's wrong or missing
- **Affected screens**: Which screens hit this
- **Workaround**: What we did instead (with file + line references)
- **Ideal fix**: What afterhours should provide

---

## See Also

- `docs/vendor_ui_sizing_issues.md` — additional verified vendor bugs (dropdown sizing, toggle font size, slider handle overflow) with debug logging details and applied fixes

---

## Gaps

### 1. ~~tab_container tab strip renders outside parent bounds~~ (FIXED)

- **Issue**: When `tab_container()` is used with `with_absolute_position()`, the tab strip (the row of tab buttons) renders at the top of the screen instead of relative to the container's position. Moving the container's y-position has no effect on the tab strip position.
- **Root cause**: `with_absolute_position(x,y)` stored the position as a translate in `HasUIModifiers` (render-time only), but `compute_rect_bounds` used `parent.computed_rel` (which was 0,0) for child offsets.
- **Fix**: Added `absolute_pos_x/y` fields to `UIComponent`, set during `apply_visuals` in component_init.h, used in `compute_relative_positions` in autolayout.h.

### 2. ~~toggle_switch creates sibling entities that consume parent layout space~~ (NOT A BUG)

- **Original report**: `toggle_switch()` creates internal entities as siblings in the parent's entity namespace, consuming more vertical space than declared.
- **Investigation**: The internal entities (label, track, knob) ARE children of the toggle entity, not siblings. `mk(entity)` correctly creates them as children. The ToggleSwitchShowcase overflow was caused by the total content (title + sections + separators + toggles + checkboxes + disabled section) exceeding the card height, not by individual toggles taking extra space. A repro with 3 toggles at 42px in a 150px container confirmed they fit correctly.

### 3. Focus ring visibility on checkbox and toggle_switch (unverified)

- **Issue**: The UI review flagged that checkboxes and toggle switches don't show focus rings when tabbed to, while buttons do. The afterhours rendering code has focus ring support for multiple element types, so the rings may actually render — this could not be verified via static screenshots (no element is focused during headless capture).
- **Affected screens**: `CheckboxShowcase.h`, `ToggleSwitchShowcase.h`
- **Workaround**: None needed if focus rings render correctly. Needs manual verification with keyboard tabbing.
- **Ideal fix**: If focus rings don't render on checkbox/toggle, the rendering pipeline should treat them the same as buttons for focus ring drawing.

---

## Session 2026-07-18 — visual audit of all 79 example screens

Found via adversarial screenshot review of `make screenshots`. Several were
root-caused and fixed directly in `vendor/afterhours` on branch
`ui-layout-fixes` (pushed to origin). Listed here so upstream maintainers can
review / adopt. **These are real library bugs, not just demo-code issues.**

**Regression tests**: added as normal, permanent `examples/*_test.cpp` suites
(same harness as `autolayout_test.cpp`; imm widgets use the shared
`examples/ui_test_harness.h`). Each asserts the *correct* behavior, so it fails
on a pristine (buggy) tree and passes once fixed:
- `examples/render_order_test.cpp` — #4 render-command ordering
- `examples/progress_bar_test.cpp` — #5, #11 progress_bar sizing
- `examples/slider_test.cpp` — #6 slider handle position
- `examples/stepper_test.cpp` — #7 multi-visible label separation
- `examples/tab_container_test.cpp` — #8 content-fit tab widths
- `examples/autolayout_test.cpp` (`gap_children_sizing_*`) — #12 children()+gap

Build & run any suite (from `vendor/afterhours`):
```
clang++ -std=c++23 -I.. -Ivendor examples/progress_bar_test.cpp -o /tmp/t && /tmp/t
```


### 4. Render-command sort tiebreaks on recycled entity id (SEVERE) — FIXED (branch `fix-render-command-sort`)

- **Issue**: In `RenderImm` and `RenderBatched` (`src/plugins/ui/rendering.h`), render commands were sorted by `(layer, entity.id)`. Entity IDs are **recycled** across screens/frames, so within a layer the id order is not the document (parent-before-child) order. An opaque-background ancestor that receives a *higher* recycled id than its own children sorts after them and **paints over its own children**, hiding titles, labels, first-row controls, and whole sections.
- **Symptom**: Manifested as three seemingly-unrelated bugs — the `modals` screen rendering as scattered widgets with no dialog; radio/toggle/stepper groups missing the first item's control; and screenshot output changing across rebuilds (because recycled-id order shifts with allocation order). All the same bug.
- **Root cause**: `render_cmds` is already queued in correct document pre-order; the id tiebreak destroyed that order within a layer.
- **Fix**: Stable-sort by `layer` only (`std::ranges::stable_sort` / stable bubble sort on Win32), preserving queue order within a layer. Explicit `render_layer` still governs true z-order (dropdowns, modals, focus rings), so overlays are unaffected.
- **Impact**: This is the single highest-value fix — it affected 76 of 79 screens. Strongly recommend upstreaming.

### 5. `progress_bar` fill/label compound percent sizing against the track — FIXED (branch `ui-layout-fixes`)

- **Issue**: In `progress_bar` (`src/plugins/ui/imm_components.h`), the fill and label are children of the track and absolutely positioned, but were sized with `config.size` (the *track's* size relative to *its* parent). A percent size therefore resolved against the track and **compounded** the fraction: a `percent(0.7)` height became 0.7 × 0.7 of the meter, and the fill width became `0.75 × value` instead of `value`. Result: fill is shorter/narrower than the track and visually offset from it.
- **Affected**: any `progress_bar` sized with percent dimensions (e.g. `MetersGaugesShowcase` inline meters).
- **Fix**: Size the fill as `percent(normalized) × percent(1.0)` and the label as `percent(1.0) × percent(1.0)` so both track the actual track box. Pixel-sized bars are unaffected.
- **Ideal fix**: (adopted) child overlays of a widget should size relative to the widget box, never re-apply the widget's own outer size.

### 6. `slider` handle position wrong on percent-sized tracks — FIXED (branch `ui-layout-fixes`)

- **Issue**: In `slider` (`src/plugins/ui/imm_components.h`), the handle's left offset was `owned_value * 0.75 * track_val`. The handle is a child of the track, so for a Percent/ScreenPercent track a percent margin already resolves against the track width — multiplying by `track_val` again double-applied the fraction and pinned the knob near the start (an 80% slider showed the handle at ~30%).
- **Affected**: `setting_row_showcase`, `forms`, `themes`, any percent-width slider.
- **Fix**: Only multiply by `track_val` for Pixels dims (where the fraction must be converted to pixels); for percent dims use the raw fraction. Same root cause family as #5.
- **Note**: The handle range still uses a `0.75` compression factor so the knob's center never quite reaches the value position at 100%. Not fixed (cosmetic); upstream may want to revisit the handle-width/position model.

### 7. `stepper` with `num_visible > 1` renders labels with no separation — FIXED (branch `ui-layout-fixes`)

- **Issue**: A multi-visible stepper (prev/current/next) put its labels in a `children()`-sized container with `JustifyContent::SpaceAround`. Because the container shrinks to content, SpaceAround has no free space to distribute, so the labels butt together ("Healer"+"Warrior"+"Mage" → "HealerWarriorMage").
- **Affected**: `StepperShowcase` card selector.
- **Fix**: Add a gap (12px) and `no_wrap` when `num_visible > 1`; `num_visible == 1` keeps a 0px gap so single-value steppers are byte-identical.

### 8. `tab_container` forces equal 1/N tab widths → long labels ellipsize — FIXED (branch `ui-layout-fixes`)

- **Issue**: `tab_container` gave every tab `percent(1/N)` width. When some labels are longer than an equal slice, they ellipsize even if the bar has room and shorter tabs have slack (e.g. "GENERAL"/"GAMEPLAY" truncate next to "VIDEO"/"AUDIO").
- **Affected**: `powerwash_settings`, `sports_settings`, `mini_motorways_settings`, `flight_options`, `kirby_options`.
- **Fix**: Tabs now use `expand()` width (even distribution — identical look for short labels) with a per-tab `min_width` of `Dim::Text` (the tab's own measured label), so a tab never shrinks below its content. Also added `Dim::Text` support to `resolve_constraint()` in `autolayout.h` so min/max-by-text works generally.
- **Remaining**: very crowded bars (many long labels summing past the bar width — e.g. flight_options' 9 tabs at the default font) still need a smaller font at the call site; content-fit only removes truncation when the labels *can* fit.

### 9. No word-wrap for static labels (FEATURE GAP)

- **Issue**: `with_word_wrap()` exists only for text-area (input) components. Static `div`/label text renders on a single line and clips or overflows its box; there is no way to wrap a long label to multiple lines.
- **Affected**: `nine_slice_borders` dialog, `example_borders` card labels, `stepper_showcase`/`fighter_menu` descriptions — all worked around by shortening text or shrinking font.
- **Ideal fix**: support word-wrap on any labeled component sized with a bounded width + `children()`/fixed height, wrapping to N lines. This would remove a whole class of per-screen "shorten the text" workarounds.

### 10. Batch/headless renderer omits `systems.run()` ordering (TOOLING, not library)

- **Issue**: `src/headless_screenshots.cpp` drove each screen with a manual `tick_all()` + `render()` split and only 2 passes, instead of `systems.run()`. Combined with #4 (recycled ids), this made screenshot baselines non-deterministic across rebuilds. Documented here only because it interacts with the render-order bug; the real fix was #4. If upstream ships a headless capture helper, it should use `systems.run()` and settle to convergence.

### 11. `progress_bar` track compounds percent size against the bar entity — FIXED (branch `ui-layout-fixes`)

- **Issue**: Discovered while writing the repro test for #5. `progress_bar` creates its `progress_track` as a child of the `progress_bar` entity, but sized the track with `config.size` — the same size the entity already has. A percent size therefore compounds: a `progress_bar` sized `percent(0.7)` tall gets a track that is `0.7 * 0.7` = 49% of the parent, not 70%. (This is distinct from #5, which was the fill/label compounding against the track.)
- **Affected**: any `progress_bar` sized with percent dimensions.
- **Fix**: the track now fills the entity with `percent(1.0) x percent(1.0)`; the fill/label (see #5) then fill the track. Pixel-sized bars unaffected.
- **Repro**: `progress_bar_test.cpp::progress_bar_percent_sizing_fills_track` (asserts `track.height == 70` and `fill.height == track.height`).

### 12. `Dim::Children` sizing ignores `flex_gap` — FIXED (branch `ui-layout-fixes`)

- **Issue**: Discovered while writing the repro test for #7. A container sized with `children()` sums its children's sizes but did **not** add the `flex_gap`. So a `children()`-sized flex container with a gap is too narrow — the gap pushes the children apart and they overflow/overlap the parent. This is why the stepper (#7) label separation didn't "take" from the gap alone.
- **Affected**: any `children()`-sized container that also sets a gap (e.g. `stepper` multi-visible label row).
- **Fix**: `_sum_children_axis_for_child_exp` in `autolayout.h` now adds `gap * (visible_children - 1)` on the main axis.
- **Repro**: `stepper_test.cpp::stepper_multi_visible_labels_separated` (asserts the label container is wider than the sum of its labels).



