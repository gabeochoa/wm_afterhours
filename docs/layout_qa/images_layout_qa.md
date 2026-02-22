# Layout QA: images

**Screenshots analyzed:** 1

## Issues Found

### 1. Sprite source rectangles partially visible next to icons
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** Across all four rows (Sprites, Image Buttons, With Background, Icon Row), each icon/sprite displays small artifact fragments to its right — visible as thin partial rectangles or edge lines adjacent to the gear, star, trophy, home, and play icons. These appear to be sprite source rectangles or debug bounds that are not fully clipped, bleeding past the intended icon display area. The artifacts are small but consistently present across all icon instances.
**Suggested fix:** Verify that sprite source rectangles are correctly sized and that the rendering clips to the intended icon bounds. If these are debug visualization artifacts, disable them. If they are part of the sprite sheet bleeding through, ensure the UV/source rect is precisely cropped to the icon content area.

### 2. "With Background" row content clipped at bottom
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "With Background" row (third row), there appears to be content partially cut off at the bottom of the container. A small fragment is visible below the Settings button with background, suggesting the container height is slightly too short for its content.
**Suggested fix:** Increase the height of the "With Background" row container to fully encompass its content, or verify the vertical padding is sufficient for the child elements.
