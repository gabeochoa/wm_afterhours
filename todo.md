# Project todo

Remaining library work and external checks follow.
See [gaps](docs/AFTERHOURS_GAPS.md) for evidence and [history](docs/history.md) for completed work.

## Screen polish & duplicate consolidation (intern handoff)

Full per-screen recommendations (all 156 screens × 5 items): [SCREEN_POLISH_AUDIT.md](docs/SCREEN_POLISH_AUDIT.md).
Full duplicate / merge analysis (156 → ~115 screens): [DUPLICATE_SCREENS_AUDIT.md](docs/DUPLICATE_SCREENS_AUDIT.md).

**How to verify any screen change locally:**
- Build: `make output` (or if `pkg-config` is slow, pass explicit `RAYLIB_FLAGS` / `RAYLIB_LIB`).
- Capture one screen headlessly: `./output/ui_tester.exe --headless-screenshots /tmp/check --screen <flag_name>` and inspect `/tmp/check/<flag_name>_720p.png` (also check stderr for `LOG_WARN`).
- Run unit + layout tests: `./output/ui_tester.exe --run-all-tests && make test-layout`.
- Design bar: quiet surface layering over harsh 1px borders; reserve saturated color for active marks/selection (avoid `Theme::Usage::Secondary` bright-green fills on unselected `ocean_navy` controls); strip `"in this demo"` / `"local preview"` meta-disclaimers out of realistic mockups; avoid Unicode arrows (`←→↑↓─`) in `AtkinsonMock` labels (draw vector lines with `raylib::DrawLineEx` instead).

### Completed
- [x] Batches 1–9 visual polish (42 screens polished, 6 runtime layout/font warnings fixed, Escape closes interactive window unless modal/navigator/dropdown/text input is active).
- [x] Codebase hygiene: deleted 5 dead legacy headers in `src/systems/` (`ExampleButtons.h`, `ExampleCheckboxes.h`, `ExampleDropdowns.h`, `ExampleSliders.h`, `ExampleComponentShowcase.h`) and stale checked-in `src/screen_includes.gen`.

### P1 — Quick visual & copy wins (great first PRs)
- [ ] **Strip `"in this demo"` / `"local preview"` meta-disclaimers from realistic mockups & dialogs:** `DialogInfoShowcase.h`, `IslandsTrainsSettings.h`, `RubberBanditsMenu.h`, `SettingRowShowcase.h`, `ShopInterface.h` (Sell tab), `OffsiteBackupApp.h` (Restore detail), `AIMChatDemo.h` (Help/Info copy), `SecureTunnelMockup.h` (Settings modal).
- [ ] **Audit `ocean_navy` screens using `Theme::Usage::Secondary` (bright `#0C5E2A` green) on non-primary buttons/tabs** (same fix we applied to `Themes.h` and `LanguageDemo.h`): `AutoTextColorShowcase.h`, `ButtonVariantsShowcase.h`, `Buttons.h`, `ModalAdvanced.h`, `RealWorldModals.h`.
- [ ] **Check remaining `─` box-drawing chars in comments vs labels:** verify `AdaptiveScalingDemo.h`, `ScrollClipBugRepro.h`, and `ToggleSwitchShowcase.h` emit zero missing-glyph warnings in `make run`.
- [ ] **Fix leaf-label `with_padding` warning candidate:** audit `PseudoLocaleLab.h` (swap `with_padding` on leaf labels to `with_text_inset`).
- [ ] **Normalize mixed corner radii (`>=4` distinct 1–14px values in one screen)** to a clean 3-step scale (`4px` controls / `8px` inner cards / `12px` outer panels): `AnimationBasicDemo.h`, `AnimationInteractiveDemo.h`, `AnimationLoopingDemo.h`, `Cards.h`, `CheckboxShowcase.h`, `ContextMenuLab.h`, `DecoratorShowcase.h`, `DragDropShowcase.h`, `EmpireTycoon.h`, `EntityIndexLab.h`, `ExampleColors.h`, `ExampleLayout.h`, `FitContentLab.h`, `FontWeightShowcase.h`, `Forms.h`, `GuessWhoLab.h`, `HorizontalDragShowcase.h`, `ImageShowcase.h`, `LanguageDemo.h`, `PaginationShowcase.h`, `ParcelCorpsSettings.h`, `PseudoLocaleLab.h`, `RubberBanditsMenu.h`, `ScrollViewShowcase.h`, `TabContainerShowcase.h`, `TransitionsLab.h`.
- [ ] **Check & fix any remaining `LOG_WARN` messages across all 156 screens:** run `./output/ui_tester.exe --headless-screenshots /tmp/all_check` (or cycle through screens in `make run`) and fix any unintentional `Layout overflow`, `Container too small for text`, or `sets padding on a label with no children` warnings. (Note: warnings in `validation_throttle_lab`, `touch_targets`, `text_cache_lab`, and `styled_text_lab` are intentional test fixtures.)

### P1.5 — Pixel-verified corners, padding & alignment issues (16 screens × 5 issues)
Every item below was verified directly in `output/*_720p.png` screenshots.

- [ ] **`casual_settings` (`src/systems/screens/CasualSettings.h`)**
  1. `"Preferences apply immediately. Kept for this session."` collides with the curved bottom peach rim — add bottom padding or lift it `12px` inside the inner card.
  2. `VIBRATION` icon stays bright blue while OFF — dim/desaturate OFF icons (matching `AngryBirdsSettings.h`).
  3. `"NOTIFICATIONS / OFF"` text sits off-center next to the toggle switch — left-align cleanly with symmetric horizontal padding inside the blue pill.
  4. `Build / Version / Player ID` block sits tight below `LANGUAGE` — increase top gap by `10–12px` and align values on a clean left column.
  5. Top-right `1000 +` coin pill is barely readable dark-on-dark — raise text contrast or add a lighter border.
- [ ] **`flight_options` (`src/systems/screens/FlightOptions.h`)**
  1. `"Standard"` inside the right `CURRENT SELECTION` box is randomly indented `~50px` right relative to `"Control Type"` and `"Saved: Standard"` — align flush left with the card's inner padding.
  2. Left nav column has no border while the right card has a bright `1px` cyan outline — unify border/elevation strategy across all 3 columns.
  3. `"Enter activates the focused control."` sits cramped under `[Enter]` with tiny bottom padding inside the footer bar — give the footer symmetrical top/bottom padding.
  4. `"APPLY & CLOSE"` and `"BACK"` float in the footer without consistent button frames or right-edge alignment.
  5. `AUDIO / LANGUAGE` section header crowds `GRAPHICS` above it — normalize vertical section-header spacing (`+8px` top margin).
- [ ] **`islands_trains_settings` (`src/systems/screens/IslandsTrainsSettings.h`)**
  1. Strip `"Changes apply immediately in this demo."` meta-copy beneath `SETTINGS`.
  2. `"PLAY TUTORIAL"` has its `▶` triangle pinned far left while the label is centered — place icon `10px` to the left of the centered text.
  3. Align `"RESET TO DEFAULTS"` and `"All six settings"` symmetrically on the bottom row (right-align `"All six settings"` flush with the controls above).
  4. Right-align `"Low 0 / 10 High"` flush with the right edge of the `Pan speed` segmented bar.
  5. Unify border/corner styling across `Keyboard controls >` (borderless) and `PLAY TUTORIAL` (`1px` dark outline).
- [ ] **`kirby_options` (`src/systems/screens/KirbyOptions.h`)**
  1. Remove the duplicate overlapping purple `"Options"` banner glued onto the top of the white card (the purple `"Options"` tab directly above already names the active tab).
  2. Add vertical breathing room between the dark-brown `"Common"` pill and the top border of the `Controller` tile, and align `"Common"` + `"Controls & visibility"` on one clean baseline.
  3. Match corner radii: `Delete Data` uses `6px` rounded corners while the 5 big category tiles (`Controller`, `Display`, `Accessibility`, `Sound`, `Online`) use sharp `0px` corners.
  4. Give `"Choose a category to customize your rider."` clean vertical padding above the bottom purple hairline rule.
  5. Align left notepad `"Profile customization"` header and `"Race day notes"` list lines to a consistent inner padding inset.
- [ ] **`parcel_corps_settings` (`src/systems/screens/ParcelCorpsSettings.h`)**
  1. `"MSAA"` sub-label under `Anti-aliasing` touches the bottom row divider — increase row height or shift `Anti-aliasing` up `4px` for balanced vertical padding.
  2. `"Changes apply locally"` sits tight against the curved bottom phone bezel — add `10px` bottom safe-area inset (and trim meta-copy).
  3. Unify overlay card corner radii: `"LIVE · EDDCOATES"` uses sharp `0px` corners while `"Grow your delivery network"` and `"9 mph"` use `8px` rounded corners.
  4. Tighten the huge horizontal gap in `"9       mph"` inside the bottom-right speedometer card.
  5. Boost contrast on the `"Lv 0"` dark-on-dark-blue badge inside `"Grow your delivery network"`.
- [ ] **`potion_crafting` (`src/systems/screens/PotionCrafting.h`)**
  1. `"Marked tiles are used by this recipe."` touches the top border of the `Moonpetal / Fire Root / Sea Salt` cards — add `6–8px` vertical gap.
  2. Add inner top/bottom padding to the `Sea Salt × 3` and `Star Dust × 1` requirement boxes and strip `"Instant demo"` from `"45 s recipe / Instant demo / Makes 1 potion"`.
  3. Add subtle breathing room above the bottom hairline rule under `"Four recipes, one workbench."`.
  4. Round the sharp `0px` `Tab` / `Enter` keycaps slightly (`4px`) and center their labels with consistent horizontal padding.
  5. Tighten the `Gold 2,450` top-right box width so there isn't an oversized empty gap between `"Gold"` and `"2,450"`.
- [ ] **`rubber_bandits_menu` (`src/systems/screens/RubberBanditsMenu.h`)**
  1. Remove or reposition the stray cyan oval outline overlapping Earl the Bird's feet and the dollar bills.
  2. Center `"EARL THE BIRD / FAVOURITE"` cleanly between `LB` and `RB` and align `"1 of 4"` symmetrically inside the brown bar.
  3. Vertically center `"thia9uers"` and the `"Online"` pill inside the top cyan header bar.
  4. Give the top-right `"Supporter pack"` card symmetrical padding around the circular lightning badge on its right edge.
  5. Style `Enter` and `Esc` in the bottom cyan bar as snug keycap pills for instant scannability.
- [ ] **`neon_strike` (`src/systems/screens/NeonStrike.h`)**
  1. Close the oversized horizontal gap in `"35       / 210"` ammo readout.
  2. Replace meta-copy (`"HUD PREVIEW"`, `"Select equipment / reload the sample magazine"`, `"Sample combat values / keys 1-4 select streaks"`) with clean tactical HUD labels.
  3. Add bottom/right inner padding inside the bottom-right `GRENADE x2` and `MELEE x1` cards so text doesn't hug the border.
  4. Offset top compass `W` and `E` markers `4px` outward so they don't touch the circular compass ring.
  5. Give `EVENT HISTORY` a subtle `1px` border and `4px` corner radius so it separates cleanly from the dark canvas.
- [ ] **`buttons` (`src/systems/screens/Buttons.h`)**
  1. **Critical contrast bug:** `"Disabled"` text inside Row 1's disabled button is invisible dark-on-dark — raise disabled label contrast to readable muted foreground.
  2. Align Row 2 (`Sizes`) captions (`Small / 90 x 45`, `Medium / 120 x 45`, `Large / 180 x 60`) directly to the left edge (or center) of each button above them.
  3. Add subtle `1px` vertical dividers between `Day | Week | Month | Year` in Row 3 (`View selection`) so the segmented group reads as 4 distinct buttons.
  4. Restyle `"Guides: off"` at top-right as a quiet secondary bordered button so it doesn't compete with the `Primary` specimen.
  5. Center the vertical divider in Row 4 (`Custom`) evenly between `Teal` and `Rounded`.
- [ ] **`button_variants` (`src/systems/screens/ButtonVariantsShowcase.h`)**
  1. Replace bright `#0C5E2A` green on `"Show bounds"` (`Theme::Usage::Secondary`) with a quiet bordered navy button.
  2. Soften the bright `1px` white borders on the `Ghost / Disabled` button so it doesn't look *more* prominent than `Ghost / Enabled`.
  3. Align `"214 x 40px"` column spec cleanly over the button columns instead of above `"Filled"`.
  4. Vertically align `Small`, `Medium`, `Large`, and `Large outline` in the `Sizes` row on a shared centerline with even bottom padding above their captions.
  5. Add `6–8px` extra bottom padding inside the `Neon Dark`, `Cozy Kraft`, and `Sage` theme preview cards below the hex strings.
- [ ] **`dropdowns` (`src/systems/screens/DropdownShowcase.h`)**
  1. Add bottom padding inside `Gemstone / style comparison` card so `"2px requested border / 12px corners"` doesn't hug the bottom edge.
  2. Label the two small `"Placement: below, flip, then clamp"` wireframe diagrams (`Below`, `Flipped`) so the schematic is self-explanatory.
  3. Match left text inset on `"Pet"` (`Pet / built-in label`) with `"Alpha"` and `"January"` in neighboring cards.
  4. Right-align `[+]` in `Preset / custom markers` (`"Option A"` left, `"[+]"` right) to match the chevron alignment of all other dropdowns.
  5. Soften the `Disabled example` fill from near-black (`#0D1117`) to a muted surface with a subtle border so it matches the dark-card depth language.
- [ ] **`drag_drop` (`src/systems/screens/DragDropShowcase.h`)**
  1. Align `Earlier`, `Later`, `<`, `>`, and `To do / In progress / Done` onto one clean horizontal baseline inside the `Move task` bottom bar (fix the staggered staircase).
  2. Left-align card titles (`Design mockups`, `API endpoints`, `Setup CI`) beside the 6-dot grip to match `SPR-01 / Design` and `Medium priority`.
  3. Align `"2 of 8 completed"` cleanly under `"Board / Lanes"` or right-aligned under `"Reset board"` in the header.
  4. Boost contrast on disabled `Up` / `Down` pagination buttons at the bottom of each column.
  5. Add a matching subtle `1px` border to the `Move task` bottom bar so it matches the 3 column cards above.
- [ ] **`file_tree` (`src/systems/screens/FileTreeShowcase.h`)**
  1. Remove `"Switch data source at right"` pointer copy and add a subtle chevron on `"Project sources"`.
  2. Right-align the `Size` column header flush with the right edge of the file sizes (`19.4 KB`, `704 B`).
  3. Deduplicate the empty preview state (`"Select a file to read its source"` + `"Select a source file on the left"`) and replace the hardcoded local path `/Users/gabeochoa/p/wm_afterhours` with a relative project root label (`wm_afterhours/`).
  4. Equalize horizontal gaps between `Home`, `Up`, and `Refresh` toolbar buttons.
  5. Soften the hollow sharp-cornered `[]` file icons with `2px` rounded corners or a subtle document fold.
- [ ] **`meters_gauges` (`src/systems/screens/MetersGaugesShowcase.h`)**
  1. Increase bottom padding inside `Animated Progress` card below `"Pause"` / `"Restart"` to match its top padding.
  2. Increase bottom padding inside `Circular Gauges` card below `"Color names these fixtures; no live thresholds."`.
  3. Add bottom viewport breathing room below the footer line (`"Native progress bars + circular progress + slider..."`) and align `0%` / `100%` flush with the slider track ends.
  4. Format integer slider percentages cleanly (`65%`, `39%`) instead of noisy trailing `.00%`.
  5. Add subtle `1px` borders to all 5 dark section cards for crisp surface layering.
- [ ] **`stepper_showcase` (`src/systems/screens/StepperShowcase.h`)**
  1. Fix the doubled bottom-edge line inside the 5 left steppers (`Resolution`, `Quality`, `Language`, `Difficulty`, `Framerate`) so borders are crisp and uniform.
  2. Round or taper the square `0px` connector notch on top of the terracotta `Warrior` card so it harmonizes with the card's rounded corners.
  3. Tighten internal alignment inside the `Warrior` card (align shield icon and title/description neatly without a giant empty left column underneath the shield).
  4. Increase vertical spacing between `"3 / 4 | Default"` captions and the next stepper row below.
  5. Add a subtle `1px` border to `"Restore defaults"` at top-right so it reads clearly as a button.
- [ ] **`toasts` (`src/systems/screens/ToastShowcase.h`)**
  1. **Semantic color bug:** change `"[+] Success Toast"` from dark purple to semantic emerald green, and `"[!] Warning Toast"` from crimson-magenta to semantic amber (distinct from `"[x] Error Toast"` red).
  2. Add bottom padding inside `Simple toasts` card so `"Operation completed successfully!"` doesn't touch the bottom card edge.
  3. Match corner radius (`6px`) on `"<  Bottom center  >"` (currently sharp `0px`) with the buttons on the left, and close the `~70px` empty vertical gap between `"Last notification"` and `"None yet"`.
  4. Replace bright purple `"Clear toasts"` (`Theme::Usage::Secondary`) at top-right with a quiet bordered secondary button.
  5. Wire `"Sample deletion"` to a real 1-item undoable toast/counter instead of `"No item is deleted and no undo action is attached."`.

### P2 — Remaining per-screen polish (~114 screens)
- [ ] **Game & App Mockups:** `CasualSettings.h`, `FlightOptions.h`, `IslandsTrainsSettings.h`, `KirbyOptions.h`, `MarloKart.h`, `MediaLibraryGrid.h`, `NeonStrike.h`, `ParcelCorpsSettings.h`, `PodcastLibrary.h`, `PotionCrafting.h`, `RubberBanditsMenu.h` (see exact 5 items per screen in [SCREEN_POLISH_AUDIT.md](docs/SCREEN_POLISH_AUDIT.md)).
- [ ] **Component Galleries:** `Buttons.h`, `ButtonVariantsShowcase.h`, `ConfigGapGallery.h`, `DecorativeFrameShowcase.h`, `DecoratorShowcase.h`, `DefaultTheme.h`, `DialogDangerShowcase.h`, `DialogFyiShowcase.h`, `DialogInfoShowcase.h`, `DialogPromptShowcase.h`, `DisclosureShowcase.h`, `DragDropShowcase.h`, `DropdownShowcase.h`, `ExampleBevelBorders.h`, `ExampleBorders.h`, `ExampleNineSliceBorders.h`, `FileTreeShowcase.h`, `FontWeightShowcase.h`, `ImageShowcase.h`, `MetersGaugesShowcase.h`, `ModalAdvanced.h`, `RealWorldModals.h`, `ScrollViewShowcase.h`, `SettingRowShowcase.h`, `StepperShowcase.h`, `TextInputDemo.h`, `TextInputSizes.h`, `ToastShowcase.h`, `TransparentControlsLab.h`, `WidgetGapGallery.h`.
- [ ] **Animations & System Demos & Tools:** work through remaining items in [SCREEN_POLISH_AUDIT.md](docs/SCREEN_POLISH_AUDIT.md), prioritizing adding live sliders/toggles + `"Copy config/snippet"` buttons to technical labs (`AnimationBasicDemo.h`, `AnimationDeclarativeDemo.h`, `AnimationLoopingDemo.h`, `ChartLab.h`, `ContextMenuLab.h`, `HoverLab.h`, `PopoverLab.h`, `PseudoLocaleLab.h`, `TextInputLab.h`, `VirtualListLab.h`).
- [ ] **Tone down heavy raw `Color{}` palettes (>=8 hardcoded colors)** into cohesive surface tokens: `AbsolutePositioningShowcase.h` (18), `AdaptiveScalingDemo.h` (16), `ContextMenuLab.h` (8), `EntityIndexLab.h` (8), `HStackShowcase.h` (8), `HoverLab.h` (11), `ModalAdvanced.h` (16), `PinchZoomLab.h` (10), `PopoverLab.h` (8), `PseudoLocaleLab.h` (16), `ScrollClickBug.h` (10), `SyncScrollLab.h` (10).

### P3 — Duplicate-screen consolidation (156 → ~115 screens)
See full rationale and screenshot-baseline caveats in [DUPLICATE_SCREENS_AUDIT.md](docs/DUPLICATE_SCREENS_AUDIT.md). Update any `.e2e` test scripts referencing merged screen flags.
- [ ] **Clean 1-for-1 pair merges:**
  - `ActiveTabStyles.h` → fold into `TabContainerShowcase.h`
  - `HorizontalDragShowcase.h` → fold into `DragDropShowcase.h`
  - `HStackShowcase.h` + `VStackShowcase.h` → single `StackShowcase.h` (axis toggle)
  - `TextAreaStatesLab.h` → fold into `TextAreaLab.h` (and fold `ComposerLab.h` as a tab)
  - `ButtonVariantsShowcase.h` → fold into `Buttons.h` (archive `ExampleSimpleButton.h` to tests)
  - `RotatedControlsShowcase.h` + `RotatedSubtreeShowcase.h` → single `RotationLab.h`
  - `SelfAlignShowcase.h` + `BaselineAlignShowcase.h` → fold into `ExampleFlexAlignment.h`
  - `SmallComponentsLab.h` → fold into `SettingRowShowcase.h`
  - `ExampleColors.h` → fold into `Themes.h` as a swatches tab
  - `TransparentControlsLab.h` + `AutoTextColorShowcase.h` → single contrast/legibility lab
  - `ConfigGapGallery.h` + `WidgetGapGallery.h` → single `GapGallery.h`
  - `CircularProgressShowcase.h` + `MetersGaugesShowcase.h` → single progress/gauges gallery
- [ ] **Larger cluster consolidations:**
  - **Animations (14 → ~6):** merge `AnimationBasicDemo` + `AnimationInteractiveDemo` + `AnimationLoopingDemo` + `AnimationSpringDemo` → `AnimationLab`; fold `AnimationDeclarativeDemo` → `TransitionsLab`; fold `CurvedTextStressLab` → `CurvedTextLab`; archive `MotionButtonCheckpoint` + `MotionModalCheckpoint` to tests.
  - **Dialogs & Modals (8 → 3):** merge `ModalAdvanced` + `RealWorldModals`; optionally unify the 5 `Dialog*Showcase` files into `DialogGallery` once the screenshot harness supports per-mode baselines.
  - **Text Input (3 → 1):** merge `TextInputDemo` + `TextInputSizes` + `TextInputLab` into tabbed `TextInput`.
  - **Typography & i18n (14 → ~7):** merge `ExampleTextShadow` + `ExampleTextStroke` + `FontWeightShowcase` + `StyledTextLab`; merge `MultiLineTextLab` + `ExampleTextOverflow`; merge `LanguageDemo` + `PseudoLocaleLab` + `LocalizedInputPrompts`.
  - **Scroll & Virtual Lists (8 → 3 + fixtures):** merge `ScrollViewShowcase` + `SyncScrollLab` + `ScrollbarStyleLab`; merge `VirtualListLab` + `VirtualListHeightsLab`; move `ScrollClickBug` + `ScrollClipBugRepro` into `LayoutBugRepros.h`.
  - **Borders & Dividers (7 → 2):** merge `ExampleBorders` + `ExampleBevelBorders` + `ExampleNineSliceBorders` + `DecorativeFrameShowcase` → `BorderLab`; merge `ExampleSeparators` + `DividerShowcase` → `DividerLab`.
- [ ] **Keep separate (do not merge):** the 10 bespoke game settings mockups (`AngryBirdsSettings`, `CasualSettings`, `DeadSpaceSettings`, `FlightOptions`, `IslandsTrainsSettings`, `KirbyOptions`, `MiniMotorwaysSettings`, `ParcelCorpsSettings`, `PowerWashSettings`, `SportsSettings`) and distinct game/app mockups (`CozyCafe`, `EmpireTycoon`, `FighterMenu`, `MarloKart`, `ShopInterface`, `OffsiteBackupApp`, `SecureTunnelMockup`, `AIMChatDemo`, `MediaLibraryGrid`, `MinesweeperLab`, `GuessWhoLab`).

### P4 — Baselines & CI
- [ ] Run `make update-baselines` and `make validate-screenshots` after polish & consolidation passes settle.
- [ ] Run full E2E suite (`make test`) and update any `.e2e` scripts whose screen names or labels changed.

## Pluginization candidates from the last 150 commits (status updated 2026-11-26)

Scope: reviewed `origin/main` `2943d9e9` back through `669cf4f9` (150 commits, 2026-09-20 → 2026-11-25) on 2026-11-26, focusing on non-screen `src/` helpers, testing/headless harness code, and reusable patterns embedded in screens. Excluded `.agents/` skill bulk, baseline PNG churn, vendor bumps, and one-off mockup content. **Status update (2026-11-26):** local WM plugin batches landed in PRs #14-#23; checked items name the merged local plugin or audit disposition (not an afterhours upstream — AH stays a later, separately-tested gap), and unchecked items are intentionally deferred with the blocker named. Preferred path is local plugin/file in `wm/src/` first (so Gabe can test it in WM), then upstream to an afterhours plugin only if it proves useful beyond WM. Do not edit afterhours for these without separate review/testing. Respect CORRECT C1: do not add new files to `src/ui_workarounds/`; a reusable local plugin with tests is different from a one-off workaround, but primitives that belong upstream should still be filed as afterhours gaps.

### Trellis UX follow-ups (owner feedback, 2026-11-26; implemented in `wm/trellis-polish-controls`)

- [x] **Hidden controls on Trellis screens:** the workspace toolbar (Overview/Back/Forward/Step out/Reset) extended to x=1272 and sat under the global "Browse screens" launcher (ScreenNavigator layer 2000, x=1068..1268). Fixed Trellis-locally: `dock::toolbar_x_ending_before_launcher` clamps the toolbar left of the launcher (`tests/dock_workspace_test.cpp` proves no intersection at 854/1280/1920). Workspace area now ends 48px above the page bottom so panels never hide under the global HUD (bottom-left name / bottom-right index). Desktop `+ Counter` moved between title and toolbar. Tab strips truncate before the 80px header-controls reserve and header F/x buttons get a visible border.
- [x] **Contrast polish (all 4 Trellis apps + DockWorkspace chrome):** darker panel borders (110,122,148), header/title/tab text, inactive tab fill, toolbar borders, Paint/Ops/IDE/Desktop body text and unselected fills. Identity kept (Paint/Ops/IDE light, Desktop dark). Board: `/tmp/wm-trellis-polish-board.png`.
- [x] **Drag/drop feel:** header height 34-40px, divider hit area 20px, workspace edge bands 44px, wide 34% center (tabify) zone, committed-target hysteresis (14px inflation) so targets don't flicker, preview + ghost now carry a readable label (`Split right onto canvas`, `Tab`, `Float`, `Dock left`...) and the ghost shows the target size. Esc-cancel verified in code (`handle_input` clears drag state). Existing Trellis e2e drag flows pass unchanged.
- [x] **Reset semantics (Trellis only):** each app has `reset_all()` (layout + `ws.reset_camera_history()` + every editable value); toolbar Reset calls it. Empirically: leaving a screen already recreated the system (`create_screen` per switch in `src/game.cpp`), so re-entry defaulted even before this change; the explicit `reset_all` + e2e leave/return checks now pin that guarantee instead of relying on recreation. Trellis apps use no global `motion::anim` tracks, so there was nothing extra to clear.
- [ ] **Broader all-screens reset audit (NOT done):** non-Trellis editable screens (e.g. GummyShop) were not audited or changed; file a separate pass if owner wants reset-on-leave semantics everywhere. GummyShop touch was avoided as low-risk proof was already covered by the Trellis leave/return e2e.


- [x] **Rolling number / odometer state** — evidence: `src/rolling_number.h` (79 lines, `sync`/`settle`/`retarget`/`advance`, reduced-motion aware) + `RollingNumberLab` from `2b39bcf8`. Destination: keep as a local WM plugin/header first; **afterhours gap candidate** for a small `ui/rolling_number` or animation helper (AH has generic animation tracks/springs, but no odometer/count-up state machine found in the plugin inventory). Useful for scores, counters, money, timers. Status (2026-11-26): DONE local plugin in WM PR #15 (`src/plugins/rolling_number/` + shim/tests); upstream AH helper only if other consumers need it.
- [x] **Pixel-preserving padded artwork atlas + frame tables** — evidence: `src/artwork_atlas.h`, `src/artwork_frames.h`, atlas commits `f1b9dbf4` and `d0e07d29`. Destination: local WM asset plugin first; **afterhours gap candidate** only if `texture_manager` cannot already express padded atlases + named frame rect metadata for mockup/game artwork. Useful for sprite/icon-heavy screens and games that need deterministic frame tables. Status (2026-11-26): DONE local plugin in WM PR #15 (`src/plugins/artwork/` + shims/tests); upstream only if `texture_manager` cannot express padded atlas/frame metadata.
- [ ] **Headless screenshot + baseline validation harness** — evidence: `src/headless_screenshots.*`, `src/testing/screenshot_validation.*`, per-screen tolerance work in `59d141e8`, baseline workflow in `869b8e33`. Destination: local WM devtools plugin first; **afterhours gap candidates** for multi-resolution capture stability (already noted in gaps), per-screen tolerance config, and a reusable baseline-compare API if `e2e_testing/screenshot_baseline.h` does not cover WM's flow. Status (2026-11-26): DEFERRED physical move: catalogued in the devtools manifest in WM PR #14, but `src/headless_screenshots.*`/`src/testing/screenshot_validation.*` stay in place until a separate build/makefile-safe extraction; AH candidates remain capture stability, tolerance config, and baseline-compare API gaps.
- [x] **Layout summary + UI-tree intent dump** — evidence: `src/testing/layout_summary.*` (`072b7c79`), `src/testing/ui_tree_dump.h` (`2a365c95`, dumps geometry *and* layout intent for an independent solver). Destination: local WM devtools plugin first; **afterhours gap candidate** for a supported layout-intent dump / named-element bounds API for agent layout QA and external layout-engine comparisons. Status (2026-11-26): DONE local devtools plugin in WM PR #20 (`src/devtools/layout_audit/` + `src/testing` shims/transport TUs + `LayoutAuditTest`); AH layout-intent/named-bounds API remains a gap candidate.
- [x] **Focus geometry / popup interaction audit** — evidence: `src/testing/focus_audit.*` (`477106ae`) plus focus-ring work in `9b0fb61b`/`586f015f`. Destination: local WM devtools plugin first; **afterhours gap candidate** for repeatable focus-order/geometry audit helpers beyond the existing focus-ring demos/tests. Status (2026-11-26): DONE local devtools plugin in WM PR #20 (`src/devtools/layout_audit/focus_audit.*` + shim/tests); AH focus-order/geometry audit helpers remain a gap candidate.
- [x] **Theme builder devtool kit** — evidence: `ThemeBuilderLab` pieces in `cebeaa44`, `4b7db56c`, `a6ac1f5e`, `441468c5` (token table, inspector jump-to-token, token links with cycle prevention, coalesced undo/redo, `.theme` export/import). Destination: local WM devtools plugin/screen kit first; **afterhours gap candidates** only for missing primitives it exposed (non-inheritable gap behaviour, text-inset charged as both-side margin, shared-popover click suppression), not necessarily the whole ThemeBuilder UI upstream. Status (2026-11-26): DONE local model plugin in WM PR #18 (`src/devtools/theme_builder/`); deferred AH primitive gaps (non-inheritable gap, text-inset-as-margin, shared-popover click suppression) still need separate AH proof before filing upstream.
- [x] **Example-gallery shell: registry + navigator + HUD + tree** — evidence: `src/systems/ExampleScreenRegistry.h`, `src/systems/ScreenNavigator.h` (`b6713ce2`), `src/systems/ScreenTree.h` (`0b068a05`), `src/systems/RenderScreenHUD.h`. Destination: local WM showcase plugin first; **afterhours gap candidate** for an optional examples/devtools gallery harness (screen registry, searchable navigator, HUD) if other afterhours consumers build similar labs. Align with open WM PRs #2/#3 instead of duplicating scaffold work. Status (2026-11-26): DONE local plugin in WM PR #17 (`src/plugins/gallery_shell/` + old-path shims); optional AH examples/devtools gallery harness remains only a gap candidate.
- [x] **Profiler overlay presentation + frame stats snapshot** — evidence: `src/frame_stats.h` (`632870e5`), `src/systems/ProfilerOverlay.h` (`2823ec74`), `SystemProfileLab` curation (`b3916728`). Destination: local WM devtools plugin first; **afterhours gap candidate** only for a curated overlay/snapshot presentation layer if AH `profiling.h` / `ui/profiler.h` / `process_metrics.h` do not already expose the same ready-to-render summary. Status (2026-11-26): DONE local devtools plugin in WM PR #22 (`src/devtools/profiling/` + shims); AH curated overlay/snapshot presentation remains a gap candidate only after diffing `profiling.h`/`ui/profiler.h`/`process_metrics.h`.
- [x] **Post-UI blur pass system wrapper** — evidence: `src/systems/ApplyBlurPass.h` (25 lines, `e5692c4f`). Destination: local WM render plugin first; **afterhours gap candidate** for a documented post-UI effect-pass system/bridge order if `effects::BlurPass` still requires each app to hand-wire render order. Status (2026-11-26): DONE local render plugin in WM PR #22 (`src/plugins/render_passes/ApplyBlurPass.h` + shim); AH documented post-UI effect-pass order remains a gap candidate if hand-wiring persists.
- [x] **Font registry / codepoint packs / deterministic mock fonts** — evidence: `src/font_config.h` (FontDef single source of truth, Korean/Japanese/inline-prompt codepoint generators, aliases/mock fonts), font-loading refactor `d4dee09b`, headless font use. Destination: local WM plugin first; **afterhours gap candidate** for reusable font-definition registry + codepoint-pack helpers + deterministic screenshot mock-font fixtures if AH font helpers do not cover that layer. Status (2026-11-26): DONE local plugin in WM PR #19 (`src/plugins/font_registry/font_config.h` + shim, byte-identical move); AH font-registry/codepoint-pack/mock-font fixtures remain gap candidates.
- [x] **Theme presets + WM default stamping as a theme pack** — evidence: `src/theme_presets.h` (`8867a8e2`, language font constants/defaults, `THEME_CORNER_RADIUS` rationale) and `src/wm_defaults.h` (`1ee811ca`, explicitly says values proven here are candidates for promotion later). Destination: local WM theme pack; **afterhours gap candidates** only for generic pieces (language-font defaults, px-based radius token guidance), not WM's exact palettes. Coordinate with open WM PR #1 (preset surface tokens/shapes/raw-colour lint) to avoid duplicate token work. Status (2026-11-26): DONE local theme pack in WM PR #19 (`src/plugins/theme_pack/` + shims/manifest sync); only generic language-font/radius guidance remains an AH gap candidate, not WM palettes.
- [ ] **App-level E2E navigation command pack** — evidence: `src/testing/e2e_integration.h`, `src/testing/e2e_commands.h` (`open_screen`/`next_screen`/`prev_screen`/`goto_screen`/`validate_screen`/resize-with-settings), `88e10926`. Destination: local WM testing plugin first; **afterhours gap candidate** for a generic screen-registry E2E command pack if hanabi/wordproc/floatinghotel adopt the same registry pattern. Do not duplicate AH `e2e_testing/screen_commands.h` without diffing it first. Status (2026-11-26): DEFERRED extraction: catalogued in WM PR #14 manifest, but `src/testing/e2e_integration.h`/`e2e_commands.h` must be diffed against AH `e2e_testing/screen_commands.h` before any local plugin move or AH gap.
- [x] **Scheduled synthetic input injector audit** — evidence: `src/engine/input_injector.*` (`39e1c010`, scheduled mouse clicks, key holds, synthetic press consumption). Destination: audit against AH `e2e_testing/input_injector.h`, `test_input.h`, and `mcp_server.h` before any plugin call; **gap only for unique scheduled-click/key-hold semantics** not already upstream, otherwise retire the local bridge in favour of AH on a future bump. Status (2026-11-26): AUDITED in WM PR #16: keep the local scheduled-input bridge (freeze note + testing manifest/docs); do not pluginize or retire until a current AH diff proves unique scheduled-click/key-hold semantics are missing.
- [ ] **Runtime log-level override shim audit** — evidence: `src/log/log_runtime.h` (`c0d37648`, runtime level for quiet modes). Destination: likely retire locally once the afterhours default-logger work lands/bumps; **gap candidate only if** AH still lacks a runtime minimum log level + `log_once_per` equivalent after re-checking current AH main/open PRs. Status (2026-11-26): AUDITED but deferred in WM PR #16 (sentinel/split-gating and `log_once_per` bypass documented); retirement is blocked on AH runtime log-level/default-logger work landing plus a WM vendor bump and `log_once_per` parity recheck.
- [ ] **LetterboxLayout duplicate audit** — evidence: `src/systems/LetterboxLayout.h` plus existing todo for its 0×0 divide-by-zero. Destination: do **not** pluginize blindly; first diff against AH `window_manager::content_viewport` / letterboxing docs already cited in gaps. **Gap candidate only if** WM's helper covers a case AH content viewport does not; otherwise migrate WM callers upstream-API-first in a separate change. Status (2026-11-26): PARTIAL in WM PR #16: non-positive-dimension guard + `letterbox_test` landed and the helper is marked keep-local; still deferred: diff against AH `window_manager::content_viewport` before deciding pluginize vs migrate callers.
- [ ] **Frozen UI workarounds → upstream primitives** — evidence: `src/ui_workarounds/GradientBackground.h` and `src/ui_workarounds/NotificationBadge.h`, both self-described workarounds with migration notes, frozen by CORRECT C1 (`3da1ea23`). Destination: **afterhours gaps** for a native gradient background primitive and a proper notification/badge primitive; keep local files untouched until an upstream primitive is tested and WM bumps. Status (2026-11-26): DEFERRED by design/CORRECT C1: local frozen files stay untouched until native AH gradient-background and badge primitives exist, are tested in WM, and WM bumps.
- [ ] **Settings foreign-file refusal policy** — evidence: `src/settings.cpp` refuses foreign/corrupt `settings.json` and avoids launch-directory fallback overwriting it (`f8766588`). Destination: local app policy unless AH settings loaders need a reusable schema/foreign-file detection hook; **gap candidate** for an opt-in settings-file identity/schema guard if other consumers hit the same footgun. Status (2026-11-26): DEFERRED: no extraction; settings code was intentionally untouched in WM PR #16 because `settings_save_test` fails identically on clean `origin/main` (pre-existing temp-file blocker), so revisit after that blocker or an AH settings identity/schema hook exists.
- [ ] **Motion recipe catalog from TransitionsLab / checkpoints** — evidence: `TransitionsLab.h` was the most-edited screen in the window (25 touches), plus `MotionButtonCheckpoint.h` / `MotionModalCheckpoint.h`. Destination: local WM motion cookbook/examples first; **afterhours gap candidates** only for specific missing primitives/recipes not already covered by AH `ui_motion`, `animation_presets`, `effects`, or `particles`. Do not propose a new motion runtime from the lab. Status (2026-11-26): DEFERRED: no cookbook extraction yet and no new motion runtime; only file narrow missing primitives after comparing against AH `ui_motion`, `animation_presets`, `effects`, and `particles`.
- [x] **File-tree data source / showcase model** — evidence: `FileTreeShowcase.h` repeated polish + tree interactions. Destination: local WM plugin candidate for filesystem-backed tree data (expand/collapse, sizes, preview, icons) on top of AH `ui/tree_view.h`; **afterhours gap candidate** only for a reusable file-tree data-source adapter if `tree_view` consumers keep hand-rolling it. Status (2026-11-26): DONE local plugin in WM PR #23 (`src/plugins/file_tree/` model + preview, showcase rewired, tests); AH `tree_view` data-source adapter remains a gap candidate only if consumers keep hand-rolling it.
- [x] **Screen scaffold / settings-page kit alignment** — evidence: repeated screen preambles across the last-150 polish passes, but already in flight as open WM PR #3 (`begin_screen`) and PR #4 (`SettingsPage` kit). Destination: do not start a competing extraction; after those land, re-audit whether the scaffold/kit should become a named local WM plugin and whether any generic piece is an **afterhours gap** (e.g. data-driven settings rows beyond vendored `setting_row`). Status (2026-11-26): DONE local plugin in WM PR #21 (`src/plugins/screen_kit/` + old-path shims); generic AH gap re-audit waits for real pilot adoption, since the headers currently name designated first adopters rather than many live consumers.
- [x] **Already upstream — do not re-file as new plugins without a diff:** terminal + command picker (AH `terminal/*`, `command_picker/*`), charts (`charts.h`, `ui/line_chart.h`), pseudo-locale (`ui/pseudo_locale.h`), settings (`settings.h`, `settings_json_preserve.h`, `settings_table.h`), E2E core (`e2e_testing/*`), models/house-of-cards/particles/effects. For these, file only narrow missing-adapter gaps found while consuming them in WM. Status (2026-11-26): Standing rule retained from the audit; no plugin work unless a fresh AH diff shows a narrow missing adapter.
## UI element follow-ups from the lab review

- [x] `with_padding` on a label with no children does nothing (it warns); leaf text spacing is `with_text_inset`/`Theme::text_inset`, while padding remains container spacing. Covered by `label_inset_test` and the ranked text-inset item below.
- [x] Should `ClosedBy::Any` (click outside dismisses) be the modal default instead of `CloseRequest`? No: `CloseRequest` stays the default; light dismiss is explicit with `with_closed_by(ClosedBy::Any)`. Pinned by `default_modal_is_not_light_dismissed_by_backdrop`.
- [x] A drawn chevron/icon primitive: `polyline::chevron_points`, `chevron_points_in_rect` and `draw_chevron` cover tip, direction and bounds forms; TransitionsLab accordion/learn-more use them. Tests: `polyline_test`; WM E2E 304/314.
- [x] Multiline label line height does not match the font pixel size, so anything that scrolls text by fixed rows (the counter reel) has to lay out one label per row. `with_line_height` now pins a label's row height: layout, `measure_config`, and both renderers stack wrapped, hard-broken and styled lines at that height, so one label can hold fixed rows. Unset, the face's measured line height is used. Tests: `multiline_text_test`; WM E2E 228 (MultiLineTextLab sample 5).
- [x] Banner stacking: hovering spreads the stack, but a long stack should scroll so every open banner can be reached. Banners are no longer auto-dismissed past three; the spread stack is clipped to its area and the wheel scrolls it (clamped so the oldest stops at the top), with a scroll readout. Tests: WM E2E 321.
- [x] Tab rows shrink label text to fit; long example titles squished at 140 px. A minimum font size or overflow rule for buttons would avoid that. The expand split now honours label-width floors by redistributing (a long tab freezes at its label width, siblings re-split the rest). When the labels together exceed the bar, content floors are soft: tabs compress inside the bar and labels ellipsize at unchanged font size. Pixel minima still always win. Tests: autolayout `expand_min_width_redistributes_instead_of_overflowing`, tab_container narrow/inside-bar, downstream `narrow_tab_bar_ellipsizes_instead_of_overflowing`; WM E2E 253 (Long labels tab).

## Theme builder

- [ ] Better theme builder (idea from Zed's Theme Builder, Feb 12 2026): a WM lab screen where the full theme token set is edited beside a live replica preview of typical WM screens, so every change shows its effect immediately. Pieces, in build order: token table by category driving live theme application; click an element in the preview to jump to the token that colours it (inspector); links so a token can follow another (surfaces share a base, icons follow text) instead of copying hex values; undo/redo over edits; export the result as a theme file and import one back. Build on the shipped colour swatch variants (C/B/A) rather than a new colour picker. Piece 1 DONE: `ThemeBuilderLab` (`theme_builder` screen, E2E 368) — all 12 `Theme::Usage` colours in a two-column table by category (Text/Surfaces/Brand/Status/Focus) with the B-variant preset+hex editor per token, sliders for panel radius + focus-ring thickness, and a usage-only settings-page preview; editor chrome stays on fixed colours, edits are screen-local via per-frame `set_theme`. Found while building: the swatch container keeps the default 24px gap (gap is not inheritable) so it measures 52 tall, and its button's left text-inset is charged as a margin on both sides, so the button needs ~280px+ — the table is two 400px columns for that reason. Piece 2 DONE: inspector — clicking a preview element jumps to its token (row highlights, palette opens, Inspector readout names it); composite widgets map to their primary token (Save -> Primary, toggle -> Accent, slider -> Secondary); `inspect_usage` replaces magic token indices. Piece 3 DONE: links — a token can follow another via a per-row Link button opening one shared popover picker (Unlink + Follow targets; followers take their root target's color; cycle-forming targets are hidden; unlink restores the token's own color; linked rows show their resolved hex, not a swatch). E2E 368 covers link, follow-through, cycle prevention, unlink. NOTE: a second popover panel breaks later clicks — the picker must stay a single shared popover, buttons directly under the panel don't receive clicks, and an anchor close-click needs popover_toggle suppression. Piece 4 DONE: undo/redo — Undo/Redo buttons in the title row; snapshot history of the 12 token colors + panel radius + focus-ring thickness + links (capped at 100 entries); edits on consecutive frames coalesce into one gesture, so a slider drag or a typed hex is a single undo step; undo/redo application is not itself recorded; preview widget state (name/notifications/volume) is not theme state and is not in history. E2E 368 covers slider, link, hex undo/redo plus a two-undo/two-redo walk. Piece 5 DONE: export/import — path field + Export/Import buttons; export writes the flattened theme (resolved colors + radius + ring) as a .theme text file via afterhours theme_io; import applies a file onto the current theme, clears links, clamps radius/ring into slider range, and is one undo step; a missing file fails without touching state. Needed a one-line afterhours fix: theme_io now round-trips panel_corner_radius. E2E 368 covers export, failed import, import restore, and undo/redo of import. Also fixed: shape sliders resync their held state when unchanged (swatch channel-slider pattern), so undo/import of radius/ring sticks instead of the next frame silently reverting it.

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

- [x] **UP-17 follow-up:** Define missing-glyph rendering and atlas recovery. Atlas-independent measurement and its cache regression are now implemented; see `vendor/afterhours/docs/font-atlas-measurement.md`. DONE (sokol backend): loaded faces fall back to each other in load order (`mesh_font_fallbacks`), and the `FONS_ATLAS_FULL` callback expands the atlas (cap 4096) so the retry draws; `font_atlas_measure_test` covers the mesh + auto-expansion + cap. Hanabi's guard (`src/util/atlas_guard.h`) is removable on bump — the removal itself is Hanabi's, in their repo. Still open: raylib fallback drawing, and a placeholder for codepoints no loaded face covers.
- [ ] Raylib fallback drawing: per-codepoint run splitting in `draw_text_ex` so a glyph missing from the active font draws from a loaded fallback face instead of vanishing (sokol already does this via the fontstash fallback chain).
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

- [x] Pseudo-localization for layout stress: `UIStylingDefaults::set_pseudo_locale(PseudoLocale::DoubleWords | RtlWords | None)` rebuilds every label (and tooltip, and styled spans) in the fake language where labels are built, so measurement and wrapping stress with it; RTL also flips unpinned label alignment to the right. Field values are user data and are not transformed; per-unit-motion labels are exempt (their spans index the original string). Tests: `pseudo_locale_test` 33/33; demo: PseudoLocaleLab (WM E2E 358). RTL also mirrors layout: horizontal padding and margin swap sides in `apply_layout`, and row containers place flow children in reverse (`UIComponent::flex_row_reversed`), so a label sits on the other side of its button. Not mirrored: absolute positions (a mirrored absolute layout is a different feature; rows and flow are the RTL contract).

- [ ] Cmd+F find for tools: a library find affordance (shortcut opens a find field, next/previous match navigation) that a tool can attach to its searchable surface instead of every consumer wiring Cmd+F, focus and match cycling itself. Scope to pin down with the requesting tool: which surfaces are searchable (label text, list rows, document text) and who owns the find field's chrome.

- [ ] `hidden=until-found` semantics for folds: content inside a collapsed fold stays searchable, and a find match inside it expands the fold to reveal the match (the HTML `hidden=until-found` + `beforematch` behaviour). Needs the fold/collapsible components to report their hidden text to the find search above and an expand-on-match path that does not fight the user's manual fold state afterwards.

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
- [x] Add scroll anchoring/preserve-position-on-prepend. Done: `HasScrollView::anchor_scroll` for built views (scroll_anchor_test) and `prepend_virtual_rows` for measured virtual lists (sep26 item 10).
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
- [x] kart `GetFontDefault()` invalid in headless: fallbacks still return it (`backends/raylib/font_helper.h:194,252`); needs a headless run to confirm or fix. VERIFY. Verified 2026-09-27: the measurement collapse was already fixed (estimate + `font_is_usable`, covered by `headless_fallback_test`); one live gap remained -- `load_font_for_string` never got the headless atlas fallback its siblings have, and now mirrors them, gated on `IsWindowReady()` because the texture upload crashes with no window at all (found by segfault; gating inside `build_font_atlas` instead broke WM, whose fonts load before the window is ready). New checks in `headless_fallback_test` (16/16).
- [x] kart `with_absolute_position()` x-vs-height: init resolves tx/ty separately (`ui/component_init.h:520`); needs a runtime rect assertion before closing. VERIFY. Confirmed and fixed 2026-09-27: `apply_modifiers` resolved BOTH translates against the screen height, so a `w1280()` x-offset was scaled by height at any non-square resolution. Each axis now resolves against its own dimension (height still passed as the cross-reference for Height-tagged sizes). Runtime assertion: `absolute_position_resolves_each_axis_against_its_own_dimension` in `ancestor_transform_test` (x=80 not 60 at the harness's 800x600; failed before, passes after). Flow translates share the code path and are corrected with it.
- [x] kart checkbox internal layout overflow: row exists (`ui/imm_components.h:1081`) with children-sized height in the default path, but no runtime validation-warning reproduction yet; kart no longer hits it. VERIFY. Reproduced and fixed 2026-09-27: a default labelled checkbox produced the exact warning ('checkbox indiv' 100x76.7 inside the 200x50 row) -- the toggle is a button, so its height came from label + theme button padding, exceeding the row's height intent. `checkbox()` now pins the toggle's height to that intent (fixed size, or the preferred value of a `children()` height; pure auto is left alone). Rect-level repro: `default_checkbox_children_stay_inside_the_row` in `checkbox_mark_test` (344/344, warning gone).
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
- [x] BUG sokol: resize tears down the render target mid-pass and aborts (hanabi #374). Needs a Metal repro (headless_capture resize-during-pass) before fixing. Already fixed by `a9b1ba0`: headless resize requests defer to the next frame (`g_pending_headless_size`, gated on `g_pass_active`), and the windowed event path never recreates GPU resources on resize. Verified 2026-09-27: `sokol_resize_test` 1356/1356 at DPI scales 1 and 2 (mid-pass request, coalescing, cancel-on-same-size, texture mode, 64 repeated resizes).
- [x] BUG ui: a focused field's border loses its top edge (hanabi #375). Hanabi's isolation says it is a clip (the field's drawable region excludes its top row), not overpaint; app-side wrapper workaround exists (`edged_field.h`). Needs a pixel-sampling repro in the WM harness before fixing. Reproduced and fixed 2026-09-27: the clip half was right. A field clips its own contents (`HasClipChildren`), and `prepare_focus_paint` scissored the focus ring to the intersected clip, whose first term is the element's own rect, so the ring's outward strokes were cut on every side (the top reads first in a form). The ring now resolves its clip with `include_self=false` (`compute_intersected_clip_rect` gained the parameter; ancestor clips, including scroll viewports, still apply, and the render/hit-test callers are unchanged). Pixel repro in `sokol_geometry_test`: ring top stroke absent before, present after, both renderers; the field's own border was never affected. Hanabi can drop `edged_field.h` on the next bump.
- [x] hanabi #39/#40 e2e runner: fixed at HEAD — end-of-stream drains the last command before finalize, and single-script `has_failed()` asks the handlers directly (`e2e_testing/runner.h:445,462`). #223's deadline-by-`dt` remainder stays VERIFY (tick(dt) exists; needs a host-loop repro).

### Unit C — EXT:ui / EXT:autolayout (canonical families first)

- [x] EXT:ui text inset: per-label inset + label honours its own padding (hanabi #85 family: #75, #84, #91, #100, #109, #277; WM line 8 `with_padding` on a label). Done at HEAD before this pass: `with_text_inset` per label over `Theme::text_inset`, applied in both renderers and in measurement; padding on a leaf label warns once instead of moving every existing label (hanabi index marks #85 FIXED). Tests: `label_inset_test`, `default_theme_test` (`text_inset_positions_plain_and_styled_text_in_both_renderers`).
- [x] EXT:ui text measurement: prepared text + cached measurements (WM line 57; hanabi text family canonical #136). Done as the bounded checklist (the full `PreparedText` redesign in docs/architecture.md, #51 text-landing geometry and #286/#68 self-reported laid-out size remain follow-ups): caches already existed (`measure_memo`, `TextMeasureCache`, `wrap_memo`) but the wrap key omitted face/size/spacing and no reload ever invalidated any of them. The key now covers face/size/spacing, wrap entries die with `measure_memo`'s generation, and `FontManager::load_font` on an existing name clears both memos and the shared cache. #48: `FontManager::has_glyph` / `missing_codepoints` over a true cmap recorded at load in both backends (`src/font_coverage.h`); raylib's glyph array alone was wrong, since `LoadFontData` inserts placeholder entries for requested-but-absent codepoints. #62: styled runs take x offsets from the joined line's measurement (`detail::text_run_offsets`) in both renderers. Tests: `text_memo_test` 29/29, `font_atlas_measure_test` (its build needed a `sokol_gfx.h` include it was missing); WM E2E 356 (TextCacheLab).
- [x] EXT:ui focus: one `focus_ring_for`, `:focus-visible`, per-widget offset, independent contrast edges (hanabi #83 family: #46, #263, #265-267; wordproc #5 focus indicators). Done: `ui::focus_ring_for` promoted out of detail (contrast edges already resolved inside it); persistent `focus_modality` + `UIContext::focus_visible()` gate `ComputeVisualFocusId` in Split mode (pointer focus draws no ring; FollowsMostRecentInput exempt); `with_focus_ring_offset` per-widget override reaches `UIComponent::focus_ring_offset`. Tests: focus_origin 74/74, downstream_gaps 105/105.
- [x] EXT:ui text_area parity: multi-line as a mode of the text-input implementation — placeholder, background, focus ring, word motion, harness visibility (hanabi #67 family: #57, #65, #105, #258, #260-263; #56 programmatic focus; #65 padding derives from field height is BUG-adjacent). Done as the bounded parity checklist (full state unification remains the XL follow-up in hanabi #67): placeholder rendered in the muted colour (#261), caller background/transparent respected (#262), 1px default border + 2px accent border while focused (#263), word motion collapses a selection to its near edge (#260), `expect_input_text` reads HasTextAreaState (#258), script parser accepts `\n`/`\t` escapes in quoted strings (#259; one pinned parser expectation updated — a literal backslash-n is now written `\\n`), auto-grow resolves line height and vertical padding at the current resolution. Tests: text_area_test and e2e_arguments_test green; WM E2E 355 (placeholder, wrapper focus, multi-line expectation) 1/1.
- [x] EXT:ui virtual list: variable row heights with retained prefix-height index + range invalidation (hanabi #326 family: #23, #31a, #147, #170, #224, #420, #455; WM virtual-list items). Done: variable heights already existed; the retained index is new. `HasVirtualListIndex` lives on the list entity, `height_of` is asked once per row until `invalidate_virtual_rows(first, last)` marks it, growth measures only new rows, and `RowMetrics` borrows the retained prefix. #30a: `prepend_virtual_rows(n)` inserts unmeasured rows at the front and the build adds their height to offset/target/eased in the same pass; `virtual_row_offset` / `virtual_rows_total` expose the retained geometry. #476: `HasScrollView::scroll_to_bottom()` / `scroll_to_top()` (plus the left/right pair) move offset, target and last_eased together (unmeasured parks at a sentinel the first clamp resolves). #163: `MeasureScrollViews` skips a view with no children and a prior measurement, so a frame where the screen is not built no longer clamps a live offset to zero; a view emptied while visible keeps its offset until it has children again. Tests: `virtual_list_test` (retained asks, prepend bump), `scroll_anchor_test` (unbuilt frame, scroll_to_ends); WM E2E 357 (VirtualListHeightsLab).
- [x] EXT:ui text-editing action surface (hanabi #257/#264/#565). Done 2026-09-28: `delete_to_line_start`/`delete_to_line_end` (source line, newline not consumed) wired into both widgets behind new action names `TextDeleteLineBack`/`TextDeleteLineForward` (registered in DefaultAction, `optional_editing_actions`, and the missing-action warning; tripwire lists in `setup_test`/`text_action_names_test` extended). `ui::default_keymap` is platform-correct now — it bound Cmd and Ctrl for every chord, which put macOS word ops on Cmd; macOS is NSText (Cmd clipboard, Option word ops, Cmd+arrows + Ctrl+A/E line motion, Cmd+Backspace / Ctrl+K line deletion), elsewhere Ctrl-based with Ctrl+Shift+Backspace and Ctrl+Y redo. Responder: `text_input::apply_edit_command(state, EditCommand)` + `apply_edit_command_to_entity` (either state type, area index rebuilt, selection-first, undo snapshot, readonly refuses mutations). Tests: `text_line_edit_test` 49/49 (line ops, commands, dispatch, keymap chords — failed to compile before, registry tests failed at runtime), WM adopted the actions/chords and E2E 359 covers both widgets end-to-end.
- [ ] EXT:ui widget lifetime remainder: library entities invisible to consumers, exit animations have nothing to animate (hanabi #171 family remainder: #146, #160, #162, #163, #525). Parent-cleanup half DONE 2026-09-27: children are linked to their parent at creation (`imm::ui_element_children`, `mk()` links automatically; `UIComponent::children` could not serve — it is cleared and rebuilt every frame, so a dead parent's list is empty exactly when retirement needs it), and `retire_unbuilt_ui_elements()` / `clear_existing_ui_elements()` walk that map down, taking the whole subtree in one sweep with records erased (a stale record would alias the id's next pooled owner). Entities with no record (dropdown options, the known case) link themselves via `imm::link_ui_child` and follow their parent; unlink+relink spares a moved child; grace 0 disables the cascade with the sweep. Tests: `widget_retirement_test` 32/32 (subtree in one sweep, raw linked child, adoption, clear, grace 0; the subtree checks failed before). Still open: exit animations have nothing to animate (a widget that stops being built still dies on the sweep's schedule, subtree included — an exit hold needs a motion-aware extension), and the consumer-visibility half of #171.
- [x] EXT:ui small components as imm helpers (not new plugins): divider, value pill, tab active indicator, compact settings list (cartographer; kart settings rows; wordproc #4 dropdown triangle is `with_dropdown_indicators`, VERIFY at bump). Done: `imm::divider_line` (static rule; `divider` was already the draggable splitter), `imm::value_pill` (text-sized readout chip for sliders/steppers), `imm::settings_list` + `imm::settings_row` (label left, right-aligned value area returned to the caller, divider under each row). Tab active indicator already existed in `tab_container` (accent underline). Tests: `small_components_test` 19/19.
- [ ] EXT:ui rendering nits (hanabi, XS/S each): #25 degenerate triangle on mixed corners, #71 grid snapping quantizes positions, #76 unpadded element gets a fraction of the screen, #78 `draw_circle_v` truncates centre, #80 boxes rasterize 1px bigger/up-left, #81 per-corner rounding bits named for the opposite corner, #88 baseline alignment, #92/#106 MSAA/antialiasing pinned off, #93/#97/#222 absolute-child placement and flow counting, #94 scrollbar overlay mode, #101 supersampled capture, #102 `on_draw_fg` scaled rect without scale, #107 selected-row fill is its own background, #110 origin rounding, #111 hover highlight is the hit rect, #276 `Dim::Percent` ignores own margin, #327 draw-only element without an Entity, #335 two view trees in one window (L, likely EXT:ui contexts). Ranked-14 pass 2026-09-27, proof-first (a failing test before any fix): PROVEN+FIXED — #78 sokol `draw_circle_v` truncated the centre to int (centroid exactly 20.00 for a 20.9 centre); now `draw_circle_filled` keeps floats, centroid 20.93 (`sokol_geometry_test`). #81 the `CornerPosition` enum named bit 0 TOP_LEFT while both backends read bit 3 as TL; enum flipped to the backend layout (`rounded_corners_test`, 5 of 6 checks failed before). ALREADY FIXED, now pinned — #25 mixed-corner fan (same test), #276 content-box percent+margins is the documented model, pinned by `margin_offsets_percent_child_into_overflow` (d41e1d8), #71 snapping rework snaps sizes only; positions accumulate from snapped extents. NOT REPRODUCING at HEAD — #80/#110 a fractional-origin rect rasterizes exactly (bbox 10..30, 441px, sokol), #102 both render paths hand `on_draw_fg` the same `cmp.rect()` as the fill, #111 hit rect and hover fill are the same `rect()`, #94 scrollbars already draw as an overlay layer and reserve no layout space, #327 the entity-less surface exists (`RenderBuffer::add_rectangle`/`add_text`/...). STILL OPEN from this line — #76 (not provable from the one-line claim; needs hanabi's repro), #88 (baseline alignment is a feature, no `AlignItems::Baseline` exists), #92/#106, #93/#97/#222, #101, #335 (own backlog row). Side find: forcing sokol test rebuilds exposed a stale-binary trap (multi-TU `.d` covers only sokol_impl.cc; explicit header prerequisites added) and a latent `begin_shader_mode` ADL ambiguity in `sokol_blend_test`, fixed by qualifying the four UI call sites to `::afterhours::`.
- [x] EXT:autolayout helpers: `Margin::left()` / `Padding::horizontal()` single-side constructors, `with_720p_size` convenience (cartographer); flex-grow to pin trailing element (hanabi #18; may already exist — VERIFY first). Verified 2026-09-27: all already exist — single-side factories are `Margin::Left()` / `Padding::Top()` etc. (capitalized so they cannot collide with the fields; `horizontal()`/`vertical()`/`all()` alongside, locked by static_asserts in `layout_types.h`), `ComponentConfig::with_720p_size` exists, and flex-grow is `expand()` with a `flex_grow()` alias. No code needed.

### Unit D — EXT:e2e_testing / EXT:input_system

- [x] EXT:e2e_testing scripted addressing (ranked #1, done): `assert_ui`/`assert_ui_text` gain `bg=#rrggbb[aa]` and `fg=#rrggbb[aa]` colour properties (fg via the shared `resolve_label_color`); new `expect_no_ui <name>` (waits one frame, then judges absence); `assert_ui_text ... in=<scope>` restricts the match to a named subtree. #59 quoted values and #86 geometry capture already existed. Tests: `e2e_arguments_test` 123/123 (3 new handler tests), WM E2E 353 end-to-end (its first run caught a wrong hex in the script itself, not the code).
- [x] EXT:e2e_testing command packs (ranked #1, done): `ui_commands::register_full_pack<InputAction>(sm, dump_fn)` registers builtins + UI + perf + unknown + cleanup in order, one call per SystemManager.
- [ ] EXT:e2e_testing runner: per-script reset in directory mode (#381), handler-owned timeout messages (#380), worked host-loop example (#41).
- [x] EXT:input_system pointer parity: right-click/multi-button in scripted input (wordproc #14; hanabi #526 synthetic right-click misses direct polling), wheel axes + smoothing (#30b, #405 family: #406-408, #445, #446), middle-button state, SUPER held state (#562, #568, #256). Done (polling parity): the right button now has press/release edges in the injector (`right_just_pressed`/`right_just_released`, middle release edge too), `test_input` polling routes all three buttons, and `platform_input` delegates pressed/released for all buttons and returns the scripted wheel in test mode instead of 0. SUPER chords (#562) and the click/wheel verbs already existed. Tests: `input_injector_mouse_delta_test` parity cases. Remaining: #405 trackpad-vs-detent smoothing (needs a platform delta kind) and #406 (hanabi ruled app-side).

### Unit E — EXT:settings / EXT:color / EXT:animation / EXT:os

- [x] EXT:settings: default settings-path override from code (cartographer); JSON comment-preserving writes — see REJECT? G6. Done 2026-09-27 (override half): `settings::set_path_override(dir)` / `clear_path_override()` redirect every resolution — `load()`, `save()`, `SettingsLoadSystem`, and `get_save_path()` now share one `resolve_path()` (previously three copies of the files-plugin-or-cwd logic); without an override the root is unchanged, and `save()` creates the resolved parent directory. Test: `settings_path_test`. The comment-preserving-writes half stays in Unit G (G6).
- [x] EXT:color: public contrast-ratio helper for custom widgets (cartographer; validator computes it internally — expose, don't duplicate). Stale: `colors::contrast_ratio` in `src/plugins/color.h` has been public since 2026-01 and is the same function the validator calls. Pinned by `color_contrast_test` (includes color.h alone; 21:1, symmetry, #767676 = 4.54).
- [ ] EXT:os: OS appearance (dark-mode) query next to `os::reduced_motion_enabled` (hanabi #1/#16); window-focus/frontmost query (#28a, S/M — accept only the query, not the platform family G1). Appearance half done 2026-09-27: `os::appearance()` (`Light`/`Dark`/`Unknown`) + `os::dark_mode_enabled()` in `plugins/appearance.h`, mirroring `reduced_motion.h` — macOS reads `AppleInterfaceStyle` via the ObjC runtime, other platforms return `Unknown` until their backend lands. Test: `appearance_test` (both branches exercised: real Light system, Dark via the defaults argument domain). Still open: the #28a window-focus/frontmost query.
- [x] EXT:ui validation: throttle/disable toggle for per-frame validation logs (cartographer); wrong layout corrected silently but warned forever (hanabi #53). Done: the validation-side dedup already existed (`report_violation` warn_once per element+category, `validate_config` once per distinct message); the remaining per-frame spam was autolayout's `Layout wrap` and `Layout overflow`, now both `warn_once` keyed on (child, parent, reason). Test: `autolayout_test` 370/370 (three frames of one violation = one line each).

### Unit F — NEW-PLUGIN candidates (explicit go-ahead required each)

- [x] NEW-PLUGIN? `context_menu`: not a plugin. The primitive already shipped as `imm::context_menu` over the shared menu list (`ui/menu.h`), with ContextMenuLab and wordproc adoption; this line was stale. Closed 2026-10-03 as VERIFY, plus the two remainders wordproc named before migrating its popup bodies: checked/radio `MenuItem` state and bounded scrolling for long menus. Tests: `menu_test`; demo: ContextMenuLab (WM E2E 361, on top of 94/182). Hanabi #287 drag-candidate thresholds stay with the drag items, not menus.
- [x] EXT:ui colour input (hanabi #58): not a plugin and not a reject. Shipped 2026-10-04 as `imm::color_swatch_inline` / `color_swatch_presets` / `color_swatch_editor` in `ui/color_swatch.h` over the existing button, slider, text input and popover, after a `colors` refactor that added backend-free HSV and hex strings and deleted the broken HSL trio. Presets stay caller-owned (`std::span`, `default_color_presets()` as the no-opinion fallback); preset hover previews without saving and a click publishes. Tests: `color_value_test`, `color_swatch_test`; demo: colour swatch lab (WM E2E 362/363/364). The drag-hover lock noted while using the editor stays filed in `docs/AFTERHOURS_GAPS.md`, not fixed here.
- [ ] NEW-PLUGIN? `accessibility`: role/name/value semantics annotations + query (hanabi #112 family: #458). Weakest candidate — desktop-app consumers only.

### Unit G — REJECT? (proposed not-library; awaiting approval)

- [ ] REJECT? G1 hanabi OS-integration family (#33a: menu bar, notifications, global hotkeys, deep links, Spotlight, bundling, installed-font discovery, #5, #28a actions, #31b, #32a exe-relative resources — #32a may return as an EXT:files bug, #36 cache-dir split, #60 sokol drag-and-drop enable, #465-474, #571, #572): desktop platform shim; hanabi carries it app-side. Generic crumbs already extracted (reduced motion; appearance query in Unit E).
- [ ] REJECT? G2 wordproc #13 table/grid component: domain widget; autolayout grid + imm composition suffice, no second consumer.
- [ ] REJECT? G3 wordproc #12 rich text as a new subsystem: per-span styling exists (styled labels/TextSpan); any parity gap is Unit C, not a subsystem.
- [ ] REJECT? G4 cartographer app E2E verbs (`set_volume`, `check_volume`, menu-flow verbs as library commands): app state, not library; the pack mechanism is Unit D.
- [ ] REJECT? G5 hanabi §4 negative results (e.g. #4 status-glyph reachability): deliberate non-features; keep recorded so they are not re-filed.
- [ ] REJECT? G6 settings comment-preserving JSON writes, if the serializer proves one-way: do not grow a JSON parser in `settings`; counter-proposal is JSON5 read support only.

### Remaining hanabi live items not in a family above (grouped, all EXT unless noted)

- [ ] Frame/host loop (EXT, host-loop plugin surface): CRITs done 2026-10-03: `frame_loop::request_frame()` (#542; thread-safe, coalescing, the wake handle #546 asks for) with `RunConfig::frame_mode` Continuous/OnDemand, backend input events requesting frames, `wait_for_frame_request` for custom hosts (`frame_loop_test` 20/20; WM HostLoopLab, E2E 360). #541 done with it: Metal caps full frames at `target_fps` in the display callback and `set_target_fps` retargets at runtime. Still open from this line: #543 indivisible frame phases (L), #544 non-consuming input-activity snapshot, #545 window exposure/backing events, #547 timer visual deadlines, #548 `dt` is callback time, #549 headless cadence harness, #541 Metal ignores `RunConfig::target_fps` (BUG), #580 cancellable background jobs, #581 deactivate hook for conditional systems, #587 frame-safe mailbox, #589 per-system CPU accounting, #585/#586 retained-byte attribution + memory-pressure event, #181/#183/#438 per-frame allocation family (ComponentConfig copies, focusable `std::set`, string churn), #155 first-draw pre-warm, #126/#125/#212/#145 GPU accounting family (byte totals, deferred frees, frame scope), #14 texture sampler mipmaps, #95 `clipboard.h` declares none of the symbols it calls (BUG), #50/#54 graphics/input reads bypass the injector (BUG), #70 entity created this frame not findable by id (BUG), #230 `mouse.pos` NaN until first event (BUG), #172 input injection requires the e2e plugin compiled in, #45 widget callbacks outlive their frame / imm `on_submit`, #38 container hover without clickable, #63 container draws over children, #64 window-level chrome/render layers, #90 `ctx.theme` read at render time, #74/#286 resolved layout tree walk + self position, #146 tree-size reporting, #3 absolute `button()` click vs manual hit-test, #13 alpha blending in `draw_texture_pro` (see Unit B opacity), #28a window focus query (Unit E), #37 text selection on read-only text (L), #259 script parser `\n` escape (EXT:e2e_testing, XS), #336 tab order scoping, #287 drag primitive unreachable from config (XS; see Unit F), #306 `with_auto_grow` row-count return (XS).

## Audit triage 2026-10-10: WM-local defects (obvious; library ones in docs/AFTERHOURS_GAPS.md)

- [ ] AnimationStressLab overrides reduced motion for the whole session: `src/systems/screens/AnimationStressLab.h:26,83` calls `motion::set_instant(reduced)` every frame with a local `reduced=false`, persisting after leaving the screen, against Settings (`src/settings.cpp:201`). Fix: init/write through Settings, delete the per-frame call. Check: entering/exiting the lab never changes the global reduced-motion setting.
- [ ] TransitionsLab hold-timers strobe or vanish under instant: thinking/reasoning holds (`src/systems/screens/TransitionsLab.h:1269` + reasoning hold) finish frame 1 and re-fire every frame; ErrorHold/toast/Skeleton/Spin/Dissolve/Drop holds (`:549,859,954,996,1656,1898`) expire instantly; loop restarts (`:1742-1743` + shimmer) lack `!is_instant()` guards. Fix: `.essential()` on the hold tracks (the spinner at `:991` already has it), instant-guards on restarts. Check: under reduced/instant each hold lasts its wall-clock duration and fires once.
- [ ] Hand-rolled demo loops ignore reduced motion: `AnimationLoopingDemo.h:36,89-100`, `TransitionsLab.h:1423-1445,1574+` (confetti/smoke/gradient `get_time()` unguarded), `AnimationSpringDemo.h:101`, `OffsiteBackupApp.h:46-50`. Fix: gate/freeze each on Settings reduced/instant. Check: with reduced motion on, none of the four screens move.
- [ ] D-pad up/down each fire two actions: `src/input_mapping.h:62,81,67,86` — `WidgetBack = LEFT_FACE_UP = WidgetUp` and `WidgetNext = LEFT_FACE_DOWN = WidgetDown`. Fix: rebind Back/Next to the triggers. Check: D-pad up moves focus only (no Back action recorded).
- [ ] Letterbox layout divides by zero on minimized windows: `src/systems/LetterboxLayout.h:19,23` divides by content width/height unchecked (inf/NaN rects at 0×0). Fix: return an empty layout when any dimension ≤ 0. Check: minimize to 0×0 and restore renders normally with no NaN rect logged.

## Audit triage 2026-10-10: native support wanted in afterhours (prerequisite to any default change)

Deduped from the six 2026-10-06 `todo-*.md` audits (147 unique capabilities; 763 OTHER items need no new support, 101 were already filed). Each item is a capability that does not exist today, verified absent by grep/opening the cited site. Paths are relative to `vendor/afterhours/src/`. `[then-default]` means a default flip also rides on the item; those defaults are NOT filed here — they stay in the separate current→proposed list pending decision.

### Motion core

- [ ] Cubic-bezier easing + strong curves: `Easing{kind,x1,y1,x2,y2}` + `cubic_bezier()` + ease_out_strong/ease_in_out_strong/ease_drawer/linear missing; `Timeline.curve` is a bare fn-ptr (`animation/timeline.h:30`). Check: grep cubic_bezier\|ease_out_strong > 0. [then-default]
- [ ] `motion::ease` helper: one-call eased timeline missing (`timeline.h`). Check: grep 'motion::ease' > 0.
- [ ] Per-keyframe easing: `Key{t,v}` only (`timeline.h:23`). Check: Key has an easing field.
- [ ] Whole-duration easing `curve_whole` flag: curve is per-segment (`timeline.h:67`). Check: flag exists.
- [ ] Asymmetric easing preset, needs cubic-bezier (`timeline.h:10`). Check: grep asymmetric > 0.
- [ ] Stepped easing `steps(n)` (`timeline.h:30`). Check: steps() constructible.
- [ ] Perceptual spring completion `perceptual_duration()` + flag; settle uses kRestRatio 1e-3 (`spring.h:78`). Check: grep perceptual > 0.
- [ ] Overdamped springs: bounce clamped [0,.999] (`spring.h:51`). Check: bounce<0 solves.
- [ ] `Spring::from_physics(stiffness,damping,mass)` (`spring.h:29`). Check: grep > 0.
- [ ] Named springs move/drawer/rotate/press/playful (`spring.h:25-28`). Check: grep 'Spring::move\|playful' > 0. [then-default]
- [ ] Velocity handoff `from(v,vel)` / `to(...,vel)` / `velocity(T)`; `from()` zeroes velocity (`track.h:118`). Check: a thrown spring carries velocity.
- [ ] Timeline retarget keeps velocity; `begin()` does vel.fill(0) (`track.h:281`). Check: mid-flight retarget preserves derivative.
- [ ] Timeline reversal shortening proportional to elapsed (`track.h:130`). Check: reversed duration scales.
- [ ] Track pause/resume/seek (`store.h:23`). Check: methods exist.
- [ ] Cancellable on_complete handle dropped on redirect (`track.h:172`). Check: handle type exists.
- [ ] `motion::after(seconds,fn)` timer (`store.h`). Check: grep > 0.
- [ ] Track head-delay `after()` (`track.h:152`). Check: grep 'Track &after' > 0.
- [ ] Finite `repeat(int)` + iterations; `repeat(bool)` only (`track.h:162`). Check: repeat(3) stops.
- [ ] `repeat_alternate()` reverse chain (`track.h:227`). Check: plays reversed.
- [ ] `motion::stagger(span,step,order)` for arbitrary tracks (`text_unit_motion.h:28`). Check: helper exists outside text units.
- [ ] `with_child_stagger(step,order)` (`ui_motion.h:358`). Check: grep > 0.
- [ ] `motion::any_active(Entity)` (`store.h:33`). Check: grep > 0.
- [ ] `Track::total_elapsed()` (`track.h:110`). Check: grep > 0.
- [ ] HasTracks prune/shrink; maps only grow (`store.h:33`). Check: prune() erases finished tracks.
- [ ] dt clamp in scaled_dt/AdvanceTracks (`store.h:29`). Check: ceiling present.
- [ ] OnDemand request_frame while tracks are active (`store.h:92`). Check: AdvanceTracks requests frames.
- [ ] e2e `wait_for_motion` (`e2e_testing/command_handlers.h:395`). Check: grep > 0.
- [ ] Auto register_bridge; tracks freeze otherwise (`ui_motion.h:430`). Check: unregistered tracks advance.
- [ ] Reduced motion != instant: `set_reduced` lands transforms, keeps ~150ms opacity/colour (`track.h:91`). Check: grep set_reduced > 0. [then-default]
- [ ] MotionRule essential flag → `Track::essential` (`ui_motion.h:68`). Check: grep essential in ui_motion.h > 0. [then-default]
- [ ] Per-property mode split / opacity no-overshoot; single mode today (`ui_motion.h:71`). Check: pop_in springs scale, eases opacity. [then-default]
- [ ] Colour tracks reject bouncy springs (`ui_motion.h:300`). Check: bounce>0 clamped/logged for colour.
- [ ] Per-property rest-delta wiring; `Spring.rest_delta` never set (`spring.h:17`). Check: deltas assigned per property. [then-default]
- [ ] `on_state(state,props,enter,exit)` overload (`ui_motion.h:375`). Check: 4-arg form exists.
- [ ] Delay param on all trigger builders; only on_appear has it (`ui_motion.h:358`). Check: on_hover with delay compiles.
- [ ] Asymmetric `MotionRule.release`; today release_mode=mode (`ui_motion.h:172`). Check: distinct press/release modes. [then-default]
- [ ] Replay `um::replay` + on_appear reset-on-hide/key; appeared never reset (`ui_motion.h:103`). Check: grep replay > 0. [then-default]
- [ ] `um::initial(props)` pose builder (`ui_motion.h:358`). Check: grep > 0.
- [ ] `um::loop(props,keyframes)` builder (`ui_motion.h:358`). Check: grep > 0.
- [ ] Presence/on_exit primitive; only a modal-private Phase exists (`ui_motion.h:66`). Exit-hold need already tracked in the widget-lifetime item above; this is the motion API half. Check: grep on_exit > 0. [then-default]
- [ ] Layout animation / FLIP `with_layout_animation` with last-rect (`store.h:33`; `autolayout.h`). Check: a rect change glides.
- [ ] Shared-element `with_shared_id(key)` (`ui_motion.h:44`). Check: grep > 0.
- [ ] scale_x/scale_y in MotionProps + HasUIModifiers + render (`ui_motion.h:44`; `components.h:311`). Check: non-uniform scale renders.
- [ ] skew_x/skew_y props (`ui_motion.h:44`). Check: grep > 0.
- [ ] rotate_x/rotate_y/perspective props (`ui_motion.h:44`). Check: grep > 0.
- [ ] `clip_inset{t,r,b,l}` prop + feathered mask (`ui_motion.h:44`). Check: grep > 0.
- [ ] MotionProps text/border colours; background only today (`ui_motion.h:52,300`). Check: tab/radio fades expressible. [then-default]
- [ ] Motion translate as Size/h720 incl. slide_in fraction + text-unit from_x/y; raw px today (`ui_motion.h:22`). Check: resolves via resolve_to_pixels. [then-default]
- [ ] Origin from Placement `with_origin_from`/`origin_for`; Placed.used discarded (`component_config.h:659`; `overlay.h:15`). Check: grep > 0. [then-default]
- [ ] on_focus restricted to bg/opacity + warn (`ui_motion.h:371`). Check: transform focus rejected.

### UI widgets

- [ ] Toast fixed enter/exit tracks + dismiss exit phase; lifetime-expo only, dismiss instant (`toast.h:70,79,308`). Check: 3s/10s toasts share exit time. [then-default]
- [ ] Toast swipe-dismiss (`toast.h`). Check: grep swipe > 0.
- [ ] Toast stack slot glide; y written directly (`toast.h:295`). Check: siblings glide. [then-default]
- [ ] Toast pin + wrapped height + length-scaled duration; fixed h720(48) (`toast.h:27`). Check: grep pin > 0.
- [ ] Toast visible cap + '+N more' (`toast.h:277`). Check: 10 toasts capped. [then-default]
- [ ] Toast scaled_dt pause/time_scale; elapsed += raw dt (`toast.h:251`). Check: pause freezes toast.
- [ ] Toast lifetime pauses on hover (`toast.h:251`). Check: hovered toast persists.
- [ ] Tooltip warm window + fade/scale enter track; owner change restarts 0.5s, pop-in (`tooltip.h:27,108`). Check: warm neighbour instant, cold fades. [then-default]
- [ ] Modal presence→transform (Center scale / edges translate); opacity only (`modal.h:449`). Check: drawer slides. [then-default]
- [ ] Modal drag-to-close rubber-band + velocity (`modal.h:53`). Check: grep drag > 0.
- [ ] Backdrop separate faster-exit track; shares presence (`modal.h:449`). Check: backdrop outruns panel.
- [ ] Cross-modal backdrop handover; flash/double-dim today (`modal.h:449`). Check: alpha held across swap. [then-default]
- [ ] Modal finish at presence<0.02 (`modal.h:367`). Check: early close.
- [ ] Exiting modal pointer-transparent; gate blocks UI (`modal.h:263`). Check: clicks pass during exit.
- [ ] Non-top modal scale .96 + dim push-back (`modal.h:136`). Check: stacked modal recedes.
- [ ] Menu/popover presence anchor-origin enter (`menu.h:101`). Check: scale+fade from anchor. [then-default]
- [ ] Menu/popover exit fade, instant on select (`menu.h:80`). Check: outside-dismiss fades. [then-default]
- [ ] Dropdown tray presence + rotating chevron entity; glyph swap today (`imm_containers.h:119,159`). Check: chevron rotates. [then-default]
- [ ] Tab sliding indicator as a single Rect track; border teleports (`imm_containers.h:402`). Check: indicator glides. [then-default]
- [ ] Tab prev-index exposure (`imm_containers.h:365`). Check: prev index readable.
- [ ] Direction-aware index-delta slide helper (`imm_containers.h`). Check: grep > 0.
- [ ] Checkbox `draw_partial(path,t)` mark track (`imm_controls.h:163`; `rendering.h:813`). Check: mark draws in ~150ms. [then-default]
- [ ] Checkbox bg on_state fade wiring (`imm_controls.h:283`). Check: 120ms crossfade. [then-default]
- [ ] Radio dot/dash scale+fade tracks (`imm_controls.h:423`). Check: dot pops. [then-default]
- [ ] Toggle knob press-widen track (`imm_controls.h:587`). Check: 20→24 while active. [then-default]
- [ ] Toggle whole-row >=44 + h720 track + hover-lighten (`imm_controls.h:495,554`). Check: the row toggles.
- [ ] Slider grab colour track, position 1:1 (`imm_value.h:196`). Check: colour steps on grab. [then-default]
- [ ] Stepper/nav label slide cue (`imm_value.h:666`; `imm_containers.h:247`). Check: directional slide. [then-default]
- [ ] Progress `with_value_motion` tween + indeterminate, bar and circular (`imm_value.h:379`). Check: grep > 0.
- [ ] Skeleton/shimmer primitive (`imm_value.h`). Check: grep skeleton > 0.
- [ ] `hold_button(ctx,ep,seconds)` fill track (`modal.h:1098`). Check: grep > 0.
- [ ] Collapsible/accordion measured-height primitive (`imm_containers.h`). Check: grep > 0.
- [ ] Upstream badge primitive (WM `NotificationBadge.h`). Check: grep 'badge(' in vendor > 0.
- [ ] Upstream NumberTicker/rolling_label (WM `rolling_number.h`). Check: grep in vendor > 0.
- [ ] Tree rotating chevron + `hash(node_id)` keys (`tree_view.h:70`). Check: keys stable across rebuilds.
- [ ] virtual_list empty-state slot (`imm_virtual_list.h:157`). Check: count=0 renders the slot.
- [ ] Dropdown label-width option; scale_x(0.5) hardwired (`imm_containers.h:85`). Check: grep > 0.
- [ ] split_pane code-set ratio glide (`imm_layout.h:265`). Check: programmatic ratio glides.
- [ ] Split divider 12px grab + offset tracking (`imm_layout.h:151`). Check: hit rect wider than drawn.
- [ ] Button press feedback: Theme `press_scale` + `with_press_scale` + on_press wiring (`imm_controls.h:32`). Check: grep press_scale > 0. [then-default]
- [ ] Pressed colour API + renderer paints pressed_bg (`color.h:534`; `rendering.h:1719`). Check: grep > 0.
- [ ] Animated hover colour track vs instant swap (`rendering.h:1721`). Check: hover crossfades. [then-default]
- [ ] Focus ring travelling-rect spring (`rendering.h:229`). Check: ring glides between widgets.
- [ ] `hit_outset` min 44px targets; hit = visual today (`systems.h:129`). Check: grep > 0. [then-default]
- [ ] Fixed slider thumb h720(20)/44, value maps over w−thumb (`imm_value.h:219`). Check: thumb size fixed. [then-default]
- [ ] Click hysteresis hit+10px + drag-away clears hot (`context.h:126,387`). Check: leaving and returning restores the click.
- [ ] `Theme::lerp` (`theme.h:169`). Check: grep > 0.
- [ ] Blur-behind-panel flush ordering (`rendering.h:2748`). Check: panel blurs its background.
- [ ] Blur clamp/warn/reduced-skip at HasBlur write (`ui_motion.h:424`). Check: out-of-range clamps and logs.
- [ ] Effect radius/axis/origin params + Blur crossfade; radius ×6 hard-coded (`effect_presets.h:80,91`). Check: params in signatures. [then-default]
- [ ] `slide_in(Direction,...)` from any side (`basic_presets.h:37`). Check: vertical slide compiles.
- [ ] shake/spin/pulse as rules + shake `amplitude_px` (`basic_presets.h:42`). Check: they return rules.
- [ ] Drag Event `release_rect`/`source_rect` (`components.h:816`). Check: grep > 0.

### Text/input

- [ ] `tracking_em(font_px)`, drop the +1px constant (`rendering.h:884`). Check: grep > 0.
- [ ] `letter_spacing_em` (`component_config.h:108`). Check: grep > 0.
- [ ] Font tiers {weight, tracking_em, leading} (`theme.h:98`). Check: a tier applies all three.
- [ ] Styled-label line-spacing parity (`rendering.h:906`). Check: styled labels honour line height. [then-default]
- [ ] `Theme::text_scale` (`theme.h:326`). Check: grep > 0.
- [ ] Label font size scales with `HasUIModifiers::scale` (`components.h:318`). Check: text grows with the modifier.
- [ ] `with_tabular_numbers()` (`text_unit_motion.h`). Check: grep > 0.
- [ ] TextUnitMotion Mode field; hard-coded quad today (`text_unit_motion.h:17`). Check: field exists. [then-default]
- [ ] Removed-unit exit crossfade (`text_unit_motion.h:95`). Check: deleted chars fade out.
- [ ] Prefix/suffix/LCS text diff; index compare only (`text_unit_motion.h:103`). Check: an inserted prefix remaps the suffix.
- [ ] Stagger total cap `max_total`; 2000 chars = 80s today (`text_unit_motion.h:24`). Check: total stagger bounded.
- [ ] `with_max_length` builder/field (`text_input/state.h:25`; only a comment in `component_config.h:36` today). Check: grep outside the comment > 0. [then-default]
- [ ] max_length codepoint semantics, not bytes (`utils.h:113`). Check: a multibyte char counts as 1.
- [ ] Grapheme-cluster caret/delete (`utils.h:131`). Check: a flag emoji deletes whole.
- [ ] Unicode word boundaries (`utils.h:194`). Check: Alt+arrow stops at ideographs.
- [ ] CJK/Thai line breaks (U+200B/3000/NBSP/`-`/`/`) (`text_selection.h:255`). Check: CJK text wraps.
- [ ] Indic/Thai/tag extends U+0900–0DFF, U+E0020–E007F (`text_units.h:35`). Check: combining marks attach.
- [ ] CJK word units + U+3000 space (`text_units.h:44`). Check: per-char split.
- [ ] text_area `scroll_offset_x` with wrap off (`text_area.h:417`). Check: horizontal scroll follows the caret.
- [ ] UTF-8 mask char storage (`component_config.h:176`). Check: '•' renders as the mask.
- [ ] Runtime-scaled MIN_FONT floor (`rendering.h:378`). Check: floor scales with ui_scale. [then-default]
- [ ] Slider keys Shift/Pg/Home/End (`imm_value.h:259`). Check: Home sets 0. [then-default]
- [ ] Revisit clipboard-disable API shape before it spreads: keep `with_copy_disabled()` / `with_cut_disabled()` / `with_paste_disabled(bool)` vs one `with_clipboard_actions_disabled(ClipboardAction::Copy | ClipboardAction::Cut)` flag-set sugar (`component_config.h:422`). Decide on discoverability, dynamic toggling, and replace-vs-accumulate semantics; do not reuse the sequential `EditCommand` enum. Check: one chosen spelling, tests updated.

### Scroll/drag

- [ ] Scroll momentum velocity decay (`components.h:536`). Check: grep scroll_velocity > 0.
- [ ] Rubber-band overscroll + spring-back; hard clamp today (`components.h:619`). Check: edge stretch and return. [then-default]
- [ ] `scroll_smoothing` as a time-constant/spring (`components.h:563`). Check: glide is refresh-independent.
- [ ] Trackpad fractional-delta smoothing bypass (`systems.h:2239`). Check: no double easing.
- [ ] Nested scroll chaining with remainder (`systems.h:2190`). Check: wheel chains outward only at the edge.
- [ ] Keyboard Pg/Home/End scrolling (`systems.h:2190`). Check: grep PAGE > 0.
- [ ] Scroll-edge fade (`components.h`). Check: grep > 0.
- [ ] `scroll_progress` + `on_enter_viewport` hooks (`components.h:536`). Check: grep > 0.
- [ ] Animated `glide_to` (`components.h:592`). Check: grep > 0.
- [ ] Scrollbar hit thickness + hover widen (`components.h:678`). Check: hit 16 / drawn 6. [then-default]
- [ ] Scrollbar reserved gutter (`components.h:671`). Check: content inset by the gutter. [then-default]
- [ ] `scroll_speed` as a Size (`components.h:560`). Check: field type is Size. [then-default]
- [ ] Pointer history + `release_velocity` (`context.h:104`). Check: grep > 0.
- [ ] Drag pointer-velocity into the settle spring (`systems.h:1738`). Check: a throw carries into the settle.
- [ ] Drag lift / spacer-grow / drop-fly + sibling FLIP (`systems.h:1701-1935`). Check: siblings glide, dropped item flies. [then-default]
- [ ] ease_scroll/pinch request_frame wake (`components.h:637`; `gestures_macos.h:111`). Check: grep at both sites > 0.

### Backends

- [ ] OS reduced-motion queries on Windows/Linux + re-read on focus; `#else false` today (`reduced_motion.h:23`). Check: non-Apple returns the OS value.
- [ ] OS reduce-transparency / increase-contrast queries (`reduced_motion.h`). Check: grep > 0.
- [ ] Theme reduce_transparency / high_contrast switches (`theme.h:319`). Check: grep > 0.
- [ ] Clipboard `get_text(max_bytes)` through all layers (`clipboard.h:14`). Check: truncation lands on a codepoint boundary.
- [ ] Zero-cost `warn_once` keys: callers hand-build `std::string` keys and the gate still does a set lookup when logging is lean/compiled out (`warn_once.h:34`). Want key parts built only on first fire, no allocation or lookup on the hot path, and the macro to compile out entirely under lean logging. Check: a hot-path `warn_once` does no string/set work when it does not fire; first fire still warns exactly once.
- [ ] pseudo_locale Accented mode (`pseudo_locale.h:33`). Check: grep > 0.
- [ ] Particle Emitter reduced-motion flag (`particles.h:29`). Check: grep > 0.

## Filed todos

- [x] `mk()` index param: passing it is unnecessary unless the call is in a loop. Audit all `mk(parent, idx)` call sites, drop the index where the call is not in a loop, and commit the cleanup in one full commit. DONE: 1,314 sites dropped (1,279 straight-line in `for_each_with` + 35 helper sites proven single-invocation per parent); kept where the index is load-bearing (loops, multi-invocation helpers like `image()`/`group()`/`label()`/`action()` in AIMChat, `NotificationBadge`, `MenuShowcase::menu()`). `mk()` identity is a hash of parent + index + call-site location, so an index is only needed when one call site runs several times for the same parent.
- [ ] Comments: at most two lines each; prefer no comment where the code is already easy to understand. Exception: a comment that explains an advanced afterhours concept to someone learning the library is valuable — keep it, within two lines. Basic readability/contrast (light text on dark background) is not advanced — no comment.
- [ ] Confusing code: add a `// TODO: reevaluate` marker instead of a long explanation, and simplify the code itself when the fix is small (e.g. named lookup over magic indices).

## Vulkan (afterhours) — remaining

Vulkan v1 (Phases 0–5) is done and pushed; the full plan lived in `plan.md`, now deleted. What remains is Windows runtime verification (blocked: no Windows box yet) plus follow-ups that were not v1.

- [ ] Windows runtime verification. Setup: up-to-date GPU driver (provides `vulkan-1.dll`); LunarG Vulkan SDK (required for debug builds — sokol requests `VK_LAYER_KHRONOS_validation` under `SOKOL_DEBUG`); `vulkaninfo` shows apiVersion ≥ 1.3 and `VK_EXT_descriptor_buffer` listed. Check: `/tmp/vulkan_proof/clear.exe` shows a colored window with clean validation; `examples/windows/demo.exe` draws bouncing entities, survives resize/minimize/restore over a few minutes, zero validation errors; scratch headless `.exe` (`/tmp/vulkan_headless_check.exe`) prints PASS (`graphics::init(Headless)` → render texture with non-zero id, `sg_query_image_state` VALID); all `win-vulkan` `.exe`s pass; `render_capture_vulkan_test.exe` PNG pixels match the Metal baseline, as do `sokol_blend_test` / `sokol_geometry_test` pixel values (small rounding diffs OK, document any accepted); blur use on Vulkan throws the not-implemented exception (the demo itself doesn't use blur).
- [ ] Windowed screenshot on Vulkan: `PrintWindow(sapp_win32_get_hwnd(), …, PW_RENDERFULLCONTENT)` + stb PNG. If a `take_screenshot` caller appears first, add a fail-loud stub branch to the `take_screenshot` wrapper in `backend.h` instead of leaving a dangling reference.
- [ ] Linux Vulkan (same abstraction as Windows Vulkan; only platform defines differ).
- [ ] Windows CI job. Verify lavapipe vs SwiftShader descriptor-buffer support before relying on software Vulkan for CI; Linux headless CI would cover backend logic but not the Win32 `sokol_app` / zig-MinGW build / `.exe` packaging.
- [ ] Real Vulkan blur shader: needs a SPIR-V path (sokol-shdc or equivalent). shdc acquisition is heavy (139 MB vendored compiler sources, no prebuilt binary, needs an unreleased zig). Blur stays throwing not-implemented until then.
- [ ] Fix the pre-existing web build failure: `crash_handler.h` includes `<execinfo.h>`, which Emscripten doesn't provide. Proven pre-existing (fails on the pre-Vulkan base too) — not a Vulkan regression.
- [ ] Regenerate `vendor/vulkan/vulkan-1.def` on sokol bumps or new afterhours `vk*` calls: it lists every `vk*` symbol sokol links plus the ones afterhours itself calls (Phase 4 added `vkCmdCopyImageToBuffer`, `vkCmdResolveImage`, `vkFreeCommandBuffers`). Procedure: compile without the lib and collect the undefined-symbol errors (or generate from Vulkan-Headers' `vk.xml`).
- [ ] Optional trim of `vendor/vulkan/` from the full 52-file Vulkan-Headers drop to the 17 used files. Kept the full drop for now by maintainer choice.
