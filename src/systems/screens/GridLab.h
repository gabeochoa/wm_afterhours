#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <cmath>
#include <afterhours/src/plugins/ui/grid.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// grid() and grid_cell(). wordproc draws its tables with ~100 lines of raw
// raylib because flex cannot line a column up across rows.
//
// The content is deliberately ragged: with flex, cells sized to their text
// would stagger. Here the tracks are declared once and every row uses them.
struct GridLab : ScreenSystem<UIContext<InputAction>> {
  bool swap_lengths = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color line{66, 87, 118, 255};
    const afterhours::Color outer_line{90, 110, 150, 255};
    const afterhours::Color head_bg{42, 58, 84, 255};
    const afterhours::Color zebra{26, 35, 49, 255};
    const afterhours::Color ink{235, 241, 250, 255};
    const afterhours::Color muted{176, 193, 217, 255};
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({17, 24, 35, 255}).with_corner_radius(0).with_debug_name("gl_root"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 98)
        .with_custom_background({30, 42, 60, 255}).with_corner_radius(12 * scale));
    label(1, "Grid layout", 20, 34, 800, 44, 34, ink, "gl_title");
    label(2, "Declare tracks once. Every row shares the same column boundaries.", 20, 83, 1104, 28, 21, muted);
    if (button(context, mk(root.ent(), 3), box(864, 43, 260, 40)
        .with_label("Swap text lengths").with_font("AtkinsonMock", pixels(22 * scale))
        .with_custom_background({48, 69, 99, 255}).with_custom_text_color(ink)
        .with_corner_radius(7 * scale).with_debug_name("gl_swap"))) swap_lengths = !swap_lengths;
    div(context, mk(root.ent(), 4), box(0, 140, 1144, 224)
        .with_custom_background({25, 34, 49, 255}).with_corner_radius(10 * scale));
    label(5, "Example 1 / equal columns, ragged text", 20, 150, 1104, 32, 25, ink, "gl_cap1");
    label(6, "Equal 1fr tracks / " + std::to_string(static_cast<int>(std::lround(1104 * scale))) + "px total / '-' is an empty-value fixture", 20, 187, 1104, 24, 20, muted);
    static const char *ragged[3][4] = {
        {"id", "name", "role", "notes"},
        {"1", "a", "maintainer of everything", "-"},
        {"2", "a much longer name", "dev", "sometimes"},
    };
    auto t1 = grid(context, mk(root.ent(), 7), GridConfig{}
        .with_rows(3).with_cols(4).with_row_height(pixels(44 * scale))
        .with_cell_border(BorderWeight::Thin, line),
        box(20, 218, 1104, 1).with_size({pixels(1104 * scale), children()})
            .with_debug_name("gl_t1").with_on_draw_fg([outer_line](RectangleType r) {
              afterhours::draw_rectangle_outline(r, outer_line, 1.f);
            }));
    for (int r = 0; r < 3; ++r) {
      for (int c = 0; c < 4; ++c) {
        const int source_row = swap_lengths && r > 0 && (c == 1 || c == 2) ? 3 - r : r;
        auto cell = grid_cell(context, t1, r, c, ComponentConfig{}
            .with_label(ragged[source_row][c]).with_font(r == 0 ? "AtkinsonMockBold" : "AtkinsonMock", pixels(21 * scale))
            .with_custom_text_color(ink).with_custom_background(r == 0 ? head_bg : zebra)
            .with_text_inset(0, 0).with_corner_radius(0)
            .with_debug_name(fmt::format("gl_a_{}_{}", r, c)));
        cell.ent().get<HasLabel>().text_x_offset = 12 * scale;
      }
    }
    div(context, mk(root.ent(), 8), box(0, 386, 1144, 254)
        .with_custom_background({25, 34, 49, 255}).with_corner_radius(10 * scale));
    label(9, "Example 2 / explicit tracks and a spanning header", 20, 397, 1104, 32, 25, ink, "gl_cap2");
    const std::array<int, 4> widths{60, 240, 120, 120};
    float x = 20;
    for (size_t i = 0; i < widths.size(); ++i) {
      label(10 + static_cast<int>(i), std::to_string(static_cast<int>(std::lround(widths[i] * scale))) + "px", x, 440, static_cast<float>(widths[i]), 26, 19, muted);
      x += static_cast<float>(widths[i]);
    }
    div(context, mk(root.ent(), 14), box(80, 470, 480, 10)
        .with_ignore_pointer_events().with_on_draw_fg([scale](RectangleType r) {
          const afterhours::Color accent{137, 195, 249, 255};
          afterhours::draw_line_ex({r.x, r.y + r.height}, {r.x, r.y}, scale, accent);
          afterhours::draw_line_ex({r.x, r.y}, {r.x + r.width, r.y}, scale, accent);
          afterhours::draw_line_ex({r.x + r.width, r.y}, {r.x + r.width, r.y + r.height}, scale, accent);
        }));
    auto t2 = grid(context, mk(root.ent(), 15), GridConfig{}
        .with_rows(4).with_cols(4)
        .with_col_widths({pixels(60 * scale), pixels(240 * scale), pixels(120 * scale), pixels(120 * scale)})
        .with_row_height(pixels(36 * scale)).with_cell_border(BorderWeight::Medium, line),
        box(20, 484, 540, 1).with_size({pixels(540 * scale), children()}).with_debug_name("gl_t2")
            .with_on_draw_fg([outer_line](RectangleType r) {
              afterhours::draw_rectangle_outline(r, outer_line, 2.f);
            }));
    // Row 0: a gutter cell then one header across the remaining three.
    grid_cell(context, t2, 0, 0, ComponentConfig{}.with_custom_background(head_bg)
        .with_corner_radius(0).with_debug_name("gl_b_0_0"));
    grid_cell(context, t2, 0, 1, ComponentConfig{}
        .with_label("one cell, spanning three tracks").with_font("AtkinsonMockBold", pixels(22 * scale))
        .with_custom_text_color(ink).with_custom_background(head_bg).with_alignment(TextAlignment::Center)
        .with_text_inset(12 * scale, 0).with_corner_radius(0).with_debug_name("gl_b_0_1"), 3);
    for (int r = 1; r < 4; ++r) {
      for (int c = 0; c < 4; ++c) {
        auto cell = grid_cell(context, t2, r, c, ComponentConfig{}
            .with_label(c == 0 ? fmt::format("{}", r) : fmt::format("r{}c{}", r, c))
            .with_font("AtkinsonMock", pixels(25 * scale)).with_custom_text_color(ink)
            .with_custom_background(r % 2 ? zebra : head_bg).with_text_inset(0, 0)
            .with_corner_radius(0).with_debug_name(fmt::format("gl_b_{}_{}", r, c)));
        cell.ent().get<HasLabel>().text_x_offset = 12 * scale;
      }
    }
    label(16, "Border inspection / Medium requests 2px", 608, 440, 516, 30, 22, ink);
    auto detail = grid(context, mk(root.ent(), 17), GridConfig{}
        .with_rows(1).with_cols(1).with_row_height(pixels(88 * scale))
        .with_cell_border(BorderWeight::Medium, outer_line),
        box(608, 483, 190, 1).with_size({pixels(190 * scale), children()}).with_debug_name("gl_border_detail"));
    auto detail_cell = grid_cell(context, detail, 0, 0, ComponentConfig{}
        .with_label("Native cell").with_font("AtkinsonMock", pixels(22 * scale)).with_custom_text_color(ink)
        .with_custom_background(zebra).with_corner_radius(0).with_text_inset(0, 0));
    detail_cell.ent().get<HasLabel>().text_x_offset = 12 * scale;
    div(context, mk(root.ent(), 18), box(824, 483, 300, 88)
        .with_ignore_pointer_events().with_on_draw_fg([scale, outer_line, zebra](RectangleType r) {
          afterhours::draw_rectangle(r, outer_line);
          afterhours::draw_rectangle({r.x + 16 * scale, r.y + 16 * scale,
                                      r.width - 32 * scale, r.height - 32 * scale}, zebra);
        }));
    label(19, "Actual native fixture", 608, 584, 204, 27, 18, muted);
    label(20, "8x requested-border diagram", 824, 584, 300, 27, 18, muted);
    x = 20;
    for (size_t i = 0; i < widths.size(); ++i) {
      div(context, mk(root.ent(), 30 + static_cast<int>(i)), box(x, 650, static_cast<float>(widths[i]) - 2, 24)
          .with_custom_background(i % 2 == 0 ? head_bg : afterhours::Color{52, 77, 109, 255})
          .with_ignore_pointer_events());
      label(34 + static_cast<int>(i), std::to_string(static_cast<int>(std::lround(widths[i] * scale))),
            x + 8, 650, static_cast<float>(widths[i]) - 16, 24, 17, ink);
      x += static_cast<float>(widths[i]);
    }
    label(21, std::to_string(static_cast<int>(std::lround(540 * scale))) + "px table / row-index gutter / striped body rows",
          608, 650, 516, 24, 18, muted);
    label(22, "Columns stay aligned across every row, even after swapping text lengths.", 20, 679, 1104, 29, 22, ink);
  }
};

REGISTER_EXAMPLE_SCREEN(grid_lab, "System Demos",
                        "grid() with tracks, spanning and cell borders",
                        GridLab)
