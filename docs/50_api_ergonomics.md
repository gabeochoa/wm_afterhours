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

