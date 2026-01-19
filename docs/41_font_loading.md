# Font Loading and Glyph Metrics

**Status:** Not implemented  
**Priority:** Medium

---

## CJK Font Loading

### Problem

Standard `load_font()` only loads ASCII glyphs. CJK characters require `load_font_with_codepoints()` which is not obvious.

### Suggested Fix
1. Add `load_font_full(name, path)` that loads all codepoints
2. Or add `load_font_range(name, path, start, end)` for Unicode ranges
3. Or auto-detect needed codepoints from font file

---

## Icon Font / Symbol Rendering

### Problem

Loading icon fonts requires manually specifying codepoints. No auto-detection.

### Suggested Fix
1. `load_font_full(name, path)` - loads all codepoints in font
2. `load_font_range(name, path, start, end)` - loads Unicode range

---

## Glyph Metrics API

### Problem

No way to get precise per-character widths for accurate caret positioning.

### Suggested Implementation

```cpp
namespace fonts {
    float measure_char(const std::string &font_name, float size, char32_t ch);
    float measure_substring(const std::string &font_name, float size, 
                            std::string_view text, size_t start, size_t len);
    size_t char_at_position(const std::string &font_name, float size,
                            std::string_view text, float x_offset);
}
```

---

## Font Configuration and Inheritance

### Problem

Font configuration is not always clear:
- When do you need `.with_font()` vs when does it inherit from parent?
- How does font size work with different screen sizes?
- Unclear if font needs to be "active" in FontManager for rendering

### Current Behavior
- Font must be explicitly set on each element OR inherited from parent
- `UIComponent::DEFAULT_FONT` must be specified explicitly
- Font size calculations for responsive design are manual
- No warning when a font is specified that hasn't been loaded

### Suggested Improvements

```cpp
// 1. Default font that's automatically used if none specified
namespace fonts {
    void set_default_font(const std::string &font_name);
    std::string get_default_font();
}

// 2. Responsive font sizing
ComponentConfig{}
    .with_responsive_font_size(base_size, reference_resolution);

// 3. Font validation - warn when font not loaded
// In debug builds, log warning:
// "Font 'MyFont' used but not registered with FontManager"

// 4. Clearer API
ComponentConfig{}
    .with_default_font()      // Uses system default
    .with_font("CustomFont")  // Explicit font
    .inherit_font()           // Explicitly inherit from parent
```

### Documentation Needed
- Explain font inheritance rules
- When explicit font setting is required
- How to register fonts with FontManager
- Best practices for responsive font sizing

