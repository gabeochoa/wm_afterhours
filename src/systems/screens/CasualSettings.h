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
  afterhours::Color text_muted{90, 75, 60, 255};
  afterhours::Color white{255, 255, 255, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", pixels(18.0f));
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
    context.scaling_mode = ScalingMode::Adaptive;

    // Toggle constants
    constexpr float cs_track_w = 60.0f, cs_track_h = 32.0f;
    constexpr float cs_knob_pad = 4.0f;
    constexpr float cs_knob_sz = cs_track_h - cs_knob_pad * 2.0f; // 24px
    constexpr float cs_knob_travel =
        cs_track_w - cs_knob_sz - cs_knob_pad * 2.0f;

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
                   .with_debug_name("casual_root"));

    // ═══════════════════════════════════════════════════════════════
    // TITLE
    // ═══════════════════════════════════════════════════════════════
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(200), pixels(45)})
            .with_font("Gaegu-Bold", pixels(38.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = pixels(8)}));

    // ═══════════════════════════════════════════════════════════════
    // PANEL (orange border via border prop, cream interior)
    // ═══════════════════════════════════════════════════════════════
    auto panel = vstack(context, mk(root.ent()),
                        ComponentConfig{}
                            .with_720p_size(680, 452)
                            .with_custom_background(panel_cream)
                            .with_border(panel_orange, 8.0f)
                            .with_rounded_corners(RoundedCorners())
                            .with_roundness(0.12f)
                            .with_padding(Padding{.top = pixels(8),
                                                  .left = pixels(40),
                                                  .bottom = pixels(12),
                                                  .right = pixels(40)})
                            .with_no_wrap()
                            .with_debug_name("panel"));

    // ── Close button row (right-aligned) ──
    auto close_row =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.96f), pixels(42)})
                   .with_justify_content(JustifyContent::FlexEnd)
                   .with_align_items(AlignItems::FlexStart)
                   .with_self_align(SelfAlign::Center)
                   .with_no_wrap()
                   .with_debug_name("close_row"));

    if (button(context, mk(close_row.ent()),
               ComponentConfig{}
                   .with_label("X")
                   .with_size(ComponentSize{pixels(40), pixels(40)})
                   .with_custom_background(close_red)
                   .with_border(afterhours::Color{190, 55, 65, 255}, 3.0f)
                   .with_font("Gaegu-Bold", pixels(24.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(1.0f)
                   .with_debug_name("close_btn"))) {
      // Close action
    }

    // ── Top sections: Audio (left) + Data (right) ──
    auto top_sections =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(118)})
                   .with_align_items(AlignItems::FlexStart)
                   .with_justify_content(JustifyContent::SpaceBetween)
                   .with_no_wrap()
                   .with_debug_name("top_sections"));

    // ── Audio section ──
    auto audio_sec =
        vstack(context, mk(top_sections.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(340), percent(1.0f)})
                   .with_no_wrap()
                   .with_debug_name("audio_sec"));

    div(context, mk(audio_sec.ent()),
        ComponentConfig{}
            .with_label("Audio")
            .with_size(ComponentSize{pixels(80), pixels(22)})
            .with_font("Gaegu-Bold", pixels(20.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("section_header_audio"));

    // Toggle row
    auto toggle_row =
        hstack(context, mk(audio_sec.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(86)})
                   .with_align_items(AlignItems::FlexStart)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(4)})
                   .with_debug_name("toggle_row"));

    struct ToggleInfo {
      const char *label;
      bool *state;
    };
    ToggleInfo toggles[] = {
        {"Music", &music_on}, {"Sound", &sound_on}, {"Vibrate", &vibrate_on}};

    for (int ti = 0; ti < 3; ti++) {
      bool is_on = *toggles[ti].state;
      afterhours::Color cs_track_off{185, 175, 165, 255};
      afterhours::Color track_col = is_on ? btn_green : cs_track_off;
      afterhours::Color track_border_col =
          is_on ? btn_green_dark : afterhours::Color{165, 155, 145, 255};

      auto toggle_col = vstack(
          context, mk(toggle_row.ent(), ti),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(112), percent(1.0f)})
              .with_align_items(AlignItems::Center)
              .with_no_wrap()
              .with_debug_name(std::string("toggle_") + toggles[ti].label));

      // Track button (clickable, styled as the track)
      if (button(context, mk(toggle_col.ent(), 0),
                 ComponentConfig{}
                     .with_size(
                         ComponentSize{pixels(cs_track_w), pixels(cs_track_h)})
                     .with_custom_background(track_col)
                     .with_border(track_border_col, 2.0f)
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.5f)
                     .with_soft_shadow(1.0f, 2.0f, 4.0f,
                                       afterhours::Color{0, 0, 0, 35})
                     .with_debug_name(std::string("toggle_track_") +
                                      toggles[ti].label))) {
        *toggles[ti].state = !*toggles[ti].state;
      }

      // Knob (overlaps track using negative margin to shift up)
      float knob_x_offset = is_on ? cs_knob_travel + cs_knob_pad : cs_knob_pad;
      div(context, mk(toggle_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(cs_knob_sz), pixels(cs_knob_sz)})
              .with_custom_background(white)
              .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(1.0f)
              .with_skip_tabbing(true)
              .with_translate(pixels(knob_x_offset - (cs_track_w / 2.0f) +
                                     (cs_knob_sz / 2.0f)),
                              pixels(-(cs_track_h - cs_knob_pad)))
              .with_debug_name(std::string("toggle_knob_") +
                               toggles[ti].label));

      // Label below
      std::string display =
          std::string(toggles[ti].label) + (is_on ? ": ON" : ": OFF");
      div(context, mk(toggle_col.ent(), 2),
          ComponentConfig{}
              .with_label(display)
              .with_size(ComponentSize{pixels(108), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              // The -20 pulled the caption up out of its own column, which is
              // what the layout overflow warnings were about.
              .with_margin(Margin{.top = pixels(4)}));
    }

    // ── Data section (right side of top) ──
    auto data_sec =
        vstack(context, mk(top_sections.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(260), percent(1.0f)})
                   .with_no_wrap()
                   .with_debug_name("data_sec"));

    div(context, mk(data_sec.ent()),
        ComponentConfig{}
            .with_label("Data")
            .with_size(ComponentSize{pixels(80), pixels(22)})
            .with_font("Gaegu-Bold", pixels(20.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("section_header_data"));

    auto data_btns =
        hstack(context, mk(data_sec.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(55)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(4)})
                   .with_debug_name("data_btns"));

    button(context, mk(data_btns.ent()),
           ComponentConfig{}
               .with_label("Save/Load Progress")
               .with_size(ComponentSize{pixels(200), pixels(50)})
               .with_custom_background(white)
               .with_border(afterhours::Color{200, 195, 185, 255}, 3.0f)
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_margin(Margin{.right = pixels(10)}));

    div(context, mk(data_btns.ent()),
        ComponentConfig{}
            .with_label("Sync")
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_custom_background(btn_green)
            .with_custom_text_color(text_dark)
            .with_font("Gaegu-Bold", pixels(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    // ═══════════════════════════════════════════════════════════════
    // SEPARATOR + MENU SECTION
    // ═══════════════════════════════════════════════════════════════
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(afterhours::Color{55, 45, 40, 40})
            .with_margin(Margin{.top = pixels(6), .bottom = pixels(4)})
            .with_debug_name("sep_toggles"));

    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("Menu")
            .with_size(ComponentSize{pixels(80), pixels(20)})
            .with_font("Gaegu-Bold", pixels(20.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.bottom = pixels(6)})
            .with_debug_name("section_header_menu"));

    // ── Menu buttons: two columns ──
    auto menu_row =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(165)})
                   .with_justify_content(JustifyContent::SpaceBetween)
                   .with_align_items(AlignItems::FlexStart)
                   .with_no_wrap()
                   .with_debug_name("menu_row"));

    auto make_menu_btn = [&](auto parent, int id, const char *label) {
      button(context, mk(parent.ent(), id),
             ComponentConfig{}
                 .with_label(label)
                 .with_720p_size(280, 45)
                 .with_custom_background(btn_blue)
                 .with_border(btn_blue_dark, 4.0f)
                 .with_font("Gaegu-Bold", pixels(22.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.5f)
                 .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                   afterhours::Color{0, 0, 0, 30})
                 .with_margin(Margin{.bottom = pixels(8)}));
    };

    // Left column
    auto left_col =
        vstack(context, mk(menu_row.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(285), percent(1.0f)})
                   .with_no_wrap()
                   .with_debug_name("menu_left"));

    make_menu_btn(left_col, 0, "Notifications: OFF");
    make_menu_btn(left_col, 1, "Language");

    // Right column
    auto right_col =
        vstack(context, mk(menu_row.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(285), percent(1.0f)})
                   .with_no_wrap()
                   .with_debug_name("menu_right"));

    make_menu_btn(right_col, 0, "Credits");
    make_menu_btn(right_col, 1, "Support");
    make_menu_btn(right_col, 2, "Terms & Privacy");

    // ═══════════════════════════════════════════════════════════════
    // FOOTER SEPARATOR + BUTTONS
    // ═══════════════════════════════════════════════════════════════
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(afterhours::Color{55, 45, 40, 40})
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("sep_footer"));

    auto footer =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(42)})
                   .with_align_items(AlignItems::Center)
                   .with_justify_content(JustifyContent::SpaceBetween)
                   .with_no_wrap()
                   .with_debug_name("footer"));

    // Left group: About + Version
    auto footer_left =
        hstack(context, mk(footer.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(246), pixels(42)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap());

    // About button
    if (button(context, mk(footer_left.ent()),
               ComponentConfig{}
                   .with_label("About")
                   .with_size(ComponentSize{pixels(100), pixels(38)})
                   .with_custom_background(btn_blue)
                   .with_border(btn_blue_dark, 3.0f)
                   .with_font("Gaegu-Bold", pixels(20.0f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f)
                   .with_soft_shadow(1.0f, 2.0f, 5.0f,
                                     afterhours::Color{0, 0, 0, 30})
                   .with_debug_name("about_btn"))) {
      show_about = !show_about;
    }

    // Version
    div(context, mk(footer_left.ent()),
        ComponentConfig{}
            .with_label("Version 1.11.0")
            .with_size(ComponentSize{pixels(130), pixels(22)})
            .with_font("Gaegu-Bold", pixels(17.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.left = pixels(8)}));

    // Right group: OK/Cancel/Apply
    auto footer_right =
        hstack(context, mk(footer.ent()),
               ComponentConfig{}
                   // 3 x 65 plus two 12px gaps. At 215 the buttons sat 2-4px
                   // apart while everything else on the screen uses 14-20.
                   .with_size(ComponentSize{pixels(219), pixels(42)})
                   .with_align_items(AlignItems::Center)
                   .with_gap(pixels(12.0f))
                   .with_no_wrap());

    auto make_footer_btn = [&](int id, const char *label, afterhours::Color bg,
                               afterhours::Color border) {
      button(context, mk(footer_right.ent(), id),
             ComponentConfig{}
                 .with_label(label)
                 .with_size(ComponentSize{pixels(65), pixels(36)})
                 .with_custom_background(bg)
                 .with_border(border, 3.0f)
                 .with_font("Gaegu-Bold", pixels(20.0f))
                 .with_custom_text_color(white)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.4f)
                 .with_margin(Margin{.left = pixels(5)}));
    };

    make_footer_btn(3, "OK", btn_green, btn_green_dark);
    make_footer_btn(4, "Cancel", btn_blue, btn_blue_dark);
    make_footer_btn(5, "Apply", btn_blue, btn_blue_dark);

    // ═══════════════════════════════════════════════════════════════
    // ABOUT OVERLAY (modal-style, still uses absolute positioning)
    // ═══════════════════════════════════════════════════════════════
    if (show_about) {
      int screen_w = Settings::get().get_screen_width();
      int screen_h = Settings::get().get_screen_height();
      float about_w = 380.0f;
      float about_h = 180.0f;
      float about_x = (float)screen_w / 2.0f - about_w / 2.0f;
      float about_y = (float)screen_h / 2.0f - about_h / 2.0f;

      // Orange border
      div(context, mk(entity),
          ComponentConfig{}
              .with_720p_size(about_w + 12, about_h + 12)
              .with_absolute_position(about_x - 6.0f, about_y - 6.0f)
              .with_custom_background(panel_orange)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.12f)
              .with_debug_name("about_border"));

      // Cream inner
      auto about_panel = vstack(context, mk(entity),
                                ComponentConfig{}
                                    .with_720p_size(about_w, about_h)
                                    .with_absolute_position(about_x, about_y)
                                    .with_custom_background(panel_cream)
                                    .with_rounded_corners(RoundedCorners())
                                    .with_roundness(0.1f)
                                    .with_padding(Padding{.top = pixels(12),
                                                          .left = pixels(20),
                                                          .bottom = pixels(12),
                                                          .right = pixels(20)})
                                    .with_no_wrap()
                                    .with_debug_name("about_inner"));

      div(context, mk(about_panel.ent()),
          ComponentConfig{}
              .with_label("About")
              .with_size(ComponentSize{percent(1.0f), pixels(30)})
              .with_font("Gaegu-Bold", pixels(24.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center));

      div(context, mk(about_panel.ent()),
          ComponentConfig{}
              .with_label("Build: 15555-1-114203-20-10200-01")
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_font("Gaegu-Bold", pixels(16.0f))
              .with_custom_text_color(text_muted)
              .with_margin(Margin{.top = pixels(6)}));

      div(context, mk(about_panel.ent()),
          ComponentConfig{}
              .with_label("Version: 1.11.0.12346")
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_font("Gaegu-Bold", pixels(16.0f))
              .with_custom_text_color(text_muted)
              .with_margin(Margin{.top = pixels(2)}));

      div(context, mk(about_panel.ent()),
          ComponentConfig{}
              .with_label("Player ID: 281676956389")
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_font("Gaegu-Bold", pixels(16.0f))
              .with_custom_text_color(text_muted)
              .with_margin(Margin{.top = pixels(2)}));

      auto about_footer =
          hstack(context, mk(about_panel.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(38)})
                     .with_justify_content(JustifyContent::Center)
                     .with_align_items(AlignItems::Center)
                     .with_margin(Margin{.top = pixels(6)}));

      if (button(context, mk(about_footer.ent()),
                 ComponentConfig{}
                     .with_label("Close")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
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
