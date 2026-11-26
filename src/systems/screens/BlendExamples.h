#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct BlendExamples : ScreenSystem<UIContext<InputAction>> {
  struct Card {
    const char *name;
    const char *formula;
    const char *result;
    afterhours::blend::Mode mode;
    bool premultiply_rgb;
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    namespace blend = afterhours::blend;

    static constexpr std::array<afterhours::Color, 3> inputs{{
        {230, 60, 60, 170}, {60, 205, 110, 170}, {70, 120, 240, 170}}};
    static constexpr std::array<Card, 4> cards{{
        {"Alpha", "S*a + D*(1-a)", "Top circle wins, softened.",
         blend::Mode::Alpha, false},
        {"Additive", "S*a + D", "Overlaps add light, run to white.",
         blend::Mode::Additive, false},
        {"Multiplied", "S*D + D*(1-a)", "Overlaps multiply, darken.",
         blend::Mode::Multiplied, false},
        {"AlphaPremultiply", "S + D*(1-a)", "Pre-scaled RGB, matches Alpha.",
         blend::Mode::AlphaPremultiply, true},
    }};

    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float left = (context.screen_width - 1144.f * s) / 2.f;
    const float top = (context.screen_height - 720.f * s) / 2.f;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{178, 190, 211, 255};
    const auto border = afterhours::Color{70, 84, 106, 255};
    const auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(left + x * s, top + y * s)
          .with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size({pixels(context.screen_width),
                                    pixels(context.screen_height)})
                        .with_corner_radius(0)
                        .with_custom_background({19, 23, 32, 255})
                        .with_debug_name("blend_examples_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float h, float size, afterhours::Color color,
                           const std::string &name = "") {
      div(context, mk(root.ent(), id++),
          box(x, y, w, h)
              .with_label(text)
              .with_font("AtkinsonMock", pixels(size * s))
              .with_custom_text_color(color)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events()
              .with_debug_name(name));
    };

    label("Blend mode examples", 0, 28, 700, 44, 34, white,
          "blend_examples_title");
    label("Same three translucent circles in every card. Only the blend mode "
          "changes.",
          0, 78, 1000, 28, 20, muted);

    static constexpr std::array<const char *, 3> input_names{
        {"1 red", "2 green", "3 blue"}};
    for (size_t i = 0; i < inputs.size(); ++i) {
      auto chip = inputs[i];
      chip.a = 255;
      div(context, mk(root.ent(), id++),
          box(static_cast<float>(i) * 150.f, 122, 18, 18)
              .with_custom_background(chip)
              .with_ignore_pointer_events());
      label(input_names[i], static_cast<float>(i) * 150.f + 26, 118, 120, 26,
            19, white);
    }
    label("Alpha 170 / 255    Draw order: 1, then 2, then 3    Backing RGB "
          "160, 165, 175",
          470, 118, 674, 26, 19, muted);

    for (size_t c = 0; c < cards.size(); ++c) {
      const auto &card = cards[c];
      const float x = static_cast<float>(c) * 290.f;
      div(context, mk(root.ent(), id++),
          box(x, 168, 274, 330)
              .with_custom_background({32, 38, 51, 255})
              .with_border(border, s)
              .with_corner_radius(10 * s)
              .with_debug_name(fmt::format("blend_examples_card_{}", c)));
      label(card.name, x + 14, 180, 246, 30, c == 3 ? 22.f : 24.f, white,
            fmt::format("blend_examples_mode_{}", c));
      label(card.formula, x + 14, 213, 246, 24, 18, muted);

      const float stage_x = x + 12;
      const float stage_y = 248;
      div(context, mk(root.ent(), id++),
          box(stage_x, stage_y, 250, 170)
              .with_custom_background({160, 165, 175, 255})
              .with_corner_radius(6 * s)
              .with_ignore_pointer_events()
              .with_debug_name(fmt::format("blend_examples_stage_{}", c)));

      static constexpr std::array<float, 3> dx{{18.f, 118.f, 68.f}};
      static constexpr std::array<float, 3> dy{{12.f, 12.f, 58.f}};
      for (size_t i = 0; i < inputs.size(); ++i) {
        auto color = inputs[i];
        if (card.premultiply_rgb) {
          const float a = color.a / 255.f;
          color.r = static_cast<unsigned char>(std::round(color.r * a));
          color.g = static_cast<unsigned char>(std::round(color.g * a));
          color.b = static_cast<unsigned char>(std::round(color.b * a));
        }
        div(context, mk(root.ent(), id++),
            box(stage_x + dx[i], stage_y + dy[i], 96, 96)
                .with_custom_background(color)
                .with_corner_radius(48 * s)
                .with_blend_mode(card.mode)
                .with_ignore_pointer_events()
                .with_debug_name(
                    fmt::format("blend_examples_circle_{}_{}", c, i)));
      }
      label(card.result, x + 14, 432, 246, 48, 18, white);    }

    label("AlphaPremultiply takes RGB already multiplied by alpha. Feeding it "
          "straight RGB double-counts the alpha.",
          0, 524, 1144, 28, 19, muted, "blend_examples_note");
    label("Each circle is one div with with_blend_mode. No custom draw code.",
          0, 556, 1144, 28, 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(blend_examples, "System Demos",
                        "with_blend_mode on widgets: Alpha, Additive, "
                        "Multiplied, AlphaPremultiply",
                        BlendExamples)
