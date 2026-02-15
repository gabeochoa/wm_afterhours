#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AdaptiveScalingDemo : ScreenSystem<UIContext<InputAction>> {
  float current_scale = 1.0f;
  int scale_index = 2; // Index into scale_steps (1.0x)
  static constexpr float scale_steps[] = {0.5f, 0.75f, 1.0f, 1.25f, 1.5f,
                                          2.0f, 2.5f,  3.0f};
  static constexpr int num_steps =
      sizeof(scale_steps) / sizeof(scale_steps[0]);

  bool use_adaptive = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {

    auto theme = afterhours::ui::theme_presets::ocean_navy();
    theme.ui_scale = current_scale;
    context.theme = theme;

    if (use_adaptive) {
      context.scaling_mode = ScalingMode::Adaptive;
    } else {
      context.scaling_mode = ScalingMode::Proportional;
    }

    // Screen dimensions for manual sizing (avoids percent-inside-expand)
    float sw = static_cast<float>(Settings::get().get_screen_width());
    float sh = static_cast<float>(Settings::get().get_screen_height());

    // Colors
    const auto bg = afterhours::Color{20, 24, 36, 255};
    const auto surface = afterhours::Color{30, 36, 52, 255};
    const auto card_bg = afterhours::Color{38, 46, 66, 255};
    const auto accent = afterhours::Color{100, 160, 255, 255};
    const auto accent_dim = afterhours::Color{70, 120, 200, 255};
    const auto green = afterhours::Color{80, 200, 140, 255};
    const auto orange = afterhours::Color{240, 180, 80, 255};
    const auto red = afterhours::Color{220, 90, 90, 255};
    const auto white = afterhours::Color{240, 240, 250, 255};
    const auto muted = afterhours::Color{140, 150, 175, 255};

    // Layout constants (all in screen-space, not affected by scaling)
    // Use per-axis screen_pct to ensure uniform pixel padding on both axes
    float header_h = 48.f / sh;
    float controls_h = 52.f / sh;
    float pad_x = 16.f / sw; // 16px horizontal padding as screen_pct
    float pad_y = 16.f / sh; // 16px vertical padding as screen_pct
    float content_h = 1.0f - header_h - controls_h;
    float left_w = 280.f / sw;
    float gap_w = 16.f / sw;
    // right_col fills remaining space: total - left - gap - 2*horizontal_padding
    float right_w = 1.0f - left_w - gap_w - (2.f * pad_x);

    // LayoutInfo for breakpoint display
    auto info = LayoutInfo::make(sw, sh, current_scale,
        use_adaptive ? ScalingMode::Adaptive : ScalingMode::Proportional);

    // ── Full screen root ────────────────────────────────────────
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg)
            .with_no_wrap()
            .with_debug_name("adaptive_root"));

    // ── Header bar ──────────────────────────────────────────────
    auto header = hstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(header_h)})
            .with_custom_background(surface)
            .with_padding(Padding::horizontal(pixels(16)))
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_no_wrap()
            .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("Adaptive Scaling Demo")
            .with_size(ComponentSize{pixels(260), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_custom_text_color(white));

    std::string scale_label =
        std::format("{}  {:.0f}%", use_adaptive ? "Adaptive" : "Proportional",
                    current_scale * 100.0f);
    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label(scale_label)
            .with_size(ComponentSize{pixels(200), pixels(28)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(accent)
            .with_alignment(TextAlignment::Right));

    // ── Controls bar ────────────────────────────────────────────
    auto controls = hstack(
        context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(controls_h)})
            .with_custom_background(
                afterhours::colors::darken(surface, 0.85f))
            .with_padding(Padding{.top = pixels(6),
                                  .left = pixels(16),
                                  .bottom = pixels(6),
                                  .right = pixels(16)})
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_no_wrap()
            .with_debug_name("controls"));

    auto mode_btn = button(
        context, mk(controls.ent(), 0),
        ComponentConfig{}
            .with_label(use_adaptive ? "Mode: Adaptive" : "Mode: Proportional")
            .with_size(ComponentSize{pixels(180), pixels(36)})
            .with_custom_background(use_adaptive ? green : orange)
            .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_margin(Margin::Right(pixels(12)))
            .with_alignment(TextAlignment::Center));
    if (mode_btn) use_adaptive = !use_adaptive;

    auto zoom_out = button(
        context, mk(controls.ent(), 1),
        ComponentConfig{}
            .with_label("-")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_custom_background(accent_dim)
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_margin(Margin::Right(pixels(4)))
            .with_alignment(TextAlignment::Center));
    if (zoom_out && scale_index > 0) {
      scale_index--;
      current_scale = scale_steps[scale_index];
    }

    div(context, mk(controls.ent(), 2),
        ComponentConfig{}
            .with_label(std::format("{:.0f}%", current_scale * 100.f))
            .with_size(ComponentSize{pixels(60), pixels(36)})
            .with_custom_background(card_bg)
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f)
            .with_margin(Margin::Right(pixels(4)))
            .with_alignment(TextAlignment::Center));

    auto zoom_in = button(
        context, mk(controls.ent(), 3),
        ComponentConfig{}
            .with_label("+")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_custom_background(accent_dim)
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_margin(Margin::Right(pixels(16)))
            .with_alignment(TextAlignment::Center));
    if (zoom_in && scale_index < num_steps - 1) {
      scale_index++;
      current_scale = scale_steps[scale_index];
    }

    std::string info_text = std::format(
        "Logical: {:.0f}x{:.0f}  |  {}",
        info.logical_w, info.logical_h,
        info.is_narrow() ? "narrow" : info.is_medium() ? "medium" : "wide");
    div(context, mk(controls.ent(), 4),
        ComponentConfig{}
            .with_label(info_text)
            .with_size(ComponentSize{pixels(320), pixels(28)})
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Left));

    // ── Content area ────────────────────────────────────────────
    // Uses screen_pct for all sizing to avoid percent-inside-expand issues
    auto content = hstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(content_h)})
            .with_padding(Padding{.top = screen_pct(pad_y),
                                  .left = screen_pct(pad_x),
                                  .bottom = screen_pct(pad_y),
                                  .right = screen_pct(pad_x)})
            .with_align_items(AlignItems::FlexStart)
            .with_no_wrap()
            .with_debug_name("content"));

    // ── Left column ─────────────────────────────────────────────
    auto left_col = vstack(
        context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(left_w),
                                     screen_pct(content_h - pad_y * 2)})
            .with_margin(Margin::Right(screen_pct(gap_w)))
            .with_no_wrap()
            .with_debug_name("left_col"));

    // Card 1: Basic pixel sizing
    {
      auto card = vstack(
          context, mk(left_col.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(160)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_margin(Margin::Bottom(pixels(12)))
              .with_no_wrap()
              .with_debug_name("card_pixel"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Pixel Sizing")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("This card is 280x160 px.")
              .with_size(ComponentSize{percent(1.0f), pixels(18)})
              .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(10))));

      auto boxes = hstack(
          context, mk(card.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(48)})
              .with_align_items(AlignItems::Center)
              .with_debug_name("boxes"));

      afterhours::Color box_colors[] = {red, orange, green, accent};
      for (int i = 0; i < 4; i++) {
        div(context, mk(boxes.ent(), i),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(48), pixels(48)})
                .with_custom_background(box_colors[i])
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.15f)
                .with_margin(i < 3 ? Margin::Right(pixels(8))
                                   : Margin{}));
      }
    }

    // Card 2: Buttons
    {
      auto card = vstack(
          context, mk(left_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(140)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_margin(Margin::Bottom(pixels(12)))
              .with_no_wrap()
              .with_debug_name("card_buttons"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Pixel Buttons")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(10))));

      button(context, mk(card.ent(), 1),
             ComponentConfig{}
                 .with_label("Button A")
                 .with_size(ComponentSize{percent(1.0f), pixels(36)})
                 .with_custom_background(accent_dim)
                 .with_custom_text_color(white)
                 .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.25f)
                 .with_margin(Margin::Bottom(pixels(6)))
                 .with_alignment(TextAlignment::Center));

      button(context, mk(card.ent(), 2),
             ComponentConfig{}
                 .with_label("Button B")
                 .with_size(ComponentSize{percent(1.0f), pixels(36)})
                 .with_custom_background(green)
                 .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
                 .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.25f)
                 .with_alignment(TextAlignment::Center));
    }

    // Card 3: Padding demo
    {
      auto card = vstack(
          context, mk(left_col.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(120)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_no_wrap()
              .with_debug_name("card_padding"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Padding: 14px all sides")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("Inner content area")
              .with_size(ComponentSize{percent(1.0f), pixels(46)})
              .with_custom_background(
                  afterhours::colors::darken(card_bg, 0.7f))
              .with_custom_text_color(muted)
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.04f)
              .with_padding(Padding::all(pixels(8))));
    }

    // ── Right column ────────────────────────────────────────────
    auto right_col = vstack(
        context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(right_w),
                                     screen_pct(content_h - pad_y * 2)})
            .with_no_wrap()
            .with_debug_name("right_col"));

    // Card 4: Expand demo
    {
      auto card = vstack(
          context, mk(right_col.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(200)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_margin(Margin::Bottom(pixels(12)))
              .with_no_wrap()
              .with_debug_name("card_expand"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Expand & Flex")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("In Adaptive mode, toggle scale to see "
                          "pixel elements grow/shrink while "
                          "percent/expand reflow.")
              .with_size(ComponentSize{percent(1.0f), pixels(32)})
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(10))));

      // Three weighted expand children
      auto row = hstack(
          context, mk(card.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(88)})
              .with_no_wrap()
              .with_align_items(AlignItems::Center)
              .with_debug_name("expand_row"));

      struct ExpandItem { const char *label; float weight; afterhours::Color color; afterhours::Color text; };
      ExpandItem items[] = {
        {"1x", 1.f, red, white},
        {"2x", 2.f, orange, {30,30,40,255}},
        {"3x", 3.f, green, {20,20,30,255}},
      };
      for (int i = 0; i < 3; i++) {
        div(context, mk(row.ent(), i),
            ComponentConfig{}
                .with_label(items[i].label)
                .with_size(ComponentSize{expand(items[i].weight), percent(1.0f)})
                .with_custom_background(items[i].color)
                .with_custom_text_color(items[i].text)
                .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.08f)
                .with_alignment(TextAlignment::Center));
      }
    }

    // Card 5: screen_pct vs pixels comparison
    {
      auto card = vstack(
          context, mk(right_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(140)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_margin(Margin::Bottom(pixels(12)))
              .with_no_wrap()
              .with_debug_name("card_screenpct"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("screen_pct vs pixels")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("Only pixels() scales in Adaptive mode.")
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(8))));

      auto bars = hstack(
          context, mk(card.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(52)})
              .with_no_wrap()
              .with_align_items(AlignItems::Center)
              .with_debug_name("bars"));

      // Bar using pixels (scales in Adaptive)
      div(context, mk(bars.ent(), 0),
          ComponentConfig{}
              .with_label("px")
              .with_size(ComponentSize{pixels(80), percent(1.0f)})
              .with_custom_background(accent)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_margin(Margin::Right(pixels(6)))
              .with_alignment(TextAlignment::Center));

      // Bar using h720 (does NOT scale) with per-component override
      div(context, mk(bars.ent(), 1),
          ComponentConfig{}
              .with_label("h720")
              .with_size(ComponentSize{h720(80.0f), percent(1.0f)})
              .with_custom_background(orange)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_margin(Margin::Right(pixels(6)))
              .with_alignment(TextAlignment::Center)
              .with_scaling_mode(ScalingMode::Proportional));

      // Bar using screen_pct (does NOT scale)
      div(context, mk(bars.ent(), 2),
          ComponentConfig{}
              .with_label("scr%")
              .with_size(ComponentSize{screen_pct(80.0f / 1280.0f),
                                       percent(1.0f)})
              .with_custom_background(green)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font(UIComponent::DEFAULT_FONT, h720(11.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_alignment(TextAlignment::Center));
    }

    // Card 6: LayoutInfo breakpoints
    {
      auto card = vstack(
          context, mk(right_col.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(90)})
              .with_custom_background(card_bg)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f)
              .with_padding(Padding::all(pixels(14)))
              .with_no_wrap()
              .with_debug_name("card_breakpoints"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("LayoutInfo Breakpoints")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      auto pills = hstack(
          context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(26)})
              .with_align_items(AlignItems::Center)
              .with_no_wrap()
              .with_debug_name("breakpoint_pills"));

      auto pill = [&](int id, const char *label, bool active) {
        div(context, mk(pills.ent(), id),
            ComponentConfig{}
                .with_label(label)
                .with_size(ComponentSize{pixels(80), pixels(26)})
                .with_custom_background(active ? accent : surface)
                .with_custom_text_color(active ? afterhours::Color{20, 20, 30, 255} : muted)
                .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.4f)
                .with_margin(Margin::Right(pixels(6)))
                .with_alignment(TextAlignment::Center));
      };

      pill(0, "narrow", info.is_narrow());
      pill(1, "medium", info.is_medium());
      pill(2, "wide", info.is_wide());
      pill(3, "short", info.is_short());
    }
  }
};

REGISTER_EXAMPLE_SCREEN(adaptive_scaling, "Tools",
                        "Interactive demo of Adaptive vs Proportional scaling",
                        AdaptiveScalingDemo)
