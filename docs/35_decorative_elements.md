# Decorative Elements

**Status:** Not implemented  
**Priority:** Low

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

## Existing Workarounds

| File | Gap | Description |
|------|-----|-------------|
| `src/ui_workarounds/GradientBackground.h` | Gradients | Layered strips |
| `src/ui_workarounds/NotificationBadge.h` | Badges | Positioned circles with text |
| `src/ui_workarounds/DecorativeFrame.h` | Frames | Layered borders with corners |

