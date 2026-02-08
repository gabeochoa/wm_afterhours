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

  // === CONFIGURABLE LAYOUT PARAMETERS ===
  // Panel dimensions
  float cfg_panel_width = 720.0f;
  float cfg_panel_height = 480.0f;
  float cfg_panel_border_width = 6.0f;
  float cfg_panel_roundness = 0.12f;

  // Header configuration
  float cfg_header_height = 85.0f;
  float cfg_header_inset = 6.0f;
  float cfg_title_font_size = 44.0f;
  float cfg_title_y_offset = 20.0f;

  // Close button configuration
  float cfg_close_btn_size = 42.0f;
  float cfg_close_btn_margin = 22.0f;
  float cfg_close_btn_border = 3.0f;

  // Toggle section configuration
  float cfg_toggle_y_offset = 115.0f;
  float cfg_toggle_x_offset = 70.0f;
  float cfg_toggle_spacing = 95.0f;
  float cfg_toggle_width = 72.0f;
  float cfg_toggle_height = 52.0f;
  float cfg_toggle_label_gap = 8.0f;
  float cfg_toggle_border_width = 4.0f;
  float cfg_toggle_font_size = 20.0f;
  float cfg_toggle_label_font_size = 16.0f;

  // Save/Load group configuration
  float cfg_saveload_width = 240.0f;
  float cfg_saveload_height = 80.0f;
  float cfg_saveload_padding = 12.0f;
  float cfg_sync_btn_size = 44.0f;

  // Pill button configuration
  float cfg_pill_btn_width = 295.0f;
  float cfg_pill_btn_height = 48.0f;
  float cfg_pill_btn_spacing = 58.0f;
  float cfg_pill_left_margin = 50.0f;
  float cfg_pill_right_margin = 30.0f;
  float cfg_pill_border_width = 4.0f;
  float cfg_pill_font_size = 20.0f;

  // Info section configuration
  float cfg_info_font_size = 16.0f;
  float cfg_info_line_height = 20.0f;

  // Shadow configuration
  float cfg_panel_shadow_offset_x = 4.0f;
  float cfg_panel_shadow_offset_y = 5.0f;
  int cfg_panel_shadow_alpha = 45;
  float cfg_btn_shadow_blur = 6.0f;
  int cfg_btn_shadow_alpha = 35;

  // Colors matching Angry Birds inspiration - warm, playful mobile game
  afterhours::Color bg_green{75, 135, 95,
                             255}; // Softer forest green background
  afterhours::Color header_coral{245, 145, 100,
                                 255}; // Warmer coral/orange header
  afterhours::Color header_dark{215, 105, 60, 255}; // Darker coral border
  afterhours::Color panel_cream{255, 250, 235,
                                255}; // Warm cream panel (slightly warmer)
  afterhours::Color panel_peach{255, 242, 222, 255}; // Inner peach
  afterhours::Color btn_green{115, 195, 85, 255};    // Bright lime green toggle
  afterhours::Color btn_green_dark{85, 160, 55, 255}; // Green shadow
  afterhours::Color btn_gray{155, 155, 155, 255};     // Gray for OFF state
  afterhours::Color btn_gray_dark{120, 120, 120, 255}; // Gray shadow
  afterhours::Color btn_blue{85, 155, 210, 255};      // Softer blue pill
  afterhours::Color btn_blue_dark{55, 120, 175, 255}; // Blue shadow
  afterhours::Color text_dark{65, 55, 45, 255};       // Dark brown text
  afterhours::Color text_muted{120, 105, 85, 255};    // Muted brown for info
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color close_red{235, 75, 85, 255};   // Brighter close button red
  afterhours::Color close_red_dark{195, 55, 65, 255}; // Darker close red
  afterhours::Color wifi_green{75, 195, 135, 255}; // WiFi icon green
  afterhours::Color wifi_green_dark{55, 165, 110, 255}; // WiFi green shadow

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = text_white;
    theme.font_muted = text_muted;
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
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_green)
            .with_debug_name("bg"));

    // ========== MAIN PANEL ==========
    float panel_w = cfg_panel_width;
    float panel_h = cfg_panel_height;
    float panel_x = ((float)screen_w - panel_w) / 2.0f;
    float panel_y = ((float)screen_h - panel_h) / 2.0f;

    // Panel shadow
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(panel_w),
                                     pxf(panel_h)})
            .with_absolute_position(panel_x + cfg_panel_shadow_offset_x,
                            panel_y + cfg_panel_shadow_offset_y)
            .with_custom_background(afterhours::Color{
                0, 0, 0, static_cast<unsigned char>(cfg_panel_shadow_alpha)})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(cfg_panel_roundness)
            .with_debug_name("panel_shadow"));

    // Main panel background
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(panel_w),
                                     pxf(panel_h)})
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_border(header_dark, cfg_panel_border_width)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(cfg_panel_roundness)
            .with_debug_name("panel"));

    // ========== CORAL HEADER ==========
    float header_inset = cfg_header_inset;
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(panel_w - header_inset * 2),
                pxf(cfg_header_height)})
            .with_absolute_position(panel_x + header_inset, panel_y + header_inset)
            .with_custom_background(header_coral)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.10f)
            .with_debug_name("header"));

    // Title
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(280), pixels(60)})
            .with_absolute_position(panel_x + panel_w / 2.0f - 140.0f,
                            panel_y + cfg_title_y_offset)
            .with_font("Fredoka", h720(cfg_title_font_size))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Close button (X) - positioned inside header bounds
    float close_x = panel_x + panel_w - cfg_close_btn_margin - cfg_close_btn_size;
    float close_y = panel_y + header_inset +
                    (cfg_header_height - cfg_close_btn_size) / 2.0f;
    button(context, mk(entity, 25),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{
                   pxf(cfg_close_btn_size),
                   pxf(cfg_close_btn_size)})
               .with_absolute_position(close_x, close_y)
               .with_custom_background(close_red)
               .with_border(close_red_dark, cfg_close_btn_border)
               .with_font("EqProRounded", h720(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(1.0f)
               .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                 afterhours::Color{0, 0, 0, 30})
               .with_debug_name("close_btn"));

    // ========== TOGGLE BUTTONS (Music, Sound, Vibration) ==========
    float toggle_y = panel_y + cfg_toggle_y_offset;
    float toggle_x = panel_x + cfg_toggle_x_offset;

    // Toggle buttons with ON/OFF text for clarity
    struct ToggleInfo {
      std::string label;
      bool *state;
    };
    std::vector<ToggleInfo> toggles = {
        {"Music", &music_on},
        {"Sound", &sound_on},
        {"Vibrate", &vibration_on},
    };

    for (size_t i = 0; i < toggles.size(); i++) {
      float tx = toggle_x + (float)i * cfg_toggle_spacing;
      bool is_on = *toggles[i].state;
      afterhours::Color bg_col = is_on ? btn_green : btn_gray;
      afterhours::Color border_col = is_on ? btn_green_dark : btn_gray_dark;
      std::string btn_text = is_on ? "ON" : "OFF";

      // Toggle button with ON/OFF text
      if (button(
              context, mk(entity, 30 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_label(btn_text)
                  .with_size(ComponentSize{
                      pxf(cfg_toggle_width),
                      pxf(cfg_toggle_height)})
                  .with_absolute_position(tx, toggle_y)
                  .with_custom_background(bg_col)
                  .with_border(border_col, cfg_toggle_border_width)
                  .with_font("EqProRounded", h720(cfg_toggle_font_size))
                  .with_custom_text_color(text_white)
                  .with_alignment(TextAlignment::Center)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.55f)
                  .with_soft_shadow(
                      2.0f, 3.0f, cfg_btn_shadow_blur,
                      afterhours::Color{0, 0, 0,
                                        static_cast<unsigned char>(
                                            cfg_btn_shadow_alpha)})
                  .with_debug_name("toggle_" + std::to_string(i)))) {
        *toggles[i].state = !(*toggles[i].state);
      }

      // Text label below the button
      div(context, mk(entity, 35 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(toggles[i].label)
              .with_size(ComponentSize{
                  pxf(cfg_toggle_width),
                  pixels(22)})
              .with_absolute_position(tx, toggle_y + cfg_toggle_height +
                                      cfg_toggle_label_gap)
              .with_font("EqProRounded", h720(cfg_toggle_label_font_size))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("toggle_label_" + std::to_string(i)));
    }

    // ========== SAVE/LOAD + SYNC GROUP ==========
    // Simplified single card design (no nested boxes)
    float saveload_x = panel_x + panel_w - cfg_saveload_width -
                       cfg_saveload_padding - 20.0f;
    float saveload_y = toggle_y - 4.0f;

    div(context, mk(entity, 39),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(cfg_saveload_width),
                pxf(cfg_saveload_height)})
            .with_absolute_position(saveload_x, saveload_y)
            .with_custom_background(panel_peach)
            .with_border(afterhours::Color{210, 195, 175, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.25f)
            .with_debug_name("save_sync_group"));

    // Save/Load Progress text (left side)
    div(context, mk(entity, 41),
        ComponentConfig{}
            .with_label("Save/Load")
            .with_size(ComponentSize{pixels(120), pixels(24)})
            .with_absolute_position(saveload_x + 14.0f, saveload_y + 14.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_debug_name("save_load_title"));

    div(context, mk(entity, 42),
        ComponentConfig{}
            .with_label("Progress")
            .with_size(ComponentSize{pixels(120), pixels(24)})
            .with_absolute_position(saveload_x + 14.0f, saveload_y + 40.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_debug_name("save_load_progress"));

    // Sync button (right side of group, vertically centered)
    float sync_x = saveload_x + cfg_saveload_width - cfg_sync_btn_size - 14.0f;
    float sync_y = saveload_y + (cfg_saveload_height - cfg_sync_btn_size) / 2.0f;
    button(context, mk(entity, 43),
           ComponentConfig{}
               .with_label("Sync")
               .with_size(ComponentSize{
                   pxf(cfg_sync_btn_size),
                   pxf(cfg_sync_btn_size)})
               .with_absolute_position(sync_x, sync_y)
               .with_custom_background(wifi_green)
               .with_border(wifi_green_dark, 3.0f)
               .with_font("EqProRounded", h720(14.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                 afterhours::Color{0, 0, 0, 25})
               .with_debug_name("sync_btn"));

    // ========== BLUE PILL BUTTONS ==========
    float btn_row1_y = toggle_y + cfg_toggle_height + 35.0f + cfg_toggle_label_gap;
    float btn_row2_y = btn_row1_y + cfg_pill_btn_spacing;
    float btn_row3_y = btn_row2_y + cfg_pill_btn_spacing;
    float left_btn_x = panel_x + cfg_pill_left_margin;
    float right_btn_x = panel_x + panel_w / 2.0f + cfg_pill_right_margin;

    // Notifications: ON/OFF (clickable)
    std::string notif_text =
        notifications_off ? "Notifications: OFF" : "Notifications: ON";
    afterhours::Color notif_icon_color =
        notifications_off ? close_red : btn_green;
    std::string notif_icon = notifications_off ? "X" : "OK";

    if (button(context, mk(entity, 50),
               ComponentConfig{}
                   .with_size(ComponentSize{
                       pxf(cfg_pill_btn_width),
                       pxf(cfg_pill_btn_height)})
                   .with_absolute_position(left_btn_x, btn_row1_y)
                   .with_custom_background(btn_blue)
                   .with_border(btn_blue_dark, cfg_pill_border_width)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f)
                   .with_soft_shadow(
                       2.0f, 3.0f, cfg_btn_shadow_blur,
                       afterhours::Color{
                           0, 0, 0,
                           static_cast<unsigned char>(cfg_btn_shadow_alpha)})
                   .with_debug_name("notifications_btn"))) {
      notifications_off = !notifications_off;
    }

    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label(notif_icon)
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position(left_btn_x + 16.0f,
                            btn_row1_y + (cfg_pill_btn_height - 26.0f) / 2.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(notif_icon_color)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("notif_icon"));

    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label(notif_text)
            .with_size(ComponentSize{pixels(200), pixels(28)})
            .with_absolute_position(left_btn_x + 46.0f,
                            btn_row1_y + (cfg_pill_btn_height - 28.0f) / 2.0f)
            .with_font("EqProRounded", h720(17.0f))
            .with_custom_text_color(text_white)
            .with_debug_name("notif_text"));

    // Credits
    button(context, mk(entity, 55),
           ComponentConfig{}
               .with_label("Credits")
               .with_size(ComponentSize{
                   pxf(cfg_pill_btn_width),
                   pxf(cfg_pill_btn_height)})
               .with_absolute_position(right_btn_x, btn_row1_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, cfg_pill_border_width)
               .with_font("EqProRounded", h720(cfg_pill_font_size))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_soft_shadow(
                   2.0f, 3.0f, cfg_btn_shadow_blur,
                   afterhours::Color{
                       0, 0, 0,
                       static_cast<unsigned char>(cfg_btn_shadow_alpha)})
               .with_debug_name("credits"));

    // Language
    button(context, mk(entity, 60),
           ComponentConfig{}
               .with_label("Language")
               .with_size(ComponentSize{
                   pxf(cfg_pill_btn_width),
                   pxf(cfg_pill_btn_height)})
               .with_absolute_position(left_btn_x, btn_row2_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, cfg_pill_border_width)
               .with_font("EqProRounded", h720(cfg_pill_font_size))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_soft_shadow(
                   2.0f, 3.0f, cfg_btn_shadow_blur,
                   afterhours::Color{
                       0, 0, 0,
                       static_cast<unsigned char>(cfg_btn_shadow_alpha)})
               .with_debug_name("language"));

    // Support
    button(context, mk(entity, 65),
           ComponentConfig{}
               .with_label("Support")
               .with_size(ComponentSize{
                   pxf(cfg_pill_btn_width),
                   pxf(cfg_pill_btn_height)})
               .with_absolute_position(right_btn_x, btn_row2_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, cfg_pill_border_width)
               .with_font("EqProRounded", h720(cfg_pill_font_size))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_soft_shadow(
                   2.0f, 3.0f, cfg_btn_shadow_blur,
                   afterhours::Color{
                       0, 0, 0,
                       static_cast<unsigned char>(cfg_btn_shadow_alpha)})
               .with_debug_name("support"));

    // ========== BOTTOM INFO (left column) ==========
    float info_y = btn_row3_y + 4.0f;
    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Build: 15555.1.114203")
            .with_size(ComponentSize{pixels(200),
                                     pxf(cfg_info_line_height)})
            .with_absolute_position(left_btn_x, info_y)
            .with_font("EqProRounded", h720(cfg_info_font_size))
            .with_custom_text_color(text_muted)
            .with_debug_name("build_id"));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_label("Version 1.11.0.12346")
            .with_size(ComponentSize{pixels(200),
                                     pxf(cfg_info_line_height)})
            .with_absolute_position(left_btn_x, info_y + cfg_info_line_height + 2.0f)
            .with_font("EqProRounded", h720(cfg_info_font_size))
            .with_custom_text_color(text_muted)
            .with_debug_name("version"));

    div(context, mk(entity, 72),
        ComponentConfig{}
            .with_label("Player: #281-676-956")
            .with_size(ComponentSize{pixels(200),
                                     pxf(cfg_info_line_height)})
            .with_absolute_position(left_btn_x,
                            info_y + (cfg_info_line_height + 2.0f) * 2.0f)
            .with_font("EqProRounded", h720(cfg_info_font_size))
            .with_custom_text_color(text_muted)
            .with_debug_name("player_id"));

    // Terms and Privacy
    button(context, mk(entity, 75),
           ComponentConfig{}
               .with_label("Terms and Privacy")
               .with_size(ComponentSize{
                   pxf(cfg_pill_btn_width),
                   pxf(cfg_pill_btn_height)})
               .with_absolute_position(right_btn_x, btn_row3_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, cfg_pill_border_width)
               .with_font("EqProRounded", h720(17.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_soft_shadow(
                   2.0f, 3.0f, cfg_btn_shadow_blur,
                   afterhours::Color{
                       0, 0, 0,
                       static_cast<unsigned char>(cfg_btn_shadow_alpha)})
               .with_debug_name("terms"));
  }
};

REGISTER_EXAMPLE_SCREEN(angry_birds_settings, "Game Mockups",
                        "Mobile game settings (Angry Birds style)",
                        AngryBirdsSettingsScreen)
