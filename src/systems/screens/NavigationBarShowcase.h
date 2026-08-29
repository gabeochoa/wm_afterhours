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
  std::vector<std::string> levels = {"Level 1", "Level 2", "Level 3",
                                     "Level 4", "Level 5", "Level 6"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Main container - centered on screen, expanded for more screen use
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.80f), screen_pct(0.92f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.04f)
                .with_debug_name("navbar_bg"));

    // Content container with padding
    auto main_container =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                   .with_padding(Spacing::xl)
                   .with_justify_content(JustifyContent::Center)
                   .with_no_wrap()
                   .with_debug_name("navbar_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Navigation Bar Component Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_background(Theme::Usage::None)
            .with_auto_text_color(false)
            .with_custom_text_color(context.theme.font)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, pixels(26.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // Section 1: Settings Categories
    div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_label("Settings Categories")
            // Same blue fill and height as the controls made every row look
            // equally clickable; the header also ran 136px wider than them.
            .with_size(ComponentSize{percent(0.85f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_auto_text_color(false)
            .with_custom_text_color(context.theme.font_muted)
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));

    navigation_bar(context, mk(main_container.ent(), 2), categories,
                   category_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Section 2: Game Modes
    div(context, mk(main_container.ent(), 4),
        ComponentConfig{}
            .with_label("Game Mode Selector")
            // Same blue fill and height as the controls made every row look
            // equally clickable; the header also ran 136px wider than them.
            .with_size(ComponentSize{percent(0.85f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_auto_text_color(false)
            .with_custom_text_color(context.theme.font_muted)
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));

    navigation_bar(context, mk(main_container.ent(), 5), modes, mode_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Section 3: Level Selector (more options)
    div(context, mk(main_container.ent(), 7),
        ComponentConfig{}
            .with_label("Level Selector (6 options)")
            // Same blue fill and height as the controls made every row look
            // equally clickable; the header also ran 136px wider than them.
            .with_size(ComponentSize{percent(0.85f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_auto_text_color(false)
            .with_custom_text_color(context.theme.font_muted)
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));

    navigation_bar(context, mk(main_container.ent(), 8), levels, level_idx,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.85f), pixels(64)})
                       .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f)));
  }
};

REGISTER_EXAMPLE_SCREEN(navigation_bar_demo, "Component Galleries",
                        "Navigation bar widget demo", NavigationBarShowcase)
