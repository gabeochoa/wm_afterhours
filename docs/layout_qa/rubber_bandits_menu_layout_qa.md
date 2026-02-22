# Layout QA: rubber_bandits_menu

**Screenshots analyzed:** 1

## Issues Found

### 1. Tooltip text clipped at bottom viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green tooltip bar at the very bottom of the screen ("Change the game's settings") is positioned flush against the bottom edge of the viewport. The text's bottom portion appears slightly cropped by the screen boundary, with the bottom of the letters touching or being cut off.
**Suggested fix:** Add bottom margin or padding to the tooltip bar so it sits a few pixels above the viewport's bottom edge, ensuring the full text height is visible.

### 2. Character selector panel tight against bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "SELECT CHARACTER" panel at the bottom-center is positioned low enough that the "FAVOURITE" label at the bottom of the panel sits very close to the viewport edge. The panel's bottom border has minimal clearance from the lower boundary.
**Suggested fix:** Move the character selector panel upward or add bottom margin to ensure the full panel including the "FAVOURITE" label has clearance from the viewport edge.

### 3. Notification badge overlaps with user status pill
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the top-right corner, the yellow notification circle with "1" and the star badge sit very close to the edge of the "thia9uers - Online" status pill. The notification badge's position could overlap the status pill on slightly different viewport sizes.
**Suggested fix:** Ensure a minimum gap between the notification badge and the status pill, or anchor them with explicit spacing constraints.
