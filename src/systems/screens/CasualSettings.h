#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CasualSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  bool music_on = true;
  bool sound_on = true;
  bool vibrate_on = false;
  bool show_about = false;

  // Colors matching Angry Birds inspiration - bright, playful mobile aesthetic
  afterhours::Color bg_green{85, 165, 95, 255};
  afterhours::Color panel_orange{245, 165, 100, 255};
  afterhours::Color panel_cream{255, 245, 225, 255};
  afterhours::Color btn_green{120, 200, 65, 255};
  afterhours::Color btn_green_dark{95, 165, 50, 255};
  afterhours::Color btn_blue{85, 175, 220, 255};
  afterhours::Color btn_blue_dark{65, 145, 185, 255};
  afterhours::Color close_red{230, 75, 85, 255};
  afterhours::Color text_dark{55, 45, 40, 255};
  afterhours::Color text_muted{120, 105, 95, 255};
  afterhours::Color white{255, 255, 255, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = white;
    theme.font_muted = text_muted;
    theme.background = bg_green;
    theme.surface = panel_cream;
    theme.primary = panel_orange;
    theme.secondary = btn_blue;
    theme.accent = btn_green;
    theme.error = close_red;
    theme.roundness = 0.18f;
    theme.segments = 16;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BLURRED BACKGROUND (simulated with gradient) ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_green)
            .with_debug_name("bg"));

    // ========== MAIN PANEL ==========
    float panel_w = 680.0f;
    float panel_h = 420.0f;
    float panel_x = ((float)screen_w - panel_w) / 2.0f;
    float panel_y = ((float)screen_h - panel_h) / 2.0f;

    // Orange border/frame
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w + 16)),
                                     pixels(static_cast<int>(panel_h + 16))})
            .with_absolute_position()
            .with_translate(panel_x - 8.0f, panel_y - 8.0f)
            .with_custom_background(panel_orange)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("panel_border"));

    // Cream inner panel
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("panel_inner"));

    // ========== TITLE: Settings ==========
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(200), pixels(45)})
            .with_absolute_position()
            .with_translate((float)screen_w / 2.0f - 100.0f, panel_y - 50.0f)
            .with_font("Gaegu-Bold", h720(38.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // ========== CLOSE BUTTON (X) ==========
    if (button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("X")
                   .with_size(ComponentSize{pixels(52), pixels(52)})
                   .with_absolute_position()
                   .with_translate(panel_x + panel_w - 30.0f, panel_y - 10.0f)
                   .with_custom_background(close_red)
                   .with_border(afterhours::Color{190, 55, 65, 255}, 4.0f)
                   .with_font("Gaegu-Bold", h720(28.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(1.0f)
                   .with_debug_name("close_btn"))) {
      // Close action
    }

    // ========== TOGGLE ICONS (Music, Sound, Vibrate) ==========
    float toggle_y = panel_y + 45.0f;
    float toggle_start_x = panel_x + 95.0f;
    float toggle_spacing = 75.0f;

    std::vector<std::tuple<std::string, bool *, std::string>> toggles = {
        {"M", &music_on, "music"},     // Music note icon
        {"S", &sound_on, "sound"},     // Speaker icon
        {"V", &vibrate_on, "vibrate"}  // Vibrate icon
    };

    // Display names for the toggle labels
    std::vector<std::string> toggle_labels = {"Music", "Sound", "Vibrate"};

    for (size_t i = 0; i < toggles.size(); i++) {
      auto &[icon, state_ptr, name] = toggles[i];
      float tx = toggle_start_x + (float)i * toggle_spacing;
      afterhours::Color toggle_bg = *state_ptr ? btn_green : text_muted;
      afterhours::Color toggle_border =
          *state_ptr ? btn_green_dark : afterhours::Color{95, 85, 75, 255};

      if (button(context, mk(entity, 40 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(icon)
                     .with_size(ComponentSize{pixels(58), pixels(58)})
                     .with_absolute_position()
                     .with_translate(tx, toggle_y)
                     .with_custom_background(toggle_bg)
                     .with_border(toggle_border, 4.0f)
                     .with_font("Gaegu-Bold", h720(24.0f))
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(1.0f)
                     .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                       afterhours::Color{0, 0, 0, 35})
                     .with_debug_name("toggle_" + name))) {
        *state_ptr = !*state_ptr;
      }

      // Add visible label below the toggle icon
      div(context, mk(entity, 60 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(toggle_labels[i])
              .with_size(ComponentSize{pixels(58), pixels(18)})
              .with_absolute_position()
              .with_translate(tx, toggle_y + 62.0f)
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("label_" + name));
    }

    // Wifi icon (positioned first, then Save/Load button to its left)
    float wifi_x = panel_x + panel_w - 75.0f;
    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("((*))")
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_absolute_position()
            .with_translate(wifi_x, toggle_y + 5.0f)
            .with_custom_background(btn_green)
            .with_font("Gaegu-Bold", h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("wifi_icon"));

    // Save/Load Progress button (positioned to the left of wifi icon with gap)
    button(context, mk(entity, 50),
           ComponentConfig{}
               .with_label("Save/Load Progress")
               .with_size(ComponentSize{pixels(210), pixels(55)})
               .with_absolute_position()
               .with_translate(wifi_x - 220.0f, toggle_y)
               .with_custom_background(white)
               .with_border(afterhours::Color{200, 195, 185, 255}, 3.0f)
               .with_font("Gaegu-Bold", h720(18.0f))
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("save_load"));

    // ========== MENU BUTTONS ==========
    std::vector<std::string> left_buttons = {"Notifications: OFF", "Language"};
    std::vector<std::string> right_buttons = {"Credits", "Support",
                                              "Terms and Privacy"};

    float btn_w = 280.0f;
    float btn_h = 55.0f;
    float left_x = panel_x + 40.0f;
    float right_x = panel_x + panel_w - btn_w - 40.0f;
    float row_y = panel_y + 145.0f;
    float row_spacing = 70.0f;

    // Left column
    for (size_t i = 0; i < left_buttons.size(); i++) {
      button(context, mk(entity, 100 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(left_buttons[i])
                 .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                          pixels(static_cast<int>(btn_h))})
                 .with_absolute_position()
                 .with_translate(left_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", h720(22.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.5f)
                 .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                   afterhours::Color{0, 0, 0, 30})
                 .with_debug_name("left_btn_" + std::to_string(i)));
    }

    // Right column
    for (size_t i = 0; i < right_buttons.size(); i++) {
      button(context, mk(entity, 110 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(right_buttons[i])
                 .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                          pixels(static_cast<int>(btn_h))})
                 .with_absolute_position()
                 .with_translate(right_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", h720(22.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.5f)
                 .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                   afterhours::Color{0, 0, 0, 30})
                 .with_debug_name("right_btn_" + std::to_string(i)));
    }

    // ========== ABOUT BUTTON (replaces inline version info) ==========
    float info_y = panel_y + panel_h - 75.0f;

    if (button(context, mk(entity, 200),
               ComponentConfig{}
                   .with_label("About")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_absolute_position()
                   .with_translate(left_x, info_y + 10.0f)
                   .with_custom_background(btn_blue)
                   .with_border(btn_blue_dark, 3.0f)
                   .with_font("Gaegu-Bold", h720(20.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(0.5f)
                   .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                     afterhours::Color{0, 0, 0, 30})
                   .with_debug_name("about_btn"))) {
      show_about = !show_about;
    }

    // Version display (simple, non-technical)
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Version 1.11.0")
            .with_size(ComponentSize{pixels(150), pixels(20)})
            .with_absolute_position()
            .with_translate(left_x + 135.0f, info_y + 22.0f)
            .with_font("Gaegu-Bold", h720(17.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("version_simple"));

    // ========== ABOUT PANEL (shows technical info when toggled) ==========
    if (show_about) {
      // About panel background overlay
      float about_w = 380.0f;
      float about_h = 180.0f;
      float about_x = (float)screen_w / 2.0f - about_w / 2.0f;
      float about_y = (float)screen_h / 2.0f - about_h / 2.0f;

      // Orange border for About panel
      div(context, mk(entity, 300),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(about_w + 12)),
                                       pixels(static_cast<int>(about_h + 12))})
              .with_absolute_position()
              .with_translate(about_x - 6.0f, about_y - 6.0f)
              .with_custom_background(panel_orange)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.12f)
              .with_debug_name("about_border"));

      // Cream inner panel
      div(context, mk(entity, 301),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(about_w)),
                                       pixels(static_cast<int>(about_h))})
              .with_absolute_position()
              .with_translate(about_x, about_y)
              .with_custom_background(panel_cream)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.1f)
              .with_debug_name("about_inner"));

      // About title
      div(context, mk(entity, 302),
          ComponentConfig{}
              .with_label("About")
              .with_size(ComponentSize{pixels(100), pixels(30)})
              .with_absolute_position()
              .with_translate(about_x + about_w / 2.0f - 50.0f, about_y + 12.0f)
              .with_font("Gaegu-Bold", h720(24.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("about_title"));

      // Build number
      div(context, mk(entity, 303),
          ComponentConfig{}
              .with_label("Build: 15555-1-114203-20-10200-01")
              .with_size(ComponentSize{pixels(340), pixels(22)})
              .with_absolute_position()
              .with_translate(about_x + 20.0f, about_y + 50.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("about_build"));

      // Full version
      div(context, mk(entity, 304),
          ComponentConfig{}
              .with_label("Version: 1.11.0.12346")
              .with_size(ComponentSize{pixels(220), pixels(22)})
              .with_absolute_position()
              .with_translate(about_x + 20.0f, about_y + 75.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("about_version"));

      // Player ID
      div(context, mk(entity, 305),
          ComponentConfig{}
              .with_label("Player ID: 281676956389")
              .with_size(ComponentSize{pixels(240), pixels(22)})
              .with_absolute_position()
              .with_translate(about_x + 20.0f, about_y + 100.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("about_player_id"));

      // Close About button
      if (button(context, mk(entity, 306),
                 ComponentConfig{}
                     .with_label("Close")
                     .with_size(ComponentSize{pixels(100), pixels(38)})
                     .with_absolute_position()
                     .with_translate(about_x + about_w / 2.0f - 50.0f,
                                     about_y + about_h - 48.0f)
                     .with_custom_background(btn_green)
                     .with_border(btn_green_dark, 3.0f)
                     .with_font("Gaegu-Bold", h720(18.0f))
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.5f)
                     .with_debug_name("about_close"))) {
        show_about = false;
      }
    }

  }
};

REGISTER_EXAMPLE_SCREEN(casual_settings, "Game Mockups",
                        "Casual mobile settings (Angry Birds style)",
                        CasualSettingsScreen)
