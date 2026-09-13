#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ThemesScreen : ScreenSystem<UIContext<InputAction>> {
  enum struct ThemeChoice {
    CozyKraft,
    NeonDark,
    OceanNavy,
    Midnight,
    SageNatural
  };

  ThemeChoice current_theme = ThemeChoice::OceanNavy;
  float slider_value = 0.5f;
  bool checkbox_state = true;
  bool toggle_state = false;

  Theme get_theme_for_choice(ThemeChoice choice) {
    switch (choice) {
    case ThemeChoice::CozyKraft:
      return theme_presets::cozy_kraft();
    case ThemeChoice::NeonDark:
      return theme_presets::neon_dark();
    case ThemeChoice::OceanNavy:
      return theme_presets::ocean_navy();
    case ThemeChoice::Midnight:
      return theme_presets::midnight();
    case ThemeChoice::SageNatural:
      return theme_presets::sage_natural();
    default:
      return theme_presets::ocean_navy();
    }
  }

  std::string get_theme_name(ThemeChoice choice) {
    switch (choice) {
    case ThemeChoice::CozyKraft:
      return "Cozy Kraft";
    case ThemeChoice::NeonDark:
      return "Neon Dark";
    case ThemeChoice::OceanNavy:
      return "Ocean Navy";
    case ThemeChoice::Midnight:
      return "Midnight";
    case ThemeChoice::SageNatural:
      return "Sage Natural";
    default:
      return "Unknown";
    }
  }

  std::string last_action = "Choose a specimen";
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = get_theme_for_choice(current_theme);
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    auto at = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto background = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_debug_name("main_bg"));
    auto root = div(context, mk(background.ent(), 0), at(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2, (context.screen_height - 720 * s) / 2));
    const auto label = [&](int id, const std::string &caption, float x, float y,
                           float w, float h, float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), at(x, y, w, h).with_label(caption)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(theme.font)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto hex = [](afterhours::Color color) { return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b); };
    // ========== HEADER ==========
    label(0, "Theme Switcher", 48, 22, 600, 44, 34);
    label(1, "Switching themes recolors every specimen below.", 48, 77, 900, 30, 21);
    label(2, "Active: " + get_theme_name(current_theme), 740, 30, 492, 32, 22, "theme_active");
    // ========== MAIN CONTENT ==========
    // LEFT - Theme Selection
    div(context, mk(root.ent(), 3), at(48, 128, 264, 558)
        .with_background(Theme::Usage::Surface).with_corner_radius(10 * s).with_debug_name("selector_panel"));
    label(4, "Select Theme", 64, 145, 232, 30, 24);
    // Theme buttons
    const std::array<ThemeChoice, 5> choices{ThemeChoice::CozyKraft, ThemeChoice::NeonDark,
        ThemeChoice::OceanNavy, ThemeChoice::Midnight, ThemeChoice::SageNatural};
    for (size_t i = 0; i < choices.size(); ++i) {
      const bool selected = choices[i] == current_theme;
      auto option = button(context, mk(root.ent(), 10 + static_cast<int>(i)),
          at(64, 188 + static_cast<float>(i) * 55, 232, 42).with_label(get_theme_name(choices[i]))
              .with_font("AtkinsonMock", pixels(21 * s)).with_alignment(TextAlignment::Left)
              .with_background(selected ? Theme::Usage::Accent : Theme::Usage::Secondary)
              .with_auto_text_color(true).with_corner_radius(5 * s)
              .with_debug_name("theme_btn_" + std::to_string(i + 1)));
      option.ent().get<HasLabel>().text_x_offset = 14 * s;
      if (selected)
        div(context, mk(root.ent(), 20 + static_cast<int>(i)), at(64, 188 + static_cast<float>(i) * 55, 4, 42)
            .with_custom_background(theme.font).with_ignore_pointer_events());
      if (option) current_theme = choices[i];
    }
    label(30, "Resolved color tokens", 64, 478, 232, 26, 20);
    const std::array<const char *, 7> token_names{"Background", "Surface", "Primary", "Secondary", "Accent", "Font", "Dark font"};
    const std::array<afterhours::Color, 7> colors{theme.background, theme.surface, theme.primary,
        theme.secondary, theme.accent, theme.font, theme.darkfont};
    for (size_t i = 0; i < colors.size(); ++i)
      label(31 + static_cast<int>(i), std::string(token_names[i]) + "  " + hex(colors[i]),
          64, 511 + static_cast<float>(i) * 23, 232, 22, 16);
    // RIGHT - Component Preview
    div(context, mk(root.ent(), 50), at(336, 128, 896, 558)
        .with_background(Theme::Usage::Surface).with_corner_radius(10 * s).with_debug_name("preview_panel"));
    label(51, "Component Preview / " + get_theme_name(current_theme), 356, 143, 856, 36, 27);
    label(52, last_action, 356, 184, 856, 26, 18, "theme_feedback");
    // Buttons row
    const std::array<Theme::Usage, 4> roles{Theme::Usage::Primary, Theme::Usage::Secondary, Theme::Usage::Accent, Theme::Usage::Primary};
    const std::array<const char *, 4> names{"Primary", "Secondary", "Accent", "Disabled"};
    for (size_t i = 0; i < names.size(); ++i) {
      if (button(context, mk(root.ent(), 60 + static_cast<int>(i)),
          at(356 + static_cast<float>(i) * 216, 221, 200, 40).with_label(names[i])
              .with_font("AtkinsonMock", pixels(20 * s)).with_background(roles[i])
              .with_auto_text_color(true).with_disabled(i == 3).with_corner_radius(6 * s)
              .with_debug_name("theme_sample_" + std::to_string(i)))) last_action = std::string(names[i]) + " button activated";
    }
    label(65, "Disabled state / no action", 1004, 265, 208, 22, 15);
    // Slider
    label(70, "Slider / " + std::to_string(static_cast<int>(std::round(slider_value * 100))) + "%",
          356, 282, 856, 27, 21, "theme_slider_value");
    slider(context, mk(root.ent(), 71), slider_value,
        at(356, 316, 856, 32).with_background(Theme::Usage::Primary).with_debug_name("preview_slider"),
        SliderHandleValueLabelPosition::None);
    label(72, fmt::format("Native {:g} px track height / handle is 25% of the track width", 32 * s), 356, 355, 856, 23, 16);
    // Checkbox
    label(80, "Checkbox", 356, 389, 190, 32, 21);
    checkbox(context, mk(root.ent(), 81), checkbox_state,
        at(566, 389, 36, 32).with_checkbox_indicators("", "").with_custom_background(theme.primary)
            .with_border(theme.font, s).with_corner_radius(0).with_debug_name("theme_checkbox"));
    if (checkbox_state)
      div(context, mk(root.ent(), 82), at(566, 389, 36, 32).with_ignore_pointer_events()
          .with_on_draw_fg([theme, s](RectangleType r) {
            afterhours::draw_line_ex({r.x + 8 * s, r.y + 16 * s}, {r.x + 15 * s, r.y + 23 * s}, 3 * s, theme.font);
            afterhours::draw_line_ex({r.x + 15 * s, r.y + 23 * s}, {r.x + 28 * s, r.y + 9 * s}, 3 * s, theme.font);
          }));
    label(83, checkbox_state ? "On" : "Off", 614, 389, 84, 32, 20, "theme_checkbox_state");
    // Toggle switch - increased height to fit toggle_track
    label(84, "Toggle", 792, 389, 160, 32, 21);
    label(85, toggle_state ? "On" : "Off", 974, 389, 70, 32, 20, "theme_toggle_state");
    auto toggle_config = at(1108, 385, 104, 40).with_debug_name("theme_toggle");
    toggle_config.corner_radius.reset();
    toggle_switch(context, mk(root.ent(), 86), toggle_state, toggle_config);
    // Cards with shadows demo
    label(90, "Shadow comparison / same Primary background", 356, 431, 856, 24, 18);
    const std::string offset = fmt::format("{:g}", 2 * s);
    const std::array<std::string, 3> shadow_names{"Hard / offset " + offset + "," + offset,
        fmt::format("Soft / blur {:g}", 6 * s), fmt::format("Soft / blur {:g}", 5 * s)};
    for (int i = 0; i < 3; ++i) {
      auto config = at(356 + static_cast<float>(i) * 288, 466, 268, 44)
          .with_label(shadow_names[static_cast<size_t>(i)]).with_background(Theme::Usage::Primary)
          .with_auto_text_color(true).with_font("AtkinsonMock", pixels(20 * s)).with_corner_radius(6 * s);
      if (i == 0) config.with_hard_shadow(2 * s, 2 * s);
      if (i == 1) config.with_soft_shadow(2 * s, 3 * s, 6 * s);
      if (i == 2) config.with_soft_shadow(2 * s, 2 * s, 5 * s);
      div(context, mk(root.ent(), 91 + i), config).ent().get<HasLabel>().text_x_offset = 12 * s;
    }
    // Text display row for theme colors
    const std::array<afterhours::Color, 3> backgrounds{theme.primary, theme.secondary, theme.surface};
    const std::array<const char *, 3> swatches{"Primary Text", "Secondary Text", "Surface Text"};
    for (size_t i = 0; i < backgrounds.size(); ++i) {
      const auto foreground = afterhours::colors::auto_text_color(backgrounds[i], theme.font, theme.darkfont);
      const float x = 356 + static_cast<float>(i) * 288;
      auto sample = div(context, mk(root.ent(), 100 + static_cast<int>(i)), at(x, 532, 268, 34)
          .with_label(swatches[i]).with_custom_background(backgrounds[i]).with_custom_text_color(foreground)
          .with_border({127, 139, 151, 255}, s).with_font("AtkinsonMock", pixels(20 * s)));
      sample.ent().get<HasLabel>().text_x_offset = 12 * s;
      label(110 + static_cast<int>(i), "FG " + hex(foreground) + " / BG " + hex(backgrounds[i]),
          x, 571, 268, 23, 15);
    }
    // Labeled separator
    label(120, "Separator / Secondary " + hex(theme.secondary), 356, 602, 856, 24, 17);
    auto line = separator(context, mk(root.ent(), 121), SeparatorOrientation::Horizontal,
        at(356, 631, 856, 2).with_background(Theme::Usage::Secondary).with_debug_name("theme_separator"));
    for (auto edge : {Axis::top, Axis::bottom}) line.cmp().desired_margin[edge] = pixels(0);
    // Progress bar
    label(122, "Loading: 72% / fill Primary / track Secondary", 356, 639, 856, 25, 18);
    progress_bar(context, mk(root.ent(), 123), .72f,
        at(356, 668, 856, 12).with_background(Theme::Usage::Primary).with_debug_name("theme_progress"),
        ProgressBarLabelStyle::None);
  }
};

REGISTER_EXAMPLE_SCREEN(themes, "Tools", "Real-time theme switching demo",
                        ThemesScreen)
