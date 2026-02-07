# Absolute Positioning and Layout Issues

**Status:** Critical bugs  
**Priority:** High  
**Source:** break_ross migration experience

---

## Overview

Multiple interrelated issues with absolute positioning make it difficult to use correctly. These issues cause elements to be invisible, incorrectly sized, or incorrectly positioned without any warnings.

---

## Issue 1: Margins Cause Negative Sizes (CRITICAL)

### Problem

When using `.with_absolute_position()` with margins, the autolayout system subtracts margins from the element's width/height, causing negative sizes and invisible elements.

### Example of the Bug

```cpp
// This creates an element with NEGATIVE width:
imm::div(context, mk(parent, 0),
    ComponentConfig{}
        .with_size(ComponentSize{screen_pct(0.4f), screen_pct(0.6f)})
        .with_margin(Margin{.left = screen_pct(0.3f), .right = screen_pct(0.3f)})
        .with_absolute_position()
);

// On a 1280px wide screen:
// desired_width = 0.4 * 1280 = 512px
// margin.left = 0.3 * 1280 = 384px
// margin.right = 0.3 * 1280 = 384px
// calculated_width = 512 - 384 - 384 = -256px (NEGATIVE!)
```

### Why This Happens

The autolayout system applies the formula:
```
final_width = desired_width - margin.left - margin.right
```

This is correct for flow layout (margins reduce available space) but wrong for absolute positioning (margins should position, not resize).

### Workaround

Use `with_translate()` instead of margins for absolute positioning:

```cpp
// WRONG: Causes negative size
.with_margin(Margin{.left = screen_pct(0.3f)})
.with_absolute_position()

// CORRECT: Use translate for positioning
.with_translate({screen_pct(0.3f), 0})
.with_absolute_position()
```

### Suggested Fix

```cpp
// In autolayout, skip margin subtraction for absolute elements:
void calculate_size(UIComponent& widget) {
    if (widget.is_absolute_positioned) {
        // Margins position the element, don't affect size
        widget.computed_x = widget.desired_width;
        widget.computed_y = widget.desired_height;
        widget.position.x = widget.margin.left;
        widget.position.y = widget.margin.top;
    } else {
        // Normal flow: margins reduce available space
        widget.computed_x = widget.desired_width - margin.left - margin.right;
        widget.computed_y = widget.desired_height - margin.top - margin.bottom;
    }
}
```

---

## Issue 2: Negative Sizes Not Detected or Warned

### Problem

The UI system allows elements to have negative widths/heights without any warning, causing silent rendering failures.

### Current Behavior
- Elements with negative sizes are created and queued for rendering
- They appear in `render_cmds` but don't render
- No validation or warning that sizes are invalid
- Debugging requires manually checking computed sizes in logs

### Suggested Fix

```cpp
// Add validation after autolayout calculation:
void validate_computed_sizes(UIComponent& widget) {
    if (widget.computed[Axis::X] < 0 || widget.computed[Axis::Y] < 0) {
        log_warn("Element '{}' has negative computed size: {}x{}\n"
                 "  This is often caused by using margins with absolute positioning.\n"
                 "  Suggestion: Use with_translate() instead of with_margin() for\n"
                 "  absolute positioned elements.",
                 widget.debug_name,
                 widget.computed[Axis::X],
                 widget.computed[Axis::Y]);
        
        // In debug mode, render a visible error indicator
        #ifdef DEBUG
        widget.computed[Axis::X] = 50;  // Minimum visible size
        widget.computed[Axis::Y] = 50;
        widget.has_error = true;  // Render in error color
        #endif
    }
}
```

---

## Issue 3: Position Logging Doesn't Show Translate

### Problem

When using `with_translate()` for positioning, the logged position shows as (0, 0) because translate is applied via `HasUIModifiers` during rendering, not stored in the base position.

### Current Behavior
```
[DEBUG] Element 'my_panel': x=0.0, y=0.0, w=200, h=100
// But it's actually rendered at (384, 144) due to translate!
```

### Suggested Fix

```cpp
// Include translate in debug logging:
void log_element_position(const UIComponent& widget, const HasUIModifiers* mods) {
    float final_x = widget.position.x;
    float final_y = widget.position.y;
    
    if (mods) {
        final_x += mods->translate.x;
        final_y += mods->translate.y;
    }
    
    log_debug("Element '{}': base=({}, {}), translate=({}, {}), final=({}, {})",
              widget.debug_name,
              widget.position.x, widget.position.y,
              mods ? mods->translate.x : 0, mods ? mods->translate.y : 0,
              final_x, final_y);
}

// Helper function for code:
Vector2 get_final_position(Entity entity) {
    auto& widget = entity.get<UIComponent>();
    auto* mods = entity.try_get<HasUIModifiers>();
    return {
        widget.position.x + (mods ? mods->translate.x : 0),
        widget.position.y + (mods ? mods->translate.y : 0)
    };
}
```

---

## Issue 4: Mixing Absolute and Relative Positioning

### Problem

Using `.with_absolute_position()` requires understanding how it interacts with parent containers. Children of absolutely positioned elements may not layout correctly.

### Gotchas

1. **Children need explicit sizing**: Children of absolute elements often can't use `fill_parent()` reliably
2. **Z-ordering confusion**: Absolute elements may render behind relative siblings
3. **No flow participation**: Absolute elements don't contribute to parent size calculation

### Documentation Needed

```cpp
// PATTERN 1: Absolute container with absolute children (explicit sizes)
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_absolute_position()
        .with_translate({100, 100})
        .with_size({400, 300})
);
    // Children should also use explicit sizes, not fill_parent()

// PATTERN 2: Absolute overlay on top of flow content
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}  // Normal flow
        .with_size(fill_parent())
);
imm::div(ctx, mk(parent, 1),
    ComponentConfig{}  // Absolute overlay
        .with_absolute_position()
        .with_translate({...})
        .with_size({...})
        .with_z_index(10)  // Ensure on top
);

// PATTERN 3: Modal dialog (absolute centered)
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_absolute_position()
        .with_size({screen_pct(0.5f), screen_pct(0.5f)})
        .with_translate({screen_pct(0.25f), screen_pct(0.25f)})
        // Or use position_center() helper if implemented
);
```

---

## Suggested API Improvements

```cpp
// Clear positioning API that avoids the margin confusion:
ComponentConfig{}
    .with_absolute_position_at(x, y)  // Position + absolute in one call
    .with_size({w, h});

// Or anchor-based positioning:
ComponentConfig{}
    .with_anchor(Anchor::TopLeft, offset_x, offset_y)
    .with_anchor(Anchor::Center)
    .with_anchor(Anchor::BottomRight, -margin, -margin)

// Deprecate margin usage with absolute positioning:
ComponentConfig{}
    .with_margin(...)           // OK for flow layout
    .with_absolute_position()   // Triggers warning:
    // "Warning: margins with absolute positioning may cause unexpected sizing.
    //  Use with_translate() or with_absolute_position_at() instead."
```

---

## Example Screen: AbsolutePositioningShowcase

**File:** `src/systems/screens/AbsolutePositioningShowcase.h`
**CLI:** `--screen=absolute_positioning`
**Category:** DX / Developer Experience

### Layout

A screen demonstrating correct absolute positioning patterns and bug verification:

1. **Bug Repro: Margin + Absolute** — A div using `with_margin().with_absolute_position()` (the old buggy pattern). After the fix: element renders at correct size. Before the fix: element would have negative width. A label shows `computed_width` to confirm it's positive.

2. **Correct Pattern: Translate + Absolute** — The same element positioned correctly using `with_translate().with_absolute_position()`. Side by side with the margin version to show they now produce identical results.

3. **Corner Positioning** — 4 colored squares, one at each corner of the screen, using `with_absolute_position_at(x, y)`. Each labeled with its position method.

4. **Centered Overlay** — A semi-transparent dark overlay covering the full screen with a centered dialog box using `position_center()` + absolute positioning. Demonstrates the modal positioning pattern.

5. **Debug Readout** — A panel showing for each absolutely positioned element: base position, translate offset, final position. Verifies that `get_final_position()` includes translate in its calculation.

6. **Mixed Layout** — A normal flow container with 3 children, plus an absolutely positioned overlay badge in the top-right corner of the container. Shows absolute + relative mixing correctly.

### Features Exercised

- `with_absolute_position()` with `with_translate()` (correct pattern)
- Negative size detection and warning
- `with_absolute_position_at(x, y)` helper
- `get_final_position()` including translate in debug output
- Mixing absolute and relative positioning

### Verification

- No element has negative computed size
- Margin+absolute warning appears in debug mode
- Corner elements are exactly at the 4 corners
- Centered overlay is visually centered on screen
- Debug readout shows correct final positions including translate

### E2E Test Plan

**Test file:** `src/testing/tests/AbsolutePositioningTest.h`

#### New Custom Commands Needed

- `expect_element_position(label, x, y, tolerance)` — check element position. Core assertion for positioning tests.
- `expect_element_size(label, w, h, tolerance)` — check element size. Needed to verify no negative sizes.

#### Screenshots

1. `abspos_basic` — element at (100, 100) via translate
2. `abspos_margin_warning` — element with margin+absolute showing debug warning
3. `abspos_corners` — 4 squares at screen corners
4. `abspos_centered_overlay` — centered dialog on dark backdrop
5. `abspos_mixed_layout` — flow container with absolute overlay badge
6. `abspos_debug_readout` — debug panel showing base/translate/final positions

#### Test Script

```cpp
TEST(abspos_corners) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Top Left");
  TestApp::expect_ui_exists("Top Right");
  TestApp::expect_ui_exists("Bottom Left");
  TestApp::expect_ui_exists("Bottom Right");

  auto snap = TestApp::capture_snapshot("abspos_corners");

  // Verify top-left is near (0, 0)
  expect_element_position("Top Left", 0, 0, 30.0f);
}

TEST(abspos_no_negative_size) {
  co_await TestApp::wait_for_frames(5);

  // The margin+absolute combo element should still have valid size
  auto *elem = TestApp::find_ui_element_by_label("Margin + Absolute");
  if (elem && elem->has<afterhours::ui::UIComponent>()) {
    auto rect = elem->get<afterhours::ui::UIComponent>().rect();
    if (rect.width < 0 || rect.height < 0) {
      throw std::runtime_error("Negative size detected!");
    }
  }

  auto snap = TestApp::capture_snapshot("abspos_margin_warning");
}

TEST(abspos_centered_overlay) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Dialog Title");
  auto snap = TestApp::capture_snapshot("abspos_centered_overlay");
}

TEST(abspos_debug_readout) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("base position:");
  TestApp::expect_ui_exists("translate:");
  TestApp::expect_ui_exists("final position:");

  auto snap = TestApp::capture_snapshot("abspos_debug_readout");
}
```

