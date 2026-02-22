# Layout QA: pagination

**Screenshots analyzed:** 1

## Issues Found

### 1. Left arrow button has inconsistent styling/border compared to page buttons
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** In the "Basic Pagination" row, the left arrow (`<`) button has a distinct light gray/white border and different background styling compared to all other pagination buttons (Page 1-5 and the right arrow). The other two pagination groups (Difficulty Selector, Option Selector) have their left arrow buttons styled consistently with the teal fill, but the first group's left arrow appears with a lighter, outlined style. This looks like a focus ring or default-state rendering issue where the button's container corners and fill do not match its siblings.
**Suggested fix:** Ensure the left arrow button in the Basic Pagination row uses the same container style (background fill, border radius, border color) as the right arrow button and page buttons. If this is a focus indicator, the focus ring should overlay consistently without changing the button's base appearance.

### 2. Pagination button rows not aligned to header bar width
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** Each section header bar (e.g., "Basic Pagination", "Difficulty Selector", "Option Selector") spans nearly the full width of the main container, but the button rows beneath them only extend about halfway across, leaving a large empty space to the right. This creates a visual imbalance between the full-width headers and the left-hugging button groups.
**Suggested fix:** If the buttons are intentionally left-aligned, this is acceptable. Otherwise, consider centering the button rows beneath their headers or giving the button group a container that visually connects it to the header width.
