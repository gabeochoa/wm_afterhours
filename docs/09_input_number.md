# Input Number (Int/Float)

**Status:** Not implemented  
**Priority:** Medium

## Problem

No specialized numeric input with validation, step buttons, or drag-to-adjust.

## Suggested Implementation

```cpp
ElementResult input_int(HasUIContext auto &ctx, EntityParent ep_pair,
                        int &value, int min, int max, int step = 1,
                        ComponentConfig config = ComponentConfig());

ElementResult input_float(HasUIContext auto &ctx, EntityParent ep_pair,
                          float &value, float min, float max, float step = 0.1f,
                          ComponentConfig config = ComponentConfig());
```

## Features Needed
- +/- buttons
- Direct text entry with validation
- Min/max clamping
- Optional drag-to-adjust

---

## Example Screen: InputNumberShowcase

**File:** `src/systems/screens/InputNumberShowcase.h`
**CLI:** `--screen=input_number`
**Category:** Widgets

### Layout

A form-style screen with numeric inputs:

1. **Integer Input** — "Player Count" with `input_int(value, 1, 16, 1)`. Shows +/- buttons and direct text entry. Label shows current value.

2. **Float Input** — "Volume" with `input_float(value, 0.0, 1.0, 0.05)`. Two decimal places displayed.

3. **Drag-to-Adjust** — "Rotation (degrees)" with `input_float(value, 0, 360, 1)`. Click and drag left/right on the number to adjust. Shift+drag for fine control (0.1 step).

4. **Clamping Demo** — An integer input with range 0-100. Typing "999" in the text field: on submit, value clamps to 100. Typing "-5" clamps to 0.

5. **Validation Feedback** — An input that only accepts even numbers. Odd values show a red border and "Must be even" error text.

### Features Exercised

- `input_int()` with min/max/step
- `input_float()` with min/max/step
- +/- button click and long-press rapid increment
- Direct text entry with validation
- Min/max clamping on submit
- Optional drag-to-adjust interaction

### Verification

- +/- buttons increment/decrement by step amount
- Typing an out-of-range value clamps on submit
- Drag-to-adjust changes value proportional to horizontal mouse movement
- Float displays correct decimal precision
- Long-press on +/- rapidly increments

### E2E Test Plan

**Test file:** `src/testing/tests/InputNumberTest.h`

#### New Custom Commands Needed

- `long_press_button(label, hold_frames)` — click and hold for N frames before releasing. Needed for testing rapid-increment on long-press of +/- buttons.
- `type_into_field(label, text)` — focus a text input by label and type text. Needed for direct text entry of numeric values.
- `drag_element_by(label, dx, dy, hold_frames)` — click, drag by offset, release. Needed for drag-to-adjust interaction.

#### Screenshots

1. `input_number_initial` — all inputs at default values
2. `input_number_int_incremented` — player count after clicking + twice
3. `input_number_clamped` — after typing "999", value clamped to 100
4. `input_number_validation_error` — odd number entered showing red border and error text
5. `input_number_drag_adjust` — during/after drag-to-adjust interaction

#### Test Script

```cpp
TEST(input_number_increment) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("input_number_initial");

  // Click + twice on Player Count
  TestApp::click_button("+");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::click_button("+");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  auto snap = TestApp::capture_snapshot("input_number_int_incremented");
}

TEST(input_number_clamp) {
  co_await TestApp::wait_for_frames(5);

  // Type 999 into clamped input (range 0-100)
  type_into_field("Clamped Input", "999");
  co_await TestApp::wait_for_frames(2);

  // Submit with Enter
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(3);

  // Should clamp to 100
  TestApp::expect_ui_exists("100");
  auto snap = TestApp::capture_snapshot("input_number_clamped");
}

TEST(input_number_validation) {
  co_await TestApp::wait_for_frames(5);

  // Type odd number into even-only input
  type_into_field("Even Only", "7");
  co_await TestApp::wait_for_frames(2);
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Must be even");
  auto snap = TestApp::capture_snapshot("input_number_validation_error");
}
```

