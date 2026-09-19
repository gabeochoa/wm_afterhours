# Project todo

Remaining library work and external checks follow.
See [gaps](docs/AFTERHOURS_GAPS.md) for evidence and [history](docs/history.md) for completed work.

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
- [ ] **UP-22:** Resolve semantic font tiers through the selected scaling mode so Adaptive zoom scales text with controls. Explicit screen-relative sizes should retain their meaning.

## Library follow-ups retained from earlier reviews

- [ ] Connect `enforce_min_touch_target` to an opt-in validator of interactive hit bounds; its current flag has no consumer. Cover visibility, disabled policy, configurable thresholds and both collection modes.
- [ ] Use the UI collection for validation-marker cleanup and overlays; default-collection queries miss split UI children. Test highlights clearing after correction, without processing the shared root twice.
- [ ] Provide a font-independent native checkmark or drawing hook while preserving string overrides. WM currently draws its own mark. Test custom fonts, scaling and disabled states.

## Afterhours-dependent work (deferred)

- [ ] Expand animations with fade, blur, unblur, lift, fall, scale, dissolve, wipe, raise, curtain, sweep, shear, stretch, iris, spotlight, swing, typewriter, zoom, recede, unroll, blinds, slide, flip, emerge, tumble, drop and cascade. Support each applicable effect on a whole item, individual words (`word-*`) or individual characters (`char-*`), with stagger, interruption and reduced-motion behavior. Preserve grapheme clusters when animating characters.

- [ ] Add shader backgrounds per component. Review component-local coordinates, clipping, configurable uniforms and animation time, batching, and a fallback for unsupported renderers.

- [ ] Explore extracting charting into a separate library, vendored as a third-party plugin. Use it to demonstrate the public extension APIs, dependencies, registration and customization that an independent plugin author would use; keep the core small.

- [ ] Define and implement native cross-axis Stretch sizing for unspecified dimensions; see docs/AFTERHOURS_GAPS.md.

- [ ] Investigate faster text layout using prepared text and cached measurements. Review [the plan](docs/architecture.md) before implementation.

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
  See `docs/architecture.md`, D-02.

### WM verification and external review still pending

- [ ] Review [chrstph-gg/sf-windows](https://github.com/chrstph-gg/sf-windows) and record useful ideas or limitations relevant to WM and afterhours before adopting anything. Blocked: the external source loader rejected this URL under its input-filtering policy. No source was retrieved, so there is no completed review.

- [ ] Measure controlled cold launch and repeat the idle CPU measurement on an otherwise idle machine. Headless and verified-vsync startup, process-only idle/active CPU, RSS, settled frame timings and atlas driver counts are in [the runtime report](docs/performance.md). Shared-machine runs do not control other users or filesystem caches; do not relabel warm launches as cold. Deferred fonts load once and remain resident after visiting their consumer.
- [ ] Refresh selected CSS/native comparisons when the required local browser is available. [The source review](mocks/README.md) fixes double-counted absolute margins, explains intrinsic-text limitations, and identifies changed fixtures. Fresh selected native screenshots/trees and a font-aware viewer are ready at `output/wm-followthrough/mock-review/`. Font files, explicit sizes, wrapping intent, spans and intrinsic text participation are now represented. Browser mismatch counts remain unmeasured because the required local browser prerequisites are absent. Verify browser-vs-Raylib advances, fallback, line breaking and styled-run auto-fit before trusting text differences; absolute origins still replay native coordinates. Do not reuse August counts as current defects.

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

- [ ] floatinghotel: on its next afterhours bump, remove redundant `std::quoted` parsing from the toast, native-menu and review-export E2E handlers. See UP-19 in [gaps](docs/AFTERHOURS_GAPS.md).

- [ ] floatinghotel: replace its five pixel-intended fractional-roundness calls during its next library bump.
- [ ] Reconcile shipped API claims in the five historical gap documents across floatinghotel, hanabi and the other consumers. Those repositories are outside this WM-only pass.
