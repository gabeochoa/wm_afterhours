# Layout QA: vstack_showcase

**Screenshots analyzed:** 1

## Issues Found

### 1. "Sidebar + Content" right panel overflows container and viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Sidebar + Content" section (bottom-right), the right-side content panel items ("Content" header, "Alerts", "Activity", "Stats") extend beyond the rounded container boundary and reach the right edge of the viewport. The colored bars bleed past the container's right border, breaking visual containment.
**Suggested fix:** Constrain child element widths within the content panel so they respect the container's inner bounds and rounded corners. Add right padding or set a max-width on the content panel.

### 2. Top section label bars extend flush to screen edges
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The pink label bars ("Start", "Center", "End", "Between", "Around") in the top JustifyContent section extend flush to the screen edges on both sides. The "Around" label reaches the right edge and the layout leaves no outer margin, making the content appear to touch or nearly touch the viewport boundary.
**Suggested fix:** Add horizontal margin or padding to the outermost container so the label bars do not touch the screen edges, ensuring consistent outer spacing.

### 3. "AlignItems" and "Sidebar + Content" header bars are vertically misaligned
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The green "AlignItems (cross-axis)" header bar and the blue "Sidebar + Content" header bar in the second row sit at slightly different vertical positions. The "Sidebar + Content" bar appears to sit higher, creating a visually misaligned row. The sub-labels ("Start", "Center", "Stretch", "End") under AlignItems also crowd the top edge of their respective card containers.
**Suggested fix:** Ensure both section header bars share the same vertical baseline. Add a small gap between the sub-labels and the top edge of the card containers below them.
