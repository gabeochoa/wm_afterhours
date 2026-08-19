#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// HasScrollView::sync_group. Group 1: a diff, three panes of different
// lengths so the short one clamps at its own end. Group 2: a header strip
// tracking a wide table's x, with no y scroll of its own.
struct SyncScrollLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr size_t kVGroup = 1;
  static constexpr size_t kHGroup = 2;
  static constexpr size_t kCols = 14;

  // Read off the live components so the readouts prove the sync.
  float left_y = 0.f, right_y = 0.f, short_y = 0.f;
  float head_x = 0.f, table_x = 0.f;

  static bool is_changed(size_t i) { return i % 7 == 3 || i % 11 == 5; }

  // `rows` differs per pane: a shorter member must stop at its own end.
  void pane(UIContext<InputAction> &context, afterhours::Entity &parent,
            int slot, const char *title, float x, float w, size_t rows,
            float *out_y) {
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto tint = afterhours::Color{slot == 1 ? 32 : 68,
                                        slot == 1 ? 62 : 34,
                                        slot == 1 ? 42 : 38, 255};

    div(context, mk(parent, 100 + slot),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(w), pixels(20)})
            .with_absolute_position(x, 88.f)
            .with_label(fmt::format("{} ({} rows)", title, rows))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(12.f)
            .with_debug_name(fmt::format("ss_head_{}", slot)));

    auto view =
        vstack(context, mk(parent, 110 + slot),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(w), pixels(300)})
                   .with_absolute_position(x, 110.f)
                   .with_custom_background(afterhours::Color{22, 26, 38, 255})
                   .with_border(muted, 1.f)
                   .with_overflow(Overflow::Scroll, Axis::Y)
                   .with_padding(Padding::all(pixels(4)))
                   .with_debug_name(fmt::format("ss_pane_{}", slot)));

    if (view.ent().has<HasScrollView>()) {
      auto &sv = view.ent().get<HasScrollView>();
      sv.sync_group = kVGroup;
      sv.show_scrollbar = true;
      *out_y = sv.scroll_offset.y;
    }

    for (size_t i = 0; i < rows; i++) {
      const bool changed = is_changed(i);
      auto cfg = ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(18)})
                     .with_label(fmt::format("{:>3}  {}", i + 1,
                                             changed ? "value = compute(x)"
                                                     : "// context"))
                     .with_alignment(TextAlignment::Left)
                     .with_custom_text_color(changed ? white : muted)
                     .with_font_size(11.f)
                     .with_debug_name(fmt::format("ss_{}_{}", slot, i));
      if (changed)
        cfg = cfg.with_custom_background(tint);
      // mk() keys on source location; each pane needs its own range.
      div(context, mk(view.ent(), i + slot * 1000), cfg);
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(28)})
            .with_absolute_position(48.f, 18.f)
            .with_label("Synchronized Scroll - sync_group locks views together")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("ss_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(22)})
            .with_absolute_position(48.f, 46.f)
            .with_label("group 1: three panes, different heights - the short "
                        "one stops at its own end")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(12.f)
            .with_debug_name("ss_hint"));

    pane(context, entity, 1, "before", 48.f, 340.f, 120, &left_y);
    pane(context, entity, 2, "after", 400.f, 340.f, 120, &right_y);
    pane(context, entity, 3, "short", 752.f, 200.f, 24, &short_y);

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(22)})
            .with_absolute_position(48.f, 418.f)
            .with_label(fmt::format(
                "y: before {:.0f}  after {:.0f}  short {:.0f}   {}   {}",
                left_y, right_y, short_y,
                left_y == right_y ? "y-in-sync" : "y-drifting",
                // "stuck" at 0 means the sync never reached it.
                short_y == left_y      ? "short-following"
                : short_y > 0.f        ? "short-clamped"
                                       : "short-stuck"))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_font_size(12.f)
            .with_debug_name("ss_readout_y"));

    horizontal_group(context, entity);
  }

  // Group 2: header synced on x only, over a wide scrolling table.
  void horizontal_group(UIContext<InputAction> &context,
                        afterhours::Entity &entity) {
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const float table_w = 900.f;
    const float col_w = 120.f;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(22)})
            .with_absolute_position(48.f, 452.f)
            .with_label("group 2: header tracks the table's x, and has no y "
                        "scroll of its own")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(12.f)
            .with_debug_name("ss_hint_h"));

    auto header =
        hstack(context, mk(entity, 4),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(table_w), pixels(26)})
                   .with_absolute_position(48.f, 478.f)
                   .with_custom_background(afterhours::Color{40, 48, 70, 255})
                   .with_border(muted, 1.f)
                   .with_overflow(Overflow::Scroll, Axis::X)
                   .with_debug_name("ss_header"));

    if (header.ent().has<HasScrollView>()) {
      auto &sv = header.ent().get<HasScrollView>();
      sv.sync_group = kHGroup;
      sv.horizontal_enabled = true;
      sv.vertical_enabled = false;
      head_x = sv.scroll_offset.x;
    }

    for (size_t c = 0; c < kCols; c++)
      div(context, mk(header.ent(), c),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(col_w), percent(1.f)})
              .with_label(fmt::format("col {}", c + 1))
              .with_custom_text_color(white)
              .with_font_size(11.f)
              .with_debug_name(fmt::format("ss_col_{}", c)));

    auto table =
        vstack(context, mk(entity, 5),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(table_w), pixels(150)})
                   .with_absolute_position(48.f, 506.f)
                   .with_custom_background(afterhours::Color{22, 26, 38, 255})
                   .with_border(muted, 1.f)
                   .with_overflow(Overflow::Scroll, Axis::X)
                   .with_debug_name("ss_table"));

    if (table.ent().has<HasScrollView>()) {
      auto &sv = table.ent().get<HasScrollView>();
      sv.sync_group = kHGroup;
      sv.horizontal_enabled = true;
      sv.vertical_enabled = false;
      sv.show_scrollbar = true;
      table_x = sv.scroll_offset.x;
    }

    for (size_t r = 0; r < 5; r++) {
      auto row = hstack(context, mk(table.ent(), r),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(kCols * col_w),
                                                     pixels(24)})
                            .with_debug_name(fmt::format("ss_row_{}", r)));
      for (size_t c = 0; c < kCols; c++)
        div(context, mk(row.ent(), c),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(col_w), percent(1.f)})
                .with_label(fmt::format("r{}c{}", r + 1, c + 1))
                .with_custom_text_color(muted)
                .with_font_size(11.f)
                .with_debug_name(fmt::format("ss_cell_{}_{}", r, c)));
    }

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(22)})
            .with_absolute_position(48.f, 664.f)
            .with_label(fmt::format(
                "x: header {:.0f}  table {:.0f}   {}", head_x, table_x,
                head_x == table_x ? "x-in-sync" : "x-drifting"))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_font_size(12.f)
            .with_debug_name("ss_readout_x"));
  }
};

REGISTER_EXAMPLE_SCREEN(sync_scroll_lab, "System Demos",
                        "Scroll views locked together by sync_group",
                        SyncScrollLab)
