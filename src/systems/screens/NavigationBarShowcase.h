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

    // Main container - centered on screen, expanded for more screen use
    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.80f), screen_pct(0.92f)})
                        .with_self_align(SelfAlign::Center)
                        .with_custom_background(theme.background)
                        .with_roundness(0.04f)
                        .with_debug_name("navbar_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("navbar_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Navigation Bar Component Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("title"));

    // Section 1: Settings Categories
    div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_label("Settings Categories")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true)
            .with_debug_name("section1_label"));

    navigation_bar(context, mk(main_container.ent(), 2), categories, category_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_debug_name("navbar_categories"));

    // Section 2: Game Modes
    div(context, mk(main_container.ent(), 4),
        ComponentConfig{}
            .with_label("Game Mode Selector")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true)
            .with_debug_name("section2_label"));

    navigation_bar(context, mk(main_container.ent(), 5), modes, mode_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_debug_name("navbar_modes"));

    // Section 3: Level Selector (more options)
    div(context, mk(main_container.ent(), 7),
        ComponentConfig{}
            .with_label("Level Selector (6 options)")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true)
            .with_debug_name("section3_label"));

    navigation_bar(context, mk(main_container.ent(), 8), levels, level_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
                       .with_debug_name("navbar_levels"));
  }
};

REGISTER_EXAMPLE_SCREEN(navigation_bar_demo, "Component Galleries",
                        "Navigation bar widget demo", NavigationBarShowcase)

