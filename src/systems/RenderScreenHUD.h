#pragma once

#include "../game.h"
#include "../render_backend.h"
#include <afterhours/ah.h>
#include <string>

// Global state for the HUD (simple approach for screen demo)
struct ScreenHUDState {
  static std::string current_screen_name;
  static std::string current_screen_description;
  static int current_index;
  static int total_screens;
};

// Initialize in the cpp file
inline std::string ScreenHUDState::current_screen_name = "";
inline std::string ScreenHUDState::current_screen_description = "";
inline int ScreenHUDState::current_index = 0;
inline int ScreenHUDState::total_screens = 0;

struct RenderScreenHUD : afterhours::System<> {
  virtual void once(float) const override {
    if (ScreenHUDState::current_screen_name.empty()) {
      return;
    }

    int screen_height = Settings::get().get_screen_height();
    int screen_width = Settings::get().get_screen_width();

    // Build the HUD text
    std::string hud_text = "--screen=" + ScreenHUDState::current_screen_name;
    std::string nav_text = "< , | . >";

    // Font sizes
    float font_size = 16.0f;
    float desc_font_size = 14.0f;

    // Calculate positions (bottom-left, with description below screen name)
    float padding = 10.0f;
    float line_height = font_size + 4.0f;
    float desc_line_height = desc_font_size + 2.0f;

    // If we have a description, the HUD is two lines tall
    bool has_description = !ScreenHUDState::current_screen_description.empty();
    float total_height =
        has_description ? (line_height + desc_line_height) : line_height;

    float y_pos = screen_height - total_height - padding;

    // Draw background rectangle for better readability
    raylib::Vector2 text_size =
        raylib::MeasureTextEx(uiFont, hud_text.c_str(), font_size, 1.0f);
    raylib::Vector2 nav_size =
        raylib::MeasureTextEx(uiFont, nav_text.c_str(), font_size, 1.0f);

    float top_line_width = text_size.x + 20.0f + nav_size.x;

    // Measure description if present
    float desc_width = 0.0f;
    if (has_description) {
      raylib::Vector2 desc_size = raylib::MeasureTextEx(
          uiFont, ScreenHUDState::current_screen_description.c_str(),
          desc_font_size, 1.0f);
      desc_width = desc_size.x;
    }

    float bg_width = std::max(top_line_width, desc_width) + 10.0f;
    raylib::Rectangle bg_rect = {padding - 5.0f, y_pos - 3.0f, bg_width,
                                 total_height + 6.0f};

    raylib::DrawRectangleRec(bg_rect, raylib::Color{0, 0, 0, 180});

    // Draw the screen name
    raylib::DrawTextEx(uiFont, hud_text.c_str(),
                       raylib::Vector2{padding, y_pos}, font_size, 1.0f,
                       raylib::Color{200, 200, 200, 255});

    // Draw navigation hint
    raylib::DrawTextEx(uiFont, nav_text.c_str(),
                       raylib::Vector2{padding + text_size.x + 20.0f, y_pos},
                       font_size, 1.0f, raylib::Color{120, 120, 120, 255});

    // Draw description on second line (muted color)
    if (has_description) {
      float desc_y = y_pos + line_height;
      raylib::DrawTextEx(uiFont,
                         ScreenHUDState::current_screen_description.c_str(),
                         raylib::Vector2{padding, desc_y}, desc_font_size, 1.0f,
                         raylib::Color{160, 160, 160, 255});
    }

    // Draw screen index (e.g., "3/11") - positioned at top right of HUD area
    std::string index_text = std::to_string(ScreenHUDState::current_index + 1) +
                             "/" +
                             std::to_string(ScreenHUDState::total_screens);
    raylib::Vector2 index_size =
        raylib::MeasureTextEx(uiFont, index_text.c_str(), font_size, 1.0f);
    float index_x = screen_width - index_size.x - padding;

    raylib::Rectangle index_bg = {index_x - 5.0f, y_pos - 3.0f,
                                  index_size.x + 10.0f, font_size + 6.0f};
    raylib::DrawRectangleRec(index_bg, raylib::Color{0, 0, 0, 180});
    raylib::DrawTextEx(uiFont, index_text.c_str(),
                       raylib::Vector2{index_x, y_pos}, font_size, 1.0f,
                       raylib::Color{200, 200, 200, 255});
  }
};
