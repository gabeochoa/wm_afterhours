#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Pointer parity: every button is polled directly here -- down, press
// edge, release edge -- plus both wheel axes. The right button used to
// have no synthetic press edge at all, and platform polling only
// consulted the injector for button 0, so a scripted right/middle click
// was invisible to code polling like this. The E2E verbs click /
// right_click / middle_click / scroll_wheel drive these same counters.
struct PointerParityLab : ScreenSystem<UIContext<InputAction>> {
  int presses[3] = {0, 0, 0};
  int releases[3] = {0, 0, 0};
  float wheel_total_x = 0.f;
  float wheel_total_y = 0.f;
  float wheel_last_x = 0.f;
  float wheel_last_y = 0.f;
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));

    bool down[3];
    for (int b = 0; b < 3; ++b) {
      down[b] = input::is_mouse_button_down(b);
      if (input::is_mouse_button_pressed(b))
        ++presses[b];
      if (input::is_mouse_button_released(b))
        ++releases[b];
    }
    const auto wheel = input::get_mouse_wheel_move_v();
    if (wheel.x != 0.f || wheel.y != 0.f) {
      wheel_last_x = wheel.x;
      wheel_last_y = wheel.y;
    }
    wheel_total_x += wheel.x;
    wheel_total_y += wheel.y;

    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("ppl_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    text(0, "Pointer parity lab", 16, 0, 700, 44, 30);
    text(1, "Click with each button anywhere. All three report down, press and release edges; the wheel reports both axes.",
         16, 48, 1150, 28, 19);

    static const char *names[3] = {"Left (0)", "Right (1)", "Middle (2)"};
    static const char *tags[3] = {"Left", "Right", "Middle"};
    for (int b = 0; b < 3; ++b) {
      const float x = 32.f + static_cast<float>(b) * 280.f;
      div(context, mk(root.ent(), 10 + b), ComponentConfig{}
          .with_size({pixels(250 * scale), pixels(120 * scale)})
          .with_absolute_position(x * scale, 120 * scale)
          .with_background(down[b] ? Theme::Usage::Primary : Theme::Usage::Surface)
          .with_corner_radius(8 * scale).with_debug_name("ppl_pad_" + std::to_string(b)));
      text(20 + b, names[b], x + 16, 132, 220, 30, 21);
      text(30 + b, down[b] ? "DOWN" : "up", x + 16, 164, 220, 28, 19,
           "ppl_down_" + std::to_string(b));
      text(40 + b, std::string(tags[b]) + " presses: " +
                       std::to_string(presses[b]) +
                       "   releases: " + std::to_string(releases[b]),
           x + 16, 196, 230, 28, 17, "ppl_counts_" + std::to_string(b));
    }

    text(50, "Wheel", 32, 290, 300, 30, 21);
    text(51, "last delta:  x " + std::to_string(wheel_last_x) +
                 "   y " + std::to_string(wheel_last_y),
         32, 330, 700, 28, 19, "ppl_wheel_last");
    text(52, "accumulated:  x " + std::to_string(wheel_total_x) +
                 "   y " + std::to_string(wheel_total_y),
         32, 364, 700, 28, 19, "ppl_wheel_total");
    text(53, "Scripted equivalents: click / right_click / middle_click x y, scroll_wheel dx dy.",
         32, 430, 1100, 28, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(pointer_parity_lab, "System Demos",
                        "all mouse buttons + wheel axes, polled directly", PointerParityLab)
