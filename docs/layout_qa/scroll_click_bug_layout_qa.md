# Layout QA: scroll_click_bug

**Screenshots analyzed:** 4

## Issues Found

### 1. Bottom button clipped by scroll container boundary
**Type:** overflow
**Screenshot:** idle_720p.png, scroll_click_before.png
**Detail:** In the initial/non-scrolled view, "Button 6" at the bottom of the scroll container is partially cut off by the container's bottom edge. Only the top portion of the button is visible -- roughly the upper third -- with the rest clipped by the container's boundary. The scroll container's height does not evenly accommodate the visible buttons, leaving the last one partially overflowing.
**Suggested fix:** Adjust the scroll container height to either fully show or fully hide the last button at the boundary. Alternatively, add a visible scrollbar or scroll indicator to communicate that additional content exists below the visible area.

### 2. Top and bottom buttons clipped after scrolling
**Type:** overflow
**Screenshot:** scroll_click_after_click.png, scroll_click_after_scroll.png
**Detail:** After scrolling down, "Button 4" at the top of the visible area is partially clipped by the container's top edge, and "Button 10" at the bottom is similarly clipped. Both the topmost and bottommost visible buttons are partially rendered, confirming the container height creates a consistent partial-button cutoff regardless of scroll position.
**Suggested fix:** Size the scroll container so that buttons are either fully visible or fully hidden at the boundary. Consider implementing scroll snapping to align to complete button boundaries.

### 3. Button 8 renders without background fill (visual gap in list)
**Type:** container_bounds
**Screenshot:** scroll_click_after_scroll.png, scroll_click_after_click.png
**Detail:** In the scrolled state, "Button 8" appears without a blue background fill, rendering as a white/transparent row among the other blue buttons. This creates a visual gap in the otherwise uniform button list. It is unclear whether this is a styling bug or an intentional "selected/clicked" state, but it breaks the visual consistency of the button list layout.
**Suggested fix:** If this is unintentional, ensure all buttons in the list render with the same background fill. If "Button 8" represents a different state (e.g., clicked/selected), apply a distinct but still-filled style rather than removing the background entirely.
