# Code Review: ParcelCorpsSettings

**File:** `src/systems/screens/ParcelCorpsSettings.h`
**Lines:** 1204
**Estimated saveable lines:** ~450

## Screen-Level Suggestions

### 1. Legend items are copy-paste with only icon/label/color changing (~100 lines saved)
**Current:** Lines 277-427 repeat the same pattern 6 times: an icon `div` + a label `div`, each with identical `.with_size`, `.with_absolute_position`, `.with_font_size`, `.with_rounded_corners`, `.with_roundness` calls.
**Suggested:**
```cpp
struct LegendItem { const char* icon; const char* label; afterhours::Color color; };
LegendItem legend[] = {{"@","Language",icon_purple}, {"~","Sync",icon_purple}, ...};
for (int i = 0; i < 6; i++) {
  int col = i % 2, row = i / 2;
  float lx = row_x + (col * legend_col_w) + 8.0f;
  float ly = legend_item_y + row * 22.0f;
  // one icon div + one label div using legend[i] data
}
```

### 2. `render_toggle_row_rainbow` duplicates 80% of `render_toggle_row_with_icon` (~50 lines saved)
**Current:** Two nearly identical methods (lines 734-888) differ only in the icon rendering (single color circle vs nested rainbow circles).
**Suggested:** Merge into one method with an `enum IconStyle { Single, Rainbow }` parameter.

### 3. `render_display_row_rainbow` duplicates `render_display_row` (~40 lines saved)
**Current:** Lines 981-1107 have two methods that differ only in icon rendering.
**Suggested:** Same as above — merge with an icon style parameter.

### 4. Repeated `std::bitset<4>(0b1111)` everywhere (~20 lines saved across 30+ uses)
**Current:** `.with_rounded_corners(std::bitset<4>(0b1111))` appears ~30 times.
**Suggested:** Define once: `const auto ALL_ROUND = std::bitset<4>(0b1111);` or use `RoundedCorners()`.

### 5. Repeated `pixels(static_cast<int>(...))` pattern (~30 lines saved)
**Current:** Dozens of calls like `.with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})`.
**Suggested:** Store pre-computed `int` sizes or use a helper: `auto px = [](float f) { return pixels(static_cast<int>(f)); };`

### 6. Every row helper repeats the same row background pattern (~25 lines saved)
**Current:** Each render method starts with a row background div with identical `.with_absolute_position()`, `.with_translate()`, `.with_custom_background(row_dark)`, `.with_rounded_corners()`, `.with_roundness()`.
**Suggested:** Extract a `render_row_bg()` helper.

### 7. Icon rendering pattern repeated across all helpers (~20 lines saved)
**Current:** Every helper renders a 28x28 icon circle with `.with_custom_background(icon_color)`, `.with_font_size(h720(14.0f))`, `.with_custom_text_color(text_white)`, `.with_alignment(TextAlignment::Center)`, `.with_rounded_corners()`, `.with_roundness(1.0f)`.
**Suggested:** Extract `render_icon(context, entity, id, x, y, symbol, color)`.

### 8. Repeated chevron/arrow button pattern (~20 lines saved)
**Current:** `<` and `>` buttons appear identically in `render_language_row` and `render_selector_row` with the same size/style.
**Suggested:** Extract `render_chevron_button(context, entity, id, x, y, label)`.

### 9. Chat messages could use a simpler loop (~15 lines saved)
**Current:** Lines 581-606 loop with two `div` calls per message using `320 + i*2` and `321 + i*2` entity IDs.
**Suggested:** Use a struct with computed positions and a single helper lambda.

### 10. HUD elements (LIVE, eye, diamond, score, username) share identical styling (~15 lines saved)
**Current:** Lines 509-574 have five separate `div` calls, all with `.with_absolute_position()`, `.with_font_size(h720(...))`, `.with_custom_text_color(text_white)`.
**Suggested:** Data-driven loop with `struct HudElement { label, size, x, y, font_size }`.

### 11. Volume slider `render_volume_slider` shares code with toggle rows (~10 lines saved)
**Current:** The row background + icon + label pattern is identical to toggle rows.
**Suggested:** Reuse `render_row_bg()` and `render_icon()` helpers.

### 12. Unnecessary `.with_debug_name()` on every element (~15 lines saved)
**Current:** Every single element has a debug name, including decorative ones like separators and battery indicators.
**Suggested:** Remove debug names from non-interactive, non-diagnosed elements. Keep only on rows, buttons, and panels.

### 13. Status bar elements could be a data loop (~10 lines saved)
**Current:** Lines 136-180 have 4 separate `div` calls for time, signal, battery.
**Suggested:** Group into a data array with position/label/style.

### 14. Quest panel items repeat the same pattern (~10 lines saved)
**Current:** Lines 448-505 repeat checkbox + level badge + text for each quest item.
**Suggested:** Use a `struct QuestItem` array and loop.

### 15. Speedometer section has 3 divs that could be nested (~5 lines saved)
**Current:** Lines 608-643 create 3 absolute-positioned elements for speed/unit/vehicle.
**Suggested:** Use a parent container with column layout.

### 16. `afterhours::Color{55, 60, 70, 180}` appears in multiple chevron buttons (~3 lines saved)
**Current:** The chevron background color is repeated inline.
**Suggested:** Define as `chevron_bg` member.

### 17. Phone frame constants could be computed from screen size (~3 lines saved)
**Current:** `phone_w`, `phone_h`, `phone_x`, `phone_y` are hardcoded.
**Suggested:** Already fine for a mockup, but the margin calculations are repeated below.

## Vendor-Level Suggestions

### V1. `ComponentConfig::with_pill()` convenience for rounded pill shapes (~60 lines across screens)
**Current:** `.with_rounded_corners(std::bitset<4>(0b1111)).with_roundness(X)` is a 2-line pattern used dozens of times.
**Suggested:** `.with_pill(0.5f)` or `.with_pill()` that sets both.

### V2. `ComponentConfig::with_circle()` for fully round elements (~30 lines across screens)
**Current:** `.with_rounded_corners(std::bitset<4>(0b1111)).with_roundness(1.0f)` for circle shapes.
**Suggested:** `.with_circle()` convenience method.

### V3. `ComponentSize` from two floats shorthand (~50 lines across screens)
**Current:** `.with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})`.
**Suggested:** `.with_size(pixels(w), pixels(h))` overload that accepts float and does the cast.

### V4. `.with_absolute(x, y)` combining position + translate (~100 lines across screens)
**Current:** `.with_absolute_position().with_translate(x, y)` is a 2-call pattern on every absolute element.
**Suggested:** `.with_absolute(x, y)` single call.

### V5. `ComponentConfig::inherit_style(other_config)` for copying visual style (~40 lines across screens)
**Current:** Multiple elements share the same font, color, alignment but must repeat all calls.
**Suggested:** Allow inheriting visual style (font, colors, alignment) from another config.

## Summary
- Total screen-level suggestions: 17
- Total vendor-level suggestions: 5
- Estimated total lines saveable: ~450
