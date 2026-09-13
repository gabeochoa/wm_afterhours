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

// blend::Mode, blend_scope, and the transition counter.
//
// The bottom two rows draw the same eight blits: one scope each, then one
// scope round the batch. The measured counts expose the difference.
struct BlendModeLab : ScreenSystem<UIContext<InputAction>> {
  struct ModeSwatch {
    const char *caption;
    const char *formula;
    afterhours::blend::Mode mode;
  };

  bool prepare_rgb = false;
  std::array<size_t, 6> panel_transitions{};
  static constexpr std::array<afterhours::Color, 3> inputs{{
      {220, 70, 70, 170}, {70, 200, 120, 170}, {80, 130, 235, 170}}};

  static afterhours::Color source_color(size_t index, bool prepare) {
    auto color = inputs[index];
    if (!prepare) return color;
    const float alpha = color.a / 255.f;
    color.r = static_cast<unsigned char>(std::round(color.r * alpha));
    color.g = static_cast<unsigned char>(std::round(color.g * alpha));
    color.b = static_cast<unsigned char>(std::round(color.b * alpha));
    return color;
  }

  static afterhours::Color expected_center(afterhours::blend::Mode mode, bool prepare) {
    namespace blend = afterhours::blend;
    std::array<float, 3> destination{28.f, 30.f, 40.f};
    for (size_t i = 0; i < inputs.size(); ++i) {
      const auto color = source_color(i, prepare);
      const std::array<float, 3> source{static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)};
      const float alpha = color.a / 255.f;
      for (size_t channel = 0; channel < destination.size(); ++channel) {
        float result = source[channel] * alpha + destination[channel] * (1.f - alpha);
        if (mode == blend::Mode::Additive) result = source[channel] * alpha + destination[channel];
        if (mode == blend::Mode::Multiplied) result = source[channel] * destination[channel] / 255.f + destination[channel] * (1.f - alpha);
        if (mode == blend::Mode::AlphaPremultiply) result = source[channel] + destination[channel] * (1.f - alpha);
        destination[channel] = std::round(std::clamp(result, 0.f, 255.f));
      }
    }
    return {static_cast<unsigned char>(destination[0]), static_cast<unsigned char>(destination[1]),
            static_cast<unsigned char>(destination[2]), 255};
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    namespace blend = afterhours::blend;

    // Read before this frame's own drawing so the number is last frame's, which
    // is the whole frame rather than a partial one.
    const size_t last_frame_transitions = blend::transitions_this_frame();
    const auto last_panel_transitions = panel_transitions;
    blend::reset_transition_count();

    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{178, 190, 211, 255};
    const auto border = afterhours::Color{70, 84, 106, 255};
    const auto backing = afterhours::Color{28, 30, 40, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)}).with_corner_radius(0)
        .with_custom_background({19, 23, 32, 255}).with_debug_name("bml_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 92)
        .with_custom_background({36, 43, 57, 255}).with_corner_radius(12 * s));
    label("Blend modes", 24, 34, 460, 42, 34, white, "bml_title");
    label("Identical inputs. Four modes. Two ways to batch.", 24, 80, 668, 26, 20, muted);
    label(fmt::format("Blend-state changes: {} / full frame", last_frame_transitions), 580, 34, 540, 32, 23, white, "bml_transitions");
    if (button(context, mk(root.ent(), id++), box(left + 814, 76, 306, 30)
        .with_label(prepare_rgb ? "Premultiply RGB: on" : "Premultiply RGB: off")
        .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(white)
        .with_custom_background(prepare_rgb ? afterhours::Color{62, 104, 173, 255} : afterhours::Color{51, 65, 89, 255})
        .with_corner_radius(6 * s).with_debug_name("bml_prepare_rgb"))) prepare_rgb = !prepare_rgb;
    label(fmt::format("Fixture {:02} / previous render", prepare_rgb ? 2 : 1), 580, 79, 234, 25, 17, muted, "bml_capture_id");

    for (size_t i = 0; i < inputs.size(); ++i) {
      const float x = static_cast<float>(i) * 286.f;
      auto opaque = inputs[i];
      opaque.a = 255;
      div(context, mk(root.ent(), id++), box(left + x, 130, 18, 18).with_custom_background(opaque));
      label(fmt::format("{}  RGB {}, {}, {}", i + 1, inputs[i].r, inputs[i].g, inputs[i].b),
            x + 28, 126, 254, 28, 19, white);
    }
    label("Alpha 170 / 255 = 66.7%", 858, 126, 286, 28, 19, white);
    label("Draw order: 1 > 2 > 3  |  radius 40px  |  centers (-24,-14), (24,-14), (0,22)",
          0, 158, 856, 26, 18, muted);
    label("Destination RGB 28, 30, 40", 858, 158, 286, 26, 18, muted);

    static const std::array<ModeSwatch, 4> swatches{{
        {"Alpha", "S*a + D*(1-a)", blend::Mode::Alpha},
        {"Additive", "S*a + D", blend::Mode::Additive},
        {"Multiply", "S*D + D*(1-a)", blend::Mode::Multiplied},
        {"Premultiplied alpha", "S + D*(1-a)", blend::Mode::AlphaPremultiply},
    }};
    for (size_t index = 0; index < swatches.size(); ++index) {
      const auto &swatch = swatches[index];
      const float x = static_cast<float>(index) * 290.f;
      const bool prepare = index == 3 && prepare_rgb;
      const auto model = expected_center(swatch.mode, prepare);
      div(context, mk(root.ent(), id++), box(left + x, 194, 274, 270)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(10 * s)
          .with_debug_name(fmt::format("bml_col_{}", index)));
      label(swatch.caption, x + 14, 203, 246, 31, index == 3 ? 23.f : 25.f, white, fmt::format("bml_cap_{}", index));
      label(swatch.formula, x + 14, 237, 246, 23, 19, muted);
      // Three overlapping discs, drawn in this swatch's mode. Overlap is what
      // makes the modes tell each other apart.
      div(context, mk(root.ent(), id++), box(left + x + 12, 264, 250, 160)
          .with_custom_background(backing)
          .with_on_draw_fg([this, mode = swatch.mode, index, prepare, s](RectangleType rr) {
            const size_t before = blend::transitions_this_frame();
            {
              afterhours::blend_scope scope(mode);
              const float r = 40.f * s;
              const float cx = rr.x + rr.width * .5f;
              const float cy = rr.y + 67.f * s;
              const std::array<float, 3> dx{-.6f, .6f, 0.f};
              const std::array<float, 3> dy{-.35f, -.35f, .55f};
              for (size_t i = 0; i < inputs.size(); ++i) {
                const auto color = source_color(i, prepare);
                afterhours::draw_circle(static_cast<int>(cx + r * dx[i]),
                                       static_cast<int>(cy + r * dy[i]), r, color);
                afterhours::draw_rectangle({rr.x + 4 * s, rr.y + 136 * s, 36 * s, 20 * s}, color);
              }
            }
            panel_transitions[index] = blend::transitions_this_frame() - before;
          }).with_debug_name(fmt::format("bml_swatch_{}", index)));
      label("1", x + 51, 280, 20, 22, 17, muted);
      label("2", x + 210, 280, 20, 22, 17, muted);
      label("3", x + 184, 377, 20, 22, 17, muted);
      label(fmt::format("RGB ~{}, {}, {}", model.r, model.g, model.b), x + 59, 398, 200, 26, 18, white,
            fmt::format("bml_center_{}", index));
      label(fmt::format("{} changes / center replay", last_panel_transitions[index]),
            x + 14, 432, 246, 24, 17, muted, fmt::format("bml_mode_count_{}", index));
    }

    // Same eight blits twice: once wrapped individually, once under one scope.
    // The captions say what each costs.
    for (int which = 0; which < 2; ++which) {
      const float x = which * 580.f;
      const bool hoisted = which == 1;
      div(context, mk(root.ent(), id++), box(left + x, 482, 564, 146)
          .with_custom_background({32, 38, 51, 255}).with_border(border, s).with_corner_radius(10 * s)
          .with_debug_name(fmt::format("bml_cmp_{}", which)));
      label(hoisted ? "One scope for the batch" : "One scope per blit", x + 16, 491, 532, 29, 24, white,
            fmt::format("bml_cmpcap_{}", which));
      label(hoisted ? fmt::format("8 blits / {} changes (2 expected)", last_panel_transitions[5])
                    : fmt::format("8 blits x 2 = {} changes", last_panel_transitions[4]),
            x + 16, 524, 532, 23, 19, muted, fmt::format("bml_batch_count_{}", which));
      div(context, mk(root.ent(), id++), box(left + x + 16, 549, 532, 48)
          .with_custom_background(backing)
          .with_on_draw_fg([this, hoisted, which, s](RectangleType rr) {
            const float pitch = rr.width / 8;
            const int brackets = hoisted ? 1 : 8;
            for (int i = 0; i < brackets; ++i) {
              const float x1 = rr.x + (hoisted ? 0.f : i * pitch) + 5 * s;
              const float width = (hoisted ? rr.width : pitch) - 10 * s;
              afterhours::draw_rectangle({x1, rr.y + s, width, s}, {146, 165, 194, 255});
              afterhours::draw_rectangle({x1, rr.y + s, s, 5 * s}, {146, 165, 194, 255});
              afterhours::draw_rectangle({x1 + width - s, rr.y + s, s, 5 * s}, {146, 165, 194, 255});
            }
            const auto draw_one = [&](int i) {
              afterhours::draw_rectangle({rr.x + i * pitch + 9 * s, rr.y + 12 * s, pitch - 18 * s, 30 * s},
                                        {120, 200, 255, 150});
            };
            const size_t before = blend::transitions_this_frame();
            if (hoisted) {
              afterhours::blend_scope outer(blend::Mode::Additive);
              for (int i = 0; i < 8; ++i) {
                afterhours::blend_scope inner(blend::Mode::Additive);
                draw_one(i);
              }
            } else {
              for (int i = 0; i < 8; ++i) {
                afterhours::blend_scope each(blend::Mode::Additive);
                draw_one(i);
              }
            }
            panel_transitions[static_cast<size_t>(4 + which)] = blend::transitions_this_frame() - before;
          }).with_debug_name(fmt::format("bml_cmpbox_{}", which)));
      for (int i = 0; i < 8; ++i)
        label(std::to_string(i + 1), x + 42 + i * 66.5f, 601, 34, 21, 16, muted);
    }
    label("Batch output should match. Hoisting the scope saves 14 blend-state changes across the same eight blits.",
          0, 641, 1144, 25, 20, white);
    label(prepare_rgb ? "AlphaPremultiply: RGB is multiplied by alpha before drawing. Compare against the Alpha panel."
                      : "AlphaPremultiply: raw RGB is intentionally unprepared. Turn preparation on to compare the correct input.",
          0, 668, 1144, 25, 18, muted, "bml_contract");
    label("S and D are normalized RGB. Small swatches replay the center overlap; RGB labels are rounded model values.",
          0, 693, 1144, 22, 17, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(blend_mode_lab, "System Demos",
                        "blend::Mode, blend_scope and the transition counter",
                        BlendModeLab)
