#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <algorithm>

struct TypographyCompositionLab
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  bool reduced_motion = false;
  float elapsed = 1.f;
  int replays = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context,
                     float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    context.theme = theme_presets::ocean_navy();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const afterhours::Color ink{25, 38, 56, 255};
    const afterhours::Color muted{77, 94, 113, 255};
    const afterhours::Color teal{0, 101, 94, 255};
    elapsed = reduced_motion ? 1.f : std::min(1.f, elapsed + std::max(0.f, dt));
    const auto box = [=](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position((left + x) * s, (top + y) * s)
          .with_corner_radius(0).with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({240, 244, 247, 255}).with_corner_radius(0)
        .with_debug_name("type_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float h, float size, afterhours::Color color,
                           const std::string &name, bool bold = false) {
      return div(context, mk(root.ent(), id++), box(x, y, w, h)
          .with_label(text).with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label("Words with room to play", 0, 26, 1144, 48, 36, ink, "type_title", true);
    label("A bold arrangement. The same clear message.", 0, 82, 1144, 32,
          22, muted, "type_subtitle");
    div(context, mk(root.ent(), id++), box(0, 138, 1144, 350)
        .with_custom_background({225, 235, 229, 255})
        .with_debug_name("type_composition"));
    label("A FRESH PERSPECTIVE", 36, 158, 650, 28, 18, teal, "type_overline");
    label("Make waves", 36, 508, 660, 38, 27, ink, "type_reading_order", true);
    label("Big ideas deserve a little space. Start with yours.", 36, 550,
          1072, 32, 22, muted, "type_description");
    if (button(context, mk(root.ent(), id++), box(0, 610, 180, 52)
        .with_label("Replay").with_font("AtkinsonMockBold", pixels(22 * s))
        .with_custom_background(teal).with_custom_text_color({255, 255, 255, 255})
        .with_corner_radius(8 * s).with_debug_name("type_replay"))) {
      ++replays;
      elapsed = reduced_motion ? 1.f : 0.f;
    }
    if (button(context, mk(root.ent(), id++), box(204, 610, 300, 52)
        .with_label(reduced_motion ? "Reduced motion: on" : "Reduced motion: off")
        .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink)
        .with_custom_background({221, 229, 236, 255}).with_border(muted, s)
        .with_corner_radius(8 * s).with_debug_name("type_reduce_motion"))) {
      reduced_motion = !reduced_motion;
      if (reduced_motion) elapsed = 1.f;
    }
    const float progress = std::clamp(elapsed / .8f, 0.f, 1.f);
    const float first = 1.f - (1.f - progress) * (1.f - progress) * (1.f - progress);
    const float delayed = std::clamp((elapsed - .12f) / .8f, 0.f, 1.f);
    const float second = 1.f - (1.f - delayed) * (1.f - delayed) * (1.f - delayed);
    div(context, mk(root.ent(), id++), box(50 - 32 * (1.f - first), 202, 620, 154)
        .with_label("MAKE").with_font("AtkinsonMockBold", pixels(144 * s))
        .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
        .with_opacity(.4f + .6f * first).with_ignore_pointer_events()
        .with_debug_name("type_make"));
    div(context, mk(root.ent(), id++), box(292 + 32 * (1.f - second), 302 + 12 * (1.f - second), 810, 156)
        .with_label("WAVES").with_font("AtkinsonMockBold", pixels(144 * s))
        .with_custom_text_color(teal).with_alignment(TextAlignment::Left)
        .with_opacity(.4f + .6f * second).with_ignore_pointer_events()
        .with_debug_name("type_waves"));
    label(reduced_motion ? "Still and clear" : elapsed < 1.f ? "Playing" : "At rest",
          538, 614, 262, 42, 22, ink, "type_phase");
    label(fmt::format("Replays: {}", replays), 866, 614, 278, 42,
          20, muted, "type_replays");
    label("Replay any time. Reduced motion keeps the words in place.",
          0, 679, 1144, 25, 18, muted, "type_help");
  }
};

REGISTER_EXAMPLE_SCREEN(typography_composition, "System Demos",
                        "Expressive whole-word typography with optional motion",
                        TypographyCompositionLab)
