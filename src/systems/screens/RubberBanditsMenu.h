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

  // Colors matching Rubber Bandits - vibrant cartoony yellow/orange
  afterhours::Color bg_yellow{245, 210, 95, 255};
  afterhours::Color bg_yellow_dark{220, 185, 70, 255};
  afterhours::Color text_dark{35, 30, 20, 255};
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color menu_bg{255, 245, 200, 255};
  afterhours::Color highlight{255, 225, 145, 255};
  afterhours::Color accent_green{145, 200, 95, 255};
  afterhours::Color accent_blue{95, 175, 215, 255};
  afterhours::Color lightning{255, 230, 85, 255};
  afterhours::Color online_pill{155, 185, 135, 255};

  std::vector<std::string> menu_items = {
      "OPTIONS", "CONTROLS", "LEADERBOARD", "ARMORY", "PLAY CREDITS", "BACK",
  };

  bool promo_dismissed = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(18.0f));
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
    context.scaling_mode = ScalingMode::Adaptive;

    // ═══════════════════════════════════════════════════════════════
    // ROOT - full screen
    // ═══════════════════════════════════════════════════════════════
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg_yellow)
                   .with_no_wrap()
                   .with_debug_name("rb_root"));

    // ═══════════════════════════════════════════════════════════════
    // TOP BAR: Title + Online status + Player info
    // ═══════════════════════════════════════════════════════════════
    auto top_bar = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(145)})
            .with_no_wrap()
            .with_align_items(AlignItems::FlexStart)
            .with_padding(Padding{
                .top = pixels(30), .left = pixels(45), .right = pixels(25)})
            .with_debug_name("top_bar"));

    // Title area
    auto title_area =
        vstack(context, mk(top_bar.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(340), percent(1.0f)})
                   .with_no_wrap());

    // RUBBER text
    div(context, mk(title_area.ent()),
        ComponentConfig{}
            .with_label("RUBBER")
            .with_size(ComponentSize{pixels(320), pixels(56)})
            .with_custom_background(afterhours::Color{60, 45, 25, 180})
            .with_font("Fredoka", pixels(44.0f))
            .with_custom_text_color(afterhours::Color{250, 245, 235, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f));

    // BANDITS text
    div(context, mk(title_area.ent()),
        ComponentConfig{}
            .with_label("BANDIT$")
            .with_size(ComponentSize{pixels(340), pixels(56)})
            .with_custom_background(afterhours::Color{40, 30, 15, 200})
            .with_font("Fredoka", pixels(44.0f))
            .with_custom_text_color(afterhours::Color{250, 245, 235, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f));

    // Spacer
    div(context, mk(top_bar.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    // Online status pill
    auto online_area =
        hstack(context, mk(top_bar.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(350), pixels(50)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap());

    // Online dot
    div(context, mk(online_area.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_custom_background(afterhours::Color{85, 185, 75, 255})
            .with_border(afterhours::Color{65, 155, 55, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("online_dot"));

    div(context, mk(online_area.ent()),
        ComponentConfig{}
            .with_label("thia9uers - Online")
            .with_size(ComponentSize{pixels(285), pixels(42)})
            .with_custom_background(online_pill)
            .with_border(afterhours::Color{115, 155, 105, 255}, 3.0f)
            .with_font("EqProRounded", pixels(22.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_margin(Margin{.left = pixels(5)}));

    // Player count + lightning badges
    auto badges = vstack(context, mk(top_bar.ent()),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(55), children()})
                             .with_no_wrap()
                             .with_align_items(AlignItems::Center)
                             .with_margin(Margin{.left = pixels(10)}));

    div(context, mk(badges.ent()),
        ComponentConfig{}
            .with_label(std::to_string(player_count))
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_custom_background(accent_blue)
            .with_border(afterhours::Color{65, 145, 185, 255}, 3.0f)
            .with_font("EqProRounded", pixels(28.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    div(context, mk(badges.ent()),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(45), pixels(45)})
            .with_custom_background(lightning)
            .with_border(afterhours::Color{215, 190, 55, 255}, 3.0f)
            .with_font("EqProRounded", pixels(28.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    // ═══════════════════════════════════════════════════════════════
    // MIDDLE: Menu + Promo
    // ═══════════════════════════════════════════════════════════════
    auto middle = hstack(context, mk(root.ent()),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.0f), expand()})
                             .with_no_wrap()
                             .with_align_items(AlignItems::FlexStart)
                             .with_padding(Padding{.top = pixels(15),
                                                   .left = pixels(40),
                                                   .right = pixels(40)})
                             .with_debug_name("middle"));

    // Menu panel
    auto menu_panel =
        vstack(context, mk(middle.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{
                       pixels(280),
                       pixels(static_cast<float>(menu_items.size() * 48 + 20))})
                   .with_custom_background(menu_bg)
                   .with_border(afterhours::Color{180, 165, 125, 255}, 3.0f)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.1f)
                   .with_no_wrap()
                   .with_padding(Padding{.top = pixels(10),
                                         .bottom = pixels(10),
                                         .left = pixels(15),
                                         .right = pixels(15)})
                   .with_debug_name("menu_bg"));

    for (size_t i = 0; i < menu_items.size(); i++) {
      bool is_selected = (i == selected_item);
      afterhours::Color item_bg = is_selected ? highlight : menu_bg;

      auto item_row =
          hstack(context, mk(menu_panel.ent(), static_cast<int>(i)),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(44)})
                     .with_align_items(AlignItems::Center)
                     .with_no_wrap());

      // Selection arrow
      if (is_selected) {
        div(context, mk(item_row.ent(), 0),
            ComponentConfig{}
                .with_label(">")
                .with_size(ComponentSize{pixels(28), pixels(36)})
                .with_font("EqProRounded", pixels(24.0f))
                .with_custom_text_color(text_dark));
      }

      if (button(context, mk(item_row.ent(), 1),
                 ComponentConfig{}
                     .with_label(menu_items[i])
                     .with_size(ComponentSize{expand(), percent(1.0f)})
                     .with_custom_background(item_bg)
                     .with_border(is_selected
                                      ? afterhours::Color{220, 190, 100, 255}
                                      : afterhours::Color{0, 0, 0, 0},
                                  2.0f)
                     .with_font("EqProRounded", pixels(22.0f))
                     .with_custom_text_color(text_dark)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.15f)
                     .with_debug_name("menu_" + std::to_string(i)))) {
        selected_item = i;
      }
    }

    // Spacer between menu and promo
    div(context, mk(middle.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), pixels(1)})
            .with_skip_tabbing(true));

    // Promo box (dismissible)
    if (!promo_dismissed) {
      auto promo = vstack(
          context, mk(middle.ent()),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(220), pixels(55)})
              .with_custom_background(afterhours::Color{245, 235, 210, 160})
              .with_border(afterhours::Color{180, 165, 135, 140}, 1.5f)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.15f)
              .with_no_wrap()
              .with_padding(Padding{
                  .top = pixels(8), .left = pixels(10), .right = pixels(10)})
              .with_debug_name("promo_box"));

      auto promo_top =
          hstack(context, mk(promo.ent()),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(24)})
                     .with_no_wrap()
                     .with_align_items(AlignItems::Center));

      div(context, mk(promo_top.ent()),
          ComponentConfig{}
              .with_label("Supporter pack available")
              .with_size(ComponentSize{expand(), pixels(24)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(afterhours::Color{100, 85, 55, 255}));

      // Dismiss X
      if (button(
              context, mk(promo_top.ent()),
              ComponentConfig{}
                  .with_label("X")
                  .with_size(ComponentSize{pixels(24), pixels(24)})
                  .with_custom_background(afterhours::Color{180, 165, 135, 200})
                  .with_font("EqProRounded", pixels(14.0f))
                  .with_custom_text_color(afterhours::Color{80, 70, 50, 255})
                  .with_alignment(TextAlignment::Center)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.5f)
                  .with_debug_name("promo_dismiss"))) {
        promo_dismissed = true;
      }

      div(context, mk(promo.ent()),
          ComponentConfig{}
              .with_label("More characters in shop")
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(afterhours::Color{100, 85, 55, 255}));
    }

    // ═══════════════════════════════════════════════════════════════
    // BOTTOM: Ground + Character selector + Hint
    // ═══════════════════════════════════════════════════════════════
    auto bottom =
        vstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(130)})
                   .with_custom_background(bg_yellow_dark)
                   .with_no_wrap()
                   .with_debug_name("ground"));

    // Character selector row
    auto char_row =
        hstack(context, mk(bottom.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(90)})
                   .with_justify_content(JustifyContent::Center)
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_debug_name("char_selector"));

    auto char_box = vstack(
        context, mk(char_row.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(400), pixels(90)})
            .with_custom_background(afterhours::Color{255, 245, 200, 150})
            .with_border(afterhours::Color{180, 165, 125, 120}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_no_wrap()
            .with_align_items(AlignItems::Center)
            .with_padding(Padding{.top = pixels(6)})
            .with_debug_name("char_selector_bg"));

    // SELECT CHARACTER label
    div(context, mk(char_box.ent()),
        ComponentConfig{}
            .with_label("SELECT CHARACTER")
            .with_size(ComponentSize{pixels(200), pixels(24)})
            .with_font("EqProRounded", pixels(16.0f))
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_alignment(TextAlignment::Center));

    // LB + Name + RB row
    auto name_row =
        hstack(context, mk(char_box.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(36)})
                   .with_justify_content(JustifyContent::Center)
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap());

    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("LB")
            .with_size(ComponentSize{pixels(52), pixels(36)})
            .with_custom_background(text_dark)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("EARL THE BIRD")
            .with_size(ComponentSize{pixels(220), pixels(36)})
            .with_font("EqProRounded", pixels(20.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("RB")
            .with_size(ComponentSize{pixels(52), pixels(36)})
            .with_custom_background(text_dark)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    // FAVOURITE label
    div(context, mk(char_box.ent()),
        ComponentConfig{}
            .with_label("FAVOURITE")
            .with_size(ComponentSize{pixels(150), pixels(28)})
            .with_custom_text_color(afterhours::Color{100, 85, 55, 255})
            .with_alignment(TextAlignment::Center));

    // Bottom hint
    auto hint_row =
        hstack(context, mk(bottom.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(40)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_padding(Padding{.left = pixels(55)}));

    div(context, mk(hint_row.ent()),
        ComponentConfig{}
            .with_label("Change the game's settings")
            .with_size(ComponentSize{pixels(320), pixels(32)})
            .with_custom_background(afterhours::Color{60, 45, 25, 140})
            .with_font("EqProRounded", pixels(18.0f))
            .with_custom_text_color(afterhours::Color{255, 250, 235, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f));
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
