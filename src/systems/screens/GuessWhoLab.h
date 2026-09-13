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
  std::string feedback = "Choose a trait, enter the answer, then ask.";
  std::vector<std::string> clue_history;

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None).with_corner_radius(0).with_skip_grid_snap(true);
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
    static constexpr std::array<const char *, 6> traits{"Glasses", "Hat", "Dark hair", "Fair hair", "Red hair", "Blue eyes"};
    clue_history.push_back(std::string(traits[attribute_index]) + ": " + answers[answer_index]);
    if (clue_history.size() > 3) clue_history.erase(clue_history.begin());
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
    clue_history.clear();
    feedback = "Choose a trait, enter the answer, then ask.";
  }

  static void draw_background(RectangleType r, float) {
    afterhours::draw_rectangle(r, {232, 240, 245, 255});
  }

  static void draw_board(RectangleType r, float scale) {
    afterhours::draw_rectangle_rounded(r, .04f, 16, {38, 72, 94, 255}, RoundedCorners().all_round());
    afterhours::draw_rectangle_rounded_lines_ex(r, .04f, 16, scale, {73, 107, 127, 255});
  }

  static void draw_portrait_panel(RectangleType r, bool eliminated,
                                  float scale, bool candidate) {
    const auto fill = eliminated ? afterhours::Color{86, 110, 125, 255}
                                 : afterhours::Color{255, 251, 240, 255};
    afterhours::draw_rectangle_rounded(r, .06f, 8, fill, RoundedCorners().all_round());
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .06f, 8, (candidate ? 3.f : 1.f) * scale,
        candidate ? afterhours::Color{250, 203, 93, 255} : afterhours::Color{152, 171, 181, 255});
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float scale =
        std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color navy{18, 63, 113, 255};
    const afterhours::Color white{255, 255, 255, 255};
    Theme theme;
    theme.font = navy;
    theme.darkfont = white;
    theme.font_muted = afterhours::Color{54, 95, 130, 255};
    theme.background = afterhours::Color{232, 240, 245, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{236, 75, 70, 255};
    theme.secondary = afterhours::Color{7, 70, 121, 255};
    theme.accent = afterhours::Color{248, 208, 86, 255};
    theme.corner_radius = 6;
    theme.roundness = 0;
    theme.segments = 16;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20.f * scale));

    if (!loaded) {
      loaded = true;
      auto portrait_image = raylib::LoadImage(afterhours::files::get_resource_path(
          "images", "guess_who_lab/portraits.png").string().c_str());
      raylib::ImageColorContrast(&portrait_image, 10.f);
      portrait_texture = raylib::LoadTextureFromImage(portrait_image);
      raylib::UnloadImage(portrait_image);
      logo_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path("images",
                                                "guess_who_lab/logo.png")
              .string()
              .c_str());
      raylib::SetTextureFilter(portrait_texture,
                               raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(logo_texture, raylib::TEXTURE_FILTER_BILINEAR);
    }

    div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("gw_canvas"));
    auto root =
        div(context, mk(entity, 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                        (context.screen_height - 720 * scale) / 2)
                .with_on_draw_bg([scale](RectangleType r) {
                  draw_background(r, scale);
                })
                .with_debug_name("gw_root"));

    auto text = [&](int id, const std::string &label, float x, float y,
                    float width, float height, float size,
                    afterhours::Color color,
                    TextAlignment alignment = TextAlignment::Left,
                    const std::string &font = "AtkinsonMock") {
      return div(context, mk(root.ent(), id),
                 box(scale, x, y, width, height)
                     .with_label(label)
                     .with_font(font, pixels(size * scale))
                     .with_custom_text_color(color)
                     .with_alignment(alignment)
                     .with_text_inset(0.f, 0.f).with_ignore_pointer_events());
    };

    const auto pick = [&](int id, const auto &options, size_t &index, float x, float y,
                          float width, float height, float font_size,
                          afterhours::Color fill, afterhours::Color ink, const std::string &name) {
      auto result = dropdown(context, mk(root.ent(), id), options, index,
          box(scale, x, y, width, height).with_font("AtkinsonMock", pixels(font_size * scale))
              .with_custom_background(fill).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
              .with_dropdown_indicators("", "").with_corner_radius(6 * scale).with_debug_name(name));
      const bool opened = result.ent().template get<HasDropdownState>().on;
      div(context, mk(root.ent(), 1000 + id), box(scale, x + width - 23, y + height / 2 - 4, 12, 8)
          .with_ignore_pointer_events().with_render_layer(2)
          .with_on_draw_fg([scale, ink, opened](RectangleType r) {
            const float y1 = opened ? r.y + r.height : r.y;
            const float y2 = opened ? r.y : r.y + r.height;
            afterhours::draw_line_ex({r.x, y1}, {r.x + r.width / 2, y2}, 1.5f * scale, ink);
            afterhours::draw_line_ex({r.x + r.width / 2, y2}, {r.x + r.width, y1}, 1.5f * scale, ink);
          }));
      return result;
    };
    sprite(context, mk(root.ent(), 1), logo_texture,
           {0.f, 0.f, static_cast<float>(logo_texture.width), static_cast<float>(logo_texture.height)},
           box(scale, 41, 8, 228, 85).with_ignore_pointer_events().with_debug_name("gw_logo"));
    div(context, mk(root.ent(), 2), box(scale, 357, 24, 34, 34)
        .with_custom_background(navy).with_corner_radius(17 * scale));
    text(3, "1", 357, 23, 34, 34, 22, white, TextAlignment::Center, "AtkinsonMock");
    text(4, "Player 1 / Your turn", 406, 23, 380, 35, 21, navy);
    text(6, "Find the mystery person", 406, 57, 470, 25, 20, theme.font_muted, TextAlignment::Left, "AtkinsonMock");

    if (button(
            context, mk(root.ent(), 5),
            box(scale, 1066.f, 29.f, 170.f, 42.f)
                .with_label("New game")
                .with_font("AtkinsonMock", pixels(21.f * scale))
                .with_custom_background(afterhours::Color{255, 255, 255, 255})
                .with_border(white, 1.f * scale)
                .with_corner_radius(10.f * scale)
                .with_custom_text_color(navy)
                .with_alignment(TextAlignment::Center)
                .with_text_inset(0.f, 0.f)
                .with_on_draw_fg([scale, navy](RectangleType r) {
                  const float cx = r.x + 152.f * scale;
                  const float cy = r.y + 21.f * scale;
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

    div(context, mk(root.ent(), 10), box(scale, 40, 94, 1200, 76)
        .with_custom_background({255, 255, 255, 255}).with_corner_radius(12 * scale).with_debug_name("gw_question"));
    text(11, "Does your person", 58, 98, 280, 24, 17, navy);
    text(16, "Answer", 364, 98, 100, 24, 17, navy, TextAlignment::Left, "AtkinsonMock");
    pick(12, attributes, attribute_index, 58, 126, 282, 36, 22, white, navy, "gw_trait");
    pick(13, answers, answer_index, 364, 126, 100, 36, 22, white, navy, "gw_answer");
    text(17, "Enter the other player's answer.\nAsk to remove faces that do not match.",
         727, 111, 332, 49, 16, theme.font_muted, TextAlignment::Left, "AtkinsonMock");
    if (button(context, mk(root.ent(), 14),
               box(scale, 488.f, 120.f, 208.f, 44.f)
                   .with_label("Ask question")
                   .with_font("AtkinsonMock", pixels(22.f * scale))
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
                     const float x = r.x + 187.f * scale;
                     const float cy = r.y + 22.f * scale;
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
    text(15, fmt::format("{} faces left", faces_left()), 1070, 99, 152, 31, 22, navy, TextAlignment::Right);
    text(18, "Board status", 1070, 132, 152, 24, 16, theme.font_muted, TextAlignment::Right, "AtkinsonMock");
    div(context, mk(root.ent(), 19), box(scale, 1080, 160, 136.f * static_cast<float>(faces_left()) / kCount, 3)
        .with_custom_background(navy).with_ignore_pointer_events());
    std::string history = "Clues appear here after you ask.";
    if (!clue_history.empty()) {
      history.clear();
      for (const auto &clue : clue_history) {
        if (!history.empty()) history += "   /   ";
        history += clue;
      }
    }
    text(21, "Recent clues", 45, 173, 126, 23, 17, navy, TextAlignment::Left, "AtkinsonMock");
    text(22, history, 180, 173, 1050, 23, 17, theme.font_muted, TextAlignment::Left, "AtkinsonMock");
    div(context, mk(root.ent(), 20), box(scale, 40, 198, 1200, 484)
        .with_on_draw_bg([scale](RectangleType r) { draw_board(r, scale); }).with_debug_name("gw_board"));
    for (int row = 0; row < kRows; ++row)
      text(30 + row, std::string(1, static_cast<char>('A' + row)), 39, 248 + row * 160.f,
           17, 25, 17, white, TextAlignment::Center, "AtkinsonMock");

    for (int index = 0; index < kCount; ++index) {
      const int row = index / kCols;
      const int col = index % kCols;
      const float x = 56.f + static_cast<float>(col) * 148.f;
      const float y = 210.f + static_cast<float>(row) * 160.f;
      const bool eliminated = down[index];
      const bool candidate = note_index[index] == 3;
      const auto face_button =
          button(context, mk(root.ent(), 100 + index),
                 box(scale, x, y, 132.f, 112.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([eliminated, candidate, scale](RectangleType r) {
                       draw_portrait_panel(r, eliminated, scale, candidate);
                     })
                     .with_debug_name("gw_face_" + std::to_string(index)));

      const float source_x = static_cast<float>(col) * 256.f;
      const float source_y = static_cast<float>(row) * 168.f;
      div(context, mk(root.ent(), 200 + index),
          box(scale, x, y + 3.f, 132.f, 86.f)
              .with_on_draw_fg([texture = portrait_texture, source_x, source_y,
                                eliminated](RectangleType r) {
                RectangleType destination = r;
                raylib::DrawTexturePro(
                    texture, {source_x, source_y, 256.f, 168.f}, destination,
                    {0.f, 0.f}, 0.f,
                    eliminated ? raylib::Color{255, 255, 255, 45}
                               : raylib::WHITE);
              })
              .with_ignore_pointer_events());
      div(context, mk(root.ent(), 300 + index),
          box(scale, x + 3.f, y + 92.f, 126.f, 20.f)
              .with_custom_background(
                  eliminated ? afterhours::Color{86, 110, 125, 255}
                             : afterhours::Color{255, 251, 240, 255})
              .with_ignore_pointer_events());
      text(400 + index, eliminated ? std::string(face(index).name) + " / out" : face(index).name, x + 3.f, y + 91.f, 126.f, 22.f,
           18.f,
           eliminated ? afterhours::Color{214, 226, 232, 255}
                      : afterhours::Color{36, 46, 55, 255},
           TextAlignment::Center);
      if (face_button) {
        down[index] = !down[index];
        feedback = down[index] ? std::string(face(index).name) + " flipped down"
                               : std::string(face(index).name) + " restored";
      }

      pick(500 + index, notes, note_index[index], x, y + 116, 132, 24, 17,
           {48, 83, 105, 255}, {239, 246, 251, 255}, "gw_note_" + std::to_string(index));
    }

    text(700, "Click a face to flip. Notes are reminders, not eliminations.", 40, 690, 650, 25,
         17, theme.font_muted, TextAlignment::Left, "AtkinsonMock");
    text(701, feedback, 708, 690, 526, 25, 17, navy, TextAlignment::Right, "AtkinsonMock");
  }
};

REGISTER_EXAMPLE_SCREEN(guess_who_lab, "Stress Tests",
                        "26 dropdowns playing guess who, with drawn faces",
                        GuessWhoLab)
