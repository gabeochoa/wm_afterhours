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

struct AnimationLoopingDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};
  afterhours::Color box_orange{249, 115, 22, 255};

  float time_elapsed = 0.0f;
  bool paused = false;
  bool show_trails = false;

  // Smooth easing function for bounce (attempt at ease-in for falling)
  float ease_out_bounce(float t) {
    // Simple approximation using sin for smooth motion
    return std::sin(t * 3.14159f);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Accumulate time for smooth continuous animations
    if (!paused) time_elapsed += dt;

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
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg_dark).with_corner_radius(0).with_debug_name("looping_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 92)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(12 * s));
    // Title
    label("Looping animations", 24, 34, 1096, 42, 34, text_light);
    // Subtitle
    label("Four continuous motions. Pause to compare values, or restart every cycle at zero.",
          24, 79, 1096, 28, 21, muted);
    const auto control = [&](const std::string &text, float x, float width, const std::string &name) {
      return button(context, mk(root.ent(), id++), box(left + x, 132, width, 44)
          .with_label(text).with_font("AtkinsonMock", pixels(21 * s))
          .with_custom_background({53, 70, 97, 255}).with_custom_text_color(text_light)
          .with_corner_radius(8 * s).with_debug_name(name));
    };
    if (control(paused ? "Resume all" : "Pause all", 0, 156, "pause_cycles")) paused = !paused;
    if (control("Restart cycles", 168, 180, "restart_cycles")) time_elapsed = 0.f;
    if (control(show_trails ? "Hide cycle samples" : "Show cycle samples", 360, 232, "toggle_cycle_samples"))
      show_trails = !show_trails;
    label(fmt::format("{}  /  {:.2f} s", paused ? "Paused" : "Running", time_elapsed),
          626, 140, 490, 30, 23, text_light, "looping_clock");

    // Use constants for cleaner math
    constexpr float kPi = 3.14159265359f;
    constexpr float kTwoPi = 2.0f * kPi;

    // Calculate smooth animated values using trigonometry
    // Pulse 1: Breathe using translate (move in/out from center) - avoids
    // layout resize
    float pulse1_phase = time_elapsed * kTwoPi / 4.0f; // 4 second cycle
    float pulse1_offset =
        10.0f * std::sin(pulse1_phase); // -10 to +10 px offset

    // Pulse 2: Opacity-based pulse - smoother since no layout recalc
    float pulse2_phase = time_elapsed * kTwoPi / 1.5f; // 1.5 second cycle
    float pulse2_opacity = 0.5f + 0.5f * std::sin(pulse2_phase); // 0 to 1

    // Bounce: Use translate instead of resize for smooth motion
    float bounce_phase = time_elapsed * kTwoPi / 2.0f;
    float bounce_offset =
        50.0f * (1.0f - std::cos(bounce_phase)); // 0 to 100 smoothly

    // Spin: Continuous rotation - full rotation every 3 seconds (slower)
    float rotation = std::fmod(time_elapsed * 120.0f,
                               360.0f); // 120 deg/sec = 3 sec rotation


    const std::array<const char *, 4> titles{"01  Breathe", "02  Glow", "03  Bounce", "04  Spin"};
    const std::array<const char *, 4> properties{"Blue / diagonal translation", "Green / opacity", "Purple / vertical translation", "Orange / orbiting marker"};
    const std::array<float, 4> periods{4.f, 1.5f, 2.f, 3.f};
    const std::array<afterhours::Color, 4> colors{box_blue, box_green, box_purple, box_orange};
    const std::array<const char *, 4> ranges{"x and y: -10 to +10 px", "Opacity: 0% to 100%", "y: 0 to +100 px", "Radius: 40 px / 120 deg/s"};
    const std::array<const char *, 4> names{"breathe", "glow", "bounce", "spin"};
    for (int i = 0; i < 4; ++i) {
      const float x = i * 290.f;
      div(context, mk(root.ent(), id++), box(left + x, 194, 274, 416)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(12 * s));
      label(titles[i], x + 16, 207, 242, 32, 26, colors[i]);
      label(properties[i], x + 16, 244, 242, 25, 18, muted);
      label(fmt::format("{:.1f} s per cycle", periods[i]), x + 16, 274, 242, 25, 20, text_light);
      label(ranges[i], x + 16, 546, 242, 25, 19, muted);
      const float phase = std::fmod(time_elapsed, periods[i]) / periods[i];
      label(fmt::format("Phase {:05.1f}%", phase * 100.f), x + 16, 574, 134, 26, 18, text_light,
            std::string(names[i]) + "_phase");
      const std::string value = i == 0 ? fmt::format("{:+.1f}px", pulse1_offset)
                              : i == 1 ? fmt::format("{:.0f}%", pulse2_opacity * 100.f)
                              : i == 2 ? fmt::format("{:.1f}px", bounce_offset)
                                       : fmt::format("{:.0f}deg", rotation);
      label(value, x + 155, 574, 105, 26, 18, colors[i], std::string(names[i]) + "_value");
      div(context, mk(root.ent(), id++), box(left + x + 77, i == 2 ? 304 : 354, 120, 120)
          .with_ignore_pointer_events().with_on_draw_bg([i, s, color = colors[i], show = show_trails](RectangleType r) {
            if (!show) return;
            const auto ghost = afterhours::colors::opacity_pct(color, .38f);
            for (int sample = 0; sample < 8; ++sample) {
              const float angle = static_cast<float>(sample) * 6.28318530718f / 8.f;
              if (i == 3) {
                raylib::DrawCircleLinesV({r.x + r.width / 2 + std::cos(angle) * 40 * s,
                                         r.y + r.height / 2 + std::sin(angle) * 40 * s}, 10 * s, ghost);
                continue;
              }
              if (i == 1) {
                afterhours::draw_rectangle({r.x - 32 * s + sample * 24 * s, r.y + 140 * s, 18 * s, 18 * s},
                    afterhours::colors::opacity_pct(color, .5f + .5f * std::sin(angle)));
                continue;
              }
              const float dx = i == 0 ? 10.f * std::sin(angle) * s : 0.f;
              const float dy = i == 0 ? dx : 50.f * (1.f - std::cos(angle)) * s;
              afterhours::draw_rectangle_rounded_lines_ex({r.x + dx, r.y + dy, r.width, r.height},
                  .5f, 16, s, ghost);
            }
          }));
    }

    const float box_size = 120.f;
    const float box_y = 354.f;
    // ========== PULSE 1: Breathe (translate-based) ==========
    const float pulse1_x = left + 77.f;
    div(context, mk(root.ent(), id++), box(pulse1_x - 3, box_y - 3, box_size + 6, box_size + 6)
        .with_ignore_pointer_events().with_on_draw_fg([s, border](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(r, .5f, 16, 2 * s, border);
        }));
    label("Fixed center outline", 35, 508, 222, 24, 18, muted);
    // Animated box - translate moves it in/out smoothly
    div(context, mk(root.ent(), id++), box(pulse1_x + pulse1_offset, box_y + pulse1_offset, box_size, box_size)
        .with_custom_background(box_blue).with_corner_radius(30 * s)
        .with_ignore_pointer_events().with_debug_name("pulse1_box"));

    // ========== PULSE 2: Opacity Glow ==========
    const float pulse2_x = left + 367.f;
    // Animated box with opacity
    div(context, mk(root.ent(), id++), box(pulse2_x, box_y, box_size, box_size)
        .with_custom_background(afterhours::colors::opacity_pct(box_green, pulse2_opacity))
        .with_corner_radius(30 * s).with_ignore_pointer_events().with_debug_name("pulse2_box"));
    div(context, mk(root.ent(), id++), box(left + 310, 314, 18, 18)
        .with_custom_background(box_green).with_corner_radius(4 * s).with_ignore_pointer_events());
    label("100% reference", 336, 308, 210, 27, 18, muted);
    label(show_trails ? "8 samples across one cycle" : "The shape stays the same", 306, 516, 242, 24, 16, muted);

    // ========== BOUNCE ==========
    const float bounce_x = left + 657.f;
    div(context, mk(root.ent(), id++), box(left + 798, 304, 28, 220)
        .with_ignore_pointer_events().with_on_draw_fg([s, border](RectangleType r) {
          const float x = r.x + 8 * s;
          raylib::DrawLineEx({x, r.y}, {x, r.y + 100 * s}, 2 * s, border);
          raylib::DrawLineEx({x - 6 * s, r.y}, {x + 6 * s, r.y}, 2 * s, border);
          raylib::DrawLineEx({x - 6 * s, r.y + 100 * s}, {x + 6 * s, r.y + 100 * s}, 2 * s, border);
          raylib::DrawLineEx({x - 5 * s, r.y + 89 * s}, {x, r.y + 96 * s}, 2 * s, border);
          raylib::DrawLineEx({x + 5 * s, r.y + 89 * s}, {x, r.y + 96 * s}, 2 * s, border);
        }));
    label("0", 818, 296, 36, 24, 16, muted);
    label("100", 818, 396, 36, 24, 16, muted);
    // Animated box (bounces down)
    div(context, mk(root.ent(), id++), box(bounce_x, 304 + bounce_offset, box_size, box_size)
        .with_custom_background(box_purple).with_corner_radius(30 * s)
        .with_ignore_pointer_events().with_debug_name("bounce_box"));

    // ========== SPIN ==========
    const float rotate_x = left + 947.f;
    // Spinning indicator orbits the center
    const float angle_rad = rotation * kPi / 180.f;
    const float orbit_radius = box_size / 3.f;
    const float indicator_size = 20.f;
    const float orbit_center_x = rotate_x + box_size / 2.f - indicator_size / 2.f;
    const float orbit_center_y = box_y + box_size / 2.f - indicator_size / 2.f;
    const float indicator_x = orbit_center_x + std::cos(angle_rad) * orbit_radius;
    const float indicator_y = orbit_center_y + std::sin(angle_rad) * orbit_radius;
    // Outer ring (static)
    div(context, mk(root.ent(), id++), box(rotate_x, box_y, box_size, box_size)
        .with_custom_background(afterhours::colors::opacity_pct(box_orange, .3f))
        .with_corner_radius(30 * s).with_ignore_pointer_events().with_debug_name("rotate_ring")
        .with_on_draw_fg([s](RectangleType r) {
          const raylib::Vector2 center{r.x + r.width / 2, r.y + r.height / 2};
          const raylib::Color guide{206, 163, 128, 255};
          raylib::DrawCircleLinesV(center, 40 * s, guide);
          raylib::DrawLineEx({center.x - 5 * s, center.y}, {center.x + 5 * s, center.y}, s, guide);
          raylib::DrawLineEx({center.x, center.y - 5 * s}, {center.x, center.y + 5 * s}, s, guide);
        }));
    // Spinning indicator
    div(context, mk(root.ent(), id++), box(indicator_x, indicator_y, indicator_size, indicator_size)
        .with_custom_background(box_orange).with_corner_radius(5 * s)
        .with_border({255, 236, 209, 255}, 2 * s).with_ignore_pointer_events().with_debug_name("rotate_indicator"));
    label("Clockwise / fixed base", 895, 508, 234, 24, 18, muted);

    // Info footer
    div(context, mk(root.ent(), id++), box(left, 626, 1144, 68)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(10 * s));
    label("phase = 2 pi t / period    |    Breathe: 10 sin(phase)    |    Glow: 0.5 + 0.5 sin(phase)",
          16, 632, 1112, 25, 20, text_light);
    label(show_trails ? "Cycle overlay: 8 equally spaced phases. Bounce: 50(1 - cos(phase)). Spin: radius 40."
                      : "Bounce: 50(1 - cos(phase)). Spin: radius 40. Values are in design pixels; geometry scales to fit.",
          16, 662, 1112, 24, 19, muted, "looping_formula");
  }
};

REGISTER_EXAMPLE_SCREEN(animation_looping, "Animations",
                        "Smooth looping animations (pulse, bounce, spin)",
                        AnimationLoopingDemo)
