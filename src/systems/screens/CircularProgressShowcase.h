#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CircularProgressShowcase : ScreenSystem<UIContext<InputAction>> {
  // Modern dark theme for visual impact
  afterhours::Color bg_dark{18, 18, 24, 255};         // Deep charcoal
  afterhours::Color card_dark{30, 32, 42, 255};       // Dark card surface
  afterhours::Color text_light{248, 250, 252, 255};   // Off-white text
  afterhours::Color text_muted{148, 163, 184, 255};   // Muted gray
  afterhours::Color accent_cyan{34, 211, 238, 255};   // Vibrant cyan
  afterhours::Color accent_purple{168, 85, 247, 255}; // Electric purple
  afterhours::Color accent_green{74, 222, 128, 255};  // Fresh green
  afterhours::Color accent_orange{251, 146, 60, 255}; // Warm orange
  afterhours::Color accent_pink{244, 114, 182, 255};  // Hot pink
  afterhours::Color track_dark{78, 89, 108, 255};      // Subtle track

  // Animated values
  float animated_progress =
      0.35f; // Start at 35% so screenshots show meaningful progress
  float animation_speed = 0.3f;

  bool paused = false;
  bool compare_static = false;
  float elapsed = 0.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = card_dark;
    theme.primary = accent_cyan;
    theme.secondary = track_dark;
    theme.accent = accent_purple;
    theme.roundness = 0.15f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const afterhours::Color muted{187, 201, 220, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg_dark).with_corner_radius(0).with_debug_name("circular_canvas"));
    int id = 0;
    const auto label = [&](const std::string &value, float x, float y, float w, float h, float size,
                           afterhours::Color color, const std::string &name = "", bool center = false) {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(center ? TextAlignment::Center : TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto ring = [&](float x, float y, float diameter, float thickness, float value,
                          afterhours::Color color, const std::string &name) {
      return circular_progress(context, mk(root.ent(), id++), value,
          box(left + x, y, diameter, diameter).with_custom_background(color)
          .with_border(track_dark, pixels(thickness * s)).with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 82)
        .with_custom_background(card_dark).with_corner_radius(12 * s));
    // Title
    label("Circular progress indicators", 20, 29, 1104, 42, 34, text_light);
    // Subtitle
    label("Arcs fill clockwise from the top (-90 degrees). Compare diameter, stroke, color and value.",
          20, 76, 1104, 25, 20, muted);
    const auto control = [&](const std::string &text, float x, float width, const std::string &name) {
      return button(context, mk(root.ent(), id++), box(left + x, 116, width, 36)
          .with_label(text).with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background({52, 68, 92, 255}).with_custom_text_color(text_light)
          .with_corner_radius(7 * s).with_debug_name(name));
    };
    if (control(paused ? "Resume live" : "Pause live", 0, 146, "circular_pause")) paused = !paused;
    if (control("Reset live to 35%", 158, 182, "circular_reset")) {
      animated_progress = .35f;
      elapsed = 0.f;
    }
    if (control(compare_static ? "Restore original samples" : "Match static samples at 75%", 352, 310,
                "circular_compare")) compare_static = !compare_static;
    label("30 percentage points/s / 3.33s full cycle", 682, 121, 462, 28, 18, muted);
    // Animate the progress value
    if (!paused) {
      animated_progress += dt * animation_speed;
      elapsed += dt;
      if (animated_progress > 1.f) animated_progress = 0.f;
    }

    // Ring sizes
    const std::array<float, 4> thicknesses{8.f, 8.f, 16.f, 3.f};
    const std::array<float, 4> values{.75f, animated_progress, compare_static ? .75f : .42f,
                                     compare_static ? .75f : .88f};
    const std::array<afterhours::Color, 4> colors{accent_cyan, accent_purple,
                                                compare_static ? accent_cyan : accent_green,
                                                compare_static ? accent_cyan : accent_orange};
    const std::array<const char *, 4> names{"Basic", "Animated", "Thick ring", "Thin ring"};
    // Top row (4 cards)
    for (int i = 0; i < 4; ++i) {
      const float x = i * 290.f;
      div(context, mk(root.ent(), id++), box(left + x, 164, 274, 180)
          .with_custom_background(card_dark).with_corner_radius(10 * s)
          .with_debug_name("card_" + std::to_string((i + 1) * 10)));
      // Card title
      label(names[i], x + 16, 172, 242, 30, 25, text_light);
      // Progress ring
      ring(x + 97, 211, 80, thicknesses[i], values[i], colors[i], "progress_" + std::to_string((i + 1) * 10));
      // Value label
      label(fmt::format("{:.0f}%", values[i] * 100), x + 97, 236, 80, 30, 20, colors[i],
            "circular_value_" + std::to_string(i), true);
      // Description label
      label(fmt::format("Stroke {:.1f}px / diameter {:.0f}px", thicknesses[i] * s, 80 * s),
            x + 12, 301, 250, 24, 17, muted);
      label(i == 1 ? fmt::format("{} / {:.2f}s elapsed", paused ? "Paused" : "Running", elapsed)
                   : i == 0 ? "75% / start -90 degrees" : compare_static ? "Matched: 75% / cyan" : "Original value and color",
            x + 12, 325, 250, 18, 15, muted, "circular_state_" + std::to_string(i));
    }

    // "Size Variations" section header
    label("Diameter", 0, 353, 160, 29, 24, text_light);
    label(compare_static ? "Size comparison: 75%, cyan, shared stroke. The live ring is unchanged."
                         : "Original samples: color, value and stroke vary. Match samples to isolate diameter.",
          170, 354, 974, 28, 18, muted, "circular_size_mode");
    const std::array<float, 5> sizes{28.f, 40.f, 52.f, 64.f, 80.f};
    const std::array<afterhours::Color, 5> size_colors{accent_cyan, accent_purple, accent_green, accent_orange, accent_pink};
    const std::array<float, 5> original_values{.25f, .50f, .75f, .90f, 1.f};
    // Bottom row (5 small cards)
    for (int i = 0; i < 5; ++i) {
      const float x = i * 232.f;
      const float thickness = compare_static ? 3.f : std::max(3.f, sizes[i] * .1f);
      const float value = compare_static ? .75f : original_values[i];
      const auto color = compare_static ? accent_cyan : size_colors[i];
      div(context, mk(root.ent(), id++), box(left + x, 386, 216, 150)
          .with_custom_background(card_dark).with_corner_radius(10 * s)
          .with_debug_name("size_card_" + std::to_string(i)));
      // Size label
      label(fmt::format("Diameter {:.0f}px", sizes[i] * s), x + 12, 389, 192, 26, 21, text_light,
            "size_label_" + std::to_string(i), true);
      // Progress indicator
      ring(x + (216 - sizes[i]) / 2, 456 - sizes[i] / 2, sizes[i], thickness, value,
            color, "progress_size_" + std::to_string(i));
      label(fmt::format("{:.0f}%{}", value * 100, value == 1.f ? " Completed" : ""),
            x + 5, 497, 206, 21, 17, muted, "progress_label_" + std::to_string(i), true);
      label(fmt::format("Stroke {:.1f}px", thickness * s), x + 5, 518, 206, 18, 15, muted,
            "stroke_label_" + std::to_string(i), true);
    }

    div(context, mk(root.ent(), id++), box(left, 548, 780, 156)
        .with_custom_background(card_dark).with_corner_radius(10 * s));
    label("MAIN / diameter / stroke / value", 12, 551, 370, 26, 18, text_light);
    label("SIZE / diameter / stroke / value", 406, 551, 362, 26, 18, text_light);
    for (int i = 0; i < 5; ++i) {
      if (i < 4)
        label(fmt::format("{}  {:.0f}px / {:.1f}px / {:.0f}%", names[i], 80 * s, thicknesses[i] * s, values[i] * 100),
              12, 578 + i * 24.f, 370, 24, 17, colors[i]);
      label(fmt::format("{:.0f}px / {:.1f}px / {:.0f}%", sizes[i] * s,
                         (compare_static ? 3.f : std::max(3.f, sizes[i] * .1f)) * s,
                         (compare_static ? .75f : original_values[i]) * 100),
            406, 578 + i * 24.f, 362, 24, 17, compare_static ? accent_cyan : size_colors[i]);
    }
    label(fmt::format("Live phase: {:.2f}s / 3.33s", animated_progress / animation_speed),
          12, 674, 370, 24, 17, muted, "circular_live_phase");
    div(context, mk(root.ent(), id++), box(left + 796, 540, 348, 164)
        .with_custom_background(card_dark).with_corner_radius(10 * s));
    label("Thin detail", 808, 553, 156, 29, 23, text_light);
    label("2x geometry", 808, 587, 156, 25, 19, muted);
    label(fmt::format("{:.0f}px diameter", 160 * s), 808, 615, 156, 24, 17, muted);
    label(fmt::format("{:.1f}px stroke", 6 * s), 808, 642, 156, 24, 17, muted);
    label("Same ring component", 808, 673, 156, 23, 14, muted);
    ring(972, 541, 160, 6, values[3], colors[3], "progress_thin_detail");
  }
};

REGISTER_EXAMPLE_SCREEN(
    circular_progress, "Component Galleries",
    "Circular/radial progress indicators with various styles",
    CircularProgressShowcase)
