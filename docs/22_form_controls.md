# Form Controls — Remaining Work

**Status:** Mostly implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

> **Already in afterhours:** `slider`, `checkbox`, `checkbox_group`, `radio_group`, `dropdown`, `toggle_switch`  
> **Not yet implemented:** Combobox (search/filter), Fieldset, Form Label, Rating, Segmented Control

---

## Combobox / Autocomplete

An input that behaves like a select with the addition of a free text input to filter options.

### Use Cases in Game UI
- Player search (find friends by name)
- Item search in large inventories
- Server browser with filter
- Console command autocomplete

### Suggested Implementation

```cpp
template <typename T>
struct SelectOption {
    T value;
    std::string label;
    std::optional<TextureConfig> icon;
    bool disabled = false;
};

template <typename T>
ElementResult combobox(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::vector<SelectOption<T>> &options,
                       T &selected_value,
                       std::string &search_text,
                       ComponentConfig config = ComponentConfig());
```

### Features Needed
- Text input for filtering
- Filtered dropdown list
- Highlight matching text
- Clear button
- Empty state when no matches

---

## Fieldset

A wrapper for grouping related form fields with an optional legend/title.

### Suggested Implementation

```cpp
ElementResult fieldset(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::string &legend,
                       ComponentConfig config = ComponentConfig());

// Usage:
auto fs = fieldset(ctx, mk(parent, 0), "Audio Settings");
slider(ctx, mk(fs.ent(), 0), master_vol, ComponentConfig{}.with_label("Master"));
slider(ctx, mk(fs.ent(), 1), music_vol, ComponentConfig{}.with_label("Music"));
```

---

## Form Label

A text label for form inputs, providing accessible labeling.

### Suggested Implementation

```cpp
ElementResult form_label(HasUIContext auto &ctx, EntityParent ep_pair,
                         const std::string &text,
                         Entity for_input,  // Associates with input
                         ComponentConfig config = ComponentConfig());
```

### Features Needed
- Click label to focus associated input
- Required indicator (*)
- Optional helper text
- Error message display

---

## Rating

Lets users see and/or set a star rating for a product or other item.

### Use Cases in Game UI
- Player ratings for user-created content
- Item quality display
- Difficulty rating

### Suggested Implementation

```cpp
struct RatingConfig {
    int max_stars = 5;
    bool half_stars = false;
    bool read_only = false;
    TextureConfig star_filled;
    TextureConfig star_empty;
    TextureConfig star_half;
    float size = 24.0f;
};

ElementResult rating(HasUIContext auto &ctx, EntityParent ep_pair,
                     float &value,
                     RatingConfig config = RatingConfig());

// Read-only display
ElementResult rating_display(HasUIContext auto &ctx, EntityParent ep_pair,
                             float value,
                             RatingConfig config = RatingConfig());
```

### Features Needed
- Click/hover to set rating
- Half-star support
- Custom icons (stars, hearts, etc.)
- Read-only mode for display
- Animation on change

---

## Segmented Control

A hybrid between button group, radio buttons, and tabs. Used to switch between different options or views.

### Use Cases in Game UI
- View mode toggle (Grid/List)
- Time period selection (Day/Week/Month)
- Sort direction (Asc/Desc)

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
```

### Features Needed
- Connected button appearance
- Active segment highlight
- Slide/animate between segments
- Icon-only mode
- Keyboard navigation
