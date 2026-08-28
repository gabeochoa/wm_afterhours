#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScrollClickBug : ScreenSystem<UIContext<InputAction>> {
  int last_clicked = -1;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = afterhours::Color{245, 245, 245, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{70, 130, 180, 255};
    theme.secondary = afterhours::Color{220, 220, 220, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{128, 128, 128, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    auto root = vstack(
        context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.82f), screen_pct(0.85f)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Background)
            .with_border(theme.font_muted, 1.0f)
            .with_roundness(0.04f)
            .with_padding(Spacing::xl)
            .with_justify_content(JustifyContent::Center)
            .with_debug_name("scroll_click_bg"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_label("Scroll Click Bug Test")
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(28.0f))
            .with_padding(Spacing::sm));

    // Status label
    std::string status = last_clicked >= 0
                             ? fmt::format("Clicked {}", last_clicked)
                             : "No button clicked yet";
    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_label(status)
            .with_custom_text_color(theme.font_muted)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f)));

    // Scroll container - 300px tall
    auto scroll_area =
        vstack(context, mk(root.ent(), 2),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), pixels(300)})
                   .with_background(Theme::Usage::Surface)
                   .with_border(theme.font_muted, 1.0f)
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.05f)
                   .with_overflow(Overflow::Scroll, Axis::Y)
                   .with_padding(Spacing::xs)
                   .with_debug_name("scroll_click_area"));

    // 20 buttons, each 40px tall = 800px total content
    for (int i = 0; i < 20; i++) {
      auto btn = button(
          context, mk(scroll_area.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_label(fmt::format("Button {}", i))
              .with_background(Theme::Usage::Primary)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_margin(Margin{.top = pixels(2), .bottom = pixels(2)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("scroll_btn_{}", i)));
      if (btn) {
        last_clicked = i;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_click_bug, "Bug Reports",
                        "Scroll offset click test", ScrollClickBug)
