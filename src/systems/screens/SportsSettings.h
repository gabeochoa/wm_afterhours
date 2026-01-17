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
  size_t selected_tab = 1; // VIDEO tab
  size_t selected_row = 4; // Max FPS selected

  // Colors matching Rematch/FIFA style - dark with bright green accents
  afterhours::Color bg_dark{18, 22, 28, 255};        // Very dark background
  afterhours::Color overlay_dark{25, 32, 42, 200};   // Semi-transparent overlay
  afterhours::Color panel_dark{32, 42, 55, 255};     // Panel background
  afterhours::Color accent_green{85, 195, 145, 255}; // Bright green accent
  afterhours::Color accent_green_dark{55, 155, 105, 255}; // Darker green
  afterhours::Color text_white{235, 240, 245, 255};       // White text
  afterhours::Color text_muted{125, 140, 155, 255};       // Muted text
  afterhours::Color highlight_row{
      45, 75, 95, 160}; // Selected row highlight (semi-transparent)
  afterhours::Color slider_bg{55, 65, 80, 255};    // Slider background
  afterhours::Color slider_track{35, 45, 55, 255}; // Slider track

  std::vector<std::string> tabs = {"GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  struct SettingRow {
    std::string label;
    bool is_slider;
    float slider_pct;
    int min_val; // For slider value mapping
    int max_val;
    std::vector<std::string> options; // For non-sliders
    size_t option_idx;                // Current option index
  };

  std::vector<SettingRow> graphics_settings = {
      {"Window mode",
       false,
       0.0f,
       0,
       0,
       {"Fullscreen", "Borderless window", "Windowed"},
       1},
      {"Resolution",
       false,
       0.0f,
       0,
       0,
       {"1920x1080", "2560x1440", "3840x2160"},
       1},
      {"Screen percentage", true, 1.0f, 50, 100, {}, 0},
      {"V Sync", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Max FPS", true, 0.5f, 30, 240, {}, 0},
      {"Gamma", true, 0.45f, 10, 30, {}, 0}, // 1.0-3.0 scaled by 10
      {"Anti-Aliasing method",
       false,
       0.0f,
       0,
       0,
       {"Off", "FXAA", "TAA", "TSR", "DLSS"},
       3},
      {"Dynamic resolution", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Framerate target", true, 0.4f, 30, 120, {}, 0},
      {"Motion blur", true, 0.7f, 0, 10, {}, 0},
      {"Graphics quality",
       false,
       0.0f,
       0,
       0,
       {"Low", "Medium", "High", "Ultra"},
       3},
      {"Texture quality",
       false,
       0.0f,
       0,
       0,
       {"Low", "Medium", "High", "Ultra"},
       3},
  };

  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty",
       false,
       0.0f,
       0,
       0,
       {"Beginner", "Amateur", "Semi-Pro", "Professional", "World Class",
        "Legendary"},
       3},
      {"Game Speed", true, 0.5f, 0, 100, {}, 0},
      {"Match Length",
       false,
       0.0f,
       0,
       0,
       {"4 min", "6 min", "8 min", "10 min", "15 min", "20 min"},
       2},
      {"Camera Type",
       false,
       0.0f,
       0,
       0,
       {"Broadcast", "Co-op", "Dynamic", "End to End", "Pro"},
       0},
      {"Camera Height", true, 0.5f, 0, 20, {}, 0},
      {"Camera Zoom", true, 0.5f, 0, 20, {}, 0},
      {"Ball Indicator", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Player Names",
       false,
       0.0f,
       0,
       0,
       {"Off", "Always", "When Controlled"},
       2},
  };

  std::vector<SettingRow> audio_settings = {
      {"Master Volume", true, 0.8f, 0, 100, {}, 0},
      {"Music Volume", true, 0.7f, 0, 100, {}, 0},
      {"Commentary Volume", true, 0.9f, 0, 100, {}, 0},
      {"Crowd Volume", true, 0.85f, 0, 100, {}, 0},
      {"SFX Volume", true, 0.75f, 0, 100, {}, 0},
      {"Commentary Language",
       false,
       0.0f,
       0,
       0,
       {"English", "Spanish", "French", "German", "Italian"},
       0},
      {"Crowd Chants", false, 0.0f, 0, 0, {"Off", "On"}, 1},
  };

  std::vector<SettingRow> controls_settings = {
      {"Vibration", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Vibration Strength", true, 0.7f, 0, 100, {}, 0},
      {"Auto Switch",
       false,
       0.0f,
       0,
       0,
       {"Off", "Ball Only", "Air Balls", "All"},
       2},
      {"Pass Assistance", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 2},
      {"Shot Assistance", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 1},
      {"Through Ball", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 1},
      {"Crossing", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 2},
  };

  std::vector<SettingRow> &get_current_settings() {
    switch (selected_tab) {
    case 0:
      return gameplay_settings;
    case 1:
      return graphics_settings;
    case 2:
      return audio_settings;
    case 3:
      return controls_settings;
    default:
      return graphics_settings;
    }
  }

  std::string get_section_header() {
    switch (selected_tab) {
    case 0:
      return "GAMEPLAY";
    case 1:
      return "GRAPHICS";
    case 2:
      return "AUDIO";
    case 3:
      return "CONTROLS";
    default:
      return "GRAPHICS";
    }
  }

  // Helper to format slider value for display - works with current tab's
  // settings
  std::string format_slider_value(size_t index, float pct) {
    std::vector<SettingRow> *settings_ptr = nullptr;
    switch (selected_tab) {
    case 0:
      settings_ptr = &gameplay_settings;
      break;
    case 1:
      settings_ptr = &graphics_settings;
      break;
    case 2:
      settings_ptr = &audio_settings;
      break;
    case 3:
      settings_ptr = &controls_settings;
      break;
    default:
      settings_ptr = &graphics_settings;
      break;
    }

    if (index >= settings_ptr->size())
      return "---";
    auto &setting = (*settings_ptr)[index];
    int val = setting.min_val +
              static_cast<int>(pct * (setting.max_val - setting.min_val));

    // Special case for gamma in graphics tab
    if (selected_tab == 1 &&
        index == 5) { // Gamma - divide by 10 for decimal display
      return fmt::format("{:.1f}", val / 10.0f);
    }
    return std::to_string(val);
  }

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

    // ========== SYNC SELECTION WITH FOCUS ==========
    // Update selected_row based on focus FIRST (before keyboard handling)
    // This ensures arrow keys work on the currently focused row
    auto &current_settings = get_current_settings();
    auto focus_id = context.focus_id;
    auto opt_focused = afterhours::EntityHelper::getEntityForID(focus_id);
    if (opt_focused.has_value()) {
      auto &focused = opt_focused.asE();
      if (focused.has<afterhours::ui::HasLabel>()) {
        auto &label = focused.get<afterhours::ui::HasLabel>().label;
        for (size_t i = 0; i < current_settings.size(); i++) {
          if (label == current_settings[i].label) {
            selected_row = i;
            break;
          }
        }
      }
    }

    // ========== KEYBOARD INPUT HANDLING ==========
    // Handle left/right arrow keys to change value when a row is selected
    if (selected_row < current_settings.size()) {
      auto &setting = current_settings[selected_row];
      float step = 0.05f; // 5% per press for sliders

      if (context.pressed(InputAction::WidgetLeft)) {
        if (setting.is_slider) {
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        } else if (!setting.options.empty()) {
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
        }
      }

      if (context.pressed(InputAction::WidgetRight)) {
        if (setting.is_slider) {
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        } else if (!setting.options.empty()) {
          setting.option_idx =
              (setting.option_idx + 1) % setting.options.size();
        }
      }
    }

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

    // LB button - cycle tabs left (skip tabbing - use keyboard shortcut)
    if (button(context, mk(entity, 5),
               ComponentConfig{}
                   .with_label("LB")
                   .with_size(ComponentSize{pixels(35), pixels(28)})
                   .with_absolute_position()
                   .with_translate(tab_start_x - 50.0f, tab_y + 4.0f)
                   .with_custom_background(panel_dark)
                   .with_border(text_muted, 1.0f)
                   .with_font("EqProRounded", 16.0f)
                   .with_custom_text_color(text_white)
                   .with_alignment(TextAlignment::Center)
                   .with_skip_tabbing(true)
                   .with_debug_name("lb_btn"))) {
      if (selected_tab > 0) {
        selected_tab--;
      } else {
        selected_tab = tabs.size() - 1; // Wrap to last tab
      }
    }

    for (size_t i = 0; i < tabs.size(); i++) {
      float tx = tab_start_x + (float)i * tab_w;
      bool is_selected = (i == selected_tab);
      // Use white for selected (on dark bg), muted for unselected
      afterhours::Color tab_text = is_selected ? text_white : text_muted;

      // Tab buttons skip tabbing - use LB/RB or click to switch
      if (button(
              context, mk(entity, 10 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_label(tabs[i])
                  .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 10)),
                                           pixels(static_cast<int>(tab_h))})
                  .with_absolute_position()
                  .with_translate(tx, tab_y)
                  .with_font("EqProRounded", 16.0f)
                  .with_custom_text_color(tab_text)
                  .with_custom_background(
                      afterhours::Color{0, 0, 0, 0}) // Transparent bg
                  .with_alignment(TextAlignment::Center)
                  .with_skip_tabbing(true)
                  .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }

      // Underline on selected tab
      if (is_selected) {
        div(context, mk(entity, 20 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(tab_w - 20)),
                                         pixels(4)})
                .with_absolute_position()
                .with_translate(tx + 5.0f, tab_y + tab_h + 2.0f)
                .with_custom_background(accent_green)
                .with_debug_name("tab_underline_" + std::to_string(i)));
      }
    }

    // RB button - cycle tabs right (skip tabbing - use keyboard shortcut)
    if (button(context, mk(entity, 6),
               ComponentConfig{}
                   .with_label("RB")
                   .with_size(ComponentSize{pixels(35), pixels(28)})
                   .with_absolute_position()
                   .with_translate(tab_start_x + (float)tabs.size() * tab_w,
                                   tab_y + 4.0f)
                   .with_custom_background(panel_dark)
                   .with_border(text_muted, 1.0f)
                   .with_font("EqProRounded", 16.0f)
                   .with_custom_text_color(text_white)
                   .with_alignment(TextAlignment::Center)
                   .with_skip_tabbing(true)
                   .with_debug_name("rb_btn"))) {
      selected_tab = (selected_tab + 1) % tabs.size();
    }

    // ========== SECTION HEADER ==========
    float header_y = tab_y + 55.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label(get_section_header())
            .with_size(ComponentSize{pixels(150), pixels(30)})
            .with_absolute_position()
            .with_translate(40.0f, header_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("section_header"));

    // ========== LEFT PANEL: Settings ==========
    float panel_x = 40.0f;
    float panel_y = header_y + 40.0f;
    float panel_w = 450.0f;
    float row_h = 42.0f;

    // Reset selected_row if out of bounds for current tab
    if (selected_row >= current_settings.size()) {
      selected_row = 0;
    }

    // Each row is a single tabbable element - the entire row is selectable
    for (size_t i = 0; i < current_settings.size(); i++) {
      float ry = panel_y + (float)i * row_h;
      bool is_selected = (i == selected_row);
      auto &setting = current_settings[i];

      // Get display value based on type
      std::string display_value;
      if (setting.is_slider) {
        display_value = format_slider_value(i, setting.slider_pct);
      } else if (!setting.options.empty()) {
        display_value = setting.options[setting.option_idx];
      } else {
        display_value = "---";
      }

      // Colors based on selection state - always use readable colors
      afterhours::Color label_color = is_selected ? text_white : text_muted;
      afterhours::Color value_color = is_selected ? accent_green : text_muted;
      afterhours::Color arrow_color = is_selected ? text_white : text_muted;

      // Row background - render first (behind everything)
      div(context, mk(entity, 50 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                       pixels(static_cast<int>(row_h - 2))})
              .with_absolute_position()
              .with_translate(panel_x - 10.0f, ry)
              .with_custom_background(is_selected
                                          ? highlight_row
                                          : afterhours::Color{35, 45, 55, 255})
              .with_render_layer(-1)
              .with_debug_name("row_bg_" + std::to_string(i)));

      // Selection indicator - left border accent
      if (is_selected) {
        div(context, mk(entity, 60 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(4),
                                         pixels(static_cast<int>(row_h - 8))})
                .with_absolute_position()
                .with_translate(panel_x - 15.0f, ry + 3.0f)
                .with_custom_background(accent_green)
                .with_render_layer(1)
                .with_debug_name("row_accent_" + std::to_string(i)));
      }

      // Label button - click to select row (this is tabbable)
      if (button(context, mk(entity, 100 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(setting.label)
                     .with_size(ComponentSize{
                         pixels(220), pixels(static_cast<int>(row_h - 4))})
                     .with_absolute_position()
                     .with_translate(panel_x, ry + 2.0f)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(label_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_debug_name("label_" + std::to_string(i)))) {
        selected_row = i;
      }

      float value_x = panel_x + 240.0f;
      float arrow_size = 24.0f;
      float step = 0.05f; // 5% per click for sliders

      // Left arrow < (skip tabbing - use row's keyboard handling) - render
      // above button
      if (button(context, mk(entity, 300 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{
                         pixels(static_cast<int>(arrow_size)), pixels(28)})
                     .with_absolute_position()
                     .with_translate(value_x, ry + 6.0f)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_skip_tabbing(true)
                     .with_render_layer(2)
                     .with_debug_name("left_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider) {
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        } else if (!setting.options.empty()) {
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
        }
      }

      // Value display text (skip tabbing) - render above button
      div(context, mk(entity, 301 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(display_value)
              .with_size(ComponentSize{pixels(130), pixels(28)})
              .with_absolute_position()
              .with_translate(value_x + arrow_size + 5.0f, ry + 6.0f)
              .with_font("EqProRounded", 16.0f)
              .with_custom_text_color(value_color)
              .with_alignment(TextAlignment::Center)
              .with_skip_tabbing(true)
              .with_render_layer(2)
              .with_debug_name("value_" + std::to_string(i)));

      // Right arrow > (skip tabbing - use row's keyboard handling) - render
      // above button
      if (button(context, mk(entity, 302 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{
                         pixels(static_cast<int>(arrow_size)), pixels(28)})
                     .with_absolute_position()
                     .with_translate(value_x + arrow_size + 140.0f, ry + 6.0f)
                     .with_font("EqProRounded", 16.0f)
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_skip_tabbing(true)
                     .with_render_layer(2)
                     .with_debug_name("right_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider) {
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        } else if (!setting.options.empty()) {
          setting.option_idx =
              (setting.option_idx + 1) % setting.options.size();
        }
      }

      // Slider visual (syncs with arrows)
      if (setting.is_slider) {
        slider(context, mk(entity, 500 + static_cast<int>(i)),
               setting.slider_pct,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(130), pixels(12)})
                   .with_absolute_position()
                   .with_translate(value_x + arrow_size + 170.0f, ry + 13.0f)
                   .with_skip_tabbing(true)
                   .with_debug_name("slider_" + std::to_string(i)));
      }
    }

    // ========== RIGHT PANEL: Help/Info ==========
    float help_x = panel_x + panel_w + 100.0f;
    float help_y = 85.0f;
    float help_w = 350.0f;

    auto &selected_setting = current_settings[selected_row];
    std::string current_val =
        selected_setting.is_slider
            ? format_slider_value(selected_row, selected_setting.slider_pct)
            : (selected_setting.options.empty()
                   ? "---"
                   : selected_setting.options[selected_setting.option_idx]);

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label(selected_setting.label)
            .with_size(
                ComponentSize{pixels(static_cast<int>(help_w)), pixels(35)})
            .with_absolute_position()
            .with_translate(help_x, help_y)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_title"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("Adjust this setting to customize your")
            .with_size(
                ComponentSize{pixels(static_cast<int>(help_w)), pixels(50)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 40.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_desc1"));

    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("gaming experience.")
            .with_size(
                ComponentSize{pixels(static_cast<int>(help_w)), pixels(30)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 70.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("help_desc2"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Current: " + current_val)
            .with_size(
                ComponentSize{pixels(static_cast<int>(help_w)), pixels(30)})
            .with_absolute_position()
            .with_translate(help_x, help_y + 110.0f)
            .with_font("EqProRounded", 19.0f)
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
            .with_font("EqProRounded", 19.0f)
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
            .with_font("EqProRounded", 19.0f)
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
            .with_font("EqProRounded", 19.0f)
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
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("back_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (FIFA/Rematch style)",
                        SportsSettingsScreen)
