#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include "../../theme_presets.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTextShadow : ScreenSystem<UIContext<InputAction>> {

  // Light theme so shadows are clearly visible
  afterhours::Color bg_light{200, 205, 215, 255};
  afterhours::Color surface{230, 235, 245, 255};
  afterhours::Color text_dark{30, 35, 45, 255};
  afterhours::Color text_muted{80, 85, 100, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_light)
            .with_debug_name("bg"));

    // Page title with prominent shadow
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Text Drop Shadow")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(55)})
            .with_absolute_position()
            .with_translate(20.0f, 15.0f)
            .with_font("BlackOpsOne", 36.0f)
            .with_custom_text_color(text_dark)
            .with_text_shadow(afterhours::Color{0, 0, 0, 255}, 6.0f,
                              6.0f) // Very visible
            .with_alignment(TextAlignment::Center)
            .with_debug_name("page_title"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label(
                "Add depth and improve legibility with text shadows")
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(28)})
            .with_absolute_position()
            .with_translate(20.0f, 65.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("page_subtitle"));

    int id = 10;
    float col1_x = 20.0f;
    float col2_x = screen_w * 0.52f;  // Right column starts at 52% of screen width

    // ========== LEFT COLUMN: Side-by-side comparisons ==========

    const char *bold_font = "BlackOpsOne";

    // Row 1: NO SHADOW vs WITH SHADOW - very prominent shadow
    afterhours::Color yellow{255, 220, 80, 255};
    afterhours::Color shadow_dark{0, 0, 0,
                                  255}; // Full opacity for max visibility

    float left_col_width = col2_x - col1_x - 20.0f;  // Available width for left column
    float half_left = left_col_width / 2.0f;

    // Configurable contrast background for light shadow examples
    afterhours::Color contrast_bg{50, 55, 70, 255};
    float contrast_panel_padding = 10.0f;

    // Dark panel behind yellow text for better shadow visibility
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(left_col_width), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x - contrast_panel_padding / 2.0f, 110.0f)
            .with_custom_background(contrast_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("yellow_panel_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("NO SHADOW")
            .with_size(ComponentSize{pixels(half_left - 10), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x, 110.0f)
            .with_font(bold_font, 32.0f)
            .with_custom_text_color(yellow)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("no_shadow_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("WITH SHADOW")
            .with_size(ComponentSize{pixels(half_left), pixels(55)})
            .with_absolute_position()
            .with_translate(col1_x + half_left, 110.0f)
            .with_font(bold_font, 32.0f)
            .with_custom_text_color(yellow)
            .with_text_shadow(shadow_dark, 6.0f,
                              6.0f) // Large offset for visibility
            .with_alignment(TextAlignment::Left)
            .with_debug_name("with_shadow_1"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Same color - shadow adds depth")
            .with_size(ComponentSize{pixels(left_col_width), pixels(24)})
            .with_absolute_position()
            .with_translate(col1_x, 165.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_1"));

    // Row 2: SOFT vs HARD shadow presets - larger offsets
    afterhours::Color hot_pink{255, 50, 150, 255};

    // Dark panel behind SOFT/HARD for better shadow visibility (especially soft shadows)
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(left_col_width), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x - contrast_panel_padding / 2.0f, 190.0f)
            .with_custom_background(contrast_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("soft_hard_panel_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("SOFT")
            .with_size(ComponentSize{pixels(half_left - 10), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x, 190.0f)
            .with_font(bold_font, 52.0f)
            .with_custom_text_color(hot_pink)
            .with_soft_text_shadow(5.0f, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("soft_shadow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("HARD")
            .with_size(ComponentSize{pixels(half_left), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x + half_left, 190.0f)
            .with_font(bold_font, 52.0f)
            .with_custom_text_color(hot_pink)
            .with_hard_text_shadow(5.0f, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("hard_shadow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Soft shadow creates a subtle glow effect")
            .with_size(ComponentSize{pixels(left_col_width), pixels(24)})
            .with_absolute_position()
            .with_translate(col1_x, 260.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_2"));

    // Row 3: Colored shadows - very visible red shadow
    afterhours::Color cyan{80, 255, 255, 255};
    afterhours::Color red_shadow{200, 0, 0, 255}; // Full opacity red

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("COLORED")
            .with_size(ComponentSize{pixels(left_col_width), pixels(65)})
            .with_absolute_position()
            .with_translate(col1_x, 285.0f)
            .with_font(bold_font, 48.0f)
            .with_custom_text_color(cyan)
            .with_text_shadow(red_shadow, 6.0f, 6.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("colored_shadow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Cyan text with red shadow for retro 3D look")
            .with_size(ComponentSize{pixels(left_col_width), pixels(24)})
            .with_absolute_position()
            .with_translate(col1_x, 350.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_3"));

    // Row 4: Combined with stroke
    afterhours::Color orange{255, 180, 60, 255};
    afterhours::Color dark_stroke{40, 20, 0, 255};
    afterhours::Color shadow_offset{0, 0, 0, 120};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("COMBO")
            .with_size(ComponentSize{pixels(left_col_width), pixels(65)})
            .with_absolute_position()
            .with_translate(col1_x, 375.0f)
            .with_font(bold_font, 48.0f)
            .with_custom_text_color(orange)
            .with_text_stroke(dark_stroke, 3.0f)
            .with_text_shadow(shadow_offset, 5.0f, 5.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("combo"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Combine stroke and shadow for maximum impact")
            .with_size(ComponentSize{pixels(left_col_width), pixels(24)})
            .with_absolute_position()
            .with_translate(col1_x, 440.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("desc_4"));

    // Row 5: Dark background with light text - shadow adds depth
    afterhours::Color dark_panel{35, 40, 55, 255};
    afterhours::Color light_text{255, 255, 255, 255};

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(left_col_width), pixels(70)})
            .with_absolute_position()
            .with_translate(col1_x, 465.0f)
            .with_custom_background(dark_panel)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_debug_name("dark_panel"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("LIGHT")
            .with_size(ComponentSize{pixels(half_left - 30), pixels(45)})
            .with_absolute_position()
            .with_translate(col1_x + 15.0f, 475.0f)
            .with_font(bold_font, 34.0f)
            .with_custom_text_color(light_text)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("light_no_shadow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("LIGHT")
            .with_size(ComponentSize{pixels(half_left - 30), pixels(45)})
            .with_absolute_position()
            .with_translate(col1_x + half_left + 15.0f, 475.0f)
            .with_font(bold_font, 34.0f)
            .with_custom_text_color(light_text)
            .with_text_shadow(afterhours::Color{0, 0, 0, 180}, 3.0f, 3.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("light_with_shadow"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("flat look          more depth!")
            .with_size(ComponentSize{pixels(left_col_width - 30), pixels(20)})
            .with_absolute_position()
            .with_translate(col1_x + 25.0f, 520.0f)
            .with_font(UIComponent::DEFAULT_FONT, 11.0f)
            .with_custom_text_color(afterhours::Color{140, 145, 170, 255})
            .with_debug_name("desc_5"));

    // ========== RIGHT COLUMN: Offset comparison ==========

    float right_col_width = screen_w - col2_x - 20.0f;  // Available width for right column

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Shadow Offset Examples:")
            .with_size(ComponentSize{pixels(right_col_width), pixels(30)})
            .with_absolute_position()
            .with_translate(col2_x, 110.0f)
            .with_font("Gaegu-Bold", 22.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("offset_title"));

    afterhours::Color purple{180, 100, 255, 255};
    afterhours::Color purple_shadow{60, 20, 100, 200};

    float offset_y = 140.0f;
    float offset_spacing = 62.0f;  // Reduced spacing to fit 6 items
    float offsets[] = {1.0f, 2.0f, 4.0f, 6.0f, 8.0f, 10.0f};
    const char *offset_labels[] = {"1px", "2px", "4px", "6px", "8px", "10px"};

    // Dark panel behind offset examples for better visibility of subtle shadows
    float offset_panel_height = 6 * offset_spacing;
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(right_col_width), pixels(offset_panel_height)})
            .with_absolute_position()
            .with_translate(col2_x - contrast_panel_padding / 2.0f, offset_y)
            .with_custom_background(contrast_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.1f)
            .with_debug_name("offset_panel_bg"));

    for (int i = 0; i < 6; i++) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label("SHADOW")
              .with_size(ComponentSize{pixels(right_col_width - 60), pixels(48)})
              .with_absolute_position()
              .with_translate(col2_x, offset_y + i * offset_spacing)
              .with_font(bold_font, 34.0f)
              .with_custom_text_color(purple)
              .with_text_shadow(purple_shadow, offsets[i], offsets[i])
              .with_alignment(TextAlignment::Left)
              .with_debug_name("offset_" + std::to_string(i)));

      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_label(offset_labels[i])
              .with_size(ComponentSize{pixels(50), pixels(26)})
              .with_absolute_position()
              .with_translate(col2_x + right_col_width - 55, offset_y + i * offset_spacing + 10.0f)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_custom_text_color(afterhours::Color{180, 185, 200, 255})  // Lighter text for dark bg
              .with_debug_name("offset_label_" + std::to_string(i)));
    }

    // ========== Code example at bottom ==========
    float code_y = screen_h - 55.0f;

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w - 40), pixels(44)})
            .with_absolute_position()
            .with_translate(20.0f, code_y)
            .with_custom_background(surface)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_debug_name("code_bg"));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_label("Tip: Use small offsets (1-4px) for subtle depth, larger offsets (6-10px) for bold effects")
            .with_size(ComponentSize{pixels(screen_w - 60), pixels(28)})
            .with_absolute_position()
            .with_translate(30.0f, code_y + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(afterhours::Color{150, 220, 150, 255})
            .with_alignment(TextAlignment::Left)
            .with_debug_name("code_text"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_shadow, "System Demos",
                        "Demonstrates native text drop shadow rendering",
                        ExampleTextShadow)
