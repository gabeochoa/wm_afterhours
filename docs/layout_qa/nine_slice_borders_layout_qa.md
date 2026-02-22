# Layout QA: nine_slice_borders

**Screenshots analyzed:** 1

## Issues Found

### 1. "Double Width" and "Double Ornate" border panels overlap each other
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Double-Width & Tinting" row, the "Double Width" panel and the "Double Ornate" panel are positioned so close together that their decorative 9-slice borders overlap. The ornate corner decorations of the right edge of "Double Width" visually merge with the left edge decorations of "Double Ornate," creating a messy artifact where the two border patterns collide. Both labels are also truncated ("Double Widt" and "Double Ornat").
**Suggested fix:** Add horizontal margin between the "Double Width" and "Double Ornate" panels so their 9-slice border decorations have clear separation. Widen panels slightly to accommodate full label text.

### 2. Dialog text truncated in Interactive Elements section
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Interactive Elements" row at the bottom, the dialog panel contains text "This is a dialog using 9-slice. Corners s..." which is truncated. The text overflows the visible area and is clipped rather than wrapping or being fully contained.
**Suggested fix:** Either increase the dialog panel width to fit the full text, enable text wrapping within the container, or ensure text ellipsis is applied cleanly.

### 3. "Interactive Elements" section header overlaps with Double-Width row
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The section header "Interactive Elements" (gold text near bottom-left) overlaps with the bottom border decorations of the "Double-Width & Tinting" row above it. The header text sits very close to or on top of the lower border ornaments of the panels in the row above.
**Suggested fix:** Add top margin to section headers or bottom margin to content rows to create clear separation between sections and their preceding content.
