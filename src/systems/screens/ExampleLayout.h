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

    // Main container with column layout - use smaller percentage for screen safe area
    auto main_container =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.75f), screen_pct(0.80f)})
                .with_custom_background(theme.background)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_roundness(0.08f)
                .with_debug_name("layout_main"));

    // Title - use percent sizing to stay within parent
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Layout System Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 26.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Row layout demo - JustifyContent::SpaceAround distributes panels evenly
    auto row_container =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.98f), pixels(120)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_align_items(AlignItems::Center)
                .with_debug_name("row_container"));

    // Three panels - SpaceAround distributes remaining space evenly
    div(context, mk(row_container.ent(), 0),
        ComponentConfig{}
            .with_label("Left Panel")
            .with_size(ComponentSize{percent(0.28f), pixels(90)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("left_panel"));

    div(context, mk(row_container.ent(), 1),
        ComponentConfig{}
            .with_label("Center Panel")
            .with_size(ComponentSize{percent(0.28f), pixels(90)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("center_panel"));

    div(context, mk(row_container.ent(), 2),
        ComponentConfig{}
            .with_label("Right Panel")
            .with_size(ComponentSize{percent(0.28f), pixels(90)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("right_panel"));

    // Column layout demo - Center to keep panels within bounds
    auto col_container =
        div(context, mk(main_container.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.98f), pixels(200)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_margin(Margin{.top = DefaultSpacing::small(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("col_demo_container"));

    // Left column with stacked items - use percent of parent
    auto left_col = div(
        context, mk(col_container.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.46f), pixels(170)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_debug_name("stacked_column"));

    // Stacked items - use percent of parent column
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("Stacked Item 1")
            .with_size(ComponentSize{percent(0.95f), pixels(48)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack1"));

    div(context, mk(left_col.ent(), 1),
        ComponentConfig{}
            .with_label("Stacked Item 2")
            .with_size(ComponentSize{percent(0.95f), pixels(48)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack2"));

    div(context, mk(left_col.ent(), 2),
        ComponentConfig{}
            .with_label("Stacked Item 3")
            .with_size(ComponentSize{percent(0.95f), pixels(48)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("stack3"));

    // Right side description - use percent sizing with proper dark text
    div(context, mk(col_container.ent(), 1),
        ComponentConfig{}
            .with_label("FlexDirection::Column stacks elements vertically. "
                        "FlexDirection::Row arranges them horizontally. "
                        "Use margins and padding to control spacing.")
            .with_size(ComponentSize{percent(0.46f), pixels(170)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_custom_text_color(theme.font)  // Use font (dark charcoal) not darkfont (light)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_skip_tabbing(true)
            .with_debug_name("description"));

    // Info footer - use percent width
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Using Sage Natural theme with responsive layouts")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(layout, "System Demos",
                        "Layout system example with containers", ExampleLayout)
