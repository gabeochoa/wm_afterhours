#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates absolute positioning patterns and verifies correct behavior.
// Covers translate positioning, corner pinning, mixed absolute+flow,
// and screen_pct sizing.
struct AbsolutePositioningShowcase : ScreenSystem<UIContext<InputAction>> {

  std::string last_sample = "Select a sample to inspect its coordinates.";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    const auto teal = afterhours::Color{0, 180, 220, 255};
    const auto pink = afterhours::Color{220, 60, 120, 255};
    const auto green = afterhours::Color{50, 210, 130, 255};
    const auto orange = afterhours::Color{250, 140, 40, 255};
    const auto purple = afterhours::Color{160, 100, 255, 255};
    const auto panel_bg = afterhours::Color{20, 26, 40, 255};
    const float sw = static_cast<float>(Settings::get().get_screen_width());
    const float sh = static_cast<float>(Settings::get().get_screen_height());
    const float scale = sh / 720.f;
    const float well_width = sw - 64.f * scale;
    const auto ink = afterhours::Color{220, 230, 243, 255};
    const auto muted = afterhours::Color{157, 178, 202, 255};
    const auto edge = afterhours::Color{74, 95, 120, 255};
    auto text = [&](afterhours::Entity &parent, int id,
                    const std::string &label, float height,
                    const std::string &name, Size width = percent(1.f)) {
      return div(context, mk(parent, id),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size({width, pixels(height * scale)})
                     .with_font("AtkinsonMock", pixels(22.f * scale))
                     .with_custom_text_color(muted)
                     .with_alignment(TextAlignment::Left)
                     .with_text_inset(0.f, 0.f)
                     .with_skip_tabbing(true)
                     .with_debug_name(name));
    };
    auto heading = [&](afterhours::Entity &parent, int id, const char *number,
                       const std::string &label) {
      auto row = hstack(context, mk(parent, id),
                        ComponentConfig{}
                            .with_size({percent(1.f), pixels(24.f * scale)})
                            .with_gap(pixels(10.f * scale))
                            .with_no_wrap());
      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(number)
              .with_size({pixels(28.f * scale), pixels(24.f * scale)})
              .with_font("AtkinsonMockBold", pixels(18.f * scale))
              .with_custom_background(teal)
              .with_custom_text_color(panel_bg)
              .with_alignment(TextAlignment::Center)
              .with_corner_radius(4.f * scale));
      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(label)
              .with_size({expand(), pixels(24.f * scale)})
              .with_font("AtkinsonMockBold", pixels(24.f * scale))
              .with_custom_text_color(ink)
              .with_text_inset(0.f, 0.f));
    };
    auto line = [&](afterhours::Entity &parent, int id, float x, float y,
                    float width, float height) {
      div(context, mk(parent, id),
          ComponentConfig{}
              .with_size({pixels(width), pixels(height)})
              .with_absolute_position(x, y)
              .with_custom_background(edge)
              .with_corner_radius(0.f)
              .with_skip_tabbing(true));
    };

    // Full-screen root
    auto root =
        vstack(context, mk(entity, 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_background(Theme::Usage::Background)
                   .with_padding(Padding{.top = pixels(8.f * scale),
                                         .left = pixels(20.f * scale),
                                         .bottom = pixels(8.f * scale),
                                         .right = pixels(20.f * scale)})
                   .with_corner_radius(0.f)
                   .with_debug_name("abs_root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Absolute Positioning")
            .with_custom_text_color(ink)
            .with_font("AtkinsonMockBold", pixels(36.f * scale))
            .with_margin(Margin{.left = pixels(12.f * scale)})
            .with_size({pixels(sw - 64.f * scale), pixels(40.f * scale)})
            .with_debug_name("title"));

    // -- Section 1: Translate positioning (correct pattern) --
    auto section1 = vstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(136.f * scale)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6.f * scale)})
            .with_padding(Padding{.top = pixels(12.f * scale),
                                  .left = pixels(12.f * scale),
                                  .bottom = pixels(12.f * scale),
                                  .right = pixels(12.f * scale)})
            .with_corner_radius(12.f * scale)
            .with_debug_name("section_translate"));

    heading(section1.ent(), 0, "01",
            "Translate: offsets start at the container origin");

    // A container with a relatively-positioned child and an absolute child
    auto s1_container =
        div(context, mk(section1.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(66.f * scale)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_border(edge, 1.f * scale)
                .with_corner_radius(6.f * scale)
                .with_debug_name("s1_container"));

    // Absolute box positioned via translate
    if (button(context, mk(s1_container.ent(), 0),
               ComponentConfig{}
                   .with_label("(100,10)")
                   .with_size(ComponentSize{pixels(160.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(green)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(14.f * scale))
                   .with_absolute_position(pixels(100.f * scale),
                                           pixels(10.f * scale))
                   .with_debug_name("abs_translate")))
      last_sample = "Translate sample: x=100, y=10; width=160, height=44 "
                    "reference pixels.";

    // Another absolute box further right
    if (button(context, mk(s1_container.ent(), 1),
               ComponentConfig{}
                   .with_label("(300,10)")
                   .with_size(ComponentSize{pixels(160.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(pink)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(14.f * scale))
                   .with_absolute_position(pixels(300.f * scale),
                                           pixels(10.f * scale))
                   .with_debug_name("abs_translate_2")))
      last_sample = "Translate sample: x=300, y=10; width=160, height=44 "
                    "reference pixels.";

    auto translate_ruler =
        div(context, mk(section1.ent(), 2),
            ComponentConfig{}.with_size({percent(1.f), pixels(22.f * scale)}));
    for (const int x : {0, 100, 300}) {
      line(s1_container.ent(), 100 + x, static_cast<float>(x) * scale, 0.f,
           scale, 6.f * scale);
      div(context, mk(translate_ruler.ent(), x),
          ComponentConfig{}
              .with_label(fmt::format("x={}", x))
              .with_font("AtkinsonMock", pixels(14.f * scale))
              .with_custom_text_color(muted)
              .with_size({pixels(90.f * scale), pixels(22.f * scale)})
              .with_absolute_position(static_cast<float>(x) * scale, 0.f));
    }
    line(s1_container.ent(), 500, 12.f * scale, 0.f, scale, 10.f * scale);
    line(s1_container.ent(), 501, 8.f * scale, 10.f * scale, 9.f * scale,
         scale);
    div(context, mk(s1_container.ent(), 502),
        ComponentConfig{}
            .with_label("y=10")
            .with_size({pixels(64.f * scale), pixels(22.f * scale)})
            .with_font("AtkinsonMock", pixels(14.f * scale))
            .with_custom_text_color(muted)
            .with_absolute_position(20.f * scale, 0.f));

    // -- Section 2: Corner pinning --
    auto section2 =
        div(context, mk(root.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(184.f * scale)})
                .with_custom_background(panel_bg)
                .with_margin(Margin{.top = pixels(6.f * scale)})
                .with_padding(Padding{.top = pixels(12.f * scale),
                                      .left = pixels(12.f * scale),
                                      .bottom = pixels(12.f * scale),
                                      .right = pixels(12.f * scale)})
                .with_corner_radius(12.f * scale)
                .with_debug_name("section_corners"));

    heading(section2.ent(), 0, "02",
            "Corner pinning: subtract the sample size from the far edge");

    // Corner container with known size
    const float corner_w = well_width;
    const float box_s = 44.f * scale;
    const float corner_h = 112.f * scale;

    auto corner_area =
        div(context, mk(section2.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(corner_w), pixels(corner_h)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_border(edge, 1.f * scale)
                .with_corner_radius(6.f * scale)
                .with_debug_name("corner_area"));

    // Top-left
    if (button(context, mk(corner_area.ent(), 0),
               ComponentConfig{}
                   .with_label("TL")
                   .with_size(ComponentSize{pixels(44.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(teal)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(22.f * scale))
                   .with_absolute_position(0.f, 0.f)
                   .with_debug_name("corner_tl")))
      last_sample = "Top left: 44 x 44 reference pixels, pinned inside the "
                    "outlined container.";

    // Top-right
    if (button(context, mk(corner_area.ent(), 1),
               ComponentConfig{}
                   .with_label("TR")
                   .with_size(ComponentSize{pixels(44.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(pink)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(22.f * scale))
                   .with_absolute_position(corner_w - box_s, 0.f)
                   .with_debug_name("corner_tr")))
      last_sample = "Top right: 44 x 44 reference pixels, pinned inside the "
                    "outlined container.";

    // Bottom-left
    if (button(context, mk(corner_area.ent(), 2),
               ComponentConfig{}
                   .with_label("BL")
                   .with_size(ComponentSize{pixels(44.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(orange)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(22.f * scale))
                   .with_absolute_position(0.f, corner_h - box_s)
                   .with_debug_name("corner_bl")))
      last_sample = "Bottom left: 44 x 44 reference pixels, pinned inside the "
                    "outlined container.";

    // Bottom-right
    if (button(context, mk(corner_area.ent(), 3),
               ComponentConfig{}
                   .with_label("BR")
                   .with_size(ComponentSize{pixels(44.f * scale),
                                            pixels(44.f * scale)})
                   .with_custom_background(purple)
                   .with_auto_text_color(true)
                   .with_font("AtkinsonMock", pixels(22.f * scale))
                   .with_absolute_position(corner_w - box_s, corner_h - box_s)
                   .with_debug_name("corner_br")))
      last_sample = "Bottom right: 44 x 44 reference pixels, pinned inside the "
                    "outlined container.";

    line(corner_area.ent(), 100, 0.f, 0.f, 9.f * scale, 2.f * scale);
    line(corner_area.ent(), 101, 0.f, 0.f, 2.f * scale, 9.f * scale);
    text(section2.ent(), 2,
         fmt::format(
             "TL / TR: top left / right     BL / BR: bottom left / right     "
             "Samples: {:.0f} x {:.0f} px    Well: {:.0f} x {:.0f} px",
             box_s, box_s, corner_w, corner_h),
         24.f, "corner_legend");

    // -- Section 3: Mixed absolute + flow --
    auto section3 = vstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(132.f * scale)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6.f * scale)})
            .with_padding(Padding{.top = pixels(12.f * scale),
                                  .left = pixels(12.f * scale),
                                  .bottom = pixels(12.f * scale),
                                  .right = pixels(12.f * scale)})
            .with_corner_radius(12.f * scale)
            .with_debug_name("section_mixed"));

    heading(section3.ent(), 0, "03",
            "Flow and overlays: the badge does not reserve layout space");

    auto mixed_container = hstack(
        context, mk(section3.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(60.f * scale)})
            .with_custom_background(afterhours::Color{30, 36, 50, 255})
            .with_border(edge, 1.f * scale)
            .with_corner_radius(6.f * scale)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_debug_name("mixed_container"));

    // Three flow children
    const char *flow_labels[] = {"Flow A", "Flow B", "Flow C"};
    afterhours::Color flow_colors[] = {
        afterhours::Color{60, 80, 120, 255},
        afterhours::Color{60, 100, 80, 255},
        afterhours::Color{100, 60, 80, 255},
    };
    for (int i = 0; i < 3; i++) {
      auto block =
          div(context, mk(mixed_container.ent(), i),
              ComponentConfig{}
                  .with_size({percent(.28f), percent(.8f)})
                  .with_custom_background(flow_colors[i])
                  .with_padding(Padding{.left = pixels(12.f * scale),
                                        .right = pixels(12.f * scale)}));
      div(context, mk(block.ent(), 0),
          ComponentConfig{}
              .with_label(flow_labels[i])
              .with_size({percent(1.f), percent(1.f)})
              .with_font("AtkinsonMock", pixels(22.f * scale))
              .with_custom_text_color(ink)
              .with_debug_name(fmt::format("flow_{}", i)));
    }

    // Absolute badge overlaid in top-right corner
    div(context, mk(mixed_container.ent(), 10),
        ComponentConfig{}
            .with_label("BADGE")
            .with_size(
                ComponentSize{pixels(90.f * scale), pixels(24.f * scale)})
            .with_custom_background(pink)
            .with_auto_text_color(true)
            .with_font("AtkinsonMock", pixels(14.0f * scale))
            .with_alignment(TextAlignment::Center)
            .with_absolute_position(well_width - 106.f * scale, 4.f * scale)
            .with_debug_name("abs_badge"));

    line(mixed_container.ent(), 100, well_width - 16.f * scale, 0.f,
         16.f * scale, scale);
    line(mixed_container.ent(), 101, well_width - 16.f * scale, 0.f, scale,
         4.f * scale);
    text(section3.ent(), 2,
         fmt::format("Space around: {:.0f}px outer / {:.0f}px between     "
                     "Badge layer: {:.0f}px from right, {:.0f}px from top",
                     well_width * .16f / 6.f, well_width * .16f / 3.f,
                     16.f * scale, 4.f * scale),
         24.f, "flow_legend");

    // -- Section 4: screen_pct sizing --
    auto section4 = vstack(
        context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(132.f * scale)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6.f * scale)})
            .with_padding(Padding{.top = pixels(12.f * scale),
                                  .left = pixels(12.f * scale),
                                  .bottom = pixels(12.f * scale),
                                  .right = pixels(12.f * scale)})
            .with_corner_radius(12.f * scale)
            .with_debug_name("section_screen_pct"));

    heading(section4.ent(), 0, "04",
            fmt::format("Screen percentages: viewport width {:.0f} px", sw));

    auto s4_container =
        div(context, mk(section4.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(60.f * scale)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_border(edge, 1.f * scale)
                .with_corner_radius(6.f * scale)
                .with_debug_name("s4_container"));

    // A box sized as 30% of screen width, 8% of screen height
    auto percentage_0 =
        div(context, mk(s4_container.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.3f), screen_pct(0.08f)})
                .with_custom_background(green)
                .with_auto_text_color(true)
                .with_font("AtkinsonMock", pixels(22.f * scale))
                .with_padding(Padding{.left = pixels(12.f * scale),
                                      .right = pixels(12.f * scale)})
                .with_absolute_position(0.f, 0.f)
                .with_debug_name("abs_screen_pct"));
    div(context, mk(percentage_0.ent(), 0),
        ComponentConfig{}
            .with_label("30% x 8% of screen")
            .with_size({percent(1.f), percent(1.f)})
            .with_font("AtkinsonMock", pixels(22.f * scale))
            .with_custom_text_color(afterhours::Color{10, 20, 30, 255}));

    // Another box at a different position
    auto percentage_1 =
        div(context, mk(s4_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.2f), screen_pct(0.08f)})
                .with_custom_background(orange)
                .with_auto_text_color(true)
                .with_font("AtkinsonMock", pixels(22.f * scale))
                .with_padding(Padding{.left = pixels(12.f * scale),
                                      .right = pixels(12.f * scale)})
                .with_absolute_position(sw * .3f + 20.f * scale, 0.f)
                .with_debug_name("abs_screen_pct_2"));
    div(context, mk(percentage_1.ent(), 0),
        ComponentConfig{}
            .with_label("20% x 8% of screen")
            .with_size({percent(1.f), percent(1.f)})
            .with_font("AtkinsonMock", pixels(22.f * scale))
            .with_custom_text_color(afterhours::Color{10, 20, 30, 255}));
    auto percentage_ruler =
        hstack(context, mk(section4.ent(), 2),
               ComponentConfig{}
                   .with_size({percent(1.f), pixels(24.f * scale)})
                   .with_no_wrap()
                   .with_justify_content(JustifyContent::SpaceBetween));
    text(
        percentage_ruler.ent(), 0,
        fmt::format("30% = {:.0f}px    20% = {:.0f}px    Height: 8% = {:.1f}px",
                    sw * .3f, sw * .2f, sh * .08f),
        24.f, "percentage_dimensions", pixels(560.f * scale));
    auto viewport_scale =
        div(context, mk(percentage_ruler.ent(), 1),
            ComponentConfig{}.with_size({expand(), pixels(24.f * scale)}));
    line(viewport_scale.ent(), 0, 0.f, 20.f * scale, well_width - 560.f * scale,
         scale);
    for (int tick = 0; tick <= 4; ++tick) {
      const float ruler_width = well_width - 560.f * scale;
      const float x =
          static_cast<float>(tick) * (ruler_width - 44.f * scale) / 4.f;
      div(context, mk(viewport_scale.ent(), tick + 1),
          ComponentConfig{}
              .with_label(fmt::format("{}%", tick * 25))
              .with_size({pixels(44.f * scale), pixels(20.f * scale)})
              .with_font("AtkinsonMock", pixels(14.f * scale))
              .with_custom_text_color(muted)
              .with_absolute_position(x, 0.f));
      line(viewport_scale.ent(), tick + 10, x, 18.f * scale, scale,
           4.f * scale);
    }
    text(root.ent(), 5,
         "Origin: top left. +X: right. +Y: down.  " + last_sample, 32.f,
         "abs_status");
  }
};

REGISTER_EXAMPLE_SCREEN(absolute_positioning, "System Demos",
                        "Absolute positioning patterns and verification",
                        AbsolutePositioningShowcase)
