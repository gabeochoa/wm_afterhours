#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates flexbox alignment properties in a clean 2-column layout
struct ExampleFlexAlignment : ScreenSystem<UIContext<InputAction>> {
  // Interactive tab selector - 0: All, 1: Justify Column, 2: Justify Row, 3: Align Items, 4: Self Align
  size_t active_category = 0;

  std::array<std::string_view, 5> category_labels = {
    "All", "Vertical", "Horizontal", "Cross-Axis", "Self Align"
  };

  // Vertical justify demo (Column direction) - compact
  void render_justify_vertical(UIContext<InputAction> &context,
                               afterhours::Entity &parent,
                               const std::string &label, JustifyContent jc,
                               int id) {
    // Outer container to hold title + demo
    auto outer = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.18f), percent(1.0f)})
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2),
                                .left = pixels(3), .right = pixels(3)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name(label + "_outer"));

    // Title label
    div(context, mk(outer.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_background(afterhours::Color{80, 90, 110, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_skip_tabbing(true)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name(label + "_title"));

    // Container with visible background for demo items
    auto container = div(
        context, mk(outer.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Spacing::xs)
            .with_margin(Margin{.top = pixels(4)})
            .with_flex_direction(FlexDirection::Column)
            .with_justify_content(jc)
            .with_debug_name(label + "_container"));

    // 3 boxes - sized to fit within container
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{percent(0.85f), percent(0.22f)})
              .with_background(Theme::Usage::Primary)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name(label + "_" + std::to_string(i)));
    }
  }

  // Horizontal justify demo (Row direction) - compact
  void render_justify_horizontal(UIContext<InputAction> &context,
                                 afterhours::Entity &parent,
                                 const std::string &label, JustifyContent jc,
                                 int id) {
    // Container with visible background
    auto container = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.18f), percent(1.0f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Spacing::xs)
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2),
                                .left = pixels(3), .right = pixels(3)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name(label + "_outer"));

    // Title
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_background(afterhours::Color{80, 90, 110, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_skip_tabbing(true)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name(label + "_title"));

    // Inner row container with justify
    auto inner = div(
        context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.60f)})
            .with_custom_background(afterhours::Color{35, 45, 65, 255})
            .with_padding(Spacing::xs)
            .with_margin(Margin{.top = pixels(4)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(jc)
            .with_debug_name(label + "_inner"));

    // Three boxes
    for (int i = 0; i < 3; i++) {
      div(context, mk(inner.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(26), percent(0.70f)})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name(label + "_" + std::to_string(i)));
    }
  }

  // Align items demo - compact
  void render_align_demo(UIContext<InputAction> &context,
                         afterhours::Entity &parent, const std::string &label,
                         AlignItems ai, int id) {
    // Outer container to hold title + demo
    auto outer = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.31f), percent(1.0f)})
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2),
                                .left = pixels(4), .right = pixels(4)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name(label + "_outer"));

    // Title label at top
    div(context, mk(outer.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_background(afterhours::Color{80, 90, 110, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_skip_tabbing(true)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name(label + "_title"));

    // Container with visible background and align items
    auto container = div(
        context, mk(outer.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Padding{.top = pixels(6), .left = pixels(8),
                                  .bottom = pixels(6), .right = pixels(8)})
            .with_margin(Margin{.top = pixels(4)})
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(ai)
            .with_debug_name(label + "_container"));

    // Three boxes of different heights using percent of container
    const float heights[] = {0.70f, 0.45f, 0.55f};
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(40), percent(heights[i])})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name(label + "_" + std::to_string(i)));
    }
  }

  // SelfAlign demo - compact
  void render_self_align_demo(UIContext<InputAction> &context,
                              afterhours::Entity &parent, int id) {
    // Container with AlignItems::Center
    auto container = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Padding{.top = pixels(6), .left = pixels(10),
                                  .bottom = pixels(6), .right = pixels(10)})
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_debug_name("self_align_container"));

    // FlexStart - aligns to TOP
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label("Start")
            .with_size(ComponentSize{pixels(70), percent(0.25f)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_self_align(SelfAlign::FlexStart)
            .with_flex_direction(FlexDirection::Column));

    // Auto - aligns to MIDDLE
    div(context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_label("Auto")
            .with_size(ComponentSize{pixels(60), percent(0.25f)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_self_align(SelfAlign::Auto)
            .with_flex_direction(FlexDirection::Column));

    // Center - aligns to MIDDLE
    div(context, mk(container.ent(), 2),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(70), percent(0.25f)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_self_align(SelfAlign::Center)
            .with_flex_direction(FlexDirection::Column));

    // FlexEnd - aligns to BOTTOM
    div(context, mk(container.ent(), 3),
        ComponentConfig{}
            .with_label("End")
            .with_size(ComponentSize{pixels(60), percent(0.25f)})
            .with_custom_background(afterhours::Color{180, 60, 90, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_self_align(SelfAlign::FlexEnd)
            .with_flex_direction(FlexDirection::Column));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Full screen root
    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(0.95f)})
            .with_background(Theme::Usage::Background)
            .with_padding(Padding{.top = pixels(8), .left = pixels(12),
                                  .bottom = pixels(8), .right = pixels(12)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("root"));

    // Title row
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Flexbox Alignment Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(24.0f))
            .with_flex_direction(FlexDirection::Row));

    // Tab selector for focusing on categories
    auto tab_row = div(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_margin(Margin{.top = pixels(6), .bottom = pixels(6)})
            .with_flex_direction(FlexDirection::Row));

    // Use tab_container for category selection
    if (auto result = tab_container(context, mk(tab_row.ent(), 0), category_labels, active_category,
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(40)})
                .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                .with_debug_name("category_tabs")); result) {
      // Tab changed
    }

    // Show All categories (original 2-column layout) or individual category
    if (active_category == 0) {
      // Main content - 2 column layout (original view)
      auto main_row = div(
          context, mk(root.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.82f)})
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name("main_row"));

      // LEFT COLUMN - Vertical demos
      auto left_col = div(
          context, mk(main_row.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
              .with_custom_background(afterhours::Color{30, 38, 55, 255})
              .with_padding(Padding{.top = pixels(6), .left = pixels(8),
                                    .bottom = pixels(6), .right = pixels(8)})
              .with_margin(Margin{.right = pixels(6)})
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name("left_col"));

      // Section 1: Vertical justify - user-friendly description
      div(context, mk(left_col.ent(), 0),
          ComponentConfig{}
              .with_label("Vertical Spacing (JustifyContent, Column)")
              .with_size(ComponentSize{percent(1.0f), pixels(28)})
              .with_custom_background(afterhours::Color{45, 55, 75, 255})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_flex_direction(FlexDirection::Row));

      auto jv_row = div(
          context, mk(left_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.45f)})
              .with_custom_background(afterhours::Color{38, 46, 65, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.bottom = pixels(8)})
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name("jv_row"));

      render_justify_vertical(context, jv_row.ent(), "Start", JustifyContent::FlexStart, 0);
      render_justify_vertical(context, jv_row.ent(), "End", JustifyContent::FlexEnd, 1);
      render_justify_vertical(context, jv_row.ent(), "Center", JustifyContent::Center, 2);
      render_justify_vertical(context, jv_row.ent(), "Between", JustifyContent::SpaceBetween, 3);
      render_justify_vertical(context, jv_row.ent(), "Around", JustifyContent::SpaceAround, 4);

      // Section 2: AlignItems - user-friendly description
      div(context, mk(left_col.ent(), 2),
          ComponentConfig{}
              .with_label("Cross-Axis Align (AlignItems)")
              .with_size(ComponentSize{percent(1.0f), pixels(28)})
              .with_custom_background(afterhours::Color{45, 55, 75, 255})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_flex_direction(FlexDirection::Row));

      auto ai_row = div(
          context, mk(left_col.ent(), 3),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.40f)})
              .with_custom_background(afterhours::Color{38, 46, 65, 255})
              .with_padding(Spacing::xs)
              .with_flex_direction(FlexDirection::Row));

      render_align_demo(context, ai_row.ent(), "Start", AlignItems::FlexStart, 0);
      render_align_demo(context, ai_row.ent(), "Center", AlignItems::Center, 1);
      render_align_demo(context, ai_row.ent(), "End", AlignItems::FlexEnd, 2);

      // RIGHT COLUMN - Horizontal demos
      auto right_col = div(
          context, mk(main_row.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
              .with_custom_background(afterhours::Color{30, 38, 55, 255})
              .with_padding(Padding{.top = pixels(6), .left = pixels(8),
                                    .bottom = pixels(6), .right = pixels(8)})
              .with_margin(Margin{.left = pixels(6)})
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name("right_col"));

      // Section 3: Horizontal justify - user-friendly description
      div(context, mk(right_col.ent(), 0),
          ComponentConfig{}
              .with_label("Horizontal Spacing (JustifyContent, Row)")
              .with_size(ComponentSize{percent(1.0f), pixels(28)})
              .with_custom_background(afterhours::Color{45, 55, 75, 255})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_flex_direction(FlexDirection::Row));

      auto jh_row = div(
          context, mk(right_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.35f)})
              .with_custom_background(afterhours::Color{38, 46, 65, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.bottom = pixels(8)})
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name("jh_row"));

      render_justify_horizontal(context, jh_row.ent(), "Start", JustifyContent::FlexStart, 0);
      render_justify_horizontal(context, jh_row.ent(), "End", JustifyContent::FlexEnd, 1);
      render_justify_horizontal(context, jh_row.ent(), "Center", JustifyContent::Center, 2);
      render_justify_horizontal(context, jh_row.ent(), "Between", JustifyContent::SpaceBetween, 3);
      render_justify_horizontal(context, jh_row.ent(), "Around", JustifyContent::SpaceAround, 4);

      // Section 4: SelfAlign - user-friendly description
      div(context, mk(right_col.ent(), 2),
          ComponentConfig{}
              .with_label("Individual Override (SelfAlign)")
              .with_size(ComponentSize{percent(1.0f), pixels(28)})
              .with_custom_background(afterhours::Color{45, 55, 75, 255})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_flex_direction(FlexDirection::Row));

      auto sa_row = div(
          context, mk(right_col.ent(), 3),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.50f)})
              .with_custom_background(afterhours::Color{38, 46, 65, 255})
              .with_padding(Spacing::xs)
              .with_flex_direction(FlexDirection::Row));

      render_self_align_demo(context, sa_row.ent(), 0);
    } else {
      // Single category focused view - more space for selected category
      auto content_panel = div(
          context, mk(root.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.82f)})
              .with_custom_background(afterhours::Color{30, 38, 55, 255})
              .with_padding(Padding{.top = pixels(12), .left = pixels(16),
                                    .bottom = pixels(12), .right = pixels(16)})
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name("content_panel"));

      if (active_category == 1) {
        // Vertical Spacing (JustifyContent: Column)
        div(context, mk(content_panel.ent(), 0),
            ComponentConfig{}
                .with_label("Vertical Spacing (JustifyContent, Column): Items distributed top-to-bottom")
                .with_size(ComponentSize{percent(1.0f), pixels(36)})
                .with_custom_background(afterhours::Color{45, 55, 75, 255})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.bottom = pixels(12)}));

        auto jv_row = div(
            context, mk(content_panel.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
                .with_custom_background(afterhours::Color{38, 46, 65, 255})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("jv_row_focused"));

        render_justify_vertical(context, jv_row.ent(), "Start", JustifyContent::FlexStart, 0);
        render_justify_vertical(context, jv_row.ent(), "End", JustifyContent::FlexEnd, 1);
        render_justify_vertical(context, jv_row.ent(), "Center", JustifyContent::Center, 2);
        render_justify_vertical(context, jv_row.ent(), "Between", JustifyContent::SpaceBetween, 3);
        render_justify_vertical(context, jv_row.ent(), "Around", JustifyContent::SpaceAround, 4);

      } else if (active_category == 2) {
        // Horizontal Spacing (JustifyContent: Row)
        div(context, mk(content_panel.ent(), 0),
            ComponentConfig{}
                .with_label("Horizontal Spacing (JustifyContent, Row): Items distributed left-to-right")
                .with_size(ComponentSize{percent(1.0f), pixels(36)})
                .with_custom_background(afterhours::Color{45, 55, 75, 255})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.bottom = pixels(12)}));

        auto jh_row = div(
            context, mk(content_panel.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.70f)})
                .with_custom_background(afterhours::Color{38, 46, 65, 255})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("jh_row_focused"));

        render_justify_horizontal(context, jh_row.ent(), "Start", JustifyContent::FlexStart, 0);
        render_justify_horizontal(context, jh_row.ent(), "End", JustifyContent::FlexEnd, 1);
        render_justify_horizontal(context, jh_row.ent(), "Center", JustifyContent::Center, 2);
        render_justify_horizontal(context, jh_row.ent(), "Between", JustifyContent::SpaceBetween, 3);
        render_justify_horizontal(context, jh_row.ent(), "Around", JustifyContent::SpaceAround, 4);

      } else if (active_category == 3) {
        // Cross-Axis Align (AlignItems)
        div(context, mk(content_panel.ent(), 0),
            ComponentConfig{}
                .with_label("Cross-Axis Align (AlignItems): Position items perpendicular to main axis")
                .with_size(ComponentSize{percent(1.0f), pixels(36)})
                .with_custom_background(afterhours::Color{45, 55, 75, 255})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.bottom = pixels(12)}));

        auto ai_row = div(
            context, mk(content_panel.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
                .with_custom_background(afterhours::Color{38, 46, 65, 255})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("ai_row_focused"));

        render_align_demo(context, ai_row.ent(), "Start", AlignItems::FlexStart, 0);
        render_align_demo(context, ai_row.ent(), "Center", AlignItems::Center, 1);
        render_align_demo(context, ai_row.ent(), "End", AlignItems::FlexEnd, 2);

      } else if (active_category == 4) {
        // Individual Override (SelfAlign)
        div(context, mk(content_panel.ent(), 0),
            ComponentConfig{}
                .with_label("Individual Override (SelfAlign): Each item overrides parent alignment")
                .with_size(ComponentSize{percent(1.0f), pixels(36)})
                .with_custom_background(afterhours::Color{45, 55, 75, 255})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.bottom = pixels(12)}));

        auto sa_row = div(
            context, mk(content_panel.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
                .with_custom_background(afterhours::Color{38, 46, 65, 255})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("sa_row_focused"));

        render_self_align_demo(context, sa_row.ent(), 0);
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(flex_alignment, "System Demos",
                        "Flexbox justify and align properties",
                        ExampleFlexAlignment)
