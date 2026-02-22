# Layout QA: auto_text_color

**Screenshots analyzed:** 1

## Issues Found

### 1. Element overflows past the right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** On the right side of the screen, vertically aligned with the "Default Behavior" section, a dark blue rectangular element is mostly off-screen with only a thin sliver visible at the right viewport edge. This element overflows past the outer container's right boundary.
**Suggested fix:** Identify this element and either constrain it within the container bounds, adjust its position so it is fully visible, or remove it if it is unintended.

### 2. "Near Black" swatch lacks visible container background
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Default Behavior" section top row, the "Near Black" label appears without a visible pill/card background, unlike "Dark Blue", "Dark Purple", and "Dark Green" which all have visible container backgrounds. The near-black background merges with the dark page background, making the container bounds invisible.
**Suggested fix:** If this is intentional (demonstrating auto text color on near-black), consider adding a subtle border so the swatch boundaries are discernible. If unintentional, ensure the container renders with a visible background.
