# Design Audit: SelfAlign Feature Demo

**Audited Against:** Apple Human Interface Guidelines (1987)
**Screen:** SelfAlign Feature Demo — game UI toolkit showcase
**Date:** 2026-02-07

---

## Audit Summary

The SelfAlign Feature Demo screen demonstrates a layout concept — overriding parent alignment for individual child elements. The screen uses a dark theme with colored labels arranged in a container to show FlexStart, Center, and FlexEnd alignment, plus a second row of standalone aligned elements below. While the conceptual demonstration is spatially clear, the screen has significant issues with contrast, typography, accessibility, and plain language.

---

### Critical Issues (Fix Immediately)

1. **Color as Only Distinguisher (Accessibility §8.1, Color §3.1)**: The three alignment types (left, center, right) are differentiated *only* by color — olive green, dark gold, and salmon/coral. A colorblind user cannot tell them apart. Color coding must be redundant with shape, position, iconography, or pattern.
   → **Fix**: Add distinct shapes, icons, border styles, or numbering so alignment type is identifiable without color. Even differing border patterns (solid, dashed, dotted) would help.

2. **Extremely Low Contrast on Descriptive Text (Color §3.2, Accessibility §8.1)**: The subtitle "Override parent alignment for individual elements" and the caption "Each element picks its own position" are rendered in a very small, low-opacity font against the dark charcoal background. They are nearly unreadable.
   → **Fix**: Increase text opacity to at least 70% and bump font size up by 2–4px. Aim for a contrast ratio of at least 4.5:1 per WCAG AA standards.

3. **Developer Jargon in Labels (Plain Language §7.1)**: Labels like "FlexStart (left)" and "FlexEnd (right)" use CSS flexbox terminology that end users would never understand. "SelfAlign" itself is a camelCase developer identifier. These violate the principle of using user vocabulary, not programmer vocabulary.
   → **Fix**: Replace with plain language. Use "Left-aligned," "Centered," and "Right-aligned." Rename the screen title to something like "Individual Alignment" or "Per-Element Alignment."

---

### Major Issues (Fix Soon)

4. **Non-Standard Typography Reduces Readability (Aesthetic Integrity §1.10)**: The handwritten/marker-style font used for labels and the title is stylistically charming but significantly reduces readability, especially at smaller sizes. It fails the test that "graphics support understanding, not just decoration."
   → **Fix**: Use a clean sans-serif font for all informational text. If the handwritten style is part of the game's brand, restrict it to the title only and use a legible font for labels and descriptions.

5. **Element Overflows Container Boundary (Perceived Stability §1.9)**: The "FlexEnd (right)" element visually extends past the right edge of the rounded container, breaking the visual boundary. This undermines the sense of containment and spatial stability.
   → **Fix**: Add internal padding to the container so all child elements sit fully within its borders, or expand the container to accommodate the rightmost element.

6. **Flat Visual Hierarchy — Everything Same Prominence (Aesthetic Integrity §1.10)**: The title block, the container with three demo elements, and the bottom row of elements all compete for attention at similar visual weights. There is no clear reading order or hierarchy of importance.
   → **Fix**: Differentiate with size, weight, and spacing. Make the title larger and bolder. Add more vertical spacing between the title section, the demo container, and the bottom example row. Consider using section labels or dividers.

7. **No Interactive Affordances or Feedback Indicators (Feedback §1.7)**: None of the elements visually communicate whether they are interactive (clickable, draggable, or purely static). There are no hover states, focus rings, or cursor changes apparent. Users cannot tell what they can interact with.
   → **Fix**: If elements are interactive, add visible affordances — borders, shadows, cursor changes on hover. If they are purely demonstrative, add a clear label stating "Non-interactive demo."

8. **Inconsistent Label Formatting (Consistency §1.4)**: Inside the container, labels use the format "FlexStart (left)" with parenthetical clarification, while the bottom row elements just say "Left," "Center," "Right." The same concept is described differently in two places on the same screen.
   → **Fix**: Use a single, consistent naming convention throughout. Either both sections use the verbose form or both use the short form.

9. **Container Border Too Subtle (Color §3.2)**: The rounded-rectangle container has a very low-contrast border that is barely distinguishable from the background. Edge definition is critical — "outlines provide clear edge definition" per the color guidelines.
   → **Fix**: Increase the border opacity or thickness, or add a subtle fill difference between the container interior and the outer background to make the boundary obvious.

---

### Minor Issues (Consider Fixing)

10. **No Relationship Explanation Between Sections (See-and-Point §1.3)**: The top container and the bottom row of elements are visually separate but conceptually related. There is no label, annotation, or connector explaining how they relate or what the difference between the two demonstrations is.
    → **Fix**: Add section headers like "Within a Container" and "Standalone Elements" to clarify the relationship and distinction.

11. **Caption Text Illegibly Small (Aesthetic Integrity §1.10)**: The "Each element picks its own position" caption below the bottom row is so small it may as well be invisible. It contains the most user-friendly explanation on the screen but is the hardest text to read.
    → **Fix**: Increase size and prominence. This is the clearest description of the feature and should be the most readable piece of text, not the least.

12. **Title Background Bar Feels Heavy (Aesthetic Integrity §1.10)**: The dark rectangular background behind "SelfAlign Feature Demo" creates a heavy visual block at the top of the screen. Combined with the dark theme, it flattens instead of elevating the title.
    → **Fix**: Consider removing the background bar or using a lighter fill/outline treatment to let the title breathe more within the dark environment.

13. **Inconsistent Spacing Between Demo Elements (Consistency §1.4)**: The vertical spacing between "FlexStart (left)," "Center," and "FlexEnd (right)" in the container does not appear uniform. The gaps vary, making the layout feel unstructured rather than intentionally designed.
    → **Fix**: Use consistent vertical spacing (or make the spacing differences clearly intentional with visual annotations) so the layout reads as deliberate.

14. **No Monochrome Fallback (Color §3.1)**: The design does not work in black and white. If desaturated, all three colored blocks become nearly identical medium-gray rectangles, making the demo meaningless.
    → **Fix**: Add secondary visual cues (text labels already help, but consider also varying the shape, size, or border treatment of each alignment block).

15. **Screen Lacks Context or Navigation (User Control §1.6)**: There is no back button, breadcrumb, or navigation element visible. The user has no visible way to leave this screen or understand where they are in the showcase.
    → **Fix**: Add a back/close affordance and a breadcrumb or screen index indicator so users can orient themselves within the toolkit showcase.

16. **Excessive Empty Space at Edges (Aesthetic Integrity §1.10)**: The demo content is clustered in the center of the screen with very large dark margins on all sides. This wastes space and makes the functional content feel small and isolated.
    → **Fix**: Either expand the demo content to use more of the available space, or add additional examples/annotations in the margins to make the screen more informative.

17. **Bottom Row Elements Lack Visual Grouping (Aesthetic Integrity §1.10, Consistency §1.4)**: The three bottom elements ("Left," "Center," "Right") float freely with no container, background, or visual grouping to indicate they belong together as a set, unlike the top elements which are enclosed in a container.
    → **Fix**: Add a subtle container, shared baseline, or background treatment to visually group the bottom row as a cohesive second example.

---

### Strengths

- **Spatial demonstration is intuitive**: The staggered diagonal arrangement of elements within the container visually communicates alignment positions effectively — left element at top-left, center element at center, right element at bottom-right.
- **Color coding is consistent**: The same color is used for the same alignment concept across both sections (green = left, gold = center, coral = right), maintaining internal color consistency.
- **Rounded corners and soft styling**: The overall visual treatment with rounded corners on the outer frame and container creates a cohesive, friendly aesthetic appropriate for a game UI toolkit.
- **Concept is self-demonstrating**: The layout itself demonstrates the feature being showcased — each element literally sits where its label says it should. This is a good application of the WYSIWYG principle.

---

### Overall Score: 3.5/10

The screen successfully communicates its spatial concept through self-demonstrating layout, but it suffers from critical accessibility failures (color-only coding, unreadable text), pervasive developer jargon, weak visual hierarchy, and missing interactive affordances. The most helpful text on the screen is the least readable. Fixing the contrast, language, and accessibility issues would significantly improve the design without requiring a structural overhaul.
