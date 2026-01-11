#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/DecorativeFrame.h"
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
    if (textures_loaded) return;
    textures_loaded = true;
    
    std::string images_path = afterhours::files::get_resource_path("images", "").string();
    star_filled_tex = raylib::LoadTexture((images_path + "star_filled.png").c_str());
    star_empty_tex = raylib::LoadTexture((images_path + "star_empty.png").c_str());
    clock_tex = raylib::LoadTexture((images_path + "clock_icon.png").c_str());
    flower_tex = raylib::LoadTexture((images_path + "flower_blossom.png").c_str());
    avatar_guildmate_tex = raylib::LoadTexture((images_path + "avatar_guildmate.png").c_str());
    avatar_devteam_tex = raylib::LoadTexture((images_path + "avatar_devteam.png").c_str());
    icon_inventory_tex = raylib::LoadTexture((images_path + "icon_inventory.png").c_str());
    icon_research_tex = raylib::LoadTexture((images_path + "icon_research.png").c_str());
    icon_crafting_tex = raylib::LoadTexture((images_path + "icon_crafting.png").c_str());
  }

  std::vector<std::string> daily_specials = {"Lavender Latte", "Honey Toast",
                                             "Matcha Cake"};

  // Colors matching the inspiration exactly
  afterhours::Color kraft_bg{215, 195, 170, 255};       // Kraft paper tan
  afterhours::Color kraft_frame{160, 130, 100, 255};    // Darker kraft border
  afterhours::Color cream_surface{255, 250, 240, 255};  // Warm cream white
  afterhours::Color brown_border{130, 110, 90, 255};    // Panel border brown
  afterhours::Color brown_header{130, 105, 85, 255};    // Title bar brown
  afterhours::Color sage_header{145, 175, 150, 255};    // Customers header sage
  afterhours::Color pink_btn{245, 195, 185, 255};       // Lavender Latte pink
  afterhours::Color mint_btn{195, 220, 185, 255};       // Honey Toast mint  
  afterhours::Color tan_btn{225, 200, 165, 255};        // Matcha Cake tan
  afterhours::Color rose_btn{235, 180, 175, 255};       // Serve Next rose
  afterhours::Color dark_text{55, 40, 30, 255};         // Dark sepia text
  afterhours::Color badge_red{170, 70, 70, 255};        // Notification badge

  struct Customer {
    std::string name;
    std::string order;
    int wait_time;
    float progress;
  };

  std::vector<Customer> waiting_customers = {
      {"Emma", "Cappuccio", 2, 0.5f},
      {"Oliver", "Croissant", 5, 0.0f},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();
    
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
    theme.roundness = 0.15f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== DECORATIVE KRAFT PAPER FRAME ==========
    ui_workarounds::kraft_paper_frame(
        context, entity, 1,
        static_cast<float>(screen_w), static_cast<float>(screen_h),
        kraft_frame, kraft_bg, 14.0f);

    // ========== HEADER ==========
    // Title with flower symbol
    // Flower icon
    if (flower_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)flower_tex.width, (float)flower_tex.height};
      sprite(context, mk(entity, 9), flower_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(38), pixels(38)})
              .with_absolute_position()
              .with_translate(35.0f, 28.0f)
              .with_debug_name("flower_icon"));
    }
    
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Blossom Cafe")
            .with_size(ComponentSize{pixels(220), pixels(50)})
            .with_absolute_position()
            .with_translate(flower_tex.id != 0 ? 78.0f : 35.0f, 28.0f)
            .with_font("Gaegu-Bold", 36.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("title"));

    // Gold pill background
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(145), pixels(38)})
            .with_absolute_position()
            .with_translate(370.0f, 28.0f)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("gold_pill"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Gold:  $" + std::to_string(gold_coins))
            .with_size(ComponentSize{pixels(130), pixels(28)})
            .with_absolute_position()
            .with_translate(377.0f, 33.0f)
            .with_font("Gaegu-Bold", 18.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("gold_text"));

    // Rating box
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(185), pixels(52)})
            .with_absolute_position()
            .with_translate(560.0f, 23.0f)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("rating_box"));

    // Rating label
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Rating:")
            .with_size(ComponentSize{pixels(55), pixels(20)})
            .with_absolute_position()
            .with_translate(570.0f, 30.0f)
            .with_font("Gaegu-Bold", 14.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("rating_label"));
    
    // Star rating icons (4 filled, 1 empty = 4/5 stars)
    // Using colored rectangles as simple star representation
    afterhours::Color star_gold{215, 175, 95, 255};
    afterhours::Color star_empty_color{190, 180, 165, 255};
    float star_x = 628.0f;
    for (int i = 0; i < 5; i++) {
      bool is_filled = (i < 4);
      div(context, mk(entity, 33 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(16), pixels(16)})
              .with_absolute_position()
              .with_translate(star_x + (float)i * 19.0f, 30.0f)
              .with_custom_background(is_filled ? star_gold : star_empty_color)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.3f)
              .with_debug_name("star_" + std::to_string(i)));
    }

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Customers Served: " + std::to_string(customers_today))
            .with_size(ComponentSize{pixels(170), pixels(18)})
            .with_absolute_position()
            .with_translate(570.0f, 50.0f)
            .with_font("Gaegu-Bold", 13.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("served"));

    // ========== LEFT PANEL: Today's Specials ==========
    // Center content to better match square inspiration on widescreen
    float content_width = 820.0f;
    float content_margin = ((float)screen_w - content_width) / 2.0f;
    float left_panel_x = content_margin;
    float panel_y = 95.0f;
    float left_panel_w = 360.0f;
    float panel_h = 400.0f;
    
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(left_panel_w)), pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(left_panel_x, panel_y)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 3.0f)
            .with_soft_shadow(4.0f, 6.0f, 18.0f, afterhours::Color{0, 0, 0, 65})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("specials_panel"));

    // Brown header
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("Today's Specials")
            .with_size(ComponentSize{pixels(static_cast<int>(left_panel_w - 6)), pixels(52)})
            .with_absolute_position()
            .with_translate(left_panel_x + 3.0f, panel_y + 3.0f)
            .with_custom_background(brown_header)
            .with_font("Gaegu-Bold", 28.0f)
            .with_custom_text_color(cream_surface)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("specials_header"));

    // Menu items - styled as pill buttons with colored fills
    afterhours::Color menu_colors[] = {pink_btn, mint_btn, tan_btn};
    float menu_y = panel_y + 70.0f;
    float menu_btn_w = left_panel_w - 50.0f;
    for (size_t i = 0; i < daily_specials.size(); i++) {
      bool selected = (i == selected_special);
      afterhours::Color btn_bg = menu_colors[i];

      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(daily_specials[i])
                     .with_size(ComponentSize{pixels(static_cast<int>(menu_btn_w)), pixels(52)})
                     .with_absolute_position()
                     .with_translate(left_panel_x + 25.0f, menu_y + (float)i * 62.0f)
                     .with_custom_background(btn_bg)
                     .with_border(brown_border, selected ? 3.0f : 2.0f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 35})
                     .with_font("Gaegu-Bold", 24.0f)
                     .with_custom_text_color(dark_text)
                     .with_rounded_corners(std::bitset<4>(0b1111))
                     .with_roundness(0.5f)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("special_" + std::to_string(i)))) {
        selected_special = i;
      }
    }

    // Promote Special button
    float promote_y = panel_y + panel_h - 70.0f;
    button(context, mk(entity, 120),
           ComponentConfig{}
               .with_label("Promote Special")
               .with_size(ComponentSize{pixels(static_cast<int>(menu_btn_w - 40)), pixels(46)})
               .with_absolute_position()
               .with_translate(left_panel_x + 25.0f, promote_y)
               .with_custom_background(cream_surface)
               .with_border(brown_border, 2.0f)
               .with_font("Gaegu-Bold", 20.0f)
               .with_custom_text_color(dark_text)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("promote"));
    
    // Clock icon on Promote button
    if (clock_tex.id != 0) {
      afterhours::texture_manager::Rectangle clock_src{0, 0, (float)clock_tex.width, (float)clock_tex.height};
      sprite(context, mk(entity, 121), clock_tex, clock_src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(26), pixels(26)})
              .with_absolute_position()
              .with_translate(left_panel_x + menu_btn_w - 35.0f, promote_y + 10.0f)
              .with_debug_name("clock_icon"));
    }

    // ========== RIGHT PANEL: Customers ==========
    float panel_gap = 20.0f;
    float right_panel_x = left_panel_x + left_panel_w + panel_gap;
    float right_panel_w = content_width - left_panel_w - panel_gap;
    
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(right_panel_w)), pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(right_panel_x, panel_y)
            .with_custom_background(cream_surface)
            .with_border(sage_header, 4.0f)
            .with_soft_shadow(4.0f, 6.0f, 18.0f, afterhours::Color{0, 0, 0, 65})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("customers_panel"));

    // Sage header
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Customers")
            .with_size(ComponentSize{pixels(static_cast<int>(right_panel_w - 8)), pixels(52)})
            .with_absolute_position()
            .with_translate(right_panel_x + 4.0f, panel_y + 4.0f)
            .with_custom_background(sage_header)
            .with_font("Gaegu-Bold", 30.0f)
            .with_custom_text_color(dark_text)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("customers_header"));

    // Customer rows
    float cust_y = panel_y + 75.0f;
    for (size_t i = 0; i < waiting_customers.size(); i++) {
      auto &c = waiting_customers[i];
      float row_y = cust_y + (float)i * 80.0f;

      // Name - Order
      div(context, mk(entity, 210 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(c.name + " - " + c.order)
              .with_size(ComponentSize{pixels(220), pixels(32)})
              .with_absolute_position()
              .with_translate(right_panel_x + 20.0f, row_y)
              .with_font("Gaegu-Bold", 22.0f)
              .with_custom_text_color(dark_text)
              .with_debug_name("cust_" + std::to_string(i)));

      // Progress bar bg
      div(context, mk(entity, 211 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(70), pixels(24)})
              .with_absolute_position()
              .with_translate(right_panel_x + 255.0f, row_y + 4.0f)
              .with_custom_background(brown_header)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.4f)
              .with_debug_name("prog_bg_" + std::to_string(i)));

      // Progress fill
      if (c.progress > 0.0f) {
        div(context, mk(entity, 212 + static_cast<int>(i) * 10),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(62 * c.progress)),
                                         pixels(18)})
                .with_absolute_position()
                .with_translate(right_panel_x + 259.0f, row_y + 7.0f)
                .with_custom_background(afterhours::Color{175, 200, 165, 255})
                .with_rounded_corners(std::bitset<4>(0b1111))
                .with_roundness(0.4f)
                .with_debug_name("prog_fill_" + std::to_string(i)));
      }

      // Time badge
      afterhours::Color time_bg = (c.wait_time > 3) ? rose_btn : brown_header;
      div(context, mk(entity, 213 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(std::to_string(c.wait_time) + "m")
              .with_size(ComponentSize{pixels(48), pixels(28)})
              .with_absolute_position()
              .with_translate(right_panel_x + 340.0f, row_y + 2.0f)
              .with_custom_background(time_bg)
              .with_font("Gaegu-Bold", 16.0f)
              .with_custom_text_color(cream_surface)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.4f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("time_" + std::to_string(i)));
    }

    // Serve Next button
    float serve_btn_y = panel_y + panel_h - 70.0f;
    button(context, mk(entity, 250),
           ComponentConfig{}
               .with_label("Serve Next")
               .with_size(ComponentSize{pixels(static_cast<int>(right_panel_w - 40)), pixels(52)})
               .with_absolute_position()
               .with_translate(right_panel_x + 20.0f, serve_btn_y)
               .with_custom_background(rose_btn)
               .with_border(sage_header, 3.0f)
               .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 40})
               .with_font("Gaegu-Bold", 28.0f)
               .with_custom_text_color(dark_text)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("serve_next"));

    // ========== MUSIC SLIDER ==========
    float slider_y = panel_y + panel_h + 15.0f;
    float slider_w = 300.0f;
    
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(slider_w)), pixels(24)})
            .with_absolute_position()
            .with_translate(left_panel_x, slider_y)
            .with_custom_background(brown_header)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("slider_bg"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>((slider_w - 20) * music_volume)),
                                     pixels(18)})
            .with_absolute_position()
            .with_translate(left_panel_x + 3.0f, slider_y + 3.0f)
            .with_custom_background(sage_header)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("slider_fill"));

    // Slider handle
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(20), pixels(28)})
            .with_absolute_position()
            .with_translate(left_panel_x + (slider_w - 20) * music_volume, slider_y - 2.0f)
            .with_custom_background(sage_header)
            .with_border(brown_header, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("slider_handle"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Music: " + std::to_string(static_cast<int>(music_volume * 100)) + "%")
            .with_size(ComponentSize{pixels(120), pixels(24)})
            .with_absolute_position()
            .with_translate(left_panel_x, slider_y + 32.0f)
            .with_font("Gaegu-Bold", 18.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("music_label"));

    // ========== CHAT BOX ==========
    float chat_y = slider_y + 65.0f;
    float chat_w = 350.0f;
    
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(chat_w)), pixels(70)})
            .with_absolute_position()
            .with_translate(left_panel_x, chat_y)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("chat_box"));

    // Chat avatar 1 (guildmate)
    float chat_line1_y = chat_y + 10.0f;
    float chat_line2_y = chat_y + 38.0f;
    
    if (avatar_guildmate_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)avatar_guildmate_tex.width, (float)avatar_guildmate_tex.height};
      sprite(context, mk(entity, 405), avatar_guildmate_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(26), pixels(26)})
              .with_absolute_position()
              .with_translate(left_panel_x + 10.0f, chat_line1_y)
              .with_debug_name("avatar1"));
    } else {
      div(context, mk(entity, 405),
          ComponentConfig{}
              .with_label("@")
              .with_size(ComponentSize{pixels(24), pixels(24)})
              .with_absolute_position()
              .with_translate(left_panel_x + 10.0f, chat_line1_y)
              .with_custom_background(afterhours::Color{180, 160, 140, 255})
              .with_font("Gaegu-Bold", 12.0f)
              .with_custom_text_color(cream_surface)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(1.0f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("avatar1"));
    }

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("Guildmate23: need help with t...")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(left_panel_x + 42.0f, chat_line1_y + 2.0f)
            .with_font("Gaegu-Bold", 14.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("chat1"));

    // Chat avatar 2 (devteam)
    if (avatar_devteam_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)avatar_devteam_tex.width, (float)avatar_devteam_tex.height};
      sprite(context, mk(entity, 406), avatar_devteam_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(26), pixels(26)})
              .with_absolute_position()
              .with_translate(left_panel_x + 10.0f, chat_line2_y)
              .with_debug_name("avatar2"));
    } else {
      div(context, mk(entity, 406),
          ComponentConfig{}
              .with_label("D")
              .with_size(ComponentSize{pixels(24), pixels(24)})
              .with_absolute_position()
              .with_translate(left_panel_x + 10.0f, chat_line2_y)
              .with_custom_background(afterhours::Color{120, 150, 180, 255})
              .with_font("Gaegu-Bold", 12.0f)
              .with_custom_text_color(cream_surface)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(1.0f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("avatar2"));
    }

    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("DevTeam_Support: Check out the new...")
            .with_size(ComponentSize{pixels(300), pixels(22)})
            .with_absolute_position()
            .with_translate(left_panel_x + 42.0f, chat_line2_y + 2.0f)
            .with_font("Gaegu-Bold", 14.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("chat2"));

    // ========== BOTTOM RIGHT: Icons with Badges ==========
    float icon_x = right_panel_x + right_panel_w - 230.0f;
    float icon_y = chat_y + 5.0f;
    float icon_size = 58.0f;
    float icon_img_size = 36.0f;
    float icon_offset = (icon_size - icon_img_size) / 2.0f;
    float icon_spacing = 78.0f;
    
    // Inventory icon with badge
    button(context, mk(entity, 500),
           ComponentConfig{}
               .with_size(ComponentSize{pixels(static_cast<int>(icon_size)), pixels(static_cast<int>(icon_size))})
               .with_absolute_position()
               .with_translate(icon_x, icon_y)
               .with_custom_background(cream_surface)
               .with_border(brown_border, 2.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("inventory_btn"));
    if (icon_inventory_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_inventory_tex.width, (float)icon_inventory_tex.height};
      sprite(context, mk(entity, 501), icon_inventory_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(icon_img_size)), pixels(static_cast<int>(icon_img_size))})
              .with_absolute_position()
              .with_translate(icon_x + icon_offset, icon_y + icon_offset)
              .with_debug_name("inventory_icon"));
    }
    ui_workarounds::notification_badge(context, entity, 502, "2", 
        icon_x + icon_size - 14.0f, icon_y - 5.0f, 20.0f, badge_red);
    div(context, mk(entity, 503),
        ComponentConfig{}
            .with_label("Inventory")
            .with_size(ComponentSize{pixels(static_cast<int>(icon_size + 10)), pixels(18)})
            .with_absolute_position()
            .with_translate(icon_x - 5.0f, icon_y + icon_size + 4.0f)
            .with_font("Gaegu-Bold", 13.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("inventory_label"));

    // Research icon with badge
    button(context, mk(entity, 510),
           ComponentConfig{}
               .with_size(ComponentSize{pixels(static_cast<int>(icon_size)), pixels(static_cast<int>(icon_size))})
               .with_absolute_position()
               .with_translate(icon_x + icon_spacing, icon_y)
               .with_custom_background(cream_surface)
               .with_border(brown_border, 2.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("research_btn"));
    if (icon_research_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_research_tex.width, (float)icon_research_tex.height};
      sprite(context, mk(entity, 511), icon_research_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(icon_img_size)), pixels(static_cast<int>(icon_img_size))})
              .with_absolute_position()
              .with_translate(icon_x + icon_spacing + icon_offset, icon_y + icon_offset)
              .with_debug_name("research_icon"));
    }
    ui_workarounds::notification_badge(context, entity, 512, "!", 
        icon_x + icon_spacing + icon_size - 14.0f, icon_y - 5.0f, 20.0f, badge_red);
    div(context, mk(entity, 513),
        ComponentConfig{}
            .with_label("Research")
            .with_size(ComponentSize{pixels(static_cast<int>(icon_size + 10)), pixels(18)})
            .with_absolute_position()
            .with_translate(icon_x + icon_spacing - 5.0f, icon_y + icon_size + 4.0f)
            .with_font("Gaegu-Bold", 13.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("research_label"));

    // Crafting icon (no badge)
    button(context, mk(entity, 520),
           ComponentConfig{}
               .with_size(ComponentSize{pixels(static_cast<int>(icon_size)), pixels(static_cast<int>(icon_size))})
               .with_absolute_position()
               .with_translate(icon_x + icon_spacing * 2, icon_y)
               .with_custom_background(cream_surface)
               .with_border(brown_border, 2.0f)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_debug_name("crafting_btn"));
    if (icon_crafting_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_crafting_tex.width, (float)icon_crafting_tex.height};
      sprite(context, mk(entity, 521), icon_crafting_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(icon_img_size)), pixels(static_cast<int>(icon_img_size))})
              .with_absolute_position()
              .with_translate(icon_x + icon_spacing * 2 + icon_offset, icon_y + icon_offset)
              .with_debug_name("crafting_icon"));
    }
    div(context, mk(entity, 522),
        ComponentConfig{}
            .with_label("Crafting")
            .with_size(ComponentSize{pixels(static_cast<int>(icon_size + 10)), pixels(18)})
            .with_absolute_position()
            .with_translate(icon_x + icon_spacing * 2 - 5.0f, icon_y + icon_size + 4.0f)
            .with_font("Gaegu-Bold", 13.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("crafting_label"));

    // ========== DECORATIVE TAPE (bottom right corner) ==========
    div(context, mk(entity, 600),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(40), pixels(18)})
            .with_absolute_position()
            .with_translate(static_cast<float>(screen_w) - 55.0f, 
                           static_cast<float>(screen_h) - 30.0f)
            .with_custom_background(afterhours::Color{230, 215, 180, 180})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("tape"));
  }
};

REGISTER_EXAMPLE_SCREEN(cozy_cafe, "Game Mockups",
                        "Cozy cafe simulation game menu", CozyCafeScreen)
