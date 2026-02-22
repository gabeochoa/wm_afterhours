# Layout QA: dropdowns

**Screenshots analyzed:** 5

## Issues Found

### 1. Dropdown menu overflows its parent container card and overlaps the card below
**Type:** overflow
**Screenshot:** dropdown_open_initial.png, dropdown_arrow_down_once.png, dropdown_arrow_down_twice.png
**Detail:** When the "With Label" dropdown (center-top card) is opened, the dropdown option list extends downward past the bottom edge of its parent card container and visually overlaps the "Disabled" card in the row below. The dropdown list items ("Dog", "Cat", "Bird", "Fish", "Disabled") extend well beyond the card's lower boundary, covering the "Disabled" title bar of the card below.
**Suggested fix:** Either render the dropdown menu in an overlay/portal layer that floats above all cards (so the overlap is intentional and z-ordered correctly), or constrain the dropdown to scroll within the card bounds. If the overlay approach is intended, ensure the card below is not receiving input events from the overlapping dropdown area.

### 2. Bottom status bar text may be clipped at narrow widths
**Type:** container_bounds
**Screenshot:** dropdown_after_arrow_select.png, idle_720p.png
**Detail:** The bottom summary bar ("Basic=Alpha, Pet=Bird, Month=January, Indicator=Option A, Disabled=Frozen, Styled=Ruby") is a single long line. While it fits in the current 720p resolution, the text is approaching the container's horizontal edges, and at slightly narrower viewports it could overflow.
**Suggested fix:** Add text wrapping or horizontal scrolling to the summary bar, or reduce font size to ensure it stays within bounds at all supported resolutions.
