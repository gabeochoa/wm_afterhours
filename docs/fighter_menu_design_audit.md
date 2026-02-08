# Design Audit: Fighter Menu Screen

**Audit Date:** 2026-02-07
**Screen:** `fighter_menu` — Main Menu with Options/Character Select
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen game main menu UI with a three-column layout on a dark brown/olive background. The top-left displays "MainMenu" in italic yellow-green text, and the top-right shows "Points: 25000" in green. Below the header sits a horizontal tab bar with four tabs (Offline, Online, Customize, Options) flanked by L/R bumper indicators, with "Options" currently highlighted in yellow-green. The left column lists six menu items (System Options, Game Options, Display Options, Sound & Language, Network Options, Button Settings) prefixed with F-key shortcuts (F1–F6), with "System Options" highlighted in bright green. The center column displays "Offline Mode" as a section label above a large gray character placeholder silhouette with "Select Character" text. The right column shows a detail panel with a teal "@" icon, the heading "System Options," and description text "Configure vibration, save data, and autosave settings." The bottom of the screen has a controller hint bar showing +/Select, A/Confirm, B/Return, L R/Change Entry.

---

## Critical Issues (Fix Immediately)

### 1. Clashing Accent Colors Create Visual Dissonance
**Principle Violated:** Aesthetic Integrity (1.10), Color Guidelines (3.1)
The screen uses two unrelated accent colors that compete for attention: yellow-green (used for the "MainMenu" title, "Options" tab highlight, "Points" counter, and F1 "System Options" selection highlight) and teal/cyan (used for the right panel's "@" icon background and the detail panel border). These two hues are close enough on the color wheel to feel unintentional but different enough to appear mismatched. The result is a screen that looks like it has two competing design languages rather than one cohesive palette. The HIG's limited palette guideline (4–7 colors max) is technically met, but the colors chosen fight each other rather than working harmoniously.
→ **Fix:** Unify the accent palette. Either use the yellow-green consistently for all interactive/highlight states (replacing the teal border and icon background), or adopt the teal as the primary accent and restyle the tab/menu selection highlights. A single accent color with one complementary neutral creates much stronger visual cohesion.

### 2. Low Contrast on "Offline Mode" Section Label
**Principle Violated:** Color Guidelines (3.2), Accessibility (8.1)
The "Offline Mode" text above the character placeholder area uses a medium-light gray or off-white color against the dark brown background. This text serves as a critical navigational landmark — it tells the user which mode they're operating in — yet it has noticeably lower contrast than surrounding elements. At typical viewing distances (especially on a TV from a couch), this label could be easily missed or misread. The HIG explicitly warns that "text and thin lines have sufficient contrast to be easily visible."
→ **Fix:** Increase the text brightness to full white or use the same yellow-green accent color used for other important labels. Consider also increasing the font weight or size to match the navigational importance of this label.

### 3. Selection State Relies Solely on Color
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
Both the selected tab ("Options") and the selected menu item ("System Options") communicate their active state exclusively through a bright green/yellow-green background color fill. No other visual indicator — border, icon, arrow, underline, text weight change, or positional shift — reinforces the selection. This fails the fundamental color guideline that "color coding should be redundant with shape, position, or pattern." Colorblind users (particularly those with deuteranopia or protanopia) may struggle to distinguish the green highlight from the neutral gray of unselected items.
→ **Fix:** Add a secondary selection indicator: a left-edge accent bar, a chevron/arrow, a border outline, bold text weight, or a subtle positional indent on the selected item. Ensure the selection is identifiable in a grayscale rendering of the screen.

### 4. Character Placeholder Area Lacks Affordance and Feedback
**Principle Violated:** Feedback and Dialog (1.7), Direct Manipulation (1.2)
The large center area contains a gray silhouette and "Select Character" text, but provides no visual cue about how to interact with it, what actions are available, or what will happen when the user engages. Is this a button? Does the user press A? Navigate to it with the D-pad? The area is visually inert — a flat gray rectangle with no hover state, border, or interactive affordance. The HIG requires that "immediate feedback confirms every action" and that users can see what's interactive.
→ **Fix:** Add a visible border or subtle interactive indicator to the character area (e.g., a dashed outline, a "Press A to select" prompt, or a pulsing highlight). When the user navigates focus to this area, provide clear visual feedback that it is now the active element.

---

## Major Issues (Fix Soon)

### 5. Inconsistent Typography Creates Fragmented Visual Identity
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The screen uses at least four distinct typographic treatments: (a) "MainMenu" in large italic serif-like text, (b) tab labels in medium sans-serif, (c) left menu items in bold block sans-serif with visible F-key prefixes, (d) right panel description text in small regular sans-serif, and (e) "System Options" heading in the right panel in a different weight/size than the same text in the left sidebar. This typographic inconsistency makes the screen feel assembled from parts rather than designed as a unified whole. The HIG states that "visual styles should be consistent throughout."
→ **Fix:** Establish a clear type hierarchy using a single font family. Use size and weight (not entirely different fonts) to create hierarchy: one size for the page title, one for section/tab labels, one for menu items, and one for descriptive body text. The italic serif "MainMenu" title especially clashes with the rest of the sans-serif UI.

### 6. F-Key Labels Embedded in Menu Items Clutter the Design
**Principle Violated:** See-and-Point (1.3), Aesthetic Integrity (1.10)
Each menu item in the left sidebar has its keyboard shortcut (F1–F6) rendered inline as a prominent prefix, taking up significant visual space and creating a visual pattern where every line starts with an "F" followed by a number. The HIG advises that "keyboard shortcuts are alternatives, never the only way" and that they should be shown alongside menu items (as in standard menu keyboard equivalents), not baked into the label text. The current treatment makes the shortcuts feel mandatory rather than supplementary.
→ **Fix:** Move the F-key shortcuts to a right-aligned position within each menu row (mirroring how macOS shows ⌘-key shortcuts in menus), or display them as small subdued text rather than as part of the primary label. Reduce their visual prominence — they should be discoverable but not dominant.

### 7. Three-Column Layout Has Unbalanced Proportions
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
The three columns have visually unequal widths and inconsistent vertical alignment. The left menu column is narrow, the center character area is wide, and the right detail panel is medium-width but positioned high with significant empty space below it. The left column's bottom edge doesn't align with either the center or right columns' bottom edges. The right panel floats without clear spatial relationship to the center area. This creates an unbalanced, lopsided composition.
→ **Fix:** Define a clear column grid (e.g., 25% / 45% / 30%) and align all three columns to consistent top and bottom baselines. Ensure consistent padding/margins between columns. The right detail panel should extend vertically to match the left sidebar's height, or both should share a common bottom alignment.

### 8. Right Detail Panel Border Color Conflicts with Design Language
**Principle Violated:** Consistency (1.4), Color Guidelines (3.1)
The right panel has a prominent teal/cyan border that is the only element on screen using that color at full saturation in a structural role. Every other structural element (tabs, menu items, header) uses neutral grays, blacks, or the yellow-green accent. The teal border makes the right panel look like it belongs to a different application. This violates the consistency principle — visual styles should be uniform throughout the interface.
→ **Fix:** Replace the teal border with a subtle gray border matching the left sidebar's style, or use the yellow-green accent color at a reduced opacity. Alternatively, remove the border entirely and use spacing and background color differences to separate the panel.

### 9. Tab Bar Spacing and Alignment Is Inconsistent
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The four tabs (Offline, Online, Customize, Options) have noticeably uneven spacing between them. "Offline" and "Online" appear closer together, while "Customize" and "Options" have more breathing room. The L/R bumper labels sit at different distances from their nearest tabs. Additionally, the tab bar sits tightly below the "MainMenu" title with minimal breathing room, while there is generous space between the tab bar and the content below. Inconsistent spacing erodes the feeling of a carefully designed grid.
→ **Fix:** Use uniform padding between all tab items. Set consistent margin between the L/R indicators and the first/last tabs. Ensure equal vertical spacing above and below the tab bar to create balanced breathing room.

### 10. Bottom Control Legend Is Visually Disconnected
**Principle Violated:** Perceived Stability (1.9), Aesthetic Integrity (1.10)
The bottom bar with button hints (+/Select, A/Confirm, B/Return, L R/Change Entry) uses colored circles (red for A, green for B) and small white text on a dark semi-transparent strip. This bar has a completely different visual language from the rest of the screen — it uses colored circles as icons while no other element on screen uses circles, and the text size is significantly smaller than anything else. It feels like an afterthought overlaid on the screen rather than an integrated part of the design.
→ **Fix:** Integrate the control legend into the overall design language. Use the same font family and weight as the menu items. Consider using simple outlined button shapes instead of filled colored circles. Give the bar consistent left/right margins that match the content columns above.

---

## Minor Issues (Consider Fixing)

### 11. "Points: 25000" Has No Clear Purpose in Context
**Principle Violated:** WYSIWYG (1.5), See-and-Point (1.3)
The "Points: 25000" display in the top-right corner is presented with the same visual prominence as the "MainMenu" title but serves no obvious purpose in the context of a settings/options screen. Users may wonder: can I spend these points here? Is this a score? A currency? The value floats without context or interaction affordance, creating confusion about whether it's informational or actionable. The HIG warns against having "no secrets — what's on screen is what exists" in a confusing way.
→ **Fix:** If points are not actionable on this screen, reduce their visual prominence (smaller text, muted color) or move them to a dedicated status bar area. If they are actionable (e.g., for character customization), provide a visual link to where they can be spent. Add a subtle tooltip or icon to clarify what "Points" means.

### 12. "@" Icon Does Not Communicate "System Options"
**Principle Violated:** Metaphors (1.1), See-and-Point (1.3)
The right detail panel shows a large "@" symbol inside a rounded square as the icon for "System Options." The "@" symbol has strong associations with email and internet addressing — not with system configuration, vibration, save data, or autosave settings. This is an abstract icon with no clear real-world meaning related to the feature it represents. The HIG requires that "icons and visual elements resemble real-world objects users already understand."
→ **Fix:** Replace the "@" icon with a gear/cog icon (universally associated with system settings), a wrench, or a sliders icon. The icon should immediately communicate "configuration" or "settings" without requiring the user to read the text below.

### 13. Unselected Menu Items Lack Visual Separation
**Principle Violated:** Aesthetic Integrity (1.10), Color Guidelines (3.2)
The six menu items in the left sidebar (F2–F6, the unselected ones) have nearly identical styling: same background darkness, same text color, same font size. There is minimal visual separation between rows — they appear as a continuous dark block of text rather than distinct, individually selectable items. Only very thin separators (possibly 1px lines or no lines at all) divide them. The HIG's principle that "outlines provide clear edge definition" is not met.
→ **Fix:** Add subtle row separators (a 1px lighter line or slight gap between items), alternate row backgrounds for a zebra-stripe effect, or increase vertical padding between items. Each menu item should read as a distinct, targetable element.

### 14. "Select Character" Text Has Very Low Contrast
**Principle Violated:** Color Guidelines (3.2), Accessibility (8.1)
The "Select Character" label at the bottom of the center character placeholder area uses a medium-gray color against the gray placeholder background. This text is nearly invisible — it blends into the placeholder area rather than standing out as a call-to-action or label. The contrast ratio appears to be well below 3:1, making it difficult to read even for users with normal vision.
→ **Fix:** Increase the text contrast significantly — use white or a bright color that stands out against the gray placeholder. Consider making this text larger or bolder since it serves as the primary instruction for the most prominent screen area.

### 15. Tab Selection Highlight Bleeds Beyond Visual Boundaries
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
The yellow-green highlight on the "Options" tab extends to a width significantly wider than the text it contains, while the other tabs appear to have tighter text-to-edge relationships. This makes "Options" look visually heavier and wider than the other tabs, even though it has fewer characters than "Customize." Inconsistent padding within tab elements disrupts the rhythm of the tab bar.
→ **Fix:** Apply uniform horizontal padding to all tab items so that the background highlight width is proportional to content. Each tab should feel like part of the same set — same padding, same background treatment, just different text.

### 16. Dark Brown Background Is Unusual and Potentially Fatiguing
**Principle Violated:** Aesthetic Integrity (1.10), Color Guidelines (3.1)
The overall background uses a dark brown/olive tone that is uncommon in menu UIs and creates a warm, muddy ambiance. While thematically intentional for a fighting game, this background color reduces the perceived crispness of all overlaid elements. Greens and teals appear slightly muddy against it, and gray elements lack the clean contrast they would have against a neutral dark gray or black background.
→ **Fix:** Consider shifting the background to a darker, more neutral tone (dark charcoal or near-black) to let the accent colors and content areas pop with higher perceived contrast. If the brown is essential to the game's identity, darken it significantly to reduce its visual influence on overlaid colors.

### 17. Right Panel Description Text Is Too Small Relative to Available Space
**Principle Violated:** Aesthetic Integrity (1.10), See-and-Point (1.3)
The description text in the right panel ("Configure vibration, save data, and autosave settings.") is rendered in a noticeably small font size within a bordered area that has ample room for larger text. The text occupies perhaps 30% of the available width and is vertically cramped near the bottom of the panel. This wastes the detail panel's real estate and makes the description harder to read than necessary.
→ **Fix:** Increase the description text size by at least 20-30%. Use the full width of the bordered area with proper horizontal padding. Consider adding vertical centering or distributing the icon, heading, and description more evenly within the panel's vertical space.

---

## Strengths

- **Clear navigational structure:** The tab bar at the top cleanly separates major sections (Offline, Online, Customize, Options), and the left sidebar provides a scannable list of sub-options. The information architecture is logical and easy to understand.
- **Controller/keyboard hint bar present:** The bottom bar acknowledges multiple input methods and provides clear button-to-action mapping, supporting discoverability for gamepad users.
- **Detail panel provides context:** The right panel's approach of showing an icon, heading, and description for the selected menu item is a strong UX pattern — it lets users preview what they'll find before committing to navigate.
- **Consistent menu item structure:** The left sidebar maintains a predictable pattern (shortcut + label) for all six items, making it easy to scan and predict the layout.
- **Visible selection state exists:** While color-only, the green highlight does provide a clear indication of the currently selected tab and menu item — the issue is redundancy, not absence.
- **Stable landmark elements:** The "MainMenu" title and "Points" counter provide consistent reference points, and the tab bar is persistently visible, supporting perceived stability.

---

## Overall Score: 4/10

The fighter menu screen establishes a functional information architecture — the tab bar, sidebar, character area, and detail panel form a sensible layout for a game menu. However, the visual execution undermines this solid structure with significant issues. The most impactful problems are the clashing dual-accent colors (yellow-green vs. teal) that fracture the visual identity, the reliance on color alone for selection states (an accessibility failure), and the inconsistent typography that makes the screen feel assembled from disparate components. The low-contrast text in the character area and mode label, the embedded F-key prefixes cluttering the sidebar, and the spatially unbalanced column proportions further erode the design's clarity. Resolving the color palette conflict, adding redundant selection indicators beyond color, and establishing a unified typographic hierarchy would produce the most significant improvement with the least effort.
