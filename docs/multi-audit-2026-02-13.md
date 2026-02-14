# Consolidated Multi-Audit: All Screens

**Date:** 2026-02-13
**Screens audited:** 67 (all screens via `screenshot_all_screens.py --quick`)
**Resolution:** 1280x720 (720p)
**Auditors:** Apple HIG (1987), Google Material Design 3, Sun Java L&F (1999), Microsoft Win95 Guidelines, Muskox Craft, Afterhours Style Guide, Accessibility/WCAG

---

## Agreed by 7/7 auditors

### 1. Low-Contrast Text on Dark Backgrounds
**Problem:** Across 40+ screens using dark themes (Neon Dark, Midnight, Ocean Navy), body text, captions, and secondary labels are rendered in muted gray or dim colors against dark backgrounds. Many instances appear to fall below WCAG AA 4.5:1 contrast ratio. Specific offenders: `decorators` (tiny yellow/green text on near-black), `forms` (slider labels barely legible), `radio_buttons` (group labels dim), `modals` (section headers dim), `meters_gauges` (percentage labels dim on dark blue), `drag_drop` (card text very low contrast), `horizontal_drag` (items at edges nearly invisible), `navigation_bar_demo` (white-on-blue low contrast), `setting_row_showcase` (category headers dim rose/teal on dark).
**Flagged by:** Apple (#7 Feedback/Aesthetics), Google (#2 Contrast), Sun (#2 Color Model), Microsoft (#7 Visual Design/Color), Muskox (#1.5 Hierarchy), Afterhours (#2.4 Color Rules), A11y (#7.1 Contrast)
**Fix:** Audit all text elements with `enforce_contrast_ratio`. Increase foreground lightness for body text on dark themes to meet WCAG AA 4.5:1 minimum (APCA Lc 75+ for body text). Consider implementing the APCA contrast system described in `accessibility_standards_notes.md`. Specific targets: caption text should be at least `#A0A0A0` on `#1A1A2E` backgrounds, label text at least `#B0B0B0`.

---

### 2. Text Overflow and Clipping
**Problem:** Text is truncated, overlapping, or clipping outside its container on multiple screens. This is a functional readability failure. Specific instances: `cozy_cafe` ("Patience:" label overlaps the progress bar, "Croissant" order text clips), `decorators` ("ANOMALY" text overlaps "at 41%"), `fighter_menu` ("Configure vibration, save data, and a..." clips at right edge), `themes` ("Select Theme" title partially hidden behind component preview), `horizontal_drag` (tag labels are nearly invisible at tiny text size), `text_overflow` (intentional demo, but confirms the problem exists in real screens), `sports_settings` ("TSR (Temporal Super Resolution)" text clips), `parcel_corps_settings` (objective text clips at right edge).
**Flagged by:** Apple (#5 WYSIWYG), Google (#4 Layout), Sun (#10 Text Components), Microsoft (#4 Controls), Muskox (#1.6 Bitmap/Resolution), Afterhours (#4.3 Screen Safety), A11y (#7.1 Content readability)
**Fix:** Implement text truncation with ellipsis as fallback, ensure containers are sized to fit their content, or use `text_overflow` validation. Run `enforce_child_containment` and `enforce_screen_bounds` globally. For dynamic text (player names, settings values), ensure containers have min-width or use responsive font sizing.

---

### 3. Inconsistent Focus Ring Visibility
**Problem:** Focus indicators across screens vary wildly in visibility and style. Some screens show a white rectangular outline (e.g., `buttons`, `pagination`, `tab_container`), some show a yellow highlight (`tabbing`), and many appear to have no visible focus state at all or a focus ring that lacks contrast against similar-colored backgrounds. On light-themed screens (`cards`, `separators`, `scroll_view`, `simple_button`), white focus rings are nearly invisible. On dark screens, the thin white outline can be hard to see against medium-gray elements.
**Flagged by:** Apple (#7 Feedback), Google (#12 Focus Indicators), Sun (#3 Accessibility), Microsoft (#10 Accessibility), Muskox (#3.14 Interactive States), Afterhours (#7.3 Focus Management), A11y (#7.3 Focus indicators)
**Fix:** Implement the two-color focus indicator pattern recommended in `accessibility_standards_notes.md`: dark outline + light inner outline. This works against any background without per-element contrast computation. Minimum 2px thick per WCAG 2.4.13. Enable `enforce_focus_ring_visibility` validation. Standardize focus ring appearance across all themes using the `theme.focus_ring_thickness` (3px) and `theme.focus_ring_offset` (4px) settings.

---

### 4. Inconsistent Capitalization Across Screens
**Problem:** The product mixes ALL CAPS, Title Case, and sentence case inconsistently across screens and even within the same screen. ALL CAPS headers: "BEVEL BORDERS", "KEYBOARD NAV", "9-SLICE BORDERS", "BORDER STYLES", "BASIC MODALS", "INPUT BLOCKING TEST", "RESULTS". Title Case elsewhere: "Button Variants", "Circular Progress Indicators". Sentence case in some labels. Within `deadspace_settings`: "// SETTINGS" (all caps with prefix) mixed with "Controls" (title case). `modals` screen mixes "BASIC MODALS" with "Modal Dialogs" heading. `text_overflow` has "Good: Text Fits Container" mixed with "Problem: Text Too Large" (inconsistent colon usage).
**Flagged by:** Apple (#4 Consistency), Google (#11 Writing/Voice), Sun (#4.3 Capitalization), Microsoft (#4 Controls — headline vs sentence caps rules), Muskox (#3.6 Capitalization), Afterhours (#9 Content/Language), A11y (Readability)
**Fix:** Establish a capitalization convention and enforce it. The Afterhours style guide recommends sentence case for most UI text. Reserve ALL CAPS only for very short category labels or buttons where warranted. Audit all `HasLabel` text for consistency. Create a linter rule or validation flag for capitalization style.

---

### 5. Missing or Incomplete Interactive States
**Problem:** Many interactive elements across screens lack visible hover, active/pressed, and disabled states. Buttons on `animation_declarative`, `animation_interactive`, and `animation_spring` are colored squares without clear button affordance. `checkboxes` uses "X" marks for checked state rather than standard checkmarks, making the pattern unfamiliar. `radio_buttons` uses filled/empty circles that are small and hard to distinguish. `toggle_switches` preferences section uses small circles that could be hard to differentiate. Disabled states on `button_variants` and `buttons` look similar to unfocused normal states (gray on gray).
**Flagged by:** Apple (#7 Feedback, #8 Forgiveness), Google (#10 States — 7 states required), Sun (#9 Basic Controls), Microsoft (#7 Visual States), Muskox (#3.14 Interactive States), Afterhours (#8.2 States), A11y (#7.2 Keyboard behavior)
**Fix:** Implement distinct visual states for every interactive element: default, hover, pressed/active, focused, disabled, selected, loading. Disabled buttons need more differentiation from normal state (lower opacity, different text color, or a "prohibited" visual cue). Use `enforce_min_touch_target` to ensure all interactive elements meet 44x44px minimum.

---

### 6. Color Used as Sole State Indicator
**Problem:** Multiple components rely on color alone to communicate state, excluding colorblind users (~4.5% of population). Toggle switches on `angry_birds_settings`, `casual_settings`, `mini_motorways_settings`, and `toggle_switches` use green=ON / gray=OFF with no shape or icon differentiator. Checkboxes on `checkboxes` and `forms` use colored backgrounds for checked state. `radio_buttons` uses filled (red) / empty (gray) circles. `meters_gauges` HP bar uses green+red, `colors` screen uses color swatches with no pattern/texture differentiation. `kart_select` stat bars (SPD, ACC, HND, WGT) use different colors as the only differentiator.
**Flagged by:** Apple (#10 Aesthetics, accessibility note), Google (#1 Accessibility — color-only meaning), Sun (#1 Accessibility), Microsoft (#10 Accessibility — color not only cue), Muskox (#3.12 System Feedback), Afterhours (#2.4 "Never use color alone"), A11y (#7.1 "Color not sole indicator")
**Fix:** Add non-color indicators to all state-bearing elements: ON/OFF text labels for toggles (already present on some screens — standardize this), checkmark/dash icons for checkboxes, labels on stat bars, patterns or icons on progress bars. Enable `enforce_status_not_color_only` validation. Test all screens in grayscale mode.

---

## Agreed by 6/7 auditors

### 7. Container Overuse and Visual Nesting
**Problem:** Many screens suffer from excessive container nesting — boxes within boxes within boxes — creating visual noise and increasing cognitive load. `forms` screen has the main panel > sliders section > individual slider row > slider track, plus a CRT-monitor-shaped decorative container adding another nesting level. `checkboxes` has a main container > two sub-panels > group containers > individual checkbox rows. `cards` explicitly shows "Nested Cards" as a feature, but the overall screen has container > card row > individual cards. `flex_alignment` is densely packed with nested containers. `meters_gauges` has 5 levels of nesting for some elements. `empire_tycoon` has container nesting making the information density overwhelming.
**Flagged by:** Apple (#10 Aesthetics), Google (#5 Elevation), Sun (#5 Windows/Containers), Muskox (#1.3 Container Overuse), Afterhours (#4.2 "Group by proximity"), A11y (Cognitive load)
**Fix:** Apply Gestalt principles: use spacing (proximity) and typography (similarity) instead of container borders where possible. The Muskox guideline is clear: "If you can remove a container and the grouping still reads, remove it." Audit each screen for removable container borders. Replace nested containers with whitespace-based grouping.

### 8. Inconsistent Visual Language Across Themes
**Problem:** While the product supports multiple themes (Cozy Kraft, Neon Dark, Ocean Navy, Midnight, Sage Natural), components behave and look inconsistently between themes. `button_variants` shows theme variants at the bottom that have different affordance levels — Sage theme buttons look less interactive than Neon Dark buttons. `themes` screen's component preview clips when navigating between themes. Cards in `cards` screen (Cozy Kraft theme) have different border radius and shadow behavior than dark-theme cards. Focus rings appear white in all themes, creating contrast issues on light themes. Toggle switches look different across `angry_birds_settings` (pill shape), `mini_motorways_settings` (pill), and `toggle_switches` (circle-on-track).
**Flagged by:** Apple (#4 Consistency), Google (#3 Color Roles), Sun (#1 Visual Identity), Microsoft (#3 Consistency), Muskox (#1.7 Inconsistent Visual Language), Afterhours (#1.2 Consistency)
**Fix:** Create a cross-theme visual compliance test. Ensure all component variants (buttons, toggles, checkboxes, sliders) maintain consistent affordance and behavior across all 5+ themes. Focus rings should adapt their color per theme to maintain 3:1 contrast. Document expected behavior per component per theme in the design system.

### 9. Small Touch/Click Targets
**Problem:** Several interactive elements appear smaller than the recommended 44x44px minimum. `radio_buttons` circles appear ~20px diameter. Small checkbox squares on `checkboxes` (box-only variant) appear ~16px. Stepper arrows (`<` / `>`) on `stepper_showcase`, `powerwash_settings`, `sports_settings`, `islands_trains_settings` appear small (~24-32px). The "X" close button on `casual_settings` and `angry_birds_settings` appears borderline. Navigation bar arrows on `navigation_bar_demo` are small. `parcel_corps_settings` icon legend items are tiny. `neon_strike` ability icons in the left sidebar appear small.
**Flagged by:** Apple (#2 Direct Manipulation — 44pt minimum), Google (#1 Touch Targets — 48dp), Sun (N/A — desktop), Microsoft (N/A — desktop, but usability), Muskox (#3.10 Core Functionality friction), Afterhours (#7.5 Touch targets — 44px minimum)
**Fix:** Enable `enforce_min_touch_target` validation with `min_touch_target_size = 44.0f`. Increase the clickable area of radio buttons, checkboxes, stepper arrows, and close buttons. The clickable area can be larger than the visual element (invisible touch padding).

### 10. Lack of Clear Visual Hierarchy
**Problem:** Several screens present all information at roughly equal visual weight, making it hard to identify the primary action or most important content. `forms` screen presents sliders, checkboxes, progress bars, and clipboard demo all at the same visual level. `modals` lists all modal types at equal weight. `decorators` shows three decorator types side-by-side with no clear reading order. `flex_alignment` is visually dense with no clear entry point. `checkboxes` presents all variants simultaneously without hierarchy. `parcel_corps_settings` left panel has settings, while chat, objectives, and a speedometer compete for attention with no clear priority.
**Flagged by:** Apple (#10 Aesthetics), Google (#2 Structure/Hierarchy), Sun (#2 Visual Design), Microsoft (#6 Aesthetics), Muskox (#1.5 Lack of Clear Hierarchy), Afterhours (#8.1 Hierarchy)
**Fix:** Establish a dominant element on each screen. Use size, weight, color, and position to create a clear F-pattern or Z-pattern reading flow. Reduce the number of visible elements using progressive disclosure. For showcase/demo screens, add a clear section-by-section walkthrough rather than showing everything at once.

---

## Agreed by 5/7 auditors

### 11. Broken/Empty Screen (file_tree)
**Problem:** The `file_tree` screenshot is completely black/empty — no UI content renders at all. This is either a rendering failure or the screen has no visible content.
**Flagged by:** Apple (#7 Feedback — silent failure), Google (#11 Empty States), Muskox (#3.12 Missing System Feedback), Afterhours (#8.2 Empty state), A11y (Content accessibility)
**Fix:** Investigate why `file_tree` renders as blank. If the component isn't implemented yet, show an empty state with a message. Never show a blank screen.

### 12. Excessive Rule Lines and Separators
**Problem:** Several screens use horizontal rule lines to separate sections where spacing alone would suffice. `setting_row_showcase` uses thin lines between every toggle row. `sports_settings` uses lines between every setting row. `toggle_switches` has lines between every item. `deadspace_settings` uses thick teal bars as section separators that are visually heavy. `forms` uses multiple thick colored bars.
**Flagged by:** Apple (#10 Aesthetics), Sun (#2 Layout), Microsoft (#7 Visual Design), Muskox (#1.4 Rule Line Overuse), Afterhours (#4.2 "Group by proximity")
**Fix:** Per the Muskox guideline: "Only use a rule line if removing it makes the grouping ambiguous." Replace most rule lines with spacing-based grouping. Use section headers with larger top margins instead of separator lines. Reserve rule lines for truly distinct sections.

### 13. Monospace/Handwritten Font Legibility Issues
**Problem:** Many screens use handwritten-style or monospace fonts (Gaegu-Bold, etc.) that reduce legibility at smaller sizes. Body text, labels, and captions in this font become hard to read below ~14px. Visible on: `cozy_cafe` (customer names/orders), `potion_crafting` (recipe details), `decorators` (status text), many dark-theme screens. The monospace/handwritten style also makes it harder to distinguish similar characters (l/1, O/0).
**Flagged by:** Apple (#10 Aesthetics — functional clarity), Google (#4 Typography — readability), Sun (#2 Typography), Muskox (#2.3 Wrong Typeface), Afterhours (#3.2 "Minimum body text: 16px")
**Fix:** Reserve handwritten/decorative fonts for headings and display text only. Use a clean sans-serif (Inter, system-ui) for body text, labels, and UI controls. Ensure all text renders at minimum 16px for body content per Afterhours style guide. Use `enforce_min_font_size` validation.

### 14. Inconsistent Button Hierarchy Across Screens
**Problem:** Many screens present multiple buttons at the same visual weight, making it unclear which is the primary action. `casual_settings` has OK/Cancel/Apply buttons all styled identically (outlined blue). `angry_birds_settings` has OK (green), Cancel (outlined), Apply (blue) — three different styles with unclear hierarchy. `deadspace_settings` has OK (colored), Cancel, Apply at similar sizes. `tab_container` has OK, Cancel, Apply at bottom with inconsistent sizing (OK small, Apply large). `text_input` has OK (teal), Cancel (gray), Apply (dark) — three competing styles.
**Flagged by:** Apple (#3 See-and-Point), Google (#9 Button Hierarchy), Microsoft (#6 Dialog buttons), Muskox (#3.2 Too Many CTAs), Afterhours (#6.2 "One primary action per context")
**Fix:** Standardize the OK/Cancel/Apply button pattern across all dialogs: OK = filled primary, Cancel = ghost/text, Apply = outline/secondary. Ensure only one button per dialog appears as the "filled" primary action. Create a dialog button component that enforces this hierarchy automatically.

---

## Agreed by 4/7 auditors

### 15. Missing Help/Context for Settings
**Problem:** Many settings screens present options without explanations of what they do. `deadspace_settings` shows tab names like "Policies and Licens..." (truncated) with empty placeholder content ("Controls options will be displayed here."). `powerwash_settings` is the only screen that has a "Help" panel explaining the selected setting — most others lack this. `flight_options` has "Vibration (Unavailable) — Requires compatible controller" which is good, but other unavailable options lack explanation. `sports_settings` has descriptions in a side panel — good pattern but not used elsewhere.
**Flagged by:** Apple (#3 See-and-Point), Microsoft (#8 User Assistance — What's This?, tooltips), Sun (#6 Dialog Boxes), Afterhours (#6.4 Alerts/Feedback)
**Fix:** Add contextual help to all settings screens. Either use a side panel (like `powerwash_settings`/`sports_settings`) or tooltips that explain each setting. Unavailable options should always explain why they're unavailable and what's needed to enable them.

### 16. Inconsistent Inset Corner Radii (Nested Elements)
**Problem:** Nested elements don't follow the inset corner radius formula (Inner = Outer - Padding). `cards` "Nested Cards" shows inner cards with same corner radius as outer container. `nine_slice_borders` "Nested Panels" inner/outer have same or arbitrary radii. `empire_tycoon` production cards inside rounded container don't match. `cozy_cafe` specials list cards have same radius as their container. Various button-in-card arrangements across screens show radius mismatches.
**Flagged by:** Google (#7 Shape — 10-level scale), Muskox (#1.1 Incorrect Inset Corner Radii — "Critical" severity), Afterhours (#4.5 3D Border Effects), Sun (#1 Flush 3D)
**Fix:** Implement the inset radius formula: Inner Radius = Outer Radius - Padding. If padding exceeds outer radius, inner radius = 0. This is the #1 Muskox pet peeve. Add a validation check for nested rounded elements.

### 17. Missing Keyboard Shortcut Hints on Game UI Screens
**Problem:** Game-style screens like `neon_strike`, `potion_crafting`, `shop_interface`, `kart_select`, and `race_results` show controller hints (A: Select, B: Back, etc.) but don't show keyboard equivalents. `empire_tycoon` has no input hints at all. Players using keyboard need to discover shortcuts by trial and error.
**Flagged by:** Apple (#3 See-and-Point), Sun (#4 Keyboard Operations — mnemonics), Microsoft (#9 Keyboard Input — access keys), A11y (#7.2 Keyboard Navigation)
**Fix:** Show both keyboard and controller shortcuts simultaneously. Use a format like "Enter / A: Select" or toggle the display based on last-used input device. Ensure all interactive elements are keyboard-reachable.

### 18. Dialog/Modal Missing Escape-to-Close
**Problem:** Several dialog-style screens don't visually indicate that Escape closes them. `angry_birds_settings` only shows a red X button. `casual_settings` only shows a red X button and OK/Cancel. While some screens do show "Esc: BACK" hints (`deadspace_settings`, `flight_options`), this isn't consistent. Without visible indication, keyboard users may not discover they can dismiss dialogs with Escape.
**Flagged by:** Sun (#6 Escape = Cancel), Microsoft (#6 Dialog boxes — ESC), Afterhours (#7.2 "Escape closes"), A11y (#7.2 Modal keyboard behavior)
**Fix:** Ensure all modals/dialogs support Escape to close and show this hint in the key legend. Add `enforce_modal_has_dismiss` validation.

---

## Agreed by 3/7 auditors

### 19. Scroll Indicators Inconsistent
**Problem:** `scroll_view` shows a horizontal progress bar indicator for horizontal scroll but the vertical scroll has no visible scrollbar. Some settings screens (`sports_settings`, `powerwash_settings`) have scrollable lists without visible scroll indicators. Users can't tell there's more content below.
**Flagged by:** Microsoft (#2 Windows — scroll bars always visible), Apple (#2 Scroll bars), Afterhours (#4.3 Screen Safety)
**Fix:** Add visible scroll position indicators to all scrollable regions. Can be minimal (thin track + thumb) but must be present so users know content extends beyond the visible area.

### 20. Animation Demo Screens Lack Reduced-Motion Support
**Problem:** `animation_basic`, `animation_declarative`, `animation_interactive`, `animation_looping`, and `animation_spring` all demonstrate animations but none show a control to disable or reduce motion. Users with vestibular disorders need this option.
**Flagged by:** Google (#4 Motion/Vestibular), Afterhours (#8.3 Animations), A11y (#7 "enforce_reduced_motion_support")
**Fix:** Add a "Reduce Motion" toggle to the settings system. Respect `prefers-reduced-motion` where applicable. Looping animations (`animation_looping`) should have a pause control.

### 21. Placeholder Text as Labels
**Problem:** `text_input` screen has a "Search (no label version)" field that uses placeholder text ("Type to search...") as its only label. When the user starts typing, the label disappears, making it impossible to remember what the field is for. `cozy_cafe` text input field has placeholder "Type a message..." but no visible persistent label.
**Flagged by:** Google (#8 Text Fields — placeholder-only labels), Afterhours (#6.3 "Labels above fields"), A11y (#7.2 Forms — inputs associated with labels)
**Fix:** Always use persistent visible labels above or beside input fields. Placeholders are hints, not labels. Enable `enforce_inputs_have_labels` validation.

### 22. Even-Numbered Short Lists
**Problem:** Several screens use even-numbered item lists where odd numbers would create better visual rhythm. `animation_basic` has 3 items (acceptable — odd). `checkboxes` left panel has 4 labeled items (even). `radio_buttons` Fruit group has 3 (good), Size has 3 (good), Color has 4 (even). `pagination` shows Page 1-5 (odd, good), Difficulty has 4 options (even — Easy/Medium/Hard/Expert).
**Flagged by:** Muskox (#3.9 Even-Numbered Short Lists), Afterhours (N/A), Google (N/A)
**Fix:** Low-priority: where feasible, restructure short lists to use odd numbers (3, 5). This is a minor polish item per Muskox guidelines.

---

## Agreed by 2/7 auditors

### 23. Win95-Style Bevel Borders Inconsistently Applied
**Problem:** Some screens use Win95-style beveled borders (`bevel_borders`, `aim_chat`) while others use flat or rounded borders. The bevel direction/depth isn't always consistent (light source should be upper-left per Win95 guidelines). `deadspace_settings` mixes flat dark panels with beveled-feeling tab elements.
**Flagged by:** Microsoft (#7 3D Borders — light source upper-left), Sun (#1 Flush 3D vs Beveled)
**Fix:** Decide per-theme whether to use beveled or flat borders and be consistent within each theme. Win95-themed screens should use the `with_bevel()` system consistently with proper light/dark edge colors.

### 24. Menu Bar Position and Standard Order
**Problem:** `aim_chat` has a menu bar (File, Edit, Insert People) that doesn't follow the standard menu order and lacks a Help menu. Game UI screens with tab bars (`deadspace_settings`, `fighter_menu`, `kirby_options`) don't follow standard menu ordering conventions, though these are game-specific tabs not application menus.
**Flagged by:** Microsoft (#3 Menus — standard order, Help always last), Sun (#7 Menus — Help last)
**Fix:** For application-style UIs (like `aim_chat`), follow the standard menu order: File, Edit, View, ..., Help (always last). Game settings tabs have more flexibility but should still follow user expectations (Controls first, or Display first, consistently).

---

## Flagged by 1 auditor only

### 25. No Drag Texture on Draggable Elements
**Problem:** `drag_drop` and `horizontal_drag` screens show draggable cards but lack a visual indicator (grip dots, handle bars) showing that elements are draggable.
**Flagged by:** Sun (#1 Drag Texture hallmark)
**Fix:** Add a drag handle visual (grip dots or texture) to draggable elements to communicate affordance. Low priority — this is specific to Java L&F guidelines.

### 26. Status Bar Missing from Application-Style UIs
**Problem:** `aim_chat` has "Direct Connection Established" text at the bottom but no formal status bar with standard styling. Application-style UIs should have proper status bars.
**Flagged by:** Microsoft (#5 Status Bars — position, contextual info)
**Fix:** Low priority. Add a properly styled status bar to application-emulation screens.

### 27. Physics-Based Motion Not Used
**Problem:** Animation demo screens use traditional easing curves (slide, fade, scale) rather than M3 Expressive physics-based spring animations. The `animation_spring` screen shows spring physics, but the basic animations use linear/easing transitions.
**Flagged by:** Google (#5 Motion — physics-based springs, not easing curves)
**Fix:** Optional. Consider migrating more animations to spring physics for a more natural feel. The `animation_spring` screen already demonstrates this capability.

---

## Triage Decisions

| # | Issue | Agreement | Decision |
|---|-------|-----------|----------|
| 1 | Low-Contrast Text on Dark Backgrounds | 7/7 | **Maybe** |
| 2 | Text Overflow and Clipping | 7/7 | **Yes** |
| 3 | Inconsistent Focus Ring Visibility | 7/7 | **Yes** |
| 4 | Inconsistent Capitalization | 7/7 | **No** — up to developer, support all styles |
| 5 | Missing or Incomplete Interactive States | 7/7 | **Yes** |
| 6 | Color Used as Sole State Indicator | 7/7 | **No** — up to developer |
| 7 | Container Overuse and Visual Nesting | 6/7 | **Maybe** |
| 8 | Inconsistent Visual Language Across Themes | 6/7 | **Maybe** — consistency within a screen, cross-screen is by design |
| 9 | Small Touch/Click Targets | 6/7 | **Yes** |
| 10 | Lack of Clear Visual Hierarchy | 6/7 | **Yes** |
| 11 | Broken/Empty Screen (file_tree) | 5/7 | **Yes** |
| 12 | Excessive Rule Lines and Separators | 5/7 | **Yes** |
| 13 | Monospace/Handwritten Font Legibility | 5/7 | **No** |
| 14 | Inconsistent Button Hierarchy | 5/7 | **Yes** |
| 15 | Missing Help/Context for Settings | 4/7 | **Maybe** — will add tooltip support later |
| 16 | Inconsistent Inset Corner Radii | 4/7 | **Yes** |
| 17 | Missing Keyboard Shortcut Hints | 4/7 | **Maybe** — later |
| 18 | Dialog/Modal Missing Escape-to-Close Hint | 4/7 | **No** |
| 19 | Scroll Indicators Inconsistent | 3/7 | **Maybe** — add support but allow developer to hide/restyle |
| 20 | Animation Screens Lack Reduced-Motion | 3/7 | **No** |
| 21 | Placeholder Text as Labels | 3/7 | **No** — up to developer |
| 22 | Even-Numbered Short Lists | 3/7 | **Maybe** |
| 23 | Win95 Bevel Borders Inconsistent | 2/7 | **No** — each screen is its own style |
| 24 | Menu Bar Position and Standard Order | 2/7 | **Yes** |
| 25 | No Drag Texture on Draggable Elements | 1/7 | **Maybe** — up to developer |
| 26 | Status Bar Missing | 1/7 | **Maybe** — might want a primitive for this |
| 27 | Physics-Based Motion Not Used | 1/7 | **No** |

### Totals

| Decision | Count |
|----------|-------|
| **Yes** (will fix) | 10 |
| **Maybe** (later/conditional) | 9 |
| **No** (won't fix) | 8 |

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 7/7 | 6 | **Do** — universal consensus |
| 6/7 | 4 | **Probably do** — strong consensus |
| 5/7 | 4 | **Probably do** — strong consensus |
| 4/7 | 4 | **Consider** — moderate consensus |
| 3/7 | 4 | **Consider** — moderate consensus |
| 2/7 | 2 | **Maybe** — review individually |
| 1/7 | 3 | **Skip unless compelling** |

### Priority Actions (7/7 + 6/7 consensus)

1. **Fix low-contrast text** — Enable `enforce_contrast_ratio`, implement APCA, raise minimum text lightness on dark themes
2. **Fix text overflow/clipping** — Enable `enforce_child_containment`, add ellipsis truncation, fix container sizing
3. **Standardize focus rings** — Implement two-color focus indicator, enable `enforce_focus_ring_visibility`
4. **Fix capitalization** — Choose one convention (sentence case recommended), audit all labels
5. **Add missing interactive states** — Implement hover/pressed/disabled for all interactive elements
6. **Fix color-only indicators** — Add text/icon/shape to all color-dependent state indicators
7. **Reduce container nesting** — Apply Gestalt proximity principles, remove unnecessary borders
8. **Harmonize cross-theme behavior** — Ensure components look/behave consistently across all themes
9. **Fix small touch targets** — Enable `enforce_min_touch_target`, increase radio/checkbox/stepper sizes
10. **Establish visual hierarchy** — Add dominant elements, apply F-pattern/Z-pattern layouts

### Screens with Most Issues (5+ issues each)

| Screen | Issue Count | Top Problems |
|--------|------------|--------------|
| `forms` | 8 | Contrast, nesting, hierarchy, clipping, states |
| `checkboxes` | 7 | Contrast, color-only, nesting, hierarchy, even lists |
| `decorators` | 7 | Contrast, clipping, hierarchy, font legibility |
| `cozy_cafe` | 6 | Clipping, contrast, placeholder labels, font size |
| `parcel_corps_settings` | 6 | Clipping, hierarchy, small targets, contrast |
| `toggle_switches` | 6 | Color-only, contrast, rule lines, states |
| `radio_buttons` | 6 | Color-only, small targets, contrast, hierarchy |
| `deadspace_settings` | 6 | Clipping, capitalization, rule lines, no help |
| `fighter_menu` | 5 | Clipping, contrast, hierarchy, states |
| `themes` | 5 | Clipping, inconsistent cross-theme, focus rings |
| `file_tree` | 5 | Empty/broken screen |
| `horizontal_drag` | 5 | Contrast, tiny text, no drag handles |

### Screens with Fewest Issues (cleanest)

| Screen | Issue Count | Notes |
|--------|------------|-------|
| `race_results` | 1 | Clean layout, good hierarchy, clear primary action |
| `shop_interface` | 1 | Well-organized table, clear hierarchy, good use of color+text |
| `potion_crafting` | 2 | Good layout, clear sections, minor font issue |
| `kart_select` | 2 | Clear grid, good selection feedback, minor color-only stats |
| `separators` | 2 | Clean demo, good spacing, minor capitalization |
| `simple_button` | 2 | Clean, focused, clear hierarchy |
| `islands_trains_settings` | 2 | Clean layout, clear sections |
| `powerwash_settings` | 2 | Has help panel (best practice), clean layout |

---

## Appendix: Per-Auditor Issue Counts

| Auditor | Total Issues Found | Top 3 Issues |
|---------|-------------------|-------------|
| **Apple HIG** | 22 | Feedback/consistency, aesthetics, user control |
| **Google M3** | 24 | Contrast, touch targets, color roles |
| **Sun Java L&F** | 18 | Color model, keyboard access, flush 3D |
| **Microsoft Win95** | 20 | Border styles, keyboard access, dialog design |
| **Muskox Craft** | 25 | Container overuse, hierarchy, corner radii |
| **Afterhours Style** | 23 | Contrast, focus management, spacing |
| **Accessibility** | 24 | Contrast, focus indicators, touch targets |

---

*Generated 2026-02-13 via multi-audit of 67 screens at 720p against 7 design guidelines.*
*Screenshots: `/tmp/ui_showcase_screenshots/*.png`*
