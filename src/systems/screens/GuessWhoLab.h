#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Twenty four dropdowns on one board, plus two driving the question row. A
// dropdown opens an overlay that has to sit above every sibling drawn after
// it, so a wall of them is the honest test of that.
struct GuessWhoLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kCols = 8;
  static constexpr int kRows = 3;
  static constexpr int kCount = kCols * kRows;

  struct Face {
    const char *name;
    int hair;    // 0 dark, 1 fair, 2 red
    int eyes;    // 0 brown, 1 blue
    bool glasses;
    bool hat;
  };

  // Fixed, so the board and its baseline are the same every run.
  static const Face &face(int i) {
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
    return faces[i];
  }

  std::vector<std::string> attributes = {"hair is dark", "hair is fair",
                                         "hair is red", "eyes are blue",
                                         "wears glasses", "wears a hat"};
  std::vector<std::string> answers = {"yes", "no"};
  std::vector<std::string> notes = {"?", "maybe", "no", "THIS ONE"};

  size_t attribute_idx = 4;
  size_t answer_idx = 0;
  size_t note_idx[kCount]{};
  bool down[kCount]{};
  int asked = 0;
  std::string last_question = "Pick a trait, answer yes or no, then Ask.";

  static bool matches(const Face &f, size_t attr) {
    switch (attr) {
    case 0: return f.hair == 0;
    case 1: return f.hair == 1;
    case 2: return f.hair == 2;
    case 3: return f.eyes == 1;
    case 4: return f.glasses;
    default: return f.hat;
    }
  }

  int standing() const {
    int n = 0;
    for (int i = 0; i < kCount; i++)
      n += down[i] ? 0 : 1;
    return n;
  }

  void ask() {
    const bool want = answer_idx == 0;
    for (int i = 0; i < kCount; i++)
      if (matches(face(i), attribute_idx) != want)
        down[i] = true;
    asked++;
    last_question = fmt::format("Q{}: {} -> {}. {} left standing.", asked,
                                attributes[attribute_idx],
                                answers[answer_idx], standing());
  }

  void reset() {
    for (int i = 0; i < kCount; i++) {
      down[i] = false;
      note_idx[i] = 0;
    }
    asked = 0;
    last_question = "Board reset. Ask again.";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.roundness = 0.14f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(18.0f));

    auto root = vstack(context, mk(entity),
                       ComponentConfig{}
                           .with_size(ComponentSize{screen_pct(0.97f),
                                                    screen_pct(0.97f)})
                           .with_self_align(SelfAlign::Center)
                           .with_background(Theme::Usage::Background)
                           .with_padding(Spacing::sm)
                           .with_no_wrap()
                           .with_debug_name("gw_root"));

    // ---- question row -------------------------------------------------
    auto bar = hstack(context, mk(root.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), pixels(56)})
                          .with_align_items(AlignItems::Center)
                          .with_custom_background(afterhours::Color{34, 38, 52, 255})
                          .with_padding(Spacing::xs)
                          .with_corner_radius(10.f)
                          .with_no_wrap()
                          .with_debug_name("gw_bar"));

    div(context, mk(bar.ent(), 0),
        ComponentConfig{}
            .with_label("Ask:")
            .with_size(ComponentSize{pixels(56), pixels(40)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{200, 210, 230, 255})
            .with_font_size(pixels(20.f)));

    dropdown(context, mk(bar.ent(), 1), attributes, attribute_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(230), pixels(40)})
                 .with_font_size(pixels(17.f))
                 .with_debug_name("gw_attr"));

    dropdown(context, mk(bar.ent(), 2), answers, answer_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(110), pixels(40)})
                 .with_font_size(pixels(17.f))
                 .with_debug_name("gw_answer"));

    if (button(context, mk(bar.ent(), 3),
               ComponentConfig{}
                   .with_label("Ask")
                   .with_size(ComponentSize{pixels(96), pixels(40)})
                   .with_click_activation(ClickActivationMode::Release)
                   .with_corner_radius(8.f)
                   .with_debug_name("gw_ask"))) {
      ask();
    }

    // Pushes the reset button to the far edge without a magic-width filler.
    spacer(context, mk(bar.ent(), 4));

    div(context, mk(bar.ent(), 5),
        ComponentConfig{}
            .with_label(fmt::format("{} standing", standing()))
            .with_size(ComponentSize{pixels(150), pixels(40)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{150, 220, 170, 255})
            .with_font_size(pixels(19.f))
            .with_debug_name("gw_standing"));

    if (button(context, mk(bar.ent(), 6),
               ComponentConfig{}
                   .with_label("Reset")
                   .with_size(ComponentSize{pixels(96), pixels(40)})
                   .with_click_activation(ClickActivationMode::Release)
                   .with_corner_radius(8.f)
                   .with_debug_name("gw_reset"))) {
      reset();
    }

    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_label(last_question)
            .with_size(ComponentSize{percent(1.f), pixels(34)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{190, 198, 218, 255})
            .with_font_size(pixels(17.f))
            .with_debug_name("gw_log"));

    // ---- the wall -----------------------------------------------------
    auto wall = vstack(context, mk(root.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_no_wrap()
                           .with_debug_name("gw_wall"));

    for (int r = 0; r < kRows; r++) {
      auto row = hstack(context, mk(wall.ent(), r),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), percent(0.333f)})
                            .with_no_wrap()
                            .with_debug_name(fmt::format("gw_row_{}", r)));

      for (int c = 0; c < kCols; c++) {
        const int i = r * kCols + c;
        const Face &f = face(i);
        const bool out = down[i];

        const afterhours::Color card_bg =
            out ? afterhours::Color{28, 30, 38, 255}
                : afterhours::Color{48, 54, 72, 255};
        const afterhours::Color name_fg =
            out ? afterhours::Color{92, 98, 112, 255}
                : afterhours::Color{235, 240, 250, 255};

        auto card = vstack(
            context, mk(row.ent(), c),
            ComponentConfig{}
                .with_size(ComponentSize{expand(), percent(0.94f)})
                .with_custom_background(card_bg)
                .with_padding(Spacing::xs)
                .with_margin(Margin{.right = pixels(5)})
                .with_corner_radius(9.f)
                .with_min_width(pixels(120))
                .with_max_width(pixels(180))
                .with_shadow(ShadowStyle::Soft, 0.f, 3.f)
                .with_clip_children(true)
                .with_align_items(AlignItems::Center)
                .with_no_wrap()
                .with_debug_name(fmt::format("gw_card_{}", i)));

        div(context, mk(card.ent(), 0),
            ComponentConfig{}
                .with_label(f.name)
                .with_size(ComponentSize{percent(1.f), pixels(30)})
                .with_background(Theme::Usage::None)
                .with_custom_text_color(name_fg)
                .with_font_size(pixels(21.f))
                .with_alignment(TextAlignment::Center));

        // The face, drawn rather than assembled from widgets: three shapes a
        // card does not need three entities for.
        const int hair = f.hair;
        const bool glasses = f.glasses;
        const bool hat = f.hat;
        div(context, mk(card.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.f), expand()})
                .with_background(Theme::Usage::None)
                .with_on_draw_fg([hair, glasses, hat, out](RectangleType rr) {
                  const float cx = rr.x + rr.width * 0.5f;
                  const float cy = rr.y + rr.height * 0.48f;
                  const float rad = std::min(rr.width, rr.height) * 0.30f;
                  const unsigned char a = out ? 90 : 255;
                  const afterhours::Color skin{214, 176, 140, a};
                  const afterhours::Color hairc =
                      hair == 0 ? afterhours::Color{60, 44, 38, a}
                      : hair == 1 ? afterhours::Color{224, 196, 118, a}
                                  : afterhours::Color{196, 92, 52, a};
                  afterhours::draw_circle_v({cx, cy}, rad, skin);
                  afterhours::draw_rectangle(
                      {cx - rad, cy - rad * 1.15f, rad * 2.f, rad * 0.72f},
                      hairc);
                  if (hat)
                    afterhours::draw_rectangle(
                        {cx - rad * 1.15f, cy - rad * 1.62f, rad * 2.3f,
                         rad * 0.5f},
                        afterhours::Color{70, 120, 190, a});
                  if (glasses) {
                    const afterhours::Color lens{240, 240, 250, a};
                    afterhours::draw_circle_v({cx - rad * 0.42f, cy}, rad * 0.26f, lens);
                    afterhours::draw_circle_v({cx + rad * 0.42f, cy}, rad * 0.26f, lens);
                  }
                }));

        dropdown(context, mk(card.ent(), 2), notes, note_idx[i],
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(30)})
                     .with_font_size(pixels(15.f))
                     .with_debug_name(fmt::format("gw_note_{}", i)));
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(guess_who_lab, "Stress Tests",
                        "26 dropdowns playing guess who, with drawn faces",
                        GuessWhoLab)
