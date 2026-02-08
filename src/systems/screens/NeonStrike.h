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
  int killstreak_count = 1;        // Current killstreak progress (0-3)
  int killstreak_max = 3;          // Max killstreak boxes

  // ========== CONFIGURABLE LAYOUT PARAMETERS ==========
  // Margins and spacing
  float margin_edge = 22.0f;        // Edge margin for UI elements
  float margin_bottom = 20.0f;      // Bottom margin
  float element_gap = 10.0f;        // Gap between adjacent elements

  // Compass configuration
  float compass_size = 70.0f;
  float compass_font_size = 65.0f;
  float compass_cardinal_size = 19.0f;
  float compass_radius = 42.0f;
  float compass_tick_size = 3.0f;

  // Killstreak sidebar configuration
  float ks_icon_size = 60.0f;
  float ks_icon_gap = 16.0f;        // Reduced gap to fit 4 icons above minimap
  float ks_start_y = 140.0f;
  int ks_sidebar_count = 4;         // Number of skill icons to show on left

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
    icon_uav_tex = raylib::LoadTexture((images_path + "icon_uav.png").c_str());
    icon_recon_tex =
        raylib::LoadTexture((images_path + "icon_recon.png").c_str());
    icon_shield_tactical_tex =
        raylib::LoadTexture((images_path + "icon_shield_tactical.png").c_str());
    icon_strike_tex =
        raylib::LoadTexture((images_path + "icon_strike.png").c_str());
    icon_danger_tex =
        raylib::LoadTexture((images_path + "icon_danger.png").c_str());
    icon_health_tex =
        raylib::LoadTexture((images_path + "icon_health.png").c_str());
    icon_skull_tex =
        raylib::LoadTexture((images_path + "icon_skull.png").c_str());
    icon_ammo_tex =
        raylib::LoadTexture((images_path + "icon_ammo.png").c_str());
    weapon_grenade_tex =
        raylib::LoadTexture((images_path + "icon_grenade.png").c_str());
    weapon_melee_tex =
        raylib::LoadTexture((images_path + "icon_melee.png").c_str());
    crosshair_tex =
        raylib::LoadTexture((images_path + "crosshair_neon.png").c_str());
  }

  // Colors matching the inspiration exactly - dark tactical feel
  afterhours::Color bg_dark{22, 20, 18, 255};
  afterhours::Color text_tan{205, 195, 175, 255};
  afterhours::Color text_muted{120, 115, 105, 255};
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
            .with_size(ComponentSize{pxf(compass_size),
                                     pxf(compass_size)})
            .with_absolute_position(compass_cx - compass_size / 2.0f, compass_top_y)
            .with_font("EqProRounded", compass_font_size)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_ring"));

    // Compass cardinal directions
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("N")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position(compass_cx - 8.0f, compass_top_y - 17.0f)
            .with_font("EqProRounded", compass_cardinal_size)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_n"));

    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label("S")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position(compass_cx - 8.0f, compass_top_y + compass_size - 3.0f)
            .with_font("EqProRounded", compass_cardinal_size)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_s"));

    div(context, mk(entity, 103),
        ComponentConfig{}
            .with_label("W")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position(compass_cx - compass_size / 2.0f - 20.0f, compass_cy - 8.0f)
            .with_font("EqProRounded", compass_cardinal_size)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_w"));

    div(context, mk(entity, 104),
        ComponentConfig{}
            .with_label("E")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position(compass_cx + compass_size / 2.0f + 3.0f, compass_cy - 8.0f)
            .with_font("EqProRounded", compass_cardinal_size)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_e"));

    // Compass needle (vertical line)
    div(context, mk(entity, 105),
        ComponentConfig{}
            .with_label("|")
            .with_size(ComponentSize{pixels(20), pixels(30)})
            .with_absolute_position(compass_cx - 10.0f, compass_cy - 12.0f)
            .with_font("EqProRounded", h720(28.0f))
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_needle"));

    // Compass tick marks (8 positions around the ring)
    for (int i = 0; i < 8; i++) {
      float angle = (float)i * 3.14159f / 4.0f - 3.14159f / 2.0f; // Start from top
      float tick_x =
          compass_cx + std::cos(angle) * compass_radius - compass_tick_size / 2.0f;
      float tick_y =
          compass_cy + std::sin(angle) * compass_radius - compass_tick_size / 2.0f;
      div(context, mk(entity, 106 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(compass_tick_size),
                                       pxf(compass_tick_size)})
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
            .with_alignment(TextAlignment::Right)
            .with_debug_name("score"));

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
            .with_roundness(1.0f)
            .with_debug_name("score_icon"));

    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_label(">> SECURE OBJECTIVE B")
            .with_size(ComponentSize{pixels(230), pixels(30)})
            .with_absolute_position((float)screen_w - 255.0f, 52.0f)
            .with_custom_background(gold_accent)
            .with_font("BlackOpsOne", h720(16.0f))
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("objective"));

    // ========== KILL FEED ==========
    float kill_y = 120.0f;
    for (size_t i = 0; i < kill_feed.size(); i++) {
      div(context, mk(entity, 120 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(kill_feed[i])
              .with_size(ComponentSize{pixels(550), pixels(28)})
              .with_absolute_position(cx - 275.0f, kill_y + (float)i * 30.0f)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(text_muted)
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
            .with_custom_text_color(text_tan)
            .with_debug_name("voice"));

    // ========== LEFT: Killstreak Icons ==========
    // Array of skill textures and labels (only show ks_sidebar_count)
    std::vector<std::tuple<raylib::Texture2D *, std::string, std::string>>
        skill_icons = {
            {&icon_uav_tex, "[T]", "UAV"},
            {&icon_recon_tex, "[O]", ""},
            {&icon_shield_tactical_tex, "[U]", ""},
            {&icon_strike_tex, "[X]", ""},
        };

    size_t icons_to_show = std::min(static_cast<size_t>(ks_sidebar_count), skill_icons.size());
    for (size_t i = 0; i < icons_to_show; i++) {
      float row_y = ks_start_y + (float)i * (ks_icon_size + ks_icon_gap);
      auto &[tex_ptr, fallback_label, label] = skill_icons[i];

      // Cog/gear icon - subtle indicator
      div(context, mk(entity, 140 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label("*")
              .with_size(ComponentSize{pixels(16), pixels(16)})
              .with_absolute_position(margin_edge - 2.0f, row_y + ks_icon_size / 2.0f - 8.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("cog_" + std::to_string(i)));

      // Icon box background
      div(context, mk(entity, 141 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(ks_icon_size),
                                       pxf(ks_icon_size)})
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
                .with_size(ComponentSize{pxf(ks_icon_size),
                                         pxf(ks_icon_size)})
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
                .with_absolute_position(margin_edge + 20.0f, row_y + ks_icon_size + 2.0f)
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
            .with_size(ComponentSize{pxf(minimap_width),
                                     pxf(minimap_label_height)})
            .with_absolute_position(margin_edge, map_label_y)
            .with_font("EqProRounded", font_large)
            .with_custom_text_color(text_tan)
            .with_debug_name("map_label"));

    // Minimap background
    div(context, mk(entity, 220),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(minimap_width),
                                     pxf(minimap_height)})
            .with_absolute_position(margin_edge, map_content_y)
            .with_custom_background(minimap_green)
            .with_border(border_dark, 2.0f)
            .with_debug_name("minimap"));

    // Map grid lines (vertical)
    float grid_cell_w = minimap_width / 4.0f;
    float grid_cell_h = minimap_height / 4.0f;
    for (int i = 1; i < 4; i++) {
      div(context, mk(entity, 230 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1), pxf(minimap_height - 15.0f)})
              .with_absolute_position(margin_edge + (float)i * grid_cell_w, map_content_y + 8.0f)
              .with_custom_background(afterhours::Color{60, 70, 60, 180})
              .with_debug_name("grid_v_" + std::to_string(i)));
    }

    // Map grid lines (horizontal)
    for (int i = 1; i < 4; i++) {
      div(context, mk(entity, 240 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(minimap_width - 15.0f), pixels(1)})
              .with_absolute_position(margin_edge + 8.0f, map_content_y + (float)i * grid_cell_h)
              .with_custom_background(afterhours::Color{60, 70, 60, 180})
              .with_debug_name("grid_h_" + std::to_string(i)));
    }

    // Red danger zone on map
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position(margin_edge + minimap_width - 65.0f, map_content_y + 10.0f)
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
            .with_alignment(TextAlignment::Center)
            .with_debug_name("player_icon"));

    // Objective marker - red dot
    div(context, mk(entity, 261),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(12), pixels(12)})
            .with_absolute_position(margin_edge + minimap_width - 45.0f, map_content_y + 30.0f)
            .with_custom_background(dot_red)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("obj_marker"));

    // ========== BOTTOM LEFT: Killstreak Progress Bar (below minimap) ==========
    float ks_bar_width = 200.0f;
    float ks_bar_height = 28.0f;
    float ks_bar_y = map_label_y - ks_bar_height - 8.0f;
    float ks_box_size = 14.0f;
    float ks_box_gap = 4.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(ks_bar_width),
                                     pxf(ks_bar_height)})
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
            .with_custom_text_color(text_tan)
            .with_debug_name("ks_text"));

    // Killstreak progress boxes (dynamic based on killstreak_count)
    float boxes_start_x = margin_edge + ks_bar_width - (ks_box_size * killstreak_max + ks_box_gap * (killstreak_max - 1)) - 10.0f;
    for (int i = 0; i < killstreak_max; i++) {
      afterhours::Color box_color = (i < killstreak_count) ? text_tan : border_dark;
      div(context, mk(entity, 202 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(ks_box_size),
                                       pxf(ks_box_size)})
              .with_absolute_position(boxes_start_x + (float)i * (ks_box_size + ks_box_gap),
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
            .with_custom_text_color(text_tan)
            .with_debug_name("ammo"));

    // ========== BOTTOM CENTER: Health & Armor ==========
    float health_x = ammo_x + 150.0f;
    float health_y = (float)screen_h - margin_bottom - health_panel_height - 15.0f;

    // Health panel
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(health_panel_width),
                                     pxf(health_panel_height)})
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
              .with_alignment(TextAlignment::Center)
              .with_debug_name("skull"));
    }

    // Health label with percentage
    int health_val = static_cast<int>(health_pct * 100);
    div(context, mk(entity, 312),
        ComponentConfig{}
            .with_label(std::to_string(health_val) + " HEALTH")
            .with_size(ComponentSize{pixels(140), pixels(20)})
            .with_absolute_position(health_x + 50.0f, health_y + 5.0f)
            .with_font("EqProRounded", font_normal)
            .with_custom_text_color(text_tan)
            .with_debug_name("health_label"));

    // Health bar bg
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(health_bar_width),
                                     pxf(health_bar_height)})
            .with_absolute_position(health_x + 50.0f, health_y + 26.0f)
            .with_custom_background(afterhours::Color{25, 25, 22, 255})
            .with_debug_name("health_bg"));

    // Health bar fill
    div(context, mk(entity, 321),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>((health_bar_width - 5.0f) * health_pct)),
                                     pxf(health_bar_height)})
            .with_absolute_position(health_x + 50.0f, health_y + 26.0f)
            .with_custom_background(health_cyan)
            .with_debug_name("health_fill"));

    // Armor label and bar (label left of bar)
    int armor_val = static_cast<int>(armor_pct * 100);
    float armor_label_width = 60.0f;
    float armor_bar_start_x = health_x + 50.0f + armor_label_width + 4.0f;
    float armor_bar_actual_width = health_bar_width - armor_label_width - 4.0f;

    div(context, mk(entity, 332),
        ComponentConfig{}
            .with_label(std::to_string(armor_val) + "%")
            .with_size(ComponentSize{pxf(armor_label_width), pixels(14)})
            .with_absolute_position(health_x + 50.0f, health_y + 44.0f)
            .with_font("EqProRounded", font_small)
            .with_custom_text_color(text_muted)
            .with_debug_name("armor_label"));

    // Armor bar bg
    div(context, mk(entity, 330),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(armor_bar_actual_width),
                                     pxf(armor_bar_height)})
            .with_absolute_position(armor_bar_start_x, health_y + 44.0f)
            .with_custom_background(afterhours::Color{25, 25, 22, 255})
            .with_debug_name("armor_bg"));

    // Armor bar fill
    div(context, mk(entity, 331),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(armor_bar_actual_width * armor_pct),
                                     pxf(armor_bar_height)})
            .with_absolute_position(armor_bar_start_x, health_y + 44.0f)
            .with_custom_background(armor_blue)
            .with_debug_name("armor_fill"));

    // ========== BOTTOM RIGHT: Equipment ==========
    // Two boxes with gap between them, positioned from right edge
    float eq_margin = margin_edge;  // margin from right edge of screen
    // Knife box (rightmost)
    float knife_x = (float)screen_w - eq_margin - eq_box_size;
    // Grenade box (left of knife)
    float grenade_x = knife_x - element_gap - eq_box_size;
    float eq_y = (float)screen_h - margin_bottom - eq_box_size - 15.0f;

    // Grenade count label (positioned left of grenade box)
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("x2")
            .with_size(ComponentSize{pixels(30), pixels(20)})
            .with_absolute_position(grenade_x - 35.0f, eq_y + eq_box_size / 2.0f - 10.0f)
            .with_font("EqProRounded", font_normal)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("x2"));

    // Grenade box - highlighted with gold border (selected)
    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(eq_box_size),
                                     pxf(eq_box_size)})
            .with_absolute_position(grenade_x, eq_y)
            .with_custom_background(panel_dark)
            .with_border(gold_accent, 3.0f)
            .with_debug_name("grenade_bg"));

    if (weapon_grenade_tex.id != 0) {
      float icon_offset = (eq_box_size - eq_icon_size) / 2.0f;
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)weapon_grenade_tex.width,
          (float)weapon_grenade_tex.height};
      sprite(context, mk(entity, 411), weapon_grenade_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pxf(eq_icon_size),
                                          pxf(eq_icon_size)})
                 .with_absolute_position(grenade_x + icon_offset, eq_y + icon_offset)
                 .with_debug_name("grenade_icon"));
    }

    // Knife box
    div(context, mk(entity, 420),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(eq_box_size),
                                     pxf(eq_box_size)})
            .with_absolute_position(knife_x, eq_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 2.0f)
            .with_debug_name("knife_bg"));

    if (weapon_melee_tex.id != 0) {
      float icon_offset = (eq_box_size - eq_icon_size) / 2.0f;
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)weapon_melee_tex.width, (float)weapon_melee_tex.height};
      sprite(context, mk(entity, 422), weapon_melee_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pxf(eq_icon_size),
                                          pxf(eq_icon_size)})
                 .with_absolute_position(knife_x + icon_offset, eq_y + icon_offset)
                 .with_debug_name("knife_icon"));
    }

    // x1 label positioned at bottom-right corner of knife box
    div(context, mk(entity, 421),
        ComponentConfig{}
            .with_label("x1")
            .with_size(ComponentSize{pixels(25), pixels(16)})
            .with_absolute_position(knife_x + eq_box_size - 25.0f, eq_y + eq_box_size - 16.0f)
            .with_font("EqProRounded", font_small)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("x1"));

    // ========== CENTER: Crosshair ==========
    if (crosshair_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{
          0, 0, (float)crosshair_tex.width, (float)crosshair_tex.height};
      sprite(context, mk(entity, 600), crosshair_tex, src,
             ComponentConfig{}
                 .with_size(ComponentSize{pxf(crosshair_size),
                                          pxf(crosshair_size)})
                 .with_absolute_position(cx - crosshair_size / 2.0f, cy - crosshair_size / 2.0f)
                 .with_debug_name("crosshair"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter HUD overlay", NeonStrikeScreen)
