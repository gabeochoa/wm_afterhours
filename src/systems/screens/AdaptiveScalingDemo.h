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
  static constexpr float scale_steps[] = {0.5f, 0.75f, 1.0f, 1.25f,
                                          1.5f, 2.0f,  2.5f, 3.0f};
  static constexpr int num_steps = sizeof(scale_steps) / sizeof(scale_steps[0]);

  bool use_adaptive = true;
  std::string selected_button = "Size";

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

    const float pixel_scale = use_adaptive ? current_scale : 1.f;
    auto caption = [&](afterhours::Entity &parent, int id,
                       const std::string &label, float height = 24.f) {
      return div(
          context, mk(parent, id),
          ComponentConfig{}
              .with_label(label)
              .with_size({percent(1.f), pixels(height)})
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_custom_text_color(muted));
    };

    // The viewport stays fixed while the scrollable canvas follows pixel scale.
    // Use per-axis screen_pct to ensure uniform pixel padding on both axes
    float header_h = 48.f / sh;
    float controls_h = 82.f / sh;
    float pad_x =
        16.f * pixel_scale / sw; // 16 logical pixels of horizontal padding
    float pad_y =
        16.f * pixel_scale / sh; // 16 logical pixels of vertical padding
    float content_h = 1.0f - header_h - controls_h;
    const float canvas_w = std::max(sw, 1280.f * pixel_scale);
    const float canvas_h = std::max(sh - 130.f, 590.f * pixel_scale);
    float left_w = 280.f * pixel_scale / sw;
    float gap_w = 16.f * pixel_scale / sw;
    // right_col fills remaining space: total - left - gap -
    // 2*horizontal_padding
    float right_w = canvas_w / sw - left_w - gap_w - (2.f * pad_x);

    // LayoutInfo for breakpoint display
    auto info = LayoutInfo::make(sw, sh, current_scale,
                                 use_adaptive ? ScalingMode::Adaptive
                                              : ScalingMode::Proportional);

    context.scaling_mode = ScalingMode::Proportional;

    // ── Full screen root ────────────────────────────────────────
    auto root =
        vstack(context, mk(entity, 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg)
                   .with_corner_radius(0.f)
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
            .with_font("AtkinsonMock", screen_pct(28.f / sh))
            .with_custom_text_color(white));

    std::string scale_label =
        std::format("Viewport: {:.0f} x {:.0f} px", sw, sh);
    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label(scale_label)
            .with_size(ComponentSize{pixels(280), pixels(28)})
            .with_font("AtkinsonMock", screen_pct(22.f / sh))
            .with_custom_text_color(accent)
            .with_alignment(TextAlignment::Right));

    // ── Controls bar ────────────────────────────────────────────
    auto controls = hstack(
        context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(52.f / sh)})
            .with_custom_background(afterhours::colors::darken(surface, 0.85f))
            .with_padding(Padding{.top = pixels(6),
                                  .left = pixels(16),
                                  .bottom = pixels(6),
                                  .right = pixels(16)})
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_no_wrap()
            .with_debug_name("controls"));

    auto mode_btn =
        button(context, mk(controls.ent(), 0),
               ComponentConfig{}
                   .with_label(use_adaptive ? "Adaptive >" : "Proportional >")
                   .with_size(ComponentSize{pixels(200), pixels(36)})
                   .with_custom_background(use_adaptive ? green : orange)
                   .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
                   .with_font("AtkinsonMock", screen_pct(22.f / sh))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.3f)
                   .with_margin(Margin::Right(pixels(12)))
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("adaptive_mode"));
    if (mode_btn)
      use_adaptive = !use_adaptive;

    auto zoom_out = button(context, mk(controls.ent(), 1),
                           ComponentConfig{}
                               .with_label("-")
                               .with_size(ComponentSize{pixels(36), pixels(36)})
                               .with_custom_background(accent_dim)
                               .with_custom_text_color(white)
                               .with_font("AtkinsonMock", screen_pct(24.f / sh))
                               .with_rounded_corners(RoundedCorners())
                               .with_roundness(0.3f)
                               .with_margin(Margin::Right(pixels(4)))
                               .with_alignment(TextAlignment::Center)
                               .with_debug_name("adaptive_minus"));
    if (zoom_out && scale_index > 0) {
      scale_index--;
      current_scale = scale_steps[scale_index];
    }

    div(context, mk(controls.ent(), 2),
        ComponentConfig{}
            .with_label(std::format("{:.0f}%", current_scale * 100.f))
            .with_size(ComponentSize{pixels(60), pixels(36)})
            .with_border(muted, 1.f)
            .with_custom_text_color(white)
            .with_font("AtkinsonMock", screen_pct(22.f / sh))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f)
            .with_margin(Margin::Right(pixels(4)))
            .with_alignment(TextAlignment::Center));

    auto zoom_in = button(context, mk(controls.ent(), 3),
                          ComponentConfig{}
                              .with_label("+")
                              .with_size(ComponentSize{pixels(36), pixels(36)})
                              .with_custom_background(accent_dim)
                              .with_custom_text_color(white)
                              .with_font("AtkinsonMock", screen_pct(24.f / sh))
                              .with_rounded_corners(RoundedCorners())
                              .with_roundness(0.3f)
                              .with_margin(Margin::Right(pixels(16)))
                              .with_alignment(TextAlignment::Center)
                              .with_debug_name("adaptive_plus"));
    if (zoom_in && scale_index < num_steps - 1) {
      scale_index++;
      current_scale = scale_steps[scale_index];
    }

    std::string info_text = std::format("Logical: {:.0f} x {:.0f} | {}",
                                        info.logical_w, info.logical_h,
                                        info.is_narrow()   ? "narrow"
                                        : info.is_medium() ? "medium"
                                                           : "wide");
    div(context, mk(controls.ent(), 4),
        ComponentConfig{}
            .with_label(info_text)
            .with_size(ComponentSize{pixels(500), pixels(28)})
            .with_font("AtkinsonMock", screen_pct(20.f / sh))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Left));

    div(context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_label(std::format("Scale steps: 50, 75, 100, 125, 150, 200, "
                                    "250, 300%.  Mode button switches to {}.",
                                    use_adaptive ? "Proportional" : "Adaptive"))
            .with_size({screen_pct(1.f), screen_pct(30.f / sh)})
            .with_font("AtkinsonMock", screen_pct(20.f / sh))
            .with_custom_text_color(muted));

    // ── Content area ────────────────────────────────────────────
    // Uses screen_pct for all sizing to avoid percent-inside-expand issues
    auto viewport =
        vstack(context, mk(root.ent(), 3),
               ComponentConfig{}
                   .with_size({screen_pct(1.f), screen_pct(content_h)})
                   .with_overflow(canvas_w > sw || canvas_h > sh - 130.f
                                      ? Overflow::Auto
                                      : Overflow::Hidden)
                   .with_debug_name("adaptive_viewport"));
    if (canvas_w <= sw && canvas_h <= sh - 130.f)
      viewport.ent().removeComponentIfExists<HasScrollView>();
    context.scaling_mode =
        use_adaptive ? ScalingMode::Adaptive : ScalingMode::Proportional;
    auto content =
        hstack(context, mk(viewport.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(canvas_w / sw),
                                            screen_pct(canvas_h / sh)})
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
                                     screen_pct(canvas_h / sh - pad_y * 2)})
            .with_margin(Margin::Right(screen_pct(gap_w)))
            .with_no_wrap()
            .with_debug_name("left_col"));

    // Card 1: Basic pixel sizing
    {
      auto card =
          vstack(context, mk(left_col.ent(), 0),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(184)})
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
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("Equal squares; color varies.")
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(10))));

      auto boxes =
          hstack(context, mk(card.ent(), 2),
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
                .with_margin(i < 3 ? Margin::Right(pixels(8)) : Margin{}));
      }
      caption(card.ent(), 3,
              std::format("Each: {:.0f} x {:.0f}px", 48.f * pixel_scale,
                          48.f * pixel_scale));
    }

    // Card 2: Buttons
    {
      auto card =
          vstack(context, mk(left_col.ent(), 1),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(168)})
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
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(10))));

      if (button(context, mk(card.ent(), 1),
                 ComponentConfig{}
                     .with_label("A: fill width x 36px")
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_custom_background(accent_dim)
                     .with_custom_text_color(white)
                     .with_font("AtkinsonMock",
                                screen_pct(22.f * pixel_scale / sh))
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_margin(Margin::Bottom(pixels(6)))
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("adaptive_button_a")))
        selected_button = "A selected";

      if (button(context, mk(card.ent(), 2),
                 ComponentConfig{}
                     .with_label("B: fill width x 36px")
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_custom_background(green)
                     .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
                     .with_font("AtkinsonMock",
                                screen_pct(22.f * pixel_scale / sh))
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("adaptive_button_b")))
        selected_button = "B selected";
      caption(card.ent(), 3,
              std::format("{}: {:.0f} x {:.0f}px", selected_button,
                          252.f * pixel_scale, 36.f * pixel_scale));
    }

    // Card 3: Padding demo
    {
      auto card =
          vstack(context, mk(left_col.ent(), 2),
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
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      auto inner = div(
          context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(46)})
              .with_custom_background(afterhours::colors::darken(card_bg, 0.7f))
              .with_custom_text_color(muted)
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.04f)
              .with_padding(Padding::all(pixels(8))));
      caption(inner.ent(), 0, "Inner content area", 30.f);
      const float width = 280.f * pixel_scale;
      const float height = 120.f * pixel_scale;
      const float pad = 14.f * pixel_scale;
      const std::array<afterhours::RectangleType, 4> brackets = {
          {{0.f, height * .5f, pad, 1.f},
           {width - pad, height * .5f, pad, 1.f},
           {width * .5f, 0.f, 1.f, pad},
           {width * .5f, height - pad, 1.f, pad}}};
      for (int i = 0; i < 4; ++i) {
        const auto bracket = brackets[static_cast<size_t>(i)];
        div(context, mk(card.ent(), 10 + i),
            ComponentConfig{}
                .with_size({screen_pct(bracket.width / sw),
                            screen_pct(bracket.height / sh)})
                .with_absolute_position(bracket.x - pad, bracket.y - pad)
                .with_custom_background(accent)
                .with_corner_radius(0.f));
      }
    }

    // ── Right column ────────────────────────────────────────────
    auto right_col = vstack(
        context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(right_w),
                                     screen_pct(canvas_h / sh - pad_y * 2)})
            .with_no_wrap()
            .with_debug_name("right_col"));

    // Card 4: Expand demo
    {
      auto card =
          vstack(context, mk(right_col.ent(), 0),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(176)})
                     .with_custom_background(card_bg)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.06f)
                     .with_padding(Padding::all(pixels(14)))
                     .with_margin(Margin::Bottom(pixels(12)))
                     .with_no_wrap()
                     .with_debug_name("card_expand"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Expand & Flex / widths in a 1 : 2 : 3 ratio")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("Adaptive scales pixel lengths. Percent and expand "
                          "share the available space.")
              .with_size(ComponentSize{percent(1.0f), pixels(32)})
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(10))));

      // Three weighted expand children
      auto row = hstack(context, mk(card.ent(), 2),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(64)})
                            .with_no_wrap()
                            .with_align_items(AlignItems::Center)
                            .with_debug_name("expand_row"));

      struct ExpandItem {
        const char *label;
        float weight;
        afterhours::Color color;
        afterhours::Color text;
      };
      ExpandItem items[] = {
          {"1x", 1.f, red, white},
          {"2x", 2.f, orange, {30, 30, 40, 255}},
          {"3x", 3.f, green, {20, 20, 30, 255}},
      };
      for (int i = 0; i < 3; i++) {
        div(context, mk(row.ent(), i),
            ComponentConfig{}
                .with_label(items[i].label)
                .with_size(
                    ComponentSize{expand(items[i].weight), percent(1.0f)})
                .with_custom_background(items[i].color)
                .with_border(surface, 1.f)
                .with_custom_text_color(items[i].text)
                .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.08f)
                .with_alignment(TextAlignment::Center));
      }
    }

    // Card 5: screen_pct vs pixels comparison
    {
      auto card =
          vstack(context, mk(right_col.ent(), 1),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(164)})
                     .with_custom_background(card_bg)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.06f)
                     .with_padding(Padding::all(pixels(14)))
                     .with_margin(Margin::Bottom(pixels(12)))
                     .with_no_wrap()
                     .with_debug_name("card_screenpct"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("pixels(80), h720(80), screen_pct(6.25%)")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label("h720(80) is 80/720 of viewport width here; 6.25% is "
                          "80/1280.")
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_custom_text_color(muted)
              .with_margin(Margin::Bottom(pixels(8))));

      auto bars =
          hstack(context, mk(card.ent(), 2),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(52)})
                     .with_no_wrap()
                     .with_align_items(AlignItems::Center)
                     .with_debug_name("bars"));

      // Bar using pixels (scales in Adaptive)
      div(context, mk(bars.ent(), 0),
          ComponentConfig{}
              .with_label("80px")
              .with_size(ComponentSize{pixels(80), percent(1.0f)})
              .with_custom_background(accent)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_margin(Margin::Right(pixels(6)))
              .with_alignment(TextAlignment::Center)
              .with_debug_name("adaptive_pixels"));

      // Bar using h720 (does NOT scale) with per-component override
      div(context, mk(bars.ent(), 1),
          ComponentConfig{}
              .with_label("80/720")
              .with_size(ComponentSize{h720(80.0f), percent(1.0f)})
              .with_custom_background(orange)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font("AtkinsonMock", screen_pct(20.f / sh))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_margin(Margin::Right(pixels(6)))
              .with_alignment(TextAlignment::Center)
              .with_scaling_mode(ScalingMode::Proportional)
              .with_debug_name("adaptive_h720"));

      // Bar using screen_pct (does NOT scale)
      div(context, mk(bars.ent(), 2),
          ComponentConfig{}
              .with_label("6.25%")
              .with_size(
                  ComponentSize{screen_pct(80.0f / 1280.0f), percent(1.0f)})
              .with_custom_background(green)
              .with_custom_text_color(afterhours::Color{20, 20, 30, 255})
              .with_font("AtkinsonMock", screen_pct(20.f / sh))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("adaptive_screen_pct"));
      auto ruler = div(context, mk(card.ent(), 3),
                       ComponentConfig{}.with_size({percent(1.f), pixels(20)}));
      const float ruler_height = 20.f * std::min(1.f, pixel_scale);
      div(context, mk(ruler.ent(), 20),
          ComponentConfig{}
              .with_size({screen_pct(320.f / sw), screen_pct(1.f / sh)})
              .with_absolute_position(0.f, ruler_height - 1.f)
              .with_custom_background(muted)
              .with_corner_radius(0.f));
      for (int tick = 0; tick <= 4; ++tick) {
        div(context, mk(ruler.ent(), tick),
            ComponentConfig{}
                .with_label(std::format("{}", tick * 80))
                .with_size({screen_pct(52.f / sw),
                            screen_pct(20.f * std::min(1.f, pixel_scale) / sh)})
                .with_absolute_position(static_cast<float>(tick * 80), 0.f)
                .with_font("AtkinsonMock",
                           screen_pct(18.f * std::min(1.f, pixel_scale) / sh))
                .with_custom_text_color(muted));
      }
    }

    // Card 6: LayoutInfo breakpoints
    {
      auto card =
          vstack(context, mk(right_col.ent(), 2),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(132)})
                     .with_custom_background(card_bg)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.06f)
                     .with_padding(Padding::all(pixels(14)))
                     .with_no_wrap()
                     .with_debug_name("card_breakpoints"));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label("Width breakpoints / logical pixels")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_font("AtkinsonMock", screen_pct(22.f * pixel_scale / sh))
              .with_custom_text_color(accent)
              .with_margin(Margin::Bottom(pixels(8))));

      auto pills =
          hstack(context, mk(card.ent(), 1),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(26)})
                     .with_align_items(AlignItems::Center)
                     .with_no_wrap()
                     .with_debug_name("breakpoint_pills"));

      auto pill = [&](int id, const std::string &label, bool active) {
        div(context, mk(pills.ent(), id),
            ComponentConfig{}
                .with_label(label + (active ? " / active" : ""))
                .with_size(ComponentSize{pixels(220), pixels(26)})
                .with_custom_background(active ? accent : surface)
                .with_custom_text_color(
                    active ? afterhours::Color{20, 20, 30, 255} : muted)
                .with_font("AtkinsonMock", screen_pct(20.f * pixel_scale / sh))
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.4f)
                .with_margin(Margin::Right(pixels(6)))
                .with_alignment(TextAlignment::Center));
      };

      pill(0, "Narrow <800", info.is_narrow());
      pill(1, "Medium 800-1199", info.is_medium());
      pill(2, "Wide >=1200", info.is_wide());
      caption(card.ent(), 2,
              info.is_short() ? "Height: short <600 / active"
                              : "Height: regular >=600 / active");
    }
    auto measurements = vstack(context, mk(right_col.ent(), 3),
                               ComponentConfig{}
                                   .with_size({percent(1.f), pixels(50)})
                                   .with_margin(Margin::Top(pixels(4))));
    caption(measurements.ent(), 0,
            "Widths at 100% -> current (physical pixels)");
    caption(measurements.ent(), 1,
            std::format("pixels: 80 -> {:.0f}    h720: {:.0f} -> {:.0f}    "
                        "6.25%: {:.0f} -> {:.0f}",
                        80.f * pixel_scale, sw * 80.f / 720.f,
                        sw * 80.f / 720.f, sw * .0625f, sw * .0625f));
  }
};

REGISTER_EXAMPLE_SCREEN(adaptive_scaling, "Tools",
                        "Interactive demo of Adaptive vs Proportional scaling",
                        AdaptiveScalingDemo)
