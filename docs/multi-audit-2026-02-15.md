# Consolidated Design Audit: All Screens (2026-02-15)

**Screens audited:** 72
**Auditors:** Apple HIG (1987), Google Material Design 3, Sun Java L&F (1999), Microsoft Win95 Guidelines

---

## Agreed by 4/4 auditors

### 1. Text Clipping, Truncation, and Overflow
**Severity:** Critical
**Problem:** Text content is frequently truncated, clipped, or overflows its container across many screens without proper handling (no ellipsis, no wrapping, no scrolling). Tab labels in fighter_menu are truncated to illegibility ("System Opt", "Game Optic", "Display Opt"). Modal trigger buttons in modals are clipped at the viewport edge ("Simple M...", "Info Dia...", "Op..."). Card descriptions are cut mid-word. The stepper_showcase card selector concatenates options into "HealerWarriorMage" with no separation. In text_overflow, red-corner overflow warnings highlight the issue explicitly. Content extends beyond container boundaries in aim_chat, empire_tycoon, horizontal_drag, and themes. Many screens show text cut off mid-word with no fallback mechanism.
**Flagged by:** Apple (#7), Google (#3), Sun (#2), Microsoft (#1)
**Affected screens:** aim_chat, cards, cozy_cafe, empire_tycoon, fighter_menu, flex_alignment, horizontal_drag, hstack_showcase, images, kirby_options, modals, nine_slice_borders, parcel_corps_settings, stepper_showcase, text, text_overflow, themes
**Fix:** Implement consistent text overflow handling across all components: use text-overflow ellipsis with tooltip on hover for constrained single-line labels, enable text wrapping for multi-line contexts, and ensure no interactive elements extend beyond the viewport boundary. For tab labels, enforce minimum width constraints or use scrollable tab bars. Add overflow detection and automatic ellipsis to all text-rendering widgets. Never clip interactive elements at container edges.

---

### 2. Inconsistent, Low-Contrast Focus Indicators
**Severity:** Critical
**Problem:** Focus indicators vary wildly across screens in style, color, thickness, and visibility. Some controls use dashed white rectangles, some use solid colored borders (yellow in tabbing, teal in deadspace_settings, gold in potion_crafting), some use barely-visible thin outlines, and some controls appear to have no focus indication at all. On dark-themed screens, dark or thin white dashed outlines provide insufficient contrast. The tabbing screen uses a thick yellow border (one of the better examples) but this style appears nowhere else. Focus rings are often only 1-2 pixels wide and use colors with poor contrast against both light and dark backgrounds, making it extremely difficult for keyboard/gamepad users to identify the focused element.
**Flagged by:** Apple (#6), Google (#2), Sun (#3), Microsoft (#9)
**Affected screens:** absolute_positioning, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, button_variants, buttons, checkboxes, circular_progress, deadspace_settings, file_tree, flex_alignment, forms, hstack_showcase, kart_select, modals, navigation_bar_demo, pagination, potion_crafting, powerwash_settings, race_results, radio_buttons, scroll_view, setting_row_showcase, shop_interface, simple_button, sports_settings, stepper_showcase, tab_container, tabbing, text_input, toasts, toggle_switches
**Fix:** Standardize on a single focus indicator style across all themes: a 3px solid offset outline using a high-contrast color (bright white on dark themes, dark blue on light themes) with a minimum 3:1 contrast ratio against adjacent colors. Consider a dual-color inner+outer ring approach (e.g., 2px white + 2px dark) for universal visibility across all theme variants. Ensure the focus indicator is visually distinct from selection and active states.

---

### 3. Color Used as Sole Differentiator for State or Category
**Severity:** Major
**Problem:** Many controls rely solely on color hue to communicate state (on/off, selected/unselected), category, or meaning, failing colorblind users and high-contrast modes. Toggle switches show on/off purely through fill color without an icon or text inside the track. Radio buttons distinguish selected vs. unselected only through fill color. In kart_select, vehicle selection uses colored circles with no text or shape differentiation. In tabbing, Options A-D are differentiated only by magenta, cyan, lime, and orange fills. In circular_progress, nine indicators are differentiated only by color. Meter/gauge bars (HP, MP, XP) use red/green/blue segments with no pattern or label. Toast types (Info, Success, Warning, Error) rely on teal/green/pink/red color coding. Race results use only colored bars for position data.
**Flagged by:** Apple (#12), Google (#8 -- checkbox/radio non-standard indicators), Sun (#6), Microsoft (#6)
**Affected screens:** auto_text_color, button_variants, buttons, checkboxes, circular_progress, colors, drag_drop, forms, horizontal_drag, kart_select, meters_gauges, neon_strike, potion_crafting, race_results, radio_buttons, tabbing, toasts, toggle_switches
**Fix:** Never rely on color alone to convey meaning. Add redundant cues: text labels, icons, shape variation, or pattern fills (stripes, dots, cross-hatch) alongside color. Toggle tracks should include I/O symbols or checkmark icons. Progress indicators should display percentage text. Toast types should include distinct icons (i, checkmark, triangle, circle-X). All states must be determinable in grayscale.

---

### 4. Insufficient Color Contrast Rendering Text Unreadable
**Severity:** Critical
**Problem:** Many screens render text in colors nearly identical to their background, making text effectively invisible or extremely difficult to read. In file_tree, the entire screen is dark charcoal with no visible text. In deadspace_settings, teal-on-dark-navy text is extremely hard to read. In forms, dark blue input fields on dark navy show no visible placeholder text. In buttons, disabled states use gray-on-gray combinations. In flight_options, active items like "High-G Turn Settings" use light blue text on medium blue background. In text, body text uses low-contrast brown/cream. Disabled states across checkboxes, toggle_switches, and buttons use low-opacity overlays that fall well below accessibility thresholds. The handwritten/monospaced font exacerbates readability at small sizes.
**Flagged by:** Apple (#1, #2), Google (#4), Sun (#1 -- font readability), Microsoft (#1 -- implicit in truncation issues)
**Affected screens:** accessibility, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, button_variants, buttons, checkboxes, cozy_cafe, deadspace_settings, file_tree, flight_options, forms, kirby_options, modals, neon_strike, parcel_corps_settings, race_results, setting_row_showcase, sports_settings, stepper_showcase, tab_container, text, text_input, text_overflow, themes, toasts, toggle_switches
**Fix:** Audit all text-on-background color combinations and enforce a minimum contrast ratio of 4.5:1 for body text and 3:1 for large text and UI components. Implement a contrast-checking utility in the theme system. For disabled states, ensure underlying text remains at least 3:1 contrast. Replace the default handwritten/monospaced font with a proportional sans-serif for all standard UI controls and body text to maximize legibility.

---

## Agreed by 3/4 auditors

### 5. Missing or Non-Standard Window Title Bars and Close Buttons
**Severity:** Critical
**Problem:** Almost no screen implements a standard window title bar with title text and close/minimize/maximize buttons. The vast majority of screens have no window chrome whatsoever. Settings dialogs use inconsistent ad-hoc close mechanisms: circular red "X" buttons (angry_birds_settings, casual_settings), text links, or no close mechanism at all (mini_motorways_settings, flight_options). Modal dialogs appear as dark panels without clear title bars, close buttons, or drag handles. Only aim_chat implements anything resembling a proper title bar. Screens lack explicit dismiss mechanisms, leaving users with no visible way to exit or navigate back.
**Flagged by:** Apple (#11), Sun (#10), Microsoft (#2)
**Affected screens:** Nearly all screens. Specifically flagged: absolute_positioning, angry_birds_settings, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, bevel_borders, button_variants, buttons, casual_settings, checkboxes, circular_progress, colors, deadspace_settings, drag_drop, fighter_menu, flex_alignment, flight_options, forms, hstack_showcase, islands_trains_settings, meters_gauges, mini_motorways_settings, modals, navigation_bar_demo, neon_strike, pagination, powerwash_settings, radio_buttons, self_align, separators, setting_row_showcase, simple_button, sports_settings, tab_container, tabbing, text, text_input, text_overflow, text_shadow, text_stroke, themes, toasts, toggle_switches, vstack_showcase
**Fix:** Add a standard window frame to all primary and secondary windows: a distinct title bar with title text, a close button in a predictable position (top-right), and optionally minimize/maximize controls. Modal dialogs must have a visible title and explicit dismiss action (Close or Cancel button). Use a visible backdrop dimming effect to indicate modality. Ensure every screen has at least one visible navigation/exit control.

---

### 6. Non-Standard Checkbox and Radio Button Indicators
**Severity:** Major
**Problem:** Checkboxes display a "V" text character instead of a standard checkmark icon/glyph. In some contexts, "X" text is used to indicate enabled/checked state (forms shows "Music X", "SFX X"), which is semantically confusing since "X" typically means close/delete. Radio buttons appear as vertically-oriented pill or capsule shapes rather than standard circles. The "box only" checkbox variants render as tiny colored squares with no standard border. Selected radio state uses a filled crimson dot in an elongated/oval shape rather than a standard circle-with-dot. These non-standard shapes reduce control recognizability and create confusion about control type and state.
**Flagged by:** Google (#8), Sun (#11), Microsoft (#13)
**Affected screens:** checkboxes, forms, radio_buttons, toggle_switches
**Fix:** Use standard checkmark glyphs (Unicode check or rendered vector path) inside a standard square checkbox frame. Radio buttons must be perfect circles with a centered filled dot for the selected state. Remove "V" text and "X" text indicators. Ensure unchecked checkboxes show a visible square border outline and unchecked radio buttons show a visible circle outline.

---

### 7. Non-Standard Button Styling (Missing 3D Borders, Inconsistent Shapes)
**Severity:** Major
**Problem:** Buttons throughout the toolkit use rounded corners, pill shapes, flat fills, and gradient styles that do not follow any consistent component pattern. The toolkit mixes fully rounded pill shapes, squared-off rectangles, and moderately rounded rectangles seemingly at random, even within the same screen. Button prominence is conveyed through arbitrary color choices rather than a structured hierarchy. The bevel_borders screen demonstrates the toolkit can render raised/sunken 3D borders, but none of the actual interactive controls use them. Button sizing varies dramatically -- from tiny 20x20px squares to full-width bars within the same screen. No consistent button type taxonomy maps across the toolkit.
**Flagged by:** Google (#7), Sun (#7), Microsoft (#3)
**Affected screens:** angry_birds_settings, button_variants, buttons, casual_settings, cozy_cafe, empire_tycoon, kart_select, language_demo, mini_motorways_settings, navigation_bar_demo, potion_crafting, race_results, shop_interface, simple_button, tabbing, toasts
**Fix:** Define a structured button hierarchy with distinct visual roles (primary/filled, secondary/tonal, tertiary/outlined, text-only). Ensure each button type has a consistent shape, size class, and styling across all themes. Standardize corner radius for buttons. Apply appropriate affordance cues (3D borders or sufficient container contrast) so buttons are clearly recognizable as interactive. Document standard button size classes (small, medium, large) with minimum hit target areas.

---

### 8. Inconsistent Toggle Switch Styling Across Themes
**Severity:** Major
**Problem:** Toggle switches appear in at least 5-6 distinct visual styles across different themes and screens. Some resemble iOS toggles (green pill with white circle in angry_birds_settings, casual_settings), some use filled crimson pills with small thumbs (toggle_switches), some use outlined circles (setting_row_showcase), some use teal/gray pills with "X" text (forms). There is no consistency in thumb size, track proportions, or color mapping. In several settings screens, mobile-style toggle switches are used where standard checkboxes would be more appropriate for binary on/off options. The toggle switch is not a standard control in older platform guidelines, yet it is used pervasively.
**Flagged by:** Google (#11), Sun (#6 -- color-only state in toggles), Microsoft (#5)
**Affected screens:** angry_birds_settings, casual_settings, forms, islands_trains_settings, layout_bug_repros, mini_motorways_settings, parcel_corps_settings, setting_row_showcase, themes, toggle_switches
**Fix:** Standardize on a single toggle switch design with consistent track dimensions, thumb size, and color mapping across all themes. On state should use a distinct track color with the thumb positioned right; off state should use a contrasting track color with thumb positioned left. Add secondary state indicators (checkmark icon in on position, or I/O text) beyond color alone. Consider providing both toggle and checkbox variants for different use cases.

---

### 9. Non-Standard Tab Control Appearance and Behavior
**Severity:** Major
**Problem:** Tab controls appear in at least 8 different visual styles across the toolkit. Some use colored backgrounds, some use thin underlines, some use font-size changes to indicate active state, some use highlight boxes, and some use completely custom bracket notation ("> Ocean Navy <" in themes). Tab labels are frequently truncated because containers are undersized (fighter_menu, mini_motorways_settings). Active tab indicators range from blue underlines (tab_container) to teal pills (deadspace_settings) to bold font (powerwash_settings) to box borders (flight_options). None provide consistent animation, ripple feedback, or support for icons alongside labels. Tab label sizing varies even within the same tab bar.
**Flagged by:** Google (#10), Sun (#14), Microsoft (#12)
**Affected screens:** angry_birds_settings, deadspace_settings, fighter_menu, flight_options, mini_motorways_settings, potion_crafting, powerwash_settings, shop_interface, sports_settings, tab_container, themes
**Fix:** Standardize tab appearance using a single TabPane component with a consistent active indicator (visible underline or connected-tab style). All tab labels should use the same font size and weight. The active tab must be visually distinct through more than just color change. Ensure scrollable tab behavior when tabs exceed container width, preventing label truncation. Minimum tab width should accommodate the longest expected label.

---

### 10. Missing Standard Menu Bars and Keyboard Shortcuts
**Severity:** Major
**Problem:** Out of 72 screens, only aim_chat implements a traditional menu bar (File, Edit, Insert, People). No screens show keyboard accelerators (Ctrl+S, Ctrl+Z) or mnemonic underlines in menus. The hstack_showcase shows a "File Edit View Settings" toolbar pattern but it is styled as a demonstration rather than a functional menu. The flight_options screen shows Enter/Esc key bindings at the bottom, but this is the sole exception. Application-level screens and settings dialogs provide no standard menu access to common operations.
**Flagged by:** Sun (#4), Microsoft (#4), Apple (#9 -- no visible back/cancel/undo)
**Affected screens:** All game screens (cozy_cafe, empire_tycoon, fighter_menu, kart_select, neon_strike, potion_crafting, race_results, rubber_bandits_menu, shop_interface) and all settings screens (angry_birds_settings, casual_settings, deadspace_settings, flight_options, islands_trains_settings, kirby_options, mini_motorways_settings, parcel_corps_settings, powerwash_settings, sports_settings)
**Fix:** Add a standard menu bar to all application-level screens with at minimum File and Help menus. Expose common operations via standard keyboard shortcuts (Ctrl+Z for undo, Escape to close/cancel). Add visible back/cancel navigation controls to every screen. For settings screens, always include Cancel and Apply actions with keyboard bindings.

---

## Agreed by 2/4 auditors

### 11. Wildly Inconsistent Visual Themes Across Screens (No Structural Consistency)
**Severity:** Critical
**Problem:** The toolkit showcases radically different visual themes with no unifying design language or structural consistency. Screens range from light beige to dark navy to bright green to bright yellow to teal-on-black to pastel pink. The same control type (e.g., toggle switch) appears in completely different visual styles across screens. While per-game theming is valid for a game UI toolkit, themes change not just colors but fundamental control shapes, spacing, and typography, preventing users from developing consistent expectations. There is no shared chrome, window decoration, or structural framework that persists across themes.
**Flagged by:** Apple (#3), Sun (#9)
**Affected screens:** All 72 screens
**Fix:** Establish a consistent structural framework that persists across all themes: standard window chrome, consistent control shapes (button corner radius, toggle proportions, checkbox size), consistent spacing grid, and consistent typography scale. Themes should vary color palette and surface textures only, not fundamental geometry or layout rules. Provide a theme-switcher control in the showcase rather than hard-coding different themes per demo page.

---

### 12. Non-Standard Slider/Trackbar Appearance
**Severity:** Major
**Problem:** Sliders do not follow standard patterns from any guideline set. In forms, sliders show dark pill tracks with purple block thumbs and no visible value tooltip. In setting_row_showcase, sliders have thin gray tracks with small green square thumbs. In cozy_cafe, the Music slider has a rounded green track with a circular teal thumb. Some settings screens (sports_settings, islands_trains_settings) use segmented block indicators instead of continuous sliders. Thumb shapes vary between square, circular, and rectangular. No implementation shows tick marks for discrete values or value label tooltips.
**Flagged by:** Google (#13), Microsoft (#11)
**Affected screens:** cozy_cafe, forms, meters_gauges, parcel_corps_settings, setting_row_showcase, sports_settings, islands_trains_settings
**Fix:** Standardize slider anatomy: a visible track (with active and inactive portions in distinct colors), a consistent circular or raised thumb with adequate size (minimum 20dp/20px), optional tick marks for discrete values, and a value label indicator on active drag. Ensure minimum 44px touch/click target around the thumb.

---

### 13. Dialog Button Ordering, Styling, and Missing OK/Cancel Pairs
**Severity:** Major
**Problem:** Dialog action buttons (OK, Cancel, Apply) show no consistent styling convention. The "OK" button variously appears as green, blue, or teal. Cancel and Apply alternate between outlined and filled styles. Button ordering varies between screens. Many screens that function as dialogs or secondary windows lack OK/Cancel entirely -- potion_crafting has "Brew!" but no cancel, shop_interface has "Purchase" but no way to dismiss. Multiple demo and animation screens display content with no visible exit, back, or cancel control. Some settings screens include OK/Cancel/Apply while others omit them entirely.
**Flagged by:** Sun (#5), Microsoft (#7)
**Affected screens:** angry_birds_settings, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, casual_settings, cozy_cafe, deadspace_settings, drag_drop, empire_tycoon, fighter_menu, flight_options, horizontal_drag, kart_select, mini_motorways_settings, neon_strike, potion_crafting, powerwash_settings, race_results, rubber_bandits_menu, shop_interface, sports_settings, tabbing, text_input
**Fix:** Standardize dialog button bars: consistent ordering (OK, Cancel, Apply or Cancel, Apply, OK per platform convention). The primary action button should be visually distinguished but all buttons should share the same basic style family. Every dialog-style window must have at minimum OK and Cancel buttons. Ensure Escape maps to Cancel. Use a dedicated DialogButtonBar component to enforce consistency.

---

### 14. Inconsistent Spacing, Alignment, and Cramped Layouts
**Severity:** Minor
**Problem:** Many screens exhibit insufficient or inconsistent spacing between interactive elements. In toasts, buttons touch edge-to-edge with zero gap. In fighter_menu, sub-tab labels are packed with no padding, causing overlap. In accessibility, elements have inconsistent widths and misaligned rows. In button_variants, buttons in a row have different heights and vertical alignment. In cards, panels with identical structure have different numbers of internal rows creating visual imbalance. The cozy_cafe "Patience" text overlaps its progress indicator. Sports_settings rows have minimal vertical padding. Some screens use equal spacing while adjacent screens use variable spacing with no consistent grid.
**Flagged by:** Apple (#13), Sun (#12)
**Affected screens:** accessibility, button_variants, cards, checkboxes, cozy_cafe, fighter_menu, flex_alignment, hstack_showcase, layout, layout_bug_repros, navigation_bar_demo, parcel_corps_settings, setting_row_showcase, sports_settings, stepper_showcase, themes, toasts
**Fix:** Define a spacing scale (e.g., 4px, 8px, 16px, 24px, 32px) and apply it consistently throughout the toolkit. Enforce minimum 6-8px between related controls and 12-16px between unrelated groups. Align elements to a grid. Ensure equal-importance elements within a row have matching sizes. Add spacing tokens to the theme system.

---

### 15. Monospace/Handwritten Font Used Globally Instead of Standard Typography
**Severity:** Critical
**Problem:** The entire toolkit uses a monospace, handwritten-style font ("Gaegu-Bold" as confirmed in language_demo) as the default typeface across all UI elements -- labels, body text, buttons, headers, and form controls. This font severely undermines readability at small sizes (captions, metadata text, file tree entries, chat messages) and makes the UI feel informal rather than structured. No type scale with distinct roles (Display, Headline, Title, Body, Label) is implemented. The single font family is used for everything from large headings to tiny caption text.
**Flagged by:** Google (#1), Sun (#1)
**Affected screens:** All 72 screens
**Fix:** Replace the default typeface with a proportional sans-serif font for all standard UI controls, labels, and body text. Reserve the handwritten/monospaced font for decorative titles or thematic game-specific headers only. Implement a proper type scale with distinct roles: headings at larger/bolder weights, body text at readable sizes (minimum 12pt equivalent), and labels at appropriate caption sizes. Define typography tokens in the theme system.

---

### 16. Inconsistent Corner Radii / Shape Scale
**Severity:** Major
**Problem:** The toolkit has no consistent shape scale. Fully rounded pill shapes, squared-off rectangles, moderately rounded rectangles, and large rounded rectangles are mixed seemingly at random, even within the same screen. In button_variants, pill-rounded filled buttons sit alongside sharp-cornered outline labels. In cards, sharp-cornered panels appear next to rounded ones. Nearly every container and panel uses large rounded corners (8-16px radius), while some screens use sharp corners. The example_borders screen demonstrates "Pill", "Rounded", "Top Only", "Diagonal" all on one screen, highlighting the inconsistency.
**Flagged by:** Google (#5), Microsoft (#14)
**Affected screens:** absolute_positioning, animation_basic, animation_interactive, animation_looping, animation_spring, auto_text_color, bevel_borders, button_variants, buttons, cards, circular_progress, decorative_frame, example_borders, flex_alignment, forms, language_demo, layout, meters_gauges, navigation_bar_demo, nine_slice_borders, pagination, self_align, setting_row_showcase, simple_button, stepper_showcase, toggle_switches
**Fix:** Define a shape scale with 5-6 radius tokens and apply them systematically: small interactive elements get small radius, medium containers get medium radius, large containers get large radius. Document which shape token applies to which component category. Enforce consistency within each component type -- all buttons should share the same corner radius, all cards should share the same corner radius.

---

### 17. Flat Surface Hierarchy / Missing Tonal Elevation
**Severity:** Major
**Problem:** Nested containers, overlays, and different hierarchy levels often use the same or nearly same surface color, making it difficult to distinguish parent-child relationships or foreground from background. In modals, sections like "BASIC MODALS", "HELPER FUNCTIONS", "MODAL STACKING" all appear as identically-colored dark slabs. In drag_drop, columns sit at the same tonal level as the background. Settings panels across deadspace_settings, flight_options, and sports_settings float on colored backgrounds without visual separation (no scrim, no tonal differentiation, no shadow).
**Flagged by:** Google (#6), Google (#14 -- missing dialog scrim)
**Affected screens:** angry_birds_settings, casual_settings, checkboxes, drag_drop, forms, hstack_showcase, language_demo, modals, navigation_bar_demo, setting_row_showcase, text_input, themes, toasts, toggle_switches, vstack_showcase
**Fix:** Implement tonal surface elevation: define a 3-5 level tonal scale where nested or overlapping containers use progressively lighter/different surface colors. Apply scrims (dimmed background overlays) behind modal dialogs. Use subtle shadows or tonal shifts to create visual separation between content layers.

---

### 18. Disabled State Visual Inconsistency
**Severity:** Major
**Problem:** Disabled states across the toolkit use inconsistent visual treatments. Buttons use fully gray fills. Checkboxes use reduced opacity. Toggle switches use slightly muted colors. Some controls (sliders, text inputs, steppers) do not demonstrate a disabled state at all. The flight_options screen shows grayed text with explanatory text below, while toggle_switches shows muted colors with different opacity. No unified disabled rendering strategy exists. Disabled text contrast frequently falls below readability thresholds.
**Flagged by:** Apple (#4 -- controls lack state differentiation), Sun (#8)
**Affected screens:** button_variants, buttons, checkboxes, flight_options, forms, stepper_showcase, text_input, toggle_switches
**Fix:** Create a unified disabled rendering strategy: all disabled controls should use the same gray-out treatment (consistent opacity overlay or dedicated disabled color tokens). Disabled text should maintain at least 3:1 contrast for identification purposes. Ensure every interactive component type has a documented and visible disabled state.

---

## Flagged by 1 auditor only

### 19. Missing or Invisible Scrollbar Indicators
**Severity:** Major
**Problem:** Scrollable content areas lack visible scrollbar indicators, leaving users with no indication that additional content exists below the visible area. In scroll_view, a list and grid display with no visible scrollbar. In shop_interface, a tall list extends to the bottom with no scroll indicator. In setting_row_showcase, settings extend vertically with no scroll affordance. The potion_crafting table panel may overflow but shows no scrollbar.
**Flagged by:** Apple (#5)
**Affected screens:** aim_chat, cozy_cafe, deadspace_settings, empire_tycoon, flight_options, forms, potion_crafting, scroll_view, setting_row_showcase, shop_interface
**Fix:** Always show scrollbar tracks for scrollable regions. Use a visible track and proportionally-sized thumb to communicate content length. Consider adding subtle fade-out gradients at scroll boundaries to hint at more content.

---

### 20. Decorative Visual Noise Competing with Functional Content
**Severity:** Major
**Problem:** Dense grids of ornate borders, bevels, and decorator patterns create visually overwhelming displays that do not help users understand practical application. In nine_slice_borders, a dense grid of ornate white-on-dark border frames in various sizes creates visual noise. In bevel_borders, rows of nearly identical beveled squares show different shadow intensities without clear purpose. In decorators, nine dense panels demonstrate overlapping effects.
**Flagged by:** Apple (#10)
**Affected screens:** bevel_borders, decorative_frame, decorators, example_borders, nine_slice_borders
**Fix:** Reduce the number of variants shown simultaneously. Group related styles with clear section headers and descriptions. Use progressive disclosure (tabs or accordion) to show one style at a time with its configuration options.

---

### 21. Inconsistent Button Sizing and Inadequate Hit Targets
**Severity:** Major
**Problem:** Button and interactive element sizes vary dramatically, with some being dangerously small hit targets. In button_variants, elements range from a tiny ~20x20px square to full-width bars. In pagination, page indicators are small rounded rectangles of varying sizes. In kart_select, color selection items are tiny squares barely larger than icons. In stepper_showcase, increment/decrement controls appear as small dark rectangles. Bottom-bar controls in empire_tycoon mix tiny square icons with elongated buttons.
**Flagged by:** Apple (#8)
**Affected screens:** button_variants, buttons, cards, cozy_cafe, empire_tycoon, kart_select, navigation_bar_demo, pagination, simple_button, stepper_showcase, tab_container
**Fix:** Establish a minimum interactive element size of at least 44x44 logical pixels. Normalize button sizing within the same context -- buttons in a toolbar should be the same height. Document standard button size classes (small, medium, large) with enforced minimums.

---

### 22. Settings Screens Lack Grouped Section Organization
**Severity:** Minor
**Problem:** Settings are presented as flat vertical lists with no visible section grouping, category headers, or dividers. In deadspace_settings, all settings are a flat list of teal bars with no grouping. In parcel_corps_settings, settings appear as a long list with alternating bars but no section titles. Even polished screens like casual_settings present all toggles without clear category organization. The only visible text in most settings screens is unreadable due to contrast issues, compounding the problem.
**Flagged by:** Apple (#14)
**Affected screens:** angry_birds_settings, casual_settings, deadspace_settings, flight_options, islands_trains_settings, kirby_options, mini_motorways_settings, parcel_corps_settings, powerwash_settings, sports_settings
**Fix:** Group related settings under clear section headers (e.g., "Audio", "Video", "Gameplay", "Controls"). Add visual separators between groups. Section headers should use a larger, bolder font than individual setting labels. Consider an accordion or tab pattern for settings with many categories.

---

### 23. Drag-and-Drop Interfaces Lack Visual Affordances
**Severity:** Minor
**Problem:** Drag-and-drop screens show no visible drop zones, insertion lines, placeholder indicators, drag handles, or ghost images. In drag_drop, a grid of colored squares provides no feedback about valid drop locations. In horizontal_drag, blue bars with checkboxes show no reorder affordances. Neither screen shows cursor changes, highlight effects on drop targets, or any active drag state feedback.
**Flagged by:** Apple (#15)
**Affected screens:** drag_drop, horizontal_drag
**Fix:** Add visible drag handles (grip icon or dotted area) on draggable items. Show highlighted drop zones when dragging is active. Display an insertion line or placeholder at potential drop locations. Use a semi-transparent ghost image of the dragged item. Provide visual feedback (color change or border highlight) on valid drop targets during hover.

---

### 24. No Visible Interaction State Layers (Hover, Pressed, Dragged)
**Severity:** Major
**Problem:** The toolkit appears to implement only Enabled, Disabled, and Focus states. No screens demonstrate hover overlays, pressed state dimming, or ripple effects. The button_variants and buttons screens show only enabled, disabled, and focused states with no hover or pressed demonstration. The drag_drop screen shows draggable items but no visual "dragged" state differentiation. Interactive elements provide no visual confirmation that they are being interacted with beyond focus rings.
**Flagged by:** Google (#12)
**Affected screens:** All interactive screens -- specifically button_variants, buttons, drag_drop, horizontal_drag, simple_button, tabbing
**Fix:** Implement interaction state layers as semi-transparent overlays on interactive containers: distinct hover state (subtle background shift), visible pressed state (darkened or ripple effect), and dragged state (elevation change or ghost). These should combine with the container color and be visible across all themes.

---

### 25. ALL-CAPS Labels Where Sentence Case Is More Readable
**Severity:** Major
**Problem:** The toolkit uses ALL CAPS extensively for section headers, tab labels, menu items, and game UI text. This reduces readability (word shapes are lost in all-caps) and creates an aggressive visual tone. Examples include "BASIC MODALS", "FLIGHT SYSTEM", "RUBBER BANDITS", "KILLSTREAK", "RACE RESULTS", "BEVEL BORDERS", "KEYBOARD NAV", and many more. While game UIs may intentionally use all-caps for stylistic impact, a toolkit should default to sentence case for maximum readability.
**Flagged by:** Google (#9)
**Affected screens:** bevel_borders, deadspace_settings, example_borders, flight_options, kart_select, kirby_options, modals, neon_strike, nine_slice_borders, powerwash_settings, race_results, rubber_bandits_menu, sports_settings, tabbing, toasts
**Fix:** Convert all label text to sentence case by default (e.g., "Basic modals" not "BASIC MODALS"). Provide an optional text-transform property for screens that want all-caps, but make sentence case the toolkit default.

---

### 26. Icon Rendering Artifacts and Inconsistent Sizing
**Severity:** Minor
**Problem:** Icons are rendered as pixelated sprite sheets, text characters inside colored shapes, or vector icons with visible artifacts near boundaries. In empire_tycoon, navigation icons show pixelated sprite rendering with blurred edges. In kirby_options, top-row icons are colored squares with text labels rather than actual icons. In images, small pixel artifacts (tiny characters like "i", commas, periods) appear near icon edges. Icon sizes vary between 24px, 40px, and 64px with no consistent scale.
**Flagged by:** Google (#15)
**Affected screens:** cozy_cafe, empire_tycoon, images, kirby_options, neon_strike, parcel_corps_settings
**Fix:** Use vector-based icons at consistent sizing for toolkit demos. Implement an icon size scale (Small=20dp, Medium=24dp, Large=40dp) and apply consistently. Ensure sprite icons are rendered at native resolution or properly scaled with filtering. Fix rendering artifacts by ensuring proper texture sampling.

---

### 27. Missing Progress/Loading Feedback for Async Operations
**Severity:** Minor
**Problem:** Screens involving asynchronous operations or staged workflows show no loading indicators, spinners, or progress feedback. Empty panels in the modals screen provide no indication whether content is loading, failed to load, or is simply empty. No empty-state messages explain blank content areas. The file_tree shows "No selection" with no loading indicator for tree population.
**Flagged by:** Sun (#13)
**Affected screens:** cozy_cafe, drag_drop, file_tree, forms, modals
**Fix:** Add indeterminate progress bars or spinners to panels that load content asynchronously. Provide empty-state messages ("No items to display" or "Click a button to open a modal") for empty content areas. Include both visual and text representations of progress.

---

### 28. Error States and Form Validation Feedback Absent
**Severity:** Minor
**Problem:** The toolkit provides almost no demonstration of form validation, error states, or constraint feedback. Text input fields show no required-field indicators, no inline validation messages, and no red-border error states. Sliders and toggles have no indication of invalid or out-of-range values. The only hint of error handling is shop_interface showing "3 Low" and "2 Low" in red text for low stock, but this uses no structured error component.
**Flagged by:** Sun (#15)
**Affected screens:** cozy_cafe, flight_options, forms, shop_interface, text_input, text_overflow
**Fix:** Add a standard error state to all form controls: distinct border + error icon + descriptive error message below the field. Implement required-field indicators. Add inline validation feedback to text inputs. Use the toast notification system for non-field-level errors.

---

### 29. Stepper/Spinner Controls Used Instead of Standard Dropdowns
**Severity:** Major
**Problem:** Left/right arrow stepper controls (< Value >) are used extensively for selecting from predefined option sets (Resolution, Quality, Language, Difficulty, Framerate). This pattern requires repeated clicking to cycle through options and provides no visibility into available choices. Users cannot see how many options exist or jump directly to a desired value.
**Flagged by:** Microsoft (#8)
**Affected screens:** flight_options, islands_trains_settings, mini_motorways_settings, navigation_bar_demo, parcel_corps_settings, powerwash_settings, setting_row_showcase, sports_settings, stepper_showcase
**Fix:** Replace stepper/spinner controls with dropdown list boxes (combo boxes) for selecting from predefined option lists. This gives users immediate visibility into all available choices and requires only a single click to select. Retain steppers only for numeric increment/decrement with small ranges.

---

### 30. No Standard Status Bar at Bottom of Primary Windows
**Severity:** Minor
**Problem:** Most screens lack any status bar displaying contextual information. The few screens with bottom-bar information (aim_chat "Direct Connection Established", race_results controller hints, language_demo "Active: ENGLISH | Font: Gaegu-Bold") use non-standard styling without proper sunken border panes or system color conventions.
**Flagged by:** Microsoft (#15)
**Affected screens:** Nearly all primary window screens
**Fix:** Add a standard status bar at the bottom of primary windows with contextual information such as current state, selection details, or key binding hints. Use consistent styling with distinct bordered panes for different information categories.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 4 | **Do** -- universal consensus |
| 3/4 | 6 | **Probably do** -- strong consensus |
| 2/4 | 8 | **Maybe** -- review individually |
| 1/4 | 12 | **Skip unless compelling** |
