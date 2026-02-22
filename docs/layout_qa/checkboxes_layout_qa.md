# Layout QA: checkboxes

**Screenshots analyzed:** 2

## Issues Found

### 1. Disabled checkbox section overflows container bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** In the left column, the "Disabled" section with "Disabled ON" and "Disabled OFF" rows extends past the visible bottom boundary of the left panel container. The bottom of "Disabled OFF" appears clipped, and a diamond decorator element between the rows overlaps the container border.
**Suggested fix:** Increase the left panel container height to fully accommodate the disabled checkbox rows, or clip the diamond decorator to the container bounds.

### 2. Status text sits outside main container
**Type:** container_bounds
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** The "Group selected: 2 of 4" and "MinMax selected: 1 of 3" text at the bottom of the screen sits outside and below the main content container boundary, floating independently in the background area.
**Suggested fix:** Extend the main container to include the status text, or add a dedicated footer row within the container for selection status information.

### 3. Unchecked rows lack consistent indicator space
**Type:** container_bounds
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** The "Secondary" and "Box Only" checkbox rows fill their full width with background color, leaving no visible reserved space for a checkmark indicator on the right side. Checked rows like "Primary" and "Accent" show "V" marks, but unchecked rows don't reserve consistent space for the indicator area.
**Suggested fix:** Reserve consistent right-side space for the checkmark indicator in all labeled checkbox rows, regardless of checked/unchecked state.
