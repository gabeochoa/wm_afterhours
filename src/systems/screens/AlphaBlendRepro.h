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

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto base = afterhours::Color{24, 29, 42, 255};
    const unsigned char steps[] = {32, 64, 128, 200, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 18.f)
            .with_label("Alpha Blend - the text should stay readable through "
                        "the wash")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("ab_title"));

    int idx = 1;
    float y = 62.f;

    const auto caption = [&](const char *text, float yy) {
      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1180), pixels(22)})
              .with_absolute_position(48.f, yy)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_debug_name(fmt::format("ab_cap_{}", idx)));
    };

    caption("1. alpha in the Color: with_custom_background({90,150,255,a})", y);
    y += 26.f;
    for (unsigned char a : steps) {
      // The content that must remain visible underneath.
      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1080), pixels(38)})
              .with_absolute_position(48.f, y)
              .with_label("the quick brown fox jumps over the lazy dog")
              .with_alignment(TextAlignment::Left)
              .with_custom_background(base)
              .with_custom_text_color(white)
              .with_font_size(15.f)
              .with_roundness(0.f)
              .with_debug_name(fmt::format("ab_under_a{}", (int)a)));

      // The overlay, drawn after and on top of the same span.
      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1080), pixels(38)})
              .with_absolute_position(48.f, y)
              .with_custom_background(afterhours::Color{90, 150, 255, a})
              .with_roundness(0.f)
              .with_debug_name(fmt::format("ab_over_a{}", (int)a)));

      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(44), pixels(38)})
              .with_absolute_position(1136.f, y)
              .with_label(fmt::format("{}", (int)a))
              .with_custom_text_color(muted)
              .with_font_size(12.f)
              .with_transparent_bg()
              .with_debug_name(fmt::format("ab_label_a{}", (int)a)));
      y += 42.f;
    }

    y += 12.f;
    caption("2. with_opacity on an otherwise solid overlay", y);
    y += 26.f;
    const float opacities[] = {0.15f, 0.35f, 0.6f, 0.85f, 1.0f};
    for (float o : opacities) {
      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1080), pixels(38)})
              .with_absolute_position(48.f, y)
              .with_label("the quick brown fox jumps over the lazy dog")
              .with_alignment(TextAlignment::Left)
              .with_custom_background(base)
              .with_custom_text_color(white)
              .with_font_size(15.f)
              .with_roundness(0.f)
              .with_debug_name(fmt::format("ab_under_o{:.0f}", o * 100)));

      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1080), pixels(38)})
              .with_absolute_position(48.f, y)
              .with_custom_background(afterhours::Color{90, 150, 255, 255})
              .with_opacity(o)
              .with_roundness(0.f)
              .with_debug_name(fmt::format("ab_over_o{:.0f}", o * 100)));

      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(44), pixels(38)})
              .with_absolute_position(1136.f, y)
              .with_label(fmt::format("{:.0f}%", o * 100))
              .with_custom_text_color(muted)
              .with_font_size(12.f)
              .with_transparent_bg()
              .with_debug_name(fmt::format("ab_label_o{:.0f}", o * 100)));
      y += 42.f;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(alpha_blend_repro, "System Demos",
                        "Translucent overlays over text (open cross-project gap)",
                        AlphaBlendRepro)
