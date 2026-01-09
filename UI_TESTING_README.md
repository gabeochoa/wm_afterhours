# UI Testing Framework Documentation

This document describes how to use the UI testing framework for the afterhours UI library.

## Overview

The UI testing framework provides:
- Input simulation (keyboard and mouse)
- Snapshot testing (visual regression testing)
- Focus state testing
- Example screens for demonstration

## Writing UI Tests

### Basic Test Structure

Tests are written as coroutines using the `TEST` macro:

```cpp
#include "../test_macros.h"
#include "../test_app.h"

TEST(my_test) {
  co_await TestApp::wait_for_frames(5);
  
  TestApp::expect_ui_exists("Button Label");
  
  TestApp::click_button("Button Label");
  
  co_await TestApp::wait_for_frames(2);
  
  TestApp::expect_ui_exists("Expected Result");
}
```

### Test Helpers

#### Waiting

- `TestApp::wait_for_frames(int frames)` - Wait for a number of frames
- `TestApp::wait_for_ui_exists(label, max_frames)` - Wait until a UI element appears
- `TestApp::wait_for_condition(condition, max_frames)` - Wait for a custom condition

#### UI Element Queries

- `TestApp::expect_ui_exists(label)` - Assert that a UI element with the given label exists
- `TestApp::expect_ui_not_exists(label)` - Assert that a UI element does not exist
- `TestApp::find_ui_element_by_label(label)` - Find a UI element by label

#### Input Simulation

- `TestApp::click_button(label)` - Click a button by label
- `TestApp::simulate_tab()` - Simulate pressing Tab
- `TestApp::simulate_shift_tab()` - Simulate pressing Shift+Tab
- `TestApp::simulate_arrow_key(key)` - Simulate arrow key press
- `TestApp::simulate_enter()` - Simulate Enter key
- `TestApp::simulate_escape()` - Simulate Escape key
- `TestApp::simulate_typing(text)` - Simulate typing text

#### Focus Testing

- `TestApp::expect_focus(label)` - Assert that the given element has focus
- `TestApp::expect_no_focus()` - Assert that no element has focus
- `TestApp::get_focused_element()` - Get the currently focused element

#### Snapshot Testing

- `TestApp::capture_snapshot(name)` - Capture a snapshot (screenshot + UI state)
- `TestApp::compare_snapshot(name, tolerance)` - Compare current state with stored snapshot

## Snapshot Testing

Snapshot testing captures both visual screenshots and UI state (positions, sizes, visibility, focus) for regression testing.

### Capturing Snapshots

```cpp
TEST(snapshot_test) {
  co_await TestApp::wait_for_frames(5);
  
  auto result = TestApp::capture_snapshot("my_snapshot");
  if (!result.success) {
    throw std::runtime_error("Failed to capture: " + result.error_message);
  }
}
```

### Comparing Snapshots

```cpp
TEST(compare_test) {
  co_await TestApp::wait_for_frames(5);
  
  auto result = TestApp::compare_snapshot("my_snapshot", 0.01f);
  if (!result.success) {
    throw std::runtime_error("Snapshot mismatch: " + result.error_message);
  }
}
```

Snapshots are stored in the `test_snapshots/` directory:
- `{name}.png` - Screenshot
- `{name}_state.json` - UI state JSON
- `{name}_diff.png` - Visual diff (if comparison fails)

## Running Tests

### List Available Tests

```bash
./output/ui_tester.exe --list-tests
```

### Run a Specific Test

```bash
./output/ui_tester.exe --run-test test_name
```

### Run Test in Slow Mode (Visible)

```bash
./output/ui_tester.exe --run-test test_name --slow-test
```

Slow mode adds delays so you can see the test execution.

## Example Screens

Example screens demonstrate UI patterns and can be launched with command-line flags.

### List Available Screens

```bash
./output/ui_tester.exe --list-screens
```

### Launch an Example Screen

```bash
./output/ui_tester.exe --simple_button
./output/ui_tester.exe --tabbing
./output/ui_tester.exe --layout
./output/ui_tester.exe --colors
./output/ui_tester.exe --text
```

## Adding New Example Screens

1. Create a new system file in `src/systems/` (e.g., `ExampleMyScreen.h`):

```cpp
#pragma once

#include "../external.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleMyScreen : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    // Create your UI here
  }
};

REGISTER_EXAMPLE_SCREEN(my_screen, "Description of my screen", ExampleMyScreen)
```

2. Include the header in `src/main.cpp`:

```cpp
#include "systems/ExampleMyScreen.h"
```

3. The screen will be automatically registered and available via `--my_screen`

## Adding New Tests

1. Create a test file in `src/testing/tests/` (e.g., `MyTest.h`):

```cpp
#pragma once

#include "../test_macros.h"
#include "../test_app.h"

TEST(my_test) {
  // Test code here
}
```

2. Include the test in `src/testing/tests/all_tests.h`:

```cpp
#include "MyTest.h"
```

3. If the test needs a custom setup system, create one in `src/systems/` and update `run_test()` in `src/game.cpp` to use it.

## Command-Line Flags

- `--help` - Show help message
- `-w, --width <pixels>` - Screen width (default: 1280)
- `-h, --height <pixels>` - Screen height (default: 720)
- `--list-tests` - List all available tests
- `--run-test <name>` - Run a specific test
- `--slow-test` - Run test in slow mode (visible)
- `--list-screens` - List all available example screens
- `--<screen_name>` - Show example screen (e.g., `--simple_button`)

## Test Setup Systems

Some tests require specific UI setups. Setup systems are registered in `run_test()`:

- `SetupSimpleButtonTest` - Simple button with click counter (default)
- `SetupTabbingTest` - Four buttons for tabbing navigation

To add a new setup system, create it in `src/systems/` and update the test name check in `run_test()`.

## Best Practices

1. **Wait for UI to stabilize**: Always wait a few frames after UI creation before interacting
2. **Use descriptive labels**: UI elements should have clear, unique labels for testing
3. **Test incrementally**: Break complex tests into smaller, focused tests
4. **Use snapshots sparingly**: Snapshots are useful for regression testing but can be brittle
5. **Handle timing**: UI updates happen over multiple frames, use appropriate wait times

## Troubleshooting

### Test fails with "UI element not found"

- Ensure the element has a label set with `.with_label()`
- Wait more frames for the UI to render
- Check that the setup system is creating the expected elements

### Snapshot comparison fails

- Check `test_snapshots/{name}_diff.png` to see visual differences
- Review `test_snapshots/{name}_state.json` for state differences
- Adjust tolerance if differences are acceptable (default: 0.01)

### Focus testing fails

- Ensure elements are focusable (buttons are automatically focusable)
- Wait for focus to update after tab key presses
- Check that tabbing is enabled in the input mapping

