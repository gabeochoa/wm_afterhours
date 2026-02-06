#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleColors : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply midnight theme for this screen
    auto theme = afterhours::ui::theme_presets::midnight();
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("bg"));

    // Main panel dimensions - expanded to use more screen
    float panel_w = 900.0f;
    float panel_h = 520.0f;
    float panel_x = (screen_width - panel_w) / 2.0f;
    float panel_y = (screen_height - panel_h) / 2.0f;

    // Main panel
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_w), pixels(panel_h)})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_background(Theme::Usage::Surface)
            .with_soft_shadow(6.0f, 10.0f, 25.0f,
                              afterhours::Color{0, 0, 0, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("main_panel"));

    // Title
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Theme Color Swatches")
            .with_size(ComponentSize{pixels(panel_w), pixels(50)})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.08f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Row dimensions
    float row_w = panel_w - 60.0f;
    float row_h = 130.0f;
    float row_x = panel_x + 30.0f;
    float row1_y = panel_y + 80.0f;
    float row2_y = row1_y + row_h + 30.0f;

    // Row 1 label
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Theme Colors")
            .with_size(ComponentSize{pixels(row_w), pixels(28)})
            .with_absolute_position()
            .with_translate(row_x, row1_y - 32.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_auto_text_color(true)
            .with_debug_name("row1_label"));

    // Color row 1: Theme colors background
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(row_w), pixels(row_h)})
            .with_absolute_position()
            .with_translate(row_x, row1_y)
            .with_background(Theme::Usage::Background)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.04f)
            .with_debug_name("row1_bg"));

    // Theme color swatches - with increased gaps
    float swatch_w = 150.0f;
    float swatch_h = 100.0f;
    float swatch_gap = 18.0f;
    float swatches_start_x = row_x + 25.0f;
    float swatch_y = row1_y + 15.0f;

    struct ThemeSwatch {
      const char *label;
      Theme::Usage usage;
    };
    ThemeSwatch theme_swatches[] = {
        {"Primary", Theme::Usage::Primary},
        {"Secondary", Theme::Usage::Secondary},
        {"Accent", Theme::Usage::Accent},
        {"Surface", Theme::Usage::Surface},
        {"Error/Danger", Theme::Usage::Error},
    };

    for (int i = 0; i < 5; i++) {
      div(context, mk(entity, 20 + i),
          ComponentConfig{}
              .with_label(theme_swatches[i].label)
              .with_size(ComponentSize{pixels(swatch_w), pixels(swatch_h)})
              .with_absolute_position()
              .with_translate(swatches_start_x + i * (swatch_w + swatch_gap),
                              swatch_y)
              .with_background(theme_swatches[i].usage)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.10f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("swatch_" + std::to_string(i)));
    }

    // Row 2 label
    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_label("Custom Colors")
            .with_size(ComponentSize{pixels(row_w), pixels(28)})
            .with_absolute_position()
            .with_translate(row_x, row2_y - 32.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_auto_text_color(true)
            .with_debug_name("row2_label"));

    // Color row 2: Custom colors background
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(row_w), pixels(row_h)})
            .with_absolute_position()
            .with_translate(row_x, row2_y)
            .with_background(Theme::Usage::Background)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.04f)
            .with_debug_name("row2_bg"));

    // Custom color swatches with hex values
    struct CustomColorInfo {
      const char *name;
      afterhours::Color color;
      const char *hex;
    };
    CustomColorInfo custom_colors[] = {
        {"Red", afterhours::Color{255, 100, 100, 255}, "#FF6464"},
        {"Green", afterhours::Color{100, 200, 100, 255}, "#64C864"},
        {"Blue", afterhours::Color{100, 150, 255, 255}, "#6496FF"},
        {"Yellow", afterhours::Color{255, 200, 100, 255}, "#FFC864"},
        {"Purple", afterhours::Color{200, 100, 255, 255}, "#C864FF"},
    };
    float custom_swatch_y = row2_y + 15.0f;

    for (int i = 0; i < 5; i++) {
      std::string label = std::string(custom_colors[i].name) + "\n" + custom_colors[i].hex;
      div(context, mk(entity, 40 + i),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{pixels(swatch_w), pixels(swatch_h)})
              .with_absolute_position()
              .with_translate(swatches_start_x + i * (swatch_w + swatch_gap),
                              custom_swatch_y)
              .with_custom_background(custom_colors[i].color)
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.10f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("custom_" + std::to_string(i)));
    }

    // Info text at bottom
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Using Midnight theme. Top row: theme colors. Bottom "
                        "row: custom colors with hex values.")
            .with_size(ComponentSize{pixels(panel_w - 60), pixels(36)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + panel_h - 55.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_auto_text_color(true)
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("info"));
  }
};

REGISTER_EXAMPLE_SCREEN(colors, "System Demos", "Color system examples",
                        ExampleColors)
