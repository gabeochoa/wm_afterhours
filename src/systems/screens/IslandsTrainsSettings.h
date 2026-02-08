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
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(15.0f));
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
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
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
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_rounded_corners(RoundedCorners())
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
               .with_720p_size(close_size, close_size)
               .with_absolute_position(close_x, close_y)
               .with_custom_background(close_bg)
               .with_border(close_border, 2.0f)
               .with_font("EqProRounded", h720(28.0f))
               .with_custom_text_color(afterhours::Color{130, 140, 130, 255})
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(1.0f));

    // ========== TITLE ==========
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(250), pixels(45)})
            .with_absolute_position(panel_x + panel_w / 2.0f - 125.0f, panel_y + 18.0f)
            .with_font("EqProRounded", h720(30.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    float content_x = panel_x + 15.0f;
    float content_y = panel_y + 60.0f;
    float row_w = panel_w - 30.0f;
    float row_h = 38.0f;
    float section_gap = 5.0f;        // Tighter gap between rows
    float section_header_gap = 8.0f; // Gap after section header

    // Section header helper
    auto section_header = [&](int id, const char *label, float y) {
      div(context, mk(entity, id),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(120), pixels(22)})
              .with_absolute_position(content_x, y)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(header_olive));
    };

    // Pill-style action button helper
    auto action_button = [&](int id, const char *label, float y) {
      button(context, mk(entity, id),
             ComponentConfig{}
                 .with_label(label)
                 .with_720p_size(row_w, row_h)
                 .with_absolute_position(content_x, y)
                 .with_custom_background(btn_cream)
                 .with_custom_text_color(text_muted)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f));
    };

    // ========== DISPLAY SECTION ==========
    section_header(100, "DISPLAY", content_y);
    float display_row_y = content_y + 22.0f;
    render_selector_row(context, entity, 110, content_x, display_row_y, row_w,
                        row_h, "Mode", display_mode, static_cast<int>(modes.size()));
    render_selector_row(context, entity, 120, content_x,
                        display_row_y + row_h + section_gap, row_w, row_h,
                        "Resolution", resolution, static_cast<int>(resolutions.size()));

    // ========== CONTROLS SECTION ==========
    float controls_y = display_row_y + 2 * (row_h + section_gap) + section_header_gap;
    section_header(130, "CONTROLS", controls_y);
    float controls_row_y = controls_y + 22.0f;
    render_slider_row(context, entity, 140, content_x, controls_row_y, row_w,
                      row_h, "Cam panning speed", cam_pan_speed, 10);
    render_slider_row(context, entity, 160, content_x,
                      controls_row_y + row_h + section_gap, row_w, row_h,
                      "Cam rotating Speed", cam_rotate_speed, 10);
    action_button(180, "KEYBOARD", controls_row_y + 2 * (row_h + section_gap));

    // ========== AUDIO SECTION ==========
    float audio_y = controls_row_y + 3 * (row_h + section_gap) + section_header_gap;
    section_header(190, "AUDIO", audio_y);
    float audio_row_y = audio_y + 22.0f;
    render_slider_row(context, entity, 200, content_x, audio_row_y, row_w,
                      row_h, "Effects Volume", effects_volume, 10);
    render_slider_row(context, entity, 220, content_x,
                      audio_row_y + row_h + section_gap, row_w, row_h,
                      "Music Volume", music_volume, 10);

    // ========== TUTORIAL SECTION ==========
    float tutorial_y = audio_row_y + 2 * (row_h + section_gap) + section_header_gap;
    section_header(240, "TUTORIAL", tutorial_y);
    action_button(250, "PLAY TUTORIAL", tutorial_y + 22.0f);

    // ========== RESET TO DEFAULTS ==========
    // Positioned at SCREEN bottom, outside the panel (matching inspiration)
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("RESET TO DEFAULTS")
            .with_size(ComponentSize{pixels(250), pixels(30)})
            .with_absolute_position(sw / 2.0f - 125.0f, sh - 70.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));
  }

  void render_selector_row(UIContext<InputAction> &context,
                           afterhours::Entity &entity, int base_id, float x,
                           float y, float w, float h, const std::string &label,
                           int &value_idx, int max_options) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    // Row background - pill shaped with subtle roundness
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_720p_size(w, h)
            .with_absolute_position(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.55f));

    // Label (left-aligned)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(160), pxf(h)})
            .with_absolute_position(x + 16.0f, y + 8.0f)
            .with_custom_text_color(text_dark));

    // Left chevron (<) - clickable
    if (button(context, mk(entity, base_id + 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(
                       ComponentSize{pixels(28), pxf(h)})
                   .with_absolute_position(x + w - 165.0f, y + 7.0f)
                   .with_font("EqProRounded", h720(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
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
            .with_size(ComponentSize{pixels(110), pxf(h)})
            .with_absolute_position(x + w - 140.0f, y + 8.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Right chevron (>) - clickable
    if (button(context, mk(entity, base_id + 4),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(
                       ComponentSize{pixels(28), pxf(h)})
                   .with_absolute_position(x + w - 28.0f, y + 7.0f)
                   .with_font("EqProRounded", h720(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      value_idx = (value_idx + 1) % max_options;
    }
  }

  void render_slider_row(UIContext<InputAction> &context,
                         afterhours::Entity &entity, int base_id, float x,
                         float y, float w, float h, const std::string &label,
                         int &value, int max_val) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    // Row background - pill shaped
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_720p_size(w, h)
            .with_absolute_position(x, y)
            .with_custom_background(row_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.55f));

    // Label (left-aligned)
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(175), pxf(h)})
            .with_absolute_position(x + 16.0f, y + 8.0f)
            .with_custom_text_color(text_dark));

    // Left chevron (<) - clickable
    if (button(context, mk(entity, base_id + 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(
                       ComponentSize{pixels(28), pxf(h)})
                   .with_absolute_position(x + w - 190.0f, y + 7.0f)
                   .with_font("EqProRounded", h720(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
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
                     .with_720p_size(seg_w, seg_h)
                     .with_absolute_position(slider_start_x + static_cast<float>(i) *
                                                          (seg_w + seg_gap),
                                     y + 11.0f)
                     .with_custom_background(seg_color)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.2f))) {
        value = i + 1; // Set value to this segment
      }
    }

    // Right chevron (>) - clickable
    if (button(context, mk(entity, base_id + 3),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(
                       ComponentSize{pixels(28), pxf(h)})
                   .with_absolute_position(x + w - 28.0f, y + 7.0f)
                   .with_font("EqProRounded", h720(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      if (value < max_val)
        value++;
    }
  }
};

// TODO: Fix entity ID conflict in vertical_gradient before re-enabling
// REGISTER_EXAMPLE_SCREEN(islands_trains_settings, "Game Mockups",
//                         "Calm puzzle game settings (Islands & Trains style)",
//                         IslandsTrainsSettingsScreen)
