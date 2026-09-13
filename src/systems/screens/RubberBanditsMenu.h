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
  bool menu_open = true;
  raylib::Texture2D cast_texture{};
  raylib::Texture2D logo_texture{};
  std::string footer_message = "Change the game's settings";

  const std::array<std::string, 6> menu_items{
      "OPTIONS", "CONTROLS", "LEADERBOARD", "ARMORY", "CREDITS", "CLOSE MENU"};
  const std::array<std::string, 6> menu_hints{
      "Change the game's settings", "Learn the controls",
      "See the best bandits", "Choose your loadout", "Meet the team",
      "Return to the character lineup"};
  const std::array<std::string, 4> character_names{
      "EARL THE BIRD", "THE GENTLEMAN", "HOT HEAD", "THE OUTLAW"};

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None)
        .with_corner_radius(0);
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
    const float cx = r.x + r.width - 48.f * scale;
    const float cy = r.y + 48.f * scale;
    afterhours::draw_circle(static_cast<int>(cx),
                            static_cast<int>(cy + 3.f * scale), 42.f * scale,
                            afterhours::Color{155, 118, 20, 90});
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            40.f * scale,
                            afterhours::Color{255, 255, 255, 255});
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            35.f * scale,
                            afterhours::Color{255, 255, 222, 255});
    const afterhours::Color bolt{126, 86, 18, 255};
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
    const float scale = std::min(context.screen_width / 1280.f,
                                 context.screen_height / 720.f);
    const afterhours::Color white{255, 255, 255, 255};
    const afterhours::Color dark{63, 52, 28, 255};
    const afterhours::Color cyan{9, 200, 211, 255};
    Theme theme;
    theme.font = white;
    theme.darkfont = dark;
    theme.background = afterhours::Color{255, 213, 25, 255};
    theme.surface = theme.background;
    theme.primary = cyan;
    theme.secondary = afterhours::Color{73, 185, 233, 255};
    theme.accent = afterhours::Color{118, 189, 60, 255};
    theme.corner_radius = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("FredokaMockBold", pixels(24 * scale));
    if (!loaded) {
      loaded = true;
      cast_texture = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "rubber_bandits/cast.png").string().c_str());
      logo_texture = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "rubber_bandits/logo.png").string().c_str());
      raylib::SetTextureFilter(cast_texture, raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(logo_texture, raylib::TEXTURE_FILTER_BILINEAR);
    }
    if (context.pressed(InputAction::MenuBack)) {
      menu_open = !menu_open;
      footer_message = menu_open ? menu_hints[selected_item] : "Character lineup";
    }
    if (menu_open && context.pressed(InputAction::WidgetDown)) {
      selected_item = (selected_item + 1) % menu_items.size();
      footer_message = menu_hints[selected_item];
    }
    if (menu_open && context.pressed(InputAction::WidgetUp)) {
      selected_item = (selected_item + menu_items.size() - 1) % menu_items.size();
      footer_message = menu_hints[selected_item];
    }
    div(context, mk(entity, 900),
        ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
            .with_custom_background(theme.background).with_corner_radius(0)
            .with_debug_name("rb_background"));
    auto root = div(context, mk(entity, 0),
        box(scale, 0, 0, 1280, 720)
            .with_absolute_position(pixels((context.screen_width - 1280 * scale) / 2),
                                    pixels((context.screen_height - 720 * scale) / 2))
            .with_on_draw_bg([scale](RectangleType r) { draw_background(r, scale); })
            .with_debug_name("rb_root"));
    auto label = [&](int id, std::string value, float x, float y, float w, float h,
                     float size, afterhours::Color color, const std::string &name,
                     TextAlignment alignment = TextAlignment::Left) {
      return div(context, mk(root.ent(), id), box(scale, x, y, w, h)
          .with_label(value).with_font("FredokaMockBold", pixels(size * scale))
          .with_custom_text_color(color).with_alignment(alignment)
          .with_text_inset(0, 0).with_ignore_pointer_events().with_debug_name(name));
    };
    sprite(context, mk(root.ent(), 1), cast_texture,
        {0, 0, static_cast<float>(cast_texture.width), static_cast<float>(cast_texture.height)},
        box(scale, 0, 0, 1280, 720).with_ignore_pointer_events().with_debug_name("rb_cast"));
    sprite(context, mk(root.ent(), 2), logo_texture,
        {0, 0, static_cast<float>(logo_texture.width), static_cast<float>(logo_texture.height)},
        box(scale, 52, 78, 330, 183).with_ignore_pointer_events().with_debug_name("rb_logo"));
    div(context, mk(root.ent(), 3), box(scale, 0, 0, 1280, 40)
        .with_custom_background(afterhours::Color{43, 150, 201, 255}).with_debug_name("rb_online_bar"));
    label(4, "thia9uers", 32, 4, 154, 32, 22, white, "rb_online");
    div(context, mk(root.ent(), 8), box(scale, 196, 7, 96, 26)
        .with_custom_background(afterhours::Color{30, 109, 135, 255}).with_corner_radius(13 * scale));
    label(5, "Online", 196, 7, 96, 26, 18, white, "rb_connection", TextAlignment::Center);
    label(6, "Players  " + std::to_string(player_count), 980, 4, 136, 32, 20, white, "rb_player_count");
    label(7, "Coins  " + std::to_string(coins), 1132, 4, 124, 32, 20, white, "rb_coins");
    auto promo = button(context, mk(root.ent(), 10), box(scale, 938, 72, 310, 138)
        .with_custom_background(afterhours::Color{165, 124, 29, 235})
        .with_border(white, 2 * scale).with_corner_radius(14 * scale)
        .with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(
              {r.x + 6 * scale, r.y + 6 * scale, r.width - 12 * scale, r.height - 12 * scale},
              .2f, 24, 1 * scale, afterhours::Color{255, 236, 150, 255});
          draw_lightning(r, scale);
        }).with_debug_name("rb_promo"));
    label(11, "Supporter pack", 956, 83, 220, 30, 24, white, "rb_promo_title");
    label(12, supporter_selected ? "Cosmetic pack preview" : "More looks for your bandits.",
          956, 118, 214, 24, 16, white, "rb_promo_description");
    label(13, supporter_selected ? "No purchase in this demo" : "View pack  >", 956, 163,
          266, 28, 19, white, "rb_promo_action");
    if (promo) {
      supporter_selected = !supporter_selected;
      footer_message = supporter_selected ? "Supporter pack preview" : menu_hints[selected_item];
    }
    if (menu_open) {
      const std::array<float, 6> row_y{324, 370, 416, 462, 508, 577};
      for (size_t i = 0; i < menu_items.size(); ++i) {
        const bool selected = i == selected_item;
        auto item = button(context, mk(root.ent(), 20 + static_cast<int>(i)),
            box(scale, 48, row_y[i], 354, 42).with_label(menu_items[i])
                .with_font("FredokaMockBold", pixels((i == 5 ? 25 : 32) * scale))
                .with_custom_text_color(white).with_alignment(TextAlignment::Left)
                .with_text_inset(0, 0).with_letter_spacing(-.3f * scale)
                .with_click_activation(ClickActivationMode::Release)
                .with_on_draw_bg([selected, scale](RectangleType r) {
                  if (!selected) return;
                  draw_selected_row(r, scale);
                }).with_on_draw_fg([selected, scale](RectangleType r) {
                  if (!selected) return;
                  draw_diamond(r, scale);
                }).with_debug_name("rb_menu_" + std::to_string(i)));
        item.ent().get<HasLabel>().text_x_offset = 22 * scale;
        if (!item) continue;
        selected_item = i;
        footer_message = menu_hints[i];
        if (i == 5) menu_open = false;
      }
      div(context, mk(root.ent(), 44), box(scale, 70, 564, 310, 1)
          .with_custom_background(afterhours::Color{142, 109, 30, 135}));
    } else {
      if (button(context, mk(root.ent(), 45), box(scale, 48, 324, 354, 48)
          .with_label("OPEN MENU").with_font("FredokaMockBold", pixels(28 * scale))
          .with_custom_background(cyan).with_corner_radius(20 * scale)
          .with_debug_name("rb_open_menu"))) {
        menu_open = true;
        selected_item = 0;
        footer_message = menu_hints[0];
      }
    }
    const std::array<float, 4> marker_x{708, 936, 1070, 1140};
    const std::array<float, 4> marker_y{576, 558, 570, 553};
    div(context, mk(root.ent(), 50), box(scale, marker_x[character_index] - 49,
        marker_y[character_index] - 8, 98, 16)
        .with_on_draw_fg([scale, cyan](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(r, 1, 32, 3 * scale, cyan);
        }).with_ignore_pointer_events().with_debug_name("rb_selected_marker"));
    div(context, mk(root.ent(), 51), box(scale, 468, 608, 764, 60)
        .with_custom_background(afterhours::Color{137, 99, 26, 245})
        .with_corner_radius(12 * scale).with_debug_name("rb_character_panel"));
    auto character_button = [&](int id, const char *caption, float x, int direction,
                                const std::string &name) {
      if (!button(context, mk(root.ent(), id), box(scale, x, 622, 48, 32)
          .with_label(caption).with_font("FredokaMockBold", pixels(18 * scale))
          .with_custom_background(dark).with_border(white, 1 * scale)
          .with_corner_radius(6 * scale).with_custom_text_color(white)
          .with_debug_name(name))) return;
      character_index = (character_index + (direction > 0 ? 1 : character_names.size() - 1)) % character_names.size();
      footer_message = "Selected " + character_names[character_index];
    };
    character_button(60, "LB", 486, -1, "rb_character_prev");
    character_button(61, "RB", 1166, 1, "rb_character_next");
    label(62, character_names[character_index], 548, 614, 418, 28, 24, white,
          "rb_character_name", TextAlignment::Center);
    label(63, "FAVOURITE", 548, 645, 418, 18, 13, afterhours::Color{255, 237, 160, 255},
          "rb_favourite", TextAlignment::Center);
    label(64, std::to_string(character_index + 1) + " of 4", 1002, 622, 126, 32,
          20, white, "rb_character_count", TextAlignment::Center);
    label(70, "1.6.0 / RUBBER BANDITS", 32, 645, 260, 24, 14, dark, "rb_build");
    div(context, mk(root.ent(), 71), box(scale, 0, 678, 1280, 42)
        .with_custom_background(cyan).with_debug_name("rb_footer"));
    label(72, footer_message, 32, 686, 830, 26, 21, dark, "rb_footer_text");
    label(73, "Enter  Select     Esc  Menu", 922, 686, 326, 26, 18, dark, "rb_input_guide");
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
