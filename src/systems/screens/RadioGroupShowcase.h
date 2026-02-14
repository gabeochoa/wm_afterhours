#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Configurable radio button styling parameters
struct RadioButtonConfig {
  float visual_circle_size = 28.0f;  // Outer ring size (larger for better visibility)
  float dot_size = 18.0f;            // Inner dot size (larger for clearer indicator)
  float border_width = 3.0f;         // Border thickness
  float group_spacing = 16.0f;       // Extra spacing between groups
  bool use_accent_color = true;      // Use accent color for selected indicator
};

// Custom radio group with configurable styling
// This allows per-showcase customization without modifying vendor code
template <size_t N>
ElementResult custom_radio_group(HasUIContext auto &ctx, EntityParent ep_pair,
                                 const std::array<std::string_view, N> &labels,
                                 size_t &selected_index,
                                 ComponentConfig config,
                                 const RadioButtonConfig &radio_config = RadioButtonConfig()) {
  auto [entity, parent] = deref(ep_pair);

  bool changed = false;

  constexpr float touch_target_sz = MIN_TOUCH_TARGET;
  const float visual_circle_sz = radio_config.visual_circle_size;
  const float dot_sz = radio_config.dot_size;
  const float border_w = radio_config.border_width;

  for (size_t i = 0; i < N; ++i) {
    bool is_selected = (i == selected_index);

    // Row button - transparent, for click handling
    auto row_size = config.size;
    if (row_size.y_axis.dim == Dim::Pixels && row_size.y_axis.value < touch_target_sz) {
      row_size.y_axis = pixels(touch_target_sz);
    }
    auto row = button(ctx, mk(parent, 100 + i),
                      ComponentConfig{}
                          .with_size(row_size)
                          .with_label("")
                          .with_background(Theme::Usage::None)
                          .with_flex_direction(FlexDirection::Row)
                          .with_align_items(AlignItems::Center)
                          .with_padding(Padding{.left = pixels(6)})
                          .with_debug_name(fmt::format("radio_{}", i)));

    if (row) {
      selected_index = i;
      changed = true;
    }

    // Outer circle ring - use accent color when selected for better visibility
    afterhours::Color ring_color = is_selected ? ctx.theme.accent : ctx.theme.font_muted;
    // Make the ring thicker when selected for additional emphasis
    float effective_border = is_selected ? border_w + 1.0f : border_w;

    auto ring =
        div(ctx, mk(row.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(visual_circle_sz), pixels(visual_circle_sz)})
                .with_background(Theme::Usage::Background)
                .with_border(ring_color, effective_border)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(1.0f)
                .with_margin(Margin{.right = pixels(12)})
                .with_skip_tabbing(true)
                .with_debug_name(fmt::format("radio_ring_{}", i)));

    // Inner filled dot when selected - larger and with accent color
    if (is_selected) {
      float offset = (visual_circle_sz - dot_sz) / 2.0f;
      div(ctx, mk(ring.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(dot_sz), pixels(dot_sz)})
              .with_absolute_position(offset, offset)
              .with_background(Theme::Usage::Accent)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(1.0f)
              .with_skip_tabbing(true)
              .with_debug_name(fmt::format("radio_dot_{}", i)));
    }

    // Label - positioned after circle
    auto label_ent =
        div(ctx, mk(row.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(150), row_size.y_axis})
                .with_label(std::string(labels[i]))
                .with_font(config.font_name, config.font_size)
                .with_custom_text_color(ctx.theme.font)
                .with_skip_tabbing(true)
                .with_debug_name(fmt::format("radio_label_{}", i)));

    // Force left alignment
    if (label_ent.ent().template has<HasLabel>()) {
      label_ent.ent().template get<HasLabel>().set_alignment(
          TextAlignment::Left);
    }
  }

  return {changed, parent, static_cast<int>(selected_index)};
}

struct RadioGroupShowcase : ScreenSystem<UIContext<InputAction>> {
  // Radio group values
  size_t fruit_idx = 1;
  size_t size_idx = 0;
  size_t color_idx = 2;

  // Configurable radio button styling - can be adjusted for better visibility
  RadioButtonConfig radio_style{
      .visual_circle_size = 28.0f,  // Larger outer ring for visibility
      .dot_size = 18.0f,            // Larger filled dot for clearer selection
      .border_width = 3.0f,         // Thicker border for emphasis
      .group_spacing = 20.0f,       // Extra spacing between groups
      .use_accent_color = true      // Use theme accent for selected state
  };

  std::array<std::string_view, 3> fruit_options = {"Apple", "Orange",
                                                    "Watermelon"};
  std::array<std::string_view, 3> size_options = {"Small", "Medium", "Large"};
  std::array<std::string_view, 4> color_options = {"Red", "Green", "Blue",
                                                    "Yellow"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container - centered on screen (wider to fit all groups with borders)
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.88f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_debug_name("radio_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_debug_name("radio_main"));

    // Title - increased font size for visibility
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Radio Button Groups")
            .with_size(ComponentSize{percent(1.0f), pixels(60)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::medium()}));

    // Content area - three columns with clearer group separation
    auto content =
        hstack(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.70f)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::sm)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_debug_name("content"));

    // Column 1 - Fruit selection (with border for clearer group separation)
    auto col1 =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.28f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_border(theme.accent, 2.0f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_roundness(0.08f)
                .with_debug_name("col1"));

    div(context, mk(col1.ent(), 0),
        ComponentConfig{}
            .with_label("Favorite Fruit")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_skip_tabbing(true));

    // Radio group with configurable styling for clearer indicator
    custom_radio_group(context, mk(col1.ent(), 1), fruit_options, fruit_idx,
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(44)})
                           .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                           .with_debug_name("fruit_radios"),
                       radio_style);

    // Column 2 - Size selection (with border for clearer group separation)
    auto col2 =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.28f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_border(theme.accent, 2.0f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_roundness(0.08f)
                .with_debug_name("col2"));

    div(context, mk(col2.ent(), 0),
        ComponentConfig{}
            .with_label("Size")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_skip_tabbing(true));

    // Radio group with configurable styling for clearer indicator
    custom_radio_group(context, mk(col2.ent(), 1), size_options, size_idx,
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(44)})
                           .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                           .with_debug_name("size_radios"),
                       radio_style);

    // Column 3 - Color selection (with border for clearer group separation)
    auto col3 =
        div(context, mk(content.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.28f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_border(theme.accent, 2.0f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_roundness(0.08f)
                .with_debug_name("col3"));

    div(context, mk(col3.ent(), 0),
        ComponentConfig{}
            .with_label("Color")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_skip_tabbing(true));

    // Radio group with configurable styling for clearer indicator
    custom_radio_group(context, mk(col3.ent(), 1), color_options, color_idx,
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(44)})
                           .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                           .with_debug_name("color_radios"),
                       radio_style);

    // Status display - 44px touch target
    std::string status =
        fmt::format("Selected: Fruit={}, Size={}, Color={}",
                    fruit_options[fruit_idx], size_options[size_idx],
                    color_options[color_idx]);

    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_custom_background(afterhours::colors::lighten(context.theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(radio_buttons, "Component Galleries",
                        "Radio button group with single-select behavior",
                        RadioGroupShowcase)
