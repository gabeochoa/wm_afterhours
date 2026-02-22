# Layout QA: scroll_view

**Screenshots analyzed:** 5

## Issues Found

### 1. Vertical scroll container clips bottom item at container border
**Type:** overflow
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png
**Detail:** The last visible item in the vertical scroll list (e.g., "Item 5" in the initial state, "Item 11" after scrolling) is clipped at the container's bottom border. Roughly half of the item row is visible, with the text cut off mid-row. The container's rounded bottom corners cut into the item's rectangular background, creating a jagged visual. When scrolled to the bottom (Item 20), all items fit cleanly, confirming this is a clipping issue at intermediate scroll positions.
**Suggested fix:** Add bottom padding inside the scroll container, or snap the viewport height to a multiple of item height plus spacing so items are either fully visible or barely peek in to invite scrolling.

### 2. Status text and scrollbar sit outside container borders
**Type:** container_bounds
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png, scroll_view_scrolled_to_bottom.png, scroll_view_horiz_scrolled.png
**Detail:** The "Item X of 20 (X%)" label for the vertical scroll and the "Item 1 of 15 (0%)" label plus the horizontal scrollbar for the horizontal scroll are positioned below and outside their respective container borders. These elements appear orphaned from the containers they describe, floating in the gap between the container bottom edge and the page content area.
**Suggested fix:** Either move these status indicators inside the container (below the scroll viewport but within the border), or visually associate them by placing them in a dedicated footer area that connects to the container.

### 3. Horizontal scroll item "5" bleeds past container's right rounded corner
**Type:** corners
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png, scroll_view_scrolled_to_bottom.png
**Detail:** In the Horizontal Scroll section, the rightmost visible item (labeled "5") extends to the very edge of the rounded-corner container. The square corners of the item tile visually bleed past the container's border radius on the right side, breaking the rounded-corner appearance.
**Suggested fix:** Apply overflow clipping to the scroll container so child items are masked by the rounded corners, or add inner padding so items don't touch the container edge.
