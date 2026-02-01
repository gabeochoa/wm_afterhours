#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AngryBirdsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  bool music_on = true;
  bool sound_on = true;
  bool vibration_on = false;
  bool notifications_off = true;

  // Colors matching Angry Birds inspiration - warm, playful mobile game
  afterhours::Color bg_green{75, 135, 95,
                             255}; // Softer forest green background
  afterhours::Color header_coral{245, 145, 100,
                                 255}; // Warmer coral/orange header
  afterhours::Color header_dark{215, 105, 60, 255}; // Darker coral border
  afterhours::Color panel_cream{255, 248, 230,
                                255}; // Warm cream panel (lighter)
  afterhours::Color panel_peach{255, 240, 218, 255}; // Inner peach
  afterhours::Color btn_green{115, 195, 85, 255};    // Bright lime green toggle
  afterhours::Color btn_green_dark{85, 160, 55, 255}; // Green shadow
  afterhours::Color btn_blue{85, 155, 210, 255};      // Softer blue pill
  afterhours::Color btn_blue_dark{55, 120, 175, 255}; // Blue shadow
  afterhours::Color text_dark{65, 55, 45, 255};       // Dark brown text
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color close_red{235, 75, 85, 255};   // Brighter close button red
  afterhours::Color wifi_green{75, 195, 135, 255}; // WiFi icon green

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = text_white;
    theme.font_muted = afterhours::Color{120, 100, 80, 255};
    theme.background = bg_green;
    theme.surface = panel_cream;
    theme.primary = header_coral;
    theme.secondary = btn_blue;
    theme.accent = btn_green;
    theme.error = close_red;
    theme.roundness = 0.15f;
    theme.segments = 12;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_green)
            .with_debug_name("bg"));

    // ========== MAIN PANEL ==========
    float panel_w = 720.0f;
    float panel_h = 470.0f;
    float panel_x = ((float)screen_w - panel_w) / 2.0f;
    float panel_y = ((float)screen_h - panel_h) / 2.0f;

    // Panel shadow (reduced)
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x + 4.0f, panel_y + 5.0f)
            .with_custom_background(afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("panel_shadow"));

    // Main panel background (thicker border)
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_border(header_dark, 6.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("panel"));

    // ========== CORAL HEADER (fuller, warmer) ==========
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 12)),
                                     pixels(80)})
            .with_absolute_position()
            .with_translate(panel_x + 6.0f, panel_y + 6.0f)
            .with_custom_background(header_coral)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.10f)
            .with_debug_name("header"));

    // Title (larger, bolder)
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(280), pixels(60)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f - 140.0f, panel_y + 18.0f)
            .with_font("Fredoka", 42.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Close button (X) - positioned inside panel bounds in header
    button(context, mk(entity, 25),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{pixels(44), pixels(44)})
               .with_absolute_position()
               .with_translate(panel_x + panel_w - 56.0f, panel_y + 22.0f)
               .with_custom_background(close_red)
               .with_border(afterhours::Color{175, 55, 65, 255}, 3.0f)
               .with_font("EqProRounded", 20.0f)
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(1.0f)
               .with_debug_name("close_btn"));

    // ========== TOGGLE BUTTONS (Music, Sound, Vibration) ==========
    float toggle_y = panel_y + 110.0f;
    float toggle_x = panel_x + 80.0f;
    float toggle_spacing = 100.0f;

    // Icon symbols with descriptive text labels for clarity
    struct ToggleInfo {
      std::string icon;
      std::string label;
      bool *state;
    };
    std::vector<ToggleInfo> toggles = {
        {"M", "Music", &music_on},
        {"S", "Sound", &sound_on},
        {"V", "Vibrate", &vibration_on},
    };

    for (size_t i = 0; i < toggles.size(); i++) {
      float tx = toggle_x + (float)i * toggle_spacing;
      bool is_on = *toggles[i].state;
      afterhours::Color bg_col =
          is_on ? btn_green : afterhours::Color{165, 165, 165, 255};
      afterhours::Color border_col =
          is_on ? btn_green_dark : afterhours::Color{125, 125, 125, 255};

      // Icon button
      if (button(context, mk(entity, 30 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(toggles[i].icon)
                     .with_size(ComponentSize{pixels(75), pixels(55)})
                     .with_absolute_position()
                     .with_translate(tx, toggle_y)
                     .with_custom_background(bg_col)
                     .with_border(border_col, 5.0f)
                     .with_font("EqProRounded", 28.0f)
                     .with_custom_text_color(text_white)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.6f)
                     .with_soft_shadow(2.0f, 3.0f, 6.0f,
                                       afterhours::Color{0, 0, 0, 40})
                     .with_debug_name("toggle_" + std::to_string(i)))) {
        *toggles[i].state = !(*toggles[i].state);
      }

      // Text label below the button for clarity
      div(context, mk(entity, 35 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(toggles[i].label)
              .with_size(ComponentSize{pixels(75), pixels(24)})
              .with_absolute_position()
              .with_translate(tx, toggle_y + 62.0f)
              .with_font("EqProRounded", 18.0f)
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("toggle_label_" + std::to_string(i)));
    }

    // Save/Load Progress button
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(200), pixels(70)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 240.0f, toggle_y)
            .with_custom_background(panel_peach)
            .with_border(afterhours::Color{200, 185, 165, 255}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("save_load_bg"));

    div(context, mk(entity, 41),
        ComponentConfig{}
            .with_label("Save/Load")
            .with_size(ComponentSize{pixels(130), pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 225.0f, toggle_y + 8.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("save_load_title"));

    div(context, mk(entity, 42),
        ComponentConfig{}
            .with_label("Progress")
            .with_size(ComponentSize{pixels(130), pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 225.0f, toggle_y + 32.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("save_load_progress"));

    // WiFi/Cloud sync icon with label
    div(context, mk(entity, 43),
        ComponentConfig{}
            .with_label("W")
            .with_size(ComponentSize{pixels(50), pixels(36)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 85.0f, toggle_y + 5.0f)
            .with_custom_background(wifi_green)
            .with_border(afterhours::Color{55, 165, 115, 255}, 3.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("wifi_icon"));

    // WiFi label for clarity
    div(context, mk(entity, 44),
        ComponentConfig{}
            .with_label("Sync")
            .with_size(ComponentSize{pixels(50), pixels(20)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 85.0f, toggle_y + 46.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("wifi_label"));

    // ========== BLUE PILL BUTTONS ==========
    float btn_y1 = toggle_y + 100.0f;
    float btn_y2 = btn_y1 + 65.0f;
    float btn_y3 = btn_y2 + 65.0f;
    float left_btn_x = panel_x + 55.0f;
    float right_btn_x = panel_x + panel_w / 2.0f + 25.0f;
    float btn_w = 300.0f;
    float btn_h = 50.0f;

    // Notifications: ON/OFF (clickable)
    std::string notif_text =
        notifications_off ? "Notifications: OFF" : "Notifications: ON";
    afterhours::Color notif_icon_color =
        notifications_off ? close_red : btn_green;
    std::string notif_icon = notifications_off ? "Ø" : "✓";

    if (button(context, mk(entity, 50),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                            pixels(static_cast<int>(btn_h))})
                   .with_absolute_position()
                   .with_translate(left_btn_x, btn_y1)
                   .with_custom_background(btn_blue)
                   .with_border(btn_blue_dark, 4.0f)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(0.5f)
                   .with_soft_shadow(2.0f, 4.0f, 8.0f,
                                     afterhours::Color{0, 0, 0, 40})
                   .with_debug_name("notifications_btn"))) {
      notifications_off = !notifications_off;
    }

    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label(notif_icon)
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(left_btn_x + 18.0f, btn_y1 + 10.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(notif_icon_color)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("notif_icon"));

    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label(notif_text)
            .with_size(ComponentSize{pixels(220), pixels(32)})
            .with_absolute_position()
            .with_translate(left_btn_x + 50.0f, btn_y1 + 8.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("notif_text"));

    // Credits
    button(
        context, mk(entity, 55),
        ComponentConfig{}
            .with_label("Credits")
            .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                     pixels(static_cast<int>(btn_h))})
            .with_absolute_position()
            .with_translate(right_btn_x, btn_y1)
            .with_custom_background(btn_blue)
            .with_border(btn_blue_dark, 4.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("credits"));

    // Language
    button(
        context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Language")
            .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                     pixels(static_cast<int>(btn_h))})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y2)
            .with_custom_background(btn_blue)
            .with_border(btn_blue_dark, 4.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("language"));

    // Support
    button(
        context, mk(entity, 65),
        ComponentConfig{}
            .with_label("Support")
            .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                     pixels(static_cast<int>(btn_h))})
            .with_absolute_position()
            .with_translate(right_btn_x, btn_y2)
            .with_custom_background(btn_blue)
            .with_border(btn_blue_dark, 4.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("support"));

    // ========== BOTTOM INFO ==========
    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_label("15555-1-114203-20-10200-01")
            .with_size(ComponentSize{pixels(280), pixels(24)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 6.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("build_id"));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_label("Version 1.11.0.12346")
            .with_size(ComponentSize{pixels(220), pixels(22)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 26.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("version"));

    div(context, mk(entity, 72),
        ComponentConfig{}
            .with_label("Player ID: 281676956389")
            .with_size(ComponentSize{pixels(220), pixels(22)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 46.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("player_id"));

    // Terms and Privacy
    button(
        context, mk(entity, 75),
        ComponentConfig{}
            .with_label("Terms and Privacy")
            .with_size(ComponentSize{pixels(static_cast<int>(btn_w)),
                                     pixels(static_cast<int>(btn_h))})
            .with_absolute_position()
            .with_translate(right_btn_x, btn_y3)
            .with_custom_background(btn_blue)
            .with_border(btn_blue_dark, 4.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("terms"));
  }
};

REGISTER_EXAMPLE_SCREEN(angry_birds_settings, "Game Mockups",
                        "Mobile game settings (Angry Birds style)",
                        AngryBirdsSettingsScreen)
