#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SportsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 1;  // VIDEO tab
  size_t selected_row = 4;  // Max FPS selected

  // Colors matching Rematch/FIFA style - dark with bright green accents
  afterhours::Color bg_dark{18, 22, 28, 255};            // Very dark background
  afterhours::Color overlay_dark{25, 32, 42, 200};       // Semi-transparent overlay
  afterhours::Color panel_dark{32, 42, 55, 255};         // Panel background
  afterhours::Color accent_green{85, 195, 145, 255};     // Bright green accent
  afterhours::Color accent_green_dark{55, 155, 105, 255};// Darker green
  afterhours::Color text_white{235, 240, 245, 255};      // White text
  afterhours::Color text_muted{125, 140, 155, 255};      // Muted text
  afterhours::Color highlight_row{45, 75, 95, 255};      // Selected row highlight
  afterhours::Color slider_bg{55, 65, 80, 255};          // Slider background
  afterhours::Color slider_track{35, 45, 55, 255};       // Slider track

  std::vector<std::string> tabs = {"GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  struct SettingRow {
    std::string label;
    std::string value;
    bool is_slider;
    float slider_pct;
  };

  std::vector<SettingRow> graphics_settings = {
      {"Window mode", "Borderless window", false, 0.0f},
      {"Resolution", "2,560x1,440", false, 0.0f},
      {"Screen percentage", "100", true, 1.0f},
      {"V Sync", "Enabled", false, 0.0f},
      {"Max FPS", "120", true, 0.5f},
      {"Gamma", "2.2", true, 0.45f},
      {"Anti-Aliasing method", "TSR", false, 0.0f},
      {"Dynamic resolution", "Enabled", false, 0.0f},
      {"Framerate target", "60", true, 0.4f},
      {"Motion blur", "1", true, 0.7f},
      {"Graphics quality", "Ultra", false, 0.0f},
      {"Texture quality", "Ultra", false, 0.0f},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = panel_dark;
    theme.primary = accent_green;
    theme.secondary = text_muted;
    theme.accent = accent_green;
    theme.error = afterhours::Color{180, 80, 80, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== TOP TAB BAR ==========
    float tab_y = 15.0f;
    float tab_w = 130.0f;
    float tab_h = 35.0f;
    float tab_start_x = 70.0f;

    // LB button
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("LB")
            .with_size(ComponentSize{pixels(35), pixels(28)})
            .with_absolute_position()
            .with_translate(tab_start_x - 50.0f, tab_y + 4.0f)
            .with_custom_background(panel_dark)
            .with_border(text_muted, 1.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lb_btn"));

    for (size_t i = 0; i < tabs.size(); i++) {
      float tx = tab_start_x + (float)i * tab_w;
      bool is_selected = (i == selected_tab);
      afterhours::Color tab_text = is_selected ? accent_green : text_muted;

      if (button(context, mk(entity, 10 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(tabs[i])
                     .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 10)), 
                                              pixels(static_cast<int>(tab_h))})
                     .with_absolute_position()
                     .with_translate(tx, tab_y)
                     .with_font("EqProRounded", 18.0f)
                     .with_custom_text_color(tab_text)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }

      // Underline on selected tab
      if (is_selected) {
        div(context, mk(entity, 20 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 20)), pixels(4)})
                .with_absolute_position()
                .with_translate(tx + 5.0f, tab_y + tab_h + 2.0f)
                .with_custom_background(accent_green)
                .with_debug_name("tab_underline_" + std::to_string(i)));
      }
    }

    // RB button
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("RB")
            .with_size(ComponentSize{pixels(35), pixels(28)})
            .with_absolute_position()
            .with_translate(tab_start_x + (float)tabs.size() * tab_w, tab_y + 4.0f)
            .with_custom_background(panel_dark)
            .with_border(text_muted, 1.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("rb_btn"));

    // ========== SECTION HEADER ==========
    float header_y = tab_y + 55.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("GRAPHICS")
            .with_size(ComponentSize{pixels(150), pixels(30)})
            .with_absolute_position()
            .with_translate(40.0f, header_y)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("section_header"));

    // ========== LEFT PANEL: Settings ==========
    float panel_x = 40.0f;
    float panel_y = header_y + 40.0f;
    float panel_w = 450.0f;
    float row_h = 42.0f;

    for (size_t i = 0; i < graphics_settings.size(); i++) {
      float ry = panel_y + (float)i * row_h;
      bool is_selected = (i == selected_row);
      auto& setting = graphics_settings[i];

      // Row background for selected
      if (is_selected) {
        div(context, mk(entity, 50 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(panel_w)), 
                                         pixels(static_cast<int>(row_h - 2))})
                .with_absolute_position()
                .with_translate(panel_x - 10.0f, ry)
                .with_custom_background(highlight_row)
                .with_debug_name("row_bg_" + std::to_string(i)));
      }

      // Label
      afterhours::Color label_color = is_selected ? text_white : text_muted;
      if (button(context, mk(entity, 100 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(setting.label)
                     .with_size(ComponentSize{pixels(220), pixels(28)})
                     .with_absolute_position()
                     .with_translate(panel_x, ry + 6.0f)
                     .with_font("EqProRounded", 17.0f)
                     .with_custom_text_color(label_color)
                     .with_debug_name("label_" + std::to_string(i)))) {
        selected_row = i;
      }

      // Value
      afterhours::Color value_color = is_selected ? accent_green : text_muted;
      div(context, mk(entity, 101 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(setting.value)
              .with_size(ComponentSize{pixels(160), pixels(28)})
              .with_absolute_position()
              .with_translate(panel_x + 240.0f, ry + 6.0f)
              .with_font("EqProRounded", 17.0f)
              .with_custom_text_color(value_color)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("value_" + std::to_string(i)));

      // Slider or dots indicator
      if (setting.is_slider) {
        // Slider track
        div(context, mk(entity, 102 + static_cast<int>(i) * 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(130), pixels(6)})
                .with_absolute_position()
                .with_translate(panel_x + 270.0f, ry + 16.0f)
                .with_custom_background(slider_track)
                .with_debug_name("slider_track_" + std::to_string(i)));

        // Slider fill
        int fill_w = static_cast<int>(125.0f * setting.slider_pct);
        div(context, mk(entity, 200 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(fill_w), pixels(6)})
                .with_absolute_position()
                .with_translate(panel_x + 270.0f, ry + 16.0f)
                .with_custom_background(accent_green)
                .with_debug_name("slider_fill_" + std::to_string(i)));

        // Slider handle
        div(context, mk(entity, 250 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(12), pixels(12)})
                .with_absolute_position()
                .with_translate(panel_x + 264.0f + 125.0f * setting.slider_pct, ry + 13.0f)
                .with_custom_background(accent_green)
                .with_rounded_corners(std::bitset<4>(0b1111))
                .with_roundness(1.0f)
                .with_debug_name("slider_handle_" + std::to_string(i)));
      } else {
        // Dots indicator
        div(context, mk(entity, 102 + static_cast<int>(i) * 3),
            ComponentConfig{}
                .with_label(". . .")
                .with_size(ComponentSize{pixels(50), pixels(20)})
                .with_absolute_position()
                .with_translate(panel_x + 390.0f, ry + 10.0f)
                .with_font("EqProRounded", 14.0f)
                .with_custom_text_color(value_color)
                .with_debug_name("dots_" + std::to_string(i)));
      }
    }

    // ========== RIGHT PANEL: Help/Info ==========
    float help_x = panel_x + panel_w + 100.0f;
    float help_y = 85.0f;
    float help_w = 350.0f;

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label(graphics_settings[selected_row].label)
            .with_size(ComponentSize{pixels(static_cast<int>(help_w)), pixels(35)})
            .with_absolute_position()
            .with_translate(help_x, help_y)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_title"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("The maximum number of frames the game will try to display")
            .with_size(ComponentSize{pixels(static_cast<int>(help_w)), pixels(50)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 40.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_desc1"));

    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("per second")
            .with_size(ComponentSize{pixels(static_cast<int>(help_w)), pixels(30)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 70.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_desc2"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Default value: 120")
            .with_size(ComponentSize{pixels(static_cast<int>(help_w)), pixels(30)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 110.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("help_default"));

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = (float)screen_h - 40.0f;
    float prompt_x = (float)screen_w - 330.0f;

    // Y Reset to default
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("Y")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(prompt_x, prompt_y)
            .with_custom_background(afterhours::Color{180, 160, 60, 255})
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("y_btn"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("Reset to default")
            .with_size(ComponentSize{pixels(130), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 35.0f, prompt_y + 2.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("reset_label"));

    // B Back
    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("B")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(prompt_x + 175.0f, prompt_y)
            .with_custom_background(afterhours::Color{180, 80, 80, 255})
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("b_btn"));

    div(context, mk(entity, 403),
        ComponentConfig{}
            .with_label("Back")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 210.0f, prompt_y + 2.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("back_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (FIFA/Rematch style)",
                        SportsSettingsScreen)

