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
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ParcelCorpsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int language_idx = 0;
  bool subtitles = true;
  bool fullscreen = true;
  bool motion_blur = true;
  bool vsync = true;
  float audio_volume = 0.85f;
  float music_volume = 0.75f;
  size_t msaa_idx = 1;
  size_t texture_idx = 2;
  std::string status = "Saved locally";
  bool settings_open = true;
  bool texture_loaded = false;
  raylib::Texture2D town_texture{};

  std::array<std::string, 4> languages = {"King's English", "English US",
                                          "Deutsch", "Francais"};
  std::array<std::string, 4> msaa_options = {"Off", "2x", "4x", "8x"};
  std::array<std::string, 4> texture_options = {"Low", "Medium", "High",
                                                "Ultra"};

  const afterhours::Color phone_black{6, 7, 8, 255};
  const afterhours::Color screen_dark{34, 39, 34, 255};
  const afterhours::Color row_dark{39, 45, 39, 245};
  const afterhours::Color separator{86, 91, 81, 255};
  const afterhours::Color white{250, 252, 248, 255};
  const afterhours::Color muted{176, 182, 166, 255};
  const afterhours::Color green{48, 205, 44, 255};
  const afterhours::Color blue{64, 158, 224, 255};
  const afterhours::Color red{222, 47, 42, 255};
  const afterhours::Color orange{244, 165, 46, 255};
  const afterhours::Color cyan{77, 188, 198, 255};

  std::vector<std::pair<std::string, std::string>> chat_messages = {
      {"reveredsoup", "you did it :)"},
      {"untrackedjackstay", "Cool :)"},
      {"NimbleGrimble", "YES :)"},
      {"untrackedjackstay", "ZZZzzzz"},
      {"sniprit09", "Zzzz"},
  };

  void load_texture_if_needed() {
    if (texture_loaded)
      return;
    texture_loaded = true;
    town_texture =
        raylib::LoadTexture(afterhours::files::get_resource_path(
                                "images", "parcel_corps_settings/town.png")
                                .string()
                                .c_str());
  }

  static void paint(raylib::Texture2D texture, RectangleType r) {
    if (texture.id == 0)
      return;
    raylib::DrawTexturePro(texture,
                           {0, 0, static_cast<float>(texture.width),
                            static_cast<float>(texture.height)},
                           r, {0, 0}, 0, raylib::WHITE);
  }

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  static void draw_scene(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{153, 218, 221, 255});
    afterhours::draw_rectangle(
        {r.x, r.y + r.height * .20f, r.width, r.height * .80f},
        afterhours::Color{208, 126, 100, 255});
    afterhours::draw_triangle({r.x, r.y + r.height * .21f},
                              {r.x + r.width * .53f, r.y + r.height * .02f},
                              {r.x + r.width * .34f, r.y + r.height * .48f},
                              afterhours::Color{122, 173, 76, 255});
    afterhours::draw_triangle({r.x + r.width * .34f, r.y + r.height * .29f},
                              {r.x + r.width * .62f, r.y + r.height * .08f},
                              {r.x + r.width, r.y + r.height * .42f},
                              afterhours::Color{172, 174, 151, 255});
    afterhours::draw_rectangle(
        {r.x + r.width * .78f, r.y, r.width * .22f, r.height * .33f},
        afterhours::Color{164, 96, 73, 255});
    for (int x = 0; x < 1280; x += 62) {
      const float sx = r.x + static_cast<float>(x) * r.width / 1280.f;
      afterhours::draw_line_ex({sx, r.y + r.height * .2f},
                               {sx + 160.f, r.y + r.height}, 1.f,
                               afterhours::Color{149, 85, 76, 125});
    }
    for (int y = 180; y < 720; y += 36) {
      const float sy = r.y + static_cast<float>(y) * r.height / 720.f;
      afterhours::draw_line_ex({r.x, sy}, {r.x + r.width, sy + 38.f}, 1.5f,
                               afterhours::Color{151, 84, 75, 105});
    }
    afterhours::draw_rectangle(
        {r.x + r.width * .82f, r.y + 6.f, r.width * .14f, r.height * .20f},
        afterhours::Color{47, 73, 75, 230});
    afterhours::draw_rectangle({r.x + r.width * .57f, r.y + r.height * .24f,
                                r.width * .29f, r.height * .15f},
                               afterhours::Color{195, 196, 166, 155});
    for (int i = 0; i < 16; ++i) {
      float x = r.x + r.width * (.58f + i * .018f);
      afterhours::draw_line_ex({x, r.y + r.height * .24f},
                               {x, r.y + r.height * .39f}, 4.f,
                               afterhours::Color{220, 216, 176, 140});
    }
    // NOTE: town.png carries the street, barriers and a side-profile rider.
    // The reference rider faces the viewer, so the front-facing cyclist and
    // background pedestrians are drawn over the texture (see on_draw_bg).
    // Silver crowd barriers on the pavement (under the texture, kept for
    // completeness when no town art is present).
    for (int b = 0; b < 3; ++b) {
      const float bx = r.x + r.width * (.62f + static_cast<float>(b) * .13f);
      const float by = r.y + r.height * .30f;
      const float bw = r.width * .115f;
      const float bh = r.height * .13f;
      afterhours::draw_rectangle({bx, by, bw, bh},
                                 afterhours::Color{196, 198, 202, 200});
      for (int v = 0; v < 6; ++v) {
        afterhours::draw_line_ex({bx + v * bw / 5.f, by},
                                 {bx + v * bw / 5.f, by + bh}, 2.f,
                                 afterhours::Color{120, 122, 126, 200});
      }
      afterhours::draw_line_ex({bx, by + bh * .45f}, {bx + bw, by + bh * .45f},
                               2.f, afterhours::Color{120, 122, 126, 200});
    }
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{0, 0, 0, 0},
                                          afterhours::Color{28, 21, 18, 40});
  }

  // Front three-quarter view like the reference: the cyclist rides toward
  // the viewer, front wheel large and centered, torso upright, arms down to
  // a wide handlebar. Flat shapes only.
  static void draw_cyclist(RectangleType r) {
    const afterhours::Color tire{31, 37, 35, 255};
    const afterhours::Color rim{218, 214, 190, 255};
    const afterhours::Color frame{209, 57, 43, 255};
    const afterhours::Color teal{73, 190, 205, 255};
    const afterhours::Color dark{31, 52, 59, 255};
    const float cx = r.x + r.width * .50f;
    const float wheel_cy = r.y + r.height * .78f;
    const float wr = r.width * .30f;
    // Front wheel (large, facing the viewer).
    afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(wheel_cy),
                                  wr, tire);
    afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(wheel_cy),
                                  wr * .86f, rim);
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(wheel_cy),
                            wr * .10f, frame);
    for (int i = 0; i < 8; ++i) {
      const float a = static_cast<float>(i) * 3.14159f / 4.f;
      afterhours::draw_line_ex({cx, wheel_cy},
                               {cx + std::cos(a) * wr * .86f,
                                wheel_cy + std::sin(a) * wr * .86f},
                               1.5f, afterhours::Color{218, 214, 190, 170});
    }
    // Rear wheel hint behind, offset up-left.
    const float rx = cx - wr * 1.15f;
    const float ry = wheel_cy - wr * .55f;
    afterhours::draw_circle_lines(static_cast<int>(rx), static_cast<int>(ry),
                                  wr * .38f, tire);
    // Frame triangle from front wheel up to the seat.
    const float seat_x = cx - wr * .25f;
    const float seat_y = r.y + r.height * .42f;
    afterhours::draw_line_ex({cx, wheel_cy}, {seat_x, seat_y}, 8.f, frame);
    afterhours::draw_line_ex({seat_x, seat_y}, {rx, ry}, 8.f, frame);
    afterhours::draw_line_ex({cx, wheel_cy}, {rx, ry}, 6.f, frame);
    // Handlebar wide across, stem down to the wheel.
    const float bars_y = r.y + r.height * .47f;
    afterhours::draw_line_ex({cx - wr * .55f, bars_y}, {cx + wr * .55f, bars_y},
                             7.f, dark);
    afterhours::draw_line_ex({cx, bars_y}, {cx, wheel_cy}, 7.f, frame);
    // Legs straddling the frame.
    afterhours::draw_line_ex(
        {seat_x, seat_y},
        {cx - wr * .15f, r.y + r.height * .66f}, 10.f, dark);
    afterhours::draw_line_ex(
        {seat_x, seat_y + wr * .06f},
        {cx + wr * .22f, r.y + r.height * .63f}, 10.f, dark);
    // Torso upright, teal jersey.
    afterhours::draw_rectangle_rounded(
        {cx - wr * .55f, r.y + r.height * .24f, wr * 1.10f, r.height * .22f},
        .25f, 18, teal, RoundedCorners().all_round());
    // Arms from shoulders down to the bar ends.
    afterhours::draw_line_ex({cx - wr * .50f, r.y + r.height * .30f},
                             {cx - wr * .55f, bars_y}, 8.f, teal);
    afterhours::draw_line_ex({cx + wr * .50f, r.y + r.height * .30f},
                             {cx + wr * .55f, bars_y}, 8.f, teal);
    // Head + helmet + visor, facing the viewer.
    afterhours::draw_circle(static_cast<int>(cx),
                            static_cast<int>(r.y + r.height * .17f),
                            wr * .30f, afterhours::Color{190, 130, 78, 255});
    afterhours::draw_rectangle({cx - wr * .32f, r.y + r.height * .09f,
                                wr * .64f, r.height * .09f},
                               teal);
    afterhours::draw_rectangle({cx - wr * .26f, r.y + r.height * .15f,
                                wr * .52f, wr * .10f}, dark);
  }

  // The reference shows the phone in 3D perspective (top receding, left edge
  // leaning), not front-on. The box the UI lives in stays the asserted
  // 40,26,440,674; the drawn body is a skewed quad inside it with a darker
  // edge thickness on the leaning side. UI content stays upright on top.
  // Small background pedestrian like the two walkers behind the rider in
  // the reference. Flat shapes, drawn over the street art.
  static void draw_pedestrian(RectangleType r, afterhours::Color top) {
    const float cx = r.x + r.width * .5f;
    afterhours::draw_circle(static_cast<int>(cx),
                            static_cast<int>(r.y + r.height * .12f),
                            r.width * .28f, afterhours::Color{190, 130, 78, 255});
    afterhours::draw_rectangle_rounded(
        {cx - r.width * .32f, r.y + r.height * .22f, r.width * .64f,
         r.height * .44f},
        .3f, 12, top, RoundedCorners().all_round());
    afterhours::draw_rectangle({cx - r.width * .30f, r.y + r.height * .62f,
                                r.width * .22f, r.height * .30f},
                               afterhours::Color{31, 52, 59, 255});
    afterhours::draw_rectangle({cx + r.width * .08f, r.y + r.height * .62f,
                                r.width * .22f, r.height * .30f},
                               afterhours::Color{31, 52, 59, 255});
  }

  static void draw_phone(RectangleType r) {
    const float s = r.width / 440;
    const Vector2Type tl{r.x + 52 * s, r.y + 72 * s};
    const Vector2Type tr{r.x + 440 * s, r.y + 10 * s};
    const Vector2Type br{r.x + 430 * s, r.y + 612 * s};
    const Vector2Type bl{r.x + 10 * s, r.y + 674 * s};
    // Shadow under the leaning body.
    const Vector2Type sh_tl{tl.x + 10 * s, tl.y + 12 * s};
    const Vector2Type sh_tr{tr.x + 10 * s, tr.y + 12 * s};
    const Vector2Type sh_br{br.x + 10 * s, br.y + 12 * s};
    const Vector2Type sh_bl{bl.x + 10 * s, bl.y + 12 * s};
    afterhours::draw_quad({sh_tl, sh_tr, sh_br, sh_bl}, {10, 14, 14, 110});
    // Body + edge thickness (the quad behind, offset up-left, reads as the
    // phone's side near the leaning bottom edge).
    afterhours::draw_quad({tl, tr, br, bl}, {38, 45, 41, 255});
    const Vector2Type b_tl{tl.x + 14 * s, tl.y + 17 * s};
    const Vector2Type b_tr{tr.x - 10 * s, tr.y + 20 * s};
    const Vector2Type b_br{br.x - 16 * s, br.y - 18 * s};
    const Vector2Type b_bl{bl.x + 16 * s, bl.y - 20 * s};
    afterhours::draw_quad({b_tl, b_tr, b_br, b_bl}, {5, 7, 8, 255});
    // Screen face, inset within the skewed body.
    const Vector2Type s_tl{b_tl.x + 6 * s, b_tl.y + 4 * s};
    const Vector2Type s_tr{b_tr.x - 8 * s, b_tr.y + 4 * s};
    const Vector2Type s_br{b_br.x - 6 * s, b_br.y - 4 * s};
    const Vector2Type s_bl{b_bl.x + 6 * s, b_bl.y - 4 * s};
    afterhours::draw_quad({s_tl, s_tr, s_br, s_bl}, {34, 39, 34, 255});
    // Notch near the top of the leaning screen.
    afterhours::draw_rectangle_rounded(
        {s_tl.x + 130 * s, s_tl.y + 2 * s, 84 * s, 9 * s}, .5f, 8,
        {6, 8, 8, 255}, std::bitset<4>().set());
  }

  enum class Icon { Language, Subtitles, Display, Texture, Motion, Sync, Audio };
  static void draw_icon(RectangleType r, Icon icon) {
    const float s = r.width / 24;
    const afterhours::Color ink{16, 34, 23, 255};
    const auto line = [&](float x, float y, float xx, float yy) {
      afterhours::draw_line_ex({r.x + x * s, r.y + y * s}, {r.x + xx * s, r.y + yy * s}, 1.7f * s, ink);
    };
    if (icon == Icon::Language) {
      afterhours::draw_circle_lines(static_cast<int>(r.x + 12 * s), static_cast<int>(r.y + 12 * s), 8 * s, ink);
      line(4, 12, 20, 12); line(12, 4, 12, 20); line(7, 7, 17, 7); line(7, 17, 17, 17);
      return;
    }
    if (icon == Icon::Subtitles) {
      raylib::DrawRectangleLinesEx({r.x + 4 * s, r.y + 6 * s, 16 * s, 12 * s}, s, ink);
      line(6, 10, 10, 10); line(13, 10, 18, 10); line(6, 14, 15, 14);
      return;
    }
    if (icon == Icon::Display) {
      raylib::DrawRectangleLinesEx({r.x + 4 * s, r.y + 5 * s, 16 * s, 12 * s}, 1.5f * s, ink);
      line(12, 17, 12, 20); line(8, 20, 16, 20);
      return;
    }
    if (icon == Icon::Texture) {
      for (int row = 0; row < 2; ++row) for (int col = 0; col < 2; ++col)
        raylib::DrawRectangleLinesEx({r.x + (5 + static_cast<float>(col) * 8) * s, r.y + (5 + static_cast<float>(row) * 8) * s, 6 * s, 6 * s}, s, ink);
      return;
    }
    if (icon == Icon::Motion) {
      line(4, 6, 14, 6); line(4, 12, 18, 12); line(4, 18, 14, 18); line(16, 8, 20, 12); line(20, 12, 16, 16);
      return;
    }
    if (icon == Icon::Sync) {
      line(5, 8, 19, 8); line(19, 8, 15, 4); line(19, 8, 15, 12);
      line(19, 17, 5, 17); line(5, 17, 9, 13); line(5, 17, 9, 21);
      return;
    }
    afterhours::draw_rectangle({r.x + 4 * s, r.y + 9 * s, 5 * s, 7 * s}, ink);
    line(9, 9, 14, 5); line(14, 5, 14, 20); line(14, 20, 9, 16);
    line(17, 8, 20, 12); line(20, 12, 17, 17);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    Theme theme;
    theme.font = white;
    theme.darkfont = phone_black;
    theme.background = phone_black;
    theme.surface = row_dark;
    theme.primary = green;
    theme.secondary = blue;
    theme.accent = orange;
    theme.corner_radius = 0;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("ArchivoMockBold", pixels(20 * scale));
    load_texture_if_needed();
    if (context.pressed(InputAction::MenuBack)) settings_open = false;
    div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({39, 49, 46, 255}).with_corner_radius(0)
        .with_debug_name("parcel_canvas"));
    auto root = div(context, mk(entity), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_on_draw_bg([texture = town_texture](RectangleType r) {
          draw_scene(r);
          paint(texture, r);
          // Reference: rider faces the viewer with a long shadow, and two
          // pedestrians cross behind him. Drawn over the town art so the
          // front-facing pose reads.
          const float s = r.width / 1280;
          afterhours::draw_ellipse(static_cast<int>(r.x + 700 * s),
                                   static_cast<int>(r.y + 610 * s), 90 * s,
                                   18 * s, {60, 30, 25, 130});
          draw_pedestrian({r.x + 462 * s, r.y + 150 * s, 30 * s, 100 * s},
                          {222, 190, 60, 255});
          draw_pedestrian({r.x + 812 * s, r.y + 140 * s, 30 * s, 105 * s},
                          {86, 130, 80, 255});
          draw_cyclist({r.x + 480 * s, r.y + 150 * s, 300 * s, 470 * s});
          afterhours::draw_circle(static_cast<int>(r.x + 637 * s), static_cast<int>(r.y + 270 * s), 2.5f * s, {46, 50, 43, 255});
          afterhours::draw_line_ex({r.x + 623 * s, r.y + 283 * s}, {r.x + 634 * s, r.y + 283 * s}, 2 * s, {95, 65, 48, 255});
          afterhours::draw_ellipse(static_cast<int>(r.x + 724 * s), static_cast<int>(r.y + 351 * s), 8 * s, 5 * s, {191, 140, 92, 255});
        }).with_debug_name("parcel_root"));
    const auto hud_text = [&](int id, const std::string &label, float x, float y,
                              float w, float h, float size, afterhours::Color color) {
      div(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(label)
          .with_font("ArchivoMockBold", pixels(size * scale)).with_custom_text_color(color)
          .with_ignore_pointer_events().with_render_layer(1));
    };
    div(context, mk(root.ent()), box(scale, 520, 34, 400, 108)
        .with_custom_background(blue).with_corner_radius(12 * scale)
        .with_render_layer(1).with_debug_name("parcel_quest"));
    // Reference quest banner: two checked/unchecked lines in caps. Smaller
    // type so each line fits the 400px banner; checkboxes drawn at left.
    hud_text(1201, "SIGN UP A BUSINESS TO YOUR DELIVERY APP", 552, 40, 360, 26, 15, white);
    hud_text(1202, "SIGN UP 4 BUSINESSES TO YOUR DELIVERY APP", 552, 74, 360, 26, 15, white);
    div(context, mk(root.ent()), box(scale, 534, 44, 13, 13)
        .with_render_layer(1).with_on_draw_fg([scale](RectangleType r) {
          const auto c = afterhours::Color{230, 255, 238, 255};
          afterhours::draw_line_ex({r.x, r.y + 7 * scale}, {r.x + 4 * scale, r.y + 11 * scale}, 2 * scale, c);
          afterhours::draw_line_ex({r.x + 4 * scale, r.y + 11 * scale}, {r.x + 12 * scale, r.y}, 2 * scale, c);
          raylib::DrawRectangleLinesEx(r, 1.5f * scale, c);
        }));
    div(context, mk(root.ent()), box(scale, 534, 78, 13, 13)
        .with_render_layer(1).with_on_draw_fg([scale](RectangleType r) {
          raylib::DrawRectangleLinesEx(r, 1.5f * scale, {230, 255, 238, 255});
        }));
    div(context, mk(root.ent()), box(scale, 944, 34, 288, 212)
        .with_custom_background({32, 56, 57, 245}).with_corner_radius(12 * scale)
        .with_render_layer(1).with_debug_name("parcel_chat"));
    hud_text(1301, "LIVE · EDDCOATES", 960, 43, 258, 28, 20, white);
    hud_text(1302, "8 viewers · 10 messages", 960, 72, 258, 25, 17, muted);
    for (size_t i = 0; i < chat_messages.size(); ++i) {
      const float y = 105 + static_cast<float>(i) * 27;
      hud_text(1310 + static_cast<int>(i) * 2, chat_messages[i].first,
               960, y, 150, 24, 15, {205, 223, 217, 255});
      hud_text(1311 + static_cast<int>(i) * 2, chat_messages[i].second,
               1114, y, 108, 24, 16, white);
    }
    div(context, mk(root.ent(), 1400), box(scale, 1104, 592, 128, 96)
        .with_custom_background({17, 26, 27, 245}).with_corner_radius(12 * scale)
        .with_render_layer(1).with_debug_name("parcel_speed"));
    hud_text(1401, "9", 1124, 603, 34, 49, 38, {227, 194, 174, 255});
    hud_text(1402, "mph", 1160, 625, 42, 27, 19, {227, 194, 174, 255});
    hud_text(1403, "GAMON", 1124, 656, 100, 24, 16, {227, 194, 174, 255});
    if (!settings_open) {
      if (button(context, mk(root.ent()), box(scale, 48, 48, 224, 48)
          .with_label("Open settings").with_font("ArchivoMockBold", pixels(23 * scale))
          .with_custom_background(phone_black).with_custom_text_color(white)
          .with_render_layer(10).with_debug_name("parcel_reopen"))) settings_open = true;
      return;
    }
    div(context, mk(root.ent()), box(scale, 0, 0, 1280, 720)
        .with_custom_background({10, 20, 24, 92}).with_render_layer(5)
        .with_ignore_pointer_events().with_debug_name("parcel_scene_dim"));
    auto phone = div(context, mk(root.ent()), box(scale, 0, 0, 1280, 720)
        .with_render_layer(10).with_debug_name("parcel_phone_layer"));
    const auto text = [&](int id, const std::string &label, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &debug = "", TextAlignment alignment = TextAlignment::Left) {
      return div(context, mk(phone.ent(), id), box(scale, x, y, w, h).with_label(label)
          .with_font("ArchivoMockBold", pixels(size * scale)).with_custom_text_color(color)
          .with_letter_spacing(0).with_ignore_pointer_events().with_alignment(alignment)
          .with_debug_name(debug));
    };
    div(context, mk(phone.ent()), box(scale, 40, 26, 440, 674)
        .with_on_draw_bg([](RectangleType r) { draw_phone(r); }).with_debug_name("parcel_phone"));
    text(11, "23:45", 68, 66, 84, 25, 19, white);
    div(context, mk(phone.ent()), box(scale, 356, 67, 84, 18)
        .with_debug_name("parcel_signal_battery").with_on_draw_fg([scale, this](RectangleType r) {
          for (int i = 0; i < 4; ++i) {
            const float h = 5 + static_cast<float>(i) * 4;
            afterhours::draw_rectangle({r.x + static_cast<float>(i) * 7 * scale, r.y + (18 - h) * scale, 4 * scale, h * scale}, white);
          }
          raylib::DrawRectangleLinesEx({r.x + 44 * scale, r.y, 32 * scale, 17 * scale}, 2 * scale, white);
          afterhours::draw_rectangle({r.x + 47 * scale, r.y + 3 * scale, 23 * scale, 11 * scale}, green);
          afterhours::draw_rectangle({r.x + 77 * scale, r.y + 5 * scale, 4 * scale, 7 * scale}, white);
        }));
    const auto chevron = [&](int id, float x, float y, bool right, const std::string &debug) {
      return button(context, mk(phone.ent(), id), box(scale, x, y, 32, 32)
          .with_custom_background({53, 63, 58, 255}).with_corner_radius(4 * scale)
          .with_on_draw_fg([scale, right, this](RectangleType r) {
            const float edge = r.x + (right ? 10 : 22) * scale;
            const float tip = r.x + (right ? 21 : 11) * scale;
            afterhours::draw_line_ex({edge, r.y + 8 * scale}, {tip, r.y + 16 * scale}, 2 * scale, white);
            afterhours::draw_line_ex({tip, r.y + 16 * scale}, {edge, r.y + 24 * scale}, 2 * scale, white);
          }).with_debug_name(debug));
    };
    if (chevron(13, 68, 109, false, "parcel_back")) settings_open = false;
    text(14, "Settings", 114, 101, 320, 46, 36, white, "parcel_title");
    text(16, "DISPLAY", 68, 258, 384, 23, 16, {244, 198, 127, 255});
    text(17, "AUDIO", 68, 536, 384, 23, 16, {237, 155, 147, 255});
    const auto icon = [&](int id, float y, Icon symbol, afterhours::Color color) {
      div(context, mk(phone.ent(), id), box(scale, 76, y + 7, 24, 24)
          .with_custom_background(color).with_corner_radius(4 * scale)
          .with_ignore_pointer_events().with_on_draw_fg([symbol](RectangleType r) { draw_icon(r, symbol); }));
    };
    const auto row = [&](int id, float y, float h, const std::string &label,
                         Icon symbol, afterhours::Color color, const std::string &debug) {
      div(context, mk(phone.ent(), id), box(scale, 68, y, 384, h)
          .with_custom_background(row_dark).with_border(separator, scale).with_debug_name(debug));
      icon(id + 1, y, symbol, color);
      text(id + 2, label, 108, y + 5, 174, 28, 20, white);
    };
    const auto toggle = [&](int id, float y, bool &value, const std::string &debug) {
      if (button(context, mk(phone.ent(), id), box(scale, 392, y + 6, 48, 26)
          .with_custom_background(value ? green : afterhours::Color{82, 94, 82, 255})
          .with_corner_radius(12 * scale).with_debug_name(debug))) value = !value;
      div(context, mk(phone.ent(), id + 1), box(scale, 395 + (value ? 23 : 0), y + 9, 20, 20)
          .with_custom_background(white).with_corner_radius(10 * scale).with_ignore_pointer_events());
      text(id + 2, value ? "On" : "Off", 345, y + 6, 38, 26, 18, white, debug + "_state", TextAlignment::Right);
    };
    auto language = button(context, mk(phone.ent()), box(scale, 68, 174, 384, 38)
        .with_custom_background(row_dark).with_border(separator, scale).with_debug_name("parcel_language_row"));
    if (language) language_idx = (language_idx + 1) % static_cast<int>(languages.size());
    icon(1001, 174, Icon::Language, green);
    text(1002, "Text Language", 108, 179, 162, 28, 20, white);
    text(1003, languages[static_cast<size_t>(language_idx)], 276, 179, 164, 28, 20, white, "parcel_language_value", TextAlignment::Right);
    row(1010, 216, 38, "Subtitles", Icon::Subtitles, green, "parcel_subtitles_row");
    toggle(1014, 216, subtitles, "parcel_subtitles_toggle");
    row(1020, 284, 38, "Resolution", Icon::Display, orange, "parcel_resolution_row");
    text(1023, "2560 × 1440", 276, 289, 164, 28, 20, white, "parcel_resolution_value", TextAlignment::Right);
    row(1030, 324, 38, "Fullscreen", Icon::Display, orange, "parcel_fullscreen_row");
    toggle(1034, 324, fullscreen, "parcel_fullscreen_toggle");
    const auto selector = [&](int id, float y, float h, const std::string &label,
                              size_t &index, const std::array<std::string, 4> &values,
                              const std::string &debug) {
      row(id, y, h, label, Icon::Texture, orange, debug + "_row");
      if (chevron(id + 4, 284, y + 4, false, debug + "_prev")) index = (index + values.size() - 1) % values.size();
      if (chevron(id + 5, 408, y + 4, true, debug + "_next")) index = (index + 1) % values.size();
      text(id + 3, values[index], 318, y + 4, 88, 32, 21, white, debug + "_value", TextAlignment::Center);
    };
    selector(1040, 364, 46, "Anti-aliasing", msaa_idx, msaa_options, "parcel_msaa");
    text(1046, "MSAA", 108, 387, 164, 17, 14, muted);
    selector(1050, 412, 38, "Texture Quality", texture_idx, texture_options, "parcel_texture");
    row(1060, 452, 38, "Motion Blur", Icon::Motion, orange, "parcel_motion_row");
    toggle(1064, 452, motion_blur, "parcel_motion_toggle");
    row(1070, 492, 38, "V-sync", Icon::Sync, orange, "parcel_vsync_row");
    toggle(1074, 492, vsync, "parcel_vsync_toggle");
    const auto volume = [&](int id, float y, const std::string &label, float &value, const std::string &debug) {
      row(id, y, 36, label, Icon::Audio, red, debug + "_row");
      const float drawn_value = value;
      auto input = slider(context, mk(phone.ent(), id + 4), value,
          box(scale, 280, y + 5, 100, 30).with_custom_background({0, 0, 0, 0})
              .with_debug_name(debug + "_slider").with_on_draw_fg([drawn_value, scale, this](RectangleType r) {
                const float width = std::max(0.f, r.width - 6.f);
                const float cy = r.y + r.height / 2;
                const float cx = r.x + width * drawn_value;
                afterhours::draw_rectangle({r.x, cy - 3 * scale, width, 6 * scale}, {105, 120, 105, 255});
                afterhours::draw_rectangle({r.x, cy - 3 * scale, width * drawn_value, 6 * scale}, green);
                afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 10 * scale, {6, 20, 9, 255});
                afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 8 * scale, white);
              }));
      for (const auto child_id : input.cmp().children) {
        auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
        if (!child.has<HasSliderState>() || !child.has<HasLeftRightListener>()) continue;
        child.get<HasLeftRightListener>().cb = [ctx = &context](afterhours::Entity &target, int direction) {
          const auto action = direction < 0 ? InputAction::WidgetLeft : InputAction::WidgetRight;
          if (!ctx->pressed_or_repeat(action)) return;
          auto &state = target.get<HasSliderState>();
          const float next = std::clamp(state.value + (direction < 0 ? -.01f : .01f), 0.f, 1.f);
          if (next == state.value) return;
          state.value = next;
          state.changed_since = true;
        };
      }
      text(id + 5, fmt::format("{:.0f}%", value * 100), 389, y + 5, 51, 30, 18, white, debug + "_value", TextAlignment::Right);
      if (input) status = fmt::format("{}: {:.0f}%", label, value * 100);
    };
    volume(1080, 564, "Audio Volume", audio_volume, "parcel_audio");
    volume(1090, 608, "Music Volume", music_volume, "parcel_music");
    text(1500, status, 68, 644, 384, 25, 17, muted, "parcel_status");
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Parcel delivery game phone settings", ParcelCorpsSettingsScreen)
