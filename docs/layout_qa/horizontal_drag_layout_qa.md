# Layout QA: horizontal_drag

**Screenshots analyzed:** 6

## Issues Found

### 1. Tag overflows High Priority lane container on the right
**Type:** overflow
**Screenshot:** horizontal_drag_initial.png, horizontal_drag_after_first_move.png, horizontal_drag_after_second_move.png, horizontal_drag_final.png, horizontal_drag_reorder.png, idle_720p.png
**Detail:** In the "High Priority (2)" lane, a tag (appears to be labeled "Rea..." — likely "Reassign" or similar) is partially visible at the far right edge of the container, clipped by the lane boundary. Only the first few characters ("Rea") and the left portion of the tag are visible. The tag extends beyond the right edge of the High Priority container in all six screenshots, indicating a persistent overflow issue where the lane does not have enough horizontal space to display all its tags.
**Suggested fix:** Either enable horizontal scrolling within the priority lane so all tags are accessible, or implement wrapping so tags flow to a second row when they exceed the lane width. Alternatively, ensure the lane container is wide enough to accommodate all tags, or truncate tag text with ellipsis while keeping tags fully within bounds.
