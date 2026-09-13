#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_area.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// text_area's multiline features (D26): word wrap, auto-grow, Enter vs
// Shift+Enter, and clipping. Type past the right edge of each field to see it
// wrap; press Enter in the composer to send and Shift+Enter for a new line.
struct ComposerLab : ScreenSystem<UIContext<InputAction>> {
  // Long enough to start at three rows, so the box is visibly taller than the
  // one row it was configured with and auto-grow shows without typing.
  std::string composer = "Meet by the north gate at nine.\nBring the map and a warm jacket.\nWe will return before sunset.";
  std::string plain = "The town square is ready for the spring festival. Lanterns hang above the market stalls, "
                      "and the musicians are setting up beside the fountain. Everyone is welcome to join.";
  std::string nowrap = "wrapping is off on this one so this long line just "
                       "runs on and gets clipped at the edge";
  std::string clipped = "one\ntwo\nthree\nfour\nfive\nsix\nseven\neight\n"
                        "nine\nten\neleven\ntwelve";
  std::string last_submitted;
  int sends = 0;
  bool row_guides = false;

  static std::array<float, 4> viewport(ElementResult field) {
    auto &outer = field.cmp();
    if (outer.children.empty()) return {outer.rect().width, outer.rect().height, 0, 0};
    auto child = UICollectionHolder::getEntityForID(outer.children.front());
    if (!child.valid()) return {outer.rect().width, outer.rect().height, 0, 0};
    const auto &inner = child.asE().get<UIComponent>();
    const auto rect = inner.rect();
    return {std::max(0.f, rect.width - inner.computed_padd[Axis::X]),
            std::max(0.f, rect.height - inner.computed_padd[Axis::Y]),
            inner.computed_padd[Axis::left], inner.computed_padd[Axis::top]};
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.secondary = {36, 46, 65, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{181, 194, 216, 255};
    const auto border = afterhours::Color{86, 108, 141, 255};
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const float line_height = 20.f * s;
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({18, 23, 33, 255}).with_corner_radius(0).with_debug_name("cl_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h, float size,
                           afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_font("AtkinsonMock", pixels(size * s)).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto panel = [&](float x, float y, float w, float h) {
      div(context, mk(root.ent(), id++), box(left + x, y, w, h)
          .with_custom_background({29, 36, 50, 255}).with_corner_radius(10 * s));
    };
    const auto control = [&](const std::string &text, float x, float y, float w, const std::string &name) {
      return button(context, mk(root.ent(), id++), box(left + x, y, w, 30).with_label(text)
          .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(white)
          .with_custom_background({53, 74, 105, 255}).with_corner_radius(6 * s).with_debug_name(name));
    };
    const auto field_config = [&](float y, float height, const std::string &name) {
      return box(left + 20, y, 684, height).with_line_height(pixels(line_height))
          .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(white)
          .with_corner_radius(6 * s).with_debug_name(name);
    };
    panel(0, 24, 1144, 88);
    label("Composer lab", 20, 32, 790, 42, 34, white, "cl_title");
    label("Edit each field to compare wrapping, growing, sending and clipping.", 20, 78, 902, 26, 20, muted);
    if (control(row_guides ? "Hide row guides" : "Show row guides", 942, 38, 180, "cl_guides")) row_guides = !row_guides;

    // 1. The chat composer: grows as you type, capped at 5 rows, Enter sends.
    panel(0, 124, 728, 192);
    label("1. Growing composer", 16, 130, 486, 28, 24, white);
    label(fmt::format("Submissions: {}", sends), 514, 133, 194, 26, 19, white, "cl_send_count");
    label("auto_grow + submit_on_enter / Enter sends / Shift+Enter adds a line", 16, 160, 696, 22, 17, muted);
    div(context, mk(root.ent(), id++), box(left + 18, 184, 688, 108)
        .with_size({pixels(688 * s), pixels(5 * line_height + 12)})
        .with_ignore_pointer_events().with_on_draw_fg([border, s](RectangleType rect) {
          raylib::DrawRectangleLinesEx(rect, s, border);
        }));
    auto sent = text_area(context, mk(root.ent(), 1000), composer,
                          field_config(186, 28, "cl_composer").with_auto_grow().with_max_lines(5).with_submit_on_enter());
    // on_submit is what submit_on_enter fires; without a listener Enter would
    // just do nothing, which would read as the feature being broken.
    sent.ent().addComponentIfMissing<afterhours::text_input::HasTextInputListener>();
    sent.ent().get<afterhours::text_input::HasTextInputListener>().on_submit =
        [this](afterhours::Entity &submitted) {
          auto &state = submitted.get<afterhours::text_input::HasTextAreaState>();
          last_submitted = state.text();
          ++sends;
          state.storage.clear();
          state.cursor_position = 0;
          state.scroll_offset_y = 0;
          state.clear_selection();
          state.rebuild_line_index();
          composer.clear();
        };
    const auto &composer_state = sent.ent().get<afterhours::text_input::HasTextAreaState>();
    label(fmt::format("Visible rows {} / cap 5  |  {} total visual rows  |  outline marks maximum height",
                      std::min<size_t>(5, composer_state.layout_cache.line_count()), composer_state.layout_cache.line_count()),
          16, 296, 696, 20, 16, muted, "cl_composer_rows");

    // 2. Fixed box, wrapping on. Enter breaks the line, the long-standing
    //    default.
    panel(0, 326, 728, 144);
    label("2. Fixed height, wrapping on", 16, 328, 696, 27, 24, white);
    label("Enter inserts a newline. Type past the edge to wrap. Row guides are optional.", 16, 354, 696, 20, 16, muted);
    auto wrapped = text_area(context, mk(root.ent(), 1100), plain, field_config(374, 88, "cl_plain").with_size({pixels(580 * s), pixels(88 * s)}));
    const auto plain_view = viewport(wrapped);
    label(fmt::format("{:.0f} full rows", std::floor(plain_view[1] / line_height)), 612, 386, 100, 24, 16, muted);
    label("Fixed height", 612, 416, 100, 24, 16, muted);
    div(context, mk(root.ent(), id++), box(left + 20, 374, 580, 88).with_render_layer(1)
        .with_ignore_pointer_events().with_on_draw_fg([show = row_guides, line_height, plain_view](RectangleType r) {
          if (!show) return;
          for (float y = plain_view[3] + line_height; y < plain_view[3] + plain_view[1]; y += line_height)
            raylib::DrawLineEx({r.x + plain_view[2], r.y + y}, {r.x + plain_view[2] + plain_view[0], r.y + y},
                               1, {88, 113, 144, 150});
        }));

    // 3. The control for panel 2: same width, wrapping off.
    panel(0, 480, 728, 88);
    label("3. No wrapping", 16, 484, 696, 28, 24, white);
    auto unwrapped = text_area(context, mk(root.ent(), 1200), nowrap,
                               field_config(536, 28, "cl_nowrap").with_size({pixels(580 * s), pixels(28 * s)}).with_word_wrap(false));
    const auto &unwrapped_state = unwrapped.ent().get<afterhours::text_input::HasTextAreaState>();
    const auto unwrapped_view = viewport(unwrapped);
    label(fmt::format("Clipped by design / {} UTF-8 bytes / {:.0f}px beyond the visible width", nowrap.size(),
                      std::max(0.f, unwrapped_state.layout_cache.max_width() - unwrapped_view[0])),
          16, 512, 696, 20, 16, muted, "cl_nowrap_extent");

    // 4. More rows than fit: they clip at the bottom edge.
    panel(0, 578, 728, 126);
    label("4. Scrollable text", 16, 582, 496, 29, 24, white);
    auto scrolled = text_area(context, mk(root.ent(), 1300), clipped, field_config(634, 68, "cl_clipped"));
    auto &scroll_state = scrolled.ent().get<afterhours::text_input::HasTextAreaState>();
    const auto scroll_view = viewport(scrolled);
    if (control("Top", 548, 582, 72, "cl_scroll_top")) {
      scroll_state.cursor_position = 0;
      scroll_state.clear_selection();
      scroll_state.ensure_cursor_visible_at_row(0, scroll_view[1], scroll_state.layout_cache.line_count());
      context.set_focus(scrolled.cmp().children.front());
    }
    if (control("Bottom", 630, 582, 78, "cl_scroll_bottom")) {
      scroll_state.cursor_position = scroll_state.text_size();
      scroll_state.clear_selection();
      scroll_state.ensure_cursor_visible_at_row(scroll_state.layout_cache.line_count() - 1,
                                                scroll_view[1], scroll_state.layout_cache.line_count());
      context.set_focus(scrolled.cmp().children.front());
    }
    const size_t total_rows = scroll_state.layout_cache.line_count();
    const size_t first = std::min(total_rows, static_cast<size_t>(scroll_state.scroll_offset_y / line_height) + 1);
    const size_t last = std::min(total_rows, static_cast<size_t>(std::ceil((scroll_state.scroll_offset_y + scroll_view[1]) / line_height)));
    label(fmt::format("Rows {}-{} of {} / {:.0f}px / Wheel scrolls, buttons move caret", first, last, total_rows,
                      scroll_state.scroll_offset_y), 16, 611, 696, 22, 16, muted, "cl_scroll_range");
    div(context, mk(root.ent(), id++), box(left + 713, 634, 5, 68)
        .with_custom_background({66, 79, 100, 255}).with_ignore_pointer_events()
        .with_on_draw_fg([offset = scroll_state.scroll_offset_y, height = scroll_view[1], total = total_rows * line_height](RectangleType r) {
          if (total <= 0) return;
          const float thumb = std::min(1.f, height / total) * r.height;
          const float travel = std::max(0.f, total - height);
          const float fraction = travel > 0 ? std::clamp(offset / travel, 0.f, 1.f) : 0;
          afterhours::draw_rectangle({r.x, r.y + fraction * (r.height - thumb), r.width, thumb}, {176, 207, 250, 255});
        }));

    // What the composer's wrap actually resolved to, so a mis-mapped caret or
    // a wrong row count shows up in a screenshot rather than only by eye.
    const std::array<afterhours::Entity *, 4> fields{&sent.ent(), &wrapped.ent(), &unwrapped.ent(), &scrolled.ent()};
    const std::array<const char *, 4> names{"Composer", "Wrapping", "No wrap", "Scrolling"};
    int active = 0;
    for (int i = 0; i < 4; ++i)
      if (fields[i]->get<afterhours::text_input::HasTextAreaState>().is_focused) active = i;
    const auto &state = fields[active]->get<afterhours::text_input::HasTextAreaState>();
    const auto &cache = state.layout_cache;
    panel(752, 124, 392, 230);
    label(fmt::format("State / {}. {}", active + 1, names[active]), 768, 133, 360, 30, 24, white, "cl_active_field");
    // Label and value as two columns: padding the label with spaces left
    // the values ragged, because the face is proportional.
    const std::array<std::pair<const char *, std::string>, 7> state_rows{{
        {"Visual rows", std::to_string(cache.line_count())},
        {"Cursor (byte offset)", std::to_string(state.cursor_position)},
        {"Row / byte col (0-based)", fmt::format("{} / {}", cache.line_at_offset(state.cursor_position), cache.column_at_offset(state.cursor_position))},
        {"Focused", state.is_focused ? "yes" : "no"},
        {"Scroll Y (px)", fmt::format("{:.1f}", state.scroll_offset_y)},
        {"Widest visual line (px)", fmt::format("{:.0f}", cache.max_width())},
        {"Text length (UTF-8 bytes)", std::to_string(state.text_size())}}};
    for (size_t i = 0; i < state_rows.size(); ++i) {
      label(state_rows[i].first, 768, 168 + i * 26.f, 248, 25, 17, muted);
      label(state_rows[i].second, 1020, 168 + i * 26.f, 108, 25, 18, white, "cl_state_" + std::to_string(i));
    }
    panel(752, 366, 392, 86);
    label(fmt::format("Last submission / {} bytes", last_submitted.size()), 768, 373, 360, 28, 21, white);
    std::string preview = last_submitted;
    for (char &c : preview) if (c == '\n') c = ' ';
    div(context, mk(root.ent(), id++), box(left + 768, 409, 360, 32)
        .with_label(sends == 0 ? "Nothing submitted yet" : preview.empty() ? "Empty submission" : preview)
        .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(muted)
        .with_text_overflow(TextOverflow::Ellipsis).with_alignment(TextAlignment::Left)
        .with_ignore_pointer_events().with_debug_name("cl_last_submission"));

    // The keys the widget answers to, so the screen documents its own controls.
    panel(752, 464, 392, 240);
    label("Keyboard and mouse", 768, 472, 360, 29, 23, white);
#ifdef __APPLE__
    const std::array<const char *, 8> shortcuts{
        "Move: Option + Left/Right by word", "Up/Down: row / Home/End: row ends",
        "Edit: Option + Backspace/Delete", "Select: Shift + move / Cmd+A all",
        "Cmd+C/X/V: copy, cut, paste", "Cmd+Z: undo / Cmd+Shift+Z: redo",
        "Mouse: click caret / drag selection", "Double-click word / triple-click row"};
#else
    const std::array<const char *, 8> shortcuts{
        "Move: Ctrl + Left/Right by word", "Up/Down: row / Home/End: row ends",
        "Edit: Ctrl + Backspace/Delete", "Select: Shift + move / Ctrl+A all",
        "Ctrl+C/X/V: copy, cut, paste", "Ctrl+Z: undo / Ctrl+Shift+Z: redo",
        "Mouse: click caret / drag selection", "Double-click word / triple-click row"};
#endif
    for (size_t i = 0; i < shortcuts.size(); ++i)
      label(shortcuts[i], 768, 508 + i * 23.f, 360, 23, 17, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(composer_lab, "System Demos",
                        "text_area wrap, auto-grow and Enter vs Shift+Enter",
                        ComposerLab)
