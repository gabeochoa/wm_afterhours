# Layout QA: powerwash_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. "CONTROLS" label overlaps with "Close" button text
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** At the bottom-left of the screen, the category tab label "CONTROLS" and the "X Close" action hint text overlap each other. The word "CONTROLS" is partially rendered on top of or directly adjacent to the "Close" label, making both difficult to read. The two text elements occupy the same vertical and horizontal space.
**Suggested fix:** Separate the tab bar row from the action-hint row with distinct vertical positioning. Place the category tabs (GENERAL, GAMEPLAY, VIDEO, AUDIO, CONTROLS) on one line and the action hints (X Close, O Reset, [] Select) on a separate line below with adequate spacing.

### 2. Settings panel left border clips the label column
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The outer rounded-corner border of the settings panel passes very close to the left edge of the first label column. The "Screen Resolution" text appears to touch or nearly touch the left border of the container, with no visible inner padding on the left side.
**Suggested fix:** Add left padding inside the settings panel container so that the first column of labels has breathing room from the rounded border.

### 3. Help panel positioned with large gap from settings panel
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "Help: Anti-Aliasing" panel on the right side is positioned with a significant horizontal gap from the main settings panel on the left. The two panels don't appear visually connected, leaving a large empty area in the center-right of the screen. This makes the help text feel disconnected from the setting it describes.
**Suggested fix:** Move the help panel closer to the settings panel, or add a visual connector element (line, background fill) to make the relationship between the selected setting and its help text clearer.
