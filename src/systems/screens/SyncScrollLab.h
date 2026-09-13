#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>

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
  std::array<float, 3> viewport_h{}, content_h{};
  bool reset_offsets = false;
  bool show_clamp = false;
  float scale = 1.f;

  static bool is_changed(size_t i) { return i % 7 == 3 || i % 11 == 5; }

  // `rows` differs per pane: a shorter member must stop at its own end.
  void pane(UIContext<InputAction> &context, afterhours::Entity &parent,
            int slot, const char *title, float x, float w, size_t rows,
            float *out_y) {
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{181, 196, 219, 255};
    const auto tint = afterhours::Color{slot == 1 ? 68 : 32,
                                        slot == 1 ? 34 : 62,
                                        slot == 1 ? 38 : 42, 255};

    div(context, mk(parent, 100 + slot),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(w * scale), pixels(24 * scale)})
            .with_absolute_position(x * scale, 84.f * scale)
            .with_label(fmt::format("{} / {} rows", title, rows))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font("AtkinsonMock", pixels((slot == 3 ? 16.f : 18.f) * scale))
            .with_debug_name(fmt::format("ss_head_{}", slot)));

    auto view =
        vstack(context, mk(parent, 110 + slot),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(w * scale), pixels(300 * scale)})
                   .with_absolute_position(x * scale, 110.f * scale)
                   .with_custom_background(afterhours::Color{22, 26, 38, 255})
                   .with_border(muted, 1.f)
                   .with_overflow(Overflow::Scroll, Axis::Y)
                   .with_padding(Padding::all(pixels(4 * scale)))
                   .with_debug_name(fmt::format("ss_pane_{}", slot)));

    if (view.ent().has<HasScrollView>()) {
      auto &sv = view.ent().get<HasScrollView>();
      sv.sync_group = kVGroup;
      sv.show_scrollbar = true;
      if (reset_offsets) {
        sv.scroll_offset = {0, 0};
        sv.scroll_target = {0, 0};
        sv.last_eased_offset = {0, 0};
      }
      if (show_clamp && slot == 1) {
        sv.scroll_offset.y = 400 * scale;
        sv.scroll_target.y = sv.scroll_offset.y;
        sv.last_eased_offset.y = sv.scroll_offset.y;
        sv.clamp_scroll();
      }
      *out_y = sv.scroll_offset.y;
      viewport_h[static_cast<size_t>(slot - 1)] = sv.viewport_or_zero().y;
      content_h[static_cast<size_t>(slot - 1)] = sv.content_size.y;
    }

    for (size_t i = 0; i < rows; i++) {
      const bool changed = is_changed(i);
      auto cfg = ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(18 * scale)})
                     .with_label(fmt::format("{:>3} {} {}", i + 1,
                         changed ? (slot == 1 ? "-" : "+") : " ",
                         changed ? (slot == 1 ? "value = compute(x)" : "value = cached(x)") : "// context"))
                     .with_alignment(TextAlignment::Left)
                     .with_custom_text_color(changed ? white : muted)
                     .with_font("NotoSansKR", pixels(15.f * scale)).with_letter_spacing(-1.f)
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
    const auto muted = afterhours::Color{181, 196, 219, 255};

    scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * scale) / 2;
    const float top = (context.screen_height - 720 * scale) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * scale));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background({15, 22, 35, 255}).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * scale), pixels(720 * scale)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("ss_root"));
    const auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    const auto text = [&](int id, std::string value, float x, float y, float w, float h,
                          float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * scale))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Synchronized scrolling", 48, 14, 680, 40, 34, white, "ss_title", true);
    text(1, "Vertical group / scroll Before; the shorter content stops at its end.",
         48, 53, 1180, 28, 20, muted, "ss_hint");
    const auto action = [&](int id, const char *label, float x, float w, const char *debug) {
      return button(context, mk(root.ent(), id), box(x, 18, w, 34).with_label(label)
          .with_font("AtkinsonMock", pixels(18 * scale)).with_custom_background({65, 89, 119, 255})
          .with_custom_text_color(white).with_corner_radius(6 * scale).with_debug_name(debug));
    };
    reset_offsets = action(10, "Reset all offsets", 992, 236, "ss_reset");
    show_clamp = action(11, "Show short-pane limit", 752, 224, "ss_clamp");
    pane(context, root.ent(), 1, "Before", 48.f, 340.f, 120, &left_y);
    pane(context, root.ent(), 2, "After", 400.f, 340.f, 120, &right_y);
    pane(context, root.ent(), 3, "Short content", 752.f, 200.f, 24, &short_y);
    text(2, fmt::format("Before y: {:.0f} px", left_y), 48, 416, 330, 27, 19, white, "ss_readout_y");
    text(3, fmt::format("After y: {:.0f} px", right_y), 400, 416, 330, 27, 19, white, "ss_after_y");
    text(4, fmt::format("Short y: {:.0f} px", short_y), 752, 416, 200, 27, 19, white, "ss_short_y");
    div(context, mk(root.ent(), 20), box(980, 110, 248, 570).with_custom_background({28, 39, 57, 255}).with_corner_radius(8 * scale));
    text(21, "Sync rules", 992, 122, 224, 32, 26, white, "", true);
    text(22, "Same group, same offset.", 992, 166, 224, 25, 17, muted);
    text(23, "Each view keeps its limits.", 992, 196, 224, 25, 17, muted);
    text(24, "- Before: removed", 992, 236, 224, 25, 18, {238, 154, 162, 255});
    text(25, "+ After: added", 992, 266, 224, 25, 18, {147, 219, 169, 255});
    text(26, "Viewport / content height", 992, 309, 224, 25, 16, muted);
    for (size_t i = 0; i < viewport_h.size(); ++i) {
      const char *names[] = {"Before", "After", "Short"};
      text(30 + static_cast<int>(i), fmt::format("{} {:.0f} / {:.0f} px", names[i], viewport_h[i], content_h[i]),
           992, 340 + static_cast<float>(i) * 29, 224, 25, 17, white);
    }
    const float short_max = std::max(0.f, content_h[2] - viewport_h[2]);
    const bool following = std::abs(short_y - left_y) < 0.5f;
    const bool clamped = std::abs(short_y - short_max) < 0.5f && left_y > short_max;
    text(40, std::abs(left_y - right_y) < 0.5f ? "Matching vertical offsets" : "Vertical offsets differ",
         992, 439, 224, 26, 17, white, "ss_sync_y");
    text(41, following ? "Short pane following" : clamped ? "Short pane at end" : "Short pane differs",
         992, 470, 224, 26, 17, white, "ss_short_state");
    text(42, "Horizontal group", 992, 521, 224, 30, 22, white, "", true);
    text(43, "Drag the table scrollbar.", 992, 559, 224, 25, 17, muted);
    text(44, "The header bar also works.", 992, 589, 224, 25, 17, muted);
    text(45, "Both scroll independently", 992, 629, 224, 23, 16, muted);
    text(46, "and share their x offset.", 992, 652, 224, 23, 16, muted);
    horizontal_group(context, root.ent());
    reset_offsets = false;
    show_clamp = false;
  }

  // Group 2: header synced on x only, over a wide scrolling table.
  void horizontal_group(UIContext<InputAction> &context,
                        afterhours::Entity &entity) {
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{181, 196, 219, 255};
    const float table_w = 900.f * scale;
    const float col_w = 120.f * scale;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(900 * scale), pixels(26 * scale)})
            .with_absolute_position(48.f * scale, 448.f * scale)
            .with_label("Horizontal header/table group / drag either scrollbar")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font("AtkinsonMock", pixels(18.f * scale))
            .with_debug_name("ss_hint_h"));

    auto header =
        hstack(context, mk(entity, 4),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(table_w), pixels(26 * scale)})
                   .with_absolute_position(48.f * scale, 478.f * scale)
                   .with_custom_background(afterhours::Color{40, 48, 70, 255})
                   .with_border(muted, 1.f)
                   .with_overflow(Overflow::Scroll, Axis::X)
                   .with_debug_name("ss_header"));

    if (header.ent().has<HasScrollView>()) {
      auto &sv = header.ent().get<HasScrollView>();
      sv.sync_group = kHGroup;
      sv.horizontal_enabled = true;
      sv.vertical_enabled = false;
      if (reset_offsets) {
        sv.scroll_offset = {0, 0};
        sv.scroll_target = {0, 0};
        sv.last_eased_offset = {0, 0};
      }
      head_x = sv.scroll_offset.x;
    }

    for (size_t c = 0; c < kCols; c++)
      div(context, mk(header.ent(), c),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(col_w), percent(1.f)})
              .with_label(fmt::format("Column {}", c + 1))
              .with_custom_text_color(white)
              .with_font("NotoSansKR", pixels(15.f * scale)).with_letter_spacing(-1.f)
              .with_text_inset(8 * scale, 2 * scale)
              .with_debug_name(fmt::format("ss_col_{}", c)));

    auto table =
        vstack(context, mk(entity, 5),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(table_w), pixels(150 * scale)})
                   .with_absolute_position(48.f * scale, 506.f * scale)
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
      if (reset_offsets) {
        sv.scroll_offset = {0, 0};
        sv.scroll_target = {0, 0};
        sv.last_eased_offset = {0, 0};
      }
      table_x = sv.scroll_offset.x;
    }

    for (size_t r = 0; r < 5; r++) {
      auto row = hstack(context, mk(table.ent(), r),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(kCols * col_w),
                                                     pixels(24 * scale)})
                            .with_debug_name(fmt::format("ss_row_{}", r)));
      for (size_t c = 0; c < kCols; c++)
        div(context, mk(row.ent(), c),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(col_w), percent(1.f)})
                .with_label(fmt::format("R{} / C{}", r + 1, c + 1))
                .with_custom_text_color(muted)
                .with_font("NotoSansKR", pixels(15.f * scale)).with_letter_spacing(-1.f)
                .with_text_inset(8 * scale, 2 * scale)
                .with_on_draw_fg([](RectangleType cell_rect) {
                  afterhours::draw_rectangle({cell_rect.x + cell_rect.width - 1, cell_rect.y, 1, cell_rect.height}, afterhours::Color{48, 62, 81, 255});
                })
                .with_debug_name(fmt::format("ss_cell_{}_{}", r, c)));
    }

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(900 * scale), pixels(26 * scale)})
            .with_absolute_position(48.f * scale, 664.f * scale)
            .with_label(fmt::format(
                "Header x: {:.0f} px / Table x: {:.0f} px / {}", head_x, table_x,
                std::abs(head_x - table_x) < 0.5f ? "Matching horizontal offsets" : "Horizontal offsets differ"))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_font("AtkinsonMock", pixels(18.f * scale))
            .with_debug_name("ss_readout_x"));
  }
};

REGISTER_EXAMPLE_SCREEN(sync_scroll_lab, "System Demos",
                        "Scroll views locked together by sync_group",
                        SyncScrollLab)
