#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct KartSelectScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_character = 2;
  size_t selected_kart = 0;

  struct Character {
    std::string name;
    std::string label; // fallback icon text
    float speed;
    float accel;
    float handling;
    float weight;
  };

  std::vector<Character> characters = {
      {"Turbo", "T", 0.9f, 0.5f, 0.4f, 0.7f},
      {"Blaze", "B", 0.7f, 0.8f, 0.6f, 0.5f},
      {"Drift", "D", 0.6f, 0.6f, 0.9f, 0.4f},
      {"Tank", "K", 0.4f, 0.3f, 0.5f, 1.0f},
      {"Zippy", "Z", 0.8f, 0.9f, 0.7f, 0.3f},
      {"Rumble", "R", 0.5f, 0.4f, 0.7f, 0.9f},
      {"Flash", "F", 1.0f, 0.6f, 0.3f, 0.6f},
      {"Pebble", "P", 0.3f, 0.7f, 1.0f, 0.5f},
  };

  std::vector<std::string> kart_names = {"Standard", "Speedster", "Off-Road",
                                          "Classic"};

  // Vibrant racing colors
  afterhours::Color bg_blue{25, 35, 65, 255};
  afterhours::Color panel_blue{35, 55, 95, 255};
  afterhours::Color panel_light{55, 80, 130, 255};
  afterhours::Color border_blue{70, 100, 160, 255};
  afterhours::Color accent_yellow{255, 210, 50, 255};
  afterhours::Color accent_orange{255, 150, 50, 255};
  afterhours::Color accent_red{220, 60, 60, 255};
  afterhours::Color accent_green{80, 200, 100, 255};
  afterhours::Color white{255, 255, 255, 255};
  afterhours::Color dark_text{20, 25, 40, 255};
  afterhours::Color muted_text{140, 155, 185, 255};
  afterhours::Color grid_cell_bg{45, 65, 110, 255};
  afterhours::Color grid_cell_selected{255, 210, 50, 255};

  // Character portrait colors (unique per character)
  afterhours::Color portrait_colors[8] = {
      {220, 60, 60, 255},   // red
      {255, 140, 40, 255},  // orange
      {60, 180, 220, 255},  // cyan
      {100, 100, 120, 255}, // gray
      {255, 220, 60, 255},  // yellow
      {140, 80, 180, 255},  // purple
      {60, 200, 120, 255},  // green
      {180, 140, 100, 255}, // tan
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = white;
    theme.darkfont = dark_text;
    theme.font_muted = muted_text;
    theme.background = bg_blue;
    theme.surface = panel_blue;
    theme.primary = border_blue;
    theme.secondary = panel_light;
    theme.accent = accent_yellow;
    theme.error = accent_red;
    theme.roundness = 0.12f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== FULL BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_blue)
            .with_debug_name("bg"));

    // ========== HEADER ==========
    // Title
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("SELECT YOUR RACER")
            .with_size(ComponentSize{pixels(450), pixels(55)})
            .with_absolute_position(30.0f, 15.0f)
            .with_font("Fredoka", h720(38.0f))
            .with_custom_text_color(accent_yellow)
            .with_text_stroke(afterhours::Color{180, 130, 0, 255}, 3.0f)
            .with_debug_name("title"));

    // Race mode indicator
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("Grand Prix - Mushroom Cup")
            .with_size(ComponentSize{pixels(280), pixels(30)})
            .with_absolute_position((float)screen_w - 310.0f, 20.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("mode"));

    // Player indicator
    div(context, mk(entity, 7),
        ComponentConfig{}
            .with_label("P1")
            .with_size(ComponentSize{pixels(50), pixels(32)})
            .with_absolute_position((float)screen_w - 60.0f, 18.0f)
            .with_custom_background(accent_yellow)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(dark_text)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_debug_name("player"));

    // ========== LEFT: CHARACTER GRID ==========
    float grid_x = 30.0f;
    float grid_y = 75.0f;
    float cell_size = 80.0f;
    float cell_gap = 8.0f;
    int cols = 4;

    // Grid panel background
    float grid_panel_w = (float)cols * (cell_size + cell_gap) + cell_gap + 20.0f;
    float grid_panel_h = (float)screen_h - grid_y - 110.0f;

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_720p_size(grid_panel_w, grid_panel_h)
            .with_absolute_position(grid_x, grid_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f, afterhours::Color{0, 0, 0, 80})
            .with_debug_name("grid_panel"));

    // Character grid cells
    for (size_t i = 0; i < characters.size(); i++) {
      int row = static_cast<int>(i) / cols;
      int col = static_cast<int>(i) % cols;
      float cx = grid_x + 10.0f + cell_gap +
                 (float)col * (cell_size + cell_gap);
      float cy = grid_y + 10.0f + cell_gap +
                 (float)row * (cell_size + cell_gap);

      bool is_selected = (i == selected_character);
      afterhours::Color cell_bg =
          is_selected ? grid_cell_selected : grid_cell_bg;
      afterhours::Color cell_border =
          is_selected ? accent_orange : afterhours::Color{60, 80, 130, 255};
      float border_w = is_selected ? 4.0f : 2.0f;

      if (button(
              context, mk(entity, 20 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_720p_size(cell_size, cell_size)
                  .with_absolute_position(cx, cy)
                  .with_custom_background(cell_bg)
                  .with_border(cell_border, border_w)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.15f)
                  .with_debug_name("char_" + std::to_string(i)))) {
        selected_character = i;
      }

      // Character portrait circle
      float portrait_size = 52.0f;
      float portrait_offset = (cell_size - portrait_size) / 2.0f;
      div(context, mk(entity, 30 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(characters[i].label)
              .with_size(
                  ComponentSize{pxf(portrait_size),
                                pxf(portrait_size)})
              .with_absolute_position(cx + portrait_offset, cy + portrait_offset - 6.0f)
              .with_custom_background(portrait_colors[i])
              .with_font("Fredoka", h720(28.0f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("portrait_" + std::to_string(i)));

      // Character name below portrait
      afterhours::Color name_color = is_selected ? dark_text : white;
      div(context, mk(entity, 40 + static_cast<int>(i)),
          ComponentConfig{}
              .with_label(characters[i].name)
              .with_size(ComponentSize{pxf(cell_size),
                                       pixels(22)})
              .with_absolute_position(cx, cy + cell_size - 22.0f)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(name_color)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("name_" + std::to_string(i)));
    }

    // ========== CENTER: CHARACTER PREVIEW ==========
    float preview_x = grid_x + grid_panel_w + 24.0f;
    float preview_y = grid_y;
    float preview_w = 280.0f;
    float preview_h = grid_panel_h;

    // Preview panel
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_720p_size(preview_w, preview_h)
            .with_absolute_position(preview_x, preview_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f,
                              afterhours::Color{0, 0, 0, 80})
            .with_debug_name("preview_panel"));

    // Large character portrait
    auto &sel = characters[selected_character];
    float big_portrait = 110.0f;
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label(sel.label)
            .with_720p_size(big_portrait, big_portrait)
            .with_absolute_position(preview_x + (preview_w - big_portrait) / 2.0f,
                            preview_y + 15.0f)
            .with_custom_background(portrait_colors[selected_character])
            .with_border(accent_yellow, 4.0f)
            .with_font("Fredoka", h720(56.0f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_soft_shadow(2.0f, 3.0f, 10.0f,
                              afterhours::Color{0, 0, 0, 60})
            .with_debug_name("big_portrait"));

    // Character name
    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label(sel.name)
            .with_size(ComponentSize{pxf(preview_w - 20),
                                     pixels(36)})
            .with_absolute_position(preview_x + 10.0f,
                            preview_y + big_portrait + 25.0f)
            .with_font("Fredoka", h720(28.0f))
            .with_custom_text_color(accent_yellow)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("sel_name"));

    // ========== STAT BARS ==========
    struct StatDef {
      std::string label;
      float value;
      afterhours::Color color;
    };
    std::vector<StatDef> stats = {
        {"SPD", sel.speed, accent_red},
        {"ACC", sel.accel, accent_orange},
        {"HND", sel.handling, accent_green},
        {"WGT", sel.weight, {100, 140, 220, 255}},
    };

    float stat_y = preview_y + big_portrait + 58.0f;
    float stat_bar_w = 140.0f;

    for (size_t i = 0; i < stats.size(); i++) {
      float sy = stat_y + (float)i * 28.0f;

      // Label
      div(context, mk(entity, 110 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(stats[i].label)
              .with_size(ComponentSize{pixels(40), pixels(22)})
              .with_absolute_position(preview_x + 20.0f, sy)
              .with_font("EqProRounded", h720(16.0f))
              .with_custom_text_color(white)
              .with_debug_name("stat_label_" + std::to_string(i)));

      // Bar background
      div(context, mk(entity, 111 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(stat_bar_w),
                                       pixels(16)})
              .with_absolute_position(preview_x + 70.0f, sy + 3.0f)
              .with_custom_background(afterhours::Color{25, 35, 60, 255})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.5f)
              .with_debug_name("stat_bg_" + std::to_string(i)));

      // Bar fill
      div(context, mk(entity, 112 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_size(ComponentSize{
                  pxf(stat_bar_w * stats[i].value),
                  pixels(16)})
              .with_absolute_position(preview_x + 70.0f, sy + 3.0f)
              .with_custom_background(stats[i].color)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.5f)
              .with_debug_name("stat_fill_" + std::to_string(i)));
    }

    // ========== RIGHT: KART SELECTION ==========
    float kart_x = preview_x + preview_w + 24.0f;
    float kart_y = grid_y;
    float kart_w = (float)screen_w - kart_x - 30.0f;
    float kart_h = grid_panel_h;

    // Kart panel
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_720p_size(kart_w, kart_h)
            .with_absolute_position(kart_x, kart_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f,
                              afterhours::Color{0, 0, 0, 80})
            .with_debug_name("kart_panel"));

    // Kart header
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("VEHICLE")
            .with_size(
                ComponentSize{pxf(kart_w - 20), pixels(30)})
            .with_absolute_position(kart_x + 10.0f, kart_y + 10.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(accent_yellow)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("kart_header"));

    // Kart options
    for (size_t i = 0; i < kart_names.size(); i++) {
      float ky = kart_y + 50.0f + (float)i * 44.0f;
      bool is_sel = (i == selected_kart);

      afterhours::Color kart_bg =
          is_sel ? accent_yellow : afterhours::Color{40, 60, 105, 255};
      afterhours::Color kart_text = is_sel ? dark_text : white;

      if (button(context, mk(entity, 210 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(kart_names[i])
                     .with_size(ComponentSize{
                         pxf(kart_w - 40), pixels(38)})
                     .with_absolute_position(kart_x + 20.0f, ky)
                     .with_custom_background(kart_bg)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(kart_text)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.3f)
                     .with_debug_name("kart_" + std::to_string(i)))) {
        selected_kart = i;
      }
    }

    // ========== BOTTOM: ACTION BAR ==========
    float bottom_y = (float)screen_h - 90.0f;

    // Bottom bar background
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w - 60), pixels(70)})
            .with_absolute_position(30.0f, bottom_y)
            .with_custom_background(panel_blue)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("bottom_bar"));

    // Back button
    button(context, mk(entity, 310),
           ComponentConfig{}
               .with_label("< Back")
               .with_size(ComponentSize{pixels(120), pixels(50)})
               .with_absolute_position(50.0f, bottom_y + 10.0f)
               .with_custom_background(afterhours::Color{60, 80, 130, 255})
               .with_border(border_blue, 2.0f)
               .with_font("EqProRounded", h720(22.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("back_btn"));

    // Selected info
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_label(sel.name + "  +  " + kart_names[selected_kart])
            .with_size(ComponentSize{pixels(400), pixels(36)})
            .with_absolute_position((float)screen_w / 2.0f - 200.0f, bottom_y + 17.0f)
            .with_font("EqProRounded", h720(24.0f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("selection_info"));

    // Ready button
    button(context, mk(entity, 330),
           ComponentConfig{}
               .with_label("READY!")
               .with_size(ComponentSize{pixels(160), pixels(50)})
               .with_absolute_position((float)screen_w - 210.0f, bottom_y + 10.0f)
               .with_custom_background(accent_green)
               .with_border(afterhours::Color{50, 160, 70, 255}, 3.0f)
               .with_font("Fredoka", h720(28.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_soft_shadow(2.0f, 3.0f, 10.0f,
                                 afterhours::Color{0, 0, 0, 60})
               .with_debug_name("ready_btn"));

    // ========== BOTTOM PROMPTS ==========
    float prompt_y = (float)screen_h - 35.0f;
    div(context, mk(entity, 340),
        ComponentConfig{}
            .with_label("A: Select   B: Back   L/R: Switch Kart")
            .with_size(ComponentSize{pixels(400), pixels(18)})
            .with_absolute_position((float)screen_w / 2.0f - 200.0f, prompt_y)
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("prompts"));
  }
};

REGISTER_EXAMPLE_SCREEN(kart_select, "Game Mockups",
                        "Racing game character and kart selection",
                        KartSelectScreen)
