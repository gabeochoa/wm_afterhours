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
  std::string status = "Changes apply locally";
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
    draw_cyclist({r.x + r.width * .40f, r.y + r.height * .24f, r.width * .25f,
                  r.height * .58f});
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{0, 0, 0, 0},
                                          afterhours::Color{28, 21, 18, 40});
  }

  static void draw_cyclist(RectangleType r) {
    const afterhours::Color tire{31, 37, 35, 255};
    const afterhours::Color frame{209, 57, 43, 255};
    const afterhours::Color teal{73, 190, 205, 255};
    const float wx1 = r.x + r.width * .22f;
    const float wx2 = r.x + r.width * .74f;
    const float wy = r.y + r.height * .70f;
    const float rr = r.width * .15f;
    afterhours::draw_circle_lines(static_cast<int>(wx1), static_cast<int>(wy),
                                  rr, tire);
    afterhours::draw_circle_lines(static_cast<int>(wx2), static_cast<int>(wy),
                                  rr, tire);
    for (int i = 0; i < 12; ++i) {
      const float a = static_cast<float>(i) * 3.14159f / 6.f;
      afterhours::draw_line_ex({wx1, wy},
                               {wx1 + std::cos(a) * rr, wy + std::sin(a) * rr},
                               1.f, afterhours::Color{218, 214, 190, 150});
      afterhours::draw_line_ex({wx2, wy},
                               {wx2 + std::cos(a) * rr, wy + std::sin(a) * rr},
                               1.f, afterhours::Color{218, 214, 190, 150});
    }
    afterhours::draw_line_ex(
        {wx1, wy}, {r.x + r.width * .45f, r.y + r.height * .55f}, 7.f, frame);
    afterhours::draw_line_ex({r.x + r.width * .45f, r.y + r.height * .55f},
                             {wx2, wy}, 7.f, frame);
    afterhours::draw_line_ex({r.x + r.width * .45f, r.y + r.height * .55f},
                             {r.x + r.width * .36f, r.y + r.height * .43f}, 7.f,
                             frame);
    afterhours::draw_rectangle({r.x + r.width * .37f, r.y + r.height * .28f,
                                r.width * .20f, r.height * .22f},
                               teal);
    afterhours::draw_rectangle({r.x + r.width * .46f, r.y + r.height * .50f,
                                r.width * .16f, r.height * .16f},
                               afterhours::Color{31, 52, 59, 255});
    afterhours::draw_circle(static_cast<int>(r.x + r.width * .47f),
                            static_cast<int>(r.y + r.height * .20f),
                            r.width * .075f,
                            afterhours::Color{190, 130, 78, 255});
    afterhours::draw_rectangle({r.x + r.width * .39f, r.y + r.height * .12f,
                                r.width * .18f, r.height * .08f},
                               teal);
  }

  static void draw_phone(RectangleType r) {
    const float s = r.width / 440;
    afterhours::draw_rectangle_rounded(r, .12f, 18, {5, 7, 8, 255}, std::bitset<4>().set());
    afterhours::draw_rectangle_rounded_lines_ex(r, .12f, 18, 6 * s, {38, 45, 41, 255});
    afterhours::draw_rectangle_rounded({r.x + 17 * s, r.y + 21 * s, r.width - 34 * s, r.height - 42 * s},
        .06f, 12, {34, 39, 34, 255}, std::bitset<4>().set());
    afterhours::draw_rectangle_rounded({r.x + r.width / 2 - 42 * s, r.y + 17 * s, 84 * s, 8 * s},
        .5f, 8, {6, 8, 8, 255}, std::bitset<4>().set());
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
    div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({39, 49, 46, 255}).with_corner_radius(0)
        .with_debug_name("parcel_canvas"));
    auto root = div(context, mk(entity, 0), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_on_draw_bg([texture = town_texture](RectangleType r) {
          draw_scene(r);
          paint(texture, r);
          const float s = r.width / 1280;
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
    div(context, mk(root.ent(), 1200), box(scale, 520, 34, 400, 108)
        .with_custom_background(blue).with_corner_radius(12 * scale)
        .with_render_layer(1).with_debug_name("parcel_quest"));
    hud_text(1201, "Grow your delivery network", 536, 45, 318, 31, 24, white);
    hud_text(1202, "Sign up 4 businesses: 1 / 4", 536, 81, 356, 25, 20, white);
    hud_text(1203, "First business signed up", 558, 109, 334, 25, 18, white);
    div(context, mk(root.ent(), 1204), box(scale, 864, 46, 42, 28)
        .with_label("Lv 0").with_font("ArchivoMockBold", pixels(18 * scale))
        .with_custom_background({24, 54, 67, 255}).with_custom_text_color(white)
        .with_alignment(TextAlignment::Center).with_render_layer(1).with_debug_name("parcel_level"));
    div(context, mk(root.ent(), 1205), box(scale, 538, 114, 13, 14)
        .with_render_layer(1).with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_line_ex({r.x, r.y + 7 * scale}, {r.x + 4 * scale, r.y + 12 * scale}, 2 * scale, {230, 255, 238, 255});
          afterhours::draw_line_ex({r.x + 4 * scale, r.y + 12 * scale}, {r.x + 12 * scale, r.y}, 2 * scale, {230, 255, 238, 255});
        }));
    div(context, mk(root.ent(), 1300), box(scale, 944, 34, 288, 212)
        .with_custom_background({32, 56, 57, 245}).with_render_layer(1).with_debug_name("parcel_chat"));
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
    hud_text(1401, "9", 1124, 603, 56, 49, 38, {227, 194, 174, 255});
    hud_text(1402, "mph", 1182, 625, 42, 27, 19, {227, 194, 174, 255});
    hud_text(1403, "GAMON", 1124, 656, 100, 24, 16, {227, 194, 174, 255});
    if (!settings_open) {
      if (button(context, mk(root.ent(), 1501), box(scale, 48, 48, 224, 48)
          .with_label("Open settings").with_font("ArchivoMockBold", pixels(23 * scale))
          .with_custom_background(phone_black).with_custom_text_color(white)
          .with_render_layer(10).with_debug_name("parcel_reopen"))) settings_open = true;
      return;
    }
    div(context, mk(root.ent(), 5), box(scale, 0, 0, 1280, 720)
        .with_custom_background({10, 20, 24, 92}).with_render_layer(5)
        .with_ignore_pointer_events().with_debug_name("parcel_scene_dim"));
    auto phone = div(context, mk(root.ent(), 6), box(scale, 0, 0, 1280, 720)
        .with_render_layer(10).with_debug_name("parcel_phone_layer"));
    const auto text = [&](int id, const std::string &label, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &debug = "", TextAlignment alignment = TextAlignment::Left) {
      return div(context, mk(phone.ent(), id), box(scale, x, y, w, h).with_label(label)
          .with_font("ArchivoMockBold", pixels(size * scale)).with_custom_text_color(color)
          .with_letter_spacing(0).with_ignore_pointer_events().with_alignment(alignment)
          .with_debug_name(debug));
    };
    div(context, mk(phone.ent(), 10), box(scale, 40, 26, 440, 674)
        .with_on_draw_bg([](RectangleType r) { draw_phone(r); }).with_debug_name("parcel_phone"));
    text(11, "23:45", 68, 66, 84, 25, 19, white);
    div(context, mk(phone.ent(), 12), box(scale, 356, 67, 84, 18)
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
    text(15, "LANGUAGE & TEXT", 68, 148, 384, 23, 16, {141, 221, 150, 255});
    text(16, "DISPLAY", 68, 258, 384, 23, 16, {244, 198, 127, 255});
    text(17, "AUDIO", 68, 536, 384, 23, 16, {237, 155, 147, 255});
    const auto icon = [&](int id, float y, Icon symbol, afterhours::Color color) {
      div(context, mk(phone.ent(), id), box(scale, 76, y + 7, 24, 24)
          .with_custom_background(color).with_corner_radius(5 * scale)
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
          .with_corner_radius(13 * scale).with_debug_name(debug))) value = !value;
      div(context, mk(phone.ent(), id + 1), box(scale, 395 + (value ? 23 : 0), y + 9, 20, 20)
          .with_custom_background(white).with_corner_radius(10 * scale).with_ignore_pointer_events());
      text(id + 2, value ? "On" : "Off", 345, y + 6, 38, 26, 18, white, debug + "_state", TextAlignment::Right);
    };
    auto language = button(context, mk(phone.ent(), 1000), box(scale, 68, 174, 384, 38)
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
    text(1046, "MSAA", 108, 391, 164, 17, 14, muted);
    selector(1050, 412, 38, "Texture Quality", texture_idx, texture_options, "parcel_texture");
    row(1060, 452, 38, "Motion Blur", Icon::Motion, orange, "parcel_motion_row");
    toggle(1064, 452, motion_blur, "parcel_motion_toggle");
    row(1070, 492, 38, "V-sync", Icon::Sync, orange, "parcel_vsync_row");
    toggle(1074, 492, vsync, "parcel_vsync_toggle");
    const auto volume = [&](int id, float y, const std::string &label, float &value, const std::string &debug) {
      row(id, y, 40, label, Icon::Audio, red, debug + "_row");
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
    text(1500, status, 68, 652, 384, 25, 17, muted, "parcel_status");
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Parcel delivery game phone settings", ParcelCorpsSettingsScreen)
