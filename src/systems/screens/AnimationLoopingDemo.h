#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AnimationLoopingDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};
  afterhours::Color box_orange{249, 115, 22, 255};

  float time_elapsed = 0.0f;

  // Smooth easing function for bounce (attempt at ease-in for falling)
  float ease_out_bounce(float t) {
    // Simple approximation using sin for smooth motion
    return std::sin(t * 3.14159f);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Accumulate time for smooth continuous animations
    time_elapsed += dt;

    // Use constants for cleaner math
    constexpr float kPi = 3.14159265359f;
    constexpr float kTwoPi = 2.0f * kPi;

    // Calculate smooth animated values using trigonometry
    // Pulse 1: Breathe using translate (move in/out from center) - avoids layout resize
    float pulse1_phase = time_elapsed * kTwoPi / 4.0f;  // 4 second cycle
    float pulse1_offset = 10.0f * std::sin(pulse1_phase);  // -10 to +10 px offset

    // Pulse 2: Opacity-based pulse - smoother since no layout recalc
    float pulse2_phase = time_elapsed * kTwoPi / 1.5f;  // 1.5 second cycle
    float pulse2_opacity = 0.5f + 0.5f * std::sin(pulse2_phase);  // 0 to 1

    // Bounce: Use translate instead of resize for smooth motion
    float bounce_phase = time_elapsed * kTwoPi / 2.0f;
    float bounce_offset = 50.0f * (1.0f - std::cos(bounce_phase));  // 0 to 100 smoothly

    // Spin: Continuous rotation - full rotation every 3 seconds (slower)
    float rotation = std::fmod(time_elapsed * 120.0f, 360.0f);  // 120 deg/sec = 3 sec rotation

    // Setup theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // Center content vertically: content spans ~310px (title to box bottoms)
    float y_offset = std::max(0.0f, (screen_h - 310.0f) / 2.0f - 30.0f);

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Looping Animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(60)})
            .with_absolute_position(0.0f, 30.0f + y_offset)
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Subtitle
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Smooth continuous pulse, bounce, and spin effects")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, 90.0f + y_offset)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Layout constants - use larger boxes for smoother size animations
    // Larger boxes = more pixel steps during resize = smoother animation
    float box_size = 120.0f;
    float box_y = 220.0f + y_offset;
    float spacing = 200.0f;
    float center_x = screen_w / 2.0f;

    // ========== PULSE 1: Breathe (translate-based) ==========
    float pulse1_x = center_x - spacing * 1.5f - box_size / 2.0f;

    // Label
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Breathe")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(pulse1_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Animated box - translate moves it in/out smoothly
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(pulse1_x + pulse1_offset, box_y + pulse1_offset)
            .with_custom_background(box_blue)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)  // Circle
            .with_debug_name("pulse1_box"));

    // ========== PULSE 2: Opacity Glow ==========
    float pulse2_x = center_x - spacing * 0.5f - box_size / 2.0f;

    // Label
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Glow")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(pulse2_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Animated box with opacity
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(pulse2_x, box_y)
            .with_custom_background(
                afterhours::colors::opacity_pct(box_green, pulse2_opacity))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)  // Circle
            .with_debug_name("pulse2_box"));

    // ========== BOUNCE ==========
    float bounce_x = center_x + spacing * 0.5f - box_size / 2.0f;

    // Label
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Bounce")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(bounce_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Animated box (bounces down)
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(bounce_x, box_y + bounce_offset)
            .with_custom_background(box_purple)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)  // Circle
            .with_debug_name("bounce_box"));

    // ========== SPIN ==========
    float rotate_x = center_x + spacing * 1.5f - box_size / 2.0f;

    // Label
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Spin")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(rotate_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Spinning indicator orbits the center
    float angle_rad = rotation * 3.14159f / 180.0f;
    float orbit_radius = box_size / 3.0f;
    float indicator_size = 20.0f;
    float orbit_center_x = rotate_x + box_size / 2.0f - indicator_size / 2.0f;
    float orbit_center_y = box_y + box_size / 2.0f - indicator_size / 2.0f;
    float indicator_x = orbit_center_x + std::cos(angle_rad) * orbit_radius;
    float indicator_y = orbit_center_y + std::sin(angle_rad) * orbit_radius;

    // Outer ring (static)
    div(context, mk(entity, 41),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(rotate_x, box_y)
            .with_custom_background(
                afterhours::colors::opacity_pct(box_orange, 0.3f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("rotate_ring"));

    // Spinning indicator
    div(context, mk(entity, 42),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(indicator_size), pixels(indicator_size)})
            .with_absolute_position(indicator_x, indicator_y)
            .with_custom_background(box_orange)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("rotate_indicator"));

    // Info footer
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Smooth time-based animations using sin/cos")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, screen_h - 80.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(animation_looping, "Animations",
                        "Smooth looping animations (pulse, bounce, spin)",
                        AnimationLoopingDemo)
