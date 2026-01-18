#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScrollViewShowcase : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Light theme for visibility
    Theme theme;
    theme.background = afterhours::Color{245, 245, 245, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{70, 130, 180, 255};  // Steel blue
    theme.secondary = afterhours::Color{220, 220, 220, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{128, 128, 128, 255};
    context.theme = theme;

    // Title
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_label("Scroll View Demo")
            .with_custom_text_color(theme.font)
            .with_debug_name("title"));

    // Instructions
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(300), pixels(20)})
            .with_label("Use mouse wheel to scroll")
            .with_custom_text_color(theme.font_muted)
            .with_debug_name("instructions"));

    // Create a scroll_view with more content than fits in the viewport
    auto scroll = scroll_view(
        context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(300), pixels(200)})
            .with_custom_background(theme.surface)
            .with_border(theme.font_muted, 1.0f)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.05f)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("scroll_container"));

    // Show scroll position info
    if (scroll.ent().has<afterhours::ui::HasScrollView>()) {
      auto &sv = scroll.ent().get<afterhours::ui::HasScrollView>();
      float max_scroll = std::max(0.0f, sv.content_size.y - sv.viewport_size.y);
      div(context, mk(entity, 100),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(300), pixels(28)})
              .with_label(fmt::format("Scroll: {:.0f} / {:.0f}", sv.scroll_offset.y, max_scroll))
              .with_font(UIComponent::DEFAULT_FONT, 24.f)
              .with_custom_text_color(theme.font_muted)
              .with_debug_name("scroll_info"));
    }

    // Add many items that exceed the container height
    // 20 items at ~44px each = ~880px total, but viewport is only 200px
    for (int i = 0; i < 20; i++) {
      div(context, mk(scroll.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_label(fmt::format("Item {}", i + 1))
              .with_background(Theme::Usage::Primary)
              .with_margin(Margin{.top = pixels(2), .bottom = pixels(2)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("item_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_view, "Component Galleries",
                        "Scrollable container demo", ScrollViewShowcase)

