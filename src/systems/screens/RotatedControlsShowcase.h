#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

struct RotatedControlsShowcase
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int clicks_a = 0;
  int clicks_b = 0;
  int clicks_c = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    const afterhours::Color bg{244, 246, 248, 255};
    const afterhours::Color card{255, 255, 255, 255};
    const afterhours::Color border{206, 214, 222, 255};
    const afterhours::Color ink{26, 36, 48, 255};
    const afterhours::Color muted{84, 98, 114, 255};
    const afterhours::Color teal{25, 112, 96, 255};
    const afterhours::Color navy{34, 78, 134, 255};
    const afterhours::Color rust{176, 74, 48, 255};

    ctx.theme = Theme{};
    ctx.theme.background = bg;
    ctx.theme.surface = card;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = navy;
    ctx.theme.focus = navy;
    ctx.scaling_mode = ScalingMode::Proportional;

    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(
        ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(1280.f * s), pixels(720.f * s)})
            .with_absolute_position((ctx.screen_width - 1280.f * s) / 2.f,
                                    (ctx.screen_height - 720.f * s) / 2.f)
            .with_custom_background(bg)
            .with_corner_radius(0)
            .with_debug_name("rotated_page"));

    auto label = [&](int id, float x, float y, float w, float h, float pt,
                     const std::string &text, afterhours::Color color,
                     bool bold = false, const char *name = "") {
      div(ctx, mk(page.ent(), id),
          ComponentConfig{}
              .with_absolute_position(x * s, y * s)
              .with_size({pixels(w * s), pixels(h * s)})
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",
                         pixels(pt * s))
              .with_custom_text_color(color)
              .with_transparent_bg()
              .with_ignore_pointer_events()
              .with_debug_name(name));
    };

    label(1, 72, 40, 800, 52, 38, "Rotated controls & hit areas", ink, true);
    label(2, 72, 98, 960, 28, 19,
          "Clicks and focus rings follow each button's rotation and pivot.",
          muted);

    div(ctx, mk(page.ent(), 3),
        ComponentConfig{}
            .with_absolute_position(72 * s, 154 * s)
            .with_size({pixels(1136 * s), pixels(410 * s)})
            .with_custom_background(card)
            .with_border(border, pixels(1.f))
            .with_corner_radius(10.f * s)
            .with_ignore_pointer_events());

    auto tilted_btn = [&](int id, float x, float y, float deg,
                          afterhours::Color color, const char *text,
                          const char *name) {
      return button(
          ctx, mk(page.ent(), id),
          ComponentConfig{}
              .with_absolute_position(x * s, y * s)
              .with_size({pixels(240 * s), pixels(64 * s)})
              .with_rotation(deg)
              .with_label(text)
              .with_font("AtkinsonMockBold", pixels(21 * s))
              .with_custom_background(color)
              .with_custom_text_color(card)
              .with_corner_radius(8.f * s)
              .with_debug_name(name));
    };

    if (tilted_btn(10, 140, 290, -18.f, teal, "Tilt -18 deg", "rot_btn_a"))
      ++clicks_a;
    if (tilted_btn(11, 520, 290, 25.f, navy, "Tilt +25 deg", "rot_btn_b"))
      ++clicks_b;
    if (tilted_btn(12, 900, 290, -35.f, rust, "Tilt -35 deg", "rot_btn_c"))
      ++clicks_c;

    label(20, 140, 440, 260, 28, 18,
          "Clicks A: " + std::to_string(clicks_a), muted, false, "rot_count_a");
    label(21, 520, 440, 260, 28, 18,
          "Clicks B: " + std::to_string(clicks_b), muted, false, "rot_count_b");
    label(22, 900, 440, 260, 28, 18,
          "Clicks C: " + std::to_string(clicks_c), muted, false, "rot_count_c");
  }
};

REGISTER_EXAMPLE_SCREEN(rotated_controls, "Layout",
                        "Rotated buttons with matching hit areas and focus rings",
                        RotatedControlsShowcase)
