# Layout QA: file_tree

**Screenshots analyzed:** 8

## Issues Found

### 1. Status bar text clipped at container bottom edge
**Type:** overflow
**Screenshot:** file_tree_initial.png, file_tree_after_home.png, file_tree_after_refresh.png, file_tree_after_up.png, file_tree_navigated.png, file_tree_item_activated.png, file_tree_final.png
**Detail:** The bottom status bar text (showing "No selection" or the current path like "/Users/gabeochoa/p/Fonts/Radiance") sits at the very bottom edge of the file tree container with minimal vertical padding. The text appears crowded against the container's bottom border. For longer paths, the text may also be clipped at the right edge.
**Suggested fix:** Add bottom padding within the container to give the status bar text breathing room. Truncate long paths with ellipsis or make them horizontally scrollable.

### 2. File list lacks visible scroll indicator when content exceeds container
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png
**Detail:** When the "Fonts/" directory is expanded, the file list grows significantly (many .ttf and .otf files). The list extends to the very bottom of the container with no visible scrollbar or scroll indicator to show that more content may exist below.
**Suggested fix:** Add a visible scrollbar or scroll indicator when the file list exceeds the container height.

### 3. Selection highlight bar extends full width without inset padding
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png, file_tree_final.png
**Detail:** The blue selection highlight bar extends to the full width of the container, touching both left and right edges with no inset. The text content has left padding but the highlight does not match this padding.
**Suggested fix:** Inset the selection highlight by a few pixels on left and right so it doesn't touch the container borders, matching the text's horizontal padding.
