#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/GradientBackground.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct IslandsTrainsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int display_mode = 0;     // 0=Borderless, 1=Windowed, 2=Fullscreen
  int resolution = 1;       // Index into resolutions (2560x1440)
  int cam_pan_speed = 7;    // 0-10
  int cam_rotate_speed = 8; // 0-10
  int effects_volume = 9;   // 0-10
  int music_volume = 10;    // 0-10

  // Colors matching Islands & Trains inspiration - warm olive/sage aesthetic
  // Background uses a vignette effect (darker edges, lighter center)
  // The inspiration has warm olive-brown tones with strong vignette
  afterhours::Color bg_olive_dark{110, 125, 100,
                                  255}; // Darker olive for edges (stronger)
  afterhours::Color bg_olive_mid{150, 162, 135, 255}; // Mid olive-sage
  afterhours::Color bg_olive_light{170, 180, 155,
                                   255}; // Lighter olive for center

  // Panel and UI elements
  afterhours::Color panel_cream{252, 252, 245,
                                255}; // Warm cream panel (more opaque)
  afterhours::Color row_cream{235, 240, 228,
                              255}; // Row background - soft sage-cream
  afterhours::Color text_dark{70, 80, 75,
                              255}; // Dark gray-green text for labels
  afterhours::Color text_muted{145, 155, 145,
                               255}; // Muted text for secondary elements
  afterhours::Color header_olive{
      95, 135, 115, 255}; // Section header - more saturated teal-olive
  afterhours::Color slider_teal{90, 155, 140,
                                255}; // Slider filled - more saturated teal
  afterhours::Color slider_empty{205, 175, 170,
                                 255}; // Slider empty - dusty rose/mauve
  afterhours::Color btn_cream{225, 232, 218, 255};    // Button background
  afterhours::Color close_bg{245, 245, 238, 255};     // Close button background
  afterhours::Color close_border{200, 200, 190, 255}; // Close button border
  afterhours::Color arrow_color{145, 155, 145, 255};  // Chevron arrows

  std::vector<std::string> modes = {"Borderless", "Windowed", "Fullscreen"};
  std::vector<std::string> resolutions = {"1920x1080", "2560x1440",
                                          "3840x2160"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = panel_cream;
    theme.font_muted = text_muted;
    theme.background = bg_olive_mid;
    theme.surface = panel_cream;
    theme.primary = header_olive;
    theme.secondary = slider_teal;
    theme.accent = slider_teal;
    theme.error = afterhours::Color{180, 100, 100, 255};
    theme.roundness = 0.12f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    float sw = static_cast<float>(screen_w);
    float sh = static_cast<float>(screen_h);

    // ========== VIGNETTE BACKGROUND ==========
    // Base layer - medium olive-sage
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_olive_mid)
            .with_debug_name("bg_base"));

    // Top edge gradient (darker at top) - strong vignette
    ui_workarounds::vertical_gradient(context, entity, 1, 0.0f, 0.0f, sw,
                                      sh * 0.3f, bg_olive_dark, bg_olive_mid,
                                      8);

    // Bottom edge gradient (darker at bottom)
    ui_workarounds::vertical_gradient(context, entity, 10, 0.0f, sh * 0.7f, sw,
                                      sh * 0.3f, bg_olive_mid, bg_olive_dark,
                                      8);

    // Left edge gradient (darker at left)
    ui_workarounds::horizontal_gradient(
        context, entity, 20, 0.0f, 0.0f, sw * 0.2f, sh,
        afterhours::Color{bg_olive_dark.r, bg_olive_dark.g, bg_olive_dark.b,
                          200},
        afterhours::Color{bg_olive_mid.r, bg_olive_mid.g, bg_olive_mid.b, 0},
        6);

    // Right edge gradient (darker at right)
    ui_workarounds::horizontal_gradient(
        context, entity, 30, sw * 0.8f, 0.0f, sw * 0.2f, sh,
        afterhours::Color{bg_olive_mid.r, bg_olive_mid.g, bg_olive_mid.b, 0},
        afterhours::Color{bg_olive_dark.r, bg_olive_dark.g, bg_olive_dark.b,
                          200},
        6);

    // ========== MAIN PANEL ==========
    // Inspiration: panel is narrower, taller, positioned in upper-center
    // Content fills more of vertical space, RESET TO DEFAULTS is below panel
    float panel_w = 430.0f; // Narrower to match inspiration
    float panel_h = 530.0f;
    float panel_x = (sw - panel_w) / 2.0f;
    float panel_y = 35.0f; // Near top

    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.03f)
            .with_soft_shadow(4.0f, 6.0f, 20.0f,
                              afterhours::Color{50, 60, 50, 30})
            .with_debug_name("panel"));

    // ========== CLOSE BUTTON (X) ==========
    // Positioned in top-right of screen - circular with bold X
    float close_x = sw - 75.0f;
    float close_y = 25.0f;
    float close_size = 50.0f;

    button(context, mk(entity, 55),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{pixels(static_cast<int>(close_size)),
                                        pixels(static_cast<int>(close_size))})
               .with_absolute_position()
               .with_translate(close_x, close_y)
               .with_custom_background(close_bg)
               .with_border(close_border, 2.0f)
               .with_font("EqProRounded", 28.0f)
               .with_custom_text_color(afterhours::Color{130, 140, 130, 255})
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(1.0f)
               .with_debug_name("close"));

    // ========== TITLE ==========
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(250), pixels(45)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f - 125.0f, panel_y + 18.0f)
            .with_font("EqProRounded", 30.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    float content_x = panel_x + 15.0f;
    float content_y = panel_y + 60.0f;
    float row_w = panel_w - 30.0f;
    float row_h = 38.0f;
    float section_gap = 5.0f;        // Tighter gap between rows
    float section_header_gap = 8.0f; // Gap after section header

    // ========== DISPLAY SECTION ==========
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("DISPLAY")
            .with_size(ComponentSize{pixels(100), pixels(22)})
            .with_absolute_position()
            .with_translate(content_x, content_y)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(header_olive)
            .with_debug_name("display_header"));

    float display_row_y = content_y + 22.0f;

    // Mode row - interactive
    render_selector_row(context, entity, 110, content_x, display_row_y, row_w,
                        row_h, "Mode", display_mode,
                        static_cast<int>(modes.size()));

    // Resolution row - interactive
    render_selector_row(context, entity, 120, content_x,
                        display_row_y + row_h + section_gap, row_w, row_h,
                        "Resolution", resolution,
                        static_cast<int>(resolutions.size()));

    // ========== CONTROLS SECTION ==========
    float controls_y =
        display_row_y + 2 * (row_h + section_gap) + section_header_gap;
    div(context, mk(entity, 130),
        ComponentConfig{}
            .with_label("CONTROLS")
            .with_size(ComponentSize{pixels(120), pixels(22)})
            .with_absolute_position()
            .with_translate(content_x, controls_y)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(header_olive)
            .with_debug_name("controls_header"));

    float controls_row_y = controls_y + 22.0f;

    // Cam panning speed
    render_slider_row(context, entity, 140, content_x, controls_row_y, row_w,
                      row_h, "Cam panning speed", cam_pan_speed, 10);

    // Cam rotating speed
    render_slider_row(context, entity, 160, content_x,
                      controls_row_y + row_h + section_gap, row_w, row_h,
                      "Cam rotating Speed", cam_rotate_speed, 10);

    // Keyboard button
    button(context, mk(entity, 180),
           ComponentConfig{}
               .with_label("KEYBOARD")
               .with_size(ComponentSize{pixels(static_cast<int>(row_w)),
                                        pixels(static_cast<int>(row_h))})
               .with_absolute_position()
               .with_translate(content_x,
                               controls_row_y + 2 * (row_h + section_gap))
               .with_custom_background(btn_cream)
               .with_font("EqProRounded", 15.0f)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("keyboard_btn"));

    // ========== AUDIO SECTION ==========
    float audio_y =
        controls_row_y + 3 * (row_h + section_gap) + section_header_gap;
    div(context, mk(entity, 190),
        ComponentConfig{}
            .with_label("AUDIO")
            .with_size(ComponentSize{pixels(80), pixels(22)})
            .with_absolute_position()
            .with_translate(content_x, audio_y)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(header_olive)
            .with_debug_name("audio_header"));

    float audio_row_y = audio_y + 22.0f;

    // Effects Volume
    render_slider_row(context, entity, 200, content_x, audio_row_y, row_w,
                      row_h, "Effects Volume", effects_volume, 10);

    // Music Volume
    render_slider_row(context, entity, 220, content_x,
                      audio_row_y + row_h + section_gap, row_w, row_h,
                      "Music Volume", music_volume, 10);

    // ========== TUTORIAL SECTION ==========
    float tutorial_y =
        audio_row_y + 2 * (row_h + section_gap) + section_header_gap;
    div(context, mk(entity, 240),
        ComponentConfig{}
            .with_label("TUTORIAL")
            .with_size(ComponentSize{pixels(100), pixels(22)})
            .with_absolute_position()
            .with_translate(content_x, tutorial_y)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(header_olive)
            .with_debug_name("tutorial_header"));

    button(context, mk(entity, 250),
           ComponentConfig{}
               .with_label("PLAY TUTORIAL")
               .with_size(ComponentSize{pixels(static_cast<int>(row_w)),
                                        pixels(static_cast<int>(row_h))})
               .with_absolute_position()
               .with_translate(content_x, tutorial_y + 22.0f)
               .with_custom_background(btn_cream)
               .with_font("EqProRounded", 15.0f)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_debug_name("tutorial_btn"));

    // ========== RESET TO DEFAULTS ==========
    // Positioned at SCREEN bottom, outside the panel (matching inspiration)
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("RESET TO DEFAULTS")
            .with_size(ComponentSize{pixels(250), pixels(30)})
            .with_absolute_position()
            .with_translate(sw / 2.0f - 125.0f, sh - 70.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("reset"));
  }

  void render_selector_row(UIContext<InputAction> &context,
                           afterhours::Entity &entity, int base_id, float x,
                           float y, float w, float h, const std::string &label,
                           int &value_idx, int max_options) {
    // Row background - pill shaped with subtle roundness
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)),
                                     pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.55f)
            .with_debug_name("row_" + std::to_string(base_id)));

    // Label (left-aligned)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(160), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 16.0f, y + 8.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("label_" + std::to_string(base_id)));

    // Left chevron (<) - clickable
    if (button(context, mk(entity, base_id + 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(
                       ComponentSize{pixels(28), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x + w - 165.0f, y + 7.0f)
                   .with_font("EqProRounded", 17.0f)
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("chevron_l_" + std::to_string(base_id)))) {
      value_idx = (value_idx == 0) ? max_options - 1 : value_idx - 1;
    }

    // Get display value
    std::string display_value;
    if (base_id == 110) {
      display_value = modes[static_cast<size_t>(value_idx)];
    } else {
      display_value = resolutions[static_cast<size_t>(value_idx)];
    }

    // Value (center-right)
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(display_value)
            .with_size(ComponentSize{pixels(110), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + w - 140.0f, y + 8.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("value_" + std::to_string(base_id)));

    // Right chevron (>) - clickable
    if (button(context, mk(entity, base_id + 4),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(
                       ComponentSize{pixels(28), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x + w - 28.0f, y + 7.0f)
                   .with_font("EqProRounded", 17.0f)
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("chevron_r_" + std::to_string(base_id)))) {
      value_idx = (value_idx + 1) % max_options;
    }
  }

  void render_slider_row(UIContext<InputAction> &context,
                         afterhours::Entity &entity, int base_id, float x,
                         float y, float w, float h, const std::string &label,
                         int &value, int max_val) {
    // Row background - pill shaped
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(w)),
                                     pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.55f)
            .with_debug_name("row_" + std::to_string(base_id)));

    // Label (left-aligned)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(175), pixels(static_cast<int>(h))})
            .with_absolute_position()
            .with_translate(x + 16.0f, y + 8.0f)
            .with_font("EqProRounded", 15.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("label_" + std::to_string(base_id)));

    // Left chevron (<) - clickable
    if (button(context, mk(entity, base_id + 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(
                       ComponentSize{pixels(28), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x + w - 190.0f, y + 7.0f)
                   .with_font("EqProRounded", 17.0f)
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("chevron_l_" + std::to_string(base_id)))) {
      if (value > 0)
        value--;
    }

    // Segmented slider - positioned between arrows
    // 10 segments with gaps, matching inspiration
    float slider_start_x = x + w - 165.0f;
    float seg_w = 10.0f;
    float seg_h = 15.0f;
    float seg_gap = 2.0f;

    for (int i = 0; i < max_val; i++) {
      bool is_filled = (i < value);
      // Saturated teal when filled, dusty rose when empty
      afterhours::Color seg_color = is_filled ? slider_teal : slider_empty;

      // Each segment is clickable to set value
      if (button(context, mk(entity, base_id + 10 + i),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(static_cast<int>(seg_w)),
                                              pixels(static_cast<int>(seg_h))})
                     .with_absolute_position()
                     .with_translate(slider_start_x + static_cast<float>(i) *
                                                          (seg_w + seg_gap),
                                     y + 11.0f)
                     .with_custom_background(seg_color)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.2f)
                     .with_debug_name("seg_" + std::to_string(base_id) + "_" +
                                      std::to_string(i)))) {
        value = i + 1; // Set value to this segment
      }
    }

    // Right chevron (>) - clickable
    if (button(context, mk(entity, base_id + 3),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(
                       ComponentSize{pixels(28), pixels(static_cast<int>(h))})
                   .with_absolute_position()
                   .with_translate(x + w - 28.0f, y + 7.0f)
                   .with_font("EqProRounded", 17.0f)
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("chevron_r_" + std::to_string(base_id)))) {
      if (value < max_val)
        value++;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(islands_trains_settings, "Game Mockups",
                        "Calm puzzle game settings (Islands & Trains style)",
                        IslandsTrainsSettingsScreen)
