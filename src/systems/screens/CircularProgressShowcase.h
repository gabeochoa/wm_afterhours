#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CircularProgressShowcase : ScreenSystem<UIContext<InputAction>> {
  // Modern dark theme for visual impact
  afterhours::Color bg_dark{18, 18, 24, 255};         // Deep charcoal
  afterhours::Color card_dark{30, 32, 42, 255};       // Dark card surface
  afterhours::Color text_light{248, 250, 252, 255};   // Off-white text
  afterhours::Color text_muted{148, 163, 184, 255};   // Muted gray
  afterhours::Color accent_cyan{34, 211, 238, 255};   // Vibrant cyan
  afterhours::Color accent_purple{168, 85, 247, 255}; // Electric purple
  afterhours::Color accent_green{74, 222, 128, 255};  // Fresh green
  afterhours::Color accent_orange{251, 146, 60, 255}; // Warm orange
  afterhours::Color accent_pink{244, 114, 182, 255};  // Hot pink
  afterhours::Color track_dark{55, 65, 81, 150};      // Subtle track

  // Animated values
  float animated_progress = 0.0f;
  float animation_speed = 0.3f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Animate the progress value
    animated_progress += dt * animation_speed;
    if (animated_progress > 1.0f) {
      animated_progress = 0.0f;
    }

    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = card_dark;
    theme.primary = accent_cyan;
    theme.secondary = track_dark;
    theme.accent = accent_purple;
    theme.roundness = 0.15f;
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

    // Scale factor based on screen height (designed for 720p)
    float scale = static_cast<float>(screen_height) / 720.0f;

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Circular Progress Indicators")
            .with_size(
                ComponentSize{pixels(screen_width), pixels(40.0f * scale)})
            .with_absolute_position()
            .with_translate(0.0f, 20.0f * scale)
            .with_font("Gaegu-Bold", 36.0f * scale)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Radial progress visualization")
            .with_size(
                ComponentSize{pixels(screen_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(0.0f, 65.0f * scale)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f * scale)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    float content_y = 95.0f * scale;
    float card_width = 200.0f * scale;
    float card_height = 240.0f * scale;
    float card_gap = 20.0f * scale;
    float total_width = card_width * 4 + card_gap * 3;
    float start_x = (screen_width - total_width) / 2.0f;

    // Ring sizes based on scale
    float ring_size = 80.0f * scale;
    float ring_thickness = 8.0f * scale;
    float title_font = 20.0f * scale;
    float value_font = 24.0f * scale;
    float desc_font = 14.0f * scale;

    // ========== CARD 1: Basic Progress ==========
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(start_x, content_y)
            .with_custom_background(card_dark)
            .with_soft_shadow(4.0f * scale, 8.0f * scale, 20.0f * scale,
                              afterhours::Color{0, 0, 0, 80})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("card1"));

    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Basic")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(start_x, content_y + 15.0f * scale)
            .with_font("Gaegu-Bold", title_font)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card1_title"));

    // 75% progress indicator
    circular_progress(
        context, mk(entity, 12), 0.75f,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(ring_size), pixels(ring_size)})
            .with_absolute_position()
            .with_translate(start_x + (card_width - ring_size) / 2,
                            content_y + 50.0f * scale)
            .with_custom_background(accent_cyan)
            .with_border(track_dark, ring_thickness)
            .with_debug_name("progress_basic"));

    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("75%")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(start_x, content_y + 145.0f * scale)
            .with_font("Gaegu-Bold", value_font)
            .with_custom_text_color(accent_cyan)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card1_value"));

    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label("Static value")
            .with_size(ComponentSize{pixels(card_width), pixels(20.0f * scale)})
            .with_absolute_position()
            .with_translate(start_x, content_y + 175.0f * scale)
            .with_font(UIComponent::DEFAULT_FONT, desc_font)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card1_desc"));

    // ========== CARD 2: Animated Progress ==========
    float card2_x = start_x + card_width + card_gap;
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card2_x, content_y)
            .with_custom_background(card_dark)
            .with_soft_shadow(4.0f * scale, 8.0f * scale, 20.0f * scale,
                              afterhours::Color{0, 0, 0, 80})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("card2"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Animated")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card2_x, content_y + 15.0f * scale)
            .with_font("Gaegu-Bold", title_font)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card2_title"));

    // Animated progress indicator
    circular_progress(
        context, mk(entity, 22), animated_progress,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(ring_size), pixels(ring_size)})
            .with_absolute_position()
            .with_translate(card2_x + (card_width - ring_size) / 2,
                            content_y + 50.0f * scale)
            .with_custom_background(accent_purple)
            .with_border(track_dark, ring_thickness)
            .with_debug_name("progress_animated"));

    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label(
                fmt::format("{}%", static_cast<int>(animated_progress * 100)))
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card2_x, content_y + 145.0f * scale)
            .with_font("Gaegu-Bold", value_font)
            .with_custom_text_color(accent_purple)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card2_value"));

    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_label("Live updating")
            .with_size(ComponentSize{pixels(card_width), pixels(20.0f * scale)})
            .with_absolute_position()
            .with_translate(card2_x, content_y + 175.0f * scale)
            .with_font(UIComponent::DEFAULT_FONT, desc_font)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card2_desc"));

    // ========== CARD 3: Thick Ring ==========
    float card3_x = start_x + (card_width + card_gap) * 2;
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card3_x, content_y)
            .with_custom_background(card_dark)
            .with_soft_shadow(4.0f * scale, 8.0f * scale, 20.0f * scale,
                              afterhours::Color{0, 0, 0, 80})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("card3"));

    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Thick Ring")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card3_x, content_y + 15.0f * scale)
            .with_font("Gaegu-Bold", title_font)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card3_title"));

    // Thick ring progress (scaled thickness)
    circular_progress(
        context, mk(entity, 32), 0.42f,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(ring_size), pixels(ring_size)})
            .with_absolute_position()
            .with_translate(card3_x + (card_width - ring_size) / 2,
                            content_y + 50.0f * scale)
            .with_custom_background(accent_green)
            .with_border(track_dark, 16.0f * scale)
            .with_debug_name("progress_thick"));

    div(context, mk(entity, 33),
        ComponentConfig{}
            .with_label("42%")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card3_x, content_y + 145.0f * scale)
            .with_font("Gaegu-Bold", value_font)
            .with_custom_text_color(accent_green)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card3_value"));

    div(context, mk(entity, 34),
        ComponentConfig{}
            .with_label("Thick ring")
            .with_size(ComponentSize{pixels(card_width), pixels(20.0f * scale)})
            .with_absolute_position()
            .with_translate(card3_x, content_y + 175.0f * scale)
            .with_font(UIComponent::DEFAULT_FONT, desc_font)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card3_desc"));

    // ========== CARD 4: Thin Ring ==========
    float card4_x = start_x + (card_width + card_gap) * 3;
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card4_x, content_y)
            .with_custom_background(card_dark)
            .with_soft_shadow(4.0f * scale, 8.0f * scale, 20.0f * scale,
                              afterhours::Color{0, 0, 0, 80})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.08f)
            .with_debug_name("card4"));

    div(context, mk(entity, 41),
        ComponentConfig{}
            .with_label("Thin Ring")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card4_x, content_y + 15.0f * scale)
            .with_font("Gaegu-Bold", title_font)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card4_title"));

    // Thin ring progress (scaled thickness)
    circular_progress(
        context, mk(entity, 42), 0.88f,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(ring_size), pixels(ring_size)})
            .with_absolute_position()
            .with_translate(card4_x + (card_width - ring_size) / 2,
                            content_y + 50.0f * scale)
            .with_custom_background(accent_orange)
            .with_border(track_dark, 3.0f * scale)
            .with_debug_name("progress_thin"));

    div(context, mk(entity, 43),
        ComponentConfig{}
            .with_label("88%")
            .with_size(ComponentSize{pixels(card_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(card4_x, content_y + 145.0f * scale)
            .with_font("Gaegu-Bold", value_font)
            .with_custom_text_color(accent_orange)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card4_value"));

    div(context, mk(entity, 44),
        ComponentConfig{}
            .with_label("Thin ring")
            .with_size(ComponentSize{pixels(card_width), pixels(20.0f * scale)})
            .with_absolute_position()
            .with_translate(card4_x, content_y + 175.0f * scale)
            .with_font(UIComponent::DEFAULT_FONT, desc_font)
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("card4_desc"));

    // ========== Bottom row: Size variations ==========
    float bottom_y = content_y + card_height + 30.0f * scale;
    float small_card_width = 120.0f * scale;
    float small_card_height = 150.0f * scale;
    float small_gap = 15.0f * scale;
    float bottom_total = small_card_width * 5 + small_gap * 4;
    float bottom_start_x = (screen_width - bottom_total) / 2.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Size Variations")
            .with_size(
                ComponentSize{pixels(screen_width), pixels(24.0f * scale)})
            .with_absolute_position()
            .with_translate(0.0f, bottom_y - 5.0f * scale)
            .with_font("Gaegu-Bold", 22.0f * scale)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_title"));

    bottom_y += 30.0f * scale;

    // Scaled sizes
    float base_sizes[] = {28.0f, 40.0f, 52.0f, 64.0f, 80.0f};
    afterhours::Color colors[] = {accent_cyan, accent_purple, accent_green,
                                  accent_orange, accent_pink};
    float progress_values[] = {0.25f, 0.50f, 0.75f, 0.90f, 1.0f};

    for (int i = 0; i < 5; i++) {
      float card_x = bottom_start_x +
                     (small_card_width + small_gap) * static_cast<float>(i);
      float size = base_sizes[i] * scale;

      // Card background
      div(context, mk(entity, 110 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(small_card_width),
                                       pixels(small_card_height)})
              .with_absolute_position()
              .with_translate(card_x, bottom_y)
              .with_custom_background(card_dark)
              .with_rounded_corners(std::bitset<4>(0b1111))
              .with_roundness(0.10f)
              .with_debug_name(fmt::format("size_card_{}", i)));

      // Progress indicator
      circular_progress(
          context, mk(entity, 120 + i), progress_values[i],
          ComponentConfig{}
              .with_size(ComponentSize{pixels(size), pixels(size)})
              .with_absolute_position()
              .with_translate(card_x + (small_card_width - size) / 2,
                              bottom_y + 15.0f * scale +
                                  (70.0f * scale - size) / 2)
              .with_custom_background(colors[i])
              .with_border(track_dark, std::max(3.0f * scale, size * 0.1f))
              .with_debug_name(fmt::format("progress_size_{}", i)));

      // Size label
      div(context, mk(entity, 130 + i),
          ComponentConfig{}
              .with_label(fmt::format("{:.0f}px", size))
              .with_size(ComponentSize{pixels(small_card_width),
                                       pixels(20.0f * scale)})
              .with_absolute_position()
              .with_translate(card_x, bottom_y + 100.0f * scale)
              .with_font("Gaegu-Bold", 16.0f * scale)
              .with_custom_text_color(colors[i])
              .with_alignment(TextAlignment::Center)
              .with_debug_name(fmt::format("size_label_{}", i)));

      // Progress label
      div(context, mk(entity, 140 + i),
          ComponentConfig{}
              .with_label(fmt::format(
                  "{}%", static_cast<int>(progress_values[i] * 100)))
              .with_size(ComponentSize{pixels(small_card_width),
                                       pixels(16.0f * scale)})
              .with_absolute_position()
              .with_translate(card_x, bottom_y + 122.0f * scale)
              .with_font(UIComponent::DEFAULT_FONT, 12.0f * scale)
              .with_custom_text_color(text_muted)
              .with_alignment(TextAlignment::Center)
              .with_debug_name(fmt::format("progress_label_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(
    circular_progress, "Component Galleries",
    "Circular/radial progress indicators with various styles",
    CircularProgressShowcase)
