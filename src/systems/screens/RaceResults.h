#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/GradientBackground.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RaceResultsScreen : ScreenSystem<UIContext<InputAction>> {
  struct RaceEntry {
    int position;
    std::string name;
    std::string time;
    int points;
    bool is_player;
  };

  std::vector<RaceEntry> results = {
      {1, "Zippy", "2:31.450", 15, false},
      {2, "YOU", "2:33.812", 12, true},
      {3, "Blaze", "2:35.100", 10, false},
      {4, "Drift", "2:37.445", 8, false},
      {5, "Tank", "2:41.220", 6, false},
      {6, "Flash", "2:42.890", 5, false},
      {7, "Rumble", "2:45.110", 4, false},
      {8, "Pebble", "2:48.330", 3, false},
  };

  std::string track_name = "Rainbow Circuit";
  std::string cup_name = "Mushroom Cup";
  int race_number = 3;
  int total_races = 4;

  // Cup standings
  struct CupStanding {
    std::string name;
    int total_points;
    bool is_player;
  };

  std::vector<CupStanding> standings = {
      {"YOU", 38, true},    {"Zippy", 42, false},
      {"Blaze", 30, false}, {"Drift", 25, false},
  };

  // Colors - celebratory racing theme
  afterhours::Color bg_dark{15, 18, 30, 255};
  afterhours::Color panel_dark{25, 30, 50, 255};
  afterhours::Color panel_blue{35, 45, 75, 255};
  afterhours::Color border_blue{55, 70, 120, 255};
  afterhours::Color gold{255, 210, 50, 255};
  afterhours::Color silver{200, 210, 225, 255};
  afterhours::Color bronze{205, 140, 70, 255};
  afterhours::Color white{255, 255, 255, 255};
  afterhours::Color muted{120, 135, 170, 255};
  afterhours::Color player_highlight{60, 90, 160, 255};
  afterhours::Color accent_green{80, 200, 100, 255};
  afterhours::Color accent_red{220, 70, 70, 255};

  afterhours::Color position_color(int pos) {
    switch (pos) {
    case 1:
      return gold;
    case 2:
      return silver;
    case 3:
      return bronze;
    default:
      return muted;
    }
  }

  std::string position_suffix(int pos) {
    switch (pos) {
    case 1:
      return "st";
    case 2:
      return "nd";
    case 3:
      return "rd";
    default:
      return "th";
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = white;
    theme.darkfont = bg_dark;
    theme.font_muted = muted;
    theme.background = bg_dark;
    theme.surface = panel_dark;
    theme.primary = border_blue;
    theme.secondary = panel_blue;
    theme.accent = gold;
    theme.error = accent_red;
    theme.roundness = 0.1f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Checkered flag decorative stripe at top
    float stripe_h = 8.0f;
    int stripe_count = screen_w / 16;
    for (int i = 0; i < stripe_count; i++) {
      // Row 0: white squares at even positions
      if (i % 2 == 0) {
        div(context, mk(entity, 1 + i),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(16),
                                         pxf(stripe_h)})
                .with_absolute_position((float)i * 16.0f, 0.0f)
                .with_custom_background(white)
                .with_debug_name("checker_r0_" + std::to_string(i)));
      }
      // Row 1: white squares at odd positions
      if (i % 2 == 1) {
        div(context, mk(entity, 1 + stripe_count + i),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(16),
                                         pxf(stripe_h)})
                .with_absolute_position((float)i * 16.0f, stripe_h)
                .with_custom_background(white)
                .with_debug_name("checker_r1_" + std::to_string(i)));
      }
    }

    // ========== HEADER ==========
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("RACE RESULTS")
            .with_size(ComponentSize{pixels(400), pixels(50)})
            .with_absolute_position(30.0f, 18.0f)
            .with_font("Fredoka", h720(36.0f))
            .with_custom_text_color(gold)
            .with_text_stroke(afterhours::Color{160, 120, 0, 255}, 3.0f)
            .with_debug_name("title"));

    // Track and race info
    div(context, mk(entity, 101),
        ComponentConfig{}
            .with_label(track_name + "  |  " + cup_name + " - Race " +
                        std::to_string(race_number) + "/" +
                        std::to_string(total_races))
            .with_size(ComponentSize{pixels(500), pixels(28)})
            .with_absolute_position((float)screen_w - 530.0f, 18.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("track_info"));

    // ========== MAIN RESULTS TABLE ==========
    float table_x = 30.0f;
    float table_y = 70.0f;
    float table_w = (float)screen_w * 0.6f;
    float table_h = (float)screen_h - table_y - 90.0f;

    // Table panel
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(table_w),
                                     pxf(table_h)})
            .with_absolute_position(table_x, table_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f,
                              afterhours::Color{0, 0, 0, 80})
            .with_debug_name("table_panel"));

    // Column headers
    float col_pos_x = table_x + 15.0f;
    float col_name_x = table_x + 65.0f;
    float col_time_x = table_x + table_w - 240.0f;
    float col_pts_x = table_x + table_w - 80.0f;
    float header_y = table_y + 10.0f;

    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("#")
            .with_size(ComponentSize{pixels(40), pixels(24)})
            .with_absolute_position(col_pos_x, header_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_debug_name("col_pos"));

    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_label("RACER")
            .with_size(ComponentSize{pixels(120), pixels(24)})
            .with_absolute_position(col_name_x, header_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_debug_name("col_name"));

    div(context, mk(entity, 203),
        ComponentConfig{}
            .with_label("TIME")
            .with_size(ComponentSize{pixels(120), pixels(24)})
            .with_absolute_position(col_time_x, header_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_debug_name("col_time"));

    div(context, mk(entity, 204),
        ComponentConfig{}
            .with_label("PTS")
            .with_size(ComponentSize{pixels(60), pixels(24)})
            .with_absolute_position(col_pts_x, header_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("col_pts"));

    // Separator
    div(context, mk(entity, 205),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(table_w - 30),
                                     pixels(1)})
            .with_absolute_position(table_x + 15.0f, header_y + 28.0f)
            .with_custom_background(border_blue)
            .with_debug_name("header_sep"));

    // Results rows
    float row_start_y = header_y + 38.0f;
    float row_h = 50.0f;

    for (size_t i = 0; i < results.size(); i++) {
      auto &r = results[i];
      float ry = row_start_y + (float)i * row_h;

      // Alternating row background for even-indexed non-player rows
      if (i % 2 == 0 && !r.is_player) {
        div(context, mk(entity, 260 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(table_w - 20), pxf(row_h - 4)})
                .with_absolute_position(table_x + 10.0f, ry - 2.0f)
                .with_custom_background(afterhours::Color{30, 35, 55, 255})
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.1f)
                .with_debug_name("alt_row_" + std::to_string(i)));
      }

      // Row highlight for player
      if (r.is_player) {
        div(context, mk(entity, 210 + static_cast<int>(i) * 5),
            ComponentConfig{}
                .with_size(
                    ComponentSize{pxf(table_w - 20),
                                  pxf(row_h - 4)})
                .with_absolute_position(table_x + 10.0f, ry - 2.0f)
                .with_custom_background(player_highlight)
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.15f)
                .with_debug_name("player_row"));
      }

      // Position number
      div(context, mk(entity, 211 + static_cast<int>(i) * 5),
          ComponentConfig{}
              .with_label(std::to_string(r.position) +
                          position_suffix(r.position))
              .with_size(ComponentSize{pixels(44), pixels(32)})
              .with_absolute_position(col_pos_x, ry + 6.0f)
              .with_font("Fredoka", h720(22.0f))
              .with_custom_text_color(position_color(r.position))
              .with_debug_name("pos_" + std::to_string(i)));

      // Name
      div(context, mk(entity, 212 + static_cast<int>(i) * 5),
          ComponentConfig{}
              .with_label(r.name)
              .with_size(ComponentSize{pixels(200), pixels(32)})
              .with_absolute_position(col_name_x, ry + 6.0f)
              .with_font("EqProRounded",
                          h720(r.is_player ? 22.0f : 20.0f))
              .with_custom_text_color(r.is_player ? gold : white)
              .with_debug_name("name_" + std::to_string(i)));

      // Time
      div(context, mk(entity, 213 + static_cast<int>(i) * 5),
          ComponentConfig{}
              .with_label(r.time)
              .with_size(ComponentSize{pixels(140), pixels(28)})
              .with_absolute_position(col_time_x, ry + 8.0f)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(white)
              .with_debug_name("time_" + std::to_string(i)));

      // Points
      div(context, mk(entity, 214 + static_cast<int>(i) * 5),
          ComponentConfig{}
              .with_label("+" + std::to_string(r.points))
              .with_size(ComponentSize{pixels(60), pixels(28)})
              .with_absolute_position(col_pts_x, ry + 8.0f)
              .with_font("EqProRounded", h720(20.0f))
              .with_custom_text_color(accent_green)
              .with_alignment(TextAlignment::Right)
              .with_debug_name("pts_" + std::to_string(i)));
    }

    // ========== RIGHT: CUP STANDINGS ==========
    float cup_x = table_x + table_w + 20.0f;
    float cup_y = table_y;
    float cup_w = (float)screen_w - cup_x - 30.0f;
    float cup_h = (table_h - 15.0f) * 0.65f;

    // Cup panel
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(cup_w),
                                     pxf(cup_h)})
            .with_absolute_position(cup_x, cup_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f,
                              afterhours::Color{0, 0, 0, 80})
            .with_debug_name("cup_panel"));

    // Cup header
    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("CUP STANDINGS")
            .with_size(ComponentSize{pxf(cup_w - 20),
                                     pixels(30)})
            .with_absolute_position(cup_x + 10.0f, cup_y + 10.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(gold)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("cup_header"));

    // Standings rows
    for (size_t i = 0; i < standings.size(); i++) {
      auto &s = standings[i];
      float sy = cup_y + 50.0f + (float)i * 46.0f;

      // Position
      div(context, mk(entity, 410 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(std::to_string(i + 1) + ".")
              .with_size(ComponentSize{pixels(30), pixels(28)})
              .with_absolute_position(cup_x + 15.0f, sy)
              .with_font("EqProRounded", h720(20.0f))
              .with_custom_text_color(position_color(static_cast<int>(i + 1)))
              .with_debug_name("cup_pos_" + std::to_string(i)));

      // Name
      div(context, mk(entity, 411 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(s.name)
              .with_size(ComponentSize{pixels(120), pixels(28)})
              .with_absolute_position(cup_x + 50.0f, sy)
              .with_font("EqProRounded", h720(20.0f))
              .with_custom_text_color(s.is_player ? gold : white)
              .with_debug_name("cup_name_" + std::to_string(i)));

      // Points
      div(context, mk(entity, 412 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(std::to_string(s.total_points) + " pts")
              .with_size(ComponentSize{pixels(80), pixels(28)})
              .with_absolute_position(cup_x + cup_w - 100.0f, sy)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(muted)
              .with_alignment(TextAlignment::Right)
              .with_debug_name("cup_pts_" + std::to_string(i)));
    }

    // ========== RIGHT: BEST LAP ==========
    float lap_y = cup_y + cup_h + 15.0f;
    float lap_h = 100.0f;

    div(context, mk(entity, 450),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(cup_w),
                                     pxf(lap_h)})
            .with_absolute_position(cup_x, lap_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("lap_panel"));

    div(context, mk(entity, 451),
        ComponentConfig{}
            .with_label("BEST LAP")
            .with_size(ComponentSize{pxf(cup_w - 20),
                                     pixels(24)})
            .with_absolute_position(cup_x + 10.0f, lap_y + 12.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lap_header"));

    div(context, mk(entity, 452),
        ComponentConfig{}
            .with_label("0:48.220")
            .with_size(ComponentSize{pxf(cup_w - 20),
                                     pixels(40)})
            .with_absolute_position(cup_x + 10.0f, lap_y + 40.0f)
            .with_font("Fredoka", h720(32.0f))
            .with_custom_text_color(accent_green)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lap_time"));

    div(context, mk(entity, 453),
        ComponentConfig{}
            .with_label("New Personal Best!")
            .with_size(ComponentSize{pxf(cup_w - 20),
                                     pixels(20)})
            .with_absolute_position(cup_x + 10.0f, lap_y + 75.0f)
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(gold)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("lap_best"));

    // ========== BOTTOM ACTIONS ==========
    float bottom_y = (float)screen_h - 80.0f;

    // Replay button
    button(context, mk(entity, 500),
           ComponentConfig{}
               .with_label("Replay")
               .with_size(ComponentSize{pixels(140), pixels(50)})
               .with_absolute_position(30.0f, bottom_y)
               .with_custom_background(panel_blue)
               .with_border(border_blue, 2.0f)
               .with_font("EqProRounded", h720(22.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("replay_btn"));

    // Next Race button
    button(context, mk(entity, 510),
           ComponentConfig{}
               .with_label("Next Race >>")
               .with_size(ComponentSize{pixels(200), pixels(50)})
               .with_absolute_position((float)screen_w - 230.0f, bottom_y)
               .with_custom_background(accent_green)
               .with_border(afterhours::Color{50, 160, 70, 255}, 3.0f)
               .with_font("Fredoka", h720(24.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_soft_shadow(2.0f, 3.0f, 10.0f,
                                 afterhours::Color{0, 0, 0, 60})
               .with_debug_name("next_btn"));

    // Quit button
    button(context, mk(entity, 520),
           ComponentConfig{}
               .with_label("Quit Cup")
               .with_size(ComponentSize{pixels(120), pixels(50)})
               .with_absolute_position(210.0f, bottom_y)
               .with_custom_background(afterhours::Color{100, 40, 40, 255})
               .with_border(accent_red, 2.0f)
               .with_font("EqProRounded", h720(20.0f))
               .with_custom_text_color(white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("quit_btn"));

    // Controller prompts
    div(context, mk(entity, 530),
        ComponentConfig{}
            .with_label("A: Select   B: Back   X: Replay")
            .with_size(ComponentSize{pixels(400), pixels(20)})
            .with_absolute_position((float)screen_w / 2.0f - 200.0f, (float)screen_h - 25.0f)
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("prompts"));
  }
};

REGISTER_EXAMPLE_SCREEN(race_results, "Game Mockups",
                        "Racing game post-race results and standings",
                        RaceResultsScreen)
