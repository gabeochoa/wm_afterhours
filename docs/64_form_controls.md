# Form Controls

**Status:** Mostly implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

> ✅ **Already in afterhours:** `slider`, `checkbox`, `radio_group`, `dropdown`, `toggle_switch`  
> ❌ **Not yet implemented:** Combobox (search/filter), Fieldset, Form Label

---

## ~~Select / Dropdown~~ ✅ IMPLEMENTED

See `dropdown()` in `vendor/afterhours/src/plugins/ui/imm_components.h`

---

## Combobox / Autocomplete

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

## ~~Slider / Range Input~~ ✅ IMPLEMENTED

See `slider()` in `vendor/afterhours/src/plugins/ui/imm_components.h`

---

## ~~Checkbox~~ ✅ IMPLEMENTED

See `checkbox()` in `vendor/afterhours/src/plugins/ui/imm_components.h`

---

## ~~Radio Button Group~~ ✅ IMPLEMENTED

See `radio_group()` in `vendor/afterhours/src/plugins/ui/imm_components.h`

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

