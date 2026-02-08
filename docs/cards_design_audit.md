# Design Audit: Cards & Panels Gallery

**Audited Against:** Apple Human Interface Guidelines (1987)
**Screen:** Cards & Panels Gallery — game UI toolkit showcase
**Date:** 2026-02-07

---

## Critical Issues (Fix Immediately)

### 1. Extremely Low Contrast on Section Labels (HIG 3.2, 8.1)
**Violation:** The "Styles:" and "Nested:" labels use small, dark text rendered on dark brownish chip backgrounds. The text is nearly illegible at normal viewing distance. The "Theme Colors:" label suffers the same problem.
**Recommendation:** Increase label font size and ensure a minimum 4.5:1 contrast ratio between label text and its background. Use a lighter background or brighter text color for label chips.

### 2. Semi-Transparent Card Description Text Is Unreadable (HIG 3.2, 7.1, 8.1)
**Violation:** The description text beneath "Semi-Transparent" — "This panel uses opacity to create a translucent overlay effect." — is rendered in a very small, low-contrast gray on a semi-transparent gray panel. It fails basic readability requirements. Text and thin lines must have sufficient contrast to be easily visible (HIG 3.2).
**Recommendation:** Increase the font size of the description text, darken it significantly, or add a solid background behind it to guarantee legibility.

### 3. Color Is the Only Differentiator for Theme Color Swatches (HIG 3.1)
**Violation:** The "Theme Colors" row at the bottom differentiates Primary, Secondary, Accent, Surface, and Background solely by fill color. No shape, pattern, icon, or other redundant cue is provided. This fails "Color coding is redundant with shape/position/pattern" and renders the section meaningless for colorblind users or on monochrome displays.
**Recommendation:** Add pattern fills, border styles, or icons within each swatch to provide a non-color differentiator. Alternatively, display the hex/RGB value inside each swatch.

### 4. "Surface" Card Has No Visible Boundary (HIG 1.10, 3.2)
**Violation:** The "Surface" card variant appears as plain text with no border, shadow, or background fill that distinguishes it from the surrounding container. It violates "Different things look different" and "Outlines provide clear edge definition." A user cannot tell where the card begins and ends.
**Recommendation:** Add a subtle border, shadow, or distinguishable background tint so the Surface card is visually identifiable as a card component.

---

## Major Issues (Fix Soon)

### 5. Inconsistent Label Placement and Treatment (HIG 1.4)
**Violation:** Section labels use inconsistent presentation. "Styles:" is rendered as a small chip/badge positioned at the upper-left edge of its container. "Nested:" is placed as a floating chip above its card group, outside the white container. "Theme Colors:" is inline at the far left within its container. The three labels use three different positioning strategies, violating internal consistency.
**Recommendation:** Standardize label placement — pick one strategy (e.g., always top-left inside the container, left-aligned with consistent padding) and apply it uniformly to all sections.

### 6. Inconsistent Card Sizing Within the Styles Row (HIG 1.4, 1.10)
**Violation:** The four card variants in the "Styles:" row — Surface, Primary, Sharp, Custom — are all different sizes. "Surface" is the widest (text-only, no tight bounding), "Primary" is compact, "Sharp" is taller with larger text, and "Custom" is somewhere in between. This inconsistency makes them look unrelated rather than variants of the same component.
**Recommendation:** Normalize card dimensions across all variants in the showcase row. Use a uniform width and height so users can compare styles directly without size confounding the comparison.

### 7. Flat Visual Hierarchy — All Sections Have Equal Weight (HIG 1.10)
**Violation:** The title "Cards & Panels Gallery" is appropriately prominent, but below it, every section (Styles, Nested, Semi-Transparent, Theme Colors) sits at the same visual priority level. Nothing guides the eye through a reading order. The HIG warns against "Flat visual hierarchy — everything same prominence."
**Recommendation:** Establish a clear hierarchy: use larger or bolder type for section headings, add more whitespace between major sections, and consider progressive disclosure or visual weight differences to guide scanning.

### 8. Monospace/Pixel Font Reduces Readability (HIG 1.10, 7.1)
**Violation:** The entire screen uses a monospace or pixel-style typeface. While this may be intentional for a game UI aesthetic, it significantly reduces readability — especially for the smaller descriptive text. The HIG calls for messages that are "concise and simple" and visually clear displays; a monospace font at small sizes works against both.
**Recommendation:** Consider using a proportional typeface for body/description text, reserving the pixel/monospace font for headings and labels where the stylistic choice is deliberate and readability is less impacted.

### 9. Insufficient Edge Definition Between Nested Layers (HIG 3.2)
**Violation:** The nested cards ("Item 1", "Item 2", "Item 3") sit inside a white container, which itself sits on the beige background. The items' borders and the parent container's borders are both thin and subtle, making it hard to discern the nesting relationship. The HIG requires "Outlines provide clear edge definition."
**Recommendation:** Increase border weight or contrast on either the parent or child cards. Use indentation, shadows, or a clear background color step to visually communicate the containment hierarchy.

### 10. Arbitrary Color Choices for Card Variants Lack Semantic Meaning (HIG 3.1, 1.1)
**Violation:** "Sharp" uses a muted pink, "Custom" uses sage green, "Primary" uses brown, and the nested items use brown/green/pink. These colors feel arbitrary — they don't map to any obvious semantic meaning or real-world metaphor. The HIG advises using a "limited palette" and ensuring color supports understanding rather than serving as decoration.
**Recommendation:** Choose card colors that communicate purpose (e.g., success = green, warning = amber, error = red) or remove the varied colors entirely if the showcase is about shape/border style, not color.

---

## Minor Issues (Consider Fixing)

### 11. Inconsistent Border Radius Across Card Variants (HIG 1.4)
**Violation:** The "Sharp" card appears to use a square corner (0 radius), while "Surface," "Primary," and "Custom" all use rounded corners. The outer containers and nested cards also use rounded corners. If "Sharp" is intentionally demonstrating a zero-radius variant, the showcase should make this clearer — currently it reads as inconsistency rather than intentional demonstration.
**Recommendation:** Add a visible annotation or label explicitly calling out "sharp corners" vs. "rounded corners" to make the design difference intentional and educational rather than confusing.

### 12. Outer Background Creates a Monotonous Visual Field (HIG 1.10)
**Violation:** The entire screen is wrapped in a uniformly warm beige/tan background with only a slightly darker outer border. This creates a flat, monotonous visual field with no differentiation between chrome (UI framework) and content (the cards being showcased). The HIG states that "Graphics support understanding, not just decoration."
**Recommendation:** Differentiate the showcase backdrop from the outer frame — for example, use a neutral gray or contrasting color for the outer shell while keeping the warm palette for the card content area.

### 13. Text Alignment Inconsistency Within Cards (HIG 1.4)
**Violation:** Card titles appear center-aligned in some cards ("Sharp," "Custom") but left-aligned or ambiguously placed in others ("Nested Cards" is left-aligned, "Semi-Transparent" is left-aligned). The nested items appear center-aligned. Mixing alignment strategies within the same component type creates visual noise.
**Recommendation:** Standardize text alignment across all card variants — either always center or always left-align card titles and content.

### 14. Theme Colors Row Lacks Clear Visual Grouping (HIG 1.10, 1.4)
**Violation:** The "Theme Colors" row at the bottom displays five color swatches on a white strip. "Surface" and "Background" swatches appear borderless or nearly invisible against the white container, while "Primary," "Secondary," and "Accent" have colored fills. This makes the row look incomplete — as if some swatches failed to render.
**Recommendation:** Add visible borders to all swatches including Surface and Background so they are clearly distinguishable as distinct items even when their fill color is close to the container background.

### 15. No Interactive Affordances Shown (HIG 1.2, 1.7)
**Violation:** None of the cards or panels show any hover, press, or focus state. For a showcase of UI components, this omits critical information about the component's interactive behavior. The HIG emphasizes "Immediate feedback confirms every action" and direct manipulation principles.
**Recommendation:** Include additional showcase rows or annotations showing hover, active, focused, and disabled states for each card variant.

### 16. Spacing Between Sections Is Uneven (HIG 1.4, 1.9)
**Violation:** The vertical gap between the "Styles:" section and the "Nested:"/"Semi-Transparent" section appears larger than the gap between that middle row and the "Theme Colors" row at the bottom. Inconsistent spacing disrupts perceived stability and visual rhythm.
**Recommendation:** Use a consistent vertical spacing value (e.g., 24px or 32px) between all major sections to create a reliable visual rhythm.

### 17. "Nested:" Label Floats Outside Its Container (HIG 1.9, 1.10)
**Violation:** The "Nested:" label chip sits above and to the left of the white card container, overlapping the beige background. This breaks the visual containment — it looks like a UI glitch rather than a deliberate label. All other labels sit within their respective containers.
**Recommendation:** Move the "Nested:" label inside the white container, aligning it with the "Nested Cards" title, or adopt a consistent external-label strategy for all sections.

---

## Strengths

- **Clear title hierarchy:** The "Cards & Panels Gallery" title is prominently displayed in a distinct banner, immediately communicating the screen's purpose.
- **Warm, cohesive color palette:** The beige/brown/cream palette creates a unified visual identity that feels intentional and appropriate for a cozy game UI.
- **Logical content organization:** The screen is divided into logical sections (Styles, Nested, Semi-Transparent, Theme Colors) that demonstrate different aspects of the card system.
- **Rounded containers provide softness:** The use of large-radius rounded rectangles for section containers gives the UI a friendly, approachable feel consistent with game UI aesthetics.
- **Showcase concept is sound:** The idea of displaying variant card styles side-by-side for comparison is a good pattern for a component gallery.

---

## Overall Score: 4/10

The Cards & Panels Gallery has a warm, cohesive palette and a sound organizational concept, but it is significantly undermined by readability failures (low-contrast labels and description text), inconsistent spacing and alignment, lack of accessibility considerations (color-only differentiation), and internal inconsistency in how labels, card sizes, and borders are treated across sections. The critical contrast issues should be addressed immediately, as several text elements are effectively unreadable. The inconsistencies across card sizing, label placement, and border treatment — while less urgent — erode the showcase's ability to communicate a unified component system.
