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

    // Page title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Text Stroke / Outline")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(50)})
            .with_absolute_position()
            .with_translate(20.0f, 20.0f)
            .with_font("Gaegu-Bold", 32.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Left)
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
    
    // Row 1: NO STROKE vs WITH STROKE (same yellow color)
    afterhours::Color yellow{255, 220, 80, 255};
    afterhours::Color dark_outline{40, 30, 10, 255};
    
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("NO STROKE")
            .with_size(ComponentSize{pixels(250), pixels(50)})
            .with_absolute_position()
            .with_translate(col1_x, 110.0f)
            .with_font("Fredoka", 40.0f)
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("no_stroke_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WITH STROKE")
            .with_size(ComponentSize{pixels(280), pixels(50)})
            .with_absolute_position()
            .with_translate(col1_x + 260.0f, 110.0f)
            .with_font("Fredoka", 40.0f)
            .with_custom_text_color(yellow)
            .with_text_stroke(dark_outline, 4.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("with_stroke_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("← Same yellow color, stroke adds depth")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 165.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_1"));

    // Row 2: EXTREME thick stroke (10px)
    afterhours::Color hot_pink{255, 50, 150, 255};
    afterhours::Color deep_purple{60, 0, 80, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("EXTREME")
            .with_size(ComponentSize{pixels(350), pixels(80)})
            .with_absolute_position()
            .with_translate(col1_x, 200.0f)
            .with_font("Fredoka", 64.0f)
            .with_custom_text_color(hot_pink)
            .with_text_stroke(deep_purple, 10.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("extreme"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("10px stroke - very chunky outline")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 285.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_2"));

    // Row 3: Contrasting stroke color (cyan text, red stroke)
    afterhours::Color cyan{80, 255, 255, 255};
    afterhours::Color red_stroke{200, 30, 30, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("CONTRAST")
            .with_size(ComponentSize{pixels(350), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x, 320.0f)
            .with_font("Fredoka", 56.0f)
            .with_custom_text_color(cyan)
            .with_text_stroke(red_stroke, 6.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("contrast"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Cyan + red stroke - complementary colors")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 395.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_3"));

    // Row 4: "Glow" effect - dark text with bright stroke
    afterhours::Color dark_text{30, 30, 50, 255};
    afterhours::Color glow_blue{100, 200, 255, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("GLOW")
            .with_size(ComponentSize{pixels(350), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x, 430.0f)
            .with_font("Fredoka", 56.0f)
            .with_custom_text_color(dark_text)
            .with_text_stroke(glow_blue, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("glow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Dark text + bright stroke = glow effect")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x, 505.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_4"));

    // Row 5: White on light background - stroke makes it readable
    afterhours::Color light_bg{200, 210, 220, 255};
    
    // Light background panel
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(580), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x, 540.0f)
            .with_custom_background(light_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("light_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(50)})
            .with_absolute_position()
            .with_translate(col1_x + 20.0f, 550.0f)
            .with_font("Fredoka", 40.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_no_stroke"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WHITE")
            .with_size(ComponentSize{pixels(180), pixels(50)})
            .with_absolute_position()
            .with_translate(col1_x + 220.0f, 550.0f)
            .with_font("Fredoka", 40.0f)
            .with_custom_text_color(text_white)
            .with_text_stroke(afterhours::Color{0, 0, 0, 255}, 3.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("white_with_stroke"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("← invisible        ← visible!")
            .with_size(ComponentSize{pixels(300), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x + 60.0f, 595.0f)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_custom_text_color(afterhours::Color{80, 80, 100, 255})
            .with_debug_name("desc_5"));

    // ========== RIGHT COLUMN: Thickness comparison ==========
    
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Stroke Thickness:")
            .with_size(ComponentSize{pixels(300), pixels(30)})
            .with_absolute_position()
            .with_translate(col2_x, 110.0f)
            .with_font("Gaegu-Bold", 22.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("thickness_title"));

    afterhours::Color orange{255, 160, 60, 255};
    afterhours::Color dark_orange{100, 50, 20, 255};
    
    float thickness_y = 150.0f;
    float thicknesses[] = {1.0f, 2.0f, 4.0f, 6.0f, 8.0f, 10.0f};
    const char* thickness_labels[] = {"1px", "2px", "4px", "6px", "8px", "10px"};
    
    for (int i = 0; i < 6; i++) {
        div(context, mk(entity, id++),
            ComponentConfig{}
                .with_label("STROKE")
                .with_size(ComponentSize{pixels(200), pixels(50)})
                .with_absolute_position()
                .with_translate(col2_x, thickness_y + i * 70.0f)
                .with_font("Fredoka", 36.0f)
                .with_custom_text_color(orange)
                .with_text_stroke(dark_orange, thicknesses[i])
                .with_alignment(TextAlignment::Left)
                .with_debug_name("thickness_" + std::to_string(i)));

        div(context, mk(entity, id++),
            ComponentConfig{}
                .with_label(thickness_labels[i])
                .with_size(ComponentSize{pixels(80), pixels(30)})
                .with_absolute_position()
                .with_translate(col2_x + 220.0f, thickness_y + i * 70.0f + 10.0f)
                .with_font(UIComponent::DEFAULT_FONT, 16.0f)
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
            .with_label("Usage: ComponentConfig{}.with_text_stroke(Color{r, g, b, a}, thickness)")
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

