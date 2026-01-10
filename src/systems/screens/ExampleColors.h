#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleColors : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply midnight theme for this screen
    auto theme = afterhours::ui::theme_presets::midnight();
    context.theme = theme;

    // Main container
    auto main_container = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.9f), screen_pct(0.85f)})
            .with_custom_color(theme.background)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("colors_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Theme Color Swatches")
            .with_size(ComponentSize{screen_pct(0.8f), pixels(50)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Color row 1: Theme colors
    auto row1 = div(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.8f), pixels(120)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_debug_name("row1"));

    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("Primary")
            .with_size(ComponentSize{pixels(110), pixels(90)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("swatch_primary"));

    div(context, mk(row1.ent(), 1),
        ComponentConfig{}
            .with_label("Secondary")
            .with_size(ComponentSize{pixels(110), pixels(90)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("swatch_secondary"));

    div(context, mk(row1.ent(), 2),
        ComponentConfig{}
            .with_label("Accent")
            .with_size(ComponentSize{pixels(110), pixels(90)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("swatch_accent"));

    div(context, mk(row1.ent(), 3),
        ComponentConfig{}
            .with_label("Surface")
            .with_size(ComponentSize{pixels(110), pixels(90)})
            .with_color_usage(Theme::Usage::Surface)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("swatch_surface"));

    div(context, mk(row1.ent(), 4),
        ComponentConfig{}
            .with_label("Error")
            .with_size(ComponentSize{pixels(110), pixels(90)})
            .with_color_usage(Theme::Usage::Error)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("swatch_error"));

    // Color row 2: Custom colors
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.8f), pixels(120)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row2"));

    afterhours::Color custom_colors[] = {
        afterhours::Color{255, 100, 100, 255},  // Red
        afterhours::Color{100, 200, 100, 255},  // Green
        afterhours::Color{100, 150, 255, 255},  // Blue
        afterhours::Color{255, 200, 100, 255},  // Yellow
        afterhours::Color{200, 100, 255, 255},  // Purple
    };
    std::string custom_labels[] = {"Red", "Green", "Blue", "Yellow", "Purple"};

    for (int i = 0; i < 5; i++) {
      div(context, mk(row2.ent(), i),
          ComponentConfig{}
              .with_label(custom_labels[i])
              .with_size(ComponentSize{pixels(110), pixels(90)})
              .with_custom_color(custom_colors[i])
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_margin(Spacing::xs)
              .with_debug_name("custom_" + std::to_string(i)));
    }

    // Info text
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Using Midnight theme. Top row: theme colors. Bottom row: custom colors.")
            .with_size(ComponentSize{screen_pct(0.8f), pixels(40)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("info"));
  }
};

REGISTER_EXAMPLE_SCREEN(colors, "Color system examples", ExampleColors)

