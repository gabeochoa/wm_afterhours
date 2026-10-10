# Afterhours gaps

Current work is in [todo.md](../todo.md). Entries below retain causes, workarounds
and closure checks. Source-only findings need reproduction before implementation;
old line numbers describe the reviewed snapshot. Completed fixes are summarized
in [history](history.md). Consumer source paths are relative to `~/p/`.

## Scrollbars over modal overlays — fixed

I assumed modal layers covered every background visual and modal input gates covered every control. Scrollbars instead drew in a final pass above all layers; wheel and thumb handlers bypassed the input gate, including text-area wheel input.

Scrollbars now draw after their layer's content, before higher layers, respecting ancestor clipping and opacity. Wheel input obeys the existing gate, and blocked thumb drags cancel. Regression tests cover both renderers, stacked layers, wheel input, drag cancellation and text areas.

Try **Tools → scrollbar_layer_repro → Open dialog**: the bar stays behind the dialog, background scrolling is blocked, and scrolling resumes after closing.

## Pseudo-locale screenshot audit, 2026-09-27

Screenshot pass over PseudoLocaleLab (Off / DoubleWords / RtlWords ×
1280×720, 900×600, 1600×900, scrolled feed, composer post + confirm).
Full report: [layout_qa/pseudo_locale_lab_layout_qa.md](layout_qa/pseudo_locale_lab_layout_qa.md).

Fixed: the RtlWords default alignment flipped every unpinned label to
Right, including buttons — a centred control label is centred in any
direction, so RTL buttons hugged the right edge and clipped (`Confirm`,
`Post`, `Home`). Library footgun, not a demo preference: only the Div
default flips now. Test: `rtl_button_labels_stay_centred`.

Fixed (library): glyphs fragmented and faded at sub-1.0 UI scale. Clean
at scale 1.0 and 1.25; at 0.9 small text broke up, at 0.5 some labels
were barely fragments — on every screen, not just the lab. Cause: the
raylib font loaders set `TEXTURE_FILTER_BILINEAR` and never generated
mipmaps, so a glyph rasterized at 96px and drawn at 10px was a ~10x
downscale sampled from four texels, and thin strokes fell between
them. `prepare_font_texture` (backends/raylib/font_helper.h) now
generates mipmaps and selects trilinear filtering (bilinear fallback
when mipmaps fail) at all three load sites. Verified by recapture:
the lab at 640×360 and the buttons screen at 900×600 are fully
legible; the buttons screen's apparent line overlaps were the same
fragmentation.

Fixed (library): doubled pseudo-locale text was not supported, only
spilled. `TextOverflow::Clip` is documented as "text is clipped at
container boundary" and is the default, but neither renderer scissored
a label to its own box -- only ancestor scroll/clip rects were
honoured. A doubled label therefore drew straight over its neighbours:
nav labels ran across the top bar, the profile name spilled out of its
button, initials spilled out of their circle, sidebar rows ran under
the feed, and a counts label was cut by the scroll container instead
of its own box. Both renderers now scissor a label's draw to its box,
intersected with the ancestor clip and with that clip restored
afterwards (rotated labels exempt: an axis-aligned scissor would cut
their corners). Follow-up from a second screenshot: the clip sliced
wrapped lines mid-glyph at the box's bottom edge, so wrapped/multiline
draws (both renderers, plain and styled runs) now drop a line that
does not fully fit instead of drawing it, and the lab's single-line
controls and labels use `TextOverflow::Ellipsis`, so truncation reads
as `Confirm Con...` rather than a sliced glyph. An earlier pass
instead exempted the lab's data labels from the transform; that was
reverted by request -- everything doubles, and is supported. Tests:
`label_clip_test` (own-box scissor, ancestor restore, line-drop),
E2E 358 (posting in DoubleWords mode asserts the new post doubles
like every other label).

Also added (library, unused by the lab): a per-label opt-out,
`ComponentConfig::with_pseudo_locale_exempt()`, skipping the
transform, the span transform and RTL mirroring for one component
(same treatment as the unit-motion exemption, including the config
merge). Test: `exempt_labels_skip_the_transform_and_mirroring`.

Fixed (library): RTL mirroring now swaps layout order, not just
text. Flow rows reversed and padding swapped, but an absolutely
positioned element kept its LTR anchor, so whole columns never moved
-- real RTL interfaces (the user's Facebook-in-Arabic screenshots)
mirror the page: nav column and sponsored column trade sides.
Autolayout now anchors an absolute child from the other side of its
parent's content box when the child participates in mirroring
(`UIComponent::rtl_mirrored`, set in apply_layout from the same
predicate as the padding swap; exempt components keep their anchor).
Per-level mirroring composes into an exact whole-window mirror --
verified by pixel measurement (frame 32..1259 -> 20..1247) and in the
lab: nav sidebar stands on the right, requests/contacts on the left,
composer Post button on the left, profile cluster on the left. One
demo fix fell out: the brand label pinned `TextAlignment::Left`, so
its text sat at the mirrored box's left edge under the search field;
unpinned, it flips with the box. Test:
`rtl_mirroring_moves_absolute_children_to_the_other_side`.

Fixed (demo): the doubled `Request confirmed` status wrapped to two
lines and crowded the next request row. It now uses
`TextOverflow::Ellipsis`, so it stays on its line at any length.

By design, not bugs: fixed-size buttons and rows clip or spill doubled
copy (that is the stress working), right-sidebar avatars and the
Confirm/Delete pair keep their absolute positions in RTL (absolutes are
not mirrored; flow rows are), and the search/composer placeholders stay
untransformed (field text is exempt).

## Consumer gap refresh, 2026-09-13

Compared with afterhours `d90db15`; source review only. UP-13/14/15/16 later landed.

### UP-17: Report incomplete fontstash measurements and avoid caching them

Sokol measurement formerly packed glyphs and silently skipped advances when the atlas filled, poisoning both caches. Hanabi's `src/util/atlas_guard.h` cannot detect every plausible partial width. `c93e10e` fixes measurement with atlas-independent `fonsTextAdvance`; the six-check failing regression now passes. No new cache or public result type was needed.

Missing-glyph drawing, automatic recovery and general error reporting remain deferred. Keep Hanabi's guard. See [measurement evidence](../vendor/afterhours/docs/font-atlas-measurement.md).

### UP-18: Actual Cmd/Super injection — implemented

Cmd was incorrectly treated as Ctrl, and the handler ignored Super. Hanabi accepted Ctrl to work around this, masking physical-modifier differences. Cmd/Super/Win/Meta now inject Super; Ctrl stays distinct. Reset, skip and timeout also clear held keys. `e2e_key_command_test` covers aliases, combined chords, release and cancellation. Hanabi's workaround remains consumer-owned.

### UP-19: Quoted E2E arguments fixed

`assert_ui file_header_label "text=a-small.cpp  +1  (new file)"` failed because generic commands split on whitespace while built-ins used separate quote rules. I assumed quoting worked across commands.

A shared reader now handles quoted arguments, escaped quotes/backslashes and empty values. Free-text commands preserve unquoted input. Malformed quotes fail with a line number. Tests reproduce Floatinghotel's assertion through the actual handler and check batch error isolation.

On Floatinghotel's next library bump, remove its second parse with `std::quoted` in `HandleShowToast`, `HandleNativeMenuAction` and `HandleExpectReviewExport` (`src/ecs/e2e_command_handlers.h`). Arguments now arrive decoded; parsing them again truncates multiword values. Its separate pinned checkout is unchanged.

### UP-20: Image and sprite tint configuration implemented

Kart used custom Raylib drawing for driver colors because native images always drew with a fixed tint. I had assumed recoloring was baked into assets. That also hid a second issue: Raylib's `UI_WHITE` is off-white, so even untinted images were slightly darkened.

`with_image_tint(Color)` now covers images, atlas sprites, image buttons and configured textures in both renderers. Tint alpha combines with widget and ancestor opacity; omitting the option restores neutral white on reused widgets. Pixel tests cover RGB multiplication, source rectangles, opacity and reset. Try **Images → Tint images** in WM.

### UP-21: Clip-aware E2E text visibility implemented

Immediate rendering registered labels against the window alone; styled runs could also register separately and bypass the parent label's visibility. The assumption was that submitting text meant the user could see it. Both renderers now register the composed label against the same ancestor clips, including text offsets and scissor rounding.

`expect_text` accepts partial label bounds; `expect_text_fully_visible` requires full bounds. `assert_ui name text="..."` checks existence/value independently. Pixel tests cover nested clips, scroll offsets and styled labels in both renderers. WM's scroll-clip test also checks an offscreen row's value without calling it visible.

### UP-22: Font tiers follow the selected scaling mode

Tiers were converted to `h720` too early, losing their logical pixel size. The assumption was that resolution scaling and UI zoom were interchangeable. A `Size` now retains the Adaptive pixel value so config copies, layout and both renderers resolve the same size. Explicit screen-relative sizes keep their meaning.

Config measurement now uses the selected mode, and text-area wrapping uses the same font size and mode as its line labels. Tests cover all tiers, window heights, zoom levels, overrides and both renderers. Try the blue **80px** sample in **Adaptive Scaling**; it now uses `FontSize::Medium` instead of manual font scaling.

## UI, layout and validation

### Overflow mode changes retain scroll state

`component_init.h` adds `HasScrollView` but does not remove it on Auto/Scroll → Hidden/Visible. In `adaptive_scaling`, zoom 300%, scroll, then return to 50% leaves a scrollbar. WM removes stale state. Define config-owned cleanup without deleting manually installed state; test transitions and clamping.

### Animation sequences ignore the first segment easing

Fixed. `AnimHandle::sequence()` initialized the target and duration but assumed the existing easing was suitable. Fresh tracks used Linear; reused tracks inherited their last easing. It now copies the first segment's easing too. At 0.5s the WM scale sequence reaches 1.118056 instead of 0.958333. Tests cover fresh/replayed tracks, mixed-easing loops and appended sequences; WM uses `.sequence()`.

### Declarative animation timing depends on frame rate

`apply_animations()` clamps dt to 5ms, so 60 frames advance only 0.3s. The assumption that clamping prevents stalls breaks ordinary durations. Consume the full elapsed time, using bounded substeps if needed; compare 60/120/240Hz.

### Declarative triggers share one track per property

Hover and click definitions share one property track and `triggered` bit; an inactive definition can reverse the active one. WM uses separate properties. Define composition or precedence and test concurrent triggers and interruption.

### Text-area scrolling, focus targets and scaled auto-grow padding

Idle wheel scrolling is fixed; unchanged rebuilds no longer reveal the caret. The rest closed with the text_area parity pass: focusing the returned wrapper focuses the field (parent-focus acceptance, verified by E2E 355 using `focus_ui` on the wrapper); auto-grow now resolves both the line height and the h720 vertical padding at the current resolution (five 30px rows at 1080p get their 162px). Floatinghotel's pixel-line-height workaround is no longer needed.

### Bracket decorations add padding to an already padded rectangle

`with_brackets` expands a rectangle that already includes padding. WM moves padding to a child. Use one bounds contract for paint, decoration and interaction; test a padded filled panel.

### Convenience dialogs do not expose presentation configuration

`modal::confirm`, `confirm_danger`, `fyi`, `info`, and `prompt` hardcode presentation. WM's `DialogPresentation.h` edits returned descendants, depending on internal names/order. Expose font, dimensions, body/actions, padding and border configuration while retaining native modal behavior.

### Menu dismissal, disabled focus targets and presentation options

Outside press/Escape, opener restoration and disabled keyboard traversal are fixed. Text padding/shortcut style remain unexposed, and disabled rows can pass pointer presses to underlying controls. WM offsets labels and adds transparent shields. Test disabled-row hit ownership as well as presentation.

### Text-input focus origin and generic SelectOnFocus conflict

Native inputs select all on keyboard focus. Generic `with_select_on_focus(true)` synthesizes a click and clears that selection. Pointer focus observed a frame later can also look like keyboard focus after just-pressed state clears. WM removes the generic flag. Retain focus origin across frames; test click-away/back typing and keyboard selection.

### Dashed polylines can stop advancing at fractional boundaries (fixed)

At 1024x768, dash 8/gap 5.6 produced a 1.9073486e-6 run at travelled 68 that could not change either float accumulator. `polyline::draw_dashed` now snaps to the pattern boundary when a run moves neither accumulator, so progress is guaranteed; WM no longer quantizes dash/gap/phase. Regression coverage in `polyline_test` plus a 540-combination marquee fuzz.

### Dropdowns have no visible-row limit or scrolling configuration

Available-space sizing, scrolling and keyboard reveal are fixed. Only an optional caller-specified visible-row cap remains; verify all options remain reachable.

### Partial rounded outlines ignore corner masks

Raylib rounded fills honor corner bits; rounded outlines round all four. `RoundedCorners::top_round` also rounds bottom-right. Make fill/outline masks agree and correct the helper; retain `example_borders` partial-corner fixtures.

### Disabled controls depend on having a label component

Pointer filtering and `HandleClicks` inspect disabled state only with `HasLabel`. Controls made from child labels remain clickable. WM removes the unavailable action's listener and guards it. Test disabled behavior on the parent with and without direct text.

### Drag previews omit styling and viewport hit testing needs review

Composite preview styling is fixed by drawing the source subtree with restored geometry, clips and opacity. Still open: `HandleDragGroups` uses unscrolled child rectangles and ignores viewport bounds. A click on visible DB schema moved Design mockups. WM pages three rows and translates indices. Test continuous scrolling, clipped cards and final-row reachability using rendered coordinates.

### Native tree rows expose only a label callback

`TreeViewConfig` has no accessory/row renderer. WM walks descendants to replace textual disclosure marks and add icons/guides/size columns. Expose a supported renderer without child-order assumptions; retain native selection, expansion and keyboard behavior.

### Bare nine-slice borders do not enter native rendering

The renderability gates in `rendering.h` omit `HasNineSliceBorder`; border-only divs remain blank. Transparent `HasColor` is WM's workaround. Recognize border visuals directly in both renderers; assert pixels, not only layout.

### Slider keyboard repeat depends on frame frequency

`HandleLeftRight` calls held listeners every update; a two-frame press changes 100% to 98%, and faster frames accelerate a hold. WM wraps callbacks with `pressed_or_repeat`. Use elapsed-time pacing with initial delay; verify exact steps and holds at multiple rates.

### Gesture backend cannot report hardware capability

No input means either idle or unsupported hardware. WM reports compiled macOS support separately from unknown device capability. Add a portable availability result before claiming hardware support.

### Configuration-owned skip-tabbing flags persist after being disabled — fixed

`apply_flags` treated config and manual skip-tabbing as the same permanent tag, leaving re-enabled pagination arrows outside `focused_ids`. Config now updates `UIComponent::skip_when_tabbing` every frame; `SkipWhenTabbing` remains independently owned by app/tray code. Forward/backward traversal and overlapping manual tags are covered. WM no longer removes the tag.

### Styled labels cannot configure line spacing

`draw_runs_in_rect` uses measured Ag height and ignores the text-area setting. Share a styled-label line-height option between measurement and drawing; test mixed sizes, wrapping and resize.

### Nested scroll decorations escape ancestor clips

Rows clip correctly, but `HasScrollView` backgrounds/borders and `RenderScrollbars` bypass ancestor intersection. `scroll_clip_bug` uses an inner viewport 1.5× the outer height. Clip decorations and scrollbars against ancestors without altering viewport geometry.

### E2E typed input queues UTF-8 bytes instead of Unicode codepoints

`HandleTypeCommand` queues chars, so signed bytes C3 A9 disappear and café becomes caf. Decode once into integer codepoints. WM's preset proves storage/caret handling, not Unicode typing. Test multibyte input, invalid sequences and subsequent ASCII.

### Batched text overflow lacks clipping and debug parity

Batched Clip lacks its own scissor; Ellipsis warns before intentional truncation; overflow debug primitives are absent. Keep `text_overflow` 150×40/90×34 fixtures. Add own-label clipping, suppress intended truncation warnings and match immediate debug output. Explicit text insets were fixed separately.

### Pagination indices and icon-row container configuration

Numbered page i+1 is used as a zero-based index; Previous subtracts twice. `icon_row` inherits config and loses absolute placement, gap and grid flags. WM remaps clicks and uses a wrapper. Fix index boundaries and preserve container configuration; test first/last/Previous.

### Tooltip presentation is hardcoded

Font inheritance and accurate measured wrapping/placement were fixed in the default-theme pass. Caller control of font size, padding and trigger gap is done: `with_tooltip_font_size`, `with_tooltip_padding` and `with_tooltip_gap` on ComponentConfig feed HasTooltip/TooltipState and RenderTooltip (defaults unchanged: styling font, 8 padding, 4 gap). Border-only guides also need recognized renderability without transparent HasColor.

### A div's own label ignores the container alignment properties

`align_items`/`justify_content` position a div's *children*; its own label is not a child, so `div(...).with_label(...).with_align_items(Center).with_justify_content(Center)` still draws the label at the box's left edge (vertically at its top), with no warning. The only lever is `with_alignment` (text alignment) plus padding/inset, which is undiscoverable from the container API authors naturally reach for. Found building SmallComponentsLab: pane labels sat flush against a divider line and value-pill text crowded the pill's left edge until both switched to text alignment. Either make the flex alignment properties apply to a leaf label, or have helpers that render a label (value_pill now centres its text by default) absorb the difference. Related: the per-label inset work in the hanabi #85 family.

### Cross-axis Stretch does not participate in native sizing

`AlignItems::Stretch` acts only like FlexStart in positioning. `children()` explicitly requests content size; `expand()` fills independently. Define unspecified cross-axis sizing and apply it during size calculation; do not relabel content sizing as CSS auto.

### Checkbox external state is treated as initialization only

Fixed: checkbox reads the supplied boolean before processing input on every call. The old code assumed retained state owned the value and overwrote app resets. External changes update disabled controls too and do not report a click; pending input toggles the supplied value. WM’s checkbox, modal and circle demos no longer write internal state.

### Chart styling and axis-domain controls

`line_chart.h` fixes stroke at 2px, uses data-only bounds and always shows hover values. Constant/single-value axes repeat endpoints. WM draws markers/readouts/budget lines and labels out-of-range thresholds. Expose stroke, hover visibility and explicit/expanded domains; test empty/constant/negative/live datasets.

### Auto-text fallback differs by rendering backend

Raylib UI_WHITE is #F5F5F5, not #FFFFFF. Against #737373 its fallbacks yield 4.35:1 and 4.43:1; true white yields 4.74:1. Use backend-independent fallback colors and test near the 4.5:1 threshold.

### Contrast validation resolves a different foreground than rendering

`ValidateComponentContrast` prefers background_hint before explicit_text_color; rendering reverses that precedence. Share foreground resolution, including disabled state. The explicit-red auto-text fixture must validate the color actually drawn.

### Capture runner: multiple resolutions in one process

The headless screenshot runner crashes on the second resolution in `invalidate_entity_slot_if_any`; separate invocations work. Repro: `--headless-screenshots --screen example_borders --resolution 720p,1080p`. Ownership between WM reset and library lifetime is unresolved; this is unrelated to cascade deletion.

### Absolute horizontal positions resolve against screen height

At the reviewed pin, `component_init.h` resolves both translate axes with screen height: w1280(1047) becomes 589px at 1280×720. WM uses explicitly scaled pixels. Verify axis/unit semantics at multiple aspect ratios before changing them.

### Uniform solid borders ignore requested thickness

Fixed: uniform borders resolve their requested width and draw inward in both renderers. The old branch assumed a generic outline respected border configuration; it drew fixed 1px or 3px strokes. Pixel tests cover scaling, fractional and oversized widths, transparency, and square/rounded/mixed corners. Focus rings retain their outward outlines.

Remaining: Raylib and Metal fills map corner bits differently. Borders now follow each backend’s existing fill order. Standardize the mapping with a migration for callers; assuming the comments matched Raylib’s actual argument order produced detached corners.

### Slider state does not follow external model changes

Retained `HasSliderState` overwrites external values after initialization. Sports uses per-tab IDs plus synchronization. Decide controlled-value semantics; verify Reset/tab changes, labels, knob position and subsequent drag/key edits.

### Grid rounding in content-sized columns

Intrinsic size sums unrounded gaps, then placement snaps accumulated positions. Six 28px profiler rows with 6px gaps overflowed a 498px content box by 16px at 1080p. WM uses expand(). Reconcile size and placement arithmetic with a measured regression.

### E2E focus-state isolation

`UIContext::reset()` retains `has_interacted`, while `focus_element` does not count as interaction. Alone/batch screenshots differ. WM clears capture state and explicitly sends Tab in focus tests. Define a common test-reset contract without clearing ordinary interaction history every screen change. AIM also had a 17×17 disabled-maximize fill difference between isolated and batch capture.

### Validation and checkbox follow-ups

Fixed: minimum click/drag bounds validation, UI-collection marker cleanup, and overlay placement before frame completion. The faulty assumptions were that exposing a flag registered its validator and that UI children lived in the default collection. Both collection modes pass; try `touch_targets` for thresholds and correction/cleanup.

Fixed: native checkbox marks now use drawn strokes in both renderers. A V glyph was assumed to be a checkmark across fonts. The checkbox also inherited its row’s left alignment and lost the caller’s text color role and inset; the indicator now receives those settings explicitly. WM uses the native mark, with custom text and empty overrides retained.

### Text measurement and rendering use different font inputs

Autolayout uses widget.font_name and spacing 1; rendering resolves weight and `1 + letter_spacing`. The assumption of a shared descriptor was wrong. Resolve actual family variant, size, spacing and wrapping once; test Dim::Text and styled/wrapped bounds against drawing. Source-confirmed; native regression still needed. One measured instance (2026-10-04, TabContainerShowcase at 720p): a tab frozen at its `Dim::Text` floor is 220px wide; its drawn label spans 216px with 2px gaps, where the floor charges 5px of text inset per side. Autolayout measured that label at 210px or less. Ellipsized narrow tabs draw with 0-1px gaps at the tab edge. Every tab still ends inside its bar.

### Glyph coverage answers were placeholders, and the wrap memo key was too small

Fixed: raylib's `font_has_glyph` trusted the glyph array, but `LoadFontData` inserts an entry for every requested codepoint, pointing absent ones at glyph 0's blank, so `↵`/`↑`/`✓` in Atkinson reported covered while drawing nothing, and `warn_on_missing_glyphs` stayed silent for them. Both backends now record the face's cmap at load (`src/font_coverage.h`) and the query answers from it (raylib: cmap ∩ loaded set; sokol: cmap, since fontstash loads the whole face). `FontManager::has_glyph` / `missing_codepoints` expose it. Found building TextCacheLab, whose coverage panel printed "covered" next to blank glyphs.

Fixed: `wrap_memo::key_for` keyed only on runs + width, so the same styled text at another font size, face or spacing was served the first wrap's line breaks; neither memo's clear was ever called, so a font reload kept stale measurements indefinitely. The key now covers face/size/spacing, entries die with `measure_memo`'s generation, and `FontManager::load_font` on an existing name invalidates both memos and the shared `TextMeasureCache`. Styled runs also take their x offsets from the joined line's measurement (`detail::text_run_offsets`) instead of summing per-run widths, which drifted a spacing at every colour boundary (hanabi #62). Tests: `text_memo_test`, `font_atlas_measure_test`; demo: TextCacheLab (WM E2E 356). Still open from this family: the full `PreparedText` redesign in docs/architecture.md, text-landing geometry (hanabi #51) and self-reported laid-out size (#286/#68).

### Virtual list re-measured every row, and an unbuilt frame reset scroll offsets

Fixed: the measured `virtual_list` summed every row's height on every build, and `height_of` is the expensive call (it wraps text). Heights and their prefix now live on the list entity (`HasVirtualListIndex`); `height_of` is asked once per row until `invalidate_virtual_rows` marks a range, and `prepend_virtual_rows` holds the viewport on the same row when older content arrives above it. `HasScrollView` also gained `scroll_to_bottom` / `scroll_to_top` (plus the left/right pair), which move offset, target and last_eased in one call.

Fixed (hanabi #163): `MeasureScrollViews` measured every scroll view every frame, including ones whose screen was not built that frame. Children are cleared for all widgets each frame, so the unbuilt view measured as empty and the clamp reset its offset to the top. The measure now skips a view that has no children and a prior measurement. Tradeoff, recorded: a view emptied while visible keeps its offset until it has children again. Tests: `virtual_list_test`, `scroll_anchor_test`; demo: VirtualListHeightsLab (WM E2E 357).

### Atlases alone do not reduce submitted draws in the measured screens

Cozy Cafe stayed at 66 draws/68 binds and Images at 59/61 after packing. Sharing a texture does not merge commands separated by render state. Attribute texture/scissor/shader/layer boundaries before coalescing; preserve painter order. Counts are driver measurements, not proof of a broken atlas API. See [performance](performance.md).

### Menu rows had no checked or radio state, and long menus ran off screen

Fixed: `MenuItem` gained `mark` (`MenuMark::Check` / `Radio`) plus `checked`, with `MenuItem::check` / `MenuItem::radio` factories; fields append after the existing four, so aggregate initializers keep compiling. A menu with any marked item reserves a mark column (`text_inset` on every row, labels left-aligned), and checked items draw a native check or dot in it. State stays caller-owned: choosing an item returns its index as before, and the next frame's items carry the new `checked` values. Long menus now bound their panel height to the roomier side of the anchor and scroll the rest (`Overflow::Scroll` on the tray, the same shape as the dropdown tray; focus reveal follows keyboard selection). Row accessories (shortcut, mark, disabled fill) became children of the row so the scroll measurement, which sums the list's own children, stops double-counting them; the disabled fill is the row's background draw. Tests: `menu_test` (marks, mark column, bounded scroll); demo: ContextMenuLab view/recent menus (WM E2E 361). Wordproc can migrate its popup bodies to `menu_list` once it bumps.

Still open: menu labels draw flush at the row edge (button padding positions children, not the label), so the tray focus ring strokes over the first glyph of a focused row. ContextMenuLab compensates with a post-hoc `text_x_offset` on its rows; a library fix (labels honoring the row's resolved left padding when left-aligned) would move every existing menu's labels and needs its own decision.

### Rounded parent corners do not clip children to the curve

`HasClipChildren` uses rectangular scissors; parent radius changes fill only. Optional rounded masks need explicit nested/backend semantics. Keep rectangular clipping fast. This is a deferred capability, separate from partial outlines and scroll-decoration clipping.

### Hot keeps tracking the pointer during a drag: fixed

Noted while dragging a channel slider in the colour swatch editor (2026-10-04). `ResolveHitTarget` now keeps `hot_id` on the already active element while the left button is down, and the legacy `active_if_mouse_inside` helper follows the same rule. Crossing other widgets during a press or drag no longer moves hover styling or pointer-focus follow to them. The release frame is not locked because `left_down` is false there, so release-mode clicks still use the real pointer position: releasing outside cancels, and releasing back inside completes. Coverage: `hit_priority_test` held-press and release-position cases; WM E2E 362/364 plus menu, slider, scrollbar, drag-drop and tab-focus scripts pass.

## Tetr input gaps, 2026-10-10

Found while bumping tetr-afterhours to `65d5292`. Consumer paths are in `tetr-afterhours/src/`.

### No per-action query keyed by the game's enum

The input collector exposes only `inputs()`, `inputs_pressed()` and `inputs_pressed_repeat()` as vectors of `ActionDone` with an `int` action, and the mapping is `std::map<int, ValidInputs>`. Every consumer writes the same scan: tetr's `held_actions()` in `systems.h`, plus `to_int`/`from_int` casts in `main.cpp`. Wanted: `input::is_held(Action)` / `is_pressed(Action)` templated on the consumer's enum, and a mapping keyed by that enum. Check: tetr deletes `held_actions()`, `to_int` and `from_int` with no behaviour change, and a held action on any gamepad counts as held.

### Hold-to-repeat needs a delay and a rate, on every device

`inputs_pressed_repeat` follows OS key repeat (about 500 ms before the first repeat), and gamepad buttons never repeat. Tetr therefore gates Move, Rotate and Drop with its own `RepeatGate` timers, which add a second problem: a fresh press can wait up to one period before it acts. Same root as the slider-repeat entry above. Wanted: a per-action repeat with a caller-set initial delay and rate (Tetris DAS/ARR) that fires on the press frame, paced by elapsed time, and covers keys, buttons and axes. Check: tetr's three `RepeatGate`s go away, the press frame moves the piece, and a hold repeats at the configured rate at 60 and 200 fps.

### `distance_sq` is missing on the raylib path

`developer.h` defines `afterhours::distance_sq` only for its fallback `MyVec2`, so raylib consumers keep their own copy (tetr `main.cpp`). Wanted: the same helper for `Vector2Type` under `AFTER_HOURS_USE_RAYLIB`. Check: tetr deletes its copy and still builds.

## Audit triage: obvious defects, 2026-10-10

Deduped from the six 2026-10-06 `todo-*.md` audits (animation-vocabulary, apple-design, break-ui, find-animation-opportunities, improve-animations, review-animations; 1,179 raw items). Only defects verified against current code (`65d5292`) with a local fix are filed here. Items that change a default, add API, or retune taste are held pending decision and are deliberately not filed. Paths are relative to `vendor/afterhours/src/` unless noted. Five WM-local defects went to `todo.md` instead.

### Masked password fields leak plaintext and corrupt the caret

`plugins/ui/text_input/component.h:582,590` allows Copy/Cut in `with_mask_char` fields (same in utils paths), and `:511-516` stores the masked string's codepoint index as a real byte offset, so clicking in `contraseña` lands inside `ñ` and the next keystroke writes invalid UTF-8. Wanted: refuse Copy/Cut while masked; convert codepoint index to byte offset in masked mode. Check: Cmd+C/X in a masked field leaves the clipboard unchanged; clicking after a masked multibyte char then typing produces valid UTF-8 with the char inserted at that codepoint.

### Stale caller indices read out of bounds across immediate containers

Shrinking the option/tab/label set (or a saved setting from a bigger monitor) indexes unchecked: `navigation_bar` (`plugins/ui/imm_containers.h:306`, `set_current_index` at `plugins/ui/components.h:220` never clamps), `dropdown` (`imm_containers.h:114`, seeded unclamped `:45`), `tab_container` (no clamp at entry, unclamped index returned at `imm_containers.h:436`), `radio_group` (`plugins/ui/imm_controls.h:469`), `grid` (`plugins/ui/grid.h:157` when `col >= col_widths.size()`), saved resolution (`plugins/window_manager.h:393`). Wanted: clamp at entry/use in each, `log_error` on mismatch (grid: fall back to `expand()`; resolution: fall back to `current_index()`). Check: build each with N=3, select last, rebuild with N=1 — no OOB/ASan hit, selection clamps to 0, returned index is in range.

### Empty containers divide by zero

`button_group` with no labels divides by `labels.size()` (`plugins/ui/imm_controls.h:129,137`); stepper index helpers do `% total` / `total-1` with `total==0` (`plugins/ui/imm_primitives.h:32-38`, SIGFPE / SIZE_MAX). Wanted: early-return/return 0 when empty. Check: empty `button_group` and `total==0` stepper next/prev run without crash and return 0/no-op.

### Text measurement memo: UB on bad sizes, hash-only hits return wrong sizes

`src/measure_memo.h:65` casts `size*64.f` to `uint64_t` (UB for negative/NaN font sizes from bad scale math), and lookup returns on hash alone (`:73`), so a collision serves another string's measured size. Adjacent to the fixed wrap-memo entry, which covers keys/generations, not this. Wanted: guard `!(size > 0)`, verify the stored text (or a second hash) on hit. Check: measuring at size NaN/-1 returns 0 without UB; two engineered colliding strings each measure to their own width.

### Arena exhaustion emits a text command with a null string

`plugins/ui/render_primitives.h:440` may get `nullptr` from `create_array_uninitialized`, yet the command is still emitted with `text = text_copy` (`:458`) and drawing calls strlen on null. Wanted: skip the command and `log_error` when allocation fails. Check: exhaust the arena with a 50KB label — no null deref, an error is logged once.

### Progress bars cast NaN to int (UB)

`plugins/ui/imm_value.h:396` clamp passes NaN through, then `:403` does `int(normalized*100)` (circular path `:520` same). Wanted: non-finite input draws 0 and labels "—". Check: `progress(0.f/0.f)` renders empty with no UB under UBSan.

### `tree_view` calls empty config callbacks mid-frame

`plugins/ui/tree_view.h:59-60` invokes `get_id`/`get_label` unchecked; a default `TreeViewConfig` throws `bad_function_call`. Wanted: assert/`log_error` at entry and bail. Check: `tree_view` with an empty config logs once and does not throw.

### Translation lookup throws on translator typos and missing English

`plugins/translation.h:100` lets `fmt::vformat` escape on a malformed format/missing param, and `:137` does `translations.at(Language::English)` which throws when the map has no English. Wanted: catch, `log_error` with the key, return the raw string / empty fallback. Check: `"{count} item{s"` and a no-English map render the raw text and log, no exception escapes UI build.

### Text-unit splitting overruns truncated UTF-8

`plugins/ui/text_units.h:31` does `i += len` unclamped, so a string ending in a truncated sequence (e.g. output of the ellipsis bug below) indexes past the end in later `substr` calls. Wanted: `i = std::min(i + len, s.size())`. Check: splitting `"\xC3"` / a half-emoji yields one replacement unit, no OOB under ASan.

### Paste silently joins lines (newline/tab dropped)

`plugins/ui/text_input/component.h:604` filters `cp >= 32`, so pasting `Line one\nLine two` gives `Line oneLine two` (file paths likewise mangled), while the shared path (`utils.h:~610`) keeps tabs — the two paths disagree. Wanted: map `\n`/`\r\n`/`\t` to one space on the single-line path, one code path only. Check: pasting the two-line string into a text input yields `Line one Line two`.

### External text replacement moves the caret and poisons undo every frame

Host-side per-frame normalization (uppercase/digits-only) hits `plugins/ui/text_input/component.h:117-122` (via the per-frame init_state callback), resetting caret=end each keystroke, leaving a stale selection anchor, and undo restores replaced text. Wanted: on external replace, clamp caret, clear selection, clear undo. Check: typing mid-string with an uppercasing host keeps the caret after the typed char; undo does not resurrect pre-replacement text.

### Theme files bypass the Builder clamps (nan/0/5 accepted)

`plugins/ui/theme_io.h:242` assigns parsed floats directly (`stof` accepts nan/inf at `:132`), so `ui_scale = 0`, `disabled_opacity = 5`, `roundness = nan` load verbatim. Wanted: reject non-finite values and apply the per-field Builder clamps on load. Check: loading that theme yields clamped finite values (scale > 0, opacity ≤ 1) and logs the rejected fields.

### Layout inspector queries the wrong collection (never finds UI)

`plugins/ui/layout_inspector.h:69` runs a default `EntityQuery` but UI entities live in `UICollectionHolder`, so every pick reports "Component not found"; `:94` also reads nonexistent `context->screen_bounds`. Wanted: look up via `getEntityForID`, use `screen_width`/`screen_height`. Check: picking any visible widget in the inspector shows that component.

### Slider reports the wrong value four ways

(1) Enter/Space on a focused slider jumps it to the mouse: `HandleDrags` fires the drag callback on `WidgetPress` (`plugins/ui/systems.h:1243-1248`) and the slider callback reads mouse position (`plugins/ui/imm_value.h:200-201`) — delete the keyboard branch (arrows already work via `HasLeftRightListener`). (2) Grabbing jumps: value maps `(mx-rx)/w` (`:201`) but the 25%-wide handle is drawn at `v*0.75w` (`:219-220`) — store a grab offset and map over `w - handle_w` (handle redesign stays a separate decision). (3) A 0-width track divides to NaN which clamp passes through (`:168`), permanently NaN; an unclamped/NaN `owned_value` (`:161`, ctor `components.h:124`) draws the handle past the track — early-out on `width <= 0`, `isfinite`-guard, clamp on init/read. (4) Percent label truncates (`:45,48,67`: 0.29 → "28%") — use `std::lround`. Check: keyboard Enter leaves the value unchanged; grabbing the handle's left edge doesn't move the value; 0-width then restored slider and `owned_value=1.5/NaN` recover to a finite in-range value; 0.29 labels "29%".

### Modal backdrops are drawn twice

`ModalBackdropRenderSystem` (`plugins/modal.h:983-997`, registered `:1066`) draws a full-alpha rect per stacked modal on top of the presence-faded backdrop div (`:449`), so dimming doubles/compounds and ignores presence fade. Wanted: delete the render system, keep the div. Check: two stacked modals dim the background exactly once at the configured alpha; mid-exit the backdrop fades with presence.

### Drag start blanks the item for a frame and centres the overlay on the cursor

`plugins/ui/systems.h:1868,1900,1907`: drag starts on `just_pressed`, the source hides the same frame and the overlay appears next frame, so a plain click blanks the card; the overlay is placed at `mouse - w/2` (`:1708,1731-1732`) ignoring the grab point. Wanted: arm on press, start on the existing `press_moved`, create the overlay the same frame at `mouse - grab_offset`. Check: click-without-move never blanks the item; grabbing a card's corner keeps that corner under the cursor.

### Fast virtual-list glides blank the current viewport

`plugins/ui/imm_virtual_list.h:234-240`: when the eased span exceeds ~3 viewports the emitted window drops the rows at the current offset, so a fast glide shows an empty viewport. Wanted: always include the window containing the current offset within the same `max_span` bound. Check: glide `scroll_to_bottom` on a 1000-row list — every frame renders the rows intersecting the viewport (no empty frame).

### Ellipsis truncation cuts inside codepoints and emoji clusters

Both ellipsis paths binary-search bytes: immediate `plugins/ui/rendering.h:1024,1036` and the batched copy `:2542` (`display_text :2554`), so narrow labels show a stray `?`/half-emoji before `...`. Wanted: one shared grapheme-safe `ellipsize(...)` snapping to codepoint (preferably grapheme, via `text_units.h`) boundaries, called from both paths. Check: `Wiśniewska-Kowalczyk`, `王秀英の設定`, `👩🏽‍💻 Priya` in a narrow Ellipsis label never split a codepoint/cluster in either renderer.

### Translation glyph loading drops all 4-byte UTF-8 (emoji, CJK Ext-B)

The hand decoder at `plugins/translation.h:288-298` handles only 1–3-byte sequences; a 4-byte lead just advances, so those glyphs never load. Wanted: decode with `GetCodepointNext`. Check: a translation containing an emoji renders the emoji (glyph present), where today it is blank/missing.

### `text_area` ignores editable/readonly/disabled state

Enter inserts a newline with no editable check, undo snapshot, or selection replacement (`plugins/ui/text_input/text_area.h:584` via `utils.h:340`), and the init callback (`text_area.h:121-137`) never copies `config.text_readonly`/`disabled` into state (text_input does, `component.h:127`), so `with_readonly(true)` areas stay editable. Wanted: copy both flags after init; guard + snapshot + `delete_selection` before `insert_newline` (sweep the other edit ops the same way). Check: readonly/disabled text_area rejects Enter and typing; Enter with a selection in an editable area replaces it and is one undo step.

### Toast fade is computed, never rendered — and ignores instant mode

`plugins/toast.h:315-317` writes opacity only if `HasOpacity` already exists, but `schedule()` never adds it, so the lifetime fade silently never draws; the hand-rolled alpha/slide at `:308-310` also has no `is_instant` check anywhere in the file. Wanted: `addComponentIfMissing<HasOpacity>` when applying the fade; snap alpha/slide when instant. Check: a toast visibly fades in its last stretch with no caller-added `HasOpacity`; under instant mode it appears/disappears with no slide.

### Instant-mode `to()` shows the from-value for one frame

`plugins/animation/track.h:130-139,261-269` with the instant landing in `advance()` (`:196-205`): `begin` keeps `start_pos=pos`, so the first rendered frame after an instant `to()` shows the old pose, then lands next tick. Wanted: land immediately in `begin`/`to` when instant. Check: with instant on, `track.to(5)` sampled before any `advance` already returns 5.

### Single-value `on_change` targets the new pose for one frame, then falls back to rest

`plugins/ui_motion.h:243-258,324-325`: a single-value change rule ends back at rest, making the builder a one-frame no-op (known victim: drop-target `on_change{{.scale=0.97f}}`). Wanted: pulse semantics locally in resolve/apply — `to(p.to).then(rest, release)`. Check: a single-value `on_change` visibly reaches its target and returns over the release mode instead of snapping back next frame.

### Text-unit changes restart from the enter pose mid-flight

`plugins/ui/text_unit_motion.h:109-112` always calls `from()`, so a unit changed while still animating snaps to `from_y`/opacity 0 and blinks. Wanted: `to()` from the current value when the unit's track is active; `from()` only for genuinely new units. Check: two rapid text changes 50ms apart keep opacity continuous (no frame at 0 for already-visible units).

### `with_reset` change rules restart their track on every fire

`plugins/ui_motion.h:170-171,248-249,320-321` applies `tr.from(reset_to)` unconditionally, so rapid re-triggers restart from the reset pose instead of continuing. Wanted: apply `reset_to` only when the track is inactive; single-fire behaviour unchanged. Check: firing the same change twice mid-flight does not teleport the value to the reset pose on the second fire.

### Built-in motion presets return `MotionRule` and are silently dropped

`fade_up/fade_in/pop_in/hover_lift/press_squash/slide_in` (`plugins/animation_presets/basic_presets.h:12-40`) return `MotionRule`; `with(T)` (`plugins/ui/component_config.h:664`) stores it as an extension, but `motion_rules()` (`plugins/ui_motion.h:388-392`) only collects `MotionExt`, so `.with(presets::fade_up())` never animates (zero `presets::` callers in WM src today). Wanted: presets return `MotionExt` (or `motion_rules` collects `MotionRule`) — one place, no default change. Check: `.with(presets::fade_up())` on a fresh widget animates y/opacity over its duration; headless test asserts the track is active frame 1.

### `with_letter_spacing(0)` is a no-op

The config merge treats 0 as unset (`plugins/ui/component_config.h:108,1239-1240`, `!= 0.f`), so an explicit zero tracking can never override an inherited one (9 WM call sites currently do nothing). Wanted: `optional<float>`, merge on `has_value`; non-zero behaviour unchanged. Check: child with `with_letter_spacing(0)` under a spaced parent measures/draws with zero extra tracking.

### Sokol backend ignores letter spacing entirely

Measure (`backends/sokol/font_helper.h:71`, memo key hardcodes `1.f` at `:84`) and draw (`backends/sokol/drawing_helpers.h:35`) take spacing as an unnamed parameter, so `with_letter_spacing(4)` is a silent no-op on Metal and layout differs from raylib. Wanted: `fonsSetSpacing` in both, spacing in the memo key. Check: the same label measures and draws wider with spacing 4 than 0 under sokol, matching raylib within a pixel.

### Rotation ignores the configured transform origin

Scale honours `mods.origin` (`plugins/ui/components.h:322`, set by `with_origin`, `component_config.h:659`) but both rotation paths (`plugins/ui/rendering.h:1704-1706,2570-2571`) pivot on the centre. Wanted: rotate about `mods.origin` in both paths; default 0.5/0.5 unchanged. Check: `with_origin(0,0)` + 90° rotation pivots on the top-left corner in both renderers (pixel test).

### `delay()` before any `to()`/`then()` silently does nothing

`plugins/animation/track.h:152-160`: with an empty chain+queue and inactive track, `delay()` falls through with no effect and no diagnostic. Wanted: `log_error` in that branch only. Check: `track.delay(1)` on a fresh track logs an error; `to(...).delay(...)` behaviour is byte-identical.

### Per-unit text motion is silently skipped for wrapped text

`plugins/ui/rendering.h:2683`: `per_unit = !wrapped && ...` falls back to a plain draw with no diagnostic when a `HasTextUnitMotion` label wraps. Wanted: `log_warn` once in that case. Check: a wrapped unit-motion label logs the warning once; unwrapped rendering is unchanged.

### Toggle knob lerps 0.2 per frame and ignores the caller's value

`plugins/ui/imm_controls.h:502`: `animation_progress += (t-p)*0.2f` once per frame — 2× speed at 120Hz, never settles, no instant/pause check; the init callback (`:498`) also never re-reads the caller's `value` and `:605` overwrites it, so external resets desync (WM's ToggleSwitchShowcase hand-writes `animation_progress` to compensate). Wanted: dt-corrected exponential (`1 - pow(0.8, dt*60)`) + settle epsilon + snap when instant + sync from `value` each call, all in the one function; 60Hz feel preserved. Check: time-to-90% matches at 60/120/200fps; setting `value` externally moves the knob next frame; under instant the knob is at target frame 1.

### Caret blink uses a hard-coded frame time and blinks under reduced motion

`plugins/ui/text_input/text_area.h:390` calls `update_blink(state, 0.016f)` (~3Hz blink at 200fps; text_input already uses `ctx.dt` at `component.h:312-313`), and `update_blink` itself (`plugins/ui/text_input/utils.h:161-166`) has no reduced/instant gate. Wanted: pass `ctx.dt`; return steady-visible when `motion::is_instant()`. Check: blink period in seconds matches at 60/200fps; under reduced/instant the caret is steady.

### Tray key-repeat drops the remainder

`plugins/ui/systems.h:1190` resets `repeat_timer = 0.f` at threshold instead of `repeat_timer -= threshold`, so repeat rate varies with frame rate. Same family as the filed slider-repeat and tetr DAS/ARR entries, different site. Wanted: carry the remainder. Check: holding a tray key for 1.0s fires the same count (±1) at 60 and 200fps.

### Sprite animation drops leftover frame time and uses raw dt

`plugins/texture_manager.h:193-197` resets `frame_time = frame_dur()` instead of carrying the remainder (24fps art plays at ~20fps at 60Hz) and ignores the scaled clock (`scaled_dt` exists, `plugins/animation/store.h:29`). Wanted: subtract-and-carry; advance by scaled dt. Check: 24fps sprite advances 24 frames in 1.0s at 60/120/200fps and freezes under pause/time-scale 0.

### Sokol `get_frame_time` ignores frames the backend itself skipped

The frame callback accumulates/resets `g_frame_elapsed` (`backends/sokol/backend.h:620-625`) but the getter (`:343-351`) returns `sapp_frame_duration()`, so at target-60 on a 120Hz display everything runs half-speed. Wanted: return the accumulated elapsed. Check: static defect verified; runtime check still owed — at target 60/120Hz a 1s timeline completes in 1.0s (needs a 120Hz host, e.g. floatinghotel).

### Particle drag flips velocity sign at low frame rates

`plugins/particles.h:75-76`: explicit-Euler `vel -= vel*drag*dt` reverses velocity when `drag*dt > 1` and decays differently per fps. Wanted: exact `vel *= exp(-drag*dt)`. Check: with drag=10, velocity after 0.5s matches at 20/60/200fps and never changes sign.

### `ease_scroll` glides on under instant/reduced motion

`plugins/ui/components.h:637-663` never checks `is_instant` (the dt-corrected smoothing at `:652-653` and a snap path at `:644` already exist). Wanted: snap `offset = target` when instant — needs the instant flag visible in the ui leaf (ui cannot include `animation/track.h` today; small seam). Non-reduced defaults unchanged. Check: with instant on, wheel scroll lands on target the same frame; with it off, the glide is unchanged.

## Other consumer requests

- Wordproc needs access-key underlines on individual characters.
- E2E packs register per SystemManager. A missing pack times out with a named diagnostic;
  automatic live-manager registries and first-tick registration were rejected for lifecycle/order cost.
- `wait` resolves at the simulation batch boundary; keep `sim_steps` small when needed.
- Puzzle's CPU image generation/conversion/sampling remains a wrapper candidate.
  Raw RGBA capture is implemented. Mutable upload UP-08 awaits consumer implementation.
  GIF encoding and node rules remain application-owned.
- Custom-color lint could catch accidental theme bypass.
- Puzzle cleanup enumerates child types. Declared ownership may avoid omissions, but
  non-owning links, cycles, undo and recycled handles need a design. Cascade-delete
  work remains unstarted.
- Tab follows allocation order. Stable cleanup prevents reshuffling but does not
  define tree order or a caller override.
- Virtual lists retain one leading spacer div; removing it needs layout-offset support.
  No measured performance reason to change it.
- Slider knob compression, crowded tab labels and hard character breaks for long
  words remain low-priority follow-ups. Wrap currently needs explicit font size.
- UP-07 accessibility semantics and UP-08 mutable textures await consumer-led upstreaming.
  Sound-feedback hooks and periodic timers remain skipped. Wider charts follow profiler needs.
- Historical Sokol alpha, theme inheritance, empty `gen_first_enforce`, switch-during-iteration,
  font-weight diagnostics and common E2E CLI requests remain in [todo.md](../todo.md).

Do not remove consumer workarounds merely because a similar feature exists. Hanabi's
atlas and keyboard-focus policies need consumer-specific checks. Query early-outs,
variable-row virtualization, right-click injection, viewport transforms, tooltips,
grids and native modal dismissal already exist; old missing-feature claims are stale.
