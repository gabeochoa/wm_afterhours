#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/NotificationBadge.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ShopInterfaceScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 0; // 0=Buy, 1=Sell
  size_t selected_item = 1;
  int player_gold = 3750;

  struct ShopItem {
    std::string name;
    std::string category;
    std::string icon;
    int price;
    int stock;
    std::string description;
    afterhours::Color icon_color;
  };

  std::vector<ShopItem> buy_items = {
      {"Iron Sword",
       "Weapon",
       "SWD",
       250,
       5,
       "A sturdy blade for beginners.",
       {180, 180, 190, 255}},
      {"Health Potion",
       "Consumable",
       "HP",
       50,
       20,
       "Restores 50 HP instantly.",
       {220, 60, 80, 255}},
      {"Mana Potion",
       "Consumable",
       "MP",
       75,
       15,
       "Restores 30 MP instantly.",
       {80, 120, 220, 255}},
      {"Leather Armor",
       "Armor",
       "ARM",
       180,
       8,
       "Light protection for adventurers.",
       {160, 120, 80, 255}},
      {"Fire Scroll",
       "Magic",
       "FIR",
       320,
       3,
       "Casts Fireball (25 damage).",
       {240, 140, 40, 255}},
      {"Torch",
       "Tool",
       "TCH",
       15,
       50,
       "Lights dark areas for 10 minutes.",
       {255, 200, 80, 255}},
      {"Antidote",
       "Consumable",
       "ANT",
       35,
       12,
       "Cures poison status.",
       {120, 200, 120, 255}},
      {"Steel Shield",
       "Armor",
       "SHL",
       400,
       2,
       "Blocks 15% incoming damage.",
       {140, 150, 170, 255}},
  };

  struct CartItem {
    size_t item_index;
    int quantity;
  };

  std::vector<CartItem> cart = {
      {1, 3}, // 3x Health Potion
      {5, 2}, // 2x Torch
  };

  // Warm merchant/shop colors
  afterhours::Color bg_warm{35, 28, 22, 255};
  afterhours::Color panel_wood{55, 42, 32, 255};
  afterhours::Color panel_light{75, 60, 48, 255};
  afterhours::Color border_wood{100, 80, 60, 255};
  afterhours::Color accent_gold{230, 190, 70, 255};
  afterhours::Color accent_green{80, 190, 100, 255};
  afterhours::Color accent_red{200, 70, 60, 255};
  afterhours::Color white{250, 245, 235, 255};
  afterhours::Color cream{240, 230, 215, 255};
  afterhours::Color muted{155, 140, 120, 255};
  afterhours::Color dark_text{35, 28, 22, 255};
  afterhours::Color selected_bg{80, 65, 50, 255};
  afterhours::Color header_bg{65, 50, 38, 255};

  int get_cart_total() {
    int total = 0;
    for (auto &ci : cart) {
      total += buy_items[ci.item_index].price * ci.quantity;
    }
    return total;
  }

  int get_cart_item_count() {
    int count = 0;
    for (auto &ci : cart) {
      count += ci.quantity;
    }
    return count;
  }

  std::string format_gold(int amount) {
    std::string s = std::to_string(amount);
    std::string result;
    int count = 0;
    for (int i = (int)s.length() - 1; i >= 0; i--) {
      if (count > 0 && count % 3 == 0)
        result = "," + result;
      result = s[i] + result;
      count++;
    }
    return result;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(14.0f));
    Theme theme;
    theme.font = white;
    theme.darkfont = dark_text;
    theme.font_muted = muted;
    theme.background = bg_warm;
    theme.surface = panel_wood;
    theme.primary = border_wood;
    theme.secondary = panel_light;
    theme.accent = accent_gold;
    theme.error = accent_red;
    theme.roundness = 0.08f;
    theme.segments = 6;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_warm)
            .with_debug_name("bg"));

    // ========== HEADER ==========
    // Shop title
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("General Store")
            .with_size(ComponentSize{pixels(300), pixels(45)})
            .with_absolute_position(25.0f, 12.0f)
            .with_font("Gaegu-Bold", h720(36.0f))
            .with_custom_text_color(accent_gold));

    // Shopkeeper quote
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("\"Welcome, adventurer! Browse my finest wares.\"")
            .with_size(ComponentSize{pixels(420), pixels(24)})
            .with_absolute_position(25.0f, 52.0f)
            .with_font("EqProRounded", h720(15.0f))
            .with_custom_text_color(muted));

    // Player gold display
    div(context, mk(entity, 8),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(180), pixels(40)})
            .with_absolute_position((float)screen_w - 210.0f, 12.0f)
            .with_custom_background(panel_wood)
            .with_border(accent_gold, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.4f)
            .with_debug_name("gold_pill"));

    div(context, mk(entity, 9),
        ComponentConfig{}
            .with_label("G " + format_gold(player_gold))
            .with_size(ComponentSize{pixels(170), pixels(32)})
            .with_absolute_position((float)screen_w - 205.0f, 16.0f)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(accent_gold)
            .with_alignment(TextAlignment::Center));

    // ========== BUY/SELL TABS ==========
    std::vector<std::string> tab_labels = {"Buy", "Sell"};
    float tab_y = 78.0f;

    for (size_t i = 0; i < tab_labels.size(); i++) {
      bool is_sel = (i == selected_tab);
      afterhours::Color tab_bg = is_sel ? accent_gold : panel_wood;
      afterhours::Color tab_text = is_sel ? dark_text : muted;

      if (button(context, mk(entity, 10 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(tab_labels[i])
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_absolute_position(25.0f + (float)i * 110.0f, tab_y)
                     .with_custom_background(tab_bg)
                     .with_border(is_sel ? accent_gold : border_wood,
                                  is_sel ? 3.0f : 2.0f)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(tab_text)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1100))
                     .with_roundness(0.2f)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // ========== ITEM LIST ==========
    float list_x = 25.0f;
    float list_y = 120.0f;
    float list_w = (float)screen_w * 0.55f;
    float list_h = (float)screen_h - list_y - 50.0f;

    // List panel
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_720p_size(list_w, list_h)
            .with_absolute_position(list_x, list_y)
            .with_custom_background(panel_wood)
            .with_border(border_wood, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.04f)
            .with_debug_name("list_panel"));

    // Column headers
    float col_icon_x = list_x + 10.0f;
    float col_name_x = list_x + 60.0f;
    float col_cat_x = list_x + list_w * 0.45f;
    float col_stock_x = list_x + list_w * 0.65f;
    float col_price_x = list_x + list_w - 90.0f;
    float hdr_y = list_y + 8.0f;

    struct ColHeader {
      const char *label;
      int id;
      float x;
      int w;
      TextAlignment align;
    };
    ColHeader headers[] = {
        {"Item", 101, col_name_x, 120, TextAlignment::None},
        {"Type", 102, col_cat_x, 80, TextAlignment::None},
        {"Stock", 103, col_stock_x, 50, TextAlignment::None},
        {"Price", 104, col_price_x, 70, TextAlignment::Right},
    };
    for (auto &hdr : headers) {
      auto cfg = ComponentConfig{}
                     .with_label(hdr.label)
                     .with_size(ComponentSize{pixels(hdr.w), pixels(22)})
                     .with_absolute_position(hdr.x, hdr_y)
                     .with_custom_text_color(muted);
      if (hdr.align != TextAlignment::None)
        cfg.with_alignment(hdr.align);
      div(context, mk(entity, hdr.id), cfg);
    }

    // Separator
    div(context, mk(entity, 105),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(list_w - 20), pixels(1)})
            .with_absolute_position(list_x + 10.0f, hdr_y + 24.0f)
            .with_custom_background(border_wood));

    // Item rows
    float row_start_y = hdr_y + 32.0f;
    float row_h = 52.0f;

    for (size_t i = 0; i < buy_items.size(); i++) {
      auto &item = buy_items[i];
      float ry = row_start_y + (float)i * row_h;

      // Skip if out of panel
      if (ry + row_h > list_y + list_h)
        break;

      bool is_sel = (i == selected_item);
      bool can_afford = player_gold >= item.price;

      // Alternating row background
      if (!is_sel && i % 2 == 0) {
        div(context, mk(entity, 160 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(list_w - 10), pxf(row_h - 4)})
                .with_absolute_position(list_x + 5.0f, ry)
                .with_custom_background(afterhours::Color{48, 36, 28, 255})
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.06f)
                .with_debug_name("alt_row_" + std::to_string(i)));
      }

      // Row highlight
      if (is_sel) {
        div(context, mk(entity, 110 + static_cast<int>(i) * 6),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(list_w - 10), pxf(row_h - 4)})
                .with_absolute_position(list_x + 5.0f, ry)
                .with_custom_background(selected_bg)
                .with_border(accent_gold, 1.0f)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.08f)
                .with_debug_name("row_sel_" + std::to_string(i)));
      }

      // Item row button (invisible, for selection)
      if (button(context, mk(entity, 111 + static_cast<int>(i) * 6),
                 ComponentConfig{}
                     .with_size(ComponentSize{pxf(list_w - 10), pxf(row_h - 4)})
                     .with_absolute_position(list_x + 5.0f, ry)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_debug_name("row_btn_" + std::to_string(i)))) {
        selected_item = i;
      }

      // Icon
      div(context, mk(entity, 112 + static_cast<int>(i) * 6),
          ComponentConfig{}
              .with_label(item.icon)
              .with_size(ComponentSize{pixels(38), pixels(38)})
              .with_absolute_position(col_icon_x, ry + 5.0f)
              .with_custom_background(item.icon_color)
              .with_font("EqProRounded", h720(22.0f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_debug_name("icon_" + std::to_string(i)));

      // Name
      div(context, mk(entity, 113 + static_cast<int>(i) * 6),
          ComponentConfig{}
              .with_label(item.name)
              .with_size(ComponentSize{pixels(180), pixels(28)})
              .with_absolute_position(col_name_x, ry + 10.0f)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(is_sel ? accent_gold : white)
              .with_debug_name("name_" + std::to_string(i)));

      // Category
      div(context, mk(entity, 114 + static_cast<int>(i) * 6),
          ComponentConfig{}
              .with_label(item.category)
              .with_size(ComponentSize{pixels(90), pixels(24)})
              .with_absolute_position(col_cat_x, ry + 12.0f)
              .with_custom_text_color(muted)
              .with_debug_name("cat_" + std::to_string(i)));

      // Stock
      div(context, mk(entity, 115 + static_cast<int>(i) * 6),
          ComponentConfig{}
              .with_label(item.stock <= 3 ? std::to_string(item.stock) + " Low"
                                          : std::to_string(item.stock))
              .with_size(ComponentSize{pixels(75), pixels(24)})
              .with_absolute_position(col_stock_x, ry + 12.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(item.stock <= 3 ? accent_red : white)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("stock_" + std::to_string(i)));

      // Price
      afterhours::Color price_color = can_afford ? accent_gold : accent_red;
      div(context, mk(entity, 116 + static_cast<int>(i) * 6),
          ComponentConfig{}
              .with_label(format_gold(item.price) + " G")
              .with_size(ComponentSize{pixels(80), pixels(24)})
              .with_absolute_position(col_price_x - 10.0f, ry + 12.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(price_color)
              .with_alignment(TextAlignment::Right)
              .with_debug_name("price_" + std::to_string(i)));
    }

    // ========== RIGHT: ITEM DETAIL & CART ==========
    float detail_x = list_x + list_w + 16.0f;
    float detail_y = list_y;
    float detail_w = (float)screen_w - detail_x - 25.0f;
    float detail_h = 200.0f;

    // Item detail panel
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_720p_size(detail_w, detail_h)
            .with_absolute_position(detail_x, detail_y)
            .with_custom_background(panel_wood)
            .with_border(border_wood, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 50})
            .with_debug_name("detail_panel"));

    auto &sel = buy_items[selected_item];

    // Large icon
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label(sel.icon)
            .with_size(ComponentSize{pixels(70), pixels(70)})
            .with_absolute_position(detail_x + 15.0f, detail_y + 15.0f)
            .with_custom_background(sel.icon_color)
            .with_border(border_wood, 2.0f)
            .with_font("EqProRounded", h720(36.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f));

    // Item name
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label(sel.name)
            .with_size(ComponentSize{pxf(detail_w - 110), pixels(30)})
            .with_absolute_position(detail_x + 95.0f, detail_y + 15.0f)
            .with_font("Gaegu-Bold", h720(26.0f))
            .with_custom_text_color(accent_gold));

    // Category badge
    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label(sel.category)
            .with_size(ComponentSize{pixels(90), pixels(24)})
            .with_absolute_position(detail_x + 95.0f, detail_y + 48.0f)
            .with_custom_background(panel_light)
            .with_border(border_wood, 1.0f)
            .with_custom_text_color(cream)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    // Description
    div(context, mk(entity, 304),
        ComponentConfig{}
            .with_label(sel.description)
            .with_size(ComponentSize{pxf(detail_w - 30), pixels(40)})
            .with_absolute_position(detail_x + 15.0f, detail_y + 95.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(white));

    // Price in detail
    div(context, mk(entity, 305),
        ComponentConfig{}
            .with_label("Price: " + format_gold(sel.price) + " G")
            .with_size(ComponentSize{pixels(160), pixels(28)})
            .with_absolute_position(detail_x + 15.0f, detail_y + 140.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(accent_gold));

    // Add to cart button
    button(context, mk(entity, 306),
           ComponentConfig{}
               .with_label("Add to Cart")
               .with_size(ComponentSize{pixels(130), pixels(36)})
               .with_absolute_position(detail_x + detail_w - 150.0f,
                                       detail_y + detail_h - 52.0f)
               .with_custom_background(accent_green)
               .with_border(afterhours::Color{60, 150, 70, 255}, 2.0f)
               .with_font("EqProRounded", h720(18.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f));

    // ========== CART ==========
    float cart_y = detail_y + detail_h + 15.0f;
    float cart_h = list_h - detail_h - 15.0f;

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_720p_size(detail_w, cart_h)
            .with_absolute_position(detail_x, cart_y)
            .with_custom_background(panel_wood)
            .with_border(border_wood, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("cart_panel"));

    // Cart header
    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("Cart (" + std::to_string(get_cart_item_count()) +
                        " items)")
            .with_size(ComponentSize{pxf(detail_w - 20), pixels(28)})
            .with_absolute_position(detail_x + 10.0f, cart_y + 8.0f)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(accent_gold));

    // Cart items
    for (size_t i = 0; i < cart.size(); i++) {
      auto &ci = cart[i];
      auto &item = buy_items[ci.item_index];
      float cy = cart_y + 42.0f + (float)i * 40.0f;

      // Item name and quantity
      div(context, mk(entity, 410 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(item.name + "  x" + std::to_string(ci.quantity))
              .with_size(ComponentSize{pxf(detail_w - 120), pixels(24)})
              .with_absolute_position(detail_x + 15.0f, cy)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(white)
              .with_debug_name("cart_item_" + std::to_string(i)));

      // Subtotal
      div(context, mk(entity, 411 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(format_gold(item.price * ci.quantity) + " G")
              .with_size(ComponentSize{pixels(90), pixels(24)})
              .with_absolute_position(detail_x + detail_w - 105.0f, cy)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(accent_gold)
              .with_alignment(TextAlignment::Right)
              .with_debug_name("cart_sub_" + std::to_string(i)));
    }

    // Cart separator
    float sep_y = cart_y + cart_h - 88.0f;
    div(context, mk(entity, 450),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(detail_w - 20), pixels(1)})
            .with_absolute_position(detail_x + 10.0f, sep_y)
            .with_custom_background(border_wood)
            .with_debug_name("cart_sep"));

    // Total
    int total = get_cart_total();
    bool can_afford_total = player_gold >= total;

    div(context, mk(entity, 451),
        ComponentConfig{}
            .with_label("Total:")
            .with_size(ComponentSize{pixels(80), pixels(28)})
            .with_absolute_position(detail_x + 15.0f, sep_y + 10.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(white));

    div(context, mk(entity, 452),
        ComponentConfig{}
            .with_label(format_gold(total) + " G")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_absolute_position(detail_x + detail_w - 145.0f, sep_y + 10.0f)
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(can_afford_total ? accent_gold : accent_red)
            .with_alignment(TextAlignment::Right));

    // Remaining gold
    div(context, mk(entity, 453),
        ComponentConfig{}
            .with_label("Remaining: " + format_gold(player_gold - total) + " G")
            .with_size(ComponentSize{pxf(detail_w - 30), pixels(22)})
            .with_absolute_position(detail_x + 15.0f, sep_y + 38.0f)
            .with_custom_text_color(can_afford_total ? muted : accent_red));

    // Purchase button
    button(
        context, mk(entity, 460),
        ComponentConfig{}
            .with_label("Purchase")
            .with_size(ComponentSize{pxf(detail_w - 30), pixels(44)})
            .with_absolute_position(detail_x + 15.0f, cart_y + cart_h - 55.0f)
            .with_custom_background(can_afford_total
                                        ? accent_gold
                                        : afterhours::Color{100, 80, 60, 255})
            .with_border(can_afford_total ? afterhours::Color{190, 155, 40, 255}
                                          : afterhours::Color{80, 60, 45, 255},
                         3.0f)
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(can_afford_total ? dark_text : muted)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 50})
            .with_debug_name("purchase_btn"));

    // ========== BOTTOM STATUS ==========
    float status_y = (float)screen_h - 35.0f;
    div(context, mk(entity, 500),
        ComponentConfig{}
            .with_label("A: Select   B: Back   X: Add to Cart   Y: Remove")
            .with_size(ComponentSize{pixels(screen_w - 50), pixels(22)})
            .with_absolute_position(25.0f, status_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(shop_interface, "Game Mockups",
                        "RPG general store with buy/sell and cart",
                        ShopInterfaceScreen)
