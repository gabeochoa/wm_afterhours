#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Resizable split panes (D30). Drag the vertical bar to resize the sidebar,
// the horizontal one to resize the preview -- nested panes divide only the
// region they sit in.
struct SplitPaneLab : ScreenSystem<UIContext<InputAction>> {
  // The panes only own geometry; the ratios live here, so the screen decides
  // the limits. hsplit_pane clamps to [0, 1] and nothing narrower.
  float sidebar_ratio = 0.28f;
  float preview_ratio = 0.62f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto bar = afterhours::Color{96, 118, 168, 255};
    const auto panel_a = afterhours::Color{46, 56, 80, 255};
    const auto panel_b = afterhours::Color{28, 34, 50, 255};
    const auto panel_c = afterhours::Color{18, 21, 32, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label("Split Pane Lab - drag either divider")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("sp_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(48.f, 50.f)
            .with_label(fmt::format("sidebar {:.0f}%   preview {:.0f}%",
                                    sidebar_ratio * 100.f,
                                    preview_ratio * 100.f))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("sp_readout"));

    // The limits go to the widget so it pins mid drag; clamping after the call
    // let the bar overshoot and snap back on alternate frames.
    auto [sidebar, vbar, content] = hsplit_pane(
        context, mk(entity, 2), sidebar_ratio,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(560)})
            .with_absolute_position(48.f, 84.f)
            .with_custom_background(panel_c)
            .with_roundness(0.f)
            .with_debug_name("sp_outer"),
        0.15f, 0.55f);
    // restyle carries colour but not roundness -- roundness only lands
    // alongside an explicit corner set, so it is not worth passing here.
    vbar.restyle(context, ComponentConfig{}.with_custom_background(bar));

    sidebar.restyle(context, ComponentConfig{}.with_custom_background(panel_a));
    div(context, mk(sidebar.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_label("sidebar")
            .with_custom_text_color(white)
            .with_transparent_bg()
            .with_debug_name("sp_sidebar_label"));

    auto [preview, hbar, details] =
        vsplit_pane(context, mk(content.ent(), 0), preview_ratio,
                    ComponentConfig{}.with_debug_name("sp_inner"), 0.2f, 0.85f);
    hbar.restyle(context, ComponentConfig{}.with_custom_background(bar));

    preview.restyle(context, ComponentConfig{}.with_custom_background(panel_b));
    div(context, mk(preview.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_label("preview")
            .with_custom_text_color(white)
            .with_transparent_bg()
            .with_debug_name("sp_preview_label"));

    details.restyle(context, ComponentConfig{}.with_custom_background(panel_a));
    div(context, mk(details.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_label("details")
            .with_custom_text_color(white)
            .with_transparent_bg()
            .with_debug_name("sp_details_label"));

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(48.f, 656.f)
            .with_label("the cursor turns into a resize arrow over each bar")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("sp_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(split_pane_lab, "System Demos",
                        "Resizable split panes with draggable dividers",
                        SplitPaneLab)
