#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DropdownShowcase : ScreenSystem<UIContext<InputAction>> {
  // Selection indices for each dropdown
  size_t basic_idx = 0;
  size_t labeled_idx = 1;
  size_t many_idx = 0;
  size_t custom_indicator_idx = 0;
  size_t disabled_idx = 2;
  size_t styled_idx = 0;

  // Option data
  std::vector<std::string> basic_options = {"Alpha", "Beta", "Gamma"};
  std::vector<std::string> labeled_options = {"Dog", "Cat", "Bird", "Fish"};
  std::vector<std::string> many_options = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  std::vector<std::string> indicator_options = {"Option A", "Option B",
                                                "Option C"};
  std::vector<std::string> disabled_options = {"Locked", "Sealed", "Frozen"};
  std::vector<std::string> styled_options = {"Ruby", "Emerald", "Sapphire",
                                             "Topaz"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    constexpr float dropdown_w = 200.0f;
    constexpr float dropdown_h = 36.0f;
    constexpr float label_h = 28.0f;

    // Root container
    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.88f)})
                        .with_self_align(SelfAlign::Center)
                        .with_background(Theme::Usage::Background)
                        .with_roundness(0.08f)
                        .with_debug_name("dropdown_bg"));

    auto main = vstack(context, mk(root.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                           .with_padding(Spacing::md)
                           .with_justify_content(JustifyContent::FlexStart)
                           .with_debug_name("dropdown_main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_label("Dropdown Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, pixels(28.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_skip_tabbing(true));

    // Content area - two rows of three columns
    auto row1 = hstack(context, mk(main.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(200)})
                           .with_justify_content(JustifyContent::SpaceAround)
                           .with_debug_name("row1"));

    // === Column 1: Basic dropdown ===
    auto col1 = vstack(context, mk(row1.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_basic"));

    div(context, mk(col1.ent(), 0),
        ComponentConfig{}
            .with_label("Basic")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col1.ent(), 1), basic_options, basic_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w), pixels(dropdown_h)})
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("basic_dropdown"));

    // === Column 2: Dropdown with label ===
    auto col2 = vstack(context, mk(row1.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_labeled"));

    div(context, mk(col2.ent(), 0),
        ComponentConfig{}
            .with_label("With Label")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col2.ent(), 1), labeled_options, labeled_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w + 80), pixels(dropdown_h)})
                 .with_label("Pet")
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("labeled_dropdown"));

    // === Column 3: Many options ===
    auto col3 = vstack(context, mk(row1.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_many"));

    div(context, mk(col3.ent(), 0),
        ComponentConfig{}
            .with_label("Many Options (12)")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col3.ent(), 1), many_options, many_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w), pixels(dropdown_h)})
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("many_dropdown"));

    // Second row
    auto row2 = hstack(context, mk(main.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(200)})
                           .with_justify_content(JustifyContent::SpaceAround)
                           .with_margin(Margin{.top = DefaultSpacing::medium()})
                           .with_debug_name("row2"));

    // === Column 4: Custom indicators ===
    auto col4 = vstack(context, mk(row2.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_indicators"));

    div(context, mk(col4.ent(), 0),
        ComponentConfig{}
            .with_label("Custom Indicators")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col4.ent(), 1), indicator_options, custom_indicator_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w), pixels(dropdown_h)})
                 .with_dropdown_indicators(" [-]", " [+]")
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("indicator_dropdown"));

    // === Column 5: Disabled ===
    auto col5 = vstack(context, mk(row2.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_disabled"));

    div(context, mk(col5.ent(), 0),
        ComponentConfig{}
            .with_label("Disabled")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col5.ent(), 1), disabled_options, disabled_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w), pixels(dropdown_h)})
                 .with_disabled(true)
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("disabled_dropdown"));

    // === Column 6: Styled ===
    auto col6 = vstack(context, mk(row2.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                           .with_custom_background(
                               afterhours::colors::darken(theme.surface, 0.95f))
                           .with_border(theme.accent, 1.5f)
                           .with_padding(Spacing::sm)
                           .with_roundness(0.08f)
                           .with_debug_name("col_styled"));

    div(context, mk(col6.ent(), 0),
        ComponentConfig{}
            .with_label("Custom Styled")
            .with_size(ComponentSize{percent(1.0f), pixels(label_h)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    dropdown(context, mk(col6.ent(), 1), styled_options, styled_idx,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(dropdown_w), pixels(dropdown_h)})
                 .with_background(Theme::Usage::Accent)
                 .with_border(theme.font, 2.0f)
                 .with_roundness(0.3f)
                 .with_margin(Margin{.top = DefaultSpacing::small()})
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                 .with_debug_name("styled_dropdown"));

    // Status bar
    std::string status = fmt::format(
        "Basic={}, Pet={}, Month={}, Indicator={}, Disabled={}, Styled={}",
        basic_options[basic_idx], labeled_options[labeled_idx],
        many_options[many_idx], indicator_options[custom_indicator_idx],
        disabled_options[disabled_idx], styled_options[styled_idx]);

    div(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(
                afterhours::colors::lighten(context.theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_roundness(0.1f)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(dropdowns, "Component Galleries",
                        "Dropdown variations: basic, labeled, many options, "
                        "custom indicators, disabled, styled",
                        DropdownShowcase)
