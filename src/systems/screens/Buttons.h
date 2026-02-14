#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ButtonsGallery : ScreenSystem<UIContext<InputAction>> {
  int click_counts[12] = {0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply ocean navy theme with default font configuration
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT, h720(16.0f));

    // Main container background - centered on screen with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_padding(Spacing::lg)  // Padding on root, not child
                .with_debug_name("buttons_bg"));

    // Content container - no padding since root has it
    auto main_container =
        vstack(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_justify_content(JustifyContent::SpaceAround)
                .with_no_wrap()  // Prevent flex wrapping
                .with_debug_name("buttons_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Button Gallery")
            .with_size(ComponentSize{percent(1.0f), pixels(70)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(36.0f))
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)}));

    // Row 1: Basic button states
    auto row1 = hstack(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(80)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::sm)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("row1_states"));

    // Section label - fixed width for alignment, auto height to center with controls
    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("States:")
            .with_size(ComponentSize{pixels(100), pixels(45)})
            .with_background(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f)));

    // Normal button
    if (button(context, mk(row1.ent(), 1),
               ComponentConfig{}
                   .with_label("Normal")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_normal"))) {
      click_counts[0]++;
    }

    // Hover state is automatic, but show a button with accent color
    if (button(context, mk(row1.ent(), 2),
               ComponentConfig{}
                   .with_label("Accent")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_background(Theme::Usage::Accent)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_accent"))) {
      click_counts[1]++;
    }

    // Secondary button
    if (button(context, mk(row1.ent(), 3),
               ComponentConfig{}
                   .with_label("Secondary")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_secondary"))) {
      click_counts[2]++;
    }

    // Disabled button
    // Enhanced disabled styling: reduced opacity + dashed border pattern
    // makes the disabled state more visually obvious per accessibility guidelines.
    // Configurable: adjust opacity (0.4-0.6) and border color as needed.
    constexpr float disabled_opacity = 0.5f;  // Configurable: lower = more obvious
    afterhours::Color disabled_border_color{120, 120, 130, 200};  // Configurable: muted border
    button(context, mk(row1.ent(), 4),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_disabled(true)
               .with_opacity(disabled_opacity)
               .with_border(disabled_border_color, 2.0f)
               .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
               .with_margin(Spacing::xs)
               .with_roundness(0.08f));

    // Row 2: Different sizes - AlignItems::Center vertically centers buttons
    auto row2 = hstack(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(90)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::sm)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row2_sizes"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("Sizes:")
            .with_size(ComponentSize{pixels(100), pixels(45)})
            .with_background(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f)));

    // Small button - increased height to meet 44px touch target, larger font for visibility
    if (button(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_label("Small")
                   .with_size(ComponentSize{pixels(90), pixels(44)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_padding(Spacing::xs)
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_small"))) {
      click_counts[3]++;
    }

    // Medium button
    if (button(context, mk(row2.ent(), 2),
               ComponentConfig{}
                   .with_label("Medium")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_medium"))) {
      click_counts[4]++;
    }

    // Large button
    if (button(context, mk(row2.ent(), 3),
               ComponentConfig{}
                   .with_label("Large Button")
                   .with_size(ComponentSize{pixels(180), pixels(60)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)
                   .with_debug_name("btn_large"))) {
      click_counts[5]++;
    }

    // Row 3: Button group - need more height to accommodate the group
    auto row3 = hstack(context, mk(main_container.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(100)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::sm)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row3_group"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("Group:")
            .with_size(ComponentSize{pixels(100), pixels(45)})
            .with_background(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f)));

    // Create individual buttons styled as a group with gaps between them
    std::array<std::string_view, 4> group_labels = {"One", "Two", "Three",
                                                    "Four"};
    for (size_t i = 0; i < group_labels.size(); i++) {
      auto corners = RoundedCorners();
      if (i == 0)
        corners = corners.sharp(TOP_RIGHT).sharp(BOTTOM_RIGHT);
      else if (i == group_labels.size() - 1)
        corners = corners.sharp(TOP_LEFT).sharp(BOTTOM_LEFT);
      else
        corners = corners.all_sharp();

      if (button(context, mk(row3.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(std::string(group_labels[i]))
                     .with_size(ComponentSize{pixels(90), pixels(45)})
                     .with_background(Theme::Usage::Primary)
                     .with_auto_text_color(true)
                     .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                     .with_rounded_corners(corners)
                     .with_margin(Spacing::xs)
                     .with_debug_name("btn_group_" + std::to_string(i)))) {
        click_counts[6 + i]++;
      }
    }

    // Row 4: Custom styled buttons
    auto row4 = hstack(context, mk(main_container.ent(), 4),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(80)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::sm)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small(),
                                            .bottom = pixels(0),
                                            .left = pixels(0),
                                            .right = pixels(0)})
                        .with_debug_name("row4_custom"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Custom:")
            .with_size(ComponentSize{pixels(100), pixels(45)})
            .with_background(Theme::Usage::Surface)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f)));

    // Custom color button - coral
    if (button(context, mk(row4.ent(), 1),
               ComponentConfig{}
                   .with_label("Coral")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_custom_background(afterhours::Color{255, 127, 80, 255})
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)  // Match theme default roundness
                   .with_debug_name("btn_coral"))) {
      click_counts[10]++;
    }

    // Custom color button - teal (darker for better contrast)
    if (button(context, mk(row4.ent(), 2),
               ComponentConfig{}
                   .with_label("Teal")
                   .with_size(ComponentSize{pixels(120), pixels(45)})
                   .with_custom_background(afterhours::Color{0, 100, 100, 255})
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Spacing::xs)
                   .with_roundness(0.08f)  // Match Coral button roundness
                   .with_debug_name("btn_teal"))) {
      click_counts[11]++;
    }

    // Sharp corners button - demonstrates .disable_rounded_corners()
    // STYLE GUIDE: When to use sharp vs rounded corners:
    // - ROUNDED (default, 0.08f): Standard buttons, form actions, navigation
    //   Creates a friendly, approachable feel. Use for primary user interactions.
    // - SHARP: Technical/utility buttons, toolbars, button groups, data tables
    //   Creates a precise, structured feel. Use for secondary UI or dense layouts.
    // - MIXED (as in button groups above): Sharp edges where buttons meet,
    //   rounded on outer edges for visual grouping.
    button(context, mk(row4.ent(), 3),
           ComponentConfig{}
               .with_label("Sharp")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
               .with_margin(Spacing::xs)
               .disable_rounded_corners());

    // Click counter display
    int total_clicks = 0;
    for (int i = 0; i < 12; i++) {
      total_clicks += click_counts[i];
    }
    std::string counter_text = "Total clicks across all buttons: " + std::to_string(total_clicks);

    div(context, mk(main_container.ent(), 5),
        ComponentConfig{}
            .with_label(counter_text)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(afterhours::colors::lighten(theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f)));
  }
};

REGISTER_EXAMPLE_SCREEN(buttons, "Component Galleries",
                        "Button component gallery with states and sizes",
                        ButtonsGallery)
