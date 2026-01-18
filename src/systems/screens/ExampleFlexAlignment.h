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

  // Vertical justify demo (Column direction) - compact
  void render_justify_vertical(UIContext<InputAction> &context,
                               afterhours::Entity &parent,
                               const std::string &label, JustifyContent jc,
                               int id) {
    // Container with visible background
    auto container = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.18f), percent(0.92f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Padding{.top = pixels(6), .left = pixels(6), 
                                  .bottom = pixels(6), .right = pixels(6)})
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2), 
                                .left = pixels(3), .right = pixels(3)})
            .with_flex_direction(FlexDirection::Column)
            .with_justify_content(jc)
            .with_debug_name(label));

    // 3 boxes - tall enough to show spacing clearly
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{percent(0.85f), pixels(28)})
              .with_background(Theme::Usage::Primary)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
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
            .with_size(ComponentSize{percent(0.18f), percent(0.92f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Padding{.top = pixels(4), .left = pixels(4), 
                                  .bottom = pixels(4), .right = pixels(4)})
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2), 
                                .left = pixels(3), .right = pixels(3)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name(label + "_outer"));

    // Title
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{percent(0.92f), pixels(22)})
            .with_custom_background(afterhours::Color{80, 90, 110, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name(label + "_title"));

    // Inner row container with justify
    auto inner = div(
        context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.92f), pixels(40)})
            .with_custom_background(afterhours::Color{35, 45, 65, 255})
            .with_padding(Padding{.top = pixels(4), .left = pixels(4), 
                                  .bottom = pixels(4), .right = pixels(4)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(jc)
            .with_debug_name(label + "_inner"));

    // Three boxes
    for (int i = 0; i < 3; i++) {
      div(context, mk(inner.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(26), pixels(28)})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name(label + "_" + std::to_string(i)));
    }
  }

  // Align items demo - compact
  void render_align_demo(UIContext<InputAction> &context,
                         afterhours::Entity &parent, const std::string &label,
                         AlignItems ai, int id) {
    // Container with visible background
    auto container = div(
        context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.31f), percent(0.92f)})
            .with_custom_background(afterhours::Color{50, 60, 80, 255})
            .with_padding(Padding{.top = pixels(6), .left = pixels(8), 
                                  .bottom = pixels(6), .right = pixels(8)})
            .with_margin(Margin{.top = pixels(2), .bottom = pixels(2), 
                                .left = pixels(4), .right = pixels(4)})
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(ai)
            .with_debug_name(label));

    // Title at left
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(75), pixels(24)})
            .with_custom_background(afterhours::Color{80, 90, 110, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name(label + "_title"));

    // Three boxes of different heights
    const float heights[] = {55.0f, 35.0f, 45.0f};
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i + 1),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(35), pixels(heights[i])})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
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
            .with_size(ComponentSize{percent(0.96f), percent(0.92f)})
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
            .with_size(ComponentSize{pixels(70), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_self_align(SelfAlign::FlexStart)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("self_start"));

    // Auto - aligns to MIDDLE
    div(context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_label("Auto")
            .with_size(ComponentSize{pixels(60), pixels(32)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_self_align(SelfAlign::Auto)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("self_auto"));

    // Center - aligns to MIDDLE
    div(context, mk(container.ent(), 2),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(70), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_self_align(SelfAlign::Center)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("self_center"));

    // FlexEnd - aligns to BOTTOM
    div(context, mk(container.ent(), 3),
        ComponentConfig{}
            .with_label("End")
            .with_size(ComponentSize{pixels(60), pixels(32)})
            .with_custom_background(afterhours::Color{180, 60, 90, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_self_align(SelfAlign::FlexEnd)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("self_end"));
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
            .with_custom_background(theme.background)
            .with_padding(Padding{.top = pixels(8), .left = pixels(12), 
                                  .bottom = pixels(8), .right = pixels(12)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("root"));

    // Title row
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Flexbox Alignment Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("title"));

    // Main content - 2 column layout
    auto main_row = div(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.88f)})
            .with_margin(Margin{.top = pixels(6)})
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

    // Section 1: Vertical justify
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("JustifyContent: Column")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_background(afterhours::Color{45, 55, 75, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("jv_label"));

    auto jv_row = div(
        context, mk(left_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(140)})
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

    // Section 2: AlignItems
    div(context, mk(left_col.ent(), 2),
        ComponentConfig{}
            .with_label("AlignItems: Row")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_background(afterhours::Color{45, 55, 75, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("ai_label"));

    auto ai_row = div(
        context, mk(left_col.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(90)})
            .with_custom_background(afterhours::Color{38, 46, 65, 255})
            .with_padding(Spacing::xs)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("ai_row"));

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

    // Section 3: Horizontal justify
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("JustifyContent: Row")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_background(afterhours::Color{45, 55, 75, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("jh_label"));

    auto jh_row = div(
        context, mk(right_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(100)})
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

    // Section 4: SelfAlign
    div(context, mk(right_col.ent(), 2),
        ComponentConfig{}
            .with_label("SelfAlign: Override Parent")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_background(afterhours::Color{45, 55, 75, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("sa_label"));

    auto sa_row = div(
        context, mk(right_col.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(80)})
            .with_custom_background(afterhours::Color{38, 46, 65, 255})
            .with_padding(Spacing::xs)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("sa_row"));

    render_self_align_demo(context, sa_row.ent(), 0);
  }
};

REGISTER_EXAMPLE_SCREEN(flex_alignment, "System Demos",
                        "Flexbox justify and align properties",
                        ExampleFlexAlignment)
