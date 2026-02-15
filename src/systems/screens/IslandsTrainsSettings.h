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
  int display_mode = 0;
  int resolution = 1;
  int cam_pan_speed = 7;
  int cam_rotate_speed = 8;
  int effects_volume = 9;
  int music_volume = 10;

  // Colors matching Islands & Trains inspiration - warm olive/sage aesthetic
  afterhours::Color bg_olive_dark{110, 125, 100, 255};
  afterhours::Color bg_olive_mid{150, 162, 135, 255};
  afterhours::Color bg_olive_light{170, 180, 155, 255};

  afterhours::Color panel_cream{252, 252, 245, 255};
  afterhours::Color row_cream{235, 240, 228, 255};
  afterhours::Color text_dark{70, 80, 75, 255};
  afterhours::Color text_muted{145, 155, 145, 255};
  afterhours::Color header_olive{95, 135, 115, 255};
  afterhours::Color slider_teal{90, 155, 140, 255};
  afterhours::Color slider_empty{205, 175, 170, 255};
  afterhours::Color btn_cream{225, 232, 218, 255};
  afterhours::Color close_bg{245, 245, 238, 255};
  afterhours::Color close_border{200, 200, 190, 255};
  afterhours::Color arrow_color{145, 155, 145, 255};

  std::vector<std::string> modes = {"Borderless", "Windowed", "Fullscreen"};
  std::vector<std::string> resolutions = {"1920x1080", "2560x1440",
                                          "3840x2160"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(15.0f));
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
    context.scaling_mode = ScalingMode::Adaptive;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    float sw = static_cast<float>(screen_w);
    float sh = static_cast<float>(screen_h);

    // ========== VIGNETTE BACKGROUND (decorative, remains absolute) ==========
    div(context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_olive_mid)
            .with_debug_name("bg_base"));

    ui_workarounds::vertical_gradient(context, entity, 1, 0.0f, 0.0f, sw,
                                      sh * 0.3f, bg_olive_dark, bg_olive_mid,
                                      8);
    ui_workarounds::vertical_gradient(context, entity, 10, 0.0f, sh * 0.7f, sw,
                                      sh * 0.3f, bg_olive_mid, bg_olive_dark,
                                      8);
    ui_workarounds::horizontal_gradient(
        context, entity, 20, 0.0f, 0.0f, sw * 0.2f, sh,
        afterhours::Color{bg_olive_dark.r, bg_olive_dark.g, bg_olive_dark.b,
                          200},
        afterhours::Color{bg_olive_mid.r, bg_olive_mid.g, bg_olive_mid.b, 0},
        6);
    ui_workarounds::horizontal_gradient(
        context, entity, 30, sw * 0.8f, 0.0f, sw * 0.2f, sh,
        afterhours::Color{bg_olive_mid.r, bg_olive_mid.g, bg_olive_mid.b, 0},
        afterhours::Color{bg_olive_dark.r, bg_olive_dark.g, bg_olive_dark.b,
                          200},
        6);

    // ═══════════════════════════════════════════════════════════════
    // MAIN LAYOUT (overlaid on gradient background)
    // ═══════════════════════════════════════════════════════════════
    auto root = vstack(
        context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.top = pixels(35), .bottom = pixels(30)})
            .with_debug_name("it_root"));

    // Close button (top-right, using translate to position)
    auto close_row = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_no_wrap()
            .with_padding(Padding{.right = pixels(25)})
            .with_debug_name("close_row"));

    button(context, mk(close_row.ent()),
           ComponentConfig{}
               .with_label("X")
               .with_720p_size(50, 50)
               .with_custom_background(close_bg)
               .with_border(close_border, 2.0f)
               .with_font("EqProRounded", pixels(28.0f))
               .with_custom_text_color(afterhours::Color{130, 140, 130, 255})
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(1.0f));

    // Panel
    auto panel = vstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_720p_size(430, 530)
            .with_custom_background(panel_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.03f)
            .with_soft_shadow(4.0f, 6.0f, 20.0f,
                              afterhours::Color{50, 60, 50, 30})
            .with_padding(Padding{.top = pixels(18), .left = pixels(15),
                                  .bottom = pixels(15), .right = pixels(15)})
            .with_no_wrap()
            .with_margin(Margin{.top = pixels(-50)})
            .with_debug_name("panel"));

    // Title
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_font("EqProRounded", pixels(30.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // ── DISPLAY section ──
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("DISPLAY")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(header_olive)
            .with_margin(Margin{.top = pixels(8)}));

    render_selector_row(context, panel.ent(), 10, "Mode", display_mode,
                        static_cast<int>(modes.size()), true);
    render_selector_row(context, panel.ent(), 11, "Resolution", resolution,
                        static_cast<int>(resolutions.size()), false);

    // ── CONTROLS section ──
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("CONTROLS")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(header_olive)
            .with_margin(Margin{.top = pixels(8)}));

    render_slider_row(context, panel.ent(), 20, "Cam panning speed",
                      cam_pan_speed, 10);
    render_slider_row(context, panel.ent(), 21, "Cam rotating Speed",
                      cam_rotate_speed, 10);

    button(context, mk(panel.ent()),
           ComponentConfig{}
               .with_label("KEYBOARD")
               .with_720p_size(400, 38)
               .with_custom_background(btn_cream)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_margin(Margin{.top = pixels(5)}));

    // ── AUDIO section ──
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("AUDIO")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(header_olive)
            .with_margin(Margin{.top = pixels(8)}));

    render_slider_row(context, panel.ent(), 30, "Effects Volume",
                      effects_volume, 10);
    render_slider_row(context, panel.ent(), 31, "Music Volume", music_volume,
                      10);

    // ── TUTORIAL section ──
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("TUTORIAL")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(header_olive)
            .with_margin(Margin{.top = pixels(8)}));

    button(context, mk(panel.ent()),
           ComponentConfig{}
               .with_label("PLAY TUTORIAL")
               .with_720p_size(400, 38)
               .with_custom_background(btn_cream)
               .with_custom_text_color(text_muted)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_margin(Margin{.top = pixels(2)}));

    // ── RESET TO DEFAULTS (below panel) ──
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("RESET TO DEFAULTS")
            .with_size(ComponentSize{pixels(250), pixels(30)})
            .with_font("EqProRounded", pixels(16.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(20)}));
  }

  // ═══════════════════════════════════════════════════════════════
  // Helper: selector row (label + < value >) as layout children
  // ═══════════════════════════════════════════════════════════════
  void render_selector_row(UIContext<InputAction> &context,
                           afterhours::Entity &parent, int id,
                           const std::string &label, int &value_idx,
                           int max_options, bool is_mode) {
    auto row = hstack(
        context, mk(parent, id),
        ComponentConfig{}
            .with_720p_size(400, 38)
            .with_custom_background(row_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.55f)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.left = pixels(16), .right = pixels(8)})
            .with_margin(Margin{.top = pixels(5)}));

    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(160), pixels(30)})
            .with_custom_text_color(text_dark));

    // Spacer
    div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    if (button(context, mk(row.ent(), 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(ComponentSize{pixels(28), pixels(30)})
                   .with_font("EqProRounded", pixels(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      value_idx = (value_idx == 0) ? max_options - 1 : value_idx - 1;
    }

    std::string display_value =
        is_mode ? modes[static_cast<size_t>(value_idx)]
                : resolutions[static_cast<size_t>(value_idx)];

    div(context, mk(row.ent(), 3),
        ComponentConfig{}
            .with_label(display_value)
            .with_size(ComponentSize{pixels(110), pixels(30)})
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(row.ent(), 4),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(ComponentSize{pixels(28), pixels(30)})
                   .with_font("EqProRounded", pixels(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      value_idx = (value_idx + 1) % max_options;
    }
  }

  // ═══════════════════════════════════════════════════════════════
  // Helper: slider row (label + < segments >) as layout children
  // ═══════════════════════════════════════════════════════════════
  void render_slider_row(UIContext<InputAction> &context,
                         afterhours::Entity &parent, int id,
                         const std::string &label, int &value, int max_val) {
    auto row = hstack(
        context, mk(parent, id),
        ComponentConfig{}
            .with_720p_size(400, 38)
            .with_custom_background(row_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.55f)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.left = pixels(16), .right = pixels(8)})
            .with_margin(Margin{.top = pixels(5)}));

    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(175), pixels(30)})
            .with_custom_text_color(text_dark));

    // Spacer
    div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    if (button(context, mk(row.ent(), 2),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(ComponentSize{pixels(28), pixels(30)})
                   .with_font("EqProRounded", pixels(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      if (value > 0)
        value--;
    }

    // Segments
    auto segs = hstack(
        context, mk(row.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(130), pixels(15)})
            .with_no_wrap()
            .with_align_items(AlignItems::Center));

    for (int si = 0; si < max_val; si++) {
      bool is_filled = (si < value);
      afterhours::Color seg_color = is_filled ? slider_teal : slider_empty;
      if (button(context, mk(segs.ent(), si),
                 ComponentConfig{}
                     .with_720p_size(10, 15)
                     .with_custom_background(seg_color)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.2f)
                     .with_margin(si > 0 ? Margin{.left = pixels(2)}
                                         : Margin{}))) {
        value = si + 1;
      }
    }

    if (button(context, mk(row.ent(), 4),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(ComponentSize{pixels(28), pixels(30)})
                   .with_font("EqProRounded", pixels(17.0f))
                   .with_custom_text_color(arrow_color)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center))) {
      if (value < max_val)
        value++;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(islands_trains_settings, "Game Mockups",
                        "Calm puzzle game settings (Islands & Trains style)",
                        IslandsTrainsSettingsScreen)
