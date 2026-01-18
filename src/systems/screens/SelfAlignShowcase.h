#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Showcase for the new SelfAlign feature
struct SelfAlignShowcase : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::sage_natural();
    theme.background = afterhours::Color{35, 40, 45, 255};
    theme.surface = afterhours::Color{50, 55, 60, 255};
    context.theme = theme;

    int sw = Settings::get().get_screen_width();
    int sh = Settings::get().get_screen_height();

    // Main centered container
    auto main = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((float)sw), pixels((float)sh)})
            .with_custom_background(theme.background)
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::Center)
            .with_debug_name("main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_label("SelfAlign Feature Demo")
            .with_size(ComponentSize{pixels(500), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_alignment(TextAlignment::Center)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_skip_tabbing(true)
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_label("Override parent alignment for individual elements")
            .with_size(ComponentSize{pixels(700), pixels(50)})
            .with_custom_text_color(afterhours::Color{180, 180, 180, 255})
            .with_alignment(TextAlignment::Center)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.top = pixels(8), .bottom = pixels(15),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("subtitle"));

    // Single demo container - a column, so children stack vertically
    // but can use SelfAlign to control their horizontal position
    auto demo = div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(600), pixels(220)})
            .with_custom_background(afterhours::Color{45, 50, 55, 255})
            .with_border(afterhours::Color{80, 80, 80, 255}, 1.0f)
            .with_flex_direction(FlexDirection::Column)
            .with_padding(Spacing::md)
            .with_debug_name("demo"));

    // FlexStart - aligns left
    div(context, mk(demo.ent(), 0),
        ComponentConfig{}
            .with_label("FlexStart (left)")
            .with_size(ComponentSize{pixels(180), pixels(45)})
            .with_self_align(SelfAlign::FlexStart)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("flex_start"));

    // Center - centers horizontally
    div(context, mk(demo.ent(), 1),
        ComponentConfig{}
            .with_label("Center")
            .with_size(ComponentSize{pixels(180), pixels(45)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("center"));

    // FlexEnd - aligns right
    div(context, mk(demo.ent(), 2),
        ComponentConfig{}
            .with_label("FlexEnd (right)")
            .with_size(ComponentSize{pixels(180), pixels(45)})
            .with_self_align(SelfAlign::FlexEnd)
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Spacing::xs)
            .with_skip_tabbing(true)
            .with_debug_name("flex_end"));

    // Code example
    div(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_label(".with_self_align(SelfAlign::Center)")
            .with_size(ComponentSize{pixels(500), pixels(50)})
            .with_custom_background(afterhours::Color{40, 50, 45, 255})
            .with_custom_text_color(afterhours::Color{150, 255, 150, 255})
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(25), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("code"));

    // Benefits
    div(context, mk(main.ent(), 4),
        ComponentConfig{}
            .with_label("No manual calculations - just set alignment!")
            .with_size(ComponentSize{pixels(500), pixels(35)})
            .with_custom_text_color(afterhours::Color{100, 220, 100, 255})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(12), .bottom = pixels(0),
                                .left = pixels(0), .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("benefits"));
  }
};

REGISTER_EXAMPLE_SCREEN(self_align, "System Demos",
                        "SelfAlign feature for centering content",
                        SelfAlignShowcase)
