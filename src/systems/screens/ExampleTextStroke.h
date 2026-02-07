#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include "../../theme_presets.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTextStroke : ScreenSystem<UIContext<InputAction>> {

  // Dark theme for better stroke visibility
  afterhours::Color bg_dark{25, 28, 38, 255};
  afterhours::Color surface{40, 45, 60, 255};
  afterhours::Color text_white{255, 255, 255, 255};
  afterhours::Color text_muted{140, 145, 160, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = surface;
    theme.primary = afterhours::Color{80, 160, 255, 255};
    theme.roundness = 0.1f;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Page title with stroke to demonstrate the feature
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Text Stroke / Outline")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(55)})
            .with_absolute_position()
            .with_translate(20.0f, 15.0f)
            .with_font("BlackOpsOne", h720(36.0f))
            .with_custom_text_color(text_white)
            .with_text_stroke(afterhours::Color{0, 0, 0, 255}, 3.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("page_title"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Create bold outlines for game titles and headers")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(28)})
            .with_absolute_position()
            .with_translate(20.0f, 65.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("page_subtitle"));

    int id = 10;
    float margin = 80.0f;  // Side margins
    float col_gap = 60.0f; // Gap between columns
    float col1_x = margin;
    float col1_width = (screen_w - 2 * margin - col_gap) * 0.55f; // Left column: 55% of content area
    float col2_x = margin + col1_width + col_gap;

    // ========== LEFT COLUMN: Side-by-side comparisons ==========

    // Using BlackOpsOne for bold, blocky text that shows stroke clearly
    const char *bold_font = "BlackOpsOne";

    // Consistent vertical spacing for left column rows
    float content_start = 110.0f;
    float desc_gap = 4.0f;       // Gap between styled text and its description
    float row_gap = 10.0f;       // Gap between description and next row
    float desc_h = 24.0f;

    // Row 1: NO STROKE vs WITH STROKE (same yellow color)
    afterhours::Color yellow{255, 220, 80, 255};
    afterhours::Color dark_outline{20, 15, 0, 255}; // Darker for more contrast

    float row1_y = content_start;
    float row1_text_h = 50.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("NO STROKE")
            .with_size(ComponentSize{pixels(280), pixels(row1_text_h)})
            .with_absolute_position()
            .with_translate(col1_x, row1_y)
            .with_font(bold_font, h720(36.0f))
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("no_stroke_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WITH STROKE")
            .with_size(ComponentSize{pixels(320), pixels(row1_text_h)})
            .with_absolute_position()
            .with_translate(col1_x + 290.0f, row1_y)
            .with_font(bold_font, h720(36.0f))
            .with_custom_text_color(yellow)
            .with_text_stroke(dark_outline, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("with_stroke_1"));

    float desc1_y = row1_y + row1_text_h + desc_gap;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Same yellow color with dark outline for contrast")
            .with_size(ComponentSize{pixels(col1_width), pixels(desc_h)})
            .with_absolute_position()
            .with_translate(col1_x, desc1_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_1"));

    // Row 2: BOLD thick stroke (8px - more legible than 12px)
    afterhours::Color hot_pink{255, 50, 150, 255};
    afterhours::Color deep_purple{40, 0, 60, 255}; // Even darker

    float row2_y = desc1_y + desc_h + row_gap;
    float row2_text_h = 85.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("BOLD")
            .with_size(ComponentSize{pixels(col1_width), pixels(row2_text_h)})
            .with_absolute_position()
            .with_translate(col1_x, row2_y)
            .with_font(bold_font, h720(72.0f))
            .with_custom_text_color(hot_pink)
            .with_text_stroke(deep_purple, 8.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("bold"));

    float desc2_y = row2_y + row2_text_h + desc_gap;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Thick 8px stroke creates a chunky game-style effect")
            .with_size(ComponentSize{pixels(col1_width), pixels(desc_h)})
            .with_absolute_position()
            .with_translate(col1_x, desc2_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_2"));

    // Row 3: Contrasting stroke color (cyan text, red stroke)
    afterhours::Color cyan{80, 255, 255, 255};
    afterhours::Color red_stroke{180, 20, 20, 255};

    float row3_y = desc2_y + desc_h + row_gap;
    float row3_text_h = 70.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("CONTRAST")
            .with_size(ComponentSize{pixels(col1_width), pixels(row3_text_h)})
            .with_absolute_position()
            .with_translate(col1_x, row3_y)
            .with_font(bold_font, h720(56.0f))
            .with_custom_text_color(cyan)
            .with_text_stroke(red_stroke, 8.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("contrast"));

    float desc3_y = row3_y + row3_text_h + desc_gap;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Cyan text with red outline for a vibrant look")
            .with_size(ComponentSize{pixels(col1_width), pixels(desc_h)})
            .with_absolute_position()
            .with_translate(col1_x, desc3_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_3"));

    // Row 4: "Glow" effect - dark text with bright stroke
    afterhours::Color dark_text{20, 20, 40, 255};
    afterhours::Color glow_cyan{80, 220, 255, 255};

    float row4_y = desc3_y + desc_h + row_gap;
    float row4_text_h = 70.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("GLOW")
            .with_size(ComponentSize{pixels(col1_width), pixels(row4_text_h)})
            .with_absolute_position()
            .with_translate(col1_x, row4_y)
            .with_font(bold_font, h720(56.0f))
            .with_custom_text_color(dark_text)
            .with_text_stroke(glow_cyan, 8.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("glow"));

    float desc4_y = row4_y + row4_text_h + desc_gap;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Dark text with bright outline creates a glow effect")
            .with_size(ComponentSize{pixels(col1_width), pixels(desc_h)})
            .with_absolute_position()
            .with_translate(col1_x, desc4_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_4"));

    // Row 5: White on light background - stroke makes it readable
    afterhours::Color light_bg{220, 225, 235, 255};
    float light_panel_width = col1_width; // Width matches left column

    float row5_y = desc4_y + desc_h + row_gap;
    float row5_panel_h = 80.0f;

    // Light background panel
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(light_panel_width), pixels(row5_panel_h)})
            .with_absolute_position()
            .with_translate(col1_x, row5_y)
            .with_custom_background(light_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("light_bg"));

    float white_text_y = row5_y + 10.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + 30.0f, white_text_y)
            .with_font(bold_font, h720(40.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_no_stroke"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + light_panel_width * 0.5f, white_text_y)
            .with_font(bold_font, h720(40.0f))
            .with_custom_text_color(text_white)
            .with_text_stroke(afterhours::Color{0, 0, 0, 255}, 4.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_with_stroke"));

    float desc5_y = row5_y + row5_panel_h + desc_gap;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("invisible")
            .with_size(ComponentSize{pixels(100), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x + 50.0f, desc5_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(afterhours::Color{60, 60, 80, 255})
            .with_debug_name("desc_5a"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("visible!")
            .with_size(ComponentSize{pixels(100), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x + light_panel_width * 0.5f + 20.0f, desc5_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(afterhours::Color{60, 60, 80, 255})
            .with_debug_name("desc_5b"));

    // ========== RIGHT COLUMN: Thickness comparison ==========

    float right_col_width = screen_w - col2_x - margin;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Outline Thickness:")
            .with_size(ComponentSize{pixels(right_col_width), pixels(34)})
            .with_absolute_position()
            .with_translate(col2_x, content_start)
            .with_font("Gaegu-Bold", h720(26.0f))
            .with_custom_text_color(text_white)
            .with_debug_name("thickness_title"));

    afterhours::Color orange{255, 180, 60, 255};
    afterhours::Color dark_orange{80, 40, 0, 255}; // Darker for more contrast

    float thickness_y = content_start + 50.0f;
    float thicknesses[] = {2.0f, 4.0f, 6.0f, 8.0f, 10.0f};
    const char *thickness_labels[] = {"2px - subtle", "4px - medium",  "6px - bold",
                                      "8px - chunky", "10px - heavy"};

    for (int i = 0; i < 5; i++) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label("STROKE")
              .with_size(ComponentSize{pixels(220), pixels(60)})
              .with_absolute_position()
              .with_translate(col2_x, thickness_y + i * 85.0f)
              .with_font(bold_font, h720(40.0f))
              .with_custom_text_color(orange)
              .with_text_stroke(dark_orange, thicknesses[i])
              .with_alignment(TextAlignment::Left)
              .with_debug_name("thickness_" + std::to_string(i)));

      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label(thickness_labels[i])
              .with_size(ComponentSize{pixels(160), pixels(34)})
              .with_absolute_position()
              .with_translate(col2_x + 230.0f, thickness_y + i * 85.0f + 14.0f)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("thickness_label_" + std::to_string(i)));
    }

    // ========== Tip bar at bottom ==========
    float code_y = screen_h - 60.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w - 2 * margin), pixels(44)})
            .with_absolute_position()
            .with_translate(margin, code_y)
            .with_custom_background(surface)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("code_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Tip: Use 2-6px for readable text, 6-10px for decorative titles and headers")
            .with_size(ComponentSize{pixels(screen_w - 2 * margin - 20), pixels(28)})
            .with_absolute_position()
            .with_translate(margin + 10.0f, code_y + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(afterhours::Color{150, 220, 150, 255})
            .with_alignment(TextAlignment::Left)
            .with_debug_name("code_text"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_stroke, "System Demos",
                        "Demonstrates native text stroke/outline rendering",
                        ExampleTextStroke)
