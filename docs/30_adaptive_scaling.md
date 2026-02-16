# Doc 30: Adaptive Scaling Mode

**Status:** Implemented (ScalingMode enum, AdaptiveScalingDemo screen)

## The Web Model

In a web browser, there are two independent controls:

1. **Ctrl+/Ctrl-** (zoom) — Makes everything bigger/smaller. A 16px font becomes 24px at 150% zoom. A 200px-wide button becomes 300px. The *layout* also changes because elements are now physically larger, so fewer fit on screen.

2. **Dragging the window edge** (resize) — Changes available space. Elements stay the same size. The layout reflows: columns wrap, sidebars collapse, content scrolls.

These are independent. You can zoom to 200% in a tiny window (everything huge, very little fits) or zoom to 50% in a huge window (everything small, tons of content visible).

### Mapping to Afterhours

| Web concept | Afterhours equivalent |
|---|---|
| Zoom level (Ctrl+/-) | `ui_scale` (float, default 1.0) |
| Window resize | Resolution change (already exists) |
| CSS `px` | `pixels()` — in Adaptive mode, multiplied by `ui_scale` |
| CSS `%` | `percent()` — percentage of parent (unchanged) |
| CSS `vw`/`vh` | `screen_pct()` — percentage of viewport (unchanged) |
| CSS media queries | Breakpoint helpers based on available logical space |

## Scaling Modes

```cpp
enum class ScalingMode {
  Proportional,  // Current behavior. Resolution changes scale everything.
  Adaptive,      // Web-like. Resolution changes reflow layout. ui_scale controls element sizes.
};
```

### Proportional (default, current behavior)

- `pixels(100)` = 100 hardware pixels, always
- `h720(18)` = `screen_pct(18/720)` = scales with screen height
- Window gets bigger → everything gets bigger
- This is good for games that render to a fixed reference resolution

### Adaptive (new)

- `pixels(100)` = 100 * ui_scale hardware pixels
- `h720(18)` = `screen_pct(18/720)` = still scales with screen height (use `pixels()` instead for Adaptive)
- `percent()`, `screen_pct()` unchanged — still useful for layout proportions
- Window gets bigger → more space, same-sized elements, layout reflows
- Ctrl+/Ctrl- → elements get bigger/smaller, layout reflows
- This is good for tools, editors, settings screens, apps

## Where `ui_scale` Lives

### In the Theme

```cpp
struct Theme {
  // ... existing fields ...
  float ui_scale = 1.0f;  // 1.0 = 100%, 1.5 = 150%
};
```

When you set `context.theme.ui_scale = 1.5`, all `pixels()` values on that screen resolve 50% larger. The theme is already per-frame, so changes take effect immediately.

### In UIStylingDefaults (app-wide default)

```cpp
struct UIStylingDefaults {
  ScalingMode scaling_mode = ScalingMode::Proportional;
  float ui_scale = 1.0f;
  // ...
};
```

### Per-screen override

Screens set their mode in `for_each_with()`:
```cpp
void for_each_with(Entity &entity, UIContext<InputAction> &context, float) override {
  context.scaling_mode = ScalingMode::Adaptive;
  context.theme.ui_scale = Settings::get().ui_scale; // from user preference
  // ...
}
```

### Per-component override

```cpp
// Background image should stretch with resolution, not scale with ui_scale
div(context, mk(entity, 0),
    ComponentConfig{}
        .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
        .with_scaling_mode(ScalingMode::Proportional));
```

### Resolution order

Component `.with_scaling_mode()` > Screen `context.scaling_mode` > App `UIStylingDefaults::scaling_mode`

## Implementation: Where Pixels Get Resolved

The `AutoLayout` struct resolves `Dim::Pixels` in these places (all in `autolayout.h`):

1. **Margin computation** (`compute_margin_for_exp`, line ~128): `case Dim::Pixels: return exp.value;`
2. **Padding computation** (`compute_padding_for_standalone_exp`, line ~191): `case Dim::Pixels: return exp.value;`
3. **Size computation** (`compute_size_for_standalone_exp`, line ~231): `case Dim::Pixels: return exp.value;`
4. **Constraint resolution** (`resolve_constraint`, line ~598): `case Dim::Pixels: return constraint.value;`
5. **Font size resolution** (`resolve_to_pixels` in `layout_types.h`, line ~162): `case Dim::Pixels: return size.value;`

In Adaptive mode, each of these becomes `return exp.value * ui_scale;`

### How AutoLayout Gets the Scale

The `AutoLayout` struct already receives the `resolution`. We add `ui_scale` and `scaling_mode`:

```cpp
struct AutoLayout {
  window_manager::Resolution resolution;
  ScalingMode scaling_mode = ScalingMode::Proportional;
  float ui_scale = 1.0f;
  // ...
  
  // Resolve pixel values — applies ui_scale in Adaptive mode
  float resolve_pixels(float value, const UIComponent &widget) const {
    ScalingMode mode = widget.resolved_scaling_mode;
    if (mode == ScalingMode::Adaptive) {
      return value * ui_scale;
    }
    return value;
  }
};
```

Then every `case Dim::Pixels: return exp.value;` becomes `case Dim::Pixels: return resolve_pixels(exp.value, widget);`

The `widget.resolved_scaling_mode` is set during component creation from the cascade: component override > screen context > app default.

### Absolute Position Scaling

Absolute positions are stored as raw floats on `UIComponent::abs_position`. During layout, the autolayout system scales these in Adaptive mode:

```cpp
// In the absolute positioning pass of autolayout:
float abs_x = widget.abs_position.x;
float abs_y = widget.abs_position.y;
if (widget.resolved_scaling_mode == ScalingMode::Adaptive) {
  abs_x *= ui_scale;
  abs_y *= ui_scale;
}
widget.computed_rel[Axis::X] = abs_x;
widget.computed_rel[Axis::Y] = abs_y;
```

This matches CSS behavior where `left: 50px` at 150% zoom = 75 physical pixels.

## Converting Screens

### Pattern B screens (Forms, Buttons, Cards) — minimal changes

These already use `percent()` for layout and `pixels()` for sizes. Just add the mode:

```cpp
// Before
void for_each_with(...) {
  UIStylingDefaults::get().set_default_font("EqProRounded", h720(16.0f));
  auto root = div(context, mk(entity, 0),
      ComponentConfig{}.with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)}));

  auto row = hstack(context, mk(root.ent(), 1),
      ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(80)}));
}

// After — add mode, swap h720() to pixels() for fonts
void for_each_with(...) {
  context.scaling_mode = ScalingMode::Adaptive;
  UIStylingDefaults::get().set_default_font("EqProRounded", pixels(16.0f));
  
  // screen_pct(0.90f) still works — 90% of viewport
  auto root = div(context, mk(entity, 0),
      ComponentConfig{}.with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)}));

  // pixels(80) now scales with ui_scale automatically
  auto row = hstack(context, mk(root.ent(), 1),
      ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(80)}));
}
```

### Pattern A screens (SportsSettings, FighterMenu) — more changes

These use manual `sx`/`sy` scale factors and absolute positioning. Converting to Adaptive:

```cpp
// Before
float sx = screen_w / 1280.0f;
float sy = screen_h / 720.0f;
div(context, mk(entity, 5),
    ComponentConfig{}
        .with_size(ComponentSize{pixels(200 * sx), pixels(44 * sy)})
        .with_absolute_position(50.0f * sx, 12.0f * sy)
        .with_font("EqProRounded", h720(18.0f)));

// After — drop sx/sy, pixels() and absolute_position auto-scale with ui_scale
context.scaling_mode = ScalingMode::Adaptive;
div(context, mk(entity, 5),
    ComponentConfig{}
        .with_size(ComponentSize{pixels(200), pixels(44)})
        .with_absolute_position(50.0f, 12.0f)  // auto-scaled by ui_scale in Adaptive
        .with_font("EqProRounded", pixels(18.0f)));
```

**Absolute positions auto-scale.** On the web, `left: 50px` at 150% zoom = 75 physical pixels. The autolayout system multiplies absolute position floats by `ui_scale` in Adaptive mode. No API change needed — existing `with_absolute_position(50.0f, 12.0f)` just works.

## Breakpoints

Helper to determine available logical space:

```cpp
namespace afterhours::ui {

struct LayoutInfo {
  float screen_w;       // physical pixels
  float screen_h;       // physical pixels
  float ui_scale;       // current zoom level
  float logical_w;      // screen_w / ui_scale — how many "CSS pixels" fit
  float logical_h;      // screen_h / ui_scale
  ScalingMode mode;

  bool is_compact_width() const { return logical_w < 640; }
  bool is_medium_width()  const { return logical_w >= 640 && logical_w < 1024; }
  bool is_wide()          const { return logical_w >= 1024; }
  bool is_ultrawide()     const { return logical_w >= 1600; }
  
  bool is_short()         const { return logical_h < 500; }

  // Factory: read from current ECS state
  static LayoutInfo current();
};

} // namespace afterhours::ui
```

Usage in screens:
```cpp
auto layout = LayoutInfo::current();
bool show_sidebar = layout.is_wide();
bool use_compact_rows = layout.is_compact_width();
```

## E2E Visibility

Commands like `expect_text` should only match elements that are actually visible on screen. Add a visibility check:

```cpp
bool is_visible_in_viewport(const UIComponent &cmp, float vw, float vh) {
  if (!cmp.was_rendered_to_screen || cmp.should_hide) return false;
  auto r = cmp.rect();
  if (r.width < 1 || r.height < 1) return false;
  // Must have at least 1px visible in viewport
  return r.x + r.width > 0 && r.y + r.height > 0 && r.x < vw && r.y < vh;
}
```

Update `HandleExpectTextCommand`, `HandleExpectUIExistsCommand`, etc. to use this check. If text exists but is off-screen, the command should fail (or have a separate `expect_text_exists` for existence-only checks).

## DefaultSpacing and Typography

`DefaultSpacing::tiny()` currently returns `h720(8.0f)` (ScreenPercent). In Adaptive mode, this still scales with resolution, not ui_scale. DefaultSpacing is made mode-aware — see Design Decision #2 below for the approach.

## File Changes

| File | Change | Risk |
|---|---|---|
| `layout_types.h` | No changes to Dim enum. Add `ScalingMode` enum here (shared across files). | None |
| `theme.h` | Add `float ui_scale = 1.0f` to Theme, `Builder::with_ui_scale()` | Low — additive |
| `styling_defaults.h` | Add `ScalingMode scaling_mode` to UIStylingDefaults, mode-aware DefaultSpacing + TypographyScale | Medium — DefaultSpacing behavior |
| `component_config.h` | Add `optional<ScalingMode> scaling_mode`, `with_scaling_mode()` | Low — additive |
| `ui_core_components.h` | Add `ScalingMode resolved_scaling_mode` to UIComponent | Low — additive |
| `autolayout.h` | Add `ui_scale`+`scaling_mode` to AutoLayout, `resolve_pixels()` helper, update 5 pixel switch cases + absolute position scaling | Medium — core logic change |
| `context.h` or similar | Add `optional<ScalingMode> scaling_mode` to UIContext | Low — additive |
| New: `layout_info.h` | LayoutInfo struct with breakpoint helpers | None — new file |
| `command_handlers.h` | Add viewport visibility check to `expect_text`, `expect_ui_exists` | Medium |
| `rendering.h` | Resolve font `Dim::Pixels` with `ui_scale` in Adaptive mode | Medium |

## Migration Plan

### Phase 1: Infrastructure (no behavior change) -- DONE
- [x] Add `ScalingMode` enum to `layout_types.h`
- [x] Add `float ui_scale = 1.0f` to Theme, `Builder::with_ui_scale()`
- [x] Add `ScalingMode scaling_mode` to UIStylingDefaults
- [x] Add `optional<ScalingMode> scaling_mode` to UIContext and ComponentConfig
- [x] Add `ScalingMode resolved_scaling_mode` to UIComponent (set during creation)
- [x] Add `resolve_pixels()` to AutoLayout (default Proportional = no behavior change)
- [x] Scale absolute positions by `ui_scale` in Adaptive mode
- [x] Add `LayoutInfo` breakpoint helper
- [x] Add `AdaptiveScalingDemo` screen for interactive testing

### Phase 2: DefaultSpacing becomes mode-aware -- DONE
- [x] `DefaultSpacing::is_adaptive()` checks `UIStylingDefaults::get().scaling_mode`
- [x] All spacing methods return `pixels()` in Adaptive, `h720()` in Proportional
- [x] `TypographyScale` follows the same pattern
- [x] All existing screens unchanged (still Proportional by default)

### Phase 3: Convert SportsSettings as reference -- DONE
- [x] Set `context.scaling_mode = ScalingMode::Adaptive`
- [x] Replace all `h720()`/`w1280()` with `pixels()` for fonts and sizes
- [x] Use `LayoutInfo` for breakpoint decisions (show/hide help panel uses logical width)
- [x] E2E responsive audit passes at 720p and 1080p

### Phase 4: E2E visibility enforcement -- DONE
- [x] `VisibleTextRegistry::register_text_if_visible()` filters off-screen text
- [x] Both text rendering paths (immediate + batched) use viewport-aware registration
- [x] `expect_text` only matches text whose bounding rect is at least partially in viewport
- [x] `assert_no_overflow` text truncation awareness (existed from earlier work)

### Phase 5: Convert remaining layout-tree screens -- DONE
- [x] 42 Pattern B screens converted: added `context.scaling_mode = ScalingMode::Adaptive` and swapped all `h720()`/`w1280()` to `pixels()`
- [x] Converted screens include: all settings screens (DeadSpace, AngryBirds, Kirby, MiniMotorways, Flight, PowerWash, Casual, IslandsTrains, RubberBandits), all gallery/showcase screens (Forms, Buttons, Cards, Checkboxes, RadioGroups, Sliders, Tabs, Toggles, Dropdowns, Decorators, etc.), text editing demos (AIMChat, TextInput, Language), layout demos (VStack, HStack, SelfAlign, ScrollView, Modal, ExampleFlex, ExampleLayout), and utility screens (Themes, FileTree, AutoTextColor, NavigationBar, Pagination, CircularProgress, MetersGauges, Toast, DragDrop, HorizontalDrag, Stepper, SettingRow, Image, DecorativeFrame)
- [x] All 20 C++ tests pass, all 37 E2E scripts pass
- 28 screens remain on Proportional: all are heavily absolute-positioned game HUD mockups (EmpireTycoon, CozyCafe, NeonStrike, ShopInterface, etc.), animation demos, or low-level showcase demos (borders, text rendering, colors) where absolute positioning is the intended design

## Design Decisions (Resolved)

All decisions follow the web model as reference:

### 1. Absolute positioning auto-scales in Adaptive mode

On the web, `position: absolute; left: 100px` at 200% zoom renders at 200 physical pixels. CSS `px` always scales with zoom.

**Decision:** `with_absolute_position(50.0f, 12.0f)` in Adaptive mode resolves as `(50 * ui_scale, 12 * ui_scale)`. No new overload needed. The autolayout system multiplies absolute position values by `ui_scale` when mode is Adaptive.

In Proportional mode, absolute positions are unchanged (raw pixels, as today).

### 2. DefaultSpacing is mode-aware

On the web, `padding: 16px` scales with zoom. In Proportional mode, the current `h720()` behavior (scales with resolution) should be preserved.

**Decision:** DefaultSpacing checks the active scaling mode:

```cpp
struct DefaultSpacing {
  static Size tiny() {
    if (UIStylingDefaults::get().scaling_mode == ScalingMode::Adaptive)
      return pixels(8.0f);   // scales with ui_scale
    return h720(8.0f);       // scales with resolution (current behavior)
  }
  // ... same pattern for small, medium, large, xlarge, container
};
```

This preserves existing Proportional behavior while giving Adaptive screens web-like spacing.

TypographyScale follows the same pattern.

### 3. `screen_pct()` and `h720()` are NOT affected by ui_scale

On the web, `100vw` is always the viewport width regardless of zoom level. Viewport units are resolution-only.

**Decision:** `screen_pct()` and `h720()` continue resolving as `value * screen_dimension`. They are unaware of `ui_scale`. In Adaptive mode, screens should use `pixels()` instead of `h720()` for element sizes and fonts.

### 4. OS DPI is separate from ui_scale

On the web, `window.devicePixelRatio` is handled by the browser compositor, separate from the user's zoom preference.

**Decision:** `ui_scale` is purely the user's zoom preference (1.0 = 100%). OS DPI/Retina scaling is handled by the rendering backend (raylib/sokol), not by the layout system.

### 5. ui_scale is clamped to a safe range

**Decision:** The engine clamps `ui_scale` to `[0.5, 3.0]`. Apps can impose tighter limits if desired. Values outside this range are likely bugs or would produce unusable layouts.
