#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
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
    theme.primary = afterhours::Color{70, 130, 180, 255}; // Steel blue
    theme.secondary = afterhours::Color{220, 220, 220, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{128, 128, 128, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Main container - centered on screen
    auto root = vstack(
        context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.85f), screen_pct(0.88f)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Background)
            .with_border(theme.font_muted, 1.0f)
            .with_roundness(0.04f)
            .with_padding(Spacing::xl)
            .with_justify_content(JustifyContent::Center)
            .with_debug_name("scroll_bg"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_label("Scroll View Demo")
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(28.0f))
            .with_padding(Spacing::sm));

    // Instructions
    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_label("Scroll: mouse wheel | Horizontal: Shift + scroll "
                        "wheel or trackpad swipe")
            .with_custom_text_color(theme.font_muted)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f)));

    // Invert scroll toggle row with label
    auto toggle_row =
        hstack(context, mk(root.ent(), 2),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), pixels(36)})
                   .with_align_items(AlignItems::Center)
                   .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    checkbox(context, mk(toggle_row.ent(), 0), invert_scroll,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(200), pixels(28)})
                 .with_label("Invert Scroll")
                 .with_background(Theme::Usage::Secondary)
                 .with_custom_text_color(theme.font)
                 .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f)));

    // Container for both scroll views side by side
    auto container =
        hstack(context, mk(root.ent(), 3),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.65f)})
                   .with_justify_content(JustifyContent::SpaceAround));

    // === VERTICAL SCROLL VIEW ===
    auto vert_section =
        vstack(context, mk(container.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.45f), percent(1.0f)})
                   .with_debug_name("vert_section"));

    div(context, mk(vert_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_label("Vertical Scroll")
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f)));

    // Vertical scroll container
    auto vert_scroll =
        vstack(context, mk(vert_section.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.70f)})
                   .with_background(Theme::Usage::Surface)
                   .with_border(theme.font_muted, 1.0f)
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.05f)
                   .with_overflow(Overflow::Scroll, Axis::Y)
                   .with_padding(Spacing::xs)
                   .with_debug_name("vert_scroll"));

    // Configure scroll runtime settings
    if (vert_scroll.ent().has<HasScrollView>()) {
      auto &sv = vert_scroll.ent().get<HasScrollView>();
      sv.invert_scroll = invert_scroll;

      float max_scroll = std::max(0.0f, sv.content_size.y - sv.viewport_or_zero().y);
      int scroll_pct =
          max_scroll > 0.0f
              ? static_cast<int>((sv.scroll_offset.y / max_scroll) * 100.0f)
              : 0;
      int total_items = 20;
      int visible_item =
          max_scroll > 0.0f
              ? 1 + static_cast<int>((sv.scroll_offset.y / max_scroll) *
                                     (total_items - 1))
              : 1;
      div(context, mk(vert_section.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_label(fmt::format("Item {} of {} ({}%)", visible_item,
                                      total_items, scroll_pct))
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_custom_text_color(theme.font_muted));
    }

    // Vertical scroll items - increased text size for touch targets
    for (int i = 0; i < 20; i++) {
      div(context, mk(vert_scroll.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.95f), pixels(44)})
              .with_label(fmt::format("Item {}", i + 1))
              .with_background(Theme::Usage::Primary)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_margin(Margin{.top = pixels(3), .bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("vert_item_{}", i)));
    }

    // === HORIZONTAL SCROLL VIEW ===
    auto horiz_section = vstack(context, mk(container.ent(), 1),
                                ComponentConfig{}.with_size(ComponentSize{
                                    percent(0.45f), percent(1.0f)}));

    div(context, mk(horiz_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_label("Horizontal Scroll")
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f)));

    // Horizontal scroll container
    auto horiz_scroll =
        hstack(context, mk(horiz_section.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.50f)})
                   .with_background(Theme::Usage::Surface)
                   .with_border(theme.font_muted, 1.0f)
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.05f)
                   .with_no_wrap()
                   .with_overflow(Overflow::Scroll, Axis::X)
                   .with_padding(Spacing::xs)
                   .with_debug_name("horiz_scroll"));

    // Configure scroll runtime settings
    if (horiz_scroll.ent().has<HasScrollView>()) {
      auto &sv = horiz_scroll.ent().get<HasScrollView>();
      sv.invert_scroll = invert_scroll;

      float max_scroll = std::max(0.0f, sv.content_size.x - sv.viewport_or_zero().x);
      int scroll_pct =
          max_scroll > 0.0f
              ? static_cast<int>((sv.scroll_offset.x / max_scroll) * 100.0f)
              : 0;
      int total_items = 15;
      int visible_item =
          max_scroll > 0.0f
              ? 1 + static_cast<int>((sv.scroll_offset.x / max_scroll) *
                                     (total_items - 1))
              : 1;
      div(context, mk(horiz_section.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_label(fmt::format("Item {} of {} ({}%)", visible_item,
                                      total_items, scroll_pct))
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_custom_text_color(theme.font_muted));

      // Horizontal scroll indicator
      float track_width = 150.0f;
      float thumb_ratio =
          sv.viewport_or_zero().x / std::max(sv.content_size.x, sv.viewport_or_zero().x);
      float thumb_width = std::max(20.0f, track_width * thumb_ratio);
      float scroll_ratio =
          max_scroll > 0.0f ? sv.scroll_offset.x / max_scroll : 0.0f;
      float thumb_offset = scroll_ratio * (track_width - thumb_width);

      // Indicator track (dark background) - increased visibility
      auto horiz_track =
          div(context, mk(horiz_section.ent(), 3),
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(track_width), pixels(10)})
                  .with_custom_background(afterhours::Color{80, 80, 80, 200})
                  .with_rounded_corners(RoundedCorners().all_round())
                  .with_roundness(0.5f)
                  .with_self_align(SelfAlign::Center)
                  .with_margin(Margin{.top = pixels(8)})
                  .with_debug_name("horiz_indicator_track"));

      // Indicator thumb (bright, visible) - increased visibility
      div(context, mk(horiz_track.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(thumb_width), pixels(10)})
              .with_custom_background(afterhours::Color{0, 120, 215, 255})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.5f)
              .with_absolute_position(pixels(thumb_offset), pixels(0))
              .with_debug_name("horiz_indicator_thumb"));
    }

    // Horizontal scroll items - sized so item 5 is clearly partially visible
    // to indicate scrollability (4 full items + ~50% of item 5 visible)
    for (int i = 0; i < 15; i++) {
      div(context, mk(horiz_scroll.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(70), pixels(70)})
              .with_label(fmt::format("{}", i + 1))
              .with_background(Theme::Usage::Accent)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_margin(Margin{.left = pixels(4), .right = pixels(4)})
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.15f)
              .with_debug_name(fmt::format("horiz_item_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_view, "Component Galleries",
                        "Scrollable container demo", ScrollViewShowcase)
