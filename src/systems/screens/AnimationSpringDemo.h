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

struct AnimationSpringDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors - vibrant palette
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_pink{236, 72, 153, 255};
  afterhours::Color box_cyan{34, 211, 238, 255};
  afterhours::Color box_yellow{250, 204, 21, 255};
  afterhours::Color box_lime{163, 230, 53, 255};
  afterhours::Color box_violet{139, 92, 246, 255};

  // Animation state
  float time_elapsed = 0.0f;
  // Eight seconds in the past, so every spring is already settled on the first
  // frame. Headless renders about two frames, which is nowhere near enough for
  // a spring that starts at zero, and the boxes captured as invisible.
  float trigger_time = -8.0f;
  int activation_count = 0;
  bool slow_preview = false;
  std::string last_sample = "None";
  std::array<float, 5> box_click_times{-10.f, -10.f, -10.f, -10.f, -10.f};

  // Spring physics parameters
  struct SpringParams {
    float frequency = 8.0f;
    float decay = 3.0f;
    float amplitude = 1.0f;
  };

  // Damped spring oscillation (settles to 0)
  float spring_oscillation(float t, const SpringParams &params) {
    if (t < 0.0f)
      return 0.0f;
    float damping = std::exp(-params.decay * t);
    float oscillation = std::sin(params.frequency * t * 6.28318f);
    return params.amplitude * oscillation * damping;
  }

  // Spring that settles to target (1.0) with overshoot
  float spring_to_target(float t, const SpringParams &params) {
    if (t < 0.0f)
      return 0.0f;
    float damping = std::exp(-params.decay * t);
    float oscillation = std::cos(params.frequency * t * 6.28318f);
    return 1.0f - oscillation * damping;
  }

  // Elastic pop - quick overshoot then settle
  // Returns minimum 0.35 to ensure container stays large enough for text with
  // margins
  float elastic_pop(float t, float overshoot = 1.3f,
                    float settle_speed = 4.0f) {
    if (t < 0.0f)
      return 0.35f;
    if (t > 2.0f)
      return 1.0f;
    float progress = 1.0f - std::exp(-settle_speed * t);
    float bounce =
        std::sin(t * 12.0f) * std::exp(-t * 3.0f) * (overshoot - 1.0f);
    return std::max(0.35f, std::min(1.0f, progress + bounce));
  }

  // Balloon pop - deflate to 0 then smoothly inflate back to 1
  float balloon_pop(float t, float duration = 0.8f) {
    if (t < 0.0f)
      return 1.0f; // Before animation, full size
    if (t >= duration)
      return 1.0f; // After animation, full size
    // Start at 0, smoothly ease-out to 1
    float progress = t / duration;
    // Ease-out cubic for smooth deceleration
    float eased =
        1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress);
    return eased;
  }

  // Horizontal rocker - starts to one side, oscillates back to center
  float pendulum_rocker(float t, float amplitude = 15.0f,
                        float frequency = 2.0f, float decay = 2.0f) {
    if (t < 0.0f)
      return 0.0f;
    // Starts at amplitude, moves back through center, settles at 0
    return amplitude * std::cos(frequency * t * 6.28318f) *
           std::exp(-decay * t);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    time_elapsed += dt * (slow_preview ? .25f : 1.f);

    // Setup theme
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const auto muted = afterhours::Color{183, 194, 214, 255};
    const auto border = afterhours::Color{75, 87, 108, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)}).with_corner_radius(0)
        .with_custom_background(bg_dark).with_debug_name("spring_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 116)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(12 * s));
    label("Springy!", 24, 34, 1096, 42, 34, text_light);
    label("Spring animation variants", 24, 80, 1096, 26, 23, text_light);
    label("Click a sample or focus it and press Enter to replay. Outlines mark the resting bounds.",
          24, 108, 1096, 25, 19, muted);

    // ========== Restart Button ==========
    if (button(context, mk(root.ent(), id++), box(left, 582, 226, 46)
        .with_label("Restart all five").with_font("AtkinsonMock", pixels(23 * s))
        .with_custom_background({62, 104, 173, 255}).with_custom_text_color(text_light)
        .with_corner_radius(8 * s).with_debug_name("restart_btn"))) {
      trigger_time = time_elapsed;
      // Reset click times
      box_click_times.fill(-10.f);
      last_sample = "All five";
    }
    if (button(context, mk(root.ent(), id++), box(left + 242, 582, 226, 46)
        .with_label(slow_preview ? "Slow preview: 0.25x" : "Slow preview: off")
        .with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background(slow_preview ? afterhours::Color{62, 104, 173, 255} : afterhours::Color{47, 60, 82, 255})
        .with_custom_text_color(text_light).with_corner_radius(8 * s).with_debug_name("slow_preview")))
      slow_preview = !slow_preview;

    const std::array<const char *, 5> titles{"01  Boing!", "02  Jelly", "03  Balloon", "04  Drop", "05  Rocker"};
    const std::array<const char *, 5> names{"Boing", "Jelly", "Balloon", "Drop", "Rocker"};
    const std::array<const char *, 5> descriptors{"Scale + vertical stretch", "Scale + horizontal wobble", "Inflate to full size", "Vertical oscillation", "Horizontal oscillation"};
    const std::array<afterhours::Color, 5> colors{box_pink, box_cyan, box_yellow, box_lime, box_violet};
    const std::array<float, 5> delays{.2f, .35f, .5f, .65f, .8f};
    const std::array<float, 5> roundness{.3f, .5f, .2f, .15f, .4f};
    const std::array<const char *, 5> primary{"4 Hz / decay 1.5", "3 Hz / decay 1.2", "0.6 s / ease-out cubic", "3 Hz / decay 1.5", "1.5 Hz / decay 1.5"};
    const std::array<const char *, 5> secondary{"Stretch: 3 Hz / 8%", "Wobble: 2.5 Hz / 4px", "Scale: 0% to 100%", "Start: 50px above", "x range: -12 to +12px"};
    const std::array<const char *, 5> tertiary{"Stretch decay: 2", "Wobble decay: 1.5", "No overshoot", "Ripple: 4 Hz / 8px", "Rotation: 0 degrees"};
    const std::array<float, 5> settled_after{3.08f, 3.84f, .6f, 3.08f, 3.08f};
    for (size_t i = 0; i < names.size(); ++i) {
      const float x = static_cast<float>(i) * 232.f;
      const float t = box_click_times[i] > trigger_time
                          ? time_elapsed - box_click_times[i]
                          : time_elapsed - trigger_time - delays[i];
      float scale = 1.f;
      float height = 100.f;
      float dx = 0.f;
      float dy = 0.f;
      if (i == 0) {
        scale = spring_to_target(t, {4.f, 1.5f, 1.f});
        height *= 1.f + spring_oscillation(t, {3.f, 2.f, .08f});
      }
      if (i == 1) {
        scale = spring_to_target(t, {3.f, 1.2f, 1.f});
        dx = spring_oscillation(t, {2.5f, 1.5f, 4.f});
      }
      if (i == 2) scale = balloon_pop(t, .6f);
      if (i == 3) {
        dy = -50.f + 50.f * spring_to_target(t, {3.f, 1.5f, 1.f});
        dy += spring_oscillation(t, {4.f, 2.5f, 8.f});
      }
      if (i == 4) dx = pendulum_rocker(t, 12.f, 1.5f, 1.5f);
      div(context, mk(root.ent(), id++), box(left + x, 160, 216, 396)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(12 * s));
      label(titles[i], x + 14, 171, 188, 34, 26, colors[i]);
      label(descriptors[i], x + 14, 209, 188, 26, 16, muted);
      if (i == 3) {
        div(context, mk(root.ent(), id++), box(left + x + 58, 248, 100, 100)
            .with_on_draw_fg([s](RectangleType r) {
              afterhours::draw_rectangle_rounded_lines_ex(r, .15f, 12, s, {88, 117, 68, 255});
            }).with_ignore_pointer_events());
        label("start", x + 58, 246, 100, 23, 16, muted);
      }
      if (button(context, mk(root.ent(), id++), box(left + x + 58 + dx, 298 + dy, 100, height)
          .with_scale(scale).with_custom_background(colors[i])
          .with_rounded_corners(RoundedCorners())
          .with_corner_radius(roundness[i] * std::min(100.f, height) * s / 2.f)
          .with_debug_name("box" + std::to_string(i + 1)))) {
        box_click_times[i] = time_elapsed;
        ++activation_count;
        last_sample = names[i];
      }
      div(context, mk(root.ent(), id++), box(left + x + 55, 295, 106, 106)
          .with_on_draw_fg([s](RectangleType r) {
            afterhours::draw_rectangle_rounded_lines_ex(r, .12f, 12, s, {151, 163, 184, 255});
          }).with_ignore_pointer_events());
      if (i == 4)
        div(context, mk(root.ent(), id++), box(left + x + 94, 407, 28, 9)
            .with_on_draw_fg([s](RectangleType r) {
              afterhours::draw_rectangle({r.x, r.y + 4 * s, r.width, s}, {183, 194, 214, 255});
              afterhours::draw_rectangle({r.x + r.width / 2, r.y, s, r.height}, {248, 250, 252, 255});
            }).with_ignore_pointer_events());
      label(primary[i], x + 14, 433, 188, 24, 19, text_light);
      label(secondary[i], x + 14, 461, 188, 24, 18, muted);
      label(tertiary[i], x + 14, 487, 188, 24, 17, muted);
      const char *phase = t < 0.f ? "Waiting" : t < settled_after[i] ? "Moving" : "Settled";
      label(phase, x + 14, 521, 188, 24, 20, text_light, "spring_phase_" + std::to_string(i + 1));
    }
    // ========== Bounce Counter ==========
    label(fmt::format("Activations: {}", activation_count), 492, 586, 260, 33, 25, text_light, "activation_count");
    label("Last sample: " + last_sample, 772, 586, 372, 33, 23, text_light, "last_sample");
    label("Activations count individual sample clicks and Enter presses. Restart preserves the count.",
          0, 640, 1144, 26, 20, muted);
    label("Decay is per second. Settled means less than 1% of the initial motion envelope remains.",
          0, 669, 1144, 25, 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(animation_spring, "Animations",
                        "Delightful spring physics with interactive bounces",
                        AnimationSpringDemo)
