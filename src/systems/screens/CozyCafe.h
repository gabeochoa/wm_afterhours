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
      return raylib::LoadTexture((images_path + name).c_str());
    };
    star_filled_tex = load("star_filled.png");
    star_empty_tex = load("star_empty.png");
    clock_tex = load("clock_icon.png");
    flower_tex = load("flower_blossom.png");
    avatar_guildmate_tex = load("avatar_guildmate.png");
    avatar_devteam_tex = load("avatar_devteam.png");
    icon_inventory_tex = load("icon_inventory.png");
    icon_research_tex = load("icon_research.png");
    icon_crafting_tex = load("icon_crafting.png");
  }

  std::vector<std::string> daily_specials = {"Lavender Latte", "Honey Toast",
                                             "Matcha Cake"};

  // Colors matching the inspiration exactly
  afterhours::Color kraft_bg{215, 195, 170, 255};      // Kraft paper tan
  afterhours::Color kraft_frame{160, 130, 100, 255};   // Darker kraft border
  afterhours::Color cream_surface{255, 250, 240, 255}; // Warm cream white
  afterhours::Color brown_border{130, 110, 90, 255};   // Panel border brown
  afterhours::Color brown_header{130, 105, 85, 255};   // Title bar brown
  afterhours::Color sage_header{145, 175, 150, 255};   // Customers header sage
  afterhours::Color pink_btn{245, 195, 185, 255};      // Lavender Latte pink
  afterhours::Color mint_btn{195, 220, 185, 255};      // Honey Toast mint
  afterhours::Color tan_btn{225, 200, 165, 255};       // Matcha Cake tan
  afterhours::Color rose_btn{235, 180, 175, 255};      // Serve Next rose
  afterhours::Color dark_text{55, 40, 30, 255};        // Dark sepia text
  afterhours::Color badge_red{170, 70, 70, 255};       // Notification badge

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

    UIStylingDefaults::get().set_default_font("Gaegu-Bold", h720(18.0f));
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
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== DECORATIVE KRAFT PAPER FRAME ==========
    decorative_frame(
        context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<float>(screen_w)),
                                     pixels(static_cast<float>(screen_h))})
            .with_custom_background(kraft_bg)
            .with_border(kraft_frame, h720(14.0f)),
        DecorativeFrameStyle::KraftPaper);

    // ========== HEADER ==========
    // Title with flower symbol
    // Flower icon
    if (flower_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)flower_tex.width,
                                                 (float)flower_tex.height};
      sprite(context, mk(entity, 9), flower_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(38), pixels(38)})
                 .with_absolute_position(35.0f, 28.0f)
                 .with_debug_name("flower_icon"));
    }

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Blossom Cafe")
            .with_size(ComponentSize{pixels(220), pixels(50)})
            .with_absolute_position(flower_tex.id != 0 ? 78.0f : 35.0f, 28.0f)
            .with_font("Gaegu-Bold", h720(36.0f))
            .with_custom_text_color(dark_text));

    // Gold pill background
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(145), pixels(38)})
            .with_absolute_position(370.0f, 28.0f)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("gold_pill"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Gold:  $" + std::to_string(gold_coins))
            .with_size(ComponentSize{pixels(130), pixels(28)})
            .with_absolute_position(377.0f, 33.0f)
            .with_custom_text_color(dark_text));

    // Rating box - widened to fit all content including numeric rating
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(310), pixels(58)})
            .with_absolute_position(530.0f, 20.0f)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(RoundedCorners()));

    // Rating label
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Rating:")
            .with_size(ComponentSize{pixels(60), pixels(22)})
            .with_absolute_position(540.0f, 26.0f)
            .with_font("Gaegu-Bold", h720(14.0f))
            .with_custom_text_color(dark_text));

    // Star rating icons (4 filled, 1 empty = 4/5 stars)
    afterhours::Color star_gold{215, 175, 95, 255};
    afterhours::Color star_empty_color{190, 180, 165, 255};
    float star_x = 600.0f;
    float star_sz = 28.0f;
    float star_gap = 32.0f;
    for (int i = 0; i < 5; i++) {
      bool is_filled = (i < 4);
      auto &star_tex = is_filled ? star_filled_tex : star_empty_tex;
      if (star_tex.id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)star_tex.width,
                                                   (float)star_tex.height};
        sprite(
            context, mk(entity, 33 + i), star_tex, src,
            ComponentConfig{}
                .with_720p_size(star_sz, star_sz)
                .with_absolute_position(star_x + (float)i * star_gap, 20.0f));
      } else {
        div(context, mk(entity, 33 + i),
            ComponentConfig{}
                .with_720p_size(star_sz, star_sz)
                .with_absolute_position(star_x + (float)i * star_gap, 20.0f)
                .with_custom_background(is_filled ? star_gold
                                                  : star_empty_color)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.3f));
      }
    }

    // Numeric rating display "4/5" for clarity
    div(context, mk(entity, 38),
        ComponentConfig{}
            .with_label("4/5")
            .with_size(ComponentSize{pixels(46), pixels(28)})
            .with_absolute_position(star_x + 5 * star_gap + 6.0f, 20.0f)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(dark_text));

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Customers Served: " + std::to_string(customers_today))
            .with_size(ComponentSize{pixels(200), pixels(20)})
            .with_absolute_position(540.0f, 50.0f)
            .with_font("Gaegu-Bold", h720(14.0f))
            .with_custom_text_color(dark_text));

    // ========== LEFT PANEL: Today's Specials ==========
    // Center content to better match square inspiration on widescreen
    // Use percent widths for cohesive layout grid
    float content_width = (float)screen_w * 0.85f;
    float content_margin = ((float)screen_w - content_width) / 2.0f;
    float left_panel_x = content_margin;
    float panel_y = 95.0f;
    float left_panel_w = content_width * 0.45f; // 45% of content width
    float panel_h = 420.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_720p_size(left_panel_w, panel_h)
            .with_absolute_position(left_panel_x, panel_y)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 3.0f)
            .with_soft_shadow(4.0f, 6.0f, 18.0f, afterhours::Color{0, 0, 0, 65})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_debug_name("specials_panel"));

    // Brown header
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("Today's Specials")
            .with_size(ComponentSize{pxf(left_panel_w - 6), pixels(56)})
            .with_absolute_position(left_panel_x + 3.0f, panel_y + 3.0f)
            .with_custom_background(brown_header)
            .with_font("Gaegu-Bold", h720(30.0f))
            .with_custom_text_color(cream_surface)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center));

    // Menu items - styled as pill buttons with colored fills (minimum 44px
    // touch targets)
    afterhours::Color menu_colors[] = {pink_btn, mint_btn, tan_btn};
    float menu_y = panel_y + 75.0f;
    float menu_btn_w = left_panel_w - 50.0f;
    for (size_t i = 0; i < daily_specials.size(); i++) {
      bool selected = (i == selected_special);
      afterhours::Color btn_bg = menu_colors[i];

      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(daily_specials[i])
                     .with_size(ComponentSize{pxf(menu_btn_w), pixels(56)})
                     .with_absolute_position(left_panel_x + 25.0f,
                                             menu_y + (float)i * 68.0f)
                     .with_custom_background(btn_bg)
                     .with_border(brown_border, selected ? 3.0f : 2.0f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                       afterhours::Color{0, 0, 0, 35})
                     .with_font("Gaegu-Bold", h720(26.0f))
                     .with_custom_text_color(dark_text)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.5f)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("special_" + std::to_string(i)))) {
        selected_special = i;
      }
    }

    // Promote Special button - widened for full text visibility (minimum 44px
    // touch target)
    float promote_y = panel_y + panel_h - 75.0f;
    button(context, mk(entity, 120),
           ComponentConfig{}
               .with_label("Promote Special")
               .with_size(ComponentSize{pxf(menu_btn_w), pixels(50)})
               .with_absolute_position(left_panel_x + 25.0f, promote_y)
               .with_custom_background(cream_surface)
               .with_border(brown_border, 2.0f)
               .with_font("Gaegu-Bold", h720(22.0f))
               .with_custom_text_color(dark_text)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center));

    // Clock icon on Promote button - positioned to not overlap text
    if (clock_tex.id != 0) {
      afterhours::texture_manager::Rectangle clock_src{
          0, 0, (float)clock_tex.width, (float)clock_tex.height};
      sprite(context, mk(entity, 121), clock_tex, clock_src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(24), pixels(24)})
                 .with_absolute_position(left_panel_x + menu_btn_w - 8.0f,
                                         promote_y + 11.0f)
                 .with_debug_name("clock_icon"));
    }

    // ========== RIGHT PANEL: Customers ==========
    // Connected to left panel via consistent panel_gap
    float panel_gap = 24.0f;
    float right_panel_x = left_panel_x + left_panel_w + panel_gap;
    float right_panel_w = content_width - left_panel_w - panel_gap;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_720p_size(right_panel_w, panel_h)
            .with_absolute_position(right_panel_x, panel_y)
            .with_custom_background(cream_surface)
            .with_border(sage_header, 4.0f)
            .with_soft_shadow(4.0f, 6.0f, 18.0f, afterhours::Color{0, 0, 0, 65})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_debug_name("customers_panel"));

    // Sage header
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Customers")
            .with_size(ComponentSize{pxf(right_panel_w - 8), pixels(56)})
            .with_absolute_position(right_panel_x + 4.0f, panel_y + 4.0f)
            .with_custom_background(sage_header)
            .with_font("Gaegu-Bold", h720(32.0f))
            .with_custom_text_color(dark_text)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_alignment(TextAlignment::Center));

    // Customer rows
    float cust_y = panel_y + 80.0f;
    for (size_t i = 0; i < waiting_customers.size(); i++) {
      auto &c = waiting_customers[i];
      float row_y = cust_y + (float)i * 90.0f;

      // Name - Order (18.0f minimum font)
      div(context, mk(entity, 210 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(c.name + " - " + c.order)
              .with_size(ComponentSize{pixels(180), pixels(36)})
              .with_absolute_position(right_panel_x + 20.0f, row_y)
              .with_font("Gaegu-Bold", h720(24.0f))
              .with_custom_text_color(dark_text)
              .with_debug_name("cust_" + std::to_string(i)));

      // Patience label with warning text for low patience
      bool low_patience = c.progress < 0.35f;
      std::string patience_text = low_patience ? "Patience: LOW" : "Patience:";
      div(context, mk(entity, 215 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(patience_text)
              .with_size(ComponentSize{pixels(120), pixels(22)})
              .with_absolute_position(right_panel_x + 175.0f, row_y + 6.0f)
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(low_patience ? badge_red
                                                   : theme.font_muted));

      // Progress bar bg with label
      div(context, mk(entity, 211 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(84), pixels(30)})
              .with_absolute_position(right_panel_x + 270.0f, row_y + 3.0f)
              .with_custom_background(brown_header)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.4f)
              .with_debug_name("prog_bg_" + std::to_string(i)));

      // Progress fill
      if (c.progress > 0.0f) {
        div(context, mk(entity, 212 + static_cast<int>(i) * 10),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(76 * c.progress), pixels(24)})
                .with_absolute_position(right_panel_x + 274.0f, row_y + 6.0f)
                .with_custom_background(afterhours::Color{175, 200, 165, 255})
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.4f)
                .with_debug_name("prog_fill_" + std::to_string(i)));
      }

      // Progress percentage label (always shown for clarity)
      {
        int prog_val = static_cast<int>(c.progress * 100);
        div(context, mk(entity, 214 + static_cast<int>(i) * 10),
            ComponentConfig{}
                .with_label(std::to_string(prog_val) + "%")
                .with_size(ComponentSize{pixels(54), pixels(24)})
                .with_absolute_position(right_panel_x + 285.0f, row_y + 6.0f)
                .with_font("Gaegu-Bold", h720(15.0f))
                .with_custom_text_color(cream_surface)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("prog_pct_" + std::to_string(i)));
      }

      // Time badge (minimum 44px for touch)
      afterhours::Color time_bg = (c.wait_time > 3) ? rose_btn : brown_header;
      div(context, mk(entity, 213 + static_cast<int>(i) * 10),
          ComponentConfig{}
              .with_label(std::to_string(c.wait_time) + "m")
              .with_size(ComponentSize{pixels(52), pixels(32)})
              .with_absolute_position(right_panel_x + 365.0f, row_y + 2.0f)
              .with_custom_background(time_bg)
              .with_custom_text_color(cream_surface)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.4f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("time_" + std::to_string(i)));
    }

    // Serve Next button (minimum 44px touch target)
    float serve_btn_y = panel_y + panel_h - 75.0f;
    button(
        context, mk(entity, 250),
        ComponentConfig{}
            .with_label("Serve Next")
            .with_size(ComponentSize{pxf(right_panel_w - 40), pixels(56)})
            .with_absolute_position(right_panel_x + 20.0f, serve_btn_y)
            .with_custom_background(rose_btn)
            .with_border(sage_header, 3.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 40})
            .with_font("Gaegu-Bold", h720(30.0f))
            .with_custom_text_color(dark_text)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_alignment(TextAlignment::Center));

    // ========== SEPARATOR: Between main panels and bottom sections ==========
    div(context, mk(entity, 299),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(content_width * 0.9f), pixels(1)})
            .with_absolute_position(left_panel_x + content_width * 0.05f,
                                    panel_y + panel_h + 8.0f)
            .with_custom_background(afterhours::Color{130, 110, 90, 60})
            .with_debug_name("section_separator_1"));

    // ========== MUSIC SLIDER ==========
    // Connected to main layout via left_panel_x
    float slider_y = panel_y + panel_h + 18.0f;
    float slider_w = 320.0f;

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(slider_w), pixels(28)})
            .with_absolute_position(left_panel_x, slider_y)
            .with_custom_background(brown_header)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("slider_bg"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_size(ComponentSize{
                pixels(static_cast<int>((slider_w - 20) * music_volume)),
                pixels(22)})
            .with_absolute_position(left_panel_x + 3.0f, slider_y + 3.0f)
            .with_custom_background(sage_header)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("slider_fill"));

    // Slider handle - increased size for better visibility (minimum 44px touch
    // target)
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(28), pixels(36)})
            .with_absolute_position(
                left_panel_x + (slider_w - 28) * music_volume, slider_y - 4.0f)
            .with_custom_background(sage_header)
            .with_border(brown_header, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("slider_handle"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Music: " +
                        std::to_string(static_cast<int>(music_volume * 100)) +
                        "%")
            .with_size(ComponentSize{pixels(140), pixels(26)})
            .with_absolute_position(left_panel_x, slider_y + 36.0f)
            .with_custom_text_color(dark_text));

    // ========== CHAT BOX ==========
    float chat_y = slider_y + 65.0f;
    float chat_w = 420.0f; // Widened to show full messages

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(chat_w), pixels(85)})
            .with_absolute_position(left_panel_x, chat_y)
            .with_custom_background(cream_surface)
            .with_border(brown_border, 2.0f)
            .with_rounded_corners(RoundedCorners())
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
    float chat_line2_y = chat_y + 45.0f;
    ChatMsg chat_msgs[] = {
        {405,
         401,
         chat_line1_y,
         &avatar_guildmate_tex,
         "@",
         {180, 160, 140, 255},
         "Guildmate23: need help with the recipe?"},
        {406,
         402,
         chat_line2_y,
         &avatar_devteam_tex,
         "D",
         {120, 150, 180, 255},
         "DevTeam_Support: Check out the new update!"},
    };
    for (auto &cm : chat_msgs) {
      if (cm.tex && cm.tex->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)cm.tex->width,
                                                   (float)cm.tex->height};
        sprite(context, mk(entity, cm.avatar_id), *cm.tex, src,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(26), pixels(26)})
                   .with_absolute_position(left_panel_x + 10.0f, cm.y));
      } else {
        div(context, mk(entity, cm.avatar_id),
            ComponentConfig{}
                .with_label(cm.fallback)
                .with_size(ComponentSize{pixels(24), pixels(24)})
                .with_absolute_position(left_panel_x + 10.0f, cm.y)
                .with_custom_background(cm.fallback_bg)
                .with_font("Gaegu-Bold", h720(14.0f))
                .with_custom_text_color(cream_surface)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(1.0f)
                .with_alignment(TextAlignment::Center));
      }
      div(context, mk(entity, cm.text_id),
          ComponentConfig{}
              .with_label(cm.message)
              .with_size(ComponentSize{pixels(370), pixels(28)})
              .with_absolute_position(left_panel_x + 42.0f, cm.y + 2.0f)
              .with_font("Gaegu-Bold", h720(14.0f))
              .with_custom_text_color(dark_text));
    }

    // ========== BOTTOM RIGHT: Icons with Badges ==========
    // Connected to right panel via consistent positioning
    float icon_x = right_panel_x + right_panel_w - 250.0f;
    float icon_y = chat_y + 5.0f;
    float icon_size = 60.0f; // Minimum 44px for touch targets
    float icon_img_size = 40.0f;
    float icon_offset = (icon_size - icon_img_size) / 2.0f;
    float icon_spacing = 82.0f;

    // ========== SEPARATOR: Between chat and icons ==========
    div(context, mk(entity, 499),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1), pixels(80)})
            .with_absolute_position(icon_x - 20.0f, chat_y + 2.0f)
            .with_custom_background(afterhours::Color{130, 110, 90, 60})
            .with_debug_name("section_separator_2"));

    // Bottom-right icon buttons - data-driven
    struct IconBtn {
      int base_id;
      raylib::Texture2D *tex;
      const char *label;
      const char *badge;
      const char *hint;
      float hint_extra_w;
    };
    IconBtn icon_btns[] = {
        {500, &icon_inventory_tex, "Inventory", "2", "2 new items", 24.0f},
        {510, &icon_research_tex, "Research", "!", "Ready!", 30.0f},
        {520, &icon_crafting_tex, "Crafting", nullptr, nullptr, 0.0f},
    };
    for (size_t ib = 0; ib < 3; ib++) {
      auto &btn = icon_btns[ib];
      float bx = icon_x + (float)ib * icon_spacing;

      button(context, mk(entity, btn.base_id),
             ComponentConfig{}
                 .with_720p_size(icon_size, icon_size)
                 .with_absolute_position(bx, icon_y)
                 .with_custom_background(cream_surface)
                 .with_border(brown_border, 2.0f)
                 .with_rounded_corners(RoundedCorners()));
      if (btn.tex && btn.tex->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)btn.tex->width,
                                                   (float)btn.tex->height};
        sprite(context, mk(entity, btn.base_id + 1), *btn.tex, src,
               ComponentConfig{}
                   .with_720p_size(icon_img_size, icon_img_size)
                   .with_absolute_position(bx + icon_offset,
                                           icon_y + icon_offset));
      }
      if (btn.badge) {
        ui_workarounds::notification_badge(context, entity, btn.base_id + 2,
                                           btn.badge, bx + icon_size - 14.0f,
                                           icon_y - 5.0f, 22.0f, badge_red);
      }
      div(context, mk(entity, btn.base_id + 3),
          ComponentConfig{}
              .with_label(btn.label)
              .with_size(ComponentSize{pxf(icon_size + 16), pixels(22)})
              .with_absolute_position(bx - 8.0f, icon_y + icon_size + 4.0f)
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center));
      if (btn.hint) {
        div(context, mk(entity, btn.base_id + 4),
            ComponentConfig{}
                .with_label(btn.hint)
                .with_size(ComponentSize{pxf(icon_size + btn.hint_extra_w),
                                         pixels(18)})
                .with_absolute_position(bx - btn.hint_extra_w / 2.0f,
                                        icon_y + icon_size + 24.0f)
                .with_font("Gaegu-Bold", h720(14.0f))
                .with_custom_text_color(theme.font_muted)
                .with_alignment(TextAlignment::Center));
      }
    }

    // ========== DECORATIVE TAPE (bottom right corner) ==========
    div(context, mk(entity, 600),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(40), pixels(18)})
            .with_absolute_position(static_cast<float>(screen_w) - 55.0f,
                                    static_cast<float>(screen_h) - 30.0f)
            .with_custom_background(afterhours::Color{230, 215, 180, 180})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f)
            .with_debug_name("tape"));
  }
};

REGISTER_EXAMPLE_SCREEN(cozy_cafe, "Game Mockups",
                        "Cozy cafe simulation game menu", CozyCafeScreen)
