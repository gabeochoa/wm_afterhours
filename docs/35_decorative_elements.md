# Decorative Elements

**Status:** Partial (decorative_frame implemented)  
**Priority:** Low

---

## Decorative Frame ✅ Implemented

Creates layered decorative borders/frames around content with multiple style variants.

### Usage

```cpp
// Kraft-paper style frame (default) - layered borders with corner accents
auto frame = decorative_frame(ctx, mk(parent),
    ComponentConfig{}
        .with_size(pixels(400), pixels(300))
        .with_custom_background(kraft_tan)
        .with_border(frame_brown, 12.0f),
    DecorativeFrameStyle::KraftPaper);

// Simple frame - single border with background
decorative_frame(ctx, mk(parent),
    ComponentConfig{}
        .with_size(percent(1.0f), percent(1.0f))
        .with_background(Theme::Usage::Surface)
        .with_border(theme.accent, 4.0f),
    DecorativeFrameStyle::Simple);

// Inset/sunken frame - shadow/highlight edges
decorative_frame(ctx, mk(parent),
    ComponentConfig{}
        .with_size(pixels(300), pixels(200)),
    DecorativeFrameStyle::Inset);
```

### Style Variants

| Style | Description |
|-------|-------------|
| `KraftPaper` | Layered borders with corner accents (scrapbook/craft feel) |
| `Simple` | Single border with background |
| `Inset` | Sunken effect with shadow/highlight edges |

---

## Corner Bracket Decorations

### Problem

Tech/sci-fi UIs commonly use corner bracket decorations on panels. DeadSpace manually creates 8 divs for this effect.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_corner_brackets(bracket_color, bracket_size, bracket_thickness);

// Or as a helper function
void corner_brackets(HasUIContext auto &ctx, Entity &parent, int base_id,
                     float x, float y, float w, float h,
                     Color color, float size = 15.0f, float thickness = 2.0f);
```

---

## Grid/Decorative Background Pattern

### Problem

MiniMotorways manually draws grid lines for background effect.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_grid_background(cell_size, line_color, line_thickness);
```

---

## Remaining Workarounds

| File | Gap | Description |
|------|-----|-------------|
| `src/ui_workarounds/GradientBackground.h` | Gradients | Layered strips |
| `src/ui_workarounds/NotificationBadge.h` | Badges | Positioned circles with text |
