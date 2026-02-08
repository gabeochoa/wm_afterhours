# Code Review: EmpireTycoon

**File:** `src/systems/screens/EmpireTycoon.h`
**Lines:** 944
**Estimated saveable lines:** ~300

## Screen-Level Suggestions

### 1. Texture loading is 14 near-identical lines (~20 lines saved)
**Current:** Lines 43-68 repeat `xxx_tex = raylib::LoadTexture((images_path + "filename.png").c_str());` 14 times.
**Suggested:**
```cpp
auto load = [&](const char* name) {
  return raylib::LoadTexture((images_path + name).c_str());
};
coin_tex = load("icon_coin_small.png");
```

### 2. Status icon rendering is duplicated for happiness/resources (~30 lines saved)
**Current:** Lines 304-351 repeat the same icon circle + sprite pattern for two status icons.
**Suggested:** Use a loop over `{icon_happiness_tex, happiness_color, x_offset}` structs.

### 3. Meter rendering (happiness + resources) is duplicated (~50 lines saved)
**Current:** Lines 360-514 repeat: pill bg, icon, label, bar bg, bar fill, percentage label — twice.
**Suggested:** Extract `render_meter(label, pct, icon, colors, y_pos)`.

### 4. Tab loop has icon fallback that could be simplified (~15 lines saved)
**Current:** Lines 574-594 have an `if (tex_ptr && tex_ptr->id != 0) { sprite... } else { div... }` pattern.
**Suggested:** Create a `render_icon_or_fallback()` helper used here and in bottom icons.

### 5. Bottom-right icons repeat the same button+sprite+label pattern (~25 lines saved)
**Current:** Lines 888-938 loop over icons but still have verbose config chains.
**Suggested:** Reduce the config chain by extracting shared button style.

### 6. Production and project items share the same layout pattern (~20 lines saved)
**Current:** Lines 679-760 have two nearly identical loops: label + up arrow.
**Suggested:** Merge into one loop with different data source.

### 7. `std::bitset<4>(0b1111)` repeated ~15 times (~8 lines saved)
**Current:** Same as ParcelCorps.
**Suggested:** `const auto ROUND_ALL = std::bitset<4>(0b1111);`

### 8. Action buttons loop already exists but config is still verbose (~10 lines saved)
**Current:** Lines 769-789 loop over button labels but each has 10+ config calls.
**Suggested:** Extract shared button style into a base config variable.

### 9. `afterhours::Color{0, 0, 0, 50}` shadow color repeated ~5 times (~3 lines saved)
**Current:** Shadow colors are inlined.
**Suggested:** Define `const afterhours::Color shadow_light{0, 0, 0, 50};`

### 10. Currency pill + text is 3 elements that could use a helper (~8 lines saved)
**Current:** Lines 249-297 create pill bg, coin icon, money text separately.
**Suggested:** A `render_currency_display()` helper.

### 11. Cloud sprites duplicate the same source rect calculation (~5 lines saved)
**Current:** Lines 168-184 compute `src` for each cloud identically.
**Suggested:** Compute `src` once before the if block.

### 12. `format_money` could be a free function or utility (~0 lines but better reuse)
**Current:** Member function, also duplicated in ShopInterface.
**Suggested:** Move to a shared utility header.

### 13. Title text (DREAM/INCORPORATED) has shadow+main = 4 similar divs (~15 lines saved)
**Current:** Lines 192-241 create 4 `div`s with nearly identical configs, just offset positions and colors.
**Suggested:** A `render_outlined_text(label, x, y, size, main_color, stroke_color, shadow_offset)` helper.

### 14. Chat box lines duplicate the same pattern (~5 lines saved)
**Current:** Lines 804-822 have 2 identical chat line divs.
**Suggested:** Loop over chat messages.

### 15. Bottom milestone bar is a manual progress bar (~10 lines saved)
**Current:** Lines 838-875 manually create bar bg + fill + percentage.
**Suggested:** Use the existing `progress_bar()` component.

### 16. `.with_debug_name()` on every element (~15 lines saved)
**Current:** Every element has a debug name.
**Suggested:** Remove from decorative/static elements.

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~50 lines saved)
Same as ParcelCorps V4.

### V2. `.with_pill()` / `.with_circle()` convenience (~20 lines saved)
Same as ParcelCorps V1/V2.

### V3. `sprite_or_fallback()` function (~20 lines across screens)
**Current:** Multiple screens have `if (tex.id != 0) { sprite... } else { div... }` pattern.
**Suggested:** A `sprite_or_fallback(ctx, ep, tex, src, fallback_label, config)` helper.

### V4. `ComponentSize` from float pair (~15 lines saved)
Same as ParcelCorps V3.

## Summary
- Total screen-level suggestions: 16
- Total vendor-level suggestions: 4
- Estimated total lines saveable: ~300
