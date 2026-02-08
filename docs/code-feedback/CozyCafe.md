# Code Review: CozyCafe

**File:** `src/systems/screens/CozyCafe.h`
**Original lines:** 800 | **Current lines:** ~695
**Lines saved so far:** ~105 (batch refactor)
**Estimated remaining saveable lines:** ~115

## Completed
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~8 lines)~~
- ~~.with_absolute_position(x, y) combined call (~45 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Texture loading is 9 near-identical lines (~12 lines saved)
**Suggested:** Load helper lambda.

### 2. Star rating rendering has triple-branching for each star (~20 lines saved)
**Suggested:** Simplify to `auto& tex = is_filled ? star_filled : star_empty; if (tex.id != 0) { sprite } else { div }`.

### 3. Chat avatars duplicated for 2 messages (~25 lines saved)
**Suggested:** Use a `struct ChatMessage` array and loop.

### 4. Bottom-right icon buttons share identical structure (~40 lines saved)
**Suggested:** Data-driven loop over `struct IconBtn { tex_ptr, badge_text, label, hint }`.

### 5. Customer row rendering repeats per customer (~15 lines saved)
**Suggested:** Extract `render_customer_row()` helper.

### 6. Menu item buttons share identical sizing/styling (~10 lines saved)
**Suggested:** Extract base button config.

### 7. Music slider is manual (track + fill + handle) (~15 lines saved)
**Suggested:** Use the `slider()` component with custom theme colors.

### 8. Panel creation pattern repeated for left and right (~10 lines saved)
**Suggested:** Extract `render_panel()` helper.

### 9. Repeated `.with_font("Gaegu-Bold", h720(X))` pattern (~5 lines saved)
**Suggested:** Set as default font or create a base config.

### 10. `.with_debug_name()` on decorative elements (~10 lines saved)
**Suggested:** Remove from non-diagnostic elements.

## Remaining Vendor-Level Suggestions

### V1. `sprite_or_fallback()` helper (~15 lines saved)
Used for stars, avatars, icon buttons.

### V2. `.with_pill()` convenience (~10 lines saved)
Gold pill, time badges, slider track.

### V3. Default font setting per-screen (~10 lines saved)
If the default font works, remove explicit `.with_font()` calls.

## Summary
- Remaining screen-level suggestions: 10
- Remaining vendor-level suggestions: 3
- Estimated remaining lines saveable: ~115
