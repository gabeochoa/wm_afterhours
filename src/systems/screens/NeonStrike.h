#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NeonStrikeScreen : ScreenSystem<UIContext<InputAction>> {
  int ammo_clip = 35;
  int ammo_reserve = 210;
  int selected_ability = -1;
  int selected_equipment = 0;
  std::string status;
  bool loaded = false;
  std::array<raylib::Texture2D, 17> textures{};
  const std::array<const char *, 4> abilities{"UAV", "Recon", "Shield",
                                              "Strike"};

  void reload() {
    int rounds = std::min(40 - ammo_clip, ammo_reserve);
    ammo_clip += rounds;
    ammo_reserve -= rounds;
    status = rounds ? "Weapon reloaded" : "Magazine full";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float sx = context.screen_width / 1280.f;
    const float sy = context.screen_height / 720.f;
    Theme theme;
    theme.font = {183, 185, 170, 255};
    theme.background = {13, 16, 15, 255};
    theme.accent = {194, 155, 77, 255};
    theme.corner_radius = 0;
    context.set_theme(theme);
    UIStylingDefaults::get().set_default_font("Archivo", pixels(18 * sy));
    static constexpr std::array<const char *, 17> names{
        "atmosphere", "compass", "shield", "mic",   "gear0",
        "uav",        "gear1",   "recon",  "gear2", "ability_shield",
        "gear3",      "strike",  "streak", "map",   "skull",
        "grenade",    "knife"};
    if (!loaded) {
      loaded = true;
      for (size_t i = 0; i < names.size(); ++i) {
        textures[i] = raylib::LoadTexture(
            afterhours::files::get_resource_path(
                "images", std::string("neon_strike/") + names[i] + ".png")
                .string()
                .c_str());
        raylib::SetTextureFilter(textures[i], raylib::TEXTURE_FILTER_BILINEAR);
      }
    }
    if (afterhours::input::is_key_pressed(raylib::KEY_R))
      reload();
    for (int i = 0; i < 4; ++i) {
      if (afterhours::input::is_key_pressed(raylib::KEY_ONE + i)) {
        selected_ability = i;
        status = std::string(abilities[i]) + " selected";
      }
    }
    if (afterhours::input::is_key_pressed(raylib::KEY_G))
      selected_equipment = 0;
    if (afterhours::input::is_key_pressed(raylib::KEY_V))
      selected_equipment = 1;

    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * sx), pixels(h * sy)})
          .with_absolute_position(x * sx, y * sy)
          .with_background(Theme::Usage::None)
          .with_corner_radius(0);
    };
    auto text = [&](int id, std::string value, float x, float y, float w,
                    float h, float size, afterhours::Color color,
                    const char *debug = "", bool bold = true) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_label(value)
              .with_font(bold ? "ArchivoMockBold" : "Archivo", pixels(size * sy))
              .with_custom_text_color(color)
              .with_alignment(id == 80 || id == 83 ? TextAlignment::Right : TextAlignment::Left)
              .with_letter_spacing(0)
              .with_debug_name(debug));
    };
    auto panel = [&](int id, float x, float y, float w, float h,
                     afterhours::Color fill, afterhours::Color edge) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_custom_background(fill)
              .with_on_draw_fg([edge, sy](RectangleType r) {
                raylib::DrawRectangleLinesEx(r, sy, edge);
              }));
    };
    auto picture = [&](int id, size_t index, float x, float y, float w,
                       float h) {
      auto t = textures[index];
      sprite(context, mk(entity, id), t,
             {0, 0, (float)t.width, (float)t.height},
             box(x, y, w, h).with_ignore_pointer_events());
    };
    const afterhours::Color ink{183, 185, 170, 255};
    const afterhours::Color muted{121, 133, 113, 255};
    const afterhours::Color edge{81, 92, 76, 255};
    const afterhours::Color gold{194, 155, 77, 255};
    div(context, mk(entity, 0),
        box(0, 0, 1280, 720)
            .with_custom_background({13, 16, 15, 255})
            .with_debug_name("neon_root"));
    picture(1, 0, 0, 0, 1280, 720);
    panel(2, 420, 34, 440, 1, {73, 76, 60, 80}, {73, 76, 60, 80});
    for (int i = 0; i < 44; ++i)
      panel(10 + i, 420.f + 10.f * i, 31, 1, 4, {72, 78, 69, 96},
            {72, 78, 69, 96});
    const std::array<const char *, 5> bearings{"300", "330", "N", "030", "060"};
    for (int i = 0; i < 5; ++i)
      text(60 + i, bearings[i], 420.f + i * 105.f, 11, 30, 20, i == 2 ? 16 : 12,
           muted);
    picture(70, 1, 594, 29, 92, 92);
    text(71, "N", 636, 29, 14, 17, 16, ink);
    text(72, "S", 636, 104, 14, 17, 14, muted);
    text(73, "W", 597, 68, 14, 17, 14, muted);
    text(74, "E", 675, 68, 14, 17, 14, muted);
    text(80, "SCORE: 15,000", 1057, 25, 151, 30, 25, ink, "neon_score", true);
    picture(81, 2, 1218, 28, 26, 26);
    panel(82, 1028, 62, 216, 23, {0, 0, 0, 0}, {133, 115, 75, 105});
    text(83, ">> SECURE OBJECTIVE B", 1035, 60, 212, 28, 21, gold,
         "neon_objective", true);
    const std::array<const char *, 3> feed{
        "Alpha_Six eliminated [Emepine] with M4",
        "[Tsha_Rio] eliminated [Emepine] with SCAR", "Alpha_Six - Under fire!"};
    for (int i = 0; i < 3; ++i) {
      panel(90 + i, 435, 136.f + i * 20, 5, 5, ink, ink);
      text(95 + i, feed[i], 449, 126.f + i * 20, 380, 24, 18, ink);
    }
    picture(100, 3, 1032, 192, 20, 20);
    text(101, "Alpha_Six: Under fire!", 1062, 187, 182, 28, 20, ink);
    panel(105, 120, 194, 1, 278, edge, edge);
    for (int i = 0; i < 4; ++i) {
      const float y = 194.f + i * 74.f;
      bool selected = selected_ability == i;
      auto cfg =
          box(55, y, 53, 56)
              .with_custom_background({21, 26, 21, 80})
              .with_debug_name("neon_ability_" + std::to_string(i))
              .with_on_draw_fg([selected, edge, gold, sy](RectangleType r) {
                raylib::DrawRectangleLinesEx(r, (selected ? 2 : 1) * sy,
                                             selected ? gold : edge);
              });
      if (button(context, mk(entity, 110 + i), cfg)) {
        selected_ability = i;
        status = std::string(abilities[i]) + " selected";
      }
      picture(120 + i, 4 + i * 2, 28, y + 19, 18, 18);
      picture(130 + i, 5 + i * 2, 65, y + 4, 33, 33);
      std::string label = abilities[i];
      std::transform(label.begin(), label.end(), label.begin(),
                     [](unsigned char c) { return std::toupper(c); });
      text(140 + i, label, 63, y + 35, 44, 18, 13, ink);
      text(150 + i, std::to_string(i + 1), 128, y + 18, 15, 18, 13, muted);
    }
    panel(160, 624, 359, 8, 1, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(161, 648, 359, 8, 1, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(162, 639, 344, 1, 8, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(163, 639, 368, 1, 8, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(164, 639, 359, 2, 2, {213, 217, 198, 118}, {213, 217, 198, 118});
    panel(170, 31, 474, 213, 25, {20, 24, 19, 153}, edge);
    picture(171, 12, 38, 478, 17, 17);
    text(172, "KILLSTREAK", 60, 474, 110, 23, 17, ink);
    for (int i = 0; i < 3; ++i)
      panel(173 + i, 198.f + i * 13.f, 482, 10, 9, i == 0 ? ink : edge, muted);
    panel(180, 31, 505, 213, 185, {14, 23, 21, 255}, {101, 112, 93, 255});
    picture(181, 13, 37, 511, 201, 173);
    text(182, "B", 185, 532, 10, 15, 12, {37, 47, 41, 255});
    text(183, "SECTOR 7B / GRID 042", 43, 668, 150, 15, 11,
         {165, 177, 167, 255});
    if (button(context, mk(entity, 190),
               box(266, 570, 140, 44)
                   .with_label(std::to_string(ammo_clip) + "/" +
                               std::to_string(ammo_reserve))
                   .with_font("ArchivoMockBold", pixels(40 * sy))
                   .with_custom_text_color({197, 197, 182, 255})
                   .with_alignment(TextAlignment::Left)
                  .with_debug_name("neon_reload")))
      reload();
    panel(200, 266, 617, 46, 50, {20, 26, 19, 255}, edge);
    picture(201, 14, 271.5, 624, 35, 35);
    text(202, "80 HEALTH", 322, 620, 176, 21, 16, {120, 157, 160, 255});
    panel(203, 322, 644, 176, 9, {29, 38, 34, 255}, edge);
    panel(204, 324, 646, 138, 5, {115, 179, 183, 255}, {115, 179, 183, 255});
    panel(205, 322, 658, 148, 7, {29, 38, 34, 255}, edge);
    panel(206, 324, 660, 65, 3, {121, 167, 167, 255}, {121, 167, 167, 255});
    text(207, "45% ARMOR", 322, 665, 148, 18, 11, muted);
    for (int i = 0; i < 2; ++i) {
      float x = 1074.f + i * 92.f;
      bool selected = selected_equipment == i;
      if (button(
              context, mk(entity, 210 + i),
              box(x, 607, 78, 80)
                  .with_custom_background({20, 25, 20, 68})
                  .with_debug_name(i == 0 ? "neon_grenade" : "neon_melee")
                  .with_on_draw_fg([selected, edge, gold, sy](RectangleType r) {
                    raylib::DrawRectangleLinesEx(r, sy, selected ? gold : edge);
                  }))) {
        selected_equipment = i;
        status = i == 0 ? "Grenade selected" : "Melee selected";
      }
      picture(220 + i, 15 + i, x + 10.5, 617, 57, 57);
      text(230 + i, i == 0 ? "x2" : "x1", i == 0 ? x - 30 : x + 50, 655, 30, 28,
           26, ink);
      text(240 + i, i == 0 ? "G" : "V", x + 64, 608, 13, 18, 13, muted);
    }
    if (!status.empty())
      text(250, status, 480, 660, 400, 30, 20, gold, "neon_status");
  }
};
REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter HUD overlay", NeonStrikeScreen)
