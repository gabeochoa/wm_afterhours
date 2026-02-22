# Layout QA: empire_tycoon

**Screenshots analyzed:** 1

## Issues Found

### 1. Left sidebar icons overlap with navigation labels
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the left sidebar, the category icons (Rides, Food, Upgrades, Finance) have icon artwork that visually overlaps or bleeds into adjacent rows. Specifically, the "Food" row icon overlaps downward into the "Upgrades" area, and the icons in general appear to stack tightly with insufficient spacing between them. The "Rides" icon above "Food" also shows a similar tight overlap.
**Suggested fix:** Increase vertical padding or margin between sidebar navigation items to give each icon adequate space, or reduce the icon size so they fit cleanly within their row bounds.

### 2. Notification badge overlaps sidebar item boundary
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The red notification badge (exclamation mark) on the "Upgrades" sidebar item extends beyond the top-right corner of the sidebar card boundary, partially overlapping the area outside the card.
**Suggested fix:** This may be intentional badge behavior, but if badges should stay within the card boundary, adjust the badge position inward or add overflow-visible only for the badge element.

### 3. Right-side decorative elements clipped at screen edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** On the far right side of the screen, there appear to be decorative elements (what looks like a parasol/umbrella icon) that are partially cut off at the right edge of the viewport. The top-right area also shows items that appear clipped.
**Suggested fix:** Ensure decorative elements either fit fully within the viewport or are intentionally designed to bleed off-screen. If they should be visible, adjust their positioning or the viewport width.

### 4. Chat log text in bottom-left lacks contained boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "GlobalChat" and "DevTeam" message lines at the bottom left sit in what appears to be a semi-transparent container, but the container's left edge appears to extend nearly to the screen edge with minimal padding. The container boundary is not well-defined compared to other UI panels.
**Suggested fix:** Add a clearer bounded container with consistent padding for the chat log area, matching the visual style of other panels on the screen.
