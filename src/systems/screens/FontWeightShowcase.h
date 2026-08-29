#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// with_font_weight across the whole enum. floatinghotel wanted semi-bold diff
// headers; this is the screen that says whether the weights are distinguishable
// with the fonts actually loaded, which is the part a unit test cannot answer.
struct FontWeightShowcase : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};

    struct Row {
      const char *name;
      afterhours::colors::FontWeight weight;
    };
    // Archivo Narrow ships no Light face, so Light resolves to Regular and
    // warns. Said out loud here, or the row reads as the feature not working.
    const Row rows[] = {
        {"Light 300 (no face, falls back)",
         afterhours::colors::FontWeight::Light},
        {"Regular 400", afterhours::colors::FontWeight::Regular},
        {"Medium 500", afterhours::colors::FontWeight::Medium},
        {"SemiBold 600", afterhours::colors::FontWeight::SemiBold},
        {"Bold 700", afterhours::colors::FontWeight::Bold},
    };

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 24.f)
            .with_label("Font Weight - Archivo Narrow, one string per weight")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("fw_title"));

    int idx = 1;
    float y = 74.f;
    for (const Row &r : rows) {
      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(230), pixels(44)})
              .with_absolute_position(48.f, y)
              .with_label(r.name)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(15.f)
              .with_debug_name(fmt::format("fw_name_{}", r.name)));

      div(context, mk(entity, idx++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(900), pixels(44)})
              .with_absolute_position(288.f, y)
              .with_label("Sphinx of black quartz, judge my vow 0123")
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(white)
              .with_font("Archivo", pixels(22.f))
              .with_font_weight(r.weight)
              .with_debug_name(fmt::format("fw_sample_{}", r.name)));
      y += 52.f;
    }

    // The case that motivated the request: a heavier run has to sit next to a
    // regular one on the same row and still line up.
    auto row = div(context, mk(entity, idx++),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(1180), pixels(40)})
                       .with_absolute_position(48.f, y + 24.f)
                       .with_flex_direction(FlexDirection::Row)
                       .with_transparent_bg()
                       .with_debug_name("fw_mixed"));
    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(150), percent(1.f)})
            .with_label("theme.h")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_font("Archivo", pixels(18.f))
            .with_font_weight(afterhours::colors::FontWeight::SemiBold)
            .with_transparent_bg()
            .with_debug_name("fw_mixed_bold"));
    div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.f)})
            .with_label("src/ui  -  regular weight, same baseline")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font("Archivo", pixels(18.f))
            .with_transparent_bg()
            .with_debug_name("fw_mixed_regular"));
  }
};

REGISTER_EXAMPLE_SCREEN(font_weight, "Component Galleries",
                        "Every FontWeight, plus mixed weights on one row",
                        FontWeightShowcase)
