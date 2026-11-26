#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

struct TextScaleShowcase
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  size_t preset = 2; // 1.25x default so the screenshot shows scaled text
  std::string sample_input = "Editable note";
  int action_clicks = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    static constexpr std::array<float, 4> SCALES{0.85f, 1.0f, 1.25f, 1.5f};
    static constexpr std::array<const char *, 4> LABELS{"0.85x", "1.0x",
                                                        "1.25x", "1.5x"};

    const afterhours::Color bg{244, 246, 248, 255};
    const afterhours::Color card{255, 255, 255, 255};
    const afterhours::Color border{206, 214, 222, 255};
    const afterhours::Color ink{26, 36, 48, 255};
    const afterhours::Color muted{84, 98, 114, 255};
    const afterhours::Color accent{28, 100, 168, 255};
    const afterhours::Color btn_idle{232, 237, 242, 255};

    ctx.theme = Theme{};
    ctx.theme.background = bg;
    ctx.theme.surface = card;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = accent;
    ctx.theme.text_scale = SCALES[preset];
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
            .with_debug_name("ts_page"));

    div(ctx, mk(page.ent(), 1),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(68.f * s)})
            .with_label("Global text scale")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMockBold", pixels(34.f * s))
            .with_custom_text_color(ink)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    auto controls = div(ctx, mk(page.ent(), 2),
                        ComponentConfig{}
                            .with_size({pixels(560.f * s), pixels(46.f * s)})
                            .with_flex_direction(FlexDirection::Row)
                            .with_gap(pixels(10.f * s))
                            .with_transparent_bg()
                            .with_debug_name("ts_controls"));

    for (size_t i = 0; i < SCALES.size(); ++i) {
      const bool active = i == preset;
      if (button(ctx, mk(controls.ent(), static_cast<int>(i)),
                 ComponentConfig{}
                     .with_size({pixels(120.f * s), pixels(44.f * s)})
                     .with_label(LABELS[i])
                     .with_font("AtkinsonMockBold", pixels(17.f * s))
                     .with_custom_background(active ? accent : btn_idle)
                     .with_custom_text_color(active ? card : ink)
                     .with_corner_radius(6.f * s)
                     .with_debug_name("ts_scale_" + std::to_string(i)))) {
        preset = i;
      }
    }

    auto panel = div(
        ctx, mk(page.ent(), 3),
        ComponentConfig{}
            .with_size({pixels(760.f * s), children()})
            .with_flex_direction(FlexDirection::Column)
            .with_gap(pixels(12.f * s))
            .with_padding(Padding::all(pixels(22.f * s)))
            .with_custom_background(card)
            .with_border(border, pixels(1.f))
            .with_corner_radius(10.f * s)
            .with_debug_name("ts_panel"));

    div(ctx, mk(panel.ent(), 0),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(44.f * s)})
            .with_label("Accessibility text scaling")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMockBold", pixels(22.f * s))
            .with_custom_text_color(ink)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    div(ctx, mk(panel.ent(), 1),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(92.f * s)})
            .with_label(
                "Theme::text_scale multiplies resolved font sizes across "
                "labels, inputs, and tooltips while leaving container "
                "dimensions unchanged.")
            .with_alignment(TextAlignment::Left)
            .with_text_overflow(TextOverflow::Wrap)
            .with_font("AtkinsonMock", pixels(16.f * s))
            .with_custom_text_color(muted)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    text_input(ctx, mk(panel.ent(), 2), sample_input,
               ComponentConfig{}
                   .with_size({pixels(420.f * s), pixels(48.f * s)})
                   .with_font("AtkinsonMock", pixels(16.f * s))
                   .with_custom_background(bg)
                   .with_custom_text_color(ink)
                   .with_border(border, pixels(1.f))
                   .with_corner_radius(6.f * s)
                   .with_debug_name("ts_input"));

    if (button(ctx, mk(panel.ent(), 3),
               ComponentConfig{}
                   .with_size({pixels(220.f * s), pixels(46.f * s)})
                   .with_label("Apply (" + std::to_string(action_clicks) + ")")
                   .with_font("AtkinsonMockBold", pixels(16.f * s))
                   .with_custom_background(accent)
                   .with_custom_text_color(card)
                   .with_corner_radius(6.f * s)
                   .with_debug_name("ts_apply"))) {
      ++action_clicks;
    }

    div(ctx, mk(page.ent(), 4),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(36.f * s)})
            .with_label(std::string("Current text_scale: ") + LABELS[preset])
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMock", pixels(16.f * s))
            .with_custom_text_color(muted)
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_debug_name("ts_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_scale, "Text",
                        "Global Theme::text_scale accessibility scaling",
                        TextScaleShowcase)
