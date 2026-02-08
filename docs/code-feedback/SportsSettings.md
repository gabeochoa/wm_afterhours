# Code Review: SportsSettings

**File:** `src/systems/screens/SportsSettings.h`
**Lines:** 769
**Estimated saveable lines:** ~200

## Screen-Level Suggestions

### 1. Setting row data uses verbose struct initialization (~30 lines saved)
**Current:** Lines 43-160 define settings with lots of empty fields: `{label, false, 0.0f, 0, 0, {options}, idx}`. Most non-slider settings have `false, 0.0f, 0, 0` as dead fields.
**Suggested:** Use two separate structs (`SliderSetting` / `OptionSetting`) or use a variant, or at minimum use named initializers: `{.label = "Window mode", .options = {...}, .option_idx = 1}`.

### 2. `get_current_settings()` and `format_slider_value()` both switch on `selected_tab` (~15 lines saved)
**Current:** Lines 162-269 have two separate switch statements mapping tab index to settings vector.
**Suggested:** Store a `std::vector<SettingRow>*` pointer once at the top of `for_each_with`.

### 3. LB and RB buttons are near-identical (~15 lines saved)
**Current:** Lines 349-423 create two button/div configs that differ only in label ("LB"/"RB"), position, and click handler.
**Suggested:** Loop over `{{"LB", x1, -1}, {"RB", x2, +1}}`.

### 4. Each settings row creates 5+ entities with repeated absolute positioning (~40 lines saved)
**Current:** Lines 450-645 loop over rows but each row creates: bg div, accent div, label button, left arrow, value text, right arrow, plus optional slider/segment visuals — all with verbose configs.
**Suggested:** Extract a `render_setting_row()` helper that takes the setting data and handles all elements.

### 5. Left/right arrow buttons are duplicated per row (~20 lines saved)
**Current:** Arrow buttons share identical styling but are created separately.
**Suggested:** Extract `render_arrow_button(ctx, entity, id, x, y, direction, on_click)`.

### 6. Segmented indicator loop creates verbose configs (~15 lines saved)
**Current:** Lines 613-644 loop over segments with long config chains.
**Suggested:** The config chain could inherit from a shared base.

### 7. `get_setting_description` and `get_setting_description_line2` are split unnecessarily (~10 lines saved)
**Current:** Two functions return line 1 and line 2 of the same description.
**Suggested:** Return a `std::pair<string, string>` from one function.

### 8. Help/info panel creates 4 nearly identical divs (~10 lines saved)
**Current:** Lines 667-709 create title, desc1, desc2, current — all with similar configs.
**Suggested:** Data-drive with `struct { label, y_offset, color }` array.

### 9. Bottom button prompts (Y, B) repeat the same pattern (~10 lines saved)
**Current:** Lines 715-763 have 4 divs (2 button icons + 2 labels) with identical styling.
**Suggested:** Loop over `{{"Y", "Reset to default", yellow}, {"B", "Back", red}}`.

### 10. `afterhours::Color{0, 0, 0, 0}` for transparent bg repeated 3x (~3 lines saved)
**Current:** Used for transparent button backgrounds.
**Suggested:** Use `afterhours::colors::transparent()`.

### 11. `.with_skip_tabbing(true)` on arrows and segments could be default for non-interactive elements (~5 lines saved)
**Current:** Applied manually on each decorative element.
**Suggested:** Already discussed in vendor suggestions.

### 12. `slider_empty_border` and `slider_track` are defined separately but used together (~2 lines saved)
**Current:** Two separate color members that always pair up.
**Suggested:** Minor — could be a struct, but not worth the complexity.

### 13. `selected_row` bounds check happens mid-render (~2 lines saved)
**Current:** Line 445 checks `selected_row >= current_settings.size()`.
**Suggested:** Move to the start of `for_each_with` before any rendering.

### 14. Tab underline is only rendered for selected tab inside the loop (~3 lines saved)
**Current:** `if (is_selected)` inside the tab loop creates one underline div.
**Suggested:** Move outside the loop — only one underline ever exists.

### 15. Keyboard input handling duplicates the left/right arrow click logic (~10 lines saved)
**Current:** Lines 310-333 handle keyboard, lines 519-578 handle mouse clicks — identical logic.
**Suggested:** Extract `adjust_setting_value(setting, direction)`.

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~40 lines saved)
Same as other screens.

### V2. `.with_transparent_bg()` convenience (~5 lines saved)
**Current:** `.with_custom_background(afterhours::Color{0, 0, 0, 0})`.
**Suggested:** `.with_transparent_bg()` or use `afterhours::colors::transparent()`.

### V3. `ComponentSize` float shorthand (~10 lines saved)
Same as other screens.

## Summary
- Total screen-level suggestions: 15
- Total vendor-level suggestions: 3
- Estimated total lines saveable: ~200
