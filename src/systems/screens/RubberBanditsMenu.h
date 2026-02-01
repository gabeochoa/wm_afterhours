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
      "REPORT PLAYER", "PLAY CREDITS", "BACK",
  };

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

    // RUBBER text (black with thick outline effect)
    // Note: Fredoka is wider, use 40pt instead of 48pt and wider container
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("RUBBER")
            .with_size(ComponentSize{pixels(320), pixels(56)})
            .with_absolute_position()
            .with_translate(title_x, title_y)
            .with_custom_background(afterhours::Color{0, 0, 0, 60})
            .with_font("Fredoka", 44.0f)
            .with_custom_text_color(afterhours::Color{20, 15, 10, 255})
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
            .with_font("Fredoka", 44.0f)
            .with_custom_text_color(accent_green)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("title_bandits"));

    // ========== TOP RIGHT: Player info ==========
    float info_x = (float)screen_w - 200.0f;

    // Online pill
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("thia9uers - Online")
            .with_size(ComponentSize{pixels(200), pixels(32)})
            .with_absolute_position()
            .with_translate(info_x - 55.0f, 20.0f)
            .with_custom_background(online_pill)
            .with_font("EqProRounded", 18.0f)
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
            .with_translate((float)screen_w - 70.0f, 15.0f)
            .with_custom_background(accent_blue)
            .with_border(afterhours::Color{65, 145, 185, 255}, 3.0f)
            .with_font("EqProRounded", 28.0f)
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
            .with_translate((float)screen_w - 70.0f, 68.0f)
            .with_custom_background(lightning)
            .with_border(afterhours::Color{215, 190, 55, 255}, 3.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("lightning"));

    // ========== MENU ITEMS ==========
    float menu_x = 55.0f;
    float menu_y = 175.0f;
    float menu_item_h = 44.0f;

    for (size_t i = 0; i < menu_items.size(); i++) {
      float item_y = menu_y + (float)i * menu_item_h;
      bool is_selected = (i == selected_item);
      afterhours::Color item_color = text_dark;

      // Selection indicator (triangle)
      if (is_selected) {
        div(context, mk(entity, 50 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(">")
                .with_size(ComponentSize{pixels(28), pixels(36)})
                .with_absolute_position()
                .with_translate(menu_x - 28.0f, item_y)
                .with_font("EqProRounded", 24.0f)
                .with_custom_text_color(text_dark)
                .with_debug_name("arrow_" + std::to_string(i)));
      }

      if (button(context, mk(entity, 100 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(menu_items[i])
                     .with_size(ComponentSize{pixels(240), pixels(40)})
                     .with_absolute_position()
                     .with_translate(menu_x, item_y)
                     .with_font("EqProRounded", 24.0f)
                     .with_custom_text_color(item_color)
                     .with_debug_name("menu_" + std::to_string(i)))) {
        selected_item = i;
      }
    }

    // ========== PROMO BOX ==========
    float promo_x = (float)screen_w - 350.0f;
    float promo_y = 140.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(75)})
            .with_absolute_position()
            .with_translate(promo_x, promo_y)
            .with_custom_background(menu_bg)
            .with_border(afterhours::Color{200, 185, 145, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("promo_box"));

    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Get more bandits!")
            .with_size(ComponentSize{pixels(180), pixels(32)})
            .with_absolute_position()
            .with_translate(promo_x + 15.0f, promo_y + 12.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("promo_text1"));

    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_label("Buy the supporter")
            .with_size(ComponentSize{pixels(180), pixels(28)})
            .with_absolute_position()
            .with_translate(promo_x + 15.0f, promo_y + 38.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_debug_name("promo_text2"));

    div(context, mk(entity, 203),
        ComponentConfig{}
            .with_label("pack and show off!")
            .with_size(ComponentSize{pixels(180), pixels(28)})
            .with_absolute_position()
            .with_translate(promo_x + 15.0f, promo_y + 58.0f)
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_debug_name("promo_text3"));

    // Promo icon
    div(context, mk(entity, 204),
        ComponentConfig{}
            .with_label("$")
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position()
            .with_translate(promo_x + 215.0f, promo_y + 12.0f)
            .with_custom_background(accent_green)
            .with_border(afterhours::Color{115, 170, 65, 255}, 3.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("promo_icon"));

    // ========== CHARACTER SELECTOR ==========
    float char_y = (float)screen_h - 90.0f;
    float char_x = (float)screen_w / 2.0f;

    // LB button
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("LB")
            .with_size(ComponentSize{pixels(52), pixels(36)})
            .with_absolute_position()
            .with_translate(char_x - 110.0f, char_y)
            .with_custom_background(text_dark)
            .with_font("EqProRounded", 18.0f)
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
            .with_font("EqProRounded", 20.0f)
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
            .with_font("EqProRounded", 18.0f)
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
            .with_font("EqProRounded", 18.0f)
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
            .with_font("EqProRounded", 18.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("hint"));

    // Note: Decorative coin elements removed to reduce clutter
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
