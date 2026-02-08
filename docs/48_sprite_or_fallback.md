# Feature Request: sprite_or_fallback() Helper

**Type:** Project-level helper (no afterhours changes needed)
**File:** `src/ui_workarounds/SpriteOrFallback.h` (new)
**Impact:** ~60 lines saved across 4+ screens

## Problem

Multiple screens (EmpireTycoon, CozyCafe, NeonStrike, KartSelect) repeat this pattern:

```cpp
if (tex_ptr && tex_ptr->id != 0) {
    afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width, (float)tex_ptr->height};
    sprite(context, mk(entity, id), *tex_ptr, src,
           ComponentConfig{}.with_size(...).with_absolute_position(x, y));
} else {
    div(context, mk(entity, id),
        ComponentConfig{}.with_label(fallback)
            .with_size(...).with_absolute_position(x, y)
            .with_font(...).with_custom_text_color(...)
            .with_alignment(TextAlignment::Center));
}
```

The if/else is 10-15 lines every time, and the sprite source rect calculation is always the same (full texture).

## Proposed Solution

```cpp
// src/ui_workarounds/SpriteOrFallback.h
#pragma once
#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Renders a sprite if texture is loaded, otherwise a text fallback div.
// sprite_config is used for both paths (fallback adds label/font/color on top).
template <typename Context>
inline void sprite_or_fallback(
    Context &ctx, afterhours::EntityPair ep,
    raylib::Texture2D &tex,
    const std::string &fallback_label,
    ComponentConfig config,
    // Fallback-only styling (ignored if texture loads):
    const std::string &font = "EqProRounded",
    float font_size = 24.0f,
    afterhours::Color text_color = afterhours::Color{55, 75, 105, 255}) {
  if (tex.id != 0) {
    afterhours::texture_manager::Rectangle src{
        0, 0, (float)tex.width, (float)tex.height};
    sprite(ctx, ep, tex, src, config);
  } else {
    div(ctx, ep,
        config.with_label(fallback_label)
            .with_font(font, font_size)
            .with_custom_text_color(text_color)
            .with_alignment(TextAlignment::Center));
  }
}

} // namespace ui_workarounds
```

## Usage

```cpp
// Before (10-15 lines):
if (tex_ptr && tex_ptr->id != 0) {
    Rectangle src{0, 0, (float)tex_ptr->width, (float)tex_ptr->height};
    sprite(context, mk(entity, 110 + i), *tex_ptr, src,
        ComponentConfig{}.with_size(...).with_absolute_position(x, y));
} else {
    div(context, mk(entity, 110 + i),
        ComponentConfig{}.with_label(fallback).with_size(...)
            .with_absolute_position(x, y).with_font(...).with_alignment(...));
}

// After (2 lines):
ui_workarounds::sprite_or_fallback(
    context, mk(entity, 110 + i), *tex_ptr, fallback,
    ComponentConfig{}.with_size(...).with_absolute_position(x, y));
```

## Screens that would benefit

- EmpireTycoon (tabs, bottom icons, coin icon)
- CozyCafe (stars, avatars, icon buttons)
- NeonStrike (killstreak icons, equipment)
- KartSelect (character icons)
