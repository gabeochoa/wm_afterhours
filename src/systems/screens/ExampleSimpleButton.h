#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
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
  afterhours::Color text_dark{55, 50, 60, 255};        // Dark text
  afterhours::Color text_muted{140, 130, 145, 255};    // Muted text
  afterhours::Color confetti_pink{255, 180, 190, 255}; // Confetti color
  afterhours::Color confetti_blue{160, 200, 255, 255}; // Confetti color
  afterhours::Color confetti_mint{170, 235, 200, 255}; // Confetti color
  afterhours::Color confetti_gold{255, 215, 140, 255}; // Confetti color

  // Layout configuration - all spacing/sizing in one place
  static constexpr float card_width = 400.0f;
  static constexpr float card_height =
      440.0f; // Increased for better bottom padding
  static constexpr float card_padding = 24.0f; // Consistent card padding
  static constexpr float section_gap = 20.0f;  // Gap between major sections
  static constexpr float element_gap =
      12.0f; // Gap between elements within sections
  static constexpr float divider_margin_y =
      18.0f; // Vertical breathing room for divider

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_warm)
            .with_debug_name("bg"));

    // Decorative confetti dots (subtle background interest)
    afterhours::Color confetti[] = {confetti_pink, confetti_blue, confetti_mint,
                                    confetti_gold};
    float dot_positions[][2] = {{0.15f, 0.2f}, {0.85f, 0.25f}, {0.12f, 0.75f},
                                {0.88f, 0.7f}, {0.25f, 0.35f}, {0.75f, 0.4f},
                                {0.3f, 0.8f},  {0.7f, 0.85f}};
    for (int i = 0; i < 8; i++) {
      float dot_size = 12.0f + (i % 3) * 6.0f;
      div(context, mk(entity, 100 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(dot_size), pixels(dot_size)})
              .with_absolute_position(screen_width * dot_positions[i][0],
                                      screen_height * dot_positions[i][1])
              .with_custom_background(
                  afterhours::colors::opacity_pct(confetti[i % 4], 0.6f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("confetti_" + std::to_string(i)));
    }

    // Main card - using configurable dimensions
    float card_w = card_width;
    float card_h = card_height;
    float card_x = (screen_width - card_w) / 2.0f;
    float card_y = (screen_height - card_h) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_w), pixels(card_h)})
            .with_absolute_position(card_x, card_y)
            .with_custom_background(card_white)
            .with_soft_shadow(8.0f, 12.0f, 30.0f,
                              afterhours::Color{80, 60, 100, 40})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_debug_name("card"));

    // Title
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Simple Button Demo")
            .with_size(
                ComponentSize{pixels(card_w - card_padding * 2), pixels(36)})
            .with_absolute_position(card_x + card_padding,
                                    card_y + card_padding + 4.0f)
            .with_font("Gaegu-Bold", h720(28.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Subtitle
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Go ahead, give it a click!")
            .with_size(
                ComponentSize{pixels(card_w - card_padding * 2), pixels(22)})
            .with_absolute_position(card_x + card_padding,
                                    card_y + card_padding + 42.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // The button!
    float button_width = 360.0f;
    float button_height = 56.0f;
    float button_x = card_x + (card_w - button_width) / 2.0f;
    float button_y = card_y + card_padding + 76.0f; // After title and subtitle

    auto button_result =
        button(context, mk(entity, 10),
               ComponentConfig{}
                   .with_label("Click Me!")
                   .with_size(ComponentSize{pixels(button_width),
                                            pixels(button_height)})
                   .with_absolute_position(button_x, button_y)
                   .with_custom_background(btn_coral)
                   .with_soft_shadow(4.0f, 6.0f, 14.0f,
                                     afterhours::Color{255, 100, 90, 50})
                   .with_font("Gaegu-Bold", h720(26.0f))
                   .with_custom_text_color(card_white)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f)
                   .with_alignment(TextAlignment::Center)
                   .with_padding(Spacing::md));

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

    // Counter background pill - positioned with consistent gap after button
    float counter_y = button_y + button_height + section_gap;
    float counter_width = 180.0f;
    float counter_height = 42.0f;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(counter_width), pixels(counter_height)})
            .with_absolute_position(card_x + (card_w - counter_width) / 2.0f,
                                    counter_y)
            .with_custom_background(
                afterhours::colors::opacity_pct(counter_color, 0.12f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("counter_bg"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label(counter_text)
            .with_size(ComponentSize{pixels(counter_width - 10), pixels(32)})
            .with_absolute_position(card_x +
                                        (card_w - (counter_width - 10)) / 2.0f,
                                    counter_y + 5.0f)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(counter_color)
            .with_alignment(TextAlignment::Center));

    // Additional button variations section
    afterhours::Color secondary_btn{100, 180, 200, 255};
    afterhours::Color outline_border{180, 170, 190, 255};

    // Divider line - with improved breathing room
    float divider_y = counter_y + counter_height + divider_margin_y;
    float divider_width = button_width - 60.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(divider_width), pixels(1)})
            .with_absolute_position(card_x + (card_w - divider_width) / 2.0f,
                                    divider_y)
            .with_custom_background(
                afterhours::colors::opacity_pct(text_muted, 0.35f))
            .with_debug_name("divider"));

    // "More Styles" label
    float more_styles_y = divider_y + divider_margin_y - 4.0f;

    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("More Styles")
            .with_size(
                ComponentSize{pixels(card_w - card_padding * 2), pixels(20)})
            .with_absolute_position(card_x + card_padding, more_styles_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Secondary button (blue/teal style)
    float small_btn_w = 170.0f;
    float small_btn_h = 44.0f;
    float small_btn_y = more_styles_y + section_gap + 8.0f;
    float btn_gap = 10.0f; // Gap between buttons
    float left_btn_x = card_x + (card_w / 2.0f) - small_btn_w - btn_gap / 2.0f;
    float right_btn_x = card_x + (card_w / 2.0f) + btn_gap / 2.0f;

    button(
        context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Secondary")
            .with_size(ComponentSize{pixels(small_btn_w), pixels(small_btn_h)})
            .with_absolute_position(left_btn_x, small_btn_y)
            .with_custom_background(secondary_btn)
            .with_soft_shadow(3.0f, 4.0f, 10.0f,
                              afterhours::Color{80, 160, 180, 40})
            .with_font("Gaegu-Bold", h720(18.0f))
            .with_custom_text_color(card_white)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.4f)
            .with_alignment(TextAlignment::Center)
            .with_padding(Spacing::sm));

    // Outline button style - add subtle shadow for depth consistency
    button(
        context, mk(entity, 33),
        ComponentConfig{}
            .with_label("Outline")
            .with_size(ComponentSize{pixels(small_btn_w), pixels(small_btn_h)})
            .with_absolute_position(right_btn_x, small_btn_y)
            .with_custom_background(card_white)
            .with_border(outline_border, 2.0f)
            .with_soft_shadow(2.0f, 3.0f, 8.0f,
                              afterhours::Color{100, 90, 110, 25})
            .with_font("Gaegu-Bold", h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.4f)
            .with_alignment(TextAlignment::Center)
            .with_padding(Spacing::sm));

    // Small pill buttons row - with subtle shadows for visual consistency
    float pill_btn_w = 105.0f;
    float pill_btn_h = 36.0f;
    float pill_y = small_btn_y + small_btn_h + element_gap + 4.0f;
    float pill_gap = 10.0f;
    float total_pills_w = pill_btn_w * 3 + pill_gap * 2;
    float pill_start_x = card_x + (card_w - total_pills_w) / 2.0f;

    button(context, mk(entity, 34),
           ComponentConfig{}
               .with_label("Small")
               .with_size(ComponentSize{pixels(pill_btn_w), pixels(pill_btn_h)})
               .with_absolute_position(pill_start_x, pill_y)
               .with_custom_background(confetti_mint)
               .with_soft_shadow(2.0f, 3.0f, 6.0f,
                                 afterhours::Color{140, 200, 170, 35})
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_custom_text_color(text_dark)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center));

    button(context, mk(entity, 35),
           ComponentConfig{}
               .with_label("Pill")
               .with_size(ComponentSize{pixels(pill_btn_w), pixels(pill_btn_h)})
               .with_absolute_position(pill_start_x + pill_btn_w + pill_gap,
                                       pill_y)
               .with_custom_background(confetti_pink)
               .with_soft_shadow(2.0f, 3.0f, 6.0f,
                                 afterhours::Color{220, 150, 160, 35})
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_custom_text_color(text_dark)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center));

    button(context, mk(entity, 36),
           ComponentConfig{}
               .with_label("Buttons")
               .with_size(ComponentSize{pixels(pill_btn_w), pixels(pill_btn_h)})
               .with_absolute_position(
                   pill_start_x + (pill_btn_w + pill_gap) * 2, pill_y)
               .with_custom_background(confetti_gold)
               .with_soft_shadow(2.0f, 3.0f, 6.0f,
                                 afterhours::Color{220, 185, 120, 35})
               .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
               .with_custom_text_color(text_dark)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(simple_button, "Tools",
                        "Simple button example with click counter",
                        ExampleSimpleButton)
