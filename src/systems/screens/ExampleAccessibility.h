#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the accessibility features:
// - Auto-contrast text color selection
// - Theme validation
// - WCAG compliance indicators
struct ExampleAccessibility : ScreenSystem<UIContext<InputAction>> {
  bool validated_theme = false;
  bool theme_is_accessible = false;

  // Clean, professional accessibility-focused palette
  afterhours::Color bg_slate{35, 40, 50, 255};          // Dark slate
  afterhours::Color panel_dark{45, 52, 65, 255};        // Panel slate
  afterhours::Color panel_light{58, 68, 85, 255};       // Lighter panel
  afterhours::Color accent_green{75, 185, 130, 255};    // Accessible green
  afterhours::Color accent_amber{225, 175, 85, 255};    // Warning amber
  afterhours::Color text_white{250, 250, 255, 255};     // White text
  afterhours::Color text_muted{140, 150, 170, 255};     // Muted text
  afterhours::Color divider{70, 80, 100, 255};          // Divider lines

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Set up theme for auto-text-color demonstration
    Theme theme;
    theme.font = text_white;
    theme.darkfont = afterhours::Color{25, 30, 40, 255};
    theme.font_muted = text_muted;
    theme.background = bg_slate;
    theme.surface = panel_dark;
    theme.primary = accent_green;
    theme.secondary = afterhours::Color{100, 120, 160, 255};
    theme.accent = accent_amber;
    theme.error = afterhours::Color{200, 85, 85, 255};
    theme.roundness = 0.12f;
    context.theme = theme;

    // Validate theme once on first frame
    if (!validated_theme) {
      theme_is_accessible = ThemeDefaults::get().validate_theme_accessibility();
      validated_theme = true;
    }

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_slate)
            .with_debug_name("bg"));

    // Main content panel
    float panel_w = 900.0f;
    float panel_h = 540.0f;
    float panel_x = (screen_width - panel_w) / 2.0f;
    float panel_y = (screen_height - panel_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_w), pixels(panel_h)})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_dark)
            .with_border(divider, 1.0f)
            .with_soft_shadow(6.0f, 10.0f, 25.0f, afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("main_panel"));

    // Header
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Accessibility Features")
            .with_size(ComponentSize{pixels(panel_w - 4), pixels(55)})
            .with_absolute_position()
            .with_translate(panel_x + 2.0f, panel_y + 2.0f)
            .with_custom_background(accent_green)
            .with_font("EqProRounded", 28.0f)
            .with_auto_text_color(true)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Theme validation status badge
    float status_y = panel_y + 70.0f;
    std::string status_text = theme_is_accessible ? "WCAG AA Compliant" : "Needs Improvement";
    afterhours::Color status_color = theme_is_accessible ? accent_green : accent_amber;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label(status_text)
            .with_size(ComponentSize{pixels(220), pixels(38)})
            .with_absolute_position()
            .with_translate(panel_x + (panel_w - 220) / 2.0f, status_y)
            .with_custom_background(status_color)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("theme_status"));

    // Content area - two columns
    float content_y = status_y + 55.0f;
    float col_w = (panel_w - 80) / 2.0f;
    float left_x = panel_x + 30.0f;
    float right_x = panel_x + 50.0f + col_w;

    // Left column header
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("WITHOUT auto_text_color")
            .with_size(ComponentSize{pixels(col_w), pixels(28)})
            .with_absolute_position()
            .with_translate(left_x, content_y)
            .with_custom_background(panel_light)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("left_header"));

    // Left column - without auto text color (manual theme.font)
    afterhours::Color demo_bg_light = {225, 225, 235, 255};
    afterhours::Color demo_bg_dark = {45, 50, 65, 255};

    button(context, mk(entity, 11),
           ComponentConfig{}
               .with_label("Light BG - Hard to Read")
               .with_size(ComponentSize{pixels(col_w), pixels(52)})
               .with_absolute_position()
               .with_translate(left_x, content_y + 40.0f)
               .with_custom_background(demo_bg_light)
               .with_auto_text_color(false)  // Using theme font (white) on light bg
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("light_no_auto"));

    button(context, mk(entity, 12),
           ComponentConfig{}
               .with_label("Dark BG - Works Fine")
               .with_size(ComponentSize{pixels(col_w), pixels(52)})
               .with_absolute_position()
               .with_translate(left_x, content_y + 100.0f)
               .with_custom_background(demo_bg_dark)
               .with_auto_text_color(false)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("dark_no_auto"));

    // Note about the issue
    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("Default theme font doesn't adapt")
            .with_size(ComponentSize{pixels(col_w), pixels(22)})
            .with_absolute_position()
            .with_translate(left_x, content_y + 162.0f)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("left_note"));

    // Right column header
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("WITH auto_text_color")
            .with_size(ComponentSize{pixels(col_w), pixels(28)})
            .with_absolute_position()
            .with_translate(right_x, content_y)
            .with_custom_background(accent_green)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_auto_text_color(true)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("right_header"));

    // Right column - with auto text color
    button(context, mk(entity, 21),
           ComponentConfig{}
               .with_label("Light BG - Auto Dark Text")
               .with_size(ComponentSize{pixels(col_w), pixels(52)})
               .with_absolute_position()
               .with_translate(right_x, content_y + 40.0f)
               .with_custom_background(demo_bg_light)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("light_auto"));

    button(context, mk(entity, 22),
           ComponentConfig{}
               .with_label("Dark BG - Auto Light Text")
               .with_size(ComponentSize{pixels(col_w), pixels(52)})
               .with_absolute_position()
               .with_translate(right_x, content_y + 100.0f)
               .with_custom_background(demo_bg_dark)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("dark_auto"));

    // Note about the solution
    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Automatically picks best contrast!")
            .with_size(ComponentSize{pixels(col_w), pixels(22)})
            .with_absolute_position()
            .with_translate(right_x, content_y + 162.0f)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_custom_text_color(accent_green)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("right_note"));

    // Color showcase section
    float showcase_y = content_y + 200.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Auto-Contrast on Various Backgrounds")
            .with_size(ComponentSize{pixels(panel_w - 60), pixels(26)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, showcase_y)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("showcase_title"));

    // Variety of background colors to showcase
    afterhours::Color showcase_colors[] = {
        {255, 200, 100, 255},  // Yellow
        {100, 180, 255, 255},  // Light blue
        {255, 100, 140, 255},  // Pink
        {150, 255, 150, 255},  // Light green
        {180, 130, 220, 255},  // Lavender
        {255, 160, 100, 255},  // Orange
        {100, 200, 180, 255},  // Teal
        {220, 180, 160, 255},  // Tan
    };

    float btn_w = (panel_w - 90) / 8.0f;
    for (int i = 0; i < 8; i++) {
      button(context, mk(entity, 40 + i),
             ComponentConfig{}
                 .with_label("Auto")
                 .with_size(ComponentSize{pixels(btn_w), pixels(52)})
                 .with_absolute_position()
                 .with_translate(panel_x + 30.0f + i * (btn_w + 5), showcase_y + 35.0f)
                 .with_custom_background(showcase_colors[i])
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_debug_name("color_" + std::to_string(i)));
    }

    // Dark showcase row
    afterhours::Color dark_colors[] = {
        {40, 60, 100, 255},    // Dark blue
        {80, 40, 80, 255},     // Dark purple
        {30, 70, 50, 255},     // Dark green
        {100, 50, 50, 255},    // Dark red
        {60, 60, 70, 255},     // Dark gray
        {80, 70, 40, 255},     // Dark brown
        {20, 60, 80, 255},     // Dark teal
        {50, 40, 60, 255},     // Dark violet
    };

    for (int i = 0; i < 8; i++) {
      button(context, mk(entity, 50 + i),
             ComponentConfig{}
                 .with_label("Auto")
                 .with_size(ComponentSize{pixels(btn_w), pixels(52)})
                 .with_absolute_position()
                 .with_translate(panel_x + 30.0f + i * (btn_w + 5), showcase_y + 95.0f)
                 .with_custom_background(dark_colors[i])
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_debug_name("dark_" + std::to_string(i)));
    }

    // Footer info
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Auto text color is enabled by default - text always remains readable")
            .with_size(ComponentSize{pixels(panel_w - 60), pixels(26)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + panel_h - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(accessibility, "System Demos",
                        "Demonstrates Garnish accessibility features",
                        ExampleAccessibility)
