#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/TextOutline.h"
#include "../../ui_workarounds/GradientBackground.h"
#include "../../ui_workarounds/NotificationBadge.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct EmpireTycoonScreen : ScreenSystem<UIContext<InputAction>> {
  int64_t cash = 1250980;
  size_t selected_tab = 0;  // Navigation tab selection
  
  // Loaded textures
  bool textures_loaded = false;
  raylib::Texture2D coin_tex{};
  raylib::Texture2D diamond_tex{};
  raylib::Texture2D star_trophy_tex{};
  raylib::Texture2D sparkle_tex{};
  raylib::Texture2D icon_happiness_tex{};
  raylib::Texture2D icon_resources_tex{};
  raylib::Texture2D icon_rides_tex{};
  raylib::Texture2D icon_food_tex{};
  raylib::Texture2D icon_upgrades_tex{};
  raylib::Texture2D icon_finance_tex{};
  raylib::Texture2D icon_shop_tex{};
  raylib::Texture2D icon_settings_tex{};
  raylib::Texture2D mascot_tex{};
  raylib::Texture2D cloud_tex{};

  void load_textures_if_needed() {
    if (textures_loaded) return;
    textures_loaded = true;
    
    std::string images_path = afterhours::files::get_resource_path("images", "").string();
    coin_tex = raylib::LoadTexture((images_path + "icon_coin_small.png").c_str());
    diamond_tex = raylib::LoadTexture((images_path + "icon_diamond.png").c_str());
    star_trophy_tex = raylib::LoadTexture((images_path + "icon_star_trophy.png").c_str());
    sparkle_tex = raylib::LoadTexture((images_path + "sparkle.png").c_str());
    icon_happiness_tex = raylib::LoadTexture((images_path + "icon_happiness.png").c_str());
    icon_resources_tex = raylib::LoadTexture((images_path + "icon_resources.png").c_str());
    icon_rides_tex = raylib::LoadTexture((images_path + "icon_rides.png").c_str());
    icon_food_tex = raylib::LoadTexture((images_path + "icon_food.png").c_str());
    icon_upgrades_tex = raylib::LoadTexture((images_path + "icon_upgrades.png").c_str());
    icon_finance_tex = raylib::LoadTexture((images_path + "icon_finance.png").c_str());
    icon_shop_tex = raylib::LoadTexture((images_path + "icon_shop.png").c_str());
    icon_settings_tex = raylib::LoadTexture((images_path + "icon_settings.png").c_str());
    mascot_tex = raylib::LoadTexture((images_path + "mascot_business.png").c_str());
    cloud_tex = raylib::LoadTexture((images_path + "cloud_white.png").c_str());
  }
  float happiness_pct = 0.85f;
  float resources_pct = 0.60f;
  float milestone_pct = 0.65f;

  struct Production {
    std::string name;
    int rate;
  };

  std::vector<Production> production = {
      {"Toys", 5200},
      {"Snacks", 3800},
      {"Gadgets", 3100},
  };

  struct Project {
    std::string name;
    int progress;
    int rate;
  };

  std::vector<Project> projects = {
      {"New Rollercoasser", 80, 0},
      {"Expand", 35, 0},
      {"Develop Robot Masoot", 10, 3100},
  };

  // Colors matching Dream Incorporated exactly
  afterhours::Color sky_top{125, 175, 220, 255};
  afterhours::Color sky_bottom{185, 200, 225, 255};
  afterhours::Color lavender_bg{190, 175, 200, 255};  // Purple tint at bottom
  afterhours::Color panel_blue{125, 190, 240, 255};
  afterhours::Color panel_blue_light{175, 215, 250, 255};
  afterhours::Color border_blue{85, 155, 205, 255};
  afterhours::Color white{255, 255, 255, 255};
  afterhours::Color dark_text{55, 75, 105, 255};
  afterhours::Color muted_text{100, 130, 170, 255};
  afterhours::Color btn_yellow{255, 200, 75, 255};
  afterhours::Color btn_yellow_dark{215, 165, 45, 255};
  afterhours::Color happy_green{110, 195, 115, 255};

  // Tab colors - pastel
  afterhours::Color tab_blue{195, 225, 255, 255};
  afterhours::Color tab_green{195, 230, 195, 255};
  afterhours::Color tab_pink{255, 205, 205, 255};
  afterhours::Color tab_purple{225, 205, 250, 255};
  afterhours::Color tab_cream{255, 235, 205, 255};

  std::string format_money(int64_t amt) {
    std::string s = std::to_string(amt);
    std::string result;
    int count = 0;
    for (int i = (int)s.length() - 1; i >= 0; i--) {
      if (count > 0 && count % 3 == 0)
        result = "," + result;
      result = s[i] + result;
      count++;
    }
    return "$" + result;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();
    
    Theme theme;
    theme.font = dark_text;
    theme.darkfont = dark_text;
    theme.font_muted = muted_text;
    theme.background = sky_top;
    theme.surface = white;
    theme.primary = panel_blue;
    theme.secondary = border_blue;
    theme.accent = btn_yellow;
    theme.error = afterhours::Color{240, 100, 100, 255};
    theme.roundness = 0.25f;
    theme.segments = 10;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND: Sky gradient with lavender at bottom ==========
    ui_workarounds::vertical_gradient(context, entity, 1, 0, 0,
        static_cast<float>(screen_w), static_cast<float>(screen_h) * 0.7f,
        sky_top, sky_bottom, 10);

    // Lavender/purple bottom section
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(static_cast<int>(screen_h * 0.35f))})
            .with_absolute_position()
            .with_translate(0.0f, static_cast<float>(screen_h) * 0.65f)
            .with_custom_background(lavender_bg)
            .with_debug_name("lavender_bg"));

    // Decorative clouds
    if (cloud_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)cloud_tex.width, (float)cloud_tex.height};
      sprite(context, mk(entity, 6), cloud_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(80), pixels(40)})
              .with_absolute_position()
              .with_translate((float)screen_w - 130.0f, 15.0f)
              .with_opacity(0.6f)
              .with_debug_name("cloud1"));
      sprite(context, mk(entity, 7), cloud_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(60), pixels(30)})
              .with_absolute_position()
              .with_translate((float)screen_w - 200.0f, 55.0f)
              .with_opacity(0.4f)
              .with_debug_name("cloud2"));
    }

    // ========== TITLE: DREAM INCORPORATED (large puffy 3D text) ==========
    // Each text_with_outline uses 9 entity IDs (8 outline + 1 main)
    // So we space them out: 10-18, 20-28, 30-38, 40-48
    
    // DREAM - large white with thick blue outline and shadow effect
    // Shadow layer first (dark blue, offset)
    ui_workarounds::text_with_outline(
        context, entity, 10,
        "DREAM",
        32.0f, 22.0f, 380.0f, 85.0f,
        "Fredoka", 78.0f,
        afterhours::Color{45, 90, 140, 180},
        afterhours::Color{35, 70, 115, 150},
        6.0f,
        TextAlignment::Left
    );
    
    // Main DREAM text
    ui_workarounds::text_with_outline(
        context, entity, 20,
        "DREAM",
        28.0f, 15.0f, 380.0f, 85.0f,
        "Fredoka", 78.0f,
        white,
        border_blue,
        6.0f,
        TextAlignment::Left
    );

    // Shadow for INCORPORATED
    ui_workarounds::text_with_outline(
        context, entity, 30,
        "INCORPORATED",
        32.0f, 100.0f, 480.0f, 60.0f,
        "Fredoka", 52.0f,
        afterhours::Color{35, 75, 125, 180},
        afterhours::Color{25, 55, 95, 150},
        5.0f,
        TextAlignment::Left
    );
    
    // Main INCORPORATED text
    ui_workarounds::text_with_outline(
        context, entity, 40,
        "INCORPORATED",
        28.0f, 95.0f, 480.0f, 60.0f,
        "Fredoka", 52.0f,
        afterhours::Color{100, 185, 240, 255},
        afterhours::Color{45, 115, 175, 255},
        5.0f,
        TextAlignment::Left
    );

    // ========== TOP RIGHT: Currency ==========
    // IDs 10-49 are used by title text_with_outline calls, start at 50
    float cur_x = (float)screen_w - 255.0f;

    // Currency pill
    div(context, mk(entity, 55),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(225), pixels(52)})
            .with_absolute_position()
            .with_translate(cur_x, 18.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("currency_pill"));

    // Gold coin
    if (coin_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)coin_tex.width, (float)coin_tex.height};
      sprite(context, mk(entity, 56), coin_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(36), pixels(36)})
              .with_absolute_position()
              .with_translate(cur_x + 12.0f, 26.0f)
              .with_debug_name("coin"));
    } else {
      div(context, mk(entity, 56),
          ComponentConfig{}
              .with_label("*")
              .with_size(ComponentSize{pixels(38), pixels(38)})
              .with_absolute_position()
              .with_translate(cur_x + 10.0f, 25.0f)
              .with_custom_background(btn_yellow)
              .with_border(btn_yellow_dark, 2.0f)
              .with_font("EqProRounded", 22.0f)
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(1.0f)
              .with_debug_name("coin"));
    }

    div(context, mk(entity, 57),
        ComponentConfig{}
            .with_label(format_money(cash))
            .with_size(ComponentSize{pixels(165), pixels(38)})
            .with_absolute_position()
            .with_translate(cur_x + 55.0f, 25.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("money"));

    // ========== STATUS ICONS ==========
    float stat_y = 70.0f;
    float stat_icon_size = 58.0f;

    // Happiness icon - larger and more prominent
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(stat_icon_size)), pixels(static_cast<int>(stat_icon_size))})
            .with_absolute_position()
            .with_translate((float)screen_w - 210.0f, stat_y)
            .with_custom_background(afterhours::Color{255, 220, 150, 255})
            .with_border(afterhours::Color{220, 180, 100, 255}, 3.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("happy_bg"));
    if (icon_happiness_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_happiness_tex.width, (float)icon_happiness_tex.height};
      sprite(context, mk(entity, 61), icon_happiness_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(44), pixels(44)})
              .with_absolute_position()
              .with_translate((float)screen_w - 203.0f, stat_y + 7.0f)
              .with_debug_name("happy_icon"));
    }

    // Resource gauge icon - larger and more prominent
    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(stat_icon_size)), pixels(static_cast<int>(stat_icon_size))})
            .with_absolute_position()
            .with_translate((float)screen_w - 85.0f, stat_y)
            .with_custom_background(afterhours::Color{180, 210, 245, 255})
            .with_border(afterhours::Color{140, 175, 215, 255}, 3.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("gauge_bg"));
    if (icon_resources_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_resources_tex.width, (float)icon_resources_tex.height};
      sprite(context, mk(entity, 63), icon_resources_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(44), pixels(44)})
              .with_absolute_position()
              .with_translate((float)screen_w - 78.0f, stat_y + 7.0f)
              .with_debug_name("resource_icon"));
    }

    // ========== METERS ==========
    // IDs 80-95 for meters
    float meter_y = 143.0f;

    // Happiness meter
    div(context, mk(entity, 80),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(175), pixels(34)})
            .with_absolute_position()
            .with_translate((float)screen_w - 340.0f, meter_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("happiness_pill"));

    div(context, mk(entity, 81),
        ComponentConfig{}
            .with_label(":)")
            .with_size(ComponentSize{pixels(22), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 335.0f, meter_y + 6.0f)
            .with_custom_background(afterhours::Color{255, 210, 130, 255})
            .with_font("EqProRounded", 12.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("happy_icon"));

    div(context, mk(entity, 82),
        ComponentConfig{}
            .with_label("Happiness")
            .with_size(ComponentSize{pixels(70), pixels(20)})
            .with_absolute_position()
            .with_translate((float)screen_w - 305.0f, meter_y + 8.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("happy_text"));

    // Happiness bar
    div(context, mk(entity, 83),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(48), pixels(18)})
            .with_absolute_position()
            .with_translate((float)screen_w - 218.0f, meter_y + 8.0f)
            .with_custom_background(afterhours::Color{225, 230, 235, 255})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("happy_bar_bg"));

    div(context, mk(entity, 84),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(44 * happiness_pct)), pixels(16)})
            .with_absolute_position()
            .with_translate((float)screen_w - 216.0f, meter_y + 9.0f)
            .with_custom_background(happy_green)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("happy_bar_fill"));

    // Resources meter
    div(context, mk(entity, 85),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(175), pixels(34)})
            .with_absolute_position()
            .with_translate((float)screen_w - 155.0f, meter_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("resources_pill"));

    div(context, mk(entity, 86),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(22), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 150.0f, meter_y + 6.0f)
            .with_custom_background(afterhours::Color{195, 215, 240, 255})
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("res_icon"));

    div(context, mk(entity, 87),
        ComponentConfig{}
            .with_label("Resources")
            .with_size(ComponentSize{pixels(75), pixels(20)})
            .with_absolute_position()
            .with_translate((float)screen_w - 120.0f, meter_y + 8.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("res_text"));

    // ========== LEFT: Navigation Tabs ==========
    // Center the main content area
    float content_width = 850.0f;
    float content_margin = ((float)screen_w - content_width) / 2.0f;
    
    std::vector<std::tuple<raylib::Texture2D*, std::string, std::string, afterhours::Color>> tabs = {
        {&icon_rides_tex, "[R]", "Rides", tab_blue},
        {&icon_food_tex, "[F]", "Food Stalls", tab_green},
        {&icon_upgrades_tex, "[!]", "Upgrades", tab_pink},
        {&icon_upgrades_tex, "[X]", "Upgrades", tab_purple},
        {&icon_finance_tex, "[$]", "Finance", tab_cream},
    };

    float nav_x = content_margin - 10.0f;  // Slightly left of content area
    float nav_y = 195.0f;
    float tab_width = 115.0f;
    float tab_height = 78.0f;
    float tab_spacing = 85.0f;
    for (size_t i = 0; i < tabs.size(); i++) {
      float tab_y = nav_y + (float)i * tab_spacing;
      auto& [tex_ptr, fallback, label, bg_color] = tabs[i];

      // Tab button background - larger with thicker border
      bool tab_selected = (i == selected_tab);
      afterhours::Color border_color = tab_selected 
          ? afterhours::Color{255, 200, 50, 255}   // Gold border when selected
          : afterhours::Color{140, 160, 180, 255};
      float border_width = tab_selected ? 5.0f : 3.0f;
      
      if (button(context, mk(entity, 100 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(static_cast<int>(tab_width)), pixels(static_cast<int>(tab_height))})
                 .with_absolute_position()
                 .with_translate(nav_x, tab_y)
                 .with_custom_background(bg_color)
                 .with_border(border_color, border_width)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.25f)
                 .with_soft_shadow(3.0f, 5.0f, 12.0f, afterhours::Color{0, 0, 0, 50})
                 .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }

      // Icon image or fallback text - larger
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width, (float)tex_ptr->height};
        sprite(context, mk(entity, 110 + static_cast<int>(i)), *tex_ptr, src,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(44), pixels(44)})
                .with_absolute_position()
                .with_translate(nav_x + tab_width/2.0f - 22.0f, tab_y + 6.0f)
                .with_debug_name("tab_icon_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 110 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(fallback)
                .with_size(ComponentSize{pixels(50), pixels(40)})
                .with_absolute_position()
                .with_translate(nav_x + tab_width/2.0f - 25.0f, tab_y + 10.0f)
                .with_font("EqProRounded", 22.0f)
                .with_custom_text_color(dark_text)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("tab_icon_fallback_" + std::to_string(i)));
      }

      // Tab label - positioned below icon
      div(context, mk(entity, 120 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(static_cast<int>(tab_width)), pixels(22)})
              .with_absolute_position()
              .with_translate(nav_x, tab_y + tab_height - 26.0f)
              .with_font("EqProRounded", 15.0f)
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("tab_label_" + std::to_string(i)));

      // Notification badge on "Upgrades" tab
      if (i == 2) {
        ui_workarounds::notification_badge(context, entity, 130 + static_cast<int>(i),
            "!", nav_x + tab_width - 5.0f, tab_y - 5.0f, 22.0f,
            afterhours::Color{230, 90, 80, 255});
      }
    }

    // ========== MAIN PANEL ==========
    float panel_x = content_margin + 110.0f; // Offset for left tabs
    float panel_y = 185.0f;
    float panel_w = content_width - 120.0f;  // Panel width minus left tabs
    float panel_h = 420.0f;

    // Main panel background - bigger with thicker border to match inspiration
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)), pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 6.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("main_panel"));

    // Production Overview section
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("Production Overview")
            .with_size(ComponentSize{pixels(200), pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + 20.0f)
            .with_font("EqProRounded", 21.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("prod_header"));

    // Production box
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(170)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + 55.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 210, 225, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("prod_box"));

    // Production items
    for (size_t i = 0; i < production.size(); i++) {
      float item_y = panel_y + 75.0f + (float)i * 48.0f;

      div(context, mk(entity, 220 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(production[i].name + ": " + 
                         std::to_string(production[i].rate) + "/min")
              .with_size(ComponentSize{pixels(230), pixels(32)})
              .with_absolute_position()
              .with_translate(panel_x + 50.0f, item_y)
              .with_font("EqProRounded", 17.0f)
              .with_custom_text_color(dark_text)
              .with_debug_name("prod_" + std::to_string(i)));

      // Up arrow
      div(context, mk(entity, 230 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label("^")
              .with_size(ComponentSize{pixels(25), pixels(25)})
              .with_absolute_position()
              .with_translate(panel_x + 275.0f, item_y + 3.0f)
              .with_font("EqProRounded", 22.0f)
              .with_custom_text_color(happy_green)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("prod_arrow_" + std::to_string(i)));
    }

    // Current Projects section
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_label("Current Projects")
            .with_size(ComponentSize{pixels(180), pixels(28)})
            .with_absolute_position()
            .with_translate(panel_x + 350.0f, panel_y + 20.0f)
            .with_font("EqProRounded", 21.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("proj_header"));

    // Projects box
    div(context, mk(entity, 251),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(310), pixels(170)})
            .with_absolute_position()
            .with_translate(panel_x + 335.0f, panel_y + 55.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 210, 225, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("proj_box"));

    // Project items
    for (size_t i = 0; i < projects.size(); i++) {
      float item_y = panel_y + 75.0f + (float)i * 48.0f;

      std::string proj_text = projects[i].name + " - " + 
          std::to_string(projects[i].progress) + "%";
      if (projects[i].rate > 0) {
        proj_text = projects[i].name + " - " + 
            std::to_string(projects[i].rate) + "/min";
      }

      div(context, mk(entity, 260 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(proj_text)
              .with_size(ComponentSize{pixels(270), pixels(32)})
              .with_absolute_position()
              .with_translate(panel_x + 355.0f, item_y)
              .with_font("EqProRounded", 15.0f)
              .with_custom_text_color(dark_text)
              .with_debug_name("proj_" + std::to_string(i)));

      div(context, mk(entity, 270 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label("^")
              .with_size(ComponentSize{pixels(25), pixels(25)})
              .with_absolute_position()
              .with_translate(panel_x + 610.0f, item_y + 3.0f)
              .with_font("EqProRounded", 22.0f)
              .with_custom_text_color(happy_green)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("proj_arrow_" + std::to_string(i)));
    }

    // ========== ACTION BUTTONS ==========
    std::vector<std::string> btn_labels = {"Speed Up", "Prioritize", "New Project"};
    float btn_y = panel_y + panel_h - 75.0f;
    float btn_w = 170.0f;
    float btn_spacing = 190.0f;

    for (size_t i = 0; i < btn_labels.size(); i++) {
      button(context, mk(entity, 300 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(btn_labels[i])
                 .with_size(ComponentSize{pixels(static_cast<int>(btn_w)), pixels(58)})
                 .with_absolute_position()
                 .with_translate(panel_x + 50.0f + (float)i * btn_spacing, btn_y)
                 .with_custom_background(btn_yellow)
                 .with_border(btn_yellow_dark, 4.0f)
                 .with_font("EqProRounded", 22.0f)
                 .with_custom_text_color(dark_text)
                 .with_alignment(TextAlignment::Center)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(0.3f)
                 .with_soft_shadow(3.0f, 5.0f, 12.0f, afterhours::Color{0, 0, 0, 60})
                 .with_debug_name("btn_" + std::to_string(i)));
    }

    // ========== BOTTOM: Chat ==========
    float bottom_y = (float)screen_h - 90.0f;

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(275), pixels(65)})
            .with_absolute_position()
            .with_translate(22.0f, bottom_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("chat_box"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("GlobalChat: New area unlocked!")
            .with_size(ComponentSize{pixels(255), pixels(20)})
            .with_absolute_position()
            .with_translate(32.0f, bottom_y + 12.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("chat1"));

    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("DevTeam: Update v2.0 is live")
            .with_size(ComponentSize{pixels(255), pixels(20)})
            .with_absolute_position()
            .with_translate(32.0f, bottom_y + 36.0f)
            .with_font("EqProRounded", 13.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("chat2"));

    // ========== BOTTOM: Next Milestone ==========
    float ms_x = (float)screen_w / 2.0f - 45.0f;

    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_label("Next Milstome")
            .with_size(ComponentSize{pixels(130), pixels(22)})
            .with_absolute_position()
            .with_translate(ms_x, (float)screen_h - 85.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("milestone_label"));

    // Milestone bar
    div(context, mk(entity, 411),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(26)})
            .with_absolute_position()
            .with_translate((float)screen_w / 2.0f - 115.0f, (float)screen_h - 55.0f)
            .with_custom_background(white)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("milestone_bg"));

    div(context, mk(entity, 412),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(268 * milestone_pct)), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w / 2.0f - 111.0f, (float)screen_h - 53.0f)
            .with_custom_background(btn_yellow)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("milestone_fill"));

    // ========== BOTTOM RIGHT: Icons ==========
    std::vector<std::tuple<raylib::Texture2D*, std::string, std::string>> icon_data = {
        {&icon_shop_tex, "$", "Shop"},
        {&icon_settings_tex, "@", "Settings"},
        {&star_trophy_tex, "#", "Leaderboards"}
    };
    float icon_x = (float)screen_w - 270.0f;
    float icon_size = 52.0f;
    float icon_img_size = 32.0f;
    float icon_offset = (icon_size - icon_img_size) / 2.0f;

    for (size_t i = 0; i < icon_data.size(); i++) {
      float ix = icon_x + (float)i * 82.0f;
      auto& [tex_ptr, fallback, label] = icon_data[i];

      // Button background
      button(context, mk(entity, 500 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(icon_size), pixels(icon_size)})
                 .with_absolute_position()
                 .with_translate(ix, (float)screen_h - 92.0f)
                 .with_custom_background(btn_yellow)
                 .with_border(btn_yellow_dark, 2.0f)
                 .with_rounded_corners(std::bitset<4>(0b1111))
                 .with_roundness(1.0f)
                 .with_debug_name("icon_btn_" + std::to_string(i)));

      // Icon image or fallback text
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width, (float)tex_ptr->height};
        sprite(context, mk(entity, 520 + static_cast<int>(i)), *tex_ptr, src,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(icon_img_size), pixels(icon_img_size)})
                .with_absolute_position()
                .with_translate(ix + icon_offset, (float)screen_h - 92.0f + icon_offset)
                .with_debug_name("icon_img_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 520 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(fallback)
                .with_size(ComponentSize{pixels(icon_size), pixels(icon_size)})
                .with_absolute_position()
                .with_translate(ix, (float)screen_h - 92.0f)
                .with_font("EqProRounded", 26.0f)
                .with_custom_text_color(dark_text)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("icon_fallback_" + std::to_string(i)));
      }

      // Label below icon
      div(context, mk(entity, 510 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(85), pixels(18)})
              .with_absolute_position()
              .with_translate(ix - 16.0f, (float)screen_h - 32.0f)
              .with_font("EqProRounded", 12.0f)
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("icon_label_" + std::to_string(i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups",
                        "Bright cartoon tycoon game", EmpireTycoonScreen)
