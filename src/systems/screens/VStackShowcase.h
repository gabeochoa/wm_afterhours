#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the vstack() primitive.
// Default: percent(1.0) width, children() height -- fills parent, shrinks to content.
struct VStackShowcase : ScreenSystem<UIContext<InputAction>> {

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    const auto teal = afterhours::Color{0, 180, 220, 255};
    const auto pink = afterhours::Color{220, 60, 120, 255};
    const auto green = afterhours::Color{50, 210, 130, 255};
    const auto orange = afterhours::Color{250, 140, 40, 255};
    const auto purple = afterhours::Color{160, 100, 255, 255};
    const auto blue = afterhours::Color{70, 140, 255, 255};
    const auto panel_bg = afterhours::Color{30, 38, 58, 255};
    const auto dark_bg = afterhours::Color{14, 18, 30, 255};

    // Full-screen root column
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_padding(Padding{.top = pixels(6), .left = pixels(10),
                                  .bottom = pixels(6), .right = pixels(10)})
            .with_debug_name("root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("vstack() Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(34)})
            .with_custom_background(teal)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f)));

    // =================================================================
    // TOP SECTION: Five JustifyContent demos
    // =================================================================
    auto top_section = hstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.42f)})
            .with_margin(Margin{.top = pixels(4)})
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_debug_name("jc_section"));

    struct JDemo { const char *label; JustifyContent jc; };
    JDemo justifies[] = {
        {"Start", JustifyContent::FlexStart},
        {"Center", JustifyContent::Center},
        {"End", JustifyContent::FlexEnd},
        {"Between", JustifyContent::SpaceBetween},
        {"Around", JustifyContent::SpaceAround},
    };
    afterhours::Color jc_colors[] = {teal, pink, green};

    for (int j = 0; j < 5; j++) {
      auto col = vstack(
          context, mk(top_section.ent(), j),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.19f), percent(1.0f)})
              .with_custom_background(panel_bg)
              .with_padding(Spacing::xs));

      // Label
      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(justifies[j].label)
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_custom_background(pink)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
              .with_skip_tabbing(true));

      // vstack demo showing this justify mode
      auto demo = vstack(
          context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.88f)})
              .with_custom_background(afterhours::Color{40, 48, 70, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.top = pixels(3)})
              .with_justify_content(justifies[j].jc)
              .with_align_items(AlignItems::Center));

      // 3 boxes using percent height -- clearly visible in each mode
      float box_h[] = {0.24f, 0.18f, 0.22f};
      for (int k = 0; k < 3; k++) {
        div(context, mk(demo.ent(), k),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.80f), percent(box_h[k])})
                .with_custom_background(jc_colors[k])
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.10f));
      }
    }

    // =================================================================
    // BOTTOM SECTION: AlignItems + sidebar pattern
    // =================================================================
    auto bot_section = hstack(
        context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.50f)})
            .with_margin(Margin{.top = pixels(4)})
            .with_debug_name("bottom"));

    // -- LEFT: AlignItems demos --
    auto ai_panel = vstack(
        context, mk(bot_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.54f), percent(1.0f)})
            .with_margin(Margin{.right = pixels(4)}));

    div(context, mk(ai_panel.ent(), 0),
        ComponentConfig{}
            .with_label("AlignItems (cross-axis)")
            .with_size(ComponentSize{percent(1.0f), pixels(20)})
            .with_custom_background(green)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
            .with_skip_tabbing(true));

    auto ai_row = hstack(
        context, mk(ai_panel.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.90f)})
            .with_margin(Margin{.top = pixels(3)})
            .with_justify_content(JustifyContent::SpaceBetween));

    struct ADemo { const char *label; AlignItems ai; };
    ADemo aligns[] = {
        {"Start", AlignItems::FlexStart},
        {"Center", AlignItems::Center},
        {"Stretch", AlignItems::Stretch},
        {"End", AlignItems::FlexEnd},
    };
    afterhours::Color ai_colors[] = {orange, purple, blue, teal};

    for (int a = 0; a < 4; a++) {
      auto col = vstack(
          context, mk(ai_row.ent(), a),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.24f), percent(1.0f)})
              .with_custom_background(afterhours::Color{35, 42, 62, 255})
              .with_padding(Spacing::xs));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(aligns[a].label)
              .with_size(ComponentSize{percent(1.0f), pixels(18)})
              .with_custom_background(green)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
              .with_skip_tabbing(true));

      auto demo = vstack(
          context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
              .with_custom_background(afterhours::Color{40, 48, 70, 255})
              .with_padding(Spacing::xs)
              .with_margin(Margin{.top = pixels(2)})
              .with_justify_content(JustifyContent::SpaceAround)
              .with_align_items(aligns[a].ai));

      float widths[] = {0.85f, 0.50f, 0.70f};
      for (int k = 0; k < 3; k++) {
        div(context, mk(demo.ent(), k),
            ComponentConfig{}
                .with_size(ComponentSize{percent(widths[k]), percent(0.24f)})
                .with_custom_background(ai_colors[a])
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.08f));
      }
    }

    // -- RIGHT: Sidebar + content --
    auto right = vstack(
        context, mk(bot_section.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.44f), percent(1.0f)}));

    div(context, mk(right.ent(), 0),
        ComponentConfig{}
            .with_label("Sidebar + Content")
            .with_size(ComponentSize{percent(1.0f), pixels(20)})
            .with_custom_background(blue)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
            .with_skip_tabbing(true));

    auto layout = hstack(
        context, mk(right.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.90f)})
            .with_margin(Margin{.top = pixels(3)}));

    // Sidebar nav
    auto sidebar = vstack(
        context, mk(layout.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.38f), percent(1.0f)})
            .with_custom_background(afterhours::Color{35, 42, 62, 255})
            .with_padding(Spacing::xs));

    const char *nav[] = {"Home", "Settings", "Profile", "Help"};
    afterhours::Color nav_c[] = {teal, pink, green, orange};
    for (int i = 0; i < 4; i++) {
      button(context, mk(sidebar.ent(), i),
             ComponentConfig{}
                 .with_label(nav[i])
                 .with_size(ComponentSize{percent(1.0f), pixels(28)})
                 .with_custom_background(nav_c[i])
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, pixels(11.0f))
                 .with_margin(Margin{.bottom = pixels(3)})
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.06f));
    }

    // Content area
    auto content = vstack(
        context, mk(layout.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.60f), percent(1.0f)})
            .with_custom_background(afterhours::Color{40, 48, 70, 255})
            .with_padding(Spacing::xs)
            .with_margin(Margin{.left = pixels(3)}));

    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("Content")
            .with_size(ComponentSize{percent(1.0f), pixels(22)})
            .with_custom_background(purple)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f)));

    const char *cards[] = {"Alerts", "Activity", "Stats"};
    afterhours::Color card_c[] = {teal, green, orange};
    for (int i = 0; i < 3; i++) {
      div(context, mk(content.ent(), i + 1),
          ComponentConfig{}
              .with_label(cards[i])
              .with_size(ComponentSize{percent(1.0f), pixels(26)})
              .with_custom_background(card_c[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(11.0f))
              .with_margin(Margin{.top = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(vstack_showcase, "Component Galleries",
                        "vstack() primitive showcase",
                        VStackShowcase)
