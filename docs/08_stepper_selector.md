# Stepper/Selector with Arrows

**Status:** Implemented
**Priority:** High

## Problem

The pattern `< value >` with left/right arrows to cycle through discrete options is reimplemented in almost every settings screen.

## Screens Using This Pattern
- ParcelCorps: `render_language_row()`, `render_selector_row()`
- MiniMotorways: Sensitivity selector
- FlightOptions, KirbyOptions, DeadSpace, etc.

## Suggested Implementation

```cpp
template <typename Container>
ElementResult stepper(HasUIContext auto &ctx, EntityParent ep_pair,
                      const Container &options, size_t &option_index,
                      ComponentConfig config = ComponentConfig());

// Numeric variants
ElementResult stepper_int(HasUIContext auto &ctx, EntityParent ep_pair,
                          int &value, int min, int max, int step = 1,
                          ComponentConfig config = ComponentConfig());

ElementResult stepper_float(HasUIContext auto &ctx, EntityParent ep_pair,
                            float &value, float min, float max, float step = 0.1f,
                            ComponentConfig config = ComponentConfig());
```

## Features Needed
- [-] and [+] buttons on sides
- Current value display in center
- Long-press for rapid increment
- Keyboard: left/right arrows when focused

---

## Example Screen: StepperShowcase

**File:** `src/systems/screens/StepperShowcase.h`
**CLI:** `--screen=stepper`
**Category:** Widgets

### Layout

A settings-style screen with multiple stepper variants:

1. **String Stepper** — "Difficulty: < Normal >" cycling through Easy/Normal/Hard/Nightmare. Shows `option_index` below.

2. **Integer Stepper** — "Player Count: < 4 >" with range 1-8, step 1. Long-press on [+] rapidly increments.

3. **Float Stepper** — "Volume: < 0.75 >" with range 0.0-1.0, step 0.05. Displays 2 decimal places.

4. **Styled Steppers** — Same steppers with different themes (neon dark, kraft, pastel) to verify theming.

5. **Disabled Stepper** — A stepper with `with_disabled(true)`, arrows grayed out and non-functional.

### Features Exercised

- `stepper()` with string container
- `stepper_int()` with min/max/step
- `stepper_float()` with min/max/step
- Long-press rapid increment behavior
- Keyboard left/right arrow cycling when focused
- Disabled state

### Verification

- [-] at minimum value is non-functional (clamped)
- [+] at maximum value is non-functional (clamped)
- Long-press on [+] increments repeatedly
- Keyboard left/right arrows cycle values when stepper is focused
- Disabled stepper ignores all input

### E2E Test Plan

**Test file:** `src/testing/tests/StepperTest.h`

#### New Custom Commands Needed

- `long_press_button(label, hold_frames)` — click button and hold for N frames before releasing. Needed for testing rapid-increment on long-press of +/- arrows.

#### Screenshots

1. `stepper_initial` — all steppers at default values
2. `stepper_string_changed` — difficulty stepper changed from Normal to Hard
3. `stepper_int_at_max` — player count at maximum (8), [+] visually disabled
4. `stepper_disabled` — disabled stepper showing grayed-out styling

#### Test Script

```cpp
TEST(stepper_string_cycle) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Normal");

  // Click > on difficulty stepper
  TestApp::click_button(">");  // first stepper's right arrow
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Hard");
  auto snap = TestApp::capture_snapshot("stepper_string_changed");
}

TEST(stepper_int_clamp) {
  co_await TestApp::wait_for_frames(5);

  // Click + repeatedly to reach max
  for (int i = 0; i < 10; i++) {
    TestApp::click_button("+");
    co_await TestApp::wait_for_frames(1);
    TestApp::release_mouse_button();
    co_await TestApp::wait_for_frames(2);
  }

  // Should clamp at max (8)
  TestApp::expect_ui_exists("8");
  auto snap = TestApp::capture_snapshot("stepper_int_at_max");
}

TEST(stepper_keyboard_nav) {
  co_await TestApp::wait_for_frames(5);

  // Focus difficulty stepper
  TestApp::click_button("Normal");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Right arrow to cycle value
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Hard");

  // Left arrow back
  TestApp::simulate_arrow_key(raylib::KEY_LEFT);
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Normal");
}

TEST(stepper_disabled) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("stepper_disabled");

  // Attempt to click disabled stepper's arrow (should do nothing)
  // The disabled stepper's value should remain unchanged
}
```

