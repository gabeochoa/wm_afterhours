#pragma once

#include "../game.h"
#include "../render_backend.h"
#include <afterhours/ah.h>
#include <string>

// Global state for the HUD (simple approach for screen demo)
struct ScreenHUDState {
  static std::string current_screen_name;
  static int current_index;
  static int total_screens;
};

// Initialize in the cpp file
inline std::string ScreenHUDState::current_screen_name = "";
inline int ScreenHUDState::current_index = 0;
inline int ScreenHUDState::total_screens = 0;

struct RenderScreenHUD : afterhours::System<> {
  virtual void once(float) const override {
    if (ScreenHUDState::current_screen_name.empty()) {
      return;
    }

    int screen_height = Settings::get().get_screen_height();

    // Build the HUD text
    std::string hud_text = "--screen=" + ScreenHUDState::current_screen_name;
    std::string nav_text = "< , | . >";

    // Position in bottom-left
    float padding = 10.0f;
    float font_size = 16.0f;
    float y_pos = screen_height - font_size - padding - 5.0f;

    // Draw background rectangle for better readability
    raylib::Vector2 text_size =
        raylib::MeasureTextEx(uiFont, hud_text.c_str(), font_size, 1.0f);
    raylib::Vector2 nav_size =
        raylib::MeasureTextEx(uiFont, nav_text.c_str(), font_size, 1.0f);

    float total_width = text_size.x + 20.0f + nav_size.x;
    raylib::Rectangle bg_rect = {padding - 5.0f, y_pos - 3.0f,
                                 total_width + 10.0f, font_size + 6.0f};

    raylib::DrawRectangleRec(bg_rect, raylib::Color{0, 0, 0, 180});

    // Draw the screen name
    raylib::DrawTextEx(uiFont, hud_text.c_str(),
                       raylib::Vector2{padding, y_pos}, font_size, 1.0f,
                       raylib::Color{200, 200, 200, 255});

    // Draw navigation hint
    raylib::DrawTextEx(uiFont, nav_text.c_str(),
                       raylib::Vector2{padding + text_size.x + 20.0f, y_pos},
                       font_size, 1.0f, raylib::Color{120, 120, 120, 255});

    // Draw screen index (e.g., "3/11")
    std::string index_text = std::to_string(ScreenHUDState::current_index + 1) +
                             "/" +
                             std::to_string(ScreenHUDState::total_screens);
    raylib::Vector2 index_size =
        raylib::MeasureTextEx(uiFont, index_text.c_str(), font_size, 1.0f);
    int screen_width = Settings::get().get_screen_width();
    float index_x = screen_width - index_size.x - padding;

    raylib::Rectangle index_bg = {index_x - 5.0f, y_pos - 3.0f,
                                  index_size.x + 10.0f, font_size + 6.0f};
    raylib::DrawRectangleRec(index_bg, raylib::Color{0, 0, 0, 180});
    raylib::DrawTextEx(uiFont, index_text.c_str(),
                       raylib::Vector2{index_x, y_pos}, font_size, 1.0f,
                       raylib::Color{200, 200, 200, 255});
  }
};

