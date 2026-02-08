# Code Review: NeonStrike

**File:** `src/systems/screens/NeonStrike.h`
**Lines:** 745
**Estimated saveable lines:** ~200

## Screen-Level Suggestions

### 1. Texture loading is 11 near-identical lines (~15 lines saved)
**Current:** Lines 90-111 repeat `xxx_tex = raylib::LoadTexture((images_path + "name").c_str())` 11 times.
**Suggested:** Use a load helper lambda (same pattern as EmpireTycoon).

### 2. Compass cardinal directions are 4 identical divs (~20 lines saved)
**Current:** Lines 180-222 create N, S, W, E with identical config except label and position.
**Suggested:**
```cpp
struct Cardinal { const char* label; float x, y; afterhours::Color color; };
Cardinal dirs[] = {{"N", cx-8, top-17, text_tan}, {"S", cx-8, top+size-3, text_muted}, ...};
for (int i = 0; i < 4; i++) { /* one div */ }
```

### 3. Compass tick marks loop is already data-driven but could be shorter (~5 lines saved)
**Current:** Lines 237-253 have clean loop but verbose config.
**Suggested:** Minor — the config chain could be shorter.

### 4. Killstreak sidebar icons have 4 entities per icon (~15 lines saved)
**Current:** Lines 330-394 loop over icons with: cog indicator, icon bg, icon image/fallback, label.
**Suggested:** The cog indicator and label are optional and could be conditional with less nesting.

### 5. Map grid lines: vertical and horizontal loops are near-identical (~10 lines saved)
**Current:** Lines 428-447 have two separate loops for vertical and horizontal grid lines.
**Suggested:** Merge into one loop with direction parameter, or a single loop creating both.

### 6. Health panel: label + bar_bg + bar_fill is a manual progress bar (~15 lines saved)
**Current:** Lines 546-609 manually create health bar bg + fill + label.
**Suggested:** Use `progress_bar()` component with custom colors.

### 7. Armor bar also manually creates bg + fill (~10 lines saved)
**Current:** Lines 617-645 repeat the bar pattern for armor.
**Suggested:** Same — use `progress_bar()`.

### 8. Equipment boxes (grenade + knife) share identical structure (~15 lines saved)
**Current:** Lines 648-727 create two equipment boxes with the same bg + sprite + label pattern.
**Suggested:** Loop over `struct Equipment { tex, label, x, selected }`.

### 9. Kill feed loop is already clean (~0 lines saved)
**Current:** Lines 294-306 loop over kill feed messages.
**Suggested:** Already well-structured.

### 10. Member variables for layout are well-organized but some are unused (~5 lines saved)
**Current:** `compass_font_size`, `font_small/medium/normal/large/xlarge` are defined as members.
**Suggested:** Move font sizes used once to local scope. Keep only truly reused ones as members.

### 11. Score + objective section has 3 divs with similar styling (~8 lines saved)
**Current:** Lines 256-291 create score text, score icon, objective banner.
**Suggested:** Could share a base config for positioning style.

### 12. `icon_or_fallback` pattern for killstreak/equipment (~10 lines saved)
**Current:** `if (tex.id != 0) { sprite } else { div }` appears for killstreak icons and equipment.
**Suggested:** A local `render_icon_or_text()` lambda.

### 13. `std::bitset<4>(0b1111)` repeated ~8 times (~4 lines saved)
**Current:** Same as other screens.
**Suggested:** Local constant.

### 14. Killstreak progress bar could use `progress_bar()` (~10 lines saved)
**Current:** Lines 492-525 manually create bar bg + boxes.
**Suggested:** The boxes are custom, but the bar container is standard.

### 15. `.with_debug_name()` on decorative elements (~10 lines saved)
**Current:** Grid lines, tick marks, decorative elements all named.
**Suggested:** Remove from decorative-only elements.

### 16. Ammo display is a single large text div (~0 lines saved)
**Current:** Clean and simple.
**Suggested:** Already good.

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~50 lines saved)
Same pattern — every element uses `.with_absolute_position().with_translate(x, y)`.

### V2. `sprite_or_fallback()` helper (~15 lines saved)
Texture + fallback pattern appears 5+ times.

### V3. `.with_circle()` convenience (~10 lines saved)
For compass ticks, objective marker, danger zone.

## Summary
- Total screen-level suggestions: 16
- Total vendor-level suggestions: 3
- Estimated total lines saveable: ~200
