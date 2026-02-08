# Design Audit: Bevel Borders Screen

*Audited against the Apple Human Interface Guidelines (1987 Desktop Interface)*
*Date: February 7, 2026*

---

## Audit Summary: Bevel Borders Screen

### Critical Issues (Fix Immediately)

1. **[Low Contrast Bevel at Small Widths — HIG 3.2]**: The 1px and 2px bevel borders are nearly invisible. The light and dark edge colors are so close in luminance to the box face color that the bevel effect is imperceptible at these sizes. The HIG explicitly requires that "small objects use high-contrast colors" and that "text and thin lines have sufficient contrast to be easily visible." A 1px line with a contrast ratio approaching 1:1 against its adjacent surface fails this rule entirely.
   → **Fix**: Increase the luminance difference between the bevel highlight/shadow colors and the face color. For 1px and 2px borders, the edge colors need to be dramatically lighter (highlight) and darker (shadow) than the face — a contrast ratio of at least 3:1 against the face to remain perceptible.

2. **[Color-Only Distinction Between Raised and Sunken — HIG 3.1, 8.1]**: The only difference between "Raised" and "Sunken" variants is which edges receive the lighter vs. darker color. There is no secondary indicator (icon, label differentiation, pattern, arrow direction) to communicate the semantic difference. On a monochrome display, or for a colorblind user, the two rows would appear nearly identical. The HIG states the design must "work in black and white first" and that "color coding is redundant with shape/position/pattern."
   → **Fix**: Add a non-color indicator for raised vs. sunken state. Options include: a small upward/downward arrow icon in each box, a subtle gradient on the face (lighter center for raised, darker center for sunken), or a textured pattern difference. The labels alone are insufficient since they require the user to cross-reference row labels with distant boxes.

3. **[Flat Visual Hierarchy — HIG 1.10]**: Every element on the screen has nearly the same visual weight. The title, subtitle, column headers, row labels, and box content all use similar-value colors on the same dark background, creating a wall of sameness. The HIG states that "most important elements stand out" and "different things look different." Nothing stands out here — the eye has no clear entry point or reading path.
   → **Fix**: Establish a clear hierarchy with at least 3 tiers of visual prominence: (1) the title should be brightest/largest, (2) the row labels "Raised" and "Sunken" should be secondary-prominent as they organize the content, (3) the column headers and box labels should be tertiary. Use brightness, size, and weight differences to create separation between these tiers.

4. **[Subtitle Text Is Nearly Illegible — HIG 3.2, 7.1]**: The explanatory subtitle "Raised: light top/left, dark bottom/right. Sunken: inverted." is rendered in extremely small, low-contrast gray text on the dark background. This is the only text that explains how bevel borders work, yet it is the hardest text on the screen to read. The HIG requires that "small objects use high-contrast colors" — this does the opposite, using the lowest contrast for the smallest text.
   → **Fix**: Increase the subtitle font size to at least match the column header text size. Increase its brightness to near-white (aim for WCAG AA 4.5:1 contrast ratio). This text is the key educational content of the showcase and should be prominent, not hidden.

5. **[No Monochrome Fallback — HIG 3.1, 8.1]**: The entire bevel border concept relies on color/luminance differences between edges. On a monochrome display set to a single gray level, the bevel effect would vanish completely, and the raised and sunken rows would be indistinguishable. The HIG requires designs to "work in black and white first."
   → **Fix**: Ensure the bevel highlight and shadow colors have sufficient luminance contrast to remain visible even in grayscale. Consider adding a secondary visual cue such as a 1px solid outline around the outer edge of all boxes so the border region is always perceivable, even when the bevel gradation is subtle.

### Major Issues (Fix Soon)

6. **[Column Headers Misaligned with Boxes — HIG 1.4, 1.10]**: The column headers (1px, 2px, 3px, 4px, 5px, 6px) appear to float above the grid without clear visual connection to the columns they label. There are no vertical guides, alignment lines, or consistent centering that ties each header to its corresponding column. The spacing between headers and boxes is ambiguous — it is unclear whether the headers refer to the boxes below or above them.
   → **Fix**: Center each column header precisely above its column of boxes. Add consistent vertical spacing. Consider using a light vertical rule or aligning the header text with the box centers using a strict grid system.

7. **[Row Labels Lack Visual Weight — HIG 1.10]**: The "Raised" and "Sunken" labels are the primary organizational elements of the screen — they tell the user what the two rows demonstrate. Yet they are rendered in a light, sketch-style font at roughly the same size and brightness as the box content labels. They fail to anchor the rows visually.
   → **Fix**: Increase the visual prominence of row labels. Use a bolder weight, larger size, or brighter color. Consider left-aligning them with a clear margin from the box grid, or placing them in a distinct color band or container to separate them from the content.

8. **[Inconsistent Title Border vs. Content Borders — HIG 1.4]**: The title "BEVEL BORDERS" sits inside a beveled container at the top of the screen, but its bevel treatment (a single subtle raised bevel) is visually distinct from any of the 12 showcase boxes below. The title bevel appears to use a different width, different edge colors, and a different face color than the showcase samples. This inconsistency undermines the showcase's own message.
   → **Fix**: Either make the title container match one of the showcase bevel styles exactly (e.g., use the 3px raised bevel) and label it accordingly, or remove the bevel from the title container entirely so it doesn't compete with the samples.

9. **[Redundant Information in Box Labels — HIG 1.10, 7.1]**: Each box displays its border width (e.g., "1px", "2px") as its content label. This information is already communicated by the column headers directly above. The duplication wastes the box's visual space and adds visual clutter without new information. The HIG warns against "decoration that distracts from function."
   → **Fix**: Either remove the column headers and let the box labels serve as the sole indicator, or remove the box labels and let the column headers be the sole indicator. Alternatively, use the box content area to display something more useful — for example, the actual hex color values of the highlight and shadow edges.

10. **[No Edge Definition Between Adjacent Boxes — HIG 3.2]**: The boxes in each row sit directly adjacent to each other with minimal gap. For the 5px and 6px variants, the thick bevel borders of adjacent boxes nearly touch or merge, making it difficult to distinguish where one box's shadow edge ends and the next box's highlight edge begins. The HIG requires "outlines provide clear edge definition."
    → **Fix**: Add a consistent gap (at least 8–12px) between all boxes to ensure the bevel borders are clearly perceivable as individual elements. The current tight spacing works against the showcase's purpose of demonstrating the bevel effect.

11. **[Dark-on-Dark Background Contrast — HIG 3.2]**: The box face color and the screen background color are very close in luminance — both are dark blue-grays separated by perhaps 10–15% brightness. This makes the boxes blend into the background, especially for the sunken variants where the shadow edges darken the box further. The HIG requires clear edge definition.
    → **Fix**: Increase the contrast between box faces and the page background. Either lighten the box faces significantly (to a mid-tone gray-blue) or darken the page background further. A contrast ratio of at least 3:1 between the box face and page background would make the boxes clearly distinct.

### Minor Issues (Consider Fixing)

12. **[Sketch-Style Typography Reduces Precision — HIG 1.10, 7.1]**: The entire screen uses a hand-drawn/sketch-style font (appears to be a marker or chalk font). While this may be an intentional game UI aesthetic choice, it undermines the precision messaging of a technical showcase about pixel-exact border widths. The casual, imprecise letterforms conflict with the precise "1px, 2px, 3px" labels they display.
    → **Fix**: Consider using a clean, monospaced, or geometric sans-serif font for the numeric labels and technical text (subtitle, column headers). The title and row labels can retain the game-style font for personality, creating a clear distinction between decorative and informational text.

13. **[No Interactive Affordance — HIG 1.2, 1.3]**: The screen is a static showcase with no apparent way to interact with or customize the bevel borders. Users cannot change the bevel colors, toggle between raised and sunken, adjust the width dynamically, or experiment with the effect. The HIG favors direct manipulation — letting users "try" rather than "read about."
    → **Fix**: Consider adding interactive elements: a slider to dynamically adjust border width, color pickers for highlight/shadow/face colors, or a toggle that switches between raised and sunken on a single large sample. Even in a showcase, interactivity teaches more effectively.

14. **[Wasted Screen Space — HIG 1.10]**: The bottom third of the screen is entirely empty dark background. The content is clustered in the upper portion, leaving a large void that makes the layout feel unbalanced and the screen feel incomplete. This wastes an opportunity to provide additional useful information.
    → **Fix**: Either center the content vertically on the screen, or use the empty space for supporting content — a legend showing the bevel color values, a larger interactive sample, or usage guidelines for when to use each border width.

15. **[Column Header Typography Too Small — HIG 3.2]**: The column header labels (1px, 2px, etc.) use a notably smaller font size than the row labels or box content, creating an inconsistency in the typographic scale. Their small size combined with low contrast makes them the second-hardest text to read after the subtitle.
    → **Fix**: Increase the column header font size to match or nearly match the row labels. All organizational text (row labels and column headers) should share a consistent size tier in the typographic hierarchy.

16. **[No Specification Values Shown — HIG 1.5]**: This is a toolkit showcase for developers/designers, yet it provides no concrete values — no hex colors for the highlight, shadow, or face; no HSL values; no opacity levels. Users cannot take actionable information from the screen to implement bevel borders themselves. The HIG WYSIWYG principle implies that what users see should be what they can use.
    → **Fix**: Add a specifications panel or tooltip showing the exact color values used for each bevel edge. For a toolkit showcase, include the CSS/code equivalent or the color tokens used. Consider adding a small legend: "Highlight: #8A92A4, Shadow: #2B3142, Face: #4B5568" (or whatever the actual values are).

17. **[Inconsistent Vertical Spacing Between Rows — HIG 1.4]**: The vertical spacing between the column headers and the "Raised" row, between the "Raised" row and the "Sunken" row, and between the "Sunken" row and the bottom of the content area all appear to be different values. Consistent spacing is a fundamental aspect of visual consistency per the HIG.
    → **Fix**: Standardize the vertical rhythm. Use the same spacing value between all major sections: header-to-grid, row-to-row, and grid-to-bottom. A consistent 24px or 32px vertical gap between sections would unify the layout.

18. **[Title Uses Small Caps Without Clear Purpose — HIG 1.4, 7.1]**: The title "BEVEL BORDERS" is rendered in all-caps with a small-caps style. While this adds visual distinction, the small-caps treatment reduces readability compared to standard title case or sentence case. The mixed-size letterforms create a visual stutter that fights the clean, simple communication the HIG recommends.
    → **Fix**: Use standard title case ("Bevel Borders") or all caps with consistent letter sizing. If small caps are desired for stylistic reasons, ensure the lowercase letters are large enough (at least 70% of cap height) to remain easily readable.

---

### Strengths

- **Clear organizational structure**: The two-row layout (Raised vs. Sunken) with progressive width columns (1px through 6px) creates a logical matrix that is intuitive to scan. The concept is immediately understandable.
- **Correct bevel rendering**: The actual bevel border effect is implemented correctly — raised boxes have lighter top/left and darker bottom/right edges, sunken boxes have the inverse. The 3D illusion works as intended, particularly at 4px–6px widths.
- **Progressive demonstration**: Showing the bevel at six different widths allows users to see how the effect scales, which is genuinely useful for a toolkit showcase. The 1px-to-6px range covers the practical spectrum of bevel border usage.
- **Consistent dark aesthetic**: The dark color palette is cohesive and modern. The muted blue-gray tones are easy on the eyes and provide a unified visual identity across the showcase.
- **Concise subtitle text**: The explanatory text "Raised: light top/left, dark bottom/right. Sunken: inverted." is well-written — concise, technically accurate, and uses plain language. It communicates the concept efficiently (though it needs better visibility).

---

### Overall Score: 3/10

The Bevel Borders screen communicates a correct and useful concept (3D bevel border effects at various widths) but is severely undermined by its execution. The most critical failure is that the bevel effect itself is nearly invisible at smaller widths due to insufficient contrast between the highlight, shadow, and face colors — the screen fails to showcase its own feature. The flat visual hierarchy, low-contrast text, color-only distinctions, and dark-on-dark palette compound to create a screen that requires significant effort to parse. The progressive width demonstration and correct bevel rendering are solid foundations, but the screen needs substantially brighter bevel edge colors, a clearer typographic hierarchy, non-color fallback indicators, and better use of contrast throughout to serve its purpose as an effective showcase.
