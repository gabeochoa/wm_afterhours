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
  size_t active_tab = 0;
  bool music_on = true;
  bool sound_on = true;
  bool vibration_on = false;
  bool notifications_off = true;

  // === CONFIGURABLE SIZE PARAMETERS ===
  float cfg_panel_width = 720.0f;
  float cfg_panel_height = 480.0f;
  float cfg_panel_border_width = 6.0f;
  float cfg_panel_roundness = 0.12f;
  float cfg_header_height = 85.0f;
  float cfg_title_font_size = 44.0f;
  float cfg_close_btn_size = 44.0f;
  float cfg_close_btn_border = 3.0f;
  float cfg_toggle_track_w = 60.0f;
  float cfg_toggle_track_h = 32.0f;
  float cfg_toggle_knob_sz = 24.0f;
  float cfg_toggle_pad = 4.0f;
  float cfg_toggle_label_font_size = 16.0f;
  float cfg_saveload_width = 240.0f;
  float cfg_saveload_height = 80.0f;
  float cfg_sync_btn_size = 44.0f;
  float cfg_pill_btn_width = 295.0f;
  float cfg_pill_btn_height = 48.0f;
  float cfg_pill_border_width = 4.0f;
  float cfg_pill_font_size = 20.0f;
  float cfg_info_font_size = 16.0f;
  float cfg_info_line_height = 20.0f;
  int cfg_panel_shadow_alpha = 45;
  float cfg_btn_shadow_blur = 6.0f;
  int cfg_btn_shadow_alpha = 35;

  // Colors matching Angry Birds inspiration - warm, playful mobile game
  afterhours::Color bg_green{75, 135, 95, 255};
  afterhours::Color header_coral{245, 145, 100, 255};
  afterhours::Color header_dark{215, 105, 60, 255};
  afterhours::Color panel_cream{255, 250, 235, 255};
  afterhours::Color panel_peach{255, 242, 222, 255};
  afterhours::Color btn_green{115, 195, 85, 255};
  afterhours::Color btn_green_dark{85, 160, 55, 255};
  afterhours::Color btn_gray{155, 155, 155, 255};
  afterhours::Color btn_gray_dark{120, 120, 120, 255};
  afterhours::Color btn_blue{85, 155, 210, 255};
  afterhours::Color btn_blue_dark{55, 120, 175, 255};
  afterhours::Color text_dark{65, 55, 45, 255};
  afterhours::Color text_muted{95, 80, 60, 255};
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color close_red{235, 75, 85, 255};
  afterhours::Color close_red_dark{195, 55, 65, 255};
  afterhours::Color wifi_green{75, 195, 135, 255};
  afterhours::Color wifi_green_dark{55, 165, 110, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(18.0f));
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = text_white;
    theme.font_muted = afterhours::Color{225, 215, 195, 255};
    theme.background = bg_green;
    theme.surface = panel_cream;
    theme.primary = header_coral;
    theme.secondary = btn_blue;
    theme.accent = btn_green;
    theme.error = close_red;
    theme.roundness = 0.15f;
    theme.segments = 12;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    afterhours::Color pill_shadow{
        0, 0, 0, static_cast<unsigned char>(cfg_btn_shadow_alpha)};

    // ═══════════════════════════════════════════════════════════════
    // ROOT - full screen, center content
    // ═══════════════════════════════════════════════════════════════
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg_green)
                   .with_align_items(AlignItems::Center)
                   .with_justify_content(JustifyContent::Center)
                   .with_no_wrap()
                   .with_debug_name("ab_root"));

    // ═══════════════════════════════════════════════════════════════
    // PANEL
    // ═══════════════════════════════════════════════════════════════
    auto panel = vstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_720p_size(cfg_panel_width, cfg_panel_height)
            .with_custom_background(panel_cream)
            .with_border(header_dark, cfg_panel_border_width)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(cfg_panel_roundness)
            .with_soft_shadow(4.0f, 5.0f, 12.0f,
                              afterhours::Color{0, 0, 0,
                                                static_cast<unsigned char>(
                                                    cfg_panel_shadow_alpha)})
            .with_no_wrap()
            .with_debug_name("panel"));

    // ── Coral header ──
    auto header = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(cfg_header_height)})
            .with_custom_background(header_coral)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.10f)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.left = pixels(20), .right = pixels(20)})
            .with_debug_name("header"));

    div(context, mk(header.ent()),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(280), pixels(60)})
            .with_font("Fredoka", pixels(cfg_title_font_size))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Spacer
    div(context, mk(header.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    // Close button
    button(context, mk(header.ent()),
           ComponentConfig{}
               .with_label("X")
               .with_size(ComponentSize{pixels(cfg_close_btn_size),
                                        pixels(cfg_close_btn_size)})
               .with_custom_background(close_red)
               .with_border(close_red_dark, cfg_close_btn_border)
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(1.0f)
               .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                 afterhours::Color{0, 0, 0, 30}));

    // ── Tab bar ──
    std::vector<std::string> settings_tabs = {"Audio", "General", "Info"};
    tab_container(context, mk(panel.ent()), settings_tabs, active_tab,
                  ComponentConfig{}
                      .with_size(ComponentSize{percent(0.96f), pixels(36)})
                      .with_self_align(SelfAlign::Center)
                      .with_margin(Margin{.top = pixels(8)}));

    // ── Content area ──
    auto content = vstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(260)})
            .with_no_wrap()
            .with_padding(Padding{
                .top = pixels(10), .left = pixels(50), .right = pixels(50)})
            .with_debug_name("content"));

    if (active_tab == 0) {
      // ════════════════ AUDIO TAB ════════════════
      auto toggle_row =
          hstack(context, mk(content.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(75)})
                     .with_align_items(AlignItems::FlexStart)
                     .with_no_wrap()
                     .with_debug_name("toggle_row"));

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
        bool is_on = *toggles[i].state;
        afterhours::Color track_col = is_on ? btn_green : btn_gray;
        afterhours::Color track_border = is_on ? btn_green_dark : btn_gray_dark;

        auto toggle_col =
            vstack(context, mk(toggle_row.ent(), static_cast<int>(i)),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(110), percent(1.0f)})
                       .with_align_items(AlignItems::Center)
                       .with_no_wrap()
                       .with_debug_name("toggle_" + std::to_string(i)));

        // Track button
        if (button(context, mk(toggle_col.ent(), 0),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(cfg_toggle_track_w),
                                                pixels(cfg_toggle_track_h)})
                       .with_custom_background(track_col)
                       .with_border(track_border, 2.0f)
                       .with_rounded_corners(RoundedCorners().all_round())
                       .with_roundness(0.5f)
                       .with_soft_shadow(1.0f, 2.0f, cfg_btn_shadow_blur,
                                         pill_shadow)
                       .with_debug_name("toggle_track_" + std::to_string(i)))) {
          *toggles[i].state = !*toggles[i].state;
        }

        // Knob
        float knob_travel =
            cfg_toggle_track_w - cfg_toggle_knob_sz - cfg_toggle_pad * 2.0f;
        float knob_x_offset =
            is_on ? knob_travel + cfg_toggle_pad : cfg_toggle_pad;
        div(context, mk(toggle_col.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(cfg_toggle_knob_sz),
                                         pixels(cfg_toggle_knob_sz)})
                .with_custom_background(afterhours::Color{255, 255, 255, 255})
                .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(1.0f)
                .with_skip_tabbing(true)
                .with_translate(pixels(knob_x_offset -
                                       (cfg_toggle_track_w / 2.0f) +
                                       (cfg_toggle_knob_sz / 2.0f)),
                                pixels(-(cfg_toggle_track_h - cfg_toggle_pad)))
                .with_debug_name("toggle_knob_" + std::to_string(i)));

        // Label
        std::string toggle_label =
            toggles[i].label + (is_on ? ": ON" : ": OFF");
        div(context, mk(toggle_col.ent(), 2),
            ComponentConfig{}
                .with_label(toggle_label)
                .with_size(ComponentSize{pixels(100), pixels(22)})
                .with_font("EqProRounded", pixels(cfg_toggle_label_font_size))
                .with_custom_text_color(text_dark)
                .with_alignment(TextAlignment::Center)
                .with_margin(Margin{.top = pixels(-20)}));
      }
    }

    if (active_tab == 1) {
      // ════════════════ GENERAL TAB ════════════════

      // Data & Sync section
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("Data & Sync")
              .with_size(ComponentSize{pixels(120), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_muted));

      auto save_group = hstack(
          context, mk(content.ent()),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(cfg_saveload_width),
                                       pixels(cfg_saveload_height)})
              .with_custom_background(panel_peach)
              .with_border(afterhours::Color{210, 195, 175, 255}, 2.0f)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.25f)
              .with_align_items(AlignItems::Center)
              .with_no_wrap()
              .with_padding(Padding{.left = pixels(14), .right = pixels(14)})
              .with_margin(Margin{.top = pixels(4)})
              .with_debug_name("save_sync_group"));

      auto save_labels =
          vstack(context, mk(save_group.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{expand(), percent(1.0f)})
                     .with_no_wrap()
                     .with_justify_content(JustifyContent::Center));

      div(context, mk(save_labels.ent()),
          ComponentConfig{}
              .with_label("Save/Load")
              .with_size(ComponentSize{pixels(120), pixels(24)})
              .with_custom_text_color(text_dark));

      div(context, mk(save_labels.ent()),
          ComponentConfig{}
              .with_label("Progress")
              .with_size(ComponentSize{pixels(120), pixels(24)})
              .with_custom_text_color(text_dark));

      button(context, mk(save_group.ent()),
             ComponentConfig{}
                 .with_label("Sync")
                 .with_size(ComponentSize{pixels(cfg_sync_btn_size),
                                          pixels(cfg_sync_btn_size)})
                 .with_custom_background(wifi_green)
                 .with_border(wifi_green_dark, 3.0f)
                 .with_font("EqProRounded", pixels(14.0f))
                 .with_custom_text_color(text_white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.4f)
                 .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                   afterhours::Color{0, 0, 0, 25}));

      // Separator
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(1)})
              .with_custom_background(afterhours::Color{65, 55, 45, 40})
              .with_margin(Margin{.top = pixels(10), .bottom = pixels(6)})
              .with_debug_name("sep_preferences"));

      // Preferences section
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("Preferences")
              .with_size(ComponentSize{pixels(120), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_muted));

      // Notifications button
      std::string notif_text =
          notifications_off ? "Notifications: OFF" : "Notifications: ON";
      if (button(context, mk(content.ent()),
                 ComponentConfig{}
                     .with_label(notif_text)
                     .with_size(ComponentSize{pixels(cfg_pill_btn_width),
                                              pixels(cfg_pill_btn_height)})
                     .with_custom_background(btn_blue)
                     .with_border(btn_blue_dark, cfg_pill_border_width)
                     .with_font("EqProRounded", pixels(cfg_pill_font_size))
                     .with_custom_text_color(text_white)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.5f)
                     .with_soft_shadow(2.0f, 3.0f, cfg_btn_shadow_blur,
                                       pill_shadow)
                     .with_margin(Margin{.top = pixels(8)})
                     .with_debug_name("notifications_btn"))) {
        notifications_off = !notifications_off;
      }

      // Language button
      button(context, mk(content.ent()),
             ComponentConfig{}
                 .with_label("Language")
                 .with_size(ComponentSize{pixels(cfg_pill_btn_width),
                                          pixels(cfg_pill_btn_height)})
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, cfg_pill_border_width)
                 .with_font("EqProRounded", pixels(cfg_pill_font_size))
                 .with_custom_text_color(text_white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f)
                 .with_soft_shadow(2.0f, 3.0f, cfg_btn_shadow_blur, pill_shadow)
                 .with_margin(Margin{.top = pixels(8)}));
    }

    if (active_tab == 2) {
      // ════════════════ INFO TAB ════════════════

      // Links section
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("Links")
              .with_size(ComponentSize{pixels(80), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_muted));

      auto links_row =
          hstack(context, mk(content.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f),
                                              pixels(cfg_pill_btn_height + 10)})
                     .with_no_wrap()
                     .with_margin(Margin{.top = pixels(8)})
                     .with_debug_name("links_row"));

      // Credits button
      button(
          context, mk(links_row.ent()),
          ComponentConfig{}
              .with_label("Credits")
              .with_size(ComponentSize{pixels(cfg_pill_btn_width),
                                       pixels(cfg_pill_btn_height)})
              .with_custom_background(btn_blue)
              .with_border(btn_blue_dark, cfg_pill_border_width)
              .with_font("EqProRounded", pixels(cfg_pill_font_size))
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.5f)
              .with_soft_shadow(2.0f, 3.0f, cfg_btn_shadow_blur, pill_shadow));

      // Support button
      button(context, mk(links_row.ent()),
             ComponentConfig{}
                 .with_label("Support")
                 .with_size(ComponentSize{pixels(cfg_pill_btn_width),
                                          pixels(cfg_pill_btn_height)})
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, cfg_pill_border_width)
                 .with_font("EqProRounded", pixels(cfg_pill_font_size))
                 .with_custom_text_color(text_white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f)
                 .with_soft_shadow(2.0f, 3.0f, cfg_btn_shadow_blur, pill_shadow)
                 .with_margin(Margin{.left = pixels(15)}));

      // Separator
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(1)})
              .with_custom_background(afterhours::Color{65, 55, 45, 40})
              .with_margin(Margin{.top = pixels(10), .bottom = pixels(6)})
              .with_debug_name("sep_about"));

      // About section
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("About")
              .with_size(ComponentSize{pixels(80), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_muted));

      auto info_row =
          hstack(context, mk(content.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(80)})
                     .with_no_wrap()
                     .with_align_items(AlignItems::FlexStart)
                     .with_margin(Margin{.top = pixels(8)})
                     .with_debug_name("info_row"));

      // Info lines column
      auto info_col =
          vstack(context, mk(info_row.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{expand(), percent(1.0f)})
                     .with_no_wrap());

      const char *info_lines[] = {"Build: 15555.1.114203",
                                  "Version 1.11.0.12346",
                                  "Player: #281-676-956"};
      for (int il = 0; il < 3; il++) {
        div(context, mk(info_col.ent(), il),
            ComponentConfig{}
                .with_label(info_lines[il])
                .with_size(
                    ComponentSize{pixels(200), pixels(cfg_info_line_height)})
                .with_font("EqProRounded", pixels(cfg_info_font_size))
                .with_custom_text_color(text_muted)
                .with_margin(il > 0 ? Margin{.top = pixels(2)} : Margin{}));
      }

      // Terms and Privacy button
      button(
          context, mk(info_row.ent()),
          ComponentConfig{}
              .with_label("Terms and Privacy")
              .with_size(ComponentSize{pixels(cfg_pill_btn_width),
                                       pixels(cfg_pill_btn_height)})
              .with_custom_background(btn_blue)
              .with_border(btn_blue_dark, cfg_pill_border_width)
              .with_font("EqProRounded", pixels(17.0f))
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.5f)
              .with_soft_shadow(2.0f, 3.0f, cfg_btn_shadow_blur, pill_shadow));
    }

    // ═══════════════════════════════════════════════════════════════
    // FOOTER SEPARATOR + BUTTONS
    // ═══════════════════════════════════════════════════════════════
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(640), pixels(1)})
            .with_custom_background(afterhours::Color{65, 55, 45, 40})
            .with_margin(Margin{.left = pixels(40), .right = pixels(40)})
            .with_debug_name("sep_footer"));

    auto footer = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(42)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.right = pixels(40), .bottom = pixels(8)})
            .with_debug_name("footer"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(btn_green)
               .with_border(btn_green_dark, 3.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_debug_name("btn_ok"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 3.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_cancel"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(btn_blue)
               .with_border(btn_blue_dark, 3.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_apply"));
  }
};

REGISTER_EXAMPLE_SCREEN(angry_birds_settings, "Game Mockups",
                        "Mobile game settings (Angry Birds style)",
                        AngryBirdsSettingsScreen)
