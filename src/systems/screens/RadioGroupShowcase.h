#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RadioGroupShowcase : ScreenSystem<UIContext<InputAction>> {
  // Radio group values
  size_t fruit_idx = 1;
  size_t size_idx = 0;
  size_t color_idx = 2;

  std::array<std::string_view, 3> fruit_options = {"Apple", "Orange",
                                                    "Watermelon"};
  std::array<std::string_view, 3> size_options = {"Small", "Medium", "Large"};
  std::array<std::string_view, 4> color_options = {"Red", "Green", "Blue",
                                                    "Yellow"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container - centered on screen
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.70f), screen_pct(0.75f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_debug_name("radio_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("radio_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Radio Button Groups")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_debug_name("title"));

    // Content area - three columns
    auto content =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.70f)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_debug_name("content"));

    // Column 1 - Fruit selection
    auto col1 =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("col1"));

    div(context, mk(col1.ent(), 0),
        ComponentConfig{}
            .with_label("Favorite Fruit")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("fruit_header"));

    radio_group(context, mk(col1.ent(), 1), fruit_options, fruit_idx,
                ComponentConfig{}
                    .with_size(ComponentSize{percent(1.0f), pixels(40)})
                    .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                    .with_debug_name("fruit_radios"));

    // Column 2 - Size selection
    auto col2 =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("col2"));

    div(context, mk(col2.ent(), 0),
        ComponentConfig{}
            .with_label("Size")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("size_header"));

    radio_group(context, mk(col2.ent(), 1), size_options, size_idx,
                ComponentConfig{}
                    .with_size(ComponentSize{percent(1.0f), pixels(40)})
                    .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                    .with_debug_name("size_radios"));

    // Column 3 - Color selection
    auto col3 =
        div(context, mk(content.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("col3"));

    div(context, mk(col3.ent(), 0),
        ComponentConfig{}
            .with_label("Color")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("color_header"));

    radio_group(context, mk(col3.ent(), 1), color_options, color_idx,
                ComponentConfig{}
                    .with_size(ComponentSize{percent(1.0f), pixels(40)})
                    .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                    .with_debug_name("color_radios"));

    // Status display
    std::string status =
        fmt::format("Selected: {} {} {}",
                    fruit_options[fruit_idx], size_options[size_idx],
                    color_options[color_idx]);

    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(radio_buttons, "Component Galleries",
                        "Radio button group with single-select behavior",
                        RadioGroupShowcase)
