# API Ergonomics

**Status:** Not implemented  
**Priority:** Medium  
**Source:** break_ross migration experience

---

## Simplified Include Path

### Problem

Need to include multiple specific headers or know to include `<afterhours/src/plugins/ui.h>` to get all UI functionality. The discovery path is not obvious.

### Suggested Fix

```cpp
// Option 1: Single umbrella header (more discoverable name)
#include <afterhours/ui.h>

// Option 2: Alternatively, document the main header prominently
// and ensure it includes everything needed for immediate-mode UI
```

---

## ComponentConfig Builder Pattern Improvements

### Problem

ComponentConfig uses method chaining which is good, but some common patterns require too much boilerplate.

### Current Code (verbose)

```cpp
imm::text(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_label("Hello World")
        .with_font(UIComponent::DEFAULT_FONT)
        .with_font_size(screen_pct(0.02f))
        .with_color_usage(Theme::Usage::Foreground)
        .with_padding(Padding{screen_pct(0.01f)})
);
```

### Suggested Helper Functions

```cpp
// Simple text label with sensible defaults
ElementResult text_label(HasUIContext auto &ctx, EntityParent ep,
                         const std::string &text,
                         ComponentConfig config = ComponentConfig());

// Centered button with callback
ElementResult centered_button(HasUIContext auto &ctx, EntityParent ep,
                              const std::string &text,
                              std::function<void()> on_click,
                              ComponentConfig config = ComponentConfig());

// Container with common padding/sizing
ElementResult container(HasUIContext auto &ctx, EntityParent ep,
                        ComponentSize size,
                        Padding padding = DefaultPadding::medium(),
                        ComponentConfig config = ComponentConfig());

// Usage becomes:
text_label(ctx, mk(parent, 0), "Hello World");
centered_button(ctx, mk(parent, 1), "Click Me", []{ /* action */ });
```

---

## Size Calculation Helpers

### Problem

Need to calculate screen percentages manually (e.g., `screen_pct(0.02f)` for 2% of screen). The API doesn't clearly distinguish X vs Y percentages or provide pixel-based alternatives.

### Current Issues
- `screen_pct()` is a single value used for both width and height
- No pixel-based helpers that auto-scale to screen size
- Manual calculation required for "10px on a 1080p screen" type requirements

### Suggested Improvements

```cpp
// Explicit axis-aware percentages
ComponentSize screen_pct_x(float percent);  // Percentage of screen width
ComponentSize screen_pct_y(float percent);  // Percentage of screen height

// Pixel-based helpers that scale to reference resolution
ComponentSize responsive_px(float pixels, int reference_width = 1920);

// Usage:
.with_size({responsive_px(200), responsive_px(50)})  // 200x50 on 1920px wide screen

// Common presets
namespace DefaultSize {
    ComponentSize button();        // Standard button size
    ComponentSize icon_small();    // 16px equivalent
    ComponentSize icon_medium();   // 24px equivalent
    ComponentSize icon_large();    // 32px equivalent
}
```

---

## Positioning Helpers

### Problem

Using `.with_absolute_position()` requires manual calculation of percentages for positioning. Common patterns like "top-left with padding" are verbose.

### Suggested Improvements

```cpp
// Position helpers for absolute positioning
ComponentConfig position_top_left(float offset_x = 0, float offset_y = 0);
ComponentConfig position_top_right(float offset_x = 0, float offset_y = 0);
ComponentConfig position_bottom_left(float offset_x = 0, float offset_y = 0);
ComponentConfig position_bottom_right(float offset_x = 0, float offset_y = 0);
ComponentConfig position_center();
ComponentConfig position_center_x(float offset_y = 0);
ComponentConfig position_center_y(float offset_x = 0);

// Usage:
imm::div(ctx, mk(parent, 0),
    position_top_right(screen_pct(0.02f), screen_pct(0.02f))
        .with_size({screen_pct(0.2f), screen_pct(0.1f)})
);
```

---

## Documentation and Examples

### Problem

The API is powerful but the learning curve is steep. Need to understand many concepts (ComponentConfig, ComponentSize, Padding, Margin, etc.) before being productive.

### Suggested Improvements

1. **Getting Started Guide**: Step-by-step tutorial covering:
   - Basic text label
   - Button with click handler
   - Container with children
   - Styling with themes
   - Common layout patterns

2. **Inline Documentation**: Add doc comments to all public APIs

3. **Example Gallery**: Code samples for common patterns:
   - Settings menu with toggles
   - Modal dialog
   - Navigation tabs
   - Form with inputs
   - Game HUD elements

4. **Troubleshooting Guide**: Common issues and solutions:
   - "Why doesn't my element render?"
   - "Why is my text invisible?"
   - "Why is positioning wrong?"

---

## Example Screen: APIErgonomicsShowcase

**File:** `src/systems/screens/APIErgonomicsShowcase.h`
**CLI:** `--screen=api_ergonomics`
**Category:** DX / Developer Experience

### Layout

A comparison screen showing verbose vs. ergonomic API usage:

1. **Before/After Code Display** — Two columns. Left: a settings panel built with verbose `ComponentConfig{}` chains (many `.with_*()` calls). Right: the same panel built with helper functions (`text_label()`, `centered_button()`, `container()`). Both render identically — a label above each says "Verbose (12 lines)" vs "Ergonomic (5 lines)".

2. **Size Helper Demo** — A row of boxes sized with different helpers: `screen_pct(0.1f)`, `responsive_px(100)`, `DefaultSize::button()`, `DefaultSize::icon_medium()`. Each labeled with the helper used and the resolved pixel size.

3. **Position Helper Demo** — 4 small colored squares positioned at each corner using `position_top_left()`, `position_top_right()`, `position_bottom_left()`, `position_bottom_right()`. A centered square using `position_center()`.

4. **Quick-Build Form** — A mini settings form built entirely with helpers: `text_label`, `centered_button`, and `container`. Demonstrates that helpers compose naturally with existing `ComponentConfig`.

### Features Exercised

- `text_label()`, `centered_button()`, `container()` helper functions
- `responsive_px()` with reference resolution scaling
- `DefaultSize::button()`, `icon_small()`, `icon_medium()`, `icon_large()`
- `position_top_left/right()`, `position_center()` helpers
- Helper function composition with existing ComponentConfig

### Verification

- Verbose and ergonomic panels render identically
- `responsive_px(200)` resolves to correct pixel count for current screen size
- Position helpers place elements at expected corners
- Helper functions accept additional ComponentConfig chaining

### E2E Test Plan

**Test file:** `src/testing/tests/APIErgonomicsTest.h`

#### New Custom Commands Needed

- `expect_element_position(label, x, y, tolerance)` — check element position. Needed to verify `position_top_left/right()` helpers place elements at expected corners.
- `expect_element_size(label, w, h, tolerance)` — check element size. Needed to verify `responsive_px()` resolves correctly.

#### Screenshots

1. `api_ergonomics_comparison` — verbose vs ergonomic panels side by side (should look identical)
2. `api_ergonomics_size_helpers` — boxes with different size helpers, labeled with resolved pixel values
3. `api_ergonomics_position_helpers` — 4 squares at corners + 1 centered
4. `api_ergonomics_quick_form` — mini form built with helpers

#### Test Script

```cpp
TEST(api_ergonomics_identical_output) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Verbose");
  TestApp::expect_ui_exists("Ergonomic");

  auto snap = TestApp::capture_snapshot("api_ergonomics_comparison");
  // Visual verification: both panels should render identically
}

TEST(api_ergonomics_position_helpers) {
  co_await TestApp::wait_for_frames(5);

  // Verify corner elements exist
  TestApp::expect_ui_exists("Top Left");
  TestApp::expect_ui_exists("Top Right");
  TestApp::expect_ui_exists("Bottom Left");
  TestApp::expect_ui_exists("Bottom Right");
  TestApp::expect_ui_exists("Center");

  auto snap = TestApp::capture_snapshot("api_ergonomics_position_helpers");

  // Check positions are near edges
  expect_element_position("Top Left", 0, 0, 50.0f);
}

TEST(api_ergonomics_size_helpers) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("api_ergonomics_size_helpers");
}
```

