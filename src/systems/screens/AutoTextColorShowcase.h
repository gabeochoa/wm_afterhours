#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "../../theme_presets.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the auto_text_color feature which is now enabled by default.
// Text color is automatically chosen for best contrast against any background.
struct AutoTextColorShowcase : ScreenSystem<UIContext<InputAction>> {

  bool theme_initialized = false;

  void setup_theme() {
    if (theme_initialized)
      return;
    theme_initialized = true;

    // Use ocean_navy theme preset (dark background theme)
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    ThemeDefaults::get().set_theme(theme);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {

    setup_theme();

    // Full screen background (no padding so it stays in bounds)
    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size({screen_pct(1.0f), screen_pct(1.0f)})
                        .with_background(Theme::Usage::Background)
                        .with_debug_name("root_bg"));

    // Content container with padding
    auto content = div(context, mk(root.ent(), 0),
                       ComponentConfig{}
                           .with_size({percent(1.0f), percent(1.0f)})
                           .with_padding(Spacing::lg)
                           .with_flex_direction(FlexDirection::Column)
                           .with_debug_name("root"));

    // Title section - row with title text and badge
    auto title_row = div(context, mk(content.ent()),
                         ComponentConfig{}
                             .with_size({percent(1.0f), pixels(70.0f)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_justify_content(JustifyContent::Center)
                             .with_align_items(AlignItems::Center)
                             .with_no_wrap()
                             .with_margin(Spacing::xs)
                             .with_debug_name("title_row"));

    // Main title text - large and prominent
    div(context, mk(title_row.ent(), 0),
        ComponentConfig{}
            .with_label("Auto Text Color")
            .with_size({pixels(350.0f), pixels(60.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(42.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Badge for "Now Default!" - bright badge that stands out clearly
    div(context, mk(title_row.ent(), 1),
        ComponentConfig{}
            .with_label("NEW DEFAULT")
            .with_size({pixels(170.0f), pixels(40.0f)})
            .with_margin(Spacing::xs)
            .with_custom_background({220, 170, 20, 255}) // Bright gold
            .with_border({255, 210, 60, 255}, 2.0f)      // Lighter gold border
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.4f)
            .with_debug_name("now_default_badge"));

    div(context, mk(content.ent()),
        ComponentConfig{}
            .with_label("Text color automatically adjusts for best contrast "
                        "against any background")
            .with_size({percent(1.0f), pixels(40.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // Section 1: Default behavior (auto enabled by default)
    auto section1 = div(context, mk(content.ent()),
                        ComponentConfig{}
                            .with_size({percent(1.0f), children()})
                            .with_padding(Spacing::sm)
                            .with_flex_direction(FlexDirection::Column)
                            .with_debug_name("section1"));

    div(context, mk(section1.ent()),
        ComponentConfig{}
            .with_label("Default Behavior (no config needed)")
            .with_size({percent(1.0f), pixels(40.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_debug_name("section1_title"));

    // Row of buttons with various backgrounds - auto contrast just works
    auto row1 = div(context, mk(section1.ent()),
                    ComponentConfig{}
                        .with_size({percent(1.0f), pixels(50.0f)})
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::SpaceAround)
                        .with_align_items(AlignItems::Center)
                        .with_no_wrap()
                        .with_debug_name("row1"));

    // Dark backgrounds
    afterhours::Color dark_colors[] = {
        {20, 20, 30, 255}, // Near black
        {40, 60, 80, 255}, // Dark blue
        {60, 40, 60, 255}, // Dark purple
        {30, 50, 40, 255}, // Dark green
    };
    const char *dark_labels[] = {"Near Black", "Dark Blue", "Dark Purple",
                                 "Dark Green"};

    for (int i = 0; i < 4; i++) {
      button(context, mk(row1.ent(), i),
             ComponentConfig{}
                 .with_label(dark_labels[i])
                 .with_size({percent(0.24f), pixels(44.0f)})
                 .with_custom_background(dark_colors[i])
                 .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                 .with_debug_name("dark_btn_" + std::to_string(i)));
    }

    // Row of light backgrounds
    auto row2 = div(context, mk(section1.ent()),
                    ComponentConfig{}
                        .with_size({percent(1.0f), pixels(50.0f)})
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::SpaceAround)
                        .with_align_items(AlignItems::Center)
                        .with_no_wrap()
                        .with_debug_name("row2"));

    afterhours::Color light_colors[] = {
        {240, 240, 245, 255}, // Near white
        {200, 220, 255, 255}, // Light blue
        {255, 220, 240, 255}, // Light pink
        {220, 255, 220, 255}, // Light green
    };
    const char *light_labels[] = {"Near White", "Light Blue", "Light Pink",
                                  "Light Green"};

    for (int i = 0; i < 4; i++) {
      button(context, mk(row2.ent(), i),
             ComponentConfig{}
                 .with_label(light_labels[i])
                 .with_size({percent(0.24f), pixels(44.0f)})
                 .with_custom_background(light_colors[i])
                 .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                 .with_debug_name("light_btn_" + std::to_string(i)));
    }

    // Section 2: Mid-tone colors (edge cases)
    auto section2 = div(context, mk(content.ent()),
                        ComponentConfig{}
                            .with_size({percent(1.0f), children()})
                            .with_padding(Spacing::sm)
                            .with_flex_direction(FlexDirection::Column)
                            .with_debug_name("section2"));

    div(context, mk(section2.ent()),
        ComponentConfig{}
            .with_label("Mid-tone Colors (best contrast)")
            .with_size({percent(1.0f), pixels(40.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_debug_name("section2_title"));

    auto row3 = div(context, mk(section2.ent()),
                    ComponentConfig{}
                        .with_size({percent(1.0f), pixels(50.0f)})
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::SpaceAround)
                        .with_align_items(AlignItems::Center)
                        .with_no_wrap()
                        .with_debug_name("row3"));

    // Mid-tone colors where the choice isn't obvious
    afterhours::Color mid_colors[] = {
        {255, 180, 100, 255}, // Orange
        {100, 180, 180, 255}, // Teal
        {140, 60, 140, 255},  // Dark Magenta (better contrast)
        {180, 180, 100, 255}, // Olive
        {150, 150, 150, 255}, // Gray
        {200, 150, 100, 255}, // Tan
    };
    const char *mid_labels[] = {"Orange", "Teal", "Dk Magenta",
                                "Olive",  "Gray", "Tan"};

    for (int i = 0; i < 6; i++) {
      button(context, mk(row3.ent(), i),
             ComponentConfig{}
                 .with_label(mid_labels[i])
                 .with_size({percent(0.155f), pixels(44.0f)})
                 .with_custom_background(mid_colors[i])
                 .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                 .with_debug_name("mid_btn_" + std::to_string(i)));
    }

    // Section 3: How to disable auto text color
    auto section3 = div(context, mk(content.ent()),
                        ComponentConfig{}
                            .with_size({percent(1.0f), children()})
                            .with_padding(Spacing::sm)
                            .with_flex_direction(FlexDirection::Column)
                            .with_debug_name("section3"));

    div(context, mk(section3.ent()),
        ComponentConfig{}
            .with_label("Explicit Control Options")
            .with_size({percent(1.0f), pixels(40.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_debug_name("section3_title"));

    auto row4 = div(context, mk(section3.ent()),
                    ComponentConfig{}
                        .with_size({percent(1.0f), pixels(50.0f)})
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::SpaceAround)
                        .with_align_items(AlignItems::Center)
                        .with_no_wrap()
                        .with_debug_name("row4"));

    // Compare auto vs disabled vs explicit text color
    afterhours::Color light_bg = {230, 230, 230, 255};
    afterhours::Color dark_bg = {50, 50, 60, 255};

    button(context, mk(row4.ent(), 0),
           ComponentConfig{}
               .with_label("Auto (default)")
               .with_size({percent(0.31f), pixels(44.0f)})
               .with_custom_background(
                   light_bg) // Light bg -> auto picks dark text
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("auto_enabled"));

    button(context, mk(row4.ent(), 1),
           ComponentConfig{}
               .with_label("Disabled (theme font)")
               .with_size({percent(0.31f), pixels(44.0f)})
               .with_custom_background(
                   dark_bg) // Dark bg works with white theme font
               .with_auto_text_color(
                   false) // Explicitly disable - uses theme font color
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("auto_disabled"));

    button(context, mk(row4.ent(), 2),
           ComponentConfig{}
               .with_label("Custom Red Text")
               .with_size({percent(0.31f), pixels(44.0f)})
               .with_custom_background(light_bg)
               .with_custom_text_color(
                   {140, 30, 30, 255}) // Explicit dark red for contrast
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("explicit_color"));

    // Section 4: Theme colors with auto contrast
    auto section4 = div(context, mk(content.ent()),
                        ComponentConfig{}
                            .with_size({percent(1.0f), children()})
                            .with_padding(Spacing::sm)
                            .with_flex_direction(FlexDirection::Column)
                            .with_debug_name("section4"));

    div(context, mk(section4.ent()),
        ComponentConfig{}
            .with_label("Works with Theme Colors Too")
            .with_size({percent(1.0f), pixels(40.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_debug_name("section4_title"));

    auto row5 = div(context, mk(section4.ent()),
                    ComponentConfig{}
                        .with_size({percent(1.0f), pixels(50.0f)})
                        .with_flex_direction(FlexDirection::Row)
                        .with_justify_content(JustifyContent::SpaceAround)
                        .with_align_items(AlignItems::Center)
                        .with_no_wrap()
                        .with_debug_name("row5"));

    button(context, mk(row5.ent(), 0),
           ComponentConfig{}
               .with_label("Primary")
               .with_size({percent(0.185f), pixels(44.0f)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("theme_primary"));

    button(context, mk(row5.ent(), 1),
           ComponentConfig{}
               .with_label("Accent")
               .with_size({percent(0.185f), pixels(44.0f)})
               .with_background(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("theme_accent"));

    button(context, mk(row5.ent(), 2),
           ComponentConfig{}
               .with_label("Secondary")
               .with_size({percent(0.185f), pixels(44.0f)})
               .with_background(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("theme_secondary"));

    button(context, mk(row5.ent(), 3),
           ComponentConfig{}
               .with_label("Background")
               .with_size({percent(0.185f), pixels(44.0f)})
               .with_background(Theme::Usage::Background)
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("theme_background"));

    button(context, mk(row5.ent(), 4),
           ComponentConfig{}
               .with_label("Surface")
               .with_size({percent(0.185f), pixels(44.0f)})
               .with_background(Theme::Usage::Surface)
               .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
               .with_debug_name("theme_surface"));
  }
};

REGISTER_EXAMPLE_SCREEN(auto_text_color, "System Demos",
                        "Showcases the auto text color feature (now default)",
                        AutoTextColorShowcase)
