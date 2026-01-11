#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleText : ScreenSystem<UIContext<InputAction>> {
  // Elegant dark theme with gold accents - like a luxury brand
  afterhours::Color bg_charcoal{28, 26, 24, 255};      // Rich charcoal
  afterhours::Color panel_dark{42, 38, 35, 255};       // Dark panel
  afterhours::Color gold_accent{205, 175, 110, 255};   // Warm gold
  afterhours::Color silver_text{195, 195, 200, 255};   // Silver text
  afterhours::Color cream_text{250, 245, 235, 255};    // Cream white
  afterhours::Color muted_text{130, 125, 120, 255};    // Muted brown-gray
  afterhours::Color divider{65, 58, 52, 255};          // Subtle divider

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = cream_text;
    theme.darkfont = bg_charcoal;
    theme.font_muted = muted_text;
    theme.background = bg_charcoal;
    theme.surface = panel_dark;
    theme.primary = gold_accent;
    theme.secondary = afterhours::Color{85, 95, 115, 255};
    theme.accent = afterhours::Color{145, 85, 85, 255};
    theme.error = afterhours::Color{180, 70, 70, 255};
    theme.roundness = 0.08f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Full background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_charcoal)
            .with_debug_name("bg"));

    // Main container panel
    float panel_w = 850.0f;
    float panel_h = 520.0f;
    float panel_x = (screen_width - panel_w) / 2.0f;
    float panel_y = (screen_height - panel_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_w), pixels(panel_h)})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_dark)
            .with_border(divider, 1.0f)
            .with_soft_shadow(6.0f, 8.0f, 20.0f, afterhours::Color{0, 0, 0, 60})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("main_panel"));

    // Header with gold accent line
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Typography Showcase")
            .with_size(ComponentSize{pixels(panel_w - 60), pixels(48)})
            .with_absolute_position()
            .with_translate(panel_x + 30.0f, panel_y + 25.0f)
            .with_font("EBGaramond", 36.0f)
            .with_custom_text_color(gold_accent)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Gold accent line under title
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(180), pixels(2)})
            .with_absolute_position()
            .with_translate(panel_x + (panel_w - 180) / 2.0f, panel_y + 78.0f)
            .with_custom_background(gold_accent)
            .with_debug_name("title_line"));

    // Typography examples in a refined layout
    float content_y = panel_y + 105.0f;
    float left_col = panel_x + 50.0f;
    float right_col = panel_x + panel_w / 2.0f + 25.0f;
    float col_w = (panel_w / 2.0f) - 75.0f;

    // Left column - Size variations
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("SIZE HIERARCHY")
            .with_size(ComponentSize{pixels(col_w), pixels(20)})
            .with_absolute_position()
            .with_translate(left_col, content_y)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_custom_text_color(muted_text)
            .with_debug_name("size_label"));

    // Display text - large
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Display Text")
            .with_size(ComponentSize{pixels(col_w), pixels(48)})
            .with_absolute_position()
            .with_translate(left_col, content_y + 30.0f)
            .with_font("EBGaramond", 42.0f)
            .with_custom_text_color(cream_text)
            .with_debug_name("display_text"));

    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Headline Style")
            .with_size(ComponentSize{pixels(col_w), pixels(36)})
            .with_absolute_position()
            .with_translate(left_col, content_y + 85.0f)
            .with_font("EBGaramond", 32.0f)
            .with_custom_text_color(silver_text)
            .with_debug_name("headline_text"));

    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("Subheading Text")
            .with_size(ComponentSize{pixels(col_w), pixels(30)})
            .with_absolute_position()
            .with_translate(left_col, content_y + 130.0f)
            .with_font("EBGaramond", 26.0f)
            .with_custom_text_color(silver_text)
            .with_debug_name("subheading_text"));

    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label("Body text for readable content that flows naturally across multiple lines and maintains excellent legibility.")
            .with_size(ComponentSize{pixels(col_w), pixels(65)})
            .with_absolute_position()
            .with_translate(left_col, content_y + 175.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(muted_text)
            .with_debug_name("body_text"));

    div(context, mk(entity, 15),
        ComponentConfig{}
            .with_label("Caption & metadata - smaller supporting text")
            .with_size(ComponentSize{pixels(col_w), pixels(22)})
            .with_absolute_position()
            .with_translate(left_col, content_y + 250.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(muted_text)
            .with_debug_name("caption_text"));

    // Divider line
    div(context, mk(entity, 16),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1), pixels(280)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w / 2.0f, content_y + 15.0f)
            .with_custom_background(divider)
            .with_debug_name("divider"));

    // Right column - Color variations
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("COLOR TREATMENTS")
            .with_size(ComponentSize{pixels(col_w), pixels(20)})
            .with_absolute_position()
            .with_translate(right_col, content_y)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_custom_text_color(muted_text)
            .with_debug_name("color_label"));

    struct ColorExample {
      std::string label;
      afterhours::Color color;
    };

    ColorExample examples[] = {
        {"Primary Gold Text", gold_accent},
        {"Light Cream Text", cream_text},
        {"Silver Accent Text", silver_text},
        {"Muted Secondary Text", muted_text},
        {"Accent Color Highlight", {165, 100, 100, 255}},
    };

    for (int i = 0; i < 5; i++) {
      // Background pill for each text sample
      div(context, mk(entity, 30 + i * 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(col_w), pixels(42)})
              .with_absolute_position()
              .with_translate(right_col, content_y + 30.0f + i * 52.0f)
              .with_custom_background(afterhours::colors::darken(panel_dark, 0.85f))
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.2f)
              .with_debug_name("color_bg_" + std::to_string(i)));

      div(context, mk(entity, 31 + i * 2),
          ComponentConfig{}
              .with_label(examples[i].label)
              .with_size(ComponentSize{pixels(col_w - 20), pixels(28)})
              .with_absolute_position()
              .with_translate(right_col + 12.0f, content_y + 38.0f + i * 52.0f)
              .with_font("EBGaramond", 22.0f)
              .with_custom_text_color(examples[i].color)
              .with_debug_name("color_text_" + std::to_string(i)));
    }

    // Footer
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Elegant typography creates visual hierarchy and guides the reader's eye")
            .with_size(ComponentSize{pixels(panel_w - 100), pixels(24)})
            .with_absolute_position()
            .with_translate(panel_x + 50.0f, panel_y + panel_h - 50.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(text, "System Demos",
                        "Text rendering examples with different sizes",
                        ExampleText)
