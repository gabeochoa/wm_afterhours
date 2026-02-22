# Layout QA: text_stroke

**Screenshots analyzed:** 1

## Issues Found

### 1. Tip bar at the bottom is partially clipped at the viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green "Tip: Use 2-6px for readable text, 6-10px for decorative titles and headers" bar at the bottom of the screen extends to the very bottom of the viewport. The bottom border/padding of the tip container appears cut off with no margin below it, making it look clipped rather than self-contained.
**Suggested fix:** Add bottom margin or reduce overall content height so the tip bar and its bottom padding are fully visible within the 720p viewport.

### 2. "invisible" and "visible!" labels clipped below comparison box
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** At the bottom-left of the screen, the labels "invisible" and "visible!" that describe the white stroke comparison examples appear partially cut off or extremely close to the bottom edge of the content area. These labels sit below the white/light-gray comparison box and are barely legible.
**Suggested fix:** Add bottom margin to the comparison section or increase the overall content area height so these labels have adequate clearance from the viewport bottom.

### 3. Title has minimal top margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main heading "Text Stroke / Outline" sits close to the top of the viewport with minimal top padding. While not clipped, the title's thick stroke effect has very little breathing room above it and could be cropped on displays with slight overscan.
**Suggested fix:** Add top padding or margin to the main container to give the title adequate spacing from the viewport edge.
