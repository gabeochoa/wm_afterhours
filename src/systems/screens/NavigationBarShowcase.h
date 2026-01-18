#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NavigationBarShowcase : ScreenSystem<UIContext<InputAction>> {
  // Navigation bar state
  size_t category_idx = 0;
  size_t mode_idx = 1;
  size_t level_idx = 0;

  std::vector<std::string> categories = {"Graphics", "Audio", "Controls",
                                         "Gameplay"};
  std::vector<std::string> modes = {"Story", "Arcade", "Multiplayer"};
  std::vector<std::string> levels = {"Level 1", "Level 2", "Level 3", "Level 4",
                                     "Level 5", "Level 6"};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;

    // Main container background
    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.88f)})
                        .with_custom_background(theme.background)
                        .with_roundness(0.08f)
                        .with_debug_name("navbar_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("navbar_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Navigation Bar Component Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Content area
    auto content =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(420)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("content"));

    // Section 1: Settings Categories
    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("Settings Categories")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section1_label"));

    navigation_bar(context, mk(content.ent(), 1), categories, category_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(400), pixels(50)})
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_debug_name("navbar_categories"));

    div(context, mk(content.ent(), 2),
        ComponentConfig{}
            .with_label("Current: " + categories[category_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::large()})
            .with_debug_name("status1"));

    // Section 2: Game Modes
    div(context, mk(content.ent(), 3),
        ComponentConfig{}
            .with_label("Game Mode Selector")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section2_label"));

    navigation_bar(context, mk(content.ent(), 4), modes, mode_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(350), pixels(50)})
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_debug_name("navbar_modes"));

    div(context, mk(content.ent(), 5),
        ComponentConfig{}
            .with_label("Mode: " + modes[mode_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::large()})
            .with_debug_name("status2"));

    // Section 3: Level Selector (more options)
    div(context, mk(content.ent(), 6),
        ComponentConfig{}
            .with_label("Level Selector (6 options)")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section3_label"));

    navigation_bar(context, mk(content.ent(), 7), levels, level_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(300), pixels(50)})
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_debug_name("navbar_levels"));

    div(context, mk(content.ent(), 8),
        ComponentConfig{}
            .with_label("Level: " + levels[level_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("status3"));
  }
};

REGISTER_EXAMPLE_SCREEN(navigation_bar_demo, "Component Galleries",
                        "Navigation bar widget demo", NavigationBarShowcase)

