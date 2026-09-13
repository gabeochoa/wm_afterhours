#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// divider() on its own, away from split_pane. cartographer asked for this so it
// would stop hand-rolling `div` with percent(1) x pixels(1) plus a margin.
struct DividerShowcase : ScreenSystem<UIContext<InputAction>> {
  float left_w = 300.f;
  float top_h = 150.f;
  float width_drag = 0.f;
  float height_drag = 0.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float offset_x = (context.screen_width - 1280 * s) / 2;
    const float offset_y = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{171, 185, 207, 255};
    const auto bar_color = afterhours::Color{70, 98, 139, 255};
    const auto first_color = afterhours::Color{39, 54, 77, 255};
    const auto second_color = afterhours::Color{27, 39, 58, 255};
    const auto box = [=](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(offset_x + x * s, offset_y + y * s)
          .with_corner_radius(0);
    };
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size,
                          afterhours::Color color, const char *name) {
      div(context, mk(entity, id), box(x, y, w, h)
          .with_label(value).with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_background(Theme::Usage::None)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(entity, 10), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({17, 23, 34, 255}).with_corner_radius(0)
        .with_debug_name("dv_canvas"));
    text(0, "Divider examples", 48, 20, 850, 43, 34, white, "dv_title");
    text(11, "Drag either grip. Measurements use screen pixels; pane shares exclude the handle.",
         48, 68, 1180, 28, 19, muted, "dv_instruction");
    if (button(context, mk(entity, 12), box(1018, 24, 210, 42)
        .with_label("Reset dividers").with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background({47, 79, 119, 255})
        .with_custom_text_color(white).with_corner_radius(8 * s)
        .with_debug_name("dv_reset"))) {
      left_w = 300;
      top_h = 150;
      width_drag = 0;
      height_drag = 0;
    }
    const auto grip = [=](bool vertical) {
      return [=](RectangleType r) {
        for (int i = -1; i <= 1; ++i) {
          const float offset = static_cast<float>(i) * 8 * s;
          raylib::DrawCircleV(
              {r.x + r.width / 2 + (vertical ? 0 : offset),
               r.y + r.height / 2 + (vertical ? offset : 0)},
              2 * s, {220, 233, 250, 255});
        }
      };
    };
    const auto pane = [&](ComponentSize size, afterhours::Color color,
                          const char *name) {
      return ComponentConfig{}.with_size(size).with_custom_background(color)
          .with_corner_radius(0).with_debug_name(name);
    };
    const auto pane_label = [&](afterhours::Entity &parent,
                                const std::string &value, float width) {
      div(context, mk(parent, 10), ComponentConfig{}
          .with_size({pixels(std::max(0.f, width - 24 * s)), pixels(28 * s)})
          .with_absolute_position(12 * s, 6 * s)
          .with_label(value).with_font("AtkinsonMock", pixels(18 * s))
          .with_custom_text_color(white).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    };
    const auto tiles = [&](afterhours::Entity &parent, const char *prefix,
                           float width) {
      const int columns = std::clamp(
          static_cast<int>((width / s - 16) / 44), 2, 6);
      for (int i = 0; i < 6; ++i) {
        div(context, mk(parent, 20 + i), ComponentConfig{}
            .with_size({pixels(36 * s), pixels(20 * s)})
            .with_absolute_position((12 + (i % columns) * 44) * s,
                                     (40 + (i / columns) * 24) * s)
            .with_label(fmt::format("{}{}", prefix, i + 1))
            .with_font("AtkinsonMock", pixels(14 * s))
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(white)
            .with_custom_background({55, 76, 105, 255})
            .with_corner_radius(4 * s).with_ignore_pointer_events());
      }
    };

    // Axis::X: the bar moves horizontally, so it is the vertical one.
    auto row = div(context, mk(entity, 1), box(48, 146, 1180, 126)
        .with_flex_direction(FlexDirection::Row)
        .with_custom_background(first_color).with_debug_name("dv_row"));
    left_w = std::clamp(left_w, 120.f, 900.f);
    auto left = div(context, mk(row.ent(), 0),
        pane({pixels(left_w * s), percent(1)}, first_color, "dv_left"));
    auto width_bar = divider(context, mk(row.ent(), 1), Axis::X,
        ComponentConfig{}.with_size({pixels(12 * s), percent(1)})
            .with_custom_background(bar_color).with_corner_radius(0)
            .with_on_draw_fg(grip(true)).with_debug_name("dv_vbar"));
    if (width_bar) {
      const float next = std::clamp(left_w + width_bar.as<float>() / s,
                                    120.f, 900.f);
      width_drag = (next - left_w) * s;
      left_w = next;
      left.cmp().set_desired_width(pixels(left_w * s));
    }
    auto right = div(context, mk(row.ent(), 2),
        pane({expand(), percent(1)}, second_color, "dv_right"));
    const float left_pixels = left.cmp().rect().width;
    const float right_pixels = right.cmp().rect().width;
    pane_label(left.ent(), "Left pane", left_w * s);
    pane_label(right.ent(), "Right pane", (1168 - left_w) * s);
    tiles(left.ent(), "A", left_w * s);
    tiles(right.ent(), "B", (1168 - left_w) * s);
    text(13, "Width divider", 48, 108, 340, 30, 24, white, "dv_width_title");
    text(14, fmt::format("Left limits: {:.0f}-{:.0f}px / total {:.0f}px",
                         120 * s, 900 * s, row.cmp().rect().width),
         650, 108, 578, 30, 20, muted, "dv_width_limits");
    const float width_total = left_pixels + right_pixels;
    text(15, fmt::format("Resolved left: {:.0f}px ({:.1f}%)    Right: {:.0f}px ({:.1f}%)    Last drag: {:+.0f}px",
                         left_pixels, width_total > 0 ? 100 * left_pixels / width_total : 0,
                         right_pixels, width_total > 0 ? 100 * right_pixels / width_total : 0,
                         width_drag),
         48, 281, 1180, 30, 19, muted, "dv_width_metrics");

    auto col = div(context, mk(entity, 2), box(48, 354, 1180, 280)
        .with_flex_direction(FlexDirection::Column)
        .with_custom_background(first_color).with_debug_name("dv_col"));
    top_h = std::clamp(top_h, 60.f, 220.f);
    auto top = div(context, mk(col.ent(), 0),
        pane({percent(1), pixels(top_h * s)}, first_color, "dv_top"));
    auto height_bar = divider(context, mk(col.ent(), 1), Axis::Y,
        ComponentConfig{}.with_size({percent(1), pixels(12 * s)})
            .with_custom_background(bar_color).with_corner_radius(0)
            .with_on_draw_fg(grip(false)).with_debug_name("dv_hbar"));
    if (height_bar) {
      const float next = std::clamp(top_h + height_bar.as<float>() / s,
                                    60.f, 220.f);
      height_drag = (next - top_h) * s;
      top_h = next;
      top.cmp().set_desired_height(pixels(top_h * s));
    }
    auto bottom = div(context, mk(col.ent(), 2),
        pane({percent(1), expand()}, second_color, "dv_bottom"));
    const auto preview = [&](ElementResult &result, const char *name,
                             const char *prefix, float height) {
      const float width = 1180 * s;
      const int visible = std::clamp(static_cast<int>((height - 38) / 22), 0, 4);
      pane_label(result.ent(), fmt::format("{} / {} of 4 rows fit", name, visible), width);
      for (int i = 0; i < visible; ++i) {
        div(context, mk(result.ent(), 20 + i), ComponentConfig{}
            .with_size({pixels(width - 24 * s), pixels(18 * s)})
            .with_absolute_position(12 * s, (38 + i * 22) * s)
            .with_label(fmt::format("{} {}", prefix, i + 1))
            .with_font("AtkinsonMock", pixels(15 * s))
            .with_custom_text_color(muted)
            .with_custom_background({33, 47, 68, 255}).with_corner_radius(3 * s)
            .with_ignore_pointer_events());
      }
    };
    preview(top, "Top pane", "Preview row", top_h);
    preview(bottom, "Bottom pane", "Detail row", 268 - top_h);
    text(16, "Height divider", 48, 317, 340, 30, 24, white, "dv_height_title");
    text(17, fmt::format("Top limits: {:.0f}-{:.0f}px / total {:.0f}px",
                         60 * s, 220 * s, col.cmp().rect().height),
         650, 317, 578, 30, 20, muted, "dv_height_limits");
    const float top_pixels = top.cmp().rect().height;
    const float bottom_pixels = bottom.cmp().rect().height;
    const float height_total = top_pixels + bottom_pixels;
    text(18, fmt::format("Resolved top: {:.0f}px ({:.1f}%)    Bottom: {:.0f}px ({:.1f}%)    Last drag: {:+.0f}px",
                         top_pixels, height_total > 0 ? 100 * top_pixels / height_total : 0,
                         bottom_pixels, height_total > 0 ? 100 * bottom_pixels / height_total : 0,
                         height_drag),
         48, 637, 1180, 29, 19, muted, "dv_height_metrics");
    div(context, mk(entity, 19), box(32, 354, 10, top_pixels / s)
        .with_background(Theme::Usage::None).with_ignore_pointer_events()
        .with_on_draw_fg([=](RectangleType r) {
          const raylib::Color ink{139, 180, 224, 255};
          raylib::DrawLineEx({r.x, r.y}, {r.x, r.y + r.height}, s, ink);
          raylib::DrawLineEx({r.x, r.y}, {r.x + r.width, r.y}, s, ink);
          raylib::DrawLineEx({r.x, r.y + r.height},
                             {r.x + r.width, r.y + r.height}, s, ink);
        }).with_debug_name("dv_height_bracket"));

    // Not every divider is draggable -- a 1px rule is the common case, and it
    // is the same widget with the drag simply ignored.
    divider(context, mk(entity, 3), Axis::Y,
        box(48, 676, 1180, 1 / s)
            .with_custom_background({139, 160, 191, 255})
            .with_ignore_pointer_events().with_skip_tabbing(true).with_debug_name("dv_rule"));
    for (int i = 0; i < 2; ++i) {
      div(context, mk(entity, 30 + i), box(i == 0 ? 42.f : 1232.f, 672, 2, 9)
          .with_custom_background({168, 194, 227, 255})
          .with_ignore_pointer_events());
    }
    div(context, mk(entity, 4), box(48, 687, 154, 28)
        .with_label("divider()")
        .with_font("AtkinsonMock", pixels(26 * s))
        .with_custom_text_color(white).with_background(Theme::Usage::None)
        .with_ignore_pointer_events().with_debug_name("dv_api"));
    text(32, "Static 1px separator", 209, 687, 620, 28, 19,
         muted, "dv_hint");
  }
};

REGISTER_EXAMPLE_SCREEN(divider_lab, "System Demos",
                        "divider() as a draggable bar and as a plain rule",
                        DividerShowcase)
