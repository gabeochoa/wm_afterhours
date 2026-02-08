# Color System

**Status:** Not implemented  
**Priority:** Low  
**Source:** break_ross migration experience

---

## Problem

Need to use `afterhours::Color` explicitly, and mixing `Theme::Usage` with `custom_color` can be confusing.

### Current Issues

1. No helper functions for common colors
2. Unclear when to use `Theme::Usage` vs `with_custom_color()`
3. Color specification is verbose
4. No color manipulation utilities (lighten, darken, alpha blend)

---

## Color Helper Functions

```cpp
namespace colors {
    // Named colors
    constexpr Color white() { return {255, 255, 255, 255}; }
    constexpr Color black() { return {0, 0, 0, 255}; }
    constexpr Color transparent() { return {0, 0, 0, 0}; }
    
    // Grayscale
    constexpr Color gray(uint8_t value) { return {value, value, value, 255}; }
    constexpr Color gray_50() { return gray(128); }
    constexpr Color gray_25() { return gray(64); }
    constexpr Color gray_75() { return gray(192); }
    
    // RGB constructors
    constexpr Color rgb(uint8_t r, uint8_t g, uint8_t b) { 
        return {r, g, b, 255}; 
    }
    constexpr Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { 
        return {r, g, b, a}; 
    }
    
    // Hex color (0xRRGGBB or 0xRRGGBBAA)
    constexpr Color hex(uint32_t value) {
        if (value <= 0xFFFFFF) {
            return {
                static_cast<uint8_t>((value >> 16) & 0xFF),
                static_cast<uint8_t>((value >> 8) & 0xFF),
                static_cast<uint8_t>(value & 0xFF),
                255
            };
        } else {
            return {
                static_cast<uint8_t>((value >> 24) & 0xFF),
                static_cast<uint8_t>((value >> 16) & 0xFF),
                static_cast<uint8_t>((value >> 8) & 0xFF),
                static_cast<uint8_t>(value & 0xFF)
            };
        }
    }
    
    // Common UI colors
    constexpr Color error() { return hex(0xDC3545); }
    constexpr Color warning() { return hex(0xFFC107); }
    constexpr Color success() { return hex(0x28A745); }
    constexpr Color info() { return hex(0x17A2B8); }
}

// Usage:
.with_custom_color(colors::hex(0x3498DB))
.with_custom_color(colors::gray(200))
.with_custom_color(colors::error())
```

---

## Color Manipulation

```cpp
namespace colors {
    // Alpha modification
    Color with_alpha(Color c, uint8_t alpha) {
        return {c.r, c.g, c.b, alpha};
    }
    Color with_alpha(Color c, float alpha) {
        return {c.r, c.g, c.b, static_cast<uint8_t>(alpha * 255)};
    }
    
    // Lighten/darken
    Color lighten(Color c, float amount) {  // amount: 0-1
        return {
            static_cast<uint8_t>(c.r + (255 - c.r) * amount),
            static_cast<uint8_t>(c.g + (255 - c.g) * amount),
            static_cast<uint8_t>(c.b + (255 - c.b) * amount),
            c.a
        };
    }
    Color darken(Color c, float amount) {
        return {
            static_cast<uint8_t>(c.r * (1 - amount)),
            static_cast<uint8_t>(c.g * (1 - amount)),
            static_cast<uint8_t>(c.b * (1 - amount)),
            c.a
        };
    }
    
    // Blend two colors
    Color blend(Color a, Color b, float t) {
        return {
            static_cast<uint8_t>(a.r + (b.r - a.r) * t),
            static_cast<uint8_t>(a.g + (b.g - a.g) * t),
            static_cast<uint8_t>(a.b + (b.b - a.b) * t),
            static_cast<uint8_t>(a.a + (b.a - a.a) * t)
        };
    }
}

// Usage:
.with_custom_color(colors::with_alpha(colors::white(), 0.5f))
.with_custom_color(colors::lighten(base_color, 0.2f))
```

---

## Theme::Usage vs Custom Color

### Documentation Needed

```cpp
// When to use Theme::Usage:
// - UI should adapt to theme changes (light/dark mode)
// - Standard UI elements (buttons, panels, text)
// - Consistent look across the application

.with_color_usage(Theme::Usage::Primary)      // Main action buttons
.with_color_usage(Theme::Usage::Secondary)    // Secondary actions
.with_color_usage(Theme::Usage::Background)   // Panel backgrounds
.with_color_usage(Theme::Usage::Foreground)   // Text color
.with_color_usage(Theme::Usage::Accent)       // Highlights, focus rings
.with_color_usage(Theme::Usage::Disabled)     // Disabled elements

// When to use custom_color:
// - Brand colors that shouldn't change with theme
// - Game-specific colors (health bar red, mana blue)
// - Decorative elements with fixed colors
// - Colors from external data (user-selected, loaded from file)

.with_custom_color(colors::hex(0xFF0000))  // Always red, regardless of theme
```

### Clear API

```cpp
// Make the choice explicit:
ComponentConfig{}
    .with_themed_color(Theme::Usage::Primary)  // Adapts to theme
    .with_fixed_color(colors::hex(0xFF0000))   // Never changes

// Or use a union type:
struct ColorSpec {
    std::variant<Theme::Usage, Color> value;
    
    static ColorSpec themed(Theme::Usage usage) { return {usage}; }
    static ColorSpec fixed(Color color) { return {color}; }
};

.with_color(ColorSpec::themed(Theme::Usage::Primary))
.with_color(ColorSpec::fixed(colors::red()))
```

---

## Example Screen: ColorSystemShowcase

**File:** `src/systems/screens/ColorSystemShowcase.h`
**CLI:** `--screen=color_system`
**Category:** DX / Developer Experience

### Layout

A comprehensive color system demo:

1. **Named Colors** — A grid of swatches: `white()`, `black()`, `gray_25()`, `gray_50()`, `gray_75()`, `error()`, `warning()`, `success()`, `info()`. Each labeled with the function name and hex value.

2. **Color Manipulation** — Starting from a base blue (`#3498DB`), a row of swatches showing: original, `lighten(0.2)`, `lighten(0.4)`, `darken(0.2)`, `darken(0.4)`, `with_alpha(0.5)`, `with_alpha(0.25)`. Each labeled with the operation.

3. **Color Blending** — A gradient bar showing `blend(red, blue, t)` for t=0.0 to 1.0 in 10 steps. Red on the left smoothly transitions to blue on the right.

4. **Hex Input** — A `text_input` for hex colors (`#RRGGBB`). Typing a valid hex updates a large preview swatch. Demonstrates `colors::hex(0xRRGGBB)` parsing.

5. **Theme vs Fixed** — Two rows of buttons: top row uses `with_themed_color(Theme::Usage::Primary)` (adapts when theme changes), bottom row uses `with_fixed_color(colors::hex(0x3498DB))` (stays the same). A theme switcher dropdown changes the theme — only the top row changes.

6. **Palette Presets** — A row of named palette previews: Material Blue, Material Dark, Nord, Dracula. Clicking one applies it as the current theme, updating all themed elements.

### Features Exercised

- `colors::white/black/gray/error/warning/success/info()`
- `colors::lighten()`, `darken()`, `with_alpha()`, `blend()`
- `colors::hex()` for hex color parsing
- `Theme::Usage` vs `custom_color` distinction
- `ColorSpec::themed()` vs `ColorSpec::fixed()`
- `palettes::apply_palette()` for preset application

### Verification

- `lighten(0.4)` produces a visibly lighter color
- `darken(0.4)` produces a visibly darker color
- `with_alpha(0.5)` shows semi-transparency
- Switching themes: themed colors change, fixed colors don't
- Blend gradient is smooth with no banding artifacts

### E2E Test Plan

**Test file:** `src/testing/tests/ColorSystemTest.h`

#### New Custom Commands Needed

None — color system is primarily visual. Snapshot comparison is the main verification method.

#### Screenshots

1. `color_system_hex_rgb` — color construction demo showing hex/rgb/rgba swatches
2. `color_system_manipulation` — lighten/darken/alpha panels showing before/after
3. `color_system_themed_vs_fixed` — themed + fixed swatches in default theme
4. `color_system_theme_switched` — same swatches after theme switch (themed changed, fixed didn't)
5. `color_system_auto_text` — auto_text_color demo (white text on dark, black on light)
6. `color_system_blend_gradient` — smooth blend gradient strip

#### Test Script

```cpp
TEST(color_system_theme_switch) {
  co_await TestApp::wait_for_frames(5);

  auto snap_before = TestApp::capture_snapshot("color_system_themed_vs_fixed");

  // Switch theme
  TestApp::click_button("Switch Theme");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_after = TestApp::capture_snapshot("color_system_theme_switched");
  // Themed swatches should change color, fixed swatches should not
}

TEST(color_system_visual_regression) {
  co_await TestApp::wait_for_frames(5);

  auto snap1 = TestApp::capture_snapshot("color_system_hex_rgb");
  auto snap2 = TestApp::capture_snapshot("color_system_manipulation");
  auto snap3 = TestApp::capture_snapshot("color_system_auto_text");
  auto snap4 = TestApp::capture_snapshot("color_system_blend_gradient");
}
```

---

## Color Palette Presets

```cpp
namespace palettes {
    struct Palette {
        Color primary;
        Color secondary;
        Color accent;
        Color background;
        Color surface;
        Color error;
        Color text_on_primary;
        Color text_on_background;
    };
    
    // Built-in palettes
    Palette material_blue();
    Palette material_dark();
    Palette solarized_light();
    Palette solarized_dark();
    Palette nord();
    Palette dracula();
    
    // Apply palette to theme
    void apply_palette(Theme& theme, const Palette& palette);
}

// Usage:
Theme my_theme;
palettes::apply_palette(my_theme, palettes::nord());
```

