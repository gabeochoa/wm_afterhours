# Color Picker

**Status:** Not implemented  
**Priority:** Low

## Problem

No way for users to select colors.

## Suggested Implementation

```cpp
ElementResult color_picker(HasUIContext auto &ctx, EntityParent ep_pair,
                           Color &value,
                           ComponentConfig config = ComponentConfig());
```

## Features Needed
- HSV color wheel or square
- RGB/HSV sliders
- Alpha channel support
- Hex input
- Color preview (old vs new)

---

## Example Screen: ColorPickerShowcase

**File:** `src/systems/screens/ColorPickerShowcase.h`
**CLI:** `--screen=color_picker`
**Category:** Widgets

### Layout

A color selection screen with multiple picker variants:

1. **HSV Picker** — A large color square (saturation x brightness) with a vertical hue strip beside it. Clicking/dragging the square sets saturation and value; dragging the hue strip sets the hue. The selected color fills a large preview swatch.

2. **RGB Sliders** — Three sliders (R, G, B) each 0-255 with colored channel fills. An alpha slider below. All linked bidirectionally to the HSV picker.

3. **Hex Input** — A `text_input` showing the hex value (e.g., `#3498DB`). Typing a valid hex updates all other controls. Invalid input shows red border.

4. **Preview Swatches** — "Old" color swatch (before editing) beside "New" color swatch, so the user can compare.

5. **Preset Palette** — A grid of 16 preset colors. Clicking one sets it as the current color.

### Features Exercised

- `color_picker()` with HSV square + hue strip
- RGB/HSV bidirectional conversion
- Alpha channel support
- Hex input parsing and formatting
- Color preview (old vs new comparison)
- Preset palette grid

### Verification

- Dragging on HSV square updates RGB sliders and hex in real time
- Typing `#FF0000` in hex input: HSV picker moves to red, RGB shows 255/0/0
- Alpha slider at 50%: preview swatch shows semi-transparent color
- Clicking a preset color updates all controls
- "Old" swatch never changes during editing

### E2E Test Plan

**Test file:** `src/testing/tests/ColorPickerTest.h`

#### New Custom Commands Needed

- `drag_from_to(from_pos, to_pos, hold_frames)` — click at position, drag to position, release. Needed for dragging on the HSV square and hue strip.
- `type_into_field(label, text)` — focus text input and type. Needed for hex input.

#### Screenshots

1. `color_picker_initial` — default color selected, all controls visible
2. `color_picker_hex_red` — after typing #FF0000, picker shows red
3. `color_picker_preset_selected` — after clicking a preset color
4. `color_picker_alpha_half` — alpha slider at 50%, preview showing transparency

#### Test Script

```cpp
TEST(color_picker_hex_input) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("color_picker_initial");

  // Type hex color
  type_into_field("Hex Input", "#FF0000");
  co_await TestApp::wait_for_frames(2);
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);

  // RGB sliders should update: R=255, G=0, B=0
  TestApp::expect_ui_exists("255");
  auto snap = TestApp::capture_snapshot("color_picker_hex_red");
}

TEST(color_picker_preset) {
  co_await TestApp::wait_for_frames(5);

  // Click a preset color
  TestApp::click_button("Preset 1");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("color_picker_preset_selected");
}
```

