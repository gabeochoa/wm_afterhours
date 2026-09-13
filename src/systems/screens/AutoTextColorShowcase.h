#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the auto_text_color feature which is now enabled by default.
struct AutoTextColorShowcase : ScreenSystem<UIContext<InputAction>> {
  enum class TextMode { Auto, ThemeFont, ExplicitRed, LightReference };
  std::string selected_name = "Near Black";
  std::string selected_debug = "dark_btn_0";
  afterhours::Color selected_background{20, 20, 30, 255};
  afterhours::Color selected_foreground{255, 255, 255, 255};

  static std::string hex(afterhours::Color color) {
    return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
  }

  static bool same_rgb(afterhours::Color a, afterhours::Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
  }

  static std::string foreground_name(afterhours::Color color, const Theme &theme) {
    if (same_rgb(color, theme.font)) return "Light";
    if (same_rgb(color, theme.darkfont)) return "Dark";
    return same_rgb(color, afterhours::colors::UI_WHITE) ? "White fallback" : "Black fallback";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Use ocean_navy theme preset (dark background theme)
    const auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    if (s <= 0.f) return;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const afterhours::Color ink{236, 242, 251, 255};
    const afterhours::Color muted{182, 197, 219, 255};
    const afterhours::Color border{80, 100, 129, 255};
    const auto box = [s, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position((left + x) * s, (top + y) * s)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };

    // Full screen background (no padding so it stays in bounds)
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0).with_debug_name("root_bg"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(x, y, w, h).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto specimen = [&](const std::string &name, afterhours::Color background,
                              float x, float y, float width, const std::string &debug,
                              TextMode mode = TextMode::Auto, Theme::Usage role = Theme::Usage::Custom) {
      const auto foreground = mode == TextMode::ExplicitRed ? afterhours::Color{140, 30, 30, 255}
          : mode == TextMode::ThemeFont || mode == TextMode::LightReference ? theme.font
          : afterhours::colors::auto_text_color(background, theme.font, theme.darkfont);
      auto config = box(x + 2, y + 2, width - 4, 30).with_label(name)
          .with_font("AtkinsonMock", pixels(21 * s)).with_alignment(TextAlignment::Center)
          .with_corner_radius(0).with_custom_hover_bg(background)
          .with_click_activation(ClickActivationMode::Release).with_debug_name(debug);
      if (role == Theme::Usage::Custom) config.with_custom_background(background);
      else config.with_background(role);
      if (mode == TextMode::ThemeFont) config.with_auto_text_color(false);
      if (mode == TextMode::ExplicitRed || mode == TextMode::LightReference)
        config.with_custom_text_color(foreground);
      div(context, mk(root.ent(), id++), box(x, y, width, 34).with_ignore_pointer_events()
          .with_on_draw_fg([s, border, picked = selected_debug == debug](RectangleType r) {
            raylib::DrawRectangleLinesEx(r, s, picked ? afterhours::Color{241, 202, 99, 255} : border);
          }));
      if (button(context, mk(root.ent(), id++), config)) {
        selected_name = name;
        selected_debug = debug;
        selected_background = background;
        selected_foreground = foreground;
      }
      const auto kind = mode == TextMode::ExplicitRed ? "Red override"
          : mode == TextMode::ThemeFont ? "Theme light"
          : mode == TextMode::LightReference ? "Light ref"
          : foreground_name(foreground, theme);
      label(fmt::format("{} / {} / {:.2f}:1", hex(background), kind,
                        afterhours::colors::contrast_ratio(foreground, background)),
            x, y + 36, width, 22, 17, muted, debug + "_result");
    };

    div(context, mk(root.ent(), id++), box(0, 12, 1144, 84)
        .with_custom_background({25, 45, 72, 255}).with_corner_radius(12 * s));
    label("Auto text color", 20, 21, 600, 39, 33, ink);
    label("Default enabled", 906, 28, 218, 28, 21, muted);
    label(fmt::format("Chooses theme light/dark text; if both are below 4.5:1, tries {} and {}.",
                      hex(afterhours::colors::UI_WHITE), hex(afterhours::colors::UI_BLACK)),
          20, 62, 1104, 26, 20, muted);

    // Section 1: Default behavior (auto enabled by default)
    label("Default behavior", 0, 106, 440, 27, 23, ink);
    label("auto_text_color = true", 674, 106, 470, 27, 20, muted, "auto_default_flag", true);
    const std::array<const char *, 4> families{"Neutral pair", "Blue pair", "Purple / pink pair", "Green pair"};
    for (size_t i = 0; i < families.size(); ++i)
      label(families[i], 100 + static_cast<float>(i) * 261, 135, 249, 21, 18, muted);
    label("Dark", 0, 160, 94, 25, 20, ink);
    label("backgrounds", 0, 186, 94, 23, 17, muted);
    label("Light", 0, 222, 94, 25, 20, ink);
    label("backgrounds", 0, 248, 94, 23, 17, muted);

    // Dark backgrounds
    const std::array<afterhours::Color, 4> dark_colors{{
        {20, 20, 30, 255}, // Near black
        {40, 60, 80, 255}, // Dark blue
        {60, 40, 60, 255}, // Dark purple
        {30, 50, 40, 255}, // Dark green
    }};
    const std::array<const char *, 4> dark_labels{"Near Black", "Dark Blue", "Dark Purple", "Dark Green"};
    const std::array<afterhours::Color, 4> light_colors{{
        {240, 240, 245, 255}, // Near white
        {200, 220, 255, 255}, // Light blue
        {255, 220, 240, 255}, // Light pink
        {220, 255, 220, 255}, // Light green
    }};
    const std::array<const char *, 4> light_labels{"Near White", "Light Blue", "Light Pink", "Light Green"};
    for (size_t i = 0; i < dark_colors.size(); ++i) {
      const float x = 100 + static_cast<float>(i) * 261;
      specimen(dark_labels[i], dark_colors[i], x, 158, 249, "dark_btn_" + std::to_string(i));
      specimen(light_labels[i], light_colors[i], x, 220, 249, "light_btn_" + std::to_string(i));
    }

    // Section 2: Mid-tone colors (edge cases)
    label("Mid tones / increasing luminance", 0, 285, 550, 27, 23, ink);
    specimen("Gray / light reference", {150, 150, 150, 255}, 672, 280, 240, "gray_light_reference", TextMode::LightReference);
    label("Below-AA reference", 926, 285, 218, 25, 18, muted);
    const std::array<afterhours::Color, 6> mid_colors{{
        {255, 180, 100, 255}, // Orange
        {100, 180, 180, 255}, // Teal
        {140, 60, 140, 255},  // Dark Magenta (better contrast)
        {180, 180, 100, 255}, // Olive
        {150, 150, 150, 255}, // Gray
        {200, 150, 100, 255}, // Tan
    }};
    const std::array<const char *, 6> mid_labels{"Orange", "Teal", "Dark magenta", "Olive", "Gray", "Tan"};
    std::array<size_t, 6> order{0, 1, 2, 3, 4, 5};
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
      return afterhours::colors::luminance(mid_colors[a]) < afterhours::colors::luminance(mid_colors[b]);
    });
    for (size_t column = 0; column < order.size(); ++column) {
      const size_t i = order[column];
      const float x = static_cast<float>(column) * 193;
      specimen(mid_labels[i], mid_colors[i], x, 338, 179, "mid_btn_" + std::to_string(i));
      label(fmt::format("Luminance {:.3f}", afterhours::colors::luminance(mid_colors[i])),
            x, 396, 179, 20, 16, muted, "mid_luminance_" + std::to_string(i));
    }

    // Section 3: How to disable auto text color
    label("Configuration examples", 0, 424, 408, 27, 23, ink);
    label("Original cases above; same light background below", 420, 426, 724, 25, 19, muted);
    // Compare auto vs disabled vs explicit text color
    const afterhours::Color light_bg{230, 230, 230, 255};
    const afterhours::Color dark_bg{50, 50, 60, 255};
    specimen("Auto (default)", light_bg, 0, 457, 368, "auto_config_default");
    specimen("Auto off: theme font", dark_bg, 388, 457, 368, "auto_config_off", TextMode::ThemeFont);
    specimen("Explicit red override", light_bg, 776, 457, 368, "auto_config_red", TextMode::ExplicitRed);
    specimen("Matched: auto", light_bg, 0, 519, 368, "auto_matched_default");
    specimen("Matched: auto off", light_bg, 388, 519, 368, "auto_matched_off", TextMode::ThemeFont);
    specimen("Matched: red override", light_bg, 776, 519, 368, "auto_matched_red", TextMode::ExplicitRed);

    // Section 4: Theme colors with auto contrast
    label("Theme role examples / Ocean Navy", 0, 583, 710, 27, 23, ink);
    label("Select a tile; outlines mark bounds", 730, 585, 414, 24, 19, muted);
    const std::array<Theme::Usage, 5> roles{Theme::Usage::Primary, Theme::Usage::Secondary,
        Theme::Usage::Accent, Theme::Usage::Background, Theme::Usage::Surface};
    const std::array<const char *, 5> role_names{"Primary", "Secondary", "Accent", "Background", "Surface"};
    for (size_t i = 0; i < roles.size(); ++i)
      specimen(role_names[i], theme.from_usage(roles[i]), static_cast<float>(i) * 231, 616, 220,
               "auto_role_" + std::to_string(i), TextMode::Auto, roles[i]);

    for (float y : {279.f, 419.f, 578.f})
      div(context, mk(root.ent(), id++), box(0, y, 1144, 1)
          .with_custom_background(border).with_ignore_pointer_events());
    label(fmt::format("Selected: {} | bg {} | text {} | Y {:.3f} | {:.2f}:1",
                      selected_name, hex(selected_background), hex(selected_foreground),
                      afterhours::colors::luminance(selected_background),
                      afterhours::colors::contrast_ratio(selected_foreground, selected_background)),
          0, 684, 1144, 27, 18, ink, "auto_inspector", true);
  }
};

REGISTER_EXAMPLE_SCREEN(auto_text_color, "System Demos",
                        "Automatic text contrast, theme candidates, and explicit overrides",
                        AutoTextColorShowcase)
