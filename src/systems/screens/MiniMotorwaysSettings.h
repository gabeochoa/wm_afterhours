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

  // === CONFIGURABLE UI OPTIONS ===
  // Sensitivity stepper button size (pixels) - minimum 48 for accessibility
  float stepper_button_size = 52.0f;
  // Version display style: "simple" (Version 1.10.2) or "technical" (release-10-patch-2...)
  std::string version_display_style = "simple";

  // Colors matching Mini Motorways inspiration - clean, minimal, pastel
  afterhours::Color bg_cream{245, 242, 235, 255};   // Warm cream background
  afterhours::Color grid_line{215, 210, 200, 255};  // Subtle grid lines
  afterhours::Color text_dark{55, 55, 55, 255};     // Dark gray text
  afterhours::Color text_muted{145, 140, 130, 255}; // Muted gray
  afterhours::Color highlight_yellow{255, 195, 85, 255}; // Selected tab yellow
  afterhours::Color tab_teal{145, 195, 185, 255};        // Teal tab
  afterhours::Color btn_teal{125, 185, 175, 255};        // Tutorial button teal
  // (toggle_circle removed — now uses pill-style toggles)

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
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(22.0f));
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

    auto *res = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_w = res ? res->current_resolution.width : 1280;
    int screen_h = res ? res->current_resolution.height : 720;

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_cream)
            .with_debug_name("bg"));

    // Note: Grid lines removed to reduce visual clutter - clean cream background only

    // ========== BACK ARROW ==========
    button(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("< Back")
            .with_size(ComponentSize{pixels(80), pixels(56)})
            .with_absolute_position(35.0f, 35.0f)
            .with_font("EqProRounded", h720(28.0f))
            .with_custom_text_color(text_dark));

    // ========== CATEGORY TABS ==========
    tab_container(context, mk(entity, 55), categories, active_tab,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(700), pixels(48)})
            .with_absolute_position(180.0f, 120.0f));

    // ========== RIGHT: Settings Content ==========
    float content_x = 500.0f;
    float content_y = 180.0f;
    float row_h = 75.0f;

    // Tab 0: Game settings (toggles + sensitivity)
    if (active_tab == 0) {
    // Pill-style toggles — clean minimal design matching MiniMotorways aesthetic
    constexpr float mm_track_w = 56.0f, mm_track_h = 30.0f;
    constexpr float mm_knob_pad = 4.0f;
    constexpr float mm_knob_sz = mm_track_h - mm_knob_pad * 2.0f;  // 22px
    constexpr float mm_knob_travel = mm_track_w - mm_knob_sz - mm_knob_pad * 2.0f;
    afterhours::Color mm_track_on = highlight_yellow;
    afterhours::Color mm_track_off = afterhours::Color{190, 185, 175, 255};
    afterhours::Color mm_knob_white = afterhours::Color{255, 255, 255, 255};

    for (size_t i = 0; i < toggles.size(); i++) {
      float row_y = content_y + (float)i * row_h;
      bool is_on = *(toggles[i].value);

      // Label with ON/OFF suffix
      std::string label_with_state = toggles[i].label + (is_on ? "  ON" : "  OFF");
      div(context, mk(entity, 100 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label(label_with_state)
              .with_size(ComponentSize{pixels(340), pixels(40)})
              .with_absolute_position(content_x, row_y)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(text_dark));

      float toggle_x = content_x + 340.0f;
      float toggle_y_pos = row_y + 4.0f;
      afterhours::Color track_col = is_on ? mm_track_on : mm_track_off;

      // Track visual (div — immune to hover color override)
      div(context, mk(entity, 101 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(mm_track_w), pixels(mm_track_h)})
              .with_absolute_position(toggle_x, toggle_y_pos)
              .with_custom_background(track_col)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.5f)
              .with_skip_tabbing(true)
              .with_debug_name("toggle_track_" + std::to_string(i)));

      // Transparent click target (overlays the track — no background)
      if (button(context, mk(entity, 102 + static_cast<int>(i) * 4),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(mm_track_w), pixels(mm_track_h)})
                     .with_absolute_position(toggle_x, toggle_y_pos)
                     .with_color_usage(Theme::Usage::None)
                     .with_debug_name("toggle_btn_" + std::to_string(i)))) {
        *(toggles[i].value) = !is_on;
      }

      // White knob (sibling div — screen coordinates)
      float knob_x = toggle_x + mm_knob_pad + (is_on ? mm_knob_travel : 0.0f);
      float knob_y = toggle_y_pos + mm_knob_pad;
      div(context, mk(entity, 103 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(mm_knob_sz), pixels(mm_knob_sz)})
              .with_absolute_position(knob_x, knob_y)
              .with_custom_background(mm_knob_white)
              .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(1.0f)
              .with_skip_tabbing(true)
              .with_debug_name("toggle_knob_" + std::to_string(i)));
    }

    // Sensitivity slider row
    float sens_y = content_y + 4.0f * row_h;

    div(context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Controller Cursor Sensitivity")
            .with_size(ComponentSize{pixels(340), pixels(40)})
            .with_absolute_position(content_x, sens_y)
            .with_custom_text_color(text_dark));

    // Left arrow - use configurable stepper size for accessibility
    int stepper_size = static_cast<int>(stepper_button_size);
    if (button(context, mk(entity, 121),
               ComponentConfig{}
                   .with_label("<")
                   .with_size(ComponentSize{pixels(stepper_size), pixels(stepper_size)})
                   .with_absolute_position(content_x + 340.0f, sens_y - 2.0f)
                   .with_font("EqProRounded", h720(28.0f))
                   .with_custom_text_color(text_dark)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("sens_left"))) {
      if (sensitivity_level > 0)
        sensitivity_level--;
    }

    // Value
    std::string sens_text = "Default";
    if (sensitivity_level == 0)
      sens_text = "Low";
    if (sensitivity_level == 2)
      sens_text = "High";

    div(context, mk(entity, 122),
        ComponentConfig{}
            .with_label(sens_text)
            .with_size(ComponentSize{pixels(110), pixels(40)})
            .with_absolute_position(content_x + 390.0f, sens_y)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Right arrow - use configurable stepper size for accessibility
    if (button(context, mk(entity, 123),
               ComponentConfig{}
                   .with_label(">")
                   .with_size(ComponentSize{pixels(stepper_size), pixels(stepper_size)})
                   .with_absolute_position(content_x + 505.0f, sens_y - 2.0f)
                   .with_font("EqProRounded", h720(28.0f))
                   .with_custom_text_color(text_dark)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("sens_right"))) {
      if (sensitivity_level < 2)
        sensitivity_level++;
    }

    // Separator line
    div(context, mk(entity, 130),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(450), pixels(2)})
            .with_absolute_position(content_x, sens_y + 50.0f)
            .with_custom_background(grid_line)
            .with_debug_name("separator"));
    } // end active_tab == 0 (Game)

    // Other tabs: placeholder content
    if (active_tab >= 1 && active_tab < categories.size()) {
      std::string tab_name = categories[active_tab];
      div(context, mk(entity, 140),
          ComponentConfig{}
              .with_label(tab_name + " settings")
              .with_size(ComponentSize{pixels(400), pixels(40)})
              .with_absolute_position(content_x, content_y)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(text_dark));
      div(context, mk(entity, 141),
          ComponentConfig{}
              .with_label("Options will be displayed here.")
              .with_size(ComponentSize{pixels(400), pixels(40)})
              .with_absolute_position(content_x, content_y + 40.0f)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(text_muted));
    }

    // ========== VERSION INFO ==========
    // Use configurable version display style
    std::string version_label;
    if (version_display_style == "technical") {
      version_label = "Mini Motorways release-10-patch-2 (202207010917)";
    } else {
      // Simple user-friendly format (default)
      version_label = "Mini Motorways Version 1.10.2";
    }
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label(version_label)
            .with_size(ComponentSize{pixels(480), pixels(28)})
            .with_absolute_position(35.0f, (float)screen_h - 45.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(text_muted));

    // ========== TUTORIAL BUTTON ==========
    button(
        context, mk(entity, 210),
        ComponentConfig{}
            .with_label("Tutorial  ->")
            .with_size(ComponentSize{pixels(170), pixels(56)})
            .with_absolute_position((float)screen_w - 205.0f, (float)screen_h - 70.0f)
            .with_custom_background(btn_teal)
            .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
            .with_alignment(TextAlignment::Center));

    // ========== FOOTER: OK / Cancel / Apply ==========
    float footer_y = (float)screen_h - 60.0f;
    float footer_btn_x = (float)screen_w / 2.0f - 150.0f;

    button(context, mk(entity, 220),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x, footer_y)
               .with_custom_background(btn_teal)
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_ok"));

    button(context, mk(entity, 221),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x + 90.0f, footer_y)
               .with_custom_background(afterhours::Color{215, 210, 200, 255})
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_cancel"));

    button(context, mk(entity, 222),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_absolute_position(footer_btn_x + 180.0f, footer_y)
               .with_custom_background(afterhours::Color{215, 210, 200, 255})
               .with_custom_text_color(text_dark)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_apply"));
  }
};

REGISTER_EXAMPLE_SCREEN(mini_motorways_settings, "Game Mockups",
                        "Minimalist settings (Mini Motorways style)",
                        MiniMotorwaysSettingsScreen)
