#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NeonStrikeScreen : ScreenSystem<UIContext<InputAction>> {
  // ========== CONFIGURABLE GAME STATE ==========
  int score = 15000;
  int ammo_clip = 35;
  int ammo_reserve = 210;
  float health_pct = 0.8f;
  float armor_pct = 0.45f;
  int killstreak_count = 1; // Current killstreak progress (0-3)
  int killstreak_max = 3;   // Max killstreak boxes

  // ========== CONFIGURABLE LAYOUT PARAMETERS ==========
  // Margins and spacing
  float margin_edge = 22.0f;   // Edge margin for UI elements
  float margin_bottom = 20.0f; // Bottom margin
  float element_gap = 10.0f;   // Gap between adjacent elements

  // Compass configuration
  float compass_size = 70.0f;
  float compass_font_size = 65.0f;
  float compass_cardinal_size = 19.0f;
  float compass_radius = 42.0f;
  float compass_tick_size = 3.0f;

  // Killstreak sidebar configuration
  float ks_icon_size = 60.0f;
  float ks_icon_gap = 16.0f; // Reduced gap to fit 4 icons above minimap
  float ks_start_y = 140.0f;
  int ks_sidebar_count = 4; // Number of skill icons to show on left

  // Minimap configuration
  float minimap_width = 200.0f;
  float minimap_height = 160.0f;
  float minimap_label_height = 26.0f;

  // Health panel configuration
  float health_panel_width = 230.0f;
  float health_panel_height = 60.0f;
  float health_bar_width = 165.0f;
  float health_bar_height = 16.0f;
  float armor_bar_height = 12.0f;

  // Equipment box configuration
  float eq_box_size = 70.0f;
  float eq_icon_size = 50.0f;

  // Crosshair configuration
  float crosshair_size = 64.0f;

  // ========== FONT SIZES ==========
  float font_small = 12.0f;
  float font_medium = 17.0f;
  float font_normal = 18.0f;
  float font_large = 21.0f;
  float font_xlarge = 42.0f;

  // Loaded textures
  bool textures_loaded = false;
  raylib::Texture2D icon_uav_tex{};
  raylib::Texture2D icon_recon_tex{};
  raylib::Texture2D icon_shield_tactical_tex{};
  raylib::Texture2D icon_strike_tex{};
  raylib::Texture2D icon_danger_tex{};
  raylib::Texture2D icon_health_tex{};
  raylib::Texture2D icon_skull_tex{};
  raylib::Texture2D icon_ammo_tex{};
  raylib::Texture2D weapon_grenade_tex{};
  raylib::Texture2D weapon_melee_tex{};
  raylib::Texture2D crosshair_tex{};

  void load_textures_if_needed() {
    if (textures_loaded)
      return;
    textures_loaded = true;

    std::string images_path =
        afterhours::files::get_resource_path("images", "").string();
    auto load = [&](const char *name) {
      return raylib::LoadTexture((images_path + name).c_str());
    };
    icon_uav_tex = load("icon_uav.png");
    icon_recon_tex = load("icon_recon.png");
    icon_shield_tactical_tex = load("icon_shield_tactical.png");
    icon_strike_tex = load("icon_strike.png");
    icon_danger_tex = load("icon_danger.png");
    icon_health_tex = load("icon_health.png");
    icon_skull_tex = load("icon_skull.png");
    icon_ammo_tex = load("icon_ammo.png");
    weapon_grenade_tex = load("icon_grenade.png");
    weapon_melee_tex = load("icon_melee.png");
    crosshair_tex = load("crosshair_neon.png");
  }

  // Colors matching the inspiration exactly - dark tactical feel
  afterhours::Color bg_dark{22, 20, 18, 255};
  afterhours::Color text_tan{205, 195, 175, 255};
  afterhours::Color text_muted{160, 155, 145, 255};
  afterhours::Color gold_accent{190, 145, 55, 255};
  afterhours::Color panel_dark{30, 28, 25, 230};
  afterhours::Color border_dark{65, 60, 52, 255};
  afterhours::Color health_cyan{65, 175, 195, 255};
  afterhours::Color armor_blue{55, 115, 160, 255};
  afterhours::Color minimap_green{45, 55, 45, 255};
  afterhours::Color dot_red{180, 60, 50, 255};

  std::vector<std::string> kill_feed = {
      "Alpha_Six eliminated [Emepine] with M4",
      "[Tsha_Rio] eliminated [Emepine] with SCAR",
      "Alpha_Six - Under fire!",
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();
    Theme theme;
    theme.font = text_tan;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = panel_dark;
    theme.primary = border_dark;
    theme.secondary = afterhours::Color{45, 42, 38, 255};
    theme.accent = gold_accent;
    theme.error = afterhours::Color{180, 50, 50, 255};
    theme.roundness = 0.08f;
    theme.segments = 4;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // Full screen dark background with grain effect simulation
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    float cx = (float)screen_w / 2.0f;
    float cy = (float)screen_h / 2.0f;

    // ========== TOP CENTER: Compass ==========
    float compass_cx = cx;
    float compass_top_y = 25.0f;
    float compass_cy = compass_top_y + compass_size / 2.0f;

    // Compass ring
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("O")
            .with_720p_size(compass_size, compass_size)
            .with_absolute_position(compass_cx - compass_size / 2.0f,
                                    compass_top_y)
            .with_font("EqProRounded", compass_font_size)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Compass cardinal directions
    struct Cardinal {
      const char *label;
      int id;
      float x;
      float y;
      afterhours::Color color;
    };
    Cardinal cardinals[] = {
        {"N", 101, compass_cx - 8.0f, compass_top_y - 17.0f, text_tan},
        {"S", 102, compass_cx - 8.0f, compass_top_y + compass_size - 3.0f,
         text_muted},
        {"W", 103, compass_cx - compass_size / 2.0f - 20.0f, compass_cy - 8.0f,
         text_muted},
        {"E", 104, compass_cx + compass_size / 2.0f + 3.0f, compass_cy - 8.0f,
         text_muted},
    };
    for (auto &c : cardinals) {
      div(context, mk(entity, c.id),
          ComponentConfig{}
              .with_label(c.label)
              .with_size(ComponentSize{pixels(20), pixels(20)})
              .with_absolute_position(c.x, c.y)
              .with_font("EqProRounded", compass_cardinal_size)
              .with_custom_text_color(c.color)
              .with_alignment(TextAlignment::Center));
    }

    // Compass needle (vertical line)
    div(context, mk(entity, 105),
        ComponentConfig{}
            .with_label("|")
            .with_size(ComponentSize{pixels(20), pixels(30)})
            .with_absolute_position(compass_cx - 10.0f, compass_cy - 12.0f)
            .with_font("EqProRounded", h720(28.0f))
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center));

    // Compass tick marks (8 positions around the ring)
    for (int i = 0; i < 8; i++) {
      float angle =
          (float)i * 3.14159f / 4.0f - 3.14159f / 2.0f; // Start from top
      float tick_x = compass_cx + std::cos(angle) * compass_radius -
                     compass_tick_size / 2.0f;
      float tick_y = compass_cy + std::sin(angle) * compass_radius -
                     compass_tick_size / 2.0f;
      div(context, mk(entity, 106 + i),
          ComponentConfig{}
              .with_720p_size(compass_tick_size, compass_tick_size)
              .with_absolute_position(tick_x, tick_y)
              .with_custom_background(text_muted)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("compass_tick_" + std::to_string(i)));
    }

    // ========== TOP RIGHT: Score & Objective ==========
    div(context, mk(entity, 110),
        ComponentConfig{}
            .with_label("SCORE: 15,000")
            .with_size(ComponentSize{pixels(260), pixels(28)})
            .with_absolute_position((float)screen_w - 330.0f, 18.0f)
            .with_font("EqProRounded", h720(15.0f))
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Right));

    div(context, mk(entity, 111),
        ComponentConfig{}
            .with_label("(U)")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position((float)screen_w - 40.0f, 15.0f)
            .with_custom_background(text_tan)
            .with_font("EqProRounded", h720(19.0f))
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));

    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_label(">> SECURE OBJECTIVE B")
            .with_size(ComponentSize{pixels(230), pixels(30)})
            .with_absolute_position((float)screen_w - 255.0f, 52.0f)
            .with_custom_background(gold_accent)
            .with_font("BlackOpsOne", h720(16.0f))
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center));

    // ========== KILL FEED ==========
    float kill_y = 120.0f;
    for (size_t i = 0; i < kill_feed.size(); i++) {
      div(context, mk(entity, 120 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(kill_feed[i])
              .with_size(ComponentSize{pixels(550), pixels(30)})
              .with_absolute_position(cx - 275.0f, kill_y + (float)i * 32.0f)
              .with_font("EqProRounded", h720(19.0f))
              .with_custom_background(afterhours::Color{0, 0, 0, 100})
              .with_custom_text_color(afterhours::Color{200, 210, 215, 255})
              .with_alignment(TextAlignment::Center)
              .with_debug_name("kill_" + std::to_string(i)));
    }

    // ========== RIGHT: Voice Indicator ==========
    div(context, mk(entity, 130),
        ComponentConfig{}
            .with_label("@ * Alpha_Six: Under fire!")
            .with_size(ComponentSize{pixels(250), pixels(26)})
            .with_absolute_position((float)screen_w - 270.0f, 195.0f)
            .with_font("EqProRounded", h720(21.0f))
            .with_custom_text_color(text_tan));

    // ========== LEFT: Killstreak Icons ==========
    // Array of skill textures and labels (only show ks_sidebar_count)
    std::vector<std::tuple<raylib::Texture2D *, std::string, std::string>>
        skill_icons = {
            {&icon_uav_tex, "[T]", "UAV"},
            {&icon_recon_tex, "[O]", "Recon"},
            {&icon_shield_tactical_tex, "[U]", "Shield"},
            {&icon_strike_tex, "[X]", "Strike"},
        };

    size_t icons_to_show =
        std::min(static_cast<size_t>(ks_sidebar_count), skill_icons.size());
    for (size_t i = 0; i < icons_to_show; i++) {
      float row_y = ks_start_y + (float)i * (ks_icon_size + ks_icon_gap);
      auto &[tex_ptr, fallback_label, label] = skill_icons[i];

      // Cog/gear icon - subtle indicator
      div(context, mk(entity, 140 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label("*")
              .with_size(ComponentSize{pixels(16), pixels(16)})
              .with_absolute_position(margin_edge - 2.0f,
                                      row_y + ks_icon_size / 2.0f - 8.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("cog_" + std::to_string(i)));

      // Icon box background
      button(context, mk(entity, 141 + static_cast<int>(i) * 4),
             ComponentConfig{}
                 .with_720p_size(ks_icon_size, ks_icon_size)
                 .with_absolute_position(margin_edge + 20.0f, row_y)
                 .with_custom_background(panel_dark)
                 .with_border(border_dark, 1.0f)
                 .with_debug_name("ks_bg_" + std::to_string(i)));

      // Icon image or fallback text
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width,
                                                   (float)tex_ptr->height};
        sprite(context, mk(entity, 142 + static_cast<int>(i) * 4), *tex_ptr,
               src,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(48), pixels(48)})
                   .with_absolute_position(margin_edge + 26.0f, row_y + 6.0f)
                   .with_debug_name("ks_icon_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 142 + static_cast<int>(i) * 4),
            ComponentConfig{}
                .with_label(fallback_label)
                .with_720p_size(ks_icon_size, ks_icon_size)
                .with_absolute_position(margin_edge + 20.0f, row_y)
                .with_font("EqProRounded", h720(20.0f))
                .with_custom_text_color(text_tan)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("ks_icon_fallback_" + std::to_string(i)));
      }

      // Label (only for UAV)
      if (!label.empty()) {
        div(context, mk(entity, 143 + static_cast<int>(i) * 4),
            ComponentConfig{}
                .with_label(label)
                .with_size(ComponentSize{pxf(ks_icon_size), pixels(16)})
                .with_absolute_position(margin_edge + 20.0f,
                                        row_y + ks_icon_size + 2.0f)
                .with_font("EqProRounded", font_medium)
                .with_custom_text_color(text_muted)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("ks_label_" + std::to_string(i)));
      }
    }

    // ========== BOTTOM LEFT: Minimap ==========
    // Calculate layout from bottom of screen upward
    float map_bottom_margin = margin_bottom;
    float map_content_y = (float)screen_h - map_bottom_margin - minimap_height;
    float map_label_y = map_content_y - minimap_label_height - 4.0f;

    // Map location label
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("7B Pop <192>")
            .with_720p_size(minimap_width, minimap_label_height)
            .with_absolute_position(margin_edge, map_label_y)
            .with_font("EqProRounded", font_large)
            .with_custom_text_color(text_tan));

    // Minimap background
    div(context, mk(entity, 220),
        ComponentConfig{}
            .with_720p_size(minimap_width, minimap_height)
            .with_absolute_position(margin_edge, map_content_y)
            .with_custom_background(minimap_green)
            .with_border(border_dark, 2.0f));

    // Map grid lines (vertical + horizontal)
    float grid_cell_w = minimap_width / 4.0f;
    float grid_cell_h = minimap_height / 4.0f;
    afterhours::Color grid_color{60, 70, 60, 180};
    for (int i = 1; i < 4; i++) {
      // Vertical
      div(context, mk(entity, 230 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1), pxf(minimap_height - 15.0f)})
              .with_absolute_position(margin_edge + (float)i * grid_cell_w,
                                      map_content_y + 8.0f)
              .with_custom_background(grid_color));
      // Horizontal
      div(context, mk(entity, 240 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(minimap_width - 15.0f), pixels(1)})
              .with_absolute_position(margin_edge + 8.0f,
                                      map_content_y + (float)i * grid_cell_h)
              .with_custom_background(grid_color));
    }

    // Red danger zone on map
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position(margin_edge + minimap_width - 65.0f,
                                    map_content_y + 10.0f)
            .with_custom_background(afterhours::Color{140, 50, 40, 120})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("danger_zone"));

    // Player icon on map - triangle shape indicator
    float player_map_x = margin_edge + minimap_width / 2.0f - 8.0f;
    float player_map_y = map_content_y + minimap_height - 35.0f;
    div(context, mk(entity, 260),
        ComponentConfig{}
            .with_label("^")
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position(player_map_x, player_map_y)
            .with_font("EqProRounded", font_large)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center));

    // Objective marker - red dot
    div(context, mk(entity, 261),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(12), pixels(12)})
            .with_absolute_position(margin_edge + minimap_width - 45.0f,
                                    map_content_y + 30.0f)
            .with_custom_background(dot_red)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("obj_marker"));

    // ========== BOTTOM LEFT: Killstreak Progress Bar (below minimap)
    // ==========
    float ks_bar_width = 200.0f;
    float ks_bar_height = 28.0f;
    float ks_bar_y = map_label_y - ks_bar_height - 8.0f;
    float ks_box_size = 14.0f;
    float ks_box_gap = 4.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_720p_size(ks_bar_width, ks_bar_height)
            .with_absolute_position(margin_edge, ks_bar_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 1.0f)
            .with_debug_name("ks_bar"));

    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("KILLSTREAK")
            .with_size(ComponentSize{pixels(120), pxf(ks_bar_height - 4.0f)})
            .with_absolute_position(margin_edge + 8.0f, ks_bar_y + 2.0f)
            .with_font("EqProRounded", font_medium)
            .with_custom_text_color(text_tan));

    // Killstreak progress boxes (dynamic based on killstreak_count)
    float boxes_start_x =
        margin_edge + ks_bar_width -
        (ks_box_size * killstreak_max + ks_box_gap * (killstreak_max - 1)) -
        10.0f;
    for (int i = 0; i < killstreak_max; i++) {
      afterhours::Color box_color =
          (i < killstreak_count) ? text_tan : border_dark;
      div(context, mk(entity, 202 + i),
          ComponentConfig{}
              .with_720p_size(ks_box_size, ks_box_size)
              .with_absolute_position(
                  boxes_start_x + (float)i * (ks_box_size + ks_box_gap),
                  ks_bar_y + (ks_bar_height - ks_box_size) / 2.0f)
              .with_custom_background(box_color)
              .with_debug_name("ks_box" + std::to_string(i)));
    }

    // ========== BOTTOM CENTER: Ammo ==========
    float ammo_x = margin_edge + minimap_width + 20.0f;
    float ammo_y = (float)screen_h - margin_bottom - 70.0f;
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label(std::to_string(ammo_clip) + "/" +
                        std::to_string(ammo_reserve))
            .with_size(ComponentSize{pixels(140), pixels(55)})
            .with_absolute_position(ammo_x, ammo_y)
            .with_font("EqProRounded", font_xlarge)
            .with_custom_text_color(text_tan));

    // ========== BOTTOM CENTER: Health & Armor ==========
    float health_x = ammo_x + 150.0f;
    float health_y =
        (float)screen_h - margin_bottom - health_panel_height - 15.0f;

    // Health panel
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_720p_size(health_panel_width, health_panel_height)
            .with_absolute_position(health_x, health_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 1.0f)
            .with_debug_name("health_panel"));

    // Skull icon for health panel
    if (icon_skull_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)icon_skull_tex.width, (float)icon_skull_tex.height};
      sprite(context, mk(entity, 311), icon_skull_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(32), pixels(32)})
                 .with_absolute_position(health_x + 10.0f, health_y + 14.0f)
                 .with_debug_name("skull_icon"));
    } else {
      div(context, mk(entity, 311),
          ComponentConfig{}
              .with_label("@")
              .with_size(ComponentSize{pixels(35), pixels(35)})
              .with_absolute_position(health_x + 8.0f, health_y + 12.0f)
              .with_font("EqProRounded", h720(24.0f))
              .with_custom_text_color(text_tan)
              .with_alignment(TextAlignment::Center));
    }

    // Health and armor bars - data-driven
    afterhours::Color bar_track{25, 25, 22, 255};
    float armor_label_width = 60.0f;
    float armor_bar_start_x = health_x + 50.0f + armor_label_width + 4.0f;
    float armor_bar_actual_width = health_bar_width - armor_label_width - 4.0f;

    struct StatBar {
      int label_id;
      const char *label_fmt;
      int val;
      float label_x;
      float label_y;
      int label_w;
      float label_font;
      afterhours::Color label_color;
      int bg_id;
      float bar_x;
      float bar_y;
      float bar_w;
      float bar_h;
      int fill_id;
      float fill_pct;
      afterhours::Color fill_color;
    };
    int health_val = static_cast<int>(health_pct * 100);
    int armor_val = static_cast<int>(armor_pct * 100);
    StatBar bars[] = {
        {312, nullptr, health_val, health_x + 50.0f, health_y + 5.0f, 140,
         font_normal, text_tan, 320, health_x + 50.0f, health_y + 26.0f,
         health_bar_width, health_bar_height, 321, health_pct, health_cyan},
        {332, nullptr, armor_val, health_x + 50.0f, health_y + 44.0f,
         static_cast<int>(armor_label_width), font_small, text_muted, 330,
         armor_bar_start_x, health_y + 44.0f, armor_bar_actual_width,
         armor_bar_height, 331, armor_pct, armor_blue},
    };
    // Health label is "80 HEALTH", armor label is "45%"
    std::string health_label_str = std::to_string(health_val) + " HEALTH";
    std::string armor_label_str = std::to_string(armor_val) + "%";
    const char *bar_labels[] = {health_label_str.c_str(),
                                armor_label_str.c_str()};

    for (int bi = 0; bi < 2; bi++) {
      auto &b = bars[bi];
      // Label
      div(context, mk(entity, b.label_id),
          ComponentConfig{}
              .with_label(bar_labels[bi])
              .with_size(ComponentSize{pixels(b.label_w), pixels(20)})
              .with_absolute_position(b.label_x, b.label_y)
              .with_font("EqProRounded", b.label_font)
              .with_custom_text_color(b.label_color));
      // Bar bg
      div(context, mk(entity, b.bg_id),
          ComponentConfig{}
              .with_720p_size(b.bar_w, b.bar_h)
              .with_absolute_position(b.bar_x, b.bar_y)
              .with_custom_background(bar_track));
      // Bar fill
      div(context, mk(entity, b.fill_id),
          ComponentConfig{}
              .with_720p_size(b.bar_w * b.fill_pct, b.bar_h)
              .with_absolute_position(b.bar_x, b.bar_y)
              .with_custom_background(b.fill_color));
    }

    // ========== BOTTOM RIGHT: Equipment ==========
    // Two boxes with gap between them, positioned from right edge
    float eq_margin = margin_edge; // margin from right edge of screen
    // Knife box (rightmost)
    float knife_x = (float)screen_w - eq_margin - eq_box_size;
    // Grenade box (left of knife)
    float grenade_x = knife_x - element_gap - eq_box_size;
    float eq_y = (float)screen_h - margin_bottom - eq_box_size - 15.0f;

    // Equipment boxes - grenade (selected) and knife
    struct EquipSlot {
      float x;
      int base_id;
      raylib::Texture2D *tex;
      afterhours::Color border;
      float border_w;
      const char *count;
      float count_x;
      float count_y;
      float count_font;
      afterhours::Color count_color;
    };
    float icon_offset = (eq_box_size - eq_icon_size) / 2.0f;
    EquipSlot equip_slots[] = {
        {grenade_x, 410, &weapon_grenade_tex, gold_accent, 3.0f, "x2",
         grenade_x - 35.0f, eq_y + eq_box_size / 2.0f - 10.0f, font_normal,
         text_tan},
        {knife_x, 420, &weapon_melee_tex, border_dark, 2.0f, "x1",
         knife_x + eq_box_size - 25.0f, eq_y + eq_box_size - 16.0f, font_small,
         text_muted},
    };
    for (auto &slot : equip_slots) {
      // Count label
      div(context, mk(entity, slot.base_id - 10),
          ComponentConfig{}
              .with_label(slot.count)
              .with_size(ComponentSize{pixels(30), pixels(20)})
              .with_absolute_position(slot.count_x, slot.count_y)
              .with_font("EqProRounded", slot.count_font)
              .with_custom_text_color(slot.count_color)
              .with_alignment(TextAlignment::Right));
      // Box background
      div(context, mk(entity, slot.base_id),
          ComponentConfig{}
              .with_720p_size(eq_box_size, eq_box_size)
              .with_absolute_position(slot.x, eq_y)
              .with_custom_background(panel_dark)
              .with_border(slot.border, slot.border_w));
      // Icon
      if (slot.tex && slot.tex->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)slot.tex->width,
                                                   (float)slot.tex->height};
        sprite(context, mk(entity, slot.base_id + 1), *slot.tex, src,
               ComponentConfig{}
                   .with_720p_size(eq_icon_size, eq_icon_size)
                   .with_absolute_position(slot.x + icon_offset,
                                           eq_y + icon_offset));
      }
    }

    // ========== CENTER: Crosshair ==========
    if (crosshair_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)crosshair_tex.width, (float)crosshair_tex.height};
      sprite(context, mk(entity, 600), crosshair_tex, src,
             ComponentConfig{}
                 .with_720p_size(crosshair_size, crosshair_size)
                 .with_absolute_position(cx - crosshair_size / 2.0f,
                                         cy - crosshair_size / 2.0f)
                 .with_debug_name("crosshair"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter HUD overlay", NeonStrikeScreen)
