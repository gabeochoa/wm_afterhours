# Layout QA: shop_interface

**Screenshots analyzed:** 8

## Issues Found

### 1. Empty/blank row appears in item list when navigating
**Type:** container_bounds
**Screenshot:** shop_final.png, shop_navigated_item.png
**Detail:** When the selection cursor moves past certain items, an empty/blank row appears in the item list (visible between "Mana Potion" and "Fire Scroll"). This empty row takes up the same vertical space as a populated item row but contains no content, creating a visual gap in the list. The "Leather Armor" row disappears and is replaced by a blank highlighted area.
**Suggested fix:** Investigate why a blank row is rendered when the selected item scrolls or is navigated away. The list should not render empty rows; it should either hide the row or show the correct item data.

### 2. Icon badge text clipped in item list
**Type:** overflow
**Screenshot:** idle_720p.png, shop_buy_tab.png, shop_cart_details.png, shop_initial.png, shop_item_selected.png, shop_sell_tab.png
**Detail:** The icon badges in the left column (SWD, ARM, FIR, TCH, ANT, SHL) have their text tightly packed within the small square badge containers. "SWD" for Iron Sword appears slightly clipped at the right edge. This same issue appears to varying degrees on other 3-character badges.
**Suggested fix:** Widen the icon badge containers slightly, or reduce the font size within the badges, so that all abbreviations fit with adequate padding.

### 3. Cart panel price values crowd right container edge
**Type:** overflow
**Screenshot:** idle_720p.png, shop_buy_tab.png, shop_cart_details.png, shop_initial.png
**Detail:** In the cart section on the right side, the price values "150 G", "30 G", and the total "180 G" are positioned at the far right edge of the cart container. The "180 G" total value appears to press against the right border of the panel with minimal padding.
**Suggested fix:** Add right padding inside the cart container to ensure price values don't crowd the container's right border.
