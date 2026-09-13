#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ShopInterfaceScreen : ScreenSystem<UIContext<InputAction>> {
  bool closed = false;
  size_t selected_tab = 0;
  size_t selected_item = 1;
  int player_gold = 3750;
  std::string status = "5 items in your basket";

  struct ShopItem {
    std::string name;
    std::string category;
    std::string icon;
    int price;
    int stock;
    std::string description;
    afterhours::Color icon_color;
  };

  struct CartItem {
    size_t item_index;
    int quantity;
  };

  std::vector<ShopItem> buy_items = {
      {"Iron Sword",
       "Weapon",
       "",
       250,
       5,
       "A sturdy blade for beginners.",
       {116, 84, 47, 255}},
      {"Health Potion",
       "Consumable",
       "p",
       50,
       20,
       "Restores 50 HP instantly.",
       {223, 73, 91, 255}},
      {"Mana Potion",
       "Consumable",
       "b",
       75,
       15,
       "Restores 30 MP instantly.",
       {80, 125, 190, 255}},
      {"Leather Armor",
       "Armor",
       "",
       180,
       8,
       "Light protection for adventurers.",
       {141, 100, 56, 255}},
      {"Fire Scroll",
       "Magic",
       "",
       320,
       3,
       "Casts Fireball. Deals 25 damage.",
       {116, 84, 47, 255}},
      {"Torch",
       "Tool",
       "",
       15,
       50,
       "Lights dark areas for 10 minutes.",
       {119, 82, 41, 255}},
      {"Antidote",
       "Consumable",
       "g",
       35,
       12,
       "Cures poison status.",
       {112, 166, 92, 255}},
      {"Steel Shield",
       "Armor",
       "",
       400,
       2,
       "Blocks 15% incoming damage.",
       {124, 92, 51, 255}},
  };

  std::vector<CartItem> cart = {{1, 3}, {5, 2}};

  const afterhours::Color bg{48, 42, 32, 255};
  const afterhours::Color parchment{224, 203, 161, 255};
  const afterhours::Color parchment_light{238, 218, 178, 255};
  const afterhours::Color paper_hi{249, 232, 188, 255};
  const afterhours::Color wood{128, 91, 46, 255};
  const afterhours::Color wood_dark{77, 50, 26, 255};
  const afterhours::Color ink{73, 51, 33, 255};
  const afterhours::Color muted{119, 94, 61, 255};
  const afterhours::Color gold{196, 164, 95, 255};
  const afterhours::Color red{170, 77, 44, 255};
  const afterhours::Color cream{238, 219, 181, 255};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0.f);
  }

  std::string format_gold(int amount) const {
    std::string s = std::to_string(std::max(0, amount));
    std::string result;
    int count = 0;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
      if (count > 0 && count % 3 == 0)
        result = "," + result;
      result = s[static_cast<size_t>(i)] + result;
      ++count;
    }
    return result;
  }

  int get_cart_total() const {
    int total = 0;
    for (const auto &ci : cart)
      total += buy_items[ci.item_index].price * ci.quantity;
    return total;
  }

  int get_cart_item_count() const {
    int count = 0;
    for (const auto &ci : cart)
      count += ci.quantity;
    return count;
  }

  void add_selected_to_cart() {
    if (selected_tab != 0) {
      status = "Selling is unavailable in this demo.";
      return;
    }
    if (selected_item >= buy_items.size())
      return;
    auto &item = buy_items[selected_item];
    if (item.stock <= 0)
      return;
    for (auto &ci : cart) {
      if (ci.item_index == selected_item) {
        ++ci.quantity;
        --item.stock;
        status = "Added " + item.name;
        return;
      }
    }
    cart.push_back({selected_item, 1});
    --item.stock;
    status = "Added " + item.name;
  }

  void remove_selected_from_cart() {
    auto it =
        std::find_if(cart.begin(), cart.end(), [this](const CartItem &item) {
          return item.item_index == selected_item;
        });
    if (it == cart.end()) {
      status = "This item is not in your basket.";
      return;
    }
    ++buy_items[selected_item].stock;
    status = "Removed " + buy_items[selected_item].name;
    if (--it->quantity == 0)
      cart.erase(it);
  }

  void purchase_cart() {
    const int total = get_cart_total();
    if (total <= 0) {
      status = "Your basket is empty.";
      return;
    }
    if (total > player_gold) {
      status = "Not enough gold.";
      return;
    }
    player_gold -= total;
    cart.clear();
    status = "Thank you, adventurer!";
  }

  static void draw_shop_background(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{48, 42, 32, 255});
    for (int y = 0; y < static_cast<int>(r.height); y += 12) {
      const unsigned char a = static_cast<unsigned char>(28 + (y % 24));
      afterhours::draw_rectangle(
          {r.x, r.y + static_cast<float>(y), r.width, 7.f},
          afterhours::Color{26, 22, 17, a});
    }
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{73, 62, 47, 70},
                                          afterhours::Color{20, 18, 14, 110});

  }

  static void draw_parchment(RectangleType r, afterhours::Color fill) {
    afterhours::draw_rectangle(r, afterhours::Color{46, 28, 16, 255});
    afterhours::draw_rectangle(
        {r.x + 4.f, r.y + 4.f, r.width - 8.f, r.height - 8.f}, fill);
    afterhours::draw_rectangle_outline(
        {r.x + 4.f, r.y + 4.f, r.width - 8.f, r.height - 8.f},
        afterhours::Color{247, 225, 181, 255}, 2.f);
    afterhours::draw_rectangle_outline(r, afterhours::Color{155, 121, 80, 255},
                                       3.f);
  }

  static void draw_wood_button(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{82, 52, 26, 255});
    afterhours::draw_rectangle(
        {r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height - 4.f},
        afterhours::Color{121, 83, 39, 255});
    afterhours::draw_rectangle_outline(
        {r.x + 4.f, r.y + 4.f, r.width - 8.f, r.height - 8.f},
        afterhours::Color{183, 139, 76, 255}, 1.5f);
  }

  static void draw_potion(RectangleType r, afterhours::Color liquid) {
    afterhours::draw_rectangle_rounded(
        {r.x + r.width * .12f, r.y + r.height * .88f, r.width * .76f, r.height * .1f},
        1, 16, afterhours::Color{113, 78, 38, 55}, RoundedCorners().all_round());
    const float cx = r.x + r.width * .5f;
    const float top = r.y + r.height * .06f;
    const float neck_w = r.width * .23f;
    const float neck_h = r.height * .30f;
    const auto glass = afterhours::Color{244, 226, 187, 255};
    const auto edge = afterhours::Color{95, 67, 40, 255};
    afterhours::draw_rectangle({cx - neck_w * .5f, top + 4.f, neck_w, neck_h},
                               glass);
    afterhours::draw_rectangle_outline(
        {cx - neck_w * .65f, top, neck_w * 1.3f, 10.f}, edge, 3.f);
    const Vector2Type left_shoulder{cx - r.width * .12f, top + neck_h};
    const Vector2Type right_shoulder{cx + r.width * .12f, top + neck_h};
    const Vector2Type left_base{cx - r.width * .36f, r.y + r.height * .76f};
    const Vector2Type right_base{cx + r.width * .36f, r.y + r.height * .76f};
    const Vector2Type bottom{cx, r.y + r.height * .88f};
    afterhours::draw_triangle(left_shoulder, left_base, right_base, glass);
    afterhours::draw_triangle(left_base, bottom, right_base, glass);
    afterhours::draw_triangle(
        {cx - r.width * .30f, r.y + r.height * .58f}, bottom,
        {cx + r.width * .31f, r.y + r.height * .58f}, liquid);
    afterhours::draw_line_ex({cx - neck_w * .5f, top + 10.f}, left_shoulder,
                             3.f, edge);
    afterhours::draw_line_ex({cx + neck_w * .5f, top + 10.f}, right_shoulder,
                             3.f, edge);
    afterhours::draw_line_ex(left_shoulder, left_base, 3.f, edge);
    afterhours::draw_line_ex(right_shoulder, right_base, 3.f, edge);
    afterhours::draw_line_ex(left_base, bottom, 3.f, edge);
    afterhours::draw_line_ex(bottom, right_base, 3.f, edge);
    afterhours::draw_line_ex({cx - r.width * .25f, r.y + r.height * .58f},
                             {cx + r.width * .25f, r.y + r.height * .58f}, 2.f,
                             edge);
    afterhours::draw_circle(static_cast<int>(cx + r.width * .12f),
                            static_cast<int>(r.y + r.height * .48f),
                            r.width * .045f,
                            afterhours::Color{255, 238, 205, 255});
  }

  static void draw_item_art(RectangleType r, size_t index,
                            afterhours::Color color) {
    if (index == 1 || index == 2 || index == 6) {
      draw_potion(r, color);
      return;
    }
    const afterhours::Color dark{92, 63, 35, 255};
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    if (index == 0) {
      afterhours::draw_line_ex({cx - r.width * .22f, cy + r.height * .22f},
                               {cx + r.width * .18f, cy - r.height * .25f}, r.width * .12f,
                               dark);
      afterhours::draw_line_ex({cx - r.width * .05f, cy + r.height * .04f},
                               {cx + r.width * .2f, cy + r.height * .25f}, r.width * .10f,
                               dark);
      afterhours::draw_line_ex({cx - r.width * .20f, cy + r.height * .25f},
                               {cx - r.width * .28f, cy + r.height * .34f}, r.width * .12f,
                               dark);
    } else if (index == 3) {
      afterhours::draw_rectangle({cx - r.width * .23f, cy - r.height * .25f,
                                  r.width * .46f, r.height * .46f},
                                 color);
      afterhours::draw_rectangle_outline({cx - r.width * .23f,
                                          cy - r.height * .25f, r.width * .46f,
                                          r.height * .46f},
                                         dark, 2.f);
      afterhours::draw_rectangle({cx - r.width * .13f, cy + r.height * .21f,
                                  r.width * .26f, r.height * .12f},
                                 dark);
    } else if (index == 4) {
      afterhours::draw_rectangle({cx - r.width * .26f, cy - r.height * .30f,
                                  r.width * .52f, r.height * .60f},
                                 afterhours::Color{238, 219, 181, 255});
      for (int i = 0; i < 5; ++i) {
        const float y =
            cy - r.height * .20f + static_cast<float>(i) * r.height * .1f;
        afterhours::draw_line_ex({cx - r.width * .20f, y},
                                 {cx + r.width * .20f, y}, 1.5f, dark);
      }
      afterhours::draw_rectangle_outline({cx - r.width * .26f,
                                          cy - r.height * .30f, r.width * .52f,
                                          r.height * .60f},
                                         dark, 1.5f);
    } else if (index == 5) {
      afterhours::draw_line_ex({cx, cy + r.height * .28f},
                               {cx, cy - r.height * .20f}, 5.f, dark);
      afterhours::draw_circle(
          static_cast<int>(cx), static_cast<int>(cy - r.height * .28f),
          r.width * .14f, afterhours::Color{225, 157, 61, 255});
    } else {
      afterhours::draw_poly({cx, cy}, 5, r.width * .34f, -18.f, color);
      afterhours::draw_poly_lines_ex({cx, cy}, 5, r.width * .34f, -18.f, 2.f,
                                     dark);
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float screen_w =
        context.screen_width > 0.f
            ? context.screen_width
            : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h =
        context.screen_height > 0.f
            ? context.screen_height
            : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);

    if (!closed && context.pressed(InputAction::WidgetDown))
      selected_item = (selected_item + 1) % buy_items.size();
    if (!closed && context.pressed(InputAction::WidgetUp))
      selected_item = (selected_item + buy_items.size() - 1) % buy_items.size();
    if (context.pressed(InputAction::WidgetRight))
      selected_tab = 1;
    if (context.pressed(InputAction::WidgetLeft))
      selected_tab = 0;
    if (!closed && afterhours::input::is_key_pressed(raylib::KEY_X))
      add_selected_to_cart();
    if (!closed && afterhours::input::is_key_pressed(raylib::KEY_Y))
      remove_selected_from_cart();
    if (afterhours::input::is_key_pressed(raylib::KEY_ESCAPE) ||
        afterhours::input::is_key_pressed(raylib::KEY_B))
      closed = !closed;

    Theme theme;
    theme.font = ink;
    theme.darkfont = cream;
    theme.font_muted = muted;
    theme.background = bg;
    theme.surface = parchment;
    theme.primary = wood;
    theme.secondary = gold;
    theme.accent = gold;
    theme.error = red;
    theme.roundness = 0.f;
    theme.segments = 8;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("Garamond", pixels(17.f * scale));

    auto root = div(
        context, mk(entity, 0),
        box(scale, 0, 0, 1280, 720)
            .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
            .with_on_draw_bg([](RectangleType r) { draw_shop_background(r); })
            .with_debug_name("shop_root"));
    if (closed) {
      div(context, mk(root.ent(), 990),
          box(scale, 390, 260, 500, 80)
              .with_label("Shop closed")
              .with_font("Garamond", pixels(42.f * scale))
              .with_custom_text_color(cream)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 991),
                 box(scale, 490, 365, 300, 60)
                     .with_label("Return to shop")
                     .with_custom_background(wood)
                     .with_font("Garamond", pixels(28.f * scale))
                     .with_custom_text_color(cream)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("shop_reopen")))
        closed = false;
      return;
    }

    div(context, mk(root.ent(), 1),
        box(scale, 38, 24, 560, 28)
            .with_label("WILLOWBROOK - MARKET DISTRICT")
            .with_font("Garamond", pixels(17.f * scale))
            .with_letter_spacing(0)
            .with_custom_text_color(gold)
            .with_debug_name("shop_location"));
    div(context, mk(root.ent(), 2),
        box(scale, 1156, 22, 86, 28)
            .with_label("Day 12")
            .with_font("Garamond", pixels(14.f * scale))
            .with_custom_text_color(afterhours::Color{225, 193, 107, 255})
            .with_alignment(TextAlignment::Right));

    div(context, mk(root.ent(), 9), box(scale, 1126, 27, 22, 22)
        .with_on_draw_fg([scale, this](RectangleType r) {
          const Vector2Type center{r.x + r.width / 2, r.y + r.height / 2};
          afterhours::draw_circle(static_cast<int>(center.x), static_cast<int>(center.y), 5 * scale, gold);
          for (int i = 0; i < 8; ++i) {
            const float a = static_cast<float>(i) * .7853982f;
            afterhours::draw_line_ex({center.x + std::cos(a) * 8 * scale, center.y + std::sin(a) * 8 * scale},
                {center.x + std::cos(a) * 11 * scale, center.y + std::sin(a) * 11 * scale}, scale, gold);
          }
        }).with_ignore_pointer_events());
    for (int i = 0; i < 2; ++i)
      div(context, mk(root.ent(), 15 + i), box(scale, i == 0 ? 40 : 447, 90, 18, 26)
          .with_on_draw_fg([this, scale](RectangleType r) {
            afterhours::draw_line_ex({r.x + r.width / 2, r.y}, {r.x + r.width / 2, r.y + r.height}, scale, gold);
            afterhours::draw_poly({r.x + r.width * .3f, r.y + r.height * .3f}, 4, 5 * scale, 0, gold);
            afterhours::draw_poly({r.x + r.width * .7f, r.y + r.height * .6f}, 4, 5 * scale, 0, gold);
          }).with_ignore_pointer_events());
    div(context, mk(root.ent(), 10),
        box(scale, 65, 73, 420, 58)
            .with_label("General Store")
            .with_font("Garamond", pixels(56.f * scale))
            .with_text_stroke(afterhours::Color{244, 222, 181, 255},
                              .35f * scale)
            .with_custom_text_color(afterhours::Color{244, 222, 181, 255})
            .with_debug_name("shop_title"));
    div(context, mk(root.ent(), 11),
        box(scale, 39, 128, 470, 25)
            .with_label("\"Welcome, adventurer! Browse my finest wares.\"")
            .with_font("Garamond", pixels(21.f * scale))
            .with_custom_text_color(afterhours::Color{212, 190, 154, 255})
            .with_debug_name("shop_quote"));

    div(context, mk(root.ent(), 20),
        box(scale, 1054, 91, 32, 32)
            .with_on_draw_fg([](RectangleType r) {
              afterhours::draw_circle(static_cast<int>(r.x + r.width * .5f),
                                      static_cast<int>(r.y + r.height * .5f),
                                      r.width * .44f,
                                      afterhours::Color{217, 160, 46, 255});
              afterhours::draw_poly(
                  {r.x + r.width * .5f, r.y + r.height * .48f}, 5,
                  r.width * .25f, -18.f, afterhours::Color{255, 235, 153, 255});
            })
            .with_ignore_pointer_events()
            .with_debug_name("shop_coin_icon"));
    div(context, mk(root.ent(), 21),
        box(scale, 1098, 84, 144, 46)
            .with_label(format_gold(player_gold) + " G")
            .with_font("Garamond", pixels(37.f * scale))
            .with_custom_text_color(afterhours::Color{244, 222, 181, 255})
            .with_alignment(TextAlignment::Left)
            .with_debug_name("shop_gold"));
    div(context, mk(root.ent(), 22),
        box(scale, 1098, 127, 144, 25)
            .with_label("YOUR GOLD")
            .with_font("Garamond", pixels(17.f * scale))
            .with_letter_spacing(0)
            .with_custom_text_color(gold));

    div(context, mk(root.ent(), 100),
        box(scale, 38, 163, 750, 500)
            .with_on_draw_bg([fill = parchment](RectangleType r) {
              draw_parchment(r, fill);
            })
            .with_debug_name("shop_wares"));
    div(context, mk(root.ent(), 101),
        box(scale, 42, 167, 742, 52)
            .with_custom_background(afterhours::Color{194, 169, 127, 255})
            .with_ignore_pointer_events());

    const std::array<const char *, 2> tab_labels = {{"Buy", "Sell"}};
    for (size_t i = 0; i < tab_labels.size(); ++i) {
      const bool active = selected_tab == i;
      if (button(context, mk(root.ent(), 110 + static_cast<int>(i)),
                 box(scale, 42 + static_cast<float>(i) * 107.f, 167, 107, 52)
                     .with_label(tab_labels[i])
                     .with_custom_background(
                         active ? parchment_light
                                : afterhours::Color{194, 169, 127, 255})
                     .with_corner_radius(0.f)
                     .with_font("Garamond", pixels(33.f * scale))
                     .with_text_stroke(active ? ink : muted, .35f * scale)
                     .with_custom_text_color(active ? ink : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("shop_tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
      if (active)
        div(context, mk(root.ent(), 114 + static_cast<int>(i)), box(scale, 52 + static_cast<float>(i) * 107, 211, 87, 4).with_custom_background(wood_dark));
    }
    div(context, mk(root.ent(), 115),
        box(scale, 290, 181, 470, 28)
            .with_label(selected_tab == 0 ? "Goods for the road ahead"
                                          : "Selling is unavailable in this demo.")
            .with_font("Garamond", pixels(21.f * scale))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("shop_tab_note"));

    const std::array<const char *, 4> headers = {
        {"ITEM", "TYPE", "STOCK", "PRICE"}};
    const std::array<float, 4> header_x = {{116.f, 382.f, 518.f, 650.f}};
    const std::array<float, 4> header_w = {{245.f, 136.f, 110.f, 100.f}};
    for (size_t i = 0; i < headers.size(); ++i) {
      div(context, mk(root.ent(), 130 + static_cast<int>(i)),
          box(scale, header_x[i], 234, header_w[i], 22)
              .with_label(headers[i])
              .with_font("Garamond", pixels(17.f * scale))
              .with_letter_spacing(0)
              .with_custom_text_color(muted)
              .with_alignment(i >= 2 ? TextAlignment::Right
                                     : TextAlignment::Left));
    }

    for (size_t i = 0; i < buy_items.size(); ++i) {
      auto &item = buy_items[i];
      const bool active = i == selected_item;
      const float y = 255.f + static_cast<float>(i) * 46.f;
      if (button(context, mk(root.ent(), 200 + static_cast<int>(i)),
                 box(scale, 53, y, 720, 46)
                     .with_label("")
                     .with_custom_background(
                         active ? paper_hi : afterhours::Color{0, 0, 0, 0})
                     .with_border(active ? wood : afterhours::Color{0, 0, 0, 0},
                                  active ? 2.f : 0.f)
                     .with_corner_radius(0.f)
                     .with_debug_name("shop_item_" + std::to_string(i)))) {
        selected_item = i;
      }
      if (active)
        div(context, mk(root.ent(), 245 + static_cast<int>(i)), box(scale, 53, y, 4, 46).with_custom_background(wood_dark));
      if (!active) {
        div(context, mk(root.ent(), 250 + static_cast<int>(i)),
            box(scale, 53, y + 45, 720, 1)
                .with_custom_background(afterhours::Color{171, 144, 98, 90})
                .with_ignore_pointer_events());
      }
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 66, y + 3, 34, 38)
              .with_label("")
              .with_font(i == 1 || i == 2 || i == 6 ? "Garamond"
                                                    : "Garamond",
                         pixels(30.f * scale))
              .with_custom_text_color(item.icon_color)
              .with_alignment(TextAlignment::Center)
              .with_on_draw_fg(
                  [index = i, color = item.icon_color](RectangleType r) {
                    draw_item_art(r, index, color);
                  })
              .with_ignore_pointer_events()
              .with_debug_name("shop_icon_" + std::to_string(i)));
      div(context, mk(root.ent(), 400 + static_cast<int>(i)),
          box(scale, 116, y + 11, 245, 28)
              .with_label(item.name)
              .with_font("Garamond", pixels(26.f * scale))
              .with_text_stroke(ink, .35f * scale)
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events()
              .with_debug_name("shop_name_" + std::to_string(i)));
      div(context, mk(root.ent(), 500 + static_cast<int>(i)),
          box(scale, 382, y + 13, 136, 24)
              .with_label(item.category)
              .with_font("Garamond", pixels(17.f * scale))
              .with_custom_text_color(muted)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 600 + static_cast<int>(i)),
          box(scale, 518, y + (item.stock <= 3 ? 1 : 12), 110, 24)
              .with_label(std::to_string(item.stock))
              .with_font("Garamond", pixels(17.f * scale))
              .with_custom_text_color(item.stock <= 3 ? red : muted)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
      if (item.stock <= 3)
        div(context, mk(root.ent(), 650 + static_cast<int>(i)), box(scale, 518, y + 25, 110, 18)
            .with_label("Low stock").with_font("Garamond", pixels(12 * scale))
            .with_custom_text_color(red).with_alignment(TextAlignment::Right).with_ignore_pointer_events());
      div(context, mk(root.ent(), 700 + static_cast<int>(i)),
          box(scale, 650, y + 10, 100, 29)
              .with_label(std::to_string(item.price) + " G")
              .with_font("Garamond", pixels(26.f * scale))
              .with_text_stroke(ink, .35f * scale)
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 780),
        box(scale, 73, 633, 680, 20)
            .with_label(
                "All goods inspected by the Willowbrook Merchants' Guild.")
            .with_font("Garamond", pixels(16.f * scale))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center));

    auto &sel = buy_items[selected_item];
    div(context, mk(root.ent(), 800),
        box(scale, 812, 163, 430, 209)
            .with_on_draw_bg([fill = parchment](RectangleType r) {
              draw_parchment(r, fill);
            })
            .with_debug_name("shop_detail"));
    div(context, mk(root.ent(), 801),
        box(scale, 847, 193, 82, 98)
            .with_on_draw_fg([index = selected_item,
                              color = sel.icon_color](RectangleType r) {
              draw_item_art(r, index, color);
            })
            .with_ignore_pointer_events()
            .with_debug_name("shop_detail_art"));
    div(context, mk(root.ent(), 802),
        box(scale, 951, 180, 266, 24)
            .with_label(sel.category == "Consumable" ? "CONSUMABLE" : sel.category)
            .with_font("Garamond", pixels(17.f * scale))
            .with_letter_spacing(0)
            .with_custom_text_color(muted)
            .with_debug_name("shop_detail_category"));
    div(context, mk(root.ent(), 803),
        box(scale, 951, 208, 266, 38)
            .with_label(sel.name)
            .with_font("Garamond", pixels(32.f * scale)).with_text_stroke(ink, .4f * scale)
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_name"));
    div(context, mk(root.ent(), 804),
        box(scale, 951, 247, 266, 40)
            .with_label(sel.description)
            .with_font("Garamond", pixels(17.f * scale))
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_description"));
    div(context, mk(root.ent(), 805),
        box(scale, 951, 289, 266, 25)
            .with_label("Price per item: " + std::to_string(sel.price) + " G")
            .with_font("Garamond", pixels(18.f * scale))
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_price"));
    if (button(
            context, mk(root.ent(), 806),
            box(scale, 831, 319, 392, 38)
                .with_label(selected_tab == 0 ? "Add to basket" : "Sell preview only")
                .with_on_draw_bg([](RectangleType r) { draw_wood_button(r); })
                .with_font("Garamond", pixels(25.f * scale))
                .with_custom_text_color(cream)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("shop_add_cart"))) {
      add_selected_to_cart();
    }

    div(context, mk(root.ent(), 900), box(scale, 812, 386, 430, 277)
        .with_on_draw_bg([fill = parchment](RectangleType r) { draw_parchment(r, fill); })
        .with_debug_name("shop_basket"));
    div(context, mk(root.ent(), 901), box(scale, 838, 402, 202, 34)
        .with_label("Your basket").with_font("Garamond", pixels(32 * scale))
        .with_text_stroke(ink, .35f * scale).with_custom_text_color(ink)
        .with_debug_name("shop_basket_title"));
    div(context, mk(root.ent(), 902), box(scale, 1050, 408, 164, 26)
        .with_label(std::to_string(get_cart_item_count()) + " items")
        .with_font("Garamond", pixels(17 * scale)).with_custom_text_color(muted)
        .with_alignment(TextAlignment::Right).with_debug_name("shop_cart_count"));
    div(context, mk(root.ent(), 903), box(scale, 1060, 435, 128, 20)
        .with_label("Line total").with_font("Garamond", pixels(14 * scale))
        .with_custom_text_color(muted).with_alignment(TextAlignment::Right));
    auto basket_rows = div(context, mk(root.ent(), 904), box(scale, 838, 457, 378, 94)
        .with_flex_direction(FlexDirection::Column).with_overflow(Overflow::Scroll, Axis::Y)
        .with_debug_name("shop_basket_rows"));
    int basket_action = 0;
    size_t action_item = 0;
    for (const auto &ci : cart) {
      const auto &item = buy_items[ci.item_index];
      const int id = static_cast<int>(ci.item_index);
      auto row = div(context, mk(basket_rows.ent(), id), ComponentConfig{}
          .with_size({pixels(378 * scale), pixels(38 * scale)})
          .with_background(Theme::Usage::None).with_corner_radius(0));
      div(context, mk(row.ent(), 0), box(scale, 0, 4, 150, 28)
          .with_label(item.name).with_font("Garamond", pixels(23 * scale))
          .with_custom_text_color(ink).with_debug_name("shop_cart_item_" + std::to_string(id)));
      div(context, mk(row.ent(), 1), box(scale, 182, 4, 28, 28)
          .with_label(std::to_string(ci.quantity)).with_font("Garamond", pixels(17 * scale))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Center));
      div(context, mk(row.ent(), 2), box(scale, 242, 4, 100, 28)
          .with_label(std::to_string(item.price * ci.quantity) + " G")
          .with_font("Garamond", pixels(23 * scale)).with_text_stroke(ink, .4f * scale)
          .with_custom_text_color(ink).with_alignment(TextAlignment::Right));
      const auto quantity_button = [&](int key, float x, const char *caption,
                                       const std::string &name, int action) {
        if (!button(context, mk(row.ent(), key), box(scale, x, 4, 28, 28)
            .with_label(caption).with_font("Garamond", pixels(20 * scale))
            .with_custom_background(parchment_light).with_border(wood, scale)
            .with_corner_radius(3 * scale).with_custom_text_color(ink)
            .with_debug_name(name + std::to_string(id)))) return;
        basket_action = action;
        action_item = ci.item_index;
      };
      quantity_button(3, 154, "-", "shop_minus_", -1);
      quantity_button(4, 210, "+", "shop_plus_", 1);
      quantity_button(5, 350, "x", "shop_remove_", 2);
    }
    if (cart.empty())
      div(context, mk(basket_rows.ent(), 99), ComponentConfig{}
          .with_size({pixels(378 * scale), pixels(38 * scale)}).with_label("Your basket is empty.")
          .with_font("Garamond", pixels(18 * scale)).with_custom_text_color(muted));
    if (basket_action != 0) selected_item = action_item;
    if (basket_action == 1) add_selected_to_cart();
    if (basket_action == -1) remove_selected_from_cart();
    if (basket_action == 2) {
      auto it = std::find_if(cart.begin(), cart.end(), [action_item](const CartItem &item) {
        return item.item_index == action_item;
      });
      if (it != cart.end()) {
        buy_items[action_item].stock += it->quantity;
        status = "Removed " + buy_items[action_item].name;
        cart.erase(it);
      }
    }
    div(context, mk(root.ent(), 930), box(scale, 838, 557, 378, 1)
        .with_custom_background(afterhours::Color{169, 139, 98, 255}));
    div(context, mk(root.ent(), 931), box(scale, 838, 562, 92, 31)
        .with_label("Total").with_font("Garamond", pixels(30 * scale))
        .with_text_stroke(ink, .35f * scale).with_custom_text_color(ink));
    div(context, mk(root.ent(), 932), box(scale, 1090, 562, 126, 31)
        .with_label(std::to_string(get_cart_total()) + " G")
        .with_font("Garamond", pixels(30 * scale)).with_text_stroke(ink, .4f * scale)
        .with_custom_text_color(ink).with_alignment(TextAlignment::Right).with_debug_name("shop_total"));
    div(context, mk(root.ent(), 933), box(scale, 838, 590, 378, 24)
        .with_label(get_cart_total() > player_gold ? "Not enough gold for this basket" :
            "Gold remaining: " + format_gold(player_gold - get_cart_total()) + " G")
        .with_font("Garamond", pixels(16 * scale)).with_custom_text_color(muted)
        .with_debug_name("shop_remaining"));
    if (button(context, mk(root.ent(), 934), box(scale, 838, 619, 378, 30)
        .with_label(cart.empty() ? "Basket empty" : "Purchase / " + std::to_string(get_cart_total()) + " G")
        .with_custom_background(wood_dark).with_border(gold, 2 * scale)
        .with_font("Garamond", pixels(25 * scale)).with_custom_text_color(cream)
        .with_alignment(TextAlignment::Center).with_debug_name("shop_purchase"))) purchase_cart();

    div(context, mk(root.ent(), 980),
        box(scale, 38, 681, 474, 30)
            .with_label("[Enter] Select   [X] Add   [Y] Remove")
            .with_font("Garamond", pixels(17.f * scale))
            .with_custom_text_color(afterhours::Color{196, 179, 151, 255})
            .with_debug_name("shop_footer_left"));
    if (button(
            context, mk(root.ent(), 981),
            box(scale, 1050, 681, 190, 30)
                .with_label("[B] Leave shop")
                .with_font("Garamond", pixels(17.f * scale))
                .with_custom_text_color(afterhours::Color{196, 179, 151, 255})
                .with_alignment(TextAlignment::Right)
                .with_debug_name("shop_leave")))
      closed = true;
    div(context, mk(root.ent(), 982),
        box(scale, 504, 681, 536, 30)
            .with_label(status)
            .with_font("Garamond", pixels(17.f * scale))
            .with_custom_text_color(afterhours::Color{212, 190, 154, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("shop_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(shop_interface, "Game Mockups",
                        "RPG general store with buy/sell and cart",
                        ShopInterfaceScreen)
