#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the accessibility features:
// - Auto-contrast text color selection
// - Theme validation
// - WCAG compliance indicators
struct ExampleAccessibility : ScreenSystem<UIContext<InputAction>> {
  std::string selected_sample =
      "Select any swatch to inspect its foreground and contrast.";
  bool validated_theme = false;
  bool theme_is_accessible = false;

  // Clean, professional accessibility-focused palette
  afterhours::Color bg_slate{35, 40, 50, 255};       // Dark slate
  afterhours::Color panel_dark{45, 52, 65, 255};     // Panel slate
  afterhours::Color panel_light{58, 68, 85, 255};    // Lighter panel
  afterhours::Color accent_green{75, 185, 130, 255}; // Accessible green
  afterhours::Color accent_amber{225, 175, 85, 255}; // Warning amber
  afterhours::Color text_white{250, 250, 255, 255};  // White text
  afterhours::Color text_muted{170, 180, 200,
                               255}; // Muted text (5.8:1 on panel_dark)
  afterhours::Color divider{70, 80, 100, 255}; // Divider lines

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Set up theme for auto-text-color demonstration
    // darkfont must be a dark color so auto_text_color has a proper choice
    // for light backgrounds. surface is set to a light tone so that
    // validate_accessibility() passes (darkfont on surface >= 4.5:1).
    Theme theme;
    theme.font = text_white;
    theme.darkfont = afterhours::Color{25, 30, 40, 255};
    theme.font_muted = text_muted;
    theme.background = bg_slate;
    theme.surface = afterhours::Color{210, 215, 225, 255};
    theme.primary = accent_green;
    theme.secondary = afterhours::Color{100, 120, 160, 255};
    theme.accent = accent_amber;
    theme.error = afterhours::Color{200, 85, 85, 255};
    theme.roundness = 0.10f;
    context.theme = theme;

    // Validate theme once on first frame
    if (!validated_theme) {
      theme_is_accessible = theme.validate_accessibility();
      validated_theme = true;
    }

    const float scale =
        static_cast<float>(Settings::get().get_screen_height()) / 720.f;
    const float screen_width =
        static_cast<float>(Settings::get().get_screen_width()) / scale;
    const float screen_height = 720.f;
    const auto px = [scale](float value) { return pixels(value * scale); };
    const auto hex = [](afterhours::Color color) {
      return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    };

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{px(screen_width), px(screen_height)})
            .with_custom_background(bg_slate)
            .with_corner_radius(0.f)
            .with_debug_name("bg"));

    // Main content panel
    float panel_w = 1120.0f;
    float panel_h = 664.0f;
    float panel_x = (screen_width - panel_w) / 2.0f;
    float panel_y = (screen_height - panel_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{px(panel_w), px(panel_h)})
            .with_absolute_position(px(panel_x), px(panel_y))
            .with_custom_background(panel_dark)
            .with_soft_shadow(6.0f, 10.0f, 25.0f,
                              afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(RoundedCorners())
            .with_debug_name("main_panel"));

    // Header - use same roundness as container and match its top edge
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Accessibility Features")
            .with_size(ComponentSize{px(panel_w), px(55)})
            .with_absolute_position(px(panel_x), px(panel_y))
            .with_custom_background(accent_green)
            .with_font("AtkinsonMock", h720(36.f))
            .with_auto_text_color(true)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_corner_radius(8.f * context.screen_height / 720.f)
            .with_alignment(TextAlignment::Center));

    // Theme validation status badge
    float status_y = panel_y + 70.0f;
    std::string status_text = theme_is_accessible
                                  ? "Theme color pairs pass AA contrast"
                                  : "Theme color pairs fail AA contrast";
    afterhours::Color status_color =
        theme_is_accessible ? accent_green : accent_amber;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label(status_text)
            .with_size(ComponentSize{px(440), px(38)})
            .with_absolute_position(px(panel_x + (panel_w - 440) / 2.0f),
                                    px(status_y))
            .with_custom_background(status_color)
            .with_auto_text_color(true)
            .with_font("AtkinsonMock", h720(22.f))
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * context.screen_height / 720.f)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label(
                "Slate demo theme: 2 checked pairs (text/background, dark "
                "text/surface) | AA normal text >= 4.5:1 | 22px reference "
                "samples")
            .with_size({px(panel_w - 60.f), px(28)})
            .with_absolute_position(px(panel_x + 30.f), px(status_y + 40.f))
            .with_font("AtkinsonMock", h720(20.f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Separator between status badge and content columns
    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_size(ComponentSize{px(panel_w - 60.f), px(1)})
            .with_absolute_position(px(panel_x + 30.f), px(status_y + 74.0f))
            .with_custom_background(divider)
            .with_debug_name("section_separator_1"));

    // Content area - two columns
    float content_y = status_y + 90.0f;
    float col_w = (panel_w - 80) / 2.0f;
    float left_x = panel_x + 30.0f;
    float right_x = panel_x + 50.0f + col_w;

    // Left column header
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Manual foreground colors")
            .with_size(ComponentSize{px(col_w), px(32)})
            .with_absolute_position(px(left_x), px(content_y))
            .with_custom_background(panel_light)
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_muted)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center));

    // Left column - without auto text color (manual theme.font)
    afterhours::Color demo_bg_light = {225, 225, 235, 255};
    afterhours::Color demo_bg_dark = {45, 50, 65, 255};

    if (button(context, mk(entity, 11),
               ComponentConfig{}
                   .with_label(fmt::format(
                       "Manual dark text  {}  |  {:.1f}:1",
                       hex(afterhours::Color{20, 20, 30, 255}),
                       afterhours::colors::contrast_ratio(
                           afterhours::Color{20, 20, 30, 255}, demo_bg_light)))
                   .with_size(ComponentSize{px(col_w), px(52)})
                   .with_absolute_position(px(left_x), px(content_y + 40.0f))
                   .with_custom_background(demo_bg_light)
                   .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
                   .with_auto_text_color(
                       false) // Manual text color, no auto-contrast
                   .with_font("AtkinsonMock", h720(22.f))
                   .with_rounded_corners(RoundedCorners())))
      selected_sample = "Manual dark foreground on a light background.";

    if (button(context, mk(entity, 12),
               ComponentConfig{}
                   .with_label(fmt::format("Manual light text  {}  |  {:.1f}:1",
                                           hex(theme.font),
                                           afterhours::colors::contrast_ratio(
                                               theme.font, demo_bg_dark)))
                   .with_size(ComponentSize{px(col_w), px(52)})
                   .with_absolute_position(px(left_x), px(content_y + 100.0f))
                   .with_custom_background(demo_bg_dark)
                   .with_border(divider, scale)
                   .with_auto_text_color(false)
                   .with_font("AtkinsonMock", h720(22.f))
                   .with_rounded_corners(RoundedCorners())))
      selected_sample = "Manual light foreground on a dark background.";

    // Note about the issue
    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("Text colors above were chosen manually.")
            .with_size(ComponentSize{px(col_w), px(28)})
            .with_absolute_position(px(left_x), px(content_y + 162.0f))
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Right column header
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Automatic foreground colors")
            .with_size(ComponentSize{px(col_w), px(32)})
            .with_absolute_position(px(right_x), px(content_y))
            .with_custom_background(accent_green)
            .with_font("AtkinsonMock", h720(22.f))
            .with_auto_text_color(true)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center));

    // Right column - with auto text color
    if (button(context, mk(entity, 21),
               ComponentConfig{}
                   .with_label(fmt::format(
                       "Auto {}  |  {:.1f}:1",
                       hex(afterhours::colors::auto_text_color(
                           demo_bg_light, theme.font, theme.darkfont)),
                       afterhours::colors::contrast_ratio(
                           afterhours::colors::auto_text_color(
                               demo_bg_light, theme.font, theme.darkfont),
                           demo_bg_light)))
                   .with_size(ComponentSize{px(col_w), px(52)})
                   .with_absolute_position(px(right_x), px(content_y + 40.0f))
                   .with_custom_background(demo_bg_light)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", h720(22.f))
                   .with_rounded_corners(RoundedCorners())))
      selected_sample = "Automatic foreground on a light background.";

    if (button(context, mk(entity, 22),
               ComponentConfig{}
                   .with_label(fmt::format(
                       "Auto {}  |  {:.1f}:1",
                       hex(afterhours::colors::auto_text_color(
                           demo_bg_dark, theme.font, theme.darkfont)),
                       afterhours::colors::contrast_ratio(
                           afterhours::colors::auto_text_color(
                               demo_bg_dark, theme.font, theme.darkfont),
                           demo_bg_dark)))
                   .with_size(ComponentSize{px(col_w), px(52)})
                   .with_absolute_position(px(right_x), px(content_y + 100.0f))
                   .with_custom_background(demo_bg_dark)
                   .with_border(divider, scale)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", h720(22.f))
                   .with_rounded_corners(RoundedCorners())))
      selected_sample = "Automatic foreground on a dark background.";

    // Note about the solution
    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Chooses the foreground with higher contrast.")
            .with_size(ComponentSize{px(col_w), px(28)})
            .with_absolute_position(px(right_x), px(content_y + 162.0f))
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Separator between columns section and color showcase
    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_size(ComponentSize{px(panel_w - 60.f), px(1)})
            .with_absolute_position(px(panel_x + 30.f), px(content_y + 195.0f))
            .with_custom_background(divider)
            .with_debug_name("section_separator_2"));

    // Color showcase section
    float showcase_y = content_y + 210.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Light backgrounds / increasing luminance / background "
                        "above foreground")
            .with_size(ComponentSize{px(panel_w - 60), px(32)})
            .with_absolute_position(px(panel_x + 30.0f), px(showcase_y))
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    // Variety of background colors to showcase
    afterhours::Color showcase_colors[] = {
        {255, 200, 100, 255}, // Yellow
        {100, 180, 255, 255}, // Light blue
        {255, 100, 140, 255}, // Pink
        {150, 255, 150, 255}, // Light green
        {180, 130, 220, 255}, // Lavender
        {255, 160, 100, 255}, // Orange
        {100, 200, 180, 255}, // Teal
        {220, 180, 160, 255}, // Tan
    };

    std::sort(std::begin(showcase_colors), std::end(showcase_colors),
              [](auto a, auto b) {
                return afterhours::colors::luminance(a) <
                       afterhours::colors::luminance(b);
              });
    float btn_w = (panel_w - 60.f - 7.f * 12.f) / 8.f;
    for (int i = 0; i < 8; i++) {
      const auto bg = showcase_colors[i];
      const auto fg =
          afterhours::colors::auto_text_color(bg, theme.font, theme.darkfont);
      const float ratio = afterhours::colors::contrast_ratio(fg, bg);
      if (button(context, mk(entity, 40 + i),
                 ComponentConfig{}
                     .with_label(hex(bg) + "\n" + hex(fg))
                     .with_size({px(btn_w), px(52)})
                     .with_absolute_position(
                         px(panel_x + 30.f +
                            static_cast<float>(i) * (btn_w + 12.f)),
                         px(showcase_y + 35.f))
                     .with_custom_background(bg)
                     .with_auto_text_color(true)
                     .with_font("AtkinsonMock", h720(20.f))
                     .with_alignment(TextAlignment::Center)
                     .with_corner_radius(6.f * scale)
                     .with_debug_name("color_" + std::to_string(i))))
        selected_sample =
            fmt::format("Background {} / foreground {} / contrast {:.2f}:1",
                        hex(bg), hex(fg), ratio);
      div(context, mk(entity, 80 + i),
          ComponentConfig{}
              .with_label(fmt::format("{:.1f}:1", ratio))
              .with_size({px(btn_w), px(24)})
              .with_absolute_position(
                  px(panel_x + 30.f + static_cast<float>(i) * (btn_w + 12.f)),
                  px(showcase_y + 35.f + 52.f))
              .with_font("AtkinsonMock", h720(20.f))
              .with_custom_text_color(text_muted)
              .with_alignment(TextAlignment::Center));
    }

    // Dark showcase row
    afterhours::Color dark_colors[] = {
        {40, 60, 100, 255}, // Dark blue
        {80, 40, 80, 255},  // Dark purple
        {30, 70, 50, 255},  // Dark green
        {100, 50, 50, 255}, // Dark red
        {60, 60, 70, 255},  // Dark gray
        {80, 70, 40, 255},  // Dark brown
        {20, 60, 80, 255},  // Dark teal
        {50, 40, 60, 255},  // Dark violet
    };

    std::sort(std::begin(dark_colors), std::end(dark_colors),
              [](auto a, auto b) {
                return afterhours::colors::luminance(a) <
                       afterhours::colors::luminance(b);
              });
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label(
                "Dark backgrounds / increasing luminance; rows are independent")
            .with_size({px(panel_w - 60.f), px(26)})
            .with_absolute_position(px(panel_x + 30.f), px(showcase_y + 108.f))
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));
    for (int i = 0; i < 8; i++) {
      const auto bg = dark_colors[i];
      const auto fg =
          afterhours::colors::auto_text_color(bg, theme.font, theme.darkfont);
      const float ratio = afterhours::colors::contrast_ratio(fg, bg);
      if (button(context, mk(entity, 50 + i),
                 ComponentConfig{}
                     .with_label(hex(bg) + "\n" + hex(fg))
                     .with_size({px(btn_w), px(52)})
                     .with_absolute_position(
                         px(panel_x + 30.f +
                            static_cast<float>(i) * (btn_w + 12.f)),
                         px(showcase_y + 138.f))
                     .with_custom_background(bg)
                     .with_auto_text_color(true)
                     .with_font("AtkinsonMock", h720(20.f))
                     .with_alignment(TextAlignment::Center)
                     .with_corner_radius(6.f * scale)
                     .with_debug_name("dark_" + std::to_string(i))))
        selected_sample =
            fmt::format("Background {} / foreground {} / contrast {:.2f}:1",
                        hex(bg), hex(fg), ratio);
      div(context, mk(entity, 90 + i),
          ComponentConfig{}
              .with_label(fmt::format("{:.1f}:1", ratio))
              .with_size({px(btn_w), px(24)})
              .with_absolute_position(
                  px(panel_x + 30.f + static_cast<float>(i) * (btn_w + 12.f)),
                  px(showcase_y + 138.f + 52.f))
              .with_font("AtkinsonMock", h720(20.f))
              .with_custom_text_color(text_muted)
              .with_alignment(TextAlignment::Center));
    }

    // Footer info
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label(selected_sample)
            .with_size(ComponentSize{px(panel_w - 60), px(32)})
            .with_absolute_position(px(panel_x + 30.0f),
                                    px(panel_y + panel_h - 68.f))
            .with_font("AtkinsonMock", h720(22.f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(accessibility, "System Demos",
                        "Demonstrates Garnish accessibility features",
                        ExampleAccessibility)
