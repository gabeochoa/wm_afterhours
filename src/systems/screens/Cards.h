#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CardsGallery : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply cozy kraft theme
    // TODO: Add font configuration when fonts are selected
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    // Main container background - centered with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_padding(Spacing::lg)  // Padding on root
                .with_debug_name("cards_bg"));

    // Content container - no padding since root has it
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("cards_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Cards & Panels Gallery")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Row 1: Basic cards - all simple like theme swatches
    auto row1 = div(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(900), pixels(160)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_debug_name("row1_cards"));

    // Card 1: Basic surface card (simple)
    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("Basic Card")
            .with_size(ComponentSize{pixels(120), pixels(120)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::md)
            .with_margin(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_debug_name("card1_surface"));

    // Card 2: Primary colored card (simple)
    div(context, mk(row1.ent(), 1),
        ComponentConfig{}
            .with_label("Primary Card")
            .with_size(ComponentSize{pixels(120), pixels(120)})
            .with_custom_background(theme.primary)
            .with_padding(Spacing::md)
            .with_margin(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_debug_name("card2_primary"));

    // Card 3: Accent card with sharp corners
    div(context, mk(row1.ent(), 2),
        ComponentConfig{}
            .with_label("Sharp Corners")
            .with_size(ComponentSize{pixels(120), pixels(120)})
            .with_custom_background(theme.accent)
            .with_padding(Spacing::md)
            .with_margin(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_auto_text_color(true)
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("card3_sharp"));

    // Card 4: Secondary card with custom corners
    div(context, mk(row1.ent(), 3),
        ComponentConfig{}
            .with_label("Custom Corners")
            .with_size(ComponentSize{pixels(120), pixels(120)})
            .with_custom_background(theme.secondary)
            .with_padding(Spacing::md)
            .with_margin(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_auto_text_color(true)
            .with_rounded_corners(
                RoundedCorners().all_sharp().round(TOP_LEFT).round(
                    BOTTOM_RIGHT))
            .with_skip_tabbing(true)
            .with_debug_name("card4_custom_radius"));

    // Row 2: Nested cards
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(0.95f), pixels(220)})
                        .with_background(Theme::Usage::None)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row2_nested"));

    // Nested card container - wider to fit inner cards
    auto nested_container =
        div(context, mk(row2.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(300), pixels(180)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)  // Reduced padding
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_roundness(0.08f)
                .with_debug_name("nested_container"));

    div(context, mk(nested_container.ent(), 0),
        ComponentConfig{}
            .with_label("Nested Cards")
            .with_size(ComponentSize{percent(0.95f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_skip_tabbing(true)
            .with_debug_name("nested_title"));

    // Inner cards row - prevent wrapping, use full width
    auto inner_row =
        div(context, mk(nested_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(100)})
                .with_background(Theme::Usage::None)
                .with_flex_direction(FlexDirection::Row)
                .with_no_wrap()
                .with_debug_name("inner_row"));

    // Inner card 1 - smaller to fit within the ~280px available width
    div(context, mk(inner_row.ent(), 0),
        ComponentConfig{}
            .with_label("1")
            .with_size(ComponentSize{pixels(70), pixels(80)})
            .with_background(Theme::Usage::Primary)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.right = DefaultSpacing::tiny()})
            .with_skip_tabbing(true)
            .with_debug_name("inner1"));

    // Inner card 2
    div(context, mk(inner_row.ent(), 1),
        ComponentConfig{}
            .with_label("2")
            .with_size(ComponentSize{pixels(70), pixels(80)})
            .with_background(Theme::Usage::Secondary)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.right = DefaultSpacing::tiny()})
            .with_skip_tabbing(true)
            .with_debug_name("inner2"));

    // Inner card 3
    div(context, mk(inner_row.ent(), 2),
        ComponentConfig{}
            .with_label("3")
            .with_size(ComponentSize{pixels(70), pixels(80)})
            .with_background(Theme::Usage::Accent)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_debug_name("inner3"));

    // Info panel with different opacity
    auto info_panel =
        div(context, mk(row2.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(280), pixels(200)})
                .with_custom_background(
                    afterhours::colors::opacity_pct(theme.primary, 0.3f))
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("info_panel"));

    div(context, mk(info_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Semi-Transparent")
            .with_size(ComponentSize{percent(0.95f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("info_title"));

    div(context, mk(info_panel.ent(), 1),
        ComponentConfig{}
            .with_label("This panel uses opacity to create a translucent "
                        "overlay effect.")
            .with_size(ComponentSize{percent(0.95f), pixels(120)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("info_body"));

    // Row 3: Theme comparison
    auto row3 = div(context, mk(main_container.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(100)})
                        .with_custom_background(theme.surface)
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
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_skip_tabbing(true)
            .with_debug_name("theme_label"));

    // Color swatches
    div(context, mk(row3.ent(), 1),
        ComponentConfig{}
            .with_label("Primary")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_primary"));

    div(context, mk(row3.ent(), 2),
        ComponentConfig{}
            .with_label("Secondary")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_secondary"));

    div(context, mk(row3.ent(), 3),
        ComponentConfig{}
            .with_label("Accent")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_accent"));

    div(context, mk(row3.ent(), 4),
        ComponentConfig{}
            .with_label("Surface")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_background(Theme::Usage::Surface)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_surface"));

    div(context, mk(row3.ent(), 5),
        ComponentConfig{}
            .with_label("Background")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_background(Theme::Usage::Background)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("swatch_background"));
  }
};

REGISTER_EXAMPLE_SCREEN(cards, "Component Galleries",
                        "Card layouts, nesting, and corner styles",
                        CardsGallery)
