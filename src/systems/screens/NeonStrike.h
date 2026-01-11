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
  int score = 15000;
  int ammo_clip = 35;
  int ammo_reserve = 210;
  float health_pct = 0.8f;
  float armor_pct = 0.45f;

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
    if (textures_loaded) return;
    textures_loaded = true;
    
    std::string images_path = afterhours::files::get_resource_path("images", "").string();
    icon_uav_tex = raylib::LoadTexture((images_path + "icon_uav.png").c_str());
    icon_recon_tex = raylib::LoadTexture((images_path + "icon_recon.png").c_str());
    icon_shield_tactical_tex = raylib::LoadTexture((images_path + "icon_shield_tactical.png").c_str());
    icon_strike_tex = raylib::LoadTexture((images_path + "icon_strike.png").c_str());
    icon_danger_tex = raylib::LoadTexture((images_path + "icon_danger.png").c_str());
    icon_health_tex = raylib::LoadTexture((images_path + "icon_health.png").c_str());
    icon_skull_tex = raylib::LoadTexture((images_path + "icon_skull.png").c_str());
    icon_ammo_tex = raylib::LoadTexture((images_path + "icon_ammo.png").c_str());
    weapon_grenade_tex = raylib::LoadTexture((images_path + "icon_grenade.png").c_str());
    weapon_melee_tex = raylib::LoadTexture((images_path + "icon_melee.png").c_str());
    crosshair_tex = raylib::LoadTexture((images_path + "crosshair_neon.png").c_str());
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
      "@ Alpha_Six = |Heth eliminated [Emepine] with M4",
      "@ [Tsha_Rio = |Heth eliminated [Emepine] with SCAR]",
      "@ Alpha_Six  - Under fire! - Under fire!",
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

    // Full screen dark background with grain effect simulation
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    float cx = (float)screen_w / 2.0f;

    // ========== TOP CENTER: Compass ==========
    // Compass ring
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("O")
            .with_size(ComponentSize{pixels(70), pixels(70)})
            .with_absolute_position()
            .with_translate(cx - 35.0f, 25.0f)
            .with_font("EqProRounded", 65.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_ring"));

    // Compass cardinal directions
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label("N")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(cx - 8.0f, 8.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_n"));

    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label("S")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(cx - 8.0f, 88.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_s"));

    div(context, mk(entity, 103),
        ComponentConfig{}
            .with_label("W")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(cx - 55.0f, 52.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_w"));

    div(context, mk(entity, 104),
        ComponentConfig{}
            .with_label("E")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(cx + 38.0f, 52.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_e"));

    // Compass needle
    div(context, mk(entity, 105),
        ComponentConfig{}
            .with_label("|")
            .with_size(ComponentSize{pixels(10), pixels(30)})
            .with_absolute_position()
            .with_translate(cx - 5.0f, 35.0f)
            .with_font("EqProRounded", 28.0f)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("compass_needle"));

    // Compass tick marks (8 positions around the ring)
    float compass_cx = cx;
    float compass_cy = 60.0f;
    float compass_radius = 42.0f;
    float tick_size = 3.0f;
    for (int i = 0; i < 8; i++) {
      float angle = (float)i * 3.14159f / 4.0f;
      float tick_x = compass_cx + std::cos(angle) * compass_radius - tick_size / 2.0f;
      float tick_y = compass_cy + std::sin(angle) * compass_radius - tick_size / 2.0f;
      div(context, mk(entity, 106 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(tick_size)), pixels(static_cast<int>(tick_size))})
              .with_absolute_position()
              .with_translate(tick_x, tick_y)
              .with_custom_background(text_muted)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(1.0f)
              .with_debug_name("compass_tick_" + std::to_string(i)));
    }

    // ========== TOP RIGHT: Score & Objective ==========
    div(context, mk(entity, 110),
        ComponentConfig{}
            .with_label("SCORE: 15,000")
            .with_size(ComponentSize{pixels(180), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 210.0f, 18.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("score"));

    div(context, mk(entity, 111),
        ComponentConfig{}
            .with_label("(U)")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_absolute_position()
            .with_translate((float)screen_w - 40.0f, 15.0f)
            .with_custom_background(text_tan)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("score_icon"));

    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_label(">> SECURE OBJECTIVE B")
            .with_size(ComponentSize{pixels(230), pixels(30)})
            .with_absolute_position()
            .with_translate((float)screen_w - 255.0f, 52.0f)
            .with_custom_background(gold_accent)
            .with_font("BlackOpsOne", 16.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("objective"));

    // ========== KILL FEED ==========
    float kill_y = 120.0f;
    for (size_t i = 0; i < kill_feed.size(); i++) {
      div(context, mk(entity, 120 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(kill_feed[i])
              .with_size(ComponentSize{pixels(500), pixels(22)})
              .with_absolute_position()
              .with_translate(cx - 250.0f, kill_y + (float)i * 22.0f)
              .with_font("EqProRounded", 19.0f)
              .with_custom_text_color(text_muted)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("kill_" + std::to_string(i)));
    }

    // ========== RIGHT: Voice Indicator ==========
    div(context, mk(entity, 130),
        ComponentConfig{}
            .with_label("@ * Alpha_Six: Under fire!")
            .with_size(ComponentSize{pixels(220), pixels(22)})
            .with_absolute_position()
            .with_translate((float)screen_w - 240.0f, 195.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("voice"));

    // ========== LEFT: Killstreak Icons ==========
    // Array of skill textures and labels
    std::vector<std::tuple<raylib::Texture2D*, std::string, std::string>> skill_icons = {
        {&icon_uav_tex, "[T]", "UAV"},
        {&icon_recon_tex, "[O]", ""},
        {&icon_shield_tactical_tex, "[U]", ""},
        {&icon_strike_tex, "[X]", ""},
        {&icon_danger_tex, "/!\\", ""},
    };

    float ks_y = 140.0f;
    for (size_t i = 0; i < skill_icons.size(); i++) {
      float row_y = ks_y + (float)i * 72.0f;
      auto& [tex_ptr, fallback_label, label] = skill_icons[i];

      // Cog/gear icon
      div(context, mk(entity, 140 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label("*")
              .with_size(ComponentSize{pixels(18), pixels(18)})
              .with_absolute_position()
              .with_translate(22.0f, row_y + 18.0f)
              .with_font("EqProRounded", 19.0f)
              .with_custom_text_color(text_muted)
              .with_debug_name("cog_" + std::to_string(i)));

      // Icon box background
      div(context, mk(entity, 141 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(55), pixels(55)})
              .with_absolute_position()
              .with_translate(45.0f, row_y)
              .with_custom_background(panel_dark)
              .with_border(border_dark, 1.0f)
              .with_debug_name("ks_bg_" + std::to_string(i)));

      // Icon image or fallback text
      if (tex_ptr && tex_ptr->id != 0) {
        afterhours::texture_manager::Rectangle src{0, 0, (float)tex_ptr->width, (float)tex_ptr->height};
        sprite(context, mk(entity, 142 + static_cast<int>(i) * 3), *tex_ptr, src,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(40), pixels(40)})
                .with_absolute_position()
                .with_translate(52.5f, row_y + 7.5f)
                .with_debug_name("ks_icon_" + std::to_string(i)));
      } else {
        div(context, mk(entity, 142 + static_cast<int>(i) * 3),
            ComponentConfig{}
                .with_label(fallback_label)
                .with_size(ComponentSize{pixels(55), pixels(55)})
                .with_absolute_position()
                .with_translate(45.0f, row_y)
                .with_font("EqProRounded", 19.0f)
                .with_custom_text_color(text_tan)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("ks_icon_fallback_" + std::to_string(i)));
      }

      // Label (only for UAV)
      if (!label.empty()) {
        div(context, mk(entity, 143 + static_cast<int>(i) * 3),
            ComponentConfig{}
                .with_label(label)
                .with_size(ComponentSize{pixels(55), pixels(16)})
                .with_absolute_position()
                .with_translate(45.0f, row_y + 56.0f)
                .with_font("EqProRounded", 19.0f)
                .with_custom_text_color(text_muted)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("ks_label_" + std::to_string(i)));
      }
    }

    // ========== BOTTOM LEFT: Killstreak Bar ==========
    float bl_y = (float)screen_h - 195.0f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(175), pixels(26)})
            .with_absolute_position()
            .with_translate(22.0f, bl_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 1.0f)
            .with_debug_name("ks_bar"));

    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("@ KILLSTREAK")
            .with_size(ComponentSize{pixels(115), pixels(20)})
            .with_absolute_position()
            .with_translate(28.0f, bl_y + 3.0f)
            .with_font("EqProRounded", 14.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("ks_text"));

    // Killstreak progress boxes
    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position()
            .with_translate(150.0f, bl_y + 5.0f)
            .with_custom_background(text_tan)
            .with_debug_name("ks_box1"));

    div(context, mk(entity, 203),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(16), pixels(16)})
            .with_absolute_position()
            .with_translate(170.0f, bl_y + 5.0f)
            .with_custom_background(border_dark)
            .with_debug_name("ks_box2"));

    // ========== BOTTOM LEFT: Minimap ==========
    float map_y = (float)screen_h - 160.0f;

    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("7B Pop <192>")
            .with_size(ComponentSize{pixels(175), pixels(18)})
            .with_absolute_position()
            .with_translate(22.0f, map_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("map_label"));

    // Minimap background
    div(context, mk(entity, 220),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(175), pixels(135)})
            .with_absolute_position()
            .with_translate(22.0f, map_y + 18.0f)
            .with_custom_background(minimap_green)
            .with_border(border_dark, 2.0f)
            .with_debug_name("minimap"));

    // Map grid lines (vertical)
    for (int i = 1; i < 4; i++) {
      div(context, mk(entity, 230 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1), pixels(120)})
              .with_absolute_position()
              .with_translate(22.0f + (float)i * 43.0f, map_y + 26.0f)
              .with_custom_background(afterhours::Color{60, 70, 60, 180})
              .with_debug_name("grid_v_" + std::to_string(i)));
    }

    // Map grid lines (horizontal)
    for (int i = 1; i < 3; i++) {
      div(context, mk(entity, 240 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(160), pixels(1)})
              .with_absolute_position()
              .with_translate(30.0f, map_y + 18.0f + (float)i * 45.0f)
              .with_custom_background(afterhours::Color{60, 70, 60, 180})
              .with_debug_name("grid_h_" + std::to_string(i)));
    }

    // Red danger zone on map
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(55), pixels(50)})
            .with_absolute_position()
            .with_translate(135.0f, map_y + 28.0f)
            .with_custom_background(afterhours::Color{140, 50, 40, 120})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("danger_zone"));

    // Player icon on map
    div(context, mk(entity, 260),
        ComponentConfig{}
            .with_label("^")
            .with_size(ComponentSize{pixels(18), pixels(18)})
            .with_absolute_position()
            .with_translate(100.0f, (float)screen_h - 75.0f)
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(text_tan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("player_icon"));

    // Objective marker
    div(context, mk(entity, 261),
        ComponentConfig{}
            .with_label("*")
            .with_size(ComponentSize{pixels(12), pixels(12)})
            .with_absolute_position()
            .with_translate(150.0f, map_y + 45.0f)
            .with_custom_background(dot_red)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)
            .with_debug_name("obj_marker"));

    // S marker below map
    div(context, mk(entity, 270),
        ComponentConfig{}
            .with_label("S $")
            .with_size(ComponentSize{pixels(30), pixels(16)})
            .with_absolute_position()
            .with_translate(95.0f, (float)screen_h - 18.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("s_marker"));

    // ========== BOTTOM CENTER: Ammo ==========
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label(std::to_string(ammo_clip) + "/" + std::to_string(ammo_reserve))
            .with_size(ComponentSize{pixels(140), pixels(55)})
            .with_absolute_position()
            .with_translate(230.0f, (float)screen_h - 90.0f)
            .with_font("EqProRounded", 42.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("ammo"));

    // ========== BOTTOM CENTER: Health & Armor ==========
    float health_x = 400.0f;
    float health_y = (float)screen_h - 95.0f;

    // Health panel
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(230), pixels(60)})
            .with_absolute_position()
            .with_translate(health_x, health_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 1.0f)
            .with_debug_name("health_panel"));

    // Skull icon for health panel
    if (icon_skull_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)icon_skull_tex.width, (float)icon_skull_tex.height};
      sprite(context, mk(entity, 311), icon_skull_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(32), pixels(32)})
              .with_absolute_position()
              .with_translate(health_x + 10.0f, health_y + 14.0f)
              .with_debug_name("skull_icon"));
    } else {
      div(context, mk(entity, 311),
          ComponentConfig{}
              .with_label("@")
              .with_size(ComponentSize{pixels(35), pixels(35)})
              .with_absolute_position()
              .with_translate(health_x + 8.0f, health_y + 12.0f)
              .with_font("EqProRounded", 24.0f)
              .with_custom_text_color(text_tan)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("skull"));
    }

    // Health label
    div(context, mk(entity, 312),
        ComponentConfig{}
            .with_label("80 HEALTH")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_absolute_position()
            .with_translate(health_x + 50.0f, health_y + 6.0f)
            .with_font("EqProRounded", 16.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("health_label"));

    // Health bar bg
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(165), pixels(14)})
            .with_absolute_position()
            .with_translate(health_x + 50.0f, health_y + 26.0f)
            .with_custom_background(afterhours::Color{25, 25, 22, 255})
            .with_debug_name("health_bg"));

    // Health bar fill
    div(context, mk(entity, 321),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(160 * health_pct)), pixels(14)})
            .with_absolute_position()
            .with_translate(health_x + 50.0f, health_y + 26.0f)
            .with_custom_background(health_cyan)
            .with_debug_name("health_fill"));

    // Armor bar bg
    div(context, mk(entity, 330),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(165), pixels(10)})
            .with_absolute_position()
            .with_translate(health_x + 50.0f, health_y + 44.0f)
            .with_custom_background(afterhours::Color{25, 25, 22, 255})
            .with_debug_name("armor_bg"));

    // Armor bar fill
    div(context, mk(entity, 331),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(160 * armor_pct)), pixels(10)})
            .with_absolute_position()
            .with_translate(health_x + 50.0f, health_y + 44.0f)
            .with_custom_background(armor_blue)
            .with_debug_name("armor_fill"));

    // ========== BOTTOM RIGHT: Equipment ==========
    float eq_x = (float)screen_w - 185.0f;
    float eq_y = (float)screen_h - 105.0f;

    // Grenade (x2) - highlighted with gold border
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("x2")
            .with_size(ComponentSize{pixels(25), pixels(18)})
            .with_absolute_position()
            .with_translate(eq_x - 25.0f, eq_y + 40.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("x2"));

    div(context, mk(entity, 410),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(70), pixels(70)})
            .with_absolute_position()
            .with_translate(eq_x, eq_y)
            .with_custom_background(panel_dark)
            .with_border(gold_accent, 3.0f)
            .with_debug_name("grenade_bg"));
    if (weapon_grenade_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)weapon_grenade_tex.width, (float)weapon_grenade_tex.height};
      sprite(context, mk(entity, 411), weapon_grenade_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(50), pixels(50)})
              .with_absolute_position()
              .with_translate(eq_x + 10.0f, eq_y + 10.0f)
              .with_debug_name("grenade_icon"));
    }

    // Knife (x1)
    div(context, mk(entity, 420),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(70), pixels(70)})
            .with_absolute_position()
            .with_translate(eq_x + 80.0f, eq_y)
            .with_custom_background(panel_dark)
            .with_border(border_dark, 2.0f)
            .with_debug_name("knife_bg"));
    if (weapon_melee_tex.id != 0) {
      afterhours::texture_manager::Rectangle src{0, 0, (float)weapon_melee_tex.width, (float)weapon_melee_tex.height};
      sprite(context, mk(entity, 422), weapon_melee_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(50), pixels(50)})
              .with_absolute_position()
              .with_translate(eq_x + 90.0f, eq_y + 10.0f)
              .with_debug_name("knife_icon"));
    }

    div(context, mk(entity, 421),
        ComponentConfig{}
            .with_label("x1")
            .with_size(ComponentSize{pixels(25), pixels(18)})
            .with_absolute_position()
            .with_translate(eq_x + 145.0f, eq_y + 72.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_tan)
            .with_debug_name("x1"));

    // ========== CENTER: Crosshair ==========
    if (crosshair_tex.id != 0) {
      float cross_cx = (float)screen_w / 2.0f;
      float cross_cy = (float)screen_h / 2.0f;
      afterhours::texture_manager::Rectangle src{0, 0, (float)crosshair_tex.width, (float)crosshair_tex.height};
      sprite(context, mk(entity, 600), crosshair_tex, src,
          ComponentConfig{}
              .with_size(ComponentSize{pixels(64), pixels(64)})
              .with_absolute_position()
              .with_translate(cross_cx - 32.0f, cross_cy - 32.0f)
              .with_debug_name("crosshair"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter HUD overlay", NeonStrikeScreen)
