# Layout QA: deadspace_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Tab labels truncated in settings tab bar
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** Several tab labels in the horizontal tab bar are truncated: "Display and Graphic" (missing 's'), "Language and Subti" (missing "tles"), and "Policies and Licens" (missing "ing"). The tab bar does not provide enough horizontal space per tab to display the full text of longer labels.
**Suggested fix:** Use abbreviated labels that fit fully (e.g., "Display", "Language", "Policies"), reduce the tab label font size, implement horizontal scrolling for the tab bar, or widen the tab bar to accommodate full labels.

### 2. "Resume Game" focus border clips at top edge
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The "Resume Game" item at the top of the pause menu sidebar has a dashed focus/selection border whose top edge clips against the top boundary of the menu list container. The top border line sits at or above the container's start position, unlike the highlighted "Menu Narration" item below which has clean bounds.
**Suggested fix:** Add top padding to the pause menu list container so the focus border of the first item is fully visible without clipping.

### 3. Action buttons float mid-panel in large empty content area
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main content area for "Controls" shows only a title and placeholder text, with OK/Cancel/Apply buttons positioned at mid-right. There is a large expanse of empty space below the buttons to the bottom of the panel, and the buttons float without being anchored to the content area's bottom boundary.
**Suggested fix:** Position the action buttons at the bottom of the content area, or vertically constrain the content area to fit actual content with buttons anchored at the bottom.
