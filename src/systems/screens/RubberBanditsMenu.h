#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RubberBanditsMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_item = 0;
  int player_count = 1;
  int coins = 20;

  // Colors matching Rubber Bandits - vibrant cartoony yellow/orange (reduced saturation)
  afterhours::Color bg_yellow{245, 210, 95, 255};      // Softened yellow background
  afterhours::Color bg_yellow_dark{220, 185, 70, 255}; // Softened darker yellow
  afterhours::Color text_dark{35, 30, 20, 255};        // Darker brown text for contrast
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color menu_bg{255, 245, 200, 255};     // Light cream menu bg
  afterhours::Color highlight{255, 225, 145, 255};   // Highlighted item
  afterhours::Color accent_green{145, 200, 95, 255}; // Green accent
  afterhours::Color accent_blue{95, 175, 215, 255};  // Blue accent
  afterhours::Color lightning{255, 230, 85, 255};    // Lightning bolt yellow
  afterhours::Color online_pill{155, 185, 135, 255}; // Online status pill

  std::vector<std::string> menu_items = {
      "OPTIONS",       "CONTROLS",     "LEADERBOARD", "ARMORY",
      "PLAY CREDITS", "BACK",
  };

  bool promo_dismissed = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = text_white;
    theme.font_muted = afterhours::Color{120, 100, 70, 255};
    theme.background = bg_yellow;
    theme.surface = menu_bg;
    theme.primary = highlight;
    theme.secondary = accent_blue;
    theme.accent = accent_green;
    theme.error = afterhours::Color{200, 80, 80, 255};
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
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_yellow)
            .with_debug_name("bg"));

    // Ground area (darker yellow)
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), pixels(150)})
            .with_absolute_position()
            .with_translate(0.0f, (float)screen_h - 150.0f)
            .with_custom_background(bg_yellow_dark)
            .with_debug_name("ground"));

    // ========== TITLE: RUBBER BANDITS ==========
    float title_x = 45.0f;
    float title_y = 30.0f;

    // RUBBER text - off-white/cream for contrast against dark overlay
    // Note: Fredoka is wider, use 40pt instead of 48pt and wider container
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("RUBBER")
            .with_size(ComponentSize{pixels(320), pixels(56)})
            .with_absolute_position()
            .with_translate(title_x, title_y)
            .with_custom_background(afterhours::Color{60, 45, 25, 180})
            .with_font("Fredoka", h720(44.0f))
            .with_custom_text_color(afterhours::Color{250, 245, 235, 255})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("title_rubber"));

    // BANDITS text with $ for S
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("BANDIT$")
            .with_size(ComponentSize{pixels(340), pixels(56)})
            .with_absolute_position()
            .with_translate(title_x, title_y + 50.0f)
            .with_custom_background(afterhours::Color{0, 0, 0, 60})
            .with_font("Fredoka", h720(44.0f))
            .with_custom_text_color(accent_green)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("title_bandits"));

    // ========== TOP RIGHT: Player info ==========
    float info_x = (float)screen_w - 220.0f;

    // Online pill - larger and more prominent
    div(context, mk(entity, 19),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position()
            .with_translate(info_x - 135.0f, 28.0f)
            .with_custom_background(afterhours::Color{85, 185, 75, 255})
            .with_border(afterhours::Color{65, 155, 55, 255}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("online_dot"));

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("thia9uers - Online")
            .with_size(ComponentSize{pixels(285), pixels(42)})
            .with_absolute_position()
            .with_translate(info_x - 125.0f, 15.0f)
            .with_custom_background(online_pill)
            .with_border(afterhours::Color{115, 155, 105, 255}, 3.0f)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("online_status"));

    // Player count
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label(std::to_string(player_count))
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_absolute_position()
            .with_translate((float)screen_w - 70.0f, 12.0f)
            .with_custom_background(accent_blue)
            .with_border(afterhours::Color{65, 145, 185, 255}, 3.0f)
            .with_font("EqProRounded", h720(28.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("player_count"));

    // Lightning bolt (premium currency)
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_absolute_position()
            .with_translate((float)screen_w - 70.0f, 65.0f)
            .with_custom_background(lightning)
            .with_border(afterhours::Color{215, 190, 55, 255}, 3.0f)
            .with_font("EqProRounded", h720(28.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("lightning"));

    // ========== MENU ITEMS ==========
    float menu_x = 55.0f;
    float menu_y = 175.0f;
    float menu_item_h = 48.0f;

    // Menu background panel for better contrast
    div(context, mk(entity, 45),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(static_cast<int>(menu_items.size() * menu_item_h + 20))})
            .with_absolute_position()
            .with_translate(menu_x - 15.0f, menu_y - 10.0f)
            .with_custom_background(menu_bg)
            .with_border(afterhours::Color{180, 165, 125, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.1f)
            .with_debug_name("menu_bg"));

    for (size_t i = 0; i < menu_items.size(); i++) {
      float item_y = menu_y + (float)i * menu_item_h;
      bool is_selected = (i == selected_item);

      // Selection indicator (triangle)
      if (is_selected) {
        div(context, mk(entity, 50 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(">")
                .with_size(ComponentSize{pixels(28), pixels(36)})
                .with_absolute_position()
                .with_translate(menu_x - 28.0f, item_y + 4.0f)
                .with_font("EqProRounded", h720(24.0f))
                .with_custom_text_color(text_dark)
                .with_debug_name("arrow_" + std::to_string(i)));
      }

      // Menu item with background for better visibility
      afterhours::Color item_bg = is_selected ? highlight : menu_bg;
      if (button(context, mk(entity, 100 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(menu_items[i])
                     .with_size(ComponentSize{pixels(240), pixels(44)})
                     .with_absolute_position()
                     .with_translate(menu_x, item_y)
                     .with_custom_background(item_bg)
                     .with_border(is_selected ? afterhours::Color{220, 190, 100, 255} : afterhours::Color{0, 0, 0, 0}, 2.0f)
                     .with_font("EqProRounded", h720(22.0f))
                     .with_custom_text_color(text_dark)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.15f)
                     .with_debug_name("menu_" + std::to_string(i)))) {
        selected_item = i;
      }
    }

    // ========== PROMO BOX (dismissible, less prominent) ==========
    if (!promo_dismissed) {
      float promo_x = (float)screen_w - 290.0f;
      float promo_y = 160.0f;

      // Smaller, less prominent promo box with muted colors
      div(context, mk(entity, 200),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(220), pixels(55)})
              .with_absolute_position()
              .with_translate(promo_x, promo_y)
              .with_custom_background(afterhours::Color{245, 235, 210, 160})
              .with_border(afterhours::Color{180, 165, 135, 140}, 1.5f)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.15f)
              .with_debug_name("promo_box"));

      div(context, mk(entity, 201),
          ComponentConfig{}
              .with_label("Supporter pack available")
              .with_size(ComponentSize{pixels(180), pixels(24)})
              .with_absolute_position()
              .with_translate(promo_x + 10.0f, promo_y + 8.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
              .with_debug_name("promo_text1"));

      div(context, mk(entity, 202),
          ComponentConfig{}
              .with_label("More characters in shop")
              .with_size(ComponentSize{pixels(180), pixels(20)})
              .with_absolute_position()
              .with_translate(promo_x + 10.0f, promo_y + 30.0f)
              .with_font("EqProRounded", h720(12.0f))
              .with_custom_text_color(afterhours::Color{130, 115, 85, 255})
              .with_debug_name("promo_text2"));

      // Dismiss X button
      if (button(context, mk(entity, 205),
                 ComponentConfig{}
                     .with_label("X")
                     .with_size(ComponentSize{pixels(24), pixels(24)})
                     .with_absolute_position()
                     .with_translate(promo_x + 190.0f, promo_y + 5.0f)
                     .with_custom_background(afterhours::Color{180, 165, 135, 200})
                     .with_font("EqProRounded", h720(14.0f))
                     .with_custom_text_color(afterhours::Color{80, 70, 50, 255})
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.5f)
                     .with_debug_name("promo_dismiss"))) {
        promo_dismissed = true;
      }
    }

    // ========== CHARACTER SELECTOR ==========
    float char_y = (float)screen_h - 90.0f;
    float char_x = (float)screen_w / 2.0f;

    // Character selector background container for grouping clarity
    div(context, mk(entity, 298),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(400), pixels(100)})
            .with_absolute_position()
            .with_translate(char_x - 145.0f, char_y - 34.0f)
            .with_custom_background(afterhours::Color{255, 245, 200, 150})
            .with_border(afterhours::Color{180, 165, 125, 120}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("char_selector_bg"));

    // Context label for character selector - larger and more prominent
    div(context, mk(entity, 299),
        ComponentConfig{}
            .with_label("SELECT CHARACTER")
            .with_size(ComponentSize{pixels(200), pixels(24)})
            .with_absolute_position()
            .with_translate(char_x - 50.0f, char_y - 28.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_debug_name("char_label"));

    // LB button
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("LB")
            .with_size(ComponentSize{pixels(52), pixels(36)})
            .with_absolute_position()
            .with_translate(char_x - 110.0f, char_y)
            .with_custom_background(text_dark)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("lb"));

    // Character name
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("EARL THE BIRD")
            .with_size(ComponentSize{pixels(220), pixels(36)})
            .with_absolute_position()
            .with_translate(char_x - 55.0f, char_y)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("char_name"));

    // RB button
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("RB")
            .with_size(ComponentSize{pixels(52), pixels(36)})
            .with_absolute_position()
            .with_translate(char_x + 170.0f, char_y)
            .with_custom_background(text_dark)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.3f)
            .with_debug_name("rb"));

    // FAVOURITE label
    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("FAVOURITE")
            .with_size(ComponentSize{pixels(150), pixels(28)})
            .with_absolute_position()
            .with_translate(char_x - 25.0f, char_y + 38.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("favourite"));

    // ========== BOTTOM HINT ==========
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("Change the game's settings")
            .with_size(ComponentSize{pixels(320), pixels(32)})
            .with_absolute_position()
            .with_translate(55.0f, (float)screen_h - 50.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(afterhours::Color{180, 180, 180, 255})
            .with_debug_name("bottom_hint"));

    // Note: Decorative coin elements removed to reduce clutter
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
