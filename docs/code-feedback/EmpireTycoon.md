# Code Review: EmpireTycoon

**File:** `src/systems/screens/EmpireTycoon.h`
**Original lines:** 944 | **Current lines:** 865
**Lines saved so far:** ~79
**Estimated remaining saveable lines:** ~150

## Completed
- ~~Texture loading lambda (~20 lines)~~
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~8 lines)~~
- ~~.with_absolute_position(x, y) combined call (~50 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~
- ~~Title text shadow+main data-driven loop (~15 lines)~~
- ~~Chat box lines loop (~5 lines)~~

## Remaining Screen-Level Suggestions

### 1. Status icon rendering is duplicated for happiness/resources (~30 lines saved)
**Current:** The same icon circle + sprite pattern repeated for two status icons.
**Suggested:** Loop over `{icon_happiness_tex, happiness_color, x_offset}` structs.

### 2. Meter rendering (happiness + resources) is duplicated (~50 lines saved)
**Current:** pill bg, icon, label, bar bg, bar fill, percentage label — all repeated twice.
**Suggested:** Extract `render_meter(label, pct, icon, colors, y_pos)`.

### 3. Tab loop icon fallback could be simplified (~15 lines saved)
**Current:** `if (tex_ptr && tex_ptr->id != 0) { sprite } else { div }` pattern.
**Suggested:** Create a `render_icon_or_fallback()` helper.

### 4. Bottom-right icons verbose config chains (~25 lines saved)
**Current:** Loop over icons but still verbose config per icon.
**Suggested:** Extract shared button style.

### 5. Action buttons config is verbose (~10 lines saved)
**Current:** Loop over button labels but each has 10+ config calls.
**Suggested:** Extract shared button style into a base config variable.

### 6. `afterhours::Color{0, 0, 0, 50}` shadow color repeated ~5 times (~3 lines saved)
**Current:** Shadow colors inlined.
**Suggested:** Define `const afterhours::Color shadow_light{0, 0, 0, 50};`

### 7. Cloud sprites duplicate source rect calculation (~5 lines saved)
**Current:** `src` computed for each cloud identically.
**Suggested:** Compute `src` once before the if block.

### 8. `format_money` duplicated in ShopInterface (~0 lines but better reuse)
**Current:** Member function, also duplicated in ShopInterface.
**Suggested:** Move to a shared utility header.

### 9. Bottom milestone bar is a manual progress bar (~10 lines saved)
**Current:** Manually creates bar bg + fill + percentage.
**Suggested:** Use the existing `progress_bar()` component.

### 10. `.with_debug_name()` on every element (~15 lines saved)
**Current:** Every element has a debug name.
**Suggested:** Remove from decorative/static elements.

## Remaining Vendor-Level Suggestions

### V1. `.with_pill()` / `.with_circle()` convenience (~20 lines saved)
Same as ParcelCorps.

### V2. `sprite_or_fallback()` function (~20 lines across screens)
Multiple screens have `if (tex.id != 0) { sprite } else { div }` pattern.

### V3. `ComponentSize` from float pair (~15 lines saved)
Same as ParcelCorps.

## Summary
- Remaining screen-level suggestions: 10
- Remaining vendor-level suggestions: 3
- Estimated remaining lines saveable: ~150
