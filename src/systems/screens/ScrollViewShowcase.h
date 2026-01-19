#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScrollViewShowcase : ScreenSystem<UIContext<InputAction>> {
  // Persistent state for the invert scroll toggle
  bool invert_scroll = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Light theme for visibility
    Theme theme;
    theme.background = afterhours::Color{245, 245, 245, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{70, 130, 180, 255};   // Steel blue
    theme.secondary = afterhours::Color{220, 220, 220, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{128, 128, 128, 255};
    context.theme = theme;

    // Main container - centered on screen
    auto root =
        div(context, mk(entity, 100),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.70f), screen_pct(0.75f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_border(theme.font_muted, 1.0f)
                .with_roundness(0.04f)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("scroll_bg"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_label("Scroll View Demo")
            .with_custom_background(theme.surface)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_padding(Spacing::sm)
            .with_debug_name("title"));

    // Instructions
    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_label("Use mouse wheel to scroll (trackpad for horizontal)")
            .with_custom_text_color(theme.font_muted)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("instructions"));

    // Invert scroll toggle
    checkbox(context, mk(root.ent(), 2), invert_scroll,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(280), pixels(52)})
                 .with_label("Invert scroll")
                 .with_margin(Margin{.bottom = DefaultSpacing::small()})
                 .with_debug_name("invert_toggle"));

    // Container for both scroll views side by side
    auto container = div(context, mk(root.ent(), 3),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.0f), percent(0.65f)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_justify_content(JustifyContent::SpaceAround)
                             .with_debug_name("container"));

    // === VERTICAL SCROLL VIEW ===
    auto vert_section = div(context, mk(container.ent(), 0),
                            ComponentConfig{}
                                .with_size(ComponentSize{percent(0.45f), percent(1.0f)})
                                .with_flex_direction(FlexDirection::Column)
                                .with_debug_name("vert_section"));

    div(context, mk(vert_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_label("Vertical Scroll")
            .with_custom_text_color(theme.font)
            .with_debug_name("vert_label"));

    auto vert_scroll = scroll_view(
        context, mk(vert_section.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.80f)})
            .with_custom_background(theme.surface)
            .with_border(theme.font_muted, 1.0f)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.05f)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("vert_scroll"));

    // Configure vertical scrolling
    if (vert_scroll.ent().has<HasScrollView>()) {
      auto &sv = vert_scroll.ent().get<HasScrollView>();
      sv.vertical_enabled = true;
      sv.horizontal_enabled = false;
      sv.invert_scroll = invert_scroll;

      float max_scroll = std::max(0.0f, sv.content_size.y - sv.viewport_size.y);
      div(context, mk(vert_section.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_label(fmt::format("Y: {:.0f} / {:.0f}", sv.scroll_offset.y,
                                      max_scroll))
              .with_font(UIComponent::DEFAULT_FONT, 16.f)
              .with_custom_text_color(theme.font_muted)
              .with_debug_name("vert_info"));
    }

    // Vertical scroll items
    for (int i = 0; i < 20; i++) {
      div(context, mk(vert_scroll.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_label(fmt::format("Item {}", i + 1))
              .with_background(Theme::Usage::Primary)
              .with_margin(Margin{.top = pixels(2), .bottom = pixels(2)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("vert_item_{}", i)));
    }

    // === HORIZONTAL SCROLL VIEW ===
    auto horiz_section = div(context, mk(container.ent(), 1),
                             ComponentConfig{}
                                 .with_size(ComponentSize{percent(0.45f), percent(1.0f)})
                                 .with_flex_direction(FlexDirection::Column)
                                 .with_debug_name("horiz_section"));

    div(context, mk(horiz_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_label("Horizontal Scroll")
            .with_custom_text_color(theme.font)
            .with_debug_name("horiz_label"));

    auto horiz_scroll = scroll_view(
        context, mk(horiz_section.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.60f)})
            .with_custom_background(theme.surface)
            .with_border(theme.font_muted, 1.0f)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.05f)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("horiz_scroll"));

    // Configure horizontal scrolling
    if (horiz_scroll.ent().has<HasScrollView>()) {
      auto &sv = horiz_scroll.ent().get<HasScrollView>();
      sv.vertical_enabled = false;
      sv.horizontal_enabled = true;
      sv.invert_scroll = invert_scroll;

      float max_scroll = std::max(0.0f, sv.content_size.x - sv.viewport_size.x);
      div(context, mk(horiz_section.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_label(fmt::format("X: {:.0f} / {:.0f}", sv.scroll_offset.x,
                                      max_scroll))
              .with_font(UIComponent::DEFAULT_FONT, 16.f)
              .with_custom_text_color(theme.font_muted)
              .with_debug_name("horiz_info"));
    }

    // Horizontal scroll items
    for (int i = 0; i < 15; i++) {
      div(context, mk(horiz_scroll.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(80), pixels(80)})
              .with_label(fmt::format("{}", i + 1))
              .with_background(Theme::Usage::Accent)
              .with_margin(Margin{.left = pixels(4), .right = pixels(4)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("horiz_item_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_view, "Component Galleries",
                        "Scrollable container demo", ScrollViewShowcase)
