# Design Audit: Mini Motorways Settings Screen

**Audited Against:** Apple Human Interface Guidelines (1987 Desktop Interface)  
**Screen:** Game Settings panel within the main Settings menu  
**Date:** 2026-02-07

---

## Audit Summary: mini_motorways_settings

### Critical Issues (Fix Immediately)

1. **Non-Standard Toggle Controls (Consistency §1.4, Metaphors §1.1):** The on/off settings (Night Mode, Vibration, Hold to Draw/Delete, Colorblind Mode) use custom hollow/filled circles that do not resemble any standard control—neither checkboxes, radio buttons, nor platform toggle switches. A hollow circle versus a filled circle is not a universally understood on/off metaphor. Users must guess what the visual states mean.  
   → **Fix:** Replace with standard toggle switches or clearly labeled checkboxes with "On"/"Off" text. Each control should use a recognizable real-world metaphor (a physical switch, a check mark) so the state is self-evident.

2. **Color-Only Distinction for Active Tab (Color §3.1, Accessibility §8.1):** The currently selected "Game" tab is distinguished from the other tabs (Video, Audio, Language, Controls, Cross-Save, Credits) primarily by its orange/yellow background color versus their teal/mint background. No additional shape, border weight, icon, or text treatment differentiates the active tab. This is a color-only distinction and fails for colorblind users or monochrome contexts.  
   → **Fix:** Add a secondary visual indicator for the active tab—such as a bold typeface, an underline, a different border style, an inset/raised appearance, or a position shift—so the active state is apparent without relying on color alone.

3. **Ambiguous Toggle State (WYSIWYG §1.5, Feedback §1.7):** It is unclear which state is "on" and which is "off" for the circle toggles. "Hold to Draw/Delete" shows a filled (dark) circle while the others show hollow circles, but nothing explicitly labels the filled state as enabled. Users cannot tell what they see equals what they will get without trial and error.  
   → **Fix:** Add explicit state labels ("On"/"Off") next to each toggle, or use an asymmetric control (like a sliding toggle with a colored track) where the active vs. inactive state is visually unambiguous.

4. **Back Button Uses Cryptic ASCII Symbol (See-and-Point §1.3, Plain Language §7.1):** The top-left navigation button displays `<-` as raw text characters inside a yellow square. This is not a universally clear icon—it reads as punctuation rather than a navigational affordance. Users must interpret the ASCII art rather than recognizing a standard back arrow or the word "Back."  
   → **Fix:** Use a proper left-arrow icon (←) or a chevron glyph, optionally paired with the word "Back." The control should be immediately recognizable as navigation without requiring interpretation.

---

### Major Issues (Fix Soon)

5. **No Undo or Reset Mechanism (Forgiveness §1.8):** There is no visible "Reset to Defaults," "Cancel," or "Undo" option on this settings screen. If a user accidentally changes a setting, there is no way to revert without knowing what the original value was. This violates the principle that users should be able to explore without fear of permanent consequences.  
   → **Fix:** Add a "Reset to Defaults" button at the bottom of the settings content area. Optionally, defer saving changes until the user confirms, or provide an undo action when a setting is toggled.

6. **Label-Control Association Is Weak Due to Excessive Spacing (Aesthetic Integrity §1.10):** The text labels for each toggle (e.g., "Night Mode," "Vibration") are left-of-center while their corresponding circle controls are positioned far to the right. The large horizontal gap between label and control weakens the visual association, forcing users to scan back and forth to confirm which label belongs to which toggle.  
   → **Fix:** Reduce the horizontal distance between labels and their controls. Use a consistent row layout where the label and control are visually grouped—either closer together or connected by a subtle horizontal line/background stripe.

7. **Inconsistent Button Styling Across the Screen (Consistency §1.4):** The screen contains at least four distinct button styles: (a) the `<-` back button in a yellow square, (b) the tab list items with teal backgrounds and dark-green left borders, (c) the `<` and `>` stepper arrows in yellow squares for sensitivity, and (d) the "Tutorial ->" button in an orange rectangle. Each has different shapes, colors, border treatments, and typography. This visual inconsistency makes it harder for users to identify interactive elements.  
   → **Fix:** Establish a unified button style system. Navigation buttons, action buttons, and stepper controls should share a consistent visual language (e.g., consistent corner radius, color family, border treatment) even if their shapes differ by function.

8. **Controller Cursor Sensitivity Stepper Provides No Range Context (See-and-Point §1.3, User Control §1.6):** The sensitivity control shows "Default" with `<` and `>` arrows, but does not indicate how many options exist, what the minimum and maximum values are, or where "Default" falls in the range. Users are forced to blindly click through to discover available options.  
   → **Fix:** Display the range (e.g., "Low / Default / High") or use a slider with tick marks so users can see all options at once. Alternatively, show a numeric scale or list of discrete values.

9. **"Tutorial" Button Is Contextually Misplaced (Perceived Stability §1.9, Consistency §1.4):** A "Tutorial ->" button appears in the bottom-right of the settings screen. Its purpose is unclear in the settings context—does it launch a tutorial about settings, or navigate away to the game's tutorial? Its placement alongside settings content creates confusion about the screen's scope and purpose.  
   → **Fix:** If the Tutorial is a global navigation element, move it to the main menu or a dedicated tab. If it is contextual help for settings, relabel it (e.g., "Settings Help") and position it near the content it describes.

10. **No Visible Focus or Hover States (Feedback §1.7, Accessibility §8.3):** None of the interactive elements—tabs, toggles, stepper buttons, or navigation buttons—display any visible hover, focus, or pressed states. Users navigating with a controller or keyboard have no indication of which element is currently focused. Even mouse users get no feedback on hover, reducing confidence about what is clickable.  
    → **Fix:** Add clear focus rings or highlight states for keyboard/controller navigation, and subtle hover effects (color shift, underline, or scale change) for pointer-based interaction.

---

### Minor Issues (Consider Fixing)

11. **Version Text Has Low Contrast (Color §3.2):** "Mini Motorways Version 1.10.2" in the bottom-left uses small, gray text on the cream/beige background. The contrast ratio appears insufficient for comfortable reading, especially at a distance (e.g., on a TV screen for console players).  
    → **Fix:** Increase the text contrast by darkening the font color or using a slightly bolder weight. Even version text should meet minimum contrast ratios for legibility.

12. **Tab List Lacks a Clear Visual Container (Perceived Stability §1.9):** The vertical list of category tabs (Game, Video, Audio, etc.) floats in space on the left side without a clear panel boundary, divider line, or background differentiation separating it from the content area on the right. The layout relationship is implied only by spatial proximity.  
    → **Fix:** Add a subtle vertical divider, a slightly different background shade for the tab column, or a container border to visually separate navigation from content.

13. **Mixed Interaction Paradigms Without Explanation (Consistency §1.4):** The screen uses three different control types—circle toggles for booleans, a stepper for sensitivity, and text buttons for navigation—without any visual or textual cue explaining how each works. A first-time user encountering the circle toggles may not know to click them, especially since they lack the affordances of standard controls.  
    → **Fix:** Use controls with clear affordances (e.g., switches that look slideable, buttons that look pressable). If custom controls are necessary, include a brief instructional tooltip or use familiar platform conventions.

14. **"Hold to Draw/Delete" Label Is Ambiguous (Plain Language §7.1):** This label assumes the user already knows what "Hold to Draw/Delete" means in the context of gameplay. A new user visiting settings before playing would have no context for this option. No description, subtitle, or tooltip is provided.  
    → **Fix:** Add a brief description below the label (e.g., "Require holding the button to draw or delete roads") or provide a tooltip/info icon that explains the behavior.

15. **Vertical Spacing Is Inconsistent Between Setting Rows (Aesthetic Integrity §1.10):** The vertical gaps between the four toggle settings appear uniform, but the "Controller Cursor Sensitivity" row breaks the rhythm—it is positioned lower with a visual separator (thin line above it) that the other rows lack. This creates an uneven visual cadence.  
    → **Fix:** Either apply consistent separators between all rows, or use consistent spacing throughout. If the sensitivity setting is a different category, group it under a visible subheading rather than using an inconsistent divider.

16. **Tab Text Has No Hierarchy Differentiation (Aesthetic Integrity §1.10):** All tab labels (Game, Video, Audio, Language, Controls, Cross-Save, Credits) use the same font size, weight, and style. The only differentiation is the background color of the active tab. Adding a text-level distinction (bold, color change, or size change) for the active tab would reinforce which section the user is viewing.  
    → **Fix:** Bold the active tab label, or change its text color to match the orange highlight, to create a clear typographic hierarchy alongside the background color change.

17. **Arrow Notation in "Tutorial ->" Is Non-Standard (Plain Language §7.1, Consistency §1.4):** The "Tutorial ->" button uses ASCII arrow characters (`->`) rather than a proper arrow glyph (→) or chevron (›). Combined with the `<-` back button, this creates an ASCII-art aesthetic that feels inconsistent with the otherwise polished visual design of the screen.  
    → **Fix:** Replace ASCII arrow text with proper Unicode arrow glyphs or chevron icons to maintain visual polish and consistency.

---

### Strengths

- **Clean, minimal background:** The cream/beige background is easy on the eyes and avoids visual clutter, letting the content take center stage.
- **Clear category organization:** The left-side tab list provides a logical, scannable grouping of settings categories (Game, Video, Audio, etc.), following a familiar settings pattern.
- **Readable typography:** The setting labels use a legible sans-serif font at a comfortable size, with good letter-spacing for readability.
- **Warm, cohesive color palette:** The orange/yellow accent color and teal/mint tab colors create a distinctive, cheerful identity that matches the game's brand without being visually aggressive.
- **Simple, uncluttered layout:** The screen avoids overloading the user with too many options at once—only five settings are shown for the Game category, keeping cognitive load low.

---

### Overall Score: 4/10

The Mini Motorways settings screen achieves a clean, pleasant aesthetic but falls short on fundamental usability principles. The most pressing issues are the non-standard toggle controls with ambiguous states (users cannot confidently tell what is on or off), the color-only tab differentiation (accessibility failure), and the absence of undo/reset mechanisms. The inconsistent button styles and lack of focus/hover states further erode usability, particularly for controller and keyboard users. While the visual identity is charming and the category organization is sound, the interface prioritizes stylistic minimalism over functional clarity—the exact trade-off the Apple HIG warns against. Addressing the critical and major issues would significantly improve both usability and accessibility without sacrificing the screen's distinctive character.
