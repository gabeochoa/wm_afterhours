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
  afterhours::Color icon_purple{145, 95, 185, 255};      // Purple icon (language)
  afterhours::Color icon_blue{65, 145, 215, 255};        // Blue icon (MSAA)
  afterhours::Color icon_red{215, 85, 85, 255};          // Red icon (motion blur, volume)
  afterhours::Color icon_green{85, 175, 125, 255};       // Green icon (subtitles, texture)
  afterhours::Color icon_rainbow1{255, 120, 120, 255};   // Rainbow icon color 1 (resolution)
  afterhours::Color icon_rainbow2{120, 255, 120, 255};   // Rainbow icon color 2
  afterhours::Color icon_rainbow3{120, 120, 255, 255};   // Rainbow icon color 3
  afterhours::Color notch_gray{45, 50, 60, 255};         // Phone notch
  afterhours::Color quest_blue{65, 155, 220, 255};       // Quest panel blue
  afterhours::Color quest_blue_dark{45, 120, 180, 255};  // Quest panel border

  std::vector<std::string> languages = {"King's English", "American", "Deutsch", "Français"};
  
  // Chat messages
  std::vector<std::pair<std::string, std::string>> chat_messages = {
      {"reveredsoup", "you did it :)"},
      {"untrackedjackstay", "Cool :)"},
      {"NimbleGrimble", "YES :)"},
      {"untrackedjackstay", "ZZZzzzz"},
      {"sniprit09", "Zzzz"},
  };

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
    theme.roundness = 0.10f;
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
    float row_h = 44.0f;
    float row_gap = 3.0f;

    // Language selector row (purple globe icon)
    render_language_row(context, entity, 100, row_x, row_start_y, row_w, row_h);

    // Subtitles toggle (green speech bubble icon)
    render_toggle_row_with_icon(context, entity, 110, row_x, row_start_y + row_h + row_gap, row_w, row_h,
                      "Subtitles", subtitles, icon_green, "...");

    // Separator
    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(row_w)), pixels(1)})
            .with_absolute_position()
            .with_translate(row_x, row_start_y + 2 * (row_h + row_gap))
            .with_custom_background(row_separator)
            .with_debug_name("separator1"));

    float section2_y = row_start_y + 2 * (row_h + row_gap) + 6.0f;

    // Resolution (rainbow/multicolor icon)
    render_display_row_rainbow(context, entity, 120, row_x, section2_y, row_w, row_h,
                       "Resolution", "2560 x 1440");

    // Full Screen toggle (rainbow icon)
    render_toggle_row_rainbow(context, entity, 130, row_x, section2_y + row_h + row_gap, row_w, row_h,
                      "Full Screen", fullscreen);

    // MSAA (blue icon)
    render_display_row(context, entity, 140, row_x, section2_y + 2 * (row_h + row_gap), row_w, row_h,
                       "MSAA", "2x", icon_blue);

    // Texture Quality (green gem icon)
    render_display_row(context, entity, 150, row_x, section2_y + 3 * (row_h + row_gap), row_w, row_h,
                       "Texture Quality", "High", icon_green);

    // Motion Blur toggle (red/pink X icon)
    render_toggle_row_with_icon(context, entity, 160, row_x, section2_y + 4 * (row_h + row_gap), row_w, row_h,
                      "Motion Blur", motion_blur, icon_red, "X");

    // VSync toggle (purple icon)
    render_toggle_row_with_icon(context, entity, 170, row_x, section2_y + 5 * (row_h + row_gap), row_w, row_h,
                      "VSync", vsync, icon_purple, "~");

    // ========== VOLUME SLIDERS ==========
    float slider_y = section2_y + 6 * (row_h + row_gap) + 8.0f;
    
    render_volume_slider(context, entity, 200, row_x, slider_y, row_w, row_h,
                         "Audio Volume", audio_volume, icon_red);
    
    render_volume_slider(context, entity, 210, row_x, slider_y + row_h + row_gap, row_w, row_h,
                         "Music Volume", music_volume, icon_red);

    // ========== QUEST PANEL (top center) ==========
    float quest_x = 420.0f;
    float quest_y = 25.0f;
    float quest_w = 340.0f;
    float quest_h = 85.0f;

    // Quest panel background
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(quest_w)), pixels(static_cast<int>(quest_h))})
            .with_absolute_position()
            .with_translate(quest_x, quest_y)
            .with_custom_background(quest_blue)
            .with_border(quest_blue_dark, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("quest_panel"));

    // Quest item 1 (completed)
    div(context, mk(entity, 251),
        ComponentConfig{}
            .with_label("[X]")
            .with_size(ComponentSize{pixels(24), pixels(24)})
            .with_absolute_position()
            .with_translate(quest_x + 12.0f, quest_y + 14.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("quest_check1"));

    div(context, mk(entity, 252),
        ComponentConfig{}
            .with_label("SIGN UP A BUSINESS TO YOUR DELIVERY APP")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(quest_x + 40.0f, quest_y + 14.0f)
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("quest_text1"));

    // Quest item 2 (incomplete)
    div(context, mk(entity, 253),
        ComponentConfig{}
            .with_label("[ ]")
            .with_size(ComponentSize{pixels(24), pixels(24)})
            .with_absolute_position()
            .with_translate(quest_x + 12.0f, quest_y + 48.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("quest_check2"));

    // Level badge
    div(context, mk(entity, 254),
        ComponentConfig{}
            .with_label("Lv0")
            .with_size(ComponentSize{pixels(32), pixels(20)})
            .with_absolute_position()
            .with_translate(quest_x + 40.0f, quest_y + 50.0f)
            .with_custom_background(slider_orange)
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("quest_level"));

    div(context, mk(entity, 255),
        ComponentConfig{}
            .with_label("SIGN UP 4 BUSINESSES TO YOUR DELIVERY APP")
            .with_size(ComponentSize{pixels(250), pixels(22)})
            .with_absolute_position()
            .with_translate(quest_x + 78.0f, quest_y + 48.0f)
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("quest_text2"));

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

    // Eye/viewers icon
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("O")
            .with_size(ComponentSize{pixels(22), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 235.0f, 28.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("eye_icon"));

    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("8")
            .with_size(ComponentSize{pixels(20), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 212.0f, 28.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("viewers_count"));

    // Diamond/score
    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("<>")
            .with_size(ComponentSize{pixels(22), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 185.0f, 28.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("diamond_icon"));

    div(context, mk(entity, 304),
        ComponentConfig{}
            .with_label("10")
            .with_size(ComponentSize{pixels(25), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 162.0f, 28.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("score"));

    // Username
    div(context, mk(entity, 305),
        ComponentConfig{}
            .with_label("EDDCOATES")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 120.0f, 25.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("username"));

    // ========== CHAT MESSAGES (right side) ==========
    float chat_x = (float)screen_w - 200.0f;
    float chat_start_y = 60.0f;
    float chat_line_h = 20.0f;
    
    for (size_t i = 0; i < chat_messages.size(); i++) {
      auto& [username, message] = chat_messages[i];
      float chat_y = chat_start_y + (float)i * chat_line_h;
      
      // Username
      div(context, mk(entity, 320 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(username)
              .with_size(ComponentSize{pixels(120), pixels(18)})
              .with_absolute_position()
              .with_translate(chat_x, chat_y)
              .with_font("EqProRounded", 12.0f)
              .with_custom_text_color(slider_orange)
              .with_debug_name("chat_user_" + std::to_string(i)));

      // Message
      div(context, mk(entity, 321 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(message)
              .with_size(ComponentSize{pixels(80), pixels(18)})
              .with_absolute_position()
              .with_translate(chat_x + 110.0f, chat_y)
              .with_font("EqProRounded", 12.0f)
              .with_custom_text_color(text_white)
              .with_debug_name("chat_msg_" + std::to_string(i)));
    }

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
            .with_font("EqProRounded", 12.0f)
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

    // Globe icon (purple)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_purple)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("lang_icon"));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label("Text Language")
            .with_size(ComponentSize{pixels(110), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("lang_label"));

    // Left chevron
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label("<")
            .with_size(ComponentSize{pixels(16), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 140.0f, y + 10.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("lang_left"));

    // UK Flag representation (simple red/blue/white)
    div(context, mk(entity, base_id + 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(18), pixels(12)})
            .with_absolute_position()
            .with_translate(x + w - 120.0f, y + 16.0f)
            .with_custom_background(icon_blue)
            .with_border(icon_red, 2.0f)
            .with_debug_name("uk_flag"));

    // Language value
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_label("King's English")
            .with_size(ComponentSize{pixels(90), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 98.0f, y + 11.0f)
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lang_value"));

    // Right chevron
    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_label(">")
            .with_size(ComponentSize{pixels(16), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 14.0f, y + 10.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("lang_right"));
  }

  void render_toggle_row_with_icon(UIContext<InputAction> &context, afterhours::Entity &entity,
                         int base_id, float x, float y, float w, float h,
                         const std::string &label, bool &value, afterhours::Color icon_color,
                         const std::string &icon_symbol) {
    // Row background - clickable to toggle
    if (button(context, mk(entity, base_id),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x, y)
                   .with_custom_background(row_dark)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(0.15f)
                   .with_debug_name("toggle_row_" + std::to_string(base_id)))) {
      value = !value;  // Toggle the boolean on click
    }

    // Icon (colored circle with symbol)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(icon_symbol)
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_color)
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
            .with_size(ComponentSize{pixels(140), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("toggle_label_" + std::to_string(base_id)));

    // Toggle track
    afterhours::Color track_color = value ? toggle_green : toggle_track;
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(44), pixels(24)})
            .with_absolute_position()
            .with_translate(x + w - 56.0f, y + 10.0f)
            .with_custom_background(track_color)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("toggle_track_" + std::to_string(base_id)));

    // Toggle knob
    float knob_x = value ? (x + w - 34.0f) : (x + w - 54.0f);
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(knob_x, y + 12.0f)
            .with_custom_background(text_white)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("toggle_knob_" + std::to_string(base_id)));
  }

  // Rainbow icon for Resolution/Full Screen rows
  void render_toggle_row_rainbow(UIContext<InputAction> &context, afterhours::Entity &entity,
                         int base_id, float x, float y, float w, float h,
                         const std::string &label, bool &value) {
    // Row background - clickable to toggle
    if (button(context, mk(entity, base_id),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x, y)
                   .with_custom_background(row_dark)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(0.15f)
                   .with_debug_name("toggle_row_" + std::to_string(base_id)))) {
      value = !value;  // Toggle the boolean on click
    }

    // Rainbow icon (multicolor circle using nested elements)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_rainbow1)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_outer_" + std::to_string(base_id)));

    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position()
            .with_translate(x + 15.0f, y + 14.0f)
            .with_custom_background(icon_rainbow2)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_mid_" + std::to_string(base_id)));

    div(context, mk(entity, base_id + 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(8), pixels(8)})
            .with_absolute_position()
            .with_translate(x + 19.0f, y + 18.0f)
            .with_custom_background(icon_rainbow3)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_inner_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(140), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("toggle_label_" + std::to_string(base_id)));

    // Toggle track
    afterhours::Color track_color = value ? toggle_green : toggle_track;
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(44), pixels(24)})
            .with_absolute_position()
            .with_translate(x + w - 56.0f, y + 10.0f)
            .with_custom_background(track_color)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("toggle_track_" + std::to_string(base_id)));

    // Toggle knob
    float knob_x = value ? (x + w - 34.0f) : (x + w - 54.0f);
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(knob_x, y + 12.0f)
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
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_color)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("display_icon_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(140), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("display_label_" + std::to_string(base_id)));

    // Value
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(value)
            .with_size(ComponentSize{pixels(95), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 105.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("display_value_" + std::to_string(base_id)));
  }

  void render_display_row_rainbow(UIContext<InputAction> &context, afterhours::Entity &entity,
                          int base_id, float x, float y, float w, float h,
                          const std::string &label, const std::string &value) {
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

    // Rainbow icon (multicolor circle)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_rainbow1)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_outer_" + std::to_string(base_id)));

    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position()
            .with_translate(x + 15.0f, y + 14.0f)
            .with_custom_background(icon_rainbow2)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_mid_" + std::to_string(base_id)));

    div(context, mk(entity, base_id + 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(8), pixels(8)})
            .with_absolute_position()
            .with_translate(x + 19.0f, y + 18.0f)
            .with_custom_background(icon_rainbow3)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("rainbow_inner_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(140), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("display_label_" + std::to_string(base_id)));

    // Value
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(value)
            .with_size(ComponentSize{pixels(95), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 105.0f, y + 11.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("display_value_" + std::to_string(base_id)));
  }

  void render_volume_slider(UIContext<InputAction> &context, afterhours::Entity &entity,
                            int base_id, float x, float y, float w, float h,
                            const std::string &label, float &value,
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
            .with_debug_name("volume_row_" + std::to_string(base_id)));

    // Icon (speaker/music note in red)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(x + 10.0f, y + 9.0f)
            .with_custom_background(icon_color)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("volume_icon_" + std::to_string(base_id)));

    // Label
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(110), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 42.0f, y + 11.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("volume_label_" + std::to_string(base_id)));

    // Use the interactive slider() function
    float slider_w = 125.0f;
    float slider_x = x + w - slider_w - 12.0f;
    slider(context, mk(entity, base_id + 3), value,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(static_cast<int>(slider_w)), pixels(14)})
               .with_absolute_position()
               .with_translate(slider_x, y + 15.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("volume_slider_" + std::to_string(base_id)));
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Phone UI settings overlay (Parcel Corps style)",
                        ParcelCorpsSettingsScreen)

