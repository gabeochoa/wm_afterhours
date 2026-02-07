# ComponentConfig Issues

**Status:** Not implemented  
**Priority:** High  
**Source:** break_ross migration experience

---

## Text Elements Requiring Background Colors

### Problem

Text elements with `.with_label()` may not render correctly without an explicit background color. The rendering system checks for `HasColor` OR `HasLabel`, but in practice, text elements often need a background color to be visible or to trigger proper layout calculations.

### Current Issues
- Unclear whether text-only elements need `with_color_usage()` or `with_custom_color()`
- Text elements without backgrounds may not render or may have layout issues
- No clear documentation on when background colors are required vs optional

### Example

```cpp
// This might not render correctly:
imm::text(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_label("Hello World")
        .with_font_size(screen_pct(0.02f))
);

// This works:
imm::text(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_label("Hello World")
        .with_font_size(screen_pct(0.02f))
        .with_custom_color(Color{0, 0, 0, 0})  // Transparent background
);
```

### Suggested Fixes

1. **Documentation**: Clearly document that text elements should either:
   - Have a background color (even if transparent) for proper rendering
   - Or be inside a container with a background color

2. **Default Behavior**: Consider automatically adding a transparent background to text-only elements if no color is specified

3. **Validation**: Add runtime warnings when text elements are created without backgrounds in contexts where they might not render

```cpp
// In debug builds, warn about potentially invisible text:
void validate_text_element(const UIComponent& widget) {
    if (widget.has_label && !widget.has_color) {
        log_warn("Text element '{}' has no background color.\n"
                 "  This may cause rendering issues.\n"
                 "  Suggestion: Add .with_custom_color(Color{0,0,0,0}) for transparent background.",
                 widget.debug_name);
    }
}
```

---

## Button Disabled State

### Problem

Can use `.with_disabled()` but it's not immediately clear how disabled buttons are styled or if they're automatically styled differently.

### Current Issues
- Documentation doesn't explain disabled button appearance
- Unclear if disabled buttons get automatic styling
- Need to verify disabled buttons don't trigger click handlers

### Suggested Improvements

```cpp
// Document disabled state behavior:
// 1. Disabled buttons use Theme::Usage::DisabledForeground and DisabledBackground
// 2. Disabled buttons don't respond to hover or click
// 3. Focus can still move to disabled buttons (for accessibility) but they don't activate

// Make disabled styling explicit:
struct Theme {
    Color disabled_foreground = {128, 128, 128, 255};  // Gray
    Color disabled_background = {200, 200, 200, 255};  // Light gray
    float disabled_opacity = 0.5f;  // Or reduce opacity
};

// Or allow customization:
ComponentConfig{}
    .with_disabled()
    .with_disabled_style(DisabledStyle::Grayed)  // Options: Grayed, Transparent, Custom
    .with_disabled_opacity(0.4f);
```

---

## Property Dependencies and Invalid Combinations

### Problem

Some ComponentConfig properties have hidden dependencies or requirements that aren't obvious. For example, text elements might need background colors, or absolute positioning might require specific parent configurations.

### Current Issues
- Property combinations that don't work aren't documented
- No validation of invalid property combinations
- Easy to create elements that don't render without understanding why

### Examples of Problematic Combinations

```cpp
// Problem 1: Margin with absolute positioning (causes negative sizes)
.with_margin({...})
.with_absolute_position()  // These don't work together correctly

// Problem 2: fill_parent() with no parent size
.with_size(fill_parent())
// Parent must have explicit size or this becomes 0

// Problem 3: Text without font
.with_label("Hello")
// Font must be set explicitly or inherited

// Problem 4: Percentage size without reference
.with_size(screen_pct(0.5f))
.with_absolute_position()
// What is 50% of? Behavior may be unexpected
```

### Suggested Validation

```cpp
// Runtime validation for common issues:
void validate_config(const ComponentConfig& config, const UIComponent& parent) {
    // Check margin + absolute positioning
    if (config.has_margin() && config.is_absolute()) {
        log_warn("Using margins with absolute positioning may cause sizing issues.\n"
                 "  Suggestion: Use with_translate() instead of with_margin().");
    }
    
    // Check fill_parent without parent size
    if (config.uses_fill_parent() && !parent.has_explicit_size()) {
        log_warn("Using fill_parent() but parent has no explicit size.\n"
                 "  This may result in zero size.");
    }
    
    // Check text without font
    if (config.has_label() && !config.has_font()) {
        log_warn("Text element has no font specified.\n"
                 "  Font will be inherited or use default.");
    }
}
```

### Helper Functions for Valid Combinations

```cpp
// Helpers that guarantee valid configurations:

// Text label with transparent background (always renders)
ComponentConfig text_label_config(const std::string& text) {
    return ComponentConfig{}
        .with_label(text)
        .with_custom_color(Color{0, 0, 0, 0})
        .with_default_font();
}

// Absolutely positioned element (uses translate, not margin)
ComponentConfig absolute_at(float x, float y, ComponentSize size) {
    return ComponentConfig{}
        .with_absolute_position()
        .with_translate({x, y})
        .with_size(size);
}

// Button with proper disabled handling
ComponentConfig button_config(bool disabled = false) {
    auto config = ComponentConfig{}
        .with_color_usage(Theme::Usage::Primary);
    if (disabled) {
        config.with_disabled()
              .with_color_usage(Theme::Usage::Disabled);
    }
    return config;
}
```

---

## Debug Mode Validation

```cpp
// In debug builds, comprehensive validation:
#ifdef DEBUG
    #define VALIDATE_CONFIG(config, parent) \
        ui::debug::validate_config(config, parent, __FILE__, __LINE__)
#else
    #define VALIDATE_CONFIG(config, parent) ((void)0)
#endif

// Validation runs before element creation:
Entity create_element(EntityParent parent, const ComponentConfig& config) {
    VALIDATE_CONFIG(config, parent);
    // ... normal creation logic
}
```

---

## Example Screen: ConfigValidationShowcase

**File:** `src/systems/screens/ConfigValidationShowcase.h`
**CLI:** `--screen=config_validation`
**Category:** DX / Developer Experience

### Layout

A screen that stress-tests ComponentConfig edge cases and validation:

1. **Text Without Background** — Two labels side by side: one with `with_custom_color(transparent)` (correct) and one without any color (the problematic case). A debug warning should appear for the one missing a background. After the fix, both render correctly.

2. **Disabled Button States** — A row of 4 buttons: Normal, Hovered, Focused, Disabled. The disabled button uses `with_disabled()`. Labels show: whether click handler fires (should not for disabled), styling differences (grayed out), and whether focus can still move to it.

3. **Invalid Config Combos** — Three panels demonstrating problematic combinations:
   - `with_margin() + with_absolute_position()` → warning + workaround applied
   - `fill_parent()` with no parent size → warning, shows 0 size
   - `with_label()` without `with_font()` → warning about font inheritance

4. **Helper Function Panel** — A mini form built with safe helper functions: `text_label_config("text")`, `absolute_at(x, y, size)`, `button_config(disabled)`. Each guarantees a valid configuration.

5. **Validation Log** — A scrollable log panel at the bottom showing all validation warnings generated by the elements above. Each warning includes element name, issue, and suggestion.

### Features Exercised

- Text element rendering with/without background color
- `with_disabled()` button styling and interaction blocking
- `validate_config()` for invalid property combinations
- `text_label_config()`, `absolute_at()`, `button_config()` helpers
- Runtime validation warnings with helpful suggestions

### Verification

- Text without background still renders (after default transparent BG fix)
- Disabled button ignores click but remains visible
- Margin+absolute combo triggers a warning in the validation log
- Helper functions produce correct, warning-free elements
- All validation messages include actionable suggestions

### E2E Test Plan

**Test file:** `src/testing/tests/ConfigValidationTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `capture_snapshot`.

#### Screenshots

1. `config_validation_initial` — all sections visible, validation log at bottom
2. `config_validation_text_no_bg` — text element rendering correctly without explicit background
3. `config_validation_disabled_btn` — disabled button showing grayed-out styling
4. `config_validation_invalid_combos` — invalid config combos with warning messages
5. `config_validation_helpers` — helper-built form with no warnings

#### Test Script

```cpp
TEST(config_validation_disabled_button) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("config_validation_initial");

  // Click disabled button — should NOT fire handler
  TestApp::click_button("Disabled");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // No click event text should appear
  TestApp::expect_ui_not_exists("Disabled clicked");
  auto snap2 = TestApp::capture_snapshot("config_validation_disabled_btn");
}

TEST(config_validation_warnings) {
  co_await TestApp::wait_for_frames(5);

  // Validation log should show warnings for invalid combos
  TestApp::expect_ui_exists("margin");
  TestApp::expect_ui_exists("absolute");

  auto snap = TestApp::capture_snapshot("config_validation_invalid_combos");
}

TEST(config_validation_helpers_clean) {
  co_await TestApp::wait_for_frames(5);

  // Helper section should produce no warnings
  auto snap = TestApp::capture_snapshot("config_validation_helpers");
}
```

