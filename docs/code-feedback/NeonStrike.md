# Code Review: NeonStrike

**File:** `src/systems/screens/NeonStrike.h`
**Original lines:** 745 | **Current lines:** ~655
**Lines saved so far:** ~90 (batch refactor)
**Estimated remaining saveable lines:** ~105

## Completed
- ~~std::bitset<4>(0b1111) -> RoundedCorners() (~4 lines)~~
- ~~.with_absolute_position(x, y) combined call (~50 lines)~~
- ~~pixels(static_cast<int>(...)) -> pxf() lambda~~

## Remaining Screen-Level Suggestions

### 1. Texture loading is 11 near-identical lines (~15 lines saved)
**Suggested:** Use a load helper lambda.

### 2. Compass cardinal directions are 4 identical divs (~20 lines saved)
**Suggested:** Data-driven loop with `struct Cardinal { label, x, y, color }`.

### 3. Killstreak sidebar icons have 4 entities per icon (~15 lines saved)
**Suggested:** The cog indicator and label are optional and could be conditional with less nesting.

### 4. Map grid lines: vertical and horizontal loops are near-identical (~10 lines saved)
**Suggested:** Merge into one loop with direction parameter.

### 5. Health panel: manual progress bar (~15 lines saved)
**Suggested:** Use `progress_bar()` component with custom colors.

### 6. Armor bar also manual bg + fill (~10 lines saved)
**Suggested:** Use `progress_bar()`.

### 7. Equipment boxes (grenade + knife) share identical structure (~15 lines saved)
**Suggested:** Loop over `struct Equipment { tex, label, x, selected }`.

### 8. `icon_or_fallback` pattern for killstreak/equipment (~10 lines saved)
**Suggested:** A local `render_icon_or_text()` lambda.

### 9. `.with_debug_name()` on decorative elements (~10 lines saved)
**Suggested:** Remove from grid lines, tick marks, decorative elements.

## Remaining Vendor-Level Suggestions

### V1. `sprite_or_fallback()` helper (~15 lines saved)
Texture + fallback pattern appears 5+ times.

### V2. `.with_circle()` convenience (~10 lines saved)
For compass ticks, objective marker, danger zone.

## Summary
- Remaining screen-level suggestions: 9
- Remaining vendor-level suggestions: 2
- Estimated remaining lines saveable: ~105
