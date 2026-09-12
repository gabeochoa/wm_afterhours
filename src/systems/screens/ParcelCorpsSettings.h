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
  std::string status = "Settings open";
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
        .with_background(Theme::Usage::None);
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
    afterhours::draw_rectangle_rounded(
        r, .12f, 18, afterhours::Color{5, 7, 8, 255}, std::bitset<4>().set());
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .12f, 18, 7.f, afterhours::Color{38, 45, 41, 255});
    afterhours::draw_rectangle_rounded(
        {r.x + 17.f, r.y + 21.f, r.width - 34.f, r.height - 43.f}, .06f, 12,
        afterhours::Color{34, 39, 34, 255}, std::bitset<4>().set());
    afterhours::draw_rectangle_rounded({r.x + 128.f, r.y + 17.f, 83.f, 8.f},
                                       .5f, 8, afterhours::Color{6, 8, 8, 255},
                                       std::bitset<4>().set());
  }

  void cycle(size_t &idx, size_t count) { idx = (idx + 1) % count; }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float screen_w =
        context.screen_width > 0.f
            ? context.screen_width
            : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h =
        context.screen_height > 0.f
            ? context.screen_height
            : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);

    Theme theme;
    theme.font = white;
    theme.darkfont = phone_black;
    theme.font_muted = muted;
    theme.background = phone_black;
    theme.surface = row_dark;
    theme.primary = green;
    theme.secondary = blue;
    theme.accent = orange;
    theme.roundness = .08f;
    theme.segments = 8;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("ArchivoMockBold", h720(22.f));

    load_texture_if_needed();

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_on_draw_bg([texture = town_texture](RectangleType r) {
                  draw_scene(r);
                  paint(texture, r);
                })
                .with_debug_name("parcel_root"));

    div(context, mk(root.ent(), 10),
        box(scale, 48, 49, 360, 626)
            .with_on_draw_bg([](RectangleType r) { draw_phone(r); })
            .with_debug_name("parcel_phone"));
    div(context, mk(root.ent(), 11),
        box(scale, 77, 90, 70, 22)
            .with_label("23:45")
            .with_font("ArchivoMockBold", h720(20.f))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 12),
        box(scale, 272, 80, 58, 20).with_custom_background(white));
    div(context, mk(root.ent(), 13),
        box(scale, 337, 77, 22, 11)
            .with_custom_background(green)
            .with_border(white, 2.f));
    div(context, mk(root.ent(), 14),
        box(scale, 79, 151, 145, 43)
            .with_label("Settings")
            .with_font("ArchivoMockBold", h720(42.f))
            .with_custom_text_color(white)
            .with_debug_name("parcel_title"));

    auto icon = [&](int id, float x, float y, afterhours::Color c,
                    const char *label) {
      div(context, mk(root.ent(), id),
          box(scale, x, y, 22, 22)
              .with_label(label)
              .with_custom_background(c)
              .with_corner_radius(5.f * scale)
              .with_font("ArchivoMockBold", h720(18.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events());
    };
    auto phone_row = [&](int id, float y, const std::string &label,
                         afterhours::Color ic, const char *ic_label,
                         const std::string &value, const std::string &debug) {
      div(context, mk(root.ent(), id),
          box(scale, 77, y, 312, 38)
              .with_custom_background(row_dark)
              .with_corner_radius(0)
              .with_border(separator, 1.f)
              .with_debug_name(debug));
      icon(id + 1, 80, y + 8, ic, ic_label);
      div(context, mk(root.ent(), id + 2),
          box(scale, 107, y + 8, 145, 24)
              .with_label(label)
              .with_font("ArchivoMockBold", h720(22.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      if (!value.empty()) {
        div(context, mk(root.ent(), id + 3),
            box(scale, 247, y + 8, 135, 24)
                .with_label(value)
                .with_font("ArchivoMockBold", h720(19.f))
                .with_custom_text_color(white)
                .with_alignment(TextAlignment::Right)
                .with_ignore_pointer_events());
      }
    };
    auto toggle = [&](int id, float x, float y, bool on,
                      const std::string &name) {
      if (button(context, mk(root.ent(), id),
                 box(scale, x, y, 51, 25)
                     .with_custom_background(
                         on ? green : afterhours::Color{83, 88, 82, 255})
                     .with_corner_radius(13.f * scale)
                     .with_debug_name(name))) {
        if (name == "parcel_subtitles_toggle")
          subtitles = !subtitles;
        if (name == "parcel_fullscreen_toggle")
          fullscreen = !fullscreen;
        if (name == "parcel_motion_toggle")
          motion_blur = !motion_blur;
        if (name == "parcel_vsync_toggle")
          vsync = !vsync;
      }
      div(context, mk(root.ent(), id + 50),
          box(scale, x + (on ? 30.f : 4.f), y + 3, 19, 19)
              .with_custom_background(white)
              .with_corner_radius(10.f * scale)
              .with_ignore_pointer_events());
    };
    auto selector = [&](int id, float y, const std::string &label, size_t &idx,
                        const std::array<std::string, 4> &values,
                        afterhours::Color ic, const std::string &debug) {
      phone_row(id, y, label, ic, "*", "", debug + "_row");
      div(context, mk(root.ent(), id + 3),
          box(scale, 297, y + 7, 56, 26)
              .with_label(values[idx])
              .with_font("ArchivoMockBold", h720(21.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events()
              .with_debug_name(debug + "_value"));
      if (button(context, mk(root.ent(), id + 4),
                 box(scale, 263, y + 2, 34, 34)
                     .with_label("<")
                     .with_custom_background(afterhours::Color{53, 59, 67, 255})
                     .with_font("ArchivoMockBold", h720(18.f))
                     .with_custom_text_color(muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name(debug + "_prev"))) {
        idx = (idx + values.size() - 1) % values.size();
      }
      if (button(context, mk(root.ent(), id + 5),
                 box(scale, 354, y + 2, 34, 34)
                     .with_label(">")
                     .with_custom_background(afterhours::Color{53, 59, 67, 255})
                     .with_font("ArchivoMockBold", h720(18.f))
                     .with_custom_text_color(muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name(debug + "_next"))) {
        idx = (idx + 1) % values.size();
      }
    };
    auto volume = [&](int id, float y, const std::string &label, float &value,
                      const std::string &debug) {
      phone_row(id, y, label, red, "*", "", debug + "_row");
      auto input = slider(context, mk(root.ent(), id + 4), value,
                 box(scale, 281, y + 8, 105, 26)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_debug_name(debug + "_slider"),
                 SliderHandleValueLabelPosition::None);
      for (auto child_id : input.cmp().children) {
        auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
        if (!child.has<HasSliderState>() || !child.has<HasLeftRightListener>())
          continue;
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
      if (input) {
        status = fmt::format("{}: {}%", label,
                             static_cast<int>(std::round(value * 100.f)));
      }
      div(context, mk(root.ent(), id + 5),
          box(scale, 281, y + 8, 105, 26)
              .with_ignore_pointer_events()
              .with_debug_name(debug + "_track")
              .with_on_draw_fg([value, scale, this](RectangleType r) {
                const float cy = r.y + r.height * .5f;
                const float radius = 9.f * scale;
                const float knob_x = r.x + radius + value * (r.width - 2.f * radius);
                afterhours::draw_rectangle_rounded(
                    {r.x, cy - 6.f * scale, r.width, 12.f * scale},
                    1.f, 16, white, RoundedCorners().all_round());
                afterhours::draw_rectangle_rounded(
                    {r.x + scale, cy - 5.f * scale, r.width - 2.f * scale, 10.f * scale},
                    1.f, 16, afterhours::Color{68, 74, 68, 255}, RoundedCorners().all_round());
                afterhours::draw_rectangle_rounded(
                    {r.x + scale, cy - 5.f * scale, knob_x - r.x, 10.f * scale},
                    1.f, 16, green, RoundedCorners().all_round());
                afterhours::draw_circle(static_cast<int>(knob_x), static_cast<int>(cy), radius, white);
              }));
    };

    if (button(context, mk(root.ent(), 1000),
               box(scale, 77, 226, 312, 38)
                   .with_label("")
                   .with_custom_background(row_dark)
                   .with_corner_radius(0)
                   .with_border(separator, 1.f)
                   .with_debug_name("parcel_language_row"))) {
      language_idx = (language_idx + 1) % static_cast<int>(languages.size());
    }
    icon(1001, 80, 234, green, "T");
    div(context, mk(root.ent(), 1002),
        box(scale, 107, 234, 130, 24)
            .with_label("Text Language")
            .with_font("ArchivoMockBold", h720(22.f))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 1003),
        box(scale, 248, 234, 130, 24)
            .with_label(languages[static_cast<size_t>(language_idx)])
            .with_font("ArchivoMockBold", h720(19.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("parcel_language_value"));
    phone_row(1010, 268, "Subtitles", green, "T", "", "parcel_subtitles_row");
    toggle(1014, 331, 276, subtitles, "parcel_subtitles_toggle");

    phone_row(1020, 315, "Resolution", orange, "D", "2560 x 1440",
              "parcel_resolution_row");
    phone_row(1030, 353, "Full Screen", orange, "D", "",
              "parcel_fullscreen_row");
    toggle(1034, 331, 361, fullscreen, "parcel_fullscreen_toggle");
    selector(1040, 391, "MSAA", msaa_idx, msaa_options, orange, "parcel_msaa");
    selector(1050, 429, "Texture Quality", texture_idx, texture_options, orange,
             "parcel_texture");
    phone_row(1060, 467, "Motion Blur", red, "X", "", "parcel_motion_row");
    toggle(1064, 331, 475, motion_blur, "parcel_motion_toggle");
    phone_row(1070, 505, "VSync", blue, "S", "", "parcel_vsync_row");
    toggle(1074, 331, 513, vsync, "parcel_vsync_toggle");
    volume(1080, 579, "Audio Volume", audio_volume, "parcel_audio");
    volume(1090, 621, "Music Volume", music_volume, "parcel_music");

    div(context, mk(root.ent(), 1200),
        box(scale, 479, 37, 326, 94)
            .with_custom_background(blue)
            .with_corner_radius(15.f * scale)
            .with_debug_name("parcel_quest"));
    div(context, mk(root.ent(), 1201),
        box(scale, 492, 47, 305, 72)
            .with_label("[x] SIGN UP A BUSINESS TO YOUR\n    DELIVERY APP\n[ ] "
                        "SIGN UP 4 BUSINESSES TO YOUR\n    DELIVERY APP")
            .with_font("ArchivoMockBold", h720(17.f))
            .with_custom_text_color(white)
            .with_text_overflow(TextOverflow::Wrap));
    div(context, mk(root.ent(), 1202),
        box(scale, 511, 104, 24, 21)
            .with_label("Lv0")
            .with_custom_background(afterhours::Color{12, 19, 23, 255})
            .with_font("ArchivoMockBold", h720(19.f))
            .with_custom_text_color(afterhours::Color{225, 247, 19, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(root.ent(), 1300),
        box(scale, 1027, 6, 203, 145)
            .with_custom_background(afterhours::Color{45, 73, 75, 220})
            .with_debug_name("parcel_chat"));
    div(context, mk(root.ent(), 1301),
        box(scale, 1005, 17, 56, 29)
            .with_label("LIVE")
            .with_custom_background(red)
            .with_corner_radius(8.f * scale)
            .with_font("ArchivoMockBold", h720(19.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 1302),
        box(scale, 1048, 25, 196, 22)
            .with_label("O 8   <> 10    EDDCOATES")
            .with_font("ArchivoMockBold", h720(16.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));
    for (size_t i = 0; i < chat_messages.size(); ++i) {
      const float y = 65.f + static_cast<float>(i) * 18.f;
      div(context, mk(root.ent(), 1310 + static_cast<int>(i) * 2),
          box(scale, 1058, y, 100, 21)
              .with_label(chat_messages[i].first)
              .with_font("ArchivoMockBold", h720(15.f))
              .with_custom_text_color(
                  i == 2 ? afterhours::Color{58, 255, 63, 255}
                         : afterhours::Color{244, 48, 233, 255}));
      div(context, mk(root.ent(), 1311 + static_cast<int>(i) * 2),
          box(scale, 1160, y, 76, 21)
              .with_label(chat_messages[i].second)
              .with_font("ArchivoMockBold", h720(15.f))
              .with_custom_text_color(white));
    }

    div(context, mk(root.ent(), 1400),
        box(scale, 1125, 595, 105, 86)
            .with_custom_background(afterhours::Color{13, 9, 8, 245})
            .with_corner_radius(17.f * scale)
            .with_debug_name("parcel_speed"));
    div(context, mk(root.ent(), 1401),
        box(scale, 1139, 604, 73, 45)
            .with_label("009")
            .with_font("ArchivoMockBold", h720(38.f))
            .with_custom_text_color(afterhours::Color{224, 165, 137, 255})
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 1402),
        box(scale, 1182, 649, 42, 26)
            .with_label("M/h")
            .with_font("ArchivoMockBold", h720(20.f))
            .with_custom_text_color(afterhours::Color{224, 165, 137, 255})
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 1403),
        box(scale, 1170, 668, 60, 20)
            .with_label("GAMON")
            .with_font("ArchivoMockBold", h720(14.f))
            .with_custom_text_color(afterhours::Color{224, 165, 137, 255})
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 1500),
        box(scale, 90, 549, 298, 23)
            .with_label(status)
            .with_font("ArchivoMockBold", h720(16.f))
            .with_custom_text_color(muted)
            .with_debug_name("parcel_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Parcel delivery game phone settings",
                        ParcelCorpsSettingsScreen)
