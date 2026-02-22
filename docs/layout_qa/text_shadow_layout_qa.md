# Layout QA: text_shadow

**Screenshots analyzed:** 1

## Issues Found

### 1. Title "Text Drop Shadow" clipped at top of viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The main title "Text Drop Shadow" at the top of the screen has its upper portion cut off by the top edge of the viewport. The ascenders of the title text and the shadow effect above them are partially obscured. The subtitle "Add depth and improve legibility with text shadows" also sits very close to the top edge with minimal clearance.
**Suggested fix:** Add sufficient top padding/margin above the title so the full text including its shadow offset is visible within the viewport. Push the entire content layout down to give the title clearance.

### 2. "COLORED" text shadow bleeds into description text below
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "COLORED" text (cyan with red shadow) has a large shadow offset that extends downward, overlapping with the description text "Cyan text with red shadow for retro 3D look" beneath it. The shadow creates a visual collision between the decorative text and its label.
**Suggested fix:** Increase the vertical spacing between the "COLORED" example and its description text to account for the shadow offset, preventing the shadow from overlapping adjacent content.

### 3. "COMBO" text shadow overlaps with description below
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "COMBO" text has a purple shadow that extends downward and overlaps with the description "Combine stroke and shadow for maximum impact." The shadow bleeds into the text line below it.
**Suggested fix:** Increase vertical spacing below the "COMBO" text element to provide clearance for the shadow effect.

### 4. Shadow Offset Examples panel — large shadows overlap adjacent rows
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Shadow Offset Examples" panel on the right side, the larger offset examples (6px, 8px, 10px) have shadows that visually bleed into the rows below them. The 10px "SHADOW" at the bottom has its shadow extending past the panel's bottom edge.
**Suggested fix:** Increase row height in the shadow offset examples panel to accommodate larger shadow offsets, or clip shadows to prevent bleed between rows.

### 5. Tip bar at the bottom is flush with viewport edges
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The light-colored tip bar ("Tip: Use small offsets (1-4px) for subtle depth, larger offsets (6-10px) for bold effects") extends to the left and right edges of the viewport with no margin, and its bottom edge is flush with the viewport bottom. The bar appears to lack rounded corners and may be partially cut off at the bottom.
**Suggested fix:** Add margin around the tip bar so it doesn't sit flush against the viewport edges, or ensure it has proper bottom padding and rounded corners if it is intended to be a contained element.
