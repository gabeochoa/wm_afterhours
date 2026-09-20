#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Animation keys for interactive demo
enum struct InteractiveAnimKey : size_t {
  Button1Scale,
  Button2Scale,
  Button3Scale,
};

struct AnimationInteractiveDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};

  // Click counters
  std::array<int, 3> activation_counts{};
  int last_pressed = -1;
  std::array<bool, 3> preview_mode{};

  // For first-run screenshot
  bool first_run = true;

  static constexpr std::array<InteractiveAnimKey, 3> keys{
      InteractiveAnimKey::Button1Scale, InteractiveAnimKey::Button2Scale,
      InteractiveAnimKey::Button3Scale};
  static constexpr std::array<const char *, 3> names{"Blue", "Green", "Purple"};

  static afterhours::motion::Track<float> &track(InteractiveAnimKey key) {
    return afterhours::motion::anim(key);
  }

  float get_anim_value(InteractiveAnimKey key) { return track(key).value(); }

  void animate_button_press(size_t index, bool preview = false) {
    using afterhours::motion::Timeline;
    using afterhours::motion::curves::ease_out_quad;
    preview_mode[index] = preview;
    const float duration_scale = preview ? 4.f : 1.f;
    const auto leg = [&](float seconds) {
      return Timeline{.keys = {{0.f, 0.f}, {seconds * duration_scale, 1.f}}, .curve = ease_out_quad};
    };
    // Quick scale down and back up for press feedback
    track(keys[index]).from(1.f).to(.85f, leg(.08f)).then(1.10f, leg(.12f)).then(1.f, leg(.10f));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Initialize animation tracks to 1.0 on first run for screenshot
    if (first_run) {
      first_run = false;
      for (auto key : keys) track(key).from(1.f);
    }

    // Setup theme
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    if (s <= 0.f) return;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const afterhours::Color muted{183, 194, 214, 255};
    const afterhours::Color border{75, 87, 108, 255};
    const auto box = [s, top, left](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position((left + x) * s, (top + y) * s)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };

    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg_dark).with_corner_radius(0).with_debug_name("interactive_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(x, y, w, h).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(0, 24, 1144, 96)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(12 * s));
    // Title
    label("Interactive animations", 24, 35, 1096, 42, 34, text_light);
    // Subtitle
    label("Three independent buttons. One shared press response: compress, overshoot, settle.",
          24, 81, 1096, 27, 21, muted);

    const std::array<afterhours::Color, 3> colors{box_blue, box_green, box_purple};
    for (size_t i = 0; i < keys.size(); ++i) {
      const float x = static_cast<float>(i) * 388.f;
      const std::string prefix = "interactive_" + std::to_string(i + 1);
      div(context, mk(root.ent(), id++), box(x, 140, 368, 404)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(12 * s));
      label(fmt::format("0{}  {} instance", i + 1, names[i]), x + 20, 152, 328, 33, 27, colors[i]);
      label("80 + 120 + 100 ms = 300 ms", x + 20, 194, 328, 26, 21, muted);
      label("EaseOutQuad / every segment", x + 20, 225, 328, 26, 20, muted);
      div(context, mk(root.ent(), id++), box(x + 134, 278, 100, 100)
          .with_on_draw_fg([s](RectangleType r) {
            afterhours::draw_rectangle_rounded_lines_ex(r, .15f, 16, 2 * s, {183, 194, 214, 255});
          }).with_ignore_pointer_events());
      const float scale = get_anim_value(keys[i]);
      // Using with_scale() for smooth animations - bypasses layout recalculation
      if (button(context, mk(root.ent(), id++), box(x + 134, 278, 100, 100)
          .with_label("Press").with_scale(scale)
          .with_font("AtkinsonMock", pixels(25 * s))
          .with_custom_background(colors[i]).with_custom_text_color({12, 23, 40, 255})
          .with_corner_radius(7.5f * s).with_click_activation(ClickActivationMode::Press)
          .with_debug_name("btn" + std::to_string(i + 1)))) {
        ++activation_counts[i];
        last_pressed = static_cast<int>(i);
        animate_button_press(i);
      }
      label("100% > 85% > 110% > 100%", x + 20, 391, 328, 27, 22, text_light);
      div(context, mk(root.ent(), id++), box(x + 20, 430, 328, 34)
          .with_custom_background({23, 29, 40, 255}).with_corner_radius(6 * s));
      label(fmt::format("Scale: {:3.0f}%", get_anim_value(keys[i]) * 100),
            x + 32, 430, 304, 34, 27, text_light, prefix + "_scale", true);
      label(fmt::format("{} activations: {}", names[i], activation_counts[i]),
            x + 20, 475, 328, 26, 22, text_light, prefix + "_count");
      const bool active = track(keys[i]).active();
      const std::string phase = preview_mode[i] ? (active ? "Slow preview running / 1.20 s" : "Slow preview complete / 1.20 s")
                                              : (active ? "Press response running / 0.30 s" : "Ready / 0.30 s response");
      label(phase, x + 20, 509, 328, 24, 19, muted, prefix + "_phase");
    }

    if (button(context, mk(root.ent(), id++), box(0, 564, 280, 42)
        .with_label("Preview all at 1/4 speed").with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background({62, 104, 173, 255}).with_custom_text_color(text_light)
        .with_corner_radius(8 * s).with_debug_name("interactive_preview"))) {
      for (size_t i = 0; i < keys.size(); ++i) animate_button_press(i, true);
    }
    if (button(context, mk(root.ent(), id++), box(296, 564, 180, 42)
        .with_label("Reset counts").with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background({53, 70, 97, 255}).with_custom_text_color(text_light)
        .with_corner_radius(8 * s).with_debug_name("interactive_reset"))) {
      activation_counts.fill(0);
      last_pressed = -1;
    }
    // Click counter display
    const int total = activation_counts[0] + activation_counts[1] + activation_counts[2];
    label(fmt::format("Total activations: {}", total), 504, 568, 308, 31, 25, text_light, "interactive_total");
    label(last_pressed < 0 ? "Last pressed: none" : "Last pressed: " + std::string(names[last_pressed]),
          824, 568, 320, 31, 23, text_light, "interactive_last");
    label("Pointer press or keyboard activation. Preview leaves counts unchanged; Reset clears counts only.",
          0, 622, 1144, 27, 21, muted);
    // Instructions section
    label("with_scale() changes visual size after layout. Resting buttons are 100 x 100 px at 720p.",
          0, 662, 1144, 28, 20, muted, "interactive_api", true);
  }
};

REGISTER_EXAMPLE_SCREEN(animation_interactive, "Animations",
                        "Independent buttons with the same press-scale response",
                        AnimationInteractiveDemo)
