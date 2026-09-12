#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MinesweeperLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kSize = 16;
  static constexpr int kMines = 40;

  enum struct Phase { Playing, Lost, Won };

  bool mine[kSize][kSize]{};
  int neighbours[kSize][kSize]{};
  bool revealed[kSize][kSize]{};
  bool flagged[kSize][kSize]{};
  bool laid_out = false;
  bool window_open = true;
  bool minimized = false;
  bool maximized = false;
  bool start_menu_open = false;
  Phase phase = Phase::Playing;
  int flags_left = kMines;
  float elapsed_seconds = 0.f;
  std::string status_message =
      "Left click to reveal. Right click to mark a mine.";

  const afterhours::Color desktop_teal{0, 128, 128, 255};
  const afterhours::Color win_gray{192, 192, 192, 255};
  const afterhours::Color win_light{255, 255, 255, 255};
  const afterhours::Color win_mid{128, 128, 128, 255};
  const afterhours::Color win_dark{51, 51, 51, 255};
  const afterhours::Color title_blue{0, 0, 128, 255};
  const afterhours::Color black{0, 0, 0, 255};

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size(ComponentSize{pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None);
  }

  static void draw_bevel(RectangleType r, afterhours::Color fill, bool raised,
                         float thickness) {
    const auto top_left = raised ? afterhours::Color{255, 255, 255, 255}
                                 : afterhours::Color{128, 128, 128, 255};
    const auto bottom_right = raised ? afterhours::Color{65, 65, 65, 255}
                                     : afterhours::Color{255, 255, 255, 255};
    afterhours::draw_rectangle(r, fill);
    afterhours::draw_rectangle({r.x, r.y, r.width, thickness}, top_left);
    afterhours::draw_rectangle({r.x, r.y, thickness, r.height}, top_left);
    afterhours::draw_rectangle(
        {r.x, r.y + r.height - thickness, r.width, thickness}, bottom_right);
    afterhours::draw_rectangle(
        {r.x + r.width - thickness, r.y, thickness, r.height}, bottom_right);
  }

  static void draw_bomb(RectangleType r, afterhours::Color color) {
    const float s = std::min(r.width, r.height) / 24.f;
    const float cx = r.x + r.width * 0.5f;
    const float cy = r.y + r.height * 0.55f;
    for (int i = 0; i < 4; ++i) {
      const float dx = i % 2 == 0 ? 1.f : 0.f;
      const float dy = i % 2 == 0 ? 0.f : 1.f;
      afterhours::draw_line_ex({cx - dx * 10.f * s, cy - dy * 10.f * s},
                               {cx + dx * 10.f * s, cy + dy * 10.f * s},
                               2.f * s, color);
    }
    afterhours::draw_line_ex({cx - 7.f * s, cy - 7.f * s},
                             {cx + 7.f * s, cy + 7.f * s}, 2.f * s, color);
    afterhours::draw_line_ex({cx + 7.f * s, cy - 7.f * s},
                             {cx - 7.f * s, cy + 7.f * s}, 2.f * s, color);
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 7.f * s,
                            color);
    afterhours::draw_rectangle(
        {cx - 4.f * s, cy - 4.f * s, 4.f * s, 4.f * s},
        afterhours::Color{255, 255, 255, 255});
  }

  static void draw_flag(RectangleType r, float scale) {
    const float cx = r.x + r.width * 0.5f;
    const float top = r.y + 5.f * scale;
    afterhours::draw_line_ex({cx, top}, {cx, r.y + 20.f * scale}, 2.f * scale,
                             afterhours::Color{0, 0, 0, 255});
    afterhours::draw_triangle({cx, top}, {cx - 9.f * scale, top + 5.f * scale},
                              {cx, top + 10.f * scale},
                              afterhours::Color{220, 0, 0, 255});
    afterhours::draw_line_ex({cx - 7.f * scale, r.y + 21.f * scale},
                             {cx + 7.f * scale, r.y + 21.f * scale},
                             2.f * scale, afterhours::Color{0, 0, 0, 255});
  }

  static void draw_smiley(RectangleType r) {
    const float s = std::min(r.width, r.height) / 44.f;
    const float cx = r.x + r.width * 0.5f;
    const float cy = r.y + r.height * 0.5f;
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            13.f * s, afterhours::Color{255, 255, 0, 255});
    afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy),
                                  13.f * s, afterhours::Color{0, 0, 0, 255});
    afterhours::draw_rectangle({cx - 6.f * s, cy - 5.f * s, 2.f * s, 5.f * s},
                               afterhours::Color{0, 0, 0, 255});
    afterhours::draw_rectangle({cx + 4.f * s, cy - 5.f * s, 2.f * s, 5.f * s},
                               afterhours::Color{0, 0, 0, 255});
    afterhours::draw_line_ex({cx - 7.f * s, cy + 3.f * s}, {cx, cy + 8.f * s},
                             2.f * s, afterhours::Color{0, 0, 0, 255});
    afterhours::draw_line_ex({cx, cy + 8.f * s}, {cx + 7.f * s, cy + 3.f * s},
                             2.f * s, afterhours::Color{0, 0, 0, 255});
  }

  static void draw_monitor(RectangleType r) {
    const float s = r.width / 48.f;
    draw_bevel({r.x + 3.f * s, r.y, 36.f * s, 30.f * s},
               afterhours::Color{192, 192, 192, 255}, true, 2.f * s);
    afterhours::draw_rectangle(
        {r.x + 7.f * s, r.y + 4.f * s, 28.f * s, 20.f * s},
        afterhours::Color{0, 0, 128, 255});
    afterhours::draw_rectangle(
        {r.x + 16.f * s, r.y + 30.f * s, 13.f * s, 6.f * s},
        afterhours::Color{192, 192, 192, 255});
    afterhours::draw_rectangle(
        {r.x + 9.f * s, r.y + 37.f * s, 33.f * s, 5.f * s},
        afterhours::Color{192, 192, 192, 255});
  }

  static void draw_windows_mark(RectangleType r) {
    const float s = std::min(r.width, r.height) / 16.f;
    const float x = r.x + (r.width - 13.f * s) * 0.5f;
    const float y = r.y + (r.height - 13.f * s) * 0.5f;
    const afterhours::Color dark{0, 96, 96, 255};
    const afterhours::Color bright{0, 160, 128, 255};
    afterhours::draw_rectangle({x, y, 6.f * s, 6.f * s}, bright);
    afterhours::draw_rectangle({x + 7.f * s, y, 6.f * s, 6.f * s}, dark);
    afterhours::draw_rectangle({x, y + 7.f * s, 6.f * s, 6.f * s}, dark);
    afterhours::draw_rectangle(
        {x + 7.f * s, y + 7.f * s, 6.f * s, 6.f * s}, bright);
  }

  void reset() {
    for (int r = 0; r < kSize; ++r)
      for (int c = 0; c < kSize; ++c) {
        mine[r][c] = false;
        neighbours[r][c] = 0;
        revealed[r][c] = false;
        flagged[r][c] = false;
      }

    unsigned state = 0x5EEDu;
    int placed = 0;
    while (placed < kMines) {
      state = state * 1664525u + 1013904223u;
      const int r = static_cast<int>((state >> 16) % kSize);
      state = state * 1664525u + 1013904223u;
      const int c = static_cast<int>((state >> 16) % kSize);
      if (mine[r][c])
        continue;
      mine[r][c] = true;
      ++placed;
    }

    for (int r = 0; r < kSize; ++r)
      for (int c = 0; c < kSize; ++c)
        for (int dr = -1; dr <= 1; ++dr)
          for (int dc = -1; dc <= 1; ++dc) {
            const int nr = r + dr;
            const int nc = c + dc;
            if (nr >= 0 && nc >= 0 && nr < kSize && nc < kSize && mine[nr][nc])
              ++neighbours[r][c];
          }

    phase = Phase::Playing;
    flags_left = kMines;
    elapsed_seconds = 0.f;
    laid_out = true;

    for (int r = 0; r < kSize; ++r) {
      bool done = false;
      for (int c = 0; c < kSize; ++c) {
        if (!mine[r][c] && neighbours[r][c] == 0) {
          reveal(r, c);
          done = true;
          break;
        }
      }
      if (done)
        break;
    }

    int flagged_count = 0;
    for (int r = 0; r < kSize && flagged_count < 2; ++r)
      for (int c = 0; c < kSize && flagged_count < 2; ++c)
        if (mine[r][c] && !revealed[r][c]) {
          flagged[r][c] = true;
          --flags_left;
          ++flagged_count;
        }
    status_message = "Left click to reveal. Right click to mark a mine.";
  }

  void reveal(int row, int col) {
    if (revealed[row][col] || flagged[row][col])
      return;
    if (mine[row][col]) {
      revealed[row][col] = true;
      phase = Phase::Lost;
      status_message = "BOOM! Click the smiley face for a new game.";
      return;
    }

    std::vector<std::pair<int, int>> stack{{row, col}};
    while (!stack.empty()) {
      const auto [r, c] = stack.back();
      stack.pop_back();
      if (r < 0 || c < 0 || r >= kSize || c >= kSize)
        continue;
      if (revealed[r][c] || flagged[r][c] || mine[r][c])
        continue;
      revealed[r][c] = true;
      if (neighbours[r][c] != 0)
        continue;
      for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc)
          if (dr != 0 || dc != 0)
            stack.push_back({r + dr, c + dc});
    }
    check_win();
    if (phase == Phase::Playing)
      status_message = fmt::format("{} squares open.", revealed_count());
  }

  void check_win() {
    for (int r = 0; r < kSize; ++r)
      for (int c = 0; c < kSize; ++c)
        if (!mine[r][c] && !revealed[r][c])
          return;
    phase = Phase::Won;
    status_message = "You win! Every safe square is clear.";
  }

  int revealed_count() const {
    int count = 0;
    for (int r = 0; r < kSize; ++r)
      for (int c = 0; c < kSize; ++c)
        count += revealed[r][c] ? 1 : 0;
    return count;
  }

  static afterhours::Color count_color(int n) {
    switch (n) {
    case 1:
      return {0, 0, 255, 255};
    case 2:
      return {0, 128, 0, 255};
    case 3:
      return {255, 0, 0, 255};
    case 4:
      return {0, 0, 128, 255};
    case 5:
      return {128, 0, 0, 255};
    case 6:
      return {0, 128, 128, 255};
    case 7:
      return {0, 0, 0, 255};
    default:
      return {128, 128, 128, 255};
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    if (!laid_out)
      reset();
    if (phase == Phase::Playing && window_open && !minimized)
      elapsed_seconds = std::min(999.f, elapsed_seconds + dt);

    Theme theme;
    theme.font = black;
    theme.darkfont = win_light;
    theme.background = win_gray;
    theme.surface = win_gray;
    theme.primary = win_gray;
    theme.secondary = win_gray;
    theme.accent = title_blue;
    theme.roundness = 0.f;
    theme.segments = 4;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(16.f));

    const float scale =
        context.screen_height > 0.f ? context.screen_height / 720.f : 1.f;
    auto root =
        div(context, mk(entity, 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_on_draw_bg([color = desktop_teal](RectangleType r) {
                  afterhours::draw_rectangle(r, color);
                })
                .with_debug_name("ms_root"));

    div(context, mk(root.ent(), 1),
        box(scale, 51.f, 26.f, 48.f, 44.f)
            .with_on_draw_bg([](RectangleType r) { draw_monitor(r); })
            .with_debug_name("ms_desktop_icon"));
    div(context, mk(root.ent(), 2),
        box(scale, 24.f, 75.f, 95.f, 18.f)
            .with_label("My Computer")
            .with_font("AtkinsonMock", pixels(16.f * scale))
            .with_custom_text_color(win_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("ms_desktop_label"));

    const float window_x = maximized ? 0.f : 406.f;
    const float window_y = maximized ? 0.f : 23.f;
    const float window_w = maximized ? 1280.f : 468.f;
    const float window_h = maximized ? 681.f : 629.f;

    if (window_open && !minimized) {
      auto window =
          div(context, mk(root.ent(), 10),
              box(scale, window_x, window_y, window_w, window_h)
                  .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                    draw_bevel(r, color, true, 2.f * scale);
                  })
                  .with_debug_name("ms_window"));

      div(context, mk(window.ent(), 0),
          box(scale, 6.f, 6.f, window_w - 12.f, 28.f)
              .with_on_draw_bg([color = title_blue](RectangleType r) {
                afterhours::draw_rectangle(r, color);
              })
              .with_debug_name("ms_titlebar"));
      div(context, mk(window.ent(), 1),
          box(scale, 8.f, 9.f, 21.f, 21.f)
              .with_on_draw_bg([scale](RectangleType r) {
                draw_bevel(r, afterhours::Color{192, 192, 192, 255}, true,
                           1.f * scale);
                draw_bomb(r, afterhours::Color{0, 0, 0, 255});
              })
              .with_debug_name("ms_title_icon"));
      div(context, mk(window.ent(), 2),
          box(scale, 32.f, 7.f, window_w - 120.f, 26.f)
              .with_label("Minesweeper")
              .with_font("ArchivoMockBold", pixels(18.f * scale))
              .with_custom_text_color(win_light)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("ms_title"));

      auto title_button = [&](int id, const std::string &label, float x,
                              const std::string &name) {
        return button(
            context, mk(window.ent(), id),
            box(scale, x, 9.f, 22.f, 22.f)
                .with_label(label)
                .with_font("ArchivoMockBold", pixels(16.f * scale))
                .with_custom_text_color(black)
                .with_alignment(TextAlignment::Center)
                .with_click_activation(ClickActivationMode::Release)
                .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                  draw_bevel(r, color, true, 2.f * scale);
                })
                .with_debug_name(name));
      };

      if (title_button(3, "_", window_w - 79.f, "ms_minimize"))
        minimized = true;
      if (title_button(4, "□", window_w - 55.f, "ms_maximize")) {
        maximized = !maximized;
        status_message = maximized ? "Window maximized." : "Window restored.";
      }
      if (title_button(5, "×", window_w - 31.f, "ms_close")) {
        window_open = false;
        minimized = false;
      }

      if (button(context, mk(window.ent(), 6),
                 box(scale, 8.f, 36.f, 54.f, 24.f)
                     .with_label("Game")
                     .with_font("AtkinsonMock", pixels(19.f * scale))
                     .with_custom_text_color(black)
                     .with_alignment(TextAlignment::Center)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("ms_game_menu"))) {
        reset();
      }
      if (button(context, mk(window.ent(), 7),
                 box(scale, 68.f, 36.f, 45.f, 24.f)
                     .with_label("Help")
                     .with_font("AtkinsonMock", pixels(19.f * scale))
                     .with_custom_text_color(black)
                     .with_alignment(TextAlignment::Center)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("ms_help"))) {
        status_message =
            "Reveal a square. Right-click to flag. Clear every safe square.";
      }

      div(context, mk(window.ent(), 8),
          box(scale, 6.f, 64.f, window_w - 12.f, 531.f)
              .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                draw_bevel(r, color, true, 3.f * scale);
              })
              .with_debug_name("ms_game"));
      div(context, mk(window.ent(), 9),
          box(scale, 21.f, 78.f, 426.f, 68.f)
              .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                draw_bevel(r, color, false, 3.f * scale);
              })
              .with_debug_name("ms_score"));

      div(context, mk(window.ent(), 11),
          box(scale, 32.f, 89.f, 141.f, 45.f)
              .with_label(fmt::format("{:03d}", flags_left))
              .with_font("DGOneMock", pixels(42.f * scale))
              .with_custom_text_color(afterhours::Color{255, 0, 0, 255})
              .with_custom_background(black)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("ms_mines"));

      if (button(
              context, mk(window.ent(), 12),
              box(scale, 212.f, 90.f, 44.f, 44.f)
                  .with_background(Theme::Usage::None)
                  .with_click_activation(ClickActivationMode::Release)
                  .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                    draw_bevel(r, color, true, 3.f * scale);
                  })
                  .with_on_draw_fg([](RectangleType r) { draw_smiley(r); })
                  .with_debug_name("ms_reset"))) {
        reset();
      }

      div(context, mk(window.ent(), 13),
          box(scale, 295.f, 89.f, 141.f, 45.f)
              .with_label(
                  fmt::format("{:03d}", static_cast<int>(elapsed_seconds)))
              .with_font("DGOneMock", pixels(42.f * scale))
              .with_custom_text_color(afterhours::Color{255, 0, 0, 255})
              .with_custom_background(black)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("ms_timer"));

      auto board =
          div(context, mk(window.ent(), 14),
              box(scale, 21.f, 158.f, 422.f, 422.f)
                  .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                    draw_bevel(r, color, false, 3.f * scale);
                  })
                  .with_debug_name("ms_board"));

      for (int row = 0; row < kSize; ++row) {
        for (int col = 0; col < kSize; ++col) {
          const bool open = revealed[row][col];
          const bool lost_mine = phase == Phase::Lost && mine[row][col];
          const bool has_flag = flagged[row][col] && !lost_mine;
          const int nearby = neighbours[row][col];
          const std::string face = open && !mine[row][col] && nearby > 0
                                       ? std::to_string(nearby)
                                       : "";

          auto cell = button(
              context, mk(board.ent(), row * kSize + col),
              box(scale, 3.f + static_cast<float>(col) * 26.f,
                  3.f + static_cast<float>(row) * 26.f, 26.f, 26.f)
                  .with_label(face)
                  .with_font("AtkinsonMock", pixels(24.f * scale))
                  .with_custom_text_color(count_color(nearby))
                  .with_alignment(TextAlignment::Center)
                  .with_click_activation(ClickActivationMode::Release)
                  .with_skip_tabbing(true)
                  .with_on_draw_bg([open, lost_mine, color = win_gray,
                                    scale](RectangleType r) {
                    if (open || lost_mine) {
                      afterhours::draw_rectangle(
                          r, lost_mine ? afterhours::Color{255, 0, 0, 255}
                                       : color);
                      afterhours::draw_rectangle(
                          {r.x + r.width - scale, r.y, scale, r.height},
                          afterhours::Color{128, 128, 128, 255});
                      afterhours::draw_rectangle(
                          {r.x, r.y + r.height - scale, r.width, scale},
                          afterhours::Color{128, 128, 128, 255});
                    } else {
                      draw_bevel(r, color, true, 3.f * scale);
                    }
                  })
                  .with_on_draw_fg(
                      [has_flag, lost_mine, scale](RectangleType r) {
                        if (has_flag)
                          draw_flag(r, scale);
                        if (lost_mine)
                          draw_bomb(r, afterhours::Color{0, 0, 0, 255});
                      })
                  .with_debug_name(fmt::format("ms_{}_{}", row, col)));

          if (phase != Phase::Playing)
            continue;
          if (cell)
            reveal(row, col);
          if (context.is_right_click(cell.ent().id) && !revealed[row][col]) {
            if (flagged[row][col]) {
              flagged[row][col] = false;
              ++flags_left;
              status_message = "Flag removed.";
            } else if (flags_left > 0) {
              flagged[row][col] = true;
              --flags_left;
              status_message = "Flag placed.";
            }
          }
        }
      }

      div(context, mk(window.ent(), 15),
          box(scale, 6.f, 598.f, window_w - 12.f, 25.f)
              .with_label(status_message)
              .with_font("AtkinsonMock", pixels(16.f * scale))
              .with_custom_text_color(black)
              .with_alignment(TextAlignment::Left)
              .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                draw_bevel(r, color, false, 1.f * scale);
              })
              .with_debug_name("ms_status"));
    }

    div(context, mk(root.ent(), 30),
        box(scale, 0.f, 681.f, 1280.f, 39.f)
            .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
              draw_bevel(r, color, true, 2.f * scale);
            })
            .with_debug_name("ms_taskbar"));

    auto start_button =
        button(context, mk(root.ent(), 31),
               box(scale, 3.f, 687.f, 76.f, 30.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                     draw_bevel(r, color, true, 2.f * scale);
                   })
                   .with_on_draw_fg([scale](RectangleType r) {
                     draw_windows_mark(
                         {r.x + 8.f * scale, r.y + 6.f * scale,
                          18.f * scale, 18.f * scale});
                   })
                   .with_debug_name("ms_start"));
    div(context, mk(root.ent(), 34),
        box(scale, 34.f, 687.f, 43.f, 30.f)
            .with_label("Start")
            .with_font("ArchivoMockBold", pixels(16.f * scale))
            .with_custom_text_color(black)
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_ignore_pointer_events());
    if (start_button) {
      start_menu_open = !start_menu_open;
    }

    auto task_button =
        button(context, mk(root.ent(), 32),
               box(scale, 88.f, 687.f, 190.f, 30.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_on_draw_bg(
                       [color = win_gray, scale,
                        active = window_open && !minimized](RectangleType r) {
                         draw_bevel(r, color, !active, 2.f * scale);
                         draw_bomb({r.x + 5.f * scale, r.y + 4.f * scale,
                                    22.f * scale, 22.f * scale},
                                   afterhours::Color{0, 0, 0, 255});
                       })
                   .with_debug_name("ms_task"));
    div(context, mk(root.ent(), 35),
        box(scale, 124.f, 687.f, 150.f, 30.f)
            .with_label("Minesweeper")
            .with_font("ArchivoMockBold", pixels(16.f * scale))
            .with_custom_text_color(black)
            .with_alignment(TextAlignment::Left)
            .with_text_inset(0.f, 0.f)
            .with_ignore_pointer_events());
    if (task_button) {
      if (!window_open) {
        window_open = true;
        minimized = false;
        maximized = false;
        reset();
      } else {
        minimized = !minimized;
      }
      start_menu_open = false;
    }

    div(context, mk(root.ent(), 33),
        box(scale, 1185.f, 687.f, 92.f, 30.f)
            .with_label("12:00 PM")
            .with_font("AtkinsonMock", pixels(16.f * scale))
            .with_custom_text_color(black)
            .with_alignment(TextAlignment::Center)
            .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
              draw_bevel(r, color, false, 2.f * scale);
            })
            .with_debug_name("ms_clock"));

    if (start_menu_open) {
      div(context, mk(root.ent(), 40),
          box(scale, 3.f, 605.f, 210.f, 76.f)
              .with_on_draw_bg([color = win_gray, scale](RectangleType r) {
                draw_bevel(r, color, true, 2.f * scale);
              })
              .with_debug_name("ms_start_menu"));
      if (button(context, mk(root.ent(), 41),
                 box(scale, 11.f, 615.f, 194.f, 52.f)
                     .with_label(window_open ? "New Minesweeper"
                                             : "Open Minesweeper")
                     .with_font("AtkinsonMock", pixels(19.f * scale))
                     .with_custom_text_color(black)
                     .with_alignment(TextAlignment::Center)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("ms_start_game"))) {
        window_open = true;
        minimized = false;
        maximized = false;
        reset();
        start_menu_open = false;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(minesweeper_lab, "Stress Tests",
                        "Classic desktop Minesweeper with 256 live cells",
                        MinesweeperLab)
