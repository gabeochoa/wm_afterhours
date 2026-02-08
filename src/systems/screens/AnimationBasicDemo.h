#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/animation.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Animation keys for this demo
enum struct BasicAnimKey : size_t {
  FadeIn,
  SlideIn,
  ScaleUp,
};

struct AnimationBasicDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};

  bool animations_started = false;
  bool first_run = true;  // Start with completed values for screenshot

  void start_animations() {
    if (animations_started)
      return;
    animations_started = true;

    if (first_run) {
      // For first run, set to end values so screenshot shows something
      first_run = false;
      auto &fade_track = afterhours::animation::manager<BasicAnimKey>().ensure_track(BasicAnimKey::FadeIn);
      fade_track.current = 1.0f;
      auto &slide_track = afterhours::animation::manager<BasicAnimKey>().ensure_track(BasicAnimKey::SlideIn);
      slide_track.current = 0.0f;
      auto &scale_track = afterhours::animation::manager<BasicAnimKey>().ensure_track(BasicAnimKey::ScaleUp);
      scale_track.current = 1.0f;
      return;
    }

    // Fade: 0 -> 1 over 1.6s (slower for visibility)
    afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::FadeIn)
        .from(0.0f)
        .to(1.0f, 1.6f, afterhours::animation::EasingType::EaseOutQuad);

    // Slide: -200 -> 0 over 1.2s (slower for visibility)
    afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::SlideIn)
        .from(-200.0f)
        .to(0.0f, 1.2f, afterhours::animation::EasingType::EaseOutQuad);

    // Scale: 0 -> 1 over 1.0s with bounce effect (sequence)
    afterhours::animation::anim<BasicAnimKey>(BasicAnimKey::ScaleUp)
        .from(0.0f)
        .sequence({
            {.to_value = 1.15f,
             .duration = 0.6f,
             .easing = afterhours::animation::EasingType::EaseOutQuad},
            {.to_value = 1.0f,
             .duration = 0.4f,
             .easing = afterhours::animation::EasingType::EaseOutQuad},
        });
  }

  float get_anim_value(BasicAnimKey key, float /*default_val*/ = 0.0f) {
    auto val =
        afterhours::animation::manager<BasicAnimKey>().get_value(key);
    if (val.has_value()) {
      return val.value();
    }
    // If animation finished, return the end value
    auto &track =
        afterhours::animation::manager<BasicAnimKey>().ensure_track(key);
    return track.current;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Update animation manager
    afterhours::animation::manager<BasicAnimKey>().update(dt);

    // Start animations on first frame
    start_animations();

    // Get animated values
    float fade_opacity = get_anim_value(BasicAnimKey::FadeIn, 0.0f);
    float slide_offset = get_anim_value(BasicAnimKey::SlideIn, -200.0f);
    float scale_factor = get_anim_value(BasicAnimKey::ScaleUp, 0.0f);

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
            .with_label("Animation Basics")
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
            .with_label("Fade, Slide, and Scale animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, 90.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // Layout constants
    float box_size = 120.0f;
    float box_y = 280.0f;
    float spacing = 200.0f;
    float center_x = screen_w / 2.0f;

    // ========== BOX 1: Slide In (left position) ==========
    float slide_base_x = center_x - spacing - box_size / 2.0f;

    // Label above box
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Slide In")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(slide_base_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("slide_label"));

    // Animated box with position offset
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(slide_base_x + slide_offset, box_y)
            .with_custom_background(box_green)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("slide_box"));

    // Value indicator
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label(fmt::format("x offset: {:.0f}px", slide_offset))
            .with_size(ComponentSize{pixels(box_size + 40), pixels(24)})
            .with_absolute_position(slide_base_x - 20.0f, box_y + box_size + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("slide_value"));

    // ========== BOX 2: Fade In (center position) ==========
    float fade_x = center_x - box_size / 2.0f;

    // Label above box
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Fade In")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(fade_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fade_label"));

    // Animated box with opacity
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(fade_x, box_y)
            .with_custom_background(
                afterhours::colors::opacity_pct(box_blue, fade_opacity))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("fade_box"));

    // Value indicator
    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label(fmt::format("opacity: {:.0f}%", fade_opacity * 100))
            .with_size(ComponentSize{pixels(box_size), pixels(24)})
            .with_absolute_position(fade_x, box_y + box_size + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fade_value"));

    // ========== BOX 3: Scale Up ==========
    float scale_x = center_x + spacing - box_size / 2.0f;

    // Label above box
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Scale Up")
            .with_size(ComponentSize{pixels(box_size), pixels(30)})
            .with_absolute_position(scale_x, box_y - 40.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("scale_label"));

    // Animated box with scale - using with_scale() for smooth animations
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(scale_x, box_y)
            .with_scale(scale_factor)  // Smooth visual scaling after layout
            .with_custom_background(box_purple)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("scale_box"));

    // Value indicator
    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label(fmt::format("scale: {:.0f}%", scale_factor * 100))
            .with_size(ComponentSize{pixels(box_size), pixels(24)})
            .with_absolute_position(scale_x, box_y + box_size + 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("scale_value"));

    // ========== Restart Button ==========
    if (button(context, mk(entity, 100),
               ComponentConfig{}
                   .with_label("Restart Animations")
                   .with_size(ComponentSize{pixels(200), pixels(48)})
                   .with_absolute_position(center_x - 100.0f, screen_h - 100.0f)
                   .with_background(Theme::Usage::Primary)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.25f)
                   .with_debug_name("restart_btn"))) {
      animations_started = false;
      start_animations();
    }
  }
};

REGISTER_EXAMPLE_SCREEN(animation_basic, "Animations",
                        "Basic property animations (fade, slide, scale)",
                        AnimationBasicDemo)
