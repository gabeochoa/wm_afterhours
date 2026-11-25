#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

struct BaselineAlignShowcase
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    const afterhours::Color bg{244, 246, 248, 255};
    const afterhours::Color card{255, 255, 255, 255};
    const afterhours::Color border{206, 214, 222, 255};
    const afterhours::Color ink{26, 36, 48, 255};
    const afterhours::Color muted{84, 98, 114, 255};
    const afterhours::Color accent{28, 100, 168, 255};
    const afterhours::Color tag_bg{228, 238, 248, 255};

    ctx.theme = Theme{};
    ctx.theme.background = bg;
    ctx.theme.surface = card;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = accent;
    ctx.scaling_mode = ScalingMode::Proportional;

    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(
        ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(1280.f * s), pixels(720.f * s)})
            .with_absolute_position((ctx.screen_width - 1280.f * s) / 2.f,
                                    (ctx.screen_height - 720.f * s) / 2.f)
            .with_custom_background(bg)
            .with_padding(Padding{.top = pixels(36.f * s),
                                  .left = pixels(72.f * s),
                                  .bottom = pixels(36.f * s),
                                  .right = pixels(72.f * s)})
            .with_flex_direction(FlexDirection::Column)
            .with_gap(pixels(14.f * s))
            .with_corner_radius(0)
            .with_debug_name("baseline_page"));

    div(ctx, mk(page.ent(), 1),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(52.f * s)})
            .with_label("Row baseline alignment")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMockBold", pixels(38.f * s))
            .with_custom_text_color(ink)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    div(ctx, mk(page.ent(), 2),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(28.f * s)})
            .with_label(
                "Mixed text sizes (16 / 48 / 24 / 15 px) under FlexStart, "
                "Center, and Baseline.")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMock", pixels(19.f * s))
            .with_custom_text_color(muted)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    struct SampleSpec {
      const char *caption;
      const char *name;
      AlignItems align;
    };
    const std::array<SampleSpec, 3> samples{{
        {"01   AlignItems::FlexStart", "row_start", AlignItems::FlexStart},
        {"02   AlignItems::Center", "row_center", AlignItems::Center},
        {"03   AlignItems::Baseline", "row_baseline", AlignItems::Baseline},
    }};

    for (size_t i = 0; i < samples.size(); ++i) {
      auto box = div(
          ctx, mk(page.ent(), 10 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size({pixels(820.f * s), children()})
              .with_flex_direction(FlexDirection::Column)
              .with_gap(pixels(6.f * s))
              .with_padding(Padding{.top = pixels(12.f * s),
                                    .left = pixels(20.f * s),
                                    .bottom = pixels(14.f * s),
                                    .right = pixels(20.f * s)})
              .with_custom_background(card)
              .with_border(samples[i].align == AlignItems::Baseline ? accent
                                                                    : border,
                           pixels(samples[i].align == AlignItems::Baseline
                                      ? 2.f
                                      : 1.f))
              .with_corner_radius(8.f * s)
              .with_ignore_pointer_events());

      div(ctx, mk(box.ent(), 0),
          ComponentConfig{}
              .with_size({percent(1.f), pixels(24.f * s)})
              .with_label(samples[i].caption)
              .with_alignment(TextAlignment::Left)
              .with_font("AtkinsonMockBold", pixels(16.f * s))
              .with_custom_text_color(
                  samples[i].align == AlignItems::Baseline ? accent : muted)
              .with_transparent_bg()
              .with_ignore_pointer_events());

      auto row = div(ctx, mk(box.ent(), 1),
                     ComponentConfig{}
                         .with_size({percent(1.f), children()})
                         .with_flex_direction(FlexDirection::Row)
                         .with_align_items(samples[i].align)
                         .with_gap(pixels(12.f * s))
                         .with_transparent_bg()
                         .with_debug_name(samples[i].name));

      auto item = [&](int id, const char *text, float pt, float w, float h,
                      afterhours::Color color, bool bold, const char *dname) {
        div(ctx, mk(row.ent(), id),
            ComponentConfig{}
                .with_size({pixels(w * s), pixels(h * s)})
                .with_label(text)
                .with_alignment(TextAlignment::Left)
                .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",
                           pixels(pt * s))
                .with_custom_text_color(color)
                .with_custom_background(tag_bg)
                .with_corner_radius(4.f * s)
                .with_ignore_pointer_events()
                .with_debug_name(dname));
      };

      item(0, "USD", 16.f, 46.f, 24.f, muted, true, "lbl_usd");
      item(1, "42.50", 48.f, 150.f, 58.f, ink, true, "lbl_price");
      item(2, "/ month", 24.f, 108.f, 34.f, accent, false, "lbl_unit");
      item(3, "billed annually", 15.f, 130.f, 24.f, muted, false, "lbl_note");
    }
  }
};

REGISTER_EXAMPLE_SCREEN(baseline_align, "Layout",
                        "Row baseline alignment for mixed-size text",
                        BaselineAlignShowcase)
