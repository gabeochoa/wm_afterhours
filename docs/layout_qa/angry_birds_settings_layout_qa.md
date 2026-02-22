# Layout QA: angry_birds_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Tab bar background extends beyond the settings panel left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The tab bar row containing "Audio", "General", and "Info" tabs has a background that extends slightly beyond the left edge of the settings panel container. The gray/green colored bar for the tabs appears to bleed past the rounded container's left boundary.
**Suggested fix:** Clip the tab bar background to the settings panel's container bounds, or inset the tab bar so it starts within the panel's padded content area.

### 2. Horizontal accent line extends beyond panel left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** There is a thin olive/yellow horizontal line below the tab bar that extends beyond the left edge of the settings panel, bleeding past the container's rounded corner boundary.
**Suggested fix:** Constrain the horizontal divider/accent line width to match the content area within the settings panel, or clip it to the panel's border-radius bounds.
