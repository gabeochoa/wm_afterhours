# Design Audit: casual_settings

**Audit Date:** 2026-02-07
**Guidelines Used:** Apple Human Interface Guidelines (1987 Classic)
**Screen:** Settings panel from a casual game UI toolkit

---

## Critical Issues (Fix Immediately)

### 1. Color-Only State Indication on Toggle Buttons (HIG §3.1, §8.1)
The Music/Sound/Vibrate toggle buttons rely solely on color to communicate ON vs. OFF state. "Music: ON" is dark olive-green, "Sound: ON" is bright lime-green, and "Vibrate: OFF" is brown/gray. A colorblind user cannot reliably distinguish these states. The HIG mandates that color must never be the only distinguisher — shape, pattern, or position must provide redundant encoding.

**Recommendation:** Add a clear visual indicator beyond color: a filled checkbox, an icon swap (speaker vs. muted speaker), or a distinct shape change (e.g., toggle switch with a track that moves) to reinforce ON/OFF semantics.

### 2. Inconsistent Toggle Colors for the Same State (HIG §1.4, §3.2)
"Music: ON" uses a dark olive-green (#6B8E23-ish) while "Sound: ON" uses a brighter yellow-green (#9ACD32-ish). Both represent the same logical state (ON) but look different, violating consistency. Users will wonder if the darker green means something different from the lighter green.

**Recommendation:** Use a single color for all ON-state toggles and a single contrasting color/style for all OFF-state toggles. The same state must always look the same.

### 3. "Save/Load Progress" Combines Two Distinct Actions (HIG §2.7, §1.3)
This button merges two opposing operations — saving and loading — into a single control. The HIG requires button labels to describe the single action that will occur. Users cannot predict whether tapping this will save their current game or overwrite it with a loaded state. This is a potential data-loss vector.

**Recommendation:** Split into two separate buttons: "Save Progress" and "Load Progress." If space is constrained, use a segmented control or a sub-menu that clearly separates the two actions.

### 4. Version Text Is Nearly Illegible (HIG §3.2, §8.1)
The version string at the bottom center ("Version 1.11.0" or similar) is rendered in an extremely small, low-contrast font against the cream background. It fails minimum contrast and legibility requirements. Users who need this information (e.g., for support tickets) cannot read it.

**Recommendation:** Increase the font size to at least the body-text minimum, darken the text color to achieve sufficient contrast (minimum 4.5:1 ratio), and consider placing it in a more discoverable location such as the About screen.

---

## Major Issues (Fix Soon)

### 5. Four Distinct Button Styles Create Visual Incoherence (HIG §1.4, §1.10)
The screen uses at least four different button treatments:
- **Green/brown filled toggles** (Music, Sound, Vibrate) — small, squared corners, colored fill
- **Blue rounded buttons** (Notifications, Language, Credits, Support, Terms and Privacy) — large, heavy rounded corners, blue fill with white text
- **White outlined button** (Save/Load Progress) — white fill with dark outline and dark text
- **Small white outlined button** (About) — smaller scale, white fill with blue outline and blue text

This inconsistency violates the HIG principle that standard controls should be used consistently throughout. Users must learn four different visual languages on a single screen.

**Recommendation:** Establish at most two button tiers (primary action and secondary/informational) and apply them consistently. All buttons of the same tier should share identical styling.

### 6. No Functional Grouping or Section Headers (HIG §1.10, §2.3)
Audio toggles, cloud save, notifications, language, and informational links (Credits, Support, Terms, About) are all scattered across the panel without section labels, dividers, or spatial grouping. The HIG emphasizes grouping related items with separators and maintaining clear visual hierarchy.

**Recommendation:** Add section headers such as "Audio," "Data," "Info" with subtle dividers or spacing breaks. Group related controls visually so users can scan by category.

### 7. Flat Visual Hierarchy Among Blue Buttons (HIG §1.10)
Notifications, Language, Credits, Support, and Terms and Privacy all share identical blue styling and similar sizing. Functionally, "Notifications" and "Language" are settings that change behavior, while "Credits," "Support," and "Terms" are informational/navigational links. They should not carry equal visual weight.

**Recommendation:** Differentiate actionable settings from informational links. Use filled buttons for settings and text links or outlined buttons for informational items, or group them in visually distinct sections.

### 8. Unlabeled Icon Next to Save/Load (HIG §1.3, §8.3)
The small circular icon to the right of "Save/Load Progress" (appears to be a broadcast/WiFi-style icon) has no label or tooltip. Its meaning is ambiguous — does it indicate cloud sync status, connectivity, or something else? The HIG requires recognition over recall; icons must have obvious meanings.

**Recommendation:** Add a text label beneath or beside the icon, or replace it with a clearly recognizable icon (e.g., a cloud with an arrow for cloud save). If it indicates a status, make that status explicit with text.

### 9. Close Button (X) Overlaps the Card Border (HIG §2.1, §1.9)
The red circle X button straddles the top-right corner of the card, half inside and half outside the panel boundary. This breaks the perceived stability of the panel's frame and creates ambiguity about whether it belongs to the card or the outer screen. Standard close boxes should be positioned consistently within the window chrome.

**Recommendation:** Move the close button fully inside the card's top-right corner with consistent padding, or place it in a standard title-bar position. Ensure it does not break the panel border.

### 10. White Text on Medium-Blue Buttons — Borderline Contrast (HIG §3.2, §8.1)
The blue buttons use white text on a medium sky-blue background. Depending on the exact blue value, this may fall below the 4.5:1 contrast ratio required for normal text (and 3:1 for large text). The rounded, casual font compounds readability concerns.

**Recommendation:** Darken the blue background or use a bolder/heavier font weight. Test with a contrast checker to confirm a minimum 4.5:1 ratio. Consider using a darker teal or navy for better legibility.

---

## Minor Issues (Consider Fixing)

### 11. Decorative Title Font Reduces Scanability (HIG §1.10, §7.1)
The "Settings" title uses a handwritten/script-style font. While it fits the casual game aesthetic, it reduces scanability compared to a clean sans-serif. Users looking for "Settings" must parse a decorative typeface, which is slower than reading a standard heading font.

**Recommendation:** Use a clean, high-legibility font for the title, or at minimum ensure the decorative font is bold and large enough to be instantly readable. Reserve decorative type for branding, not functional headings.

### 12. Inconsistent Button Sizing and Alignment (HIG §1.4, §1.9)
The blue buttons in the left column are a different width than those in the right column, and the right column has three items while the left has two, creating a ragged layout. The "About" button is significantly smaller and isolated in the bottom-left corner with no visual relationship to other elements.

**Recommendation:** Use a consistent grid or column system. Either make all buttons the same width within their column, or adopt a uniform full-width button layout. Integrate "About" into the informational group rather than isolating it.

### 13. Excessive Outer Green Space / Wasted Screen Real Estate (HIG §1.10)
The large, plain green area surrounding the card takes up roughly 40% of the total screen. It provides no information or interaction and creates a tunnel-vision effect, making the settings panel feel cramped while the overall screen feels empty.

**Recommendation:** Either expand the card to use more of the available space (allowing for better grouping and spacing of controls), or add contextual elements to the background (subtle pattern, illustration) that support the game's theme without distracting.

### 14. "Vibrate: OFF" Brown Color Lacks Semantic Meaning (HIG §3.1, §3.2)
The brown/gray color used for the "Vibrate: OFF" state does not clearly communicate "disabled" or "inactive." Brown is not a standard color for representing an off state — it simply looks like a different-colored button rather than a deactivated control.

**Recommendation:** Use a desaturated or grayed-out version of the ON color (e.g., a muted gray-green) to clearly signal the off state. Standard practice is to make OFF states look faded or dimmed relative to their ON counterparts.

### 15. No Explicit "Done" or "Back" Button (HIG §2.5, §2.7)
The only way to dismiss the settings panel is the red X close button. The HIG recommends that dialogs include a clearly labeled action button (e.g., "Done," "Close," or "Back") in addition to or instead of a small icon-only close control. An explicit button is more discoverable, especially for less experienced users.

**Recommendation:** Add a "Done" or "Close" button at the bottom of the panel, styled as a primary action button, to provide a clear and accessible way to dismiss the settings screen.

### 16. Missing Feedback for Current Toggle States (HIG §1.7)
While the toggle labels include "ON"/"OFF" text, there is no affordance to indicate that these are interactive toggles rather than static labels. They look like colored badges rather than tappable controls. Users may not realize they can tap to change the state.

**Recommendation:** Add a toggle-switch affordance (e.g., a slider track, a check/uncheck animation), or add a pressed/hover state that makes interactivity obvious. Consider using standard toggle/switch components.

### 17. "Notifications: OFF" Uses Different Pattern Than Audio Toggles (HIG §1.4)
The Notifications toggle is styled as a large blue button with "OFF" in the label, while the audio toggles (Music, Sound, Vibrate) are styled as small colored blocks. Both are ON/OFF toggles but they look completely different, breaking consistency.

**Recommendation:** Use the same toggle component for all binary ON/OFF settings. If Notifications belongs in a different category, at minimum use the same visual pattern (size, shape, color scheme) for its toggle mechanism.

---

## Strengths

- **Clear labeling:** All buttons use plain, user-friendly language (no jargon, no technical codes). Labels like "Music: ON," "Language," "Credits" are immediately understandable.
- **Warm, inviting color palette:** The cream card, green background, and rounded shapes establish a friendly, approachable tone appropriate for a casual game.
- **Audio controls are prominently placed:** Music, Sound, and Vibrate controls are positioned at the top of the panel where users most commonly look first — good for the most-changed settings.
- **Version information is present:** Including a version string (even if too small) shows thoughtfulness for support scenarios.
- **Close button is visually distinct:** The red X provides a strong color contrast against the green/cream palette, making it easy to find the dismiss action.

---

## Overall Score: 4/10

The casual_settings screen has a warm, inviting aesthetic that suits its casual-game context, but it suffers from significant consistency and accessibility violations. The four distinct button styles, color-only state communication, flat visual hierarchy, and lack of functional grouping make the screen harder to scan, understand, and use than it should be. The dual-action "Save/Load Progress" button is a potential usability hazard. Addressing the critical and major issues — particularly consistent button styling, accessible toggle states, and clear functional grouping — would substantially improve both usability and visual coherence.
