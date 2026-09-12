#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ShopInterfaceScreen : ScreenSystem<UIContext<InputAction>> {
  bool closed = false;
  size_t selected_tab = 0;
  size_t selected_item = 1;
  int player_gold = 3750;
  std::string status = "Welcome, adventurer!";

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
    afterhours::draw_circle(static_cast<int>(r.x + r.width * .42f),
                            static_cast<int>(r.y + r.height * .18f),
                            r.width * .42f,
                            afterhours::Color{118, 100, 67, 35});
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
                               {cx + r.width * .18f, cy - r.height * .25f}, 3.f,
                               dark);
      afterhours::draw_line_ex({cx - r.width * .05f, cy + r.height * .04f},
                               {cx + r.width * .2f, cy + r.height * .25f}, 2.5f,
                               dark);
      afterhours::draw_line_ex({cx - r.width * .20f, cy + r.height * .25f},
                               {cx - r.width * .28f, cy + r.height * .34f}, 3.f,
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

    if (context.pressed(InputAction::WidgetDown))
      selected_item = (selected_item + 1) % buy_items.size();
    if (context.pressed(InputAction::WidgetUp))
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
    UIStylingDefaults::get().set_default_font("Garamond", h720(18.f));

    auto root = div(
        context, mk(entity, 0),
        box(scale, 0, 0, 1280, 720)
            .with_on_draw_bg([](RectangleType r) { draw_shop_background(r); })
            .with_debug_name("shop_root"));
    if (closed) {
      div(context, mk(root.ent(), 990),
          box(scale, 390, 260, 500, 80)
              .with_label("Shop closed")
              .with_font("Garamond", h720(42.f))
              .with_custom_text_color(cream)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 991),
                 box(scale, 490, 365, 300, 60)
                     .with_label("Return to shop")
                     .with_custom_background(wood)
                     .with_font("Garamond", h720(28.f))
                     .with_custom_text_color(cream)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("shop_reopen")))
        closed = false;
      return;
    }

    div(context, mk(root.ent(), 1),
        box(scale, 38, 24, 360, 22)
            .with_label("WILLOWBROOK - MARKET DISTRICT")
            .with_font("Archivo", h720(14.f))
            .with_letter_spacing(2.f * scale)
            .with_custom_text_color(gold)
            .with_debug_name("shop_location"));
    div(context, mk(root.ent(), 2),
        box(scale, 1172, 24, 70, 22)
            .with_label("Day 12  *")
            .with_font("Archivo@bold", h720(14.f))
            .with_custom_text_color(afterhours::Color{225, 193, 107, 255})
            .with_alignment(TextAlignment::Right));

    div(context, mk(root.ent(), 10),
        box(scale, 39, 73, 430, 58)
            .with_label("*  General Store  *")
            .with_font("Garamond", h720(56.f))
            .with_text_stroke(afterhours::Color{244, 222, 181, 255},
                              .35f * scale)
            .with_custom_text_color(afterhours::Color{244, 222, 181, 255})
            .with_debug_name("shop_title"));
    div(context, mk(root.ent(), 11),
        box(scale, 39, 128, 470, 25)
            .with_label("\"Welcome, adventurer! Browse my finest wares.\"")
            .with_font("Garamond", h720(21.f))
            .with_custom_text_color(afterhours::Color{212, 190, 154, 255})
            .with_debug_name("shop_quote"));

    div(context, mk(root.ent(), 20),
        box(scale, 1081, 91, 32, 32)
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
        box(scale, 1122, 84, 118, 46)
            .with_label(format_gold(player_gold))
            .with_font("Garamond", h720(43.f))
            .with_custom_text_color(afterhours::Color{244, 222, 181, 255})
            .with_alignment(TextAlignment::Left)
            .with_debug_name("shop_gold"));
    div(context, mk(root.ent(), 22),
        box(scale, 1121, 122, 100, 15)
            .with_label("YOUR GOLD")
            .with_font("Archivo", h720(14.f))
            .with_letter_spacing(2.f * scale)
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
                     .with_font("Garamond", h720(33.f))
                     .with_text_stroke(active ? ink : muted, .35f * scale)
                     .with_custom_text_color(active ? ink : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("shop_tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }
    div(context, mk(root.ent(), 115),
        box(scale, 592, 184, 175, 24)
            .with_label(selected_tab == 0 ? "Goods for the road ahead"
                                          : "Fair prices for spare gear")
            .with_font("Garamond", h720(21.f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("shop_tab_note"));

    const std::array<const char *, 4> headers = {
        {"ITEM", "TYPE", "STOCK", "PRICE"}};
    const std::array<float, 4> header_x = {{116.f, 440.f, 566.f, 671.f}};
    const std::array<float, 4> header_w = {{170.f, 80.f, 75.f, 80.f}};
    for (size_t i = 0; i < headers.size(); ++i) {
      div(context, mk(root.ent(), 130 + static_cast<int>(i)),
          box(scale, header_x[i], 234, header_w[i], 18)
              .with_label(headers[i])
              .with_font("Archivo", h720(14.f))
              .with_letter_spacing(1.5f * scale)
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
      if (!active) {
        div(context, mk(root.ent(), 250 + static_cast<int>(i)),
            box(scale, 53, y + 45, 720, 1)
                .with_custom_background(afterhours::Color{171, 144, 98, 90})
                .with_ignore_pointer_events());
      }
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 68, y + 8, 28, 29)
              .with_label("")
              .with_font(i == 1 || i == 2 || i == 6 ? "Archivo@bold"
                                                    : "Garamond",
                         h720(30.f))
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
              .with_font("Garamond", h720(26.f))
              .with_text_stroke(ink, .35f * scale)
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events()
              .with_debug_name("shop_name_" + std::to_string(i)));
      div(context, mk(root.ent(), 500 + static_cast<int>(i)),
          box(scale, 382, y + 13, 118, 24)
              .with_label(item.category)
              .with_font("Garamond", h720(18.f))
              .with_custom_text_color(muted)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 600 + static_cast<int>(i)),
          box(scale, 506, y + 13, 88, 24)
              .with_label(item.stock <= 3 ? std::to_string(item.stock) + " left"
                                          : std::to_string(item.stock))
              .with_font("Garamond", h720(18.f))
              .with_custom_text_color(item.stock <= 3 ? red : muted)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 700 + static_cast<int>(i)),
          box(scale, 620, y + 10, 86, 29)
              .with_label(std::to_string(item.price) + " G")
              .with_font("Garamond", h720(26.f))
              .with_text_stroke(ink, .35f * scale)
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 780),
        box(scale, 205, 640, 440, 22)
            .with_label(
                "All goods inspected by the Willowbrook Merchants' Guild.")
            .with_font("Garamond", h720(16.f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center));

    auto &sel = buy_items[selected_item];
    div(context, mk(root.ent(), 800),
        box(scale, 812, 163, 430, 230)
            .with_on_draw_bg([fill = parchment](RectangleType r) {
              draw_parchment(r, fill);
            })
            .with_debug_name("shop_detail"));
    div(context, mk(root.ent(), 801),
        box(scale, 849, 198, 82, 100)
            .with_on_draw_fg([index = selected_item,
                              color = sel.icon_color](RectangleType r) {
              draw_item_art(r, index, color);
            })
            .with_ignore_pointer_events()
            .with_debug_name("shop_detail_art"));
    div(context, mk(root.ent(), 802),
        box(scale, 961, 197, 170, 18)
            .with_label(sel.category)
            .with_font("Archivo", h720(14.f))
            .with_letter_spacing(2.f * scale)
            .with_custom_text_color(muted)
            .with_debug_name("shop_detail_category"));
    div(context, mk(root.ent(), 803),
        box(scale, 961, 223, 225, 34)
            .with_label(sel.name)
            .with_font("Garamond", h720(32.f))
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_name"));
    div(context, mk(root.ent(), 804),
        box(scale, 961, 262, 230, 26)
            .with_label(sel.description)
            .with_font("Garamond", h720(18.f))
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_description"));
    div(context, mk(root.ent(), 805),
        box(scale, 961, 294, 120, 31)
            .with_label(std::to_string(sel.price) + " G")
            .with_font("Garamond", h720(27.f))
            .with_custom_text_color(ink)
            .with_debug_name("shop_detail_price"));
    if (button(
            context, mk(root.ent(), 806),
            box(scale, 831, 327, 392, 42)
                .with_label("+ Add to cart")
                .with_on_draw_bg([](RectangleType r) { draw_wood_button(r); })
                .with_font("Garamond", h720(25.f))
                .with_custom_text_color(cream)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("shop_add_cart"))) {
      add_selected_to_cart();
    }

    div(context, mk(root.ent(), 900),
        box(scale, 812, 410, 430, 253)
            .with_on_draw_bg([fill = parchment](RectangleType r) {
              draw_parchment(r, fill);
            })
            .with_debug_name("shop_basket"));
    div(context, mk(root.ent(), 901),
        box(scale, 838, 437, 180, 34)
            .with_label("Your basket")
            .with_font("Garamond", h720(32.f))
            .with_text_stroke(ink, .35f * scale)
            .with_custom_text_color(ink)
            .with_debug_name("shop_basket_title"));
    div(context, mk(root.ent(), 902),
        box(scale, 1178, 444, 38, 18)
            .with_label(std::to_string(get_cart_item_count()) + " items")
            .with_font("Archivo", h720(11.f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("shop_cart_count"));
    div(context, mk(root.ent(), 903),
        box(scale, 838, 474, 378, 1)
            .with_custom_background(afterhours::Color{169, 139, 98, 255}));

    for (size_t i = 0; i < cart.size() && i < 3; ++i) {
      const auto &ci = cart[i];
      const auto &item = buy_items[ci.item_index];
      const float y = 491.f + static_cast<float>(i) * 32.f;
      div(context, mk(root.ent(), 910 + static_cast<int>(i) * 3),
          box(scale, 838, y, 170, 23)
              .with_label(item.name)
              .with_font("Garamond", h720(21.f))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("shop_cart_item_" + std::to_string(i)));
      div(context, mk(root.ent(), 911 + static_cast<int>(i) * 3),
          box(scale, 948, y, 38, 23)
              .with_label("x " + std::to_string(ci.quantity))
              .with_font("Garamond", h720(18.f))
              .with_custom_text_color(muted));
      div(context, mk(root.ent(), 912 + static_cast<int>(i) * 3),
          box(scale, 1144, y, 72, 23)
              .with_label(std::to_string(item.price * ci.quantity) + " G")
              .with_font("Garamond", h720(21.f))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Right));
    }

    div(context, mk(root.ent(), 930),
        box(scale, 838, 549, 378, 1)
            .with_custom_background(afterhours::Color{169, 139, 98, 255}));
    div(context, mk(root.ent(), 931),
        box(scale, 838, 565, 92, 31)
            .with_label("Total")
            .with_font("Garamond", h720(30.f))
            .with_text_stroke(ink, .35f * scale)
            .with_custom_text_color(ink));
    div(context, mk(root.ent(), 932),
        box(scale, 1118, 565, 98, 31)
            .with_label(std::to_string(get_cart_total()) + " G")
            .with_font("Garamond", h720(30.f))
            .with_text_stroke(ink, .35f * scale)
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("shop_total"));
    div(context, mk(root.ent(), 933),
        box(scale, 838, 596, 170, 18)
            .with_label("Gold remaining: " +
                        format_gold(player_gold - get_cart_total()) + " G")
            .with_font("Archivo", h720(14.f))
            .with_custom_text_color(muted)
            .with_debug_name("shop_remaining"));
    if (button(
            context, mk(root.ent(), 934),
            box(scale, 838, 621, 377, 36)
                .with_label(cart.empty() ? "Basket empty" : "Purchase goods")
                .with_on_draw_bg([](RectangleType r) { draw_wood_button(r); })
                .with_font("Garamond", h720(25.f))
                .with_custom_text_color(cream)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("shop_purchase"))) {
      purchase_cart();
    }

    div(context, mk(root.ent(), 980),
        box(scale, 38, 688, 440, 20)
            .with_label("A Select   X Add to cart   Y Remove")
            .with_font("Archivo", h720(16.f))
            .with_custom_text_color(afterhours::Color{196, 179, 151, 255})
            .with_debug_name("shop_footer_left"));
    if (button(
            context, mk(root.ent(), 981),
            box(scale, 1040, 688, 200, 20)
                .with_label("B Leave shop")
                .with_font("Archivo", h720(16.f))
                .with_custom_text_color(afterhours::Color{196, 179, 151, 255})
                .with_alignment(TextAlignment::Right)
                .with_debug_name("shop_leave")))
      closed = true;
    div(context, mk(root.ent(), 982),
        box(scale, 460, 688, 360, 20)
            .with_label(status)
            .with_font("Archivo", h720(14.f))
            .with_custom_text_color(afterhours::Color{212, 190, 154, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("shop_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(shop_interface, "Game Mockups",
                        "RPG general store with buy/sell and cart",
                        ShopInterfaceScreen)
