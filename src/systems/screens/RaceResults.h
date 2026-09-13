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
      {2, "Blaze / You", "2:33.812", 12, true, {51, 190, 91, 255}},
      {3, "Turbo", "2:35.100", 10, false, {245, 143, 48, 255}},
      {4, "Drift", "2:37.445", 8, false, {112, 90, 188, 255}},
      {5, "Tank", "2:41.220", 6, false, {236, 114, 168, 255}},
      {6, "Flash", "2:42.890", 5, false, {60, 184, 210, 255}},
      {7, "Rumble", "2:45.110", 4, false, {234, 201, 71, 255}},
      {8, "Pebble", "2:48.330", 3, false, {169, 139, 116, 255}},
  };
  std::vector<CupStanding> standings = {{"Zippy", 42, false},
                                        {"Blaze / You", 38, true},
                                        {"Turbo", 30, false},
                                        {"Drift", 25, false}};

  std::array<raylib::Texture2D, 8> portraits{};
  raylib::Texture2D scenery{};
  bool artwork_loaded = false;
  size_t selected_row = 1;
  std::string status = "Arrows to choose";

  const afterhours::Color sky{91, 188, 226, 255};
  const afterhours::Color navy{25, 75, 111, 255};
  const afterhours::Color blue_panel{32, 91, 129, 248};
  const afterhours::Color paper{223, 240, 242, 255};
  const afterhours::Color paper_alt{205, 227, 231, 255};
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
    UIStylingDefaults::get().set_default_font("ArchivoMockBold", pixels(18.f * scale));

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
                        .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
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
            .with_font("ArchivoMockBoldItalic", pixels(43.f * scale))
            .with_custom_text_color(dark_text)
            .with_letter_spacing(-2.f * scale)
            .with_debug_name("race_title"));
    div(context, mk(root.ent(), 3),
        box(scale, 860, 10, 280, 34)
            .with_label("Rainbow Circuit")
            .with_font("ArchivoMockBold", pixels(25.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 4),
        box(scale, 1150, 20, 94, 44)
            .with_label("Player 1")
            .with_custom_background(yellow)
            .with_font("ArchivoMockBold", pixels(23.f * scale))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("race_p1"));

    const auto caption = [&](int id, const std::string &text, float x, float y, float w, float h,
                              float size, afterhours::Color color) {
      div(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("ArchivoMockBold", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    };
    caption(10, "Race 3/4", 866, 46, 128, 28, 21, white);
    for (int i = 0; i < 4; ++i)
      div(context, mk(root.ent(), 11 + i), box(scale, 1006 + static_cast<float>(i) * 28, 57, 20, 5)
          .with_custom_background(i < 3 ? yellow : blue_panel));
    div(context, mk(root.ent(), 100),
        box(scale, 29, 113, 772, 480)
            .with_on_draw_bg([](RectangleType r) {
              draw_slanted(r, afterhours::Color{217, 237, 237, 238});
            })
            .with_debug_name("race_results_panel"));
    div(context, mk(root.ent(), 101),
        box(scale, 54, 114, 747, 38)
            .with_custom_background(afterhours::Color{23, 82, 119, 255})
            .with_debug_name("race_table_header"));
    div(context, mk(root.ent(), 102),
        box(scale, 72, 120, 70, 28)
            .with_label("POS.")
            .with_font("ArchivoMockBold", pixels(18.f * scale))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 103),
        box(scale, 180, 120, 260, 28)
            .with_label("RACER")
            .with_font("ArchivoMockBold", pixels(18.f * scale))
            .with_custom_text_color(white));
    div(context, mk(root.ent(), 104),
        box(scale, 552, 120, 128, 28)
            .with_label("Race time")
            .with_font("ArchivoMockBold", pixels(18.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));
    div(context, mk(root.ent(), 105),
        box(scale, 682, 120, 99, 28)
            .with_label("Race points")
            .with_font("ArchivoMockBold", pixels(18.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));

    for (size_t i = 0; i < results.size(); ++i) {
      const auto &r = results[i];
      const float y = 152.f + static_cast<float>(i) * 55.f;
      const bool active = i == selected_row;
      if (button(
              context, mk(root.ent(), 200 + static_cast<int>(i)),
              box(scale, 54, y, 747, 55)
                  .with_label("")
                  .with_on_draw_bg([=, this](RectangleType r) {
                    const auto fill =
                        active ? yellow : (i % 2 == 0 ? paper : paper_alt);
                    afterhours::draw_rectangle(r, fill);
                    afterhours::draw_rectangle({r.x, r.y + r.height - scale, r.width, scale}, {133, 173, 185, 255});
                  })
                  .with_debug_name("race_result_row_" + std::to_string(i)))) {
        selected_row = i;
        status = results[i].name;
      }
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 73, y + 9, 42, 40)
              .with_label(std::to_string(r.position))
              .with_font("ArchivoMockBold", pixels(31.f * scale))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 400 + static_cast<int>(i)),
          box(scale, 129, y + 5, 40, 43)
              .with_on_draw_fg([texture = portraits[std::array<size_t, 8>{4, 1, 0, 2, 3, 6, 5, 7}[i]]](RectangleType rr) {
                paint(texture, rr);
              })
              .with_ignore_pointer_events()
              .with_debug_name("race_portrait_" + std::to_string(i)));
      div(context, mk(root.ent(), 500 + static_cast<int>(i)),
          box(scale, 178, y + 10, 280, 36)
              .with_label(r.name)
              .with_font("ArchivoMockBold", pixels(31.f * scale))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 600 + static_cast<int>(i)),
          box(scale, 552, y + 10, 128, 36)
              .with_label(r.time)
              .with_font("ArchivoMockBold", pixels(22.f * scale))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 700 + static_cast<int>(i)),
          box(scale, 722, y + 10, 59, 36)
              .with_label("+" + std::to_string(r.points))
              .with_font("ArchivoMockBold", pixels(31.f * scale))
              .with_custom_text_color(teal)
              .with_alignment(TextAlignment::Right)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 800),
        box(scale, 816, 113, 421, 504)
            .with_custom_background(blue_panel)
            .with_debug_name("race_standings_panel"));
    div(context, mk(root.ent(), 801),
        box(scale, 948, 136, 188, 78)
            .with_label("2nd")
            .with_font("ArchivoMockBoldItalic", pixels(76.f * scale))
            .with_custom_text_color(yellow)
            .with_text_shadow(afterhours::Color{8, 57, 87, 255}, 0.f,
                              5.f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("race_position"));
    div(context, mk(root.ent(), 802),
        box(scale, 870, 231, 320, 38)
            .with_label("CUP STANDINGS")
            .with_font("ArchivoMockBoldItalic", pixels(28.f * scale))
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < standings.size(); ++i) {
      const auto &s = standings[i];
      const float y = 298.f + static_cast<float>(i) * 40.f;
      div(context, mk(root.ent(), 820 + static_cast<int>(i) * 3),
          box(scale, 841, y, 372, 1)
              .with_custom_background(afterhours::Color{119, 163, 176, 150}));
      div(context, mk(root.ent(), 821 + static_cast<int>(i) * 3),
          box(scale, 844, y - 1, 240, 38)
              .with_label(std::to_string(i + 1) + ".   " + s.name)
              .with_font("ArchivoMockBold", pixels(25.f * scale))
              .with_custom_text_color(s.is_player ? yellow : white)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("race_standing_" + std::to_string(i)));
      div(context, mk(root.ent(), 822 + static_cast<int>(i) * 3),
          box(scale, 1122, y - 1, 88, 38)
              .with_label(std::to_string(s.total_points) + " pts")
              .with_font("ArchivoMockBold", pixels(25.f * scale))
              .with_custom_text_color(s.is_player ? yellow : white)
              .with_alignment(TextAlignment::Right));
    }
    caption(850, "Race finish", 948, 111, 188, 27, 19, white);
    caption(851, "Top 4 of 8 / cumulative points", 854, 269, 348, 26, 18, white);
    caption(852, "4 points behind the leader", 854, 460, 348, 26, 20, yellow);
    caption(853, "+2.362 behind the race winner", 841, 207, 372, 26, 19, white);
    caption(854, "Player 1 / Blaze", 54, 600, 280, 30, 22, dark_text);
    caption(855, status, 344, 600, 455, 30, 21, dark_text);
    caption(856, "Demo continuation / final race", 1010, 632, 246, 26, 18, dark_text);
    div(context, mk(root.ent(), 857), box(scale, 117, 215, 18, 22)
        .with_label("1").with_font("ArchivoMockBold", pixels(16 * scale))
        .with_custom_background(teal).with_custom_text_color(white)
        .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    div(context, mk(root.ent(), 840),
        box(scale, 841, 487, 372, 2)
            .with_custom_background(afterhours::Color{190, 230, 236, 255}));
    div(context, mk(root.ent(), 841),
        box(scale, 916, 494, 252, 30)
            .with_label("YOUR BEST LAP")
            .with_font("Archivo", pixels(18.f * scale))
            .with_letter_spacing(2.f * scale)
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 842),
        box(scale, 920, 526, 245, 62)
            .with_label("0:48.220")
            .with_font("ArchivoMockBold", pixels(48.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 843),
        box(scale, 866, 581, 330, 29)
            .with_label("No previous lap recorded")
            .with_font("ArchivoMockBold", pixels(18.f * scale))
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(root.ent(), 900),
               box(scale, 38, 665, 150, 48)
                   .with_label("< Back")
                   .with_on_draw_bg([](RectangleType r) { draw_slanted(r, {17, 112, 169, 255}); })
                   .with_font("ArchivoMockBold", pixels(22.f * scale))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("race_back"))) {
      status = "Back";
    }
    div(context, mk(root.ent(), 901),
        box(scale, 220, 684, 790, 28)
            .with_label("Up / Down: Racer     Enter: Next race     Esc: Back")
            .with_font("Archivo", pixels(19.f * scale))
            .with_custom_text_color(afterhours::Color{54, 105, 130, 255})
            .with_alignment(TextAlignment::Center).with_debug_name("race_status"));
    if (button(context, mk(root.ent(), 902),
               box(scale, 1044, 664, 200, 54)
                   .with_label("NEXT RACE >")
                   .with_on_draw_bg([](RectangleType r) { draw_slanted(r, {255, 224, 55, 255}); })
                   .with_font("ArchivoMockBold", pixels(25.f * scale))
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
