# Layout QA: radio_buttons

**Screenshots analyzed:** 1

## Issues Found

### 1. Red debug/wireframe borders visible on all three radio group containers
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** All three radio group containers ("Favorite Fruit", "Size", "Color") display thin red/pink/magenta border outlines that appear to be debug wireframe borders rather than intentional design. These borders are visible around each card container and create a double-border effect with the outer rounded container.
**Suggested fix:** Remove the debug border rendering. If these are intentional borders, they should match the design system's border color rather than using a bright red/pink diagnostic color.

### 2. Focus rectangle on "Apple" option overflows its radio group container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Favorite Fruit" radio group (left column), the "Apple" option has a large white focus/selection rectangle that extends to the full width of the panel and pushes against or past the container's left and right inner edges. The focus outline is visually much wider than the other radio button rows (Orange, Watermelon), creating an inconsistent appearance and potentially overflowing the container's inner padding bounds.
**Suggested fix:** Constrain the focus rectangle width to the radio group's content area. Clamp the focus indicator's width to the container's inner padding bounds so it matches the width of the other option rows.

### 3. Radio button filled indicators render as partial shapes
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The filled radio button indicators for the selected options (Orange in Fruit, Small in Size, Blue in Color) appear as half-circle or partial-fill shapes rather than complete filled circles. This suggests the filled indicator is either being clipped by its container or rendered with incorrect geometry. The unfilled radio indicators also appear as oblong/capsule shapes rather than circles.
**Suggested fix:** Ensure radio button indicators render as complete circles. Verify the filled state uses a full circular fill rather than a partial or clipped shape. Check that the indicator container dimensions are square and not stretched.
