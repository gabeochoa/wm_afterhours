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
  size_t selected_category = 0;
  bool night_mode = false;
  bool vibration = false;
  bool hold_to_draw = true;
  bool colorblind_mode = false;
  int sensitivity_level = 1; // 0=Low, 1=Default, 2=High

  // === CONFIGURABLE UI OPTIONS ===
  // Toggle indicator style: "checkmark" (standard) or "vx" (V/X style)
  std::string toggle_indicator_style = "checkmark";
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
  afterhours::Color toggle_circle{60, 60, 60, 255};      // Toggle circle

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
            .with_label("<-")
            .with_size(ComponentSize{pixels(56), pixels(56)})
            .with_absolute_position(35.0f, 35.0f)
            .with_font("EqProRounded", h720(32.0f))
            .with_custom_text_color(text_dark)
            .with_debug_name("back_arrow"));

    // ========== LEFT: Category Tabs ==========
    // Vertical metro-style line
    float line_x = 180.0f;
    div(context, mk(entity, 55),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(4), pixels(380)})
            .with_absolute_position(line_x, 150.0f)
            .with_custom_background(grid_line)
            .with_debug_name("metro_line"));

    // Metro style horizontal connectors
    float tab_start_y = 160.0f;
    float tab_h = 52.0f;

    for (size_t i = 0; i < categories.size(); i++) {
      float tab_y = tab_start_y + (float)i * tab_h;
      bool selected = (i == selected_category);

      // Horizontal connector to tab
      div(context, mk(entity, 60 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(25), pixels(4)})
              .with_absolute_position(line_x - 2.0f, tab_y + 22.0f)
              .with_custom_background(grid_line)
              .with_debug_name("connector_" + std::to_string(i)));

      // Tab button
      afterhours::Color tab_bg = selected ? highlight_yellow : tab_teal;

      if (button(context, mk(entity, 70 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(categories[i])
                     .with_size(ComponentSize{pixels(160), pixels(48)})
                     .with_absolute_position(205.0f, tab_y)
                     .with_custom_background(tab_bg)
                     .with_font("EqProRounded", h720(22.0f))
                     .with_custom_text_color(text_dark)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_category = i;
      }
    }

    // ========== RIGHT: Settings Content ==========
    float content_x = 500.0f;
    float content_y = 180.0f;
    float row_h = 75.0f;

    // Toggles with X/checkmark style
    for (size_t i = 0; i < toggles.size(); i++) {
      float row_y = content_y + (float)i * row_h;
      bool is_on = *(toggles[i].value);

      // Label
      div(context, mk(entity, 100 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(toggles[i].label)
              .with_size(ComponentSize{pixels(300), pixels(40)})
              .with_absolute_position(content_x, row_y)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(text_dark)
              .with_debug_name("label_" + std::to_string(i)));

      // Toggle circle - use configurable indicator style
      std::string toggle_icon;
      afterhours::Color toggle_bg;
      if (toggle_indicator_style == "vx") {
        // Original V/X style
        toggle_icon = is_on ? "V" : "X";
        toggle_bg = afterhours::Color{0, 0, 0, 0};  // Transparent
      } else {
        // Standard filled/empty circle style (more universally recognized)
        // Empty circle for off, filled inner circle for on
        toggle_icon = "";  // No text - use visual fill instead
        // For "on" state, use a smaller filled circle inside the ring
        toggle_bg = afterhours::Color{0, 0, 0, 0};  // Handle fill separately
      }
      afterhours::Color icon_color = is_on ? text_dark : text_muted;

      if (button(context, mk(entity, 101 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(toggle_icon)
                     .with_size(ComponentSize{pixels(52), pixels(52)})
                     .with_absolute_position(content_x + 340.0f, row_y - 5.0f)
                     .with_custom_background(toggle_bg)
                     .with_border(toggle_circle, 3.0f)
                     .with_font("EqProRounded", h720(22.0f))
                     .with_custom_text_color(icon_color)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(1.0f)
                     .with_debug_name("toggle_" + std::to_string(i)))) {
        *(toggles[i].value) = !is_on;
      }

      // For checkmark style: add filled inner circle when on
      if (toggle_indicator_style != "vx" && is_on) {
        div(context, mk(entity, 102 + static_cast<int>(i) * 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(28), pixels(28)})
                .with_absolute_position(content_x + 340.0f + 12.0f, row_y - 5.0f + 12.0f)
                .with_custom_background(toggle_circle)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(1.0f)
                .with_debug_name("toggle_fill_" + std::to_string(i)));
      }
    }

    // Sensitivity slider row
    float sens_y = content_y + 4.0f * row_h;

    div(context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Controller Cursor Sensitivity")
            .with_size(ComponentSize{pixels(340), pixels(40)})
            .with_absolute_position(content_x, sens_y)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(text_dark)
            .with_debug_name("sens_label"));

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
            .with_alignment(TextAlignment::Center)
            .with_debug_name("sens_value"));

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
            .with_custom_text_color(text_muted)
            .with_debug_name("version"));

    // ========== TUTORIAL BUTTON ==========
    button(
        context, mk(entity, 210),
        ComponentConfig{}
            .with_label("Tutorial  ->")
            .with_size(ComponentSize{pixels(170), pixels(56)})
            .with_absolute_position((float)screen_w - 205.0f, (float)screen_h - 70.0f)
            .with_custom_background(btn_teal)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("tutorial"));
  }
};

REGISTER_EXAMPLE_SCREEN(mini_motorways_settings, "Game Mockups",
                        "Minimalist settings (Mini Motorways style)",
                        MiniMotorwaysSettingsScreen)
