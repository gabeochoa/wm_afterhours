#pragma once

#include "../../external.h"
#include "../../frame_stats.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation_presets/effect_presets.h>
#include <afterhours/src/plugins/ui/text_unit_motion.h>
#include <afterhours/src/plugins/ui_motion.h>
#include <cmath>

namespace um = afterhours::ui_motion;
using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AnimationStressLab : ScreenSystem<UIContext<InputAction>> {
  enum struct Key : size_t { Phase };

  FrameStats stats;
  float itemsSlider = 0.45f;
  float phaseClock = 0.f;
  bool springPhase = false;
  bool paused = false;
  bool reduced = false;
  size_t replayStamp = 0;
  size_t seenReplay = 0;
  bool started = false;

  int items() const { return 4 + static_cast<int>(std::lround(itemsSlider * 28.f)); }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    namespace motion = afterhours::motion;
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    if (!paused) {
      stats.push(dt);
      phaseClock += dt;
      if (phaseClock > 1.6f) {
        phaseClock = 0.f;
        springPhase = !springPhase;
      }
    }
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{232, 238, 248, 255}, muted{160, 172, 196, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
                        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
                        .with_custom_background({16, 20, 30, 255}).with_corner_radius(0)
                        .with_debug_name("anim_stress_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float size,
                           bool dim = false, const std::string &debug = "") {
      return div(context, mk(root.ent(), id++),
                 box(x, y, w, size * 1.4f).with_label(text)
                     .with_font("AtkinsonMock", pixels(size * s))
                     .with_custom_text_color(dim ? muted : ink)
                     .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
                     .with_ignore_pointer_events().with_debug_name(debug));
    };
    const auto control = [&](const std::string &text, float x, float y, float w,
                             const std::string &debug, bool primary = false) {
      return button(context, mk(root.ent(), id++),
                    box(x, y, w, 40).with_label(text)
                        .with_font("AtkinsonMock", pixels(17 * s)).with_corner_radius(8 * s)
                        .with_custom_background(primary ? afterhours::Color{0, 105, 98, 255}
                                                        : afterhours::Color{44, 54, 74, 255})
                        .with_custom_text_color(ink).with_debug_name(debug));
    };

    if (!started) {
      started = true;
      const motion::Timeline loop{.keys = {{0.f, 0.f}, {1.2f, 1.f}},
                                  .repeat = motion::Timeline::Repeat::PingPong,
                                  .curve = motion::curves::ease_in_out_quad};
      motion::anim(Key::Phase).from(0.f).to(1.f, loop);
    }
    motion::set_instant(reduced);
    const float phase = paused ? 0.5f : motion::anim(Key::Phase).value_or(0.f);
    const int count = items();

    label("Animation stress: every new animation type, at scale", 48, 28, 800, 26, false,
          "anim_stress_title");
    label(fmt::format("frame {:.1f} fps  avg {:.2f} ms  p95 {:.2f} ms  max {:.2f} ms  items/row "
                      "{}  widgets ~{}",
                      stats.fps, stats.avgMs, stats.p95Ms, stats.maxMs, count, count * 6),
          48, 66, 900, 16, true, "anim_stress_stats");
    label("Items per row", 950, 40, 140, 14, true);
    slider(context, mk(root.ent(), id++), itemsSlider,
           box(950, 62, 220, 28).with_debug_name("anim_stress_items"));
    if (control(paused ? "Resume" : "Pause", 950, 100, 104, "anim_stress_pause"))
      paused = !paused;
    if (control("Restart", 1062, 100, 108, "anim_stress_restart", true))
      ++replayStamp;
    if (control(reduced ? "Reduced: on" : "Reduced: off", 950, 148, 220, "anim_stress_reduced"))
      reduced = !reduced;

    const float rowX = 210.f, gap = 5.f;
    const float itemW = std::min(30.f, (1224.f - rowX) / static_cast<float>(count) - gap);
    const auto rowLabel = [&](const char *text, float y) {
      label(text, 48, y + 8, 156, 14, true);
    };
    const auto tri = [](float p) { return p < 0.5f ? p * 2.f : 2.f - p * 2.f; };

    rowLabel("OKLab colour", 168);
    for (int i = 0; i < count; ++i) {
      const float t = tri(std::fmod(phase + static_cast<float>(i) * 0.06f, 1.f));
      const auto a = motion::Components<ColorType>::to(ColorType{255, 60, 60, 255});
      const auto b = motion::Components<ColorType>::to(ColorType{40, 90, 255, 255});
      std::array<float, 4> mix{};
      for (size_t k = 0; k < 4; ++k)
        mix[k] = a[k] + (b[k] - a[k]) * t;
      div(context, mk(root.ent(), id++),
          box(rowX + i * (itemW + gap), 168, itemW, 40)
              .with_custom_background(motion::Components<ColorType>::from(mix))
              .with_corner_radius(5 * s).with_ignore_pointer_events());
    }

    rowLabel("Text units", 224);
    for (int i = 0; i < count; ++i) {
      auto cell = div(context, mk(root.ent(), id++),
                      box(rowX + i * (itemW + gap), 224, itemW, 40)
                          .with_label("Ab").with_font("AtkinsonMock", pixels(17 * s))
                          .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                          .with_background(Theme::Usage::None).with_ignore_pointer_events()
                          .with_unit_motion(TextUnitMotion{.unit = TextUnit::Char,
                                                           .from_y = 10.f,
                                                           .duration = 0.4f,
                                                           .stagger = 0.05f}));
      if (seenReplay != replayStamp)
        restart_text_units(cell.ent());
    }
    if (seenReplay != replayStamp)
      seenReplay = replayStamp;

    rowLabel("Effect quads", 280);
    for (int i = 0; i < count; ++i) {
      const auto &preset = afterhours::presets::effect_presets[static_cast<size_t>(i) % 17];
      const float t = tri(std::fmod(phase + static_cast<float>(i) * 0.05f, 1.f));
      const float x = rowX + i * (itemW + gap);
      div(context, mk(root.ent(), id++),
          box(x, 280, itemW, 40).with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_on_draw_fg([preset, t](RectangleType r) {
                const auto q = afterhours::presets::effect_quad(preset, r, t);
                afterhours::draw_quad(q.corners, {80, 140, 220,
                                                  static_cast<unsigned char>(255.f * q.opacity)});
              }));
    }

    rowLabel("Blur radius", 336);
    for (int i = 0; i < count; ++i) {
      const auto &preset = afterhours::presets::effect_presets[0];
      const float t = tri(std::fmod(phase + static_cast<float>(i) * 0.05f, 1.f));
      div(context, mk(root.ent(), id++),
          box(rowX + i * (itemW + gap), 336, itemW, 40)
              .with_custom_background(afterhours::Color{52, 68, 96, 255})
              .with_corner_radius(5 * s)
              .with_blur(afterhours::presets::effect_blur_radius(preset, t))
              .with_ignore_pointer_events());
    }

    rowLabel("Relayout", 392);
    for (int i = 0; i < count; ++i) {
      const float w = 14.f + tri(std::fmod(phase + static_cast<float>(i) * 0.08f, 1.f)) * 18.f;
      div(context, mk(root.ent(), id++),
          box(rowX + i * (itemW + gap), 392, w, 40)
              .with_custom_background(afterhours::Color{46, 130, 118, 255})
              .with_corner_radius(5 * s).with_ignore_pointer_events());
    }

    rowLabel("ui_motion springs", 448);
    for (int i = 0; i < count; ++i) {
      div(context, mk(root.ent(), id++),
          box(rowX + i * (itemW + gap), 448, itemW, 40)
              .with_custom_background(afterhours::Color{96, 64, 120, 255})
              .with_corner_radius(5 * s).with_ignore_pointer_events()
              .with(um::on_state(springPhase,
                                 {.translate_x = {0.f, 14.f * s}, .rotation = {0.f, 8.f}},
                                 motion::Spring::bouncy())));
    }

    label("Rows: OKLab colour conversion / char text-unit batching / effect-quad corners / blur "
          "passes / width relayout / ui_motion spring state. Restart replays text units; Reduced "
          "makes every track instant.",
          48, 520, 1180, 15, true);
    label(paused ? "paused" : "running", 48, 552, 200, 15, true, "anim_stress_state");
  }
};

REGISTER_EXAMPLE_SCREEN(animation_stress_lab, "Animations",
                        "Stress: all new animation types at scale", AnimationStressLab)
