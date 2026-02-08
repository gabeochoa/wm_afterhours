# Design Audit: Dead Space Settings Screen

**Audit Date:** 2026-02-07
**Screen:** `deadspace_settings` — Pause Menu / Settings Panel
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen game settings UI with two panels: a left sidebar labeled "PAUSE MENU" containing quick-access options (Resume Game, Menu Narration, Voice Language, Subtitles, Select Difficulty, Inverted Camera, Show Content Warning, More Settings), and a right main panel titled "// SETTINGS" listing settings categories (Controls, Gameplay, Display and Graphics, Audio, Language and Subtitles, Accessibility, Policies and Licenses, Credits) as large teal horizontal rows. The bottom of the screen shows controller/keyboard hints. The overall color scheme is dark background with teal/cyan accents and a hand-drawn typographic style.

---

## Critical Issues (Fix Immediately)

### 1. Extremely Poor Text-on-Background Contrast in Settings Rows
**Principle Violated:** Aesthetic Integrity (1.10), Color Guidelines (3.2)
The settings category labels (Controls, Gameplay, Audio, etc.) use a light teal/off-white text on a medium-teal background. This produces a contrast ratio well below the WCAG AA minimum of 4.5:1 for body text. The text blends into the row background, making labels difficult to read at a glance.
→ **Fix:** Use white (#FFFFFF) or very light text on the teal rows, or darken the row background significantly to create a contrast ratio of at least 4.5:1. Alternatively, switch to light text on a dark row background consistent with the overall dark theme.

### 2. Disabled/Dimmed Items Are Nearly Illegible
**Principle Violated:** Perceived Stability (1.9), Accessibility (8.1)
"Inverted Camera (Y-Axis)" and "Show Content Warning" in the left sidebar appear dimmed to indicate unavailability, but they are so faint against the dark background that they are nearly invisible. The HIG states that unavailable items should be dimmed, not removed — but "dimmed" should still be readable enough that users understand the option exists.
→ **Fix:** Increase the opacity of disabled items from the current ~25% to at least 40-50%. The text should be clearly readable as dimmed, not invisible. Consider adding a secondary indicator like a lock icon or "(unavailable)" suffix.

### 3. Selection/Focus State Is Barely Distinguishable
**Principle Violated:** Feedback and Dialog (1.7), Selection (5.1)
The currently focused item ("Gameplay" row) is indicated only by a marginally brighter left-edge accent bar — a difference of perhaps 5-10% brightness compared to non-focused rows. This violates the requirement for immediate, clear visual feedback on selection. Users scanning the screen cannot quickly identify which item is currently selected.
→ **Fix:** Make the focused row dramatically different: use a fully opaque highlight bar, change the row background color, add a border outline, increase text brightness to white, or animate a subtle glow. The selected state must be instantly recognizable without comparing rows side by side.

### 4. Color Is the Only Distinguisher Between States
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
Active vs. inactive items, selected vs. unselected rows, and enabled vs. disabled options are distinguished solely by color/brightness variations. No shape, icon, position, pattern, or text change accompanies these states. This fails the fundamental color guideline: "Color coding should be redundant with shape, position, or pattern."
→ **Fix:** Add secondary indicators: a chevron or arrow icon for the selected row, a different border treatment for active items, or text weight/style changes (bold for selected, regular for unselected). Ensure the design is fully usable on a monochrome display.

---

## Major Issues (Fix Soon)

### 5. Hand-Drawn Font Severely Reduces Readability
**Principle Violated:** Aesthetic Integrity (1.10), Plain Language (7.1)
The entire interface uses a scratchy, hand-drawn typeface that prioritizes stylistic atmosphere over legibility. This is especially problematic for smaller text in the left sidebar and the bottom button hints, where letterforms become ambiguous. While thematic for Dead Space's horror aesthetic, it violates the principle that graphics should "support understanding, not just decoration."
→ **Fix:** Reserve the hand-drawn font for headings and titles only (e.g., "// SETTINGS" and "PAUSE MENU"). Use a clean, legible sans-serif font for menu items, labels, and button hints where readability is critical. Alternatively, increase the font weight and size of the hand-drawn font to improve legibility at all sizes.

### 6. Dual-Panel Layout Creates Navigational Confusion
**Principle Violated:** Consistency (1.4), See-and-Point (1.3)
The left sidebar contains a mix of direct-action items (Resume Game), direct toggles (Subtitles), and a navigation item (More Settings), while the right panel contains only navigation categories. These two panels serve fundamentally different purposes but are presented as parallel structures with no visual distinction in their interaction models. Users must guess whether clicking an item will toggle a setting, navigate deeper, or perform an action.
→ **Fix:** Visually differentiate the two panels' roles. Add chevrons (>) to items that navigate deeper. Use toggle switches for items that are direct toggles. Give the left panel a distinct background treatment or border to separate it as a "quick actions" area vs. the main settings navigation.

### 7. Technical Jargon in Title: "// SETTINGS"
**Principle Violated:** Plain Language (7.1), Metaphors (1.1)
The "//" prefix is programming syntax (a comment delimiter in C-family languages). While it contributes to the Dead Space sci-fi aesthetic, it is meaningless jargon to most users and violates the plain language principle. The title should communicate "Settings" clearly without decorative code syntax.
→ **Fix:** Remove the "//" prefix and use "SETTINGS" alone. If the sci-fi aesthetic is essential, use a thematic but still understandable prefix like a small icon, a horizontal rule, or a geometric symbol rather than literal code syntax.

### 8. Inconsistent Text Alignment Between Panels
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The left sidebar items are left-aligned, while the right panel's settings category labels are center-aligned within their rows. This creates a visual disconnect between the two halves of the screen. Consistent alignment is a fundamental principle of visual design — mixed alignment makes the interface feel disjointed.
→ **Fix:** Left-align both panels' text content. If center alignment is preferred for the settings rows, also center the left sidebar items. Consistent alignment across the entire screen creates visual cohesion.

### 9. No Visual Grouping of Related Settings Categories
**Principle Violated:** See-and-Point (1.3), Aesthetic Integrity (1.10)
All eight settings categories are presented as identically-styled rows with uniform spacing, despite being logically groupable (e.g., Controls + Gameplay = game behavior; Display + Audio = output settings; Language + Accessibility = accessibility cluster; Policies + Credits = legal/info). The flat list forces users to read every item rather than scanning groups.
→ **Fix:** Add subtle visual separators (increased spacing, a thin divider line, or a section header) between logical groups. For example: a small gap after "Gameplay," after "Audio," and after "Accessibility" to create three visual clusters.

### 10. Bottom Button Hints Have Insufficient Contrast and Size
**Principle Violated:** Color Guidelines (3.2), See-and-Point (1.3)
The bottom-of-screen key hints ("[← SELECT" and "Esc BACK") are rendered in very small text with low contrast against the dark background. These are critical navigational affordances — they tell the user how to interact — yet they are the least visible element on screen. The "SELECT" label above them is also small and low-contrast.
→ **Fix:** Increase the font size of button hints by at least 30%. Use a higher-contrast color (white or bright teal) for the key labels. Consider placing them in a semi-transparent dark bar to create a consistent footer region. Add padding around the hint icons to increase their tap/visual target.

---

## Minor Issues (Consider Fixing)

### 11. Left Sidebar Selection Indicator Box is Visually Weak
**Principle Violated:** Feedback (1.7)
The "Resume Game" item at the top of the left sidebar has a thin rectangular outline indicating it is the focused item in that panel. This outline is a single-pixel border in a color barely distinguishable from the background. As the primary focus indicator for the sidebar, it needs to be much more prominent.
→ **Fix:** Increase the border width to 2-3px. Use a brighter color for the focus border (white or bright teal). Consider adding a subtle background fill inside the focused item to make it stand out further.

### 12. No Scroll Indicator for Potentially Overflowing Content
**Principle Violated:** WYSIWYG (1.5), Perceived Stability (1.9)
Neither panel shows any scroll indicator. If there are additional items below the visible area (or if the screen is viewed at a smaller resolution where items overflow), users have no way to know. The HIG requires that scroll bars represent the entire document dimension and show relative position.
→ **Fix:** If content can overflow, add a scroll indicator (a thin track or up/down arrows). If all content always fits, this is N/A — but consider adding a subtle "end of list" indicator so users know they're seeing everything.

### 13. Row Heights Are Uniform Regardless of Importance
**Principle Violated:** Aesthetic Integrity (1.10)
Every settings category row has exactly the same height, giving equal visual weight to "Controls" (frequently accessed) and "Policies and Licenses" (rarely accessed). This flat hierarchy means the user's eye has no guidance toward the most relevant options.
→ **Fix:** Consider making the most commonly accessed categories (Controls, Gameplay, Display and Graphics, Audio) slightly taller or giving them a different visual weight. Alternatively, place less-used items (Policies, Credits) in a secondary section with smaller rows.

### 14. No Escape Route Indicator in Left Panel
**Principle Violated:** Forgiveness (1.8), User Control (1.6)
While "Resume Game" is present at the top of the sidebar, there is no visual treatment that distinguishes it as the "escape" action. Users looking for a way to return to gameplay must read through the list. The "Esc BACK" hint at the bottom is easy to miss.
→ **Fix:** Give "Resume Game" a distinct visual treatment — a different color, an icon (e.g., a play triangle), or position it separately from the settings items with a divider. This makes the "exit" path immediately scannable.

### 15. Monochromatic Color Palette Limits Information Encoding
**Principle Violated:** Color Guidelines (3.1), Aesthetic Integrity (1.10)
The entire screen uses only two colors: near-black and various shades of teal/cyan. While thematically consistent with Dead Space, this severely limits the UI's ability to encode information through color. Active states, disabled states, headings, body text, interactive elements, and decorative elements all use the same teal hue at different opacities, making it difficult to quickly parse the screen's information hierarchy.
→ **Fix:** Introduce a secondary accent color (e.g., a warm amber or a cool blue-white) for interactive focus states or headings. Even within a constrained sci-fi palette, two distinct hues allow much richer information encoding than opacity variations of a single hue.

### 16. Teal Row Separators Are Too Subtle
**Principle Violated:** Color Guidelines (3.2)
The narrow dark gaps between the teal settings rows serve as the only visual separators. These gaps are very thin (approximately 2-4px) and blend into the dark background, making it hard to distinguish where one row ends and the next begins, especially in peripheral vision. This undermines the "outlines provide clear edge definition" guideline.
→ **Fix:** Increase the gap between rows to at least 6-8px, or add a subtle 1px border or shadow at the bottom of each row to create clearer visual separation. Alternatively, alternate row colors slightly (even a 5% brightness difference) to create a "zebra stripe" effect.

### 17. "PAUSE MENU" Label Has No Visual Hierarchy Over Its Children
**Principle Violated:** Aesthetic Integrity (1.10)
The "PAUSE MENU" heading at the top of the left sidebar is styled in approximately the same size and weight as its child items. It doesn't visually register as a section heading — it looks like another menu item. The only distinguisher is its position and the lack of a border box.
→ **Fix:** Increase the font size or weight of "PAUSE MENU," add a bottom border or underline beneath it, or use a different color to clearly distinguish it as a section label rather than an interactive item.

---

## Strengths

- **Clear spatial organization:** The two-panel layout (quick actions left, detailed categories right) is a sensible information architecture pattern for game settings.
- **Consistent row styling:** The right panel's settings categories are uniformly styled, creating a clean, predictable list structure.
- **Keyboard/controller hints present:** The bottom bar shows input method hints, acknowledging that users may be using different input devices.
- **Thematic cohesion:** The color palette and typography are strongly cohesive with the Dead Space franchise's sci-fi horror aesthetic — the UI feels like it belongs in the game world.
- **Escape path available:** Both "Resume Game" in the sidebar and "Esc BACK" at the bottom provide ways to exit, supporting user control.
- **Disabled items dimmed, not removed:** Following the HIG principle of perceived stability, unavailable options are dimmed rather than hidden (though they are too dim, as noted above).

---

## Overall Score: 4/10

The Dead Space settings screen successfully establishes atmospheric cohesion with the game's dark sci-fi horror aesthetic, but this comes at a significant cost to fundamental usability. The most critical failures are in contrast and readability — the teal-on-teal text in settings rows, the near-invisible disabled items, and the barely-perceptible selection state all make the interface physically difficult to read and navigate. The monochromatic palette and decorative typography compound these issues by limiting the visual vocabulary available for conveying state and hierarchy. Addressing the contrast and focus-state issues alone would substantially improve the score, as these are the root causes of most identified violations.
