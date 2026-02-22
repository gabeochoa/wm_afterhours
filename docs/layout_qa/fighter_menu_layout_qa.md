# Layout QA: fighter_menu

**Screenshots analyzed:** 1

## Issues Found

### 1. Right panel and description text overflow off-screen
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The right-side "System Options" panel extends beyond the right edge of the viewport. The description text reads "Configure vibration, save data, and a..." with the remainder cut off. The panel's right border is not visible, confirming it is clipped by the viewport.
**Suggested fix:** Constrain the right panel width so it fits within the 720p viewport. Apply text wrapping to the description text container so all content remains visible.

### 2. Secondary tab labels visually compressed on the right
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The secondary tab bar ("System Options", "Game Options", "Display Options", "Sound & Language", "Network Options", "Button Settings") shows the rightmost tabs with darker/grayed styling and they appear visually compressed with insufficient spacing between labels.
**Suggested fix:** Reduce the number of visible tabs and add horizontal scrolling, or decrease tab label font size to ensure all tabs fit with adequate padding.

### 3. Center content area has excessive unused vertical space
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The center content area ("Offline Mode" with character preview and "Select Character") has significant empty space below the content. The panel extends much further down than its content requires.
**Suggested fix:** Consider sizing the content panel to fit its content more tightly, or vertically center the character preview within the available space.
