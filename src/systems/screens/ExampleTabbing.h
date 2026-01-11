#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTabbing : ScreenSystem<UIContext<InputAction>> {
  int focused_button = 0;
  std::vector<int> button_clicks = {0, 0, 0, 0};

  // Retro arcade color palette
  afterhours::Color bg_dark{18, 12, 28, 255};        // Deep purple-black
  afterhours::Color panel_bg{35, 28, 52, 255};       // Dark purple panel
  afterhours::Color btn_magenta{165, 55, 140, 255};  // Electric magenta
  afterhours::Color btn_cyan{55, 195, 195, 255};     // Electric cyan
  afterhours::Color btn_lime{145, 215, 65, 255};     // Electric lime
  afterhours::Color btn_amber{235, 175, 55, 255};    // Electric amber
  afterhours::Color text_light{240, 235, 250, 255};  // Off-white
  afterhours::Color border_glow{120, 75, 155, 255};  // Purple glow

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Set up retro arcade theme
    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_dark;
    theme.font_muted = afterhours::Color{140, 130, 160, 255};
    theme.background = bg_dark;
    theme.surface = panel_bg;
    theme.primary = btn_magenta;
    theme.secondary = btn_cyan;
    theme.accent = btn_lime;
    theme.error = afterhours::Color{220, 70, 70, 255};
    theme.roundness = 0.12f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Full screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_dark)
            .with_debug_name("background"));

    // Main panel with glow effect
    float panel_width = 380.0f;
    float panel_height = 420.0f;
    float panel_x = (screen_width - panel_width) / 2.0f;
    float panel_y = (screen_height - panel_height) / 2.0f - 20.0f;

    // Panel shadow/glow
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_width + 8), pixels(panel_height + 8)})
            .with_absolute_position()
            .with_translate(panel_x - 4.0f, panel_y - 4.0f)
            .with_custom_background(afterhours::Color{100, 60, 140, 60})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("panel_glow"));

    // Main panel
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_width), pixels(panel_height)})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_bg)
            .with_border(border_glow, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.1f)
            .with_debug_name("main_panel"));

    // Title bar
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("KEYBOARD NAV")
            .with_size(ComponentSize{pixels(panel_width - 4), pixels(50)})
            .with_absolute_position()
            .with_translate(panel_x + 2.0f, panel_y + 2.0f)
            .with_custom_background(btn_magenta)
            .with_font("EqProRounded", 26.0f)
            .with_custom_text_color(text_light)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.15f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Instruction text
    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_label("Use TAB to navigate, ENTER to click")
            .with_size(ComponentSize{pixels(panel_width - 40), pixels(24)})
            .with_absolute_position()
            .with_translate(panel_x + 20.0f, panel_y + 62.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("instructions"));

    // Button colors
    afterhours::Color btn_colors[] = {btn_magenta, btn_cyan, btn_lime, btn_amber};
    std::string btn_labels[] = {"OPTION A", "OPTION B", "OPTION C", "OPTION D"};

    float button_width = panel_width - 60.0f;
    float button_height = 55.0f;
    float button_spacing = 12.0f;
    float start_y = panel_y + 100.0f;
    float start_x = panel_x + 30.0f;

    for (int i = 0; i < 4; i++) {
      float button_y = start_y + i * (button_height + button_spacing);
      std::string label = btn_labels[i];
      if (button_clicks[i] > 0) {
        label += " (" + std::to_string(button_clicks[i]) + ")";
      }

      auto button_result =
          button(context, mk(entity, 10 + i),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{pixels(button_width), pixels(button_height)})
                     .with_absolute_position()
                     .with_translate(start_x, button_y)
                     .with_custom_background(btn_colors[i])
                     .with_border(afterhours::colors::lighten(btn_colors[i], 1.3f), 2.0f)
                     .with_soft_shadow(3.0f, 4.0f, 10.0f, afterhours::Color{0, 0, 0, 80})
                     .with_auto_text_color(true)
                     .with_font("EqProRounded", 22.0f)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.4f)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("tab_button_" + std::to_string(i)));

      if (button_result) {
        button_clicks[i]++;
        log_info("Button {} clicked! Count: {}", i + 1, button_clicks[i]);
      }
    }

    // Footer with total clicks
    int total_clicks = 0;
    for (int i = 0; i < 4; i++) total_clicks += button_clicks[i];

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Total Clicks: " + std::to_string(total_clicks))
            .with_size(ComponentSize{pixels(panel_width - 40), pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + 20.0f, panel_y + panel_height - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(btn_lime)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(tabbing, "System Demos",
                        "Keyboard tabbing navigation example", ExampleTabbing)
