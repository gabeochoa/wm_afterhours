#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Modern blueprint/technical drawing aesthetic
  afterhours::Color bg_deep{24, 32, 48, 255};         // Deep blue-gray
  afterhours::Color panel_dark{32, 42, 62, 255};      // Panel blue
  afterhours::Color grid_line{45, 58, 85, 255};       // Grid lines
  afterhours::Color accent_blue{80, 145, 220, 255};   // Blueprint blue
  afterhours::Color accent_cyan{70, 190, 200, 255};   // Cyan highlight
  afterhours::Color accent_orange{235, 145, 70, 255}; // Orange accent
  afterhours::Color text_light{220, 225, 235, 255};   // Light text
  afterhours::Color text_muted{120, 135, 160, 255};   // Muted text
  afterhours::Color sample_white{250, 250, 252, 255}; // Sample card white

  int page = 0;
  int clicks = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_deep;
    theme.font_muted = {171, 187, 212, 255};
    theme.background = bg_deep;
    theme.surface = panel_dark;
    theme.primary = accent_blue;
    theme.secondary = accent_cyan;
    theme.accent = accent_orange;
    theme.error = {200, 80, 80, 255};
    theme.roundness = 0.08f;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1160 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None)
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    const auto fraction = [](ComponentConfig config, float value, std::bitset<4> corners = std::bitset<4>(15)) {
      config.corner_radius.reset();
      return config.with_roundness(value).with_rounded_corners(corners);
    };
    // Background - covers entire screen
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg_deep).with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1160, 720)
        .with_absolute_position(left, top).with_debug_name("borders_root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size = 18.f,
                           bool strong = false, const std::string &name = "") {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(strong ? text_light : theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    label(root.ent(), 1, "Border styles", 0, 18, 1160, 44, 34, true);
    // Legend - explanation of border types
    label(root.ent(), 2, "Compare width and color separately. Other samples combine corners, shadows and edge styles.",
          0, 67, 1160, 27, 20);
    for (int i = 0; i < 3; ++i) {
      if (!button(context, mk(root.ent(), 3 + i), box(i * 378.f, 106, 362, 38)
          .with_label(i == 0 ? "Width, corners and shadows" : i == 1 ? "Themes and interaction" : "Per-side and dotted borders")
          .with_font("AtkinsonMock", pixels(20 * s)).with_alignment(TextAlignment::Center)
          .with_custom_background(page == i ? accent_blue : panel_dark)
          .with_custom_text_color(page == i ? bg_deep : text_light).with_corner_radius(8 * s)
          .with_debug_name("borders_page_" + std::to_string(i)))) continue;
      page = i;
    }
    label(root.ent(), 5, "Dimensions and radii are shown at 720p; the gallery scales to fit the window.",
          0, 694, 1160, 22, 17);
    if (page == 0) {
      // Row 1: Basic borders with labels
      label(root.ent(), 10, "Width / requested thickness", 0, 159, 525, 27, 22, true);
      label(root.ent(), 11, "Color / all request 3px", 612, 159, 548, 27, 22, true);
      const std::array<afterhours::Color, 6> colors{{{100, 100, 100, 255}, {60, 60, 60, 255},
          {30, 30, 30, 255}, accent_blue, accent_cyan, accent_orange}};
      const std::array<std::string, 6> names{"Thin", "Medium", "Thick", "Blue", "Cyan", "Orange"};
      const std::array<float, 6> widths{1, 3, 6, 3, 3, 3};
      const std::array<std::string, 6> captions{"1px requested", "3px requested", "6px requested", "#5091DC", "#46BEC8", "#EB9146"};
      for (int i = 0; i < 6; ++i) {
        const float x = i < 3 ? i * 172.f : 612 + (i - 3) * 188.f;
        div(context, mk(root.ent(), 20 + i), box(x, 192, 100, 100)
            .with_custom_background(sample_white).with_border(colors[i], widths[i] * s)
            .with_debug_name("border_" + std::to_string(i)));
        label(root.ent(), 30 + i, names[i], x, 296, 170, 24, 20, true);
        label(root.ent(), 40 + i, captions[i], x, 321, 172, 22, 17);
      }
      // Row 2: Rounded corners and shadows
      label(root.ent(), 13, "Corners / fraction of half the short side", 0, 382, 730, 27, 22, true);
      label(root.ent(), 14, "Shadows / neutral backdrop", 792, 382, 368, 27, 22, true);
      const std::array<float, 4> fractions{.2f, .5f, .25f, .3f};
      const std::array<std::bitset<4>, 4> masks{std::bitset<4>(15), std::bitset<4>(15), std::bitset<4>(12), std::bitset<4>(9)};
      const std::array<std::string, 4> corner_names{"Rounded", "Roundness 0.5", "Bottom corners", "Top-left + bottom-right"};
      const std::array<std::string, 4> corner_captions{"0.2 / radius 10px", "0.5 / radius 25px", "0.25 / radius 12.5px", "0.3 / radius 15px"};
      for (int i = 0; i < 4; ++i) {
        const float x = i * 192.f;
        div(context, mk(root.ent(), 50 + i), fraction(box(x, 417, 100, 100), fractions[i], masks[i])
            .with_custom_background(sample_white).with_border(i == 1 ? accent_cyan : i == 3 ? accent_orange : accent_blue, 2 * s)
            .with_debug_name("corner_" + std::to_string(i)));
        label(root.ent(), 60 + i, corner_names[i], x, 521, 192, 24, i == 3 ? 16.f : 18.f, true);
        label(root.ent(), 70 + i, corner_captions[i], x, 547, 192, 22, 16);
      }
      div(context, mk(root.ent(), 80), fraction(box(192, 593, 160, 36), 1.f)
          .with_custom_background(sample_white).with_border(accent_cyan, 2 * s)
          .with_debug_name("true_capsule"));
      label(root.ent(), 81, "Capsule 1.0 / r18px", 192, 636, 194, 22, 16);
      for (int i = 0; i < 2; ++i) {
        const float x = 792 + i * 192.f;
        div(context, mk(root.ent(), 82 + i), box(x, 417, 168, 112).with_custom_background({192, 197, 205, 255}).with_corner_radius(8 * s));
        auto config = box(x + 26, 422, 100, 100).with_custom_background(sample_white)
            .with_border(i == 0 ? afterhours::Color{80, 80, 80, 255} : afterhours::Color{100, 100, 100, 255}, (i == 0 ? 2 : 1) * s)
            .with_corner_radius(8 * s).with_debug_name(i == 0 ? "hard_shadow" : "soft_shadow");
        if (i == 0) config.with_hard_shadow(5 * s, 5 * s, {0, 0, 0, 120});
        else config.with_soft_shadow(4 * s, 6 * s, 14 * s, {0, 0, 0, 80});
        div(context, mk(root.ent(), 84 + i), config);
        label(root.ent(), 86 + i, i == 0 ? "Hard shadow" : "Soft shadow", x, 534, 176, 24, 18, true);
        label(root.ent(), 88 + i, i == 0 ? "Offset 5,5 / no blur" : "Offset 4,6 / blur 14", x, 560, 176, 22, 16);
      }
      label(root.ent(), 90, "0.5 is a rounded rectangle. The wider companion uses 1.0 for a true capsule.",
            400, 608, 750, 27, 18);
      return;
    }
    if (page == 1) {
      // Row 3: Themed examples
      label(root.ent(), 90, "Themed treatments", 0, 168, 1160, 25, 22, true);
      const std::array<std::string, 6> theme_names{"Dark mode", "Neon glow", "Cozy", "Danger", "Success", "Outline only"};
      const std::array<std::string, 6> theme_details{"Cyan outline", "Cyan blur 12", "Warm fill / blur 10", "Error border", "Success border", "Transparent fill"};
      const std::array<afterhours::Color, 6> fills{{bg_deep, {15, 20, 30, 255}, {255, 250, 242, 255},
          {80, 25, 25, 255}, {25, 70, 40, 255}, {0, 0, 0, 0}}};
      const std::array<afterhours::Color, 6> borders{{accent_cyan, {0, 255, 200, 255}, {160, 130, 100, 255},
          {220, 70, 70, 255}, {80, 200, 120, 255}, text_light}};
      for (int i = 0; i < 6; ++i) {
        const float x = i * 194.f;
        auto config = box(x, 205, 100, 100).with_custom_background(fills[i]).with_border(borders[i], (i == 0 || i == 5 ? 2 : 3) * s)
            .with_corner_radius(8 * s).with_debug_name("theme_" + std::to_string(i));
        if (i == 1) config.with_soft_shadow(0, 0, 12 * s, {0, 255, 200, 60});
        if (i == 2) config = fraction(config, .15f).with_soft_shadow(3 * s, 4 * s, 10 * s, {80, 60, 40, 50});
        auto sample = div(context, mk(root.ent(), 100 + i), config);
        if (i == 3 || i == 4) div(context, mk(sample.ent(), 0), box(38, 35, 24, 26)
            .with_label(i == 3 ? "!" : "+").with_font("AtkinsonMock", pixels(21 * s))
            .with_custom_text_color(i == 3 ? afterhours::Color{255, 220, 220, 255} : afterhours::Color{220, 255, 230, 255})
            .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
        label(root.ent(), 110 + i, theme_names[i], x, 312, 190, 25, 20, true);
        label(root.ent(), 120 + i, theme_details[i], x, 341, 190, 25, 18);
      }


      // Interactive button with border
      label(root.ent(), 130, "Interaction / live control and static state previews", 0, 397, 820, 28, 22, true);
      label(root.ent(), 131, "Nested panel", 868, 397, 292, 28, 22, true);
      auto live = button(context, mk(root.ent(), 132), fraction(box(0, 439, 220, 56), .4f)
          .with_label("Try bordered button").with_font("AtkinsonMock", pixels(20 * s))
          .with_alignment(TextAlignment::Center).with_custom_background(accent_blue)
          .with_custom_hover_bg(afterhours::colors::lighten(accent_blue, 1.2f))
          .with_border(afterhours::colors::lighten(accent_blue, 1.3f), 3 * s)
          .with_soft_shadow(3 * s, 5 * s, 12 * s, {80, 145, 220, 60})
          .with_custom_text_color(bg_deep).with_cursor(CursorType::Pointer).with_debug_name("border_live"));
      if (live) ++clicks;
      const std::array<std::string, 3> states{"Rest", "Hover", "Pressed"};
      for (int i = 0; i < 3; ++i) {
        const float x = 252 + i * 192.f;
        div(context, mk(root.ent(), 140 + i), fraction(box(x, 439, 164, 56), .4f)
            .with_custom_background(i == 0 ? accent_blue : afterhours::colors::lighten(accent_blue, 1.2f))
            .with_border(afterhours::colors::lighten(accent_blue, 1.3f), 3 * s)
            .with_soft_shadow(3 * s, 5 * s, 12 * s, {80, 145, 220, 60})
            .with_debug_name("border_state_" + std::to_string(i)));
        label(root.ent(), 150 + i, states[i], x, 509, 170, 26, 20, true);
      }
      label(root.ent(), 133, fmt::format("Button activations: {}", clicks), 0, 510, 244, 26, 20, true, "border_clicks");
      label(root.ent(), 134, "Hover and pressed share the same fill. Native activation is reported on press.",
            0, 547, 828, 26, 18);
      // Nested panel demo
      auto nested = div(context, mk(root.ent(), 135), box(868, 439, 280, 110).with_custom_background(panel_dark)
          .with_border(accent_cyan, 2 * s).with_corner_radius(8 * s).with_debug_name("nested_outer"));
      label(nested.ent(), 0, "Parent / cyan", 12, 8, 256, 24, 18);
      div(context, mk(nested.ent(), 1), box(16, 43, 120, 55).with_label("Filled child")
          .with_font("AtkinsonMock", pixels(17 * s)).with_alignment(TextAlignment::Center)
          .with_custom_text_color(bg_deep).with_custom_background(sample_white)
          .with_border(accent_orange, 2 * s).with_corner_radius(8 * s).with_debug_name("nested_filled"));
      div(context, mk(nested.ent(), 2), box(150, 43, 100, 55).with_label("Outline child")
          .with_font("AtkinsonMock", pixels(15 * s)).with_alignment(TextAlignment::Center)
          .with_custom_text_color(text_light).with_border(text_light, 2 * s)
          .with_corner_radius(8 * s).with_debug_name("nested_outline"));
      label(root.ent(), 136, "Children / orange and white", 868, 554, 292, 25, 17);
      label(root.ent(), 137, "Live button: Tab to focus, Enter to activate, or press the button.", 0, 601, 1160, 27, 20);
      label(root.ent(), 138, "The parent and both outlined children are separate native UI elements.", 0, 638, 1160, 27, 18);
      return;
    }
    // Row 5: Per-side borders (new feature demo)
    label(root.ent(), 160, "Per-side borders", 0, 168, 1160, 28, 22, true);
    const std::array<std::string, 5> edge_names{"Top only", "Bottom only", "Left and right", "Mixed edges", "Underline"};
    const std::array<std::string, 5> edge_captions{"Top / blue 3px", "Bottom / cyan 3px", "Both / orange 3px", "Top blue 3 / right cyan 2", "Bottom / red 4px"};
    for (int i = 0; i < 5; ++i) {
      const float x = i * 236.f;
      auto config = box(x, 211, 208, 100).with_custom_background(sample_white)
          .with_debug_name("edge_" + std::to_string(i));
      if (i == 0) config.with_border_top(accent_blue, pixels(3 * s));
      if (i == 1) config.with_border_bottom(accent_cyan, pixels(3 * s));
      if (i == 2) config.with_border_left(accent_orange, pixels(3 * s)).with_border_right(accent_orange, pixels(3 * s));
      if (i == 3) config.with_border_top(accent_blue, pixels(3 * s)).with_border_right(accent_cyan, pixels(2 * s))
          .with_border_bottom(accent_orange, pixels(3 * s)).with_border_left(text_light, pixels(2 * s));
      if (i == 4) config.with_border_bottom({200, 60, 60, 255}, pixels(4 * s));
      div(context, mk(root.ent(), 170 + i), config);
      label(root.ent(), 180 + i, edge_names[i], x, 320, 216, 25, 20, true);
      label(root.ent(), 190 + i, edge_captions[i], x, 350, 216, 22, i == 3 ? 16.f : 17.f);
    }
    label(root.ent(), 161, "Bottom orange 3 / left white 2", 708, 377, 232, 22, 16);
    // Row 6: Dotted borders (new BorderStyle::Dotted feature demo)
    label(root.ent(), 200, "Dotted borders / dash and gap are each twice the border thickness", 0, 438, 1160, 28, 22, true);
    const std::array<float, 5> dotted_widths{1, 3, 6, 3, 4};
    const std::array<std::string, 5> dotted_names{"Thin", "Medium", "Thick", "Orange", "Underline"};
    const std::array<afterhours::Color, 5> dotted_colors{{{80, 80, 80, 255}, {50, 50, 50, 255}, {30, 30, 30, 255}, accent_orange, accent_blue}};
    for (int i = 0; i < 5; ++i) {
      const float x = i * 236.f;
      auto config = box(x, 481, 208, 100).with_custom_background(sample_white).with_debug_name("dotted_" + std::to_string(i));
      if (i == 4) config.with_border_bottom(dotted_colors[i], pixels(dotted_widths[i] * s), BorderStyle::Dotted);
      else config.with_border(dotted_colors[i], dotted_widths[i] * s, BorderStyle::Dotted);
      div(context, mk(root.ent(), 210 + i), config);
      label(root.ent(), 220 + i, dotted_names[i], x, 591, 216, 24, 20, true);
      label(root.ent(), 230 + i, fmt::format("{:.0f}px / dash {:.0f} / gap {:.0f}", dotted_widths[i], dotted_widths[i] * 2, dotted_widths[i] * 2),
            x, 621, 216, 22, 17);
    }
  }
};

REGISTER_EXAMPLE_SCREEN(example_borders, "Component Galleries", "Border styling options",
                        ExampleBordersScreen)
