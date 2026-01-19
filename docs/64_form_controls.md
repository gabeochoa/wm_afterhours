# Form Controls

**Status:** Partially implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

---

## Select / Dropdown

### Description

A form input for selecting a value from a list of predefined options. Shows currently selected option when collapsed, expands to show all options.

**Also known as:** Dropdown, Select input

### Use Cases in Game UI
- Language selection
- Difficulty selection
- Resolution/display mode selection
- Character class selection
- Sort order selection

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
ElementResult select(HasUIContext auto &ctx, EntityParent ep_pair,
                     const std::vector<SelectOption<T>> &options,
                     T &selected_value,
                     ComponentConfig config = ComponentConfig());

// Usage:
select(ctx, mk(parent, 0), {
    {Difficulty::Easy, "Easy", easy_icon},
    {Difficulty::Normal, "Normal", normal_icon},
    {Difficulty::Hard, "Hard", hard_icon},
    {Difficulty::Nightmare, "Nightmare", skull_icon}
}, current_difficulty);
```

### Features Needed
- Click to open dropdown
- Scrollable option list for many items
- Currently selected option highlighted
- Keyboard navigation (arrows, enter, escape)
- Optional icons per option
- Optional search/filter (see Combobox)
- Close on click outside

---

## Combobox / Autocomplete

### Description

An input that behaves like a select with the addition of a free text input to filter options.

**Also known as:** Autocomplete, Autosuggest

### Use Cases in Game UI
- Player search (find friends by name)
- Item search in large inventories
- Server browser with filter
- Console command autocomplete

### Suggested Implementation

```cpp
template <typename T>
ElementResult combobox(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::vector<SelectOption<T>> &options,
                       T &selected_value,
                       std::string &search_text,
                       ComponentConfig config = ComponentConfig());

// Usage:
combobox(ctx, mk(parent, 0), all_items, selected_item, search_query);
```

### Features Needed
- Text input for filtering
- Filtered dropdown list
- Highlight matching text
- Clear button
- Empty state when no matches

---

## Slider / Range Input

### Description

A form control for choosing a value within a preset range by dragging a handle.

### Use Cases in Game UI
- Volume controls
- Brightness/gamma
- Mouse sensitivity
- Game speed
- Character creation sliders

### Suggested Implementation

```cpp
struct SliderConfig {
    float min = 0.0f;
    float max = 1.0f;
    float step = 0.0f;  // 0 = continuous
    bool show_value = true;
    bool show_ticks = false;
    std::string value_format = "{:.0%}";  // Format string
};

ElementResult slider(HasUIContext auto &ctx, EntityParent ep_pair,
                     float &value,
                     SliderConfig config = SliderConfig());

// Integer variant
ElementResult slider_int(HasUIContext auto &ctx, EntityParent ep_pair,
                         int &value, int min, int max, int step = 1,
                         ComponentConfig config = ComponentConfig());

// Usage:
slider(ctx, mk(parent, 0), volume, SliderConfig{.min = 0, .max = 1});
slider_int(ctx, mk(parent, 1), brightness, 0, 100, 5);
```

### Features Needed
- Draggable handle
- Track fill showing current value
- Value label display
- Tick marks for discrete steps
- Keyboard control (+/- to adjust)
- Optional dual handles for range selection

---

## Checkbox

### Description

An input for boolean choices or selecting multiple items from a list.

### Note

Basic checkbox exists in the library. This documents the expected API and features.

### Suggested Implementation

```cpp
ElementResult checkbox(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::string &label,
                       bool &checked,
                       ComponentConfig config = ComponentConfig());

// Indeterminate state for parent checkboxes
ElementResult checkbox(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::string &label,
                       std::optional<bool> &checked,  // nullopt = indeterminate
                       ComponentConfig config = ComponentConfig());
```

### Features Needed
- Checked/unchecked states
- Indeterminate state (for "select all" with partial selection)
- Label positioning (right/left)
- Disabled state
- Focus ring

---

## Radio Button Group

### Description

Radio buttons allow selecting a single option from a list of predefined options.

**Also known as:** Radio, Radio group

### Note

Radio buttons exist in the library. This documents the expected API.

### Suggested Implementation

```cpp
template <typename T>
ElementResult radio_group(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::vector<std::pair<T, std::string>> &options,
                          T &selected_value,
                          ComponentConfig config = ComponentConfig());

// Usage:
radio_group(ctx, mk(parent, 0), {
    {Quality::Low, "Low"},
    {Quality::Medium, "Medium"},
    {Quality::High, "High"},
    {Quality::Ultra, "Ultra"}
}, graphics_quality);
```

---

## Fieldset

### Description

A wrapper for grouping related form fields with an optional legend/title.

### Suggested Implementation

```cpp
ElementResult fieldset(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::string &legend,
                       ComponentConfig config = ComponentConfig());

// Usage:
fieldset(ctx, mk(parent, 0), "Audio Settings") {
    slider(ctx, mk(current, 0), "Master", master_vol);
    slider(ctx, mk(current, 1), "Music", music_vol);
    slider(ctx, mk(current, 2), "Effects", sfx_vol);
}
```

---

## Label

### Description

A text label for form inputs, providing accessible labeling.

### Suggested Implementation

```cpp
ElementResult form_label(HasUIContext auto &ctx, EntityParent ep_pair,
                         const std::string &text,
                         Entity for_input,  // Associates with input
                         ComponentConfig config = ComponentConfig());

// Or integrated with inputs:
ComponentConfig{}.with_label("Username");
```

### Features Needed
- Click label to focus associated input
- Required indicator (*)
- Optional helper text
- Error message display

