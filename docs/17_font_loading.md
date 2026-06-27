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

---

## Example Screen: FontLoadingShowcase

**File:** `src/systems/screens/FontLoadingShowcase.h`
**CLI:** `--screen=font_loading`
**Category:** Infrastructure

### Layout

A typography test screen for font loading and metrics:

1. **Font Comparison** — The same sentence rendered in 3 different loaded fonts, each labeled with the font name and path. Demonstrates `load_font()` with different font files.

2. **CJK Character Test** — A line of Chinese (你好世界), Japanese (こんにちは), and Korean (안녕하세요) characters. A label below: "CJK rendering: OK/FAIL" based on whether glyphs render correctly vs. showing boxes.

3. **Glyph Metrics Display** — A word rendered at large size (48px) with per-character width markers drawn above each character. Below: a table showing `measure_char()` results for each character. Demonstrates `measure_substring()` accuracy.

4. **Caret Positioning Test** — A `text_input` where clicking precisely between characters places the caret correctly. Below: "Caret at byte offset: N, character: M". Uses `char_at_position()` for click-to-caret mapping.

5. **Font Size Scaling** — The same text at sizes 12, 16, 20, 24, 32, 48px showing consistent proportional scaling.

### Features Exercised

- `load_font()`, `load_font_full()`, `load_font_range()` for different glyph sets
- `fonts::measure_char()` and `fonts::measure_substring()` for precise widths
- `fonts::char_at_position()` for click-to-caret mapping
- CJK codepoint loading with `load_font_with_codepoints()`
- Font inheritance and default font behavior

### Verification

- CJK characters render as actual glyphs, not rectangles/boxes
- Per-character width markers match visual character boundaries
- Clicking between characters in text_input places caret at correct position
- Font sizes scale proportionally without distortion

### E2E Test Plan

**Test file:** `src/testing/tests/FontLoadingTest.h`

#### New Custom Commands Needed

None — font loading is primarily visual. Snapshot comparison is the main verification method. Unit-style assertions (like `FontConfigTest.h`) can verify glyph metric values without new commands.

#### Screenshots

1. `font_loading_families` — multiple font families rendered at 24px
2. `font_loading_cjk` — CJK characters rendered (vs. fallback boxes)
3. `font_loading_metrics` — per-character width markers above large text
4. `font_loading_sizes` — same text at sizes 12, 16, 20, 24, 32, 48px
5. `font_loading_caret` — text input with caret placed between characters

#### Test Script

```cpp
TEST(font_loading_render) {
  co_await TestApp::wait_for_frames(5);

  // Verify font families are displayed
  TestApp::expect_ui_exists("Default Font");
  TestApp::expect_ui_exists("Monospace");

  auto snap_families = TestApp::capture_snapshot("font_loading_families");
  auto snap_sizes = TestApp::capture_snapshot("font_loading_sizes");
}

TEST(font_loading_cjk) {
  co_await TestApp::wait_for_frames(5);

  // CJK text should exist as labels
  TestApp::expect_ui_exists("CJK rendering:");

  auto snap = TestApp::capture_snapshot("font_loading_cjk");
}

TEST(font_loading_caret_position) {
  co_await TestApp::wait_for_frames(5);

  // Click into caret positioning test input
  TestApp::click_button("Caret Test Input");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Verify caret offset readout exists
  TestApp::expect_ui_exists("Caret at byte offset:");
  auto snap = TestApp::capture_snapshot("font_loading_caret");
}

TEST(font_loading_metrics_accuracy) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("font_loading_metrics");

  // Visual regression: width markers should align with character edges
  auto compare = TestApp::compare_snapshot("font_loading_metrics");
  if (!compare.success) {
    throw std::runtime_error("Font metrics regression: " + compare.error_message);
  }
}
```

