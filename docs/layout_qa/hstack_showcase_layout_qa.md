# Layout QA: hstack_showcase

**Screenshots analyzed:** 1

## Issues Found

### 1. Element clipped at right edge of the screen
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** At the far right edge of the screen (approximately x=1245-1280), there is a partially visible element — what appears to be a cyan/teal rectangle with the letter "A" — that is clipped by the screen boundary. This element sits to the right of the "Toolbar Pattern" section area, suggesting it overflows the main content container or viewport.
**Suggested fix:** Ensure all elements within the hstack showcase fit within the viewport width. Check if the toolbar pattern section or a sibling element has excessive width or positioning that pushes content beyond the right edge. Add appropriate constraints or reduce element sizing to keep everything within bounds.

### 2. "Churn" card extends to screen edge with no right margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the bottom card row (Users, Revenue, Orders, Growth, Active, Churn), the "Churn" card at the far right extends very close to or touches the right edge of the viewport at approximately x=1230. Unlike the left side which has visible margin/padding, the right side has no breathing room, creating an asymmetric layout.
**Suggested fix:** Add right padding or margin to the card row container to match the left side spacing, or constrain the row width so all cards fit symmetrically within the viewport with equal margins on both sides.
