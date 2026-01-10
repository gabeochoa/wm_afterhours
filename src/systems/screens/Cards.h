#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CardsGallery : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply cozy kraft theme
    // TODO: Add font configuration when fonts are selected
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    // Main container
    auto main_container =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.9f)})
                .with_custom_color(theme.background)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("cards_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Cards & Panels Gallery")
            .with_size(ComponentSize{pixels(1100), pixels(50)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Row 1: Basic cards with different styles
    auto row1 = div(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(1100), pixels(180)})
                        .with_color_usage(Theme::Usage::None)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_debug_name("row1_cards"));

    // Card 1: Basic surface card
    auto card1 = div(context, mk(row1.ent(), 0),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(200), pixels(160)})
                         .with_custom_color(theme.surface)
                         .with_padding(Spacing::md)
                         .with_flex_direction(FlexDirection::Column)
                         .with_margin(Spacing::sm)
                         .with_debug_name("card1_surface"));

    div(context, mk(card1.ent(), 0),
        ComponentConfig{}
            .with_label("Basic Card")
            .with_size(ComponentSize{pixels(160), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card1_title"));

    div(context, mk(card1.ent(), 1),
        ComponentConfig{}
            .with_label(
                "A simple card with surface color and default rounded corners.")
            .with_size(ComponentSize{pixels(160), pixels(80)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card1_body"));

    // Card 2: Primary colored card
    auto card2 = div(context, mk(row1.ent(), 1),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(200), pixels(160)})
                         .with_color_usage(Theme::Usage::Primary)
                         .with_padding(Spacing::md)
                         .with_flex_direction(FlexDirection::Column)
                         .with_margin(Spacing::sm)
                         .with_debug_name("card2_primary"));

    div(context, mk(card2.ent(), 0),
        ComponentConfig{}
            .with_label("Primary Card")
            .with_size(ComponentSize{pixels(160), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card2_title"));

    div(context, mk(card2.ent(), 1),
        ComponentConfig{}
            .with_label("Uses the theme's primary color for emphasis.")
            .with_size(ComponentSize{pixels(160), pixels(80)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card2_body"));

    // Card 3: Accent card with sharp corners
    auto card3 = div(context, mk(row1.ent(), 2),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(200), pixels(160)})
                         .with_color_usage(Theme::Usage::Accent)
                         .with_padding(Spacing::md)
                         .with_flex_direction(FlexDirection::Column)
                         .with_margin(Spacing::sm)
                         .disable_rounded_corners()
                         .with_debug_name("card3_sharp"));

    div(context, mk(card3.ent(), 0),
        ComponentConfig{}
            .with_label("Sharp Corners")
            .with_size(ComponentSize{pixels(160), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card3_title"));

    div(context, mk(card3.ent(), 1),
        ComponentConfig{}
            .with_label("No rounded corners for a different aesthetic.")
            .with_size(ComponentSize{pixels(160), pixels(80)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card3_body"));

    // Card 4: Custom corner radius
    auto card4 = div(context, mk(row1.ent(), 3),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(200), pixels(160)})
                         .with_custom_color(theme.surface)
                         .with_padding(Spacing::md)
                         .with_flex_direction(FlexDirection::Column)
                         .with_margin(Spacing::sm)
                         .with_rounded_corners(
                             RoundedCorners().all_sharp().round(TOP_LEFT).round(
                                 BOTTOM_RIGHT))
                         .with_debug_name("card4_custom_radius"));

    div(context, mk(card4.ent(), 0),
        ComponentConfig{}
            .with_label("Custom Corners")
            .with_size(ComponentSize{pixels(160), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card4_title"));

    div(context, mk(card4.ent(), 1),
        ComponentConfig{}
            .with_label("Only top-left and bottom-right corners are rounded.")
            .with_size(ComponentSize{pixels(160), pixels(80)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_skip_tabbing(true)
            .with_debug_name("card4_body"));

    // Row 2: Nested cards
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(1100), pixels(220)})
                        .with_color_usage(Theme::Usage::None)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row2_nested"));

    // Nested card container
    auto nested_container =
        div(context, mk(row2.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(420), pixels(200)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("nested_container"));

    div(context, mk(nested_container.ent(), 0),
        ComponentConfig{}
            .with_label("Nested Cards")
            .with_size(ComponentSize{pixels(380), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("nested_title"));

    // Inner cards row
    auto inner_row = div(context, mk(nested_container.ent(), 1),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(380), pixels(120)})
                             .with_color_usage(Theme::Usage::None)
                             .with_flex_direction(FlexDirection::Row)
                             .with_debug_name("inner_row"));

    // Inner card 1
    div(context, mk(inner_row.ent(), 0),
        ComponentConfig{}
            .with_label("Item 1")
            .with_size(ComponentSize{pixels(110), pixels(100)})
            .with_color_usage(Theme::Usage::Primary)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("inner1"));

    // Inner card 2
    div(context, mk(inner_row.ent(), 1),
        ComponentConfig{}
            .with_label("Item 2")
            .with_size(ComponentSize{pixels(110), pixels(100)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("inner2"));

    // Inner card 3
    div(context, mk(inner_row.ent(), 2),
        ComponentConfig{}
            .with_label("Item 3")
            .with_size(ComponentSize{pixels(110), pixels(100)})
            .with_color_usage(Theme::Usage::Accent)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("inner3"));

    // Info panel with different opacity
    auto info_panel =
        div(context, mk(row2.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(300), pixels(200)})
                .with_custom_color(
                    afterhours::colors::opacity_pct(theme.primary, 0.3f))
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("info_panel"));

    div(context, mk(info_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Semi-Transparent Panel")
            .with_size(ComponentSize{pixels(260), pixels(30)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("info_title"));

    div(context, mk(info_panel.ent(), 1),
        ComponentConfig{}
            .with_label("This panel uses opacity to create a translucent "
                        "overlay effect. "
                        "Useful for HUDs and popup dialogs.")
            .with_size(ComponentSize{pixels(260), pixels(120)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_skip_tabbing(true)
            .with_debug_name("info_body"));

    // Row 3: Theme comparison
    auto row3 = div(context, mk(main_container.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(1100), pixels(100)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row3_themes"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("Theme Colors:")
            .with_size(ComponentSize{pixels(120), pixels(60)})
            .with_color_usage(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_debug_name("theme_label"));

    // Color swatches
    div(context, mk(row3.ent(), 1),
        ComponentConfig{}
            .with_label("Primary")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_primary"));

    div(context, mk(row3.ent(), 2),
        ComponentConfig{}
            .with_label("Secondary")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_secondary"));

    div(context, mk(row3.ent(), 3),
        ComponentConfig{}
            .with_label("Accent")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_accent"));

    div(context, mk(row3.ent(), 4),
        ComponentConfig{}
            .with_label("Surface")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_color_usage(Theme::Usage::Surface)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_surface"));

    div(context, mk(row3.ent(), 5),
        ComponentConfig{}
            .with_label("Background")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_color_usage(Theme::Usage::Background)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_background"));
  }
};

REGISTER_EXAMPLE_SCREEN(cards, "Component Galleries",
                        "Card layouts, nesting, and corner styles",
                        CardsGallery)
