# Primitive Toggle Button Design

## Summary

Introduce an `imm::primitive` namespace for stateless UI building blocks. The first primitive is `toggle_button` — a button that flips a `bool&` on click with no internal state management. This replaces `checkbox_no_label` and simplifies `checkbox` by separating "click to flip a bool" (primitive) from "checkbox with state, label row, and visual indicators" (convenience).

## Motivation

The toggle family (`checkbox_no_label`, `checkbox`, `toggle_switch`, `radio_group`) all share one core behavior: a button that flips a boolean on click. Today each reimplements this with its own state component, click listener setup, and `changed_since` bookkeeping. This creates ~100 lines of duplicated logic and subtle divergences that can harbor bugs.

By extracting the shared behavior into a stateless primitive, the convenience components become thin wrappers that focus on what makes them unique (visuals, layout, state management) rather than reimplementing click-to-toggle.

## Architecture

### Two-layer design

| Layer | Namespace | Owns state? | Manages visuals? | Example |
|---|---|---|---|---|
| Primitive | `imm::primitive` | No | No (caller sets config) | `toggle_button` |
| Convenience | `imm` | Yes (ECS components) | Yes (labels, layout, indicators) | `checkbox` |

### Convention

All primitives follow the signature pattern: `(ctx, ep_pair, config, &mutable_state...)` — config before mutable pointers/refs.

## API

### `imm::primitive::toggle_button`

```cpp
namespace afterhours::ui::imm::primitive {

/// A button that flips a bool on click. Stateless — caller owns the bool.
/// Returns ElementResult where bool() == true when value changed this frame.
ElementResult toggle_button(HasUIContext auto &ctx, EntityParent ep_pair,
                            ComponentConfig config,
                            bool &value);

} // namespace afterhours::ui::imm::primitive
```

**What it does:**
- Calls `init_component` with `ComponentType::Button`, `enable_color=true`
- Adds `HasClickListener` that flips `value` and sets a local `changed` flag
- Returns `ElementResult{changed, entity, value}`

**What it does NOT do:**
- No `HasCheckboxState` or any ECS state component
- No label swapping — caller sets `config.label` before calling
- No font overrides — caller picks the font
- No visual children (no circle, no knob, no dot)

### Usage

```cpp
// Raw usage — full control, no state management
bool my_value = false;
if (primitive::toggle_button(ctx, mk(parent),
        ComponentConfig{}.with_label(my_value ? "X" : "O"),
        my_value)) {
    // value was flipped this frame
    play_sound(SFX::Click);
}
```

## Implementation

### `toggle_button` (~15 lines)

```cpp
ElementResult toggle_button(HasUIContext auto &ctx, EntityParent ep_pair,
                            ComponentConfig config,
                            bool &value) {
    auto [entity, parent] = deref(ep_pair);

    init_component(ctx, ep_pair, config, ComponentType::Button, true, "toggle_button");

    // Apply flex-direction for buttons (matches existing button() behavior)
    entity.get<UIComponent>().flex_direction = config.flex_direction;

    bool changed = false;
    entity.addComponentIfMissing<HasClickListener>([&value, &changed](Entity &) {
        value = !value;
        changed = true;
    });

    // Check if click happened this frame
    changed = entity.get<HasClickListener>().down;
    if (changed) {
        value = !value;
    }

    return ElementResult{changed, entity, value};
}
```

Note: The exact click detection mechanism should match how `button()` works today — via `HasClickListener::down` being set by the input system. The lambda on `HasClickListener` is for external callbacks; the actual toggle happens by reading `.down`.

### Simplified `checkbox` (was ~100 lines across `checkbox` + `checkbox_no_label`, now ~45 lines)

```cpp
ElementResult checkbox(HasUIContext auto &ctx, EntityParent ep_pair,
                       bool &value,
                       ComponentConfig config = ComponentConfig()) {
    auto [entity, parent] = deref(ep_pair);

    // --- State management (this is what makes it a convenience wrapper) ---
    HasCheckboxState &state =
        init_state<HasCheckboxState>(entity, [&](auto &) {}, value);

    // --- Label row setup ---
    auto label = config.label;
    config.label = "";
    config.with_flex_direction(FlexDirection::Row)
        .with_align_items(AlignItems::Center)
        .with_no_wrap();
    init_component(ctx, ep_pair, config, ComponentType::Div, false, "checkbox_row");
    entity.template addComponentIfMissing<FocusClusterRoot>();

    // Resolve sizing (styling defaults or fallback)
    if (config.size.is_default) {
        auto &styling_defaults = UIStylingDefaults::get();
        if (auto def = styling_defaults.get_component_config(ComponentType::Checkbox);
            def.has_value()) {
            config.size = def->size;
        } else {
            config.size = ComponentSize(pixels(default_component_size.x),
                                        children(default_component_size.y));
        }
    }

    // Label child (left side)
    if (!label.empty()) {
        config.size = config.size.scale_x(0.5f);
        div(ctx, mk(entity),
            ComponentConfig::inherit_from(config, "checkbox_label")
                .with_size(config.size)
                .with_label(label)
                .with_color_usage(Theme::Usage::Primary)
                .with_rounded_corners(RoundedCorners().right_sharp()))
            .ent().template addComponentIfMissing<InFocusCluster>();
    }

    // Toggle button (right side) — the primitive does the click-to-flip
    std::string checked = config.checkbox_checked_indicator
                              .value_or(ComponentConfig::DEFAULT_CHECKBOX_CHECKED);
    std::string unchecked = config.checkbox_unchecked_indicator
                                .value_or(ComponentConfig::DEFAULT_CHECKBOX_UNCHECKED);

    auto toggle_config =
        ComponentConfig::inherit_from(config, "checkbox_toggle")
            .with_size(config.size)
            .with_label(state.on ? checked : unchecked)
            .with_font(UIComponent::SYMBOL_FONT, pixels(20.f))
            .with_auto_text_color(true)
            .with_color_usage(Theme::Usage::Primary)
            .with_rounded_corners(RoundedCorners().left_sharp());

    auto result = primitive::toggle_button(ctx, mk(entity), toggle_config, state.on);
    result.ent().template addComponentIfMissing<InFocusCluster>();

    // Sync state back
    if (result) {
        state.changed_since = true;
    }
    value = state.on;

    ElementResult final_result{state.changed_since, entity, value};
    state.changed_since = false;
    return final_result;
}
```

### `checkbox_no_label` — removed

Replaced by `primitive::toggle_button`. Any caller that was using `checkbox_no_label` directly can switch to:

```cpp
// Before:
checkbox_no_label(ctx, mk(parent), value, config);

// After:
std::string label = value ? "X" : "O"; // or use DEFAULT_CHECKBOX_CHECKED/UNCHECKED
primitive::toggle_button(ctx, mk(parent),
    config.with_label(label).with_font(UIComponent::SYMBOL_FONT, pixels(20.f)),
    value);
```

Or use the stateful `checkbox()` if they want managed state.

## Migration

1. Add `imm::primitive` namespace with `toggle_button`
2. Rewrite `checkbox` to use `toggle_button` internally
3. Deprecate `checkbox_no_label` (keep as inline wrapper for one release, then remove)
4. Update `checkbox_group` to call the new `checkbox` (should work as-is since the signature is the same)

## Changes to existing components

| Component | Change |
|---|---|
| `checkbox_no_label` | Deprecated, replaced by `primitive::toggle_button` |
| `checkbox` | Simplified, delegates click behavior to `toggle_button` |
| `checkbox_group` | No API change, calls updated `checkbox` |

## Future work

- **`imm::stateful` namespace**: Consider moving all stateful convenience wrappers to a separate namespace for clearer layering.
- **Move existing primitives**: Consider moving `div`, `button`, `sprite`, `image` into `imm::primitive` (breaking change, needs migration path).
- **`arrow_nav` primitive**: Extract `[<] [content] [>]` pattern for `stepper`, `navigation_bar`, `pagination`.
- **`labeled_row` primitive**: Extract "label + control in a row" pattern for `checkbox`, `toggle_switch`, `slider`, `dropdown`.
- **Apply `toggle_button` to other toggle family members**: `toggle_switch` and `radio_group` can use `toggle_button` the same way `checkbox` does.
