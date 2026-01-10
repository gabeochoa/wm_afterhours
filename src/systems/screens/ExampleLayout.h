#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleLayout : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply sage natural theme for this screen
    auto theme = afterhours::ui::theme_presets::sage_natural();
    context.theme = theme;

    // Main container with column layout
    auto main_container =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.9f), screen_pct(0.85f)})
                .with_custom_color(theme.background)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("layout_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Layout System Demo")
            .with_size(ComponentSize{screen_pct(0.8f), pixels(50)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Row layout demo
    auto row_container =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.8f), pixels(150)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("row_container"));

    div(context, mk(row_container.ent(), 0),
        ComponentConfig{}
            .with_label("Left Panel")
            .with_size(ComponentSize{screen_pct(0.25f), pixels(120)})
            .with_color_usage(Theme::Usage::Primary)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("left_panel"));

    div(context, mk(row_container.ent(), 1),
        ComponentConfig{}
            .with_label("Center Panel")
            .with_size(ComponentSize{screen_pct(0.25f), pixels(120)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("center_panel"));

    div(context, mk(row_container.ent(), 2),
        ComponentConfig{}
            .with_label("Right Panel")
            .with_size(ComponentSize{screen_pct(0.25f), pixels(120)})
            .with_color_usage(Theme::Usage::Accent)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("right_panel"));

    // Column layout demo
    auto col_container =
        div(context, mk(main_container.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.8f), pixels(220)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("col_demo_container"));

    // Left column with stacked items
    auto left_col =
        div(context, mk(col_container.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.35f), pixels(190)})
                .with_custom_color(afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .with_debug_name("stacked_column"));

    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("Stacked Item 1")
            .with_size(ComponentSize{screen_pct(0.3f), pixels(50)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack1"));

    div(context, mk(left_col.ent(), 1),
        ComponentConfig{}
            .with_label("Stacked Item 2")
            .with_size(ComponentSize{screen_pct(0.3f), pixels(50)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack2"));

    div(context, mk(left_col.ent(), 2),
        ComponentConfig{}
            .with_label("Stacked Item 3")
            .with_size(ComponentSize{screen_pct(0.3f), pixels(50)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack3"));

    // Right side description
    div(context, mk(col_container.ent(), 1),
        ComponentConfig{}
            .with_label("FlexDirection::Column stacks elements vertically. "
                        "FlexDirection::Row arranges them horizontally. "
                        "Use margins and padding to control spacing.")
            .with_size(ComponentSize{screen_pct(0.38f), pixels(190)})
            .with_custom_color(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("description"));

    // Info footer
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Using Sage Natural theme with responsive layouts")
            .with_size(ComponentSize{screen_pct(0.8f), pixels(35)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(layout, "Layout system example with containers",
                        ExampleLayout)

