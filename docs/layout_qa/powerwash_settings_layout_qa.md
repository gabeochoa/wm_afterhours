# Layout QA: powerwash_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. "CONTROLS" label overlaps with "Close" button text
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** At the bottom-left of the screen, the category tab label "CONTROLS" and the "X Close" action hint text overlap each other. The word "CONTROLS" is partially rendered on top of or directly adjacent to the "Close" label, making both difficult to read. The two text elements occupy the same vertical and horizontal space.
**Suggested fix:** Separate the tab bar row from the action-hint row with distinct vertical positioning. Place the category tabs (GENERAL, GAMEPLAY, VIDEO, AUDIO, CONTROLS) on one line and the action hints (X Close, O Reset, [] Select) on a separate line below with adequate spacing.

### 2. Category tabs wrap across two lines
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The bottom category tabs (GENERAL, GAMEPLAY, VIDEO, AUDIO, CONTROLS) do not all fit on a single line. "CONTROLS" wraps to a second line beneath the other tabs, creating an uneven layout where the action buttons (Close, Reset, Select) share the same vertical space as the wrapped "CONTROLS" label.
**Suggested fix:** Either widen the tab container to accommodate all five category labels on a single row, reduce the tab label font size, or use abbreviated labels to prevent wrapping.

### 3. Settings panel left border clips the label column
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The outer rounded-corner border of the settings panel passes very close to the left edge of the first label column. The "Screen Resolution" text appears to touch or nearly touch the left border of the container, with no visible inner padding on the left side.
**Suggested fix:** Add left padding inside the settings panel container so that the first column of labels has breathing room from the rounded border.
