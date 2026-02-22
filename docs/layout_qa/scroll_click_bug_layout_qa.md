# Layout QA: scroll_click_bug

**Screenshots analyzed:** 4

## Issues Found

### 1. Bottom button clipped by scroll container boundary
**Type:** overflow
**Screenshot:** idle_720p.png, scroll_click_before.png
**Detail:** In the initial/non-scrolled view, "Button 6" at the bottom of the scroll container is partially cut off by the container's bottom edge. Only the top portion of the button is visible — roughly the upper third — with the rest clipped by the container's boundary. The scroll container's height does not evenly accommodate the visible buttons, leaving the last one partially overflowing.
**Suggested fix:** Adjust the scroll container height to either fully show or fully hide the last button at the boundary. Alternatively, add a visible scrollbar or scroll indicator to communicate that additional content exists below the visible area.

### 2. Bottom buttons clipped after scrolling
**Type:** overflow
**Screenshot:** scroll_click_after_click.png, scroll_click_after_scroll.png
**Detail:** After scrolling down, "Button 10" at the bottom of the list is similarly clipped by the scroll container's bottom edge. The button is partially visible with its lower portion cut off, confirming the container height creates a consistent partial-button cutoff regardless of scroll position.
**Suggested fix:** Size the scroll container so that buttons are either fully visible or fully hidden at the boundary. Consider snapping the scroll position to show complete buttons.

### 3. Top button clipped after scrolling
**Type:** overflow
**Screenshot:** scroll_click_after_click.png, scroll_click_after_scroll.png
**Detail:** After scrolling down, "Button 4" at the top of the visible area is partially clipped by the container's top edge. The top portion of the button is cut off, mirroring the bottom clipping issue. Both the topmost and bottommost visible buttons are partially rendered after scrolling.
**Suggested fix:** Apply the same container height adjustment as the bottom clipping fix to ensure consistent full-button visibility, or add overflow clipping with a visual fade indicator at both the top and bottom edges to signal truncated content.
