#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PaginationShowcase : ScreenSystem<UIContext<InputAction>> {
  // Pagination state
  size_t page_idx = 0;
  size_t difficulty_idx = 1;
  size_t color_idx = 2;

  std::vector<std::string> pages = {"Page 1", "Page 2", "Page 3", "Page 4",
                                    "Page 5"};
  std::vector<std::string> difficulties = {"Easy", "Medium", "Hard", "Expert"};
  std::vector<std::string> colors = {"Red", "Green", "Blue", "Yellow", "Purple"};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container background - centered with padding
    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                        .with_self_align(SelfAlign::Center)
                        .with_custom_background(theme.background)
                        .with_roundness(0.08f)
                        .with_padding(Spacing::lg)  // Padding on root
                        .with_debug_name("pagination_bg"));

    // Content container - no padding since root has it
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("pagination_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Pagination Component Showcase")
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
                .with_size(ComponentSize{percent(1.0f), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("content"));

    // Section 1: Basic Pagination
    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("Basic Pagination")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section1_label"));

    // Height increased to 50px to fit internal buttons
    pagination(context, mk(content.ent(), 1), pages, page_idx,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(500), pixels(50)})
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                   .with_debug_name("pagination_basic"));

    // Status text
    div(context, mk(content.ent(), 2),
        ComponentConfig{}
            .with_label("Selected: " + pages[page_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::large()})
            .with_debug_name("status1"));

    // Section 2: Difficulty Selector
    div(context, mk(content.ent(), 3),
        ComponentConfig{}
            .with_label("Difficulty Selector (4 options)")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section2_label"));

    pagination(context, mk(content.ent(), 4), difficulties, difficulty_idx,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(400), pixels(50)})
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                   .with_debug_name("pagination_difficulty"));

    div(context, mk(content.ent(), 5),
        ComponentConfig{}
            .with_label("Difficulty: " + difficulties[difficulty_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::large()})
            .with_debug_name("status2"));

    // Section 3: Color Picker
    div(context, mk(content.ent(), 6),
        ComponentConfig{}
            .with_label("Color Picker (5 options)")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("section3_label"));

    pagination(context, mk(content.ent(), 7), colors, color_idx,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(450), pixels(50)})
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_debug_name("pagination_colors"));

    div(context, mk(content.ent(), 8),
        ComponentConfig{}
            .with_label("Color: " + colors[color_idx])
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("status3"));
  }
};

REGISTER_EXAMPLE_SCREEN(pagination, "Component Galleries",
                        "Pagination widget demo", PaginationShowcase)

