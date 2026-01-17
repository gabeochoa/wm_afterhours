#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// This screen demonstrates the text overflow debug indicator feature.
// When AFTERHOURS_DEBUG_TEXT_OVERFLOW is enabled, containers that are too
// small for their text content will show a red corner indicator.
struct ExampleTextOverflow : ScreenSystem<UIContext<InputAction>> {

  // Color palette
  afterhours::Color bg_dark{30, 32, 40, 255};
  afterhours::Color card_bg{45, 48, 58, 255};
  afterhours::Color text_light{240, 240, 245, 255};
  afterhours::Color text_muted{140, 145, 160, 255};
  afterhours::Color success_green{80, 200, 120, 255};
  afterhours::Color warning_orange{255, 180, 80, 255};
  afterhours::Color error_red{255, 90, 90, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = card_bg;
    theme.primary = success_green;
    theme.secondary = warning_orange;
    theme.accent = afterhours::Color{100, 150, 255, 255};
    theme.error = error_red;
    theme.roundness = 0.10f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Text Overflow Debug Demo")
            .with_size(ComponentSize{pixels(screen_width - 40), pixels(50)})
            .with_absolute_position()
            .with_translate(20.0f, 20.0f)
            .with_font("Gaegu-Bold", 32.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Instructions
    std::string instructions =
#ifdef AFTERHOURS_DEBUG_TEXT_OVERFLOW
        "Debug mode ENABLED - red corners show text overflow";
#else
        "Debug mode DISABLED - rebuild with -DAFTERHOURS_DEBUG_TEXT_OVERFLOW";
#endif

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label(instructions)
            .with_size(ComponentSize{pixels(screen_width - 40), pixels(30)})
            .with_absolute_position()
            .with_translate(20.0f, 75.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(
#ifdef AFTERHOURS_DEBUG_TEXT_OVERFLOW
                success_green
#else
                warning_orange
#endif
                )
            .with_alignment(TextAlignment::Center)
            .with_debug_name("instructions"));

    float card_y = 130.0f;
    float card_spacing = 20.0f;
    float card_height = 100.0f;
    float card_width = 380.0f;
    float left_col_x = (screen_width / 2.0f) - card_width - 10.0f;
    float right_col_x = (screen_width / 2.0f) + 10.0f;

    // === WORKING EXAMPLES (Left Column) ===
    // Section header
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Working (text fits)")
            .with_size(ComponentSize{pixels(card_width), pixels(25)})
            .with_absolute_position()
            .with_translate(left_col_x, card_y)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(success_green)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("section_working"));

    card_y += 35.0f;

    // Card 1: Normal button (plenty of space)
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(left_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(success_green, 2.0f)
            .with_debug_name("card_ok_1"));

    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("This text fits perfectly")
            .with_size(ComponentSize{pixels(card_width - 30),
                                     pixels(card_height - 20)})
            .with_absolute_position()
            .with_translate(left_col_x + 15.0f, card_y + 10.0f)
            .with_font("Gaegu-Bold", 24.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_ok_1"));

    card_y += card_height + card_spacing;

    // Card 2: Short text, medium box
    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(60)})
            .with_absolute_position()
            .with_translate(left_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(success_green, 2.0f)
            .with_debug_name("card_ok_2"));

    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label("Short text")
            .with_size(ComponentSize{pixels(card_width - 30), pixels(50)})
            .with_absolute_position()
            .with_translate(left_col_x + 15.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_ok_2"));

    card_y += 60 + card_spacing;

    // Card 3: Icon-sized box with single char
    div(context, mk(entity, 15),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(50), pixels(50)})
            .with_absolute_position()
            .with_translate(left_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(success_green, 2.0f)
            .with_debug_name("card_ok_3"));

    div(context, mk(entity, 16),
        ComponentConfig{}
            .with_label("OK")
            .with_size(ComponentSize{pixels(40), pixels(40)})
            .with_absolute_position()
            .with_translate(left_col_x + 5.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_ok_3"));

    // Reset for right column
    card_y = 130.0f;

    // === OVERFLOW EXAMPLES (Right Column) ===
    // Section header
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Overflow (text doesn't fit)")
            .with_size(ComponentSize{pixels(card_width), pixels(25)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(error_red)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("section_overflow"));

    card_y += 35.0f;

    // Card 1: Way too much text for a small box
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(150), pixels(30)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(error_red, 2.0f)
            .with_debug_name("card_overflow_1"));

    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("This is way too much text for this tiny container!")
            .with_size(ComponentSize{pixels(140), pixels(20)})
            .with_absolute_position()
            .with_translate(right_col_x + 5.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_overflow_1"));

    card_y += 30 + card_spacing;

    // Card 2: Tall text in flat container
    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(15)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(error_red, 2.0f)
            .with_debug_name("card_overflow_2"));

    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_label("Height is too small!")
            .with_size(ComponentSize{pixels(card_width - 20), pixels(5)})
            .with_absolute_position()
            .with_translate(right_col_x + 10.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_overflow_2"));

    card_y += 15 + card_spacing;

    // Card 3: Extremely tiny container
    div(context, mk(entity, 25),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(error_red, 2.0f)
            .with_debug_name("card_overflow_3"));

    div(context, mk(entity, 26),
        ComponentConfig{}
            .with_label("No way this fits")
            .with_size(ComponentSize{pixels(10), pixels(10)})
            .with_absolute_position()
            .with_translate(right_col_x + 5.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_overflow_3"));

    // Label for the tiny box
    div(context, mk(entity, 27),
        ComponentConfig{}
            .with_label("<-- Tiny 20x20 box")
            .with_size(ComponentSize{pixels(200), pixels(20)})
            .with_absolute_position()
            .with_translate(right_col_x + 30.0f, card_y)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("tiny_label"));

    card_y += 30 + card_spacing;

    // Card 4: Zero-margin nightmare
    div(context, mk(entity, 28),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(100), pixels(25)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_custom_background(card_bg)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_border(error_red, 2.0f)
            .with_debug_name("card_overflow_4"));

    div(context, mk(entity, 29),
        ComponentConfig{}
            .with_label("Long text in narrow space")
            .with_size(ComponentSize{pixels(90), pixels(15)})
            .with_absolute_position()
            .with_translate(right_col_x + 5.0f, card_y + 5.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("text_overflow_4"));

    card_y += 25 + card_spacing;

    // Row of progressively smaller boxes
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Shrinking boxes with same text:")
            .with_size(ComponentSize{pixels(300), pixels(20)})
            .with_absolute_position()
            .with_translate(right_col_x, card_y)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("shrinking_label"));

    card_y += 25.0f;

    float box_sizes[] = {120.0f, 80.0f, 50.0f, 30.0f, 15.0f};
    float box_x = right_col_x;
    for (int i = 0; i < 5; i++) {
      float size = box_sizes[i];
      div(context, mk(entity, 40 + i * 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(size), pixels(size)})
              .with_absolute_position()
              .with_translate(box_x, card_y)
              .with_custom_background(card_bg)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_border(i < 2 ? success_green : error_red, 2.0f)
              .with_debug_name("shrink_box_" + std::to_string(i)));

      div(context, mk(entity, 41 + i * 2),
          ComponentConfig{}
              .with_label("ABC")
              .with_size(ComponentSize{pixels(size - 10), pixels(size - 10)})
              .with_absolute_position()
              .with_translate(box_x + 5.0f, card_y + 5.0f)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_custom_text_color(text_light)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("shrink_text_" + std::to_string(i)));

      box_x += size + 10.0f;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(text_overflow, "Tools",
                        "Demonstrates text overflow debug indicators",
                        ExampleTextOverflow)
