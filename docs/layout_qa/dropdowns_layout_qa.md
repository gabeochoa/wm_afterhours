# Layout QA: dropdowns

**Screenshots analyzed:** 5

## Issues Found

### 1. Dropdown menu overflows parent container card and overlaps card below
**Type:** overflow
**Screenshot:** dropdown_open_initial.png, dropdown_arrow_down_once.png, dropdown_arrow_down_twice.png
**Detail:** When the "With Label" dropdown (center-top card) is opened, the dropdown option list ("Dog", "Cat", "Bird", "Fish", "Disabled") extends downward past the bottom edge of its parent card container and visually overlaps the "Disabled" card in the row below. The dropdown list covers the "Disabled" card's title bar area, creating visual ambiguity about whether "Disabled" is a dropdown option or the card header beneath.
**Suggested fix:** Either render the dropdown menu in an overlay/portal layer that floats above all cards with a distinct shadow boundary, or constrain the dropdown to scroll within the card bounds using max-height with overflow-y scroll. If the overlay approach is used, ensure the dropdown has a clear visual border/shadow so it reads as floating above the grid.

### 2. Bottom status bar text may be clipped at narrower viewports
**Type:** container_bounds
**Screenshot:** dropdown_after_arrow_select.png, idle_720p.png
**Detail:** The bottom summary bar ("Basic=Alpha, Pet=Bird, Month=January, Indicator=Option A, Disabled=Frozen, Styled=Ruby") is a single long line that approaches the horizontal edges of its container. At 720p it fits, but the margins are tight.
**Suggested fix:** Add text wrapping or horizontal scrolling to the summary bar to ensure it stays within bounds at all supported resolutions.
