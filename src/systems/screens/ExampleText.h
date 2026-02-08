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
  afterhours::Color bg_charcoal{28, 26, 24, 255};    // Rich charcoal
  afterhours::Color panel_dark{42, 38, 35, 255};     // Dark panel
  afterhours::Color gold_accent{205, 175, 110, 255}; // Warm gold
  afterhours::Color silver_text{195, 195, 200, 255}; // Silver text
  afterhours::Color cream_text{250, 245, 235, 255};  // Cream white
  afterhours::Color muted_text{180, 175, 170, 255};  // Muted brown-gray (brightened further for clarity)
  afterhours::Color divider{65, 58, 52, 255};        // Subtle divider

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

    auto *res = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_width = res ? res->current_resolution.width : 1280;
    int screen_height = res ? res->current_resolution.height : 720;

    // Full background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_charcoal)
            .with_debug_name("bg"));

    // Main container panel - sized proportionally to screen
    float panel_w = screen_width * 0.85f;  // 85% of screen width
    float panel_h = screen_height * 0.80f; // 80% of screen height
    float panel_x = (screen_width - panel_w) / 2.0f;
    float panel_y = (screen_height - panel_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(panel_w), pixels(panel_h)})
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_dark)
            .with_soft_shadow(6.0f, 8.0f, 20.0f, afterhours::Color{0, 0, 0, 60})
            .with_rounded_corners(RoundedCorners())
            .with_debug_name("main_panel"));

    // Header with gold accent line
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Typography Showcase")
            .with_size(ComponentSize{pixels(panel_w - 60), pixels(48)})
            .with_absolute_position(panel_x + 30.0f, panel_y + 25.0f)
            .with_font("Garamond", h720(36.0f))
            .with_custom_text_color(gold_accent)
            .with_alignment(TextAlignment::Center));

    // Gold accent line under title
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(180), pixels(2)})
            .with_absolute_position(panel_x + (panel_w - 180) / 2.0f, panel_y + 78.0f)
            .with_custom_background(gold_accent));

    // Typography examples in a refined layout
    float content_y = panel_y + 110.0f;
    float left_col = panel_x + 50.0f;
    float right_col = panel_x + panel_w / 2.0f + 30.0f;
    float col_w = (panel_w / 2.0f) - 80.0f;
    float content_height = panel_h - 180.0f; // Available height for content

    // Left column - Size variations
    float left_spacing = content_height / 6.0f; // Divide content area into sections

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Text Size Hierarchy")
            .with_size(ComponentSize{pixels(col_w), pixels(32)})
            .with_absolute_position(left_col, content_y)
            .with_font("Garamond", h720(22.0f))
            .with_custom_text_color(gold_accent));

    // Display text - large
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Display Text")
            .with_size(ComponentSize{pixels(col_w), pixels(56)})
            .with_absolute_position(left_col, content_y + left_spacing * 0.5f)
            .with_font("Garamond", h720(48.0f))
            .with_custom_text_color(cream_text));

    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Headline Style")
            .with_size(ComponentSize{pixels(col_w), pixels(44)})
            .with_absolute_position(left_col, content_y + left_spacing * 1.3f)
            .with_font("Garamond", h720(36.0f))
            .with_custom_text_color(silver_text));

    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("Subheading Text")
            .with_size(ComponentSize{pixels(col_w), pixels(36)})
            .with_absolute_position(left_col, content_y + left_spacing * 2.0f)
            .with_font("Garamond", h720(28.0f))
            .with_custom_text_color(silver_text));

    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label(
                "Body text for readable content that flows naturally "
                "and maintains excellent legibility.")
            .with_size(ComponentSize{pixels(col_w), pixels(80)})
            .with_absolute_position(left_col, content_y + left_spacing * 2.7f)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_md())
            .with_custom_text_color(muted_text));

    div(context, mk(entity, 15),
        ComponentConfig{}
            .with_label("Caption & metadata - smaller supporting text")
            .with_size(ComponentSize{pixels(col_w), pixels(28)})
            .with_absolute_position(left_col, content_y + left_spacing * 4.0f)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
            .with_custom_text_color(muted_text));

    // Divider line
    div(context, mk(entity, 16),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1), pixels(content_height)})
            .with_absolute_position(panel_x + panel_w / 2.0f, content_y)
            .with_custom_background(divider));

    // Right column - Color variations
    float right_item_spacing = (content_height - 30.0f) / 5.0f; // Space for 5 items plus header

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Color Treatments")
            .with_size(ComponentSize{pixels(col_w), pixels(32)})
            .with_absolute_position(right_col, content_y)
            .with_font("Garamond", h720(22.0f))
            .with_custom_text_color(gold_accent));

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
      float item_y = content_y + 35.0f + i * right_item_spacing;
      float pill_height = std::min(right_item_spacing - 10.0f, 50.0f);

      // Background pill for each text sample
      div(context, mk(entity, 30 + i * 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(col_w), pixels(pill_height)})
              .with_absolute_position(right_col, item_y)
              .with_custom_background(
                  afterhours::colors::darken(panel_dark, 0.85f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_debug_name("color_bg_" + std::to_string(i)));

      div(context, mk(entity, 31 + i * 2),
          ComponentConfig{}
              .with_label(examples[i].label)
              .with_size(ComponentSize{pixels(col_w - 24), pixels(pill_height - 8)})
              .with_absolute_position(right_col + 12.0f, item_y + 4.0f)
              .with_font("Garamond", h720(24.0f))
              .with_custom_text_color(examples[i].color)
              .with_debug_name("color_text_" + std::to_string(i)));
    }

    // Footer
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Elegant typography creates visual hierarchy and "
                        "guides the reader's eye")
            .with_size(ComponentSize{pixels(panel_w - 100), pixels(30)})
            .with_absolute_position(panel_x + 50.0f, panel_y + panel_h - 45.0f)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_md())
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(text, "System Demos",
                        "Text rendering examples with different sizes",
                        ExampleText)
