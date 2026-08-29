#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AnimationSpringDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors - vibrant palette
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_pink{236, 72, 153, 255};
  afterhours::Color box_cyan{34, 211, 238, 255};
  afterhours::Color box_yellow{250, 204, 21, 255};
  afterhours::Color box_lime{163, 230, 53, 255};
  afterhours::Color box_violet{139, 92, 246, 255};

  // Animation state
  float time_elapsed = 0.0f;
  // Four seconds in the past, so every spring is already settled on the first
  // frame. Headless renders about two frames, which is nowhere near enough for
  // a spring that starts at zero, and the boxes captured as invisible.
  float trigger_time = -4.0f;
  int bounce_count = 0;

  // Spring physics parameters
  struct SpringParams {
    float frequency = 8.0f;
    float decay = 3.0f;
    float amplitude = 1.0f;
  };

  // Damped spring oscillation (settles to 0)
  float spring_oscillation(float t, const SpringParams &params) {
    if (t < 0.0f)
      return 0.0f;
    float damping = std::exp(-params.decay * t);
    float oscillation = std::sin(params.frequency * t * 6.28318f);
    return params.amplitude * oscillation * damping;
  }

  // Spring that settles to target (1.0) with overshoot
  float spring_to_target(float t, const SpringParams &params) {
    if (t < 0.0f)
      return 0.0f;
    float damping = std::exp(-params.decay * t);
    float oscillation = std::cos(params.frequency * t * 6.28318f);
    return 1.0f - oscillation * damping;
  }

  // Elastic pop - quick overshoot then settle
  // Returns minimum 0.35 to ensure container stays large enough for text with
  // margins
  float elastic_pop(float t, float overshoot = 1.3f,
                    float settle_speed = 4.0f) {
    if (t < 0.0f)
      return 0.35f;
    if (t > 2.0f)
      return 1.0f;
    float progress = 1.0f - std::exp(-settle_speed * t);
    float bounce =
        std::sin(t * 12.0f) * std::exp(-t * 3.0f) * (overshoot - 1.0f);
    return std::max(0.35f, std::min(1.0f, progress + bounce));
  }

  // Balloon pop - deflate to 0 then smoothly inflate back to 1
  float balloon_pop(float t, float duration = 0.8f) {
    if (t < 0.0f)
      return 1.0f; // Before animation, full size
    if (t >= duration)
      return 1.0f; // After animation, full size
    // Start at 0, smoothly ease-out to 1
    float progress = t / duration;
    // Ease-out cubic for smooth deceleration
    float eased =
        1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress);
    return eased;
  }

  // Pendulum rocker - starts swung to one side, oscillates back to center
  float pendulum_rocker(float t, float amplitude = 15.0f,
                        float frequency = 2.0f, float decay = 2.0f) {
    if (t < 0.0f)
      return 0.0f;
    // Starts at amplitude (tilted), swings back through center, settles at 0
    return amplitude * std::cos(frequency * t * 6.28318f) *
           std::exp(-decay * t);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    time_elapsed += dt;

    float anim_t = time_elapsed - trigger_time;

    // Setup theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    float center_x = screen_w / 2.0f;

    // Center content vertically: content spans ~375px (title to bounce counter)
    float y_offset = std::max(0.0f, (screen_h - 375.0f) / 2.0f - 25.0f);

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Title with bounce-in effect
    float title_scale = elastic_pop(anim_t - 0.0f, 1.1f, 2.5f);
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Springy!")
            .with_size(ComponentSize{pixels(400), pixels(70)})
            .with_absolute_position(center_x - 200.0f, 25.0f + y_offset)
            .with_scale(title_scale)
            .with_font(UIComponent::DEFAULT_FONT, h720(42.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Subtitle
    float sub_scale = elastic_pop(anim_t - 0.15f, 1.05f, 3.0f);
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Click the boxes or restart to see them bounce")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, 100.0f + y_offset)
            .with_scale(sub_scale)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Layout
    float box_size = 100.0f;
    float box_y = 200.0f + y_offset;
    float spacing = 150.0f;

    // Store click times for each box
    static float box_click_times[5] = {-10.0f, -10.0f, -10.0f, -10.0f, -10.0f};

    // ========== BOX 1: Super Bouncy ==========
    float delay1 = 0.2f;
    SpringParams bouncy1{.frequency = 4.0f, .decay = 1.5f, .amplitude = 1.0f};
    // Use click time if clicked since last restart, otherwise entrance
    // animation
    float t1 = (box_click_times[0] > trigger_time)
                   ? (time_elapsed - box_click_times[0])
                   : (anim_t - delay1);
    float scale1 = 0.0f + 1.0f * spring_to_target(t1, bouncy1);
    // Add a little squash/stretch
    float squash1 = 1.0f + spring_oscillation(t1, {3.0f, 2.0f, 0.08f});

    float x1 = center_x - spacing * 2.0f - box_size / 2.0f;

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Boing!")
            .with_size(ComponentSize{pixels(box_size + 20), pixels(24)})
            .with_absolute_position(x1 - 10.0f, box_y - 35.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(entity, 11),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(box_size),
                                            pixels(box_size * squash1)})
                   .with_absolute_position(x1, box_y)
                   .with_scale(scale1)
                   .with_custom_background(box_pink)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.3f)
                   .with_debug_name("box1"))) {
      box_click_times[0] = time_elapsed;
      bounce_count++;
    }

    // ========== BOX 2: Jelly ==========
    float delay2 = 0.35f;
    SpringParams jelly{.frequency = 3.0f, .decay = 1.2f, .amplitude = 1.0f};
    float t2 = (box_click_times[1] > trigger_time)
                   ? (time_elapsed - box_click_times[1])
                   : (anim_t - delay2);
    float scale2 = spring_to_target(t2, jelly);
    float wobble2 = spring_oscillation(t2, {2.5f, 1.5f, 4.0f});

    float x2 = center_x - spacing - box_size / 2.0f;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Jelly")
            .with_size(ComponentSize{pixels(box_size + 20), pixels(24)})
            .with_absolute_position(x2 - 10.0f, box_y - 35.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(entity, 21),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
                   .with_absolute_position(x2 + wobble2, box_y)
                   .with_scale(scale2)
                   .with_custom_background(box_cyan)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f) // Circle
                   .with_debug_name("box2"))) {
      box_click_times[1] = time_elapsed;
      bounce_count++;
    }

    // ========== BOX 3: Balloon ==========
    float delay3 = 0.5f;
    float t3 = (box_click_times[2] > trigger_time)
                   ? (time_elapsed - box_click_times[2])
                   : (anim_t - delay3);
    float scale3 = balloon_pop(t3, 0.6f);

    float x3 = center_x - box_size / 2.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Balloon")
            .with_size(ComponentSize{pixels(box_size + 20), pixels(24)})
            .with_absolute_position(x3 - 10.0f, box_y - 35.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(entity, 31),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
                   .with_absolute_position(x3, box_y)
                   .with_scale(scale3)
                   .with_custom_background(box_yellow)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.2f)
                   .with_debug_name("box3"))) {
      box_click_times[2] = time_elapsed;
      bounce_count++;
    }

    // ========== BOX 4: Drop ==========
    float delay4 = 0.65f;
    float t4 = (box_click_times[3] > trigger_time)
                   ? (time_elapsed - box_click_times[3])
                   : (anim_t - delay4);
    float drop_offset =
        -50.0f + 50.0f * spring_to_target(t4, {3.0f, 1.5f, 1.0f});
    drop_offset += spring_oscillation(t4, {4.0f, 2.5f, 8.0f});

    float x4 = center_x + spacing - box_size / 2.0f;

    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Drop")
            .with_size(ComponentSize{pixels(box_size + 20), pixels(24)})
            .with_absolute_position(x4 - 10.0f, box_y - 35.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(entity, 41),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
                   .with_absolute_position(x4, box_y + drop_offset)
                   .with_custom_background(box_lime)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_debug_name("box4"))) {
      box_click_times[3] = time_elapsed;
      bounce_count++;
    }

    // ========== BOX 5: Rocker ==========
    float delay5 = 0.8f;
    float t5 = (box_click_times[4] > trigger_time)
                   ? (time_elapsed - box_click_times[4])
                   : (anim_t - delay5);
    // Pendulum swing - starts tilted right, swings back and forth, settles
    float rock = pendulum_rocker(t5, 12.0f, 1.5f, 1.5f);

    float x5 = center_x + spacing * 2.0f - box_size / 2.0f;

    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Rocker")
            .with_size(ComponentSize{pixels(box_size + 20), pixels(24)})
            .with_absolute_position(x5 - 10.0f, box_y - 35.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    if (button(context, mk(entity, 51),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
                   .with_absolute_position(x5 + rock, box_y)
                   .with_custom_background(box_violet)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.4f)
                   .with_debug_name("box5"))) {
      box_click_times[4] = time_elapsed;
      bounce_count++;
    }

    // ========== Bounce Counter ==========
    float counter_scale = 1.0f;
    static int last_count = 0;
    static float count_bounce_time = -10.0f;
    if (bounce_count != last_count) {
      count_bounce_time = time_elapsed;
      last_count = bounce_count;
    }
    counter_scale = 1.0f + spring_oscillation(time_elapsed - count_bounce_time,
                                              {5.0f, 3.0f, 0.15f});

    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label(fmt::format("Bounces: {}", bounce_count))
            .with_size(ComponentSize{pixels(200), pixels(50)})
            .with_absolute_position(center_x - 100.0f, box_y + box_size + 50.0f)
            .with_scale(counter_scale)
            .with_font(UIComponent::DEFAULT_FONT, h720(24.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // ========== Restart Button ==========
    float btn_scale = elastic_pop(anim_t - 1.0f, 1.08f, 3.0f);
    if (button(context, mk(entity, 100),
               ComponentConfig{}
                   .with_label("Restart!")
                   .with_size(ComponentSize{pixels(180), pixels(50)})
                   .with_absolute_position(center_x - 90.0f, screen_h - 90.0f)
                   .with_scale(btn_scale)
                   .with_background(Theme::Usage::Primary)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.5f)
                   .with_debug_name("restart_btn"))) {
      trigger_time = time_elapsed;
      // Reset click times
      for (int i = 0; i < 5; i++) {
        box_click_times[i] = -10.0f;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(animation_spring, "Animations",
                        "Delightful spring physics with interactive bounces",
                        AnimationSpringDemo)
