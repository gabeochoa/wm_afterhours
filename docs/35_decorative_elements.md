# Decorative Elements — Remaining Work

**Status:** Partial  
**Priority:** Low

> `decorative_frame()` with KraftPaper/Simple/Inset styles is implemented in `imm_components.h`.

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

## Quote / Blockquote

A styled text element for quotations or highlighted passages. Renders with a left accent bar, optional quote marks, and optional attribution.

### Use Cases in Game UI
- NPC dialogue highlights
- Lore text
- Tips/hints display
- Achievement descriptions

### Suggested Implementation

```cpp
struct QuoteConfig {
    std::optional<std::string> attribution;
    bool show_quote_marks = true;
    Color accent_color = Theme::Usage::Accent;
};

ElementResult quote(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::string &text,
                    QuoteConfig config = QuoteConfig());
```

### Features Needed
- Left accent bar (colored vertical strip)
- Italic text styling
- Optional attribution line below (e.g. "— Elder Sage")
- Optional decorative quote marks

---

## Remaining Workarounds

| File | Gap | Description |
|------|-----|-------------|
| `src/ui_workarounds/GradientBackground.h` | Gradients | Layered strips |
| `src/ui_workarounds/NotificationBadge.h` | Badges | Positioned circles with text |
