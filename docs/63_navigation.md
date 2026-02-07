# Navigation Components — Remaining Work

**Status:** Partially implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

> `navigation_bar()` and `pagination()` are implemented in afterhours.  
> The items below (Breadcrumbs, Navigation/Menu, Link) are NOT yet implemented.

---

## Breadcrumbs

A list of links showing the location of the current page in the navigational hierarchy.

### Use Cases in Game UI
- Settings > Audio > Music Volume
- Inventory > Weapons > Swords
- Shop > Categories > Armor

### Suggested Implementation

```cpp
struct BreadcrumbItem {
    std::string label;
    std::function<void()> on_click;
    bool is_current = false;
};

struct BreadcrumbConfig {
    std::string separator = ">";
    bool show_home_icon = false;
    int max_items = 0;  // 0 = show all, otherwise collapse middle items
};

ElementResult breadcrumbs(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::vector<BreadcrumbItem> &items,
                          BreadcrumbConfig config = BreadcrumbConfig());
```

### Features Needed
- Clickable links for navigation
- Separator between items
- Current page styling (non-clickable, bold)
- Optional home icon
- Collapsing for long paths (... in middle)

---

## Navigation / Menu

A container for navigation links to other pages or elements within the current page.

### Use Cases in Game UI
- Main menu navigation
- In-game pause menu
- Settings categories sidebar

### Suggested Implementation

```cpp
struct NavItem {
    std::string label;
    std::optional<TextureConfig> icon;
    std::function<void()> on_select;
    bool is_active = false;
    bool is_disabled = false;
    std::optional<int> badge_count;
};

enum class NavOrientation { Horizontal, Vertical };

ElementResult navigation(HasUIContext auto &ctx, EntityParent ep_pair,
                         const std::vector<NavItem> &items,
                         NavConfig config = NavConfig());
```

### Features Needed
- Horizontal and vertical layouts
- Active state highlighting
- Icon + label or icon-only modes
- Badge/notification count
- Keyboard navigation
- Hover states

---

## Link

A clickable text element that navigates to another location or triggers an action.

### Use Cases in Game UI
- "Forgot password?" links
- "Learn more" links
- Terms of service links
- External URLs (opens browser)

### Suggested Implementation

```cpp
struct LinkConfig {
    bool underline = true;
    bool external = false;  // Opens in browser
    Color color = Theme::Usage::Accent;
};

ElementResult link(HasUIContext auto &ctx, EntityParent ep_pair,
                   const std::string &text,
                   std::function<void()> on_click,
                   LinkConfig config = LinkConfig());
```

### Features Needed
- Underline styling
- Hover state (color change)
- Visited state (optional)
- External link indicator icon
- Focus state for keyboard navigation
