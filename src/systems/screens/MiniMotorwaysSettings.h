#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MiniMotorwaysSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 0;
  bool night_mode = false;
  bool vibration = false;
  bool hold_to_draw = true;
  bool colorblind_mode = false;
  int sensitivity_level = 1; // 0=Low, 1=Default, 2=High

  float stepper_button_size = 52.0f;
  std::string version_display_style = "simple";

  // Colors matching Mini Motorways inspiration - clean, minimal, pastel
  afterhours::Color bg_cream{245, 242, 235, 255};
  afterhours::Color grid_line{215, 210, 200, 255};
  afterhours::Color text_dark{55, 55, 55, 255};
  afterhours::Color text_muted{145, 140, 130, 255};
  afterhours::Color highlight_yellow{255, 195, 85, 255};
  afterhours::Color tab_teal{145, 195, 185, 255};
  afterhours::Color btn_teal{125, 185, 175, 255};

  std::vector<std::string> categories = {
      "Game", "Video", "Audio", "Language", "Controls", "Cross-Save", "Credits",
  };

  struct SettingToggle {
    std::string label;
    bool *value;
  };

  std::vector<SettingToggle> toggles;

  MiniMotorwaysSettingsScreen() {
    toggles = {
        {"Night Mode", &night_mode},
        {"Vibration", &vibration},
        {"Hold to Draw/Delete", &hold_to_draw},
        {"Colorblind Mode", &colorblind_mode},
    };
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(22.0f));
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = bg_cream;
    theme.font_muted = text_muted;
    theme.background = bg_cream;
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = highlight_yellow;
    theme.secondary = tab_teal;
    theme.accent = btn_teal;
    theme.error = afterhours::Color{180, 80, 80, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    constexpr float mm_track_w = 56.0f, mm_track_h = 30.0f;
    constexpr float mm_knob_pad = 4.0f;
    constexpr float mm_knob_sz = mm_track_h - mm_knob_pad * 2.0f;
    constexpr float mm_knob_travel =
        mm_track_w - mm_knob_sz - mm_knob_pad * 2.0f;
    afterhours::Color mm_track_on = highlight_yellow;
    afterhours::Color mm_track_off = afterhours::Color{190, 185, 175, 255};
    afterhours::Color mm_knob_white = afterhours::Color{255, 255, 255, 255};

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg_cream)
                   .with_padding(Padding{.top = pixels(20),
                                         .left = pixels(35),
                                         .bottom = pixels(5),
                                         .right = pixels(35)})
                   .with_no_wrap()
                   .with_debug_name("mm_root"));

    // ── Back button ──
    button(context, mk(root.ent()),
           ComponentConfig{}
               .with_label("< Back")
               .with_size(ComponentSize{pixels(100), pixels(56)})
               .with_font("EqProRounded", pixels(28.0f))
               .with_custom_text_color(text_dark));

    // ── Tab bar ──
    tab_container(
        context, mk(root.ent()), categories, active_tab,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(660), pixels(48)})
            .with_margin(Margin{.top = pixels(10), .left = pixels(145)}));

    // ── Content area ──
    auto content =
        vstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(745), pixels(350)})
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(8), .left = pixels(465)})
                   .with_debug_name("content"));

    if (active_tab == 0) {
      // Game tab: toggles + sensitivity
      for (size_t i = 0; i < toggles.size(); i++) {
        bool is_on = *(toggles[i].value);
        afterhours::Color track_col = is_on ? mm_track_on : mm_track_off;

        auto toggle_row = hstack(
            context, mk(content.ent(), static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(40)})
                .with_align_items(AlignItems::Center)
                .with_no_wrap()
                .with_margin(Margin{.top = i > 0 ? pixels(25) : Size{}}));

        std::string label_with_state =
            toggles[i].label + (is_on ? "  ON" : "  OFF");
        div(context, mk(toggle_row.ent(), 0),
            ComponentConfig{}
                .with_label(label_with_state)
                .with_size(ComponentSize{expand(), pixels(40)})
                .with_font("EqProRounded", pixels(20.0f))
                .with_custom_text_color(text_dark)
                .with_text_overflow(TextOverflow::Ellipsis));

        // Toggle track (clickable button)
        if (button(context, mk(toggle_row.ent(), 1),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(mm_track_w),
                                                pixels(mm_track_h)})
                       .with_custom_background(track_col)
                       .with_rounded_corners(RoundedCorners().all_round())
                       .with_roundness(0.5f)
                       .with_debug_name("toggle_track_" + std::to_string(i)))) {
          *(toggles[i].value) = !is_on;
        }

        // Knob (translated over the track)
        float knob_x_offset =
            is_on ? mm_knob_travel + mm_knob_pad : mm_knob_pad;
        div(context, mk(toggle_row.ent(), 2),
            ComponentConfig{}
                .with_size(
                    ComponentSize{pixels(mm_knob_sz), pixels(mm_knob_sz)})
                .with_custom_background(mm_knob_white)
                .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(1.0f)
                .with_skip_tabbing(true)
                .with_translate(pixels(knob_x_offset - mm_track_w -
                                       mm_knob_sz / 2.0f + mm_knob_pad),
                                pixels(0))
                .with_debug_name("toggle_knob_" + std::to_string(i)));
      }

      // Sensitivity row
      auto sens_row =
          hstack(context, mk(content.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(52)})
                     .with_align_items(AlignItems::Center)
                     .with_no_wrap()
                     .with_margin(Margin{.top = pixels(25)})
                     .with_debug_name("sens_row"));

      div(context, mk(sens_row.ent()),
          ComponentConfig{}
              .with_label("Cursor Sensitivity")
              .with_size(ComponentSize{expand(), pixels(40)})
              .with_custom_text_color(text_dark)
              .with_text_overflow(TextOverflow::Ellipsis));

      if (button(context, mk(sens_row.ent()),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{pixels(52), pixels(52)})
                     .with_font("EqProRounded", pixels(28.0f))
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("sens_left"))) {
        if (sensitivity_level > 0)
          sensitivity_level--;
      }

      std::string sens_text = "Default";
      if (sensitivity_level == 0)
        sens_text = "Low";
      if (sensitivity_level == 2)
        sens_text = "High";

      div(context, mk(sens_row.ent()),
          ComponentConfig{}
              .with_label(sens_text)
              .with_size(ComponentSize{pixels(110), pixels(40)})
              .with_font("EqProRounded", 22.0f)
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center));

      if (button(context, mk(sens_row.ent()),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{pixels(52), pixels(52)})
                     .with_font("EqProRounded", pixels(28.0f))
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("sens_right"))) {
        if (sensitivity_level < 2)
          sensitivity_level++;
      }

      // Separator
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(450), pixels(2)})
              .with_custom_background(grid_line)
              .with_margin(Margin{.top = pixels(15)})
              .with_debug_name("separator"));
    }

    // Other tabs: placeholder
    if (active_tab >= 1 && active_tab < categories.size()) {
      std::string tab_name = categories[active_tab];
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label(tab_name + " settings")
              .with_size(ComponentSize{pixels(400), pixels(40)})
              .with_font("EqProRounded", pixels(24.0f))
              .with_custom_text_color(text_dark));
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("Options will be displayed here.")
              .with_size(ComponentSize{pixels(400), pixels(40)})
              .with_font("EqProRounded", pixels(24.0f))
              .with_custom_text_color(text_muted));
    }

    // ── Bottom area: version + tutorial + footer ──
    auto bottom =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(56)})
                   .with_align_items(AlignItems::FlexEnd)
                   .with_no_wrap()
                   .with_debug_name("bottom"));

    // Version
    std::string version_label;
    if (version_display_style == "technical") {
      version_label = "Mini Motorways release-10-patch-2 (202207010917)";
    } else {
      version_label = "Mini Motorways Version 1.10.2";
    }
    div(context, mk(bottom.ent()),
        ComponentConfig{}
            .with_label(version_label)
            .with_size(ComponentSize{pixels(480), pixels(28)})
            .with_font("EqProRounded", pixels(18.0f))
            .with_custom_text_color(text_muted));

    // Spacer
    div(context, mk(bottom.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    // Tutorial button
    button(context, mk(bottom.ent()),
           ComponentConfig{}
               .with_label("Tutorial  ->")
               .with_size(ComponentSize{pixels(170), pixels(56)})
               .with_custom_background(btn_teal)
               .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
               .with_alignment(TextAlignment::Center));

    // Footer buttons row
    auto footer =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(36)})
                   .with_justify_content(JustifyContent::Center)
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(5)})
                   .with_debug_name("footer"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(btn_teal)
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_ok"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(afterhours::Color{215, 210, 200, 255})
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_cancel"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(afterhours::Color{215, 210, 200, 255})
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_apply"));
  }
};

REGISTER_EXAMPLE_SCREEN(mini_motorways_settings, "Game Mockups",
                        "Minimalist settings (Mini Motorways style)",
                        MiniMotorwaysSettingsScreen)
