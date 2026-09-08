#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/grid.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// grid() and grid_cell(). wordproc draws its tables with ~100 lines of raw
// raylib because flex cannot line a column up across rows.
//
// The content is deliberately ragged: with flex, cells sized to their text
// would stagger. Here the tracks are declared once and every row uses them.
struct GridLab : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const auto line = afterhours::Color{90, 110, 150, 255};
    const auto head_bg = afterhours::Color{40, 55, 85, 255};
    const auto zebra = afterhours::Color{30, 34, 46, 255};

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_padding(Padding::all(w1280(16)))
                        .with_debug_name("gl_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("grid: tracks declared once, every row uses them")
            .with_size(ComponentSize{percent(1.f), h720(34)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("gl_title"));

    // ---- equal tracks, ragged content ------------------------------------
    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_label("equal columns, ragged text")
            .with_size(ComponentSize{percent(1.f), h720(24)})
            .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
            .with_background(Theme::Usage::None)
            .with_margin(Margin{.top = h720(10)})
            .with_debug_name("gl_cap1"));

    static const char *ragged[3][4] = {
        {"id", "name", "role", "notes"},
        {"1", "a", "maintainer of everything", "-"},
        {"2", "a much longer name", "dev", "sometimes"},
    };
    auto t1 = grid(context, mk(root.ent(), 2),
                   GridConfig{}
                       .with_rows(3)
                       .with_cols(4)
                       .with_row_height(h720(30))
                       .with_cell_border(BorderWeight::Thin, line),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.f), h720(90)})
                       .with_debug_name("gl_t1"));
    for (int r = 0; r < 3; r++)
      for (int c = 0; c < 4; c++)
        grid_cell(context, t1, r, c,
                  ComponentConfig{}
                      .with_label(ragged[r][c])
                      .with_font(UIComponent::DEFAULT_FONT, h720(12.f))
                      .with_custom_background(r == 0 ? head_bg : zebra)
                      .with_debug_name(fmt::format("gl_a_{}_{}", r, c)));

    // ---- explicit tracks and a spanning header ----------------------------
    div(context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_label("explicit tracks, a header spanning three, thick border")
            .with_size(ComponentSize{percent(1.f), h720(24)})
            .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
            .with_background(Theme::Usage::None)
            .with_margin(Margin{.top = h720(16)})
            .with_debug_name("gl_cap2"));

    auto t2 = grid(context, mk(root.ent(), 4),
                   GridConfig{}
                       .with_rows(4)
                       .with_cols(4)
                       .with_col_widths({w1280(60), w1280(240), w1280(120),
                                         w1280(120)})
                       .with_row_height(h720(30))
                       .with_cell_border(BorderWeight::Medium, line),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.f), h720(120)})
                       .with_debug_name("gl_t2"));

    // Row 0: a gutter cell then one header across the remaining three.
    grid_cell(context, t2, 0, 0,
              ComponentConfig{}
                  .with_custom_background(head_bg)
                  .with_debug_name("gl_b_0_0"));
    grid_cell(context, t2, 0, 1,
              ComponentConfig{}
                  .with_label("one cell, spanning three tracks")
                  .with_font(UIComponent::DEFAULT_FONT, h720(12.f))
                  .with_custom_background(head_bg)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("gl_b_0_1"),
              3);

    for (int r = 1; r < 4; r++)
      for (int c = 0; c < 4; c++)
        grid_cell(context, t2, r, c,
                  ComponentConfig{}
                      .with_label(c == 0 ? fmt::format("{}", r)
                                         : fmt::format("r{}c{}", r, c))
                      .with_font(UIComponent::DEFAULT_FONT, h720(12.f))
                      .with_custom_background(r % 2 ? zebra : head_bg)
                      .with_debug_name(fmt::format("gl_b_{}_{}", r, c)));
  }
};

REGISTER_EXAMPLE_SCREEN(grid_lab, "System Demos",
                        "grid() with tracks, spanning and cell borders",
                        GridLab)
