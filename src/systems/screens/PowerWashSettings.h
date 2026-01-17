#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PowerWashSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 2; // VIDEO tab selected
  size_t selected_row = 4; // Anti-Aliasing selected

  // Colors matching PowerWash Simulator - dark blue interface
  afterhours::Color bg_dark{25, 45, 75, 255};       // Dark navy blue
  afterhours::Color panel_blue{35, 65, 105, 255};   // Panel background
  afterhours::Color panel_border{55, 95, 145, 255}; // Panel border
  afterhours::Color header_bar{45, 80, 130, 255};   // Header bar
  afterhours::Color text_white{235, 240, 245, 255}; // White text
  afterhours::Color text_cyan{145, 215, 245, 255};  // Cyan accent text
  afterhours::Color text_muted{165, 190, 215,
                               255}; // Muted text (lighter for readability)
  afterhours::Color dropdown_bg{55, 90, 140, 255};      // Dropdown background
  afterhours::Color dropdown_border{75, 120, 175, 255}; // Dropdown border
  afterhours::Color tab_selected{65, 105, 165, 255};    // Selected tab
  afterhours::Color highlight_blue{85, 145, 215, 255};  // Highlight color

  std::vector<std::string> tabs = {"GENERAL", "GAMEPLAY", "VIDEO", "AUDIO",
                                   "CONTROLS"};

  struct SettingRow {
    std::string label;
    std::vector<std::string> options;
    size_t option_idx;
  };

  std::vector<SettingRow> video_settings = {
      {"Screen Resolution", {"1920 x 1080", "2560 x 1440", "3840 x 2160"}, 1},
      {"Window Mode", {"Fullscreen", "Borderless Windowed", "Windowed"}, 1},
      {"Target Framerate", {"30", "60", "120", "Unlimited"}, 3},
      {"Vsync", {"Off", "On"}, 1},
      {"Anti-Aliasing", {"Off", "2x", "4x", "8x"}, 1},
      {"SSAO", {"Off", "2x", "4x"}, 1},
      {"Render Scale", {"Off", "75%", "100%", "125%"}, 0},
      {"Model Quality", {"Low", "Medium", "High", "Ultra"}, 2},
      {"Texture Quality", {"Low", "Medium", "High", "Highest"}, 3},
  };

  std::vector<SettingRow> general_settings = {
      {"Language", {"English", "Spanish", "French", "German"}, 0},
      {"Subtitles", {"Off", "On"}, 1},
      {"Colorblind Mode",
       {"Off", "Deuteranopia", "Protanopia", "Tritanopia"},
       0},
  };

  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty", {"Easy", "Normal", "Hard"}, 1},
      {"Camera Sensitivity", {"Low", "Medium", "High"}, 1},
      {"Invert Y Axis", {"Off", "On"}, 0},
  };

  std::vector<SettingRow> audio_settings = {
      {"Master Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Music Volume", {"0%", "25%", "50%", "75%", "100%"}, 3},
      {"SFX Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Voice Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
  };

  std::vector<SettingRow> controls_settings = {
      {"Controller Vibration", {"Off", "On"}, 1},
      {"Button Layout", {"Default", "Alternate A", "Alternate B"}, 0},
  };

  std::vector<SettingRow> &get_current_settings() {
    switch (selected_tab) {
    case 0:
      return general_settings;
    case 1:
      return gameplay_settings;
    case 2:
      return video_settings;
    case 3:
      return audio_settings;
    case 4:
      return controls_settings;
    default:
      return video_settings;
    }
  }

  // Currency/resource display values
  int time_val = 10;
  int water_val = 200;
  int star_val = 1;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = panel_blue;
    theme.primary = highlight_blue;
    theme.secondary = panel_border;
    theme.accent = text_cyan;
    theme.error = afterhours::Color{180, 80, 80, 255};
    theme.roundness = 0.08f;
    theme.segments = 6;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== TOP BAR: Settings title and resources ==========
    float top_y = 20.0f;

    // X close button
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("X")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_absolute_position()
            .with_translate(25.0f, top_y + 5.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("close"));

    // SETTINGS title
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(120), pixels(30)})
            .with_absolute_position()
            .with_translate(65.0f, top_y + 5.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("title"));

    // Resource icons on right
    float res_x = (float)screen_w - 250.0f;

    // Time
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("@" + std::to_string(time_val) + ".00")
            .with_size(ComponentSize{pixels(80), pixels(25)})
            .with_absolute_position()
            .with_translate(res_x, top_y + 5.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_cyan)
            .with_debug_name("time"));

    // Water
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("%" + std::to_string(water_val))
            .with_size(ComponentSize{pixels(70), pixels(25)})
            .with_absolute_position()
            .with_translate(res_x + 90.0f, top_y + 5.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_cyan)
            .with_debug_name("water"));

    // Star
    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("*" + std::to_string(star_val))
            .with_size(ComponentSize{pixels(40), pixels(25)})
            .with_absolute_position()
            .with_translate(res_x + 170.0f, top_y + 5.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_cyan)
            .with_debug_name("star"));

    // ========== MAIN PANEL ==========
    float panel_x = 55.0f;
    float panel_y = 60.0f;
    float panel_w = 600.0f;
    float panel_h = 420.0f;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_blue)
            .with_border(panel_border, 2.0f)
            .with_debug_name("main_panel"));

    // ========== SETTINGS ROWS ==========
    float row_x = panel_x + 20.0f;
    float row_y = panel_y + 20.0f;
    float row_h = 38.0f;
    float label_w = 200.0f;
    float dropdown_w = 220.0f;

    auto &current_settings = get_current_settings();

    // Reset selected_row if it's out of bounds for current tab
    if (selected_row >= current_settings.size()) {
      selected_row = 0;
    }

    for (size_t i = 0; i < current_settings.size(); i++) {
      float ry = row_y + (float)i * row_h;
      bool is_selected = (i == selected_row);
      afterhours::Color label_color = is_selected ? text_white : text_muted;

      // Label - clicking selects the row
      if (button(context, mk(entity, 50 + static_cast<int>(i) * 4),
                 ComponentConfig{}
                     .with_label(current_settings[i].label)
                     .with_size(ComponentSize{pixels(static_cast<int>(label_w)),
                                              pixels(28)})
                     .with_absolute_position()
                     .with_translate(row_x, ry)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(label_color)
                     .with_debug_name("label_" + std::to_string(i)))) {
        selected_row = i;
      }

      // Dropdown with < > arrows for cycling values
      afterhours::Color dd_bg = is_selected ? dropdown_bg : panel_blue;
      afterhours::Color dd_border =
          is_selected ? dropdown_border : panel_border;
      afterhours::Color arrow_color = is_selected ? text_white : text_muted;

      // Left arrow <
      if (button(context, mk(entity, 51 + static_cast<int>(i) * 4),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{pixels(24), pixels(28)})
                     .with_absolute_position()
                     .with_translate(row_x + label_w + 40.0f, ry)
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(arrow_color)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("left_" + std::to_string(i)))) {
        selected_row = i;
        auto &setting = current_settings[i];
        setting.option_idx = (setting.option_idx == 0)
                                 ? setting.options.size() - 1
                                 : setting.option_idx - 1;
      }

      // Value display
      div(context, mk(entity, 52 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label(
                  current_settings[i].options[current_settings[i].option_idx])
              .with_size(ComponentSize{
                  pixels(static_cast<int>(dropdown_w - 54)), pixels(28)})
              .with_absolute_position()
              .with_translate(row_x + label_w + 66.0f, ry)
              .with_custom_background(dd_bg)
              .with_font("EqProRounded", 14.0f)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("value_" + std::to_string(i)));

      // Right arrow >
      if (button(context, mk(entity, 53 + static_cast<int>(i) * 4),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{pixels(24), pixels(28)})
                     .with_absolute_position()
                     .with_translate(
                         row_x + label_w + 40.0f + dropdown_w - 26.0f, ry)
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(arrow_color)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("right_" + std::to_string(i)))) {
        selected_row = i;
        auto &setting = current_settings[i];
        setting.option_idx = (setting.option_idx + 1) % setting.options.size();
      }
    }

    // ========== HELP TEXT PANEL ==========
    float help_x = panel_x + panel_w + 30.0f;
    float help_y = panel_y;
    float help_w = 280.0f;
    float help_h = 150.0f;

    div(context, mk(entity, 150),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(help_w)),
                                     pixels(static_cast<int>(help_h))})
            .with_absolute_position()
            .with_translate(help_x, help_y)
            .with_custom_background(panel_blue)
            .with_border(panel_border, 2.0f)
            .with_debug_name("help_panel"));

    // Dynamic help text based on selected setting
    std::string setting_name = current_settings[selected_row].label;
    std::string help_line1 = "Adjust the " + setting_name;
    std::string help_line2 = "setting to your preference.";
    std::string current_val =
        "Current: " + current_settings[selected_row]
                          .options[current_settings[selected_row].option_idx];

    div(context, mk(entity, 151),
        ComponentConfig{}
            .with_label(setting_name)
            .with_size(ComponentSize{pixels(static_cast<int>(help_w - 30)),
                                     pixels(28)})
            .with_absolute_position()
            .with_translate(help_x + 15.0f, help_y + 15.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_cyan)
            .with_debug_name("help_title"));

    div(context, mk(entity, 152),
        ComponentConfig{}
            .with_label(help_line1)
            .with_size(ComponentSize{pixels(static_cast<int>(help_w - 30)),
                                     pixels(25)})
            .with_absolute_position()
            .with_translate(help_x + 15.0f, help_y + 50.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help1"));

    div(context, mk(entity, 153),
        ComponentConfig{}
            .with_label(help_line2)
            .with_size(ComponentSize{pixels(static_cast<int>(help_w - 30)),
                                     pixels(25)})
            .with_absolute_position()
            .with_translate(help_x + 15.0f, help_y + 72.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help2"));

    div(context, mk(entity, 154),
        ComponentConfig{}
            .with_label(current_val)
            .with_size(ComponentSize{pixels(static_cast<int>(help_w - 30)),
                                     pixels(25)})
            .with_absolute_position()
            .with_translate(help_x + 15.0f, help_y + 110.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("help_current"));

    // ========== BOTTOM TAB BAR ==========
    float tab_y = panel_y + panel_h + 15.0f;
    float tab_w = 110.0f;
    float tab_h = 35.0f;
    float tab_total = tab_w * (float)tabs.size();
    float tab_start_x = panel_x + (panel_w - tab_total) / 2.0f;

    for (size_t i = 0; i < tabs.size(); i++) {
      float tx = tab_start_x + (float)i * tab_w;
      bool is_selected = (i == selected_tab);
      afterhours::Color tab_bg = is_selected ? tab_selected : panel_blue;
      afterhours::Color tab_text = is_selected ? text_white : text_muted;

      if (button(
              context, mk(entity, 200 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_label(tabs[i])
                  .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 4)),
                                           pixels(static_cast<int>(tab_h))})
                  .with_absolute_position()
                  .with_translate(tx, tab_y)
                  .with_custom_background(tab_bg)
                  .with_border(panel_border, 1.0f)
                  .with_font("EqProRounded", 14.0f)
                  .with_custom_text_color(tab_text)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }

      // Underline on selected
      if (is_selected) {
        div(context, mk(entity, 210 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 8)),
                                         pixels(3)})
                .with_absolute_position()
                .with_translate(tx + 2.0f, tab_y + tab_h - 5.0f)
                .with_custom_background(highlight_blue)
                .with_debug_name("tab_underline_" + std::to_string(i)));
      }
    }

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = (float)screen_h - 45.0f;
    float prompt_x = panel_x;

    // X Close
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("X")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(prompt_x, prompt_y)
            .with_custom_background(afterhours::Color{55, 75, 95, 255})
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("x_btn"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("Close")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 35.0f, prompt_y + 2.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("close_label"));

    // O Reset
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("O")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(prompt_x + 100.0f, prompt_y)
            .with_custom_background(afterhours::Color{55, 75, 95, 255})
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("o_btn"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Reset")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 135.0f, prompt_y + 2.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("reset_label"));

    // [] Select
    div(context, mk(entity, 304),
        ComponentConfig{}
            .with_label("[]")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(prompt_x + 195.0f, prompt_y)
            .with_custom_background(afterhours::Color{55, 75, 95, 255})
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("square_btn"));

    div(context, mk(entity, 305),
        ComponentConfig{}
            .with_label("Select")
            .with_size(ComponentSize{pixels(55), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 230.0f, prompt_y + 2.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("select_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(powerwash_settings, "Game Mockups",
                        "Simulation game settings (PowerWash style)",
                        PowerWashSettingsScreen)
