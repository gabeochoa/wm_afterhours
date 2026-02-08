# Design Audit: Colors (Theme Color Swatches)

**Audited Against:** Apple Human Interface Guidelines (1987)
**Screen:** Theme Color Swatches — Midnight theme showcase
**Date:** 2026-02-07

---

## Critical Issues (Fix Immediately)

### 1. Color Is the Only Distinguisher Between Swatches
**HIG Violation:** 3.1 Color Fundamentals, 8.1 Vision Accessibility
The color swatches rely entirely on hue to differentiate one from another. There are no patterns, textures, icons, or shapes to provide redundant coding. A colorblind user (protanopia, deuteranopia) cannot distinguish the Red swatch from the Green swatch, or the Primary swatch from Error/Danger. The HIG explicitly states: "Color coding is redundant with shape/position/pattern" and "Color not the only distinguisher."
**Recommendation:** Add a secondary visual cue — a small icon, a unique pattern fill, a distinct border style, or a shape marker — to each swatch so that color is supplementary, not required.

### 2. Low Contrast: White Text on Gray "Secondary" Swatch
**HIG Violation:** 3.2 Color Usage Rules — "Text and thin lines have sufficient contrast to be easily visible"
The Secondary swatch is a medium gray (~#808080) with white text. This yields an estimated contrast ratio of roughly 2.5:1, well below the 4.5:1 minimum for normal text and even below the 3:1 large-text threshold. The label is difficult to read.
**Recommendation:** Use black or very dark text on the gray swatch, or darken the gray significantly to achieve at least 4.5:1 contrast with white text.

### 3. Low Contrast: White Text on Coral "Error/Danger" Swatch
**HIG Violation:** 3.2 Color Usage Rules
The Error/Danger swatch is a light salmon/coral color (~#FF6B6B) with white text. White on a pastel or light-warm color produces very poor contrast (estimated ~2:1). This makes the most important status color — the one indicating errors — the hardest to read.
**Recommendation:** Switch to black or very dark text on the Error/Danger swatch, or use a much darker red that maintains at least 4.5:1 contrast with white.

---

## Major Issues (Fix Soon)

### 4. Inconsistent Text Color Logic Across Swatches
**HIG Violation:** 1.4 Consistency
The top row uses white text on most swatches (Primary, Secondary, Surface, Error/Danger) but switches to black text on Accent. The bottom row uses black text on all swatches. There is no visible system governing when text is white vs. black. This inconsistency violates the HIG principle that "visual styles [should be] consistent throughout."
**Recommendation:** Implement a systematic auto-contrast rule (e.g., use black text when background luminance > 0.5, white when < 0.5) and apply it uniformly to all swatches.

### 5. Section Labels Are Near-Invisible Low-Contrast Text
**HIG Violation:** 3.2 Color Usage Rules, 1.10 Aesthetic Integrity — "Clear visual hierarchy—most important elements stand out"
The "Theme Colors" and "Custom Colors" section labels are rendered in a small, muted reddish-gray font against the very dark panel background. They are barely legible and fail to establish section hierarchy. Section headings should be prominent navigational landmarks.
**Recommendation:** Increase the font size and brightness of section labels. Use a lighter color (e.g., white or light gray) or a bolder weight to ensure they read clearly against the dark background.

### 6. No Visual Hierarchy — All Swatches Have Equal Weight
**HIG Violation:** 1.10 Aesthetic Integrity — "Clear visual hierarchy—most important elements stand out"
Every swatch is the same size, same border treatment, and same visual prominence. Primary, Secondary, Accent, Surface, and Error/Danger are all equally weighted, providing no guidance about which colors are most important or most frequently used. The HIG requires that "different things look different" and that there be a clear visual hierarchy.
**Recommendation:** Differentiate the Primary and Accent colors with larger swatches or more prominent placement. Consider using size, border weight, or label styling to signal relative importance.

### 7. Title Bar Red Conflicts with Primary Swatch Red
**HIG Violation:** 3.1 Color Fundamentals — "Limited palette (4-7 colors max for coding)"
The title bar uses a bright crimson red (#CC2222 approximate) while the Primary swatch uses a darker crimson red (#8B0000 approximate). Having two different reds in close proximity creates confusion about which red is "Primary." The header red is not identified as a theme color, adding an unexplained color to the palette.
**Recommendation:** Either make the title bar use the actual Primary color, use a neutral color for the header, or explicitly label the header color as part of the theme.

### 8. Monospace/Pixel Font Reduces Readability
**HIG Violation:** 1.10 Aesthetic Integrity, 7.1 Text Guidelines — "Messages are concise and simple"
The entire screen uses a monospace pixel-style font, including the title, labels, hex values, and footer text. While thematically appropriate for a game UI, monospace fonts are harder to scan for body text and labels. Character spacing is uneven for proportional words like "Primary" and "Error/Danger," hurting legibility.
**Recommendation:** If the pixel font is a deliberate game aesthetic choice, ensure it is rendered at a size large enough to remain legible. Consider using a proportional font for the description footer, or increase letter-spacing for readability.

### 9. Swatch Sizing Is Inconsistent in the Top Row
**HIG Violation:** 1.4 Consistency, 1.10 Aesthetic Integrity
The "Error/Danger" swatch in the top row is visually wider than the others to accommodate its longer label, breaking the uniform grid. The bottom row has more consistent swatch sizing. Inconsistent element sizes undermine the sense of a cohesive, stable layout.
**Recommendation:** Use a fixed swatch size for all entries and allow text to wrap or abbreviate. Alternatively, size all swatches to the widest label so they are uniformly large.

### 10. No Interaction Affordance — Swatches Don't Look Actionable
**HIG Violation:** 1.3 See-and-Point, 1.7 Feedback and Dialog
The swatches are flat colored rectangles with no hover state, pressed state, selection indicator, or cursor change implied by their design. If users are expected to click to copy a hex value, select a color, or view details, there is no visual affordance suggesting interactivity. The HIG requires that all available actions are visible and discoverable.
**Recommendation:** Add a subtle hover effect, a clickable border treatment, or a "click to copy" tooltip. If the swatches are display-only, add a visual cue (e.g., removing the rounded-corner button-like styling) to make that clear.

---

## Minor Issues (Consider Fixing)

### 11. Hex Values Shown Only for Custom Colors, Not Theme Colors
**HIG Violation:** 1.4 Consistency
The bottom row ("Custom Colors") displays hex values beneath each color name, but the top row ("Theme Colors") does not. If this is a color reference screen, users would benefit from seeing hex values for all swatches. The inconsistency violates the HIG consistency principle.
**Recommendation:** Show hex values for all swatches, or provide a consistent way to reveal them (e.g., hover or tap).

### 12. Excessive Dead Space Between Bottom Row and Footer Text
**HIG Violation:** 1.10 Aesthetic Integrity
There is a large empty gap between the Custom Colors row and the footer description text at the bottom of the panel. This wasted space makes the layout feel unbalanced, with content clustered at the top and a sparse bottom half.
**Recommendation:** Tighten the vertical spacing or add additional color information (e.g., HSL values, usage guidelines) to fill the space meaningfully.

### 13. Footer Text Is Small and Low-Contrast
**HIG Violation:** 3.2 Color Usage Rules, 7.1 Text Guidelines
The footer text ("Using Midnight theme. Top row: theme colors. Bottom row: custom colors with hex values.") is rendered in the same small monospace font and appears in a muted gray that is hard to read against the dark panel background.
**Recommendation:** Increase the footer text size slightly or use a brighter gray to improve legibility. The explanatory text is useful and should be easy to read.

### 14. Green Borders Appear on Only Some Swatches
**HIG Violation:** 1.4 Consistency
Some swatches (notably Primary and Accent in the top row) appear to have bright green borders, while others have darker, less visible borders. If the green border indicates selection or a special state, it is not explained. If it is decorative, the inconsistency is confusing.
**Recommendation:** Apply a uniform border color to all swatches, or clearly indicate what the green border signifies (e.g., "currently selected" or "active color").

### 15. No Colorblind-Safe Labeling or Accessibility Indicators
**HIG Violation:** 8.1 Vision Accessibility
Beyond the color-only coding issue (Critical #1), the screen offers no accessibility accommodations. There are no patterns, no contrast mode, no way to identify colors without seeing them. The labels help somewhat, but the visual design assumes full color vision.
**Recommendation:** Add a small swatch pattern (diagonal lines, dots, crosshatch) unique to each color. Consider offering a colorblind simulation toggle to preview how the palette appears under different vision conditions.

### 16. Panel Has No Discernible Window Controls or Navigation
**HIG Violation:** 2.1 Windows, 1.9 Perceived Stability
The panel floats on a solid black background with no title bar controls (close, minimize, zoom), no scroll bars, and no navigation back to the showcase. There are no familiar window landmarks to orient the user.
**Recommendation:** Add standard window controls or navigation affordances (back button, close button) so users understand where they are and how to leave the screen.

### 17. Surface Swatch Is Visually Confusing — Yellow for a "Surface" Color
**HIG Violation:** 1.1 Metaphors from the Real World
The "Surface" color is rendered as a bright yellow/gold, which is an unusual choice for a surface/background color. In most design systems, surface colors are neutral (white, gray, dark gray) to serve as a backdrop. A vibrant yellow surface would make text and UI elements placed on it very hard to read, undermining the metaphor of a "surface" as a canvas.
**Recommendation:** Reconsider the Surface color choice. If yellow is intentional for the Midnight theme, show a small preview of how it would look as an actual background with text on it, so users can evaluate its utility.

---

## Strengths

- **Clear labeling:** Each swatch is clearly labeled with its semantic role (Primary, Secondary, Accent, Surface, Error/Danger) or its color name and hex value, which aids understanding.
- **Logical grouping:** Separating theme colors from custom colors into two distinct rows is a sensible organizational choice.
- **Dark theme execution:** The Midnight theme's dark panel on black background creates a focused, immersive feel appropriate for a game UI toolkit.
- **Hex values provided:** The custom color row includes hex codes, which is useful for developers referencing exact values.
- **Descriptive footer:** The footer text explains the layout context and which theme is active, helping orient the user.

---

## Overall Score: 4/10

The Colors screen succeeds as a basic color palette display but has significant issues with contrast, accessibility, consistency, and visual hierarchy. Three swatches have text contrast failures that make labels difficult to read — and one of these is the Error/Danger color, the most safety-critical color in any UI system. The complete reliance on color as the only distinguishing feature is a critical accessibility violation. Inconsistent text color logic, border treatments, and information density (hex values for one row but not the other) undermine the HIG's consistency principle. Addressing the contrast failures and adding redundant non-color coding would substantially improve the screen's usability and raise the score.
