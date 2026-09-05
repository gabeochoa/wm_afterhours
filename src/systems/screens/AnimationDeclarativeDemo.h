#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AnimationDeclarativeDemo : ScreenSystem<UIContext<InputAction>> {
  // Theme colors
  afterhours::Color bg_dark{24, 24, 32, 255};
  afterhours::Color text_light{248, 250, 252, 255};
  afterhours::Color box_blue{59, 130, 246, 255};
  afterhours::Color box_green{34, 197, 94, 255};
  afterhours::Color box_purple{168, 85, 247, 255};
  afterhours::Color box_orange{249, 115, 22, 255};

  int click_count = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    // Set dt for animation updates
    context.dt = dt;

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
            .with_label("Declarative Animations")
            .with_size(ComponentSize{pixels(screen_w), pixels(60)})
            .with_absolute_position(0.0f, 30.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Subtitle
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("No boilerplate - animations declared inline!")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, 90.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Layout constants
    float button_size = 100.0f;
    float button_y = 180.0f;
    float spacing = 130.0f;
    float center_x = screen_w / 2.0f;

    // ========== BUTTON 1: Hover Scale ==========
    // Using button() so hover state is tracked
    float btn1_x = center_x - spacing * 1.5f - button_size / 2.0f;

    button(
        context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Hover")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn1_x, button_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().scale(1.0f, 1.2f).spring(10.0f, 6.0f))
            .with_animation(
                Anim::on_hover().translate_y(0.0f, -8.0f).spring(12.0f, 7.0f)));

    // ========== BUTTON 2: Click Scale ==========
    float btn2_x = center_x - spacing * 0.5f - button_size / 2.0f;

    if (button(context, mk(entity, 20),
               ComponentConfig{}
                   .with_label("Click!")
                   .with_size(
                       ComponentSize{pixels(button_size), pixels(button_size)})
                   .with_absolute_position(btn2_x, button_y)
                   .with_custom_background(box_green)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_animation(
                       Anim::on_click().scale(1.0f, 0.85f).spring(15.0f, 10.0f))
                   .with_debug_name("click_btn"))) {
      click_count++;
    }

    // ========== BUTTON 3: Both Hover + Click ==========
    float btn3_x = center_x + spacing * 0.5f - button_size / 2.0f;

    if (button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("Both!")
                   .with_size(
                       ComponentSize{pixels(button_size), pixels(button_size)})
                   .with_absolute_position(btn3_x, button_y)
                   .with_custom_background(box_purple)
                   .with_custom_text_color(text_light)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.15f)
                   .with_animation(
                       Anim::on_hover().scale(1.0f, 1.1f).ease_out(0.08f))
                   .with_animation(
                       Anim::on_click().scale(1.0f, 0.9f).spring(18.0f, 12.0f))
                   .with_debug_name("both_btn"))) {
      click_count++;
    }

    // ========== BUTTON 4: Appear Animation ==========
    float btn4_x = center_x + spacing * 1.5f - button_size / 2.0f;

    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Appear")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn4_x, button_y)
            .with_custom_background(box_orange)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_appear().scale(0.3f, 1.0f).spring(10.0f, 6.0f))
            .with_animation(
                Anim::on_appear().opacity(0.0f, 1.0f).ease_out(0.2f)));

    // Second row - different animation types (all buttons for hover tracking)
    float row2_y = button_y + button_size + 60.0f;

    // ========== Translate X on hover ==========
    button(
        context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Slide")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn1_x, row2_y)
            .with_custom_background(box_green)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().translate_x(0.0f, 20.0f).spring(15.0f, 8.0f)));

    // ========== Bounce on hover ==========
    button(
        context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Bounce")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn2_x, row2_y)
            .with_custom_background(box_purple)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(Anim::on_hover()
                                .translate_y(0.0f, -15.0f)
                                .spring(12.0f, 5.0f)));

    // ========== Fade on hover ==========
    button(
        context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Fade")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn3_x, row2_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().opacity(1.0f, 0.6f).ease_out(0.1f)));

    // ========== Loop animation ==========
    div(context, mk(entity, 80),
        ComponentConfig{}
            .with_label("Pulse")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn4_x, row2_y)
            .with_custom_background(box_orange)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::loop().scale(0.95f, 1.05f).ease_in_out(0.4f)));

    // Third row - composition tests (slide+rotate combinations)
    float row3_y = row2_y + button_size + 60.0f;

    // ========== Slide then Rotate (translate_x + rotate) ==========
    button(
        context, mk(entity, 110),
        ComponentConfig{}
            .with_label("Slide+Rot")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn1_x, row3_y)
            .with_custom_background(box_purple)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().translate_x(0.0f, 15.0f).spring(10.0f, 6.0f))
            .with_animation(
                Anim::on_hover().rotate(0.0f, 5.0f).spring(12.0f, 7.0f)));

    // ========== Rotate then Slide (rotate + translate_x) ==========
    button(
        context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Rot+Slide")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn2_x, row3_y)
            .with_custom_background(box_green)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().rotate(0.0f, -5.0f).spring(12.0f, 7.0f))
            .with_animation(Anim::on_hover()
                                .translate_x(0.0f, -15.0f)
                                .spring(10.0f, 6.0f)));

    // ========== All combined: scale + translate + rotate ==========
    button(
        context, mk(entity, 130),
        ComponentConfig{}
            .with_label("All 3")
            .with_size(ComponentSize{pixels(button_size), pixels(button_size)})
            .with_absolute_position(btn3_x, row3_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_animation(
                Anim::on_hover().scale(1.0f, 1.15f).spring(10.0f, 6.0f))
            .with_animation(
                Anim::on_hover().translate_y(0.0f, -10.0f).spring(12.0f, 7.0f))
            .with_animation(
                Anim::on_hover().rotate(0.0f, 3.0f).spring(14.0f, 8.0f)));

    // Click counter display
    div(context, mk(entity, 90),
        ComponentConfig{}
            .with_label(fmt::format("Total clicks: {}", click_count))
            .with_size(ComponentSize{pixels(200), pixels(40)})
            .with_absolute_position(center_x - 100.0f,
                                    row3_y + button_size + 20.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Code comparison
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label(
                "Old: ~50 lines of boilerplate | New: "
                ".with_animation(Anim::on_hover().scale(1.1f).spring())")
            .with_size(ComponentSize{pixels(screen_w), pixels(30)})
            .with_absolute_position(0.0f, screen_h - 50.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(animation_declarative, "Animations",
                        "Declarative animations with .with_animation() API",
                        AnimationDeclarativeDemo)
