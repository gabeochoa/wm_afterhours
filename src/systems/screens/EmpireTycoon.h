#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/GradientBackground.h"
#include "../../ui_workarounds/NotificationBadge.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct EmpireTycoonScreen : ScreenSystem<UIContext<InputAction>> {
  int64_t cash = 1250980;
  size_t selected_tab = 0; // Navigation tab selection

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
    if (textures_loaded)
      return;
    textures_loaded = true;

    std::string images_path =
        afterhours::files::get_resource_path("images", "").string();
    auto load = [&](const char *name) {
      return raylib::LoadTexture((images_path + name).c_str());
    };
    coin_tex = load("icon_coin_small.png");
    diamond_tex = load("icon_diamond.png");
    star_trophy_tex = load("icon_star_trophy.png");
    sparkle_tex = load("sparkle.png");
    icon_happiness_tex = load("icon_happiness.png");
    icon_resources_tex = load("icon_resources.png");
    icon_rides_tex = load("icon_rides.png");
    icon_food_tex = load("icon_food.png");
    icon_upgrades_tex = load("icon_upgrades.png");
    icon_finance_tex = load("icon_finance.png");
    icon_shop_tex = load("icon_shop.png");
    icon_settings_tex = load("icon_settings.png");
    mascot_tex = load("mascot_business.png");
    cloud_tex = load("cloud_white.png");
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
  afterhours::Color lavender_bg{190, 175, 200, 255}; // Purple tint at bottom
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

    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    UIStylingDefaults::get().set_default_font("EqProRounded", h720(18.0f));
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
    theme.roundness = 0.12f;
    theme.segments = 10;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND: Sky gradient with lavender at bottom ==========
    ui_workarounds::vertical_gradient(
        context, entity, 1, 0, 0, static_cast<float>(screen_w),
        static_cast<float>(screen_h) * 0.7f, sky_top, sky_bottom, 10);

    // Lavender/purple bottom section
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{
                pixels(screen_w), pxf(screen_h * 0.35f)})
            .with_absolute_position(0.0f, static_cast<float>(screen_h) * 0.65f)
            .with_custom_background(lavender_bg)
            .with_debug_name("lavender_bg"));

    // Decorative clouds
    if (cloud_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)cloud_tex.width,
                                                 (float)cloud_tex.height};
      sprite(context, mk(entity, 6), cloud_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(80), pixels(40)})
                 .with_absolute_position((float)screen_w - 130.0f, 15.0f)
                 .with_opacity(0.6f)
                 .with_debug_name("cloud1"));
      sprite(context, mk(entity, 7), cloud_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(60), pixels(30)})
                 .with_absolute_position((float)screen_w - 200.0f, 55.0f)
                 .with_opacity(0.4f)
                 .with_debug_name("cloud2"));
    }

    // ========== TITLE: DREAM INCORPORATED (large puffy 3D text) ==========
    // Using native with_text_stroke() API for efficient outline rendering

    // Title text with shadow: shadow layer (offset +4,+7) then main layer
    struct TitleWord {
      const char *text; int id; int w; int h; float x; float y; float font_sz;
      float stroke_w;
      afterhours::Color main_color, main_stroke, shadow_color, shadow_stroke;
    };
    TitleWord titles[] = {
        {"DREAM", 10, 420, 85, 28.0f, 15.0f, 64.0f, 5.0f,
         white, border_blue,
         {45, 90, 140, 180}, {35, 70, 115, 150}},
        {"INCORPORATED", 12, 520, 55, 28.0f, 90.0f, 42.0f, 4.0f,
         {100, 185, 240, 255}, {45, 115, 175, 255},
         {35, 75, 125, 180}, {25, 55, 95, 150}},
    };
    for (auto &t : titles) {
      auto base = ComponentConfig{}
          .with_label(t.text)
          .with_size(ComponentSize{pixels(t.w), pixels(t.h)})
          .with_font("Fredoka", h720(t.font_sz))
          .with_alignment(TextAlignment::Left);
      // Shadow
      div(context, mk(entity, t.id), ComponentConfig{base}
          .with_absolute_position(t.x + 4.0f, t.y + 7.0f)
          .with_custom_text_color(t.shadow_color)
          .with_text_stroke(t.shadow_stroke, t.stroke_w)
          .with_debug_name(std::string(t.text) + "_shadow"));
      // Main
      div(context, mk(entity, t.id + 1), ComponentConfig{base}
          .with_absolute_position(t.x, t.y)
          .with_custom_text_color(t.main_color)
          .with_text_stroke(t.main_stroke, t.stroke_w)
          .with_debug_name(std::string(t.text) + "_main"));
    }

    // ========== TOP RIGHT: Currency ==========
    // IDs 10-13 are used by title text, start at 50
    // Increased width to prevent currency text clipping for large amounts
    float cur_x = (float)screen_w - 340.0f;

    // Currency pill - wider to fit full amount like $1,250,980
    div(context, mk(entity, 55),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(320), pixels(60)})
            .with_absolute_position(cur_x, 15.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("currency_pill"));

    // Gold coin
    if (coin_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)coin_tex.width,
                                                 (float)coin_tex.height};
      sprite(context, mk(entity, 56), coin_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(44), pixels(44)})
                 .with_absolute_position(cur_x + 12.0f, 23.0f)
                 .with_debug_name("coin"));
    } else {
      div(context, mk(entity, 56),
          ComponentConfig{}
              .with_label("*")
              .with_size(ComponentSize{pixels(44), pixels(44)})
              .with_absolute_position(cur_x + 10.0f, 23.0f)
              .with_custom_background(btn_yellow)
              .with_border(btn_yellow_dark, 2.0f)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("coin"));
    }

    // Hero number style currency display - large 28.0f+ font
    div(context, mk(entity, 57),
        ComponentConfig{}
            .with_label(format_money(cash))
            .with_size(ComponentSize{pixels(250), pixels(50)})
            .with_absolute_position(cur_x + 62.0f, 20.0f)
            .with_font("EqProRounded", h720(32.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("money"));

    // ========== STATUS ICONS ==========
    float stat_y = 80.0f;
    float stat_icon_size = 60.0f;

    // Happiness icon - larger and more prominent (minimum 44px)
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_720p_size(stat_icon_size, stat_icon_size)
            .with_absolute_position((float)screen_w - 210.0f, stat_y)
            .with_custom_background(afterhours::Color{255, 220, 150, 255})
            .with_border(afterhours::Color{220, 180, 100, 255}, 3.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("happy_bg"));
    if (icon_happiness_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)icon_happiness_tex.width,
          (float)icon_happiness_tex.height};
      sprite(context, mk(entity, 61), icon_happiness_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(48), pixels(48)})
                 .with_absolute_position((float)screen_w - 204.0f, stat_y + 6.0f)
                 .with_debug_name("happy_icon"));
    }

    // Resource gauge icon - larger and more prominent (minimum 44px)
    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_720p_size(stat_icon_size, stat_icon_size)
            .with_absolute_position((float)screen_w - 85.0f, stat_y)
            .with_custom_background(afterhours::Color{180, 210, 245, 255})
            .with_border(afterhours::Color{140, 175, 215, 255}, 3.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f, afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("gauge_bg"));
    if (icon_resources_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)icon_resources_tex.width,
          (float)icon_resources_tex.height};
      sprite(context, mk(entity, 63), icon_resources_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(48), pixels(48)})
                 .with_absolute_position((float)screen_w - 79.0f, stat_y + 6.0f)
                 .with_debug_name("resource_icon"));
    }

    // ========== METERS ==========
    // IDs 80-95 for meters
    // Two rows: happiness on top, resources below
    float meter_y = 152.0f;
    float meter_row2_y = meter_y + 44.0f;

    // Happiness meter with label - full row
    div(context, mk(entity, 80),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(40)})
            .with_absolute_position((float)screen_w - 340.0f, meter_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("happiness_pill"));

    div(context, mk(entity, 81),
        ComponentConfig{}
            .with_label(":)")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position((float)screen_w - 335.0f, meter_y + 7.0f)
            .with_custom_background(afterhours::Color{255, 210, 130, 255})
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("happy_icon"));

    div(context, mk(entity, 82),
        ComponentConfig{}
            .with_label("Happiness")
            .with_size(ComponentSize{pixels(90), pixels(22)})
            .with_absolute_position((float)screen_w - 302.0f, meter_y + 9.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("happy_text"));

    // Happiness bar background
    div(context, mk(entity, 83),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(80), pixels(24)})
            .with_absolute_position((float)screen_w - 200.0f, meter_y + 8.0f)
            .with_custom_background(afterhours::Color{225, 230, 235, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("happy_bar_bg"));

    // Happiness bar fill
    div(context, mk(entity, 84),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(74 * happiness_pct), pixels(20)})
            .with_absolute_position((float)screen_w - 197.0f, meter_y + 10.0f)
            .with_custom_background(happy_green)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("happy_bar_fill"));

    // Happiness percentage label - large and prominent with background highlight
    int happy_val = static_cast<int>(happiness_pct * 100);
    div(context, mk(entity, 92),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(68), pixels(32)})
            .with_absolute_position((float)screen_w - 124.0f, meter_y + 4.0f)
            .with_custom_background(afterhours::Color{220, 245, 220, 255})
            .with_border(happy_green, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("happy_pct_bg"));

    div(context, mk(entity, 88),
        ComponentConfig{}
            .with_label(std::to_string(happy_val) + "%")
            .with_size(ComponentSize{pixels(68), pixels(32)})
            .with_absolute_position((float)screen_w - 124.0f, meter_y + 4.0f)
            .with_font("EqProRounded", h720(26.0f))
            .with_custom_text_color(afterhours::Color{40, 130, 50, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("happy_pct"));

    // Resources meter with label - second row, no overlap
    div(context, mk(entity, 85),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(40)})
            .with_absolute_position((float)screen_w - 340.0f, meter_row2_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("resources_pill"));

    div(context, mk(entity, 86),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(26), pixels(26)})
            .with_absolute_position((float)screen_w - 335.0f, meter_row2_y + 7.0f)
            .with_custom_background(afterhours::Color{195, 215, 240, 255})
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("res_icon"));

    div(context, mk(entity, 87),
        ComponentConfig{}
            .with_label("Resources")
            .with_size(ComponentSize{pixels(90), pixels(22)})
            .with_absolute_position((float)screen_w - 302.0f, meter_row2_y + 9.0f)
            .with_custom_text_color(dark_text)
            .with_debug_name("res_text"));

    // Resources bar background
    div(context, mk(entity, 90),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(80), pixels(24)})
            .with_absolute_position((float)screen_w - 200.0f, meter_row2_y + 8.0f)
            .with_custom_background(afterhours::Color{225, 230, 235, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("res_bar_bg"));

    // Resources bar fill
    div(context, mk(entity, 91),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(74 * resources_pct), pixels(20)})
            .with_absolute_position((float)screen_w - 197.0f, meter_row2_y + 10.0f)
            .with_custom_background(panel_blue)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("res_bar_fill"));

    // Resources percentage label with current/max context
    int res_current = static_cast<int>(resources_pct * 1000);  // Simulated current value
    int res_max = 1000;  // Simulated max value
    int res_pct_val = static_cast<int>(resources_pct * 100);
    div(context, mk(entity, 89),
        ComponentConfig{}
            .with_label(std::to_string(res_current) + "/" + std::to_string(res_max) + " (" + std::to_string(res_pct_val) + "%)")
            .with_size(ComponentSize{pixels(130), pixels(30)})
            .with_absolute_position((float)screen_w - 128.0f, meter_row2_y + 5.0f)
            .with_custom_text_color(panel_blue)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("res_pct"));

    // ========== LEFT: Navigation Tabs ==========
    // Center the main content area
    float content_width = 85.0f; // Use percent of screen width
    content_width = (float)screen_w * 0.85f;
    float content_margin = ((float)screen_w - content_width) / 2.0f;

    std::vector<std::tuple<raylib::Texture2D *, std::string, std::string,
                           afterhours::Color>>
        tabs = {
            {&icon_rides_tex, "[R]", "Rides", tab_blue},
            {&icon_food_tex, "[F]", "Food", tab_green},
            {&icon_upgrades_tex, "[!]", "Upgrades", tab_pink},
            {&icon_finance_tex, "[$]", "Finance", tab_cream},
        };

    float tab_width = 110.0f;  // Wide enough for "Upgrades" label
    float tab_height = 70.0f;  // Proper touch target size
    float tab_spacing = 78.0f; // Adjusted spacing
    // Ensure tabs don't go past left edge - minimum 15px from left
    float nav_x = std::max(15.0f, content_margin - tab_width - 15.0f);
    float nav_y = 210.0f;
    for (size_t i = 0; i < tabs.size(); i++) {
      float tab_y = nav_y + (float)i * tab_spacing;
      auto &[tex_ptr, fallback, label, bg_color] = tabs[i];

      // Tab button background - larger with thicker border (minimum 44px)
      bool tab_selected = (i == selected_tab);
      // Reduced saturation on button colors
      afterhours::Color adjusted_bg = bg_color;
      adjusted_bg.r = static_cast<uint8_t>(std::min(255, (int)adjusted_bg.r + 20));
      adjusted_bg.g = static_cast<uint8_t>(std::min(255, (int)adjusted_bg.g + 20));
      adjusted_bg.b = static_cast<uint8_t>(std::min(255, (int)adjusted_bg.b + 20));
      afterhours::Color border_color =
          tab_selected ? afterhours::Color{255, 200, 50, 255}
                       // Gold border when selected
                       : afterhours::Color{140, 160, 180, 255};
      float border_width = tab_selected ? 5.0f : 3.0f;

      if (button(context, mk(entity, 100 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_size(
                         ComponentSize{pxf(tab_width),
                                       pxf(tab_height)})
                     .with_absolute_position(nav_x, tab_y)
                     .with_custom_background(adjusted_bg)
                     .with_border(border_color, border_width)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_soft_shadow(3.0f, 5.0f, 12.0f,
                                       afterhours::Color{0, 0, 0, 50})
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }

      // Icon image or fallback text - larger (minimum 44px)
      // NOTE: Icons are 64x48 but have wrong text labels baked in at bottom
      // Crop to top 28px to only show the icon graphic, not the wrong text
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width,
                                                   28.0f}; // Only top 28px - hide baked-in text
        sprite(
            context, mk(entity, 110 + static_cast<int>(i)), *tex_ptr, src,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(48), pixels(28)})
                .with_absolute_position(nav_x + tab_width / 2.0f - 24.0f, tab_y + 8.0f)
                .with_debug_name("tab_icon_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 110 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(fallback)
                .with_size(ComponentSize{pixels(52), pixels(44)})
                .with_absolute_position(nav_x + tab_width / 2.0f - 26.0f, tab_y + 8.0f)
                .with_font("EqProRounded", h720(24.0f))
                .with_custom_text_color(dark_text)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("tab_icon_fallback_" + std::to_string(i)));
      }

      // Tab label - positioned below icon, larger 16.0f font for readability
      div(context, mk(entity, 120 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pxf(tab_width),
                                       pixels(24)})
              .with_absolute_position(nav_x, tab_y + tab_height - 26.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("tab_label_" + std::to_string(i)));

      // Notification badge on "Upgrades" tab
      if (i == 2) {
        ui_workarounds::notification_badge(
            context, entity, 130 + static_cast<int>(i), "!",
            nav_x + tab_width - 5.0f, tab_y - 5.0f, 24.0f,
            afterhours::Color{230, 90, 80, 255});
      }
    }

    // ========== MAIN PANEL ==========
    // Panel should not overlap with sidebar tabs - start after tab area
    float panel_x = nav_x + tab_width + 30.0f; // Start after tabs with margin
    float panel_y = 200.0f;
    float panel_w = (float)screen_w - panel_x - 30.0f; // Fill remaining width with margin
    float panel_h = 440.0f;

    // Main panel background - bigger with thicker border to match inspiration
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 6.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f)
            .with_debug_name("main_panel"));

    // Production Overview section
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("Production Overview")
            .with_size(ComponentSize{pixels(250), pixels(32)})
            .with_absolute_position(panel_x + 30.0f, panel_y + 20.0f)
            .with_font("EqProRounded", h720(24.0f))
            .with_custom_text_color(dark_text)
            .with_debug_name("prod_header"));

    // Trend indicator legend - prominent pill at top right of panel
    div(context, mk(entity, 212),
        ComponentConfig{}
            .with_label("^ = Trending Up")
            .with_size(ComponentSize{pixels(190), pixels(32)})
            .with_absolute_position(panel_x + panel_w - 210.0f, panel_y + 22.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(afterhours::Color{40, 130, 50, 255})
            .with_custom_background(afterhours::Color{220, 245, 220, 255})
            .with_border(happy_green, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("trend_legend"));

    // Production box - uses clip_children for text overflow protection
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(340), pixels(190)})
            .with_absolute_position(panel_x + 30.0f, panel_y + 60.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 210, 225, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_clip_children(true)
            .with_debug_name("prod_box"));

    // Production items
    for (size_t i = 0; i < production.size(); i++) {
      float item_y = panel_y + 80.0f + (float)i * 54.0f;

      div(context, mk(entity, 220 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(production[i].name + ": " +
                          std::to_string(production[i].rate) + "/min")
              .with_size(ComponentSize{pixels(280), pixels(36)})
              .with_absolute_position(panel_x + 50.0f, item_y)
              .with_custom_text_color(dark_text)
              .with_debug_name("prod_" + std::to_string(i)));

      // Up arrow
      div(context, mk(entity, 230 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label("^")
              .with_size(ComponentSize{pixels(28), pixels(28)})
              .with_absolute_position(panel_x + 330.0f, item_y + 4.0f)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(happy_green)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("prod_arrow_" + std::to_string(i)));
    }

    // Current Projects section
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_label("Current Projects")
            .with_size(ComponentSize{pixels(220), pixels(32)})
            .with_absolute_position(panel_x + 420.0f, panel_y + 20.0f)
            .with_font("EqProRounded", h720(24.0f))
            .with_custom_text_color(dark_text)
            .with_debug_name("proj_header"));

    // Projects box - uses clip_children for text overflow protection
    div(context, mk(entity, 251),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(420), pixels(190)})
            .with_absolute_position(panel_x + 400.0f, panel_y + 60.0f)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 210, 225, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_clip_children(true)
            .with_debug_name("proj_box"));

    // Project items with progress bar labels
    for (size_t i = 0; i < projects.size(); i++) {
      float item_y = panel_y + 80.0f + (float)i * 54.0f;

      std::string proj_text =
          projects[i].name + " - " + std::to_string(projects[i].progress) + "%";
      if (projects[i].rate > 0) {
        proj_text = projects[i].name + " - " +
                    std::to_string(projects[i].rate) + "/min";
      }

      div(context, mk(entity, 260 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(proj_text)
              .with_size(ComponentSize{pixels(360), pixels(36)})
              .with_absolute_position(panel_x + 420.0f, item_y)
              .with_custom_text_color(dark_text)
              .with_debug_name("proj_" + std::to_string(i)));

      div(context, mk(entity, 270 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label("^")
              .with_size(ComponentSize{pixels(28), pixels(28)})
              .with_absolute_position(panel_x + 780.0f, item_y + 4.0f)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(happy_green)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("proj_arrow_" + std::to_string(i)));
    }

    // ========== ACTION BUTTONS ==========
    std::vector<std::string> btn_labels = {"Speed Up", "Prioritize",
                                           "New Project"};
    float btn_y = panel_y + panel_h - 80.0f;
    float btn_w = 190.0f;
    float btn_spacing = 230.0f;  // Wider spacing for larger panel

    for (size_t i = 0; i < btn_labels.size(); i++) {
      button(
          context, mk(entity, 300 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(btn_labels[i])
              .with_size(
                  ComponentSize{pxf(btn_w), pixels(60)})
              .with_absolute_position(panel_x + 90.0f + (float)i * btn_spacing, btn_y)
              // Reduced saturation on yellow buttons
              .with_custom_background(afterhours::Color{255, 215, 100, 255})
              .with_border(btn_yellow_dark, 4.0f)
              .with_font("EqProRounded", h720(22.0f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.3f)
              .with_soft_shadow(3.0f, 5.0f, 12.0f,
                                afterhours::Color{0, 0, 0, 60})
              .with_debug_name("btn_" + std::to_string(i)));
    }

    // ========== BOTTOM: Chat ==========
    float bottom_y = (float)screen_h - 100.0f;

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(480), pixels(80)})
            .with_absolute_position(22.0f, bottom_y)
            .with_custom_background(white)
            .with_border(afterhours::Color{195, 205, 215, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_debug_name("chat_box"));

    const char *chat_msgs[] = {
        "GlobalChat: New area unlocked! Explore now.",
        "DevTeam: Update v2.0 is live - check patch notes!",
    };
    for (int i = 0; i < 2; i++) {
      div(context, mk(entity, 401 + i),
          ComponentConfig{}
              .with_label(chat_msgs[i])
              .with_size(ComponentSize{pixels(460), pixels(24)})
              .with_absolute_position(32.0f, bottom_y + 14.0f + (float)i * 30.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(dark_text)
              .with_debug_name("chat" + std::to_string(i + 1)));
    }

    // ========== BOTTOM: Next Milestone ==========
    float ms_x = (float)screen_w / 2.0f - 60.0f;

    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_label("Next Milestone")
            .with_size(ComponentSize{pixels(160), pixels(26)})
            .with_absolute_position(ms_x, (float)screen_h - 90.0f)
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("milestone_label"));

    // Milestone bar with percentage label
    div(context, mk(entity, 411),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_absolute_position((float)screen_w / 2.0f - 125.0f,
                            (float)screen_h - 58.0f)
            .with_custom_background(white)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("milestone_bg"));

    div(context, mk(entity, 412),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(288 * milestone_pct), pixels(26)})
            .with_absolute_position((float)screen_w / 2.0f - 121.0f,
                            (float)screen_h - 56.0f)
            .with_custom_background(btn_yellow)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("milestone_fill"));

    // Milestone percentage label
    int ms_val = static_cast<int>(milestone_pct * 100);
    div(context, mk(entity, 413),
        ComponentConfig{}
            .with_label(std::to_string(ms_val) + "%")
            .with_size(ComponentSize{pixels(60), pixels(24)})
            .with_absolute_position((float)screen_w / 2.0f - 30.0f,
                            (float)screen_h - 55.0f)
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("milestone_pct"));

    // ========== BOTTOM RIGHT: Icons ==========
    std::vector<std::tuple<raylib::Texture2D *, std::string, std::string>>
        icon_data = {{&icon_shop_tex, "$", "Shop"},
                     {&icon_settings_tex, "@", "Settings"},
                     {&star_trophy_tex, "#", "Leaderboard"}};
    float icon_x = (float)screen_w - 290.0f;
    float icon_size = 56.0f;  // Minimum 44px for touch targets
    float icon_img_size = 36.0f;
    float icon_offset = (icon_size - icon_img_size) / 2.0f;
    float icon_btn_y = (float)screen_h - 110.0f; // Move icons up to leave room for labels

    for (size_t i = 0; i < icon_data.size(); i++) {
      float ix = icon_x + (float)i * 90.0f;
      auto &[tex_ptr, fallback, label] = icon_data[i];

      // Button background - reduced saturation
      button(context, mk(entity, 500 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_720p_size(icon_size, icon_size)
                 .with_absolute_position(ix, icon_btn_y)
                 .with_custom_background(afterhours::Color{255, 215, 100, 255})
                 .with_border(btn_yellow_dark, 2.0f)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(1.0f)
                 .with_debug_name("icon_btn_" + std::to_string(i)));

      // Icon image or fallback text
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width,
                                                   (float)tex_ptr->height};
        sprite(context, mk(entity, 520 + static_cast<int>(i)), *tex_ptr, src,
               ComponentConfig{}
                   .with_720p_size(icon_img_size, icon_img_size)
                   .with_absolute_position(ix + icon_offset, icon_btn_y + icon_offset)
                   .with_debug_name("icon_img_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 520 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(fallback)
                .with_720p_size(icon_size, icon_size)
                .with_absolute_position(ix, icon_btn_y)
                .with_font("EqProRounded", h720(28.0f))
                .with_custom_text_color(dark_text)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("icon_fallback_" + std::to_string(i)));
      }

      // Label below icon - larger and properly positioned (18.0f minimum)
      div(context, mk(entity, 510 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(85), pixels(24)})
              .with_absolute_position(ix - 14.0f, icon_btn_y + icon_size + 5.0f)
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("icon_label_" + std::to_string(i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups",
                        "Bright cartoon tycoon game", EmpireTycoonScreen)
