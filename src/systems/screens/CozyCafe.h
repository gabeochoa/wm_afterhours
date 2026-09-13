#pragma once

#include "../../external.h"
#include "../../artwork_atlas.h"
#include "../../artwork_frames.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/menu.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CozyCafeScreen : ScreenSystem<UIContext<InputAction>> {
  // Game state
  float music_volume = 0.7f;
  size_t selected_special = 0;
  int gold_coins = 1347;
  int customers_today = 23;
  int opened_tool = -1;
  std::string status_message = "Guildmate23: need help with the recipe?";

  // Loaded textures
  artwork::Atlas atlas;
  artwork::Sprite star_filled_tex{};
  artwork::Sprite star_empty_tex{};
  artwork::Sprite flower_tex{};
  artwork::Sprite icon_inventory_tex{};
  artwork::Sprite icon_crafting_tex{};

  void load_textures_if_needed() {
    atlas.load("cafe.png");
    star_filled_tex = atlas.sprite(artwork::cafe_frames[0]);
    star_empty_tex = atlas.sprite(artwork::cafe_frames[1]);
    flower_tex = atlas.sprite(artwork::cafe_frames[2]);
    icon_inventory_tex = atlas.sprite(artwork::cafe_frames[3]);
    icon_crafting_tex = atlas.sprite(artwork::cafe_frames[4]);
  }

  std::vector<std::string> daily_specials = {"Lavender Latte", "Honey Toast",
                                             "Matcha Cake"};

  // Colors matching the inspiration exactly
  afterhours::Color kraft_bg{154, 126, 87, 255};       // Kraft paper tan
  afterhours::Color kraft_frame{160, 130, 100, 255};   // Darker kraft border
  afterhours::Color cream_surface{255, 250, 240, 255}; // Warm cream white
  afterhours::Color brown_border{118, 83, 56, 255};    // Panel border brown
  afterhours::Color brown_header{130, 105, 85, 255};   // Title bar brown
  afterhours::Color sage_header{145, 175, 150, 255};   // Customers header sage
  afterhours::Color pink_btn{233, 174, 172, 255};      // Lavender Latte pink
  afterhours::Color mint_btn{182, 201, 152, 255};      // Honey Toast mint
  afterhours::Color tan_btn{216, 184, 135, 255};       // Matcha Cake tan
  afterhours::Color rose_btn{235, 180, 175, 255};      // Serve Next rose
  afterhours::Color dark_text{112, 82, 56, 255};       // Dark sepia text
  afterhours::Color badge_red{170, 70, 70, 255};       // Notification badge
  afterhours::Color paper_surface{250, 238, 205, 255};
  afterhours::Color panel_left{199, 177, 140, 255};
  afterhours::Color panel_right{173, 189, 155, 255};
  afterhours::Color chat_bg{142, 118, 83, 255};

  struct Customer {
    std::string name;
    std::string order;
    int wait_time;
    float progress;
  };

  std::vector<Customer> waiting_customers = {
      {"Emma", "Cappuccio", 2, 0.5f},
      {"Oliver", "Croissant", 5, 0.14f},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();

    const float screen_w = context.screen_width > 0.f
                               ? context.screen_width
                               : Settings::get().get_screen_width();
    const float screen_h = context.screen_height > 0.f
                               ? context.screen_height
                               : Settings::get().get_screen_height();
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);
    const float offset_x = (screen_w - 1280.f * scale) / 2;
    const float offset_y = (screen_h - 720.f * scale) / 2;
    auto ax = [=](float v) { return offset_x + v * scale; };
    auto ay = [=](float v) { return offset_y + v * scale; };
    auto px = [=](float v) { return pixels(v * scale); };

    UIStylingDefaults::get().set_default_font("GaeguMock", px(18));
    Theme theme;
    theme.font = dark_text;
    theme.darkfont = cream_surface;
    theme.font_muted = afterhours::Color{100, 85, 70, 255};
    theme.background = kraft_bg;
    theme.surface = cream_surface;
    theme.primary = brown_header;
    theme.secondary = sage_header;
    theme.accent = rose_btn;
    theme.error = badge_red;
    theme.corner_radius = 8.f * scale;
    context.set_theme(theme);

    auto outline = [=](float radius, float thickness, afterhours::Color color) {
      return [=](RectangleType rect) {
        const float width = thickness * scale;
        rect.x += width / 2;
        rect.y += width / 2;
        rect.width -= width;
        rect.height -= width;
        const float roundness = std::clamp(
            2 * (radius * scale - width / 2) / std::min(rect.width, rect.height),
            0.f, 1.f);
        afterhours::draw_rectangle_rounded_lines_ex(rect, roundness, 24, width,
                                                   color);
      };
    };
    auto box = [&](float x, float y, float w, float h,
                   afterhours::Color color, float radius = 0.f) {
      return ComponentConfig{}
          .with_size({px(w), px(h)})
          .with_absolute_position(ax(x), ay(y))
          .with_custom_background(color)
          .with_corner_radius(radius * scale);
    };
    auto label = [&](int id, const std::string &value, float x, float y,
                     float w, float h, float size,
                     afterhours::Color color, const char *name = "",
                     const char *font = "GaeguMock") {
      div(context, mk(entity, id),
          box(x, y, w, h, {0, 0, 0, 0})
              .with_label(value)
              .with_font(font, px(size))
              .with_custom_text_color(color)
              .with_letter_spacing(0)
              .with_ignore_pointer_events()
              .with_debug_name(name));
    };
    auto image = [&](int id, const artwork::Sprite &image,
                     float x, float y, float w, float h, const char *name) {
      if (image.texture.id == 0) return;
      sprite(context, mk(entity, id), image.texture, image.source,
             box(x, y, w, h, {0, 0, 0, 0})
                 .with_ignore_pointer_events()
                 .with_debug_name(name));
    };

    // ========== BACKGROUND AND PAPER BOARD ==========
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size({pixels(screen_w), pixels(screen_h)})
            .with_corner_radius(0)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_rectangle_gradient_v(r, {157, 131, 94, 255},
                                                    {137, 109, 76, 255});
            })
            .with_background(Theme::Usage::None)
            .with_debug_name("cafe_background"));
    div(context, mk(entity, 2),
        box(116, 15, 1048, 690, paper_surface, 38)
            .with_soft_shadow(0, 6 * scale, 0, {110, 83, 52, 45})
            .with_on_draw_fg(outline(38, 4, {107, 78, 52, 255}))
            .with_debug_name("paper_board"));

    // ========== HEADER ==========
    // Title with flower symbol
    // Flower icon
    image(9, flower_tex, 148, 49, 38, 38, "flower_icon");
    label(10, "Blossom Cafe", 188, 41, 428, 60, 52, dark_text,
          "cafe_title", "GaeguMock");
    // Gold pill background
    div(context, mk(entity, 20),
        box(663, 43, 207, 56, {255, 247, 222, 255}, 16)
            .with_on_draw_fg(outline(16, 2, brown_border))
            .with_debug_name("gold_pill"));
    div(context, mk(entity, 22),
        box(676, 57, 27, 27, {223, 174, 70, 255}, 13.5f)
            .with_label("G")
            .with_font("GaeguMock", px(18))
            .with_custom_text_color(brown_border)
            .with_alignment(TextAlignment::Center)
            .with_on_draw_fg(outline(13.5f, 2, brown_border))
            .with_ignore_pointer_events());
    label(21, std::to_string(gold_coins) + " coins", 705, 52, 158, 38,
          26, dark_text, "gold_text", "GaeguMock");
    // Rating box - widened to fit all content including numeric rating
    div(context, mk(entity, 30),
        box(883, 37, 252, 68, {255, 247, 222, 255}, 16)
            .with_on_draw_fg(outline(16, 2, brown_border))
            .with_debug_name("rating_box"));
    // Rating label
    label(31, "4 / 5", 893, 44, 67, 28, 20, dark_text, "rating_value");
    for (int i = 0; i < 5; ++i) {
      if (i == 4) {
        div(context, mk(entity, 38),
            box(1093, 46, 25, 25, {211, 199, 168, 255}, 5)
                .with_ignore_pointer_events());
      }
      image(33 + i, i < 4 ? star_filled_tex : star_empty_tex,
            969.f + i * 31.f, 46, 25, 25, "rating_star");
    }
    label(32, "Customers served: " + std::to_string(customers_today),
          898, 75, 223, 23, 17, dark_text, "served_text");

    // ========== LEFT PANEL: Today's Specials ==========
    // Center content to better match square inspiration on widescreen
    div(context, mk(entity, 100),
        box(149, 125, 465, 390, panel_left, 36)
            .with_soft_shadow(4 * scale, 7 * scale, 0, {110, 83, 52, 55})
            .with_on_draw_fg(outline(36, 3, brown_border))
            .with_debug_name("specials_panel"));
    // Brown header
    div(context, mk(entity, 101),
        box(149, 133, 465, 60, {0, 0, 0, 0})
            .with_label("Today's Specials")
            .with_font("GaeguMock", px(43))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("specials_title"));
    div(context, mk(entity, 102),
        box(166, 203, 429, 292, {255, 249, 227, 255}, 22)
            .with_on_draw_fg(outline(22, 2, {158, 130, 91, 255}))
            .with_debug_name("specials_inner"));
    const afterhours::Color menu_colors[] = {pink_btn, mint_btn, tan_btn};
    const char *descriptions[] = {"Floral coffee / promotion earns 25 coins",
                                  "Sweet toast / promotion earns 25 coins",
                                  "Green tea cake / promotion earns 25 coins"};
    for (size_t i = 0; i < daily_specials.size(); ++i) {
      const bool selected = i == selected_special;
      const float y = 215.f + static_cast<float>(i) * 63.f;
      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 box(185, y, 391, 57, menu_colors[i], 18)
                     .with_on_draw_fg(outline(18, selected ? 3.5f : 2.f,
                                              brown_border))
                     .with_debug_name("special_" + std::to_string(i)))) {
        selected_special = i;
      }
      label(130 + static_cast<int>(i), daily_specials[i], 200, y + 1,
            272, 33, 30, dark_text, "special_name", "GaeguMock");
      label(140 + static_cast<int>(i), descriptions[i], 200, y + 31,
            364, 20, 15, dark_text, "special_description");
      if (!selected) continue;
      div(context, mk(entity, 150 + static_cast<int>(i)),
          box(480, y + 8, 83, 23, brown_border, 8)
              .with_label("Selected")
              .with_font("GaeguMock", px(15))
              .with_custom_text_color(cream_surface)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events()
              .with_debug_name("selected_special"));
    }
    // Promote Special button - widened for full text visibility (minimum 44px
    // touch target)
    if (button(context, mk(entity, 120),
               box(185, 410, 391, 46, {255, 240, 194, 255}, 14)
                   .with_label("Promote " + daily_specials[selected_special])
                   .with_font("GaeguMock", px(28))
                   .with_custom_text_color(dark_text)
                   .with_alignment(TextAlignment::Center)
                   .with_on_draw_fg(outline(14, 2, brown_border))
                   .with_debug_name("promote_special"))) {
      gold_coins += 25;
      status_message = daily_specials[selected_special] + " promoted";
    }
    label(121, "+25 coins instantly / no cost", 217, 463, 339, 24,
          17, dark_text, "promotion_reward");

    // ========== RIGHT PANEL: Customers ==========
    div(context, mk(entity, 200),
        box(644, 125, 490, 390, panel_right, 36)
            .with_soft_shadow(4 * scale, 7 * scale, 0, {110, 83, 52, 55})
            .with_on_draw_fg(outline(36, 3, brown_border))
            .with_debug_name("customers_panel"));
    // Sage header
    label(201, "Customers", 690, 132, 240, 57, 43, dark_text,
          "customers_title", "GaeguMock");
    label(203, std::to_string(waiting_customers.size()) + " waiting",
          959, 149, 135, 29, 22, dark_text, "waiting_count");
    div(context, mk(entity, 202),
        box(661, 203, 453, 292, {255, 249, 227, 255}, 22)
            .with_on_draw_fg(outline(22, 2, {158, 130, 91, 255}))
            .with_debug_name("customers_inner"));

    // Customer rows
    for (size_t i = 0; i < waiting_customers.size(); ++i) {
      const auto &customer = waiting_customers[i];
      const float y = 219.f + static_cast<float>(i) * 95.f;
      const int id = 210 + static_cast<int>(i) * 20;
      const bool low = customer.progress < .35f;
      div(context, mk(entity, id),
          box(679, y, 417, 88, low ? afterhours::Color{249, 227, 212, 255}
                                   : afterhours::Color{247, 241, 216, 255}, 12)
              .with_debug_name("customer_row_" + std::to_string(i)));
      if (low) {
        div(context, mk(entity, id + 1),
            box(679, y + 12, 4, 64, badge_red, 2)
                .with_debug_name("customer_urgent"));
      }
      label(id + 2, customer.name, 691, y - 1, 241, 35, 30,
            dark_text, "cust_name", "GaeguMock");
      label(id + 3, customer.order, 695, y + 31, 229, 25, 19,
            dark_text, "cust_order");
      label(id + 4, "Waited " + std::to_string(customer.wait_time) + "m",
            970, y + 4, 119, 27, 18, low ? badge_red : dark_text,
            "customer_wait");
      // Patience row sits BELOW the name
      label(id + 5, "Patience", 695, y + 60, 116, 23, 17,
            dark_text, "patience_label");
      // Progress bar bg with label
      div(context, mk(entity, id + 6),
          box(809, y + 65, 158, 14, {211, 195, 158, 255}, 7)
              .with_debug_name("prog_bg_" + std::to_string(i)));
      // Progress fill
      if (customer.progress > 0) {
        div(context, mk(entity, id + 7),
            box(809, y + 65, 158 * customer.progress, 14,
                low ? badge_red : afterhours::Color{104, 130, 82, 255}, 7)
                .with_debug_name("prog_fill_" + std::to_string(i)));
      }
      label(id + 8, std::to_string(static_cast<int>(std::round(
                             customer.progress * 100))) + "%",
            972, y + 59, 60, 24, 17, dark_text, "patience_value");
      if (!low) continue;
      div(context, mk(entity, id + 9),
          box(1034, y + 58, 54, 26, badge_red, 8)
              .with_label("LOW")
              .with_font("GaeguMock", px(18))
              .with_custom_text_color(cream_surface)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("patience_low"));
    }
    if (button(context, mk(entity, 250),
               box(709, 421, 355, 48,
                   waiting_customers.empty() ? panel_left : rose_btn, 20)
                   .with_label(waiting_customers.empty()
                                   ? "Queue complete"
                                   : "Serve " + waiting_customers.front().name)
                   .with_font("GaeguMock", px(34))
                   .with_custom_text_color(dark_text)
                   .with_alignment(TextAlignment::Center)
                   .with_disabled(waiting_customers.empty())
                   .with_on_draw_fg(outline(20, 2.5f, brown_border))
                   .with_debug_name("serve_next"))) {
      ++customers_today;
      gold_coins += 12;
      status_message = "Served " + waiting_customers.front().name + "'s order";
      waiting_customers.erase(waiting_customers.begin());
    }
    label(251, "+12 coins per order / first in queue", 725, 471, 331, 22,
          16, dark_text, "serve_reward");
    if (waiting_customers.empty()) {
      label(260, "All orders served", 733, 283, 316, 45, 32,
            dark_text, "queue_empty", "GaeguMock");
      label(261, "The counter is ready for a quiet moment.", 704, 330,
            377, 27, 18, dark_text, "queue_empty_detail");
    }

    // ========== MUSIC SLIDER ==========
    label(303, "Music: " + std::to_string(static_cast<int>(
                                std::round(music_volume * 100))) + "%",
          153, 536, 161, 35, 24, dark_text, "music_label", "GaeguMock");
    slider(context, mk(entity, 300), music_volume,
           box(320, 537, 373, 36, {0, 0, 0, 0}, 18)
               .with_debug_name("music_slider"),
           SliderHandleValueLabelPosition::None);
    div(context, mk(entity, 301),
        box(324, 548, 365, 14, {205, 186, 148, 255}, 7)
            .with_ignore_pointer_events()
            .with_debug_name("music_track"));
    if (music_volume > 0) {
      div(context, mk(entity, 302),
          box(324, 548, 365 * music_volume, 14, {104, 130, 82, 255}, 7)
              .with_ignore_pointer_events()
              .with_debug_name("music_fill"));
    }
    div(context, mk(entity, 304),
        box(320 + 345 * music_volume, 541, 28, 28, cream_surface, 14)
            .with_on_draw_fg(outline(14, 2.5f, brown_border))
            .with_ignore_pointer_events()
            .with_debug_name("music_handle"));

    // ========== CHAT BOX ==========
    label(399, "Guild chat", 154, 575, 250, 27, 24, dark_text,
          "guild_chat_title", "GaeguMock");
    div(context, mk(entity, 400),
        box(152, 605, 543, 78, chat_bg, 12)
            .with_on_draw_fg(outline(12, 2, brown_border))
            .with_debug_name("chat_box"));
    // Chat messages with avatars - data-driven
    const char *messages[] = {status_message.c_str(),
                             "DevTeam_Support: Check out the new update!"};
    for (int i = 0; i < 2; ++i) {
      const float y = 615.f + static_cast<float>(i) * 30.f;
      div(context, mk(entity, 405 + i),
          box(164, y + 3, 23, 23, i == 0 ? mint_btn : pink_btn, 11.5f)
              .with_label(i == 0 ? "G" : "D")
              .with_font("GaeguMock", px(14))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center));
      label(401 + i, messages[i], 194, y, 488, 29, 20, cream_surface,
            i == 0 ? "chat_status" : "chat_support", "GaeguMock");
    }

    // ========== BOTTOM RIGHT: Icons with Badges ==========
    // Connected to right panel via consistent positioning
    // Bottom-right icon buttons - data-driven
    struct IconBtn {
      int id;
      artwork::Sprite *texture;
      const char *name;
      const char *badge;
      const char *detail;
    };
    const IconBtn tools[] = {{500, &icon_inventory_tex, "Inventory", "2", "2 notices"},
                             {510, nullptr, "Research", "1", "1 recipe note"},
                             {520, &icon_crafting_tex, "Crafting", nullptr, "No notices"}};
    for (size_t i = 0; i < 3; ++i) {
      const auto &tool = tools[i];
      const float x = 801.f + static_cast<float>(i) * 114.f;
      if (button(context, mk(entity, tool.id),
                 box(x, 574, 68, 64, {250, 237, 206, 255}, 14)
                     .with_on_draw_fg(outline(14, 2, brown_border))
                     .with_debug_name(std::string("tool_") + tool.name))) {
        opened_tool = static_cast<int>(i);
        status_message = std::string(tool.name) + " opened";
      }
      if (i != 1) {
        image(tool.id + 1, *tool.texture, x + 8, 580, 52, 52, "tool_icon");
      } else {
        div(context, mk(entity, tool.id + 1),
            box(x + 13, 585, 42, 42, {0, 0, 0, 0})
                .with_ignore_pointer_events()
                .with_on_draw_fg([=](RectangleType r) {
                  raylib::DrawRectangleRounded(
                      {r.x, r.y, r.width, r.height}, .14f, 8,
                      {118, 83, 56, 255});
                  raylib::DrawRectangleRounded(
                      {r.x + 3 * scale, r.y + 3 * scale,
                       r.width - 6 * scale, r.height - 6 * scale},
                      .1f, 8, {255, 249, 227, 255});
                  raylib::DrawLineEx({r.x + r.width / 2, r.y + 4 * scale},
                                     {r.x + r.width / 2, r.y + r.height - 4 * scale},
                                     2 * scale, {118, 83, 56, 255});
                  for (int line = 0; line < 3; ++line) {
                    const float y = r.y + (12.f + static_cast<float>(line) * 8.f) * scale;
                    raylib::DrawLineEx({r.x + 7 * scale, y},
                                       {r.x + 16 * scale, y}, 2 * scale,
                                       {154, 172, 124, 255});
                    raylib::DrawLineEx({r.x + 26 * scale, y},
                                       {r.x + 35 * scale, y}, 2 * scale,
                                       {154, 172, 124, 255});
                  }
                })
                .with_debug_name("research_book"));
      }
      if (tool.badge) {
        div(context, mk(entity, tool.id + 2),
            box(x + 50, 565, 29, 29, badge_red, 14.5f)
                .with_label(tool.badge)
                .with_font("GaeguMock", px(19))
                .with_custom_text_color(cream_surface)
                .with_alignment(TextAlignment::Center)
                .with_ignore_pointer_events()
                .with_debug_name(std::string("notices_") + tool.name));
      }
      div(context, mk(entity, tool.id + 3),
          box(x - 23, 641, 114, 29, {0, 0, 0, 0})
              .with_label(tool.name)
              .with_font("GaeguMock", px(25))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events());
      div(context, mk(entity, tool.id + 4),
          box(x - 23, 669, 114, 21, {0, 0, 0, 0})
              .with_label(tool.detail)
              .with_font("GaeguMock", px(15))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events());
    }
    bool tool_open = opened_tool >= 0;
    auto tool_panel = popover(context, mk(entity, 600),
        {ax(152), ay(687), 543 * scale, 0}, tool_open,
        overlay::Placement::Above,
        ComponentConfig{}.with_size({px(543), px(112)})
            .with_custom_background(cream_surface).with_corner_radius(12 * scale)
            .with_render_layer(10).with_on_draw_fg(outline(12, 2, brown_border))
            .with_debug_name("tool_details"));
    if (!tool_open) { opened_tool = -1; return; }
    const auto &tool = tools[opened_tool];
    div(context, mk(tool_panel.ent(), 601),
        ComponentConfig{}.with_size({px(424), px(28)}).with_absolute_position(pixels(12 * scale), pixels(5 * scale)).with_transparent_bg()
            .with_label(std::string(tool.name) + " opened")
            .with_font("GaeguMock", px(27))
            .with_custom_text_color(dark_text)
            .with_render_layer(11));
    const char *details[] = {"Pantry: stock check due\nStorage: shelf labels ready",
                              "Recipe notebook: try a lavender latte.",
                              "Your crafting station has no new notices."};
    div(context, mk(tool_panel.ent(), 602),
        ComponentConfig{}.with_size({px(510), px(61)}).with_absolute_position(pixels(13 * scale), pixels(41 * scale)).with_transparent_bg()
            .with_label(details[opened_tool])
            .with_font("GaeguMock", px(18))
            .with_custom_text_color(dark_text)
            .with_render_layer(11));
    if (button(context, mk(tool_panel.ent(), 603),
               ComponentConfig{}.with_size({px(84), px(30)}).with_absolute_position(pixels(443 * scale), pixels(6 * scale)).with_custom_background(panel_left).with_corner_radius(8 * scale)
                   .with_label("Close")
                   .with_font("GaeguMock", px(17))
                   .with_custom_text_color(dark_text)
                   .with_alignment(TextAlignment::Center)
                   .with_render_layer(12)
                   .with_debug_name("tool_close"))) {
      opened_tool = -1;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(cozy_cafe, "Game Mockups",
                        "Cozy cafe simulation game menu", CozyCafeScreen)
