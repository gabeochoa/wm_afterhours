# Project todo

WM implementation items are complete. The remaining WM verification/research needs
the external prerequisites listed below; library changes stay deferred. Completed
work and scope decisions are recorded in [the WM pass](docs/plans/wm-only-todos.md).

## New consumer gaps collected September 13

These items are collection results, not implementation approvals. Source evidence,
causes, scope and closure checks are in [AFTERHOURS_GAPS.md](docs/AFTERHOURS_GAPS.md#consumer-gap-refresh-2026-09-13).

- [ ] **UP-17 follow-up:** Define missing-glyph rendering and atlas recovery. Atlas-independent measurement and its cache regression are now implemented; see `vendor/afterhours/docs/font-atlas-measurement.md`. Fallback drawing, automatic recovery and removal of Hanabi's guard remain deferred.
- [ ] **UP-18:** Inject and release actual Cmd/Super modifiers in E2E chords; define alias behavior without silently substituting Ctrl. Hanabi currently accepts Ctrl to make shortcuts testable.
- [ ] **UP-19:** Parse quoted E2E property values, escaped quotes and backslashes consistently. Floatinghotel cannot assert its multiword file-header property through the generic parser.
- [ ] **UP-20:** Add native image/sprite tint configuration composed with opacity. Kart uses custom Raylib drawing for driver-colored sprites.
- [ ] **UP-21:** Make immediate-renderer E2E text visibility respect ancestor clips, matching the already corrected batched path. Distinguish full visibility from partial visibility and text existence.
- [ ] **UP-22:** Resolve semantic font tiers through the selected scaling mode so Adaptive zoom scales text with controls. Explicit screen-relative sizes should retain their meaning.

## Library follow-ups retained from earlier reviews

- [ ] **Connect the advertised minimum touch-target validation option.** `vendor/afterhours/src/plugins/ui/validation_config.h:86–87` exposes `enforce_min_touch_target` and `min_touch_target_size`, and `any_enabled()` includes the flag. A complete source search finds no consumer of either setting outside that file; neither registration helper in `validation_systems.h:855–897` registers a corresponding check. Enabling the flag therefore silently does nothing. Implement an opt-in validator against actual interactive hit rectangles, respect visibility/disabled policy, and verify a too-small enabled control reports while an adequate control does not, with the flag off/on and split/single collection configurations. Keep the threshold configurable; 44px is a project/default guideline rather than an unconditional WCAG requirement.

- [ ] **Make validation highlights and their lifecycle use the UI collection.** `validation_systems.h:47–51` registers `ClearViolations` as an ordinary system, so `SystemManager::tick()` runs it over the default collection (`core/system.h:631–637`). `RenderOverlay` then uses default `EntityQuery()` at `validation_systems.h:788–790`. In the default split-collection mode, validators add `ValidationViolation` to child widgets via `UICollectionHolder`, so those child markers are not visited for clearing or drawing. The root is shared into the default collection (`ui/utilities.h:217–225`), which makes the root-recursive checks work; the old audit's blanket claim that all root validators miss UI widgets is outdated. Route marker cleanup and overlay queries to the UI collection without double processing the shared root. Verify a violating child highlights and clears after correction, including default split and compatibility single-collection modes. Source-confirmed discrepancy; runtime reproduction remains to be added when implementing.

- [ ] **Provide a native checkmark rendering option that does not depend on the active text font.** Current `component_config.h:178` still defines `DEFAULT_CHECKBOX_CHECKED = "V"`, and `imm_components.h:1112–1122` copies the indicator string into a label, with a symbol-font fallback only if no explicit font is provided. The WM Checkbox showcase suppresses the native indicator with `with_checkbox_indicators("", "")` and draws a square/check using `draw_mark()` locally. That solves the screen, not the previously accepted library item. Preserve the existing string override for intentional theme styles while providing a real built-in mark or configurable drawing hook. Verify checked/unchecked/disabled states, scaling, and custom fonts. The retained icon-registry proposal is broader and unimplemented; it does not make this native control fix complete. Do not reopen the WM screen audit.

## Afterhours-dependent work (deferred)

- [ ] Expand animations with fade, blur, unblur, lift, fall, scale, dissolve, wipe, raise, curtain, sweep, shear, stretch, iris, spotlight, swing, typewriter, zoom, recede, unroll, blinds, slide, flip, emerge, tumble, drop and cascade. Support each applicable effect on a whole item, individual words (`word-*`) or individual characters (`char-*`), with stagger, interruption and reduced-motion behavior. Preserve grapheme clusters when animating characters.

- [ ] Add shader backgrounds per component. Review component-local coordinates, clipping, configurable uniforms and animation time, batching, and a fallback for unsupported renderers.

- [ ] Explore extracting charting into a separate library, vendored as a third-party plugin. Use it to demonstrate the public extension APIs, dependencies, registration and customization that an independent plugin author would use; keep the core small.

- [ ] Define and implement native cross-axis Stretch sizing for unspecified dimensions; see docs/AFTERHOURS_GAPS.md.

- [ ] Investigate faster text layout using prepared text and cached measurements. Review [the plan](docs/plans/text-layout-performance.md) before implementation.

- [ ] Persist optional theme radius/color overrides and the panel radius through theme-file save/load. The serializer is `vendor/afterhours/src/plugins/ui/theme_io.h`; this needs a library change.

- [ ] Expose tooltip font, padding and trigger-gap configuration; see docs/AFTERHOURS_GAPS.md.

- [ ] Correct native pagination numbered/Previous indices and icon-row container config inheritance; see docs/AFTERHOURS_GAPS.md.

- [ ] Fix batched Clip bounds, deliberate-Ellipsis warning suppression and overflow debug overlays; see docs/AFTERHOURS_GAPS.md.

- [ ] Decode e2e typed UTF-8 into Unicode codepoints before queuing input; see docs/AFTERHOURS_GAPS.md.

- [ ] Clip nested scroll backgrounds, borders and scrollbars to their ancestor viewport; see docs/AFTERHOURS_GAPS.md.

- [ ] Add consistent configurable line spacing for styled labels; see docs/AFTERHOURS_GAPS.md.

- [ ] Reconcile configuration-owned skip-tabbing flags on true-to-false transitions; see docs/AFTERHOURS_GAPS.md.

- [ ] Expose gesture hardware capability separately from idle input; see docs/AFTERHOURS_GAPS.md.

- [ ] Make slider keyboard repeat independent of update frequency; see docs/AFTERHOURS_GAPS.md.

- [ ] Include HasNineSliceBorder in native renderability gates so border-only divs paint; see docs/AFTERHOURS_GAPS.md.

- [ ] Review a supported tree row/accessory renderer for icons and metadata columns; wm currently restyles tree_view descendants. See docs/AFTERHOURS_GAPS.md.

- [ ] Review composite drag-preview styling and viewport-aware drag hit testing; see docs/AFTERHOURS_GAPS.md.

- [ ] Make disabled controls noninteractive even when their labels are child entities; see docs/AFTERHOURS_GAPS.md.

- [ ] Make partial rounded outlines match fill masks and correct RoundedCorners::top_round; see docs/AFTERHOURS_GAPS.md.

- [ ] Consider an application-configured visible-row limit for native dropdown trays. Available-space sizing, scrolling, and keyboard reveal are implemented; see docs/AFTERHOURS_GAPS.md.

- [ ] Fix fractional-boundary nonprogress in native polyline::draw_dashed; wm quantizes marquee parameters to avoid the hang. See docs/AFTERHOURS_GAPS.md.

- [ ] Review native text-input focus origin across frames and its conflict with generic SelectOnFocus; see docs/AFTERHOURS_GAPS.md.

- [ ] Review native menu item/shortcut presentation options and disabled-row pointer shielding. Outside-click/Escape dismissal, opener restoration and skipping disabled rows during tray traversal are implemented; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review a presentation/config argument for native convenience dialogs so consumers need not restyle returned descendants; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review bracket decoration bounds when the decorated component has padding; wm currently puts content inset on a child. See `docs/AFTERHOURS_GAPS.md`.

- [ ] Review text-area wheel scrolling, returned focus target and scaled auto-grow padding; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review external checkbox state synchronization so app resets do not require editing `HasCheckboxState`; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Add reviewed chart options for series stroke, hover-label visibility and explicit axis bounds; native chart styling limitations remain in `docs/AFTERHOURS_GAPS.md`.

- [ ] Review backend-dependent auto-text fallbacks and contrast-validator foreground precedence; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review declarative animation elapsed-time clamping and same-property trigger composition in afterhours; see `docs/AFTERHOURS_GAPS.md`.

### First-segment animation easing

- [ ] Make `AnimHandle::sequence()` apply the first segment's easing on fresh and replayed tracks.
  `animation_basic` uses chained `.to()` calls until the library fix is reviewed.
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
  See `docs/gap-design-decisions.md`, D-02.

### WM verification and external review still pending

- [ ] Review [chrstph-gg/sf-windows](https://github.com/chrstph-gg/sf-windows) and record useful ideas or limitations relevant to WM and afterhours before adopting anything. Blocked: the external source loader rejected this URL under its input-filtering policy. No source was retrieved, so there is no completed review.

- [ ] Measure controlled cold launch and repeat the idle CPU measurement on an otherwise idle machine. Headless and verified-vsync startup, process-only idle/active CPU, RSS, settled frame timings and atlas driver counts are in [the runtime report](docs/reports/runtime-performance-2026-09-13.md). Shared-machine runs do not control other users or filesystem caches; do not relabel warm launches as cold. Deferred fonts load once and remain resident after visiting their consumer.
- [ ] Refresh selected CSS/native comparisons when the required local browser is available. [The source review](mocks/REVIEW-2026-09-13.md) fixes double-counted absolute margins, explains intrinsic-text limitations, and identifies changed fixtures. Fresh selected native screenshots/trees and a font-aware viewer are ready at `output/wm-followthrough/mock-review/`. Font files, explicit sizes, wrapping intent, spans and intrinsic text participation are now represented. Browser mismatch counts remain unmeasured because the required local browser prerequisites are absent. Verify browser-vs-Raylib advances, fallback, line breaking and styled-run auto-fit before trusting text differences; absolute origins still replay native coordinates. Do not reuse August counts as current defects.

### Earlier library requests, still deferred

- [ ] Make font-family weight registration explicit and diagnose missing variants. WM already registers Archivo regular/medium/semibold/bold and Atkinson regular/bold; that content work is complete.
- [ ] Reproduce low-alpha backgrounds in the Sokol backend using floatinghotel. Raylib's alpha specimen does not reproduce that report.
- [ ] Recheck native scrollbar-thumb dragging against the intended backend. Current WM pointer tests pass; do not treat the historical missing-feature claim as current evidence.
- [ ] Distinguish real child overflow from percent-width-plus-margin idioms in the native warning tolerance.
- [ ] Add scroll anchoring/preserve-position-on-prepend and native stagger/exit animation follow-ups.
- [ ] Review slider handle compression, crowded native tab sizing and character breaking for overlong words.
- [ ] Clarify the native same-frame `mk()` ID-collision diagnostic; retain file, line, function and suggested fix.
- [ ] Upstream shared E2E CLI configuration and command registration; app-owned baseline/screenshot destination flags stay in consumers. Missing parser registration must not silently deliver empty arguments.
- [ ] Correct the virtual list's final reachable row and clipping-aware visibility assertions; see the retained gap record.
- [ ] Add an E2E scroll-to/scroll-handle command that resolves the actual widget geometry instead of hard-coded pointer coordinates.
- [ ] Consider folding sync-scroll collection into the native scroll pass, preserving invisible/no-overflow participants. No measured performance need yet.
- [ ] Consider `RefComponent<T>` and `RefComponents<T>` aliases matching `RefEntity` for collect-then-apply systems.
- [ ] Review persistent chrome theme inheritance, deferred screen replacement during iteration, empty `gen_first_enforce` access, and native toggle label width calculation. WM workarounds remain in place.
- [ ] Review native rotation/curved text and grapheme-aware placement. WM's new typography composition demonstrates whole-word layout and optional motion only.

### Other project ownership

- [ ] floatinghotel: replace its five pixel-intended fractional-roundness calls during its next library bump.
- [ ] Reconcile shipped API claims in the five historical gap documents across floatinghotel, hanabi and the other consumers. Those repositories are outside this WM-only pass.

Completed WM screen, test and build work is recorded in [the WM-only pass](docs/plans/wm-only-todos.md). Historical August audit counts are not an active acceptance checklist for today's replacement screens.
