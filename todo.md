# Project todo

## New consumer gaps collected September 13

These items are collection results, not implementation approvals. Source evidence,
causes, scope and closure checks are in [AFTERHOURS_GAPS.md](docs/AFTERHOURS_GAPS.md#consumer-gap-refresh-2026-09-13).

- [ ] **UP-17:** Detect incomplete fontstash measurements and keep them out of layout caches; define missing-glyph rendering and recovery. Hanabi's atlas guard is still needed despite the existing atlas-full warning.
- [ ] **UP-18:** Inject and release actual Cmd/Super modifiers in E2E chords; define alias behavior without silently substituting Ctrl. Hanabi currently accepts Ctrl to make shortcuts testable.
- [ ] **UP-19:** Parse quoted E2E property values, escaped quotes and backslashes consistently. Floatinghotel cannot assert its multiword file-header property through the generic parser.
- [ ] **UP-20:** Add native image/sprite tint configuration composed with opacity. Kart uses custom Raylib drawing for driver-colored sprites.
- [ ] **UP-21:** Make immediate-renderer E2E text visibility respect ancestor clips, matching the already corrected batched path. Distinguish full visibility from partial visibility and text existence.
- [ ] **UP-22:** Resolve semantic font tiers through the selected scaling mode so Adaptive zoom scales text with controls. Explicit screen-relative sizes should retain their meaning.

## Library follow-ups retained from earlier reviews

- [ ] **Connect the advertised minimum touch-target validation option.** `vendor/afterhours/src/plugins/ui/validation_config.h:86–87` exposes `enforce_min_touch_target` and `min_touch_target_size`, and `any_enabled()` includes the flag. A complete source search finds no consumer of either setting outside that file; neither registration helper in `validation_systems.h:855–897` registers a corresponding check. Enabling the flag therefore silently does nothing. Implement an opt-in validator against actual interactive hit rectangles, respect visibility/disabled policy, and verify a too-small enabled control reports while an adequate control does not, with the flag off/on and split/single collection configurations. Keep the threshold configurable; 44px is a project/default guideline rather than an unconditional WCAG requirement.

- [ ] **Make validation highlights and their lifecycle use the UI collection.** `validation_systems.h:47–51` registers `ClearViolations` as an ordinary system, so `SystemManager::tick()` runs it over the default collection (`core/system.h:631–637`). `RenderOverlay` then uses default `EntityQuery()` at `validation_systems.h:788–790`. In the default split-collection mode, validators add `ValidationViolation` to child widgets via `UICollectionHolder`, so those child markers are not visited for clearing or drawing. The root is shared into the default collection (`ui/utilities.h:217–225`), which makes the root-recursive checks work; the old audit's blanket claim that all root validators miss UI widgets is outdated. Route marker cleanup and overlay queries to the UI collection without double processing the shared root. Verify a violating child highlights and clears after correction, including default split and compatibility single-collection modes. Source-confirmed discrepancy; runtime reproduction remains to be added when implementing.

- [ ] **Provide a native checkmark rendering option that does not depend on the active text font.** Current `component_config.h:178` still defines `DEFAULT_CHECKBOX_CHECKED = "V"`, and `imm_components.h:1112–1122` copies the indicator string into a label, with a symbol-font fallback only if no explicit font is provided. The WM Checkbox showcase suppresses the native indicator with `with_checkbox_indicators("", "")` and draws a square/check using `draw_mark()` locally. That solves the screen, not the previously accepted library item. Preserve the existing string override for intentional theme styles while providing a real built-in mark or configurable drawing hook. Verify checked/unchecked/disabled states, scaling, and custom fonts. The retained icon-registry proposal is broader and unimplemented; it does not make this native control fix complete. Do not reopen the WM screen audit.

- [ ] **Record optional rounded child clipping as a renderer limitation.** `HasClipChildren` and scroll clipping currently use rectangle intersections and scissor commands (`rendering.h:63–97`, `2543–2568`); parent corner radius is not part of the clip shape. A rectangular child can therefore paint in the cut-out corners of a rounded clipping parent. This is an unsupported feature, not proof the new square-framed fixtures are broken. Keep rectangular scissoring fast; consider an explicit rounded-mask clip only for callers that need it, with nested-clip/backend tests. It is separate from partial rounded outline masks and the already tracked nested scroll-decoration ancestor bypass. This entry preserves the limitation; it does not authorize or require a new masking backend in the current screen pass.

## Existing project backlog

- [ ] Review Adham Dannaway’s supplied “16 little UI design tips that make a big impact” as a practical design checklist. Group related content with spacing; keep visual treatments and behavior consistent; make hierarchy and the primary action clear with a squint/blur check; remove unnecessary decoration and use color purposefully. Check text and essential control contrast against the applicable WCAG thresholds, and pair color indicators with another cue. Default ordinary UI to one readable sans-serif family with a high x-height, regular/bold weights and limited uppercase; left-align longer text and evaluate 1.5–2× body line height. Review dark-gray text and brand-specific exceptions in context rather than applying every guideline mechanically.

- [ ] Explore expressive typography layouts and placement inspired by contemporary Gen Z design: oversized and overlapping type, rotated or curved text, custom alignment and composition, and animated arrangements. Build visual examples while keeping font-family choices deliberate, text readable, and reduced-motion/accessibility fallbacks available.

- [ ] Expand animations with fade, blur, unblur, lift, fall, scale, dissolve, wipe, raise, curtain, sweep, shear, stretch, iris, spotlight, swing, typewriter, zoom, recede, unroll, blinds, slide, flip, emerge, tumble, drop and cascade. Support each applicable effect on a whole item, individual words (`word-*`) or individual characters (`char-*`), with stagger, interruption and reduced-motion behavior. Preserve grapheme clusters when animating characters.

- [ ] Improve components with transparent backgrounds. Check text contrast, borders, hover/focus/disabled states, hit testing and compositing over different underlying surfaces.

- [ ] Add shader backgrounds per component. Review component-local coordinates, clipping, configurable uniforms and animation time, batching, and a fallback for unsupported renderers.

- [ ] Explore extracting charting into a separate library, vendored as a third-party plugin. Use it to demonstrate the public extension APIs, dependencies, registration and customization that an independent plugin author would use; keep the core small.

- [ ] Support custom indicators for graphics and charts; review marker shapes, labels, thresholds and application-provided drawing against the existing chart API.

- [ ] Add a rolling number animation for counters and changing values: a smooth, blurred transition when a number changes, rather than instantly swapping the text. Define digit transitions, carry/borrow, interruption by a new value, reduced-motion behavior, and a reusable demo before choosing an API.

- [ ] Review [chrstph-gg/sf-windows](https://github.com/chrstph-gg/sf-windows) and record useful ideas or limitations relevant to WM and afterhours before adopting anything.

- [ ] Study [Adham Dannaway’s design system examples](https://www.adhamdannaway.com/blog/design-systems/design-system-examples). Use the supplied article to review consistent typography and spacing, Carbon’s surface colors and larger UI patterns, Atlassian’s semantic color/elevation tokens, Spectrum’s input-dependent sizing and writing guidance, and GOV.UK’s tested accessible patterns. Document a small set of choices that fit WM and afterhours, with visual examples, before adding new APIs.

- [ ] Define and implement native cross-axis Stretch sizing for unspecified dimensions; see docs/AFTERHOURS_GAPS.md.

- [ ] Investigate faster text layout using prepared text and cached measurements. Review [the plan](docs/plans/text-layout-performance.md) before implementation.

- [ ] Persist optional theme radius/color overrides and the panel radius through theme-file save/load.

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

### Performance follow-ups — 2026-09-12

- [ ] Move screen artwork into texture atlases. Measure texture switches, draw
  calls and frame time before and after; preserve filtering and crisp edges.
- [ ] Speed up launch. Profile time to the first usable screen, including font
  loading, asset loading and screen initialization; defer work that is not
  needed for that first screen. Record cold and warm startup measurements.
- [ ] Audit CPU and RAM usage. Measure idle and active screens, then repeat
  screen switches to check for memory growth and retained textures/entities.
  Record representative frame times, CPU usage, resident memory and peaks
  before choosing optimizations.

Start with wm-owned changes. Any required afterhours edits remain pending user
review. These follow-ups are separate from the current visual parity audit.

Open work from the 2026-08-15 session: the mock-tool layout sweep and the
cross-project gap-doc audit. Grouped by where the fix lands.

Numbers came from `./mocks/build.sh --no-serve && ./mocks/analyze.py` and
`./mocks/sheets.sh`. Detail lives in `mocks/ISSUES.md`.

### Where each fix lands

"Both" means the two halves are separable and want separate commits. "Unknown"
means the sweep found a disagreement but nobody has decided yet whether
afterhours is wrong or the screen is — that decision IS the work.

| # | issue | afterhours | wm | other |
|---|---|:--:|:--:|:--:|
| 1 | Font weights fall back silently | explicit family registration + warn | register variants | |
| 2 | Alpha blending on sokol | sokol blend mode | | verify from floatinghotel |
| 3 | Scrollbar thumb dragging | hit-testing and dragging | | |
| 4 | Overflow is silent | warning tolerance | over-specified screens | |
| 7 | Never-implemented gap requests | most of them | e2e pack upstreams **from** wm | |
| 8 | Slider knob, tab bars, hard wrap | all three | | |
| 9 | Pixel-intended roundness calls | | convert remaining sites | |
| 13 | Family D (heights) | likely for `file_tree`/`forms` | text metrics for `multiline_text_lab` | |
| 14 | Family E (diagonal offset) | **unknown** | border theory disproved | |
| 15 | Outliers, `islands_trains` 80 | **likely** | | |
| 16 | Remaining mock limitations | | unreachable nodes and absolute placement | |
| 17 | Five `with_roundness(px)` sites | | | floatinghotel |
| 18 | Gap docs list shipped work | | | 5 docs, 4 repos |

### afterhours (library)

#### 1. Font weights fall back silently — **afterhours** (API), wm (content)
**Earlier entry here was wrong** and is corrected: I filed this as "the API
landed but the capability did not" and said the text path needed variant
selection. It already has it. `FontManager::resolve_weighted`
(`ui_core_components.h:420`) maps `(base, weight)` to a registered
`"<base>@bold"` and the render path calls it in four places.

All five weights render identically on `font_weight_720p` because that screen
uses the default font, and wm only registers one variant in the whole app
(`DGOne@bold`, `font_config.h:85`). Nothing is broken — the fallback is doing
what it says.

The real defect is the *shape of the contract*, and it is the same one
`is_right_click` had:

- The requirement is a **string naming convention** (`"Inter@bold"`) that
  appears nowhere in `with_font_weight`'s signature or docs. A caller has no
  way to discover it.
- `resolve_weighted` falls back to the base font **silently**. You ask for
  SemiBold, you get Regular, and nothing says why. floatinghotel filed "No
  Font Weight Support" — they almost certainly hit exactly this.

Worth fixing as an API question, not a font question: declare a family's
weights explicitly rather than string-encoding them, and warn once when a
requested weight falls back.

#### 2. Alpha blending — verify on sokol — **afterhours** (sokol backend)
The one gap filed independently by two projects (floatinghotel, hanabi #15):
low-alpha `with_custom_background` reported to render opaque. It does **not**
reproduce on raylib — `alpha_blend_repro_720p` steps cleanly from 32 to 255 with
the text readable through the wash, for both a low-alpha `Color` and
`with_opacity`.

Both reporters are sokol. `RL_BLEND_ALPHA` is set only in
`backends/raylib/headless.h:52`; no blend mode is set in the sokol path. Run
`alpha_blend_repro` from floatinghotel before calling it fixed or broken.

#### 3. Scrollbar thumb dragging — afterhours

Add hit-testing and dragging to the scrollbar thumb. The existing indicator
reports position without setting it.

#### 4. Children that do not fit are silently overflowed — **both**
`mocks/ISSUES.md` issue 1. 21 nodes across `powerwash_settings` (9),
`flex_alignment` (5), `layout` (3), `deadspace_settings`, `meters_gauges`,
`rubber_bandits_menu`, `forms`.

Caller error in every case, so the library question is only whether it is
*visible* at authoring time. `strictness = 1` correctly refuses to shrink and
`flex_wrap` is NoWrap by intent, so what is actually left is that the overflow
warning does not fire: its tolerance is `sx + child_margin + 4px`, and
`powerwash_settings` overflows by 8px behind a 20px margin. That allowance was
deliberate (a `percent(1)` child plus margin overflows by exactly its margin and
the caller cannot act on it), so tightening it needs a way to tell that idiom
apart from real overflow — not a smaller constant.

#### 7. Remaining requests from the gap docs — afterhours

- **Scroll anchoring / preserve-position-on-prepend** (hanabi #30).
- **Shared e2e CLI arguments** — see item 23.
- **Animation follow-ups** — per-item stagger and exit animations (hanabi AN-8..AN-12).

#### 8. Long-standing, low priority — **afterhours**
From `docs/AFTERHOURS_GAPS.md`, unchanged: slider handle 0.75 compression;
crowded tab bars still need a smaller font at the call site; word-wrap has no
hard character break.

---

### wm_afterhours

#### 9. Convert remaining pixel-intended roundness calls — wm

Review per-call `with_roundness` sites that intended pixels and convert them
to explicit corner radii. Caller fractions override the theme default.

### The html/mock sweep — every issue it found

`./mocks/build.sh --no-serve && ./mocks/sheets.sh` →
`mocks/sheets/all_disagreements.png` (44 screens) plus `sheet01..16.png`.
Dashed red is afterhours' rect over the CSS pane.

Deltas below are `(dx, dy, dw, dh)` in px, CSS minus afterhours. Counts in the
family tables are POST-fix unless marked.

#### 13. Family D — height disagreements — **unknown**, mixed
- `multiline_text_lab` — `Pixels/Text`, dh -54 and -72. The only `Dim::Text`
  users in the whole sweep, and the biggest height gaps. Wrapped line count
  differs, so this is text metrics again, but far larger than Family B.
- `file_tree` — x2 `Percent/Expand` (0,-17,0,22). Expand resolving to a
  different height, worth a look after the cross-axis expand fix.
- `decorators` — x2 `Percent/Children` (0,7,0,-14).
- `forms` — x3 `Percent/Percent` (0,-1,0,-13).
- `drag_drop` — x4 `Percent/Percent` (0,0,0,-3).

#### 14. Family E — uniform diagonal offset — **unknown**
`decorative_frame` (2,2) and (8,8); `cozy_cafe` (4,4) and (14,14). dx equals dy
exactly.

**The border hypothesis is wrong.** `HasBorder` exists
(`components.h:398`) but `autolayout.h` never reads it — a border is drawn
inside the rect and does not inset the content box. So adding border width to
the dump would not help, and putting a CSS `border` on these nodes would
actively break them under border-box. Cause still unknown; the equal dx/dy is
the only lead.

#### 15. Single-screen outliers — **unknown**
- **`islands_trains_settings` — 80 origins, the largest single screen.** raylib
  renders a 430px centred modal; the CSS re-solve spreads it far wider (Δx up to
  361 on the buttons). One of the two is wrong about `align_items: Center` on a
  `ScreenPercent` child. Not explained by Family A.
- `themes` — one node at dx 278.
- `meters_gauges` — one node at dx 115.
- `pagination` (x6 dw 3), `parcel_corps_settings` (x6 dw 7) — uniform small
  widths, possibly Family A.
- `cards`, `kirby_options`, `vstack_showcase`, `hstack_showcase` — 1-2 nodes
  each, sub-5px. Noise-adjacent; check last.

#### 16. Remaining mock limitations

Investigate the three remaining unreachable nodes in `aim_chat`.
Absolute nodes are replayed from afterhours coordinates, so the mock cannot
independently catch absolute-placement bugs.

#### 21. Clarify the `mk()` collision diagnostic

A same-frame ID collision already logs file, line, function and the suggested
fix before throwing `bad_optional_access`. Cartographer found the message
confusing; improve its wording.

#### 23. Shared e2e CLI surface and command registration — afterhours

**Still open from the plan: the CLI surface.** `E2EArgs` + `parse_e2e_args`
lives only in wm; ten of its twelve flags drive something afterhours owns
(`runner.set_timeout`, `graphics::Config::time_scale`,
`capture_every_n_frames`, ...) and belong upstream. The two that do not —
`--update-baselines` and kart's `--screenshot-dir` — stay down, because the
`screenshot` command takes an app-supplied callback and afterhours has no
opinion on where files land. Not started; it touches every consumer's `main()`.

**Also found:** adding an e2e command needs the handler *and* an entry in
`runner.h`'s per-command parse chain. Miss the second and args arrive empty
with no diagnostic.

#### 29. Virtual list renders ~20 rows below its clip at max scroll — **afterhours**
At the end of `virtual_list_lab`, the bottom visible row is 09979 while rows up
to 09999 are rendered below the pane's clip rect. Clamping stops short of
putting the true last row on screen, so the end of a virtual list is
unreachable.

Masked until now: `95_scrollbar_drag.e2e` asserted `row 09999` and passed only
because the visible-text registry ignored clipping. It now asserts 09979 with a
pointer here.

#### 27. e2e command to grab the scroll handle — **afterhours**
Scrolling in a test means hand-computing the thumb's pixel position:

```
mouse_move 384 125
mouse_down 384 125
mouse_move 384 400
mouse_up 384 400
```

384 is the pane's right edge minus half the bar thickness, and 125 is "near the
top of the track" — both derived by hand from `scrollbar_geometry()` and both
silently wrong the moment the pane moves or `scrollbar_thickness` changes. A
miss does not fail; it scrolls nothing and the assertions afterwards pass or
fail for the wrong reason.

Want something like `scroll_to <debug_name> <percent>` that resolves the view
by name, asks `scrollbar_geometry()` where the thumb is, and drives the drag —
or writes the offset directly and skips the pointer entirely. Fails loudly if
the name does not resolve or the view does not scroll.

First users: `95_scrollbar_drag.e2e`, `97a_sync_scroll.e2e`.

#### 28. Fold `SyncScrollViews` into `HandleScrollInput`'s pass — **afterhours**
Low priority; filed because it was asked about, not because it is measurable.

`SyncScrollViews` iterates `HasScrollView` a second time each frame, over a set
that is ≤5 entities on the busiest wm screen (14 across all 100), with
`if (sync_group == 0) return;` as its first line. `HandleScrollInput` already
walks the same set, so the collect could hoist into it and the apply into its
`after()` — one fewer system, no extra pass.

Not done because that system early-returns on `!was_rendered_to_screen`,
`should_hide`, and the `auto_overflow` no-overflow case. The collect has to sit
*above* all three or members silently stop syncing in those states, and the
symptom would be a green test with a broken pane.

**Rejected alternatives**, so nobody re-proposes them: caching the group→member
index across frames (the UI is immediate-mode — `mk()` rebuilds the tree every
frame, so cached component references dangle), and running the apply at a lower
tick rate (scroll is latency-critical; a throttled apply shears the panes apart
mid-drag).

#### 26. `RefComponent` to match `RefEntity` — **afterhours**
`entity.h:300` has `RefEntity = std::reference_wrapper<Entity>` and a
`RefEntities` vector alias, but there is no component-level equivalent. A
system that collects during `for_each_with` and acts in `after()` — the pattern
forced by `EntityQuery` not seeing UI entities (`docs/47_silent_traps.md`) —
has to spell `std::vector<std::reference_wrapper<HasScrollView>>` by hand, or
reach for raw pointers.

```cpp
template <typename T> using RefComponent = std::reference_wrapper<T>;
template <typename T> using RefComponents = std::vector<RefComponent<T>>;
```

Then `SyncScrollViews::members` is `RefComponents<HasScrollView>`. Trivial, but
it is the difference between the collect-then-apply pattern looking idiomatic
and looking like a workaround. First user is `SyncScrollViews` in
`plugins/ui/systems.h`.

### Other repos (not ours to land)

#### 17. floatinghotel: five `with_roundness(px)` sites render as pills — **floatinghotel**
`src/ui/diff_renderer.h:614`, `src/ecs/main_content_system.h:815`,
`src/ecs/sidebar_system.h:700`, `:706`, `:781` pass `4.0f` / `2.0f` into a 0..1
fraction. Both backends clamp the computed radius to half the short side, so on
a 28px row `4.0f` is a 14px radius — a full pill, not 4px corners.
`with_corner_radius(4.f)` is the fix. **Owner will do this on their next bump**;
the new warn will point at each one.

#### 18. Reconcile the five gap docs — they list shipped work as blockers — **other repos**
`floatinghotel/docs/afterhours-gaps.md` (last touched Aug 1) lists six "Missing
Primitives", all with app-local workarounds. Every one now exists upstream —
`menu.h` and `overlay.h` were added Aug 3, two days later:

| doc says missing | actually in afterhours |
|---|---|
| Draggable Divider — BLOCKER P0 | `divider()` `imm_components.h:338` |
| Split Pane — BLOCKER P0 | `hsplit_pane` / `vsplit_pane` `:417` |
| Tree Node — BLOCKER P1 | `tree_view()` `tree_view.h:110` |
| Dropdown Menu — HIGH P0 | `dropdown_menu()` `menu.h:208` |
| Context Menu — HIGH P1 | `context_menu()` `menu.h:237` |
| Anchored Popover — MEDIUM P0 | `popover()` `overlay.h:248` |
| No rich text / multi-color | `with_styled_label(std::vector<TextSpan>)` |
| Virtualized list | `virtual_list()` `imm_components.h:141` |
| Row flex broken with `expand()` | 4 passing tests, incl. `expand_fills_remaining_row` |

hanabi is the same in places (#29 placeholder → `with_placeholder` exists; #23
virtualization → shipped; #18 "no flex-grow" → `expand()` is tested), though it
self-marks several RESOLVED already.

They are maintaining app-local `split_panel.h`, `tree_view.h`, `menu_setup.h`
and `context_menu.h` against gaps that closed. Worth a pass marking each shipped
item with the API to migrate to.

---

### What's next

1. Item 7: scroll anchoring, shared e2e CLI arguments and animation follow-ups.
2. Item 21: clarify the collision diagnostic.
3. Items 13, 14 and 15: investigate remaining layout disagreements.
4. Item 3: scrollbar dragging.

Item 2 needs a sokol run of `alpha_blend_repro` from floatinghotel.
Regenerate mock comparisons with `./mocks/build.sh --no-serve && ./mocks/sheets.sh`.

## 2026-08-29 session — the showcase quality pass

Filed by Gabe as rapid-fire TODOs while agents worked in parallel, plus what
the work turned up. Status here is what was *measured*, not what was claimed:
several agent reports of "fixed" and "not broken" were re-checked by hand and
three of my own regression calls did not survive measurement.

### Partly done

- **app mockups** — backup, chat, media grid and VPN landed. Both new ones
  still print debug readouts ("18 of the 24 entries have no widgets at all
  right now", "toggle=off link=Not connected dialing=0.00s"), which cuts
  against the "make them feel real" ask.
- **screen ordering** — the navigator groups by category, so no renaming is
  needed. But six near-duplicate category strings hold ~10 screens:
  `Components` (3) vs `Component Galleries` (34), plus singleton `Widgets`,
  `Text`, `Text Editing`, `Bug Reports`. Ten one-word edits, zero pixels moved.

### Reported not broken — needs your eyes, not more agent time

An agent drove each of these with injected input and found them responsive:
scroll-in-scroll (inner thumb measured 239px against 242 expected), context
menu lab, composer lab, real-world modals / side drawer / bottom sheet.

Either headless does not reproduce what you saw, or the injector passes where a
human cannot. That second possibility is the harness gap below, so do not treat
these as closed until you have clicked them yourself.

### Diagnosed, not fixed

**Focus rings on `self_align`.** `UIComponent::focus_rect()` insets by its
offset, so the ring draws *inside* the element, which matches "too small".
Measured: only the left edge (20px of 48) and the bottom (156px of 184) render,
in colour (193,195,196), and only on the 1px falling outside the button. No top
or right. Draw order says the ring should be on top (background at
`rendering.h:1521`, ring at `:1613`), so the obvious covered-by-fill theory does
not hold and the real cause is still open.

### Not started

Popover lab opens its commit options by default and is ugly. Anchored menus
render text outside their bounds. The layout bug repros screen needs a lot of
work. Config options has contrast and overlap problems. Mini motorways toggles
are ugly. Checkboxes are ugly, separate from the click fix. Horizontal drag wants a ghosting toggle. Game screens should be interactive enough to tell a real control
from a painted one. More screens in the shape of hover_lab. Add juice / jazz
them up. File tree wants the synthetic-vs-root toggle (decided, not
built). Entity index lab should be reframed as a real UI demo. drag_drop wants
consolidating with its sibling. Theme swatches / podcast UI. Language demo
wants a resize screen with icons repositioning per language, and a thicker
Japanese font.

### Infrastructure problems found this session

These are the reason bugs survived a green suite, so they rank above most of
the list above.

1. **Assert-free e2e scripts.** An agent counted 23 that perform a click, key
   or drag and then assert nothing, ending at a screenshot. That is why three
   genuinely dead widgets sat behind a green 113/113, and the list included
   exactly the widgets reported broken. A crude re-count by last-line flagged
   far more, so the real number needs a careful pass rather than a heuristic.
2. **`src/screen_includes.gen` is shared.** One agent's `make SCREEN=X`
   rewrites it, so the next agent's full build silently renders one screen. It
   cost the borders agent three build cycles and produced a screenshot set of 1.
   Wants a per-`OUTPUT_DIR` path.
3. **`file_tree` renders the live working directory**, so every agent's
   `output-*` dir changes that screen. Passing on tolerance today, will drift.
4. **The navigator's interaction has no coverage.** It is registered only in
   `run_screen_demo`, which needs a display and exits rc=1 headless, so click
   and scroll are compile-verified and reasoned, never exercised. This is the
   same shape as problem 1.
5. **Headless renders very few frames** (2 at dt=0.016), so time-based easing
   can never settle before capture. Seed animated state at its resting value
   and put motion behind interaction.

### Library rough edges worked around, not fixed

- `text_input` inherits the *current screen's* theme, so persistent chrome over
  themed content changes colour per screen. The navigator pins explicit colours.
- Swapping screens from a click handler is a use-after-free: `load_screen`
  frees the system `ScreenCyclerSystem` is still iterating. The navigator
  records the index and drains it after `systems.run`.
- `gen_first_enforce` (`entity_query.h:485-491`) logs an error on empty and
  then still returns `values[0]` on the empty vector.
- `toggle_switch` sizes its label from `parent_width - track_w`; the padding
  fix landed but the underlying "compute width by hand to dodge `expand()`
  resolution" workaround is still there.

### Method note

Measure, do not eyeball. Upscaled NEAREST crops produced three wrong regression
calls this session, and a fourth suspicion (that clicking a checkbox label
would strand keyboard focus) died to two probe scripts. Contrast claims should
carry a computed ratio: the transparent-centre border labels went 1.29:1 ->
15.37:1, and that number is the whole argument.

### Remaining polish checks

Review real layout warnings in `decorators`, `nine_slice_borders`,
`parcel_corps` and `cards`. Separate them from intentional overflow demos.

- Investigate why `layout` renders two different ways run to run in the e2e
  path while remaining stable in the full-render path.
