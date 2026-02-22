# Layout QA: toggle_switches

**Screenshots analyzed:** 1

## Issues Found

### 1. Bottom row clipped by viewport
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "Unavailable Option (OFF)" row at the bottom of the "Disabled" section is partially cut off by the bottom edge of the screen. The lower portion of the row and its rounded corners are not fully visible, indicating the overall container height exceeds the 720p viewport.
**Suggested fix:** Either make the settings panel scrollable so all content is accessible, or reduce vertical spacing/padding between sections so all rows fit within the 720p viewport.

### 2. Divider lines clip through Preferences row containers
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Preferences" section, thin vertical divider lines appear to run through the middle of each row ("Dark Mode", "Auto-Save", "Cloud Sync"), extending beyond the row bounds. These lines overlap the row content area and break the visual containment of each row, cutting through the horizontal row backgrounds.
**Suggested fix:** Remove the mid-row dividers or constrain them to sit between rows rather than overlapping row content. If they are meant to separate the label from the checkmark, align them as internal cell dividers within the row bounds.
