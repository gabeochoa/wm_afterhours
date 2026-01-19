#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CheckboxShowcase : ScreenSystem<UIContext<InputAction>> {
  // Basic checkbox values
  bool basic_checked = true;
  bool basic_unchecked = false;

  // Checkbox with label values
  bool labeled_primary = true;
  bool labeled_secondary = false;
  bool labeled_accent = true;

  // No-label checkbox values
  bool no_label_1 = true;
  bool no_label_2 = false;
  bool no_label_3 = true;
  bool no_label_4 = false;

  // Disabled checkbox values
  bool disabled_checked = true;
  bool disabled_unchecked = false;

  // Checkbox group values (bitset for multi-select)
  std::bitset<4> options_group{0b0101}; // Options 1 and 3 selected
  std::bitset<3> min_max_group{0b001};  // One selected (min 1, max 2)

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply a clean theme for this showcase
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container background - centered on screen with safe margins
    // Use smaller size to prevent internal overflow issues
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_debug_name("checkbox_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("checkbox_main"));

    // Title - centered text with proper sizing
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Checkbox Components")
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

    // Content area - two columns with proper spacing
    auto content = div(context, mk(main_container.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
                           .with_custom_background(theme.surface)
                           .with_padding(Spacing::md)
                           .with_flex_direction(FlexDirection::Row)
                           .with_justify_content(JustifyContent::SpaceBetween)
                           .with_debug_name("content"));

    // ========== Left column - Basic types ==========
    auto left_col =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()  // Prevent horizontal wrapping
                .with_debug_name("left_column"));

    // === Section: Checkbox with Label ===
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("With Label")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("labeled_header"));

    checkbox(context, mk(left_col.ent(), 1), labeled_primary,
             ComponentConfig{}
                 .with_label("Primary Color")
                 .with_size(ComponentSize{percent(1.0f), pixels(52)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_debug_name("checkbox_primary"));

    checkbox(context, mk(left_col.ent(), 2), labeled_secondary,
             ComponentConfig{}
                 .with_label("Secondary Color")
                 .with_size(ComponentSize{percent(1.0f), pixels(52)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_debug_name("checkbox_secondary"));

    checkbox(context, mk(left_col.ent(), 3), labeled_accent,
             ComponentConfig{}
                 .with_label("Accent Color")
                 .with_size(ComponentSize{percent(1.0f), pixels(52)})
                 .with_background(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_debug_name("checkbox_accent"));

    // === Section: Checkbox without Label ===
    div(context, mk(left_col.ent(), 4),
        ComponentConfig{}
            .with_label("Box Only (No Label)")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("no_label_header"));

    // Row of no-label checkboxes
    auto no_label_row =
        div(context, mk(left_col.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(50)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_align_items(AlignItems::Center)
                .with_debug_name("no_label_row"));

    checkbox_no_label(context, mk(no_label_row.ent(), 0), no_label_1,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(40), pixels(40)})
                          .with_background(Theme::Usage::Primary)
                          .with_font(UIComponent::SYMBOL_FONT, 24.0f)
                          .with_debug_name("no_label_1"));

    checkbox_no_label(context, mk(no_label_row.ent(), 1), no_label_2,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(40), pixels(40)})
                          .with_background(Theme::Usage::Secondary)
                          .with_font(UIComponent::SYMBOL_FONT, 24.0f)
                          .with_debug_name("no_label_2"));

    checkbox_no_label(context, mk(no_label_row.ent(), 2), no_label_3,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(40), pixels(40)})
                          .with_background(Theme::Usage::Accent)
                          .with_font(UIComponent::SYMBOL_FONT, 24.0f)
                          .with_debug_name("no_label_3"));

    checkbox_no_label(context, mk(no_label_row.ent(), 3), no_label_4,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(40), pixels(40)})
                          .with_background(Theme::Usage::Primary)
                          .with_font(UIComponent::SYMBOL_FONT, 24.0f)
                          .with_debug_name("no_label_4"));

    // === Section: Disabled Checkboxes ===
    div(context, mk(left_col.ent(), 6),
        ComponentConfig{}
            .with_label("Disabled State")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("disabled_header"));

    checkbox(context, mk(left_col.ent(), 7), disabled_checked,
             ComponentConfig{}
                 .with_label("Disabled (Checked)")
                 .with_size(ComponentSize{percent(1.0f), pixels(52)})
                 .with_background(Theme::Usage::Primary)
                 .with_disabled(true)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_debug_name("disabled_checked"));

    checkbox(context, mk(left_col.ent(), 8), disabled_unchecked,
             ComponentConfig{}
                 .with_label("Disabled (Unchecked)")
                 .with_size(ComponentSize{percent(1.0f), pixels(52)})
                 .with_background(Theme::Usage::Primary)
                 .with_disabled(true)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_debug_name("disabled_unchecked"));

    // ========== Right column - Groups ==========
    auto right_col =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()  // Prevent horizontal wrapping
                .with_debug_name("right_column"));

    // === Section: Checkbox Group (no min/max) ===
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("Multi-Select Group")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("group_header"));

    // Simple checkbox group - select any combination
    // Use percent(0.95f) width to allow buffer for internal checkbox sizing
    checkbox_group(context, mk(right_col.ent(), 1), options_group,
                   std::array<std::string_view, 4>{"Option A", "Option B",
                                                   "Option C", "Option D"},
                   {-1, -1}, // No min/max constraints
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.95f), children()})
                       .with_flex_direction(FlexDirection::Column)
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                       .with_debug_name("options_group"));

    // === Section: Checkbox Group with Min/Max ===
    div(context, mk(right_col.ent(), 2),
        ComponentConfig{}
            .with_label("Min/Max Constraints")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("minmax_header"));

    // Checkbox group with min=1, max=2 constraint
    // Use percent(0.95f) width to allow buffer for internal checkbox sizing
    checkbox_group(
        context, mk(right_col.ent(), 3), min_max_group,
        std::array<std::string_view, 3>{"Choice 1", "Choice 2", "Choice 3"},
        {1, 2}, // Min 1, max 2 selections
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.95f), children()})
            .with_flex_direction(FlexDirection::Column)
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("minmax_group"));

    div(context, mk(right_col.ent(), 4),
        ComponentConfig{}
            .with_label("(min: 1, max: 2 allowed)")
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_text_color(theme.darkfont)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_debug_name("minmax_note"));

    // Status display
    std::string status = "Selected: ";
    status += labeled_primary ? "[Primary] " : "";
    status += labeled_secondary ? "[Secondary] " : "";
    status += labeled_accent ? "[Accent] " : "";
    status += "| Group: " + std::to_string(options_group.count()) + "/4";
    status += " | MinMax: " + std::to_string(min_max_group.count()) + "/3";

    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes, "Component Galleries",
                        "All checkbox component types and variations",
                        CheckboxShowcase)
