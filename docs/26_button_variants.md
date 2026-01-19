# Button Variants

**Status:** Partially implemented  
**Priority:** Low

> ✅ `button()`, `button_group()`, and `image_button()` exist in afterhours.  
> ❌ Button variants (Outline, Ghost) and icon+text buttons not yet implemented.

## Problem

Only a single `button()` function exists. Game UIs need multiple button styles.

## Suggested Implementation

```cpp
enum struct ButtonVariant { Filled, Outline, Ghost, Icon };

ElementResult button(HasUIContext auto &ctx, EntityParent ep_pair,
                     ComponentConfig config = ComponentConfig(),
                     ButtonVariant variant = ButtonVariant::Filled);

// Or via config:
ComponentConfig{}.with_button_variant(ButtonVariant::Outline);
```

---

# Icon + Text Button

**Status:** Not implemented  
**Priority:** Medium

## Problem

No built-in way to create buttons with both an icon and label.

## Suggested Implementation

```cpp
ElementResult icon_button(HasUIContext auto &ctx, EntityParent ep_pair,
                          texture_manager::Texture icon,
                          texture_manager::Rectangle icon_rect,
                          ComponentConfig config = ComponentConfig());

// Or via config
ComponentConfig{}
    .with_label("Settings")
    .with_icon(gear_texture, gear_rect)
    .with_icon_position(IconPosition::Left);  // Left, Right, Top, Bottom
```

