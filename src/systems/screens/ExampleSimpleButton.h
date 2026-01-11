#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleSimpleButton : ScreenSystem<UIContext<InputAction>> {
  int button_click_count = 0;

  // Playful candy-like color scheme
  afterhours::Color bg_warm{255, 245, 235, 255};       // Warm cream
  afterhours::Color card_white{255, 255, 255, 255};    // Pure white
  afterhours::Color btn_coral{255, 115, 105, 255};     // Vibrant coral
  afterhours::Color btn_coral_dark{230, 90, 80, 255};  // Darker coral for border
  afterhours::Color text_dark{55, 50, 60, 255};        // Dark text
  afterhours::Color text_muted{140, 130, 145, 255};    // Muted text
  afterhours::Color confetti_pink{255, 180, 190, 255}; // Confetti color
  afterhours::Color confetti_blue{160, 200, 255, 255}; // Confetti color
  afterhours::Color confetti_mint{170, 235, 200, 255}; // Confetti color
  afterhours::Color confetti_gold{255, 215, 140, 255}; // Confetti color

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = card_white;
    theme.font_muted = text_muted;
    theme.background = bg_warm;
    theme.surface = card_white;
    theme.primary = btn_coral;
    theme.secondary = afterhours::Color{100, 180, 200, 255};
    theme.accent = afterhours::Color{255, 180, 100, 255};
    theme.error = afterhours::Color{220, 80, 80, 255};
    theme.roundness = 0.15f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_warm)
            .with_debug_name("bg"));

    // Decorative confetti dots (subtle background interest)
    afterhours::Color confetti[] = {confetti_pink, confetti_blue, confetti_mint, confetti_gold};
    float dot_positions[][2] = {
        {0.15f, 0.2f}, {0.85f, 0.25f}, {0.12f, 0.75f}, {0.88f, 0.7f},
        {0.25f, 0.35f}, {0.75f, 0.4f}, {0.3f, 0.8f}, {0.7f, 0.85f}
    };
    for (int i = 0; i < 8; i++) {
      float dot_size = 12.0f + (i % 3) * 6.0f;
      div(context, mk(entity, 100 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(dot_size), pixels(dot_size)})
              .with_absolute_position()
              .with_translate(screen_width * dot_positions[i][0],
                             screen_height * dot_positions[i][1])
              .with_custom_background(afterhours::colors::opacity_pct(confetti[i % 4], 0.6f))
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(1.0f)
              .with_debug_name("confetti_" + std::to_string(i)));
    }

    // Main card
    float card_w = 340.0f;
    float card_h = 280.0f;
    float card_x = (screen_width - card_w) / 2.0f;
    float card_y = (screen_height - card_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_w), pixels(card_h)})
            .with_absolute_position()
            .with_translate(card_x, card_y)
            .with_custom_background(card_white)
            .with_soft_shadow(8.0f, 12.0f, 30.0f, afterhours::Color{80, 60, 100, 40})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.12f)
            .with_debug_name("card"));

    // Title
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Simple Button Demo")
            .with_size(ComponentSize{pixels(card_w - 40), pixels(36)})
            .with_absolute_position()
            .with_translate(card_x + 20.0f, card_y + 30.0f)
            .with_font("Gaegu-Bold", 28.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Go ahead, give it a click!")
            .with_size(ComponentSize{pixels(card_w - 40), pixels(22)})
            .with_absolute_position()
            .with_translate(card_x + 20.0f, card_y + 68.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // The button!
    float button_width = 200.0f;
    float button_height = 60.0f;
    float button_x = card_x + (card_w - button_width) / 2.0f;
    float button_y = card_y + 115.0f;

    auto button_result =
        button(context, mk(entity, 10),
               ComponentConfig{}
                   .with_label("Click Me!")
                   .with_size(ComponentSize{pixels(button_width), pixels(button_height)})
                   .with_absolute_position()
                   .with_translate(button_x, button_y)
                   .with_custom_background(btn_coral)
                   .with_border(btn_coral_dark, 3.0f)
                   .with_soft_shadow(4.0f, 6.0f, 14.0f, afterhours::Color{255, 100, 90, 50})
                   .with_font("Gaegu-Bold", 26.0f)
                   .with_custom_text_color(card_white)
                   .with_rounded_corners(std::bitset<4>(0b1111))
                   .with_roundness(0.5f)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("example_button"));

    if (button_result) {
      button_click_count++;
      log_info("Button clicked! Count: {}", button_click_count);
    }

    // Click counter with fun styling
    std::string counter_text;
    afterhours::Color counter_color = text_muted;
    if (button_click_count == 0) {
      counter_text = "No clicks yet...";
    } else if (button_click_count == 1) {
      counter_text = "1 click!";
      counter_color = btn_coral;
    } else if (button_click_count < 10) {
      counter_text = std::to_string(button_click_count) + " clicks!";
      counter_color = btn_coral;
    } else if (button_click_count < 50) {
      counter_text = std::to_string(button_click_count) + " clicks! Nice!";
      counter_color = afterhours::Color{100, 180, 100, 255};
    } else {
      counter_text = std::to_string(button_click_count) + " clicks! WOW!";
      counter_color = afterhours::Color{200, 150, 50, 255};
    }

    // Counter background pill
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(180), pixels(42)})
            .with_absolute_position()
            .with_translate(card_x + (card_w - 180) / 2.0f, card_y + 195.0f)
            .with_custom_background(afterhours::colors::opacity_pct(counter_color, 0.12f))
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_debug_name("counter_bg"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label(counter_text)
            .with_size(ComponentSize{pixels(170), pixels(32)})
            .with_absolute_position()
            .with_translate(card_x + (card_w - 170) / 2.0f, card_y + 200.0f)
            .with_font("Gaegu-Bold", 22.0f)
            .with_custom_text_color(counter_color)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("click_counter"));
  }
};

REGISTER_EXAMPLE_SCREEN(simple_button, "Tools",
                        "Simple button example with click counter",
                        ExampleSimpleButton)
