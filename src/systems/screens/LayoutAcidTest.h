#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct LayoutAcidTest : ScreenSystem<UIContext<InputAction>> {
  bool broken = false;
  const std::array<const char *, 12> picture{{
      "....YYYY....", "..YYYYYYYY..", ".YYYYYYYYYY.", "YYYYYYYYYYYY",
      "YYYKKYYKKYYY", "YYYKKYYKKYYY", "YYYYYYYYYYYY", "YYKYYYYYYKYY",
      "YYYKYYYYKYYY", ".YYYKKKKYYY.", "..YYYYYYYY..", "....YYYY....",
  }};

  static afterhours::Color color(char value) {
    if (value == 'Y') return {246, 198, 61, 255};
    if (value == 'K') return {24, 29, 37, 255};
    return {242, 245, 249, 255};
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto ink = afterhours::Color{230, 239, 249, 255};
    const auto muted = afterhours::Color{166, 186, 207, 255};
    Theme theme;
    theme.background = {15, 22, 33, 255};
    theme.font = ink;
    theme.font_muted = muted;
    theme.primary = {131, 211, 190, 255};
    context.set_theme(theme);
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(22 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2,
                                (context.screen_height - 720 * s) / 2).with_debug_name("acid_root"));
    const auto label = [&](int id, const std::string &value, float x, float y,
                           float w, float h, float size, afterhours::Color fill,
                           const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(fill)
          .with_text_inset(0, 0).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label(1, "A smile is a layout test.", 56, 30, 1140, 54, 38, ink);
    label(2, "Both faces should match. Red gaps or pink edges mean something is wrong.",
          56, 92, 1168, 32, 22, muted);
    if (button(context, mk(root.ent(), 3), box(936, 146, 288, 44)
        .with_label(broken ? "Restore correct layout" : "Show broken layout")
        .with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background(broken ? afterhours::Color{139, 54, 68, 255} : afterhours::Color{36, 71, 82, 255})
        .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("acid_toggle"))) broken = !broken;
    label(4, "Live layout", 128, 166, 380, 38, 28, ink);
    label(5, "Reference", 768, 166, 380, 38, 28, ink);
    div(context, mk(root.ent(), 6), box(104, 216, 336, 336)
        .with_custom_background({28, 40, 56, 255}).with_corner_radius(12 * s));
    div(context, mk(root.ent(), 7), box(744, 216, 336, 336)
        .with_custom_background({28, 40, 56, 255}).with_corner_radius(12 * s));
    auto board = vstack(context, mk(root.ent(), 10), box(128, 240, 288, 288)
        .with_custom_background({210, 39, 65, 255})
        .with_overflow(broken ? Overflow::Visible : Overflow::Hidden)
        .with_no_wrap().with_gap(pixels(broken ? 3 * s : 0))
        .with_debug_name("acid_board"));
    for (int edge = 0; edge < 2; ++edge)
      div(context, mk(board.ent(), 200 + edge), box(edge == 0 ? -24 : 288, 0, 24, 288)
          .with_custom_background({249, 89, 210, 255}).with_ignore_pointer_events()
          .with_debug_name("acid_clip_probe_" + std::to_string(edge)));
    for (size_t row_index = 0; row_index < picture.size(); ++row_index) {
      auto row = hstack(context, mk(board.ent(), static_cast<int>(row_index)), ComponentConfig{}
          .with_size({percent(1.f), expand()}).with_transparent_bg().with_corner_radius(0)
          .with_no_wrap().with_gap(pixels(broken ? 3 * s : 0)).with_skip_grid_snap(true)
          .with_debug_name("acid_row_" + std::to_string(row_index)));
      for (int col = 0; col < 12; ++col)
        div(context, mk(row.ent(), col), ComponentConfig{}
            .with_size({expand(), percent(1.f)})
            .with_custom_background(color(picture[row_index][col])).with_corner_radius(0)
            .with_skip_grid_snap(true).with_ignore_pointer_events()
            .with_debug_name("acid_cell_" + std::to_string(row_index) + "_" + std::to_string(col)));
    }
    div(context, mk(root.ent(), 11), box(768, 240, 288, 288)
        .with_on_draw_fg([pattern = picture](RectangleType r) {
          const float cell = r.width / 12;
          for (size_t y = 0; y < pattern.size(); ++y)
            for (int x = 0; x < 12; ++x)
              afterhours::draw_rectangle({r.x + static_cast<float>(x) * cell,
                  r.y + static_cast<float>(y) * cell, cell, cell}, color(pattern[y][x]));
        }).with_ignore_pointer_events().with_debug_name("acid_reference"));
    label(12, broken ? "Intentional failure" : "Expected: a clean smile", 128, 572, 500, 38, 25,
          broken ? afterhours::Color{255, 160, 174, 255} : theme.primary, "acid_status");
    label(13, "12 equal rows. 12 equal columns.", 128, 620, 500, 30, 21, muted);
    label(14, "No gaps. No pixels beyond the frame.", 128, 657, 520, 30, 21, muted);
    label(15, "The reference is drawn independently.", 744, 572, 500, 38, 22, ink);
    label(16, "Change the window size and compare again.", 744, 620, 500, 30, 21, muted);
    label(17, "The break button adds gaps and removes clipping.", 744, 657, 500, 30, 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(layout_acid_test, "Tools",
                        "Visual correctness target for layout and clipping", LayoutAcidTest)
