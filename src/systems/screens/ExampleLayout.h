#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleLayout : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply sage natural theme for this screen
    auto theme = afterhours::ui::theme_presets::sage_natural();
    context.theme = theme;

    // Main container with column layout - use smaller percentage for screen
    // safe area
    auto main_container =
        vstack(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.88f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_padding(Spacing::sm)
                .with_justify_content(JustifyContent::Center)
                .with_roundness(0.08f)
                .with_no_wrap()
                .with_debug_name("layout_main"));

    // Title - use percent sizing to stay within parent
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Layout System Demo")
            .with_size(ComponentSize{percent(0.95f), pixels(48)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Row layout demo - JustifyContent::Center to keep panels within container
    auto row_container =
        hstack(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), pixels(120)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::sm)
                .with_justify_content(JustifyContent::Center)
                .with_align_items(AlignItems::Center)
                .with_debug_name("row_container"));

    // Three panels with fixed sizes that fit within container
    div(context, mk(row_container.ent(), 0),
        ComponentConfig{}
            .with_label("Left Panel")
            .with_size(ComponentSize{pixels(180), pixels(80)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Spacing::sm));

    div(context, mk(row_container.ent(), 1),
        ComponentConfig{}
            .with_label("Center Panel")
            .with_size(ComponentSize{pixels(180), pixels(80)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Spacing::sm));

    div(context, mk(row_container.ent(), 2),
        ComponentConfig{}
            .with_label("Right Panel")
            .with_size(ComponentSize{pixels(180), pixels(80)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Spacing::sm));

    // Separator between row demo and column demo
    div(context, mk(main_container.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.9f), pixels(1)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.7f))
            .with_self_align(SelfAlign::Center)
            .with_margin(Margin{.top = DefaultSpacing::small(), .bottom = DefaultSpacing::small()})
            .with_debug_name("section_separator_1"));

    // Column layout demo - stacked items demonstration
    auto col_container =
        hstack(context, mk(main_container.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), pixels(220)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::md)
                .with_justify_content(JustifyContent::Center)
                .with_margin(Margin{.top = DefaultSpacing::small(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("col_demo_container"));

    // Column with stacked items - centered within container
    auto left_col =
        vstack(context, mk(col_container.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.60f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::xs)
                .with_align_items(AlignItems::Center)
                .with_no_wrap()
                .with_debug_name("stacked_column"));

    // Stacked items - use percent of parent column
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("Stacked Item 1")
            .with_size(ComponentSize{percent(0.95f), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = pixels(3)}));

    div(context, mk(left_col.ent(), 1),
        ComponentConfig{}
            .with_label("Stacked Item 2")
            .with_size(ComponentSize{percent(0.95f), pixels(40)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = pixels(3)}));

    div(context, mk(left_col.ent(), 2),
        ComponentConfig{}
            .with_label("Stacked Item 3")
            .with_size(ComponentSize{percent(0.95f), pixels(40)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f)));

    // Separator between column demo and description
    div(context, mk(main_container.ent(), 11),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.9f), pixels(1)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.7f))
            .with_self_align(SelfAlign::Center)
            .with_margin(Margin{.top = DefaultSpacing::small(), .bottom = DefaultSpacing::small()})
            .with_debug_name("section_separator_2"));

    // Layout guide - positioned below demo content to avoid obstruction
    auto desc_panel = hstack(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.95f), pixels(44)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("description_panel"));

    div(context, mk(desc_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Column: stacks vertically")
            .with_size(ComponentSize{percent(0.30f), pixels(32)})
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    div(context, mk(desc_panel.ent(), 1),
        ComponentConfig{}
            .with_label("Row: arranges horizontally")
            .with_size(ComponentSize{percent(0.30f), pixels(32)})
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    div(context, mk(desc_panel.ent(), 2),
        ComponentConfig{}
            .with_label("Margins control spacing")
            .with_size(ComponentSize{percent(0.30f), pixels(32)})
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    // Info footer - use percent width
    div(context, mk(main_container.ent(), 4),
        ComponentConfig{}
            .with_label("Using Sage Natural theme with responsive layouts")
            .with_size(ComponentSize{percent(0.95f), pixels(36)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_margin(Margin{.top = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(layout, "System Demos",
                        "Layout system example with containers", ExampleLayout)
