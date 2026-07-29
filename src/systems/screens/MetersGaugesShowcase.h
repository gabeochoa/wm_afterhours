#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MetersGaugesShowcase : ScreenSystem<UIContext<InputAction>> {
  float control_value = 0.65f;
  float anim_progress = 0.f;
  bool anim_forward = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    const auto FONT = UIComponent::DEFAULT_FONT;

    // Theme-complementary colors (softer, work with ocean_navy palette)
    // Instead of harsh saturated RGB, use muted tones that harmonize
    const afterhours::Color hp_color{190, 65, 65, 255};      // muted red
    const afterhours::Color mp_color{65, 110, 190, 255};     // muted blue
    const afterhours::Color xp_color{190, 155, 50, 255};     // muted gold
    const afterhours::Color health_color{60, 170, 80, 255};  // muted green
    const afterhours::Color shield_color{70, 130, 200, 230}; // muted blue
    const afterhours::Color track_color{55, 70, 100, 255};   // visible track

    // Animate progress (bounces 0 -> 1 -> 0 over ~3s each way)
    if (anim_forward) {
      anim_progress += dt / 3.0f;
      if (anim_progress >= 1.0f) {
        anim_progress = 1.0f;
        anim_forward = false;
      }
    } else {
      anim_progress -= dt / 3.0f;
      if (anim_progress <= 0.0f) {
        anim_progress = 0.0f;
        anim_forward = true;
      }
    }

    // Shared rounded corners for visual consistency (default = all rounded)
    auto corners = RoundedCorners();

    // Main container
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Background)
            .with_padding(Spacing::sm)
            .with_no_wrap()
            .with_debug_name("mg_root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Meters & Gauges")
            .with_size(ComponentSize{percent(1.0f), percent(0.06f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(FONT, pixels(24.0f))
            .with_rounded_corners(corners));

    // =================================================================
    // Row 1: Inline Meters
    // How to make an inline meter: row div with a label + progress_bar
    // =================================================================
    auto row1 =
        vstack(context, mk(root.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.17f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(corners)
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("mg_row_meters"));

    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("Inline Meters")
            .with_size(ComponentSize{percent(1.0f), percent(0.22f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font(FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    auto meters_row =
        hstack(context, mk(row1.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.73f)})
                   .with_background(Theme::Usage::Surface)
                   .with_align_items(AlignItems::Center));

    struct MeterInfo {
      const char *label;
      float value;
      afterhours::Color color;
      const char *name;
    };
    MeterInfo meters[] = {
        {"HP", control_value, hp_color, "mg_hp"},
        {"MP", control_value * 0.6f, mp_color, "mg_mp"},
        {"XP", std::min(control_value * 1.3f, 1.0f), xp_color, "mg_xp"},
    };

    for (int m = 0; m < 3; m++) {
      auto meter =
          hstack(context, mk(meters_row.ent(), m),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(0.31f), percent(0.9f)})
                     .with_background(Theme::Usage::Surface)
                     .with_align_items(AlignItems::Center)
                     .with_debug_name(meters[m].name));

      // Label
      div(context, mk(meter.ent(), 0),
          ComponentConfig{}
              .with_label(meters[m].label)
              .with_size(ComponentSize{percent(0.2f), percent(0.9f)})
              .with_background(Theme::Usage::Surface)
              .with_auto_text_color(true)
              .with_font(FONT, pixels(16.0f))
              .with_skip_tabbing(true)
              .with_debug_name(fmt::format("{}_label", meters[m].name)));

      // Progress bar with rounded corners and visible track
      progress_bar(context, mk(meter.ent(), 1), meters[m].value,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.75f), pixels(28)})
                       .with_custom_background(meters[m].color)
                       .with_background(Theme::Usage::Custom)
                       .with_font(FONT, pixels(14.0f))
                       .with_skip_tabbing(true)
                       .with_rounded_corners(corners)
                       .with_debug_name(fmt::format("{}_bar", meters[m].name)),
                   ProgressBarLabelStyle::Percentage);
    }

    // =================================================================
    // Row 2: Stacked Bars
    // How to make stacked bars: just put two progress_bars in a row.
    // For overlapping (e.g. health under shield), use absolute_position()
    // on both bars inside a container. Here we show them side-by-side
    // since absolute positioning with percent sizes triggers config warnings.
    // =================================================================
    auto row2 =
        vstack(context, mk(root.ent(), 2),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.15f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(corners)
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("mg_row_stacked"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("Stacked Bars (Health + Shield)")
            .with_size(ComponentSize{percent(1.0f), percent(0.25f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font(FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    auto stack_row =
        hstack(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.9f), percent(0.6f)})
                   .with_background(Theme::Usage::Surface)
                   .with_align_items(AlignItems::Center));

    progress_bar(context, mk(stack_row.ent(), 0), control_value,
                 ComponentConfig{}
                     .with_label("Health")
                     .with_size(ComponentSize{percent(0.47f), pixels(28)})
                     .with_custom_background(health_color)
                     .with_font(FONT, pixels(14.0f))
                     .with_skip_tabbing(true)
                     .with_rounded_corners(corners)
                     .with_margin(Spacing::xs)
                     .with_debug_name("mg_health_bar"),
                 ProgressBarLabelStyle::Percentage);

    progress_bar(context, mk(stack_row.ent(), 1), control_value * 0.45f,
                 ComponentConfig{}
                     .with_label("Shield")
                     .with_size(ComponentSize{percent(0.45f), pixels(28)})
                     .with_custom_background(shield_color)
                     .with_font(FONT, pixels(14.0f))
                     .with_skip_tabbing(true)
                     .with_rounded_corners(corners)
                     .with_margin(Spacing::xs)
                     .with_debug_name("mg_shield_bar"),
                 ProgressBarLabelStyle::Percentage);

    // =================================================================
    // Row 3: Animated Progress
    // =================================================================
    auto row3 =
        vstack(context, mk(root.ent(), 3),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.14f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(corners)
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("mg_row_animated"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("Animated Progress")
            .with_size(ComponentSize{percent(1.0f), percent(0.28f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font(FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    auto anim_row =
        hstack(context, mk(row3.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.65f)})
                   .with_background(Theme::Usage::Surface)
                   .with_align_items(AlignItems::Center));

    progress_bar(context, mk(anim_row.ent(), 0), anim_progress,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(0.55f), pixels(28)})
                     .with_background(Theme::Usage::Primary)
                     .with_font(FONT, pixels(14.0f))
                     .with_margin(Spacing::xs)
                     .with_rounded_corners(corners)
                     .with_debug_name("mg_anim_bar"),
                 ProgressBarLabelStyle::Percentage);

    circular_progress(context, mk(anim_row.ent(), 1), anim_progress,
                      ComponentConfig{}
                          // 44 (was 60) so it fits the ~47px anim_row; left-only
                          // margin keeps the gap without eating row height.
                          .with_size(ComponentSize{pixels(44), pixels(44)})
                          .with_background(Theme::Usage::Primary)
                          .with_border(track_color, pixels(7.0f))
                          .with_margin(Margin{.left = DefaultSpacing::small()})
                          .with_debug_name("mg_anim_circle"));

    // =================================================================
    // Row 4: Circular Gauges (larger, centered, with visible labels)
    // =================================================================
    auto row4 =
        vstack(context, mk(root.ent(), 4),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.24f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(corners)
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("mg_row_circular"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Circular Gauges")
            .with_size(ComponentSize{percent(1.0f), percent(0.14f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font(FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    auto circles_row =
        hstack(context, mk(row4.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.82f)})
                   .with_background(Theme::Usage::Surface)
                   .with_align_items(AlignItems::Center));

    struct GaugeInfo {
      float value;
      afterhours::Color color;
      const char *name;
    };
    GaugeInfo gauges[] = {
        {0.25f, hp_color, "mg_gauge_25"},
        {0.50f, xp_color, "mg_gauge_50"},
        {0.90f, health_color, "mg_gauge_90"},
    };

    for (int g = 0; g < 3; g++) {
      auto gauge_col =
          vstack(context, mk(circles_row.ent(), g),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(0.33f), percent(0.95f)})
                     .with_background(Theme::Usage::Surface)
                     .with_align_items(AlignItems::Center)
                     .with_debug_name(gauges[g].name));

      circular_progress(
          context, mk(gauge_col.ent(), 0), gauges[g].value,
          ComponentConfig{}
              // 72 (was 90) so ring + % label fit the ~104px gauge_col
              .with_size(ComponentSize{pixels(72), pixels(72)})
              .with_custom_background(gauges[g].color)
              .with_border(track_color, pixels(8.0f))
              .with_debug_name(fmt::format("{}_ring", gauges[g].name)));

      div(context, mk(gauge_col.ent(), 1),
          ComponentConfig{}
              .with_label(
                  fmt::format("{}%", static_cast<int>(gauges[g].value * 100)))
              .with_size(ComponentSize{percent(0.8f), percent(0.18f)})
              .with_custom_text_color(theme.font)
              .with_alignment(TextAlignment::Center)
              .with_font(FONT, pixels(16.0f))
              .with_skip_tabbing(true)
              .with_debug_name(fmt::format("{}_label", gauges[g].name)));
    }

    // =================================================================
    // Row 5: Interactive Slider
    // =================================================================
    auto row5 =
        vstack(context, mk(root.ent(), 5),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.13f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(corners)
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("mg_row_slider"));

    div(context, mk(row5.ent(), 0),
        ComponentConfig{}
            .with_label("Interactive Control")
            .with_size(ComponentSize{percent(1.0f), percent(0.3f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font(FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    // Slider in a left-aligned row (mirrors the animated-progress row) so the
    // 70%-wide slider anchors at the left instead of drifting off the right.
    auto slider_row =
        hstack(context, mk(row5.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.6f)})
                   .with_background(Theme::Usage::Surface)
                   .with_align_items(AlignItems::Center));

    slider(context, mk(slider_row.ent(), 0), control_value,
           ComponentConfig{}
               .with_label("Value")
               .with_size(ComponentSize{percent(0.7f), pixels(36)})
               .with_background(Theme::Usage::Accent)
               .with_font(FONT, pixels(14.0f))
               .with_margin(Spacing::xs)
               .with_debug_name("mg_slider"),
           SliderHandleValueLabelPosition::WithLabel);
  }
};

REGISTER_EXAMPLE_SCREEN(meters_gauges, "Widgets",
                        "Meters & gauges: inline meters, stacked bars, "
                        "circular progress, animated values",
                        MetersGaugesShowcase)
