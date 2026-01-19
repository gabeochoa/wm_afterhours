# Navigation Components

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

---

## Breadcrumbs

### Description

A list of links showing the location of the current page in the navigational hierarchy.

**Also known as:** Breadcrumb trail

### Use Cases in Game UI
- Settings > Audio > Music Volume
- Inventory > Weapons > Swords
- Shop > Categories > Armor
- Profile > Stats > Combat

### Suggested Implementation

```cpp
struct BreadcrumbItem {
    std::string label;
    std::function<void()> on_click;
    bool is_current = false;
};

struct BreadcrumbConfig {
    std::string separator = ">";  // Or "/" or "»"
    bool show_home_icon = false;
    int max_items = 0;  // 0 = show all, otherwise collapse middle items
};

ElementResult breadcrumbs(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::vector<BreadcrumbItem> &items,
                          BreadcrumbConfig config = BreadcrumbConfig());

// Usage:
breadcrumbs(ctx, mk(parent, 0), {
    {"Home", []{ goto_home(); }},
    {"Settings", []{ goto_settings(); }},
    {"Audio", nullptr, true}  // Current page, no click
});
```

### Features Needed
- Clickable links for navigation
- Separator between items
- Current page styling (non-clickable, bold)
- Optional home icon
- Collapsing for long paths (... in middle)

---

## Navigation / Menu

### Description

A container for navigation links; these can be to other pages or to elements within the current page.

**Also known as:** Nav, Menu

### Use Cases in Game UI
- Main menu navigation
- In-game pause menu
- Settings categories sidebar
- Tab navigation within screens

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

struct NavConfig {
    NavOrientation orientation = NavOrientation::Horizontal;
    bool show_icons = true;
    bool compact = false;
};

ElementResult navigation(HasUIContext auto &ctx, EntityParent ep_pair,
                         const std::vector<NavItem> &items,
                         NavConfig config = NavConfig());

// Usage:
navigation(ctx, mk(parent, 0), {
    {"Play", play_icon, []{ start_game(); }, true},
    {"Settings", gear_icon, []{ open_settings(); }},
    {"Achievements", trophy_icon, []{ open_achievements(); }, false, false, 3},
    {"Quit", exit_icon, []{ quit_game(); }}
}, NavConfig{.orientation = NavOrientation::Vertical});
```

### Features Needed
- Horizontal and vertical layouts
- Active state highlighting
- Icon + label or icon-only modes
- Badge/notification count
- Keyboard navigation
- Hover states

---

## Pagination

### Description

Interface component for navigating between pages when content is split across multiple pages.

### Use Cases in Game UI
- Leaderboard pages
- Inventory pages
- Shop catalog pages
- Achievement lists
- Save file browser

### Suggested Implementation

```cpp
struct PaginationConfig {
    int total_pages;
    int current_page;
    int visible_pages = 5;  // How many page numbers to show
    bool show_first_last = true;  // << >> buttons
    bool show_prev_next = true;   // < > buttons
};

ElementResult pagination(HasUIContext auto &ctx, EntityParent ep_pair,
                         PaginationConfig &config,
                         std::function<void(int)> on_page_change,
                         ComponentConfig style = ComponentConfig());

// Usage:
pagination(ctx, mk(parent, 0), 
           PaginationConfig{.total_pages = 10, .current_page = 3},
           [](int page) { load_page(page); });
```

### Features Needed
- First/last page buttons
- Previous/next buttons
- Page number buttons
- Current page highlight
- Ellipsis for many pages (1 ... 5 6 7 ... 20)
- Keyboard navigation

---

## Link

### Description

A clickable text element that navigates to another location or triggers an action.

**Also known as:** Anchor, Hyperlink

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

// Usage:
link(ctx, mk(parent, 0), "Terms of Service", []{ open_tos(); });
link(ctx, mk(parent, 1), "Visit our website", []{ open_url("https://..."); },
     LinkConfig{.external = true});
```

### Features Needed
- Underline styling
- Hover state (color change)
- Visited state (optional)
- External link indicator icon
- Focus state for keyboard navigation

