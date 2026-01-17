# Afterhours UI Library Gaps

Issues and limitations discovered during UI Showcase development. Only includes items that need implementation.

---

# Missing Widgets - By Priority

## HIGH PRIORITY

### Toggle Switch (iOS-style)

**Status:** Not implemented

**Issue:** Toggle switches (pill-shaped track with sliding circular knob) are manually reimplemented in multiple screens. Different from checkbox which is a square with checkmark.

**Screens Using This Pattern:**
- ParcelCorpsSettings: `render_toggle_row_with_icon()` and `render_toggle_row_rainbow()`
- MiniMotorwaysSettings: Circle toggles with X/V markers
- AngryBirdsSettings: Similar toggle buttons

**Suggested Implementation:**
```cpp
enum struct ToggleSwitchStyle {
    Pill,      // iOS-style pill with sliding knob (default)
    Circle,    // Single circle with X/checkmark inside
    Icon,      // Circle with custom icons for on/off states
};

ElementResult toggle_switch(HasUIContext auto &ctx, EntityParent ep_pair,
                            bool &value,
                            ComponentConfig config = ComponentConfig(),
                            ToggleSwitchStyle style = ToggleSwitchStyle::Pill);
```

**Features Needed:**
- Pill-shaped track with sliding circular knob
- Animation for state transition
- Optional label (inline with toggle)
- Circle variant with X/checkmark markers

---

### Stepper/Selector with Arrows

**Status:** Not implemented

**Issue:** The pattern `< value >` with left/right arrows to cycle through discrete options is reimplemented in almost every settings screen.

**Screens Using This Pattern:**
- ParcelCorps: `render_language_row()`, `render_selector_row()`
- MiniMotorways: Sensitivity selector
- FlightOptions, KirbyOptions, DeadSpace, etc.

**Suggested Implementation:**
```cpp
template <typename Container>
ElementResult stepper(HasUIContext auto &ctx, EntityParent ep_pair,
                      const Container &options, size_t &option_index,
                      ComponentConfig config = ComponentConfig());

// Numeric variants
ElementResult stepper_int(HasUIContext auto &ctx, EntityParent ep_pair,
                          int &value, int min, int max, int step = 1,
                          ComponentConfig config = ComponentConfig());

ElementResult stepper_float(HasUIContext auto &ctx, EntityParent ep_pair,
                            float &value, float min, float max, float step = 0.1f,
                            ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- [-] and [+] buttons on sides
- Current value display in center
- Long-press for rapid increment
- Keyboard: left/right arrows when focused

---

### Labeled Setting Row

**Status:** Not implemented

**Issue:** The pattern `[Icon] Label .............. [Control]` is the most common UI pattern in settings screens.

**Pattern Structure:**
```
┌────────────────────────────────────────────────────────┐
│ [Icon]  Label Text                     [Toggle/Value] │
└────────────────────────────────────────────────────────┘
```

**Suggested Implementation:**
```cpp
enum struct SettingRowControlType {
    Toggle,      // Boolean toggle switch
    Stepper,     // < value > arrows
    Dropdown,    // Opens dropdown
    Slider,      // Inline slider
    Display,     // Read-only value display
};

struct SettingRowConfig {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<Color> icon_bg_color;
    SettingRowControlType control_type;
};

ElementResult setting_row(HasUIContext auto &ctx, EntityParent ep_pair,
                          const SettingRowConfig &config,
                          /* control value reference */);
```

---

### Radio Button Group

**Status:** Not implemented

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

## MEDIUM PRIORITY

### Text Input

**Status:** Not implemented

**Issue:** No way to accept text input from users. Fundamental widget for forms, search boxes, chat, and settings.

**Suggested Implementation:**
```cpp
ElementResult text_input(HasUIContext auto &ctx, EntityParent ep_pair,
                         std::string &value,
                         ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Single-line text entry
- Cursor position tracking
- Text selection (click-drag, Shift+Arrow)
- Copy/paste support (Ctrl+C/V)
- Keyboard navigation (Home, End, Arrow keys)
- Placeholder text support

---

### Scroll Container

**Status:** Not implemented

**Issue:** Content that exceeds container bounds is clipped with no way to access it. Critical for lists, logs, and content of unknown length.

**Suggested Implementation:**
```cpp
ElementResult scroll_view(HasUIContext auto &ctx, EntityParent ep_pair,
                          ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Vertical scrolling (mouse wheel, drag scrollbar)
- Horizontal scrolling (Shift+wheel)
- Scrollbar rendering (optional hide when not needed)
- Scroll position get/set
- Scroll-to-element support
- Touch/gamepad support

---

### Tab Bar

**Status:** Not implemented

**Issue:** No way to organize content into tabbed panels. Common for settings pages and multi-view interfaces.

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

### Tooltip

**Status:** Not implemented

**Issue:** No hover-based contextual help.

**Suggested Implementation:**
```cpp
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

### Modal / Dialog

**Status:** Not implemented

**Issue:** No overlay dialogs that block interaction with background content.

**Suggested Implementation:**
```cpp
ElementResult modal(HasUIContext auto &ctx, EntityParent ep_pair,
                    bool &is_open,
                    ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Dim/blur background
- Centered positioning
- Click-outside-to-close option
- Focus trap (tab stays within modal)
- Escape to close

---

### Context Menu / Popup

**Status:** Not implemented

**Issue:** No right-click menus or popup menus.

**Suggested Implementation:**
```cpp
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

### Tree Node / Collapsible Header

**Status:** Not implemented

**Issue:** No expandable/collapsible sections. Needed for hierarchical data and settings categories.

**Suggested Implementation:**
```cpp
ElementResult tree_node(HasUIContext auto &ctx, EntityParent ep_pair,
                        const std::string &label,
                        bool &expanded,
                          ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Expand/collapse toggle (click or keyboard)
- Expand/collapse icon (arrow or +/-)
- Indentation for nested nodes
- Optional selection state

---

### Button Variants (Filled/Outline/Ghost)

**Status:** Not implemented

**Issue:** Only a single `button()` function exists. Game UIs need multiple button styles.

**Suggested Implementation:**
```cpp
enum struct ButtonVariant { Filled, Outline, Ghost, Icon };

ElementResult button(HasUIContext auto &ctx, EntityParent ep_pair,
                     ComponentConfig config = ComponentConfig(),
                     ButtonVariant variant = ButtonVariant::Filled);

// Or via config:
ComponentConfig{}.with_button_variant(ButtonVariant::Outline);
```

---

### Icon + Text Button

**Status:** Not implemented

**Issue:** No built-in way to create buttons with both an icon and label.

**Suggested Implementation:**
```cpp
ElementResult icon_button(HasUIContext auto &ctx, EntityParent ep_pair,
                          texture_manager::Texture icon,
                          texture_manager::Rectangle icon_rect,
                          ComponentConfig config = ComponentConfig());

// Or via config
ComponentConfig{}
    .with_label("Settings")
    .with_icon(gear_texture, gear_rect)
    .with_icon_position(IconPosition::Left);  // Left, Right, Top, Bottom
```

---

### Input Number (Int/Float)

**Status:** Not implemented

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

### Centered Content Container

**Status:** Not implemented

**Issue:** Almost every screen manually calculates content centering.

**Suggested Implementation:**
```cpp
Size centered_max(float max_width);  // Centers with max width constraint

// Or alignment on containers
ComponentConfig{}
    .with_size(ComponentSize{pixels(850), percent(1.0f)})
    .with_self_align(SelfAlign::Center);  // Centers within parent
```

---

### Inline Meter/Gauge

**Status:** Not implemented

**Issue:** Small inline progress indicators with icon and label are common in game UIs.

**Pattern Structure:**
```
┌─────────────────────────────────┐
│ [Icon] Label   [███████░░░░░░] │
└─────────────────────────────────┘
```

**Suggested Implementation:**
```cpp
ElementResult inline_meter(HasUIContext auto &ctx, EntityParent ep_pair,
                           float value,  // 0.0 to 1.0
                           ComponentConfig config = ComponentConfig());
```

---

### Corner Bracket Decorations

**Status:** Not implemented

**Issue:** Tech/sci-fi UIs commonly use corner bracket decorations on panels. DeadSpace manually creates 8 divs for this effect.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_corner_brackets(bracket_color, bracket_size, bracket_thickness);

// Or as a helper function
void corner_brackets(HasUIContext auto &ctx, Entity &parent, int base_id,
                     float x, float y, float w, float h,
                     Color color, float size = 15.0f, float thickness = 2.0f);
```

---

### Vertical Tab Bar

**Status:** Not implemented

**Issue:** Vertical sidebar navigation with icons is common (EmpireTycoon left nav, MiniMotorways category tabs).

**Suggested Implementation:**
```cpp
enum struct TabBarOrientation { Horizontal, Vertical };

struct TabConfig {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<Color> bg_color;
    std::optional<std::string> badge_text;
};

ElementResult tab_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                      const std::vector<TabConfig> &tabs,
                      size_t &active_tab,
                      TabBarOrientation orientation = TabBarOrientation::Horizontal,
                      ComponentConfig config = ComponentConfig());
```

---

## LOW PRIORITY

### List Box

**Status:** Not implemented

**Issue:** No scrollable list with selection. Dropdown works for small option sets but not for large lists.

**Suggested Implementation:**
```cpp
template <typename Container>
ElementResult list_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const Container &items,
                       size_t &selected_index,
                      ComponentConfig config = ComponentConfig());
```

---

### Table / Data Grid

**Status:** Not implemented

**Issue:** No structured table layout with columns.

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

### Color Picker

**Status:** Not implemented

**Issue:** No way for users to select colors.

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

### Drag and Drop

**Status:** Not implemented

**Issue:** No way to drag items between containers.

**Suggested Implementation:**
```cpp
ComponentConfig{}.with_draggable(true);
ComponentConfig{}.with_drop_target(true);
// Plus callbacks for drag start, over, drop
```

---

### CSS Grid Layout

**Status:** Not implemented

**Issue:** Only Row/Column flex layout is available.

**Suggested Implementation:**
```cpp
ElementResult grid(HasUIContext auto &ctx, EntityParent ep_pair,
                   int columns, int rows = -1,  // -1 = auto rows
                      ComponentConfig config = ComponentConfig());

ComponentConfig{}
    .with_grid_gap(DefaultSpacing::small())
    .with_grid_columns(3)
    .with_grid_auto_rows(pixels(100));
```

---

### Text Area (Multiline Input)

**Status:** Not implemented

**Issue:** No way to accept multi-line text input.

**Additional Features Beyond Text Input:**
- Line wrapping
- Vertical scrolling
- Line numbers (optional)
- Tab handling

---

### Currency/Resource Display

**Status:** Not implemented

**Issue:** Currency pills (icon + formatted number) are common but manually composed.

**Pattern Structure:**
```
┌──────────────────────┐
│ [💰] $1,250,980      │
└──────────────────────┘
```

**Suggested Implementation:**
```cpp
ElementResult currency_display(HasUIContext auto &ctx, EntityParent ep_pair,
                               int64_t amount,
                               ComponentConfig config = ComponentConfig());

ElementResult resource_display(HasUIContext auto &ctx, EntityParent ep_pair,
                               TextureConfig icon,
                               const std::string &value,
                               ComponentConfig config = ComponentConfig());
```

---

### Chat/Message List

**Status:** Not implemented

**Issue:** Chat message displays appear in multiple screens.

**Suggested Implementation:**
```cpp
struct ChatMessage {
    std::string username;
    std::string message;
    std::optional<TextureConfig> avatar;
    std::optional<Color> username_color;
};

ElementResult chat_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::vector<ChatMessage> &messages,
                       ComponentConfig config = ComponentConfig(),
                       size_t max_visible = 3);
```

---

### Equipment/Inventory Slot

**Status:** Not implemented

**Issue:** Equipment slots with icon, quantity badge, and selection border are common.

**Suggested Implementation:**
```cpp
struct EquipmentSlotConfig {
    std::optional<TextureConfig> icon;
    std::optional<std::string> quantity_text;
    bool is_selected = false;
    bool is_disabled = false;
};

ElementResult equipment_slot(HasUIContext auto &ctx, EntityParent ep_pair,
                              const EquipmentSlotConfig &slot_config,
                              ComponentConfig config = ComponentConfig());
```

---

### Dual/Stacked Progress Bars

**Status:** Not implemented

**Issue:** Health + Armor/Shield bars stacked together are common in action games.

**Suggested Implementation:**
```cpp
ElementResult stacked_bars(HasUIContext auto &ctx, EntityParent ep_pair,
                           float primary_value, float secondary_value,
                           ComponentConfig config = ComponentConfig());
```

---

### Notification Stack / Kill Feed

**Status:** Not implemented

**Issue:** Stacked temporary notifications with auto-dismiss are common in multiplayer games.

**Suggested Implementation:**
```cpp
struct Notification {
    std::string text;
    std::optional<Color> color;
    float duration;
};

ElementResult notification_stack(HasUIContext auto &ctx, EntityParent ep_pair,
                                  std::vector<Notification> &notifications,
                      ComponentConfig config = ComponentConfig());
```

---

### Grid/Decorative Background Pattern

**Status:** Not implemented

**Issue:** MiniMotorways manually draws grid lines for background effect.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_grid_background(cell_size, line_color, line_thickness);
```

---

# Library Infrastructure Gaps

## Focus Ring Style Configuration

**Issue:** Only a single focus ring style is supported (outline rectangle). Games use many different focus indication styles.

**Inspiration Examples:**
- **Dead Space**: Teal highlight bar on the left edge
- **Fighter Menu**: Background color change when focused
- **Kirby Options**: Scales element up + adds thicker border
- **Mini Motorways**: Subtle glow/shadow effect

**Suggested Implementation:**
```cpp
enum class FocusRingStyle {
  None,           // Disable focus ring
  Outline,        // Current default
  Fill,           // Change background color when focused
  HighlightBar,   // Colored bar on edge
  Glow,           // Soft outer glow/shadow effect
  Underline,      // Line under the element
  Scale,          // Element becomes slightly larger
  Bracket,        // Decorative brackets "> Option <"
};

ComponentConfig{}
    .with_focus_style(FocusRingStyle::HighlightBar)
    .with_focus_color(Theme::Usage::Accent)
    .with_focus_edge(Edge::Left)
    .with_focus_thickness(4.0f);

struct Theme {
    FocusRingStyle default_focus_style = FocusRingStyle::Outline;
    float focus_ring_thickness = 2.0f;
    float focus_glow_radius = 8.0f;
    float focus_scale_factor = 1.05f;
};
```

---

## CJK Font Loading

**Issue:** Standard `load_font()` only loads ASCII glyphs. CJK characters require `load_font_with_codepoints()` which is not obvious.

**Suggested Fix:** 
1. Add `load_font_full(name, path)` that loads all codepoints
2. Or add `load_font_range(name, path, start, end)` for Unicode ranges
3. Or auto-detect needed codepoints from font file

---

## Circular Dependency with Styling Defaults

**Location:** `vendor/afterhours/src/plugins/ui/context.h`

**Issue:** `UIStylingDefaults` cannot be stored in `UIContext` due to circular dependency.

**Suggested Fix:** Forward declarations or moving types.

---

## Tabbing with Value Controls

**Issue:** Unclear how to handle components that want to use Widget Value Down/Up to control values while also participating in tabbing navigation.

**Suggested Fix:** Add a mode or flag to distinguish between navigation and value adjustment contexts.

---

---

## Test Input Hooks

**Status:** Not implemented

**Issue:** No first-class way to inject input events for automated tests.

**Suggested Implementation:**
```cpp
namespace ui::test {
    void push_key(int keycode);
    void push_char(char32_t ch);
    void set_mouse_position(float x, float y);
    void click_mouse(MouseButton button);
    void clear();
}
```

---

## Glyph Metrics API

**Status:** Not implemented

**Issue:** No way to get precise per-character widths for accurate caret positioning.

**Suggested Implementation:**
```cpp
namespace fonts {
    float measure_char(const std::string &font_name, float size, char32_t ch);
    float measure_substring(const std::string &font_name, float size, 
                            std::string_view text, size_t start, size_t len);
    size_t char_at_position(const std::string &font_name, float size,
                            std::string_view text, float x_offset);
}
```

---

# Visual Effects Gaps

## ~~Text Stroke / Outline~~ ✅ RESOLVED

**Status:** Implemented natively in the library.

**Usage:**
```cpp
ComponentConfig{}
    .with_label("DREAM")
    .with_text_stroke(Color{90, 160, 210, 255}, 2.0f);
```

The `with_text_stroke(color, thickness)` method renders text with an outline effect
by drawing the text at 8 offset positions before drawing the main text on top.
Supports opacity inheritance and works with all text alignment modes.

**Workaround:** Deleted - use native API

---

## Gradient Backgrounds

**Issue:** Cannot render gradient fills (linear, radial).

**Workaround:** `src/ui_workarounds/GradientBackground.h` - layered strips

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_gradient_background(
        GradientType::Linear,
        Color{155, 195, 235, 255},  // start
        Color{200, 220, 245, 255},  // end
        0.0f);                       // angle
```

---

## Circular Progress Indicator

**Issue:** Cannot render circular/radial progress indicators. Needs `draw_arc`.

**Suggested Implementation:**
```cpp
circular_progress(context, mk(entity, 0),
    0.75f,  // value 0-1
    ComponentConfig{}
        .with_size(pixels(50), pixels(50))
        .with_custom_background(fill_color)
        .with_border(track_color, 4.0f));
```

---

## 3D/Puffy Text Effect

**Issue:** Cannot create 3D "puffy" text effects with depth/volume.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_puffy_text(outline_color, highlight_color, shadow_color, depth);
```

---

## Decorative/Irregular Borders

**Issue:** Only uniform rectangular borders. Cannot create scalloped, dashed, or 9-slice borders.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_border_style(BorderStyle::Scalloped)  // or Dashed, Dotted, Double
    .with_border_image(texture);                 // 9-slice border image
```

---

## Icon Font / Symbol Rendering

**Issue:** Loading icon fonts requires manually specifying codepoints. No auto-detection.

**Suggested Fix:** 
1. `load_font_full(name, path)` - loads all codepoints in font
2. `load_font_range(name, path, start, end)` - loads Unicode range

---

# Existing Workarounds Reference

| File | Gap | Description |
|------|-----|-------------|
| ~~`src/ui_workarounds/TextOutline.h`~~ | ~~Text Stroke~~ | ✅ RESOLVED: Use `with_text_stroke()` |
| ~~`src/ui_workarounds/TextShadow.h`~~ | ~~Drop Shadow~~ | ✅ RESOLVED: Use `with_text_shadow()` |
| `src/ui_workarounds/GradientBackground.h` | Gradients | Layered strips |
| `src/ui_workarounds/NotificationBadge.h` | Badges | Positioned circles with text |
| `src/ui_workarounds/DecorativeFrame.h` | Frames | Layered borders with corners |

---

# Priority Summary

## Trivial (~1-2 hours)
- (none remaining)

## Small (~half day)
- Toggle Switch
- Stepper/Selector
- Radio Group
- Button Variants
- Corner Brackets
- Currency Display

## Medium (~1-2 days)
- Setting Row
- ~~Text Input (basic)~~ ✅ Implemented
- Tooltip
- Input Number
- Centered Container
- Inline Meter
- Vertical Tab Bar
- Test Input Hooks
- Glyph Metrics API

## Large (~3-5 days)
- Focus Ring Styles
- Tab Bar
- Tree Node
- Modal/Dialog
- Context Menu
- CSS Grid Layout

## Very Large (~1+ week)
- Text Input (full with selection/clipboard)
- Text Area
- Scroll Container
- List Box
- Table
- Drag and Drop
- Color Picker
- Classic UI Themes
