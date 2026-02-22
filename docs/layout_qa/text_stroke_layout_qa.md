# Layout QA: text_stroke

**Screenshots analyzed:** 1

## Issues Found

### 1. Tip bar at the bottom is partially clipped at the viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green "Tip: Use 2-6px for readable text, 6-10px for decorative titles and headers" bar at the bottom of the screen extends to the very bottom of the viewport. The bottom border/padding of the tip container appears cut off with no margin below it, making it look clipped rather than self-contained.
**Suggested fix:** Add bottom margin or reduce overall content height so the tip bar and its bottom padding are fully visible within the 720p viewport.

### 2. Title has minimal top margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main heading "Text Stroke / Outline" sits close to the top of the viewport with minimal top padding. While not clipped, the title's thick stroke effect has very little breathing room above it and could be cropped on displays with slight overscan.
**Suggested fix:** Add top padding or margin to the main container to give the title adequate spacing from the viewport edge.

### 3. Left and right content columns lack consistent vertical alignment
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The left column's first example ("NO STROKE / WITH STROKE") starts higher than the right column's first entry ("Outline Thickness: STROKE" at 2px). The two columns don't share a consistent top alignment or baseline grid. The left column also extends lower (WHITE comparison box + "invisible" / "visible!" labels) than the right column's last entry (10px - heavy).
**Suggested fix:** Align the starting vertical positions of both columns and ensure they share a consistent vertical rhythm or at least a common top edge.
