# Layout QA: fighter_menu

**Screenshots analyzed:** 1

## Issues Found

### 1. System Options description text overflows off-screen to the right
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the right-side panel, the "System Options" description text reads "Configure vibration, save data, and a..." with the text visibly cut off at the right edge of the screen. The text extends beyond the visible viewport and is not fully readable.
**Suggested fix:** Add text wrapping to the description text container, or constrain the right panel width so that all content remains within the viewport bounds. Alternatively, use text-overflow ellipsis with a tooltip for long descriptions.

### 2. Right panel extends beyond viewport boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The entire right-side panel containing "System Options" with its icon, title, and description appears to extend past the right edge of the screen. The panel's right border is not visible, suggesting it is clipped by the viewport.
**Suggested fix:** Constrain the right panel's width or adjust its horizontal position so it fits fully within the 720p viewport. Ensure the panel's right border and padding are visible.

### 3. Tab row items "Sound & Language", "Network Options", "Button Settings" appear visually compressed
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The secondary tab bar below the main navigation ("System Options", "Game Options", "Display Options", "Sound & Language", "Network Options", "Button Settings") shows the rightmost tabs with darker/grayed styling that appears compressed. The tab labels may be running into each other with insufficient spacing.
**Suggested fix:** Either reduce the number of visible tabs and add horizontal scrolling, or decrease tab label font size to ensure all tabs fit with adequate padding.
