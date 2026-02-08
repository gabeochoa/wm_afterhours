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

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(16.0f));
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
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

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
            .with_absolute_position(25.0f, top_y + 5.0f)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    // SETTINGS title
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(120), pixels(30)})
            .with_absolute_position(65.0f, top_y + 5.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(text_white));

    // NOTE: HUD elements (currency/resources) are intentionally hidden in settings
    // to avoid confusion - they serve no purpose in this context

    // ========== MAIN PANEL ==========
    float panel_x = 55.0f;
    float panel_y = 60.0f;
    float panel_w = 480.0f;
    float panel_h = 380.0f;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_blue)
            .with_border(panel_border, 2.0f)
            .with_debug_name("main_panel"));

    // ========== SETTINGS ROWS ==========
    float row_x = panel_x + 15.0f;
    float row_y = panel_y + 15.0f;
    float row_h = 36.0f;
    float label_w = 140.0f;
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
                     .with_size(ComponentSize{pxf(label_w),
                                              pixels(44)})
                     .with_absolute_position(row_x, ry - 8.0f)
                     .with_font("EqProRounded", h720(18.0f))
                     .with_custom_text_color(label_color))) {
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
                     .with_size(ComponentSize{pixels(44), pixels(44)})
                     .with_absolute_position(row_x + label_w + 20.0f, ry - 8.0f)
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", h720(18.0f))
                     .with_custom_text_color(arrow_color)
                     .with_alignment(TextAlignment::Center))) {
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
                  pxf(dropdown_w - 92), pixels(44)})
              .with_absolute_position(row_x + label_w + 65.0f, ry - 8.0f)
              .with_custom_background(dd_bg)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center));

      // Right arrow >
      if (button(context, mk(entity, 53 + static_cast<int>(i) * 4),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{pixels(44), pixels(44)})
                     .with_absolute_position(
                         row_x + label_w + 20.0f + dropdown_w - 24.0f, ry - 8.0f)
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", h720(18.0f))
                     .with_custom_text_color(arrow_color)
                     .with_alignment(TextAlignment::Center))) {
        selected_row = i;
        auto &setting = current_settings[i];
        setting.option_idx = (setting.option_idx + 1) % setting.options.size();
      }
    }

    // ========== HELP TEXT PANEL ==========
    float help_x = panel_x + panel_w + 15.0f;
    float help_y = panel_y;
    float help_w = 220.0f;
    float help_h = 140.0f;

    div(context, mk(entity, 150),
        ComponentConfig{}
            .with_720p_size(help_w, help_h)
            .with_absolute_position(help_x, help_y)
            .with_custom_background(panel_blue)
            .with_border(panel_border, 2.0f)
            .with_debug_name("help_panel"));

    // Dynamic help text based on selected setting
    std::string setting_name = current_settings[selected_row].label;
    std::string help_title = "Help: " + setting_name;
    std::string help_line1 = "Adjust the " + setting_name;
    std::string help_line2 = "setting to your preference.";
    std::string current_val =
        "Current: " + current_settings[selected_row]
                          .options[current_settings[selected_row].option_idx];

    struct HelpLine { int id; const std::string &text; int h; float y_off; float font_sz; afterhours::Color color; };
    HelpLine help_lines[] = {
        {151, help_title, 28, 12.0f, 17.0f, text_cyan},
        {152, help_line1, 25, 45.0f, 13.0f, text_white},
        {153, help_line2, 25, 65.0f, 13.0f, text_white},
        {154, current_val, 25, 100.0f, 13.0f, text_muted},
    };
    for (auto &hl : help_lines) {
      div(context, mk(entity, hl.id),
          ComponentConfig{}
              .with_label(hl.text)
              .with_size(ComponentSize{pxf(help_w - 24), pixels(hl.h)})
              .with_absolute_position(help_x + 12.0f, help_y + hl.y_off)
              .with_font("EqProRounded", h720(hl.font_sz))
              .with_custom_text_color(hl.color));
    }

    // ========== BOTTOM TAB BAR ==========
    float tab_y = panel_y + panel_h + 15.0f;
    float tab_w = 95.0f;
    float tab_h = 44.0f;
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
                  .with_720p_size(tab_w - 6, tab_h)
                  .with_absolute_position(tx, tab_y)
                  .with_custom_background(tab_bg)
                  .with_border(panel_border, 1.0f)
                  .with_font("EqProRounded", h720(14.0f))
                  .with_custom_text_color(tab_text)
                  .with_alignment(TextAlignment::Center))) {
        selected_tab = i;
      }

      // Underline on selected - thicker for visibility
      if (is_selected) {
        div(context, mk(entity, 210 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(tab_w - 10),
                                         pixels(5)})
                .with_absolute_position(tx + 2.0f, tab_y + tab_h - 7.0f)
                .with_custom_background(highlight_blue));
      }
    }

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = panel_y + panel_h + 55.0f;
    float prompt_x = panel_x;

    struct Prompt { const char *icon; const char *label; int icon_id; int label_id; float x_off; int label_w; };
    afterhours::Color prompt_bg{55, 75, 95, 255};
    Prompt prompts[] = {
        {"X", "Close", 300, 301, 0.0f, 50},
        {"O", "Reset", 302, 303, 100.0f, 50},
        {"[]", "Select", 304, 305, 195.0f, 55},
    };
    for (auto &p : prompts) {
      div(context, mk(entity, p.icon_id),
          ComponentConfig{}
              .with_label(p.icon)
              .with_size(ComponentSize{pixels(28), pixels(28)})
              .with_absolute_position(prompt_x + p.x_off, prompt_y)
              .with_custom_background(prompt_bg)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center));
      div(context, mk(entity, p.label_id),
          ComponentConfig{}
              .with_label(p.label)
              .with_size(ComponentSize{pixels(p.label_w), pixels(25)})
              .with_absolute_position(prompt_x + p.x_off + 35.0f, prompt_y + 2.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(text_white));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(powerwash_settings, "Game Mockups",
                        "Simulation game settings (PowerWash style)",
                        PowerWashSettingsScreen)
