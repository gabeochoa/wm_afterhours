#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation.h>
#include <array>
#include <optional>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Animation keys for this demo
enum struct BasicAnimKey : size_t {
  FadeIn,
  SlideIn,
  ScaleUp,
};

struct AnimationBasicDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};

  bool animations_started = false;
  bool first_run = true; // Start with completed values for screenshot

  std::optional<BasicAnimKey> last_replayed;

  void start_animations(std::optional<BasicAnimKey> only = std::nullopt) {
    if (animations_started && !only)
      return;
    animations_started = true;
    last_replayed = only;

    if (first_run) {
      // For first run, set to end values so screenshot shows something
      first_run = false;
      auto &fade_track =
          afterhours::animation::manager<BasicAnimKey>().ensure_track(
              BasicAnimKey::FadeIn);
      fade_track = {};
      fade_track.current = 1.0f;
      auto &slide_track =
          afterhours::animation::manager<BasicAnimKey>().ensure_track(
              BasicAnimKey::SlideIn);
      slide_track = {};
      slide_track.current = 0.0f;
      auto &scale_track =
          afterhours::animation::manager<BasicAnimKey>().ensure_track(
              BasicAnimKey::ScaleUp);
      scale_track = {};
      scale_track.current = 1.0f;
      return;
    }

    // Fade: 0 -> 1 over 1.6s (slower for visibility)
    if (!only || *only == BasicAnimKey::FadeIn)
      afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::FadeIn)
        .from(0.0f)
        .to(1.0f, 1.6f, afterhours::animation::EasingType::EaseOutQuad);

    // Slide: -200 -> 0 over 1.2s (slower for visibility)
    if (!only || *only == BasicAnimKey::SlideIn)
      afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::SlideIn)
        .from(-200.0f)
        .to(0.0f, 1.2f, afterhours::animation::EasingType::EaseOutQuad);

    // Scale: 0 -> 1 over 1.0s with bounce effect (sequence)
    if (!only || *only == BasicAnimKey::ScaleUp)
      afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::ScaleUp)
        .from(0.0f)
        .sequence({
            {1.15f, 0.6f, afterhours::animation::EasingType::EaseOutQuad},
            {1.0f, 0.4f, afterhours::animation::EasingType::EaseOutQuad},
        });
  }

  float get_anim_value(BasicAnimKey key, float /*default_val*/ = 0.0f) {
    auto val = afterhours::animation::manager<BasicAnimKey>().get_value(key);
    if (val.has_value()) return val.value();
    // If animation finished, return the end value
    auto &track =
        afterhours::animation::manager<BasicAnimKey>().ensure_track(key);
    return track.current;
  }

  float elapsed(BasicAnimKey key) {
    const auto &track = afterhours::animation::manager<BasicAnimKey>().ensure_track(key);
    const float duration = key == BasicAnimKey::SlideIn ? 1.2f : key == BasicAnimKey::FadeIn ? 1.6f : 1.f;
    if (!track.active) return duration;
    return std::min(duration, track.elapsed + (key == BasicAnimKey::ScaleUp && track.queue.empty() ? .6f : 0.f));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Update animation manager
    afterhours::animation::manager<BasicAnimKey>().update(dt);
    // Start animations on first frame
    start_animations();
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
        .with_custom_background(bg_dark).with_debug_name("basic_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 96)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(12 * s));
    // Title
    label("Animation basics", 24, 35, 1096, 42, 34, text_light);
    // Subtitle
    label("Slide, fade, scale. Replay together or inspect one motion at a time.", 24, 81, 1096, 27, 21, muted);
    const std::array<BasicAnimKey, 3> keys{BasicAnimKey::SlideIn, BasicAnimKey::FadeIn, BasicAnimKey::ScaleUp};
    const std::array<afterhours::Color, 3> colors{box_green, box_blue, box_purple};
    const std::array<const char *, 3> titles{"01  Slide in", "02  Fade in", "03  Scale up"};
    const std::array<const char *, 3> timing{"1.2 s / EaseOutQuad", "1.6 s / EaseOutQuad", "0.6 + 0.4 s / EaseOutQuad"};
    const std::array<const char *, 3> ranges{"x: -200 px to 0 px", "Opacity: 0% to 100%", "Scale: 0% to 115% to 100%"};
    const std::array<const char *, 3> names{"slide", "fade", "scale"};
    for (int i = 0; i < 3; ++i) {
      const float x = i * 388.f;
      const float value = get_anim_value(keys[i]);
      const bool active = afterhours::animation::manager<BasicAnimKey>().ensure_track(keys[i]).active;
      div(context, mk(root.ent(), id++), box(left + x, 140, 368, 412)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(12 * s));
      label(titles[i], x + 20, 152, 328, 33, 27, colors[i]);
      label(timing[i], x + 20, 193, 328, 25, 20, muted);
      label(ranges[i], x + 20, 222, 328, 25, 19, text_light);
      const float base_x = x + (i == 0 ? 224.f : 124.f);
      if (i == 0) {
        div(context, mk(root.ent(), id++), box(left + x + 24, 262, 120, 120)
            .with_on_draw_fg([s](RectangleType r) {
              afterhours::draw_rectangle_rounded_lines_ex(r, .15f, 16, 2 * s, {84, 131, 105, 255});
            }).with_ignore_pointer_events());
        label("origin", x + 51, 304, 74, 28, 19, muted);
      }
      if (i == 1) {
        div(context, mk(root.ent(), id++), box(left + base_x - 12, 250, 144, 144)
            .with_on_draw_bg([](RectangleType r) {
              const float cell = r.width / 10;
              for (int row = 0; row < 10; ++row)
                for (int col = 0; col < 10; ++col)
                  afterhours::draw_rectangle({r.x + col * cell, r.y + row * cell, cell, cell},
                      (row + col) % 2 ? afterhours::Color{88, 99, 117, 255} : afterhours::Color{50, 61, 79, 255});
            }).with_ignore_pointer_events());
      }
      if (i == 2)
        div(context, mk(root.ent(), id++), box(left + base_x - 3, 259, 126, 126)
            .with_on_draw_fg([s](RectangleType r) {
              afterhours::draw_rectangle_rounded_lines_ex(r, .17f, 16, s, {192, 158, 215, 255});
            }).with_ignore_pointer_events());
      auto specimen = box(left + base_x + (i == 0 ? value : 0.f), 262, 120, 120)
          .with_custom_background(i == 1 ? afterhours::colors::opacity_pct(colors[i], value) : colors[i])
          .with_corner_radius(9 * s).with_ignore_pointer_events().with_debug_name(std::string(names[i]) + "_box");
      if (i == 2) specimen.with_scale(value);
      div(context, mk(root.ent(), id++), specimen);
      const std::string metric = i == 0 ? fmt::format("x offset: {:.0f}px", value)
                                : i == 1 ? fmt::format("opacity: {:.0f}%", value * 100)
                                         : fmt::format("scale: {:.0f}%", value * 100);
      div(context, mk(root.ent(), id++), box(left + x + 20, 397, 328, 32)
          .with_custom_background({23, 29, 40, 255}).with_corner_radius(6 * s));
      label(metric, x + 32, 397, 304, 32, 27, text_light, std::string(names[i]) + "_metric", true);
      if (button(context, mk(root.ent(), id++), box(left + x + 20, 439, 144, 35)
          .with_label("Replay " + std::string(names[i])).with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background({53, 70, 97, 255}).with_custom_text_color(text_light).with_corner_radius(7 * s)
          .with_debug_name("replay_" + std::string(names[i])))) start_animations(keys[i]);
      label(active ? "Running" : "Complete", x + 184, 442, 164, 28, 19, muted,
            std::string(names[i]) + "_phase");
      const std::array<const char *, 3> phases{"Start", "Midpoint", "End"};
      for (int step = 0; step < 3; ++step) {
        label(phases[step], x + 20 + step * 112, 482, 104, 23, 16, muted);
        const float preview = step == 0 ? 0.f : step == 2 ? 1.f : i == 2 ? 1.15f * (1.f - 1.f / 36.f) : .75f;
        div(context, mk(root.ent(), id++), box(left + x + 20 + step * 112, 509, 96, 26)
            .with_on_draw_bg([i, preview, color = colors[i]](RectangleType r) {
              afterhours::draw_rectangle(r, {23, 29, 40, 255});
              float width = r.height - 6.f;
              if (i == 2) width *= preview;
              const float offset = i == 0 ? 3.f + preview * (r.width - width - 6.f) : (r.width - width) / 2.f;
              afterhours::draw_rectangle({r.x + offset, r.y + (r.height - width) / 2.f, width, width},
                  i == 1 ? afterhours::colors::opacity_pct(color, preview) : color);
            }).with_ignore_pointer_events());
      }
    }
    const BasicAnimKey clock_key = last_replayed.value_or(BasicAnimKey::FadeIn);
    const float duration = clock_key == BasicAnimKey::FadeIn ? 1.6f : clock_key == BasicAnimKey::SlideIn ? 1.2f : 1.f;
    const float time = elapsed(clock_key);
    const std::string replay_name = !last_replayed ? "all three" : clock_key == BasicAnimKey::FadeIn ? "fade" : clock_key == BasicAnimKey::SlideIn ? "slide" : "scale";
    label(fmt::format("Last replay: {}  /  {:.2f} of {:.1f} s", replay_name, time, duration), 0, 567, 1144, 28, 21, text_light, "basic_timeline_label");
    div(context, mk(root.ent(), id++), box(left, 604, 1144, 8)
        .with_custom_background({66, 79, 101, 255}).with_ignore_pointer_events()
        .with_on_draw_fg([time, duration](RectangleType r) {
          afterhours::draw_rectangle({r.x, r.y, r.width * time / duration, r.height}, {129, 167, 232, 255});
          raylib::DrawCircleV({r.x + r.width * time / duration, r.y + r.height / 2}, r.height * .8f, {235, 240, 250, 255});
        }));
    // ========== Restart Button ==========
    if (button(context, mk(root.ent(), id++), box(left, 636, 240, 44)
        .with_label("Replay all animations").with_font("AtkinsonMock", pixels(23 * s))
        .with_custom_background({62, 104, 173, 255}).with_custom_text_color(text_light).with_corner_radius(8 * s)
        .with_debug_name("restart_btn"))) {
      animations_started = false;
      start_animations();
    }
    bool running = false;
    for (auto key : keys) running |= afterhours::animation::manager<BasicAnimKey>().ensure_track(key).active;
    label(running ? "Running" : "Complete", 270, 644, 170, 30, 23, text_light, "basic_phase");
    label("Thumbnails show 0%, 50% and 100% of each duration.", 500, 644, 644, 30, 19, muted);
  }

};

REGISTER_EXAMPLE_SCREEN(animation_basic, "Animations",
                        "Basic property animations (fade, slide, scale)",
                        AnimationBasicDemo)
