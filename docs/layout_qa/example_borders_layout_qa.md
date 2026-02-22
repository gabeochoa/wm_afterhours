# Layout QA: example_borders

**Screenshots analyzed:** 1

## Issues Found

### 1. "Soft Shadow" and "Outline Only" labels truncated at right viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Corners & Shadows" row, the "Soft Shadow" label text is clipped at the right side of its card. Similarly, in the "Themed Styles" row, the "Outline Only" label is partially cut off ("Outline Only" shows as "Outline On..." at the right edge). The rightmost cards in multiple rows are positioned flush against the viewport boundary with no clearance.
**Suggested fix:** Add right-side padding/margin to the overall layout container, or reduce the number of cards per row so all rightmost cards have adequate space for their labels.

### 2. "Per-Side" row labels truncated
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom "Per-Side" row, the labels "Bottom Only" and "Left+Right" are truncated — "Bottom Only" appears as "Bottom Onl" and "Left+Right" as "Left+Righ". The card width is not sufficient to display full label text in this row.
**Suggested fix:** Increase card width in the Per-Side row, or allow text to wrap within the card labels to prevent truncation.

### 3. Row label text left-aligned too tightly against card grid
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The row section labels ("Width & Color", "Corners & Shadows", "Themed Styles", "Interactive", "Per-Side") are positioned to the left of the card grid. "Corners & Shadows" wraps to two lines due to limited horizontal space. This left-label layout causes cards to start further right, contributing to the right-edge truncation.
**Suggested fix:** Place row labels above their respective rows instead of to the left, or allocate more horizontal space for the label column so it doesn't compress the card grid area.
