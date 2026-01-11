#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FlightOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_category = 0;
  size_t selected_option = 0;

  // Colors matching Ace Combat inspiration - dark cyber/military aesthetic
  afterhours::Color bg_dark{8, 12, 22, 255};
  afterhours::Color text_cyan{85, 175, 225, 255};
  afterhours::Color text_bright{165, 215, 245, 255};
  afterhours::Color text_muted{75, 95, 120, 255};
  afterhours::Color highlight_line{35, 105, 160, 255};  // Darkened for WCAG AA contrast
  afterhours::Color grid_color{20, 35, 55, 180};

  std::vector<std::string> categories = {
      "FLIGHT SYSTEM", "CONTROLS", "KEYBOARD", "MOUSE",
      "FLIGHT STICK",  "DISPLAY",  "GRAPHICS", "SOUND",
      "LANGUAGE",
  };

  std::vector<std::string> suboptions = {
      "Control Type",
      "High-G Turn Settings",
      "Reduced Collision Damage",
      "Vibration",
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    // Use pure white/black for auto_text_color to achieve WCAG AA contrast
    theme.font = afterhours::Color{255, 255, 255, 255};  // Pure white for dark bgs
    theme.darkfont = afterhours::Color{10, 15, 25, 255}; // Near-black for light bgs
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = afterhours::Color{12, 18, 30, 255};
    theme.primary = highlight_line;
    theme.secondary = text_muted;
    theme.accent = text_cyan;
    theme.error = afterhours::Color{180, 50, 50, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== GRID LINES (subtle background effect) ==========
    // Vertical grid lines
    for (int i = 0; i < 20; i++) {
      float x = (float)i * 80.0f;
      div(context, mk(entity, 10 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1), pixels(screen_h)})
              .with_absolute_position()
              .with_translate(x, 0.0f)
              .with_custom_background(grid_color)
              .with_debug_name("grid_v_" + std::to_string(i)));
    }
    // Horizontal grid lines
    for (int i = 0; i < 15; i++) {
      float y = (float)i * 60.0f;
      div(context, mk(entity, 40 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(screen_w), pixels(1)})
              .with_absolute_position()
              .with_translate(0.0f, y)
              .with_custom_background(grid_color)
              .with_debug_name("grid_h_" + std::to_string(i)));
    }

    // ========== DECORATIVE HUD LINES ==========
    // Angled lines from top-left corner
    float line_origin_x = 145.0f;
    float line_origin_y = 135.0f;

    // Top angled line
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(120), pixels(2)})
            .with_absolute_position()
            .with_translate(25.0f, 130.0f)
            .with_custom_background(highlight_line)
            .with_debug_name("line_top"));

    // Connecting vertical line
    div(context, mk(entity, 61),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels(400)})
            .with_absolute_position()
            .with_translate(line_origin_x, line_origin_y)
            .with_custom_background(highlight_line)
            .with_debug_name("line_vert"));

    // Glow dot at intersection
    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(8), pixels(8)})
            .with_absolute_position()
            .with_translate(line_origin_x - 3.0f, line_origin_y - 3.0f)
            .with_custom_background(afterhours::Color{255, 200, 80, 255})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("glow_dot"));

    // Horizontal tick marks along vertical line
    for (int i = 0; i < 9; i++) {
      float tick_y = 160.0f + (float)i * 32.0f;
      div(context, mk(entity, 70 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(12), pixels(2)})
              .with_absolute_position()
              .with_translate(line_origin_x - 12.0f, tick_y)
              .with_custom_background(
                  i == (int)selected_category ? text_cyan : text_muted)
              .with_debug_name("tick_" + std::to_string(i)));
    }

    // ========== TITLE: OPTIONS ==========
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(200), pixels(50)})
            .with_absolute_position()
            .with_translate(135.0f, 60.0f)
            .with_font("EqProRounded", 36.0f)
            .with_custom_text_color(text_cyan)
            .with_debug_name("title"));

    // ========== MENU CATEGORIES ==========
    float menu_x = 170.0f;
    float menu_y = 150.0f;

    for (size_t i = 0; i < categories.size(); i++) {
      bool selected = (i == selected_category);
      afterhours::Color item_color = selected ? text_bright : text_cyan;

      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(categories[i])
                     .with_size(ComponentSize{pixels(180), pixels(28)})
                     .with_absolute_position()
                     .with_translate(menu_x, menu_y + (float)i * 32.0f)
                     .with_font("EqProRounded", 19.0f)
                     .with_custom_text_color(item_color)
                     .with_debug_name("cat_" + std::to_string(i)))) {
        selected_category = i;
      }

      // Selection indicator bar
      if (selected) {
        div(context, mk(entity, 150 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(4), pixels(22)})
                .with_absolute_position()
                .with_translate(menu_x - 18.0f, menu_y + (float)i * 32.0f + 3.0f)
                .with_custom_background(text_cyan)
                .with_debug_name("select_bar_" + std::to_string(i)));
      }
    }

    // ========== SUB-OPTIONS (right side) ==========
    float sub_x = 380.0f;
    float sub_y = 250.0f;

    for (size_t i = 0; i < suboptions.size(); i++) {
      bool is_selected = (i == selected_option);
      afterhours::Color opt_color = is_selected ? text_bright : text_muted;

      if (button(context, mk(entity, 200 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(suboptions[i])
                     .with_size(ComponentSize{pixels(220), pixels(24)})
                     .with_absolute_position()
                     .with_translate(sub_x, sub_y + (float)i * 26.0f)
                     .with_font("EqProRounded", 19.0f)
                     .with_custom_text_color(opt_color)
                     .with_debug_name("opt_" + std::to_string(i)))) {
        selected_option = i;
      }
    }

    // ========== HELP TEXT ==========
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("Select the in-flight system.")
            .with_size(ComponentSize{pixels(350), pixels(30)})
            .with_absolute_position()
            .with_translate(170.0f, (float)screen_h - 180.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_bright)
            .with_debug_name("help"));

    // ========== BOTTOM BUTTON PROMPTS ==========
    float btn_y = (float)screen_h - 60.0f;

    // Enter key box
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("Enter")
            .with_size(ComponentSize{pixels(50), pixels(28)})
            .with_absolute_position()
            .with_translate(160.0f, btn_y)
            .with_custom_background(afterhours::Color{35, 50, 70, 255})
            .with_border(text_muted, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("enter_key"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("OK")
            .with_size(ComponentSize{pixels(30), pixels(28)})
            .with_absolute_position()
            .with_translate(218.0f, btn_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_bright)
            .with_debug_name("ok_label"));

    // Esc key box
    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(40), pixels(28)})
            .with_absolute_position()
            .with_translate(265.0f, btn_y)
            .with_custom_background(afterhours::Color{35, 50, 70, 255})
            .with_border(text_muted, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("esc_key"));

    div(context, mk(entity, 403),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(50), pixels(28)})
            .with_absolute_position()
            .with_translate(313.0f, btn_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_bright)
            .with_debug_name("back_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Flight sim options menu (Ace Combat style)",
                        FlightOptionsScreen)

