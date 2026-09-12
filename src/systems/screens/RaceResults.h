#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RaceResultsScreen : ScreenSystem<UIContext<InputAction>> {
  struct RaceEntry {
    int position;
    std::string name;
    std::string time;
    int points;
    bool is_player;
    afterhours::Color color;
  };

  struct CupStanding {
    std::string name;
    int total_points;
    bool is_player;
  };

  std::vector<RaceEntry> results = {
      {1, "Zippy", "2:31.450", 15, false, {236, 83, 69, 255}},
      {2, "YOU", "2:33.812", 12, true, {51, 190, 91, 255}},
      {3, "Blaze", "2:35.100", 10, false, {245, 143, 48, 255}},
      {4, "Drift", "2:37.445", 8, false, {112, 90, 188, 255}},
      {5, "Tank", "2:41.220", 6, false, {236, 114, 168, 255}},
      {6, "Flash", "2:42.890", 5, false, {60, 184, 210, 255}},
      {7, "Rumble", "2:45.110", 4, false, {234, 201, 71, 255}},
      {8, "Pebble", "2:48.330", 3, false, {169, 139, 116, 255}},
  };
  std::vector<CupStanding> standings = {{"Zippy", 42, false},
                                        {"YOU", 38, true},
                                        {"Blaze", 30, false},
                                        {"Drift", 25, false}};

  std::array<raylib::Texture2D, 8> portraits{};
  raylib::Texture2D scenery{};
  bool artwork_loaded = false;
  size_t selected_row = 1;
  std::string status = "Arrows to choose";

  const afterhours::Color sky{91, 188, 226, 255};
  const afterhours::Color navy{25, 75, 111, 255};
  const afterhours::Color blue_panel{43, 109, 150, 226};
  const afterhours::Color paper{217, 237, 237, 226};
  const afterhours::Color paper_alt{204, 226, 229, 222};
  const afterhours::Color yellow{255, 224, 55, 255};
  const afterhours::Color white{255, 255, 255, 255};
  const afterhours::Color teal{35, 128, 119, 255};
  const afterhours::Color dark_text{24, 75, 111, 255};
  const afterhours::Color muted{92, 113, 126, 255};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0);
  }

  static void triangle(Vector2Type a, Vector2Type b, Vector2Type c,
                       afterhours::Color color) {
    if ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0)
      std::swap(b, c);
    afterhours::draw_triangle(a, b, c, color);
  }

  static void paint(raylib::Texture2D t, RectangleType r) {
    if (t.id)
      raylib::DrawTexturePro(t, {0, 0, (float)t.width, (float)t.height}, r,
                             {0, 0}, 0, raylib::WHITE);
  }

  static void draw_header(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{91, 188, 226, 255});
    afterhours::draw_rectangle({r.x, r.y, r.width * .54f, r.height},
                               afterhours::Color{241, 244, 245, 255});
    triangle(
        {r.x + r.width * .54f, r.y}, {r.x + r.width * .54f, r.y + r.height},
        {r.x + r.width * .59f, r.y}, afterhours::Color{241, 244, 245, 255});
  }

  static void draw_slanted(RectangleType r, afterhours::Color fill) {
    const float inset = r.width * 26.f / 772.f;
    triangle({r.x + inset, r.y}, {r.x, r.y + r.height},
             {r.x + r.width - inset, r.y + r.height}, fill);
    triangle({r.x + inset, r.y}, {r.x + r.width - inset, r.y + r.height},
             {r.x + r.width, r.y}, fill);
  }

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

    if (context.pressed(InputAction::WidgetDown))
      selected_row = (selected_row + 1) % results.size();
    if (context.pressed(InputAction::WidgetUp))
      selected_row = (selected_row + results.size() - 1) % results.size();
    if (context.pressed(InputAction::WidgetPress))
      status = "Next race selected";
    if (context.pressed(InputAction::MenuBack))
      status = "Back";

    Theme theme;
    theme.font = dark_text;
    theme.darkfont = white;
    theme.font_muted = muted;
    theme.background = sky;
    theme.surface = paper;
    theme.primary = yellow;
    theme.secondary = blue_panel;
    theme.accent = teal;
    theme.roundness = .02f;
    theme.segments = 6;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("ArchivoMockBold", h720(18.f));

    if (!artwork_loaded) {
      artwork_loaded = true;
      const auto load = [](const std::string &name) {
        auto t =
            raylib::LoadTexture(afterhours::files::get_resource_path(
                                    "images", "marlo_kart/" + name + ".png")
                                    .string()
                                    .c_str());
        raylib::SetTextureFilter(t, raylib::TEXTURE_FILTER_BILINEAR);
        return t;
      };
      scenery = load("scenery");
      for (size_t i = 0; i < portraits.size(); ++i)
        portraits[i] = load("portrait_" + std::to_string(i));
    }
    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_on_draw_bg([this](RectangleType r) {
                          paint(scenery, r);
                          const float sy = r.height / 720.f;
                          afterhours::draw_rectangle(
                              {r.x, r.y + 654.f * sy, r.width, 66.f * sy},
                              afterhours::Color{242, 245, 246, 255});
                          afterhours::draw_rectangle(
                              {r.x, r.y + 654.f * sy, r.width, 4.f * sy},
                              afterhours::Color{187, 236, 251, 255});
                        })
                        .with_debug_name("race_root"));
    div(context, mk(root.ent(), 1),
        box(scale, 0, 0, 1280, 84)
            .with_on_draw_bg([](RectangleType r) { draw_header(r); })
            .with_debug_name("race_header"));
    div(context, mk(root.ent(), 2),
        box(scale, 43, 20, 380, 56)
            .with_label("RACE RESULTS")
            .with_font("ArchivoMockBold", h720(43.f))
            .with_custom_text_color(dark_text)
            .with_letter_spacing(-2.f * scale)
            .with_debug_name("race_title"));
    div(context, mk(root.ent(), 3),
        box(scale, 908, 30, 273, 32)
            .with_label("Rainbow Circuit - Race 3/4")
            .with_font("ArchivoMockBold", h720(25.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 4),
        box(scale, 1191, 20, 47, 44)
            .with_label("P1")
            .with_custom_background(yellow)
            .with_font("ArchivoMockBold", h720(31.f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("race_p1"));

    div(context, mk(root.ent(), 100),
        box(scale, 29, 113, 772, 510)
            .with_on_draw_bg([](RectangleType r) {
              draw_slanted(r, afterhours::Color{217, 237, 237, 238});
            })
            .with_debug_name("race_results_panel"));
    div(context, mk(root.ent(), 101),
        box(scale, 54, 114, 747, 38)
            .with_custom_background(afterhours::Color{23, 82, 119, 255})
            .with_debug_name("race_table_header"));
    div(context, mk(root.ent(), 102),
        box(scale, 76, 126, 48, 16)
            .with_label("POS.")
            .with_font("ArchivoMockBold", h720(13.f))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 103),
        box(scale, 180, 126, 120, 16)
            .with_label("RACER")
            .with_font("ArchivoMockBold", h720(13.f))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 104),
        box(scale, 644, 126, 70, 16)
            .with_label("TIME")
            .with_font("ArchivoMockBold", h720(13.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 105),
        box(scale, 756, 126, 40, 16)
            .with_label("PTS")
            .with_font("ArchivoMockBold", h720(13.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));

    for (size_t i = 0; i < results.size(); ++i) {
      const auto &r = results[i];
      const float y = 152.f + static_cast<float>(i) * 55.f;
      const bool active = i == selected_row;
      if (button(
              context, mk(root.ent(), 200 + static_cast<int>(i)),
              box(scale, 55.f - (y - 113.f) * 26.f / 510.f, y, 746, 55)
                  .with_label("")
                  .with_on_draw_bg([=, this](RectangleType r) {
                    const auto fill =
                        active ? yellow : (i % 2 == 0 ? paper : paper_alt);
                    const float shift = 55.f * 26.f / 510.f * scale;
                    triangle({r.x, r.y}, {r.x - shift, r.y + r.height},
                             {r.x + r.width - shift, r.y + r.height}, fill);
                    triangle({r.x, r.y},
                             {r.x + r.width - shift, r.y + r.height},
                             {r.x + r.width, r.y}, fill);
                  })
                  .with_debug_name("race_result_row_" + std::to_string(i)))) {
        selected_row = i;
        status = results[i].name;
      }
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 73, y + 9, 42, 40)
              .with_label(std::to_string(r.position))
              .with_font("ArchivoMockBold", h720(31.f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 400 + static_cast<int>(i)),
          box(scale, 129, y + 5, 40, 43)
              .with_on_draw_fg([texture = portraits[i]](RectangleType rr) {
                paint(texture, rr);
              })
              .with_ignore_pointer_events()
              .with_debug_name("race_portrait_" + std::to_string(i)));
      div(context, mk(root.ent(), 500 + static_cast<int>(i)),
          box(scale, 178, y + 10, 250, 36)
              .with_label(r.name)
              .with_font("ArchivoMockBold", h720(31.f))
              .with_custom_text_color(active ? dark_text : dark_text)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 600 + static_cast<int>(i)),
          box(scale, 584, y + 10, 128, 36)
              .with_label(r.time)
              .with_font("ArchivoMockBold", h720(22.f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 700 + static_cast<int>(i)),
          box(scale, 726, y + 10, 50, 36)
              .with_label("+" + std::to_string(r.points))
              .with_font("ArchivoMockBold", h720(31.f))
              .with_custom_text_color(teal)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 800),
        box(scale, 816, 113, 421, 504)
            .with_custom_background(blue_panel)
            .with_debug_name("race_standings_panel"));
    div(context, mk(root.ent(), 801),
        box(scale, 948, 132, 188, 84)
            .with_label("2nd")
            .with_font("ArchivoMockBold", h720(76.f))
            .with_custom_text_color(yellow)
            .with_text_shadow(afterhours::Color{8, 57, 87, 255}, 0.f,
                              5.f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("race_position"));
    div(context, mk(root.ent(), 802),
        box(scale, 920, 232, 245, 38)
            .with_label("CUP STANDINGS")
            .with_font("ArchivoMockBold", h720(28.f))
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < standings.size(); ++i) {
      const auto &s = standings[i];
      const float y = 289.f + static_cast<float>(i) * 45.f;
      div(context, mk(root.ent(), 820 + static_cast<int>(i) * 3),
          box(scale, 841, y, 372, 1)
              .with_custom_background(afterhours::Color{119, 163, 176, 150}));
      div(context, mk(root.ent(), 821 + static_cast<int>(i) * 3),
          box(scale, 844, y - 1, 240, 38)
              .with_label(std::to_string(i + 1) + ".   " + s.name)
              .with_font("ArchivoMockBold", h720(25.f))
              .with_custom_text_color(s.is_player ? yellow : white)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("race_standing_" + std::to_string(i)));
      div(context, mk(root.ent(), 822 + static_cast<int>(i) * 3),
          box(scale, 1122, y - 1, 88, 38)
              .with_label(std::to_string(s.total_points) + " pts")
              .with_font("ArchivoMockBold", h720(25.f))
              .with_custom_text_color(s.is_player ? yellow : white)
              .with_alignment(TextAlignment::Right));
    }
    div(context, mk(root.ent(), 840),
        box(scale, 841, 481, 372, 3)
            .with_custom_background(afterhours::Color{190, 230, 236, 255}));
    div(context, mk(root.ent(), 841),
        box(scale, 982, 504, 120, 22)
            .with_label("BEST LAP")
            .with_font("Archivo", h720(14.f))
            .with_letter_spacing(2.f * scale)
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 842),
        box(scale, 920, 526, 245, 62)
            .with_label("0:48.220")
            .with_font("ArchivoMockBold", h720(48.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 843),
        box(scale, 952, 584, 182, 24)
            .with_label("New personal best!")
            .with_font("ArchivoMockBold", h720(12.f))
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(root.ent(), 900),
               box(scale, 38, 665, 97, 48)
                   .with_label("< Back")
                   .with_custom_background(afterhours::Color{17, 112, 169, 255})
                   .with_font("ArchivoMockBold", h720(22.f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("race_back"))) {
      status = "Back";
    }
    div(context, mk(root.ent(), 901),
        box(scale, 158, 680, 360, 26)
            .with_label(status + "   A Confirm   B Back")
            .with_font("Archivo", h720(15.f))
            .with_custom_text_color(afterhours::Color{54, 105, 130, 255})
            .with_debug_name("race_status"));
    if (button(context, mk(root.ent(), 902),
               box(scale, 1044, 664, 200, 54)
                   .with_label("NEXT RACE >")
                   .with_custom_background(yellow)
                   .with_font("ArchivoMockBold", h720(25.f))
                   .with_custom_text_color(dark_text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("race_next"))) {
      status = "Next race selected";
    }
  }
};

REGISTER_EXAMPLE_SCREEN(race_results, "Game Mockups",
                        "Racing game post-race results and standings",
                        RaceResultsScreen)
