# Layout QA: sports_settings

**Screenshots analyzed:** 1

## Issues Found

### 1. Settings rows extend to the far left screen edge with no left margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The settings rows (Window mode, Resolution, Screen percentage, etc.) start at the very left edge of the screen with minimal or no left margin. The row labels and their highlighted backgrounds begin at approximately x=0. Meanwhile, the right side has ample space occupied by the description panel. This creates an asymmetric layout where the left edge feels clipped.
**Suggested fix:** Add a left margin or padding to the settings list container so that row labels and backgrounds have breathing room from the screen edge.

### 2. "Anti-Aliasing method" value text crowds its stepper controls
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The highlighted row "Anti-Aliasing method" has the value "TSR (Temporal Super Resolution)" which is a long string. The text fills the entire space between the `<` and `>` stepper arrows with very little padding, and the text's background highlight box crowds adjacent elements. The visual bar indicators to the right are also tightly packed.
**Suggested fix:** Allow the value text area to accommodate longer strings with proper padding, or truncate/abbreviate long values with an ellipsis. Ensure the stepper arrows have consistent spacing from the value text.
