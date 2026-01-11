#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ParcelCorpsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int language_idx = 0;
  bool subtitles = true;
  bool fullscreen = true;
  bool motion_blur = true;
  bool vsync = true;
  float audio_volume = 0.85f;
  float music_volume = 0.75f;

  // Colors matching Parcel Corps - dark phone UI with green toggles
  afterhours::Color bg_dark{18, 22, 28, 255};            // Very dark background
  afterhours::Color phone_black{12, 14, 18, 255};        // Phone frame black
  afterhours::Color row_dark{35, 40, 50, 180};           // Row background (semi-transparent)
  afterhours::Color row_separator{55, 60, 70, 255};      // Row separator
  afterhours::Color text_white{245, 248, 250, 255};      // White text
  afterhours::Color text_muted{145, 155, 165, 255};      // Muted text
  afterhours::Color toggle_green{75, 195, 95, 255};      // iOS-style green toggle
  afterhours::Color toggle_track{85, 90, 100, 255};      // Toggle track (off)
  afterhours::Color slider_green{95, 185, 85, 255};      // Volume slider green
  afterhours::Color slider_orange{235, 155, 65, 255};    // Music slider orange
  afterhours::Color slider_track{55, 60, 70, 255};       // Slider track background
  afterhours::Color icon_purple{145, 95, 185, 255};      // Purple icon
  afterhours::Color icon_blue{65, 145, 215, 255};        // Blue icon
  afterhours::Color icon_red{215, 85, 85, 255};          // Red icon
  afterhours::Color icon_green{85, 175, 125, 255};       // Green icon
  afterhours::Color notch_gray{45, 50, 60, 255};         // Phone notch

  std::vector<std::string> languages = {"King's English", "American", "Deutsch", "Français"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = phone_black;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = row_dark;
    theme.primary = toggle_green;
    theme.secondary = text_muted;
    theme.accent = toggle_green;
    theme.error = icon_red;
    theme.roundness = 0.15f;
    theme.segments = 10;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND (game scene visible) ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== PHONE FRAME ==========
    float phone_w = 320.0f;
    float phone_h = 560.0f;
    float phone_x = 50.0f;
    float phone_y = ((float)screen_h - phone_h) / 2.0f;

    // Phone outer frame
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(phone_w)), 
                                     pixels(static_cast<int>(phone_h))})
            .with_absolute_position()
            .with_translate(phone_x, phone_y)
            .with_custom_background(phone_black)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_soft_shadow(6.0f, 8.0f, 25.0f, afterhours::Color{0, 0, 0, 120})
            .with_debug_name("phone_frame"));

    // Phone screen area
    float screen_margin = 8.0f;
    float screen_x = phone_x + screen_margin;
    float screen_y = phone_y + screen_margin;
    float screen_inner_w = phone_w - screen_margin * 2;
    float screen_inner_h = phone_h - screen_margin * 2;

    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(screen_inner_w)), 
                                     pixels(static_cast<int>(screen_inner_h))})
            .with_absolute_position()
            .with_translate(screen_x, screen_y)
            .with_custom_background(afterhours::Color{22, 26, 32, 255})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("phone_screen"));

    // ========== STATUS BAR ==========
    float status_y = screen_y + 12.0f;
    
    // Time
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("23:45")
            .with_size(ComponentSize{pixels(60), pixels(22)})
            .with_absolute_position()
            .with_translate(screen_x + 15.0f, status_y)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("time"));

    // Signal bars
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("||||")
            .with_size(ComponentSize{pixels(40), pixels(22)})
            .with_absolute_position()
            .with_translate(screen_x + screen_inner_w - 95.0f, status_y)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("signal"));

    // Battery
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(32), pixels(14)})
            .with_absolute_position()
            .with_translate(screen_x + screen_inner_w - 48.0f, status_y + 4.0f)
            .with_custom_background(toggle_green)
            .with_border(text_white, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("battery"));

    // ========== SETTINGS TITLE ==========
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_absolute_position()
            .with_translate(screen_x + 15.0f, status_y + 35.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("title"));

    // ========== SETTINGS ROWS ==========
    float row_x = screen_x + 8.0f;
    float row_start_y = status_y + 85.0f;
    float row_w = screen_inner_w - 16.0f;
    float row_h = 48.0f;
    float row_gap = 4.0f;

    // Language selector row
    render_language_row(context, entity, 100, row_x, row_start_y, row_w, row_h);

    // Subtitles toggle
    render_toggle_row(context, entity, 110, row_x, row_start_y + row_h + row_gap, row_w, row_h,
                      "Subtitles", subtitles, toggle_green);

    // Separator
    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(row_w)), pixels(1)})
            .with_absolute_position()
            .with_translate(row_x, row_start_y + 2 * (row_h + row_gap))
            .with_custom_background(row_separator)
            .with_debug_name("separator1"));

    float section2_y = row_start_y + 2 * (row_h + row_gap) + 8.0f;

    // Resolution (display only)
    render_display_row(context, entity, 120, row_x, section2_y, row_w, row_h,
                       "Resolution", "2560 x 1440", icon_purple);

    // Full Screen toggle
    render_toggle_row(context, entity, 130, row_x, section2_y + row_h + row_gap, row_w, row_h,
                      "Full Screen", fullscreen, toggle_green);

    // MSAA (display only)
    render_display_row(context, entity, 140, row_x, section2_y + 2 * (row_h + row_gap), row_w, row_h,
                       "MSAA", "2x", icon_blue);

    // Texture Quality
    render_display_row(context, entity, 150, row_x, section2_y + 3 * (row_h + row_gap), row_w, row_h,
                       "Texture Quality", "High", icon_green);

    // Motion Blur toggle
    render_toggle_row(context, entity, 160, row_x, section2_y + 4 * (row_h + row_gap), row_w, row_h,
                      "Motion Blur", motion_blur, toggle_green);

    // VSync toggle
    render_toggle_row(context, entity, 170, row_x, section2_y + 5 * (row_h + row_gap), row_w, row_h,
                      "VSync", vsync, toggle_green);

    // ========== VOLUME SLIDERS ==========
    float slider_y = section2_y + 6 * (row_h + row_gap) + 10.0f;
    
    render_volume_slider(context, entity, 200, row_x, slider_y, row_w, row_h,
                         "Audio Volume", audio_volume, slider_green);
    
    render_volume_slider(context, entity, 210, row_x, slider_y + row_h + row_gap, row_w, row_h,
                         "Music Volume", music_volume, slider_orange);

    // ========== HUD ELEMENTS (right side) ==========
    // LIVE indicator
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("LIVE")
            .with_size(ComponentSize{pixels(55), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 300.0f, 25.0f)
            .with_custom_background(icon_red)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("live"));

    // Viewers count
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("@ 8")
            .with_size(ComponentSize{pixels(50), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 235.0f, 25.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("viewers"));

    // Score
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("10")
            .with_size(ComponentSize{pixels(40), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 185.0f, 25.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("score"));

    // Username
    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("EDDCOATES")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 140.0f, 25.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("username"));

    // ========== SPEEDOMETER (bottom right) ==========
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("009")
            .with_size(ComponentSize{pixels(85), pixels(55)})
            .with_absolute_position()
            .with_translate((float)screen_w - 120.0f, (float)screen_h - 90.0f)
            .with_custom_background(afterhours::Color{25, 30, 38, 230})
            .with_font("EqProRounded", 36.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("speedometer"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("M/h")
            .with_size(ComponentSize{pixels(50), pixels(20)})
            .with_absolute_position()
            .with_translate((float)screen_w - 90.0f, (float)screen_h - 45.0f)
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("speed_unit"));

    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("GAMON")
            .with_size(ComponentSize{pixels(70), pixels(18)})
            .with_absolute_position()
            .with_translate((float)screen_w - 100.0f, (float)screen_h - 28.0f)
            .with_font("EqProRounded", 11.0f)
            .with_custom_text_color(slider_orange)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("vehicle_name"));
  }

  void render_language_row(UIContext<InputAction> &context, afterhours::Entity &entity,
                           int base_id, float x, float y, float w, float h) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_dark)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("lang_row"));

    // Globe icon (placeholder)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(x + 12.0f, y + 10.0f)
            .with_custom_background(icon_purple)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("lang_icon"));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label("Text Language")
            .with_size(ComponentSize{pixels(120), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 48.0f, y + 12.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("lang_label"));

    // Left chevron
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label("<")
            .with_size(ComponentSize{pixels(20), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 145.0f, y + 12.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("lang_left"));

    // Flag + Value
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_label("King's English")
            .with_size(ComponentSize{pixels(110), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 120.0f, y + 12.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lang_value"));

    // Right chevron
    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_label(">")
            .with_size(ComponentSize{pixels(20), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 18.0f, y + 12.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("lang_right"));
  }

  void render_toggle_row(UIContext<InputAction> &context, afterhours::Entity &entity,
                         int base_id, float x, float y, float w, float h,
                         const std::string &label, bool value, afterhours::Color active_color) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_dark)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("toggle_row_" + std::to_string(base_id)));

    // Icon (colored circle)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("O")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(x + 12.0f, y + 10.0f)
            .with_custom_background(active_color)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("toggle_icon_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(150), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 48.0f, y + 12.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("toggle_label_" + std::to_string(base_id)));

    // Toggle track
    afterhours::Color track_color = value ? active_color : toggle_track;
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(48), pixels(26)})
            .with_absolute_position()
            .with_translate(x + w - 60.0f, y + 11.0f)
            .with_custom_background(track_color)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("toggle_track_" + std::to_string(base_id)));

    // Toggle knob
    float knob_x = value ? (x + w - 36.0f) : (x + w - 58.0f);
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(22), pixels(22)})
            .with_absolute_position()
            .with_translate(knob_x, y + 13.0f)
            .with_custom_background(text_white)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("toggle_knob_" + std::to_string(base_id)));
  }

  void render_display_row(UIContext<InputAction> &context, afterhours::Entity &entity,
                          int base_id, float x, float y, float w, float h,
                          const std::string &label, const std::string &value,
                          afterhours::Color icon_color) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_dark)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("display_row_" + std::to_string(base_id)));

    // Icon
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(x + 12.0f, y + 10.0f)
            .with_custom_background(icon_color)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("display_icon_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(150), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 48.0f, y + 12.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("display_label_" + std::to_string(base_id)));

    // Value
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(value)
            .with_size(ComponentSize{pixels(100), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 110.0f, y + 12.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("display_value_" + std::to_string(base_id)));
  }

  void render_volume_slider(UIContext<InputAction> &context, afterhours::Entity &entity,
                            int base_id, float x, float y, float w, float h,
                            const std::string &label, float value, afterhours::Color fill_color) {
    // Row background
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_dark)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("volume_row_" + std::to_string(base_id)));

    // Icon
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(x + 12.0f, y + 10.0f)
            .with_custom_background(fill_color)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("volume_icon_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(120), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 48.0f, y + 12.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("volume_label_" + std::to_string(base_id)));

    // Slider track
    float slider_w = 130.0f;
    float slider_x = x + w - slider_w - 15.0f;
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(slider_w)), pixels(10)})
            .with_absolute_position()
            .with_translate(slider_x, y + 19.0f)
            .with_custom_background(slider_track)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("volume_track_" + std::to_string(base_id)));

    // Slider fill
    int fill_w = static_cast<int>(slider_w * value);
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(fill_w), pixels(10)})
            .with_absolute_position()
            .with_translate(slider_x, y + 19.0f)
            .with_custom_background(fill_color)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("volume_fill_" + std::to_string(base_id)));
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Phone UI settings overlay (Parcel Corps style)",
                        ParcelCorpsSettingsScreen)

