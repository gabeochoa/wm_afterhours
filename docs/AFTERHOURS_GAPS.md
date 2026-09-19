# Afterhours gaps

Current work is in [todo.md](../todo.md). Entries below retain causes, workarounds
and closure checks. Source-only findings need reproduction before implementation;
old line numbers describe the reviewed snapshot. Completed fixes are summarized
in [history](history.md). Consumer source paths are relative to `~/p/`.

## Scrollbars over modal overlays — fixed

I assumed modal layers covered every background visual and modal input gates covered every control. Scrollbars instead drew in a final pass above all layers; wheel and thumb handlers bypassed the input gate, including text-area wheel input.

Scrollbars now draw after their layer's content, before higher layers, respecting ancestor clipping and opacity. Wheel input obeys the existing gate, and blocked thumb drags cancel. Regression tests cover both renderers, stacked layers, wheel input, drag cancellation and text areas.

Try **Tools → scrollbar_layer_repro → Open dialog**: the bar stays behind the dialog, background scrolling is blocked, and scrolling resumes after closing.

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

`AnimHandle::sequence()` omits the first `current_easing`, using linear or a previous segment's easing. At 0.5s a 0→1.15, 0.6s EaseOutQuad segment gives 0.958333 instead of 1.118056. WM uses chained `.to()`. Copy first-segment easing and test fresh/replayed sequences.

### Declarative animation timing depends on frame rate

`apply_animations()` clamps dt to 5ms, so 60 frames advance only 0.3s. The assumption that clamping prevents stalls breaks ordinary durations. Consume the full elapsed time, using bounded substeps if needed; compare 60/120/240Hz.

### Declarative triggers share one track per property

Hover and click definitions share one property track and `triggered` bit; an inactive definition can reverse the active one. WM uses separate properties. Define composition or precedence and test concurrent triggers and interruption.

### Text-area scrolling, focus targets and scaled auto-grow padding

Idle wheel scrolling is fixed; unchanged rebuilds no longer reveal the caret. Still open: returned wrapper is not the focus target; auto-grow adds fixed 8px despite scaled padding; `text_area.h` reads line-height `.value` without resolving Size. At 1080p five 30px rows get 158px instead of 162px. Provide supported focus targeting and resolve all units, including Adaptive zoom. Floatinghotel uses pixel line heights as a workaround.

### Bracket decorations add padding to an already padded rectangle

`with_brackets` expands a rectangle that already includes padding. WM moves padding to a child. Use one bounds contract for paint, decoration and interaction; test a padded filled panel.

### Convenience dialogs do not expose presentation configuration

`modal::confirm`, `confirm_danger`, `fyi`, `info`, and `prompt` hardcode presentation. WM's `DialogPresentation.h` edits returned descendants, depending on internal names/order. Expose font, dimensions, body/actions, padding and border configuration while retaining native modal behavior.

### Menu dismissal, disabled focus targets and presentation options

Outside press/Escape, opener restoration and disabled keyboard traversal are fixed. Text padding/shortcut style remain unexposed, and disabled rows can pass pointer presses to underlying controls. WM offsets labels and adds transparent shields. Test disabled-row hit ownership as well as presentation.

### Text-input focus origin and generic SelectOnFocus conflict

Native inputs select all on keyboard focus. Generic `with_select_on_focus(true)` synthesizes a click and clears that selection. Pointer focus observed a frame later can also look like keyboard focus after just-pressed state clears. WM removes the generic flag. Retain focus origin across frames; test click-away/back typing and keyboard selection.

### Dashed polylines can stop advancing at fractional boundaries

At 1024×768, dash 8/gap 5.6 produces period 13.6000004; at travelled=68 a 1.9073486e-6 run cannot change the float accumulator. `polyline.h:92/96` loops forever. WM quantizes dash/gap/phase. Guarantee progress at pattern/segment boundaries; the workaround completed 18,796 isolated combinations.

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

### Configuration-owned skip-tabbing flags persist after being disabled

`apply_flags` adds `SkipWhenTabbing` for true but never removes it for false. An enabled pagination arrow stays outside `focused_ids`. WM removes the tag. Reconcile config-owned state while preserving manually installed flags; test true→false.

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

Font inheritance and accurate measured wrapping/placement were fixed in the default-theme pass. Caller control of font size, padding and trigger gap remains open. Border-only guides also need recognized renderability without transparent HasColor.

### Cross-axis Stretch does not participate in native sizing

`AlignItems::Stretch` acts only like FlexStart in positioning. `children()` explicitly requests content size; `expand()` fills independently. Define unspecified cross-axis sizing and apply it during size calculation; do not relabel content sizing as CSS auto.

### Checkbox external state is treated as initialization only

The bool reference initializes `HasCheckboxState` once; retained state then overwrites app resets. WM synchronizes the component manually. Define controlled-value ownership and test external reset followed by pointer/keyboard changes.

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

The uniform branch omits `BorderSide::thickness`; rounded outlines become 1px, sharp outlines fixed 3px. WM uses a thickness-aware draw helper. Test requested thickness on sharp/rounded borders in both renderers.

### Slider state does not follow external model changes

Retained `HasSliderState` overwrites external values after initialization. Sports uses per-tab IDs plus synchronization. Decide controlled-value semantics; verify Reset/tab changes, labels, knob position and subsequent drag/key edits.

### Grid rounding in content-sized columns

Intrinsic size sums unrounded gaps, then placement snaps accumulated positions. Six 28px profiler rows with 6px gaps overflowed a 498px content box by 16px at 1080p. WM uses expand(). Reconcile size and placement arithmetic with a measured regression.

### E2E focus-state isolation

`UIContext::reset()` retains `has_interacted`, while `focus_element` does not count as interaction. Alone/batch screenshots differ. WM clears capture state and explicitly sends Tab in focus tests. Define a common test-reset contract without clearing ordinary interaction history every screen change. AIM also had a 17×17 disabled-maximize fill difference between isolated and batch capture.

### Validation and checkbox follow-ups

`enforce_min_touch_target` is exposed but has no registered validator. `ClearViolations`/`RenderOverlay` use the default collection and miss markers on split UI children. Native checkmarks still depend on a V glyph; WM draws its own. Test flag off/on, split/single collection cleanup, and custom-font checked/disabled controls. See the root todo for source references.

### Text measurement and rendering use different font inputs

Autolayout uses widget.font_name and spacing 1; rendering resolves weight and `1 + letter_spacing`. The assumption of a shared descriptor was wrong. Resolve actual family variant, size, spacing and wrapping once; test Dim::Text and styled/wrapped bounds against drawing. Source-confirmed; native regression still needed.

### Atlases alone do not reduce submitted draws in the measured screens

Cozy Cafe stayed at 66 draws/68 binds and Images at 59/61 after packing. Sharing a texture does not merge commands separated by render state. Attribute texture/scissor/shader/layer boundaries before coalescing; preserve painter order. Counts are driver measurements, not proof of a broken atlas API. See [performance](performance.md).

### Rounded parent corners do not clip children to the curve

`HasClipChildren` uses rectangular scissors; parent radius changes fill only. Optional rounded masks need explicit nested/backend semantics. Keep rectangular clipping fast. This is a deferred capability, separate from partial outlines and scroll-decoration clipping.

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
