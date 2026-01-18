#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the new flexbox alignment properties:
// - justify_content: FlexStart, FlexEnd, Center, SpaceBetween, SpaceAround
// - align_items: FlexStart, FlexEnd, Center
struct ExampleFlexAlignment : ScreenSystem<UIContext<InputAction>> {

  // Vertical justify demo (Column direction)
  void render_justify_vertical(UIContext<InputAction> &context,
                               afterhours::Entity &parent,
                               const std::string &label, JustifyContent jc,
                               int id) {
    auto &theme = context.theme;

    // Container - use Column direction, NO flex on children to prevent wrap
    auto container =
        div(context, mk(parent, id),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(150), pixels(120)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.92f))
                .with_padding(Padding{.top = pixels(4),
                                      .left = pixels(4),
                                      .bottom = pixels(4),
                                      .right = pixels(4)})
                .with_margin(Margin{.top = pixels(0), .bottom = pixels(0),
                                    .left = pixels(4), .right = pixels(4)})
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(jc)
                .with_debug_name(label));

    // Three boxes with labels like "FlexEnd 1" - use Row to prevent Column wrap
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i),
          ComponentConfig{}
              .with_label(label + " " + std::to_string(i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(28)})
              .with_background(Theme::Usage::Primary)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 12.0f)
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name(label + "_box_" + std::to_string(i)));
    }
  }

  // Horizontal justify demo (Row direction)
  void render_justify_horizontal(UIContext<InputAction> &context,
                                 afterhours::Entity &parent,
                                 const std::string &label, JustifyContent jc,
                                 int id) {
    auto &theme = context.theme;

    // Container for this demo - wide and short for horizontal layout
    auto container =
        div(context, mk(parent, id),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.18f), pixels(110)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.92f))
                .with_padding(Padding{.top = pixels(4),
                                      .left = pixels(4),
                                      .bottom = pixels(4),
                                      .right = pixels(4)})
                .with_margin(Spacing::xs)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name(label + "_outer"));

    // Title at top
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{percent(0.95f), pixels(22)})
            .with_custom_background(theme.secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_skip_tabbing(true)
            .with_debug_name(label + "_title"));

    // Inner row container with justify
    auto inner = div(context, mk(container.ent(), 1),
                     ComponentConfig{}
                         .with_size(ComponentSize{percent(1.0f), pixels(60)})
                         .with_flex_direction(FlexDirection::Row)
                         .with_justify_content(jc)
                         .with_debug_name(label + "_inner"));

    // Three boxes of different widths
    const float widths[] = {35.0f, 25.0f, 30.0f};
    for (int i = 0; i < 3; i++) {
      div(context, mk(inner.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(widths[i]), pixels(50)})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_debug_name(label + "_box_" + std::to_string(i)));
    }
  }

  void render_align_demo(UIContext<InputAction> &context,
                         afterhours::Entity &parent, const std::string &label,
                         AlignItems ai, int id) {
    auto &theme = context.theme;

    // Container for this demo
    auto container =
        div(context, mk(parent, id),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.30f), pixels(105)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.92f))
                .with_padding(Padding{.top = pixels(4),
                                      .left = pixels(4),
                                      .bottom = pixels(4),
                                      .right = pixels(4)})
                .with_margin(Spacing::xs)
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(ai)
                .with_debug_name(label));

    // Title at top
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(80), pixels(24)})
            .with_custom_background(theme.secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_skip_tabbing(true)
            .with_debug_name(label + "_title"));

    // Three boxes of different heights to show cross-axis alignment
    const float heights[] = {70.0f, 40.0f, 55.0f};
    for (int i = 0; i < 3; i++) {
      div(context, mk(container.ent(), i + 1),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(45), pixels(heights[i])})
              .with_background(Theme::Usage::Accent)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_debug_name(label + "_box_" + std::to_string(i)));
    }
  }

  // Demo for SelfAlign - individual child overrides parent's AlignItems
  void render_self_align_demo(UIContext<InputAction> &context,
                              afterhours::Entity &parent, int id) {
    auto &theme = context.theme;

    // Container with AlignItems::Center - children can override
    auto container =
        div(context, mk(parent, id),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), pixels(80)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.92f))
                .with_padding(Padding{.top = pixels(5),
                                      .left = pixels(8),
                                      .bottom = pixels(5),
                                      .right = pixels(8)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)  // Parent default
                .with_justify_content(JustifyContent::SpaceAround)
                .with_debug_name("self_align_container"));

    // Child 1: FlexStart (overrides parent's Center) - aligns to TOP
    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_label("FlexStart")
            .with_size(ComponentSize{pixels(90), pixels(28)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_self_align(SelfAlign::FlexStart)
            .with_debug_name("self_start"));

    // Child 2: Auto (uses parent's Center) - aligns to MIDDLE
    div(context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_label("Auto")
            .with_size(ComponentSize{pixels(65), pixels(28)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_self_align(SelfAlign::Auto)
            .with_debug_name("self_auto"));

    // Child 3: Center (explicit, same as parent) - aligns to MIDDLE
    div(context, mk(container.ent(), 2),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(70), pixels(28)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_self_align(SelfAlign::Center)
            .with_debug_name("self_center"));

    // Child 4: FlexEnd (overrides parent's Center) - aligns to BOTTOM
    div(context, mk(container.ent(), 3),
        ComponentConfig{}
            .with_label("FlexEnd")
            .with_size(ComponentSize{pixels(90), pixels(28)})
            .with_custom_background(afterhours::Color{200, 70, 100, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_self_align(SelfAlign::FlexEnd)
            .with_debug_name("self_end"));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Use a clean modern theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container background (no padding so it stays in bounds)
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.90f)})
                .with_custom_background(theme.background)
                .with_roundness(0.06f)
                .with_debug_name("flex_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("flex_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Flexbox Alignment Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(45)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("title"));

    // Section 1: Vertical justify_content (Column direction)
    div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_label("justify_content - Column (vertical distribution)")
            .with_size(ComponentSize{percent(1.0f), pixels(26)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("justify_v_label"));

    // Row of vertical justify demos
    auto justify_v_row =
        div(context, mk(main_container.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(135)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("justify_v_row"));

    render_justify_vertical(context, justify_v_row.ent(), "FlexStart",
                            JustifyContent::FlexStart, 0);
    render_justify_vertical(context, justify_v_row.ent(), "FlexEnd",
                            JustifyContent::FlexEnd, 1);
    render_justify_vertical(context, justify_v_row.ent(), "Center",
                            JustifyContent::Center, 2);
    render_justify_vertical(context, justify_v_row.ent(), "SpaceBtwn",
                            JustifyContent::SpaceBetween, 3);
    render_justify_vertical(context, justify_v_row.ent(), "SpaceArnd",
                            JustifyContent::SpaceAround, 4);

    // Section 2: Horizontal justify_content (Row direction)
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("justify_content - Row (horizontal distribution)")
            .with_size(ComponentSize{percent(1.0f), pixels(26)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("justify_h_label"));

    // Row of horizontal justify demos
    auto justify_h_row =
        div(context, mk(main_container.ent(), 4),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(120)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("justify_h_row"));

    render_justify_horizontal(context, justify_h_row.ent(), "FlexStart",
                              JustifyContent::FlexStart, 0);
    render_justify_horizontal(context, justify_h_row.ent(), "FlexEnd",
                              JustifyContent::FlexEnd, 1);
    render_justify_horizontal(context, justify_h_row.ent(), "Center",
                              JustifyContent::Center, 2);
    render_justify_horizontal(context, justify_h_row.ent(), "SpaceBetween",
                              JustifyContent::SpaceBetween, 3);
    render_justify_horizontal(context, justify_h_row.ent(), "SpaceAround",
                              JustifyContent::SpaceAround, 4);

    // Section 3: align_items
    div(context, mk(main_container.ent(), 5),
        ComponentConfig{}
            .with_label("align_items (cross axis alignment)")
            .with_size(ComponentSize{percent(1.0f), pixels(26)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("align_label"));

    // Row of align_items demos
    auto align_row =
        div(context, mk(main_container.ent(), 6),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(120)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("align_row"));

    render_align_demo(context, align_row.ent(), "FlexStart",
                      AlignItems::FlexStart, 0);
    render_align_demo(context, align_row.ent(), "Center", AlignItems::Center,
                      1);
    render_align_demo(context, align_row.ent(), "FlexEnd", AlignItems::FlexEnd,
                      2);

    // Section 4: SelfAlign (individual child override)
    div(context, mk(main_container.ent(), 7),
        ComponentConfig{}
            .with_label("self_align (child overrides parent's align_items)")
            .with_size(ComponentSize{percent(1.0f), pixels(26)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("self_align_label"));

    render_self_align_demo(context, main_container.ent(), 8);

    // Footer
    div(context, mk(main_container.ent(), 9),
        ComponentConfig{}
            .with_label("with_justify_content() + with_align_items() + with_self_align()")
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(flex_alignment, "System Demos",
                        "Flexbox justify and align properties",
                        ExampleFlexAlignment)

