#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation_presets/effect_presets.h>
#include <afterhours/src/plugins/ui_motion.h>
#include <array>
#include <cstdio>

namespace um = afterhours::ui_motion;

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ColourTrackLab : ScreenSystem<UIContext<InputAction>> {
  enum struct Key : size_t { Progress, Alpha, Row0 };

  struct Pair {
    const char *name;
    ColorType from;
    ColorType to;
    const char *mid;
  };
  static constexpr std::array<Pair, 5> pairs{{
      {"Red to blue", {255, 0, 0, 255}, {0, 0, 255, 255}, "140, 83, 162"},
      {"Red to green", {255, 0, 0, 255}, {0, 255, 0, 255}, "208, 168, 0"},
      {"Yellow to cyan", {255, 255, 0, 255}, {0, 255, 255, 255}, "176, 255, 176"},
      {"Black to white", {0, 0, 0, 255}, {255, 255, 255, 255}, "99, 99, 99"},
      {"Magenta to lime", {255, 0, 255, 255}, {0, 255, 0, 255}, "198, 180, 180"},
  }};

  bool started = false;
  bool spring_on = false;
  size_t replay_stamp = 0;
  size_t seen_replay = 0;

  void restart() {
    namespace motion = afterhours::motion;
    const motion::Timeline sweep{.keys = {{0.f, 0.f}, {2.f, 1.f}},
                                 .repeat = motion::Timeline::Repeat::PingPong,
                                 .curve = motion::curves::ease_in_out_quad};
    motion::anim(Key::Progress).from(0.f).to(1.f, sweep);
    for (size_t i = 0; i < pairs.size(); ++i)
      motion::anim<ColorType>(static_cast<Key>(static_cast<size_t>(Key::Row0) + i))
          .from(pairs[i].from)
          .to(pairs[i].to, sweep);
    motion::anim<ColorType>(Key::Alpha)
        .from(ColorType{255, 115, 105, 255})
        .to(ColorType{255, 115, 105, 60}, sweep);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    namespace motion = afterhours::motion;
    context.theme = afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{55, 50, 60, 255}, muted{99, 87, 105, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float size, bool dim = false,
                           const std::string &debug = "") {
      return div(context, mk(entity, id), box(x, y, w, size * 1.4f).with_label(text)
                                              .with_font("AtkinsonMock", pixels(size * s))
                                              .with_custom_text_color(dim ? muted : ink)
                                              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
                                              .with_ignore_pointer_events().with_debug_name(debug));
    };
    const auto swatch = [&](int id, float x, float y, ColorType c) {
      return div(context, mk(entity, id), box(x, y, 64, 44).with_custom_background(c).with_corner_radius(6 * s)
                                              .with_ignore_pointer_events());
    };
    const auto rgb = [](ColorType c) {
      char buf[40];
      std::snprintf(buf, sizeof buf, "%d, %d, %d", int(c.r), int(c.g), int(c.b));
      return std::string(buf);
    };

    if (!started || seen_replay != replay_stamp) {
      started = true;
      seen_replay = replay_stamp;
      restart();
    }
    auto &prog = motion::anim(Key::Progress);
    const float p = prog.value_or(0.f);
    auto &alpha_track = motion::anim<ColorType>(Key::Alpha);

    div(context, mk(entity, 0), box(0, 0, 1280, 720).with_custom_background({255, 245, 235, 255}).with_debug_name("bg"));
    label(1, "Colour tracks: OKLab", 60, 36, 700, 30, false, "title");
    label(2, "Colour tracks interpolate in OKLab (L, a, b) plus linear alpha. The middle swatch is the track colour, the right swatch is a per-channel sRGB lerp at the same progress.", 60, 76, 1160, 17, true);
    char pbuf[40];
    std::snprintf(pbuf, sizeof pbuf, "progress: %d%%", int(std::lround(p * 100.f)));
    label(3, pbuf, 60, 106, 300, 19, false, "progress");
    label(4, "pair", 60, 140, 220, 16, true);
    label(5, "from", 300, 140, 80, 16, true);
    label(6, "track (OKLab)", 388, 140, 170, 16, true);
    label(7, "sRGB lerp", 566, 140, 170, 16, true);
    label(8, "to", 744, 140, 80, 16, true);
    label(9, "midpoint rgb / expected", 840, 140, 400, 16, true);

    for (size_t i = 0; i < pairs.size(); ++i) {
      const Pair &pair = pairs[i];
      const float y = 172.f + float(i) * 62.f;
      const int base = 100 + int(i) * 10;
      auto &track = motion::anim<ColorType>(static_cast<Key>(static_cast<size_t>(Key::Row0) + i));
      const ColorType cur = track.value_or(pair.from);
      const auto naive_ch = [&](unsigned char a, unsigned char b) {
        return static_cast<unsigned char>(std::lround(a + (int(b) - int(a)) * p));
      };
      const ColorType naive{naive_ch(pair.from.r, pair.to.r), naive_ch(pair.from.g, pair.to.g),
                            naive_ch(pair.from.b, pair.to.b), 255};
      label(base, pair.name, 60, y + 10, 230, 18, false, "pair_" + std::to_string(i));
      swatch(base + 1, 300, y, pair.from);
      swatch(base + 2, 388, y, cur);
      swatch(base + 3, 566, y, naive);
      swatch(base + 4, 744, y, pair.to);
      label(base + 5, rgb(cur) + " / " + pair.mid, 836, y + 12, 436, 15, false, "rgb_" + std::to_string(i));
    }

    const float ay = 492.f;
    label(200, "Alpha fades linearly: coral 255 to 60 with the same sweep.", 60, ay, 700, 18, false, "alpha_label");
    swatch(201, 760, ay - 8, alpha_track.value_or(ColorType{255, 115, 105, 255}));
    char abuf[24];
    std::snprintf(abuf, sizeof abuf, "a: %d", int(alpha_track.value_or(ColorType{255, 115, 105, 255}).a));
    label(202, abuf, 840, ay, 200, 18, false, "alpha_value");

    auto spring_btn = button(context, mk(entity, 210),
                             box(60, 548, 300, 48).with_label(spring_on ? "Spring: blue" : "Spring: coral")
                                 .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(ink)
                                 .with_corner_radius(10 * s).with_debug_name("spring_btn")
                                 .with(um::on_state(spring_on, {.background = {ColorType{255, 115, 105, 255}, ColorType{80, 120, 220, 255}}},
                                                    motion::Spring::smooth())));
    if (spring_btn) spring_on = !spring_on;
    label(211, "The button background is a ui_motion colour track on a spring; toggling crosses in OKLab.", 380, 560, 820, 17, true);
    if (button(context, mk(entity, 212), box(60, 624, 160, 44).with_label("Replay")
                                             .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(ink)
                                             .with_custom_background({230, 220, 235, 255}).with_corner_radius(8 * s)
                                             .with_debug_name("replay_btn")))
      ++replay_stamp;
    label(213, std::string("motion: ") + (prog.active() ? "in flight" : "settled"), 240, 634, 300, 18, false, "motion_state");
  }
};

REGISTER_EXAMPLE_SCREEN(colour_track_lab, "Animations",
                        "Colour track lab: OKLab interpolation, alpha and spring backgrounds",
                        ColourTrackLab)
