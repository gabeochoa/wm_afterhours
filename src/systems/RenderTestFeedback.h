#pragma once

#include "../external.h"
#include "../testing/test_feedback.h"
#include "../testing/test_input.h"
#include <afterhours/ah.h>

struct RenderTestFeedback
    : afterhours::System<afterhours::ui::UIContext<InputAction>> {

  void for_each_with(afterhours::Entity & /*entity*/,
                     afterhours::ui::UIContext<InputAction> & /*context*/,
                     float dt) override {
    // Only render if test mode is active
    if (!test_input::test_mode) {
      return;
    }

    test_feedback::update_toasts(dt);

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Render fake mouse cursor if simulation is active
    if (test_input::mouse_state.simulation_active &&
        test_input::mouse_state.position.has_value()) {
      vec2 pos = test_input::mouse_state.position.value();

      // Draw cursor circle
      float cursor_size = 15.0f;
      afterhours::Color cursor_color =
          test_input::mouse_state.left_button_held
              ? afterhours::Color{255, 50, 50, 200}  // Red when clicking
              : afterhours::Color{50, 200, 50, 200}; // Green when moving

      raylib::DrawCircle(
          static_cast<int>(pos.x), static_cast<int>(pos.y), cursor_size,
          {cursor_color.r, cursor_color.g, cursor_color.b, cursor_color.a});

      // Draw outline
      raylib::DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y),
                              cursor_size, raylib::RAYWHITE);

      // Draw crosshair
      raylib::DrawLine(static_cast<int>(pos.x) - 5, static_cast<int>(pos.y),
                       static_cast<int>(pos.x) + 5, static_cast<int>(pos.y),
                       raylib::RAYWHITE);
      raylib::DrawLine(static_cast<int>(pos.x), static_cast<int>(pos.y) - 5,
                       static_cast<int>(pos.x), static_cast<int>(pos.y) + 5,
                       raylib::RAYWHITE);
    }

    // Render toasts
    float toast_y = screen_height - 50.0f;
    for (auto it = test_feedback::toasts.rbegin();
         it != test_feedback::toasts.rend(); ++it) {
      const auto &toast = *it;

      // Calculate opacity based on remaining time
      float alpha = 1.0f;
      if (toast.duration - toast.time_alive < 0.5f) {
        alpha = (toast.duration - toast.time_alive) / 0.5f;
      }

      afterhours::Color bg_color = {50, 50, 50,
                                    static_cast<unsigned char>(200 * alpha)};
      if (toast.color == "success")
        bg_color = {50, 150, 50, static_cast<unsigned char>(200 * alpha)};
      if (toast.color == "error")
        bg_color = {150, 50, 50, static_cast<unsigned char>(200 * alpha)};

      int text_width = raylib::MeasureText(toast.message.c_str(), 20);
      int padding = 10;
      int rect_width = text_width + padding * 2;
      int rect_height = 30;
      int rect_x = (screen_width - rect_width) / 2;

      raylib::DrawRectangle(rect_x, static_cast<int>(toast_y) - rect_height,
                            rect_width, rect_height,
                            {bg_color.r, bg_color.g, bg_color.b, bg_color.a});

      raylib::DrawText(
          toast.message.c_str(), rect_x + padding,
          static_cast<int>(toast_y) - rect_height + 5, 20,
          {255, 255, 255, static_cast<unsigned char>(255 * alpha)});

      toast_y -= (rect_height + 5);
    }
  }
};
