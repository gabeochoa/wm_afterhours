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

struct GuessWhoLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kCols = 8;
  static constexpr int kRows = 3;
  static constexpr int kCount = kCols * kRows;

  struct Face {
    const char *name;
    int hair;
    int eyes;
    bool glasses;
    bool hat;
  };

  static const Face &face(int index) {
    static const Face faces[kCount] = {
        {"Ada", 0, 1, true, false},   {"Bram", 1, 0, false, true},
        {"Cleo", 2, 1, false, false}, {"Dov", 0, 0, true, true},
        {"Esme", 1, 1, false, false}, {"Finn", 2, 0, true, false},
        {"Gil", 0, 1, false, true},   {"Hana", 1, 0, true, false},
        {"Ivo", 2, 1, true, true},    {"Juno", 0, 0, false, false},
        {"Kit", 1, 1, true, true},    {"Lior", 2, 0, false, true},
        {"Mira", 0, 1, false, false}, {"Nils", 1, 0, true, false},
        {"Oona", 2, 1, true, false},  {"Pax", 0, 0, false, true},
        {"Quin", 1, 1, false, true},  {"Rune", 2, 0, true, true},
        {"Sena", 0, 1, true, false},  {"Tao", 1, 0, false, false},
        {"Uma", 2, 1, false, true},   {"Vero", 0, 0, true, false},
        {"Wren", 1, 1, true, false},  {"Yuki", 2, 0, false, true},
    };
    return faces[index];
  }

  const std::vector<std::string> attributes{
      "wear glasses?", "wear a hat?",      "have dark hair?",
      "have fair hair?", "have red hair?", "have blue eyes?"};
  const std::vector<std::string> answers{"Yes", "No"};
  const std::vector<std::string> notes{"Not sure", "Maybe", "No",
                                       "This one!"};

  size_t attribute_index = 0;
  size_t answer_index = 0;
  size_t note_index[kCount]{};
  bool down[kCount]{};
  int asked = 0;
  bool loaded = false;
  raylib::Texture2D portrait_texture{};
  raylib::Texture2D logo_texture{};
  std::string feedback = "A little question. A big clue.";

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None);
  }

  static bool matches(const Face &candidate, size_t attribute) {
    switch (attribute) {
    case 0:
      return candidate.glasses;
    case 1:
      return candidate.hat;
    case 2:
      return candidate.hair == 0;
    case 3:
      return candidate.hair == 1;
    case 4:
      return candidate.hair == 2;
    default:
      return candidate.eyes == 1;
    }
  }

  int faces_left() const {
    int count = 0;
    for (bool eliminated : down)
      count += eliminated ? 0 : 1;
    return count;
  }

  void ask_question() {
    const bool desired = answer_index == 0;
    for (int index = 0; index < kCount; ++index)
      if (matches(face(index), attribute_index) != desired)
        down[index] = true;
    ++asked;
    const int left = faces_left();
    feedback = fmt::format("Question {}: {}! {} possible {} left.", asked,
                           answers[answer_index], left,
                           left == 1 ? "person" : "people");
  }

  void reset() {
    for (int index = 0; index < kCount; ++index) {
      down[index] = false;
      note_index[index] = 0;
    }
    attribute_index = 0;
    answer_index = 0;
    asked = 0;
    feedback = "A little question. A big clue.";
  }

  static void draw_background(RectangleType r, float scale) {
    const afterhours::Color edge{167, 210, 235, 255};
    const afterhours::Color center{211, 237, 251, 255};
    afterhours::draw_rectangle(r, edge);
    for (int layer = 72; layer >= 1; --layer) {
      const float radius = static_cast<float>(layer) / 72.f;
      const float toward_center = 1.f - radius;
      const auto blend = [toward_center](unsigned char a, unsigned char b) {
        return static_cast<unsigned char>(
            static_cast<float>(a) +
            (static_cast<float>(b) - static_cast<float>(a)) *
                toward_center);
      };
      afterhours::draw_ellipse(
          static_cast<int>(r.x + 640.f * scale),
          static_cast<int>(r.y + 216.f * scale), 760.f * scale * radius,
          500.f * scale * radius,
          afterhours::Color{blend(edge.r, center.r),
                            blend(edge.g, center.g),
                            blend(edge.b, center.b), 255});
    }
  }

  static void draw_board(RectangleType r, float scale) {
    afterhours::draw_rectangle_rounded(
        {r.x, r.y + 10.f * scale, r.width, r.height}, .08f, 20,
        afterhours::Color{8, 73, 120, 255}, RoundedCorners().all_round());
    afterhours::draw_rectangle_rounded(
        r, .08f, 20, afterhours::Color{34, 120, 184, 255},
        RoundedCorners().all_round());
    afterhours::draw_rectangle_gradient_v(
        {r.x + 7.f * scale, r.y + 7.f * scale, r.width - 14.f * scale,
         r.height - 14.f * scale},
        afterhours::Color{40, 124, 192, 255},
        afterhours::Color{18, 98, 169, 255});
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .08f, 20, 7.f * scale, afterhours::Color{34, 120, 184, 255});
    afterhours::draw_rectangle(
        {r.x + 8.f * scale, r.y, r.width - 16.f * scale, 7.f * scale},
        afterhours::Color{81, 158, 222, 255});
  }

  static void draw_portrait_panel(RectangleType r, bool eliminated,
                                  float scale) {
    if (eliminated) {
      RectangleType folded{r.x + 3.f * scale, r.y + 60.f * scale,
                           r.width - 6.f * scale, 39.f * scale};
      afterhours::draw_rectangle_gradient_v(
          folded, afterhours::Color{255, 247, 219, 120},
          afterhours::Color{255, 253, 244, 120});
      afterhours::draw_rectangle_rounded_lines_ex(
          folded, .06f, 8, 3.f * scale,
          afterhours::Color{248, 208, 86, 130});
      return;
    }
    afterhours::draw_rectangle_gradient_v(
        r, afterhours::Color{255, 247, 219, 255},
        afterhours::Color{255, 253, 244, 255});
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .06f, 8, 4.f * scale, afterhours::Color{248, 208, 86, 255});
    afterhours::draw_rectangle(
        {r.x + 4.f * scale, r.y + r.height - 5.f * scale,
         r.width - 8.f * scale, 5.f * scale},
        afterhours::Color{229, 172, 34, 255});
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float scale =
        context.screen_height > 0.f ? context.screen_height / 720.f : 1.f;
    const afterhours::Color navy{18, 63, 113, 255};
    const afterhours::Color white{255, 255, 255, 255};
    Theme theme;
    theme.font = navy;
    theme.darkfont = white;
    theme.font_muted = afterhours::Color{54, 95, 130, 255};
    theme.background = afterhours::Color{167, 210, 235, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{236, 75, 70, 255};
    theme.secondary = afterhours::Color{7, 70, 121, 255};
    theme.accent = afterhours::Color{248, 208, 86, 255};
    theme.roundness = .1f;
    theme.segments = 16;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("FredokaMockBold", pixels(20.f));

    if (!loaded) {
      loaded = true;
      portrait_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path(
              "images", "guess_who_lab/portraits.png")
              .string()
              .c_str());
      logo_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path("images",
                                                "guess_who_lab/logo.png")
              .string()
              .c_str());
      raylib::SetTextureFilter(portrait_texture,
                               raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(logo_texture, raylib::TEXTURE_FILTER_BILINEAR);
    }

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_on_draw_bg([scale](RectangleType r) {
                  draw_background(r, scale);
                })
                .with_debug_name("gw_root"));

    auto text = [&](int id, const std::string &label, float x, float y,
                    float width, float height, float size,
                    afterhours::Color color,
                    TextAlignment alignment = TextAlignment::Left,
                    const std::string &font = "FredokaMockBold") {
      return div(context, mk(root.ent(), id),
                 box(scale, x, y, width, height)
                     .with_label(label)
                     .with_font(font, pixels(size * scale))
                     .with_custom_text_color(color)
                     .with_alignment(alignment)
                     .with_text_inset(0.f, 0.f));
    };

    sprite(context, mk(root.ent(), 1), logo_texture,
           {0.f, 0.f, static_cast<float>(logo_texture.width),
            static_cast<float>(logo_texture.height)},
           box(scale, 41.f, 4.f, 271.f, 101.f)
               .with_ignore_pointer_events()
               .with_debug_name("gw_logo"));
    div(context, mk(root.ent(), 2),
        box(scale, 379.f, 36.f, 10.f, 10.f)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_circle(
                  static_cast<int>(r.x + r.width * .5f),
                  static_cast<int>(r.y + r.height * .5f), r.width * .5f,
                  afterhours::Color{230, 62, 62, 255});
            }));
    text(3, "YOUR TURN", 402.f, 24.f, 200.f, 38.f, 21.f, navy);
    text(4, "Find the mystery person", 378.f, 55.f, 270.f, 32.f, 20.f,
         afterhours::Color{54, 95, 130, 255}, TextAlignment::Left,
         "AtkinsonMock");

    if (button(
            context, mk(root.ent(), 5),
            box(scale, 1093.f, 31.f, 143.f, 48.f)
                .with_label("New game")
                .with_font("AtkinsonMock", pixels(21.f * scale))
                .with_custom_background(afterhours::Color{255, 255, 255, 96})
                .with_border(white, 2.f * scale)
                .with_corner_radius(20.f * scale)
                .with_custom_text_color(navy)
                .with_alignment(TextAlignment::Center)
                .with_text_inset(0.f, 0.f)
                .with_on_draw_fg([scale, navy](RectangleType r) {
                  const float cx = r.x + 119.f * scale;
                  const float cy = r.y + 24.f * scale;
                  afterhours::draw_ring_segment(
                      cx, cy, 5.f * scale, 7.f * scale, -55.f, 245.f, 18,
                      navy);
                  afterhours::draw_line_ex(
                      {cx + 2.f * scale, cy - 7.f * scale},
                      {cx + 8.f * scale, cy - 7.f * scale}, 2.f * scale,
                      navy);
                  afterhours::draw_line_ex(
                      {cx + 8.f * scale, cy - 7.f * scale},
                      {cx + 7.f * scale, cy - 1.f * scale}, 2.f * scale,
                      navy);
                })
                .with_debug_name("gw_reset"))) {
      reset();
    }

    div(context, mk(root.ent(), 10),
        box(scale, 40.f, 105.f, 1200.f, 75.f)
            .with_custom_background(afterhours::Color{255, 255, 255, 153})
            .with_corner_radius(14.f * scale)
            .with_on_draw_fg([scale](RectangleType r) {
              afterhours::draw_rectangle_rounded(
                  {r.x, r.y + r.height - 2.f * scale, r.width, 2.f * scale},
                  .2f, 8, afterhours::Color{133, 179, 209, 255},
                  RoundedCorners().all_round());
            })
            .with_debug_name("gw_question"));
    text(11, "DOES YOUR PERSON", 60.f, 119.f, 180.f, 46.f, 21.f, navy);

    dropdown(context, mk(root.ent(), 12), attributes, attribute_index,
             box(scale, 240.f, 120.f, 245.f, 45.f)
                 .with_font("AtkinsonMock", pixels(24.f * scale))
                 .with_custom_background(white)
                 .with_border(afterhours::Color{176, 207, 225, 255},
                              2.f * scale)
                 .with_corner_radius(8.f * scale)
                 .with_custom_text_color(navy)
                 .with_debug_name("gw_trait"));
    dropdown(context, mk(root.ent(), 13), answers, answer_index,
             box(scale, 501.f, 120.f, 80.f, 45.f)
                 .with_font("AtkinsonMock", pixels(24.f * scale))
                 .with_custom_background(white)
                 .with_border(afterhours::Color{176, 207, 225, 255},
                              2.f * scale)
                 .with_corner_radius(8.f * scale)
                 .with_custom_text_color(navy)
                 .with_debug_name("gw_answer"));
    if (button(context, mk(root.ent(), 14),
               box(scale, 597.f, 117.f, 174.f, 50.f)
                   .with_label("Ask question")
                   .with_font("AtkinsonMock", pixels(23.f * scale))
                   .with_custom_background(afterhours::Color{236, 75, 70, 255})
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_text_inset(0.f, 0.f)
                   .with_corner_radius(9.f * scale)
                   .with_on_draw_fg([scale](RectangleType r) {
                     afterhours::draw_rectangle(
                         {r.x, r.y + r.height - 4.f * scale, r.width,
                         4.f * scale},
                         afterhours::Color{181, 44, 49, 255});
                     const afterhours::Color white{255, 255, 255, 255};
                     const float x = r.x + 151.f * scale;
                     const float cy = r.y + 24.f * scale;
                     afterhours::draw_line_ex(
                         {x - 8.f * scale, cy}, {x, cy}, 2.f * scale,
                         white);
                     afterhours::draw_line_ex(
                         {x - 5.f * scale, cy - 5.f * scale}, {x, cy},
                         2.f * scale, white);
                     afterhours::draw_line_ex(
                         {x, cy}, {x - 5.f * scale, cy + 5.f * scale},
                         2.f * scale, white);
                   })
                   .with_debug_name("gw_ask"))) {
      ask_question();
    }
    text(15, fmt::format("{} faces left", faces_left()), 1070.f, 119.f,
         150.f, 46.f, 23.f, navy, TextAlignment::Right);

    div(context, mk(root.ent(), 20),
        box(scale, 40.f, 193.f, 1200.f, 461.f)
            .with_on_draw_bg([scale](RectangleType r) {
              draw_board(r, scale);
            })
            .with_debug_name("gw_board"));

    for (int index = 0; index < kCount; ++index) {
      const int row = index / kCols;
      const int col = index % kCols;
      const float x = 69.f + static_cast<float>(col) * 145.f;
      const float y = 217.f + static_cast<float>(row) * 142.f;
      const bool eliminated = down[index];
      const auto face_button =
          button(context, mk(root.ent(), 100 + index),
                 box(scale, x, y, 128.f, 102.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([eliminated, scale](RectangleType r) {
                       draw_portrait_panel(r, eliminated, scale);
                     })
                     .with_debug_name("gw_face_" + std::to_string(index)));

      const float source_x = static_cast<float>(col) * 256.f;
      const float source_y = static_cast<float>(row) * 168.f;
      div(context, mk(root.ent(), 200 + index),
          box(scale, x, y - 5.f, 128.f, 84.f)
              .with_on_draw_fg([texture = portrait_texture, source_x, source_y,
                                eliminated, scale](RectangleType r) {
                RectangleType destination = r;
                if (eliminated) {
                  destination.x += 17.f * scale;
                  destination.y += 58.f * scale;
                  destination.width -= 34.f * scale;
                  destination.height = 31.f * scale;
                }
                raylib::DrawTexturePro(
                    texture, {source_x, source_y, 256.f, 168.f}, destination,
                    {0.f, 0.f}, 0.f,
                    eliminated ? raylib::Color{255, 255, 255, 105}
                               : raylib::WHITE);
              })
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 300 + index),
          box(scale, x + 4.f, y + 77.f, 120.f, 20.f)
              .with_custom_background(
                  eliminated ? afterhours::Color{255, 247, 220, 110}
                             : afterhours::Color{255, 247, 220, 255})
              .with_ignore_pointer_events());
      text(400 + index, face(index).name, x + 4.f, y + 76.f, 120.f, 22.f,
           17.5f,
           eliminated ? afterhours::Color{36, 46, 55, 115}
                      : afterhours::Color{36, 46, 55, 255},
           TextAlignment::Center);
      if (face_button) {
        down[index] = !down[index];
        feedback = down[index] ? std::string(face(index).name) + " flipped down"
                               : std::string(face(index).name) + " restored";
      }

      dropdown(
          context, mk(root.ent(), 500 + index), notes, note_index[index],
          box(scale, x, y + 106.f, 128.f, 22.f)
              .with_font("AtkinsonMock", pixels(14.f * scale))
              .with_custom_background(afterhours::Color{7, 70, 121, 255})
              .with_custom_text_color(afterhours::Color{212, 233, 246, 255})
              .with_corner_radius(3.f * scale)
              .with_debug_name("gw_note_" + std::to_string(index)));
    }

    text(700, feedback, 48.f, 674.f, 650.f, 34.f, 20.f,
         afterhours::Color{54, 95, 130, 255}, TextAlignment::Left,
         "AtkinsonMock");
    text(701, "Click a face to flip it down.", 950.f, 674.f, 282.f, 34.f,
         20.f, afterhours::Color{54, 95, 130, 255}, TextAlignment::Right,
         "AtkinsonMock");
  }
};

REGISTER_EXAMPLE_SCREEN(guess_who_lab, "Stress Tests",
                        "26 dropdowns playing guess who, with drawn faces",
                        GuessWhoLab)
