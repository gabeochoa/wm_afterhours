# Overlay Panels

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

---

## Drawer / Flyout / Sheet

### Description

A panel which slides out from the edge of the screen, typically used for secondary navigation or detail views.

**Also known as:** Tray, Flyout, Sheet, Side panel

### Use Cases in Game UI
- Inventory sidebar
- Character equipment panel
- Quest details panel
- Settings panel (slides from right)
- Player stats panel
- Chat panel

### Suggested Implementation

```cpp
enum class DrawerEdge { Left, Right, Top, Bottom };

struct DrawerConfig {
    DrawerEdge edge = DrawerEdge::Right;
    float width_or_height = 0.3f;  // As screen percentage, or pixels if > 1
    bool close_on_outside_click = true;
    bool show_backdrop = true;
    float backdrop_opacity = 0.5f;
    bool animate = true;
    float animation_duration = 0.25f;
};

bool begin_drawer(HasUIContext auto &ctx, EntityParent ep_pair,
                  bool &is_open,
                  const std::string &title,
                  DrawerConfig config = DrawerConfig());
void end_drawer();

// Or immediate-mode style:
ElementResult drawer(HasUIContext auto &ctx, EntityParent ep_pair,
                     bool &is_open,
                     const std::string &title,
                     DrawerConfig config = DrawerConfig());

// Usage:
if (begin_drawer(ctx, mk(parent, 0), inventory_open, "Inventory",
                 DrawerConfig{.edge = DrawerEdge::Left})) {
    // Drawer content
    for (auto& item : items) {
        inventory_slot(ctx, mk(current, item.id), item);
    }
    end_drawer();
}
```

### Features Needed
- Slide in/out animation from edge
- Optional backdrop overlay
- Close button in header
- Close on outside click
- Close on Escape key
- Swipe to close (for touch)
- Scrollable content area

---

## Popover

### Description

An element that pops up from another element, floating above other content. Unlike tooltips, popovers are typically triggered via click and can contain interactive elements.

### Use Cases in Game UI
- Item info popup (on click)
- Quick actions menu
- Emoji picker
- Color palette popup
- Share menu
- User profile preview

### Suggested Implementation

```cpp
enum class PopoverPlacement {
    Top, TopStart, TopEnd,
    Bottom, BottomStart, BottomEnd,
    Left, LeftStart, LeftEnd,
    Right, RightStart, RightEnd,
    Auto  // Automatically choose best placement
};

struct PopoverConfig {
    PopoverPlacement placement = PopoverPlacement::Auto;
    bool close_on_outside_click = true;
    bool show_arrow = true;
    float offset = 8.0f;  // Distance from trigger
};

bool begin_popover(HasUIContext auto &ctx, EntityParent ep_pair,
                   Entity trigger_element,
                   bool &is_open,
                   PopoverConfig config = PopoverConfig());
void end_popover();

// Usage:
if (button(ctx, mk(parent, 0), ComponentConfig{}.with_label("⋮")).clicked) {
    more_menu_open = !more_menu_open;
}

if (begin_popover(ctx, mk(parent, 1), mk(parent, 0).entity, more_menu_open)) {
    if (button(ctx, mk(current, 0), "Edit").clicked) { /* ... */ }
    if (button(ctx, mk(current, 1), "Delete").clicked) { /* ... */ }
    if (button(ctx, mk(current, 2), "Share").clicked) { /* ... */ }
    end_popover();
}
```

### Features Needed
- Positioning relative to trigger element
- Auto-flip when near screen edge
- Optional arrow/caret pointing to trigger
- Close on outside click
- Close on Escape
- Keyboard navigation within popover
- Focus trap

---

## Dropdown Menu

### Description

A menu that appears below a button, showing actions or navigation options.

**Also known as:** Select menu, Action menu

### Note

Related to `24_context_menu.md` but triggered differently (click vs right-click).

### Suggested Implementation

```cpp
struct MenuItem {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<std::string> shortcut;  // "Ctrl+C"
    std::function<void()> on_select;
    bool disabled = false;
    bool is_separator = false;
    std::vector<MenuItem> submenu;  // For nested menus
};

ElementResult dropdown_menu(HasUIContext auto &ctx, EntityParent ep_pair,
                            Entity trigger_button,
                            const std::vector<MenuItem> &items,
                            bool &is_open);

// Usage:
if (button(ctx, mk(parent, 0), ComponentConfig{}.with_label("File")).clicked) {
    file_menu_open = true;
}

dropdown_menu(ctx, mk(parent, 1), mk(parent, 0).entity, {
    {"New", new_icon, "Ctrl+N", []{ new_file(); }},
    {"Open", open_icon, "Ctrl+O", []{ open_file(); }},
    {"Save", save_icon, "Ctrl+S", []{ save_file(); }},
    {.is_separator = true},
    {"Exit", std::nullopt, std::nullopt, []{ exit(); }}
}, file_menu_open);
```

### Features Needed
- All popover features
- Menu item hover highlighting
- Keyboard navigation (arrows, enter)
- Shortcut key display
- Dividers/separators
- Nested submenus
- Icons
- Disabled items

