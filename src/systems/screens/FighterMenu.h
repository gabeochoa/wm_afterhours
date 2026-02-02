#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/window_manager.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FighterMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 3;    // Options tab selected
  size_t selected_option = 0; // System Options selected
  int currency = 25000;

  // Colors matching Cross Tag Battle inspiration - darker, more industrial
  afterhours::Color bg_dark{15, 12, 10, 255};   // Very dark for contrast
  afterhours::Color bg_brown{85, 72, 58, 255};  // Warmer brown background
  afterhours::Color header_black{8, 8, 8, 255}; // Pure black header bar
  afterhours::Color tab_bg_unselected{248, 245, 240, 255}; // Cream white tabs
  afterhours::Color tab_selected{85, 195, 55, 255};        // Bright lime green
  afterhours::Color tab_text_unselected{35, 30, 25, 255};  // Dark text on light
  afterhours::Color tab_border{40, 35, 30, 255};           // Dark border
  afterhours::Color menu_item_bg{10, 10, 10, 255};         // Pure black bars
  afterhours::Color menu_highlight{95, 210, 55, 255};      // Bright lime green
  afterhours::Color text_white{250, 248, 245, 255};
  afterhours::Color text_gray{155, 150, 145, 255};
  afterhours::Color holograph_teal{85, 215, 200, 255}; // Brighter cyan-teal
  afterhours::Color holograph_white{252, 255, 253, 255};
  afterhours::Color gold_text{255, 210, 55, 255}; // Bright gold for P$
  afterhours::Color menu_text_unselected{175, 170, 165,
                                         255}; // Gray text on dark
  afterhours::Color title_yellow{255, 235, 130,
                                 255}; // Yellow for MainMenu title

  std::vector<std::string> tabs = {"Offline", "Online", "Customize", "Options"};

  std::vector<std::pair<std::string, std::string>> menu_options = {
      {"F1", "System Options"},  {"F2", "Game Options"},
      {"F3", "Display Options"}, {"F4", "Sound & Language"},
      {"F5", "Network Options"}, {"F6", "Button Settings"},
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

    auto *res = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_w = res ? res->current_resolution.width : 1280;
    int screen_h = res ? res->current_resolution.height : 720;

    // ========== FULL BACKGROUND (black header + transition) ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(header_black)
            .with_debug_name("bg_full"));

    // ========== LOWER BROWN AREA (below header bar) ==========
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h - 145)})
            .with_absolute_position()
            .with_translate(0.0f, 145.0f)
            .with_custom_background(bg_brown)
            .with_debug_name("bg_lower"));

    // ========== TITLE: MainMenu (stylized yellow on black) ==========
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("MainMenu")
            .with_size(ComponentSize{pixels(380), pixels(75)})
            .with_absolute_position()
            .with_translate(20.0f, 8.0f)
            .with_font("Gaegu-Bold", 58.0f)
            .with_custom_text_color(title_yellow)
            .with_debug_name("title"));

    // ========== CURRENCY DISPLAY (top right on black header) ==========
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Points: " + std::to_string(currency))
            .with_size(ComponentSize{pixels(200), pixels(40)})
            .with_absolute_position()
            .with_translate((float)screen_w - 220.0f, 22.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(gold_text)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("currency"));

    // ========== TAB BAR ==========
    float tab_y = 95.0f;
    float tab_w = 180.0f;
    float tab_start_x = 110.0f;

    // L bumper indicator - matching inspiration
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(35), pixels(35)})
            .with_absolute_position()
            .with_translate(68.0f, tab_y + 2.0f)
            .with_custom_background(tab_bg_unselected)
            .with_border(tab_border, 2.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(tab_text_unselected)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("l_bumper"));

    // Tab buttons - white bg for unselected, green for selected (like
    // inspiration)
    // Minimum 44px height for touch targets
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      afterhours::Color bg_color =
          is_selected ? tab_selected : tab_bg_unselected;
      // Selected tab has gold/orange text, unselected has dark text
      afterhours::Color text_color =
          is_selected ? gold_text : tab_text_unselected;

      if (button(context, mk(entity, 30 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(tabs[i])
                     .with_size(ComponentSize{pixels(static_cast<int>(tab_w)),
                                              pixels(44)})
                     .with_absolute_position()
                     .with_translate(tab_start_x + (float)i * tab_w, tab_y)
                     .with_custom_background(bg_color)
                     .with_border(tab_border, 2.0f)
                     .with_font("EqProRounded", 20.0f)
                     .with_custom_text_color(text_color)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper indicator
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(35), pixels(35)})
            .with_absolute_position()
            .with_translate(tab_start_x + 4 * tab_w + 10.0f, tab_y + 2.0f)
            .with_custom_background(tab_bg_unselected)
            .with_border(tab_border, 2.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(tab_text_unselected)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
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
    // Black horizontal bars with icons on left, matching inspiration
    // Connected visually to content area via consistent spacing
    float menu_x = 45.0f;
    float menu_y = 180.0f;
    float menu_item_h = 58.0f; // Minimum 44px for touch targets
    float icon_w = 56.0f;      // Minimum 44px for touch targets
    float menu_item_w = 360.0f; // Expanded width for better visual connection

    for (size_t i = 0; i < menu_options.size(); i++) {
      bool is_selected = (i == selected_option);
      float item_y = menu_y + (float)i * menu_item_h;
      auto &[icon, label] = menu_options[i];

      // Icon box on left - dark with border (minimum 44px touch target)
      afterhours::Color icon_bg = is_selected ? menu_highlight : menu_item_bg;
      afterhours::Color icon_color = is_selected ? bg_dark : text_white;

      div(context, mk(entity, 100 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(icon)
              .with_size(
                  ComponentSize{pixels(static_cast<int>(icon_w)),
                                pixels(static_cast<int>(menu_item_h - 4))})
              .with_absolute_position()
              .with_translate(menu_x, item_y)
              .with_custom_background(icon_bg)
              .with_font("EqProRounded", 24.0f)
              .with_custom_text_color(icon_color)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("icon_" + std::to_string(i)));

      // Menu item bar - black normally, bright green when selected (with hover state)
      // Using border for hover state indication
      afterhours::Color item_bg = is_selected ? menu_highlight : menu_item_bg;
      afterhours::Color item_text =
          is_selected ? bg_dark : menu_text_unselected;
      afterhours::Color item_border = is_selected ? holograph_teal : afterhours::Color{40, 40, 40, 255};

      if (button(context, mk(entity, 101 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{
                         pixels(static_cast<int>(menu_item_w - icon_w)),
                         pixels(static_cast<int>(menu_item_h - 4))})
                     .with_absolute_position()
                     .with_translate(menu_x + icon_w, item_y)
                     .with_custom_background(item_bg)
                     .with_border(item_border, is_selected ? 2.0f : 0.0f)
                     .with_font("EqProRounded", 22.0f)
                     .with_custom_text_color(item_text)
                     .with_alignment(TextAlignment::Left)
                     .with_debug_name("menu_" + std::to_string(i)))) {
        selected_option = i;
      }
    }

    // "Offline Mode" indicator - position in center area
    float offline_x = menu_x + menu_item_w + 40.0f;
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("Offline Mode")
            .with_size(ComponentSize{pixels(200), pixels(32)})
            .with_absolute_position()
            .with_translate(offline_x, 165.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("offline_mode"));

    // Offline mode status indicator
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Not connected to network")
            .with_size(ComponentSize{pixels(200), pixels(24)})
            .with_absolute_position()
            .with_translate(offline_x, 192.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_gray)
            .with_debug_name("offline_status"));

    // ========== CENTER CHARACTER AREA (fills dead space) ==========
    // Position between menu (ends ~405px) and card panel
    float center_area_start = menu_x + menu_item_w + 30.0f;
    float center_area_end = (float)screen_w - 380.0f - 60.0f; // before card
    float center_area_width = center_area_end - center_area_start;
    float character_x = center_area_start + center_area_width / 2.0f - 80.0f;

    // Add background panel to fill dead space and connect visually
    div(context, mk(entity, 205),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(center_area_width)),
                                     pixels(360)})
            .with_absolute_position()
            .with_translate(center_area_start, 200.0f)
            .with_custom_background(afterhours::Color{40, 35, 30, 120})
            .with_border(afterhours::Color{60, 55, 50, 200}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.1f)
            .with_debug_name("center_bg"));

    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("^_^")
            .with_size(ComponentSize{pixels(160), pixels(220)})
            .with_absolute_position()
            .with_translate(character_x, 260.0f)
            .with_font("EqProRounded", 56.0f)
            .with_custom_text_color(text_gray)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("character"));

    // ========== RIGHT PANEL: Info Card (holographic style) ==========
    float card_w = 380.0f;
    float card_h = 340.0f;
    float card_x = (float)screen_w - card_w - 50.0f; // 50px from right edge
    float card_y = 195.0f;

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
            .with_size(
                ComponentSize{pixels(8), pixels(static_cast<int>(card_h))})
            .with_absolute_position()
            .with_translate(card_x + card_w - 8.0f, card_y)
            .with_custom_background(holograph_teal)
            .with_debug_name("card_edge"));

    // Large gear icon
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(150), pixels(150)})
            .with_absolute_position()
            .with_translate(card_x + 45.0f, card_y + 30.0f)
            .with_custom_background(afterhours::Color{55, 175, 165, 180})
            .with_font("EqProRounded", 90.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("gear_icon"));

    // Card title
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_label("System Options")
            .with_size(ComponentSize{pixels(300), pixels(48)})
            .with_absolute_position()
            .with_translate(card_x + 35.0f, card_y + 195.0f)
            .with_font("Gaegu-Bold", 34.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_title"));

    // Card description - minimum 18.0f font
    div(context, mk(entity, 321),
        ComponentConfig{}
            .with_label("System Options")
            .with_size(ComponentSize{pixels(320), pixels(26)})
            .with_absolute_position()
            .with_translate(card_x + 35.0f, card_y + 243.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_subtitle"));

    div(context, mk(entity, 322),
        ComponentConfig{}
            .with_label("Change various settings in the")
            .with_size(ComponentSize{pixels(320), pixels(28)})
            .with_absolute_position()
            .with_translate(card_x + 35.0f, card_y + 268.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc1"));

    div(context, mk(entity, 323),
        ComponentConfig{}
            .with_label("game, such as vibration, save,")
            .with_size(ComponentSize{pixels(320), pixels(28)})
            .with_absolute_position()
            .with_translate(card_x + 35.0f, card_y + 293.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc2"));

    div(context, mk(entity, 324),
        ComponentConfig{}
            .with_label("and autosave functions.")
            .with_size(ComponentSize{pixels(320), pixels(28)})
            .with_absolute_position()
            .with_translate(card_x + 35.0f, card_y + 318.0f)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(bg_dark)
            .with_debug_name("card_desc3"));

    // ========== BOTTOM BUTTON PROMPTS ==========
    // Increased keyboard hint spacing for better readability
    float prompt_y = (float)screen_h - 60.0f;
    float hint_spacing = 150.0f; // Increased from ~120px spacing

    // D-pad (minimum 44px touch target)
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("+")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_absolute_position()
            .with_translate(200.0f, prompt_y)
            .with_custom_background(menu_item_bg)
            .with_border(text_gray, 2.0f)
            .with_font("EqProRounded", 24.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("dpad"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label(": Select")
            .with_size(ComponentSize{pixels(90), pixels(36)})
            .with_absolute_position()
            .with_translate(242.0f, prompt_y)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("select_label"));

    // A button (confirm) - minimum 44px touch target
    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_label("o")
            .with_size(ComponentSize{pixels(34), pixels(34)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing, prompt_y + 1.0f)
            .with_custom_background(afterhours::Color{180, 60, 60, 255})
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("a_btn"));

    div(context, mk(entity, 411),
        ComponentConfig{}
            .with_label(": Confirm")
            .with_size(ComponentSize{pixels(100), pixels(36)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing + 40.0f, prompt_y)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("confirm_label"));

    // B button (return) - minimum 44px touch target
    div(context, mk(entity, 420),
        ComponentConfig{}
            .with_label("o")
            .with_size(ComponentSize{pixels(34), pixels(34)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing * 2, prompt_y + 1.0f)
            .with_custom_background(afterhours::Color{180, 160, 60, 255})
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("b_btn"));

    div(context, mk(entity, 421),
        ComponentConfig{}
            .with_label(": Return")
            .with_size(ComponentSize{pixels(90), pixels(36)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing * 2 + 40.0f, prompt_y)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("return_label"));

    // L/R for change entry - moved left to prevent clipping (minimum 44px touch)
    div(context, mk(entity, 430),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(32), pixels(32)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing * 3, prompt_y + 2.0f)
            .with_custom_background(menu_item_bg)
            .with_border(text_gray, 2.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("l_btn"));

    div(context, mk(entity, 431),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(32), pixels(32)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing * 3 + 38.0f, prompt_y + 2.0f)
            .with_custom_background(menu_item_bg)
            .with_border(text_gray, 2.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r_btn"));

    div(context, mk(entity, 432),
        ComponentConfig{}
            .with_label(": Change Entry")
            .with_size(ComponentSize{pixels(150), pixels(36)})
            .with_absolute_position()
            .with_translate(200.0f + hint_spacing * 3 + 76.0f, prompt_y)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("change_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(fighter_menu, "Game Mockups",
                        "Anime fighting game menu (Cross Tag style)",
                        FighterMenuScreen)
