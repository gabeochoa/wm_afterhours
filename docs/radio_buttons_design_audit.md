# Design Audit: Radio Button Groups

**Screen:** `radio_buttons` — Game UI Toolkit Showcase
**Audited Against:** Apple Human Interface Guidelines (1987 Classic)
**Date:** 2026-02-07

---

## Critical Issues (Fix Immediately)

### 1. Unselected Radio Buttons Are Nearly Invisible (HIG 3.2, 8.1)
The unselected radio button circles are rendered as thin, dark outlines on a near-black background. They are extremely difficult to see, violating the guideline that small objects must use high-contrast colors and that the interface must work on monochrome displays. Users cannot confidently identify which options are available without straining.

**Recommendation:** Use a light stroke (white or light gray, ~2px) for unselected radio button circles so they are clearly visible against the dark background.

### 2. Red Borders Imply Error State (HIG 1.4, 1.10)
The red/pink borders around each group card are a severe consistency violation. In virtually all UI conventions, red borders signal validation errors or critical problems. Using them as purely decorative group dividers misleads users into thinking something is wrong with each group.

**Recommendation:** Replace red borders with neutral colors (dark gray, muted teal, or subtle light outlines) that convey grouping without implying errors.

### 3. Selected State Relies Solely on Color (HIG 3.1, 8.1)
The selected radio option is indicated only by a change in text color (to red/crimson) and a filled radio dot. There is no secondary cue — no bold weight, no background highlight, no checkmark, no underline. Colorblind users (particularly those with red-green deficiency) will struggle to distinguish selected from unselected items.

**Recommendation:** Add a redundant non-color indicator: bold text weight, a background highlight, a checkmark icon, or a visible outline/underline on the selected option.

### 4. Insufficient Contrast Between Card Background and Page Background (HIG 1.10, 3.2)
The group cards use a slightly lighter dark shade against the near-black page background. The difference is barely perceptible, making the cards blend into the background. The HIG states that different things must look different.

**Recommendation:** Increase the contrast between card surfaces and the page background. Use a distinctly lighter dark tone (e.g., `#2a2a3a`) for cards or add a visible border in a neutral color.

---

## Major Issues (Fix Soon)

### 5. Status Text Lacks Delimiters (HIG 1.7, 7.1)
The bottom status bar reads `Selected: Orange Small Blue` with no commas, pipes, or other separators between values. This is ambiguous — a user could read it as "Orange Small Blue" (one item) rather than three separate selections. The HIG requires messages to be clear and unambiguous.

**Recommendation:** Use explicit delimiters: `Selected: Orange, Small, Blue` or label each value: `Fruit: Orange | Size: Small | Color: Blue`.

### 6. Radio Button Hit Targets Are Too Small (HIG 2.7, 5.2)
The radio button circles appear to be approximately 12-14px in diameter. This is below comfortable click/tap target sizes. Users with motor impairments or those using a gamepad will struggle to accurately select options.

**Recommendation:** Increase radio button circle diameter to at least 20-24px, and ensure the entire row (label + radio button) is clickable as a single hit target.

### 7. Group Header Text Contrast Against Teal Background (HIG 3.2)
The group header labels ("Favorite Fruit", "Size", "Color") appear to use dark text on a teal/cyan background. The contrast ratio between dark text and a medium-toned teal is marginal, particularly at the font sizes used. Thin lines and text require high contrast per the HIG.

**Recommendation:** Use white or very light text on the teal header backgrounds, or lighten the teal significantly to ensure the text passes WCAG AA contrast ratios.

### 8. Inconsistent Group Card Widths (HIG 1.4, 1.10)
The three group cards are not equally sized — "Favorite Fruit" and "Color" appear wider than "Size," despite all groups serving the same structural purpose. This breaks visual consistency and creates an unbalanced layout.

**Recommendation:** Set all group cards to the same fixed width, or use a uniform flex distribution so they share space equally.

### 9. No Visible Focus or Hover States (HIG 1.2, 2.7, 8.3)
There are no visible hover or keyboard focus indicators on any radio button option. The HIG requires that visual feedback accompanies every action and that the interface supports keyboard navigation. Without focus states, keyboard and gamepad users have no idea which option they are about to select.

**Recommendation:** Add a visible focus ring or highlight (e.g., a teal or white outline) around the currently focused radio option, and a subtle hover highlight on mouse-over.

### 10. Selected Text Color Matches Error Border Color (HIG 1.4, 3.1)
The selected option text uses a red/crimson color that is nearly identical to the red group borders. This creates a confusing association — selected items look like they are "errored" because they match the implied-error border color. Two semantically different states share the same visual language.

**Recommendation:** Use a distinct, non-red accent color (teal, bright blue, or white-bold) for selected text, clearly separating it from any error signaling.

---

## Minor Issues (Consider Fixing)

### 11. Monospace/Pixel Font Reduces Readability (HIG 1.10, 7.1)
The typeface appears to be a monospace or pixel-style font throughout. While thematic for a game UI, monospace fonts are harder to read for body text and option labels, especially at smaller sizes. The HIG values readability and plain language.

**Recommendation:** Consider using a proportional sans-serif font for option labels and status text, reserving the pixel/monospace font for headers or decorative elements only.

### 12. "Watermelon" Appears Dimmer Than Other Unselected Labels (HIG 1.4, 3.2)
In the "Favorite Fruit" group, "Watermelon" appears to render in a slightly different (dimmer) shade than "Apple." If both are unselected, they should have identical visual treatment. This inconsistency suggests a rendering bug or unintentional style variation.

**Recommendation:** Ensure all unselected option labels use the exact same color and opacity.

### 13. Title Bar Style Inconsistent with Group Headers (HIG 1.4)
The top title "Radio Button Groups" has a teal background bar spanning the full width, while group headers ("Favorite Fruit", "Size", "Color") have smaller teal badges that vary in width. Two different header styles at different levels of hierarchy create visual inconsistency.

**Recommendation:** Establish a clear header hierarchy: the page title should be visually distinct from group headers, but group headers should be internally consistent (same width, padding, and style).

### 14. No Visual Grouping Metaphor Beyond Borders (HIG 1.1)
The groups are delineated only by rectangular borders with rounded corners. There is no metaphor reinforcing why these are grouped — no fieldset legend styling, no indentation or card elevation, no shadow to suggest depth. The HIG encourages metaphors that help users understand relationships.

**Recommendation:** Add subtle depth cues (a light drop shadow or inner shadow) to the group cards, or use a more distinct background fill to make them feel like tangible containers.

### 15. Status Bar Lacks Visual Hierarchy (HIG 1.10)
The bottom status bar (`Selected: Orange Small Blue`) uses the same font size and weight as the option labels above. It blends into the layout rather than standing out as a summary or feedback element. The HIG says feedback should be visually clear.

**Recommendation:** Differentiate the status bar with a slightly different font size, weight (bold the label "Selected:"), or background shade to distinguish it from the interactive content above.

### 16. Excessive Vertical Padding Within Group Cards (HIG 1.10)
The group cards have large amounts of empty space below the last option, particularly in "Size" (3 options) compared to "Color" (4 options). This wasted space makes the cards feel unfinished and loosely structured.

**Recommendation:** Tighten the internal padding so cards wrap their content more closely, or equalize the number of visible rows across cards.

### 17. No Indication of Interactivity on Labels (HIG 1.2, 5.1)
The option labels ("Apple", "Orange", etc.) appear as plain text with no affordance signaling they are clickable. Users may assume only the small radio circle is interactive. The HIG encourages direct manipulation with clear affordances.

**Recommendation:** Add a subtle hover/active state to the entire label row, or use a hand/pointer cursor on hover to signal interactivity.

---

## Strengths

- **Correct radio button semantics:** Each group enforces mutual exclusivity with exactly one option selected per group, matching HIG 2.7 radio button guidelines.
- **Groups are visually separated:** Despite the border color issues, the three groups are clearly distinct containers, making it easy to understand there are three independent choices.
- **Real-time feedback:** The status bar at the bottom provides immediate feedback on current selections, satisfying HIG 1.7 (Feedback and Dialog).
- **Descriptive group labels:** Each group has a clear, descriptive header ("Favorite Fruit", "Size", "Color") using plain language, consistent with HIG 7.1.
- **Logical layout:** The horizontal arrangement of groups reads left-to-right naturally, and the selection summary is placed below the controls in a predictable location.

---

## Overall Score: 3.5 / 10

The radio button groups screen demonstrates correct radio button semantics and logical grouping, but suffers from severe contrast and visibility issues that undermine basic usability. The near-invisible unselected radio buttons, misleading red error-style borders, color-only selected state indication, and lack of focus/hover states are critical barriers. The dark-on-dark color scheme, while atmospheric for a game context, sacrifices readability and accessibility at nearly every level. Addressing the contrast, border color, and redundant selection indicators would dramatically improve the design's alignment with Apple HIG principles.
