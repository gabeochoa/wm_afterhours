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

**Working Implementation (wordproc):**
- `src/ecs/components.h` (ScrollComponent)
- `src/ecs/input_system.h` (mouse wheel handling)
- `src/ecs/render_system.h` (clipping + offset)

**Suggested Implementation:**
```cpp
namespace afterhours::input {
  float get_mouse_wheel_move();
  Vector2 get_mouse_wheel_move_v();  // trackpad / horizontal scroll
}

struct HasScrollView : BaseComponent {
  Vector2 scroll_offset = {0, 0};
  Vector2 content_size = {0, 0};
  Vector2 viewport_size = {0, 0};
  bool horizontal_enabled = false;
  bool vertical_enabled = true;
  float scroll_speed = 20.0f;
};

ElementResult scroll_view(HasUIContext auto &ctx, EntityParent ep_pair,
                          ComponentConfig config = ComponentConfig());
```

**Features Needed:**
- Vertical scrolling (mouse wheel, drag scrollbar)
- Horizontal scrolling (Shift+wheel)
- Scrollbar rendering (optional hide when not needed)
- Scroll position get/set
- Scroll-to-element support
- Content clipping via scissor/clip rect
- Touch/gamepad support
 - Keyboard support (Page Up/Down, arrows when focused)

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

**Working Implementation (wordproc):**
- `src/ui/win95_widgets.h/.cpp` (DrawMessageDialog, DrawInputDialog)
- `src/ecs/components.h` (dialog state flags)

**Suggested Implementation:**
```cpp
enum class DialogResult { Pending, Confirmed, Cancelled, Dismissed, Custom };

bool begin_modal(HasUIContext auto &ctx, EntityParent ep_pair,
                 const std::string &title,
                 ComponentConfig config = ComponentConfig());
void end_modal();
void close_modal(HasUIContext auto &ctx, EntityParent ep_pair,
                 DialogResult result = DialogResult::Dismissed);
```

**Features Needed:**
- Dim/blur background
- Centered positioning
- Click-outside-to-close option
- Focus trap (tab stays within modal)
- Escape to close
 - Result handling (OK/Cancel/Custom)

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

### Text Area / Text Editor (Multiline)

**Status:** Not implemented

**Issue:** No way to accept multi-line text input or full text editing (selection, undo/redo, layout).

**Working Implementation (wordproc):**
- `src/editor/text_buffer.h/.cpp` (gap buffer + selection + undo)
- `src/editor/text_layout.h/.cpp` (layout + rendering)

**Additional Features Beyond Text Input:**
- Line wrapping
- Vertical scrolling
- Line numbers (optional)
- Tab handling
 - Caret movement + selection
 - Undo/redo
 - Clipboard integration

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

### Notification Stack / Toasts

**Status:** Not implemented

**Issue:** Stacked temporary notifications with auto-dismiss are common in multiplayer games and apps (save/error toasts).

**Working Implementation (wordproc):**
- `src/extracted/status_notifications.h` (ready-to-PR implementation)
- `src/ecs/components.h` (StatusComponent)

**Suggested Implementation:**
```cpp
enum class NotificationLevel { Info, Success, Warning, Error };

struct Notification {
    std::string message;
    NotificationLevel level = NotificationLevel::Info;
    double duration = 3.0;
};

struct ProvidesNotifications : BaseComponent {
    void push(Notification notif, double current_time);
    void cleanup(double current_time);
    std::vector<const Notification *> get_visible(double current_time) const;
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

## Render System Const Constraint

**Issue:** Render systems only call the `const` version of `for_each_with`, which blocks immediate-mode UI patterns that need to mutate state during render (e.g., closing a dialog on click).

**Suggested Implementation:**
```cpp
// Option 1: opt-in mutable render systems
void register_mutable_render_system(std::unique_ptr<SystemBase> system);

// Option 2: deferred mutations from render pass
namespace ui {
  void defer(std::function<void()> fn);
}
```

---

## Action Binding System

**Issue:** Only low-level input APIs exist; no named actions with remappable keybindings and modifiers.

**Working Implementation (wordproc):**
- `src/input/action_map.h/.cpp`

**Suggested Implementation:**
```cpp
namespace afterhours::input {
  enum class Modifiers : uint8_t { None = 0, Ctrl = 1, Shift = 2, Alt = 4 };

  struct KeyBinding {
    KeyCode key = 0;
    Modifiers modifiers = Modifiers::None;
  };

  template <typename ActionEnum>
  class ActionMap {
   public:
    void bind(const KeyBinding &binding, ActionEnum action);
    bool is_action_pressed(ActionEnum action) const;
    bool is_action_down(ActionEnum action) const;
    std::optional<KeyBinding> get_binding(ActionEnum action) const;
  };

  Modifiers get_current_modifiers();
  std::string format_binding(const KeyBinding &binding);
}
```

---

## Renderer Abstraction Layer

**Issue:** Afterhours is tightly coupled to raylib; no headless backend or backend swapping.

**Working Implementation (wordproc):**
- `src/renderer/renderer_interface.h/.cpp`
- `src/renderer/raylib_renderer.h`

**Suggested Implementation:**
```cpp
namespace afterhours {
  struct IRenderer {
    virtual ~IRenderer() = default;
    virtual void begin_frame() = 0;
    virtual void end_frame() = 0;
    virtual void draw_rectangle(Rectangle rect, Color color) = 0;
    virtual void draw_text(const Font &font, const std::string &text,
                           Vector2 pos, float size, float spacing, Color color) = 0;
  };

  IRenderer &get_renderer();
  void set_renderer(std::unique_ptr<IRenderer> renderer);
  std::unique_ptr<IRenderer> create_headless_renderer(int width, int height);
}
```

---

## Command History (Undo/Redo)

**Issue:** No generic undo/redo system based on the Command pattern.

**Working Implementation (wordproc):**
- `src/extracted/command_history.h`
- `src/editor/text_buffer.h`

**Suggested Implementation:**
```cpp
namespace afterhours {
  class Command {
   public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const { return "Command"; }
  };

  class CommandHistory {
   public:
    void execute(std::unique_ptr<Command> cmd);
    void undo();
    void redo();
    bool can_undo() const;
    bool can_redo() const;
  };
}
```

---

---

## Test Input Hooks

**Status:** Not implemented

**Issue:** No first-class way to inject input events for automated tests.

**Working Implementation (wordproc):**
- `src/testing/test_input.h/.cpp` (high-level input queue)
- `src/testing/input_injector.h/.cpp` (low-level raylib injection)
- `src/testing/test_input_provider.h` (UIContext integration)
- `src/external.h` (macro wrappers)

**Suggested Implementation:**
```cpp
namespace afterhours::ui::test {
  void push_key(int keycode);
  void push_char(char32_t ch);
  void set_mouse_position(float x, float y);
  void press_mouse_button(int button);  // one-frame press
  void hold_mouse_button(int button);
  void release_mouse_button(int button);
  void scroll_wheel(float delta);
  void advance_frame();
  void clear_all();
}

// Optional: provider interface for per-context input
struct InputProvider {
  virtual ~InputProvider() = default;
  virtual Vector2 get_mouse_position() = 0;
  virtual bool is_mouse_button_pressed(int button) = 0;
  virtual bool is_mouse_button_down(int button) = 0;
  virtual bool is_key_pressed(int key) = 0;
  virtual int get_char_pressed() = 0;
};
```

**Notes:**
- Pressed input must be visible for exactly one frame (matches hardware behavior).
- Avoid macro overrides by letting `UIContext` consume a configurable provider.

---

## E2E Testing Framework

**Issue:** No built-in end-to-end UI testing or scripting framework.

**Working Implementation (wordproc):**
- `src/testing/e2e_script.h` (DSL + runner)
- `src/testing/e2e_runner.h/.cpp`
- `src/extracted/e2e_testing.h` (single-file)

**Suggested Implementation:**
```cpp
namespace afterhours::testing {
  class E2ERunner {
   public:
    void load_script(const std::string &path);
    void load_scripts_from_directory(const std::string &dir);
    void set_timeout_frames(int frames);
    void tick();
    bool is_finished() const;
    bool has_failed() const;
  };

  namespace visible_text {
    void register_text(const std::string &text);
    bool contains(const std::string &text);
  }
}
```

---

## Icon Registry / Icon Management

**Issue:** No centralized icon registry for mapping actions/items to icons with fallbacks.

**Working Implementation (wordproc):**
- `src/ui/icon_registry.h`
- `src/extracted/icon_registry.h`

**Suggested Implementation:**
```cpp
namespace afterhours::ui {
  struct IconInfo {
    std::string name;
    std::string resource_path;
    char fallback_symbol = '?';
    bool is_mirrored = false;
  };

  class IconRegistry {
   public:
    void register_icon(const std::string &id, IconInfo info);
    std::optional<IconInfo> get(const std::string &id) const;
    char get_symbol(const std::string &id) const;
  };
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

## 3D/Puffy Text Effect

**Issue:** Cannot create 3D "puffy" text effects with depth/volume.

**Suggested Implementation:**
```cpp
ComponentConfig{}
    .with_puffy_text(outline_color, highlight_color, shadow_color, depth);
```

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

## Small (~half day)
- Toggle Switch
- Stepper/Selector
- Radio Group
- Button Variants
- Corner Brackets
- Currency Display
- Beveled / 3D Borders
- Icon Registry
- Status Notifications / Toasts

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
- Action Binding System
- Command History (Undo/Redo)

## Large (~3-5 days)
- Focus Ring Styles
- Tab Bar
- Tree Node
- Modal/Dialog
- Context Menu
- CSS Grid Layout
- Render System Const Constraint
- Renderer Abstraction Layer
- E2E Testing Framework

## Very Large (~1+ week)
- Text Input (full with selection/clipboard)
- Text Area / Text Editor
- Scroll Container
- List Box
- Table
- Drag and Drop
- Color Picker
- Classic UI Themes
