# Layout QA: decorative_frame

**Screenshots analyzed:** 1

## Issues Found

### 1. KraftPaper frame corner decorations extend outside frame bounds
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The KraftPaper decorative frame (leftmost) has small dashed L-shaped corner bracket decorations at the top-left and top-right that extend beyond the rounded rectangle of the main frame. The dark brown layered corner elements also bleed past the frame's border radius area, protruding outside the frame boundary.
**Suggested fix:** If the corner brackets are intentional decorative flourishes, ensure they don't clip against neighboring elements. If they should be contained, clip the corner decorations to the frame bounds or position them just inside the rounded corners.

### 2. Inset frame has corner mismatch between dashed border and shadow rectangle
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The Inset decorative frame (rightmost) has a dashed outer border that extends slightly beyond the gray shadow/inset area behind it. The dashed border at the top-left and top-right corners peeks past the background shadow rectangle, creating a mismatch. The outer dashed border uses a different corner radius than the inner rounded rectangle.
**Suggested fix:** Align the dashed border outline to match the shadow rectangle bounds, or use consistent corner radius for both the outer dashed border and inner rounded shape.
