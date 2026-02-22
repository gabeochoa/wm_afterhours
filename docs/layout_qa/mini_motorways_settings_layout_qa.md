# Layout QA: mini_motorways_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Controller Customization row elements overlapping
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Controller Cu..." row near the bottom of the settings panel, the label text is truncated ("Controller Cu...") and visually collides with the "<" stepper button. Partially visible text fragments ("ntiv") appear between the "<" button and the "Default" value text, creating a cluttered overlapping region where label, stepper arrows, and value text all crowd together.
**Suggested fix:** Ensure the controller customization row has sufficient width for the full label text, or properly truncate with ellipsis before it reaches the stepper arrows. Enforce non-overlapping spacing between the label, left arrow, value text, and right arrow.

### 2. "Colorblind Mode OFF" text crowds into the toggle switch
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "Colorblind Mode" row shows the "OFF" status text sitting immediately adjacent to the toggle switch with no visible gap. The text appears to touch or overlap the toggle control's left edge, unlike other toggle rows which have clearer separation.
**Suggested fix:** Add horizontal spacing (margin or padding) between the "OFF" status label and the toggle switch to maintain consistent spacing with other setting rows.

### 3. Tab bar text sizes are inconsistent and cramped
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar ("Game Video Audio Language Controls Cross-Save Credits") has inconsistent text sizes. "Game" appears largest as the active tab, while "Language", "Controls", "Cross-Save" appear at notably smaller sizes. The "Cross-Save" label is especially small and cramped between adjacent tabs, creating an uneven layout.
**Suggested fix:** Normalize tab text sizes or ensure all inactive tabs use the same font size. Differentiate the active tab with an underline or background rather than dramatically different sizing that causes cramped layout.
