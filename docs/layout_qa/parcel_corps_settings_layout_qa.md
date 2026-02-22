# Layout QA: parcel_corps_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Settings panel content clipped at bottom edge of viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The settings panel on the left extends to the very bottom of the viewport. The "Icon Legend" section is cramped against the bottom, and a teal element is partially visible below the legend, clipped by the screen boundary. The panel does not provide enough vertical space to fully render all its content within the 720p viewport.
**Suggested fix:** Add bottom padding to the settings panel, or make the settings list scrollable so the legend and any elements below it are fully visible without clipping at the viewport edge.

### 2. Objective panel text runs flush against right screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the upper-right area, the objective/quest panel text "[X] SIGN UP A BUSINESS TO YOUR APP" and "[ ] Lv0 SIGN UP 4 BUSINESSES TO YOUR APP" extends to the very edge of the viewport. The text runs flush against the right screen boundary with no margin, and the "Lv0" badge appears to slightly overflow its container bounds.
**Suggested fix:** Constrain the objective panel to a fixed max-width with right margin, or enable text wrapping/truncation so content does not extend beyond the safe area.

### 3. Slider handle extends past track boundary
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The volume slider handles (Audio Volume, Music Volume) extend slightly beyond the right edge of their slider track containers. The white square handle sits at a position where it visually clips past the track border.
**Suggested fix:** Ensure the slider handle is constrained within the track bounds, or give the track sufficient padding so the handle doesn't visually overflow at any position.
