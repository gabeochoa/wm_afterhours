# Layout QA: cards

**Screenshots analyzed:** 1

## Issues Found

### 1. Description text overflows past the right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Nested:" row, the description text next to the "Semi-Transparent" card ("This panel uses opacity to create a translucent...") is truncated at the right edge of the screen. The text extends beyond the viewport/container boundary with the word cut off mid-word and no ellipsis or wrapping applied.
**Suggested fix:** Constrain the text element to its parent container width. Apply text wrapping or text-overflow ellipsis so the description stays within bounds. Alternatively, allow the container to expand vertically to accommodate the full text.

### 2. "Nested:" label positioned outside its row container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "Nested:" row label on the left side appears to sit outside and to the left of the card containers in that row, with its small dark background box not aligning with the row container structure used by "Styles" above. The label breaks the expected content padding.
**Suggested fix:** Align the "Nested:" label within the same container padding used by the "Styles" label above, ensuring consistent horizontal alignment of row labels.

### 3. "Styles" label vertical misalignment with card elements
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Styles" row, the "Styles" label and the "Surface" text-only card sit at a different vertical position than the taller card elements ("Primary", "Sharp", "Custom") which extend above them. This creates uneven vertical alignment within the row. Additionally, the "Surface" card variant renders with no visible border or background differentiation from the parent container, making its spatial extent ambiguous.
**Suggested fix:** Vertically center all elements in the Styles row. Consider adding a subtle border to the Surface card so its bounds are distinguishable from the background.
