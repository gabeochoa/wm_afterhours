#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

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
            .with_font("BlackOpsOne", 36.0f)
            .with_custom_text_color(text_white)
            .with_text_stroke(afterhours::Color{0, 0, 0, 255}, 3.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("page_title"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Native with_text_stroke() API for game UI titles")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(24)})
            .with_absolute_position()
            .with_translate(20.0f, 65.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("page_subtitle"));

    int id = 10;
    float col1_x = 40.0f;
    float col2_x = 660.0f;

    // ========== LEFT COLUMN: Side-by-side comparisons ==========

    // Using BlackOpsOne for bold, blocky text that shows stroke clearly
    const char *bold_font = "BlackOpsOne";

    // Row 1: NO STROKE vs WITH STROKE (same yellow color)
    afterhours::Color yellow{255, 220, 80, 255};
    afterhours::Color dark_outline{20, 15, 0, 255}; // Darker for more contrast

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("NO STROKE")
            .with_size(ComponentSize{pixels(280), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x, 110.0f)
            .with_font(bold_font, 36.0f)
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("no_stroke_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WITH STROKE")
            .with_size(ComponentSize{pixels(320), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + 290.0f, 110.0f)
            .with_font(bold_font, 36.0f)
            .with_custom_text_color(yellow)
            .with_text_stroke(dark_outline, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("with_stroke_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Same yellow color - stroke adds dark outline")
            .with_size(ComponentSize{pixels(450), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 170.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_1"));

    // Row 2: EXTREME thick stroke (12px)
    afterhours::Color hot_pink{255, 50, 150, 255};
    afterhours::Color deep_purple{40, 0, 60, 255}; // Even darker

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("EXTREME")
            .with_size(ComponentSize{pixels(400), pixels(90)})
            .with_absolute_position()
            .with_translate(col1_x, 200.0f)
            .with_font(bold_font, 72.0f)
            .with_custom_text_color(hot_pink)
            .with_text_stroke(deep_purple, 12.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("extreme"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("12px stroke - very chunky outline")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 295.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_2"));

    // Row 3: Contrasting stroke color (cyan text, red stroke)
    afterhours::Color cyan{80, 255, 255, 255};
    afterhours::Color red_stroke{180, 20, 20, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("CONTRAST")
            .with_size(ComponentSize{pixels(400), pixels(75)})
            .with_absolute_position()
            .with_translate(col1_x, 330.0f)
            .with_font(bold_font, 56.0f)
            .with_custom_text_color(cyan)
            .with_text_stroke(red_stroke, 8.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("contrast"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Cyan + red stroke - complementary colors")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 410.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_3"));

    // Row 4: "Glow" effect - dark text with bright stroke
    afterhours::Color dark_text{20, 20, 40, 255};
    afterhours::Color glow_cyan{80, 220, 255, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("GLOW")
            .with_size(ComponentSize{pixels(350), pixels(75)})
            .with_absolute_position()
            .with_translate(col1_x, 445.0f)
            .with_font(bold_font, 56.0f)
            .with_custom_text_color(dark_text)
            .with_text_stroke(glow_cyan, 8.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("glow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Dark text + bright stroke = glow effect")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 525.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_4"));

    // Row 5: White on light background - stroke makes it readable
    afterhours::Color light_bg{220, 225, 235, 255};

    // Light background panel
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(580), pixels(80)})
            .with_absolute_position()
            .with_translate(col1_x, 555.0f)
            .with_custom_background(light_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("light_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + 30.0f, 565.0f)
            .with_font(bold_font, 40.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_no_stroke"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + 280.0f, 565.0f)
            .with_font(bold_font, 40.0f)
            .with_custom_text_color(text_white)
            .with_text_stroke(afterhours::Color{0, 0, 0, 255}, 4.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_with_stroke"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("invisible           visible!")
            .with_size(ComponentSize{pixels(300), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x + 60.0f, 618.0f)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_custom_text_color(afterhours::Color{60, 60, 80, 255})
            .with_debug_name("desc_5"));

    // ========== RIGHT COLUMN: Thickness comparison ==========

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Stroke Thickness:")
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_absolute_position()
            .with_translate(col2_x, 110.0f)
            .with_font("Gaegu-Bold", 24.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("thickness_title"));

    afterhours::Color orange{255, 180, 60, 255};
    afterhours::Color dark_orange{80, 40, 0, 255}; // Darker for more contrast

    float thickness_y = 155.0f;
    float thicknesses[] = {2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 14.0f};
    const char *thickness_labels[] = {"2px", "4px",  "6px",
                                      "8px", "10px", "14px"};

    for (int i = 0; i < 6; i++) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label("STROKE")
              .with_size(ComponentSize{pixels(220), pixels(55)})
              .with_absolute_position()
              .with_translate(col2_x, thickness_y + i * 75.0f)
              .with_font(bold_font, 40.0f)
              .with_custom_text_color(orange)
              .with_text_stroke(dark_orange, thicknesses[i])
              .with_alignment(TextAlignment::Left)
              .with_debug_name("thickness_" + std::to_string(i)));

      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label(thickness_labels[i])
              .with_size(ComponentSize{pixels(80), pixels(30)})
              .with_absolute_position()
              .with_translate(col2_x + 240.0f, thickness_y + i * 75.0f + 12.0f)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_custom_text_color(text_muted)
              .with_debug_name("thickness_label_" + std::to_string(i)));
    }

    // ========== Code example at bottom ==========
    float code_y = screen_h - 60.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w - 80), pixels(40)})
            .with_absolute_position()
            .with_translate(40.0f, code_y)
            .with_custom_background(surface)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("code_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Usage: ComponentConfig{}.with_text_stroke(Color{r, g, "
                        "b, a}, thickness)")
            .with_size(ComponentSize{pixels(screen_w - 100), pixels(24)})
            .with_absolute_position()
            .with_translate(50.0f, code_y + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(afterhours::Color{150, 220, 150, 255})
            .with_alignment(TextAlignment::Left)
            .with_debug_name("code_text"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_stroke, "System Demos",
                        "Demonstrates native text stroke/outline rendering",
                        ExampleTextStroke)
