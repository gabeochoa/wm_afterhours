# Layout QA: pseudo_locale_lab

**Screenshots analyzed:** 19 (Off / DoubleWords / RtlWords × 1280×720, 900×600,
1600×900, plus scrolled feed and composer-post + confirm states at 1280×720;
control captures: buttons screen at 900×600, lab at 1152×648, 1024×576, 640×360)
**Mode:** game (immediate-mode UI)

## Issues Found

### 1. Glyphs fragment and fade at sub-1.0 UI scale
**Type:** overflow (rendering)
**Severity:** CRITICAL
**Screenshot:** plqa_lab_640.png, plqa_lab_1024.png, plqa_buttons_900.png
**Detail:** At scale 1.0 and 1.25 text is clean. At 0.9 small text (14px and
under) starts to break up; at ~0.7 sidebar rows and post bodies are sliced
mid-glyph; at 0.5 some labels are faint fragments (`Friends`, `Memories`
barely render). Reproduces on the unrelated buttons screen at 900×600, so it
is a global font-rendering issue, not this screen and not pseudo-locale.
**Suggested fix:** Library/backend investigation — fonts rasterize at 96px
(`src/font_config.h`) and are drawn far below that via the raylib atlas path.
Recorded in docs/AFTERHOURS_GAPS.md; not fixed in this pass.

### 2. RTL button labels hugged the right edge and clipped
**Type:** overflow
**Severity:** HIGH — fixed in this pass
**Screenshot:** plqa_rtl_1280.png (before), recapture (after)
**Detail:** In RtlWords mode every unpinned label defaulted to Right,
including buttons. `Confirm` lost its final `m`, `Post` its `t`, `Home` and
`Gabe` spilled past their backgrounds (~4–6px, estimated from captures).
A centred control label is centred in any direction; only Div defaults
should flip. Fixed in vendor `component_init.h` (`overwrite_defaults`),
test `rtl_button_labels_stay_centred`; after-fix capture shows all button
labels centred in RTL.

### 3. User data transforms once it becomes a label
**Type:** overlap / overflow
**Severity:** MEDIUM
**Screenshot:** plqa_double_posted.png, plqa_double_1280.png
**Detail:** A posted composer message renders doubled/reversed like UI copy.
Avatar initials double (`GO GO`, `MC M…`) and spill out of their circles;
the alerts badge `3` becomes `3 3`. There is no per-label opt-out from the
pseudo transform (field values are exempt; labels are not). Library gap,
recorded in docs/AFTERHOURS_GAPS.md.

### 4. "Your shortcuts" heading indented 12px from its rows
**Type:** bounds (alignment)
**Severity:** LOW — fixed in this pass
**Screenshot:** plqa_off_1280.png
**Detail:** Measured with pixel scan: sidebar rows start at x=49, the heading
at x=61 — exactly the 12px row padding. The heading is parented to the root,
the rows to the sidebar. Demo fix: heading x 44 → 32 in PseudoLocaleLab.h;
recapture shows one left edge.

### 5. Doubled "Request confirmed" wraps into the next request row's space
**Type:** overlap
**Severity:** LOW
**Screenshot:** plqa_double_posted.png
**Detail:** The status line is a fixed 220×24 box at a fixed y; doubled it
wraps to two lines and its second line sits ~2px (estimated) above the next
request's avatar. Demo-level: the row pitch (76) has no room for a two-line
status. Left as visible stress; fixing means taller request rows in the demo.

## By design (checked, not issues)

- Fixed-size buttons, nav items, the brand block and the counts row clip or
  spill doubled copy — the stress working as intended.
- Scrolled feed clips correctly at the viewport in all three modes; post
  cards, counts and action bars keep their spacing while scrolling.
- RTL flow mirroring is correct where it applies: story order, post header,
  counts row and Like/Comment/Share all reverse; contacts dots move right.
- Absolute-positioned elements (request avatars, Confirm/Delete pair,
  sponsored art) do not mirror in RTL — documented pseudo-locale behaviour.
- Search and composer placeholders/values are never transformed.
- At mismatched aspect (900×600) the proportional layout anchors top-left
  and leaves dead space at the bottom — expected for ScalingMode::Proportional.
