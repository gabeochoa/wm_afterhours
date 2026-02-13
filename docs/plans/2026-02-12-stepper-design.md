# Stepper Component Design

## Summary

Add a standalone `stepper()` primitive to `imm_components.h` that renders `[ < ] [ value ] [ > ]` — two arrow buttons flanking a value display. String variant only (cycles through a `vector<string>`). Replaces 5+ manual reimplementations across settings screens and the inline stepper code in `setting_row.h`.

## API

```cpp
// In afterhours::ui::imm namespace, inside imm_components.h

template <typename Container>
ElementResult stepper(
    HasUIContext auto &ctx,
    EntityParent ep_pair,
    const Container &options,       // String options to cycle through
    size_t &option_index,           // Current selection index (mutated on click)
    ComponentConfig config = ComponentConfig()
);
```

### Usage

```cpp
size_t quality_idx = 2;
std::vector<std::string> quality_options = {"Low", "Medium", "High", "Ultra"};

if (stepper(ctx, mk(parent), quality_options, quality_idx)) {
    // Value changed
}

// With custom sizing/styling
stepper(ctx, mk(parent), quality_options, quality_idx,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(200), pixels(44)})
            .with_font("EqProRounded", 18.0f));
```

## Internal Structure

**Container** (Div, flex Row, AlignItems::Center, JustifyContent::SpaceBetween):
- **Left arrow** — `button()` with label `"<"`, transparent bg, muted text. Decrements index (wraps).
- **Value display** — `div()` with current option string, centered text.
- **Right arrow** — `button()` with label `">"`, same styling. Increments index (wraps).

Uses `detail::prev_index()` and `detail::next_index()` which already exist in `imm_components.h`.

### Keyboard

`HasLeftRightListener` on the container entity — left/right arrow keys cycle the value when focused. Same pattern as `slider()`.

### Focus

`FocusClusterRoot` on the container. Arrow buttons and value label get `InFocusCluster` so the stepper is one tab stop. Same pattern as `checkbox()`.

### State

No custom state component. Mutation is done directly on `option_index`. Changed status tracked by comparing before/after.

### Sizing

Default size: `children()` width (fits content), `children()` height. Can be overridden via config. Arrow buttons get fixed width from styling defaults or a reasonable default (24px). Value display fills remaining space.

### ComponentType

Add `Stepper` to the `ComponentType` enum in `styling_defaults.h`.

## setting_row.h Integration

Replace lines 295-352 (the `SettingRowControlType::Stepper` case) with:

```cpp
case SettingRowControlType::Stepper: {
    if constexpr (std::is_same_v<ValueT, std::pair<size_t *, std::vector<std::string>>>) {
        auto stepper_cfg = ComponentConfig{}
            .with_size(ComponentSize{
                pixels((int)(row_config.stepper_arrow_width * 2 + row_config.stepper_value_width)),
                pixels((int)row_h - 8)})
            .with_font(config.font_name, config.font_size)
            .with_debug_name("setting_row_stepper");

        if (row_config.slot_control_config)
            stepper_cfg = stepper_cfg.apply_overrides(*row_config.slot_control_config);

        if (stepper(ctx, mk(entity), value.second, *value.first, stepper_cfg))
            changed = true;
    }
    break;
}
```

## Files Changed

1. `vendor/afterhours/src/plugins/ui/styling_defaults.h` — add `Stepper` to `ComponentType` enum
2. `vendor/afterhours/src/plugins/ui/imm_components.h` — add `stepper()` function before closing namespace braces
3. `vendor/afterhours/src/plugins/ui/setting_row.h` — replace inline stepper with call to `stepper()`
