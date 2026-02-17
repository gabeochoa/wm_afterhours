# Config Presets: User-Defined Design Systems

## Problem

Every screen hand-rolls `ComponentConfig` chains. Buttons on one screen have different sizes, fonts, margins, and roundness than buttons on another. There's no way for a library user to say "every button in my app looks like this" and have it enforced.

## Solution

Free functions (`UpperCamelCase`) that return pre-filled `ComponentConfig` objects. The library user defines these once in a header; junior devs use them at call sites without touching `ComponentConfig` directly. Chaining still works for per-instance overrides.

## Design

### Convention

- Free functions returning `ComponentConfig`
- `UpperCamelCase` naming (reads as "what you're making")
- No suffix — `Button()`, not `ButtonConfig()`
- Use `Theme::Usage` enums so presets work across themes without hardcoded values
- Use parameters for orthogonal axes (e.g. variant), not separate functions

### User-defined design system (one header per app)

```cpp
// my_game_ui.h

inline ComponentConfig Button(ButtonVariant variant = ButtonVariant::Filled) {
    return ComponentConfig{}
        .with_size(ComponentSize{pixels(180), pixels(48)})
        .with_background(Theme::Usage::Primary)
        .with_auto_text_color(true)
        .with_button_variant(variant)
        .with_font(UIComponent::DEFAULT_FONT, pixels(20.0f))
        .with_margin(Spacing::xs);
}

inline ComponentConfig SmallButton(ButtonVariant variant = ButtonVariant::Filled) {
    return Button(variant)
        .with_size(ComponentSize{pixels(100), pixels(36)})
        .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f));
}

inline ComponentConfig LargeButton(ButtonVariant variant = ButtonVariant::Filled) {
    return Button(variant)
        .with_size(ComponentSize{pixels(200), pixels(56)})
        .with_font(UIComponent::DEFAULT_FONT, pixels(24.0f));
}

inline ComponentConfig PageRoot() {
    return ComponentConfig{}
        .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.95f)})
        .with_self_align(SelfAlign::Center)
        .with_background(Theme::Usage::Background)
        .with_padding(Spacing::md)
        .with_no_wrap();
}

inline ComponentConfig Row() {
    return ComponentConfig{}
        .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
        .with_background(Theme::Usage::Surface)
        .with_padding(Spacing::xs)
        .with_align_items(AlignItems::Center);
}

inline ComponentConfig PageTitle() {
    return ComponentConfig{}
        .with_size(ComponentSize{percent(1.0f), pixels(60)})
        .with_background(Theme::Usage::Surface)
        .with_auto_text_color(true)
        .with_font(UIComponent::DEFAULT_FONT, pixels(30.0f))
        .with_skip_tabbing(true);
}

inline ComponentConfig SectionLabel() {
    return ComponentConfig{}
        .with_size(ComponentSize{pixels(120), percent(0.9f)})
        .with_background(Theme::Usage::Surface)
        .with_skip_tabbing(true)
        .with_font(UIComponent::DEFAULT_FONT, pixels(20.0f));
}

inline ComponentConfig BodyText() {
    return ComponentConfig{}
        .with_background(Theme::Usage::None)
        .with_auto_text_color(true)
        .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
        .with_skip_tabbing(true);
}
```

### Call site (what junior devs write)

```cpp
ctx.theme = MyGameTheme();

auto root = vstack(ctx, mk(entity, 0), PageRoot());
div(ctx, mk(root.ent(), 0), PageTitle().with_label("Settings"));

auto row = hstack(ctx, mk(root.ent(), 1), Row());
if (button(ctx, mk(row.ent(), 0), Button().with_label("Save"))) {
    save();
}
if (button(ctx, mk(row.ent(), 1), Button(ButtonVariant::Outline).with_label("Cancel"))) {
    go_back();
}
```

### Composing presets

Presets compose by calling one and chaining overrides:

```cpp
inline ComponentConfig DangerButton(ButtonVariant variant = ButtonVariant::Filled) {
    return Button(variant).with_background(Theme::Usage::Error);
}

// Size presets compose from the base Button:
inline ComponentConfig SmallButton(ButtonVariant variant = ButtonVariant::Filled) {
    return Button(variant)
        .with_size(ComponentSize{pixels(100), pixels(36)})
        .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f));
}
```

### Per-instance overrides

Any `.with_*` call after the preset overrides that property:

```cpp
button(ctx, mk(row, 1), Button()
    .with_label("Wide Save")
    .with_size(ComponentSize{pixels(300), pixels(48)}));

button(ctx, mk(row, 2), Button()
    .with_label("Accent")
    .with_background(Theme::Usage::Accent));
```

## What the library ships

The library does NOT ship opinionated presets like `Button()`. That's the user's design system. The library ships:

1. `ComponentConfig` with the builder pattern (already exists)
2. `Theme::Usage` enums that resolve at render time (already exists)
3. `FontSizing::Tier` for theme-relative font sizes (already exists)
4. Documentation and examples showing the preset pattern

## Key properties

- **Zero new API surface** — just a convention using existing `ComponentConfig`
- **Theme-dynamic** — presets use `Theme::Usage` enums, work across any theme
- **Composable** — build presets from other presets via chaining
- **Overridable** — any property can be overridden at the call site
- **Parameterized** — use function params for orthogonal axes (variant, size)
- **No registration** — no global registry, no macros, just functions
