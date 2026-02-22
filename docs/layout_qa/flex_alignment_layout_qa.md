# Layout QA: flex_alignment

**Screenshots analyzed:** 1

## Issues Found

### 1. Cross-Axis Align demo items overflow their container boxes
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom-left "Cross-Axis Align (AlignItems)" section, the demo items (numbered blocks 1, 2, 3) in the "Start" and "End" columns appear to extend slightly beyond their gray container boundaries. The pink numbered blocks have portions that visually bleed past the container edges, particularly at the bottom.
**Suggested fix:** Increase the container height for the Cross-Axis Align demo boxes, or reduce the size of the demo elements so they fit fully within their parent containers.

### 2. "Gap" tab label near right edge of tab bar
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar at the top ("All", "Vertical", "Horizontal", "Cross-Axis", "Self Align", "Gap") has the rightmost "Gap" tab positioned at the very edge of the viewport. While the text is still readable, it has minimal right padding and sits flush against the screen boundary.
**Suggested fix:** Add right padding or margin to the tab bar container, or allow horizontal scrolling for the tab bar so edge tabs have proper spacing.
