# Code Review: ModalShowcase

**File:** `src/systems/screens/ModalShowcase.h`
**Lines:** 607
**Estimated saveable lines:** ~150

## Screen-Level Suggestions

### 1. Section creation pattern repeated 5 times identically (~50 lines saved)
**Current:** Lines 91-371 create 5 sections with identical structure: section div, header div, row div. Each section is ~30 lines with the same config pattern.
**Suggested:**
```cpp
auto make_section = [&](int id, const char* label) {
  auto section = div(context, mk(main_container.ent(), id),
      ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(SECTION_HEIGHT)})
          .with_background(Theme::Usage::Surface)
          .with_padding(Spacing::md).with_roundness(SECTION_ROUNDNESS)
          .with_flex_direction(FlexDirection::Column)
          .with_margin(Margin{.bottom = DefaultSpacing::small()}));
  div(context, mk(section.ent(), 0),
      ComponentConfig{}.with_label(label)
          .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
          .with_background(Theme::Usage::Primary).with_auto_text_color(true)
          .with_padding(Spacing::sm)
          .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
          .with_alignment(TextAlignment::Left)
          .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));
  return div(context, mk(section.ent(), 1),
      ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(ROW_HEIGHT)})
          .with_flex_direction(FlexDirection::Row)
          .with_align_items(AlignItems::Center)
          .with_justify_content(JustifyContent::FlexStart));
};
auto row1 = make_section(1, "BASIC MODALS");
auto row2 = make_section(2, "HELPER FUNCTIONS");
```

### 2. Button config pattern repeated 8+ times in sections (~30 lines saved)
**Current:** Every button in sections 1-4 repeats: `.with_size()`, `.with_background()`, `.with_auto_text_color(true)`, `.with_font()`, `.with_roundness()`.
**Suggested:** Define a base button config:
```cpp
auto btn_base = ComponentConfig{}.with_size(ComponentSize{pixels(180), pixels(BUTTON_HEIGHT)})
    .with_auto_text_color(true).with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
    .with_roundness(BUTTON_ROUNDNESS);
```

### 3. `.with_render_layer(MODAL_CONTENT_LAYER)` on every modal child (~15 lines saved)
**Current:** Every element inside a modal has `.with_render_layer(MODAL_CONTENT_LAYER)`.
**Suggested:** This might be a framework issue — modal children shouldn't need explicit render layers.

### 4. Button row pattern inside modals repeated 3 times (~15 lines saved)
**Current:** Basic modal, composable modal, and settings modal each create an identical button row div.
**Suggested:** Extract `make_modal_button_row()`.

### 5. Result string conversion uses if/else chain (~5 lines saved)
**Current:** Lines 374-388 convert `DialogResult` to string with if/else.
**Suggested:** Use a `switch` or map.

### 6. `static bool` inside for_each_with for modal checkboxes (~0 lines but code smell)
**Current:** Lines 462, 553 use `static bool` for checkbox state inside the render function.
**Suggested:** Move to member variables for clarity and correctness.

### 7. `.with_debug_name()` on every section and row (~10 lines saved)
**Current:** All sections, rows, and buttons have debug names.
**Suggested:** Remove from sections and rows — the labels already identify them.

### 8. Section header config is identical across all 5 sections (~10 lines saved)
**Current:** Each section header has the same 8-line config.
**Suggested:** Part of the `make_section` helper (suggestion 1).

### 9. Modal helper calls are already clean (~0 lines saved)
**Current:** `modal::info`, `modal::confirm`, `modal::fyi` are concise.
**Suggested:** Already good examples of the helper API.

### 10. Constants at the top are good but some are only used once (~0 lines saved)
**Current:** `SECTION_ROUNDNESS`, `BUTTON_ROUNDNESS`, etc.
**Suggested:** Good practice — keep as-is.

### 11. `ComponentConfig{}` initialized fresh each time (~3 lines saved)
**Current:** Each element starts with `ComponentConfig{}`.
**Suggested:** Use inherited configs where possible.

### 12. Margin patterns repeated (`Margin{.bottom = DefaultSpacing::small()}`) (~5 lines saved)
**Current:** Same margin on every section.
**Suggested:** Part of the `make_section` helper.

### 13. `FlexDirection::Row` + `AlignItems::Center` + `JustifyContent::FlexStart` repeated 5 times (~5 lines saved)
**Current:** Every row div has the same 3 calls.
**Suggested:** Part of the `make_section` helper.

### 14. Composable modal checkbox row could be simpler (~3 lines saved)
**Current:** Creates a flex row container just for 2 checkboxes.
**Suggested:** Minor — already clean enough.

### 15. `afterhours::ModalConfig` chain could be shorter (~3 lines saved)
**Current:** `.with_size(pixels(400), pixels(200)).with_title("Basic Modal").with_closed_by(...)`.
**Suggested:** Already clean, but could use named constants for common sizes.

## Vendor-Level Suggestions

### V1. Modal children should inherit render layer from parent (~15 lines saved per modal screen)
**Current:** Every child element inside a modal must specify `.with_render_layer(MODAL_CONTENT_LAYER)`.
**Suggested:** The modal system should automatically set render layer on descendant components.

### V2. `ComponentConfig` could have `.with_section_style()` or `.with_card_style()` presets (~20 lines across screens)
**Current:** The section pattern (surface bg, padding, roundness, column direction, margin) is repeated extensively.
**Suggested:** Preset configs for common UI patterns.

### V3. `.with_row_layout()` convenience for `FlexDirection::Row + AlignItems::Center` (~10 lines across screens)
**Current:** Three separate calls to set up a row container.
**Suggested:** `.with_row_layout()` or `.with_column_layout()` shortcuts.

## Summary
- Total screen-level suggestions: 15
- Total vendor-level suggestions: 3
- Estimated total lines saveable: ~150
