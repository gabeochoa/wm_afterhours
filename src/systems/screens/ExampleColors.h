#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleColors : ScreenSystem<UIContext<InputAction>> {
  size_t selected = 0;
  int activations = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply midnight theme for this screen
    const auto theme = afterhours::ui::theme_presets::midnight();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const auto white = afterhours::Color{240, 244, 250, 255};
    const auto muted = afterhours::Color{185, 197, 215, 255};
    const auto neutral = afterhours::Color{30, 35, 44, 255};
    const auto outline = afterhours::Color{107, 122, 145, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    const auto hex = [](afterhours::Color color) {
      return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    };
    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bg"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    // Main panel
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 674)
        .with_custom_background(theme.background).with_debug_name("main_panel"));
    // Title
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 90)
        .with_custom_background(neutral).with_corner_radius(12 * s));
    label("Theme Color Swatches", 20, 34, 834, 42, 33, white);
    label("Midnight", 902, 39, 222, 34, 25, white, "colors_theme");
    label("Select a swatch for its value, foreground, contrast, and semantic purpose.",
          20, 79, 1104, 26, 20, muted);

    struct ThemeSwatch {
      const char *label;
      Theme::Usage usage;
      const char *purpose;
      int debug_index;
    };
    const std::array<ThemeSwatch, 6> theme_swatches{{
        {"Primary", Theme::Usage::Primary, "Main action", 0},
        {"Secondary", Theme::Usage::Secondary, "Supporting action", 1},
        {"Accent", Theme::Usage::Accent, "Emphasis and selection", 2},
        {"Surface", Theme::Usage::Surface, "Panels and cards", 3},
        {"Background", Theme::Usage::Background, "Page background", 5},
        {"Error", Theme::Usage::Error, "Error status and destructive actions", 4},
    }};
    // Row 1 label
    label("Semantic roles", 20, 126, 1104, 31, 25, white);
    label("Actions", 20, 159, 546, 23, 18, muted);
    label("Surfaces", 578, 159, 360, 23, 18, muted);
    label("Status", 950, 159, 174, 23, 18, muted);
    // Color row 1: Theme colors background
    div(context, mk(root.ent(), id++), box(left, 185, 1144, 189)
        .with_custom_background({19, 23, 30, 255}).with_corner_radius(10 * s).with_debug_name("theme_tray"));
    for (size_t i = 0; i < theme_swatches.size(); ++i) {
      const auto &swatch = theme_swatches[i];
      const auto color = theme.from_usage(swatch.usage);
      const auto foreground = afterhours::colors::auto_text_color(color, theme.font, theme.darkfont);
      const float x = 20 + static_cast<float>(i) * 186;
      div(context, mk(root.ent(), id++), box(left + x - 3, 194, 180, 84)
          .with_on_draw_fg([s, selected = selected == i, outline](RectangleType r) {
            afterhours::draw_rectangle_rounded_lines_ex(r, .12f, 12, (selected ? 2.f : 1.f) * s,
                selected ? afterhours::Color{221, 233, 252, 255} : outline);
          }).with_ignore_pointer_events());
      if (button(context, mk(root.ent(), id++), box(left + x, 197, 174, 78)
          .with_label(swatch.label).with_font("AtkinsonMock", pixels(22 * s))
          .with_background(swatch.usage).with_custom_hover_bg(color).with_auto_text_color(true)
          .with_corner_radius(7 * s).with_alignment(TextAlignment::Center)
          .with_debug_name("swatch_" + std::to_string(swatch.debug_index)))) {
        selected = i;
        ++activations;
      }
      label(hex(color), x, 282, 174, 27, 19, white, "role_hex_" + std::to_string(i), true);
      label("Text " + hex(foreground), x, 313, 174, 24, 17, muted);
      label(fmt::format("Contrast {:.2f}:1", afterhours::colors::contrast_ratio(foreground, color)),
            x, 342, 174, 23, 17, muted, "role_contrast_" + std::to_string(i));
    }

    // Row 2 label
    label("Fixed custom values", 20, 390, 1104, 31, 25, white);
    // Color row 2: Custom colors background
    div(context, mk(root.ent(), id++), box(left, 429, 1144, 161)
        .with_custom_background({19, 23, 30, 255}).with_corner_radius(10 * s).with_debug_name("custom_tray"));
    // Custom color swatches with hex values
    struct CustomColorInfo {
      const char *name;
      afterhours::Color color;
    };
    const std::array<CustomColorInfo, 5> custom_colors{{
        {"Coral pink", {255, 100, 100, 255}},
        {"Green", {100, 200, 100, 255}},
        {"Blue", {100, 150, 255, 255}},
        {"Amber", {255, 200, 100, 255}},
        {"Purple", {200, 100, 255, 255}},
    }};
    for (size_t i = 0; i < custom_colors.size(); ++i) {
      const auto &swatch = custom_colors[i];
      const auto foreground = afterhours::colors::auto_text_color(swatch.color, theme.font, theme.darkfont);
      const float x = 20 + static_cast<float>(i) * 224;
      div(context, mk(root.ent(), id++), box(left + x - 3, 438, 214, 66)
          .with_on_draw_fg([s, selected = selected == i + 6, outline](RectangleType r) {
            afterhours::draw_rectangle_rounded_lines_ex(r, .12f, 12, (selected ? 2.f : 1.f) * s,
                selected ? afterhours::Color{221, 233, 252, 255} : outline);
          }).with_ignore_pointer_events());
      if (button(context, mk(root.ent(), id++), box(left + x, 441, 208, 60)
          .with_label(swatch.name).with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background(swatch.color).with_custom_hover_bg(swatch.color).with_auto_text_color(true)
          .with_corner_radius(7 * s).with_alignment(TextAlignment::Center).with_debug_name("custom_" + std::to_string(i)))) {
        selected = i + 6;
        ++activations;
      }
      label(hex(swatch.color), x, 510, 208, 27, 19, white, "custom_hex_" + std::to_string(i), true);
      label(fmt::format("Text {} / {:.2f}:1", hex(foreground), afterhours::colors::contrast_ratio(foreground, swatch.color)),
            x, 540, 208, 24, 16, muted, "custom_contrast_" + std::to_string(i));
    }
    label("Coral pink = Midnight Error (#FF6464). Primary is the darker action red (#B91C1C).",
          20, 565, 1104, 23, 18, muted);

    // Info text at bottom
    const bool semantic = selected < theme_swatches.size();
    const auto chosen = semantic ? theme.from_usage(theme_swatches[selected].usage) : custom_colors[selected - 6].color;
    const auto foreground = afterhours::colors::auto_text_color(chosen, theme.font, theme.darkfont);
    const std::string chosen_name = semantic ? theme_swatches[selected].label : custom_colors[selected - 6].name;
    label("Selected: " + chosen_name + " / " + hex(chosen), 20, 604, 638, 30, 23, white, "colors_selected");
    label(semantic ? theme_swatches[selected].purpose : "Fixed RGB value, independent of semantic roles",
          20, 638, 638, 25, 19, muted, "colors_purpose");
    label(fmt::format("RGB {}, {}, {} / text {} / {:.2f}:1 / {} activations",
                      chosen.r, chosen.g, chosen.b, hex(foreground), afterhours::colors::contrast_ratio(foreground, chosen), activations),
          20, 671, 1104, 25, 17, muted, "colors_metadata");
    div(context, mk(root.ent(), id++), box(left + 702, 604, 422, 57)
        .with_custom_background({240, 242, 247, 255}).with_corner_radius(6 * s).with_debug_name("colors_light_backing"));
    label("Same selected color", 718, 618, 254, 28, 18, {27, 35, 48, 255});
    div(context, mk(root.ent(), id++), box(left + 994, 613, 112, 39)
        .with_custom_background(chosen).with_border({112, 124, 143, 255}, s)
        .with_corner_radius(4 * s).with_debug_name("colors_light_sample"));
  }
};

REGISTER_EXAMPLE_SCREEN(colors, "System Demos", "Color system examples",
                        ExampleColors)
