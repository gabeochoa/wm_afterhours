#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_unit_motion.h>
#include <array>
#include <cstdio>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextMotionLab : ScreenSystem<UIContext<InputAction>> {
  struct Row {
    const char *name;
    const char *text;
    TextUnitMotion cfg;
  };
  static constexpr std::array<Row, 9> rows{{
      {"Char rise", "Letters rise and fade in", {.unit = TextUnit::Char, .from_y = 10.f, .stagger = 0.03f}},
      {"Char slide", "Letters slide from the left", {.unit = TextUnit::Char, .from_x = -14.f, .from_y = 0.f, .stagger = 0.03f}},
      {"Char scale", "Letters grow into place", {.unit = TextUnit::Char, .from_y = 0.f, .from_scale = 0.4f, .stagger = 0.03f}},
      {"Word rise", "Words rise with a stagger", {.unit = TextUnit::Word, .from_y = 8.f, .duration = 0.45f, .stagger = 0.07f}},
      {"Word fade", "Words fade in word by word", {.unit = TextUnit::Word, .from_y = 0.f, .duration = 0.5f, .stagger = 0.12f}},
      {"Word scale", "Words scale in one by one", {.unit = TextUnit::Word, .from_y = 0.f, .from_scale = 0.6f, .stagger = 0.09f}},
      {"Word reverse", "Words arrive in reverse order", {.unit = TextUnit::Word, .from_y = 8.f, .stagger = 0.07f, .order = StaggerOrder::Reverse}},
      {"Char centre-out", "Letters open from the centre", {.unit = TextUnit::Char, .from_y = 10.f, .stagger = 0.04f, .order = StaggerOrder::CenterOut}},
      {"Word random", "Words land in random order", {.unit = TextUnit::Word, .from_y = 8.f, .duration = 0.5f, .stagger = 0.09f, .order = StaggerOrder::Random}},
  }};
  std::array<afterhours::Entity *, 9> row_entity{};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme = theme_presets::midnight();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{236, 238, 245, 255}, muted{148, 154, 168, 255}, panel{43, 47, 60, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float size, bool dim = false,
                           const std::string &debug = "") {
      return div(context, mk(entity, id), box(x, y, w, size * 1.5f).with_label(text)
                                              .with_font("AtkinsonMock", pixels(size * s))
                                              .with_custom_text_color(dim ? muted : ink)
                                              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
                                              .with_ignore_pointer_events().with_debug_name(debug));
    };

    div(context, mk(entity, 0), box(0, 0, 1280, 720).with_custom_background({22, 24, 31, 255}).with_debug_name("bg"));
    label(1, "Text motion: per-unit batching", 60, 30, 700, 28, false, "title");
    label(2, "Char and Word units: rise, slide, fade, scale. Forward, Reverse, CentreOut and Random staggers. One TextUnits command per label.", 60, 66, 1000, 15, true);
    if (button(context, mk(entity, 3), box(1080, 26, 140, 40).with_label("Reset all")
                                           .with_font("AtkinsonMock", pixels(16 * s)).with_custom_text_color(ink)
                                           .with_custom_background(panel).with_corner_radius(8 * s)
                                           .with_debug_name("reset_all_btn")))
      for (afterhours::Entity *e : row_entity)
        if (e)
          restart_text_units(*e);

    for (size_t i = 0; i < rows.size(); ++i) {
      const Row &row = rows[i];
      const float y = 118.f + float(i) * 62.f;
      const int base = 100 + int(i) * 10;
      label(base, row.name, 60, y + 8, 180, 16, true, "row_name_" + std::to_string(i));
      auto result = div(context, mk(entity, base + 1),
                        box(250, y, 740, 40).with_label(row.text).with_font("AtkinsonMock", pixels(19 * s))
                            .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                            .with_background(Theme::Usage::None).with_unit_motion(row.cfg)
                            .with_debug_name("row_text_" + std::to_string(i)));
      row_entity[i] = &result.ent();
      label(base + 2, text_units_active(result.ent()) ? "in flight" : "settled", 1000, y + 8, 110, 15, true,
            "row_state_" + std::to_string(i));
      if (button(context, mk(entity, base + 3), box(1130, y, 90, 36).with_label("Reset")
                                                    .with_font("AtkinsonMock", pixels(15 * s)).with_custom_text_color(ink)
                                                    .with_custom_background(panel).with_corner_radius(8 * s)
                                                    .with_debug_name("reset_" + std::to_string(i))))
        restart_text_units(result.ent());
    }
    label(900, "Reset returns a row to the beginning of its transition: every unit restarts from its from values with the row stagger.", 60, 668, 1100, 14, true);
  }
};

REGISTER_EXAMPLE_SCREEN(text_motion_lab, "Animations",
                        "Text motion lab: every word and text animation, batched per unit",
                        TextMotionLab)
