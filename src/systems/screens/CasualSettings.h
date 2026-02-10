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
  bool show_about = false;

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
  afterhours::Color text_muted{90, 75, 60, 255};  // Darker for 4.5:1 on cream
  afterhours::Color white{255, 255, 255, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", h720(18.0f));
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
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

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
            .with_720p_size(panel_w + 16, panel_h + 16)
            .with_absolute_position(panel_x - 8.0f, panel_y - 8.0f)
            .with_custom_background(panel_orange)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("panel_border"));

    // Cream inner panel
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_cream)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_debug_name("panel_inner"));

    // ========== TITLE: Settings ==========
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(200), pixels(45)})
            .with_absolute_position((float)screen_w / 2.0f - 100.0f, panel_y - 50.0f)
            .with_font("Gaegu-Bold", h720(38.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // ========== CLOSE BUTTON (X) ==========
    if (button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("X")
                   .with_size(ComponentSize{pixels(52), pixels(52)})
                   .with_absolute_position(panel_x + panel_w - 30.0f, panel_y - 10.0f)
                   .with_custom_background(close_red)
                   .with_border(afterhours::Color{190, 55, 65, 255}, 4.0f)
                   .with_font("Gaegu-Bold", h720(28.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(1.0f)
                   .with_debug_name("close_btn"))) {
      // Close action
    }

    // ========== TOGGLE BUTTONS (Music, Sound, Vibrate) ==========
    float toggle_y = panel_y + 40.0f;
    float toggle_start_x = panel_x + 40.0f;
    float toggle_spacing = 100.0f;

    std::vector<std::tuple<std::string, bool *, std::string>> toggles = {
        {"Music", &music_on, "music"},
        {"Sound", &sound_on, "sound"},
        {"Vibrate", &vibrate_on, "vibrate"}
    };

    constexpr float cs_track_w = 60.0f, cs_track_h = 32.0f;
    constexpr float cs_knob_pad = 4.0f;
    constexpr float cs_knob_sz = cs_track_h - cs_knob_pad * 2.0f;  // 24px
    constexpr float cs_knob_travel = cs_track_w - cs_knob_sz - cs_knob_pad * 2.0f;

    for (size_t i = 0; i < toggles.size(); i++) {
      auto &[label, state_ptr, name] = toggles[i];
      float tx = toggle_start_x + (float)i * toggle_spacing;
      bool is_on = *state_ptr;
      // OFF track: light warm gray visible on cream, ON: bright green
      afterhours::Color cs_track_off{185, 175, 165, 255};
      afterhours::Color track_col = is_on ? btn_green : cs_track_off;
      afterhours::Color track_border_col =
          is_on ? btn_green_dark : afterhours::Color{165, 155, 145, 255};

      float track_y = toggle_y + 10.0f;

      // IDs: i*4+55 range to avoid colliding with IDs 50-51 (Save/Load, Wifi)
      int base = 55 + static_cast<int>(i) * 4;

      // Track visual (div — immune to hover color override)
      div(context, mk(entity, base),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(cs_track_w), pxf(cs_track_h)})
              .with_absolute_position(tx, track_y)
              .with_custom_background(track_col)
              .with_border(track_border_col, 2.0f)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.5f)
              .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                afterhours::Color{0, 0, 0, 35})
              .with_debug_name("toggle_track_" + name));

      // Transparent click target (overlays the track — no background)
      if (button(context, mk(entity, base + 1),
                 ComponentConfig{}
                     .with_size(ComponentSize{pxf(cs_track_w), pxf(cs_track_h)})
                     .with_absolute_position(tx, track_y)
                     .with_color_usage(Theme::Usage::None)
                     .with_debug_name("toggle_btn_" + name))) {
        *state_ptr = !*state_ptr;
      }

      // White sliding knob (sibling div — screen coordinates)
      float knob_x = tx + cs_knob_pad + (is_on ? cs_knob_travel : 0.0f);
      float knob_y = track_y + cs_knob_pad;
      div(context, mk(entity, base + 2),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(cs_knob_sz), pxf(cs_knob_sz)})
              .with_absolute_position(knob_x, knob_y)
              .with_custom_background(white)
              .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(1.0f)
              .with_skip_tabbing(true)
              .with_debug_name("toggle_knob_" + name));

      // Label below
      std::string display = label + (is_on ? ": ON" : ": OFF");
      div(context, mk(entity, base + 3),
          ComponentConfig{}
              .with_label(display)
              .with_size(ComponentSize{pxf(cs_track_w + 30.0f), pixels(22)})
              .with_absolute_position(tx - 15.0f, track_y + cs_track_h + 4.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center));
    }

    // Wifi icon (positioned first, then Save/Load button to its left)
    float wifi_x = panel_x + panel_w - 75.0f;
    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Sync")
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_absolute_position(wifi_x, toggle_y + 5.0f)
            .with_custom_background(btn_green)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    // Save/Load Progress button (positioned to the left of wifi icon with gap)
    button(context, mk(entity, 50),
           ComponentConfig{}
               .with_label("Save/Load Progress")
               .with_size(ComponentSize{pixels(210), pixels(55)})
               .with_absolute_position(wifi_x - 220.0f, toggle_y)
               .with_custom_background(white)
               .with_border(afterhours::Color{200, 195, 185, 255}, 3.0f)
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f));

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

    // ========== SEPARATOR: Between toggles and menu buttons ==========
    div(context, mk(entity, 99),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((int)(panel_w - 80.0f)), pixels(1)})
            .with_absolute_position(panel_x + 40.0f, row_y - 15.0f)
            .with_custom_background(afterhours::Color{55, 45, 40, 40})
            .with_debug_name("section_separator_toggles"));

    // Left column
    for (size_t i = 0; i < left_buttons.size(); i++) {
      button(context, mk(entity, 100 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(left_buttons[i])
                 .with_720p_size(btn_w, btn_h)
                 .with_absolute_position(left_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", h720(22.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f)
                 .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                   afterhours::Color{0, 0, 0, 30})
                 .with_debug_name("left_btn_" + std::to_string(i)));
    }

    // Right column
    for (size_t i = 0; i < right_buttons.size(); i++) {
      button(context, mk(entity, 110 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(right_buttons[i])
                 .with_720p_size(btn_w, btn_h)
                 .with_absolute_position(right_x, row_y + (float)i * row_spacing)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", h720(22.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f)
                 .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                   afterhours::Color{0, 0, 0, 30})
                 .with_debug_name("right_btn_" + std::to_string(i)));
    }

    // ========== ABOUT BUTTON (replaces inline version info) ==========
    float info_y = panel_y + panel_h - 75.0f;

    if (button(context, mk(entity, 200),
               ComponentConfig{}
                   .with_label("About")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_absolute_position(left_x, info_y + 10.0f)
                   .with_custom_background(btn_blue)
                   .with_border(btn_blue_dark, 3.0f)
                   .with_font("Gaegu-Bold", h720(20.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f)
                   .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                     afterhours::Color{0, 0, 0, 30})
                   .with_debug_name("about_btn"))) {
      show_about = !show_about;
    }

    // Version display (simple, non-technical)
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Version 1.11.0")
            .with_size(ComponentSize{pixels(150), pixels(20)})
            .with_absolute_position(left_x + 135.0f, info_y + 22.0f)
            .with_font("Gaegu-Bold", h720(17.0f))
            .with_custom_text_color(text_muted));

    // ========== SEPARATOR BEFORE FOOTER ==========
    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((int)(panel_w - 80.0f)), pixels(1)})
            .with_absolute_position(panel_x + 40.0f, panel_y + panel_h - 60.0f)
            .with_custom_background(afterhours::Color{55, 45, 40, 40})
            .with_debug_name("section_separator_footer"));

    // ========== FOOTER: OK / Cancel / Apply ==========
    float footer_y = panel_y + panel_h - 50.0f;
    float footer_btn_x = panel_x + panel_w - 300.0f;

    button(context, mk(entity, 210),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x, footer_y)
               .with_custom_background(btn_green)
               .with_border(btn_green_dark, 3.0f)
               .with_font("Gaegu-Bold", h720(20.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_debug_name("btn_ok"));

    button(context, mk(entity, 211),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x + 90.0f, footer_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 3.0f)
               .with_font("Gaegu-Bold", h720(20.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_debug_name("btn_cancel"));

    button(context, mk(entity, 212),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x + 180.0f, footer_y)
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 3.0f)
               .with_font("Gaegu-Bold", h720(20.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_debug_name("btn_apply"));

    // ========== ABOUT PANEL (shows technical info when toggled) ==========
    if (show_about) {
      // About panel background overlay
      float about_w = 380.0f;
      float about_h = 180.0f;
      float about_x = (float)screen_w / 2.0f - about_w / 2.0f;
      float about_y = (float)screen_h / 2.0f - about_h / 2.0f;

      // Orange border for About panel
      div(context, mk(entity, 300),
          ComponentConfig{}
              .with_720p_size(about_w + 12, about_h + 12)
              .with_absolute_position(about_x - 6.0f, about_y - 6.0f)
              .with_custom_background(panel_orange)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.12f)
              .with_debug_name("about_border"));

      // Cream inner panel
      div(context, mk(entity, 301),
          ComponentConfig{}
              .with_720p_size(about_w, about_h)
              .with_absolute_position(about_x, about_y)
              .with_custom_background(panel_cream)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.1f)
              .with_debug_name("about_inner"));

      // About title
      div(context, mk(entity, 302),
          ComponentConfig{}
              .with_label("About")
              .with_size(ComponentSize{pixels(100), pixels(30)})
              .with_absolute_position(about_x + about_w / 2.0f - 50.0f, about_y + 12.0f)
              .with_font("Gaegu-Bold", h720(24.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center));

      // Build number
      div(context, mk(entity, 303),
          ComponentConfig{}
              .with_label("Build: 15555-1-114203-20-10200-01")
              .with_size(ComponentSize{pixels(340), pixels(22)})
              .with_absolute_position(about_x + 20.0f, about_y + 50.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted));

      // Full version
      div(context, mk(entity, 304),
          ComponentConfig{}
              .with_label("Version: 1.11.0.12346")
              .with_size(ComponentSize{pixels(220), pixels(22)})
              .with_absolute_position(about_x + 20.0f, about_y + 75.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted));

      // Player ID
      div(context, mk(entity, 305),
          ComponentConfig{}
              .with_label("Player ID: 281676956389")
              .with_size(ComponentSize{pixels(240), pixels(22)})
              .with_absolute_position(about_x + 20.0f, about_y + 100.0f)
              .with_font("Gaegu-Bold", h720(16.0f))
              .with_custom_text_color(text_muted));

      // Close About button
      if (button(context, mk(entity, 306),
                 ComponentConfig{}
                     .with_label("Close")
                     .with_size(ComponentSize{pixels(100), pixels(38)})
                     .with_absolute_position(about_x + about_w / 2.0f - 50.0f,
                                     about_y + about_h - 48.0f)
                     .with_custom_background(btn_green)
                     .with_border(btn_green_dark, 3.0f)
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.5f)
                     .with_debug_name("about_close"))) {
        show_about = false;
      }
    }

  }
};

REGISTER_EXAMPLE_SCREEN(casual_settings, "Game Mockups",
                        "Casual mobile settings (Angry Birds style)",
                        CasualSettingsScreen)
