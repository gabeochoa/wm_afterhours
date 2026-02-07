# Button Variants

**Status:** Partially implemented  
**Priority:** Low

> ✅ `button()`, `button_group()`, and `image_button()` exist in afterhours.  
> ❌ Button variants (Outline, Ghost) and icon+text buttons not yet implemented.

## Problem

Only a single `button()` function exists. Game UIs need multiple button styles.

## Suggested Implementation

```cpp
enum struct ButtonVariant { Filled, Outline, Ghost, Icon };

ElementResult button(HasUIContext auto &ctx, EntityParent ep_pair,
                     ComponentConfig config = ComponentConfig(),
                     ButtonVariant variant = ButtonVariant::Filled);

// Or via config:
ComponentConfig{}.with_button_variant(ButtonVariant::Outline);
```

---

# Icon + Text Button

**Status:** Not implemented  
**Priority:** Medium

## Problem

No built-in way to create buttons with both an icon and label.

## Suggested Implementation

```cpp
ElementResult icon_button(HasUIContext auto &ctx, EntityParent ep_pair,
                          texture_manager::Texture icon,
                          texture_manager::Rectangle icon_rect,
                          ComponentConfig config = ComponentConfig());

// Or via config
ComponentConfig{}
    .with_label("Settings")
    .with_icon(gear_texture, gear_rect)
    .with_icon_position(IconPosition::Left);  // Left, Right, Top, Bottom
```

---

## Example Screen: ButtonVariantsShowcase

**File:** `src/systems/screens/ButtonVariantsShowcase.h`
**CLI:** `--screen=button_variants`
**Category:** Widgets

### Layout

A grid displaying all button variants with states:

1. **Variant Row: Filled** — 4 buttons (Normal, Hovered, Active/Pressed, Disabled) in the default filled style with primary theme color.

2. **Variant Row: Outline** — Same 4 states but with `ButtonVariant::Outline` — transparent background, colored border.

3. **Variant Row: Ghost** — Same 4 states with `ButtonVariant::Ghost` — no background or border, text only, hover reveals a subtle background.

4. **Icon+Text Buttons** — A row of buttons with icons: Settings (gear+text), Save (disk+text), Delete (trash+text). Icon positions: Left, Right, Top, Bottom variants.

5. **Size Variants** — Small, Medium, Large buttons side by side to show size scaling.

6. **Theme Comparison** — The same filled/outline/ghost buttons rendered in 3 different themes (neon dark, kraft, pastel blue) side by side.

### Features Exercised

- `ButtonVariant::Filled`, `Outline`, `Ghost`
- `with_button_variant()` on ComponentConfig
- `with_icon()` and `with_icon_position()` for icon+text buttons
- `with_disabled()` interaction with variants
- Theme color application to each variant

### Verification

- Filled button has solid background, text with auto-contrast color
- Outline button has transparent background with visible border
- Ghost button has no background until hovered
- Disabled variant is visually muted regardless of variant type
- Icon renders at the specified position relative to text

### E2E Test Plan

**Test file:** `src/testing/tests/ButtonVariantsTest.h`

#### New Custom Commands Needed

- `hover_element(label)` — move mouse to element center without clicking. Needed to verify Ghost variant's hover-reveal background.

#### Screenshots

1. `button_variants_filled` — filled variant row showing normal/hovered/active/disabled states
2. `button_variants_outline` — outline variant row
3. `button_variants_ghost` — ghost variant row (no background visible)
4. `button_variants_ghost_hovered` — ghost variant with hover, showing subtle background appear
5. `button_variants_icon_text` — icon+text buttons in all icon positions
6. `button_variants_sizes` — small/medium/large buttons side by side
7. `button_variants_themed` — same buttons in 3 different themes

#### Test Script

```cpp
TEST(button_variants_click) {
  co_await TestApp::wait_for_frames(5);

  auto snap_filled = TestApp::capture_snapshot("button_variants_filled");

  // Click each variant to verify it works
  TestApp::click_button("Filled Normal");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::click_button("Outline Normal");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::click_button("Ghost Normal");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);
}

TEST(button_variants_disabled) {
  co_await TestApp::wait_for_frames(5);

  // Disabled button should not respond
  TestApp::click_button("Filled Disabled");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // No click event should fire (verify via absence of click counter)
}

TEST(button_variants_ghost_hover) {
  co_await TestApp::wait_for_frames(5);

  auto snap_no_hover = TestApp::capture_snapshot("button_variants_ghost");

  hover_element("Ghost Normal");
  co_await TestApp::wait_for_frames(5);

  auto snap_hovered = TestApp::capture_snapshot("button_variants_ghost_hovered");
  // Visual diff: ghost button should show subtle background when hovered
}

TEST(button_variants_visual_comparison) {
  co_await TestApp::wait_for_frames(5);

  auto snap_outline = TestApp::capture_snapshot("button_variants_outline");
  auto snap_icons = TestApp::capture_snapshot("button_variants_icon_text");
  auto snap_sizes = TestApp::capture_snapshot("button_variants_sizes");
  auto snap_themes = TestApp::capture_snapshot("button_variants_themed");
}
```

