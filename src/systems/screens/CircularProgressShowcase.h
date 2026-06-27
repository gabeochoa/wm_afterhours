#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
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
  float animated_progress =
      0.35f; // Start at 35% so screenshots show meaningful progress
  float animation_speed = 0.3f;

  // Configuration: minimum size (in base pixels, before scaling) to show
  // percentage text Sizes below this threshold will hide the percentage label
  // for better readability
  float min_size_for_percentage_text = 50.0f;

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
    context.scaling_mode = ScalingMode::Adaptive;

    // Ring sizes
    Size ring_size = h720(80.0f);
    float ring_thickness = 8.0f;
    Size title_font = h720(22.0f);
    Size value_font = h720(26.0f);
    Size desc_font = h720(18.0f);

    // Root — centering vstack
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_dark)
            .with_align_items(AlignItems::Center)
            .with_debug_name("bg"));

    // Title
    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_label("Circular Progress Indicators")
            .with_size(ComponentSize{percent(1.0f), h720(40.0f)})
            .with_margin(Margin{.top = h720(20.0f)})
            .with_font("Gaegu-Bold", h720(36.0f))
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Subtitle
    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label("Radial progress visualization")
            .with_size(ComponentSize{percent(1.0f), h720(28.0f)})
            .with_margin(Margin{.top = h720(5.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Top row (4 cards)
    auto top_row = hstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.90f), children()})
            .with_margin(Margin{.top = h720(10.0f)})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_gap(h720(10.0f))
            .with_debug_name("top_row"));

    // Helper lambda to create a card with circular progress
    auto make_card = [&](afterhours::Entity &parent, int base_id,
                         const char *card_title, float progress,
                         const char *pct_label, const char *desc_label,
                         afterhours::Color accent, float thickness) {
      auto card = vstack(
          context, mk(parent, base_id),
          ComponentConfig{}
              .with_size(ComponentSize{h720(170.0f), children()})
              .with_custom_background(card_dark)
              .with_soft_shadow(4.0f, 8.0f, 20.0f,
                                afterhours::Color{0, 0, 0, 80})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_align_items(AlignItems::Center)
              .with_padding(Padding{.top = h720(15.0f), .bottom = h720(15.0f)})
              .with_debug_name(fmt::format("card_{}", base_id)));

      // Card title
      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label(card_title)
              .with_size(ComponentSize{percent(1.0f), h720(24.0f)})
              .with_font("Gaegu-Bold", title_font)
              .with_custom_text_color(text_light)
              .with_alignment(TextAlignment::Center));

      // Progress ring
      circular_progress(
          context, mk(card.ent(), 1), progress,
          ComponentConfig{}
              .with_size(ComponentSize{ring_size, ring_size})
              .with_margin(Margin{.top = h720(15.0f)})
              .with_custom_background(accent)
              .with_border(track_dark, h720(thickness))
              .with_debug_name(fmt::format("progress_{}", base_id)));

      // Value label
      div(context, mk(card.ent(), 2),
          ComponentConfig{}
              .with_label(pct_label)
              .with_size(ComponentSize{percent(1.0f), h720(24.0f)})
              .with_margin(Margin{.top = h720(15.0f)})
              .with_font("Gaegu-Bold", value_font)
              .with_custom_text_color(accent)
              .with_alignment(TextAlignment::Center));

      // Description label
      div(context, mk(card.ent(), 3),
          ComponentConfig{}
              .with_label(desc_label)
              .with_size(ComponentSize{percent(1.0f), h720(20.0f)})
              .with_margin(Margin{.top = h720(5.0f)})
              .with_font(UIComponent::DEFAULT_FONT, desc_font)
              .with_custom_text_color(text_muted)
              .with_alignment(TextAlignment::Center));
    };

    // Card 1: Basic Progress
    make_card(top_row.ent(), 10, "Basic", 0.75f, "75%", "Static value",
              accent_cyan, ring_thickness);

    // Card 2: Animated Progress
    make_card(
        top_row.ent(), 20, "Animated", animated_progress,
        fmt::format("{}%", static_cast<int>(animated_progress * 100)).c_str(),
        "Live updating", accent_purple, ring_thickness);

    // Card 3: Thick Ring
    make_card(top_row.ent(), 30, "Thick Ring", 0.42f, "42%", "Thick ring",
              accent_green, 16.0f);

    // Card 4: Thin Ring
    make_card(top_row.ent(), 40, "Thin Ring", 0.88f, "88%", "Thin ring",
              accent_orange, 3.0f);

    // "Size Variations" section header
    div(context, mk(root.ent(), 100),
        ComponentConfig{}
            .with_label("Size Variations")
            .with_size(ComponentSize{percent(1.0f), h720(28.0f)})
            .with_margin(Margin{.top = h720(30.0f)})
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Bottom row (5 small cards)
    auto bottom_row = hstack(
        context, mk(root.ent(), 101),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.90f), children()})
            .with_margin(Margin{.top = h720(5.0f)})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_gap(h720(8.0f))
            .with_debug_name("bottom_row"));

    float base_sizes[] = {28.0f, 40.0f, 52.0f, 64.0f, 80.0f};
    afterhours::Color colors[] = {accent_cyan, accent_purple, accent_green,
                                  accent_orange, accent_pink};
    float progress_values[] = {0.25f, 0.50f, 0.75f, 0.90f, 1.0f};

    for (int i = 0; i < 5; i++) {
      auto card = vstack(
          context, mk(bottom_row.ent(), 110 + i),
          ComponentConfig{}
              .with_size(ComponentSize{h720(120.0f), children()})
              .with_custom_background(card_dark)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.10f)
              .with_align_items(AlignItems::Center)
              .with_padding(Padding{.top = h720(15.0f), .bottom = h720(15.0f)})
              .with_debug_name(fmt::format("size_card_{}", i)));

      // Progress indicator
      Size ring_sz = h720(base_sizes[i]);
      circular_progress(
          context, mk(card.ent(), 0), progress_values[i],
          ComponentConfig{}
              .with_size(ComponentSize{ring_sz, ring_sz})
              .with_custom_background(colors[i])
              .with_border(track_dark,
                           h720(std::max(3.0f, base_sizes[i] * 0.1f)))
              .with_debug_name(fmt::format("progress_size_{}", i)));

      // Size label
      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label(fmt::format("{:.0f}px", base_sizes[i]))
              .with_size(ComponentSize{percent(1.0f), h720(24.0f)})
              .with_margin(Margin{.top = h720(10.0f)})
              .with_font("Gaegu-Bold", h720(18.0f))
              .with_custom_text_color(colors[i])
              .with_alignment(TextAlignment::Center)
              .with_debug_name(fmt::format("size_label_{}", i)));

      // Progress label - only show for sizes above the configured threshold
      if (base_sizes[i] >= min_size_for_percentage_text) {
        div(context, mk(card.ent(), 2),
            ComponentConfig{}
                .with_label(fmt::format(
                    "{}%", static_cast<int>(progress_values[i] * 100)))
                .with_size(ComponentSize{percent(1.0f), h720(20.0f)})
                .with_margin(Margin{.top = h720(2.0f)})
                .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                .with_custom_text_color(text_muted)
                .with_alignment(TextAlignment::Center)
                .with_debug_name(fmt::format("progress_label_{}", i)));
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(
    circular_progress, "Component Galleries",
    "Circular/radial progress indicators with various styles",
    CircularProgressShowcase)
