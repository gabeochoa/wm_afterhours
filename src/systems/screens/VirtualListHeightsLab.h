#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Variable-height virtual list over the retained index: height_of is asked
// once per row until that row is invalidated, prepends hold the viewport
// on the same message, and the End button is one scroll_to_bottom call.
struct VirtualListHeightsLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr size_t kInitial = 300;
  size_t count = kInitial;
  long first_msg = 1000; // message number at row 0; prepends lower it
  int asked_last_frame = 0;
  int asked_this_frame = 0;
  std::map<long, float> grown; // message number -> height multiplier
  afterhours::EntityID list_id = -1;
  bool want_end = false;
  bool want_start = false;

  float height_of_msg(long msg) const {
    const long lines = 1 + (msg * 7) % 3;
    float h = static_cast<float>(lines) * 22.f + 10.f;
    if (auto it = grown.find(msg); it != grown.end())
      h *= it->second;
    return h;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("vlh_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    text(0, "Virtual list / variable heights", 16, 0, 760, 44, 30);
    text(1, "Rows are 1 to 3 lines tall. The retained index measures each row once; "
            "scrolling and rebuilding ask for none of them again.",
         16, 48, 1180, 28, 19);

    auto btn = [&](int id, const char *label, float x, const char *debug) {
      return button(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(190 * scale), pixels(40 * scale)})
          .with_absolute_position(x * scale, 96 * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(18 * scale))
          .with_custom_background({52, 74, 105, 255})
          .with_custom_text_color(theme.font).with_corner_radius(5 * scale)
          .with_debug_name(debug));
    };
    if (btn(2, "Prepend 20 older", 32, "vlh_prepend") && list_id >= 0) {
      auto &list_ent = AutoLayout::to_ent_static(list_id);
      prepend_virtual_rows(list_ent, 20);
      first_msg -= 20;
      count += 20;
    }
    if (btn(3, "Grow first row", 240, "vlh_grow") && list_id >= 0) {
      auto &list_ent = AutoLayout::to_ent_static(list_id);
      auto &scroll = list_ent.get<HasScrollView>();
      size_t row = 0;
      while (row + 1 < count &&
             virtual_row_offset(list_ent, row + 1) <= scroll.scroll_offset.y)
        row++;
      grown[first_msg + static_cast<long>(row)] = 2.f;
      invalidate_virtual_rows(list_ent, row, row + 1);
    }
    if (btn(4, "End", 448, "vlh_end")) want_end = true;
    if (btn(5, "Start", 656, "vlh_start")) want_start = true;

    asked_this_frame = 0;
    auto list = virtual_list(
        context, mk(root.ent(), 6), count,
        [&](size_t i) {
          asked_this_frame++;
          return height_of_msg(first_msg + static_cast<long>(i));
        },
        [&](size_t i, afterhours::Entity &row) {
          const long msg = first_msg + static_cast<long>(i);
          const long lines = 1 + (msg * 7) % 3;
          div(context, mk(row, 0), ComponentConfig{}
              .with_size({percent(1.f), percent(1.f)})
              .with_label(fmt::format("msg {} / {} line{}", msg, lines,
                                      lines == 1 ? "" : "s"))
              .with_font("AtkinsonMock", pixels(17 * scale))
              .with_custom_text_color(theme.font)
              .with_custom_background(i % 2 ? afterhours::Color{28, 39, 57, 255}
                                            : afterhours::Color{35, 48, 68, 255})
              .with_text_inset(8 * scale, 0)
              .with_debug_name(fmt::format("vlh_row_{}", i)));
        },
        ComponentConfig{}
            .with_size({pixels(760 * scale), pixels(430 * scale)})
            .with_absolute_position(32 * scale, 152 * scale)
            .with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * scale)
            .with_debug_name("vlh_list"));
    list_id = list.id();
    asked_last_frame = asked_this_frame;
    auto &scroll = list.ent().get<HasScrollView>();
    if (want_end) scroll.scroll_to_bottom();
    if (want_start) scroll.scroll_to_top();
    want_end = want_start = false;

    const float total = virtual_rows_total(list.ent());
    text(7, fmt::format("{} rows / total {:.0f}px / offset {:.0f}px", count,
                        total, scroll.scroll_offset.y),
         830, 152, 420, 30, 20);
    text(8, fmt::format("measured {} rows last frame", asked_last_frame),
         830, 190, 420, 30, 20);
    text(9, "Prepend holds the viewport on the same message: the new rows' "
            "height joins the scroll offset in the same build.",
         830, 240, 400, 90, 18);
    text(10, "Grow first row invalidates one row: the next build measures "
             "exactly that row, and rows below it shift by its new height.",
         830, 340, 400, 90, 18);
    text(11, "End and Start are single calls that move the offset, the "
             "target and the eased position together.",
         830, 440, 400, 90, 18);
  }
};

REGISTER_EXAMPLE_SCREEN(virtual_list_heights_lab, "System Demos",
                        "variable-height virtual list: retained index, prepend anchor, scroll_to_bottom",
                        VirtualListHeightsLab)
