# Type Safety Improvements

**Status:** Not implemented  
**Priority:** Low  
**Source:** break_ross migration experience

---

## Problem

Many API parameters use raw `float` types where stronger typing would catch errors at compile time and improve documentation.

### Current Issues

```cpp
// All of these are just floats - easy to mix up:
.with_font_size(24.0f)           // Is this pixels? Percentage? 
.with_opacity(0.8f)              // Must be 0-1, but not enforced
.with_border_radius(screen_pct(0.02f))  // This returns float too
.with_padding(Padding{0.05f})    // Is this pixels or percentage?
```

---

## Suggested Strong Types

### FontSize

```cpp
struct FontSize {
    float value;
    enum class Unit { Pixels, ScreenPercent, Em } unit;
    
    static FontSize pixels(float px) { return {px, Unit::Pixels}; }
    static FontSize screen_pct(float pct) { return {pct, Unit::ScreenPercent}; }
    static FontSize em(float multiplier) { return {multiplier, Unit::Em}; }
};

// Usage:
.with_font_size(FontSize::pixels(24))
.with_font_size(FontSize::screen_pct(0.02f))
.with_font_size(FontSize::em(1.5f))  // 1.5x parent font size
```

### Opacity

```cpp
struct Opacity {
    float value;  // Clamped to 0-1
    
    explicit Opacity(float v) : value(std::clamp(v, 0.0f, 1.0f)) {}
    
    static Opacity full() { return Opacity(1.0f); }
    static Opacity half() { return Opacity(0.5f); }
    static Opacity none() { return Opacity(0.0f); }
};

// Usage:
.with_opacity(Opacity(0.8f))
.with_opacity(Opacity::half())
```

### Percent

```cpp
struct Percent {
    float value;  // 0.0 to 1.0
    
    explicit Percent(float v) : value(v) {
        assert(v >= 0.0f && v <= 1.0f);
    }
    
    // Named constructors for clarity:
    static Percent from_ratio(float ratio) { return Percent(ratio); }
    static Percent from_percentage(float pct) { return Percent(pct / 100.0f); }
};

// Usage:
.with_width(Percent::from_percentage(50))  // 50%
.with_width(Percent::from_ratio(0.5f))     // Same thing
```

### Dimension (unifies pixels and percentages)

```cpp
struct Dimension {
    float value;
    enum class Unit { Pixels, Percent, Auto } unit;
    
    static Dimension px(float pixels) { return {pixels, Unit::Pixels}; }
    static Dimension pct(float percent) { return {percent, Unit::Percent}; }
    static Dimension auto_size() { return {0, Unit::Auto}; }
    
    // Resolve to pixels given parent size:
    float resolve(float parent_size) const {
        switch (unit) {
            case Unit::Pixels: return value;
            case Unit::Percent: return value * parent_size;
            case Unit::Auto: return 0;  // Calculated by layout
        }
    }
};

// Usage:
.with_width(Dimension::px(200))
.with_width(Dimension::pct(0.5f))
.with_height(Dimension::auto_size())
```

---

## Const Correctness

### Problem

Some functions could be more const-correct, making it clearer what can be modified.

### Examples

```cpp
// These should be const:
Color Theme::get_color(Usage usage) const;
Font& FontManager::get_font(const std::string& name) const;

// Query functions should clearly not modify state:
bool UIComponent::is_focused() const;
bool UIComponent::is_hovered() const;
Rectangle UIComponent::get_bounds() const;

// Mutable access should be explicit:
UIComponent& get_component_mut(Entity e);
const UIComponent& get_component(Entity e) const;
```

---

## Benefits of Strong Typing

1. **Compile-Time Errors**: Can't accidentally pass opacity where font size expected
2. **Self-Documenting**: `FontSize::pixels(24)` is clearer than `24.0f`
3. **Unit Conversion**: Types can handle conversion logic internally
4. **IDE Support**: Better autocomplete and documentation on hover
5. **Refactoring Safety**: Changes to unit handling are localized to type

---

## Migration Strategy

1. **Phase 1**: Add new strong types alongside existing float APIs
2. **Phase 2**: Deprecate float overloads with warnings
3. **Phase 3**: Remove float overloads in next major version

```cpp
// Phase 1: Both work
.with_font_size(24.0f)                    // Still works
.with_font_size(FontSize::pixels(24))     // New API

// Phase 2: Float version warns
.with_font_size(24.0f)  // Warning: Use FontSize::pixels() instead

// Phase 3: Only strong types
.with_font_size(FontSize::pixels(24))     // Only option
```

---

## Example Screen: TypeSafetyShowcase

**File:** `src/systems/screens/TypeSafetyShowcase.h`
**CLI:** `--screen=type_safety`
**Category:** DX / Developer Experience

### Layout

A screen demonstrating strong-typed APIs vs raw floats:

1. **FontSize Types** — Three text labels rendered with `FontSize::pixels(24)`, `FontSize::screen_pct(0.03f)`, and `FontSize::em(1.5f)`. Each labeled with the type used and the resolved pixel size at current resolution.

2. **Opacity Type** — A row of 5 colored boxes using `Opacity(1.0f)`, `Opacity(0.75f)`, `Opacity::half()`, `Opacity(0.25f)`, `Opacity::none()`. Labels show the clamped value. Attempting `Opacity(1.5f)` shows it clamps to 1.0.

3. **Dimension Type** — Three equal-looking panels created with `Dimension::px(200)`, `Dimension::pct(0.15f)`, `Dimension::auto_size()`. Labels show the resolved pixel value, demonstrating that percentage resolves relative to parent.

4. **Const Correctness** — A readout panel showing theme query results via const methods: `theme.get_color(Usage::Primary)`, `widget.is_focused()`, `widget.get_bounds()`. Each labeled "const" to indicate they don't mutate state.

### Features Exercised

- `FontSize::pixels()`, `FontSize::screen_pct()`, `FontSize::em()`
- `Opacity` with clamping and named constructors
- `Dimension::px()`, `Dimension::pct()`, `Dimension::auto_size()`
- Const-correct query methods on Theme and UIComponent

### Verification

- FontSize::pixels(24) always renders at 24px regardless of screen size
- FontSize::screen_pct scales with screen resolution
- Opacity(1.5) clamps to 1.0 (fully opaque)
- Dimension::pct(0.5) resolves to half of parent width

### E2E Test Plan

**Test file:** `src/testing/tests/TypeSafetyTest.h`

#### New Custom Commands Needed

- `expect_element_size(label, w, h, tolerance)` — check element dimensions. Needed to verify `Dimension::pct(0.5)` resolves to half of parent width.

#### Screenshots

1. `type_safety_font_sizes` — three labels at different FontSize types with resolved px shown
2. `type_safety_opacity` — 5 colored boxes at different opacities
3. `type_safety_dimensions` — 3 panels with px/pct/auto dimensions, resolved values shown
4. `type_safety_const` — readout panel showing const query results

#### Test Script

```cpp
TEST(type_safety_font_size) {
  co_await TestApp::wait_for_frames(5);

  // FontSize labels should exist with resolved values
  TestApp::expect_ui_exists("pixels(24)");
  TestApp::expect_ui_exists("screen_pct");

  auto snap = TestApp::capture_snapshot("type_safety_font_sizes");
}

TEST(type_safety_opacity_clamp) {
  co_await TestApp::wait_for_frames(5);

  // Verify clamping label
  TestApp::expect_ui_exists("1.0");  // Opacity(1.5) clamped to 1.0

  auto snap = TestApp::capture_snapshot("type_safety_opacity");
}

TEST(type_safety_dimension_resolve) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("type_safety_dimensions");

  // Verify pct(0.5) panel is approximately half parent width
  // Uses snapshot for visual regression
}

TEST(type_safety_const_queries) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("const");
  auto snap = TestApp::capture_snapshot("type_safety_const");
}
```

