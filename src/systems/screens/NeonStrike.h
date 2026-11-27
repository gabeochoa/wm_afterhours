#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <cmath>
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
    // Square stage: the square ref (1024x1024) fits a centered 720x720
    // stage; the dark playfield shows in the side margins on wide windows.
    const float sy = std::min(context.screen_width / 720.f, context.screen_height / 720.f);
    const float sx = sy;
    const float left = (context.screen_width - 720 * sy) / 2;
    const float top = (context.screen_height - 720 * sy) / 2;
    Theme theme;
    theme.font = {183, 185, 170, 255};
    theme.background = {13, 16, 15, 255};
    theme.accent = {194, 155, 77, 255};
    theme.corner_radius = 0;
    context.set_theme(theme);
    UIStylingDefaults::get().set_default_font("ArchivoMock", pixels(18 * sy));
    static constexpr std::array<const char *, 17> names{
        "atmosphere", "compass", "shield", "mic",   "gear0",
        "uav",        "gear1",   "recon",  "gear2", "ability_shield",
        "gear3",      "strike",  "streak", "map",   "skull",
        "grenade",    "knife"};
    if (!loaded) {
      loaded = true;
      for (size_t i = 0; i < names.size(); ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 15 || i == 16) continue;
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
    if (afterhours::input::is_key_pressed(raylib::KEY_G)) {
      selected_equipment = 0;
      status = "Grenade selected";
    }
    if (afterhours::input::is_key_pressed(raylib::KEY_V)) {
      selected_equipment = 1;
      status = "Melee selected";
    }

    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * sx), pixels(h * sy)})
          .with_absolute_position(left + x * sx, top + y * sy)
          .with_background(Theme::Usage::None)
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w,
                    float h, float size, afterhours::Color color,
                    const char *debug = "", bool bold = true, TextAlignment alignment = TextAlignment::Left) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_label(value)
              .with_font(bold ? "ArchivoMockBold" : "ArchivoMock", pixels(size * sy))
              .with_custom_text_color(color)
              .with_alignment(id == 80 || id == 83 ? TextAlignment::Right : alignment)
              .with_letter_spacing(0)
              .with_ignore_pointer_events().with_debug_name(debug));
    };
    auto panel = [&](int id, float x, float y, float w, float h,
                     afterhours::Color fill, afterhours::Color edge) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_ignore_pointer_events().with_custom_background(fill)
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
    const afterhours::Color muted{152, 164, 142, 255};
    const afterhours::Color edge{81, 92, 76, 255};
    const afterhours::Color gold{194, 155, 77, 255};
    div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({13, 16, 15, 255}).with_corner_radius(0));
    div(context, mk(entity), box(0, 0, 720, 720)
        .with_custom_background({13, 16, 15, 255}).with_debug_name("neon_root"));
    picture(1, 0, 0, 0, 720, 720);
    panel(2, 0, 0, 720, 720, {13, 16, 15, 105}, {0, 0, 0, 0});
    panel(3, 140, 38, 440, 1, {73, 76, 60, 110}, {73, 76, 60, 110});
    for (int i = 0; i < 44; ++i)
      panel(10 + i, 140.f + 10.f * i, 32, 1, i % 10 == 0 ? 8 : 4,
            {92, 100, 86, 150}, {92, 100, 86, 150});
    const std::array<const char *, 5> bearings{"300", "330", "N", "030", "060"};
    for (int i = 0; i < 5; ++i)
      text(60 + i, bearings[i], 134.f + i * 105.f, 8, 44, 24, i == 2 ? 20 : 17, ink);
    picture(70, 1, 314, 42, 92, 92);
    text(71, "N", 352, 40, 22, 23, 18, ink);
    text(72, "S", 352, 112, 22, 23, 17, muted);
    text(73, "W", 307, 76, 24, 23, 17, muted);
    text(74, "E", 394, 76, 24, 23, 17, muted);
    text(80, "SCORE 15,000", 496, 25, 200, 32, 27, ink, "neon_score");
    panel(82, 490, 67, 220, 38, {28, 30, 21, 220}, {133, 115, 75, 190});
    text(83, "SECURE OBJECTIVE B", 500, 73, 200, 26, 23, gold, "neon_objective");
    // Sparse kill feed (ref: small lines under the compass, no panel).
    const std::array<const char *, 3> actors{"Alpha_Six", "Tsha_Rio", "Alpha_Six"};
    const std::array<const char *, 3> events{"eliminated Emepine / M4", "eliminated Emepine / SCAR", "radio: requested cover"};
    for (int i = 0; i < 3; ++i) {
      text(300 + i * 2, actors[i], 150, 148.f + i * 22, 104, 20, 15, gold);
      text(301 + i * 2, events[i], 258, 148.f + i * 22, 260, 20, 15, ink, "", false);
    }
    // Voice line (ref: small, right of center).
    picture(351, 3, 538, 170, 20, 24);
    text(352, "Alpha_Six: Under fire!", 566, 168, 150, 24, 17, ink);
    const auto keycap = [&](int id, const std::string &key, float x, float y, float w = 28.f) {
      panel(id, x, y, w, 26, {42, 49, 38, 255}, {111, 124, 99, 255});
      text(id + 1, key, x, y + 1, w, 24, 18, ink, "", true, TextAlignment::Center);
    };
    // Left vertical icon rail (ref: small square icons, not wide rows).
    for (int i = 0; i < 4; ++i) {
      const float y = 170.f + i * 66.f;
      const bool selected = selected_ability == i;
      if (button(context, mk(entity, 110 + i), box(12, y, 56, 56)
          .with_custom_background({19, 26, 21, 230}).with_debug_name("neon_ability_" + std::to_string(i))
          .with_on_draw_fg([selected, edge, gold, sy](RectangleType r) {
            raylib::DrawRectangleLinesEx(r, sy, selected ? gold : edge);
            if (selected) afterhours::draw_rectangle({r.x, r.y + r.height - 3 * sy, r.width, 3 * sy}, gold);
          }))) {
        selected_ability = i;
        status = std::string(abilities[i]) + " selected";
      }
      if (i < 3) picture(130 + i, 5 + i * 2, 20, y + 8, 40, 40);
      else div(context, mk(entity, 133), box(20, y + 8, 40, 40).with_ignore_pointer_events()
          .with_on_draw_fg([ink](RectangleType r) {
            const float s = r.width / 40;
            raylib::DrawLineEx({r.x + 27 * s, r.y + 7 * s}, {r.x + 10 * s, r.y + 27 * s}, 5 * s, ink);
            raylib::DrawLineEx({r.x + 20 * s, r.y + 8 * s}, {r.x + 31 * s, r.y + 17 * s}, 3 * s, ink);
            raylib::DrawLineEx({r.x + 9 * s, r.y + 23 * s}, {r.x + 16 * s, r.y + 29 * s}, 3 * s, ink);
            raylib::DrawLineEx({r.x + 8 * s, r.y + 32 * s}, {r.x + 30 * s, r.y + 32 * s}, 2 * s, ink);
            raylib::DrawLineEx({r.x + 3 * s, r.y + 29 * s}, {r.x + 8 * s, r.y + 23 * s}, 2 * s, ink);
          }));
      std::string name = abilities[i];
      std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
      text(140 + i, name, 12, y + 58, 56, 18, 13, ink, "", true, TextAlignment::Center);
      keycap(410 + i * 2, std::to_string(i + 1), 62, y + 15);
    }
    panel(160, 344, 344, 8, 1, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(161, 368, 344, 8, 1, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(162, 359, 329, 1, 8, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(163, 359, 353, 1, 8, {177, 192, 171, 118}, {177, 192, 171, 118});
    panel(164, 359, 344, 2, 2, {213, 217, 198, 118}, {213, 217, 198, 118});
    // Minimap bottom-left with killstreak header (ref).
    panel(170, 12, 520, 200, 26, {20, 24, 19, 230}, edge);
    text(172, "KILLSTREAK", 20, 523, 150, 20, 15, ink);
    for (int i = 0; i < 3; ++i)
      panel(173 + i, 170.f + i * 16.f, 528, 11, 11, i == 0 ? ink : edge, muted);
    panel(180, 12, 550, 200, 158, {14, 23, 21, 255}, {101, 112, 93, 255});
    div(context, mk(entity), box(18, 556, 188, 128).with_ignore_pointer_events()
        .with_on_draw_fg([gold](RectangleType r) {
          const float x = r.width / 201.f, y = r.height / 173.f;
          const auto rect = [&](float px, float py, float w, float h, afterhours::Color color) {
            afterhours::draw_rectangle({r.x + px * x, r.y + py * y, w * x, h * y}, color);
          };
          const afterhours::Color road{97, 112, 103, 255};
          rect(0, 0, 201, 173, {34, 52, 46, 255});
          rect(78, 0, 22, 173, road); rect(0, 61, 100, 18, road);
          rect(100, 48, 101, 18, road); rect(0, 105, 201, 20, road);
          rect(123, 111, 20, 62, road);
          rect(24, 16, 35, 29, {62, 79, 71, 255});
          rect(24, 89, 35, 15, {62, 79, 71, 255});
          rect(149, 138, 33, 34, {62, 79, 71, 255});
          rect(106, 0, 95, 48, {158, 122, 67, 75});
          const raylib::Vector2 player{r.x + 103 * x, r.y + 119 * y};
          raylib::DrawTriangle(player, {r.x + 120 * x, r.y + 88 * y},
                               {r.x + 96 * x, r.y + 86 * y}, {210, 181, 108, 48});
          raylib::DrawTriangle({player.x, player.y - 8 * y}, {player.x - 6 * x, player.y + 6 * y},
                               {player.x + 6 * x, player.y + 6 * y}, {19, 35, 31, 255});
          raylib::DrawTriangle({player.x, player.y - 5 * y}, {player.x - 3 * x, player.y + 4 * y},
                               {player.x + 3 * x, player.y + 4 * y}, {242, 245, 225, 255});
          for (const auto &p : std::array<raylib::Vector2, 4>{{{46, 48}, {170, 49}, {163, 66}, {45, 148}}})
            raylib::DrawRing({r.x + p.x * x, r.y + p.y * y}, 3.1f * x, 4.6f * x, 0, 360, 24, {99, 210, 218, 255});
          raylib::DrawCircleV({r.x + 145 * x, r.y + 28 * y}, 11 * x, gold);
        }).with_debug_name("neon_map"));
    text(182, "B", 160, 564, 30, 28, 20, {22, 35, 29, 255});
    text(183, "SECTOR 7B / GRID 042", 18, 684, 188, 20, 14, ink);
    // Ammo + health bottom center (ref: "35/210" combined).
    text(191, std::to_string(ammo_clip) + "/" + std::to_string(ammo_reserve), 226, 596, 110, 40, 34, ink, "neon_ammo");
    text(193, "M4 / AUTOMATIC", 226, 640, 150, 22, 15, muted);
    if (button(context, mk(entity), box(226, 566, 190, 46)
        .with_custom_background({15, 23, 19, 140}).with_debug_name("neon_reload"))) reload();
    keycap(430, "R", 380, 576);
    text(196, "Reload", 414, 577, 64, 24, 16, ink);
    picture(201, 14, 226, 670, 23, 23);
    text(202, "HEALTH 80/100", 257, 668, 140, 24, 16, {147, 212, 214, 255});
    panel(203, 400, 674, 120, 10, {29, 38, 34, 255}, edge);
    panel(204, 402, 676, 93, 6, {115, 179, 183, 255}, {115, 179, 183, 255});
    // Equipment squares bottom-right (ref).
    for (int i = 0; i < 2; ++i) {
      const float x = 580.f + i * 88.f;
      const bool selected = selected_equipment == i;
      if (button(context, mk(entity, 210 + i), box(x, 592, 80, 64)
          .with_custom_background({20, 25, 20, 230}).with_debug_name(i == 0 ? "neon_grenade" : "neon_melee")
          .with_on_draw_fg([selected, edge, gold, sy](RectangleType r) {
            raylib::DrawRectangleLinesEx(r, sy, selected ? gold : edge);
            if (selected) afterhours::draw_rectangle({r.x + 1, r.y + r.height - 4 * sy, r.width - 2, 3 * sy}, gold);
          }))) {
        selected_equipment = i;
        status = i == 0 ? "Grenade selected" : "Melee selected";
      }
      div(context, mk(entity, 220 + i), box(x + 20, 598, 40, 40).with_ignore_pointer_events()
          .with_on_draw_fg([i, ink](RectangleType r) {
            const float s = r.width / 40.f;
            const auto line = [&](float x1, float y1, float x2, float y2, float width = 2.f) {
              raylib::DrawLineEx({r.x + x1 * s, r.y + y1 * s}, {r.x + x2 * s, r.y + y2 * s}, width * s, ink);
            };
            if (i == 1) {
              line(13, 29, 32, 5); line(32, 5, 32, 18); line(32, 18, 21, 33);
              line(21, 33, 13, 29); line(13, 29, 5, 39, 5); line(9, 25, 25, 37, 3);
              return;
            }
            for (int segment = 0; segment < 32; ++segment) {
              const float a = segment * 6.2831853f / 32.f, b = (segment + 1) * 6.2831853f / 32.f;
              line(20 + 11 * std::cos(a), 23 + 15 * std::sin(a), 20 + 11 * std::cos(b), 23 + 15 * std::sin(b));
            }
            line(14, 11, 14, 35); line(25, 11, 25, 35);
            line(10, 18, 30, 18); line(9, 27, 31, 27);
            line(13, 4, 26, 4); line(12, 8, 27, 8); line(26, 4, 33, 11);
          }));
      text(230 + i, i == 0 ? "x2" : "x1", x + 48, 632, 30, 22, 18, ink,
           i == 0 ? "neon_grenade_count" : "neon_melee_count");
      text(232 + i, i == 0 ? "GRENADE" : "MELEE", x + 8, 640, 64, 14, 11, ink);
      keycap(440 + i * 2, i == 0 ? "G" : "V", x + 8, 602, 26);
    }
    if (!status.empty()) text(250, status, 240, 536, 300, 28, 20, gold, "neon_status");
    text(251, "Streaks: keys 1-4", 240, 694, 300, 20, 14, muted, "", false);
  }
};
REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter HUD overlay", NeonStrikeScreen)
