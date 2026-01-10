# Afterhours UI Library Gaps

Issues and limitations discovered during UI Showcase development.

---

## 1. ~~Font Configuration Not in Theme~~ (RESOLVED)

**Location:** Theme struct in `vendor/afterhours/src/plugins/ui/theme.h`

**Status:** RESOLVED - Font configuration added to Theme.

**Solution:** Added `FontConfig` struct and per-language font configuration to `Theme`:
```cpp
struct FontConfig {
    std::string font_name;      // Key in FontManager
    float size_scale = 1.0f;    // Multiplier for this font's visual size
};

struct Theme {
    // ... existing color fields ...
    
    // Per-language font configuration
    std::map<translation::Language, FontConfig> language_fonts;
    
    // Base font sizes (in pixels)
    float font_size_sm = 14.f;
    float font_size_md = 18.f;
    float font_size_lg = 28.f;
    float font_size_xl = 36.f;
    
    // Helpers
    const FontConfig& get_font_config(translation::Language lang) const;
    float get_scaled_font_size(translation::Language lang, float base_size) const;
    std::string get_font_name(translation::Language lang) const;
};
```

**Usage:**
```cpp
Theme theme;
theme.language_fonts = {
    {Language::English,  {"Gaegu-Bold", 1.0f}},
    {Language::Korean,   {"NotoSansKR", 0.95f}},
    {Language::Japanese, {"Sazanami", 0.92f}},
};

// At render time:
Language lang = TranslationPlugin::get_language();
std::string font_name = theme.get_font_name(lang);
float size = theme.get_scaled_font_size(lang, theme.font_size_md);
```

**TODO:** Investigate using FontID enum instead of strings for type safety.

---

## 2. Sizing API: `screen_pct()` vs `percent()` (NOT A GAP - Documentation)

**Location:** `vendor/afterhours/src/plugins/autolayout.h`

**Clarification:** There are TWO different percentage-based sizing functions:

- **`screen_pct(value)`** - Percentage of **screen** dimensions (uses `Dim::ScreenPercent`)
- **`percent(value)`** - Percentage of **parent** dimensions (uses `Dim::Percent`)

**Common Mistake:**
```cpp
// Parent is 50% of screen width
auto parent = div(context, mk(entity, 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));

// WRONG: Using screen_pct() makes child 50% of SCREEN, not parent
auto child = div(context, mk(parent.ent(), 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));
```

**Correct Usage:**
```cpp
// Parent is 50% of screen width
auto parent = div(context, mk(entity, 0),
    ComponentConfig{}.with_size({screen_pct(0.5f), screen_pct(0.5f)}));

// CORRECT: Using percent() makes child 50% of PARENT
auto child = div(context, mk(parent.ent(), 0),
    ComponentConfig{}.with_size({percent(0.5f), percent(0.5f)}));
```

**Summary of sizing functions:**
- `pixels(value)` - Fixed pixel size
- `percent(value)` - Percentage of parent container
- `screen_pct(value)` - Percentage of screen dimensions
- `children()` - Size based on children content
- `h720(px)` / `w1280(px)` - Resolution-independent pixels (scaled from 720p/1280p baseline)

---

## 3. ~~Text Color vs Background Color Confusion in `div`~~ (RESOLVED)

**Location:** `vendor/afterhours/src/plugins/ui/component_config.h`

**Status:** RESOLVED - API has been clarified with explicit naming.

**The Problem Was:** `with_color_usage()` set background color, but the name was ambiguous.

**Solution:** New explicit APIs added, old ones deprecated:

| Old API (deprecated) | New API |
|---------------------|---------|
| `with_color_usage()` | `with_background()` |
| `with_custom_color()` | `with_custom_background()` |
| *(new)* | `with_text_color()` |
| *(new)* | `with_custom_text_color()` |

**Correct Usage:**
```cpp
div(context, mk(parent.ent(), 0),
    ComponentConfig{}
        .with_label("Some text")
        .with_background(Theme::Usage::Primary)    // Sets BACKGROUND (clear!)
        .with_text_color(Theme::Usage::Accent));   // Sets TEXT (new!)
```

Old code using `with_color_usage()` still works but shows compiler deprecation warnings guiding migration.

---

## 4. Font Size 0 Warnings with No Visual Feedback

**Location:** `vendor/afterhours/src/plugins/font_helper.h` (line 133)

**Issue:** The `position_text` auto-sizer produces font size 0 when containers are too small relative to text content. This causes text to silently not render with only a console warning.

**Symptoms:**
- Text doesn't appear in UI
- Console shows: `Invalid font size 0 passed to measure_text_utf8`
- No visual indication of what's wrong

**Current Workaround:** Make containers larger, or use fixed font sizes instead of auto-sizing.

**Suggested Fix:** Add minimum font size clamping, or visual debug indicator when text can't fit.

---

## 5. Missing Flexbox Alignment Properties

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h`

**Issue:** `with_justify_content()` and `with_align_items()` are not available in the imm `ComponentConfig` API, making it difficult to center content within containers.

**Current Workaround:** Use manual spacing/padding or fixed positioning.

**Suggested Fix:** Add these standard flexbox properties to `ComponentConfig`.

---

## 6. CJK Font Loading Requires Special Handling

**Location:** `vendor/afterhours/src/plugins/ui/font_manager.h`

**Issue:** Standard `load_font()` only loads ASCII glyphs (codepoints 32-126). CJK characters require using `load_font_with_codepoints()` which is not obvious from the API.

**Example Problem:**
```cpp
// This won't render Korean/Japanese/Chinese characters
font_manager.load_font("NotoSansKR", korean_font_path);

// Must use this instead (not obvious!)
font_manager.load_font_with_codepoints("NotoSansKR", korean_font_path, 
    0x0000, 0xFFFF);  // Or specific CJK ranges
```

**Current Workaround:** Manually use `load_font_with_codepoints()` for any non-ASCII fonts.

**Suggested Fix:** Either auto-detect font requirements, or clearly document this in the API. Consider making `load_font()` load more codepoints by default.

---

## 7. Dropdown Close Behavior

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~760)

**Issue:** No built-in way to close dropdown when clicking outside of it. Dropdowns remain open until user selects an option.

**Current Workaround:** None available - dropdowns stay open.

**Suggested Fix:** Add click-outside detection that closes any open dropdown.

---

## 8. Circular Dependency with Styling Defaults

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (lines 60-61)

**Issue:** `UIStylingDefaults` cannot be stored in `UIContext` due to circular dependency.

**Code Comment:**
```cpp
// TODO: Add styling defaults back when circular dependency is resolved
// imm::UIStylingDefaults styling_defaults;
```

**Suggested Fix:** Refactor to break the circular dependency, possibly by forward declarations or moving types.

---

## 9. Focus Ring Positioning on Checkbox

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~220)

**Issue:** Checkbox focus ring is drawn on the wrong element. The clickable area is `checkbox_no_label` but focus ring appears elsewhere.

**Code Comment:**
```cpp
// TODO the focus ring is not correct because the actual clickable area is the
// checkbox_no_label element, not the checkbox element.
```

**Suggested Fix:** Move focus ring rendering to the actual clickable element.

---

## 10. Checkbox Corner Config Merging

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~258)

**Issue:** No way to merge user-provided corner configuration with internal checkbox styling.

**Code Comment:**
```cpp
// TODO - if the user wants to mess with the corners, how can we merge
// these
```

**Suggested Fix:** Allow corner config to be passed through and merged with component defaults.

---

## 11. Tabbing with Value Controls

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (line ~123)

**Issue:** Unclear how to handle components that want to use Widget Value Down/Up to control values while also participating in tabbing navigation.

**Code Comment:**
```cpp
// TODO How do we handle something that wants to use
// Widget Value Down/Up to control the value?
```

**Suggested Fix:** Add a mode or flag to distinguish between navigation and value adjustment contexts.

---

## 12. Auto Text Color Should Be Default Behavior

**Location:** `vendor/afterhours/src/plugins/ui/component_config.h` and `rendering.h`

**Issue:** When a background color is set via `with_background()` or `with_custom_background()`, but no text color is specified, the text uses the theme's default font color. This frequently results in invisible or low-contrast text (e.g., dark text on dark backgrounds).

**Current Behavior:**
```cpp
// This can produce invisible text if theme.font is dark and Primary is also dark
div(context, mk(entity, 0),
    ComponentConfig{}
        .with_label("Some text")
        .with_background(Theme::Usage::Primary));
```

**Required Workaround:**
```cpp
// Must manually add auto_text_color to every element with a background
div(context, mk(entity, 0),
    ComponentConfig{}
        .with_label("Some text")
        .with_background(Theme::Usage::Primary)
        .with_auto_text_color(true));  // Easy to forget!
```

**Suggested Fix:** Enable auto-contrast text color by default when:
1. A background color is specified (via `with_background()` or `with_custom_background()`)
2. No explicit text color is set (via `with_text_color()` or `with_custom_text_color()`)

This should be the default in `apply_label()` in `component_config.h`:
```cpp
// In apply_label(), when no explicit text color is set:
if (!config.has_explicit_text_color() && entity.has<HasColor>()) {
    lbl.set_background_hint(entity.get<HasColor>().color());
}
```

**Benefits:**
- Eliminates the most common cause of invisible text
- Backwards compatible (explicit text colors still work)
- Follows "sensible defaults" principle
- Reduces boilerplate in UI code

---

## 13. UI Entity Cleanup on Screen Switch (was #12)

**Location:** Screen switching logic and `vendor/afterhours/src/plugins/ui/systems.h`

**Issue:** When switching between screens in a multi-screen application, UI entities from the previous screen may persist in memory. While the `HandleTabbing` system checks `was_rendered_to_screen` before processing focus (line 357), entities themselves are not cleaned up when a new screen loads.

**Symptoms:**
- Tab navigation may attempt to focus on elements from previous screens
- Memory usage grows as entities accumulate
- Potential for entity ID collisions

**Current Behavior:**
- Screen systems are swapped but entity cleanup is left to the caller
- The `existing_ui_elements` map in `entity_management.h` caches UI entities by hash
- No automatic mechanism to clear entities when context changes

**Potential Workarounds:**
1. Call `EntityHelper::cleanup()` or similar before loading a new screen
2. Implement screen-scoped entity tracking
3. Add `was_rendered_to_screen = false` clearing more aggressively

**Suggested Fix:** Add a `clear_all_ui_entities()` function or screen transition hook that properly cleans up UI state when switching screens.

---

# ImGui Comparison: Missing Widgets

The following widgets are available in Dear ImGui but missing from afterhours. These represent potential additions to make the library more complete.

---

## 13. Missing Widget: Text Input

**Status:** Not implemented

**ImGui Equivalent:** `InputText()`, `InputTextWithHint()`

**Issue:** No way to accept text input from users. This is a fundamental widget for forms, search boxes, chat, and settings.

**Required Features:**
- Single-line text entry
- Cursor position tracking
- Text selection (click-drag, Shift+Arrow)
- Copy/paste support (Ctrl+C/V)
- Keyboard navigation (Home, End, Arrow keys)
- Placeholder text support

**Suggested Implementation:**
```cpp
ElementResult text_input(HasUIContext auto &ctx, EntityParent ep_pair,
                         std::string &value,
                         ComponentConfig config = ComponentConfig());
```

---

## 14. Missing Widget: Text Area (Multiline Input)

**Status:** Not implemented

**ImGui Equivalent:** `InputTextMultiline()`

**Issue:** No way to accept multi-line text input. Needed for descriptions, notes, comments, and code editing.

**Additional Features Beyond Text Input:**
- Line wrapping
- Vertical scrolling
- Line numbers (optional)
- Tab handling

---

## 15. Missing Widget: Progress Bar

**Status:** Not implemented

**ImGui Equivalent:** `ProgressBar()`

**Issue:** No visual indicator for loading states, download progress, health bars, or completion status.

**Suggested Implementation:**
```cpp
ElementResult progress_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                           float value,  // 0.0 to 1.0
                           ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Horizontal bar with fill based on value
- Optional label showing percentage or custom text
- Animated/indeterminate mode for unknown duration tasks
- Vertical orientation option

---

## 16. Missing Widget: Tooltip

**Status:** Not implemented

**ImGui Equivalent:** `SetTooltip()`, `BeginTooltip()`/`EndTooltip()`

**Issue:** No hover-based contextual help. Users can't get additional information about UI elements without clicking.

**Suggested Implementation:**
```cpp
// Attach to previous element
void set_tooltip(HasUIContext auto &ctx, const std::string &text);

// Or as a modifier
ComponentConfig{}.with_tooltip("Help text here");
```

**Features Needed:**
- Delay before showing (e.g., 0.5s hover)
- Position near cursor or element
- Auto-hide on mouse move away
- Rich content support (not just text)

---

## 17. Missing Widget: Scroll Container

**Status:** Not implemented

**ImGui Equivalent:** `BeginChild()`/`EndChild()` with scrolling, `SetScrollY()`

**Issue:** Content that exceeds container bounds is clipped with no way to access it. This is critical for lists, logs, and content of unknown length.

**Suggested Implementation:**
```cpp
ElementResult scroll_view(HasUIContext auto &ctx, EntityParent ep_pair,
                          ComponentConfig config = ComponentConfig());
// All children are scrollable within this container
```

**Features Needed:**
- Vertical scrolling (mouse wheel, drag scrollbar)
- Horizontal scrolling (Shift+wheel)
- Scrollbar rendering (optional hide when not needed)
- Scroll position get/set
- Scroll-to-element support
- Touch/gamepad support

---

## 18. Missing Widget: Tab Bar

**Status:** Not implemented

**ImGui Equivalent:** `BeginTabBar()`/`EndTabBar()`, `BeginTabItem()`/`EndTabItem()`

**Issue:** No way to organize content into tabbed panels. Common need for settings pages, inventory categories, and multi-view interfaces.

**Suggested Implementation:**
```cpp
ElementResult tab_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                      const std::vector<std::string> &tab_labels,
                      size_t &active_tab,
                      ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Horizontal row of tabs
- Active tab highlighting
- Click to switch
- Keyboard navigation (arrows when focused)
- Optional close button per tab

---

## 19. Missing Widget: Tree Node / Collapsible Header

**Status:** Not implemented

**ImGui Equivalent:** `TreeNode()`, `CollapsingHeader()`

**Issue:** No expandable/collapsible sections. Needed for hierarchical data, settings categories, and file browsers.

**Suggested Implementation:**
```cpp
ElementResult tree_node(HasUIContext auto &ctx, EntityParent ep_pair,
                        const std::string &label,
                        bool &expanded,
                        ComponentConfig config = ComponentConfig());
// Children are shown/hidden based on expanded state
```

**Features Needed:**
- Expand/collapse toggle (click or keyboard)
- Expand/collapse icon (arrow or +/-)
- Indentation for nested nodes
- Optional selection state

---

## 20. Missing Widget: Radio Button Group

**Status:** Not implemented (checkbox_group requires using bitset)

**ImGui Equivalent:** `RadioButton()`

**Issue:** `checkbox_group` uses `std::bitset` for multi-select. No single-select radio button where exactly one option is active.

**Suggested Implementation:**
```cpp
template <typename Container>
ElementResult radio_group(HasUIContext auto &ctx, EntityParent ep_pair,
                          const Container &labels,
                          size_t &selected_index,
                          ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Circular indicator (vs checkbox square)
- Exactly one selected at a time
- Keyboard navigation between options

---

## 21. Missing Widget: Separator

**Status:** Not implemented

**ImGui Equivalent:** `Separator()`

**Issue:** No visual divider between UI sections. Currently must use empty divs with background color.

**Suggested Implementation:**
```cpp
ElementResult separator(HasUIContext auto &ctx, EntityParent ep_pair,
                        ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Horizontal line by default
- Vertical option
- Themeable color
- Optional label in middle ("--- Section ---")

---

## 22. Missing Widget: Color Picker

**Status:** Not implemented

**ImGui Equivalent:** `ColorPicker3()`, `ColorPicker4()`, `ColorEdit3()`, `ColorEdit4()`

**Issue:** No way for users to select colors. Needed for theme customization, art tools, and settings.

**Suggested Implementation:**
```cpp
ElementResult color_picker(HasUIContext auto &ctx, EntityParent ep_pair,
                           Color &value,
                           ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- HSV color wheel or square
- RGB/HSV sliders
- Alpha channel support
- Hex input
- Color preview (old vs new)

---

## 23. Missing Widget: Modal / Dialog

**Status:** Not implemented

**ImGui Equivalent:** `OpenPopup()`, `BeginPopupModal()`/`EndPopup()`

**Issue:** No overlay dialogs that block interaction with background content. Needed for confirmations, alerts, and focused input.

**Suggested Implementation:**
```cpp
ElementResult modal(HasUIContext auto &ctx, EntityParent ep_pair,
                    bool &is_open,
                    ComponentConfig config = ComponentConfig());
// Renders on top of everything, dims background
```

**Features Needed:**
- Dim/blur background
- Centered positioning
- Click-outside-to-close option
- Focus trap (tab stays within modal)
- Escape to close

---

## 24. Missing Widget: Context Menu / Popup

**Status:** Not implemented

**ImGui Equivalent:** `BeginPopupContextItem()`, `BeginPopup()`

**Issue:** No right-click menus or popup menus. Common interaction pattern for additional actions.

**Suggested Implementation:**
```cpp
// Attach to previous element
ElementResult context_menu(HasUIContext auto &ctx, EntityParent ep_pair,
                           const std::vector<std::string> &options,
                           ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Trigger on right-click
- Position near cursor
- Click-outside-to-close
- Keyboard navigation
- Nested submenus

---

## 25. Missing Capability: Drag and Drop

**Status:** Not implemented

**ImGui Equivalent:** `BeginDragDropSource()`, `BeginDragDropTarget()`

**Issue:** No way to drag items between containers. Needed for inventory management, list reordering, and visual builders.

**Suggested Implementation:**
```cpp
ComponentConfig{}.with_draggable(true);
ComponentConfig{}.with_drop_target(true);
// Plus callbacks for drag start, over, drop
```

---

## 26. Missing Widget: Input Number (Int/Float)

**Status:** Not implemented

**ImGui Equivalent:** `InputInt()`, `InputFloat()`, `DragInt()`, `DragFloat()`

**Issue:** No specialized numeric input with validation, step buttons, or drag-to-adjust.

**Suggested Implementation:**
```cpp
ElementResult input_int(HasUIContext auto &ctx, EntityParent ep_pair,
                        int &value, int min, int max, int step = 1,
                        ComponentConfig config = ComponentConfig());

ElementResult input_float(HasUIContext auto &ctx, EntityParent ep_pair,
                          float &value, float min, float max, float step = 0.1f,
                          ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- +/- buttons
- Direct text entry with validation
- Min/max clamping
- Optional drag-to-adjust

---

## 27. Missing Widget: List Box

**Status:** Not implemented

**ImGui Equivalent:** `ListBox()`, `BeginListBox()`/`EndListBox()`

**Issue:** No scrollable list with selection. Dropdown works for small option sets but not for large lists.

**Suggested Implementation:**
```cpp
template <typename Container>
ElementResult list_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const Container &items,
                       size_t &selected_index,
                       ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Scrollable container
- Single or multi-select
- Keyboard navigation (arrows, Page Up/Down)
- Search/filter by typing

---

## 28. Missing Widget: Table / Data Grid

**Status:** Not implemented

**ImGui Equivalent:** `BeginTable()`/`EndTable()`, `TableNextColumn()`

**Issue:** No structured table layout with columns. Currently must manually create column-like layouts.

**Suggested Implementation:**
```cpp
ElementResult table(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::vector<std::string> &headers,
                    int num_columns,
                    ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Column headers
- Resizable columns
- Sortable columns (click header)
- Row selection
- Scrollable body with fixed header

---

# Priority

| Gap | Severity | User Impact | Status |
|-----|----------|-------------|--------|
| Font in Theme | Medium | Theming feels incomplete | RESOLVED |
| Text vs Background color | High | Invisible text, confusing API | RESOLVED |
| **Auto text color default** | **High** | **Invisible text without workaround** | Open |
| `screen_pct()` nesting | High | Layout overflow, hard to debug | Open |
| Font size 0 warnings | Medium | Silent failures, hard to debug | Open |
| Missing flexbox alignment | Medium | Can't center content easily | Open |
| CJK font loading | Medium | Non-ASCII text doesn't render | Open |
| Dropdown close | High | Poor UX, confusing behavior | Open |
| Styling defaults | Low | Internal architecture issue | Open |
| Focus ring | Medium | Accessibility concern | Open |
| Corner merging | Low | Minor customization limit | Open |
| Tabbing + values | Medium | Complex widget support | Open |
| Screen switch cleanup | High | Tabbing to hidden elements | **Fixed in game.cpp** |
| **Text Input** | **High** | **Can't accept user text** | Missing |
| **Scroll Container** | **High** | **Content overflow unusable** | Missing |
| **Progress Bar** | **High** | **No loading feedback** | Missing |
| **Tooltip** | **High** | **No hover help** | Missing |
| **Tab Bar** | **Medium** | **No tabbed interfaces** | Missing |
| **Tree Node** | **Medium** | **No collapsible sections** | Missing |
| **Modal/Dialog** | **Medium** | **No overlay confirmations** | Missing |
| **Radio Group** | **Low** | **Use checkbox_group workaround** | Missing |
| **Separator** | **Low** | **Minor visual polish** | Missing |
| **Color Picker** | **Low** | **Niche use case** | Missing |
| **Context Menu** | **Medium** | **No right-click menus** | Missing |
| **Drag and Drop** | **Medium** | **Can't reorder items** | Missing |
| **Input Number** | **Medium** | **No validated numeric entry** | Missing |
| **List Box** | **Medium** | **Dropdown doesn't scale** | Missing |
| **Table** | **Medium** | **No data grid** | Missing |

