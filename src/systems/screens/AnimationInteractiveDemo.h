#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Animation keys for interactive demo
enum struct InteractiveAnimKey : size_t {
  Button1Scale,
  Button2Scale,
  Button3Scale,
};

struct AnimationInteractiveDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};

  // Click counters
  int click_count = 0;

  // For first-run screenshot
  bool first_run = true;

  float get_anim_value(InteractiveAnimKey key, float default_val = 1.0f) {
    auto val =
        afterhours::animation::manager<InteractiveAnimKey>().get_value(key);
    if (val.has_value()) {
      return val.value();
    }
    auto &track =
        afterhours::animation::manager<InteractiveAnimKey>().ensure_track(key);
    if (track.current != 0.0f) {
      return track.current;
    }
    return default_val;
  }

  void animate_button_press(InteractiveAnimKey key) {
    // Quick scale down and back up for press feedback
    afterhours::animation::anim<InteractiveAnimKey>(key)
        .from(1.0f)
        .sequence({
            {.to_value = 0.85f,
             .duration = 0.08f,
             .easing = afterhours::animation::EasingType::EaseOutQuad},
            {.to_value = 1.1f,
             .duration = 0.12f,
             .easing = afterhours::animation::EasingType::EaseOutQuad},
            {.to_value = 1.0f,
             .duration = 0.1f,
             .easing = afterhours::animation::EasingType::EaseOutQuad},
        });
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Update animation manager
    afterhours::animation::manager<InteractiveAnimKey>().update(dt);

    // Initialize animation tracks to 1.0 on first run for screenshot
    if (first_run) {
      first_run = false;
      auto &track1 = afterhours::animation::manager<InteractiveAnimKey>().ensure_track(InteractiveAnimKey::Button1Scale);
      track1.current = 1.0f;
      auto &track2 = afterhours::animation::manager<InteractiveAnimKey>().ensure_track(InteractiveAnimKey::Button2Scale);
      track2.current = 1.0f;
      auto &track3 = afterhours::animation::manager<InteractiveAnimKey>().ensure_track(InteractiveAnimKey::Button3Scale);
      track3.current = 1.0f;
    }

    // Setup theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Interactive Animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(60)})
            .with_absolute_position(0.0f, 30.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Click buttons to see press animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, 90.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // Layout constants
    float button_size = 100.0f;
    float button_y = 220.0f;
    float spacing = 160.0f;
    float center_x = screen_w / 2.0f;

    // Get animated scale values
    float scale1 = get_anim_value(InteractiveAnimKey::Button1Scale, 1.0f);
    float scale2 = get_anim_value(InteractiveAnimKey::Button2Scale, 1.0f);
    float scale3 = get_anim_value(InteractiveAnimKey::Button3Scale, 1.0f);

    // ========== BUTTON 1 ==========
    // Using with_scale() for smooth animations - bypasses layout recalculation
    float btn1_x = center_x - spacing - button_size / 2.0f;

    if (button(context, mk(entity, 10),
               ComponentConfig{}
                   .with_label("Click!")
                   .with_size(ComponentSize{pixels(button_size),
                                            pixels(button_size)})
                   .with_absolute_position(btn1_x, button_y)
                   .with_scale(scale1)  // Smooth visual scaling after layout
                   .with_custom_background(box_blue)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_debug_name("btn1"))) {
      click_count++;
      animate_button_press(InteractiveAnimKey::Button1Scale);
    }

    // ========== BUTTON 2 ==========
    float btn2_x = center_x - button_size / 2.0f;

    if (button(context, mk(entity, 20),
               ComponentConfig{}
                   .with_label("Press!")
                   .with_size(ComponentSize{pixels(button_size),
                                            pixels(button_size)})
                   .with_absolute_position(btn2_x, button_y)
                   .with_scale(scale2)  // Smooth visual scaling after layout
                   .with_custom_background(box_green)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_debug_name("btn2"))) {
      click_count++;
      animate_button_press(InteractiveAnimKey::Button2Scale);
    }

    // ========== BUTTON 3 ==========
    float btn3_x = center_x + spacing - button_size / 2.0f;

    if (button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("Tap!")
                   .with_size(ComponentSize{pixels(button_size),
                                            pixels(button_size)})
                   .with_absolute_position(btn3_x, button_y)
                   .with_scale(scale3)  // Smooth visual scaling after layout
                   .with_custom_background(box_purple)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_debug_name("btn3"))) {
      click_count++;
      animate_button_press(InteractiveAnimKey::Button3Scale);
    }

    // Click counter display
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label(fmt::format("Total clicks: {}", click_count))
            .with_size(ComponentSize{pixels(200), pixels(40)})
            .with_absolute_position(center_x - 100.0f, button_y + button_size + 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("click_counter"));

    // Instructions section
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Using with_scale() for smooth visual scaling animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, screen_h - 80.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(animation_interactive, "Animations",
                        "Interactive click animations with squash/stretch",
                        AnimationInteractiveDemo)
