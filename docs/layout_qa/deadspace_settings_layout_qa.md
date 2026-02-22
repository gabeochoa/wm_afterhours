# Layout QA: deadspace_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Tab labels truncated in settings tab bar
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The horizontal tab bar spanning "Controls, Gameplay, Display & Graphics, Audio, Language & Subs, Accessibility, Policies & Licenses, Credits" extends to the very right edge of the viewport. While the tab labels appear to display fully at the current font size, they are tightly packed with minimal inter-tab spacing, leaving no right-side margin before the screen edge.
**Suggested fix:** Add right padding to the tab bar container, or make the tab bar horizontally scrollable. Alternatively, reduce tab label font size or spacing to ensure all tabs fit with adequate margin.

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
