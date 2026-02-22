# Layout QA: casual_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Close button (X) overlaps container top-right corner
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The red circular close button ("X") in the top-right of the settings panel straddles the container's rounded corner border. It sits partially inside and partially outside the beige panel boundary, breaking the clean rounded corner edge.
**Suggested fix:** Move the close button fully inside the container with adequate margin from the top and right edges, or position it as an intentional external element with a consistent offset from the corner.

### 2. "Sync" button text clipped by circular boundary
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green circular "Sync" button next to "Save/Load Progress" has its text label ("Sync") slightly clipped on the right side by the button's circular boundary. The "c" character is partially cut off, and the button sits close to the container's right edge.
**Suggested fix:** Increase the button diameter to fully accommodate the text label, or reduce the font size so the label fits cleanly within the circle.

### 3. "Terms and Privacy" button breaks two-column grid symmetry
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The Menu section uses a two-column layout (Notifications/Credits, Language/Support), but "Terms and Privacy" sits alone in the right column with no counterpart on the left, creating a visual gap and asymmetric bottom row.
**Suggested fix:** Center "Terms and Privacy" across both columns if it is the only item in the last row, or span it full-width.

### 4. Asymmetric left/right padding in main container
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The left side of the settings container has noticeably more internal padding than the right side. The Audio section starts well-inset from the left edge, but the right-column buttons (Credits, Support, Terms and Privacy) and the OK/Cancel/Apply buttons extend closer to the right border with less margin.
**Suggested fix:** Ensure consistent horizontal padding on both sides of the container.
