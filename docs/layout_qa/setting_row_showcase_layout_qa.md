# Layout QA: setting_row_showcase

**Screenshots analyzed:** 1

## Issues Found

### 1. Main container's left edge is clipped at screen boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main settings panel's left border and rounded corner are barely visible or cut off at the left edge of the viewport. The container appears to start at or very near x=0, so the left border and rounded corner are clipped or pushed against the screen edge, while the right side has generous padding. This asymmetry makes the left border appear missing.
**Suggested fix:** Add equal horizontal margin/padding on both sides of the main container so the left rounded corner and border are fully visible and match the right side's spacing.

### 2. "Music Volume" slider row is cramped at the container bottom
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "Music Volume" row at the bottom of the VOLUME section sits at the very bottom edge of the container. The slider track and value label ("65%") are present but the row has minimal or no bottom padding, making it look cramped against the container's bottom border.
**Suggested fix:** Add bottom padding inside the container so the last settings row has breathing room before the container edge.
