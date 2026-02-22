# Layout QA: self_align

**Screenshots analyzed:** 1

## Issues Found

### 1. "FlexStart (left)" element overflows the container's left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green "FlexStart (left)" button extends beyond the left border of the rounded-corner demonstration container. The button's left edge sits outside the container's border line, and its top-left corner visually bleeds past the container's rounded corner, breaking the containment boundary.
**Suggested fix:** Add left padding inside the container or constrain the child element so it respects the container's border and border-radius. The element should be fully contained within the parent's visible bounds.

### 2. "FlexEnd (right)" element overflows the container's right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The orange-brown "FlexEnd (right)" button extends beyond the right border of the rounded-corner container. The button's right edge sits outside the container border, and its bottom-right corner bleeds past the container's rounded corner.
**Suggested fix:** Add right padding inside the container or constrain the child so it stays within the container's border-radius clipping area. Apply overflow clipping or equivalent to the container.
