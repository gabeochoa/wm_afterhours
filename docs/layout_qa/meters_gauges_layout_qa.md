# Layout QA: meters_gauges

**Screenshots analyzed:** 1

## Issues Found

### 1. Shield bar track extends beyond parent container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Stacked Bars (Health + Shield)" section, the shield bar (labeled "Shield: 29%") extends noticeably past the right edge of its parent container. The bar's background track overshoots the container boundary by several pixels into the dark background area, while the Health bar above stays within bounds.
**Suggested fix:** Constrain the shield bar's maximum width to respect the parent container's right padding/boundary. Ensure the stacked bar layout accounts for both bars fitting within the available container width.

### 2. Slider handle/track extends beyond the Interactive Control container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Interactive Control" section at the bottom, the white slider track/handle element extends past the right edge of the green progress bar fill area. The white track portion protrudes beyond the container's right boundary.
**Suggested fix:** Ensure the slider track is clipped to the container bounds, or adjust the container width to accommodate the full slider track length including the handle.

### 3. Outer container corners clipped by viewport on left and right edges
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main rounded container for the entire "Meters & Gauges" screen extends slightly beyond the left and right edges of the viewport at 720p. The rounded corners on the left and right sides are partially cut off by the screen edge, particularly visible in the lower half where the container's curved edges are clipped.
**Suggested fix:** Add horizontal margin or reduce the container's max-width so the full rounded corners are visible within the 720p viewport.
