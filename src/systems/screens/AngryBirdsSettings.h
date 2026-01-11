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
  afterhours::Color bg_green{95, 145, 85, 255};          // Forest green background
  afterhours::Color header_coral{235, 135, 95, 255};     // Coral/orange header
  afterhours::Color header_dark{195, 95, 55, 255};       // Darker coral border
  afterhours::Color panel_cream{255, 245, 225, 255};     // Warm cream panel
  afterhours::Color panel_peach{255, 235, 215, 255};     // Slightly darker peach
  afterhours::Color btn_green{125, 195, 95, 255};        // Bright green toggle
  afterhours::Color btn_green_dark{95, 165, 65, 255};    // Green shadow
  afterhours::Color btn_blue{95, 165, 215, 255};         // Blue pill button
  afterhours::Color btn_blue_dark{65, 135, 185, 255};    // Blue shadow
  afterhours::Color text_dark{55, 45, 35, 255};          // Dark brown text
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color close_red{220, 85, 95, 255};         // Close button red
  afterhours::Color wifi_green{85, 195, 145, 255};       // WiFi icon green

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
    theme.roundness = 0.3f;
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
    float panel_w = 680.0f;
    float panel_h = 420.0f;
    float panel_x = ((float)screen_w - panel_w) / 2.0f;
    float panel_y = ((float)screen_h - panel_h) / 2.0f;

    // Panel shadow
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)), 
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x + 6.0f, panel_y + 8.0f)
            .with_custom_background(afterhours::Color{0, 0, 0, 60})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("panel_shadow"));

    // Main panel background
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)), 
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_border(header_dark, 4.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("panel"));

    // ========== CORAL HEADER ==========
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 8)), pixels(70)})
            .with_absolute_position()
            .with_translate(panel_x + 4.0f, panel_y + 4.0f)
            .with_custom_background(header_coral)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.15f)
            .with_debug_name("header"));

    // Title
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(200), pixels(50)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f - 100.0f, panel_y + 15.0f)
            .with_font("EqProRounded", 38.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Close button (X)
    button(context, mk(entity, 25),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{pixels(52), pixels(52)})
               .with_absolute_position()
               .with_translate(panel_x + panel_w - 65.0f, panel_y - 8.0f)
               .with_custom_background(close_red)
               .with_border(afterhours::Color{175, 55, 65, 255}, 3.0f)
               .with_font("EqProRounded", 28.0f)
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(1.0f)
               .with_debug_name("close_btn"));

    // ========== TOGGLE BUTTONS (Music, Sound, Vibration) ==========
    float toggle_y = panel_y + 100.0f;
    float toggle_x = panel_x + 75.0f;
    float toggle_spacing = 90.0f;

    std::vector<std::pair<std::string, bool*>> toggles = {
        {"♫", &music_on},
        {"◀", &sound_on},
        {"◇", &vibration_on},
    };

    for (size_t i = 0; i < toggles.size(); i++) {
      float tx = toggle_x + (float)i * toggle_spacing;
      bool is_on = *toggles[i].second;
      afterhours::Color bg_col = is_on ? btn_green : afterhours::Color{180, 180, 180, 255};
      afterhours::Color border_col = is_on ? btn_green_dark : afterhours::Color{140, 140, 140, 255};

      if (button(context, mk(entity, 30 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(toggles[i].first)
                     .with_size(ComponentSize{pixels(70), pixels(70)})
                     .with_absolute_position()
                     .with_translate(tx, toggle_y)
                     .with_custom_background(bg_col)
                     .with_border(border_col, 4.0f)
                     .with_font("EqProRounded", 32.0f)
                     .with_custom_text_color(text_white)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(1.0f)
                     .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 50})
                     .with_debug_name("toggle_" + std::to_string(i)))) {
        *toggles[i].second = !(*toggles[i].second);
      }
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

    // WiFi icon
    div(context, mk(entity, 43),
        ComponentConfig{}
            .with_label("(O)")
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 85.0f, toggle_y + 10.0f)
            .with_custom_background(wifi_green)
            .with_border(afterhours::Color{55, 165, 115, 255}, 3.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("wifi_icon"));

    // ========== BLUE PILL BUTTONS ==========
    float btn_y1 = toggle_y + 95.0f;
    float btn_y2 = btn_y1 + 70.0f;
    float btn_y3 = btn_y2 + 70.0f;
    float left_btn_x = panel_x + 50.0f;
    float right_btn_x = panel_x + panel_w / 2.0f + 20.0f;
    float btn_w = 280.0f;
    float btn_h = 55.0f;

    // Notifications: OFF
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y1)
            .with_custom_background(btn_blue)
            .with_border(btn_blue_dark, 4.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("notifications_bg"));

    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Ø")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_absolute_position()
            .with_translate(left_btn_x + 20.0f, btn_y1 + 12.0f)
            .with_font("EqProRounded", 24.0f)
            .with_custom_text_color(close_red)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("notif_icon"));

    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label("Notifications: OFF")
            .with_size(ComponentSize{pixels(200), pixels(35)})
            .with_absolute_position()
            .with_translate(left_btn_x + 55.0f, btn_y1 + 10.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("notif_text"));

    // Credits
    button(context, mk(entity, 55),
           ComponentConfig{}
               .with_label("Credits")
               .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
               .with_absolute_position()
               .with_translate(right_btn_x, btn_y1)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 4.0f)
               .with_font("EqProRounded", 26.0f)
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
               .with_debug_name("credits"));

    // Language
    button(context, mk(entity, 60),
           ComponentConfig{}
               .with_label("Language")
               .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
               .with_absolute_position()
               .with_translate(left_btn_x, btn_y2)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 4.0f)
               .with_font("EqProRounded", 26.0f)
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_soft_shadow(2.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
               .with_debug_name("language"));

    // Support
    button(context, mk(entity, 65),
           ComponentConfig{}
               .with_label("Support")
               .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
               .with_absolute_position()
               .with_translate(right_btn_x, btn_y2)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 4.0f)
               .with_font("EqProRounded", 26.0f)
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
            .with_size(ComponentSize{pixels(280), pixels(22)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 10.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("build_id"));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_label("Version 1.11.0.12346")
            .with_size(ComponentSize{pixels(200), pixels(20)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 32.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("version"));

    div(context, mk(entity, 72),
        ComponentConfig{}
            .with_label("Player ID: 281676956389")
            .with_size(ComponentSize{pixels(200), pixels(20)})
            .with_absolute_position()
            .with_translate(left_btn_x, btn_y3 + 52.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(afterhours::Color{130, 115, 95, 255})
            .with_debug_name("player_id"));

    // Terms and Privacy
    button(context, mk(entity, 75),
           ComponentConfig{}
               .with_label("Terms and Privacy")
               .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
               .with_absolute_position()
               .with_translate(right_btn_x, btn_y3)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 4.0f)
               .with_font("EqProRounded", 22.0f)
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

