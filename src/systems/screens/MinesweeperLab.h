#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// 256 buttons in one screen, every one of them hit-testable and individually
// styled, which is the point: a widget count a real game reaches rather than a
// gallery's dozen.
struct MinesweeperLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kSize = 16;
  static constexpr int kMines = 40;

  enum struct Phase { Playing, Lost, Won };

  bool mine[kSize][kSize]{};
  int neighbours[kSize][kSize]{};
  bool revealed[kSize][kSize]{};
  bool flagged[kSize][kSize]{};
  bool laid_out = false;
  Phase phase = Phase::Playing;
  int flags_left = kMines;

  // A fixed sequence, not a random one: the screenshot baseline has to be the
  // same board every run.
  void reset() {
    for (int r = 0; r < kSize; r++)
      for (int c = 0; c < kSize; c++) {
        mine[r][c] = false;
        neighbours[r][c] = 0;
        revealed[r][c] = false;
        flagged[r][c] = false;
      }

    unsigned state = 0x5EEDu;
    int placed = 0;
    while (placed < kMines) {
      state = state * 1664525u + 1013904223u;
      const int r = (int)((state >> 16) % kSize);
      state = state * 1664525u + 1013904223u;
      const int c = (int)((state >> 16) % kSize);
      if (mine[r][c])
        continue;
      mine[r][c] = true;
      placed++;
    }

    for (int r = 0; r < kSize; r++)
      for (int c = 0; c < kSize; c++)
        for (int dr = -1; dr <= 1; dr++)
          for (int dc = -1; dc <= 1; dc++) {
            const int nr = r + dr, nc = c + dc;
            if (nr < 0 || nc < 0 || nr >= kSize || nc >= kSize)
              continue;
            if (mine[nr][nc])
              neighbours[r][c]++;
          }

    phase = Phase::Playing;
    flags_left = kMines;
    laid_out = true;
  }

  // Iterative rather than recursive: a board of empties would otherwise
  // recurse 256 deep for a single click.
  void reveal(int row, int col) {
    if (revealed[row][col] || flagged[row][col])
      return;
    if (mine[row][col]) {
      revealed[row][col] = true;
      phase = Phase::Lost;
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
      for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++)
          if (dr || dc)
            stack.push_back({r + dr, c + dc});
    }
    check_win();
  }

  void check_win() {
    for (int r = 0; r < kSize; r++)
      for (int c = 0; c < kSize; c++)
        if (!mine[r][c] && !revealed[r][c])
          return;
    phase = Phase::Won;
  }

  int revealed_count() const {
    int n = 0;
    for (int r = 0; r < kSize; r++)
      for (int c = 0; c < kSize; c++)
        n += revealed[r][c] ? 1 : 0;
    return n;
  }

  static afterhours::Color count_color(int n) {
    switch (n) {
    case 1: return afterhours::Color{110, 180, 255, 255};
    case 2: return afterhours::Color{110, 220, 140, 255};
    case 3: return afterhours::Color{255, 140, 140, 255};
    case 4: return afterhours::Color{180, 150, 255, 255};
    case 5: return afterhours::Color{240, 180, 100, 255};
    case 6: return afterhours::Color{110, 220, 220, 255};
    case 7: return afterhours::Color{230, 230, 230, 255};
    default: return afterhours::Color{255, 120, 200, 255};
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    if (!laid_out)
      reset();

    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.roundness = 0.18f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(20.0f));

    const afterhours::Color hidden_bg{62, 68, 88, 255};
    const afterhours::Color open_bg{30, 33, 44, 255};
    const afterhours::Color boom_bg{170, 55, 60, 255};
    const afterhours::Color flag_bg{90, 78, 46, 255};

    auto root = vstack(context, mk(entity),
                       ComponentConfig{}
                           .with_size(ComponentSize{screen_pct(0.98f),
                                                    screen_pct(0.98f)})
                           .with_self_align(SelfAlign::Center)
                           .with_background(Theme::Usage::Background)
                           .with_align_items(AlignItems::Center)
                           .with_padding(Spacing::sm)
                           .with_no_wrap()
                           .with_debug_name("ms_root"));

    const char *status = phase == Phase::Lost  ? "BOOM. Reset to try again."
                         : phase == Phase::Won ? "Swept it. Every cell clear."
                                               : "Left click reveals, right click flags.";

    auto header = hstack(context, mk(root.ent(), 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f), pixels(46)})
                             .with_align_items(AlignItems::Center)
                             .with_no_wrap()
                             .with_debug_name("ms_header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format("MINES {}", flags_left))
            .with_size(ComponentSize{pixels(150), pixels(38)})
            .with_custom_background(afterhours::Color{40, 44, 58, 255})
            .with_custom_text_color(afterhours::Color{255, 180, 120, 255})
            .with_font_size(pixels(20.f))
            .with_corner_radius(8.f)
            .with_debug_name("ms_mines"));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{expand(), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{205, 212, 230, 255})
            .with_font_size(pixels(19.f))
            .with_debug_name("ms_status"));

    div(context, mk(header.ent(), 2),
        ComponentConfig{}
            .with_label(fmt::format("OPEN {}", revealed_count()))
            .with_size(ComponentSize{pixels(140), pixels(38)})
            .with_custom_background(afterhours::Color{40, 44, 58, 255})
            .with_custom_text_color(afterhours::Color{150, 220, 170, 255})
            .with_font_size(pixels(20.f))
            .with_corner_radius(8.f)
            .with_debug_name("ms_open"));

    // Release activation, so a press that slides off a cell does not fire it.
    // On a 256 target grid that is the difference between playable and not.
    if (button(context, mk(header.ent(), 3),
               ComponentConfig{}
                   .with_label("Reset")
                   .with_size(ComponentSize{pixels(110), pixels(38)})
                   .with_click_activation(ClickActivationMode::Release)
                   .with_corner_radius(8.f)
                   .with_debug_name("ms_reset"))) {
      reset();
    }

    auto board = vstack(context, mk(root.ent(), 1),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(608), pixels(608)})
                            .with_custom_background(open_bg)
                            .with_padding(Spacing::xs)
                            .with_align_items(AlignItems::Center)
                            .with_no_wrap()
                            .with_corner_radius(10.f)
                            .with_shadow(ShadowStyle::Soft, 0.f, 6.f)
                            .with_debug_name("ms_board"));

    for (int r = 0; r < kSize; r++) {
      auto row = hstack(context, mk(board.ent(), r),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), pixels(37)})
                            .with_no_wrap()
                            .with_debug_name(fmt::format("ms_row_{}", r)));

      for (int c = 0; c < kSize; c++) {
        const bool open = revealed[r][c];
        const bool lost_mine = phase == Phase::Lost && mine[r][c];
        const int n = neighbours[r][c];

        std::string face;
        afterhours::Color fg{230, 235, 245, 255};
        afterhours::Color bg = hidden_bg;
        if (lost_mine) {
          face = "*";
          bg = boom_bg;
        } else if (flagged[r][c]) {
          face = "F";
          bg = flag_bg;
          fg = afterhours::Color{255, 210, 120, 255};
        } else if (open) {
          bg = open_bg;
          if (n > 0) {
            face = std::to_string(n);
            fg = count_color(n);
          }
        }

        auto cell =
            button(context, mk(row.ent(), c),
                   ComponentConfig{}
                       .with_label(face)
                       .with_size(ComponentSize{pixels(36), pixels(35)})
                       .with_custom_background(bg)
                       .with_custom_text_color(fg)
                       .with_font_size(pixels(20.f))
                       .with_corner_radius(4.f)
                       .with_margin(Margin{.right = pixels(1)})
                       .with_click_activation(ClickActivationMode::Release)
                       .with_skip_tabbing(true)
                       .with_debug_name(fmt::format("ms_{}_{}", r, c)));

        if (phase != Phase::Playing)
          continue;

        if (cell)
          reveal(r, c);

        if (context.is_right_click(cell.ent().id) && !revealed[r][c]) {
          flagged[r][c] = !flagged[r][c];
          flags_left += flagged[r][c] ? -1 : 1;
        }
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(minesweeper_lab, "Stress Tests",
                        "256 buttons playing minesweeper, click and right click",
                        MinesweeperLab)
