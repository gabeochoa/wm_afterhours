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

  const std::array<std::string, 7> menu_items{
      "OPTIONS", "CONTROLS", "LEADERBOARD", "ARMORY", "REPORT PLAYER",
      "PLAY CREDITS", "BACK"};
  const std::array<std::string, 7> menu_hints{
      "Change the game's settings", "Learn the controls",
      "See the best bandits", "Choose your loadout", "Report a player",
      "Meet the team", "Return to the character lineup"};
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
    const float cy = r.y + r.height * .5f;
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
    div(context, mk(entity),
        ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
            .with_custom_background(theme.background).with_corner_radius(0)
            .with_debug_name("rb_background"));
    auto root = div(context, mk(entity),
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
    sprite(context, mk(root.ent()), cast_texture,
        {0, 0, static_cast<float>(cast_texture.width), static_cast<float>(cast_texture.height)},
        box(scale, 0, 0, 1280, 720).with_ignore_pointer_events().with_debug_name("rb_cast"));
    // The reference lineup is busier than the cast image alone: extra loose
    // bills and a couple of small bandits crowd the floor between the menu
    // and the hero. Drawn as plain shapes (no extracted art).
    div(context, mk(root.ent()), box(scale, 360, 470, 920, 140)
        .with_ignore_pointer_events().with_debug_name("rb_money_props")
        .with_on_draw_fg([scale](RectangleType r) {
          const float ox = r.x - 360.f * scale;
          const float oy = r.y - 470.f * scale;
          const afterhours::Color bill{89, 176, 92, 255};
          const afterhours::Color bill_dark{52, 119, 62, 255};
          const afterhours::Color gold{255, 203, 46, 255};
          const afterhours::Color skin{227, 180, 141, 255};
          const afterhours::Color floor_shadow{150, 100, 5, 170};
          // Floor shadows under each standing bandit (feet sit near the
          // character-panel line): the reference grounds the whole row.
          const std::array<std::array<float, 3>, 8> feet{{
              {545, 592, 52}, {758, 585, 68}, {880, 596, 46}, {935, 598, 44},
              {1005, 600, 42}, {1045, 598, 44}, {1085, 596, 46}, {1140, 594, 48}}};
          for (const auto &f : feet) {
            afterhours::draw_ellipse(
                static_cast<int>(ox + f[0] * scale), static_cast<int>(oy + f[1] * scale),
                f[2] * scale, 9 * scale, floor_shadow);
          }
          for (int i = 0; i < 56; ++i) {
            const float bx = ox + (362.f + static_cast<float>((i * 37) % 880)) * scale;
            const float by = oy + (540.f + static_cast<float>((i * 13) % 6) * 9.f) * scale;
            afterhours::draw_rectangle({bx, by, 22 * scale, 11 * scale}, bill);
            afterhours::draw_line_ex({bx, by}, {bx + 22 * scale, by + 11 * scale},
                                     1 * scale, bill_dark);
            afterhours::draw_circle(static_cast<int>(bx + 11 * scale),
                                    static_cast<int>(by + 5.5f * scale), 2.5f * scale,
                                    bill_dark);
          }
          // Money piles: stacked bills fanned into low mounds.
          const auto pile = [=](float fx, float fy, int n) {
            for (int i = 0; i < n; ++i) {
              const float w = 30.f * scale - static_cast<float>(i) * 2.4f * scale;
              afterhours::draw_rectangle(
                  {ox + fx * scale - w / 2, oy + (fy - static_cast<float>(i) * 4.f) * scale,
                   w, 5 * scale},
                  i % 2 == 0 ? bill : bill_dark);
            }
          };
          pile(610, 600, 5); pile(700, 604, 4); pile(880, 604, 5); pile(1010, 606, 4);
          pile(1180, 602, 5); pile(470, 604, 3);
          // Burst suitcases at the lineup's feet, bills spilling out.
          const auto suitcase = [=](float fx, float fy, float w, float h) {
            const afterhours::Color case_c{138, 44, 62, 255};
            const afterhours::Color case_dark{96, 28, 42, 255};
            afterhours::draw_rectangle_rounded(
                {ox + fx * scale, oy + fy * scale, w * scale, h * scale}, .12f, 8,
                case_c, RoundedCorners().all_round());
            afterhours::draw_rectangle({ox + fx * scale, oy + (fy + h * .42f) * scale,
                                        w * scale, h * .09f}, case_dark);
            afterhours::draw_rectangle_rounded(
                {ox + (fx + w * .38f) * scale, oy + (fy - 5) * scale, w * .24f * scale,
                 7 * scale},
                .4f, 6, case_dark, RoundedCorners().all_round());
            afterhours::draw_rectangle({ox + fx * scale + 4 * scale, oy + (fy + h) * scale,
                                        26 * scale, 8 * scale}, bill);
            afterhours::draw_rectangle({ox + fx * scale + w * scale - 30 * scale,
                                        oy + (fy + h) * scale, 26 * scale, 8 * scale}, bill);
          };
          suitcase(500, 540, 104, 48); suitcase(830, 548, 116, 50);
          const auto mini_bandit = [&](float fx, float fy, afterhours::Color body) {
            afterhours::draw_rectangle_rounded(
                {ox + (fx - 16) * scale, oy + fy * scale, 32 * scale, 44 * scale}, .2f,
                12, body, RoundedCorners().all_round());
            afterhours::draw_circle(static_cast<int>(ox + fx * scale),
                                    static_cast<int>(oy + (fy - 8) * scale),
                                    15 * scale, skin);
            afterhours::draw_rectangle_rounded(
                {ox + (fx - 14) * scale, oy + (fy - 14) * scale, 28 * scale,
                 9 * scale},
                .3f, 9, afterhours::Color{20, 20, 20, 255},
                RoundedCorners().all_round());
            afterhours::draw_rectangle(
                {ox + (fx - 11) * scale, oy + (fy + 44) * scale, 8 * scale, 8 * scale},
                afterhours::Color{70, 50, 30, 255});
            afterhours::draw_rectangle(
                {ox + (fx + 3) * scale, oy + (fy + 44) * scale, 8 * scale, 8 * scale},
                afterhours::Color{70, 50, 30, 255});
          };
          mini_bandit(425, 488, afterhours::Color{69, 166, 171, 255});
          mini_bandit(1215, 478, afterhours::Color{196, 115, 57, 255});
          mini_bandit(1175, 494, afterhours::Color{140, 95, 60, 255});
          // Spilled coins between the piles.
          for (int i = 0; i < 9; ++i) {
            afterhours::draw_circle(
                static_cast<int>(ox + (655.f + static_cast<float>(i * 13)) * scale),
                static_cast<int>(oy + (600.f - static_cast<float>(i % 3) * 5.f) * scale),
                7 * scale, gold);
            afterhours::draw_circle(
                static_cast<int>(ox + (935.f + static_cast<float>(i * 11)) * scale),
                static_cast<int>(oy + (602.f - static_cast<float>((i + 1) % 3) * 5.f) * scale),
                7 * scale, gold);
          }
        }));
    sprite(context, mk(root.ent()), logo_texture,
        {0, 0, static_cast<float>(logo_texture.width), static_cast<float>(logo_texture.height)},
        box(scale, 52, 78, 330, 183).with_ignore_pointer_events().with_debug_name("rb_logo"));
    div(context, mk(root.ent()), box(scale, 0, 0, 1280, 40)
        .with_custom_background(afterhours::Color{43, 150, 201, 255}).with_debug_name("rb_online_bar"));
    label(4, "thia9uers - Online", 32, 6, 300, 28, 22, white, "rb_online");
    // Keep player/coin labels clear of the launcher HUD corner (x1068..1268,
    // y8..40); they sit left of it in the bar. PR #38 moved this there too.
    // Chips behind each readout, as the reference pills Players/Coins.
    div(context, mk(root.ent()), box(scale, 692, 6, 176, 28)
        .with_custom_background(afterhours::Color{31, 122, 168, 200})
        .with_corner_radius(14 * scale).with_ignore_pointer_events().with_debug_name("rb_players_chip"));
    div(context, mk(root.ent()), box(scale, 876, 6, 172, 28)
        .with_custom_background(afterhours::Color{31, 122, 168, 200})
        .with_corner_radius(14 * scale).with_ignore_pointer_events().with_debug_name("rb_coins_chip"));
    div(context, mk(root.ent()), box(scale, 900, 13, 14, 14)
        .with_ignore_pointer_events().with_debug_name("rb_coins_dot")
        .with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_circle(static_cast<int>(r.x + r.width / 2),
                                  static_cast<int>(r.y + r.height / 2), r.width / 2,
                                  afterhours::Color{255, 203, 46, 255});
        }));
    label(6, "Players  " + std::to_string(player_count), 704, 4, 170, 32, 20, white, "rb_player_count");
    label(7, "Coins  " + std::to_string(coins), 922, 4, 130, 32, 20, white, "rb_coins");
    auto promo = button(context, mk(root.ent()), box(scale, 938, 72, 310, 138)
        .with_custom_background(afterhours::Color{165, 124, 29, 235})
        .with_border(white, 2 * scale).with_corner_radius(12 * scale)
        .with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(
              {r.x + 6 * scale, r.y + 6 * scale, r.width - 12 * scale, r.height - 12 * scale},
              .2f, 24, 1 * scale, afterhours::Color{255, 236, 150, 255});
          draw_lightning(r, scale);
        }).with_debug_name("rb_promo"));
    label(11, "Get more bandits!", 956, 82, 260, 32, 26, white, "rb_promo_title");
    label(12, supporter_selected ? "Cosmetic pack preview" : "Buy the supporter pack",
          956, 118, 240, 24, 15, white, "rb_promo_description");
    label(120, supporter_selected ? "" : "and show off!",
          956, 141, 240, 24, 15, white, "rb_promo_description2");
    label(13, supporter_selected ? "No purchase in this demo" : "View pack  >", 956, 163,
          250, 28, 20, white, "rb_promo_action");
    if (promo) {
      supporter_selected = !supporter_selected;
      footer_message = supporter_selected ? "Supporter pack preview" : menu_hints[selected_item];
    }
    if (menu_open) {
      const std::array<float, 7> row_y{300, 340, 380, 420, 460, 500, 560};
      for (size_t i = 0; i < menu_items.size(); ++i) {
        const bool selected = i == selected_item;
        auto item = button(context, mk(root.ent(), 20 + static_cast<int>(i)),
            box(scale, 48, row_y[i], 354, 42).with_label(menu_items[i])
                .with_font("FredokaMockBold", pixels((i == 6 ? 25 : 32) * scale))
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
        if (i == 6) menu_open = false;
      }
      div(context, mk(root.ent()), box(scale, 70, 550, 310, 1)
          .with_custom_background(afterhours::Color{142, 109, 30, 135}));
    } else {
      if (button(context, mk(root.ent()), box(scale, 48, 324, 354, 48)
          .with_label("OPEN MENU").with_font("FredokaMockBold", pixels(28 * scale))
          .with_custom_background(cyan).with_corner_radius(12 * scale)
          .with_debug_name("rb_open_menu"))) {
        menu_open = true;
        selected_item = 0;
        footer_message = menu_hints[0];
      }
    }
    const std::array<float, 4> marker_x{708, 936, 1070, 1140};
    const std::array<float, 4> marker_y{564, 546, 558, 541};
    div(context, mk(root.ent()), box(scale, marker_x[character_index] - 49,
        marker_y[character_index], 98, 12)
        .with_on_draw_fg([scale, cyan](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(r, 1, 32, 3 * scale, cyan);
        }).with_ignore_pointer_events().with_debug_name("rb_selected_marker"));
    div(context, mk(root.ent()), box(scale, 468, 608, 764, 60)
        .with_custom_background(afterhours::Color{137, 99, 26, 245})
        .with_corner_radius(12 * scale).with_debug_name("rb_character_panel"));
    auto character_button = [&](int id, const char *caption, float x, int direction,
                                const std::string &name) {
      if (!button(context, mk(root.ent(), id), box(scale, x, 622, 48, 32)
          .with_label(caption).with_font("FredokaMockBold", pixels(18 * scale))
          .with_custom_background(dark).with_border(white, 1 * scale)
          .with_corner_radius(4 * scale).with_custom_text_color(white)
          .with_debug_name(name))) return;
      character_index = (character_index + (direction > 0 ? 1 : character_names.size() - 1)) % character_names.size();
      footer_message = "Selected " + character_names[character_index];
    };
    character_button(60, "LB", 486, -1, "rb_character_prev");
    character_button(61, "RB", 1166, 1, "rb_character_next");
    label(62, character_names[character_index], 640, 614, 420, 28, 24, white,
          "rb_character_name", TextAlignment::Center);
    label(63, "FAVOURITE", 640, 645, 420, 18, 13, afterhours::Color{255, 237, 160, 255},
          "rb_favourite", TextAlignment::Center);
    label(64, std::to_string(character_index + 1) + " of 4", 1080, 622, 72, 32,
          20, white, "rb_character_count", TextAlignment::Center);
    label(70, "1.6.0 / RUBBER BANDITS", 32, 645, 260, 24, 14, dark, "rb_build");
    div(context, mk(root.ent()), box(scale, 0, 678, 1280, 42)
        .with_custom_background(cyan).with_debug_name("rb_footer"));
    label(72, footer_message, 32, 686, 830, 26, 21, dark, "rb_footer_text");
    div(context, mk(root.ent()), box(scale, 922, 684, 52, 26)
        .with_label("Enter").with_font("FredokaMockBold", pixels(18 * scale))
        .with_custom_background(dark).with_custom_text_color(white)
        .with_alignment(TextAlignment::Center).with_corner_radius(4 * scale)
        .with_ignore_pointer_events().with_debug_name("rb_enter_keycap"));
    label(74, "Select", 980, 686, 70, 26, 18, dark, "rb_select_hint");
    div(context, mk(root.ent()), box(scale, 1056, 684, 46, 26)
        .with_label("Esc").with_font("FredokaMockBold", pixels(18 * scale))
        .with_custom_background(dark).with_custom_text_color(white)
        .with_alignment(TextAlignment::Center).with_corner_radius(4 * scale)
        .with_ignore_pointer_events().with_debug_name("rb_esc_keycap"));
    label(75, "Menu", 1108, 686, 140, 26, 18, dark, "rb_menu_hint");
  }
};

REGISTER_EXAMPLE_SCREEN(rubber_bandits_menu, "Game Mockups",
                        "Cartoony party game menu (Rubber Bandits style)",
                        RubberBanditsMenuScreen)
