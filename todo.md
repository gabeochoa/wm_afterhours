# Project todo

Remaining library work and external checks follow.
See [gaps](docs/AFTERHOURS_GAPS.md) for evidence and [history](docs/history.md) for completed work.

## UI element follow-ups from the lab review

- [ ] `with_padding` on a label with no children does nothing (it warns); the lab wraps every padded text in a child. Decide whether label text should honour padding directly.
- [ ] Should `ClosedBy::Any` (click outside dismisses) be the modal default instead of `CloseRequest`? The lab opts in per dialog.
- [ ] A drawn chevron/icon primitive: the accordion and learn-more examples hand-draw theirs with `draw_line_ex` because glyph fonts render only ASCII.
- [ ] Multiline label line height does not match the font pixel size, so anything that scrolls text by fixed rows (the counter reel) has to lay out one label per row.
- [ ] Banner stacking: hovering spreads the stack, but a long stack should scroll so every open banner can be reached.
- [ ] Tab rows shrink label text to fit; long example titles squished at 140 px. A minimum font size or overflow rule for buttons would avoid that.

## Terminal consumer support

- [x] Live, argument-aware completion callbacks with explicit refresh after app data changes. Puzzle needs level IDs after `reload_levels` and later-argument choices such as `juice bloom on`.
- [x] Optional queued execution: own submitted arguments, drain commands at an app-chosen safe point, and report results afterward. Cartographer and Puzzle dispatch through later systems.
- [x] Optional overlay controls: opening/closing, focus restoration, and an app-specified input layer so typing cannot trigger gameplay.
- [x] Usage hints and command availability with an explanation; recheck before immediate or queued execution. WM demonstrates this with `reset`.
- [x] Separate command-picker plugin with categories, shortcut labels, fuzzy search and a virtual list. Try the WM `command_picker` screen.

- [x] Scrollbars respect modal layers; wheel and thumb input obey modal gates. See [gap](docs/AFTERHOURS_GAPS.md#scrollbars-over-modal-overlays--fixed).

## New consumer gaps collected September 13

These items are collection results, not implementation approvals. Source evidence,
causes, scope and closure checks are in [AFTERHOURS_GAPS.md](docs/AFTERHOURS_GAPS.md#consumer-gap-refresh-2026-09-13).

- [ ] **UP-17 follow-up:** Define missing-glyph rendering and atlas recovery. Atlas-independent measurement and its cache regression are now implemented; see `vendor/afterhours/docs/font-atlas-measurement.md`. Fallback drawing, automatic recovery and removal of Hanabi's guard remain deferred.
- [x] **UP-19:** Shared E2E argument parsing handles quoted properties, escaped quotes/backslashes and empty values.
- [x] **UP-20:** Native image/sprite tint configuration composes with opacity. WM Images demonstrates tint and reset; Kart can adopt it on its next library bump.
- [x] **UP-21:** Both renderers respect ancestor clips for E2E text visibility; full label bounds and label-value assertions are separate.
- [x] **UP-22:** Font tiers follow Adaptive zoom and Proportional resolution scaling, including config copies and measurement. Explicit screen-relative sizes retain their meaning.

## Library follow-ups retained from earlier reviews

- [x] Validate minimum click/drag target sizes with configurable thresholds, visibility and disabled exclusions, and both collection modes. Try `touch_targets`.
- [x] Use the UI collection for validation-marker cleanup and overlays; corrected targets clear their highlights without processing the shared root twice.
- [x] Native checkbox marks work independently of fonts, preserve string overrides, and respect sizing, disabled colors and opacity. WM uses them in `checkboxes`.

## Afterhours-dependent work (deferred)

- [x] Flip the ui → animation dependency: animation leaf, ui leaf, opt-in `plugins/ui_motion.h` bridge that owns the trigger blocks (`.with(um::on_hover(...))`), the resolver and the component writes. ui needs a generic seam first (`ComponentConfig::with(T)` + `extensions_of<T>()`, a per-widget init-hook registry run after `apply_visuals`, a pre-build system slot the pre-update bridge runs); then move `ui/motion_config.h` out, drop the `motion` field / `on_*` setters / `AdvanceTracks` push from ui, port wm call sites, add a boundary check. Do it before any game bumps afterhours.
- [x] Effect presets beyond the shipped set (fade, lift, fall, scale, slide, zoom, drop, shake, spin, pulse, typewriter, cascade): blur, unblur, dissolve, wipe, curtain, sweep, shear, stretch, iris, spotlight, swing, recede, unroll, blinds, flip, emerge and tumble ship as `presets::effect_presets` (shader, quad and blur paths) with a Transitions Lab group showing each one.
- [x] Sokol/Metal backend for `effects::BlurPass` (raw sokol-gfx textured-quad pipeline, blur.fs MSL; verified by headless_capture blur mode) and fail-closed `effects::Effect` loads on Metal (sokol-gl has one fixed fragment shader, so generic with_shader cannot run there; unsupported .fs now loads as not-ok instead of silently no-opping).
- [x] OKLab interpolation for colour tracks; `Track<ColorType>` lerps per channel in sRGB, which greys out saturated fades.
- [x] Full-GPU text motion: per-unit motion re-issues one draw per grapheme; a glyph-instanced path with per-instance transforms would keep long labels cheap.
- [x] Query the OS reduced-motion setting and seed `reduced_motion_enabled` from it on first run.
- [x] Subtree relayout for size tracks: a `with_size()` fed by a track re-lays out the whole root every frame; a dirty-subtree pass would scope it.
- [x] Shader backgrounds per component: `with_shader` scopes one widget's draws in an `effects::Effect`; uniforms are set per frame from the app. Clipping and the sokol fallback are the items above.

- [ ] Explore extracting charting into a separate library, vendored as a third-party plugin. Use it to demonstrate the public extension APIs, dependencies, registration and customization that an independent plugin author would use; keep the core small.

- [ ] Define and implement native cross-axis Stretch sizing for unspecified dimensions; see docs/AFTERHOURS_GAPS.md.

- [ ] Investigate faster text layout using prepared text and cached measurements. Review [the plan](docs/architecture.md) before implementation.

- [ ] Persist optional theme radius/color overrides and the panel radius through theme-file save/load. The serializer is `vendor/afterhours/src/plugins/ui/theme_io.h`; this needs a library change.

- [x] Expose tooltip font, padding and trigger-gap configuration; see docs/AFTERHOURS_GAPS.md. Done: `with_tooltip_font_size` / `with_tooltip_padding` / `with_tooltip_gap` on ComponentConfig, carried through HasTooltip and TooltipState into RenderTooltip; TooltipLab has a sizes row.

- [ ] Correct native pagination numbered/Previous indices and icon-row container config inheritance; see docs/AFTERHOURS_GAPS.md.

- [ ] Fix batched Clip bounds, deliberate-Ellipsis warning suppression and overflow debug overlays; see docs/AFTERHOURS_GAPS.md.

- [ ] Decode e2e typed UTF-8 into Unicode codepoints before queuing input; see docs/AFTERHOURS_GAPS.md.

- [ ] Clip nested scroll backgrounds, borders and scrollbars to their ancestor viewport; see docs/AFTERHOURS_GAPS.md.

- [ ] Add consistent configurable line spacing for styled labels; see docs/AFTERHOURS_GAPS.md.

- [x] Skip-tabbing configuration follows each frame, restoring keyboard access when cleared while preserving manually added tags.

- [ ] Expose gesture hardware capability separately from idle input; see docs/AFTERHOURS_GAPS.md.

- [ ] Make slider keyboard repeat independent of update frequency; see docs/AFTERHOURS_GAPS.md.

- [ ] Include HasNineSliceBorder in native renderability gates so border-only divs paint; see docs/AFTERHOURS_GAPS.md.

- [ ] Review a supported tree row/accessory renderer for icons and metadata columns; wm currently restyles tree_view descendants. See docs/AFTERHOURS_GAPS.md.

- [ ] Review composite drag-preview styling and viewport-aware drag hit testing; see docs/AFTERHOURS_GAPS.md.

- [ ] Make disabled controls noninteractive even when their labels are child entities; see docs/AFTERHOURS_GAPS.md.

- [ ] Make partial rounded outlines match fill masks and correct RoundedCorners::top_round; see docs/AFTERHOURS_GAPS.md.

- [ ] Consider an application-configured visible-row limit for native dropdown trays. Available-space sizing, scrolling, and keyboard reveal are implemented; see docs/AFTERHOURS_GAPS.md.

- [x] Fix fractional-boundary nonprogress in native polyline::draw_dashed; wm quantizes marquee parameters to avoid the hang. See docs/AFTERHOURS_GAPS.md.

- [ ] Review native text-input focus origin across frames and its conflict with generic SelectOnFocus; see docs/AFTERHOURS_GAPS.md.

- [ ] Review native menu item/shortcut presentation options and disabled-row pointer shielding. Outside-click/Escape dismissal, opener restoration and skipping disabled rows during tray traversal are implemented; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review a presentation/config argument for native convenience dialogs so consumers need not restyle returned descendants; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review bracket decoration bounds when the decorated component has padding; wm currently puts content inset on a child. See `docs/AFTERHOURS_GAPS.md`.

- [ ] Review text-area wheel scrolling, returned focus target and scaled auto-grow padding; see `docs/AFTERHOURS_GAPS.md`.

- [x] Checkboxes follow the app’s boolean before processing input; resets no longer require editing `HasCheckboxState`.

- [ ] Add reviewed chart options for series stroke, hover-label visibility and explicit axis bounds; native chart styling limitations remain in `docs/AFTERHOURS_GAPS.md`.

- [ ] Review backend-dependent auto-text fallbacks and contrast-validator foreground precedence; see `docs/AFTERHOURS_GAPS.md`.

- [x] Declarative animation elapsed-time clamping and same-property trigger composition: the motion rewrite resolves one target per property and springs in closed form.

### First-segment animation easing

- [x] Chained steps apply the first segment's easing on fresh, replayed and looping tracks (`Track::to().then()`).
  `animation_basic` uses the native chain API.
  See [the measured gap](docs/AFTERHOURS_GAPS.md#animation-sequences-ignore-the-first-segment-easing).

### Scroll state after changing overflow

- [ ] Remove obsolete scroll state when an element changes from Auto/Scroll to Hidden/Visible.
  wm clears it explicitly in `adaptive_scaling`. See
  [the gap](docs/AFTERHOURS_GAPS.md#overflow-mode-changes-retain-scroll-state).

### Charts after the profiling UI

- [ ] Extend UP-12 beyond the charts needed by the default profiling UI.
  Keep unneeded area, bar, scatter and sparkline variants as follow-ups.
  Reassess pie/donut, stacked charts, histograms and pan/zoom when a consumer
  needs them; these options are not individually approved requirements.
  Extend the interactive wm chart test screen with each implemented type.
  See `docs/architecture.md`, D-02.

### WM verification and external review still pending

- [ ] Review [chrstph-gg/sf-windows](https://github.com/chrstph-gg/sf-windows) and record useful ideas or limitations relevant to WM and afterhours before adopting anything. Blocked: the external source loader rejected this URL under its input-filtering policy. No source was retrieved, so there is no completed review.

- [ ] Reduce `ui_tester` memory usage, especially during unit testing (`make test` runs all 347 E2E scripts in one process). Profile peak RSS over a full E2E run first and attribute it before changing anything: deferred CJK fonts and atlases stay resident after their first screen (peak sampled RSS 360.1 MiB after lazy fonts, 816.2 MiB before; see [the performance report](docs/performance.md)), and capture/render-texture buffers, screen caches and profiler recording are the other candidates. Reduce the largest test-only retention without dropping coverage; sampled live RSS alone does not prove a leak.
- [ ] Measure controlled cold launch and repeat the idle CPU measurement on an otherwise idle machine. Headless and verified-vsync startup, process-only idle/active CPU, RSS, settled frame timings and atlas driver counts are in [the runtime report](docs/performance.md). Shared-machine runs do not control other users or filesystem caches; do not relabel warm launches as cold. Deferred fonts load once and remain resident after visiting their consumer.
- [ ] Refresh selected CSS/native comparisons when the required local browser is available. [The source review](mocks/README.md) fixes double-counted absolute margins, explains intrinsic-text limitations, and identifies changed fixtures. Fresh selected native screenshots/trees and a font-aware viewer are ready at `output/wm-followthrough/mock-review/`. Font files, explicit sizes, wrapping intent, spans and intrinsic text participation are now represented. Browser mismatch counts remain unmeasured because the required local browser prerequisites are absent. Verify browser-vs-Raylib advances, fallback, line breaking and styled-run auto-fit before trusting text differences; absolute origins still replay native coordinates. Do not reuse August counts as current defects.

### Earlier library requests, still deferred

- [ ] Make font-family weight registration explicit and diagnose missing variants. WM already registers Archivo regular/medium/semibold/bold and Atkinson regular/bold; that content work is complete.
- [ ] Reproduce low-alpha backgrounds in the Sokol backend using floatinghotel. Raylib's alpha specimen does not reproduce that report.
- [ ] Recheck native scrollbar-thumb dragging against the intended backend. Current WM pointer tests pass; do not treat the historical missing-feature claim as current evidence.
- [ ] Distinguish real child overflow from percent-width-plus-margin idioms in the native warning tolerance.
- [ ] Add scroll anchoring/preserve-position-on-prepend.
- [ ] Review slider handle compression, crowded native tab sizing and character breaking for overlong words.
- [ ] Clarify the native same-frame `mk()` ID-collision diagnostic; retain file, line, function and suggested fix.
- [ ] Upstream shared E2E CLI configuration and command registration; app-owned baseline/screenshot destination flags stay in consumers. Missing parser registration must not silently deliver empty arguments.
- [ ] Correct the virtual list's final reachable row and clipping-aware visibility assertions; see the retained gap record.
- [ ] Add an E2E scroll-to/scroll-handle command that resolves the actual widget geometry instead of hard-coded pointer coordinates.
- [ ] Consider folding sync-scroll collection into the native scroll pass, preserving invisible/no-overflow participants. No measured performance need yet.
- [ ] Consider `RefComponent<T>` and `RefComponents<T>` aliases matching `RefEntity` for collect-then-apply systems.
- [ ] Review persistent chrome theme inheritance, deferred screen replacement during iteration, empty `gen_first_enforce` access, and native toggle label width calculation. WM workarounds remain in place.
- [x] Review native rotation/curved text and grapheme-aware placement. Native whole-label rotation and grapheme splitting already existed; afterhours 1188f1f adds layout_text_on_arc plus per-unit TextUnits rotation, demonstrated by CurvedTextLab (WM).

### Other project ownership

- [ ] floatinghotel: on its next afterhours bump, remove redundant `std::quoted` parsing from the toast, native-menu and review-export E2E handlers. See UP-19 in [gaps](docs/AFTERHOURS_GAPS.md).

- [ ] floatinghotel: replace its five pixel-intended fractional-roundness calls during its next library bump.
- [ ] Reconcile shipped API claims in the five historical gap documents across floatinghotel, hanabi and the other consumers. Those repositories are outside this WM-only pass.

## Gap intake 2026-09-23 (hanabi, kart, wordproc, cartographer, WM)

Classification: `BUG` wrong result in existing code · `EXT:<plugin>` missing capability inside a plugin's job · `NEW-PLUGIN` coherent new surface (needs explicit go-ahead) · `VERIFY` claim may be stale vs HEAD · `REJECT?` proposed not-library, awaiting approval. Hanabi items are by its gap numbers; its 14 families are the canonical items and dup members point at them.

### Ranked backlog (demand = distinct projects asking; easiest first within a tier)

Demand 3 — do first:
1. E2E scripted addressing + command packs (hanabi #51 family, cartographer, wordproc #14 mechanism) — EXT:e2e_testing, M. Colour/text/absence assertions unlock every consumer's test suites.
2. Tooltip caller config: font size, padding, trigger gap (WM, wordproc #8) — EXT:ui, S. Already an open WM line; wordproc asks too.

Demand 2:
3. Public contrast-ratio helper (cartographer; WM validation work touches the same code) — EXT:color, XS. One function, already computed internally.
4. Focus ring family: one `focus_ring_for`, `:focus-visible`, per-widget offset (hanabi #83, wordproc #5) — EXT:ui, M.
5. Small imm components: divider, value pill, tab indicator, settings list (cartographer, kart) — EXT:ui, S.
6. Validation-log throttle toggle (cartographer, hanabi #53) — EXT:ui, XS.
7. Pointer parity: right-click/multi-button scripted input, wheel axes, SUPER state (wordproc #14, hanabi #405/#526/#562) — EXT:input_system + e2e, M.
8. text_area parity with text_input (hanabi #67 family; floatinghotel carries the workaround) — EXT:ui, M.
9. Prepared text + cached measurements, per-label text inset (WM line 57 + hanabi text family #136 and inset family #85 — the largest internal volume, ~28 headings) — EXT:ui, L.
10. Virtual list variable row heights + scroll-anchor (hanabi #326 family, WM virtual-list items) — EXT:ui, L.

Demand 1 (single project, ordered easy → hard):
11. kart VERIFYs: `GetFontDefault` headless, absolute-position rect assertion, checkbox overflow warning — XS/S each, may close with no code.
12. `Margin::left()` / `Padding::horizontal()` + settings-path override (cartographer) — EXT:autolayout/settings, XS.
13. OS appearance (dark-mode) query (hanabi #1/#16) — EXT:os, S.
14. Rendering-nits batch (hanabi: #25, #71, #76, #78, #80, #81, #88, #94, #102, #107, #110, #111, #276, #327) — EXT:ui, S total, XS each.
15. #374 sokol resize mid-pass abort (hanabi) — BUG, M (repro is the hard part).
16. #375 focused-field border top edge (hanabi) — BUG, M (clip repro first).
17. Widget-lifetime remainder (hanabi #171 family) — EXT:ui, M.
18. Text-editing action surface: delete-to-line-start, macOS keymap, responder commands (hanabi #257/#264/#565) — EXT:ui/input, M.
19. Frame/host-loop family: request-frame + frame-wake (hanabi #542/#546, both CRIT for that app), phases, mailbox, accounting (hanabi) — EXT, L. Highest severity in the whole intake despite single-project demand.
20. `context_menu` plugin (wordproc #15) — NEW-PLUGIN?, M/L.
21. Colour input (hanabi #58) — EXT:ui, L (no second consumer; candidate REJECT?).
22. `accessibility` semantics plugin (hanabi #112 family) — NEW-PLUGIN?, L.
23. Two view trees in one window (hanabi #335) — EXT:ui contexts, XL. Hardest item in the intake.

### Unit A — VERIFY / close (checked 2026-09-23)

- [x] kart `--screenshot-dir` / `--e2e-speed` flags: exist (`e2e_testing/harness.h:70,85`). Close on kart's next bump.
- [x] wordproc #3 hover state: `ui_motion` triggers (`on_hover`) ship in the bridge. #6 animation/motion: motion tracks + presets shipped (items 44/45). #9 custom render callback: `with_on_draw_fg/bg` (`ui/component_config.h:551`). #7 font: resolved upstream.
- [x] hanabi #2 property tween: `motion::Track` + timelines/springs ship. AN-8 per-item stagger/delay: `Track::delay` (`animation/track.h:152`) + `TextUnitMotion.stagger`. AN-10 one-shot trigger: `um::on_change` / `on_appear`.
- [ ] kart `GetFontDefault()` invalid in headless: fallbacks still return it (`backends/raylib/font_helper.h:194,252`); needs a headless run to confirm or fix. VERIFY.
- [ ] kart `with_absolute_position()` x-vs-height: init resolves tx/ty separately (`ui/component_init.h:520`); needs a runtime rect assertion before closing. VERIFY.
- [ ] kart checkbox internal layout overflow: row exists (`ui/imm_components.h:1081`) with children-sized height in the default path, but no runtime validation-warning reproduction yet; kart no longer hits it. VERIFY.
- [ ] hanabi #16 OS appearance (dark-mode) query: only `os::reduced_motion_enabled` exists. Not closed; moves to Unit E as EXT.
- [x] kart `with_gap()` ignores the `Size` dim: fixed at HEAD — `resolve_gap` is dim-aware (`autolayout.h:116`, comment cites this bug).
- [x] kart `with_opacity()` dark rectangle: fixed at HEAD — `opacity_pct` scales existing alpha (`color.h:148`, comment cites the symptom). Covers hanabi #15's mechanism; #13 alpha blending in `draw_texture_pro` stays in Unit C remainder.
- [x] kart `imm::slider` label overflow: fixed at HEAD — label and background take complementary 50%/50% percents (`imm_components.h:1590,1612`).
- [x] kart `apply_overrides` drops visual fields: fixed at HEAD — opacity/scale/translate/gap/border/shadows/on_draw now forwarded (`component_config.h:1140`, comment cites it).

### Unit B — BUGs (owning plugin, focused test each)
- [x] Animation sequences ignore the first segment easing: already fixed (WM gaps record; `AnimHandle::sequence` copies first easing).
- [x] Declarative animation timing depends on frame rate: fixed by construction — sampling is analytic over accumulated time, no dt clamp remains. Proof added: `animation_test` rate-independence checks (30/60/240 steps and one step agree; 69/69).
- [x] Declarative triggers share one track per property: fixed by design — `resolve_motion` resolves one target per property with defined precedence (Press > Hover > Focus > State > Appear rest; release recovers the still-active trigger), covered by `motion_config_test` (33/33).
- [x] Overflow-mode changes retain scroll state: fixed — component_init removes config-owned `HasScrollView` when the config stops asking for scrolling (new `config_owned` flag; app-installed scroll views survive). Test: `changing_overflow_releases_and_restores_scroll_view` in `drag_preview_test` (3272/3272; scroll_anchor/scrollbar_render green).
- [x] Contrast validation resolves a different foreground than rendering: fixed — `ValidateComponentContrast` now uses the shared `detail::resolve_label_color` (explicit → auto → theme, disabled darkening). Test: `contrast_validation_test` (failed pre-fix, passes post-fix).
- [x] Checkbox external state treated as initialization only: already fixed (WM gaps record; checkbox reads the supplied boolean every call).
- [ ] BUG sokol: resize tears down the render target mid-pass and aborts (hanabi #374). Needs a Metal repro (headless_capture resize-during-pass) before fixing.
- [ ] BUG ui: a focused field's border loses its top edge (hanabi #375). Hanabi's isolation says it is a clip (the field's drawable region excludes its top row), not overpaint; app-side wrapper workaround exists (`edged_field.h`). Needs a pixel-sampling repro in the WM harness before fixing.
- [x] hanabi #39/#40 e2e runner: fixed at HEAD — end-of-stream drains the last command before finalize, and single-script `has_failed()` asks the handlers directly (`e2e_testing/runner.h:445,462`). #223's deadline-by-`dt` remainder stays VERIFY (tick(dt) exists; needs a host-loop repro).

### Unit C — EXT:ui / EXT:autolayout (canonical families first)

- [ ] EXT:ui text inset: per-label inset + label honours its own padding (hanabi #85 family: #75, #84, #91, #100, #109, #277; WM line 8 `with_padding` on a label). `kTextInset` exists (`ui/rendering.h:397`) but is not per-label settable.
- [ ] EXT:ui text measurement: prepared text + cached measurements (WM line 57; hanabi text family canonical #136: #69, #79, #82, #87, #103, #116, #135, #137, #190, #191, #340, #435-437, #450, #570, #574-576, #579). Includes #48 missing-codepoint query, #51 text-landing geometry, #62 styled spans in monospace blocks, #286/#68 self-reported laid-out size.
- [ ] EXT:ui focus: one `focus_ring_for`, `:focus-visible`, per-widget offset, independent contrast edges (hanabi #83 family: #46, #263, #265-267; wordproc #5 focus indicators).
- [ ] EXT:ui text_area parity: multi-line as a mode of the text-input implementation — placeholder, background, focus ring, word motion, harness visibility (hanabi #67 family: #57, #65, #105, #258, #260-263; #56 programmatic focus; #65 padding derives from field height is BUG-adjacent).
- [ ] EXT:ui virtual list: variable row heights with retained prefix-height index + range invalidation (hanabi #326 family: #23, #31a, #147, #170, #224, #420, #455; WM virtual-list items). Includes #30a scroll-anchor/prepend, #476 atomic scroll-to-end, #163 clamp against retired children.
- [ ] EXT:ui widget lifetime remainder: library entities invisible to consumers, exit animations have nothing to animate (hanabi #171 family remainder: #146, #160, #162, #163, #525).
- [ ] EXT:ui small components as imm helpers (not new plugins): divider, value pill, tab active indicator, compact settings list (cartographer; kart settings rows; wordproc #4 dropdown triangle is `with_dropdown_indicators`, VERIFY at bump).
- [ ] EXT:ui rendering nits (hanabi, XS/S each): #25 degenerate triangle on mixed corners, #71 grid snapping quantizes positions, #76 unpadded element gets a fraction of the screen, #78 `draw_circle_v` truncates centre, #80 boxes rasterize 1px bigger/up-left, #81 per-corner rounding bits named for the opposite corner, #88 baseline alignment, #92/#106 MSAA/antialiasing pinned off, #93/#97/#222 absolute-child placement and flow counting, #94 scrollbar overlay mode, #101 supersampled capture, #102 `on_draw_fg` scaled rect without scale, #107 selected-row fill is its own background, #110 origin rounding, #111 hover highlight is the hit rect, #276 `Dim::Percent` ignores own margin, #327 draw-only element without an Entity, #335 two view trees in one window (L, likely EXT:ui contexts).
- [ ] EXT:autolayout helpers: `Margin::left()` / `Padding::horizontal()` single-side constructors, `with_720p_size` convenience (cartographer); flex-grow to pin trailing element (hanabi #18; may already exist — VERIFY first).

### Unit D — EXT:e2e_testing / EXT:input_system

- [x] EXT:e2e_testing scripted addressing (ranked #1, done): `assert_ui`/`assert_ui_text` gain `bg=#rrggbb[aa]` and `fg=#rrggbb[aa]` colour properties (fg via the shared `resolve_label_color`); new `expect_no_ui <name>` (waits one frame, then judges absence); `assert_ui_text ... in=<scope>` restricts the match to a named subtree. #59 quoted values and #86 geometry capture already existed. Tests: `e2e_arguments_test` 123/123 (3 new handler tests), WM E2E 353 end-to-end (its first run caught a wrong hex in the script itself, not the code).
- [x] EXT:e2e_testing command packs (ranked #1, done): `ui_commands::register_full_pack<InputAction>(sm, dump_fn)` registers builtins + UI + perf + unknown + cleanup in order, one call per SystemManager.
- [ ] EXT:e2e_testing runner: per-script reset in directory mode (#381), handler-owned timeout messages (#380), worked host-loop example (#41).
- [ ] EXT:input_system pointer parity: right-click/multi-button in scripted input (wordproc #14; hanabi #526 synthetic right-click misses direct polling), wheel axes + smoothing (#30b, #405 family: #406-408, #445, #446), middle-button state, SUPER held state (#562, #568, #256).

### Unit E — EXT:settings / EXT:color / EXT:animation / EXT:os

- [ ] EXT:settings: default settings-path override from code (cartographer); JSON comment-preserving writes — see REJECT? G6.
- [x] EXT:color: public contrast-ratio helper for custom widgets (cartographer; validator computes it internally — expose, don't duplicate). Stale: `colors::contrast_ratio` in `src/plugins/color.h` has been public since 2026-01 and is the same function the validator calls. Pinned by `color_contrast_test` (includes color.h alone; 21:1, symmetry, #767676 = 4.54).
- [ ] EXT:os: OS appearance (dark-mode) query next to `os::reduced_motion_enabled` (hanabi #1/#16); window-focus/frontmost query (#28a, S/M — accept only the query, not the platform family G1).
- [ ] EXT:ui validation: throttle/disable toggle for per-frame validation logs (cartographer); wrong layout corrected silently but warned forever (hanabi #53).

### Unit F — NEW-PLUGIN candidates (explicit go-ahead required each)

- [ ] NEW-PLUGIN? `context_menu`: generic context-menu primitive over modal/overlay (wordproc #15; hanabi #287 drag-candidate thresholds partially). Right-click input itself is Unit D.
- [ ] NEW-PLUGIN? `accessibility`: role/name/value semantics annotations + query (hanabi #112 family: #458). Weakest candidate — desktop-app consumers only.

### Unit G — REJECT? (proposed not-library; awaiting approval)

- [ ] REJECT? G1 hanabi OS-integration family (#33a: menu bar, notifications, global hotkeys, deep links, Spotlight, bundling, installed-font discovery, #5, #28a actions, #31b, #32a exe-relative resources — #32a may return as an EXT:files bug, #36 cache-dir split, #60 sokol drag-and-drop enable, #465-474, #571, #572): desktop platform shim; hanabi carries it app-side. Generic crumbs already extracted (reduced motion; appearance query in Unit E).
- [ ] REJECT? G2 wordproc #13 table/grid component: domain widget; autolayout grid + imm composition suffice, no second consumer.
- [ ] REJECT? G3 wordproc #12 rich text as a new subsystem: per-span styling exists (styled labels/TextSpan); any parity gap is Unit C, not a subsystem.
- [ ] REJECT? G4 cartographer app E2E verbs (`set_volume`, `check_volume`, menu-flow verbs as library commands): app state, not library; the pack mechanism is Unit D.
- [ ] REJECT? G5 hanabi §4 negative results (e.g. #4 status-glyph reachability): deliberate non-features; keep recorded so they are not re-filed.
- [ ] REJECT? G6 settings comment-preserving JSON writes, if the serializer proves one-way: do not grow a JSON parser in `settings`; counter-proposal is JSON5 read support only.

### Remaining hanabi live items not in a family above (grouped, all EXT unless noted)

- [ ] Frame/host loop (EXT, host-loop plugin surface): #542 request-frame primitive (CRIT), #546 frame-wake contract for futures/SSE (CRIT), #543 indivisible frame phases (L), #544 non-consuming input-activity snapshot, #545 window exposure/backing events, #547 timer visual deadlines, #548 `dt` is callback time, #549 headless cadence harness, #541 Metal ignores `RunConfig::target_fps` (BUG), #580 cancellable background jobs, #581 deactivate hook for conditional systems, #587 frame-safe mailbox, #589 per-system CPU accounting, #585/#586 retained-byte attribution + memory-pressure event, #181/#183/#438 per-frame allocation family (ComponentConfig copies, focusable `std::set`, string churn), #155 first-draw pre-warm, #126/#125/#212/#145 GPU accounting family (byte totals, deferred frees, frame scope), #14 texture sampler mipmaps, #95 `clipboard.h` declares none of the symbols it calls (BUG), #50/#54 graphics/input reads bypass the injector (BUG), #70 entity created this frame not findable by id (BUG), #230 `mouse.pos` NaN until first event (BUG), #172 input injection requires the e2e plugin compiled in, #45 widget callbacks outlive their frame / imm `on_submit`, #38 container hover without clickable, #63 container draws over children, #64 window-level chrome/render layers, #90 `ctx.theme` read at render time, #74/#286 resolved layout tree walk + self position, #146 tree-size reporting, #3 absolute `button()` click vs manual hit-test, #13 alpha blending in `draw_texture_pro` (see Unit B opacity), #28a window focus query (Unit E), #37 text selection on read-only text (L), #58 colour input of any kind (L; likely REJECT? — no second consumer — decision deferred to Unit F review), #257/#264/#565 text-editing action surface (delete-to-line-start, macOS-correct default keymap, imperative responder commands), #259 script parser `\n` escape (EXT:e2e_testing, XS), #336 tab order scoping, #287 drag primitive unreachable from config (XS; see Unit F), #306 `with_auto_grow` row-count return (XS).
