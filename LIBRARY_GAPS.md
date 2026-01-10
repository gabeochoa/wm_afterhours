# Afterhours UI Library Gaps

Issues and limitations discovered during UI Showcase development.

---

## 1. ~~Font Configuration Not in Theme~~ (RESOLVED)

**Location:** Theme struct in `vendor/afterhours/src/plugins/ui/theme.h`

**Status:** RESOLVED - Font configuration added to Theme.

**Solution:** Added `FontConfig` struct and per-language font configuration to `Theme`:
```cpp
struct FontConfig {
    std::string font_name;      // Key in FontManager
    float size_scale = 1.0f;    // Multiplier for this font's visual size
};

struct Theme {
    // ... existing color fields ...
    
    // Per-language font configuration
    std::map<translation::Language, FontConfig> language_fonts;
    
    // Base font sizes (in pixels)
    float font_size_sm = 14.f;
    float font_size_md = 18.f;
    float font_size_lg = 28.f;
    float font_size_xl = 36.f;
    
    // Helpers
    const FontConfig& get_font_config(translation::Language lang) const;
    float get_scaled_font_size(translation::Language lang, float base_size) const;
    std::string get_font_name(translation::Language lang) const;
};
```

**Usage:**
```cpp
Theme theme;
theme.language_fonts = {
    {Language::English,  {"Gaegu-Bold", 1.0f}},
    {Language::Korean,   {"NotoSansKR", 0.95f}},
    {Language::Japanese, {"Sazanami", 0.92f}},
};

// At render time:
Language lang = TranslationPlugin::get_language();
std::string font_name = theme.get_font_name(lang);
float size = theme.get_scaled_font_size(lang, theme.font_size_md);
```

**TODO:** Investigate using FontID enum instead of strings for type safety.

---

## 2. Sizing API: `screen_pct()` vs `percent()` (NOT A GAP - Documentation)

**Location:** `vendor/afterhours/src/plugins/autolayout.h`

**Clarification:** There are TWO different percentage-based sizing functions:

- **`screen_pct(value)`** - Percentage of **screen** dimensions (uses `Dim::ScreenPercent`)
- **`percent(value)`** - Percentage of **parent** dimensions (uses `Dim::Percent`)

**Common Mistake:**
```cpp
// Parent is 50% of screen width
auto parent = div(context, mk(entity, 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));

// WRONG: Using screen_pct() makes child 50% of SCREEN, not parent
auto child = div(context, mk(parent.ent(), 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));
```

**Correct Usage:**
```cpp
// Parent is 50% of screen width
auto parent = div(context, mk(entity, 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));

// CORRECT: Using percent() makes child 50% of PARENT
auto child = div(context, mk(parent.ent(), 0),
    ComponentConfig{}.with_size({percent(0.5f), percent(0.5f)}));
```

**Summary of sizing functions:**
- `pixels(value)` - Fixed pixel size
- `percent(value)` - Percentage of parent container
- `screen_pct(value)` - Percentage of screen dimensions
- `children()` - Size based on children content
- `h720(px)` / `w1280(px)` - Resolution-independent pixels (scaled from 720p/1280p baseline)

---

## 3. Text Color vs Background Color Confusion in `div` (RESOLVED)

**Location:** `vendor/afterhours/src/plugins/ui/component_config.h`

**Status:** FIXED - API has been clarified with explicit naming.

**The Problem Was:** `with_color_usage()` set background color, but the name was ambiguous.

**Solution:** New explicit APIs added, old ones deprecated:

| Old API (deprecated) | New API |
|---------------------|---------|
| `with_color_usage()` | `with_background()` |
| `with_custom_color()` | `with_custom_background()` |
| *(new)* | `with_text_color()` |
| *(new)* | `with_custom_text_color()` |

**Correct Usage:**
```cpp
div(context, mk(parent.ent(), 0),
    ComponentConfig{}
        .with_label("Some text")
        .with_background(Theme::Usage::Primary)    // Sets BACKGROUND (clear!)
        .with_text_color(Theme::Usage::Accent));   // Sets TEXT (new!)
```

Old code using `with_color_usage()` still works but shows compiler deprecation warnings guiding migration.

---

## 4. Font Size 0 Warnings with No Visual Feedback

**Location:** `vendor/afterhours/src/plugins/font_helper.h` (line 133)

**Issue:** The `position_text` auto-sizer produces font size 0 when containers are too small relative to text content. This causes text to silently not render with only a console warning.

**Symptoms:**
- Text doesn't appear in UI
- Console shows: `Invalid font size 0 passed to measure_text_utf8`
- No visual indication of what's wrong

**Current Workaround:** Make containers larger, or use fixed font sizes instead of auto-sizing.

**Suggested Fix:** Add minimum font size clamping, or visual debug indicator when text can't fit.

---

## 5. Missing Flexbox Alignment Properties

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h`

**Issue:** `with_justify_content()` and `with_align_items()` are not available in the imm `ComponentConfig` API, making it difficult to center content within containers.

**Current Workaround:** Use manual spacing/padding or fixed positioning.

**Suggested Fix:** Add these standard flexbox properties to `ComponentConfig`.

---

## 6. CJK Font Loading Requires Special Handling

**Location:** `vendor/afterhours/src/plugins/ui/font_manager.h`

**Issue:** Standard `load_font()` only loads ASCII glyphs (codepoints 32-126). CJK characters require using `load_font_with_codepoints()` which is not obvious from the API.

**Example Problem:**
```cpp
// This won't render Korean/Japanese/Chinese characters
font_manager.load_font("NotoSansKR", korean_font_path);

// Must use this instead (not obvious!)
font_manager.load_font_with_codepoints("NotoSansKR", korean_font_path, 
    0x0000, 0xFFFF);  // Or specific CJK ranges
```

**Current Workaround:** Manually use `load_font_with_codepoints()` for any non-ASCII fonts.

**Suggested Fix:** Either auto-detect font requirements, or clearly document this in the API. Consider making `load_font()` load more codepoints by default.

---

## 7. Dropdown Close Behavior

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~760)

**Issue:** No built-in way to close dropdown when clicking outside of it. Dropdowns remain open until user selects an option.

**Current Workaround:** None available - dropdowns stay open.

**Suggested Fix:** Add click-outside detection that closes any open dropdown.

---

## 8. Circular Dependency with Styling Defaults

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (lines 60-61)

**Issue:** `UIStylingDefaults` cannot be stored in `UIContext` due to circular dependency.

**Code Comment:**
```cpp
// TODO: Add styling defaults back when circular dependency is resolved
// imm::UIStylingDefaults styling_defaults;
```

**Suggested Fix:** Refactor to break the circular dependency, possibly by forward declarations or moving types.

---

## 9. Focus Ring Positioning on Checkbox

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~220)

**Issue:** Checkbox focus ring is drawn on the wrong element. The clickable area is `checkbox_no_label` but focus ring appears elsewhere.

**Code Comment:**
```cpp
// TODO the focus ring is not correct because the actual clickable area is the
// checkbox_no_label element, not the checkbox element.
```

**Suggested Fix:** Move focus ring rendering to the actual clickable element.

---

## 10. Checkbox Corner Config Merging

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~258)

**Issue:** No way to merge user-provided corner configuration with internal checkbox styling.

**Code Comment:**
```cpp
// TODO - if the user wants to mess with the corners, how can we merge
// these
```

**Suggested Fix:** Allow corner config to be passed through and merged with component defaults.

---

## 11. Tabbing with Value Controls

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (line ~123)

**Issue:** Unclear how to handle components that want to use Widget Value Down/Up to control values while also participating in tabbing navigation.

**Code Comment:**
```cpp
// TODO How do we handle something that wants to use
// Widget Value Down/Up to control the value?
```

**Suggested Fix:** Add a mode or flag to distinguish between navigation and value adjustment contexts.

---

## Priority

| Gap | Severity | User Impact | Status |
|-----|----------|-------------|--------|
| Font in Theme | Medium | Theming feels incomplete | RESOLVED |
| `screen_pct()` nesting | High | Layout overflow, hard to debug | Open |
| Text vs Background color | High | Invisible text, confusing API | Open |
| Font size 0 warnings | Medium | Silent failures, hard to debug | Open |
| Missing flexbox alignment | Medium | Can't center content easily | Open |
| CJK font loading | Medium | Non-ASCII text doesn't render | Open |
| Dropdown close | High | Poor UX, confusing behavior | Open |
| Styling defaults | Low | Internal architecture issue | Open |
| Focus ring | Medium | Accessibility concern | Open |
| Corner merging | Low | Minor customization limit | Open |
| Tabbing + values | Medium | Complex widget support | Open |

