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
  size_t selected_tab = 5;     // Options tab (gear icon)
  size_t selected_option = 0;  // Controller selected

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
  afterhours::Color text_black{0, 0, 0, 255};  // Pure black for max contrast on colored backgrounds
  afterhours::Color text_muted{120, 115, 125, 255};
  afterhours::Color border_gray{195, 190, 185, 255};

  // Tab icons and colors
  std::vector<std::tuple<std::string, afterhours::Color>> tabs = {
      {"((*))", icon_blue},    // Wifi
      {"[O]", icon_red},       // Controller
      {"[^]", icon_green},     // Home
      {"[@]", icon_purple},    // Mail
      {"[#]", btn_yellow},     // Star/coins
      {"[*]", tab_purple},     // Gear (options) - selected
  };

  std::vector<std::tuple<std::string, std::string, afterhours::Color>> options = {
      {"[O]", "Controller", icon_green},
      {"[=]", "Display", icon_blue},
      {"[+]", "Accessibility", icon_purple},
      {"[~]", "Sound", icon_blue},
      {"[(*))", "Online", icon_green},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
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

    // L bumper
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(32), pixels(32)})
            .with_absolute_position()
            .with_translate(tab_start_x - 50.0f, tab_bar_y + 13.0f)
            .with_custom_background(border_gray)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("l_bumper"));

    // Tab icons
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      auto &[icon, color] = tabs[i];
      float tx = tab_start_x + (float)i * tab_spacing;

      afterhours::Color tab_bg = is_selected ? tab_purple : color;
      float scale = is_selected ? 1.15f : 1.0f;
      int size = static_cast<int>(tab_size * scale);

      if (button(context, mk(entity, 20 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(icon)
                     .with_size(ComponentSize{pixels(size), pixels(size)})
                     .with_absolute_position()
                     .with_translate(tx - (scale - 1.0f) * tab_size / 2.0f,
                                     tab_bar_y - (scale - 1.0f) * tab_size / 2.0f)
                     .with_custom_background(tab_bg)
                     .with_border(is_selected ? tab_purple_dark : afterhours::Color{0, 0, 0, 0}, 
                                  is_selected ? 4.0f : 0.0f)
                     .with_font("Gaegu-Bold", 22.0f * scale)
                     .with_custom_text_color(text_black)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.25f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(32), pixels(32)})
            .with_absolute_position()
            .with_translate(tab_start_x + 6 * tab_spacing + 10.0f, tab_bar_y + 13.0f)
            .with_custom_background(border_gray)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("r_bumper"));

    // ========== "Options" LABEL under selected tab ==========
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Options")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_absolute_position()
            .with_translate(tab_start_x + 5 * tab_spacing - 15.0f, tab_bar_y + tab_size + 8.0f)
            .with_custom_background(tab_purple)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.4f)
            .with_debug_name("options_label"));

    // ========== MAIN CONTENT PANEL ==========
    float panel_x = 130.0f;
    float panel_y = 130.0f;
    float panel_w = 620.0f;
    float panel_h = 370.0f;

    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_white)
            .with_border(border_gray, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 5.0f, 15.0f, afterhours::Color{0, 0, 0, 35})
            .with_debug_name("main_panel"));

    // ========== LEFT SIDE: Tools icons ==========
    float tools_x = panel_x - 65.0f;
    float tools_y = panel_y + 30.0f;

    std::vector<std::tuple<std::string, afterhours::Color>> tools = {
        {"[/]", text_black},   // Pencil - use black for contrast
        {"[-]", text_dark},    // Eraser
    };

    for (size_t i = 0; i < tools.size(); i++) {
      auto &[icon, color] = tools[i];
      div(context, mk(entity, 60 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(icon)
              .with_size(ComponentSize{pixels(45), pixels(45)})
              .with_absolute_position()
              .with_translate(tools_x, tools_y + (float)i * 55.0f)
              .with_custom_background(panel_white)
              .with_border(border_gray, 2.0f)
              .with_font("Gaegu-Bold", 20.0f)
              .with_custom_text_color(color)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.2f)
              .with_debug_name("tool_" + std::to_string(i)));
    }

    // ========== NAME BUTTON (Yellow pill with avatar) ==========
    float name_x = panel_x + 180.0f;
    float name_y = panel_y + 30.0f;

    button(context, mk(entity, 100),
           ComponentConfig{}
               .with_size(ComponentSize{pixels(260), pixels(58)})
               .with_absolute_position()
               .with_translate(name_x, name_y)
               .with_custom_background(btn_yellow)
               .with_border(btn_yellow_dark, 4.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
               .with_debug_name("name_btn"));

    // Avatar circle
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(42), pixels(42)})
            .with_absolute_position()
            .with_translate(name_x + 12.0f, name_y + 8.0f)
            .with_custom_background(icon_blue)
            .with_font("Gaegu-Bold", 24.0f)
            .with_custom_text_color(text_black)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("avatar"));

    // Name text
    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label("Name")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_absolute_position()
            .with_translate(name_x + 65.0f, name_y + 10.0f)
            .with_font("Gaegu-Bold", 28.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("name_text"));

    // Delete Data button (right side)
    div(context, mk(entity, 110),
        ComponentConfig{}
            .with_label("[x] Delete Data")
            .with_size(ComponentSize{pixels(110), pixels(22)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - 140.0f, name_y + 20.0f)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("delete_data"));

    // ========== COMMON LABEL ==========
    div(context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Common")
            .with_size(ComponentSize{pixels(80), pixels(22)})
            .with_absolute_position()
            .with_translate(panel_x + 25.0f, panel_y + 105.0f)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("common_label"));

    // ========== OPTIONS GRID (2 columns) ==========
    float grid_x = panel_x + 35.0f;
    float grid_y = panel_y + 135.0f;
    float icon_size = 75.0f;
    float col_spacing = 160.0f;
    float row_spacing = 100.0f;

    for (size_t i = 0; i < options.size(); i++) {
      auto &[icon, label, color] = options[i];
      int row = static_cast<int>(i) / 3;
      int col = static_cast<int>(i) % 3;
      float ox = grid_x + (float)col * col_spacing;
      float oy = grid_y + (float)row * row_spacing;

      bool is_selected = (i == selected_option);
      afterhours::Color opt_bg = is_selected ? color : afterhours::Color{235, 230, 225, 255};
      afterhours::Color opt_text = is_selected ? text_black : color;

      // Option icon button
      if (button(context, mk(entity, 200 + static_cast<int>(i) * 2),
                 ComponentConfig{}
                     .with_label(icon)
                     .with_size(ComponentSize{pixels(static_cast<int>(icon_size)),
                                              pixels(static_cast<int>(icon_size))})
                     .with_absolute_position()
                     .with_translate(ox, oy)
                     .with_custom_background(opt_bg)
                     .with_border(is_selected ? afterhours::Color{0, 0, 0, 0} : border_gray, 2.0f)
                     .with_font("Gaegu-Bold", 32.0f)
                     .with_custom_text_color(opt_text)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.25f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 30})
                     .with_debug_name("opt_icon_" + std::to_string(i)))) {
        selected_option = i;
      }

      // Option label
      div(context, mk(entity, 201 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(static_cast<int>(icon_size + 40)),
                                       pixels(22)})
              .with_absolute_position()
              .with_translate(ox - 20.0f, oy + icon_size + 8.0f)
              .with_font("Gaegu-Bold", 19.0f)
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("opt_label_" + std::to_string(i)));
    }

    // ========== DESCRIPTION TEXT ==========
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("Enter a new name and customize your controls.")
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 60)),
                                     pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + panel_h - 45.0f)
            .with_font("Gaegu-Bold", 19.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("description"));

    // ========== LEFT DECORATIVE ELEMENTS (checkerboard pattern hint) ==========
    float check_x = 30.0f;
    float check_y = panel_y + panel_h - 80.0f;
    afterhours::Color check1{195, 225, 195, 255};  // Light green
    afterhours::Color check2{175, 205, 235, 255};  // Light blue

    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 4; col++) {
        afterhours::Color c = ((row + col) % 2 == 0) ? check1 : check2;
        div(context, mk(entity, 400 + row * 4 + col),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(18), pixels(18)})
                .with_absolute_position()
                .with_translate(check_x + (float)col * 22.0f,
                               check_y + (float)row * 22.0f)
                .with_custom_background(c)
                .with_debug_name("check_" + std::to_string(row) + "_" + std::to_string(col)));
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Colorful Nintendo options menu (Kirby style)",
                        KirbyOptionsScreen)

