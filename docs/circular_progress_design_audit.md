# Design Audit: Circular Progress Indicators

**Screen:** `circular_progress` — Game UI Toolkit Showcase
**Audited Against:** Apple Human Interface Guidelines (1987)
**Date:** 2026-02-07

---

## Critical Issues (Fix Immediately)

### 1. Color-Only Distinction Between Variants (HIG 3.1 — Color Fundamentals)
Each progress indicator variant (Basic, Animated, Thick Ring, Thin Ring) is distinguished **solely by color** — cyan, purple, green, orange. There is no redundant shape, pattern, or position-based coding. A colorblind user cannot tell the variants apart by their ring appearance alone.
→ **Fix**: Add pattern differentiation (dashed, dotted, segmented rings), vary stroke cap styles, or add small iconographic badges to each variant so they remain distinguishable without color.

### 2. Excessive Color Palette With No Semantic Meaning (HIG 3.1 — Limited Palette)
The screen uses at least 8 distinct hues: cyan, purple, green, orange, teal, gray-purple, blue, and pink/magenta. The HIG recommends 4–7 colors maximum for coding, and each color should carry consistent meaning. Here, colors appear decorative and arbitrary — cyan in one row does not mean the same thing as teal in the next.
→ **Fix**: Reduce to a 3–4 color palette with clear semantic roles (e.g., one accent color for all progress indicators, a secondary color for labels, neutral tones for chrome). If each variant must have a unique color, keep the same palette across both rows.

### 3. Low Contrast Subtitle and Description Text (HIG 3.2 — Text Contrast)
The subtitle "Radial progress visualization" and card descriptions ("Static value", "Live updating", "Thick ring", "Thin ring") render in a dim gray against the dark charcoal background. Estimated contrast ratio is well below WCAG AA minimums (~2.5:1). These labels are functionally important but nearly invisible.
→ **Fix**: Increase the lightness of secondary text to at least `#A0A0A0` or higher against the dark background, targeting a minimum 4.5:1 contrast ratio for body text.

### 4. Percentage Labels Rely on Color Alone for Association (HIG 3.1, 8.1 — Accessibility)
The percentage values ("75%", "89%", "42%", "88%") are colored to match their respective rings, which is the **only** visual link between the number and the indicator. If color is lost (grayscale, colorblindness), the association breaks.
→ **Fix**: Position percentage labels inside or directly adjacent to their ring, or connect them with a leader line. The spatial relationship should make the association obvious even without color.

---

## Major Issues (Fix Soon)

### 5. Inconsistent Color Mapping Between Rows (HIG 1.4 — Consistency)
The top row uses cyan, purple, green, and orange. The bottom "Size Variations" row uses completely different colors: teal, gray-purple, blue, orange, and pink. There is no consistent color language — the same "type" of indicator appears in different colors depending on which row it's in. This violates the principle that the same concept should look the same everywhere.
→ **Fix**: Use a single accent color for all size-variation rings (or reuse the top-row color scheme) so that color changes only when the *variant type* changes, not the size.

### 6. Monospace / Pixel Font Reduces Readability (HIG 1.10 — Aesthetic Integrity)
The title, subtitle, card labels, and percentage values all use a monospace or pixel-art style typeface. While thematically appropriate for a game UI, the low x-height and uniform character width make body text and small labels harder to scan. Aesthetic choices should support understanding, not fight it.
→ **Fix**: Reserve the pixel/display font for the main title. Use a proportional sans-serif for card labels, descriptions, and percentage values to improve legibility at small sizes.

### 7. Inconsistent Information Density Across Size-Variation Cards (HIG 1.4, 1.10 — Consistency, Aesthetics)
In the bottom row, the 28px and 40px cards show only the size label. The 52px card adds "75%", the 64px card shows "90%", and the 80px card shows "100%". This inconsistency makes it unclear whether the missing percentages are a design choice, a data gap, or a rendering issue. Users expect parallel structures to contain parallel information.
→ **Fix**: Show the same metadata (size label + percentage value) on every size-variation card, or explicitly omit percentages from all of them if they are not relevant to the size comparison.

### 8. Flat Visual Hierarchy Within Cards (HIG 1.10 — Aesthetic Integrity, Visual Hierarchy)
Inside each top-row card, the variant title, the progress ring, the percentage value, and the description text all compete for attention at roughly the same visual weight. There is no clear reading order or focal point. The HIG states that "different things should look different" and the most important element should stand out.
→ **Fix**: Establish a clear hierarchy: make the progress ring the dominant element (largest, most saturated), followed by the percentage (medium weight), then the variant title (smaller, less prominent), and the description (smallest, most subdued).

### 9. Uneven Card Layout — 4 Top, 5 Bottom (HIG 1.9 — Perceived Stability)
The top row contains 4 equally-sized cards; the bottom row contains 5 smaller cards. This asymmetry creates visual imbalance. The two rows feel like unrelated sections rather than parts of a coherent showcase. The different card sizing and count undermines the feeling of a stable, organized grid.
→ **Fix**: Align both rows to the same grid. Either use 4 columns in both rows (combining or dropping a size variant), or use 5 columns in both rows. Alternatively, make the bottom-row cards span the same total width as the top row for visual alignment.

### 10. "px" Unit Labels Use Developer Jargon (HIG 7.1 — Plain Language)
The size variation labels read "28px", "40px", "52px", "64px", "80px". The "px" suffix is programmer terminology. A non-technical user or designer reviewing this showcase may not understand what "px" means, and it mixes implementation detail into the presentation layer.
→ **Fix**: Use plain language labels like "Extra Small", "Small", "Medium", "Large", "Extra Large" — or if pixel values are needed for developer reference, present them as secondary metadata below a human-readable label.

### 11. Center-Placed Percentage in "Thick Ring" Has Poor Legibility (HIG 3.2 — Contrast)
The "42%" label placed inside the thick ring sits against a mixed green-and-gray background. The text color is similar to the background track color, making it difficult to read at smaller sizes. Text overlaid on multi-colored backgrounds needs careful contrast management.
→ **Fix**: Give the center label a solid background knockout, use a high-contrast text color (pure white), or move the percentage outside the ring to a consistent position.

---

## Minor Issues (Consider Fixing)

### 12. Decorative Card Borders Add Visual Noise (HIG 1.10 — Aesthetic Integrity)
Each card has a subtle beveled/raised border treatment that creates a 3D card-on-card look. While adding depth, these borders compete with the progress rings for attention and increase visual complexity without adding functional information. The HIG warns against decoration that distracts from function.
→ **Fix**: Simplify card borders to a single 1px stroke or rely on background color difference alone. Reserve visual embellishment for the interactive/data elements (the rings themselves).

### 13. No Interaction Affordances or State Indicators (HIG 1.7, 1.2 — Feedback, Direct Manipulation)
None of the cards or indicators show hover states, focus states, active states, or cursor changes. It is unclear whether any element is interactive. Users rely on visual cues to know what they can click, drag, or inspect.
→ **Fix**: If cards are interactive, add a hover highlight, cursor change, and focus ring. If they are purely display, ensure the showcase makes this clear (e.g., "View only" label or removing button-like styling from cards).

### 14. Title Typography Competes With Section Headers (HIG 1.10 — Visual Hierarchy)
The main title "Circular Progress Indicators" uses a large bold display font, but "Size Variations" below uses a similar style at nearly the same size. Two near-equal headings dilute the page hierarchy. The user's eye doesn't know which is the page title vs. a section label.
→ **Fix**: Reduce the "Size Variations" heading to a smaller, lighter weight that clearly reads as a subsection heading beneath the main title.

### 15. Progress Track Colors Are Inconsistent (HIG 1.4 — Consistency)
The unfilled portion of each ring (the "track") varies in color and opacity across the four variants. The Basic ring has a dark teal track, the Animated ring has a dark purple track, the Thick Ring has a gray track, and the Thin Ring has no visible track at all. This inconsistency makes it harder to compare progress values at a glance.
→ **Fix**: Use a single, consistent track color/opacity across all variants (e.g., 20% white or a uniform dark gray) so the filled portion is the only variable.

### 16. Misaligned Content Across Top-Row Cards (HIG 1.10, 1.9 — Aesthetics, Stability)
The vertical positioning of elements within the top-row cards is not perfectly consistent. The variant titles, ring centers, and description labels sit at slightly different vertical offsets across the four cards, likely due to varying text lengths and ring sizes. This micro-misalignment creates a subtly unpolished feel.
→ **Fix**: Enforce a fixed vertical layout grid within each card: title at a fixed Y offset, ring center at a fixed Y offset, percentage at a fixed Y offset, and description at a fixed Y offset — regardless of content length.

### 17. No Contextual Help or Legend (HIG 1.3 — See-and-Point)
The showcase displays four variants and five sizes but provides no guidance on when to use each one. A new user seeing this screen cannot determine the purpose of "Animated" vs. "Basic" or when "Thick Ring" is preferable to "Thin Ring". The HIG emphasizes that users should be able to discover meaning by looking, not guessing.
→ **Fix**: Add brief usage guidance beneath each variant (e.g., "Use for long-running tasks" under Animated, "Use for at-a-glance status" under Thick Ring) or provide a legend/tooltip system.

---

## Strengths

- **Clear conceptual grouping**: Variants in one row, sizes in another — the two-section layout communicates the organizational intent well.
- **Percentage labels present**: Showing the numeric value alongside the visual indicator is good practice and provides an accessible text alternative to the ring graphic.
- **Dark theme execution**: The overall dark palette is well-suited for a game UI toolkit and the card containers create clear visual regions.
- **Consistent card structure**: Within the top row, each card follows the same title → ring → percentage → description layout, establishing a predictable pattern.
- **Size progression is intuitive**: The bottom row shows a clear left-to-right size progression that reads naturally.

---

## Overall Score: 4/10

The screen demonstrates a solid structural concept — grouping variants and sizes into clearly separated sections — but falls short on several fundamental HIG principles. The most pressing issues are accessibility failures: color-only distinction between variants, low-contrast text, and an excessive rainbow palette with no semantic consistency. Typography choices prioritize theme over readability, and inconsistent information density across cards undermines the parallel structure the layout establishes. Addressing the critical contrast and color-coding issues would substantially improve the screen's usability and bring it closer to HIG compliance.
