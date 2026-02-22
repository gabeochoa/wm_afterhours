# Layout QA: file_tree

**Screenshots analyzed:** 8

## Issues Found

### 1. Status bar text at bottom is clipped by container edge
**Type:** overflow
**Screenshot:** file_tree_initial.png, file_tree_after_home.png, file_tree_after_refresh.png, file_tree_after_up.png, file_tree_navigated.png, file_tree_item_activated.png, file_tree_final.png
**Detail:** The bottom status bar text (showing "No selection" or the current path like "/Users/gabeochoa/p/Fonts") is positioned at the very bottom edge of the file tree container. The text appears to sit on or very close to the container's bottom border, with the bottom portion of the text potentially clipped. In screenshots showing longer paths (e.g., "/Users/gabeochoa/p/Fonts/Radiance"), the text is partially cut off at the right.
**Suggested fix:** Add bottom padding within the container to give the status bar text breathing room, and ensure long paths are truncated with ellipsis or horizontally scrollable rather than being clipped.

### 2. File list can potentially overflow container without scrollbar indication
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png
**Detail:** When the "Fonts/" directory is expanded, the file list grows significantly (showing many .ttf and .otf files). The list appears to extend to the very bottom of the container with the last visible entries close to the status bar. There is no visible scrollbar or scroll indicator to show that more content may exist below the visible area.
**Suggested fix:** Add a visible scrollbar or scroll indicator when the file list exceeds the container height, so users know there is more content to scroll through.

### 3. Highlight selection bar extends full width without padding
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png, file_tree_final.png
**Detail:** The blue selection highlight bar extends to the full width of the container, reaching the left and right edges. While functional, this causes the highlight to touch the container borders directly with no inset padding, which looks slightly unpolished compared to the text content which has left padding.
**Suggested fix:** Inset the selection highlight by a few pixels on the left and right so it doesn't touch the container borders, or ensure it matches the text's horizontal padding.
