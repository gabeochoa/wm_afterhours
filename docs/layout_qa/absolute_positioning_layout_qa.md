# Layout QA: absolute_positioning

**Screenshots analyzed:** 1

## Issues Found

### 1. "TR" corner-pinned element clipped by container/screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In section "2. Corner Pinning (absolute + translate)", the top-right ("TR") element is positioned at the far right edge and is partially clipped by the container boundary. The element's right side is cut off, and its full rounded shape is not visible.
**Suggested fix:** Adjust the translate offset or add padding so the TR element remains fully within the container bounds at 720p resolution.

### 2. "BR" corner-pinned element clipped by container/screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The bottom-right ("BR") element in section 2 is similarly clipped on the right side, mirroring the TR issue. Its right edge is cut off by the container boundary.
**Suggested fix:** Apply the same inward offset fix as TR to ensure the BR element is fully visible within the container bounds.

### 3. "BADGE" overlay text truncated at container right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In section "3. Mixed: Flow children + Absolute overlay", the pink badge element at the top-right is clipped by the container's right edge. The label reads "BADG" with the trailing "E" cut off, and the element extends beyond the container's visible area.
**Suggested fix:** Reposition the badge element inward so it sits fully within the container, or increase the container's width to accommodate the badge. If the overlay is intentional, ensure the text is not truncated.
