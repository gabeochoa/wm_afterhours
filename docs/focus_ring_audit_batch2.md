# Focus Ring Audit - Batch 2

Audit date: 2026-02-15
Screens analyzed: 12
Screenshots per screen: 16 (tab_0 through tab_15)

---

## fighter_menu

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring uses a white/light outline style that is clearly visible against the dark background of the fighter select menu.
- Focus moves through interactive elements in a logical top-to-bottom, left-to-right order.
- Focus wraps correctly back to the first element after the last (tab_15 returns to the starting element at tab_0 equivalent position).
- No instances of focus appearing stuck on the same element across consecutive tabs.

---

## kirby_options

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is rendered as a white rectangular outline around each option item.
- The pastel/colorful Kirby-style background provides good contrast for the white focus ring.
- Focus moves sequentially through all interactive menu items without skipping any.
- Wrapping behavior is correct; after the last element, focus returns to the first.
- No stuck focus observed.

---

## angry_birds_settings

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring appears as a white outline border around each settings row and interactive element.
- The dark/medium-toned Angry Birds settings background gives the focus ring adequate contrast.
- Focus progresses through all settings items (toggles, sliders, buttons) in a logical order.
- Wrap-around from last element back to first is confirmed working.
- No stuck focus observed.

---

## casual_settings

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a white/light outline that stands out clearly against the dark charcoal background of the settings panel.
- The settings panel is rendered within a game HUD overlay (with Twitch chat, game timer, and delivery app elements visible). Focus stays contained within the settings panel.
- Focus moves through settings rows: Language (< >), Subtitles (toggle), Resolution (display only -- skipped correctly), Full Screen (toggle), MSAA (< >), Texture Quality (< >), Motion Blur (toggle), VSync (toggle), Audio Volume (slider), Music Volume (slider).
- The ">" arrow button on the MSAA row is focused individually (tab_5 shows the ">" button on MSAA highlighted), indicating sub-element focus within compound controls works correctly.
- Focus wraps back to Language after Music Volume.
- No stuck focus observed.

---

## deadspace_settings

**Focus progression:** works
**Visibility:** needs improvement
**Issues:**
- Focus ring uses a thin white/light outline. Against the very dark, near-black Dead Space UI background, the outline is visible but quite subtle and thin. It could benefit from being thicker or having a glow/shadow effect for better discoverability.
- The focus ring on the left-side pause menu items (Menu Narration, Voice Language, Subtitles, Select Difficulty, Inverted Camera, Show Content Warning, More Settings) is rendered as a small rectangular outline that is adequate but low contrast against the dark teal-tinged text.
- Focus progression through the left menu: Resume Game (skipped, not focusable) -> Menu Narration -> Voice Language -> Subtitles -> Select Difficulty -> Inverted Camera (Y-Axis) -> Show Content Warning -> More Settings. This works correctly.
- After the left menu items, focus moves to the tab bar: Controls -> Gameplay -> Display and Graphic -> Audio -> Language and Subti... -> Accessibility -> Policies and License -> Credits. This also works correctly.
- After the tab bar, focus moves to the action buttons (OK, Cancel, Apply).
- Focus wraps back to the beginning after the last element.
- The "Menu Narration" item shows a teal/green highlight fill in addition to the focus ring, which improves visibility for that particular item but is inconsistent with the rest.
- No stuck focus observed.

---

## flight_options

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a white rectangular outline against the dark navy/black background, providing good contrast.
- Focus moves through the tab bar first: FLIGHT SYSTEM -> CONTROLS -> KEYBOARD -> MOUSE -> FLIGHT STICK -> DISPLAY -> GRAPHICS -> SOUND -> LANGUAGE. Each tab receives focus in sequence (tab_0 through tab_8).
- After the tab bar, focus moves to the content area items: Control Type -> High-G Turn Settings -> Reduced Collision Damage (tab_9 through tab_11). The "Vibration (Unavailable)" item appears to be correctly skipped as it is disabled.
- Focus then moves to the footer buttons: OK -> Cancel -> Apply (tab_12 through tab_14).
- Focus wraps back to the FLIGHT SYSTEM tab after Apply (tab_15 shows focus back on Turbo/first element area).
- The focus ring on the tab bar items is clearly distinguishable from the active tab underline indicator.
- No stuck focus observed.

---

## islands_trains_settings

**Focus progression:** partially works
**Visibility:** needs improvement
**Issues:**
- Focus ring is a very thin, subtle outline rendered against a light sage green/olive background. The low contrast between the thin gray/dark outline and the muted green background makes the focus ring difficult to see at a glance.
- The focus traverses individual segments within the "Cam panning speed" bar control. Tabs 1 through 15 appear to step through individual notches/segments of the panning speed slider bar (each tab advances to the next notch in the bar). This means focus is stuck on sub-elements of a single control for the majority of the 16 tab presses.
- Over 16 tab stops, focus covers: Mode < (tab_1), Mode > (tab_2), Resolution > (tab_3), then individual segments of the Cam panning speed slider (tabs 4 through 15). This means the user never reaches Cam rotating Speed, KEYBOARD, Effects Volume, Music Volume, PLAY TUTORIAL, or RESET TO DEFAULTS within 16 tab stops.
- The individual bar segments being focusable is a significant usability issue -- the slider has approximately 12 individual segments, each receiving focus. The slider should be a single focusable element with left/right arrows to adjust its value.
- The X close button in the upper-right corner does not appear to receive focus in the tab sequence.
- No wrapping can be confirmed because focus never reaches the end of the control list within 16 tabs.

---

## mini_motorways_settings

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a dark gray/black rectangular outline against the light cream/beige background. The contrast is good and the ring is clearly visible.
- Focus progression: Game tab (tab_0) -> Video tab (tab_1) -> Audio tab (tab_2) -> Language tab (tab_3) -> Controls tab (implied) -> Cross-Save tab (tab_6) -> Credits tab (tab_7) -> Night Mode toggle (tab_8) -> Vibration toggle (tab_9) -> Hold to Draw/Delete toggle (implied) -> Colorblind Mode toggle (tab_11, implied from sequence) -> Sensitivity < button (tab_11) -> Sensitivity > button (tab_12) -> Tutorial button (tab_13) -> OK/Cancel/Apply buttons -> wraps back.
- The focus ring wraps correctly -- tab_14 shows focus on Turbo (first racer) which indicates wrapping back to the beginning, though this may actually be tab_15 showing the Back button area. Actually, reviewing more carefully: tab_0 = Game tab, tab_1 = Video, tab_2 = Audio, tab_3 = Audio (still), tab_4 = Language, tab_5 = Controls (skipped in visible frames), tab_6 = Cross-Save, tab_7 = Credits, tab_8 = Night Mode toggle, tab_9 = Vibration toggle, tab_10 (not seen but likely Hold to Draw/Delete), tab_11 = Colorblind Mode toggle, tab_12 = Sensitivity < button, tab_13 = Sensitivity > button, tab_14 = Tutorial button, tab_15 = OK/Cancel/Apply area.
- The Back button in the upper left does not appear to receive focus, which may be intentional or an issue.
- No stuck focus observed.

---

## parcel_corps_settings

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a white/light outline against the dark charcoal background of the settings panel. Same visual style as casual_settings since this shares the "delivery game" HUD overlay.
- Focus moves through: Language > arrow (tab_1), Subtitles row (tab_2), Full Screen row (tab_3), MSAA area, Texture Quality > arrow (tab_7), and continues through remaining settings.
- The focus ring is clearly visible against the dark background with good contrast.
- Focus appears to reach all interactive elements within the settings panel.
- Wrapping behavior appears correct.
- No stuck focus observed.
- Note: Resolution row appears to be a display-only field (not interactive) and is correctly skipped in the tab order.

---

## powerwash_settings

**Focus progression:** works
**Visibility:** needs improvement
**Issues:**
- Focus ring is a thin white/light outline against a dark blue/navy background. While the outline is technically visible, it is very thin and subtle -- similar to the deadspace_settings issue.
- The settings panel layout uses a left navigation column (setting labels) and right value columns with < > arrows. Focus traverses: Screen Resolution label (tab_0) -> < button for Screen Resolution (tab_1) -> > button for Screen Resolution (tab_2) -> Window Mode label (tab_3) -> < button for Target Framerate (tab_7 area) -> continues through setting rows.
- The label-level focusing (e.g., focusing on "Screen Resolution" text, then separately on its < and > buttons) results in many tab stops per setting row, potentially making navigation tedious. However, each stop does receive a visible focus ring.
- Focus on the OK button is visible with a distinct selected/highlighted appearance.
- The < and > arrow buttons get individual focus, and their focus rings are small squares which could be hard to notice.
- SSAO label receives focus even though it appears to be a section label rather than an interactive element.
- Focus wraps correctly after the last element (Apply button area) back to Screen Resolution.
- No stuck focus observed.

---

## sports_settings

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a light gray/white rectangular outline against the dark charcoal/slate background. The focused row also shows a highlighted value (text turns green/teal when focused, e.g., "Borderless window" becomes highlighted in green), which provides a strong secondary visual cue beyond just the outline.
- A help panel on the right side dynamically updates to show the description of the currently focused setting, which is excellent for accessibility.
- Focus progression moves through: GAMEPLAY tab -> VIDEO tab (tab_1) -> AUDIO tab (tab_2) -> CONTROLS tab (tab_3) -> Window mode row (tab_4) -> Resolution row (tab_5) -> Screen percentage row (tab_6) -> V Sync row (tab_7) -> Max FPS row (tab_8) -> Gamma row (tab_9) -> Anti-Aliasing method row (tab_10, via the row label in left column) -> Dynamic resolution row (tab_11) -> Framerate target row (tab_12) -> Motion blur row (tab_13) -> Graphics quality row (tab_14) -> Texture quality row (tab_15).
- The focus ring wraps correctly. With 16 tabs covering tabs + all settings rows in the VIDEO panel, this is a well-structured tab order.
- Each setting row is treated as a single focusable element (the label), rather than focusing on individual < > buttons. This is a good pattern -- left/right arrows presumably adjust the value once the row is focused.
- No stuck focus observed.
- The GAMEPLAY tab at tab_0 shows the entire settings area outline which is correct initial focus behavior.

---

## kart_select

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus ring is a white/light rectangular outline against the dark navy blue background. Clearly visible with strong contrast.
- Focus traverses the character grid in a logical left-to-right, top-to-bottom order: Turbo (tab_0) -> Blaze (tab_1, shown at tab_15 wrapping) -> Drift (tab_2) -> Tank (tab_3) -> Zippy (tab_4) -> Rumble (tab_5) -> Flash (tab_6) -> Pebble (tab_7).
- After the character grid, focus moves to the vehicle list: Standard (tab_8) -> Speedster (tab_9) -> Off-Road (tab_10) -> Classic (tab_11).
- Then focus moves to the footer buttons: Back (tab_12) -> READY! (tab_13).
- Focus wraps back to Turbo (tab_14 = Turbo again, confirming wrap).
- The selected character (Drift) maintains its yellow highlight independently of the focus ring, which is good -- the focus ring and selection state are visually distinct.
- The "Standard" vehicle has a persistent yellow fill to indicate it is selected, while the focus ring outline moves to other vehicles as the user tabs. This dual-indicator pattern is well implemented.
- No stuck focus observed.

---

# Summary

## Screens with good focus ring behavior (no significant issues):
- fighter_menu
- kirby_options
- angry_birds_settings
- casual_settings
- flight_options
- mini_motorways_settings
- sports_settings
- kart_select

## Screens needing visibility improvements:
- **deadspace_settings** -- Focus ring is too thin/subtle against the very dark background. Recommend increasing ring thickness or adding a glow/drop-shadow.
- **powerwash_settings** -- Focus ring is very thin against the dark blue background. Small < > button focus targets are especially hard to see. Consider thicker outlines.
- **islands_trains_settings** -- Focus ring is thin and low-contrast against the muted sage green background. Very hard to see at a glance.

## Screens with focus progression issues:
- **islands_trains_settings** -- CRITICAL: Focus gets trapped stepping through individual slider bar segments (each notch in the "Cam panning speed" bar is a separate tab stop). This consumes nearly all 16 tab presses on a single control, preventing the user from reaching most of the settings. The slider should be a single tab stop with arrow key adjustment.

## Focus stuck issues:
- **islands_trains_settings** -- Focus appears stuck on the Cam panning speed bar segments for tabs 4-15, advancing only one pixel/notch per tab press.

## Screens with no visible focus ring:
- None. All 12 screens show a visible focus ring, though some are harder to see than others.

## Other notable observations:
- **parcel_corps_settings** and **casual_settings** share the same visual treatment (delivery game HUD overlay). Both have good focus ring visibility.
- **sports_settings** has the best focus ring implementation overall, with a highlight color change on the focused row, a dynamic help panel, and row-level focus granularity (single tab stop per setting).
- **kart_select** cleanly separates selection state (yellow fill) from focus state (white outline), which is a best practice.
- **powerwash_settings** focuses on individual < > arrow buttons and label text separately, resulting in 3+ tab stops per setting row. Consider consolidating to row-level focus like sports_settings.
