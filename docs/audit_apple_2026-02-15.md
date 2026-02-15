# Apple HIG Audit: All Screens (2026-02-15)

Audited 67 screenshots from `/output/screenshots/` against the 1987 Apple Human Interface Guidelines.

---

## 1. Pervasive Absence of Text Labels on Controls and UI Elements
**Severity:** Critical
**Affected screens:** accessibility, aim_chat, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, bevel_borders, button_variants, buttons, cards, casual_settings, checkboxes, circular_progress, colors, deadspace_settings, decorators, drag_drop, empire_tycoon, example_borders, fighter_menu, file_tree, flex_alignment, flight_options, forms, horizontal_drag, images, kart_select, kirby_options, language_demo, layout, meters_gauges, mini_motorways_settings, modals, navigation_bar_demo, neon_strike, pagination, parcel_corps_settings, potion_crafting, powerwash_settings, race_results, radio_buttons, rubber_bandits_menu, scroll_view, self_align, separators, setting_row_showcase, shop_interface, simple_button, sports_settings, stepper_showcase, tab_container, tabbing, text, text_input, text_overflow, text_shadow, text_stroke, themes, toasts, toggle_switches
**Problem:** Across virtually every screenshot, buttons, tabs, navigation items, setting rows, radio buttons, checkboxes, toggle switches, sliders, and other interactive controls appear as colored rectangles or shapes with no visible text labels whatsoever. For example, in `buttons`, rows of colored rectangles (blue, dark yellow, green, gray) have no label text. In `navigation_bar_demo`, icon-based nav items show small white icons (gear, star, monitor, house) but the accompanying text labels are either missing or rendered in a color indistinguishable from the background. In `toggle_switches`, the toggle rows show only colored dot-circles with no label describing what each toggle controls. In `setting_row_showcase`, setting rows appear as dark bars with green toggle switches but no readable text explaining what each setting controls.
**Guideline:** See-and-Point (all actions must be available via visible, labeled controls) and WYSIWYG
**Fix:** Ensure every interactive control has a clearly visible text label rendered in a color with sufficient contrast against its background. Labels should use a legible font size (minimum 12pt equivalent at 720p). If text is present but rendered in a near-background color, fix the text color contrast to meet a minimum 4.5:1 ratio.

---

## 2. Text Rendered Invisible Due to Insufficient Color Contrast
**Severity:** Critical
**Affected screens:** accessibility, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, button_variants, buttons, deadspace_settings, file_tree, flight_options, forms, modals, neon_strike, parcel_corps_settings, race_results, setting_row_showcase, sports_settings, stepper_showcase, tab_container, text, text_input, text_overflow, themes, toasts, toggle_switches
**Problem:** Many screens render text in colors nearly identical to their background, making the text effectively invisible. In `animation_basic`, small dark-blue labels above colored squares are barely distinguishable from the dark navy background. In `text`, the right half of the screen shows dark bars on a near-black background with no visible text content. In `file_tree`, the entire screen is solid dark charcoal with no visible text -- a file tree widget with no readable node labels. In `deadspace_settings`, teal-on-dark-navy text and controls are extremely hard to read. In `forms`, the form with dark blue input fields on dark navy shows no visible placeholder or field label text.
**Guideline:** WYSIWYG (what you see is what you get -- users must actually be able to see the content) and Aesthetic Integrity
**Fix:** Audit all text-on-background color combinations and enforce a minimum contrast ratio of 4.5:1 for body text and 3:1 for large text. Implement a contrast-checking utility in the theme system to prevent low-contrast text configurations.

---

## 3. Wildly Inconsistent Visual Themes Across Screens
**Severity:** Critical
**Affected screens:** All 67 screens
**Problem:** The toolkit showcase presents radically different visual themes with no unifying design language. Screens range from light beige (cozy_cafe, decorative_frame, separators, mini_motorways_settings) to dark navy (animation_basic, buttons, meters_gauges, text) to bright green (angry_birds_settings, casual_settings) to bright yellow (rubber_bandits_menu) to teal-on-black (deadspace_settings, neon_strike) to pastel pink (simple_button). While themed flexibility is expected in a game UI toolkit, the showcase itself lacks a consistent demonstration framework. The same control type (e.g., toggle switch) appears in completely different visual styles across angry_birds_settings (green pill on cream), toggle_switches (dark crimson dots on dark gray), casual_settings (green pills on cream), and setting_row_showcase (green pills on dark gray).
**Guideline:** Consistency (same actions and elements should appear consistent across contexts)
**Fix:** While per-game theming is valid, the toolkit showcase should use a single base theme for widget demonstrations so users can compare controls side-by-side. Provide a theme-switcher control rather than hard-coding different themes per demo page. Document the mapping from base components to themed variants.

---

## 4. Interactive Controls Lack Visible State Differentiation
**Severity:** Major
**Affected screens:** checkboxes, radio_buttons, toggle_switches, buttons, button_variants, navigation_bar_demo, tab_container, tabbing, pagination
**Problem:** In `checkboxes`, the checkbox screen shows colored squares with small checkbox controls visible in the dark teal-on-dark theme, but there is no clear visual distinction between checked and unchecked states beyond a subtle color fill. In `radio_buttons`, the three groups each show small oval radio dots where the selected state (crimson-filled) versus unselected (dark outline) have poor contrast against the dark background. In `toggle_switches`, the on/off states are differentiated only by a subtle crimson vs. gray fill that is hard to distinguish. In `tabbing`, tab headers appear as barely-distinguishable gray bars with no active-tab indicator except a subtle blue underline. In `pagination`, page indicator dots are small and similarly colored, making current-page hard to identify.
**Guideline:** Feedback (provide immediate visual feedback for every action) and See-and-Point
**Fix:** Implement clearly distinct visual states for all interactive controls: use high-contrast fill colors, checkmarks for checkboxes, bold ring fills for radio buttons, and prominent color/position changes for toggles. The active tab should have a visibly different background color or a thick underline indicator. Use at least two visual cues (color + shape change) for state changes to support colorblind users.

---

## 5. Missing or Invisible Scrollbar Indicators in Scrollable Content Areas
**Severity:** Major
**Affected screens:** scroll_view, shop_interface, cozy_cafe, deadspace_settings, empire_tycoon, flight_options, forms, setting_row_showcase, potion_crafting
**Problem:** In `scroll_view`, a list and grid of items are displayed with no visible scrollbar, leaving users with no indication that additional content exists below the visible area. In `aim_chat`, there is a tall white content area with a barely visible gray scrollbar track on the right edge. In `shop_interface`, a tall list of colored-dot items extends to the bottom of the panel with no scrollbar visible. In `setting_row_showcase`, the settings list extends vertically with no scroll indicator. The potion_crafting screen shows a table-like right panel that may overflow but has no scrollbar.
**Guideline:** See-and-Point (users must see available actions and content extent) and Perceived Stability
**Fix:** Always show scrollbar tracks for scrollable regions, even if the scrollbar thumb is only shown on hover. Use a visible track and proportionally-sized thumb to communicate content length. Consider adding subtle fade-out gradients at scroll boundaries to hint at more content.

---

## 6. Ambiguous Focus/Selection Indicator Appearance
**Severity:** Major
**Affected screens:** accessibility, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, buttons, button_variants, checkboxes, circular_progress, kart_select, navigation_bar_demo, pagination, radio_buttons, tab_container, toggle_switches, text_input, stepper_showcase
**Problem:** Across many screens, the focus indicator (keyboard selection highlight) appears as a thin, sometimes barely visible white or light-colored outline rectangle around one element. In `animation_basic`, a thin white outline appears around the top-left blue square. In `buttons`, the first blue button in the top row has a subtle white outline. In `kart_select`, a similarly thin white outline marks the first kart color option. In `toggle_switches`, the top toggle has a thin white outline. These focus rings are often only 1-2 pixels wide and use a color with poor contrast against both light and dark backgrounds. This makes it very difficult for keyboard/gamepad users to identify which element is currently focused.
**Guideline:** Feedback (every action needs visual feedback) and See-and-Point
**Fix:** Increase focus ring thickness to at least 3px. Use a high-contrast focus ring color (bright white on dark themes, dark blue on light themes) with an optional inner shadow or glow effect. Consider a dual-color focus ring (e.g., white border with dark outline) that works on any background color. Ensure the focus indicator is visually distinct from selection/active states.

---

## 7. UI Elements Clipped or Overflowing Container Boundaries
**Severity:** Major
**Affected screens:** aim_chat, empire_tycoon, flex_alignment, horizontal_drag, flight_options, themes, images, text_overflow
**Problem:** In `aim_chat`, the entire chat window is positioned off-center with elements (title bar, buttons) clipped at the top edge of the screen. In `empire_tycoon`, sidebar category icons on the left edge are partially clipped by the screen boundary. In `flex_alignment`, the top-left element appears partially cut off at the screen edge. In `horizontal_drag`, the top-left corner shows a partially visible blue bar and text clipped at screen edge. In `themes`, the top of the screen shows a dark blue/green bar that extends off-screen at the top. In `text_overflow`, small boxes in the lower right have content visibly overflowing their borders in red. In `images`, the checkerboard pattern at the top of the screen indicates a transparency/overflow area where content extends beyond its container.
**Guideline:** Perceived Stability (the interface should feel stable and predictable) and WYSIWYG
**Fix:** Ensure all UI containers properly clip or wrap their content. Add overflow handling (scroll, ellipsis, or container resizing) for content that exceeds container bounds. Review layout calculations to prevent off-screen positioning. Never allow interactive elements to be partially clipped by parent containers.

---

## 8. Inconsistent Button Sizing and Hit Target Areas
**Severity:** Major
**Affected screens:** buttons, button_variants, navigation_bar_demo, tab_container, kart_select, pagination, simple_button, cozy_cafe, empire_tycoon, cards, stepper_showcase
**Problem:** Button and interactive element sizes vary dramatically, with some being dangerously small hit targets. In `button_variants`, the row of buttons ranges from a tiny square (about 20x20px at 720p) to full-width bars, with no consistent sizing. In `navigation_bar_demo`, the bottom row of icon-labeled nav buttons has very different widths per item. In `pagination`, page number indicators are small rounded rectangles of varying size. In `kart_select`, the color selection items at the top are tiny squares barely larger than icons. In `stepper_showcase`, the increment/decrement controls appear as small dark rectangles that could be hard to target. In `empire_tycoon`, bottom-bar controls mix tiny square icons with elongated buttons.
**Guideline:** See-and-Point (targets must be easy to acquire) and Consistency
**Fix:** Establish a minimum interactive element size of at least 44x44 logical pixels (per modern Apple HIG guidance, relevant even for game UIs). Normalize button sizing within the same context -- buttons in a toolbar should be the same height, buttons in a group should be proportional. Document standard button size classes (small, medium, large) in the toolkit.

---

## 9. No Visible Undo/Cancel/Back Affordances on Many Screens
**Severity:** Major
**Affected screens:** animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, circular_progress, colors, decorative_frame, drag_drop, example_borders, file_tree, flex_alignment, horizontal_drag, layout, nine_slice_borders, self_align, separators, text, text_overflow, text_shadow, text_stroke
**Problem:** Many demo/showcase screens display widgets without any visible back button, cancel button, close button, or navigation control to exit the current view. In `file_tree`, the screen is entirely empty dark charcoal. In `colors`, only color swatches are shown with no navigation. In `decorative_frame`, three decorative frames are shown in isolation with no way to navigate away. In `nine_slice_borders`, a comprehensive grid of border styles is shown but no navigation or close control is present. Most of the animation demo screens show animated colored boxes with no visible exit control.
**Guideline:** Forgiveness (allow undo, make actions reversible) and User Control (user initiates and controls actions)
**Fix:** Add a consistent navigation header or back button to every demo screen. Provide a visible "Back" or "Close" control in a predictable location (e.g., top-left corner). For settings screens, always include "Apply" and "Cancel" buttons to support reversible changes.

---

## 10. Decorative Visual Noise Competing with Functional Content
**Severity:** Major
**Affected screens:** nine_slice_borders, bevel_borders, example_borders, decorators, decorative_frame
**Problem:** In `nine_slice_borders`, a dense grid of ornate white-on-dark border frames in various sizes creates a visually overwhelming display. The bottom row adds colored (gold, blue, red, green) nine-slice frames. In `bevel_borders`, two rows of six beveled squares each show different shadow intensities but look nearly identical, creating visual noise without clear purpose. In `decorators`, nine panels demonstrate different decorator/overlay effects stacked densely. In `example_borders`, three columns of bordered rectangles with grid overlays and corner markers create a technical display that does not help users understand practical application.
**Guideline:** Aesthetic Integrity (visual design should serve function, not compete with it) and Perceived Stability
**Fix:** Reduce the number of variants shown simultaneously. Group related border styles with clear section headers and descriptions. Use a progressive-disclosure pattern (e.g., tabs or accordion) to show one border style at a time with its configuration options, rather than dumping all variants on screen simultaneously.

---

## 11. Modal and Dialog Screens Lack Standard Window Chrome
**Severity:** Major
**Affected screens:** modals, simple_button, fighter_menu, potion_crafting, neon_strike
**Problem:** In `modals`, the modal dialog overlay appears as dark panels without a clear title bar, close button, or drag handle. In `simple_button`, the centered card dialog has no visible close/dismiss control. In `fighter_menu`, a complex multi-panel character select layout has no window controls or clear dismissal mechanism. In `neon_strike`, a shooter-style HUD includes overlay panels with no standard dialog chrome. The HIG requires that modal dialogs have clear window boundaries, a title, and an explicit dismiss mechanism.
**Guideline:** Metaphors (use familiar real-world concepts -- dialogs should look like dialogs) and User Control
**Fix:** Add standard dialog chrome to all modal windows: a distinct title bar with a title label, a close/X button in the top-right corner, and optionally a drag handle. Use a visible backdrop dimming effect to indicate modality. Ensure all dialogs have at least one explicit dismiss action (Cancel or Close button).

---

## 12. Color-Only Differentiation of Semantically Distinct Items
**Severity:** Major
**Affected screens:** colors, auto_text_color, kart_select, circular_progress, meters_gauges, potion_crafting, race_results, drag_drop
**Problem:** In `colors`, both the dark-on-dark palette row and the light pastel palette row rely entirely on color hue to differentiate items with no text labels, patterns, or icons. In `kart_select`, vehicle/color selection is done via colored circles with no text or shape differentiation. In `circular_progress`, nine circular progress indicators at different fill levels are differentiated only by color (cyan, purple, green, orange, pink). In `meters_gauges`, stacked bar meters use red/green/blue segments with no pattern or label differentiation. In `race_results`, race position results are shown as blue horizontal bars of different lengths with no position numbers or text visible.
**Guideline:** See-and-Point and Aesthetic Integrity (design must serve all users, including colorblind users)
**Fix:** Never rely on color alone to convey meaning. Add text labels, icons, or patterns (stripes, dots, cross-hatch) to differentiate items. For progress indicators, add percentage text labels. For selection grids, add text labels below each option. For data visualizations, use patterns in addition to colors.

---

## 13. Inconsistent Spacing and Alignment Within Individual Screens
**Severity:** Minor
**Affected screens:** accessibility, button_variants, cards, flex_alignment, layout, navigation_bar_demo, setting_row_showcase, stepper_showcase, themes
**Problem:** In `accessibility`, the top green bar is very tall while the small button below it is compact, and the two input fields below have inconsistent widths. Color swatches at the bottom have equal spacing but the two rows have a different number of items. In `button_variants`, buttons in a row have different heights and vertical alignment. In `cards`, three card panels have identical internal structure but the rightmost card has fewer internal rows, creating visual imbalance. In `flex_alignment`, the two-panel layout shows dense packed colored elements on the left but sparse uniform elements on the right, with inconsistent internal padding. In `themes`, the sidebar list of colored squares has inconsistent spacing with the main content area.
**Guideline:** Consistency and Perceived Stability
**Fix:** Define a spacing scale (e.g., 4px, 8px, 16px, 24px, 32px) and apply it consistently. Align elements to a grid. Ensure equal-importance elements within a row or column have matching sizes. Use layout constraints to keep spacing proportional when containers resize.

---

## 14. Settings Screens Lack Grouped Section Organization
**Severity:** Minor
**Affected screens:** angry_birds_settings, casual_settings, deadspace_settings, islands_trains_settings, kirby_options, mini_motorways_settings, parcel_corps_settings, powerwash_settings, sports_settings, flight_options
**Problem:** In `deadspace_settings`, all settings are presented as a flat list of teal bars and colored controls (teal, crimson, purple) with no visible section grouping or category headers. In `parcel_corps_settings`, settings appear as a long vertical list with alternating teal bars but no grouping dividers or section titles. In `sports_settings`, the left pane shows a flat list of setting rows with steppers and toggles but no section breaks. In `islands_trains_settings` and `kirby_options`, settings are similarly presented as flat vertical lists. Even `casual_settings`, which has a more polished appearance, presents all toggles and option rows without clear category grouping. The only visible text in most of these screens is unreadable due to contrast issues.
**Guideline:** Metaphors (settings should be organized like real control panels with labeled sections) and Perceived Stability
**Fix:** Group related settings under clear section headers (e.g., "Audio", "Video", "Gameplay", "Controls"). Add visual separators between groups. Consider using an accordion or tab pattern for settings with many categories. Each section header should be in a larger, bolder font than the individual setting labels.

---

## 15. Drag-and-Drop Interfaces Lack Visual Drop-Target Affordances
**Severity:** Minor
**Affected screens:** drag_drop, horizontal_drag
**Problem:** In `drag_drop`, a grid of colored squares (blue, green, purple, orange) is displayed in three rows with a small dark square that appears to be a dragged item, but there are no visible drop zones, insertion lines, or placeholder indicators showing where items can be dropped. In `horizontal_drag`, three horizontal blue bars appear with small checkboxes on the left side, plus some small dark elements at the bottom, but no visual feedback shows valid drop locations, drag handles, or reorder affordances. Neither screen shows cursor changes, highlight effects on drop targets, or ghost/phantom images of the item being dragged.
**Guideline:** Direct Manipulation (users should feel they are directly controlling objects) and Feedback
**Fix:** Add visible drag handles (e.g., a grip icon or dotted area) on draggable items. Show highlighted drop zones when dragging is active. Display an insertion line or placeholder space at the potential drop location. Use a semi-transparent "ghost" image of the dragged item attached to the cursor. Provide visual feedback (color change or border highlight) on valid drop targets when an item hovers over them.
