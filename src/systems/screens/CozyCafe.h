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

struct CozyCafeScreen : ScreenSystem<UIContext<InputAction>> {
  // Game state
  float music_volume = 0.7f;
  size_t selected_special = 0;
  int gold_coins = 1347;
  int customers_today = 23;
  std::string status_message = "Guildmate23: need help with the recipe?";

  // Loaded textures
  bool textures_loaded = false;
  raylib::Texture2D star_filled_tex{};
  raylib::Texture2D star_empty_tex{};
  raylib::Texture2D clock_tex{};
  raylib::Texture2D flower_tex{};
  raylib::Texture2D avatar_guildmate_tex{};
  raylib::Texture2D avatar_devteam_tex{};
  raylib::Texture2D icon_inventory_tex{};
  raylib::Texture2D icon_research_tex{};
  raylib::Texture2D icon_crafting_tex{};

  void load_textures_if_needed() {
    if (textures_loaded)
      return;
    textures_loaded = true;

    std::string images_path =
        afterhours::files::get_resource_path("images", "").string();
    auto load = [&](const char *name) {
      auto texture = raylib::LoadTexture((images_path + name).c_str());
      raylib::SetTextureFilter(texture, raylib::TEXTURE_FILTER_BILINEAR);
      return texture;
    };
    star_filled_tex = load("cozy_cafe/star.png");
    star_empty_tex = load("cozy_cafe/star_empty.png");
    clock_tex = load("cozy_cafe/clock.png");
    flower_tex = load("cozy_cafe/flower.png");

    icon_inventory_tex = load("cozy_cafe/inventory.png");
    icon_research_tex = load("cozy_cafe/research.png");
    icon_crafting_tex = load("cozy_cafe/crafting.png");
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

    UIStylingDefaults::get().set_default_font("GaeguMock", h720(18.0f));
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
    theme.roundness = 0.12f;
    theme.segments = 8;
    context.set_theme(theme);

    const float screen_w =
        context.screen_width > 0.f
            ? context.screen_width
            : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h =
        context.screen_height > 0.f
            ? context.screen_height
            : static_cast<float>(Settings::get().get_screen_height());
    const float sx = screen_w / 1280.f;
    const float sy = screen_h / 720.f;
    auto ax = [sx](float v) { return v * sx; };
    auto ay = [sy](float v) { return v * sy; };
    auto pxf = [sx](float v) { return pixels(static_cast<int>(v * sx)); };
    auto pyf = [sy](float v) { return pixels(static_cast<int>(v * sy)); };

    auto outline = [sy](float radius, float thickness,
                        afterhours::Color color) {
      return [=](RectangleType rect) {
        const float width = thickness * sy;
        rect.x += width / 2;
        rect.y += width / 2;
        rect.width -= width;
        rect.height -= width;
        const float roundness = std::clamp(
            2 * (radius * sy - width / 2) / std::min(rect.width, rect.height),
            0.f, 1.f);
        afterhours::draw_rectangle_rounded_lines_ex(rect, roundness, 24, width,
                                                    color);
      };
    };

    // ========== BACKGROUND AND PAPER BOARD ==========
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<float>(screen_w)),
                                     pixels(static_cast<float>(screen_h))})
            .with_corner_radius(0)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_rectangle_gradient_v(r, {157, 131, 94, 255},
                                                    {137, 109, 76, 255});
            })
            .with_background(Theme::Usage::None)
            .with_debug_name("cafe_background"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(1048.f), pyf(690.f)})
            .with_absolute_position(ax(116.0f), ay(15.0f))
            .with_custom_background(paper_surface)
            .with_border(afterhours::Color{107, 78, 52, 255}, 4.0f)
            .with_soft_shadow(0.0f, 6.0f, 0.0f,
                              afterhours::Color{110, 83, 52, 45})
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(38.f * sy)
            .with_on_draw_fg(outline(38, 4, {107, 78, 52, 255}))
            .with_debug_name("paper_board"));

    // ========== HEADER ==========
    // Title with flower symbol
    // Flower icon
    if (flower_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)flower_tex.width,
                                                 (float)flower_tex.height};
      sprite(context, mk(entity, 9), flower_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pxf(38), pyf(38)})
                 .with_absolute_position(ax(148.0f), ay(49.0f))
                 .with_debug_name("flower_icon"));
    }

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Blossom Cafe")
            .with_size(ComponentSize{pxf(340), pyf(50)})
            .with_absolute_position(ax(flower_tex.id != 0 ? 188.0f : 150.0f),
                                    ay(47.0f))
            .with_font("GaeguMock", h720(47.0f))
            .with_custom_text_color(cream_surface)
            .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
            .with_letter_spacing(0.f)
            .with_letter_spacing(-2.5f * sy)
            .with_debug_name("cafe_title"));

    // Gold pill background
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(185), pyf(52)})
            .with_absolute_position(ax(677.0f), ay(45.0f))
            .with_custom_background(afterhours::Color{255, 247, 222, 180})
            .with_border(afterhours::Color{123, 93, 63, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(15.f * sy)
            .with_on_draw_fg(outline(15, 2, brown_border))
            .with_debug_name("gold_pill"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Gold:  $" + std::to_string(gold_coins))
            .with_size(ComponentSize{pxf(165), pyf(32)})
            .with_absolute_position(ax(696.0f), ay(58.0f))
            .with_font("GaeguMock", h720(26.0f))
            .with_custom_text_color(cream_surface)
            .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
            .with_letter_spacing(0.f)
            .with_debug_name("gold_text"));

    // Rating box - widened to fit all content including numeric rating
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(252), pyf(68)})
            .with_absolute_position(ax(883.0f), ay(37.0f))
            .with_custom_background(afterhours::Color{255, 247, 222, 165})
            .with_border(afterhours::Color{123, 93, 63, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(16.f * sy)
            .with_on_draw_fg(outline(16, 2, brown_border))
            .with_debug_name("rating_box"));

    // Rating label
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Rating:")
            .with_size(ComponentSize{pxf(84), pyf(29)})
            .with_absolute_position(ax(908.0f), ay(51.0f))
            .with_font("GaeguMock", h720(25.0f))
            .with_letter_spacing(0.f)
            .with_custom_text_color(dark_text));

    for (int i = 0; i < 5; ++i) {
      const auto texture = i < 4 ? star_filled_tex : star_empty_tex;
      sprite(context, mk(entity, 33 + i), texture,
             {0, 0, (float)texture.width, (float)texture.height},
             ComponentConfig{}
                 .with_size({pxf(23), pyf(23)})
                 .with_absolute_position(ax(991.f + i * 24.f), ay(45.f))
                 .with_ignore_pointer_events()
                 .with_debug_name(fmt::format("cc_star_{}", i)));
    }

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Customers Served: " + std::to_string(customers_today))
            .with_size(ComponentSize{pxf(220), pyf(26)})
            .with_absolute_position(ax(908.0f), ay(76.0f))
            .with_font("GaeguMock", h720(22.0f))
            .with_custom_text_color(dark_text)
            .with_letter_spacing(0.f)
            .with_debug_name("served_text"));

    // ========== LEFT PANEL: Today's Specials ==========
    // Center content to better match square inspiration on widescreen
    // Use percent widths for cohesive layout grid
    float left_panel_x = 149.0f;
    float panel_y = 125.0f;
    float left_panel_w = 465.0f;
    float panel_h = 390.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(left_panel_w), pyf(panel_h)})
            .with_absolute_position(ax(left_panel_x), ay(panel_y))
            .with_custom_background(panel_left)
            .with_border(afterhours::Color{116, 83, 56, 255}, 3.0f)
            .with_soft_shadow(4.0f, 7.0f, 0.0f,
                              afterhours::Color{110, 83, 52, 55})
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(36.f * sy)
            .with_on_draw_fg(outline(36, 3, brown_border))
            .with_debug_name("specials_panel"));

    // Brown header
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("Today's Specials")
            .with_size(ComponentSize{pxf(left_panel_w), pyf(72)})
            .with_absolute_position(ax(left_panel_x), ay(panel_y + 3.0f))
            .with_background(Theme::Usage::None)
            .with_font("GaeguMock", h720(43.0f))
            .with_custom_text_color(cream_surface)
            .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
            .with_letter_spacing(0.f)
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(-2.f * sy)
            .with_debug_name("specials_title"));

    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(429.f), pyf(292.f)})
            .with_absolute_position(ax(166.0f), ay(203.0f))
            .with_custom_background(afterhours::Color{255, 249, 227, 255})
            .with_border(afterhours::Color{158, 130, 91, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(22.f * sy)
            .with_on_draw_fg(outline(22, 2, {158, 130, 91, 255}))
            .with_debug_name("specials_inner"));

    // Menu items - styled as pill buttons with colored fills (minimum 44px
    // touch targets)
    afterhours::Color menu_colors[] = {pink_btn, mint_btn, tan_btn};
    float menu_y = 227.0f;
    float menu_btn_w = 357.0f;
    for (size_t i = 0; i < daily_specials.size(); i++) {
      bool selected = (i == selected_special);
      afterhours::Color btn_bg = menu_colors[i];

      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(daily_specials[i])
                     .with_size(ComponentSize{pxf(menu_btn_w), pyf(50)})
                     .with_absolute_position(ax(203.0f),
                                             ay(menu_y + (float)i * 67.0f))
                     .with_custom_background(btn_bg)
                     .with_border(brown_border, selected ? 3.0f : 2.0f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                       afterhours::Color{0, 0, 0, 35})
                     .with_font("GaeguMock", h720(32.0f))
                     .with_custom_text_color(cream_surface)
                     .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
                     .with_letter_spacing(0.f)
                     .with_rounded_corners(RoundedCorners())
                     .with_corner_radius(25.f * sy)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg(outline(25, selected ? 4.f : 2.5f, brown_border))
                     .with_debug_name("special_" + std::to_string(i)))) {
        selected_special = i;
      }
    }

    // Promote Special button - widened for full text visibility (minimum 44px
    // touch target)
    float promote_y = 431.0f;
    if (button(
            context, mk(entity, 120),
            ComponentConfig{}
                .with_label("Promote Special   ")
                .with_size(ComponentSize{pxf(menu_btn_w), pyf(46)})
                .with_absolute_position(ax(203.0f), ay(promote_y))
                .with_custom_background(afterhours::Color{255, 247, 222, 255})
                .with_border(brown_border, 2.0f)
                .with_font("GaeguMock", h720(29.0f))
                .with_custom_text_color(cream_surface)
                .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
                .with_letter_spacing(0.f)
                .with_rounded_corners(RoundedCorners())
                .with_corner_radius(14.f * sy)
                .with_alignment(TextAlignment::Center)
                .with_on_draw_fg(outline(14, 2, brown_border))
                .with_debug_name("promote_special"))) {
      gold_coins += 25;
      status_message = daily_specials[selected_special] + " promoted";
    }

    // Clock icon on Promote button - positioned to not overlap text
    if (clock_tex.id != 0) {
      afterhours::texture_manager::Rectangle clock_src{
          0, 0, (float)clock_tex.width, (float)clock_tex.height};
      sprite(context, mk(entity, 121), clock_tex, clock_src,
             ComponentConfig{}
                 .with_size(ComponentSize{pxf(33), pyf(33)})
                 .with_ignore_pointer_events()
                 .with_absolute_position(ax(479.0f), ay(promote_y + 7.0f))
                 .with_debug_name("clock_icon"));
    }

    // ========== RIGHT PANEL: Customers ==========
    // Connected to left panel via consistent panel_gap
    float right_panel_x = 644.0f;
    float right_panel_w = 490.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(right_panel_w), pyf(panel_h)})
            .with_absolute_position(ax(right_panel_x), ay(panel_y))
            .with_custom_background(panel_right)
            .with_border(afterhours::Color{116, 83, 56, 255}, 3.0f)
            .with_soft_shadow(4.0f, 7.0f, 0.0f,
                              afterhours::Color{110, 83, 52, 55})
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(36.f * sy)
            .with_on_draw_fg(outline(36, 3, brown_border))
            .with_debug_name("customers_panel"));

    // Sage header
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Customers")
            .with_size(ComponentSize{pxf(right_panel_w), pyf(72)})
            .with_absolute_position(ax(right_panel_x), ay(panel_y + 3.0f))
            .with_background(Theme::Usage::None)
            .with_font("GaeguMock", h720(43.0f))
            .with_custom_text_color(cream_surface)
            .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
            .with_letter_spacing(0.f)
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(-2.f * sy)
            .with_debug_name("customers_title"));

    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(453.f), pyf(292.f)})
            .with_absolute_position(ax(661.0f), ay(203.0f))
            .with_custom_background(afterhours::Color{255, 249, 227, 255})
            .with_border(afterhours::Color{158, 130, 91, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(22.f * sy)
            .with_on_draw_fg(outline(22, 2, {158, 130, 91, 255}))
            .with_debug_name("customers_inner"));

    // Customer rows
    float cust_y = 247.0f;
    for (size_t i = 0; i < waiting_customers.size(); i++) {
      auto &c = waiting_customers[i];
      float row_y = cust_y + (float)i * 90.0f;

      // Name - Order (own line, full width so it never collides with the
      // patience row below it)
      div(context, mk(entity, 210 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(c.name + " - " + c.order)
              .with_size(ComponentSize{pxf(300), pyf(34)})
              .with_absolute_position(ax(696.0f), ay(row_y))
              .with_font("GaeguMock", h720(28.0f))
              .with_custom_text_color(dark_text)
              .with_letter_spacing(0.f)
              .with_debug_name("cust_" + std::to_string(i)));

      // Patience row sits BELOW the name
      float prow_y = row_y + 42.0f;

      // Patience label with warning text for low patience
      bool low_patience = c.progress < 0.35f;
      std::string patience_text = low_patience ? "Patience: LOW" : "Patience:";
      div(context, mk(entity, 215 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(patience_text)
              .with_size(ComponentSize{pxf(120), pyf(22)})
              .with_absolute_position(ax(696.0f), ay(prow_y + 4.0f))
              .with_font("GaeguMock", h720(14.0f))
              .with_letter_spacing(0.f)
              .with_custom_text_color(low_patience ? badge_red
                                                   : theme.font_muted));

      // Progress bar bg with label
      div(context, mk(entity, 211 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(162), pyf(8)})
              .with_absolute_position(ax(803.0f), ay(prow_y + 8.0f))
              .with_custom_background(afterhours::Color{211, 195, 158, 255})
              .with_border(afterhours::Color{164, 147, 112, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(25.f * sy)
              .with_debug_name("prog_bg_" + std::to_string(i)));

      // Progress fill
      if (c.progress > 0.0f) {
        div(context, mk(entity, 212 + static_cast<int>(i) * 10),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(160 * c.progress), pyf(6)})
                .with_absolute_position(ax(804.0f), ay(prow_y + 9.0f))
                .with_custom_background(
                    i == 0 ? afterhours::Color{154, 172, 124, 255}
                           : afterhours::Color{202, 151, 128, 255})
                .with_rounded_corners(RoundedCorners())
                .with_corner_radius(25.f * sy)
                .with_debug_name("prog_fill_" + std::to_string(i)));
      }

      // Time badge (minimum 44px for touch)
      afterhours::Color time_bg = (c.wait_time > 3) ? rose_btn : panel_right;
      div(context, mk(entity, 213 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(std::to_string(c.wait_time) + "m")
              .with_size(ComponentSize{pxf(56), pyf(32)})
              .with_absolute_position(ax(1039.0f), ay(row_y - 2.0f))
              .with_custom_background(time_bg)
              .with_custom_text_color(cream_surface)
              .with_border(afterhours::Color{124, 91, 60, 255}, 2.0f)
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(6.f * sy)
              .with_alignment(TextAlignment::Center)
              .with_font("GaeguMock", h720(20.0f))
              .with_letter_spacing(0.f)
              .with_debug_name("time_" + std::to_string(i)));
    }

    // Serve Next button (minimum 44px touch target)
    float serve_btn_y = 421.0f;
    if (button(
            context, mk(entity, 250),
            ComponentConfig{}
                .with_label("Serve Next")
                .with_size(ComponentSize{pxf(355), pyf(53)})
                .with_absolute_position(ax(709.0f), ay(serve_btn_y))
                .with_custom_background(rose_btn)
                .with_border(afterhours::Color{124, 91, 60, 255}, 2.0f)
                .with_soft_shadow(2.0f, 3.0f, 10.0f,
                                  afterhours::Color{0, 0, 0, 40})
                .with_font("GaeguMock", h720(37.0f))
                .with_custom_text_color(cream_surface)
                .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
                .with_letter_spacing(0.f)
                .with_rounded_corners(RoundedCorners())
                .with_corner_radius(25.f * sy)
                .with_alignment(TextAlignment::Center)
                .with_disabled(waiting_customers.empty())
                .with_on_draw_fg(outline(25, 2.5, brown_border))
                .with_debug_name("serve_next"))) {
      customers_today += 1;
      gold_coins += 12;
      status_message = "Served " + waiting_customers.front().name + "'s order";
      waiting_customers.erase(waiting_customers.begin());
    }

    if (waiting_customers.empty()) {
      div(context, mk(entity, 260),
          ComponentConfig{}
              .with_label("All orders served")
              .with_size({pxf(355), pyf(48)})
              .with_absolute_position(ax(709), ay(292))
              .with_font("GaeguMock", h720(28))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center));
    }

    // ========== SEPARATOR: Between main panels and bottom sections ==========
    div(context, mk(entity, 299),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(978), pyf(1)})
            .with_absolute_position(ax(150.0f), ay(panel_y + panel_h + 8.0f))
            .with_custom_background(afterhours::Color{130, 110, 90, 0})
            .with_debug_name("section_separator_1"));

    // ========== MUSIC SLIDER ==========
    // Connected to main layout via left_panel_x
    float slider_y = 533.0f;
    float slider_w = 378.0f;

    slider(context, mk(entity, 300), music_volume,
           ComponentConfig{}
               .with_size(ComponentSize{pxf(slider_w), pyf(26)})
               .with_absolute_position(ax(155.0f), ay(slider_y))
               .with_custom_background(afterhours::Color{0, 0, 0, 0})
               .with_rounded_corners(RoundedCorners())
               .with_corner_radius(25.f * sy)
               .with_debug_name("music_slider"),
           SliderHandleValueLabelPosition::None);

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(slider_w), pyf(17)})
            .with_absolute_position(ax(155.0f), ay(slider_y + 2.0f))
            .with_custom_background(afterhours::Color{116, 81, 53, 255})
            .with_border(afterhours::Color{119, 86, 56, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(25.f * sy)
            .with_ignore_pointer_events()
            .with_debug_name("music_track"));

    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(slider_w * music_volume), pyf(11)})
            .with_absolute_position(ax(159.0f), ay(slider_y + 5.0f))
            .with_custom_background(panel_right)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(25.f * sy)
            .with_ignore_pointer_events()
            .with_debug_name("music_fill"));

    div(context, mk(entity, 304),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(27), pyf(30)})
            .with_absolute_position(
                ax(155.0f + (slider_w - 27.0f) * music_volume),
                ay(slider_y - 4.0f))
            .with_custom_background(panel_right)
            .with_border(afterhours::Color{124, 91, 60, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_ignore_pointer_events()
            .with_debug_name("music_handle"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Music: " +
                        std::to_string(
                            static_cast<int>(std::round(music_volume * 100))) +
                        "%")
            .with_size(ComponentSize{pxf(180), pyf(30)})
            .with_absolute_position(ax(157.0f), ay(575.0f))
            .with_font("GaeguMock", h720(27.0f))
            .with_custom_text_color(cream_surface)
            .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
            .with_letter_spacing(0.f)
            .with_debug_name("music_label"));

    // ========== CHAT BOX ==========
    float chat_y = 603.0f;
    float chat_w = 543.0f;

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(chat_w), pyf(75)})
            .with_absolute_position(ax(152.0f), ay(chat_y))
            .with_custom_background(chat_bg)
            .with_border(afterhours::Color{121, 91, 61, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * sy)
            .with_on_draw_fg(outline(8, 2, brown_border))
            .with_debug_name("chat_box"));

    // Chat messages with avatars - data-driven
    struct ChatMsg {
      int avatar_id;
      int text_id;
      float y;
      raylib::Texture2D *tex;
      const char *fallback;
      afterhours::Color fallback_bg;
      const char *message;
    };
    float chat_line1_y = chat_y + 10.0f;
    float chat_line2_y = chat_y + 37.0f;
    ChatMsg chat_msgs[] = {
        {405,
         401,
         chat_line1_y,
         &avatar_guildmate_tex,
         "G",
         {182, 201, 155, 255},
         status_message.c_str()},
        {406,
         402,
         chat_line2_y,
         &avatar_devteam_tex,
         "D",
         {214, 163, 167, 255},
         "DevTeam_Support: Check out the new update!"},
    };
    for (auto &cm : chat_msgs) {
      div(context, mk(entity, cm.avatar_id),
          ComponentConfig{}
              .with_label(cm.fallback)
              .with_size({pxf(21), pyf(21)})
              .with_absolute_position(ax(164.f), ay(cm.y + 5.f))
              .with_custom_background(cm.fallback_bg)
              .with_font("Atkinson", h720(12))
              .with_custom_text_color({100, 91, 61, 255})
              .with_roundness(1)
              .with_alignment(TextAlignment::Center));
      div(context, mk(entity, cm.text_id),
          ComponentConfig{}
              .with_label(cm.message)
              .with_size(ComponentSize{pxf(500), pyf(28)})
              .with_absolute_position(ax(193.0f), ay(cm.y + 1.0f))
              .with_font("Archivo", h720(22.0f))
              .with_custom_text_color(cream_surface)
              .with_letter_spacing(0.f)
              .with_debug_name(cm.text_id == 401 ? "chat_status"
                                                 : "chat_support"));
    }

    // ========== BOTTOM RIGHT: Icons with Badges ==========
    // Connected to right panel via consistent positioning
    float icon_x = 835.0f;
    float icon_y = 590.0f;
    float icon_size = 64.0f; // Minimum 44px for touch targets
    float icon_img_size = 52.0f;
    float icon_offset = (icon_size - icon_img_size) / 2.0f;
    float icon_spacing = 109.0f;

    // ========== SEPARATOR: Between chat and icons ==========
    div(context, mk(entity, 499),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(1), pyf(80)})
            .with_absolute_position(ax(icon_x - 20.0f), ay(chat_y + 2.0f))
            .with_custom_background(afterhours::Color{130, 110, 90, 0})
            .with_debug_name("section_separator_2"));

    // Bottom-right icon buttons - data-driven
    struct IconBtn {
      int base_id;
      raylib::Texture2D *tex;
      const char *label;
      const char *badge;
    };
    IconBtn icon_btns[] = {
        {500, &icon_inventory_tex, "Inventory", "2"},
        {510, &icon_research_tex, "Research", "!"},
        {520, &icon_crafting_tex, "Crafting", nullptr},
    };
    for (size_t ib = 0; ib < 3; ib++) {
      auto &btn = icon_btns[ib];
      float bx = icon_x + (float)ib * icon_spacing;

      if (button(
              context, mk(entity, btn.base_id),
              ComponentConfig{}
                  .with_size(ComponentSize{pxf(icon_size), pyf(icon_size)})
                  .with_absolute_position(ax(bx), ay(icon_y))
                  .with_custom_background(afterhours::Color{250, 237, 206, 255})
                  .with_border(brown_border, 2.0f)
                  .with_rounded_corners(RoundedCorners())
                  .with_corner_radius(14.f * sy)
                  .with_debug_name(std::string("tool_") + btn.label))) {
        status_message = std::string(btn.label) + " opened";
      }
      sprite(context, mk(entity, btn.base_id + 1), *btn.tex,
             {0, 0, (float)btn.tex->width, (float)btn.tex->height},
             ComponentConfig{}
                 .with_size({pxf(icon_img_size), pyf(icon_img_size)})
                 .with_absolute_position(ax(bx + icon_offset),
                                         ay(icon_y + icon_offset))
                 .with_ignore_pointer_events());
      if (btn.badge) {
        ui_workarounds::notification_badge(
            context, entity, btn.base_id + 2, btn.badge,
            ax(bx + icon_size - 14.0f), ay(icon_y - 5.0f), ax(22.0f),
            badge_red);
      }
      div(context, mk(entity, btn.base_id + 3),
          ComponentConfig{}
              .with_label(btn.label)
              .with_size(ComponentSize{pxf(icon_size + 44), pyf(22)})
              .with_absolute_position(ax(bx - 22.0f),
                                      ay(icon_y + icon_size + 8.0f))
              .with_font("GaeguMock", h720(20.0f))
              .with_custom_text_color(cream_surface)
              .with_text_stroke(afterhours::Color{107, 78, 52, 255}, .65f * sy)
              .with_letter_spacing(0.f)
              .with_alignment(TextAlignment::Center));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(cozy_cafe, "Game Mockups",
                        "Cozy cafe simulation game menu", CozyCafeScreen)
