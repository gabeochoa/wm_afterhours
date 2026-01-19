# Page Section Components

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Header

### Description

An element that appears across the top of screens, usually containing the title and main navigation.

### Use Cases in Game UI
- Main menu header
- In-game HUD top bar
- Settings screen header
- Pause menu header

### Suggested Implementation

```cpp
struct HeaderConfig {
    std::optional<TextureConfig> logo;
    bool sticky = false;  // Fixed at top when scrolling
    bool show_back_button = false;
    std::function<void()> on_back;
};

ElementResult header(HasUIContext auto &ctx, EntityParent ep_pair,
                     const std::string &title,
                     HeaderConfig config = HeaderConfig());

// Usage:
header(ctx, mk(parent, 0), "Settings",
       HeaderConfig{.show_back_button = true, .on_back = []{ go_back(); }});
```

### Features Needed
- Title display
- Optional logo/icon
- Back button
- Optional right-side actions
- Sticky positioning
- Consistent height across screens

---

## Footer

### Description

Appears at the bottom of a page or section, displaying secondary information, copyright, or navigation links.

### Use Cases in Game UI
- Copyright/version info
- Controller button hints
- Navigation shortcuts
- Status bar

### Suggested Implementation

```cpp
ElementResult footer(HasUIContext auto &ctx, EntityParent ep_pair,
                     ComponentConfig config = ComponentConfig());

// Controller button hints footer
struct ButtonHint {
    std::string button;  // "A", "B", "X", "Y", etc.
    std::string action;  // "Select", "Back", etc.
};

ElementResult button_hints_footer(HasUIContext auto &ctx, EntityParent ep_pair,
                                  const std::vector<ButtonHint> &hints,
                                  ComponentConfig config = ComponentConfig());

// Usage:
button_hints_footer(ctx, mk(parent, 0), {
    {"A", "Select"},
    {"B", "Back"},
    {"Y", "Options"}
});
```

---

## Hero / Jumbotron / Banner

### Description

A large banner, usually one of the first items on a page, often containing a full-width image, title, and call-to-action.

### Use Cases in Game UI
- Main menu hero image
- Event/update banners
- Featured content
- Welcome screens

### Suggested Implementation

```cpp
struct HeroConfig {
    std::optional<TextureConfig> background_image;
    std::optional<TextureConfig> overlay_gradient;
    float height = 0.4f;  // As screen percentage
    bool parallax = false;
};

ElementResult hero(HasUIContext auto &ctx, EntityParent ep_pair,
                   const std::string &title,
                   const std::string &subtitle,
                   HeroConfig config = HeroConfig());

// Usage:
hero(ctx, mk(parent, 0), 
     "Welcome Back, Player!",
     "Your adventure continues...",
     HeroConfig{.background_image = hero_bg, .parallax = true});
```

### Features Needed
- Full-width layout
- Background image
- Overlay gradient for text readability
- Title and subtitle
- Optional call-to-action button
- Parallax scrolling effect

---

## Heading

### Description

A title or caption used to introduce a new section, with appropriate visual hierarchy.

### Suggested Implementation

```cpp
enum class HeadingLevel { H1, H2, H3, H4, H5, H6 };

ElementResult heading(HasUIContext auto &ctx, EntityParent ep_pair,
                      const std::string &text,
                      HeadingLevel level = HeadingLevel::H1,
                      ComponentConfig config = ComponentConfig());

// Usage:
heading(ctx, mk(parent, 0), "Graphics Settings", HeadingLevel::H2);
```

### Features Needed
- Size hierarchy (H1 largest, H6 smallest)
- Semantic structure
- Consistent styling per level
- Optional icon

---

## Card

### Description

A container for content representing a single entity (contact, article, item, etc.).

**Also known as:** Tile

### Use Cases in Game UI
- Item cards
- Player profile cards
- Achievement cards
- Save game slots
- Shop items

### Suggested Implementation

```cpp
struct CardConfig {
    bool elevated = true;  // Shadow
    bool hoverable = true;  // Lift on hover
    bool clickable = true;
    float border_radius = 8.0f;
    std::optional<TextureConfig> header_image;
};

ElementResult card(HasUIContext auto &ctx, EntityParent ep_pair,
                   CardConfig config = CardConfig());

// Pre-built card sections
ElementResult card_header(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::string &title,
                          const std::string &subtitle = "");
ElementResult card_body(HasUIContext auto &ctx, EntityParent ep_pair);
ElementResult card_footer(HasUIContext auto &ctx, EntityParent ep_pair);

// Usage:
card(ctx, mk(parent, 0)) {
    if (item.image) {
        image(ctx, mk(current, 0), item.image, {fill_parent(), pixels(120)});
    }
    card_header(ctx, mk(current, 1), item.name, item.category);
    card_body(ctx, mk(current, 2)) {
        text(ctx, mk(current, 0), item.description);
    }
    card_footer(ctx, mk(current, 3)) {
        price_display(ctx, mk(current, 0), item.price);
        button(ctx, mk(current, 1), "Buy");
    }
}
```

### Features Needed
- Elevation/shadow
- Hover state
- Click handling
- Header image slot
- Header/body/footer sections
- Border radius
- Content padding

