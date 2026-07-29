#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

namespace cards_presets {

inline ComponentConfig CardConfig(const std::string &label,
                                  Theme::Usage bg = Theme::Usage::Surface) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{pixels(140), pixels(100)})
      .with_background(bg)
      .with_padding(Spacing::md)
      .with_margin(Spacing::sm)
      .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
      .with_auto_text_color(true)
      .with_skip_tabbing(true);
}

inline ComponentConfig InnerCardConfig(const std::string &label,
                                       Theme::Usage bg) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.30f), pixels(55)})
      .with_background(bg)
      .with_auto_text_color(true)
      .with_padding(Spacing::xs)
      .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
      .with_roundness(0.06f) // Slightly smaller than parent (0.08f)
      .with_skip_tabbing(true);
}

inline ComponentConfig PanelTitleConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.95f), pixels(32)})
      .with_background(Theme::Usage::Surface)
      .with_auto_text_color(true)
      .with_padding(Spacing::xs)
      .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
      .with_alignment(TextAlignment::Left)
      .with_skip_tabbing(true);
}

inline ComponentConfig SwatchConfig(const std::string &label,
                                    Theme::Usage bg) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.13f), pixels(44)})
      .with_background(bg)
      .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
      .with_margin(Spacing::xs)
      .with_skip_tabbing(true);
}

} // namespace cards_presets

struct CardsGallery : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply cozy kraft theme with handwritten font to match warm aesthetic
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", pixels(18.0f));

    using namespace cards_presets;

    // Main container background - centered with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_padding(Spacing::sm) // Padding on root
                .with_debug_name("cards_bg"));

    // Content container - no padding since root has it
    auto main_container =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                   .with_justify_content(JustifyContent::SpaceBetween)
                   .with_no_wrap()
                   .with_debug_name("cards_main"));

    // Title - larger font for better visibility
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Cards & Panels Gallery")
            .with_size(ComponentSize{percent(0.95f), pixels(50)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, pixels(24.0f)));

    // Row 1: Basic cards - all simple like theme swatches
    auto row1 = hstack(context, mk(main_container.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(150)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_no_wrap()
                           .with_debug_name("row1_cards"));

    // Section label - aligned with card row
    div(context, mk(row1.ent(), 100),
        ComponentConfig{}
            .with_label("Styles:")
            .with_size(ComponentSize{pixels(85), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true));

    // Card 1: Basic surface card (simple) - standardized height
    div(context, mk(row1.ent(), 0), CardConfig("Surface"));
    // Card 2: Primary colored card (simple)
    div(context, mk(row1.ent(), 1), CardConfig("Primary", Theme::Usage::Primary));
    // Card 3: Accent card with sharp corners
    div(context, mk(row1.ent(), 2),
        CardConfig("Sharp", Theme::Usage::Accent).disable_rounded_corners());
    // Card 4: Secondary card with custom corners
    div(context, mk(row1.ent(), 3),
        CardConfig("Custom", Theme::Usage::Secondary)
            .with_rounded_corners(
                RoundedCorners().all_sharp().round(TOP_LEFT).round(
                    BOTTOM_RIGHT)));

    // Row 2: Nested cards
    auto row2 = hstack(context, mk(main_container.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(165)})
                           .with_background(Theme::Usage::None)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_no_wrap()
                           .with_debug_name("row2_nested"));

    // Section label for nested cards
    div(context, mk(row2.ent(), 100),
        ComponentConfig{}
            .with_label("Nested:")
            .with_size(ComponentSize{pixels(75), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_margin(Margin{.right = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));

    // Nested card container - sized proportionally to fit within row
    auto nested_container =
        vstack(context, mk(row2.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.33f), children()})
                   .with_background(Theme::Usage::Surface)
                   .with_border(
                       afterhours::colors::darken(context.theme.surface, 0.75f),
                       2.0f)
                   .with_padding(Spacing::sm) // Reduced padding
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("nested_container"));

    div(context, mk(nested_container.ent(), 0), PanelTitleConfig("Nested Cards"));

    // Inner cards row - prevent wrapping, ensure enough height for cards
    auto inner_row =
        hstack(context, mk(nested_container.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(75)})
                   .with_background(Theme::Usage::None)
                   .with_no_wrap()
                   .with_align_items(AlignItems::Center)
                   .with_debug_name("inner_row"));

    // Inner card 1 - sized proportionally to fit within container
    div(context, mk(inner_row.ent(), 0),
        InnerCardConfig("Item 1", Theme::Usage::Primary));
    // Inner card 2
    div(context, mk(inner_row.ent(), 1),
        InnerCardConfig("Item 2", Theme::Usage::Secondary));
    // Inner card 3
    div(context, mk(inner_row.ent(), 2),
        InnerCardConfig("Item 3", Theme::Usage::Accent));

    // Info panel with different opacity
    auto info_panel =
        vstack(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.33f), children()})
                   .with_custom_background(
                       afterhours::colors::opacity_pct(theme.primary, 0.3f))
                   .with_padding(Spacing::sm)
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f) // Match nested_container roundness
                   .with_debug_name("info_panel"));

    div(context, mk(info_panel.ent(), 0),
        PanelTitleConfig("Semi-Transparent"));

    div(context, mk(info_panel.ent(), 1),
        ComponentConfig{}
            .with_label("Uses opacity for a translucent effect.")
            .with_size(ComponentSize{percent(0.95f), pixels(80)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_custom_text_color(theme.font)
            .with_skip_tabbing(true));

    // Row 3: Theme comparison
    auto row3 = hstack(context, mk(main_container.ent(), 3),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.95f), pixels(70)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_debug_name("row3_themes"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("Theme Colors:")
            .with_size(ComponentSize{percent(0.14f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true));

    // Color swatches - with gaps between them
    div(context, mk(row3.ent(), 1),
        SwatchConfig("Primary", Theme::Usage::Primary));
    div(context, mk(row3.ent(), 2),
        SwatchConfig("Secondary", Theme::Usage::Secondary));
    div(context, mk(row3.ent(), 3),
        SwatchConfig("Accent", Theme::Usage::Accent));
    div(context, mk(row3.ent(), 4),
        SwatchConfig("Surface", Theme::Usage::Surface));
    div(context, mk(row3.ent(), 5),
        SwatchConfig("Background", Theme::Usage::Background));
  }
};

REGISTER_EXAMPLE_SCREEN(cards, "Component Galleries",
                        "Card layouts, nesting, and corner styles",
                        CardsGallery)
