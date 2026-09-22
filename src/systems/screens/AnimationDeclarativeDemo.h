#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui_motion.h>
namespace um = afterhours::ui_motion;
using afterhours::ui_motion::HasMotionState;
using afterhours::ui_motion::MotionProperty;
#include <array>

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
  std::string last_trigger = "none";

  static afterhours::motion::Spring spring(float freq, float decay) {
    return afterhours::motion::Spring::from_freq_decay(freq, decay);
  }
  static afterhours::motion::Timeline ease_out(float seconds) {
    return {.keys = {{0.f, 0.f}, {seconds, 1.f}}, .curve = afterhours::motion::curves::ease_out_quad};
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    using afterhours::motion::Timeline;
    // Set dt for animation updates
    context.dt = dt;

    // Setup theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    const float s = std::min(screen_w / 1280.f, screen_h / 720.f);
    const float top = (screen_h - 720.f * s) / 2.f;
    const float left = (screen_w - 1112.f * s) / 2.f;
    context.scaling_mode = ScalingMode::Proportional;
    const auto muted = afterhours::Color{180, 190, 210, 255};
    const auto box = [s, top, left](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(left + x * s, top + y * s)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    int label_id = 200;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float size, afterhours::Color color, bool emphasis = false) {
      div(context, mk(entity, label_id++), box(x, y, w, 28).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    };
    // Background
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(screen_w), pixels(screen_h)}).with_custom_background(bg_dark).with_corner_radius(0)
        .with_debug_name("bg"));
    div(context, mk(entity, 1), box(0, 24, 1112, 92).with_custom_background({36, 43, 57, 255})
        .with_corner_radius(12 * s));
    // Title
    div(context, mk(entity, 2), box(20, 34, 860, 40).with_label("Declarative animations")
        .with_font("AtkinsonMock", pixels(32 * s)).with_custom_text_color(text_light).with_ignore_pointer_events());
    // Subtitle
    label("Each tile declares a trigger and the properties it animates.", 20, 78, 860, 20, muted);
    const bool replay_entry = button(context, mk(entity, 3), box(872, 45, 220, 44)
        .with_label("Replay entry").with_font("AtkinsonMock", pixels(22 * s))
        .with_custom_background({60, 103, 173, 255}).with_custom_text_color(text_light).with_corner_radius(8 * s)
        .with_debug_name("replay_entry"));
    const std::array<const char *, 3> section_names{"01  Triggers", "02  Motion", "03  Property combinations"};
    for (int row = 0; row < 3; ++row) {
      label(section_names[row], 0, 126 + row * 162, 500, 23, text_light);
      for (int col = 0; col < 4; ++col)
        div(context, mk(entity, 4 + row * 4 + col + 300), box(col * 282.f, 158 + row * 162, 266, 128)
            .with_custom_background({32, 38, 51, 255}).with_corner_radius(8 * s));
    }
    label(fmt::format("Clicks: {}  /  Last: {}", click_count, last_trigger), 520, 126, 624, 19, muted);
    float button_size = 164.f * s;
    float button_height = 64.f * s;
    float button_y = top + 170.f * s;
    float spacing = 282.f * s;
    float center_x = screen_w / 2.f;

    // ========== BUTTON 1: Hover Scale ==========
    // Using button() so hover state is tracked
    float btn1_x = center_x - spacing * 1.5f - button_size / 2.0f;

    auto hover = button(
        context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Hover scale").with_debug_name("decl_0")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn1_x, button_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.scale = 1.2f}, spring(10.0f, 6.0f)))
            .with(um::on_hover({.translate_y = -8.0f * s}, spring(12.0f, 7.0f))));

    // ========== BUTTON 2: Click Scale ==========
    float btn2_x = center_x - spacing * 0.5f - button_size / 2.0f;

    auto click = button(context, mk(entity, 20),
               ComponentConfig{}
                   .with_label("Click to scale")
                   .with_size(
                       ComponentSize{pixels(button_size), pixels(button_height)})
                   .with_absolute_position(btn2_x, button_y)
                   .with_custom_background(box_green)
                   .with_custom_text_color(text_light)
                   .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_corner_radius(9.f * s)
                   .with(um::on_press({.scale = 0.85f}, spring(15.0f, 10.0f)))
                   .with_debug_name("click_btn"));

    // ========== BUTTON 3: Both Hover + Click ==========
    float btn3_x = center_x + spacing * 0.5f - button_size / 2.0f;

    auto both = button(context, mk(entity, 30),
               ComponentConfig{}
                   .with_label("Hover + click")
                   .with_size(
                       ComponentSize{pixels(button_size), pixels(button_height)})
                   .with_absolute_position(btn3_x, button_y)
                   .with_custom_background(box_purple)
                   .with_custom_text_color(text_light)
                   .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
                   .with_rounded_corners(RoundedCorners())
                   .with_corner_radius(9.f * s)
                   .with(um::on_hover({.translate_y = -8.0f * s}, ease_out(0.08f)))
                   .with(um::on_press({.scale = 0.9f}, spring(18.0f, 12.0f)))
                   .with_debug_name("both_btn"));

    // ========== BUTTON 4: Appear Animation ==========
    float btn4_x = center_x + spacing * 1.5f - button_size / 2.0f;

    auto appear = div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Appear").with_debug_name("decl_3")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn4_x, button_y)
            .with_custom_background(box_orange)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_appear({.scale = {0.3f, 1.0f}}, spring(10.0f, 6.0f)))
            .with(um::on_appear({.opacity = {0.0f, 1.0f}}, ease_out(0.2f))));

    // Second row - different animation types (all buttons for hover tracking)
    float row2_y = button_y + 162.f * s;

    // ========== Translate X on hover ==========
    auto slide = button(
        context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Slide").with_debug_name("decl_4")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn1_x, row2_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.translate_x = 20.0f * s}, spring(15.0f, 8.0f))));

    // ========== Bounce on hover ==========
    auto bounce = button(
        context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Bounce").with_debug_name("decl_5")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn2_x, row2_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.translate_y = -15.0f * s}, spring(12.0f, 5.0f))));

    // ========== Fade on hover ==========
    auto fade = button(
        context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Fade").with_debug_name("decl_6")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn3_x, row2_y)
            .with_custom_background(box_blue)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.opacity = 0.6f}, ease_out(0.1f))));

    // ========== Loop animation ==========
    auto pulse = div(context, mk(entity, 80),
        ComponentConfig{}
            .with_label("Pulse").with_debug_name("decl_7")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn4_x, row2_y)
            .with_custom_background(box_orange)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(21.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_appear({.scale = {0.95f, 1.05f}},
                       Timeline{.keys = {{0.f, 0.f}, {0.4f, 1.f}},
                                .repeat = Timeline::Repeat::PingPong,
                                .curve = afterhours::motion::curves::ease_in_out_quad})));

    // Third row - composition tests (slide+rotate combinations)
    float row3_y = row2_y + 162.f * s;

    // ========== Slide then Rotate (translate_x + rotate) ==========
    auto slide_rotate = button(
        context, mk(entity, 110),
        ComponentConfig{}
            .with_label("Slide + rotate").with_debug_name("decl_8")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn1_x, row3_y)
            .with_custom_background(box_purple)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(20.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.translate_x = 15.0f * s}, spring(10.0f, 6.0f)))
            .with(um::on_hover({.rotation = 5.0f}, spring(12.0f, 7.0f))));

    // ========== Rotate then Slide (rotate + translate_x) ==========
    auto rotate_slide = button(
        context, mk(entity, 120),
        ComponentConfig{}
            .with_label("Rotate + slide").with_debug_name("decl_9")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn2_x, row3_y)
            .with_custom_background(box_purple)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(20.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.rotation = -5.0f}, spring(12.0f, 7.0f)))
            .with(um::on_hover({.translate_x = -15.0f * s}, spring(10.0f, 6.0f))));

    // ========== All combined: scale + translate + rotate ==========
    auto combined = button(
        context, mk(entity, 130),
        ComponentConfig{}
            .with_label("Scale + lift + rotate").with_debug_name("decl_10")
            .with_size(ComponentSize{pixels(button_size), pixels(button_height)})
            .with_absolute_position(btn3_x, row3_y)
            .with_custom_background(box_purple)
            .with_custom_text_color(text_light)
            .with_font("AtkinsonMock", pixels(20.f * s))
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(9.f * s)
            .with(um::on_hover({.scale = 1.15f}, spring(10.0f, 6.0f)))
            .with(um::on_hover({.translate_y = -10.0f * s}, spring(12.0f, 7.0f)))
            .with(um::on_hover({.rotation = 3.0f}, spring(14.0f, 8.0f))));

    const std::array<const char *, 11> names{"Hover", "Click", "Hover + click", "Appear", "Slide", "Bounce", "Fade", "Pulse", "Slide + rotate", "Rotate + slide", "Combined"};
    const std::array<const char *, 11> captions{
        "Hover: 100-120%; lift 8 px", "Press: 100-85%; spring 15/10", "Hover: lift 8 px; press: 90%", "On entry: 30-100%; fade in",
        "Hover: right 20 px; spring 15/8", "Hover: up 15 px; spring 12/5", "Hover: 100-60% opacity", "Loop: 95-105%; ease in/out",
        "1. Right 15 px  2. Rotate +5 deg", "1. Rotate -5 deg  2. Left 15 px", "115%; up 10 px; rotate +3 deg"};
    std::array<afterhours::Entity *, 11> tiles{&hover.ent(), &click.ent(), &both.ent(), &appear.ent(), &slide.ent(), &bounce.ent(), &fade.ent(), &pulse.ent(), &slide_rotate.ent(), &rotate_slide.ent(), &combined.ent()};
    for (int i = 0; i < 11; ++i) {
      const float x = (i % 4) * 282.f;
      const float y = (i / 4) * 162.f;
      label(captions[i], x + 12, 236 + y, 250, 16, muted);
      bool moving = false;
      if (tiles[i]->has<afterhours::motion::HasTracks>())
        for (const auto &[key, tr] : tiles[i]->get<afterhours::motion::HasTracks>().floats) moving |= tr.active();
      const std::string phase = i == 7 ? "Looping" : moving ? "Animating" : i == 3 ? "Entry complete" : context.was_hot(tiles[i]->id) ? "Hovered" : "Ready";
      label(phase, x + 12, 256 + y, 250, 16, muted);
      if (i < 8) continue;
      div(context, mk(entity, 400 + i), box(x + 51, 494, 164, 64)
          .with_on_draw_fg([s](RectangleType r) { raylib::DrawRectangleLinesEx(r, s, {148, 161, 188, 255}); })
          .with_ignore_pointer_events());
    }
    if (replay_entry) {
      appear.ent().removeComponentIfExists<HasMotionState>();
      appear.ent().removeComponentIfExists<afterhours::motion::HasTracks>();
    }
    const std::array<bool, 11> activated{bool(hover), bool(click), bool(both), false, bool(slide), bool(bounce), bool(fade), false, bool(slide_rotate), bool(rotate_slide), bool(combined)};
    for (int i = 0; i < 11; ++i) {
      if (!activated[i]) continue;
      ++click_count;
      last_trigger = names[i];
    }
    label("Blue: hover / green: press", 858, 482, 242, 17, text_light);
    label("Purple: mixed / orange: auto", 858, 506, 242, 16, muted);
    label("Scale, translate, then rotate.", 858, 541, 242, 17, text_light);
    label("Numbers give declaration order.", 858, 565, 242, 16, muted);
    div(context, mk(entity, 100), box(0, 636, 1112, 62).with_custom_background({36, 43, 57, 255}).with_corner_radius(8 * s));
    label("Hover tile configuration", 16, 637, 1100, 18, muted);
    label(".with(um::on_hover({.scale = 1.2f}, Spring::from_freq_decay(10.0f, 6.0f)))", 16, 663, 1100, 23, text_light, true);
  }
};

REGISTER_EXAMPLE_SCREEN(animation_declarative, "Animations",
                        "Declarative animations with trigger blocks",
                        AnimationDeclarativeDemo)
