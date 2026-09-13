#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Repro for the one gap both floatinghotel and hanabi (#15) filed independently:
// a low-alpha div background is reported to render OPAQUE, so a translucent
// selection wash over text is impossible and both apps drew an opaque box and
// re-drew the text on top.
//
// Read it as: every row has the same text under the same overlay. If alpha is
// honoured the text stays legible through the wash and the swatches step from
// faint to solid. If it is not, the text vanishes and every swatch looks the
// same. Two spellings are covered because the reports name both: alpha in the
// Color, and with_opacity on the overlay.
struct AlphaBlendRepro : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = context.screen_height / 720.f;
    const float left = (context.screen_width / scale - 1168.f) / 2.f;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{178, 188, 209, 255};
    const auto base = afterhours::Color{24, 29, 42, 255};
    const auto blue = afterhours::Color{90, 150, 255, 255};
    const auto outline = afterhours::Color{68, 81, 104, 255};
    const std::string specimen = "the quick brown fox jumps over the lazy dog";
    const auto box = [scale](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale)
          .with_background(Theme::Usage::None)
          .with_corner_radius(0)
          .with_ignore_pointer_events();
    };
    auto root = div(context, mk(entity), box(0, 0, context.screen_width / scale, 720)
        .with_custom_background({15, 19, 28, 255}).with_debug_name("ab_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float h, float size, afterhours::Color color,
                           const std::string &name, bool emphasis = false) {
      div(context, mk(root.ent(), id++), box(left + x, y, w, h)
          .with_label(text).with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * scale))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_debug_name(name));
    };
    label("Alpha blend / two ways to draw the same wash", 0, 26, 1168, 42, 34, white, "ab_title");
    label("Low opacity reveals the text. At full opacity, the overlay must hide it completely.",
          0, 76, 1168, 28, 22, muted, "ab_expectation");
    div(context, mk(root.ent(), id++), box(left, 118, 24, 24).with_custom_background(blue));
    label("Overlay RGB 90, 150, 255", 34, 116, 290, 28, 19, white, "ab_overlay_color");
    label("Backing #181D2A", 360, 116, 240, 28, 19, muted, "ab_backing");
    label("Text #EBF0FA", 664, 116, 230, 28, 19, muted, "ab_foreground");
    const unsigned char alphas[] = {0, 32, 64, 128, 153, 200, 255};
    const float opacities[] = {0.f, .15f, .35f, 128.f / 255.f, .6f, .85f, 1.f};
    for (int method = 0; method < 2; ++method) {
      const float x = method == 0 ? 0.f : 600.f;
      div(context, mk(root.ent(), id++), box(left + x, 160, 568, 422)
          .with_custom_background({21, 27, 39, 255}).with_border(outline, scale));
      label(method == 0 ? "01  Color alpha" : "02  Overlay opacity", x + 16, 172, 536, 30, 24, white,
            method == 0 ? "ab_alpha_title" : "ab_opacity_title");
      label(method == 0 ? "with_custom_background" : "with_opacity", x + 16, 207, 536, 25, 16, muted,
            method == 0 ? "ab_alpha_api" : "ab_opacity_api", true);
      label(method == 0 ? "Alpha / 255" : "Opacity", x + 16, 242, 104, 24, 17, muted,
            method == 0 ? "ab_alpha_units" : "ab_opacity_units");
      label("Text under blue overlay", x + 126, 242, 420, 24, 18, muted,
            method == 0 ? "ab_alpha_sample" : "ab_opacity_sample");
      for (int row = 0; row < 7; ++row) {
        const float y = 277.f + row * 42.f;
        const auto suffix = method == 0 ? fmt::format("a{}", static_cast<int>(alphas[row]))
                                       : fmt::format("o{:.0f}", opacities[row] * 100);
        label(method == 0 ? fmt::format("{}", static_cast<int>(alphas[row]))
                          : fmt::format(row == 3 ? "{:.1f}%" : "{:.0f}%", opacities[row] * 100),
              x + 16, y, 104, 32, 18, row == 3 || row == 4 ? blue : white, "ab_label_" + suffix, true);
        // The content that must remain visible underneath.
        auto under = div(context, mk(root.ent(), id++), box(left + x + 126, y, 426, 32)
            .with_custom_background(base).with_debug_name("ab_under_" + suffix));
        div(context, mk(under.ent()), box(12, 0, 402, 32).with_label(specimen)
            .with_font("AtkinsonMock", pixels(18 * scale)).with_alignment(TextAlignment::Left)
            .with_custom_text_color(white));
        // The overlay, drawn after and on top of the same span.
        auto overlay = method == 0
            ? box(left + x + 126, y, 426, 32).with_custom_background({90, 150, 255, alphas[row]})
            : box(left + x + 126, y, 426, 32).with_custom_background(blue).with_opacity(opacities[row]);
        div(context, mk(root.ent(), id++), overlay.with_debug_name("ab_over_" + suffix));
      }
    }
    label("Reference: " + specimen, 0, 588, 594, 25, 18, white, "ab_reference_note");
    label("255 / 100% = expected complete occlusion", 616, 588, 536, 25, 18, muted, "ab_opaque_note");
    label("Matched pairs: 128 / 255 = 50.2%; 153 / 255 = 60%. Both methods should look identical.",
          0, 627, 1168, 27, 21, white, "ab_equivalence");
    label("Observe: gradual blue steps and fading text. Solid blue at every value indicates a blending failure.",
          0, 658, 1168, 26, 19, muted, "ab_observe");
    label(fmt::format("Raylib renderer  |  {} x {}  |  opaque backing  |  source-over compositing",
                      static_cast<int>(context.screen_width), static_cast<int>(context.screen_height)),
          0, 689, 1168, 23, 16, muted, "ab_context");
  }
};

REGISTER_EXAMPLE_SCREEN(alpha_blend_repro, "System Demos",
                        "Translucent overlays over text (open cross-project gap)",
                        AlphaBlendRepro)
