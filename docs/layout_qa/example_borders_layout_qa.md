# Layout QA: example_borders

**Screenshots analyzed:** 1

## Issues Found

### 1. "Soft Shadow" and "Outline Only" labels truncated at right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Corners & Shadows" row, the "Soft Shadow" label text appears to be clipped/truncated at the right side of its card. Similarly, in the "Themed Styles" row, the "Outline Only" label is partially cut off at the right edge of the visible area. The rightmost cards in several rows are positioned very close to the viewport edge.
**Suggested fix:** Add horizontal padding or margin on the right side of the overall layout container, or make the grid responsive so that rightmost cards have sufficient space for their labels.

### 2. "Per-Side" row labels truncated
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom "Per-Side" row, the labels "Bottom Only", "Left+Right", and "Underline" appear to have their text cut off or tightly compressed. "Bottom Only" shows as "Bottom On" with the rest clipped, and "Left+Right" text is similarly squeezed.
**Suggested fix:** Increase card width in the Per-Side row, or allow text to wrap within the card labels to prevent truncation.

### 3. Row label text ("Width & Color", "Corners & Shadows") left-aligned outside card grid
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The row section labels on the left side ("Width & Color", "Corners & Shadows", "Themed Styles", "Interactive", "Per-Side") are positioned to the left of the card grid. "Corners & Shadows" wraps to two lines due to limited horizontal space. This left-label layout causes the cards to start further right, contributing to the right-edge truncation issues.
**Suggested fix:** Consider placing row labels above their respective rows instead of to the left, or allocate more horizontal space for the label column so it doesn't compress the card grid area.
