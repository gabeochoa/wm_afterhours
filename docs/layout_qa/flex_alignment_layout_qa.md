# Layout QA: flex_alignment

**Screenshots analyzed:** 1

## Issues Found

### 1. Cross-Axis Align demo items overflow their container boxes
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom-left "Cross-Axis Align (AlignItems)" section, the demo items (pink numbered blocks 1, 2, 3) in the "Start" and "End" columns extend slightly beyond their gray container boundaries. The blocks have portions that visually bleed past the container edges, particularly at the bottom.
**Suggested fix:** Increase the container height for the Cross-Axis Align demo boxes, or reduce the size of the demo elements so they fit fully within their parent containers.

### 2. "Gap" tab label at right edge of tab bar
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar at the top ("All", "Vertical", "Horizontal", "Cross-Axis", "Self Align", "Gap") has the rightmost "Gap" tab positioned at the very edge of the viewport with minimal right padding, sitting flush against the screen boundary.
**Suggested fix:** Add right padding or margin to the tab bar container so edge tabs have proper spacing from the viewport boundary.
