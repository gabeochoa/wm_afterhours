# Layout QA: empire_tycoon

**Screenshots analyzed:** 1

## Issues Found

### 1. Left sidebar icons overlap with adjacent navigation items
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the left sidebar, the category icons (Rides, Food, Upgrades, Finance) have icon artwork that visually overlaps or bleeds into adjacent rows. The "Food" row icon overlaps downward into the "Upgrades" area, and the "Rides" icon above "Food" shows similar tight overlap. The icons appear to stack on top of each other rather than being cleanly contained within each nav button's bounds.
**Suggested fix:** Increase vertical padding/margin between sidebar navigation items to give each icon adequate space, or reduce icon sizes so they fit cleanly within their individual row bounds.

### 2. Right-side info panel text clipped at screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The right-side "System Options" panel contains description text reading "Configure vibration, save data, and a..." — the text is visibly truncated at the right edge of the viewport. The panel or its content extends beyond the visible screen boundary.
**Suggested fix:** Constrain the right-side panel width to stay within viewport bounds and apply text wrapping so the description flows to the next line rather than overflowing off-screen.

### 3. Notification badge overlaps sidebar item boundary
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The red notification badge (exclamation mark) on the "Upgrades" sidebar item extends beyond the top-right corner of the sidebar card boundary, partially overlapping the area outside the card.
**Suggested fix:** This may be intentional badge behavior. If badges should stay within the card boundary, adjust the badge position inward or add overflow-visible only for the badge element.

### 4. Right-side decorative elements clipped at viewport edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** On the far right side of the screen, decorative elements (parasol/umbrella icons) are partially cut off at the right edge of the viewport. The top-right area also shows items that appear clipped.
**Suggested fix:** Ensure decorative elements either fit fully within the viewport or are intentionally designed to bleed off-screen. If they should be fully visible, adjust their positioning.
