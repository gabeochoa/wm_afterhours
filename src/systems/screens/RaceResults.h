#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/GradientBackground.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/grid.h>
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
      {1, "Zippy", "2:31.450", 15, false}, {2, "YOU", "2:33.812", 12, true},
      {3, "Blaze", "2:35.100", 10, false}, {4, "Drift", "2:37.445", 8, false},
      {5, "Tank", "2:41.220", 6, false},   {6, "Flash", "2:42.890", 5, false},
      {7, "Rumble", "2:45.110", 4, false}, {8, "Pebble", "2:48.330", 3, false},
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
      {"YOU", 38, true},
      {"Zippy", 42, false},
      {"Blaze", 30, false},
      {"Drift", 25, false},
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
  afterhours::Color muted{155, 168, 200, 255}; // Brighter for 4.5:1 on dark
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
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(16.0f));
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

    // Checkered flag decorative stripe at top (two rows, offset)
    float stripe_h = 8.0f;
    int stripe_count = screen_w / 16;
    for (int i = 0; i < stripe_count; i++) {
      int row = i % 2; // row 0: even positions, row 1: odd positions
      div(context, mk(entity, 1 + row * stripe_count + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(16), pxf(stripe_h)})
              .with_absolute_position((float)i * 16.0f, (float)row * stripe_h)
              .with_custom_background(white));
    }

    // ========== HEADER ==========
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("RACE RESULTS")
            .with_size(ComponentSize{pixels(400), pixels(50)})
            .with_absolute_position(30.0f, 18.0f)
            .with_font("Fredoka", h720(36.0f))
            .with_custom_text_color(gold)
            .with_text_stroke(afterhours::Color{160, 120, 0, 255}, 3.0f));

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
            .with_alignment(TextAlignment::Right));

    // ========== MAIN RESULTS TABLE ==========
    float table_x = 30.0f;
    float table_y = 70.0f;
    float table_w = (float)screen_w * 0.6f;
    float table_h = (float)screen_h - table_y - 90.0f;

    // Table panel
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_720p_size(table_w, table_h)
            .with_absolute_position(table_x, table_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f, afterhours::Color{0, 0, 0, 80})
            .with_debug_name("table_panel"));

    // The table as an actual grid. It used to be four hand-computed column
    // positions (col_pos_x .. col_pts_x) with every cell absolutely placed,
    // and the header widths had already drifted from the body's: 40/120/140/60
    // against 44/200/140/60. Declaring the tracks once makes that impossible.
    auto table = grid(
        context, mk(entity, 201),
        GridConfig{}
            .with_rows(1 + static_cast<int>(results.size()))
            .with_cols(4)
            .with_col_widths({pxf(55.f), pxf(table_w - 295.f), pxf(140.f),
                              pxf(70.f)})
            .with_row_height(h720(44)),
        ComponentConfig{}
            .with_720p_size(table_w - 20, table_h - 30)
            .with_absolute_position(table_x + 10.0f, table_y + 10.0f)
            .with_debug_name("results_table"));

    struct ColHeader {
      const char *label;
      TextAlignment align;
    };
    static const ColHeader headers[] = {
        {"#", TextAlignment::Left},
        {"RACER", TextAlignment::Left},
        {"TIME", TextAlignment::Right},
        {"PTS", TextAlignment::Right},
    };
    for (int c = 0; c < 4; c++)
      grid_cell(context, table, 0, c,
                ComponentConfig{}
                    .with_label(headers[c].label)
                    .with_alignment(headers[c].align)
                    .with_custom_text_color(muted)
                    .with_background(Theme::Usage::None)
                    .with_debug_name(fmt::format("rr_head_{}", c)));

    for (size_t i = 0; i < results.size(); i++) {
      auto &r = results[i];
      const int row = 1 + static_cast<int>(i);
      const auto row_bg =
          r.is_player ? player_highlight
                      : (i % 2 == 0 ? afterhours::Color{30, 35, 55, 255}
                                    : afterhours::Color{0, 0, 0, 0});

      const auto cell = [&](int c, const std::string &text,
                            TextAlignment align, afterhours::Color text_color,
                            float font_px) {
        grid_cell(context, table, row, c,
                  ComponentConfig{}
                      .with_label(text)
                      .with_alignment(align)
                      .with_font("EqProRounded", h720(font_px))
                      .with_custom_text_color(text_color)
                      .with_custom_background(row_bg)
                      .with_debug_name(fmt::format("rr_{}_{}", row, c)));
      };

      cell(0, std::to_string(r.position) + position_suffix(r.position),
           TextAlignment::Left, position_color(r.position), 20.f);
      cell(1, r.is_player ? "> " + r.name : r.name, TextAlignment::Left,
           r.is_player ? gold : white, r.is_player ? 20.f : 18.f);
      cell(2, r.time, TextAlignment::Right, white, 17.f);
      cell(3, "+" + std::to_string(r.points), TextAlignment::Right,
           accent_green, 18.f);
    }


    // ========== RIGHT: CUP STANDINGS ==========
    float cup_x = table_x + table_w + 20.0f;
    float cup_y = table_y;
    float cup_w = (float)screen_w - cup_x - 30.0f;
    float cup_h = (table_h - 15.0f) * 0.65f;

    // Cup panel
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_720p_size(cup_w, cup_h)
            .with_absolute_position(cup_x, cup_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f, afterhours::Color{0, 0, 0, 80})
            .with_debug_name("cup_panel"));

    // Cup header
    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("CUP STANDINGS")
            .with_size(ComponentSize{pxf(cup_w - 20), pixels(30)})
            .with_absolute_position(cup_x + 10.0f, cup_y + 10.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(gold)
            .with_alignment(TextAlignment::Center));

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
              .with_custom_text_color(position_color(static_cast<int>(i + 1))));

      // Name
      div(context, mk(entity, 411 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(s.name)
              .with_size(ComponentSize{pixels(120), pixels(28)})
              .with_absolute_position(cup_x + 50.0f, sy)
              .with_font("EqProRounded", h720(20.0f))
              .with_custom_text_color(s.is_player ? gold : white));

      // Points
      div(context, mk(entity, 412 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(std::to_string(s.total_points) + " pts")
              .with_size(ComponentSize{pixels(80), pixels(28)})
              .with_absolute_position(cup_x + cup_w - 100.0f, sy)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(muted)
              .with_alignment(TextAlignment::Right));
    }

    // ========== RIGHT: BEST LAP ==========
    float lap_y = cup_y + cup_h + 15.0f;
    float lap_h = 100.0f;

    div(context, mk(entity, 450),
        ComponentConfig{}
            .with_720p_size(cup_w, lap_h)
            .with_absolute_position(cup_x, lap_y)
            .with_custom_background(panel_dark)
            .with_border(border_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("lap_panel"));

    struct LapLine {
      const char *label;
      int id;
      int h;
      float y_off;
      const char *font;
      float font_sz;
      afterhours::Color color;
    };
    LapLine lap_lines[] = {
        {"BEST LAP", 451, 24, 12.0f, "EqProRounded", 16.0f, muted},
        {"0:48.220", 452, 40, 40.0f, "Fredoka", 32.0f, accent_green},
        {"New Personal Best!", 453, 20, 75.0f, "EqProRounded", 14.0f, gold},
    };
    for (auto &ll : lap_lines) {
      div(context, mk(entity, ll.id),
          ComponentConfig{}
              .with_label(ll.label)
              .with_size(ComponentSize{pxf(cup_w - 20), pixels(ll.h)})
              .with_absolute_position(cup_x + 10.0f, lap_y + ll.y_off)
              .with_font(ll.font, h720(ll.font_sz))
              .with_custom_text_color(ll.color)
              .with_alignment(TextAlignment::Center));
    }

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
               .with_roundness(0.3f));

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
                                 afterhours::Color{0, 0, 0, 60}));

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
               .with_roundness(0.3f));

    // Controller prompts
    div(context, mk(entity, 530),
        ComponentConfig{}
            .with_label("A: Select   B: Back   X: Replay")
            .with_size(ComponentSize{pixels(400), pixels(22)})
            .with_absolute_position((float)screen_w / 2.0f - 200.0f,
                                    (float)screen_h - 25.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(race_results, "Game Mockups",
                        "Racing game post-race results and standings",
                        RaceResultsScreen)
