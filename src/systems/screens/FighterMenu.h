#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FighterMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 3;     // Options tab selected
  size_t selected_option = 0;  // System Options selected
  int currency = 25000;

  // Colors matching Cross Tag Battle inspiration
  afterhours::Color bg_dark{25, 22, 20, 255};
  afterhours::Color bg_brown{45, 38, 32, 255};
  afterhours::Color tab_bg{15, 15, 15, 255};
  afterhours::Color tab_selected{85, 165, 55, 255};  // Bright green highlight
  afterhours::Color tab_text{200, 195, 190, 255};
  afterhours::Color menu_item_bg{35, 32, 28, 255};
  afterhours::Color menu_highlight{120, 195, 75, 255};
  afterhours::Color text_white{245, 240, 235, 255};
  afterhours::Color text_gray{140, 135, 125, 255};
  afterhours::Color holograph_teal{65, 185, 175, 255};
  afterhours::Color holograph_white{235, 250, 248, 255};
  afterhours::Color gold_text{220, 185, 85, 255};

  std::vector<std::string> tabs = {"Offline", "Online", "Customize", "Options"};

  std::vector<std::pair<std::string, std::string>> menu_options = {
      {"[v]", "System Options"},
      {"[@]", "Game Options"},
      {"[/]", "Display Options"},
      {"[~]", "Sound & Language"},
      {"[*]", "Network Options"},
      {"[O]", "Button Settings"},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_gray;
    theme.background = bg_dark;
    theme.surface = menu_item_bg;
    theme.primary = tab_selected;
    theme.secondary = holograph_teal;
    theme.accent = menu_highlight;
    theme.error = afterhours::Color{200, 60, 60, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_brown)
            .with_debug_name("bg"));

    // ========== TITLE: MainMenu (stylized) ==========
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("MainMenu")
            .with_size(ComponentSize{pixels(350), pixels(70)})
            .with_absolute_position()
            .with_translate(25.0f, 15.0f)
            .with_font("Gaegu-Bold", 52.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("title"));

    // ========== CURRENCY DISPLAY ==========
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label(std::to_string(currency) + " P$")
            .with_size(ComponentSize{pixels(140), pixels(35)})
            .with_absolute_position()
            .with_translate((float)screen_w - 165.0f, 22.0f)
            .with_font("EqProRounded", 24.0f)
            .with_custom_text_color(gold_text)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("currency"));

    // ========== TAB BAR ==========
    float tab_y = 95.0f;
    float tab_w = 180.0f;
    float tab_start_x = 110.0f;

    // L bumper indicator
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_absolute_position()
            .with_translate(70.0f, tab_y + 5.0f)
            .with_custom_background(tab_bg)
            .with_border(text_gray, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("l_bumper"));

    // Tab buttons
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      afterhours::Color tab_bg_color = is_selected ? tab_selected : tab_bg;
      afterhours::Color tab_text_color = is_selected ? bg_dark : tab_text;

      if (button(context, mk(entity, 30 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(tabs[i])
                     .with_size(ComponentSize{pixels(static_cast<int>(tab_w)),
                                              pixels(40)})
                     .with_absolute_position()
                     .with_translate(tab_start_x + (float)i * tab_w, tab_y)
                     .with_custom_background(tab_bg_color)
                     .with_border(text_gray, 1.0f)
                     .with_font("EqProRounded", 20.0f)
                     .with_custom_text_color(tab_text_color)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper indicator
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_absolute_position()
            .with_translate(tab_start_x + 4 * tab_w + 10.0f, tab_y + 5.0f)
            .with_custom_background(tab_bg)
            .with_border(text_gray, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r_bumper"));

    // Decorative line under tabs
    div(context, mk(entity, 45),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(tab_w * 4 + 80)),
                                     pixels(3)})
            .with_absolute_position()
            .with_translate(70.0f, tab_y + 42.0f)
            .with_custom_background(tab_selected)
            .with_debug_name("tab_underline"));

    // ========== LEFT SIDEBAR MENU ==========
    float menu_x = 45.0f;
    float menu_y = 175.0f;
    float menu_item_h = 58.0f;
    float icon_w = 55.0f;

    for (size_t i = 0; i < menu_options.size(); i++) {
      bool is_selected = (i == selected_option);
      float item_y = menu_y + (float)i * menu_item_h;
      auto &[icon, label] = menu_options[i];

      // Icon background
      div(context, mk(entity, 100 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(icon)
              .with_size(ComponentSize{pixels(static_cast<int>(icon_w)),
                                       pixels(static_cast<int>(menu_item_h - 6))})
              .with_absolute_position()
              .with_translate(menu_x, item_y)
              .with_custom_background(menu_item_bg)
              .with_border(text_gray, 1.0f)
              .with_font("EqProRounded", 20.0f)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("icon_" + std::to_string(i)));

      // Menu item background
      afterhours::Color item_bg = is_selected ? menu_highlight : menu_item_bg;
      afterhours::Color item_text = is_selected ? bg_dark : tab_text;

      if (button(context, mk(entity, 101 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{pixels(280),
                                              pixels(static_cast<int>(menu_item_h - 6))})
                     .with_absolute_position()
                     .with_translate(menu_x + icon_w, item_y)
                     .with_custom_background(item_bg)
                     .with_border(text_gray, 1.0f)
                     .with_font("EqProRounded", 20.0f)
                     .with_custom_text_color(item_text)
                     .with_alignment(TextAlignment::Left)
                     .with_debug_name("menu_" + std::to_string(i)))) {
        selected_option = i;
      }
    }

    // "Offline Mode" indicator
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("Offline Mode")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_absolute_position()
            .with_translate(430.0f, 165.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("offline_mode"));

    // ========== CENTER CHARACTER AREA (placeholder) ==========
    // Just show a simple character placeholder
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("^_^")
            .with_size(ComponentSize{pixels(120), pixels(180)})
            .with_absolute_position()
            .with_translate(450.0f, 260.0f)
            .with_font("EqProRounded", 48.0f)
            .with_custom_text_color(text_gray)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("character"));

    // ========== RIGHT PANEL: Info Card (holographic style) ==========
    float card_x = 620.0f;
    float card_y = 200.0f;
    float card_w = 350.0f;
    float card_h = 320.0f;

    // Card background (diagonal cut effect simulated)
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(card_w)),
                                     pixels(static_cast<int>(card_h))})
            .with_absolute_position()
            .with_translate(card_x, card_y)
            .with_custom_background(holograph_white)
            .with_debug_name("card_bg"));

    // Teal accent edge
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(8), pixels(static_cast<int>(card_h))})
            .with_absolute_position()
            .with_translate(card_x + card_w - 8.0f, card_y)
            .with_custom_background(holograph_teal)
            .with_debug_name("card_edge"));

    // Large gear icon
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(140), pixels(140)})
            .with_absolute_position()
            .with_translate(card_x + 40.0f, card_y + 30.0f)
            .with_custom_background(afterhours::Color{55, 175, 165, 180})
            .with_font("EqProRounded", 85.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("gear_icon"));

    // Card title
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_label("System Options")
            .with_size(ComponentSize{pixels(280), pixels(45)})
            .with_absolute_position()
            .with_translate(card_x + 30.0f, card_y + 190.0f)
            .with_font("Gaegu-Bold", 32.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_title"));

    // Card description
    div(context, mk(entity, 321),
        ComponentConfig{}
            .with_label("System Options")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(card_x + 30.0f, card_y + 235.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_subtitle"));

    div(context, mk(entity, 322),
        ComponentConfig{}
            .with_label("Change various settings in the")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(card_x + 30.0f, card_y + 260.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc1"));

    div(context, mk(entity, 323),
        ComponentConfig{}
            .with_label("game, such as vibration, save,")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(card_x + 30.0f, card_y + 280.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc2"));

    div(context, mk(entity, 324),
        ComponentConfig{}
            .with_label("and autosave functions.")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(card_x + 30.0f, card_y + 300.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc3"));

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = (float)screen_h - 55.0f;

    // D-pad
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("+")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_absolute_position()
            .with_translate(280.0f, prompt_y)
            .with_custom_background(tab_bg)
            .with_border(text_gray, 1.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("dpad"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label(": Select")
            .with_size(ComponentSize{pixels(70), pixels(30)})
            .with_absolute_position()
            .with_translate(315.0f, prompt_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("select_label"));

    // A button (confirm)
    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_label("o")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(420.0f, prompt_y + 1.0f)
            .with_custom_background(afterhours::Color{180, 60, 60, 255})
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("a_btn"));

    div(context, mk(entity, 411),
        ComponentConfig{}
            .with_label(": Confirm")
            .with_size(ComponentSize{pixels(80), pixels(30)})
            .with_absolute_position()
            .with_translate(453.0f, prompt_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("confirm_label"));

    // B button (return)
    div(context, mk(entity, 420),
        ComponentConfig{}
            .with_label("o")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate(560.0f, prompt_y + 1.0f)
            .with_custom_background(afterhours::Color{180, 160, 60, 255})
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("b_btn"));

    div(context, mk(entity, 421),
        ComponentConfig{}
            .with_label(": Return")
            .with_size(ComponentSize{pixels(70), pixels(30)})
            .with_absolute_position()
            .with_translate(593.0f, prompt_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("return_label"));

    // L/R for change entry
    div(context, mk(entity, 430),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(690.0f, prompt_y + 2.0f)
            .with_custom_background(tab_bg)
            .with_border(text_gray, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("l_btn"));

    div(context, mk(entity, 431),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position()
            .with_translate(720.0f, prompt_y + 2.0f)
            .with_custom_background(tab_bg)
            .with_border(text_gray, 1.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r_btn"));

    div(context, mk(entity, 432),
        ComponentConfig{}
            .with_label(": Change Entry")
            .with_size(ComponentSize{pixels(120), pixels(30)})
            .with_absolute_position()
            .with_translate(750.0f, prompt_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("change_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(fighter_menu, "Game Mockups",
                        "Anime fighting game menu (Cross Tag style)",
                        FighterMenuScreen)

