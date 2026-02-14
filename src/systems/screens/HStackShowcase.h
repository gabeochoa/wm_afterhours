#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the hstack() primitive.
// Default: percent(1.0) width, children() height -- fills parent, shrinks to content.
struct HStackShowcase : ScreenSystem<UIContext<InputAction>> {

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Bright, high-contrast colors for demo boxes
    const auto teal = afterhours::Color{0, 180, 220, 255};
    const auto pink = afterhours::Color{220, 60, 120, 255};
    const auto green = afterhours::Color{50, 210, 130, 255};
    const auto orange = afterhours::Color{250, 140, 40, 255};
    const auto purple = afterhours::Color{160, 100, 255, 255};
    const auto blue = afterhours::Color{70, 140, 255, 255};
    const auto panel_bg = afterhours::Color{20, 26, 40, 255};

    // Full-screen root
    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_padding(Padding{.top = pixels(8), .left = pixels(16),
                                  .bottom = pixels(8), .right = pixels(16)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("hstack() Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(teal)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_debug_name("title"));

    // == Row 1: Basic hstack with 3 large boxes ==
    // Note: hstack auto-fills parent width; only height needs specifying
    auto row1 = hstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(80)})  // override height
            .with_custom_background(panel_bg)
            .with_padding(Spacing::sm)
            .with_margin(Margin{.top = pixels(6)})
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_debug_name("basic_row"));

    afterhours::Color row1_colors[] = {teal, pink, green};
    const char *row1_labels[] = {"Item 1", "Item 2", "Item 3"};
    for (int i = 0; i < 3; i++) {
      div(context, mk(row1.ent(), i),
          ComponentConfig{}
              .with_label(row1_labels[i])
              .with_size(ComponentSize{percent(0.30f), percent(0.85f)})
              .with_custom_background(row1_colors[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("box_{}", i)));
    }

    // == Row 2: Five JustifyContent demos side-by-side ==
    auto row2 = hstack(
        context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(160)})
            .with_margin(Margin{.top = pixels(6)})
            .with_debug_name("justify_row"));

    struct JDemo { const char *label; JustifyContent jc; };
    JDemo justifies[] = {
        {"Start", JustifyContent::FlexStart},
        {"End", JustifyContent::FlexEnd},
        {"Center", JustifyContent::Center},
        {"Between", JustifyContent::SpaceBetween},
        {"Around", JustifyContent::SpaceAround},
    };

    for (int j = 0; j < 5; j++) {
      auto col = div(
          context, mk(row2.ent(), j),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.19f), percent(1.0f)})
              .with_custom_background(panel_bg)
              .with_padding(Padding{.top = pixels(4), .left = pixels(4),
                                    .bottom = pixels(4), .right = pixels(4)})
              .with_margin(Margin{.left = pixels(2), .right = pixels(2)})
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name(fmt::format("jc_{}", justifies[j].label)));

      // Label at top
      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(justifies[j].label)
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_custom_background(pink)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
              .with_skip_tabbing(true));

      // hstack container showing this justify mode
      auto jr = hstack(
          context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), expand()})
              .with_custom_background(afterhours::Color{10, 14, 25, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.top = pixels(4)})
              .with_justify_content(justifies[j].jc)
              .with_align_items(AlignItems::Center)
              .with_debug_name(fmt::format("jc_row_{}", j)));

      for (int k = 0; k < 3; k++) {
        div(context, mk(jr.ent(), k),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(30), pixels(30)})
                .with_custom_background(teal)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.15f)
                .with_debug_name(fmt::format("jc_box_{}_{}", j, k)));
      }
    }

    // == Row 3: AlignItems demos + real-world toolbar ==
    auto row3 = hstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(160)})
            .with_margin(Margin{.top = pixels(6)})
            .with_debug_name("align_and_patterns"));

    // LEFT: 3 AlignItems demos
    afterhours::Color align_colors[] = {green, orange, purple};
    struct ADemo { const char *label; AlignItems ai; };
    ADemo aligns[] = {
        {"Start", AlignItems::FlexStart},
        {"Center", AlignItems::Center},
        {"End", AlignItems::FlexEnd},
    };
    float heights[] = {0.85f, 0.50f, 0.65f};

    for (int a = 0; a < 3; a++) {
      auto col = div(
          context, mk(row3.ent(), a),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.19f), percent(1.0f)})
              .with_custom_background(panel_bg)
              .with_padding(Padding{.top = pixels(4), .left = pixels(4),
                                    .bottom = pixels(4), .right = pixels(4)})
              .with_margin(Margin{.left = pixels(2), .right = pixels(2)})
              .with_flex_direction(FlexDirection::Column)
              .with_debug_name(fmt::format("ai_{}", aligns[a].label)));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(aligns[a].label)
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_custom_background(green)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
              .with_skip_tabbing(true));

      auto ar = hstack(
          context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), expand()})
              .with_custom_background(afterhours::Color{10, 14, 25, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.top = pixels(4)})
              .with_align_items(aligns[a].ai)
              .with_justify_content(JustifyContent::SpaceAround)
              .with_debug_name(fmt::format("ai_row_{}", a)));

      for (int k = 0; k < 3; k++) {
        div(context, mk(ar.ent(), k),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(28), percent(heights[k])})
                .with_custom_background(align_colors[k])
                .with_debug_name(fmt::format("ai_box_{}_{}", a, k)));
      }
    }

    // RIGHT: Real-world toolbar pattern
    auto patterns = div(
        context, mk(row3.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.39f), percent(1.0f)})
            .with_custom_background(panel_bg)
            .with_padding(Spacing::sm)
            .with_margin(Margin{.left = pixels(4)})
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("patterns"));

    div(context, mk(patterns.ent(), 0),
        ComponentConfig{}
            .with_label("Toolbar Pattern")
            .with_size(ComponentSize{percent(1.0f), pixels(22)})
            .with_custom_background(blue)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_skip_tabbing(true));

    auto toolbar = hstack(
        context, mk(patterns.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(afterhours::Color{30, 38, 55, 255})
            .with_padding(Padding{.left = pixels(6), .right = pixels(6)})
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_margin(Margin{.top = pixels(4)})
            .with_debug_name("toolbar"));

    // Nested hstack: shrinks to content since parent already constrains width
    auto tb_left = hstack(context, mk(toolbar.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{children(), children()})
            .with_align_items(AlignItems::Center));
    for (int i = 0; i < 3; i++) {
      const char *t[] = {"File", "Edit", "View"};
      button(context, mk(tb_left.ent(), i),
          ComponentConfig{}
              .with_label(t[i])
              .with_size(ComponentSize{pixels(50), pixels(30)})
              .with_button_variant(ButtonVariant::Ghost)
              .with_font(UIComponent::DEFAULT_FONT, h720(13.0f)));
    }
    button(context, mk(toolbar.ent(), 1),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(70), pixels(30)})
            .with_button_variant(ButtonVariant::Outline)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f)));

    // Card row
    div(context, mk(patterns.ent(), 2),
        ComponentConfig{}
            .with_label("Card Row")
            .with_size(ComponentSize{percent(1.0f), pixels(22)})
            .with_custom_background(blue)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_margin(Margin{.top = pixels(6)})
            .with_skip_tabbing(true));

    auto cards = hstack(
        context, mk(patterns.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_margin(Margin{.top = pixels(4)})
            .with_debug_name("cards"));

    afterhours::Color card_c[] = {teal, green, orange, purple};
    const char *card_l[] = {"A", "B", "C", "D"};
    for (int i = 0; i < 4; i++) {
      div(context, mk(cards.ent(), i),
          ComponentConfig{}
              .with_label(card_l[i])
              .with_size(ComponentSize{percent(0.22f), percent(0.85f)})
              .with_custom_background(card_c[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.12f));
    }

    // == Row 4: Dashboard cards spanning full width ==
    auto row4 = hstack(
        context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_align_items(AlignItems::Center)
            .with_margin(Margin{.top = pixels(6)})
            .with_debug_name("dashboard"));

    afterhours::Color dash_c[] = {blue, pink, green, orange, purple, teal};
    const char *dash_l[] = {"Users", "Revenue", "Orders", "Growth", "Active", "Churn"};
    for (int i = 0; i < 6; i++) {
      div(context, mk(row4.ent(), i),
          ComponentConfig{}
              .with_label(dash_l[i])
              .with_size(ComponentSize{percent(0.15f), percent(0.80f)})
              .with_custom_background(dash_c[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.10f));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(hstack_showcase, "Component Galleries",
                        "hstack() primitive showcase",
                        HStackShowcase)
