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
  Custom,         // User-provided path function
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

### Custom Path Focus Rings

For styles that don't fit any preset, you can supply a function that maps an angle (0–2π around the element perimeter) to an outward offset. The renderer walks the perimeter, calls your function at each sample point, and displaces the ring point by the returned amount. This lets you create wavy borders, star-shaped rings, heartbeat pulses, etc. without adding a new enum value for every shape.

#### Path Function Signature

```cpp
/// angle   — position around the element perimeter in radians [0, 2π)
///           0 = right-center, π/2 = bottom-center, π = left-center, 3π/2 = top-center
/// t       — normalized animation time [0, 1], for animated/pulsing rings
/// returns — vec2 displacement in the element's local coordinate space
///           x = outward (positive = away from element), y = tangential
using FocusPathFn = std::function<vec2(float angle, float t)>;
```

#### API

```cpp
// Set a custom path function on a single component
ComponentConfig{}
    .with_focus_style(FocusRingStyle::Custom)
    .with_focus_path([](float angle, float t) -> vec2 {
        // Wavy ring: sinusoidal displacement
        float offset = 4.0f + 2.0f * std::sin(angle * 8.0f + t * 6.28f);
        return {offset, 0.0f};
    });

// Or set it as the theme default
Theme myTheme;
myTheme.default_focus_style = FocusRingStyle::Custom;
myTheme.custom_focus_path = [](float angle, float t) -> vec2 {
    // Star shape: 5-pointed, varying distance
    float star = 4.0f + 3.0f * std::abs(std::sin(angle * 2.5f));
    return {star, 0.0f};
};
```

#### Built-in Path Helpers

Common patterns as factory functions so users don't have to write math from scratch:

```cpp
namespace focus_paths {

/// Sinusoidal wave around the perimeter
/// frequency: how many waves around the element (e.g. 8)
/// amplitude: wave height in pixels (e.g. 3.0f)
/// animated: if true, wave scrolls over time using t
FocusPathFn wavy(float frequency = 8.0f, float amplitude = 3.0f, bool animated = false);

/// Star/gear shape with N points
/// points: number of star points (e.g. 5)
/// inner_offset / outer_offset: distance from element edge
FocusPathFn star(int points = 5, float inner_offset = 3.0f, float outer_offset = 8.0f);

/// Pulsing ring that breathes in/out over time
/// min_offset / max_offset: range of displacement
FocusPathFn pulse(float min_offset = 2.0f, float max_offset = 6.0f);

/// Dashed ring — alternates between visible and invisible segments
/// dash_count: number of dashes around the perimeter
/// gap_ratio: fraction of each segment that is a gap (0.0–1.0)
FocusPathFn dashed(int dash_count = 12, float gap_ratio = 0.4f, float offset = 4.0f);

/// Corner-only — only draws near the four corners, fading out along edges
/// corner_fraction: how much of each edge to draw (0.0–0.5)
FocusPathFn corners_only(float corner_fraction = 0.15f, float offset = 4.0f);

} // namespace focus_paths

// Usage:
ComponentConfig{}
    .with_focus_style(FocusRingStyle::Custom)
    .with_focus_path(focus_paths::wavy(6, 2.5f, true));

ComponentConfig{}
    .with_focus_style(FocusRingStyle::Custom)
    .with_focus_path(focus_paths::dashed(16, 0.3f));
```

#### Rendering

The renderer samples N points around the element bounds (e.g. 64–128 depending on perimeter length), calls the path function at each, and draws a polyline through the displaced points:

```cpp
void render_custom_focus_ring(const Bounds &bounds, FocusPathFn &path_fn,
                              float t, Color color, float thickness) {
  const int num_samples = std::max(64, (int)(bounds.perimeter() / 4.0f));
  std::vector<vec2> points;
  points.reserve(num_samples);

  for (int i = 0; i < num_samples; ++i) {
    float angle = (float)i / (float)num_samples * 2.0f * M_PI;

    // Map angle to position on the rectangular perimeter
    vec2 perimeter_pos = bounds.point_at_angle(angle);
    vec2 outward_normal = bounds.normal_at_angle(angle);
    vec2 tangent = bounds.tangent_at_angle(angle);

    vec2 displacement = path_fn(angle, t);

    points.push_back(perimeter_pos
                     + outward_normal * displacement.x
                     + tangent * displacement.y);
  }

  draw_polyline(points, color, thickness, /*closed=*/true);
}
```

#### Notes

- When `FocusRingStyle::Custom` is set but no path function is provided, fall back to `Outline`.
- The `t` parameter is driven by the UI's animation clock, normalized to [0,1] over a configurable period (`theme.focus_animation_period`, default 1.0s).
- Path functions should be cheap — they're called ~64–128 times per focused element per frame. Avoid allocations.
- The tangential component (`displacement.y`) is useful for effects like rotating dashes or spiral patterns.

---

## Example Screen: FocusRingShowcase

**File:** `src/systems/screens/FocusRingShowcase.h`
**CLI:** `--screen=focus_ring`
**Category:** Visual Effects

### Layout

A screen demonstrating all focus ring styles side by side:

1. **Style Gallery** — 9 rows, one per `FocusRingStyle`. Each row: a label naming the style, then 3 buttons. The middle button in each row is auto-focused so the focus ring is visible. Styles: None, Outline, Fill, HighlightBar, Glow, Underline, Scale, Bracket, Custom.

2. **Customization Panel** — Sliders for: focus_ring_thickness (1-8px), focus_ring_offset (0-10px), focus_glow_radius (2-20px), focus_scale_factor (1.0-1.2). Changes apply to a demo button in real time.

3. **Custom Path Gallery** — A row of 5 buttons, each using a different built-in path helper: `wavy()`, `star()`, `pulse()`, `dashed()`, `corners_only()`. Each button auto-focuses on hover so the custom ring is visible. A 6th button shows a hand-written lambda (spiral pattern).

4. **Theme Comparison** — The same Outline focus ring shown in 3 themes: white ring on dark background, blue ring on light background, cyan ring on medium background. Demonstrates Focus color from `Theme::Usage::Focus`.

5. **Tab Navigation Demo** — A row of 6 buttons. Tab between them to see the focus ring move. The current `FocusRingStyle` is selectable via a dropdown (including Custom with a path picker), changing all buttons' style live.

### Features Exercised

- All `FocusRingStyle` variants including `Custom`
- `theme.focus`, `theme.focus_ring_thickness`, `theme.focus_ring_offset`
- `with_focus_style()` and `with_focus_path()` on ComponentConfig
- Built-in path helpers: `wavy()`, `star()`, `pulse()`, `dashed()`, `corners_only()`
- Custom lambda path functions
- Focus ring rendering with different themes
- Tab navigation visual feedback

### Verification

- Outline: visible rectangular outline offset from element bounds
- Fill: background color changes when focused
- HighlightBar: colored bar on left edge when focused
- Glow: soft outer glow effect around focused element
- Scale: element visually grows when focused, shrinks when unfocused
- Bracket: `> Option <` style indicators appear when focused
- Custom: path function displaces ring points around perimeter
- Wavy, star, pulse, dashed, corners_only helpers produce distinct shapes
- Thickness/offset sliders produce visible changes in real time

### E2E Test Plan

**Test file:** `src/testing/tests/FocusRingTest.h`

#### New Custom Commands Needed

None — uses existing `simulate_tab`, `expect_focus`, `capture_snapshot`, `click_button`.

#### Screenshots

1. `focus_ring_outline` — Outline style with visible rectangular outline around focused button
2. `focus_ring_fill` — Fill style with background color change on focus
3. `focus_ring_highlight_bar` — HighlightBar with colored left-edge bar
4. `focus_ring_glow` — Glow style with soft outer glow
5. `focus_ring_scale` — Scale style with enlarged focused element
6. `focus_ring_bracket` — Bracket style with `> Option <` indicators
7. `focus_ring_custom_wavy` — Custom path with wavy() helper
8. `focus_ring_custom_star` — Custom path with star() helper
9. `focus_ring_custom_dashed` — Custom path with dashed() helper
10. `focus_ring_tab_navigation` — focus ring moving via Tab between buttons

#### Test Script

```cpp
TEST(focus_ring_styles_visible) {
  co_await TestApp::wait_for_frames(5);

  // Each row has auto-focused middle button — capture each style
  auto snap1 = TestApp::capture_snapshot("focus_ring_outline");

  // The snapshot captures the whole screen showing all styles at once
  // Each row's middle button should show its respective focus ring
}

TEST(focus_ring_tab_navigation) {
  co_await TestApp::wait_for_frames(5);

  // Tab to focus the first button in the nav demo row
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Nav Button 1");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_focus("Nav Button 2");

  auto snap_mid = TestApp::capture_snapshot("focus_ring_tab_navigation");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_focus("Nav Button 3");
}

TEST(focus_ring_style_change) {
  co_await TestApp::wait_for_frames(5);

  // Change style via dropdown
  TestApp::click_button("Style Selector");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::click_button("Glow");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("focus_ring_glow");
}

TEST(focus_ring_custom_paths) {
  co_await TestApp::wait_for_frames(5);

  // The custom path gallery row has auto-focused buttons
  // Capture each custom path style
  TestApp::click_button("Wavy Ring");
  co_await TestApp::wait_for_frames(5);
  auto snap_wavy = TestApp::capture_snapshot("focus_ring_custom_wavy");

  TestApp::click_button("Star Ring");
  co_await TestApp::wait_for_frames(5);
  auto snap_star = TestApp::capture_snapshot("focus_ring_custom_star");

  TestApp::click_button("Dashed Ring");
  co_await TestApp::wait_for_frames(5);
  auto snap_dashed = TestApp::capture_snapshot("focus_ring_custom_dashed");
}
```
