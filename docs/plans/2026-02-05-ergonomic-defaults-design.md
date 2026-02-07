# Ergonomic Defaults Design

**Goal:** Components look great by default with minimal configuration. Reduce boilerplate across all screens.

## Part 1: Library-Level Defaults (ComponentConfig)

Changes to `vendor/afterhours/src/plugins/ui/component_config.h`:

```cpp
struct ComponentConfig {
  // NEW DEFAULTS
  bool auto_text_color = true;          // was: false
  float roundness = 0.08f;              // was: 0.0f
};
```

### Button-Specific Defaults

- Minimum height: 44px (touch target accessibility)
- Default size: `{pixels(120), pixels(44)}`
- Default padding: `Spacing::sm`

### Before/After

```cpp
// BEFORE: Every button needs this
button(context, mk(entity, 0),
    ComponentConfig{}
        .with_label("Play")
        .with_size(ComponentSize{pixels(120), pixels(45)})
        .with_auto_text_color(true)
        .with_roundness(0.08f)
        .with_margin(Spacing::xs));

// AFTER: Just this
button(context, mk(entity), ComponentConfig{}.with_label("Play"));
```

---

## Part 2: Auto-Incrementing Entity IDs

Changes to `vendor/afterhours/src/plugins/ui/entity_management.h`:

```cpp
static std::map<std::string, int> location_counters;

static EntityParent mk(Entity &parent,
    const std::source_location location = std::source_location::current()) {

  std::string loc_key = std::string(location.file_name()) + ":" +
                        std::to_string(location.line()) + ":" +
                        std::to_string(location.column());

  int idx = location_counters[loc_key]++;

  std::stringstream pre_hash;
  pre_hash << parent.id << idx << "file: " << location.file_name() << '('
           << location.line() << ':' << location.column() << ") `"
           << location.function_name() << "`: " << '\n';

  // ... rest stays the same
}

// Called at frame start by the UI system
static void reset_mk_counters() {
  location_counters.clear();
}
```

### Before/After

```cpp
// BEFORE: Manual index tracking
for (size_t i = 0; i < buttons.size(); i++) {
  button(context, mk(row.ent(), static_cast<int>(i)), config);
}

// AFTER: Just works
for (const auto& btn : buttons) {
  button(context, mk(row.ent()), config);
}
```

---

## Part 3: Theme Builder with Palette

### FontSizing Struct

```cpp
struct FontSizing {
  float small  = -1.0f;  // -1 = not set, will interpolate
  float medium = -1.0f;
  float large  = -1.0f;
  float xl     = -1.0f;

  // Returns interpolated value if tier not explicitly set
  float get(Tier tier) const;
};
```

**Auto-interpolation:** If you set `small = 14` and `large = 32`:
- `medium` = 23 (lerped)
- `xl` = 41 (extrapolated)

### Theme::create() Builder

```cpp
struct ThemePalette {
  // All optional - missing ones auto-generated
  std::optional<Color> background;
  std::optional<Color> surface;
  std::optional<Color> primary;
  std::optional<Color> secondary;
  std::optional<Color> accent;
  std::optional<Color> error;
  std::optional<Color> font;
  std::optional<Color> darkfont;
  std::optional<Color> font_muted;
};

// Usage:
auto theme = Theme::create()
  .with_palette({
    .background = {25, 45, 75},
    .surface = {35, 65, 105},
    .primary = {85, 145, 215},
    .accent = {145, 215, 245}
    // Auto-generates: secondary, error, font, darkfont, font_muted
  })
  .with_font("EqProRounded")
  .with_font_sizing({.small = 14, .large = 32})
  .with_roundness(0.08f);
```

### Auto-Generated Colors

From the provided palette colors:
- `font` - white or dark based on background contrast
- `darkfont` - opposite of font
- `font_muted` - font color at 60% opacity
- `secondary` - surface lightened 20%
- `error` - default red `{180, 80, 80}` unless overridden

---

## Part 4: Component Font Defaults

Components use theme font by default:
- Font family: `context.theme.font_family`
- Font size: `context.theme.font_sizing.get(Medium)`

All components default to `medium` font size.

---

## Implementation Order

1. **ComponentConfig defaults** - Change default values for `auto_text_color`, `roundness`
2. **Button defaults** - Add default sizing/padding for buttons
3. **mk() auto-increment** - Update entity_management.h, add reset call to frame loop
4. **FontSizing struct** - Add to Theme with interpolation logic
5. **Theme::create() builder** - Add palette support with auto-generation
6. **Update theme presets** - Migrate existing presets to new system
7. **Update example screens** - Remove now-redundant config (optional, can be gradual)

---

## Not Changing

- Spacing system (`Spacing::sm`, `Spacing::md`, etc.) - works fine as-is
- Debug names - no good C++ reflection alternative
- Semantic components (like `title()`, `section_header()`) - not wanted
