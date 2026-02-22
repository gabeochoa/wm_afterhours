# Layout QA: themes

**Screenshots analyzed:** 2

## Issues Found

### 1. Active theme label "Ocean Navy" truncated with orphaned parenthesis
**Type:** overflow
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** In the left sidebar under "Select Theme," the currently active theme is displayed as "> Ocean Navy (" — there is clearly additional text after the opening parenthesis that is cut off by the sidebar container width. The orphaned "(" character makes the truncation appear as a bug rather than intentional ellipsis. This is visible identically in both screenshots.
**Suggested fix:** Widen the sidebar panel to accommodate the full selected-theme label including any active indicator text, or apply proper ellipsis truncation that doesn't leave orphaned punctuation visible.

### 2. Color swatch labels truncated to single letters
**Type:** container_bounds
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** In the "Component Preview" section, the top row of color swatches shows labels as single characters: "P", "S", "A", "D" — these likely represent "Primary", "Secondary", "Accent", and "Disabled" (or similar). The swatch containers are too narrow to display full label text, while other labels on the same screen (e.g., "Checkbox", "Toggle", "Hard", "Soft") are fully spelled out.
**Suggested fix:** Widen the swatch containers to fit full label text, or use a two-line layout with the label below each swatch.

### 3. Outer container corners clip near header bar
**Type:** corners
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** The outermost container has large rounded corners. The "Theme Switcher" label and "Active: Ocean Navy" badge at the top-left are positioned close to the top-left rounded corner, and the bottom corners of the outer container are close to the viewport edges, leaving minimal clearance. The large border-radius reduces usable space at all four corners of the container.
**Suggested fix:** Add inner padding so content elements sit below and away from the rounded corner curves, or reduce the outer container's border-radius to a more moderate value to reclaim corner space.
