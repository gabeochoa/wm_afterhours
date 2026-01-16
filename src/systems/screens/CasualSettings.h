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
            .with_font("Gaegu-Bold", 38.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // ========== CLOSE BUTTON (X) ==========
    if (button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("X")
                   .with_size(ComponentSize{pixels(52), pixels(52)})
                   .with_absolute_position()
                   .with_translate(panel_x + panel_w - 20.0f, panel_y - 20.0f)
                   .with_custom_background(close_red)
                   .with_border(afterhours::Color{190, 55, 65, 255}, 4.0f)
                   .with_font("Gaegu-Bold", 28.0f)
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
        {"~", &music_on, "music"},     // Music note symbol
        {"<)", &sound_on, "sound"},    // Speaker symbol
        {"[]", &vibrate_on, "vibrate"} // Phone vibrate symbol
    };

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
                     .with_font("Gaegu-Bold", 24.0f)
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(1.0f)
                     .with_soft_shadow(2.0f, 4.0f, 8.0f,
                                       afterhours::Color{0, 0, 0, 50})
                     .with_debug_name("toggle_" + name))) {
        *state_ptr = !*state_ptr;
      }
    }

    // Save/Load Progress button
    button(context, mk(entity, 50),
           ComponentConfig{}
               .with_label("Save/Load Progress")
               .with_size(ComponentSize{pixels(195), pixels(55)})
               .with_absolute_position()
               .with_translate(panel_x + panel_w - 260.0f, toggle_y)
               .with_custom_background(white)
               .with_border(afterhours::Color{200, 195, 185, 255}, 3.0f)
               .with_font("Gaegu-Bold", 19.0f)
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("save_load"));

    // Wifi icon
    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("((*))")
            .with_size(ComponentSize{pixels(40), pixels(40)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 75.0f, toggle_y + 8.0f)
            .with_custom_background(btn_green)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("wifi_icon"));

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
                 .with_size(
                     ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
                 .with_absolute_position()
                 .with_translate(left_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", 22.0f)
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.5f)
                 .with_soft_shadow(2.0f, 4.0f, 10.0f,
                                   afterhours::Color{0, 0, 0, 45})
                 .with_debug_name("left_btn_" + std::to_string(i)));
    }

    // Right column
    for (size_t i = 0; i < right_buttons.size(); i++) {
      button(context, mk(entity, 110 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(right_buttons[i])
                 .with_size(
                     ComponentSize{pixels(static_cast<int>(btn_w)), pixels(static_cast<int>(btn_h))})
                 .with_absolute_position()
                 .with_translate(right_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", 22.0f)
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.5f)
                 .with_soft_shadow(2.0f, 4.0f, 10.0f,
                                   afterhours::Color{0, 0, 0, 45})
                 .with_debug_name("right_btn_" + std::to_string(i)));
    }

    // ========== VERSION INFO ==========
    float info_y = panel_y + panel_h - 75.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("15555-1-114203-20-10200-01")
            .with_size(ComponentSize{pixels(280), pixels(20)})
            .with_absolute_position()
            .with_translate(left_x, info_y)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("build_id"));

    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Version 1.11.0.12346")
            .with_size(ComponentSize{pixels(180), pixels(20)})
            .with_absolute_position()
            .with_translate(left_x, info_y + 22.0f)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("version"));

    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_label("Player ID: 281676956389")
            .with_size(ComponentSize{pixels(200), pixels(20)})
            .with_absolute_position()
            .with_translate(left_x, info_y + 44.0f)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("player_id"));

    // ========== CORNER DECORATIONS ==========
    // Settings gear icon (bottom left)
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position()
            .with_translate(55.0f, (float)screen_h - 75.0f)
            .with_custom_background(afterhours::Color{100, 135, 160, 200})
            .with_font("Gaegu-Bold", 28.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("gear_icon"));

    // Level indicator (bottom right)
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_label("4")
            .with_size(ComponentSize{pixels(65), pixels(65)})
            .with_absolute_position()
            .with_translate((float)screen_w - 95.0f, (float)screen_h - 85.0f)
            .with_custom_background(btn_green)
            .with_border(btn_green_dark, 5.0f)
            .with_font("Gaegu-Bold", 36.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("level_badge"));
  }
};

REGISTER_EXAMPLE_SCREEN(casual_settings, "Game Mockups",
                        "Casual mobile settings (Angry Birds style)",
                        CasualSettingsScreen)

