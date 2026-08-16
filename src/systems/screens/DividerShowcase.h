#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// divider() on its own, away from split_pane. cartographer asked for this so it
// would stop hand-rolling `div` with percent(1) x pixels(1) plus a margin.
struct DividerShowcase : ScreenSystem<UIContext<InputAction>> {
  float left_w = 300.f;
  float top_h = 150.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto bar = afterhours::Color{96, 118, 168, 255};
    const auto panel = afterhours::Color{34, 41, 60, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label("Divider - drag either bar; a plain rule at the bottom")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("dv_title"));

    // Axis::X: the bar moves horizontally, so it is the vertical one.
    auto row = div(context, mk(entity, 1),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(1180), pixels(240)})
                       .with_absolute_position(48.f, 64.f)
                       .with_flex_direction(FlexDirection::Row)
                       .with_custom_background(panel)
                       .with_debug_name("dv_row"));

    left_w = std::clamp(left_w, 120.f, 900.f);
    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(left_w), percent(1.f)})
            .with_label(fmt::format("left {:.0f}px", left_w))
            .with_custom_background(afterhours::Color{46, 56, 80, 255})
            .with_custom_text_color(white)
            .with_font_size(16.f)
            .with_debug_name("dv_left"));

    if (auto d = divider(context, mk(row.ent(), 1), Axis::X,
                         ComponentConfig{}
                             .with_custom_background(bar)
                             .with_debug_name("dv_vbar")))
      left_w += d.as<float>();

    div(context, mk(row.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.f)})
            .with_label("right fills the rest")
            .with_custom_background(afterhours::Color{28, 34, 50, 255})
            .with_custom_text_color(white)
            .with_font_size(16.f)
            .with_debug_name("dv_right"));

    auto col = div(context, mk(entity, 2),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(1180), pixels(280)})
                       .with_absolute_position(48.f, 320.f)
                       .with_flex_direction(FlexDirection::Column)
                       .with_custom_background(panel)
                       .with_debug_name("dv_col"));

    top_h = std::clamp(top_h, 60.f, 220.f);
    div(context, mk(col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(top_h)})
            .with_label(fmt::format("top {:.0f}px", top_h))
            .with_custom_background(afterhours::Color{46, 56, 80, 255})
            .with_custom_text_color(white)
            .with_font_size(16.f)
            .with_debug_name("dv_top"));

    if (auto d = divider(context, mk(col.ent(), 1), Axis::Y,
                         ComponentConfig{}
                             .with_custom_background(bar)
                             .with_debug_name("dv_hbar")))
      top_h += d.as<float>();

    div(context, mk(col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), expand()})
            .with_label("bottom fills the rest")
            .with_custom_background(afterhours::Color{28, 34, 50, 255})
            .with_custom_text_color(white)
            .with_font_size(16.f)
            .with_debug_name("dv_bottom"));

    // Not every divider is draggable -- a 1px rule is the common case, and it
    // is the same widget with the drag simply ignored.
    divider(context, mk(entity, 3), Axis::Y,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1180), pixels(1)})
                .with_absolute_position(48.f, 624.f)
                .with_custom_background(afterhours::Color{70, 82, 112, 255})
                .with_debug_name("dv_rule"));

    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(48.f, 636.f)
            .with_label("the rule above is divider() with its movement ignored")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("dv_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(divider_lab, "System Demos",
                        "divider() as a draggable bar and as a plain rule",
                        DividerShowcase)
