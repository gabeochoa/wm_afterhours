# Miscellaneous Widgets

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Rating

### Description

Lets users see and/or set a star rating for a product or other item.

### Use Cases in Game UI
- Player ratings for user-created content
- Item quality display
- Difficulty rating
- Review system

### Suggested Implementation

```cpp
struct RatingConfig {
    int max_stars = 5;
    bool half_stars = false;  // Allow 0.5 increments
    bool read_only = false;
    TextureConfig star_filled;
    TextureConfig star_empty;
    TextureConfig star_half;  // For half stars
    float size = 24.0f;
};

ElementResult rating(HasUIContext auto &ctx, EntityParent ep_pair,
                     float &value,  // 0 to max_stars
                     RatingConfig config = RatingConfig());

// Read-only display
ElementResult rating_display(HasUIContext auto &ctx, EntityParent ep_pair,
                             float value,
                             RatingConfig config = RatingConfig());

// Usage:
rating(ctx, mk(parent, 0), user_rating, RatingConfig{.half_stars = true});
rating_display(ctx, mk(parent, 1), average_rating);
```

### Features Needed
- Click/hover to set rating
- Half-star support
- Custom icons (stars, hearts, etc.)
- Read-only mode for display
- Size variants
- Animation on change

---

## Segmented Control

### Description

A hybrid between button group, radio buttons, and tabs. Used to switch between different options or views.

**Also known as:** Toggle button group

### Use Cases in Game UI
- View mode toggle (Grid/List)
- Time period selection (Day/Week/Month)
- Sort direction (Asc/Desc)
- Filter toggles

### Suggested Implementation

```cpp
template <typename T>
struct SegmentOption {
    T value;
    std::string label;
    std::optional<TextureConfig> icon;
    bool disabled = false;
};

template <typename T>
ElementResult segmented_control(HasUIContext auto &ctx, EntityParent ep_pair,
                                const std::vector<SegmentOption<T>> &options,
                                T &selected_value,
                                ComponentConfig config = ComponentConfig());

// Usage:
segmented_control(ctx, mk(parent, 0), {
    {ViewMode::Grid, "Grid", grid_icon},
    {ViewMode::List, "List", list_icon}
}, current_view_mode);
```

### Features Needed
- Connected button appearance
- Active segment highlight
- Slide/animate between segments
- Icon-only mode
- Disabled segments
- Keyboard navigation

---

## Search Input

### Description

A text input optimized for search functionality.

**Also known as:** Search

### Use Cases in Game UI
- Inventory search
- Player search
- Item database search
- Console command search
- Server browser filter

### Suggested Implementation

```cpp
struct SearchConfig {
    std::string placeholder = "Search...";
    bool show_clear_button = true;
    bool auto_focus = false;
    int debounce_ms = 300;  // Delay before triggering search
};

ElementResult search_input(HasUIContext auto &ctx, EntityParent ep_pair,
                           std::string &query,
                           std::function<void(const std::string&)> on_search,
                           SearchConfig config = SearchConfig());

// Usage:
search_input(ctx, mk(parent, 0), search_query,
             [](const std::string& q) { filter_items(q); });
```

### Features Needed
- Search icon
- Clear button (X)
- Debounced input (wait for typing to stop)
- Loading indicator while searching
- Keyboard shortcut to focus (Ctrl+F)

---

## Separator / Divider

### Description

A visual separator between elements, usually a horizontal line.

**Also known as:** Divider, Horizontal rule, Vertical rule

### Suggested Implementation

```cpp
enum class SeparatorOrientation { Horizontal, Vertical };

struct SeparatorConfig {
    SeparatorOrientation orientation = SeparatorOrientation::Horizontal;
    float thickness = 1.0f;
    Color color = Theme::Usage::Border;
    std::optional<std::string> label;  // Text in the middle
};

ElementResult separator(HasUIContext auto &ctx, EntityParent ep_pair,
                        SeparatorConfig config = SeparatorConfig());

// Usage:
separator(ctx, mk(parent, 0));
separator(ctx, mk(parent, 1), SeparatorConfig{.label = "OR"});
```

---

## Stack

### Description

A wrapper component for adding consistent spacing between child components.

### Note

This may be handled by the existing layout system with gap property.

### Suggested Implementation

```cpp
enum class StackDirection { Vertical, Horizontal };

struct StackConfig {
    StackDirection direction = StackDirection::Vertical;
    float gap = DefaultSpacing::medium();
    bool wrap = false;  // Wrap to next line/column
};

ElementResult stack(HasUIContext auto &ctx, EntityParent ep_pair,
                    StackConfig config = StackConfig());

// Usage - may just be:
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_direction(Direction::Column)
        .with_gap(DefaultSpacing::medium())
);
```

---

## Quote / Blockquote

### Description

Display for quotations or highlighted text passages.

### Use Cases in Game UI
- NPC dialogue highlights
- Lore text
- Tips/hints display
- Achievement descriptions

### Suggested Implementation

```cpp
struct QuoteConfig {
    std::optional<std::string> attribution;  // Author/source
    bool show_quote_marks = true;
    Color accent_color = Theme::Usage::Accent;
};

ElementResult quote(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::string &text,
                    QuoteConfig config = QuoteConfig());

// Usage:
quote(ctx, mk(parent, 0), 
      "The ancient texts speak of a hero who will rise...",
      QuoteConfig{.attribution = "Elder Scrolls"});
```

