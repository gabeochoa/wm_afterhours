# Afterhours UI demo: duplicate / combinable screen audit

No files modified. All 159 headers under `src/systems/screens/` were inspected (flag, category, description, distinctive widgets/behaviour); `src/screen_includes.gen` was compared against the build-generated list.

## 0. Counts and the `screen_includes.gen` discrepancy — read this first

- **159 files** in `src/systems/screens/` = **156 `REGISTER_EXAMPLE_SCREEN` registrations + 3 non-screens** with no registration:
  - `DialogBackdrop.h`, `DialogPresentation.h` — shared helpers included by the dialog/modal screens.
  - `KirbyArtwork.h` — artwork data included by `KirbyOptions.h`.
- **No registered screen is actually unbuilt.** The checked-in `src/screen_includes.gen` (123 lines, 120 screens) is stale: the `makefile` (lines 148–171) generates `output/objs/all/generated/screen_includes.gen` from `$(wildcard src/systems/screens/*.h)` and puts the generated directory ahead of `src/` on the include path, so `src/main.cpp: #include <screen_includes.gen>` resolves to the generated file. The generated file contains all 159 headers. The 36 registrations missing from the checked-in file are therefore **live, not dead** — the checked-in file should be deleted or regenerated, not treated as the source of truth.
- The 36 missing from `src/screen_includes.gen`, with flags:

| File | Flag |
|---|---|
| `ActiveTabStyles.h` | `active_tab_styles` |
| `AnimationStressLab.h` | `animation_stress_lab` |
| `BaselineAlignShowcase.h` | `baseline_align` |
| `ColourTrackLab.h` | `colour_track_lab` |
| `CommandPickerDemo.h` | `command_picker` |
| `CurvedTextLab.h` | `curved_text_lab` |
| `CurvedTextStressLab.h` | `curved_text_stress_lab` |
| `DisclosureShowcase.h` | `disclosure_showcase` |
| `ExampleColorSwatch.h` | `color_swatch_lab` |
| `FocusRingLab.h` | `focus_ring_lab` |
| `HostLoopLab.h` | `host_loop_lab` |
| `MotionButtonCheckpoint.h` | `motion_button` |
| `MotionModalCheckpoint.h` | `motion_modal` |
| `PodcastLibrary.h` | `podcast_library` |
| `PointerParityLab.h` | `pointer_parity_lab` |
| `PseudoLocaleLab.h` | `pseudo_locale_lab` |
| `RollingNumberLab.h` | `rolling_number` |
| `RotatedControlsShowcase.h` | `rotated_controls` |
| `RotatedSubtreeShowcase.h` | `rotated_subtree` |
| `ScrollbarLayerRepro.h` | `scrollbar_layer_repro` |
| `SmallComponentsLab.h` | `small_components_lab` |
| `StaleIndexLab.h` | `stale_index_lab` |
| `TerminalDemo.h` | `terminal` |
| `TextAreaLab.h` | `text_area_lab` |
| `TextAreaStatesLab.h` | `text_area_states_lab` |
| `TextCacheLab.h` | `text_cache_lab` |
| `TextMotionLab.h` | `text_motion_lab` |
| `TextScaleShowcase.h` | `text_scale` |
| `ThemeBuilderLab.h` | `theme_builder` |
| `TouchTargetDemo.h` | `touch_targets` |
| `TransitionsLab.h` | `transitions_lab` |
| `TransparentControlsLab.h` | `transparent_controls` |
| `TypographyCompositionLab.h` | `typography_composition` |
| `ValidationThrottleLab.h` | `validation_throttle_lab` |
| `VirtualListHeightsLab.h` | `virtual_list_heights_lab` |
| `WmDefaultsLab.h` | `wm_defaults_lab` |

Also note: `src/systems/ExampleButtons.h` (`buttons`), `ExampleCheckboxes.h` (`checkboxes`), `ExampleDropdowns.h` (`dropdowns`), `ExampleSliders.h` (`sliders`), `ExampleComponentShowcase.h` (`showcase`) contain `REGISTER_EXAMPLE_SCREEN` but are included by nothing, so they are currently dead. `buttons`/`checkboxes`/`dropdowns` would collide with the same flags in `src/systems/screens/` if ever included. `sliders` has no counterpart in `screens/` at all — `Forms.h` is the de-facto slider demo.

General caveat for every merge below: several screens exist as **open-by-default screenshot fixtures** (dialogs, menus, popovers). Merging them into tabbed screens loses one-baseline-per-screen unless the screenshot harness gains a mode/parameter. That is a reason to keep some groups separate, flagged where it applies.

## 1. Animation / motion — 14 screens, the worst duplication

| File | Flag | Distinctive content |
|---|---|---|
| `AnimationBasicDemo.h` | `animation_basic` | Legacy `plugins/animation.h`: fade/slide/scale |
| `AnimationInteractiveDemo.h` | `animation_interactive` | Legacy API: 3 buttons, same press-scale |
| `AnimationLoopingDemo.h` | `animation_looping` | No animation plugin: manual pulse/bounce/spin loops |
| `AnimationSpringDemo.h` | `animation_spring` | Manual spring/bounce, no `ui_motion` |
| `AnimationDeclarativeDemo.h` | `animation_declarative` | New `ui_motion`: trigger blocks, springs |
| `TransitionsLab.h` (1,925 lines) | `transitions_lab` | New `ui_motion`: **all 43 transitions** on working controls |
| `AnimationStressLab.h` | `animation_stress_lab` | New API at scale + `text_unit_motion`, frame stats |
| `ColourTrackLab.h` | `colour_track_lab` | New API: colour tracks, OKLab interpolation, alpha/spring backgrounds |
| `CurvedTextLab.h` | `curved_text_lab` | Grapheme-aware curved text, languages/fonts |
| `CurvedTextStressLab.h` | `curved_text_stress_lab` | Same curved-text API, stage timings/stress |
| `TextMotionLab.h` | `text_motion_lab` | `text_unit_motion`: per-word/unit text animation, 9 rows |
| `RollingNumberLab.h` | `rolling_number` | `rolling_number` plugin, blur/reduced-motion |
| `MotionButtonCheckpoint.h` | `motion_button` | One button, motion-rewrite checkpoint |
| `MotionModalCheckpoint.h` | `motion_modal` | Modal enter/visible/exit/hidden contract |

**Overlap:** Basic/Interactive/Looping/Spring are four 169–236-line screens teaching the same pre-`ui_motion` concepts with the same dark background and coloured boxes. Declarative is a subset of what `TransitionsLab` already demonstrates (triggers, springs, fades/scales). CurvedTextStressLab is CurvedTextLab under load.

**Recommendation — merge aggressively:**
- Merge `animation_basic` + `animation_interactive` + `animation_looping` + `animation_spring` into one `AnimationLab` with Basic / Looping / Spring / Interactive modes. Keep it only as the legacy-API record; otherwise archive it in favour of `TransitionsLab`.
- Merge `animation_declarative` into `transitions_lab` as a trigger-block mode.
- Merge `curved_text_stress_lab` into `curved_text_lab` as a stress toggle.
- Keep separate: `transitions_lab` (catalogue), `animation_stress_lab` (perf), `colour_track_lab` (colour-track API only), `text_motion_lab` (text-unit API only).
- **Archive/move to tests:** `motion_button`, `motion_modal` — single-control checkpoints superseded by `TransitionsLab`/`ModalShowcase`; they add no gallery value. `rolling_number` could become a mode of `text_motion_lab`, but its plugin is distinct enough to keep if cheap.

## 2. Dialogs / modals — 8 screens + 2 helpers

| File | Flag |
|---|---|
| `DialogConfirmShowcase.h` | `dialog_confirm` |
| `DialogDangerShowcase.h` | `dialog_danger` |
| `DialogFyiShowcase.h` | `dialog_fyi` |
| `DialogInfoShowcase.h` | `dialog_info` |
| `DialogPromptShowcase.h` | `dialog_prompt` |
| `ModalShowcase.h` | `modals` |
| `ModalAdvanced.h` (857 lines) | `advanced_modals` |
| `RealWorldModals.h` | `real_world_modals` |

**Overlap:** All five `Dialog*Showcase` screens share `DialogBackdrop.h` + `DialogPresentation.h`, the same `neon_dark` theme, and `open=true` so screenshots capture the dialog. They differ only in the `modal::` variant: confirm (with countdown), danger-confirm, fyi (3-button, with a text editor behind it), info (single acknowledge), prompt (input + validation). `ModalShowcase` already contains info/confirm/fyi plus basic/composable/stacked modals, closed by default. `ModalAdvanced` (login, wizard, settings, feedback forms) overlaps `RealWorldModals` (input-gated delete confirm, command palette, long-content/TOS, drawer, sheet, cookie).

**Recommendation — merge, with the baseline caveat:**
- One `DialogGallery` with a variant selector (Confirm / Danger / FYI / Info / Prompt), each mode opening by default — preserves all five fixtures, deletes five near-identical scaffolds. Only do this if the screenshot harness can capture per-mode baselines; otherwise keep the five as fixtures and say so.
- Merge `advanced_modals` + `real_world_modals` into one `AdvancedModals` (forms/wizard + palette/scroll/drawer/sheet/delete-gating are complementary sections, not separate screens).
- Keep `modals` as the basic catalogue, minus its duplicated info/confirm/fyi variants once the gallery exists.

## 3. Text input / text area — 6 screens

| File | Flag | Distinctive content |
|---|---|---|
| `TextInputDemo.h` | `text_input` | Realistic profile/search form; readonly/disabled/password |
| `TextInputLab.h` | `text_input_lab` | Single input + live caret/selection/scroll readout; overflow field |
| `TextInputSizes.h` | `text_input_sizes` | Same widget at 7 heights (24–100px), cursor scaling |
| `TextAreaLab.h` | `text_area_lab` | `text_area` parity with `text_input`: placeholder, backgrounds, focus ring, auto-grow |
| `TextAreaStatesLab.h` | `text_area_states_lab` | `text_area`: editable / readonly / disabled only (72 lines) |
| `ComposerLab.h` | `composer_lab` | `text_area`: wrap, auto-grow, Enter vs Shift+Enter, no-wrap clipping |

**Overlap:** `TextInputLab`'s own comment says `TextInputDemo` shows the realistic form and it shows internals — same widget, complementary views. `TextAreaStatesLab` is a strict subset of `TextAreaLab`'s parity concerns. `ComposerLab` exercises the same `text_area` multiline features `TextAreaLab`'s auto-grow covers.

**Recommendation — merge into two screens:** one `TextInput` screen with Gallery / Sizes / Internals tabs (`text_input` + `text_input_sizes` + `text_input_lab`); one `TextArea` screen with Parity / States / Composer tabs (`text_area_lab` + `text_area_states_lab` + `composer_lab`). Nothing genuinely distinct is lost; both keep their current content as sections.

## 4. Text rendering / typography — ~12 screens

| File | Flag | Content |
|---|---|---|
| `ExampleText.h` | `text` | Sizes, colours page, guides |
| `ExampleTextOverflow.h` | `text_overflow` | Overflow debug indicators, comparison mode |
| `ExampleTextShadow.h` | `text_shadow` | Native drop shadow |
| `ExampleTextStroke.h` | `text_stroke` | Native stroke/outline |
| `StyledTextLab.h` | `styled_text_lab` | Per-span font weight |
| `FontWeightShowcase.h` | `font_weight` | Every `FontWeight`, mixed weights on one row |
| `TextScaleShowcase.h` | `text_scale` | Global `Theme::text_scale` accessibility scaling |
| `TypographyCompositionLab.h` | `typography_composition` | Expressive whole-word typography, optional motion |
| `MultiLineTextLab.h` | `multiline_text_lab` | Hard breaks/wrapping/self-sizing; layout-vs-render line-count agreement |
| `TextCacheLab.h` | `text_cache_lab` | Wrap memo, styled-run columns, glyph coverage, `text_inset` |
| `LanguageDemo.h` | `language_demo` | Multi-language font support |
| `PseudoLocaleLab.h` | `pseudo_locale_lab` | Pseudo-localisation over a fake social app |
| `LocalizedInputPrompts.h` | `localized_input_prompts` | Keyboard prompts, EN/KO word order |
| `AutoTextColorShowcase.h` | `auto_text_color` | Auto contrast, theme candidates, overrides |

**Overlap:** Shadow/Stroke/FontWeight/StyledText are all "one text-effect specimen grid" screens. MultiLine/Overflow both show wrapping/clipping disagreement. Language/PseudoLocale/LocalizedInputPrompts are all i18n stress. TypographyComposition overlaps StyledText (weights) and TextMotion (motion).

**Recommendation:**
- Merge `text_shadow` + `text_stroke` + `font_weight` + `styled_text_lab` into one `TypographyEffects`/`StyledText` lab with tabs. Keep `ExampleText.h` as the basic sizes/colours gallery, or fold it in as the first tab.
- Merge `multiline_text_lab` + `text_overflow` into one wrapping/overflow lab. Keep `text_cache_lab` (internal caches/glyph coverage — genuinely distinct, diagnostic) and `text_scale` (accessibility scaling — distinct) separate.
- Merge `language_demo` + `pseudo_locale_lab` + `localized_input_prompts` into one `InternationalizationLab` (fonts / pseudo-locale / word-order tabs).
- Fold `typography_composition` into the styled-text lab or `text_motion_lab`; fold `auto_text_color` into the contrast group in §10.

## 5. Scroll / scrollbar / virtual list — 8 screens

| File | Flag | Distinctive content |
|---|---|---|
| `ScrollViewShowcase.h` | `scroll_view` | Basic scrollable container |
| `SyncScrollLab.h` | `sync_scroll_lab` | `sync_group`: locked panes, header tracking a wide table |
| `ScrollbarStyleLab.h` | `scrollbar_style_lab` | Track/thumb colours, thickness; 4 panes |
| `ScrollbarLayerRepro.h` | `scrollbar_layer_repro` | Modal layering, background scroll isolation |
| `ScrollClickBug.h` | `scroll_click_bug` | Click hit-testing after scroll offset |
| `ScrollClipBugRepro.h` | `scroll_clip_bug` | Nested clip-container scissoring, 4 panels |
| `VirtualListLab.h` | `virtual_list_lab` | 10k-row windowed list, bounded row pool |
| `VirtualListHeightsLab.h` | `virtual_list_heights_lab` | Variable heights, retained index, prepend anchor, `scroll_to_bottom` |

**Overlap:** All are `HasScrollView` facets. The three `*Bug`/`*Repro` screens are regression fixtures, not galleries.

**Recommendation — one gallery + regression fixtures:**
- Merge `scroll_view` + `sync_scroll_lab` + `scrollbar_style_lab` into one `ScrollLab` (Basic / Synced / Styled modes).
- Merge `virtual_list_lab` + `virtual_list_heights_lab` into one `VirtualListLab` with fixed/variable-height modes; prepend-anchor/`scroll_to_bottom` is a mode, not a screen.
- Keep `scroll_click_bug`, `scroll_clip_bug`, `scrollbar_layer_repro` as `Tools` regression fixtures — or better, consolidate the two scroll bug repros into `LayoutBugRepros.h` (§9), which already exists for exactly this purpose. Keep `scrollbar_layer_repro` separate only because it is a modal-layering repro, not a scroll repro.

## 6. Borders / separators / dividers / frames — 7 screens

| File | Flag | Content |
|---|---|---|
| `ExampleBorders.h` | `example_borders` | Border styling options (blueprint aesthetic) |
| `ExampleBevelBorders.h` | `bevel_borders` | Bevel styles/thickness |
| `ExampleNineSliceBorders.h` | `nine_slice_borders` | 9-slice border textures (fantasy parchment) |
| `DecorativeFrameShowcase.h` | `decorative_frame` | Decorative frame border styles (craft theme) |
| `ExampleSeparators.h` | `separators` | `separator()` horizontal/vertical/labeled (92 lines) |
| `DividerShowcase.h` | `divider_lab` | `divider()` as draggable bar and as plain rule |
| `SplitPaneLab.h` | `split_pane_lab` | Resizable split panes with draggable dividers |
| (see also) `SmallComponentsLab.h` | `small_components_lab` | `divider_line` as a static rule, plus settings helpers |

**Overlap:** The four border screens are specimen grids differing only in border mechanism; nine-slice needs textures, bevel needs light/dark edges, but the presentation is identical. `separator()` vs `divider()`-as-rule vs `divider_line` are three spellings of a static rule, spread across three screens; `divider()`-as-draggable-bar is the same widget `SplitPaneLab` already exercises in context.

**Recommendation:**
- Merge the four border screens into one `BorderLab` with Standard / Bevel / Nine-Slice / Decorative tabs.
- Merge `separators` + `divider_lab` + the `divider_line` section of `small_components_lab` into one `DividerLab` (Static rule / Labeled / Draggable modes). Keep `split_pane_lab` separate — it tests pane resizing, not the divider widget — and cross-link it.

## 7. Settings mockups — 10 screens, keep separate (with a caveat)

`AngryBirdsSettings.h` (`angry_birds_settings`), `CasualSettings.h` (`casual_settings`), `DeadSpaceSettings.h` (`deadspace_settings`), `FlightOptions.h` (`flight_options`), `IslandsTrainsSettings.h` (`islands_trains_settings`), `KirbyOptions.h` (`kirby_options`), `MiniMotorwaysSettings.h` (`mini_motorways_settings`), `ParcelCorpsSettings.h` (`parcel_corps_settings`), `PowerWashSettings.h` (`powerwash_settings`), `SportsSettings.h` (`sports_settings`). 297–682 lines each.

**Overlap:** All are themed settings menus; most use hand-drawn buttons and almost no shared widgets (only MiniMotorways/ParcelCorps/Sports use `slider()` at all; none use `setting_row`/`settings_list`; Kirby alone uses `text_input` for a profile). As *widget* coverage they are almost entirely redundant with `SettingRowShowcase`/`Forms` and each other.

**Distinct:** Only art direction, theme, and a few preferences (Kirby profile editing, Sports' slider-heavy layout, Flight's editable local preferences, MiniMotorways' minimalism). That is the entire point of a mockup gallery.

**Recommendation — keep separate, do not merge.** Merging into a theme-switchable settings screen would destroy the bespoke per-game drawing that justifies them, for no widget-coverage gain. Instead: declare a moratorium on new ones, and treat them as the visual-mockup gallery. If screen count must fall, these are candidates for archival as a group — but not for merging into each other.

Related, genuinely combinable: `SettingRowShowcase.h` (`setting_row_showcase`) vs `SmallComponentsLab.h` (`small_components_lab`) vs `Forms.h` (`forms`). `SmallComponentsLab` is `settings_list`/`settings_row` + `value_pill` + `divider_line` — i.e. the same helpers `SettingRowShowcase` demonstrates, plus a pill readout. `Forms.h` is sliders + checkboxes + dropdowns arranged as a settings form, duplicating the individual component showcases. **Merge** `small_components_lab` into `setting_row_showcase` as sections; keep `forms` only as a composite integration screen, or fold it into the same screen.

## 8. Buttons / controls galleries — 3 button screens + legacy duplicates

| File | Flag | Content |
|---|---|---|
| `Buttons.h` | `buttons` | Button gallery: states, sizes, groups |
| `ButtonVariantsShowcase.h` | `button_variants` | Filled / Outline / Ghost variants + sizes |
| `ExampleSimpleButton.h` | `simple_button` | One button + click counter (74 lines, `Tools`) |
| `CheckboxShowcase.h` | `checkboxes` | All checkbox types |
| `RadioGroupShowcase.h` | `radio_buttons` | Radio groups (uses tray navigation) |
| `ToggleSwitchShowcase.h` | `toggle_switches` | Pill/round toggles, disabled states |
| `StepperShowcase.h` | `stepper_showcase` | Steppers + card selector |
| `DropdownShowcase.h` | `dropdowns` | Basic/labeled/many/disabled/styled dropdowns |

**Overlap:** `buttons` vs `button_variants` is the same gallery split by variant-vs-state organisation, with sizes shown in both. `simple_button` is a trivial subset. Keep `checkboxes`/`radio_buttons`/`toggle_switches`/`stepper_showcase`/`dropdowns` separate — distinct widgets — but note the repeated option data (resolutions/quality/languages) across `Forms`, `StepperShowcase`, and `DropdownShowcase` is shared content, not a merge reason.

**Recommendation:** Merge `button_variants` into `buttons` as variant tabs; archive `simple_button` (move to E2E/tests — a click counter proves nothing a gallery doesn't); delete the dead legacy headers in §0.

## 9. Layout primitives / patterns — 13 screens

| File | Flag | Content |
|---|---|---|
| `HStackShowcase.h` | `hstack_showcase` | `hstack()` primitive |
| `VStackShowcase.h` | `vstack_showcase` | `vstack()` primitive — same structure, mirrored |
| `ExampleFlexAlignment.h` | `flex_alignment` | Flex justify/align, tabbed (Overview/Vertical/Horizontal/Cross-axis/Self align) |
| `SelfAlignShowcase.h` | `self_align` | `SelfAlign` centering |
| `BaselineAlignShowcase.h` | `baseline_align` | Row baseline alignment, mixed-size text |
| `ExampleLayout.h` | `layout` | Row/column containers |
| `GridLab.h` | `grid_lab` | `grid()`/`grid_cell()`, cross-row column alignment |
| `AbsolutePositioningShowcase.h` | `absolute_positioning` | Translate/corner-pinning/mixed/screen_pct |
| `FitContentLab.h` | `fit_content_lab` | `with_fit_content()` chat bubbles |
| `RotatedControlsShowcase.h` | `rotated_controls` | Rotated buttons, hit areas, focus rings |
| `RotatedSubtreeShowcase.h` | `rotated_subtree` | Rotated container, children paint/click rotated |
| `LayoutPatterns.h` | `layout_patterns` | Minimal fixed-size layouts for agent E2E |
| `LayoutAcidTest.h` | `layout_acid_test` | Visual correctness target for layout/clipping |
| `LayoutBugRepros.h` | `layout_bug_repros` | Seven retained layout regression fixtures |
| `RectAlgebraLab.h` | `rect_algebra_lab` | `rect::cut/pad/expand/align` drawn directly |

**Overlap:** HStack/VStack are one screen with an axis toggle. SelfAlign is already a tab inside FlexAlignment. Baseline alignment is a flex-alignment concern. Rotated-controls vs rotated-subtree differ only in whether the rotation is applied to one widget or a container — one screen with two sections. LayoutPatterns/AcidTest are both small fixed E2E/visual fixtures.

**Recommendation:**
- Merge `hstack_showcase` + `vstack_showcase` → one `StackShowcase` (axis toggle).
- Merge `self_align` + `baseline_align` into `flex_alignment` (it already has a Self-align tab).
- Merge `rotated_controls` + `rotated_subtree` → one `RotationLab`.
- Merge `layout_patterns` + `layout_acid_test` into one E2E/visual fixture screen; keep `layout_bug_repros` as the regression bundle (and move `scroll_click_bug`/`scroll_clip_bug` into it, §5).
- Keep separate: `layout`, `grid_lab`, `absolute_positioning`, `fit_content_lab`, `rect_algebra_lab` — each covers a distinct primitive/mechanism.

## 10. Themes / colour / contrast — 10 screens

| File | Flag | Content |
|---|---|---|
| `Themes.h` | `themes` | Real-time preset switching |
| `DefaultTheme.h` | `default_theme` | A workspace rendered with the default theme |
| `ThemeBuilderLab.h` (857 lines) | `theme_builder` | Token table + live preview + undo/redo + import/export |
| `ExampleColors.h` | `colors` | Theme colour swatches |
| `ExampleColorSwatch.h` | `color_swatch_lab` | `color_swatch` picker variants (popover editors) |
| `BlendModeLab.h` | `blend_mode_lab` | `blend::Mode`, `blend_scope`, transition counter |
| `AlphaBlendRepro.h` | `alpha_blend_repro` | Translucent overlays over text (open cross-project repro) |
| `TransparentControlsLab.h` | `transparent_controls` | Transparent native controls on light/dark/patterned backgrounds |
| `AutoTextColorShowcase.h` | `auto_text_color` | Automatic text contrast |
| `WmDefaultsLab.h` | `wm_defaults_lab` | WM-adopted defaults: release clicks, outset focus ring, font-scaled text-area rows |

**Overlap:** `colors` is a swatch gallery inside what `themes` already shows per preset. `TransparentControlsLab` and `AutoTextColorShowcase` both answer "is text/controls legible on this background" — same question, different mechanism. `WmDefaultsLab` is a small defaults demo that could live on the theme screens.

**Recommendation:** Merge `colors` into `themes` as a swatches tab; merge `transparent_controls` + `auto_text_color` into one contrast/legibility lab; fold `wm_defaults_lab` into `default_theme`/`themes` as a defaults section (or keep — it is small and specific). Keep `theme_builder` (full editor), `color_swatch_lab` (picker widget), `blend_mode_lab` and `alpha_blend_repro` (rendering-correctness repros) separate.

## 11. Floating layers / navigation — 10 screens

| File | Flag | Content |
|---|---|---|
| `PopoverLab.h` | `popover_lab` | `popover()` with controls inside, edge flip |
| `MenuShowcase.h` | `menu_showcase` | Anchored dropdown menus, edge flip |
| `TooltipLab.h` | `tooltip_lab` | `with_tooltip`, corner/edge placement, flipping |
| `ContextMenuLab.h` | `context_menu_lab` | Right-click `context_menu()`, edge flip |
| `TabContainerShowcase.h` | `tab_container` | Tab container component |
| `ActiveTabStyles.h` | `active_tab_styles` | Default and caller-styled active tabs — same component as above |
| `NavigationBarShowcase.h` | `navigation_bar_demo` | Navigation bar widget |
| `PaginationShowcase.h` | `pagination` | Pagination widget |
| `TrayShowcase.h` | `tray` | Single-tab-stop tray, arrow-key selection |
| `DisclosureShowcase.h` | `disclosure_showcase` | Expand/collapse sections |
| (related) `ExampleTabbing.h` | `tabbing` | Keyboard tabbing navigation |
| (related) `FocusRingLab.h` | `focus_ring_lab` | `:focus-visible`, ring offsets, highlight modes |

**Overlap:** Popover/Menu/Tooltip/ContextMenu all exercise the same anchored-placement + edge-flip machinery and explicitly cross-reference each other; they differ in trigger (hover, anchor, right-click, arbitrary content) and content. `ActiveTabStyles` is purely a styling variant of `tab_container`. Tray/Tabbing/FocusRing are three views of keyboard focus.

**Recommendation:** Merge `active_tab_styles` into `tab_container` unconditionally (same widget, styling tabs). For the four anchored-layer screens: merging into one `AnchoredLayersLab` (Popover / Menu / Tooltip / Context-Menu tabs) is the clean consolidation, but each is an open-by-default screenshot fixture — keep separate if per-screen baselines matter, merge if they don't. Keep `navigation_bar_demo`, `pagination`, `disclosure_showcase` separate (distinct widgets). Keep `tray` separate but cross-link `tabbing`/`focus_ring_lab`; folding all three focus screens into one would bury genuinely different mechanisms (single-stop arrow navigation vs tab order vs ring rendering).

## 12. Drag, progress, gap galleries, and other small groups

- **Drag:** `DragDropShowcase.h` (`drag_drop`: boards + horizontal priority lanes) vs `HorizontalDragShowcase.h` (`horizontal_drag`: priority lanes only). Horizontal is a strict subset. **Merge `horizontal_drag` into `drag_drop` as a mode.**
- **Progress:** `CircularProgressShowcase.h` (`circular_progress`: radial indicators) vs `MetersGaugesShowcase.h` (`meters_gauges`: inline meters, paired bars). Same family, different geometry. **Merge into one Progress/Gauges screen** with radial + linear sections.
- **"Gap" galleries:** `ConfigGapGallery.h` (`config_gap_gallery`: shadow/clip/insets/`on_draw`/click-activation config) vs `WidgetGapGallery.h` (`widget_gap_gallery`: `button_group`, pagination, spacer, image, `icon_row` — shipped widgets with no other consumer). Same purpose (coverage of unshowcased API), disjoint content. **Merge into one `GapGallery`** with Config / Widgets tabs.
- **Cards/decorators:** `Cards.h` (`cards`) vs `DecoratorShowcase.h` (`decorators`) vs `ConfigGapGallery` — no merge; cards are layout specimens, decorators are higher-order components. Keep.
- **Command execution:** `CommandPickerDemo.h` (`command_picker`: search/run categorised commands) vs `TerminalDemo.h` (`terminal`: register/run developer commands). Same job, two UIs (palette vs terminal). **Merge into one developer-tools screen** with Picker / Terminal modes, or keep both and note the duplication — the UIs are the content.
- **Index staleness (borderline):** `EntityIndexLab.h` (`entity_index_lab`: parent→children secondary index) vs `StaleIndexLab.h` (`stale_index_lab`: selection indices clamping when options shrink). Related "index goes stale" theme, different mechanisms. **Optional merge** into one `IndexLab`; keeping both is defensible.
- **Profiling (borderline):** `HostLoopLab.h` (`host_loop_lab`: request-frame/frame-wake/target FPS) vs `SystemProfileLab.h` (`system_profile_lab`: frame/system profiling) vs `ChartLab.h` (`chart_lab`: interactive timing charts). HostLoop and SystemProfile overlap on frame timing; ChartLab visualises it. **Consider** HostLoop as a section of SystemProfileLab; keep ChartLab separate.

## 13. Genuinely distinct — keep separate, no merge recommended

Game/app mockups whose value is the specific flow or artwork, not a widget: `FighterMenu`, `CozyCafe`, `EmpireTycoon`, `NeonStrike`, `RubberBanditsMenu`, `PotionCrafting`, `ShopInterface`, `FileTreeShowcase`, `SecureTunnelMockup`, `OffsiteBackupApp`, `AIMChatDemo`, `MediaLibraryGrid`, `KartSelect`/`MarloKart`/`RaceResults` (select / playable race / results — note `MarloKart` already contains title/select/race/podium, so the other two are extractable modes of it if count must fall), `MinesweeperLab`, `GuessWhoLab` (stress tests, distinct), `PinchZoomLab`, `PointerParityLab`, `HoverLab`, `TouchTargetDemo`, `AdaptiveScalingDemo`, `GridLab`, `ImageShowcase`, `ExampleAccessibility`, `LayoutBugRepros`, `EntityIndexLab`, `ValidationThrottleLab`, `TextCacheLab`, `ThemeBuilderLab`, `ExampleColorSwatch`.

## Summary of recommended actions

| Action | Screens affected |
|---|---|
| Merge into one animation lab + fold declarative/curved-stress; archive motion checkpoints | §1: 14 → ~6 |
| One dialog gallery; merge advanced + real-world modals | §2: 8 → 3 |
| Two screens (TextInput, TextArea) instead of six | §3: 6 → 2 |
| Typography effects lab, wrapping lab, i18n lab | §4: 14 → ~7 |
| One scroll lab + one virtual-list screen; bug repros to `LayoutBugRepros` | §5: 8 → 3 + fixtures |
| One border lab, one divider lab | §6: 7 → 2 (+ SplitPane kept) |
| Merge settings-helper screens; keep 10 game settings mockups | §7: 3 → 1, mockups kept |
| Merge button variants; archive simple button | §8: 3 → 1 |
| Stack, flex/baseline/self-align, rotation, E2E-fixture merges | §9: 15 → ~10 |
| Theme swatches fold, contrast fold, WM-defaults fold | §10: 10 → ~7 |
| Tab-styles fold; anchored-layer merge optional (baseline-dependent) | §11: 12 → 10–11 |
| Drag, progress, gap-gallery, command-tool merges | §12: 8 → 4 |

Net effect of the firm (non-optional) recommendations: roughly **156 → ~115 screens**, with the largest wins in animation, dialogs, text input, typography, scroll, and borders — and with the 10 settings mockups and the game/app mockups deliberately preserved.
