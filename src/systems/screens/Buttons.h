#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ButtonsGallery : afterhours::System<UIContext<InputAction>> {
  int click_counts[12] = {0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply a theme for this screen
    // TODO: Add font configuration when fonts are selected
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;

    // Main container
    auto main_container = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.9f)})
            .with_custom_color(theme.background)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("buttons_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Button Gallery")
            .with_size(ComponentSize{screen_pct(0.9f), pixels(50)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Row 1: Basic button states
    auto row1 = div(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.9f), pixels(80)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_debug_name("row1_states"));

    // Section label
    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("States:")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_color_usage(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("states_label"));

    // Normal button
    if (button(context, mk(row1.ent(), 1),
               ComponentConfig{}
                   .with_label("Normal")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_color_usage(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_normal"))) {
      click_counts[0]++;
    }

    // Hover state is automatic, but show a button with accent color
    if (button(context, mk(row1.ent(), 2),
               ComponentConfig{}
                   .with_label("Accent")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_color_usage(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_accent"))) {
      click_counts[1]++;
    }

    // Secondary button
    if (button(context, mk(row1.ent(), 3),
               ComponentConfig{}
                   .with_label("Secondary")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_color_usage(Theme::Usage::Secondary)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_secondary"))) {
      click_counts[2]++;
    }

    // Disabled button
    button(context, mk(row1.ent(), 4),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_disabled(true)
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_margin(Spacing::xs)
               .with_debug_name("btn_disabled"));

    // Row 2: Different sizes
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.9f), pixels(90)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row2_sizes"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("Sizes:")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_color_usage(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("sizes_label"));

    // Small button
    if (button(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_label("Small")
                   .with_size(ComponentSize{pixels(80), pixels(32)})
                   .with_color_usage(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, 12.0f)
                   .with_padding(Spacing::xs)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_small"))) {
      click_counts[3]++;
    }

    // Medium button
    if (button(context, mk(row2.ent(), 2),
               ComponentConfig{}
                   .with_label("Medium")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_color_usage(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_medium"))) {
      click_counts[4]++;
    }

    // Large button
    if (button(context, mk(row2.ent(), 3),
               ComponentConfig{}
                   .with_label("Large Button")
                   .with_size(ComponentSize{pixels(180), pixels(60)})
                   .with_color_usage(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_large"))) {
      click_counts[5]++;
    }

    // Row 3: Button group - need more height to accommodate the group
    auto row3 = div(context, mk(main_container.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.9f), pixels(100)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row3_group"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("Group:")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_color_usage(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("group_label"));

    // Create individual buttons styled as a group
    std::array<std::string_view, 4> group_labels = {"One", "Two", "Three", "Four"};
    for (size_t i = 0; i < group_labels.size(); i++) {
      auto corners = RoundedCorners();
      if (i == 0) corners = corners.sharp(TOP_RIGHT).sharp(BOTTOM_RIGHT);
      else if (i == group_labels.size() - 1) corners = corners.sharp(TOP_LEFT).sharp(BOTTOM_LEFT);
      else corners = corners.all_sharp();
      
      if (button(context, mk(row3.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(std::string(group_labels[i]))
                     .with_size(ComponentSize{pixels(90), pixels(45)})
                     .with_color_usage(Theme::Usage::Primary)
                     .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                     .with_rounded_corners(corners)
                     .with_debug_name("btn_group_" + std::to_string(i)))) {
        click_counts[6 + i]++;
      }
    }

    // Row 4: Custom styled buttons
    auto row4 = div(context, mk(main_container.ent(), 4),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.9f), pixels(80)})
                        .with_custom_color(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row4_custom"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Custom:")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_color_usage(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("custom_label"));

    // Custom color button - coral
    if (button(context, mk(row4.ent(), 1),
               ComponentConfig{}
                   .with_label("Coral")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_custom_color(afterhours::Color{255, 127, 80, 255})
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_coral"))) {
      click_counts[10]++;
    }

    // Custom color button - teal
    if (button(context, mk(row4.ent(), 2),
               ComponentConfig{}
                   .with_label("Teal")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_custom_color(afterhours::Color{0, 128, 128, 255})
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_teal"))) {
      click_counts[11]++;
    }

    // No rounded corners
    button(context, mk(row4.ent(), 3),
           ComponentConfig{}
               .with_label("Sharp")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_margin(Spacing::xs)
               .disable_rounded_corners()
               .with_debug_name("btn_sharp"));

    // Click counter display
    int total_clicks = 0;
    for (int i = 0; i < 12; i++) {
      total_clicks += click_counts[i];
    }
    std::string counter_text = "Total clicks: " + std::to_string(total_clicks);

    div(context, mk(main_container.ent(), 5),
        ComponentConfig{}
            .with_label(counter_text)
            .with_size(ComponentSize{screen_pct(0.9f), pixels(40)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.top = DefaultSpacing::medium(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("click_counter"));
  }
};

REGISTER_EXAMPLE_SCREEN(buttons, "Button component gallery with states and sizes",
                        ButtonsGallery)

