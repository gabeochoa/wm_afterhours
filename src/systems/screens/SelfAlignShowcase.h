#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Showcase for the new SelfAlign feature
struct SelfAlignShowcase : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::sage_natural();
    theme.background = afterhours::Color{35, 40, 45, 255};
    theme.surface = afterhours::Color{50, 55, 60, 255};
    context.theme = theme;

    int sw = Settings::get().get_screen_width();
    int sh = Settings::get().get_screen_height();

    // Main centered container
    auto main = vstack(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((float)sw), pixels((float)sh)})
            .with_background(Theme::Usage::Background)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::Center)
            .with_debug_name("main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_label("SelfAlign Feature Demo")
            .with_size(ComponentSize{pixels(500), pixels(50)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_alignment(TextAlignment::Center)
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_skip_tabbing(true));

    // Subtitle
    div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_label("Override parent alignment for individual elements")
            .with_size(ComponentSize{pixels(704), pixels(48)})
            .with_custom_text_color(afterhours::Color{180, 180, 180, 255})
            .with_alignment(TextAlignment::Center)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Margin{.top = DefaultSpacing::tiny(), .bottom = DefaultSpacing::small(),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true));

    // Demo container - a column, so children stack vertically
    // but can use SelfAlign to control their horizontal position
    // Using explicit pixel values for spacing instead of padding
    // to avoid layout calculation issues with SelfAlign
    float container_width = 600.0f;
    float container_height = 224.0f;

    auto demo = vstack(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(container_width), pixels(container_height)})
            .with_custom_background(afterhours::Color{45, 50, 55, 255})
            .with_border(afterhours::Color{100, 110, 120, 255}, 2.0f)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_debug_name("demo"));

    // FlexStart - aligns left with margin for visual spacing from edge
    div(context, mk(demo.ent(), 0),
        ComponentConfig{}
            .with_label("FlexStart (left)")
            .with_size(ComponentSize{pixels(184), pixels(48)})
            .with_self_align(SelfAlign::FlexStart)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_margin(Margin{.top = pixels(0), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true));

    // Center - centers horizontally
    div(context, mk(demo.ent(), 1),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(184), pixels(48)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_skip_tabbing(true));

    // FlexEnd - aligns right with margin for visual spacing from edge
    div(context, mk(demo.ent(), 2),
        ComponentConfig{}
            .with_label("FlexEnd (right)")
            .with_size(ComponentSize{pixels(184), pixels(48)})
            .with_self_align(SelfAlign::FlexEnd)
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_margin(Margin{.top = pixels(0), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true));

    // Visual legend row showing color-to-position mapping
    auto legend = hstack(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(container_width), pixels(36)})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_margin(Margin{.top = DefaultSpacing::small(), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("legend"));

    // Legend: left
    div(context, mk(legend.ent(), 0),
        ComponentConfig{}
            .with_label("Left")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true));

    // Legend: center
    div(context, mk(legend.ent(), 1),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true));

    // Legend: right
    div(context, mk(legend.ent(), 2),
        ComponentConfig{}
            .with_label("Right")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true));

    // Summary line
    div(context, mk(main.ent(), 4),
        ComponentConfig{}
            .with_label("Each element picks its own position")
            .with_size(ComponentSize{pixels(container_width), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 180, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(15.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = DefaultSpacing::tiny(), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(self_align, "System Demos",
                        "SelfAlign feature for centering content",
                        SelfAlignShowcase)
