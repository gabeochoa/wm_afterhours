# Code Review: ShopInterface

**File:** `src/systems/screens/ShopInterface.h`
**Lines:** 658
**Estimated saveable lines:** ~180

## Screen-Level Suggestions

### 1. Item row loop creates 7 entities with verbose repeated configs (~40 lines saved)
**Current:** Lines 283-400 loop over items, creating: alt row bg, selection highlight, invisible button, icon, name, category, stock, price — each with full config chains.
**Suggested:** Extract `render_item_row(item, index, is_selected, y_pos)` helper.

### 2. Column header divs repeat the same pattern 4 times (~15 lines saved)
**Current:** Lines 228-267 create 4 header divs with identical font, color, and size patterns.
**Suggested:** Loop over `{{"Item", col_name_x}, {"Type", col_cat_x}, ...}`.

### 3. `format_gold()` is duplicated from EmpireTycoon (~10 lines saved)
**Current:** Lines 89-100 define the same number formatting function.
**Suggested:** Extract to a shared utility header.

### 4. Cart items loop is verbose for simple content (~10 lines saved)
**Current:** Lines 538-566 create name+quantity and subtotal divs with full configs.
**Suggested:** Reduce config chain by extracting shared text style.

### 5. Detail panel creates 6 entities that could share a base config (~15 lines saved)
**Current:** Lines 409-506 create icon, name, category, description, price, add button — each with unique but similar configs.
**Suggested:** Define a `detail_text_config` base and customize per element.

### 6. `std::bitset<4>(0b1111)` repeated ~10 times (~5 lines saved)
**Current:** Used for every rounded element.
**Suggested:** Local constant.

### 7. Panel creation pattern (bg + border + rounded + shadow) repeated 3 times (~8 lines saved)
**Current:** List panel, detail panel, cart panel all use the same 5-line pattern.
**Suggested:** Helper or shared base config: `auto panel_config = ComponentConfig{}.with_custom_background(panel_wood).with_border(border_wood, 2.0f)...;`

### 8. Buy/sell tab loop is clean but tab styling could be extracted (~5 lines saved)
**Current:** Lines 178-199 have clear loop structure but config is still verbose.
**Suggested:** Minor improvement — extract tab config builder.

### 9. Total/remaining display is 3 separate divs (~5 lines saved)
**Current:** Lines 580-615 create total label, total amount, remaining — similar configs.
**Suggested:** Could be a column container with children.

### 10. `afterhours::Color{0, 0, 0, 0}` for invisible button bg (~2 lines saved)
**Current:** Transparent button background.
**Suggested:** `afterhours::colors::transparent()`.

### 11. Alternating row background + selection highlight is complex (~10 lines saved)
**Current:** Two separate divs (alt bg + selection) before the actual row content.
**Suggested:** Single div with ternary color selection.

### 12. Purchase button has complex ternary for disabled state (~5 lines saved)
**Current:** Lines 618-639 have ternary expressions for bg, border, and text color.
**Suggested:** Pre-compute `auto [btn_bg, btn_border, btn_text] = can_afford_total ? ... : ...;`

### 13. Status bar prompt could be a simple one-liner (~3 lines saved)
**Current:** 8 lines of config for a single text label.
**Suggested:** Reduce to essential calls only.

### 14. Gold pill display (bg + text) could be a helper (~5 lines saved)
**Current:** Lines 152-172 create a pill + text, same pattern as EmpireTycoon's currency.
**Suggested:** Shared `render_currency_pill()`.

### 15. `.with_debug_name()` on every element including decorative ones (~10 lines saved)
**Current:** All elements named.
**Suggested:** Remove from separators, alt row bgs, static text.

### 16. Each `.with_absolute_position().with_translate(x, y)` pair (~15 lines saved)
**Current:** Two calls for every positioned element.
**Suggested:** Vendor improvement (see V1).

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~30 lines saved)
Same as other screens.

### V2. `.with_pill()` convenience (~10 lines saved)
Same as other screens.

### V3. `ComponentSize` float pair shorthand (~10 lines saved)
Same as other screens.

## Summary
- Total screen-level suggestions: 16
- Total vendor-level suggestions: 3
- Estimated total lines saveable: ~180
