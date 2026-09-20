#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MotionButtonCheckpoint : ScreenSystem<UIContext<InputAction>> {
  int clicks = 0;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    namespace motion = afterhours::motion;
    context.theme = afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{55, 50, 60, 255}, muted{99, 87, 105, 255}, coral{255, 115, 105, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({255, 245, 235, 255}).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h, float size, bool dim = false) {
      return div(context, mk(entity, id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(dim ? muted : ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(1, "Motion checkpoint", 80, 60, 700, 44, 32);
    label(2, "One button on the new core: appear, hover, press, focus, reversal, instant, teardown.", 80, 110, 1100, 30, 20, true);

    auto btn = button(context, mk(entity, 10),
                      box(440, 300, 400, 80).with_label("Hover, press, focus me")
                          .with_custom_background(coral).with_font("AtkinsonMock", pixels(24 * s))
                          .with_custom_text_color(ink).with_corner_radius(16 * s).with_debug_name("motion_btn")
                          .on_appear({.translate_y = {24.f * s, 0.f}, .opacity = {0.f, 1.f}}, motion::Spring::gentle(), 0.1f)
                          .on_focus({.scale = 1.03f})
                          .on_hover({.scale = 1.05f, .translate_y = -4.f * s})
                          .on_press({.scale = 0.92f}));
    if (btn) ++clicks;

    float scale = 1.f, ty = 0.f, opacity = 1.f;
    bool in_flight = false;
    afterhours::Entity &be = btn.ent();
    if (be.has<motion::HasTracks>()) {
      auto &tracks = be.get<motion::HasTracks>();
      const auto read = [&](MotionProperty p, float &into) {
        auto it = tracks.floats.find(static_cast<size_t>(p));
        if (it == tracks.floats.end()) return;
        into = it->second.value();
        in_flight = in_flight || it->second.active();
      };
      read(MotionProperty::Scale, scale);
      read(MotionProperty::TranslateY, ty);
      read(MotionProperty::Opacity, opacity);
    }
    const auto pct = [](float v) { return std::to_string(static_cast<int>(std::lround(v * 100.f))) + "%"; };
    label(20, "scale: " + pct(scale), 80, 460, 400, 30, 22);
    label(21, "y: " + std::to_string(static_cast<int>(std::lround(ty / s))) + "px", 80, 496, 400, 30, 22);
    label(22, "opacity: " + pct(opacity), 80, 532, 400, 30, 22);
    label(23, std::string("motion: ") + (in_flight ? "in flight" : "settled"), 80, 568, 400, 30, 22);
    label(24, std::string("hover: ") + (context.was_hot(be.id) ? "yes" : "no"), 500, 460, 400, 30, 22);
    label(25, std::string("pressed: ") + (context.was_active(be.id) ? "yes" : "no"), 500, 496, 400, 30, 22);
    label(26, std::string("focus: ") + (context.has_focus(be.id) ? "yes" : "no"), 500, 532, 400, 30, 22);
    label(27, "clicks: " + std::to_string(clicks), 500, 568, 400, 30, 22);
  }
};

REGISTER_EXAMPLE_SCREEN(motion_button, "Animations",
                        "First checkpoint for the motion rewrite: one button",
                        MotionButtonCheckpoint)
