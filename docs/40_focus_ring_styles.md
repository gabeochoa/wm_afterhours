# Focus Ring Style Configuration

**Status:** Not implemented  
**Priority:** Large

## Problem

Only a single focus ring style is supported (outline rectangle). Games use many different focus indication styles.

## Inspiration Examples
- **Dead Space**: Teal highlight bar on the left edge
- **Fighter Menu**: Background color change when focused
- **Kirby Options**: Scales element up + adds thicker border
- **Mini Motorways**: Subtle glow/shadow effect

## Suggested Implementation

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
    float focus_ring_thickness = 2.0f;
    float focus_glow_radius = 8.0f;
    float focus_scale_factor = 1.05f;
};
```

