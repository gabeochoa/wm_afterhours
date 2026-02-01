# Focus Ring Style Configuration

**Status:** Partially implemented (basic outline with configurable color and thickness)
**Priority:** Large

## Current Implementation

The focus ring system now supports:
- **Dedicated focus color** (`Theme::Usage::Focus`) - defaults to white for high contrast
- **Configurable thickness** (`theme.focus_ring_thickness`) - defaults to 3.0f for visibility
- **Configurable offset** (`theme.focus_ring_offset`) - defaults to 4.0f to render outside component bounds

### Theme Configuration

```cpp
struct Theme {
    Color focus{255, 255, 255, 255};    // White - high contrast focus ring
    float focus_ring_thickness = 3.0f;  // Thickness of focus ring outline (2-3px for visibility)
    float focus_ring_offset = 4.0f;     // Gap between element and focus ring (ensures no clipping)
};
```

### Usage

The focus ring automatically uses the theme's focus color and settings. To customize:

```cpp
// Set a custom focus color
ui::imm::ThemeDefaults::get().set_theme_color(Theme::Usage::Focus, Color{100, 200, 255, 255});

// Or configure the entire theme
Theme myTheme;
myTheme.focus = colors::cyan;           // Bright cyan focus ring
myTheme.focus_ring_thickness = 4.0f;    // Thicker ring
myTheme.focus_ring_offset = 6.0f;       // More gap from component
ui::imm::ThemeDefaults::get().set_theme(myTheme);
```

## Problem (Original)

Only a single focus ring style is supported (outline rectangle). Games use many different focus indication styles.

## Inspiration Examples
- **Dead Space**: Teal highlight bar on the left edge
- **Fighter Menu**: Background color change when focused
- **Kirby Options**: Scales element up + adds thicker border
- **Mini Motorways**: Subtle glow/shadow effect

## Future Implementation (Not Yet Done)

```cpp
enum class FocusRingStyle {
  None,           // Disable focus ring
  Outline,        // Current default
  Fill,           // Change background color when focused
  HighlightBar,   // Colored bar on edge
  Glow,           // Soft outer glow/shadow effect
  Underline,      // Line under the element
  Scale,          // Element becomes slightly larger
  Bracket,        // Decorative brackets "> Option <"
};

ComponentConfig{}
    .with_focus_style(FocusRingStyle::HighlightBar)
    .with_focus_color(Theme::Usage::Accent)
    .with_focus_edge(Edge::Left)
    .with_focus_thickness(4.0f);

struct Theme {
    FocusRingStyle default_focus_style = FocusRingStyle::Outline;
    float focus_glow_radius = 8.0f;
    float focus_scale_factor = 1.05f;
};
```
