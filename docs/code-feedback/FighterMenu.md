# Code Review: FighterMenu

**File:** `src/systems/screens/FighterMenu.h`
**Lines:** 641
**Estimated saveable lines:** ~180

## Screen-Level Suggestions

### 1. Bottom button prompts are 10 entities with repeated patterns (~40 lines saved)
**Current:** Lines 497-635 create: prompt bg, D-pad + label, A button + label, B button + label, L/R buttons + label — each as separate positioned divs.
**Suggested:** Data-driven loop:
```cpp
struct Prompt { const char* btn; const char* label; afterhours::Color color; float x; };
Prompt prompts[] = {{"+", "Select", gray, 0}, {"A", "Confirm", red, spacing}, ...};
```

### 2. L and R bumper indicators are nearly identical (~15 lines saved)
**Current:** Lines 188-256 create L and R bumpers with identical configs except label and position.
**Suggested:** Loop over `{{"L", x_left}, {"R", x_right}}`.

### 3. LayoutConfig struct has ~30 floats but many are only used once (~0 lines saved, but cleaner)
**Current:** Lines 21-78 define a large config struct.
**Suggested:** Good practice for configurability, but values used only once could be local. Keep as-is for readability.

### 4. Tab button loop is clean but could pre-compute selected shadow config (~5 lines saved)
**Current:** Lines 207-237 build config then conditionally add shadow.
**Suggested:** Minor — `config = is_selected ? config.with_soft_shadow(...) : config;`

### 5. Menu option loop creates icon + item bar — two configs with shared attributes (~15 lines saved)
**Current:** Lines 275-332 build `icon_config` and `item_config` separately, then conditionally add shadow to each.
**Suggested:** Share common attributes via a base config.

### 6. Character area has 4 divs (offline label, bg panel, character bg, character text, character label) (~10 lines saved)
**Current:** Lines 334-399 create 5 positioned elements.
**Suggested:** Some could be nested (character bg + text inside bg panel).

### 7. Card panel has 8 entities for what could be a structured card layout (~15 lines saved)
**Current:** Lines 401-495 create: card bg, edge, top edge, icon, title, desc bg, desc text.
**Suggested:** Use a column container with children instead of absolute positioning.

### 8. `std::bitset<4>(0b1111)` repeated ~8 times (~4 lines saved)
**Current:** For bumpers, icon, card.
**Suggested:** Local constant.

### 9. `afterhours::Color{0, 0, 0, X}` shadow colors repeated inline (~3 lines saved)
**Current:** Multiple inline shadow color definitions.
**Suggested:** Use `shadow_color` member (already defined but not used for all shadows).

### 10. `.with_absolute_position().with_translate(x, y)` everywhere (~20 lines saved)
**Current:** Every element uses this two-call pattern.
**Suggested:** Vendor improvement.

### 11. Selected menu item conditionally adds shadow — duplicated for icon and item (~5 lines saved)
**Current:** `if (is_selected) { config = config.with_soft_shadow(...); }` appears twice per menu item.
**Suggested:** Compute shadow once, apply to both.

### 12. Tab underline is a separate element (~3 lines saved)
**Current:** Line 259-266 creates a decorative green line under tabs.
**Suggested:** Could be a border-bottom on the tab bar, but it spans wider than the tabs so it's fine as-is.

### 13. `.with_debug_name()` on every element (~10 lines saved)
**Current:** All named.
**Suggested:** Remove from decorative elements.

### 14. Description background + text are two separate positioned divs (~5 lines saved)
**Current:** Lines 472-495 create bg + text.
**Suggested:** Single div with padding and background.

### 15. Currency display is a single div — already clean (~0 lines saved)
**Current:** Line 171-180.
**Suggested:** Already good.

### 16. L/R prompt buttons share the same config as L bumper (~8 lines saved)
**Current:** Lines 595-625 create two small buttons that match the top bumpers.
**Suggested:** Extract a `render_bumper_button()` helper.

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~30 lines saved)
Same as other screens.

### V2. `.with_pill()` / `.with_circle()` convenience (~8 lines saved)
For bumper buttons.

### V3. `ComponentSize` float shorthand (~8 lines saved)
Same as other screens.

## Summary
- Total screen-level suggestions: 16
- Total vendor-level suggestions: 3
- Estimated total lines saveable: ~180
