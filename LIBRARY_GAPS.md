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

## 9b. Focus Ring Style Configuration

**Location:** `vendor/afterhours/src/plugins/ui/rendering.h` (lines 463-478)

**Issue:** Only a single focus ring style is supported (outline rectangle). Games use many different focus indication styles, and the current implementation forces all elements to use the same outline style.

**Current Implementation:**
```cpp
if (context.visual_focus_id == entity.id) {
  Color focus_col = context.theme.from_usage(Theme::Usage::Accent);
  // ...
  if (corner_settings.any()) {
    draw_rectangle_rounded(focus_rect, roundness, segments, focus_col, corner_settings);
  } else {
    draw_rectangle_outline(focus_rect, focus_col);
  }
}
```

**Inspiration Examples:**
- **Dead Space Settings**: Uses a teal highlight bar on the left edge of selected items
- **Fighter Menu**: Changes background color when focused (no visible ring)
- **Kirby Options**: Scales element up + adds thicker border when selected
- **Cozy Cafe**: Increases border thickness on selected items
- **Mini Motorways**: Uses a subtle glow/shadow effect around focused elements

**Suggested Implementation:**

Add `FocusRingStyle` enum and per-element configuration:
```cpp
enum class FocusRingStyle {
  None,           // Disable focus ring (for custom focus handling)
  Outline,        // Current default - rectangle outline around element
  Fill,           // Change background color when focused
  HighlightBar,   // Colored bar on edge (left, right, top, or bottom)
  Glow,           // Soft outer glow/shadow effect
  Underline,      // Line under the element
  Scale,          // Element becomes slightly larger when focused
  Bracket,        // Decorative brackets around element (e.g., "> Option <")
};

// Per-element configuration
ComponentConfig{}
    .with_focus_style(FocusRingStyle::HighlightBar)
    .with_focus_color(Theme::Usage::Accent)  // or with_custom_focus_color(...)
    .with_focus_edge(Edge::Left)             // for HighlightBar style
    .with_focus_thickness(4.0f);             // bar/underline thickness

// Or theme-level default
struct Theme {
    FocusRingStyle default_focus_style = FocusRingStyle::Outline;
    float focus_ring_thickness = 2.0f;
    float focus_glow_radius = 8.0f;
    float focus_scale_factor = 1.05f;
    // ... etc
};
```

**Additional Features Needed:**
- Animate focus transitions (fade in/out, scale animation)
- Support combining styles (e.g., glow + scale)
- Per-direction configuration for HighlightBar (left/right/top/bottom)
- Focus ring offset from element bounds

**Current Workaround:** Manually implement focus indication in each screen by:
1. Setting `skip_when_tabbing = true` on library elements
2. Tracking selection state manually
3. Applying custom styling based on selection state
4. Example from Dead Space:
```cpp
// Highlight bar on selected
if (is_selected) {
    div(context, mk(row.ent(), 1000),
        ComponentConfig{}
            .with_absolute()
            .with_size({pixels(4), percent(1.0f)})
            .with_custom_background(teal_highlight));
}
```

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
| Focus ring position | Medium | Accessibility concern | Open |
| **Focus ring styles** | **High** | **Can't match game focus patterns** | Open |
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
| **Slider/Range** | **High** | **No slider control** | Missing |
| **Toggle Switch** | **Medium** | **No iOS-style on/off** | Missing |


---

## 37. Missing Widget: Slider / Range Input

**Status:** Not implemented

**ImGui Equivalent:** `SliderFloat()`, `SliderInt()`

**Issue:** No visual slider control with a track, fill, and draggable handle. Different from `InputNumber` (#26) which is about text entry with +/- buttons. Sliders are essential for volume, brightness, sensitivity, and other continuous value settings.

**Inspiration Examples:** 
- PowerWash Simulator settings use sliders for Anti-Aliasing levels
- FIFA/Rematch settings use sliders for Max FPS, Gamma, Motion blur
- Mini Motorways uses sliders for Controller Cursor Sensitivity
- Islands & Trains uses segmented discrete sliders (bar made of separate segments)
- Parcel Corps uses colored volume bars with continuous fill

**Current Workaround:** Stack three divs manually - track background, fill bar, handle circle:
```cpp
// Track background
div(context, mk(entity, 100), ComponentConfig{}
    .with_size({pixels(200), pixels(6)})
    .with_custom_background(track_color));

// Fill (percentage of track)
div(context, mk(entity, 101), ComponentConfig{}
    .with_size({pixels(int(200 * value)), pixels(6)})
    .with_custom_background(fill_color));

// Handle
div(context, mk(entity, 102), ComponentConfig{}
    .with_size({pixels(12), pixels(12)})
    .with_translate(200 * value - 6, -3)
    .with_custom_background(handle_color)
    .with_rounded_corners(0b1111)
    .with_roundness(1.0f));
```

**Suggested Implementation:**
```cpp
ElementResult slider(HasUIContext auto &ctx, EntityParent ep_pair,
                     float &value, float min, float max,
                     ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Visual track with fill indicator
- Draggable handle (mouse + touch)
- Keyboard support (arrows adjust value)
- Gamepad support (left/right on focused)
- Optional value display
- Vertical orientation option

---

## 38. Missing Widget: Toggle Switch

**Status:** Not implemented

**ImGui Equivalent:** None (ImGui uses Checkbox)

**Issue:** No iOS/Android-style toggle switch. Different from checkbox which is a square with checkmark. Toggle switches are a mobile UI pattern that's now common in all game settings.

**Inspiration Examples:**
- Angry Birds settings uses circular toggle buttons for Music/Sound/Vibration
- Mini Motorways uses circle toggles with X/checkmark for Night Mode, Vibration, etc.
- Parcel Corps uses iOS-style pill toggles with sliding white knobs

**Current Workaround:** Style a button to look like a toggle:
```cpp
if (button(context, mk(entity, 0),
    ComponentConfig{}
        .with_label(is_on ? "V" : "X")
        .with_custom_background(is_on ? green : gray)
        .with_rounded_corners(0b1111)
        .with_roundness(1.0f))) {
    is_on = !is_on;
}
```

**Suggested Implementation:**
```cpp
ElementResult toggle_switch(HasUIContext auto &ctx, EntityParent ep_pair,
                            bool &value,
                            ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Pill-shaped track with sliding circle
- Animation between states
- Clear on/off visual distinction
- Optional on/off labels

---

# Visual Effects Gaps (Discovered matching UI inspirations)

## 29. Missing Feature: Text Stroke / Outline

**Status:** Not implemented

**Issue:** Cannot render text with an outline/stroke effect. This is critical for game UI titles and badges where text needs to stand out against varying backgrounds.

**Inspiration Example:** "DREAM INCORPORATED" title in tycoon game needs white text with blue outline for legibility and style.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_label("DREAM")
    .with_text_stroke(Color{90, 160, 210, 255}, 2.0f);  // color, thickness
```

**Workaround:** Render text multiple times at offsets to simulate outline (implemented in `src/ui_workarounds/TextOutline.h`)

---

## 30. Missing Feature: Gradient Backgrounds

**Status:** Not implemented

**Issue:** Cannot render gradient fills (linear, radial). Solid colors limit visual depth and polish.

**Inspiration Example:** Sky backgrounds, button highlights, panel depth effects all benefit from gradients.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_gradient_background(
        GradientType::Linear,
        Color{155, 195, 235, 255},  // start
        Color{200, 220, 245, 255},  // end
        0.0f);                       // angle in degrees
```

**Workaround:** Layer multiple semi-transparent divs to approximate gradients.

---

## 31. Missing Feature: Decorative/Irregular Borders

**Status:** Limited - only uniform rectangular borders with optional rounded corners

**Issue:** Cannot create decorative frames like scalloped edges, hand-drawn borders, or irregular outlines common in cozy game aesthetics.

**Inspiration Example:** The kraft paper frame in the cozy cafe has a slightly irregular, textured border appearance.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_border_style(BorderStyle::Scalloped)  // or Dashed, Dotted, Double
    .with_border_image(texture);                 // 9-slice border image
```

**Workaround:** Use 9-slice border images via `with_texture()` on border elements.

---

## 32. Missing Feature: Circular Progress Indicator

**Status:** Not implemented (only linear progress via manual div stacking)

**Issue:** Cannot render circular/radial progress indicators. Common in game UIs for cooldowns, gauges, and status indicators.

**Inspiration Example:** Happiness and Resources gauges in tycoon game use circular progress style.

**Suggested Implementation:**
```cpp
circular_progress(context, mk(entity, 0),
    0.75f,  // value 0-1
    ComponentConfig{}
        .with_size(pixels(50), pixels(50))
        .with_custom_background(fill_color)
        .with_border(track_color, 4.0f));
```

**Workaround:** Approximate with pie-slice divs or use icon images.

---

## 33. Missing Feature: Icon Font / Symbol Rendering

**Status:** Limited - Unicode symbols don't render if font lacks codepoints

**Issue:** Loading custom icon fonts (like Font Awesome, Material Icons, Nerd Fonts) requires manually loading all needed codepoints. No automatic codepoint detection from font files.

**Inspiration Example:** All three game mockups need distinct icon sets (coffee beans, gears, weapons, etc.)

**Current Limitation:**
```cpp
// Standard load_font only gets ASCII 32-126
font_manager.load_font("Icons", icon_font_path);  // Icons won't render!

// Must manually specify codepoints
font_manager.load_font_with_codepoints("Icons", icon_font_path,
    codepoints.data(), codepoints.size());
```

**Suggested Fix:** 
1. Add `load_font_full(name, path)` that loads all codepoints in the font
2. Or add `load_font_range(name, path, start, end)` for Unicode ranges
3. Or auto-detect needed codepoints from font file

---

## 34. Missing Feature: Drop Shadow on Text

**Status:** Not implemented (only panel shadows via `with_soft_shadow`)

**Issue:** Cannot add drop shadows to text for depth and legibility. The existing `with_soft_shadow` only applies to the element's box, not the text within.

**Inspiration Example:** Title text in game UIs often has subtle drop shadows for depth.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_label("Title")
    .with_text_shadow(Color{0, 0, 0, 100}, 2.0f, 2.0f);  // color, offset_x, offset_y
```

**Workaround:** Render text twice - once offset in shadow color, once in normal position (implemented in `src/ui_workarounds/TextShadow.h`)

---

## 35. Missing Feature: Notification Badges

**Status:** Not implemented

**Issue:** Cannot add notification badges (small circles with numbers or symbols) to icons/buttons. Common pattern for unread counts, alerts, and status indicators.

**Inspiration Example:** Cozy Cafe icons have red badges with "2" and "!" to indicate notifications.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_notification_badge("2", Color{200, 60, 60, 255});
// Automatically positions at top-right of element
```

**Workaround:** Manually position small circular divs with absolute positioning (implemented in `src/ui_workarounds/NotificationBadge.h`)

---

## 36. Missing Feature: Decorative Frame/Border

**Status:** Not implemented

**Issue:** Cannot create decorative frames around screens or panels with depth effects (multiple border layers, corner accents).

**Inspiration Example:** The kraft paper frame in Cozy Cafe has corner accents and layered borders for a scrapbook feel.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_decorative_frame(FrameStyle::KraftPaper, 12.0f);  // style, thickness
```

**Workaround:** Layer multiple divs to create frame effect (implemented in `src/ui_workarounds/DecorativeFrame.h`)

---

## 39. Missing Feature: 3D/Puffy Text Effect

**Status:** Not implemented

**Issue:** Cannot create 3D "puffy" text effects where text appears to have depth/volume with highlights and shadows. This is common in casual game titles.

**Inspiration Example:** The "DREAM INCORPORATED" title has a thick white outline with a subtle 3D shadow making the text appear to pop out.

**Current Workaround:** Using text outline workaround provides flat outline only, not 3D depth.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_puffy_text(outline_color, highlight_color, shadow_color, depth);
```

**Workaround:** Enhanced text outline with offset shadow layers (not yet implemented - would need new workaround file)

---

# Workaround Files Reference

The following workaround files have been created to compensate for missing library features. These should be migrated to native library features when available:

| File | Gap | Description |
|------|-----|-------------|
| `src/ui_workarounds/TextOutline.h` | #29 | Renders text multiple times at offsets to simulate outline |
| `src/ui_workarounds/TextShadow.h` | #34 | Renders text twice for drop shadow effect |
| `src/ui_workarounds/GradientBackground.h` | #30 | Layers horizontal strips to approximate vertical gradient |
| `src/ui_workarounds/NotificationBadge.h` | #35 | Creates positioned circular badges with text |
| `src/ui_workarounds/DecorativeFrame.h` | #36 | Creates layered borders with corner accents |

---

# Updated Priority Table (Visual Effects)

| Gap | Severity | User Impact | Status |
|-----|----------|-------------|--------|
| **Text Stroke/Outline** | **High** | **Can't match game title styles** | Workaround |
| **Gradient Backgrounds** | **Medium** | **Flat colors lack depth** | Workaround |
| **Decorative Borders** | **Low** | **Can approximate with images** | Workaround |
| **Circular Progress** | **Medium** | **No radial gauges** | Missing |
| **Icon Font Loading** | **High** | **Icons show as ?** | Limited |
| **Text Drop Shadow** | **Medium** | **Text lacks depth** | Workaround |
| **Notification Badges** | **Medium** | **No alert indicators** | Workaround |
| **Decorative Frame** | **Low** | **No scrapbook-style borders** | Workaround |
| **3D/Puffy Text** | **Medium** | **Game titles lack depth** | Missing |
