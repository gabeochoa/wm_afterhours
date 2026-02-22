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
// Covers: translate positioning, margin-as-offset, corner pinning,
// centered overlay, mixed absolute+flow, and screen_pct sizing.
struct AbsolutePositioningShowcase : ScreenSystem<UIContext<InputAction>> {

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
    const auto white = afterhours::Color{255, 255, 255, 255};
    const float sw = (float)Settings::get().get_screen_width();

    // Full-screen root
    auto root = vstack(context, mk(entity, 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                           .with_background(Theme::Usage::Background)
                           .with_padding(Padding{.top = pixels(8),
                                                 .left = pixels(16),
                                                 .bottom = pixels(8),
                                                 .right = pixels(16)})
                           .with_debug_name("abs_root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Absolute Positioning")
            .with_size(ComponentSize{percent(1.0f), h720(36.f)})
            .with_custom_background(teal)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_debug_name("title"));

    // -- Section 1: Translate positioning (correct pattern) --
    auto section1 = vstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(120.f)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6)})
            .with_padding(Spacing::sm)
            .with_debug_name("section_translate"));

    div(context, mk(section1.ent(), 0),
        ComponentConfig{}
            .with_label("1. Translate + Absolute (correct pattern)")
            .with_size(ComponentSize{percent(1.0f), h720(20.f)})
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    // A container with a relatively-positioned child and an absolute child
    auto s1_container =
        div(context, mk(section1.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), h720(70.f)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_debug_name("s1_container"));

    // Absolute box positioned via translate
    button(context, mk(s1_container.ent(), 0),
           ComponentConfig{}
               .with_label("translate(100,10)")
               .with_size(
                   ComponentSize{h720(160.f), h720(50.f)})
               .with_custom_background(green)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(h720(100.f), h720(10.f))
               .with_debug_name("abs_translate"));

    // Another absolute box further right
    button(context, mk(s1_container.ent(), 1),
           ComponentConfig{}
               .with_label("translate(300,10)")
               .with_size(
                   ComponentSize{h720(160.f), h720(50.f)})
               .with_custom_background(pink)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(h720(300.f), h720(10.f))
               .with_debug_name("abs_translate_2"));

    // -- Section 2: Corner pinning --
    auto section2 =
        div(context, mk(root.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), h720(160.f)})
                .with_custom_background(panel_bg)
                .with_margin(Margin{.top = pixels(6)})
                .with_padding(Spacing::sm)
                .with_debug_name("section_corners"));

    div(context, mk(section2.ent(), 0),
        ComponentConfig{}
            .with_label("2. Corner Pinning (absolute + translate)")
            .with_size(ComponentSize{percent(1.0f), h720(20.f)})
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    // Corner container with known size
    float corner_w = sw - 80.f;
    float sh = (float)Settings::get().get_screen_height();
    float box_s = 50.f * sh / 720.f;
    float corner_h = 110.f * sh / 720.f;

    auto corner_area =
        div(context, mk(section2.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(corner_w), h720(110.f)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_debug_name("corner_area"));

    // Top-left
    button(context, mk(corner_area.ent(), 0),
           ComponentConfig{}
               .with_label("TL")
               .with_size(ComponentSize{h720(50.f), h720(50.f)})
               .with_custom_background(teal)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(0.f, 0.f)
               .with_debug_name("corner_tl"));

    // Top-right
    button(context, mk(corner_area.ent(), 1),
           ComponentConfig{}
               .with_label("TR")
               .with_size(ComponentSize{h720(50.f), h720(50.f)})
               .with_custom_background(pink)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(corner_w - box_s, 0.f)
               .with_debug_name("corner_tr"));

    // Bottom-left
    button(context, mk(corner_area.ent(), 2),
           ComponentConfig{}
               .with_label("BL")
               .with_size(ComponentSize{h720(50.f), h720(50.f)})
               .with_custom_background(orange)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(0.f, corner_h - box_s)
               .with_debug_name("corner_bl"));

    // Bottom-right
    button(context, mk(corner_area.ent(), 3),
           ComponentConfig{}
               .with_label("BR")
               .with_size(ComponentSize{h720(50.f), h720(50.f)})
               .with_custom_background(purple)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_absolute_position(corner_w - box_s, corner_h - box_s)
               .with_debug_name("corner_br"));

    // -- Section 3: Mixed absolute + flow --
    auto section3 = vstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(120.f)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6)})
            .with_padding(Spacing::sm)
            .with_debug_name("section_mixed"));

    div(context, mk(section3.ent(), 0),
        ComponentConfig{}
            .with_label("3. Mixed: Flow children + Absolute overlay")
            .with_size(ComponentSize{percent(1.0f), h720(20.f)})
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    auto mixed_container = hstack(
        context, mk(section3.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(70.f)})
            .with_custom_background(afterhours::Color{30, 36, 50, 255})
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
      div(context, mk(mixed_container.ent(), i),
          ComponentConfig{}
              .with_label(flow_labels[i])
              .with_size(ComponentSize{percent(0.28f), percent(0.8f)})
              .with_custom_background(flow_colors[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
              .with_debug_name(fmt::format("flow_{}", i)));
    }

    // Absolute badge overlaid in top-right corner
    div(context, mk(mixed_container.ent(), 10),
        ComponentConfig{}
            .with_label("BADGE")
            .with_size(
                ComponentSize{h720(70.f), h720(24.f)})
            .with_custom_background(pink)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_absolute_position(sw - 180.f * sh / 720.f, 4.f * sh / 720.f)
            .with_debug_name("abs_badge"));

    // -- Section 4: screen_pct sizing --
    auto section4 = vstack(
        context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(120.f)})
            .with_custom_background(panel_bg)
            .with_margin(Margin{.top = pixels(6)})
            .with_padding(Spacing::sm)
            .with_debug_name("section_screen_pct"));

    div(context, mk(section4.ent(), 0),
        ComponentConfig{}
            .with_label("4. Absolute + screen_pct sizing")
            .with_size(ComponentSize{percent(1.0f), h720(20.f)})
            .with_custom_text_color(white)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_skip_tabbing(true));

    auto s4_container =
        div(context, mk(section4.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), h720(70.f)})
                .with_custom_background(afterhours::Color{30, 36, 50, 255})
                .with_debug_name("s4_container"));

    // A box sized as 30% of screen width, 8% of screen height
    div(context, mk(s4_container.ent(), 0),
        ComponentConfig{}
            .with_label("30% x 8% of screen")
            .with_size(ComponentSize{screen_pct(0.3f), screen_pct(0.08f)})
            .with_custom_background(green)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_absolute_position(h720(20.f), h720(10.f))
            .with_debug_name("abs_screen_pct"));

    // Another box at a different position
    div(context, mk(s4_container.ent(), 1),
        ComponentConfig{}
            .with_label("20% x 8% of screen")
            .with_size(ComponentSize{screen_pct(0.2f), screen_pct(0.08f)})
            .with_custom_background(orange)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_absolute_position(h720(420.f), h720(10.f))
            .with_debug_name("abs_screen_pct_2"));
  }
};

REGISTER_EXAMPLE_SCREEN(absolute_positioning, "System Demos",
                        "Absolute positioning patterns and verification",
                        AbsolutePositioningShowcase)
