#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct KirbyOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 5;    // Options tab (gear icon)
  size_t active_tab = 0;      // Options category tab
  bool show_delete_confirm = false; // Confirmation barrier for delete

  // Colors matching Kirby Air Ride inspiration - bright Nintendo aesthetic
  afterhours::Color bg_cream{245, 240, 230, 255};
  afterhours::Color panel_white{255, 255, 255, 255};
  afterhours::Color tab_purple{105, 75, 165, 255};
  afterhours::Color tab_purple_dark{85, 55, 135, 255};
  afterhours::Color btn_yellow{255, 205, 45, 255};
  afterhours::Color btn_yellow_dark{225, 175, 25, 255};
  afterhours::Color icon_red{215, 75, 75, 255};
  afterhours::Color icon_blue{75, 135, 200, 255};
  afterhours::Color icon_green{95, 175, 95, 255};
  afterhours::Color icon_purple{145, 95, 185, 255};
  afterhours::Color text_dark{45, 45, 55, 255};
  afterhours::Color text_black{
      0, 0, 0, 255}; // Pure black for max contrast on colored backgrounds
  afterhours::Color text_muted{120, 115, 125, 255};
  afterhours::Color border_gray{195, 190, 185, 255};

  // Tab icons and colors - full labels for clarity
  std::vector<std::tuple<std::string, afterhours::Color>> tabs = {
      {"WiFi", icon_blue},         // Wifi
      {"Control", icon_red},       // Controller
      {"Home", icon_green},        // Home
      {"Mail", icon_purple},       // Mail
      {"Star", btn_yellow},        // Star/coins
      {"Gear", tab_purple},        // Gear (options) - selected
  };

  std::vector<std::tuple<std::string, std::string, afterhours::Color>> options =
      {
          {"Controls", "Controls", icon_green},
          {"Display", "Display", icon_blue},
          {"Accessibility", "Accessibility", icon_purple},
          {"Sound", "Sound", icon_blue},
          {"Network", "Network", icon_green},
      };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", h720(20.0f));
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = panel_white;
    theme.font_muted = text_muted;
    theme.background = bg_cream;
    theme.surface = panel_white;
    theme.primary = tab_purple;
    theme.secondary = icon_blue;
    theme.accent = btn_yellow;
    theme.error = icon_red;
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
            .with_custom_background(bg_cream)
            .with_debug_name("bg"));

    // ========== TOP TAB BAR ==========
    float tab_bar_y = 25.0f;
    float tab_size = 58.0f;
    float tab_spacing = 68.0f;
    float tab_start_x = 200.0f;

    // L bumper - consistent styling with tab bar
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_absolute_position(tab_start_x - 55.0f, tab_bar_y + 11.0f)
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f, afterhours::Color{0, 0, 0, 30}));

    // Tab icons
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      auto &[icon, color] = tabs[i];
      float tx = tab_start_x + (float)i * tab_spacing;

      afterhours::Color tab_bg = is_selected ? tab_purple : color;
      float scale = is_selected ? 1.15f : 1.0f;
      int size = static_cast<int>(tab_size * scale);

      if (button(
              context, mk(entity, 20 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_label(icon)
                  .with_size(ComponentSize{pixels(size), pixels(size)})
                  .with_absolute_position(tx - (scale - 1.0f) * tab_size / 2.0f,
                                  tab_bar_y - (scale - 1.0f) * tab_size / 2.0f)
                  .with_custom_background(tab_bg)
                  .with_border(is_selected ? tab_purple_dark
                                           : afterhours::Color{0, 0, 0, 0},
                               is_selected ? 4.0f : 0.0f)
                  .with_font("Gaegu-Bold", h720(18.0f * scale))
                  .with_custom_text_color(text_black)
                  .with_alignment(TextAlignment::Center)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.25f)
                  .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                    afterhours::Color{0, 0, 0, 40})
                  .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper - consistent styling with tab bar
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_absolute_position(tab_start_x + 6 * tab_spacing + 10.0f,
                            tab_bar_y + 11.0f)
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f, afterhours::Color{0, 0, 0, 30}));

    // ========== "Options" LABEL under selected tab ==========
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Options")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_absolute_position(tab_start_x + 5 * tab_spacing - 15.0f,
                            tab_bar_y + tab_size + 8.0f)
            .with_custom_background(tab_purple)
            .with_font("Gaegu-Bold", h720(19.0f))
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.4f));

    // ========== MAIN CONTENT PANEL ==========
    float panel_x = 130.0f;
    float panel_y = 130.0f;
    float panel_w = static_cast<float>(screen_w) - 180.0f;  // Wider panel
    float panel_h = static_cast<float>(screen_h) - 180.0f;  // Taller panel

    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_white)
            .with_border(border_gray, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 5.0f, 15.0f, afterhours::Color{0, 0, 0, 35})
            .with_debug_name("main_panel"));

    // ========== LEFT SIDE: Tools icons ==========
    float tools_x = panel_x - 65.0f;
    float tools_y = panel_y + 30.0f;

  std::vector<std::tuple<std::string, afterhours::Color>> tools = {
        {"Edit", text_black},  // Pencil - use black for contrast
        {"Erase", text_dark},  // Eraser
    };

    for (size_t i = 0; i < tools.size(); i++) {
      auto &[icon, color] = tools[i];
      div(context, mk(entity, 60 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(icon)
              .with_size(ComponentSize{pixels(52), pixels(52)})
              .with_absolute_position(tools_x, tools_y + (float)i * 60.0f)
              .with_custom_background(panel_white)
              .with_border(border_gray, 2.0f)
              .with_font("Gaegu-Bold", h720(18.0f))
              .with_custom_text_color(color)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_debug_name("tool_" + std::to_string(i)));
    }

    // ========== NAME BUTTON (Yellow pill with avatar) ==========
    float name_x = panel_x + 180.0f;
    float name_y = panel_y + 30.0f;

    button(
        context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(260), pixels(58)})
            .with_absolute_position(name_x, name_y)
            .with_custom_background(btn_yellow)
            .with_border(btn_yellow_dark, 4.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("name_btn"));

    // Avatar circle
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(42), pixels(42)})
            .with_absolute_position(name_x + 12.0f, name_y + 8.0f)
            .with_custom_background(icon_blue)
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(text_black)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    // Name text
    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label("Name")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_absolute_position(name_x + 65.0f, name_y + 10.0f)
            .with_font("Gaegu-Bold", h720(28.0f))
            .with_custom_text_color(text_dark));

    // Delete Data button with confirmation barrier
    // Positioned in bottom-right corner of panel, away from main options
    if (!show_delete_confirm) {
      // Initial state: subtle, understated text-like button
      if (button(context, mk(entity, 110),
          ComponentConfig{}
              .with_label("Data...")
              .with_size(ComponentSize{pixels(80), pixels(24)})
              .with_absolute_position(panel_x + panel_w - 110.0f, panel_y + panel_h - 65.0f)
              .with_custom_background(afterhours::Color{230, 228, 225, 255})
              .with_border(afterhours::Color{210, 208, 205, 255}, 1.0f)
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(text_muted)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.25f)
              .with_debug_name("data_menu"))) {
        show_delete_confirm = true;
      }
    } else {
      // Confirmation state: show Cancel and Delete buttons
      // Cancel button
      if (button(context, mk(entity, 111),
          ComponentConfig{}
              .with_label("Cancel")
              .with_size(ComponentSize{pixels(70), pixels(24)})
              .with_absolute_position(panel_x + panel_w - 190.0f, panel_y + panel_h - 65.0f)
              .with_custom_background(afterhours::Color{210, 208, 205, 255})
              .with_border(border_gray, 1.0f)
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(text_dark)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.25f)
              .with_debug_name("cancel_delete"))) {
        show_delete_confirm = false;
      }
      // Confirm Delete button - muted warm tone instead of bright red
      button(context, mk(entity, 112),
          ComponentConfig{}
              .with_label("Delete")
              .with_size(ComponentSize{pixels(70), pixels(24)})
              .with_absolute_position(panel_x + panel_w - 110.0f, panel_y + panel_h - 65.0f)
              .with_custom_background(afterhours::Color{180, 120, 110, 255})
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(panel_white)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.25f));
    }

    // ========== COMMON LABEL ==========
    div(context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Common")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_absolute_position(panel_x + 25.0f, panel_y + 105.0f)
            .with_custom_text_color(text_dark));

    // ========== OPTIONS TABS ==========
    std::vector<std::string> option_labels;
    for (auto &[icon, label, color] : options) {
      option_labels.push_back(label);
    }

    // MANUAL TAB BUTTONS — workaround for afterhours tab_container rendering
    // tab strip outside parent bounds when using absolute_position.
    // See AFTERHOURS_GAPS.md #1.
    {
      float tabs_x = panel_x + 25.0f;
      float tabs_y = panel_y + 135.0f;
      float total_tabs_w = panel_w - 50.0f;
      float tab_w = total_tabs_w / (float)option_labels.size();
      for (size_t ti = 0; ti < option_labels.size(); ti++) {
        bool is_active = (ti == active_tab);
        afterhours::Color opt_tab_bg = is_active
            ? tab_purple
            : afterhours::Color{230, 226, 220, 255};
        afterhours::Color opt_tab_text = is_active ? panel_white : text_dark;

        if (button(context, mk(entity, 200 + static_cast<int>(ti)),
                   ComponentConfig{}
                       .with_label(option_labels[ti])
                       .with_size(ComponentSize{pxf(tab_w - 2.0f), pixels(36)})
                       .with_absolute_position(tabs_x + (float)ti * tab_w,
                                               tabs_y)
                       .with_custom_background(opt_tab_bg)
                       .with_custom_text_color(opt_tab_text)
                       .with_font("Gaegu-Bold", h720(16.0f))
                       .with_alignment(TextAlignment::Center)
                       .with_border(is_active ? tab_purple_dark : border_gray,
                                    is_active ? 2.0f : 1.0f)
                       .with_rounded_corners(
                           std::bitset<4>(0b1100)) // top corners only
                       .with_roundness(0.2f)
                       .with_debug_name("opt_tab_" + std::to_string(ti)))) {
          active_tab = ti;
        }
      }
    }

    // Tab content area
    float tab_content_y = panel_y + 190.0f;
    auto &[tab_icon, tab_label, tab_color] = options[active_tab];
    std::string tab_desc = tab_label + " options will be displayed here.";

    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label(tab_label)
            .with_size(ComponentSize{pxf(panel_w - 80), pixels(40)})
            .with_absolute_position(panel_x + 40.0f, tab_content_y)
            .with_font("Gaegu-Bold", h720(28.0f))
            .with_custom_text_color(tab_color));

    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_label(tab_desc)
            .with_size(ComponentSize{pxf(panel_w - 80), pixels(30)})
            .with_absolute_position(panel_x + 40.0f, tab_content_y + 45.0f)
            .with_font("Gaegu-Bold", h720(20.0f))
            .with_custom_text_color(text_muted));

    // ========== DESCRIPTION TEXT (more prominent) ==========
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("Enter a new name and customize your controls.")
            .with_size(ComponentSize{pxf(panel_w - 40),
                                     pixels(46)})
            .with_absolute_position(panel_x + 20.0f, panel_y + panel_h - 55.0f)
            .with_custom_background(afterhours::Color{240, 235, 230, 255})
            .with_border(border_gray, 1.0f)
            .with_font("Gaegu-Bold", h720(26.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Colorful Nintendo options menu (Kirby style)",
                        KirbyOptionsScreen)
