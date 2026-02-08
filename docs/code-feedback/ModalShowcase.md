# Code Review: ModalShowcase

**File:** `src/systems/screens/ModalShowcase.h`
**Lines:** 607 (no batch changes applied — uses flexbox layout, not absolute positioning)
**Estimated remaining saveable lines:** ~150

## Remaining Screen-Level Suggestions

### 1. Section creation pattern repeated 5 times identically (~50 lines saved)
**Suggested:** Extract `make_section(id, label)` helper that returns the row entity.

### 2. Button config pattern repeated 8+ times (~30 lines saved)
**Suggested:** Define a `btn_base` config and customize per button.

### 3. `.with_render_layer(MODAL_CONTENT_LAYER)` on every modal child (~15 lines saved)
**Suggested:** May be a framework issue — modal children shouldn't need explicit render layers.

### 4. Button row pattern inside modals repeated 3 times (~15 lines saved)
**Suggested:** Extract `make_modal_button_row()`.

### 5. Result string conversion uses if/else chain (~5 lines saved)
**Suggested:** Use a `switch` or map.

### 6. `static bool` inside for_each_with for modal checkboxes (~0 lines but code smell)
**Suggested:** Move to member variables.

### 7. `.with_debug_name()` on every section and row (~10 lines saved)
**Suggested:** Remove from sections and rows.

### 8. Margin patterns repeated (`Margin{.bottom = DefaultSpacing::small()}`) (~5 lines saved)
**Suggested:** Part of the `make_section` helper.

### 9. `FlexDirection::Row` + `AlignItems::Center` + `JustifyContent::FlexStart` repeated 5 times (~5 lines saved)
**Suggested:** Part of the `make_section` helper.

### 10. Composable modal checkbox row could be simpler (~3 lines saved)
**Suggested:** Minor — already clean enough.

## Remaining Vendor-Level Suggestions

### V1. Modal children should inherit render layer from parent (~15 lines per modal screen)
Every child element inside a modal must specify `.with_render_layer(MODAL_CONTENT_LAYER)`.

### V2. `.with_row_layout()` convenience (~10 lines across screens)
`FlexDirection::Row + AlignItems::Center` as a single call.

### V3. `ComponentConfig` presets for common UI patterns (~20 lines across screens)
Section style (surface bg, padding, roundness, column direction, margin).

## Summary
- Remaining screen-level suggestions: 10
- Remaining vendor-level suggestions: 3
- Estimated remaining lines saveable: ~150
