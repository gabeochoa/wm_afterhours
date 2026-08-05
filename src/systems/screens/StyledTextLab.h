#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Per-span font weight in styled labels (gap D31).
//
// Weight resolves as base + "@bold" through FontManager::resolve_weighted, so
// this screen uses DGOne / DGOne@bold -- the only genuine same-family weight
// pair bundled. Every other block here uses the default face, which has no
// weighted variant registered, and therefore renders regular no matter what
// weight a span asks for. That fallback is deliberate and is itself worth
// seeing: it is what lets an app adopt weights one font at a time.
struct StyledTextLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr float FS = 20.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto box = afterhours::Color{60, 70, 100, 255};
    const auto amber = afterhours::Color{240, 200, 110, 255};
    const auto green = afterhours::Color{120, 220, 150, 255};
    using afterhours::colors::FontWeight;

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(700), pixels(36)})
            .with_absolute_position(48.f, 24.f)
            .with_label("Styled Text Lab - per-span font weight")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("st_title"));

    int id = 1;
    auto caption = [&](float x, float y, const std::string &text,
                       float w = 420.f) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(w), pixels(28)})
              .with_absolute_position(x, y)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_debug_name(fmt::format("st_caption_{}", id)));
    };

    // --- The one that actually shows a weight change: DGOne has @bold.
    caption(48.f, 76.f, "DGOne + DGOne@bold - a real weight pair");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(560), pixels(60)})
            .with_absolute_position(48.f, 104.f)
            .with_font("DGOne", FS)
            .with_styled_label({
                TextSpan{"regular ", white, FontWeight::Regular},
                TextSpan{"BOLD", amber, FontWeight::Bold},
                TextSpan{" regular again", white, FontWeight::Regular},
            })
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .disable_rounded_corners()
            .with_debug_name("st_real_pair"));

    // --- Weight is independent of colour: same colour, different weights.
    caption(48.f, 176.f, "same colour, different weight (not a colour cue)");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(560), pixels(60)})
            .with_absolute_position(48.f, 204.f)
            .with_font("DGOne", FS)
            .with_styled_label({
                TextSpan{"one colour ", white, FontWeight::Regular},
                TextSpan{"two weights", white, FontWeight::Bold},
            })
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .disable_rounded_corners()
            .with_debug_name("st_same_colour"));

    // --- Bold inside a wrapping paragraph. Bold glyphs are wider, so the
    // wrapper has to measure each run at its own weight or the line overruns.
    caption(48.f, 276.f, "bold inside a wrapping paragraph");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(360), pixels(140)})
            .with_absolute_position(48.f, 304.f)
            .with_font("DGOne", pixels(16.f))
            .with_styled_label({
                TextSpan{"the quick brown ", white, FontWeight::Regular},
                TextSpan{"fox jumps over", green, FontWeight::Bold},
                TextSpan{" the lazy dog and keeps running", white,
                         FontWeight::Regular},
            })
            .with_text_overflow(TextOverflow::Wrap)
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .disable_rounded_corners()
            .with_debug_name("st_wrap_bold"));

    // --- Weight survives a hard break, the markdown/diff shape.
    caption(440.f, 276.f, "weight across a hard \\n");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(360), pixels(140)})
            .with_absolute_position(440.f, 304.f)
            .with_font("DGOne", pixels(16.f))
            .with_styled_label({
                TextSpan{"Heading\n", amber, FontWeight::Bold},
                TextSpan{"body text on the next line\n", white,
                         FontWeight::Regular},
                TextSpan{"Another heading\n", amber, FontWeight::Bold},
                TextSpan{"more body text", white, FontWeight::Regular},
            })
            .with_text_overflow(TextOverflow::Wrap)
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .disable_rounded_corners()
            .with_debug_name("st_break_bold"));

    // --- The fallback: default face has no @bold registered, so a Bold span
    // renders regular rather than failing to find a font.
    caption(48.f, 464.f,
            "default face has no @bold: Bold spans fall back to regular");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(560), pixels(60)})
            .with_absolute_position(48.f, 492.f)
            .with_font_size(FS)
            .with_styled_label({
                TextSpan{"asks for regular ", white, FontWeight::Regular},
                TextSpan{"asks for bold", amber, FontWeight::Bold},
            })
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .disable_rounded_corners()
            .with_debug_name("st_fallback"));
  }
};

REGISTER_EXAMPLE_SCREEN(styled_text_lab, "System Demos",
                        "Per-span font weight in styled labels",
                        StyledTextLab)
