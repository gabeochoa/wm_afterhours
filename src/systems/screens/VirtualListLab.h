#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <charconv>
#include <cmath>
#include <optional>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// virtual_list() over a count no one would build entities for. floatinghotel
// (10k commits) and hanabi (#23) both asked for this; the scrollbar describes
// the whole list while visible and overscan rows are built from a bounded pool.
struct VirtualListLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr size_t kCount = 10000;
  static constexpr float kRowHeight = 26.f;
  std::string index_text = "5000";
  std::string jump_status = "IDs start at 0. Marked every seventh row.";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const afterhours::Color white{235, 240, 250, 255}, muted{184, 201, 221, 255};
    const afterhours::Color cyan{119, 220, 222, 255}, ink{15, 22, 35, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    UIStylingDefaults::get().set_grid_snapping(false);
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("vl_root"));
    const auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    const auto text = [&](int id, std::string value, float x, float y, float w,
                          float h, float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Virtual list / 10,000 rows", 48, 10, 620, 40, 32, white, "vl_title", true);
    text(1, "Index", 60, 56, 160, 24, 18, muted);
    text(3, "State", 232, 56, 144, 24, 18, muted);
    text(4, "Scroll through a bounded row pool", 388, 56, 330, 24, 18, muted, "vl_hint");
    std::optional<size_t> jump;
    const auto preset = [&](int id, const char *label, float x, const char *debug) {
      return button(context, mk(root.ent(), id), box(x, 15, 86, 34).with_label(label)
          .with_font("AtkinsonMock", pixels(18 * s)).with_custom_background({52, 74, 105, 255})
          .with_custom_text_color(white).with_corner_radius(5 * s).with_debug_name(debug));
    };
    if (preset(10, "Start", 696, "vl_start")) jump = 0;
    if (preset(11, "Middle", 790, "vl_middle")) jump = kCount / 2;
    if (preset(12, "End", 884, "vl_end")) jump = kCount - 1;
    text_input(context, mk(root.ent(), 13), index_text, box(986, 15, 144, 34)
        .with_font("AtkinsonMock", pixels(18 * s)).with_custom_background({34, 46, 65, 255})
        .with_border(muted, 1.f).with_placeholder("0 to 9999").with_debug_name("vl_index"));
    if (button(context, mk(root.ent(), 14), box(1140, 15, 88, 34).with_label("Jump")
        .with_font("AtkinsonMock", pixels(18 * s)).with_custom_background(cyan)
        .with_custom_text_color(ink).with_corner_radius(5 * s).with_debug_name("vl_jump"))) {
      size_t requested = 0;
      const auto result = std::from_chars(index_text.data(), index_text.data() + index_text.size(), requested);
      if (result.ec == std::errc{} && result.ptr == index_text.data() + index_text.size() && requested < kCount)
        jump = requested;
      else jump_status = "Enter a whole row index from 0 to 9999.";
    }
    if (jump) jump_status = fmt::format("Jump requested: row {:05}", *jump);
    text(15, jump_status, 736, 56, 492, 24, 18, cyan, "vl_jump_status");
    size_t built_rows = 0;
    auto list = virtual_list(context, mk(root.ent(), 2), kCount, kRowHeight,
        [&](size_t i, afterhours::Entity &row) {
          ++built_rows;
          auto content = div(context, mk(row, 0), ComponentConfig{}
              .with_size({percent(1.f), pixels(kRowHeight)}).with_skip_grid_snap(true)
              .with_label(fmt::format("row {:05}", i)).with_text_inset(0, 0)
              .with_alignment(TextAlignment::Left).with_font("AtkinsonMock", pixels(18))
              .with_custom_background(i % 2 ? afterhours::Color{28, 39, 57, 255} : afterhours::Color{35, 48, 68, 255})
              .with_custom_text_color(white).with_corner_radius(0)
              .with_debug_name(fmt::format("vl_row_{}", i))
              .with_on_draw_fg([cyan, s, i](RectangleType r) {
                afterhours::draw_rectangle({r.x + 172 * s, r.y, 1, r.height}, {62, 81, 105, 255});
                if (i % 7 != 0) return;
                afterhours::draw_rectangle({r.x, r.y + 3, 3, r.height - 6}, cyan);
              }));
          content.ent().get<HasLabel>().text_x_offset = 12;
          div(context, mk(content.ent(), 0), ComponentConfig{}
              .with_size({pixels(148 * s), pixels(kRowHeight)}).with_absolute_position(184 * s, 0)
              .with_label(i % 7 == 0 ? "Marked" : "Plain").with_alignment(TextAlignment::Left)
              .with_font("AtkinsonMock", pixels(18)).with_custom_text_color(i % 7 == 0 ? cyan : muted)
              .with_transparent_bg().with_ignore_pointer_events().with_skip_grid_snap(true));
        }, box(48, 84, 1180, 560).with_custom_background({18, 25, 39, 255}).with_debug_name("vl_list"));
    auto &scroll = list.ent().get<HasScrollView>();
    if (jump) {
      scroll.scroll_offset.y = static_cast<float>(*jump) * kRowHeight;
      scroll.clamp_scroll();
      scroll.scroll_target = scroll.scroll_offset;
      scroll.last_eased_offset = scroll.scroll_offset;
    }
    const float height = scroll.viewport_or_zero().y;
    const float max_offset = std::max(0.f, scroll.content_size.y - height);
    const size_t first = std::min(kCount - 1, static_cast<size_t>(std::max(0.f, std::floor(scroll.scroll_offset.y / kRowHeight))));
    const size_t end = std::clamp(static_cast<size_t>(std::max(1.f, std::ceil((scroll.scroll_offset.y + height) / kRowHeight))), size_t{1}, kCount);
    text(20, fmt::format("Visible rows {} to {} of 10,000", first, end - 1), 48, 650, 460, 27, 20, white, "vl_range");
    text(21, fmt::format("Built {} rows / {} row UI entities", built_rows, built_rows * 3), 514, 650, 414, 27, 19, white, "vl_built");
    text(22, fmt::format("{:.0f} px / {:.1f}%", scroll.scroll_offset.y, max_offset > 0 ? 100.f * scroll.scroll_offset.y / max_offset : 0.f),
         938, 650, 290, 27, 19, cyan, "vl_offset");
    text(23, fmt::format("26 px rows / {:.0f} px viewport / {} visible incl. partial", height, end - first), 48, 684, 620, 25, 18, muted, "vl_metrics");
    text(24, "Built count includes overscan. Last row: 09999.", 688, 684, 540, 25, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(virtual_list_lab, "System Demos",
                        "10k-row windowed list; visible and overscan rows share a bounded pool",
                        VirtualListLab)
