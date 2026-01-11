#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct IslandsTrainsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int display_mode = 0;       // 0=Borderless, 1=Windowed, 2=Fullscreen
  int resolution = 2;         // Index into resolutions
  int cam_pan_speed = 7;      // 0-10
  int cam_rotate_speed = 8;   // 0-10
  int effects_volume = 9;     // 0-10
  int music_volume = 10;      // 0-10

  // Colors matching Islands & Trains - calm, muted pastel aesthetic
  afterhours::Color bg_sage{175, 185, 170, 255};         // Muted sage green background
  afterhours::Color bg_sage_light{195, 205, 190, 255};   // Lighter sage
  afterhours::Color panel_cream{245, 245, 235, 255};     // Warm cream panel
  afterhours::Color row_cream{235, 238, 230, 255};       // Row background
  afterhours::Color text_dark{85, 95, 90, 255};          // Dark gray-green text
  afterhours::Color text_muted{145, 155, 150, 255};      // Muted text
  afterhours::Color header_teal{125, 165, 155, 255};     // Section header teal
  afterhours::Color slider_teal{145, 185, 175, 255};     // Slider filled
  afterhours::Color slider_pink{215, 175, 175, 255};     // Slider empty
  afterhours::Color slider_bg{205, 210, 200, 255};       // Slider background
  afterhours::Color btn_cream{225, 230, 220, 255};       // Button background
  afterhours::Color close_cream{235, 235, 225, 255};     // Close button

  std::vector<std::string> modes = {"Borderless", "Windowed", "Fullscreen"};
  std::vector<std::string> resolutions = {"1920x1080", "2560x1440", "3840x2160"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = panel_cream;
    theme.font_muted = text_muted;
    theme.background = bg_sage;
    theme.surface = panel_cream;
    theme.primary = header_teal;
    theme.secondary = slider_teal;
    theme.accent = slider_teal;
    theme.error = afterhours::Color{180, 100, 100, 255};
    theme.roundness = 0.2f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_sage)
            .with_debug_name("bg"));

    // ========== MAIN PANEL ==========
    float panel_w = 500.0f;
    float panel_h = 580.0f;
    float panel_x = ((float)screen_w - panel_w) / 2.0f;
    float panel_y = ((float)screen_h - panel_h) / 2.0f;

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)), 
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_soft_shadow(4.0f, 6.0f, 20.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("panel"));

    // ========== CLOSE BUTTON (X) ==========
    button(context, mk(entity, 15),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_absolute_position()
               .with_translate((float)screen_w - 80.0f, 35.0f)
               .with_custom_background(close_cream)
               .with_border(afterhours::Color{200, 200, 190, 255}, 2.0f)
               .with_font("EqProRounded", 28.0f)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(1.0f)
               .with_debug_name("close"));

    // ========== TITLE ==========
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(200), pixels(45)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f - 100.0f, panel_y + 25.0f)
            .with_font("EqProRounded", 32.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    float content_x = panel_x + 30.0f;
    float content_y = panel_y + 85.0f;
    float row_w = panel_w - 60.0f;
    float row_h = 42.0f;

    // ========== DISPLAY SECTION ==========
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("DISPLAY")
            .with_size(ComponentSize{pixels(100), pixels(25)})
            .with_absolute_position()
            .with_translate(content_x, content_y)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(header_teal)
            .with_debug_name("display_header"));

    // Mode row
    render_selector_row(context, entity, 100, content_x, content_y + 30.0f, row_w, row_h,
                        "Mode", modes[display_mode]);
    
    // Resolution row
    render_selector_row(context, entity, 110, content_x, content_y + 30.0f + row_h + 4.0f, row_w, row_h,
                        "Resolution", resolutions[resolution]);

    // ========== CONTROLS SECTION ==========
    float controls_y = content_y + 125.0f;
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("CONTROLS")
            .with_size(ComponentSize{pixels(120), pixels(25)})
            .with_absolute_position()
            .with_translate(content_x, controls_y)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(header_teal)
            .with_debug_name("controls_header"));

    // Cam panning speed
    render_slider_row(context, entity, 120, content_x, controls_y + 30.0f, row_w, row_h,
                      "Cam panning speed", cam_pan_speed, 10);
    
    // Cam rotating speed
    render_slider_row(context, entity, 130, content_x, controls_y + 30.0f + row_h + 4.0f, row_w, row_h,
                      "Cam rotating Speed", cam_rotate_speed, 10);

    // Keyboard button
    button(context, mk(entity, 140),
           ComponentConfig{}
               .with_label("KEYBOARD")
               .with_size(ComponentSize{pixels(static_cast<int>(row_w)), pixels(static_cast<int>(row_h))})
               .with_absolute_position()
               .with_translate(content_x, controls_y + 30.0f + 2 * (row_h + 4.0f))
               .with_custom_background(btn_cream)
               .with_font("EqProRounded", 18.0f)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("keyboard_btn"));

    // ========== AUDIO SECTION ==========
    float audio_y = controls_y + 175.0f;
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("AUDIO")
            .with_size(ComponentSize{pixels(80), pixels(25)})
            .with_absolute_position()
            .with_translate(content_x, audio_y)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(header_teal)
            .with_debug_name("audio_header"));

    // Effects Volume
    render_slider_row(context, entity, 150, content_x, audio_y + 30.0f, row_w, row_h,
                      "Effects Volume", effects_volume, 10);
    
    // Music Volume
    render_slider_row(context, entity, 160, content_x, audio_y + 30.0f + row_h + 4.0f, row_w, row_h,
                      "Music Volume", music_volume, 10);

    // ========== TUTORIAL SECTION ==========
    float tutorial_y = audio_y + 125.0f;
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("TUTORIAL")
            .with_size(ComponentSize{pixels(100), pixels(25)})
            .with_absolute_position()
            .with_translate(content_x, tutorial_y)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(header_teal)
            .with_debug_name("tutorial_header"));

    button(context, mk(entity, 170),
           ComponentConfig{}
               .with_label("PLAY TUTORIAL")
               .with_size(ComponentSize{pixels(static_cast<int>(row_w)), pixels(static_cast<int>(row_h))})
               .with_absolute_position()
               .with_translate(content_x, tutorial_y + 30.0f)
               .with_custom_background(btn_cream)
               .with_font("EqProRounded", 18.0f)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("tutorial_btn"));

    // ========== RESET TO DEFAULTS ==========
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("RESET TO DEFAULTS")
            .with_size(ComponentSize{pixels(220), pixels(30)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f - 110.0f, panel_y + panel_h - 50.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("reset"));
  }

  void render_selector_row(UIContext<InputAction> &context, afterhours::Entity &entity,
                           int base_id, float x, float y, float w, float h,
                           const std::string &label, const std::string &value) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("row_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(180), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 20.0f, y + 8.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("label_" + std::to_string(base_id)));

    // Left chevron
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label("<")
            .with_size(ComponentSize{pixels(25), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 180.0f, y + 8.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("chevron_l_" + std::to_string(base_id)));

    // Value
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(value)
            .with_size(ComponentSize{pixels(120), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 150.0f, y + 8.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("value_" + std::to_string(base_id)));

    // Right chevron
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_label(">")
            .with_size(ComponentSize{pixels(25), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 35.0f, y + 8.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("chevron_r_" + std::to_string(base_id)));
  }

  void render_slider_row(UIContext<InputAction> &context, afterhours::Entity &entity,
                         int base_id, float x, float y, float w, float h,
                         const std::string &label, int value, int max_val) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("row_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(200), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 20.0f, y + 8.0f)
            .with_font("EqProRounded", 17.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("label_" + std::to_string(base_id)));

    // Left chevron
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label("<")
            .with_size(ComponentSize{pixels(25), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 200.0f, y + 8.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("chevron_l_" + std::to_string(base_id)));

    // Segmented slider
    float slider_x = x + w - 170.0f;
    float seg_w = 12.0f;
    float seg_h = 18.0f;
    float seg_gap = 3.0f;

    for (int i = 0; i < max_val; i++) {
      bool is_filled = (i < value);
      afterhours::Color seg_color = is_filled ? slider_teal : slider_pink;
      
      div(context, mk(entity, base_id + 10 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(seg_w)), pixels(static_cast<int>(seg_h))})
              .with_absolute_position()
              .with_translate(slider_x + (float)i * (seg_w + seg_gap), y + 12.0f)
              .with_custom_background(seg_color)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.3f)
              .with_debug_name("seg_" + std::to_string(base_id) + "_" + std::to_string(i)));
    }

    // Right chevron
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(">")
            .with_size(ComponentSize{pixels(25), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 20.0f, y + 8.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("chevron_r_" + std::to_string(base_id)));
  }
};

REGISTER_EXAMPLE_SCREEN(islands_trains_settings, "Game Mockups",
                        "Calm puzzle game settings (Islands & Trains style)",
                        IslandsTrainsSettingsScreen)

