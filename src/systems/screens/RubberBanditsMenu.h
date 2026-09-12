#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RubberBanditsMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_item = 0;
  size_t character_index = 0;
  int player_count = 1;
  int coins = 20;
  bool loaded = false;
  bool supporter_selected = false;
  raylib::Texture2D cast_texture{};
  raylib::Texture2D logo_texture{};
  std::string footer_message = "Change the game's settings";

  const std::array<std::string, 6> menu_items{
      "OPTIONS", "CONTROLS", "LEADERBOARD", "ARMORY", "PLAY CREDITS", "BACK"};
  const std::array<std::string, 6> menu_hints{
      "Change the game's settings", "Learn the controls",
      "See the best bandits", "Choose your loadout", "Meet the team",
      "Return to the main menu"};
  const std::array<std::string, 4> character_names{
      "EARL THE BIRD", "THE GENTLEMAN", "HOT HEAD", "THE OUTLAW"};

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None);
  }

  static void draw_background(RectangleType r, float scale) {
    afterhours::draw_rectangle_gradient_v(
        {r.x, r.y, r.width, 302.f * scale},
        afterhours::Color{255, 233, 85, 255},
        afterhours::Color{255, 213, 25, 255});
    afterhours::draw_rectangle_gradient_v(
        {r.x, r.y + 302.f * scale, r.width, 274.f * scale},
        afterhours::Color{255, 213, 25, 255},
        afterhours::Color{255, 191, 0, 255});
    afterhours::draw_rectangle_gradient_v(
        {r.x, r.y + 576.f * scale, r.width, 144.f * scale},
        afterhours::Color{255, 191, 0, 255},
        afterhours::Color{245, 173, 5, 255});
  }

  static void draw_selected_row(RectangleType r, float scale) {
    afterhours::draw_rectangle_rounded(
        {r.x, r.y + 3.f * scale, r.width, r.height}, 1.f, 24,
        afterhours::Color{118, 189, 60, 255},
        RoundedCorners().all_round());
    afterhours::draw_rectangle_rounded(r, 1.f, 24,
                                       afterhours::Color{12, 202, 210, 255},
                                       RoundedCorners().all_round());
  }

  static void draw_diamond(RectangleType r, float scale) {
    const float cx = r.x + r.width - 26.f * scale;
    const float cy = r.y + r.height * .5f;
    const float radius = 12.f * scale;
    const float stroke = 3.f * scale;
    const afterhours::Color white{255, 255, 255, 255};
    afterhours::draw_line_ex({cx, cy - radius}, {cx + radius, cy}, stroke,
                             white);
    afterhours::draw_line_ex({cx + radius, cy}, {cx, cy + radius}, stroke,
                             white);
    afterhours::draw_line_ex({cx, cy + radius}, {cx - radius, cy}, stroke,
                             white);
    afterhours::draw_line_ex({cx - radius, cy}, {cx, cy - radius}, stroke,
                             white);
    afterhours::draw_line_ex(
        {cx, cy - 7.f * scale}, {cx + 7.f * scale, cy}, 1.5f * scale,
        afterhours::Color{225, 203, 76, 255});
    afterhours::draw_line_ex(
        {cx + 7.f * scale, cy}, {cx, cy + 7.f * scale}, 1.5f * scale,
        afterhours::Color{225, 203, 76, 255});
    afterhours::draw_line_ex(
        {cx, cy + 7.f * scale}, {cx - 7.f * scale, cy}, 1.5f * scale,
        afterhours::Color{225, 203, 76, 255});
    afterhours::draw_line_ex(
        {cx - 7.f * scale, cy}, {cx, cy - 7.f * scale}, 1.5f * scale,
        afterhours::Color{225, 203, 76, 255});
  }

  static void draw_lightning(RectangleType r, float scale) {
    const float cx = r.x + 229.f * scale;
    const float cy = r.y + 52.f * scale;
    afterhours::draw_circle(static_cast<int>(cx),
                            static_cast<int>(cy + 3.f * scale), 42.f * scale,
                            afterhours::Color{155, 118, 20, 90});
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            40.f * scale,
                            afterhours::Color{255, 255, 255, 255});
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            35.f * scale,
                            afterhours::Color{255, 255, 222, 255});
    const afterhours::Color bolt{239, 193, 20, 255};
    afterhours::draw_line_ex(
        {cx + 7.f * scale, cy - 28.f * scale},
        {cx - 6.f * scale, cy + 1.f * scale}, 13.f * scale, bolt);
    afterhours::draw_line_ex(
        {cx - 6.f * scale, cy + 1.f * scale},
        {cx + 8.f * scale, cy + 1.f * scale}, 13.f * scale, bolt);
    afterhours::draw_line_ex(
        {cx + 8.f * scale, cy + 1.f * scale},
        {cx - 7.f * scale, cy + 29.f * scale}, 13.f * scale, bolt);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float scale =
        context.screen_height > 0.f ? context.screen_height / 720.f : 1.f;
    const afterhours::Color white{255, 255, 255, 255};
    const afterhours::Color dark{54, 54, 43, 255};
    const afterhours::Color cyan{9, 200, 211, 255};

    Theme theme;
    theme.font = white;
    theme.darkfont = dark;
    theme.background = afterhours::Color{255, 213, 25, 255};
    theme.surface = afterhours::Color{255, 213, 25, 255};
    theme.primary = cyan;
    theme.secondary = afterhours::Color{73, 185, 233, 255};
    theme.accent = afterhours::Color{118, 189, 60, 255};
    theme.roundness = 0.f;
    theme.segments = 24;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("FredokaMockBold", pixels(24.f));

    if (!loaded) {
      loaded = true;
      cast_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path("images",
                                                "rubber_bandits/cast.png")
              .string()
              .c_str());
      logo_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path("images",
                                                "rubber_bandits/logo.png")
              .string()
              .c_str());
      raylib::SetTextureFilter(cast_texture, raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(logo_texture, raylib::TEXTURE_FILTER_BILINEAR);
    }

    if (context.pressed(InputAction::WidgetDown)) {
      selected_item = (selected_item + 1) % menu_items.size();
      footer_message = menu_hints[selected_item];
    }
    if (context.pressed(InputAction::WidgetUp)) {
      selected_item =
          (selected_item + menu_items.size() - 1) % menu_items.size();
      footer_message = menu_hints[selected_item];
    }

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_on_draw_bg([scale](RectangleType r) {
                  draw_background(r, scale);
                })
                .with_debug_name("rb_root"));

    sprite(context, mk(root.ent(), 1), cast_texture,
           {0.f, 0.f, static_cast<float>(cast_texture.width),
            static_cast<float>(cast_texture.height)},
           box(scale, 0.f, 0.f, 1280.f, 720.f)
               .with_ignore_pointer_events()
               .with_debug_name("rb_cast"));
    sprite(context, mk(root.ent(), 2), logo_texture,
           {0.f, 0.f, static_cast<float>(logo_texture.width),
            static_cast<float>(logo_texture.height)},
           box(scale, 42.f, 71.f, 368.f, 204.f)
               .with_ignore_pointer_events()
               .with_debug_name("rb_logo"));

    div(context, mk(root.ent(), 3),
        box(scale, 0.f, 0.f, 1280.f, 26.f)
            .with_custom_background(afterhours::Color{73, 185, 233, 255})
            .with_debug_name("rb_online_bar"));
    div(context, mk(root.ent(), 4),
        box(scale, 545.f, 0.f, 190.f, 26.f)
            .with_label("thia9uers · Online")
            .with_font("FredokaMockBold", pixels(20.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_debug_name("rb_online"));
    div(context, mk(root.ent(), 5),
        box(scale, 1166.f, 1.f, 26.f, 24.f)
            .with_label(std::to_string(player_count))
            .with_font("FredokaMockBold", pixels(19.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_on_draw_bg([white, scale](RectangleType r) {
              afterhours::draw_circle(
                  static_cast<int>(r.x + r.width * .5f),
                  static_cast<int>(r.y + r.height * .5f), 10.f * scale,
                  afterhours::Color{73, 185, 233, 255});
              afterhours::draw_circle_lines(
                  static_cast<int>(r.x + r.width * .5f),
                  static_cast<int>(r.y + r.height * .5f), 10.f * scale, white);
            })
            .with_debug_name("rb_player_count"));
    div(context, mk(root.ent(), 6),
        box(scale, 1196.f, 0.f, 49.f, 26.f)
            .with_label(std::to_string(coins))
            .with_font("FredokaMockBold", pixels(21.f * scale))
            .with_custom_background(afterhours::Color{241, 250, 203, 255})
            .with_border(white, 2.f * scale)
            .with_corner_radius(4.f * scale)
            .with_custom_text_color(afterhours::Color{87, 174, 78, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_debug_name("rb_coins"));

    auto promo =
        button(context, mk(root.ent(), 10),
               box(scale, 952.f, 69.f, 283.f, 104.f)
                   .with_custom_background(
                       supporter_selected
                           ? afterhours::Color{184, 147, 39, 235}
                           : afterhours::Color{181, 147, 39, 180})
                   .with_border(afterhours::Color{255, 249, 198, 255},
                                3.f * scale)
                   .with_corner_radius(14.f * scale)
                   .with_on_draw_fg([scale](RectangleType r) {
                     draw_lightning(r, scale);
                   })
                   .with_debug_name("rb_promo"));
    div(context, mk(root.ent(), 11),
        box(scale, 968.f, 87.f, 164.f, 72.f)
            .with_label(
                "Get more bandits!\nBuy the supporter\npack and show off!")
            .with_font("FredokaMockBold", pixels(21.5f * scale))
            .with_custom_text_color(white)
            .with_text_inset(0.f, 0.f)
            .with_letter_spacing(-.2f * scale)
            .with_text_overflow(TextOverflow::Wrap)
            .with_ignore_pointer_events());
    if (promo) {
      supporter_selected = true;
      footer_message = "Supporter pack selected";
    }

    const std::array<float, 6> row_y{344.f, 385.f, 426.f,
                                     467.f, 508.f, 566.f};
    for (size_t i = 0; i < menu_items.size(); ++i) {
      const bool selected = i == selected_item;
      auto item =
          button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, 57.f, row_y[i], 308.f, 38.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([selected, scale](RectangleType r) {
                       if (selected)
                         draw_selected_row(r, scale);
                     })
                     .with_on_draw_fg([selected, scale](RectangleType r) {
                       if (selected)
                         draw_diamond(r, scale);
                     })
                     .with_debug_name("rb_menu_" + std::to_string(i)));
      div(context, mk(root.ent(), 40 + static_cast<int>(i)),
          box(scale, 70.f, row_y[i], 250.f, 38.f)
              .with_label(menu_items[i])
              .with_font("FredokaMockBold", pixels(37.5f * scale))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Left)
              .with_text_inset(0.f, 0.f)
              .with_letter_spacing(.4f * scale)
              .with_text_shadow(afterhours::Color{206, 170, 50, 255}, 0.f,
                                2.f * scale)
              .with_ignore_pointer_events());
      if (item) {
        selected_item = i;
        footer_message = menu_hints[i];
      }
    }

    auto character_button = [&](int id, const char *label, float x,
                                int direction,
                                const std::string &debug_name) {
      if (button(
              context, mk(root.ent(), id),
              box(scale, x, 645.f, 40.f, 28.f)
                  .with_label(label)
                  .with_font("FredokaMockBold", pixels(18.f * scale))
                  .with_custom_background(dark)
                  .with_border(white, 2.f * scale)
                  .with_corner_radius(5.f * scale)
                  .with_custom_text_color(white)
                  .with_alignment(TextAlignment::Center)
                  .with_text_inset(0.f, 0.f)
                  .with_debug_name(debug_name))) {
        character_index =
            (character_index + character_names.size() +
             static_cast<size_t>(direction > 0 ? 1 : character_names.size() - 1)) %
            character_names.size();
        footer_message = "Selected " + character_names[character_index];
      }
    };
    character_button(60, "LB", 583.f, -1, "rb_character_prev");
    character_button(61, "RB", 822.f, 1, "rb_character_next");
    div(context, mk(root.ent(), 62),
        box(scale, 624.f, 635.f, 196.f, 29.f)
            .with_label(character_names[character_index])
            .with_font("FredokaMockBold", pixels(24.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_text_shadow(afterhours::Color{189, 141, 42, 255}, 0.f,
                              2.f * scale)
            .with_debug_name("rb_character_name"));
    div(context, mk(root.ent(), 63),
        box(scale, 663.f, 662.f, 130.f, 17.f)
            .with_label("FAVOURITE")
            .with_font("FredokaMockBold", pixels(15.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f));

    div(context, mk(root.ent(), 70),
        box(scale, 8.f, 675.f, 132.f, 14.f)
            .with_label("1.6.0 / RUBBER BANDITS")
            .with_font("FredokaMockBold", pixels(10.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Left)
            .with_text_inset(0.f, 0.f)
            .with_debug_name("rb_build"));
    div(context, mk(root.ent(), 71),
        box(scale, 0.f, 691.f, 1280.f, 29.f)
            .with_custom_background(cyan)
            .with_debug_name("rb_footer"));
    div(context, mk(root.ent(), 72),
        box(scale, 350.f, 691.f, 580.f, 29.f)
            .with_label(footer_message)
            .with_font("FredokaMockBold", pixels(21.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_debug_name("rb_footer_text"));
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
