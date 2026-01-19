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

