# Layout QA: decorators

**Screenshots analyzed:** 1

## Issues Found

### 1. with_quote() first quote text overflows container right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the right column (with_quote() section), the first quote "The only way to do great work is to love what you do." extends to or past the right edge of the viewport. The text does not wrap within its container, causing the end of the quote to be clipped by the screen boundary.
**Suggested fix:** Constrain the quote container width so text wraps to multiple lines within the visible viewport. Ensure the with_quote() container has a max-width that respects screen bounds.

### 2. Corner bracket decorations extend outside panel boundaries
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** In the with_brackets() column (left), the L-shaped corner bracket decorations on each panel extend outside the panel's border edges. The colored horizontal bar accents (cyan/teal) at the top and bottom of each panel also protrude past the panel's side boundaries. While this appears intentional as a decorative style, the brackets overflow the panel's actual bounding box.
**Suggested fix:** If the bracket extensions are by design (decorative flourishes), ensure they don't clip against neighboring elements or the viewport edge. If they should be contained, clip the brackets and accent bars to the panel's bounding box.

### 3. with_grid_bg() panels have inconsistent vertical spacing and sizing
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The three panels in the with_grid_bg() center column have uneven vertical spacing and different heights. The gap between the first and second panel differs from the gap between the second and third. The small green/yellow square indicators on the left side of the panels also vary in vertical positioning.
**Suggested fix:** Normalize the vertical gap between all three grid background panels and apply consistent panel heights throughout the column.

### 4. Stray rendering artifact inside with_grid_bg() top panel
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The top panel in the with_grid_bg() column contains a small misplaced colored square/dot near the center of the panel. This appears to be a stray rendering artifact or unintended child element overlapping the otherwise empty grid background.
**Suggested fix:** Investigate whether this is an unintended child element or rendering artifact and remove it if it is not part of the design.
