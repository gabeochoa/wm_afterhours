# Code Review: CozyCafe

**File:** `src/systems/screens/CozyCafe.h`
**Lines:** 800
**Estimated saveable lines:** ~220

## Screen-Level Suggestions

### 1. Texture loading is 9 near-identical lines (~12 lines saved)
**Current:** Lines 40-56 repeat the same `raylib::LoadTexture` pattern.
**Suggested:** Load helper lambda.

### 2. Star rating rendering has triple-branching for each star (~20 lines saved)
**Current:** Lines 193-227 loop over 5 stars with: `if (filled && tex) { sprite } else if (!filled && tex) { sprite } else { div }`. The two sprite branches are identical except for the texture.
**Suggested:** Simplify to: `auto& tex = is_filled ? star_filled_tex : star_empty_tex; if (tex.id != 0) { sprite } else { div }`.

### 3. Chat avatars are duplicated for 2 messages (~25 lines saved)
**Current:** Lines 560-631 repeat the avatar + message pattern for 2 chat lines.
**Suggested:** Use a `struct ChatMessage { tex_ptr, label, text }` array and loop.

### 4. Bottom-right icon buttons (Inventory/Research/Crafting) share identical structure (~40 lines saved)
**Current:** Lines 642-783 create 3 icon buttons with nearly identical code: button bg, sprite/fallback, badge, label, hint.
**Suggested:** Data-driven loop over `struct IconBtn { tex_ptr, badge_text, label, hint }`.

### 5. Customer row rendering repeats the same layout per customer (~15 lines saved)
**Current:** Lines 385-466 loop over customers with 5+ entities per customer.
**Suggested:** Extract `render_customer_row()` helper.

### 6. `std::bitset<4>(0b1111)` repeated ~15 times (~8 lines saved)
**Current:** Same pattern everywhere.
**Suggested:** Local constant.

### 7. Menu item buttons share identical sizing and styling (~10 lines saved)
**Current:** Lines 292-316 create 3 menu buttons with nearly identical configs.
**Suggested:** Loop already exists but configs are verbose — extract base button config.

### 8. Music slider is a manual slider (track + fill + handle) (~15 lines saved)
**Current:** Lines 493-527 manually create slider track, fill, and handle.
**Suggested:** Use the `slider()` component with custom theme colors.

### 9. Panel creation pattern repeated for left and right panels (~10 lines saved)
**Current:** Lines 260-366 create two panels with identical structure: bg + border + shadow + header.
**Suggested:** Extract `render_panel(x, y, w, h, header_label, header_color)`.

### 10. Progress bar in customer rows is manual (bg + fill + label) (~10 lines saved)
**Current:** Lines 412-449 manually create progress bar elements.
**Suggested:** Use `progress_bar()` component.

### 11. `.with_debug_name()` on decorative elements (~10 lines saved)
**Current:** Tape, decorative elements all have debug names.
**Suggested:** Remove from non-diagnostic elements.

### 12. Header section (title + gold + rating) has 8 separate positioned elements (~10 lines saved)
**Current:** Lines 119-248 create many individual positioned elements.
**Suggested:** Some could be grouped into containers.

### 13. `notification_badge` calls are clean but could pass color from data (~3 lines saved)
**Current:** Badge color is always `badge_red`.
**Suggested:** Already fine, minor.

### 14. Repeated `.with_font("Gaegu-Bold", h720(X))` pattern (~5 lines saved)
**Current:** Same font specified on 15+ elements.
**Suggested:** Set as default font or create a base config.

### 15. Icon offset calculations repeated for each icon button (~5 lines saved)
**Current:** `float icon_offset = (icon_size - icon_img_size) / 2.0f;` computed once but sizing repeated.
**Suggested:** Already clean, minor.

### 16. Promote button and Serve Next button share similar styling (~8 lines saved)
**Current:** Both are large action buttons with border + shadow + rounded + centered text.
**Suggested:** Shared base config for action buttons.

## Vendor-Level Suggestions

### V1. `.with_absolute(x, y)` shorthand (~45 lines saved)
Same as other screens.

### V2. `sprite_or_fallback()` helper (~15 lines saved)
Used for stars, avatars, icon buttons.

### V3. `.with_pill()` convenience (~10 lines saved)
Gold pill, time badges, slider track.

### V4. Default font setting per-screen (~10 lines saved)
**Current:** `UIStylingDefaults::get().set_default_font()` exists but most elements still specify `.with_font()`.
**Suggested:** If the default font works, remove explicit `.with_font()` calls.

## Summary
- Total screen-level suggestions: 16
- Total vendor-level suggestions: 4
- Estimated total lines saveable: ~220
