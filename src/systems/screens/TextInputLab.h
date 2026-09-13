#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// A single text input plus a live dump of its state. TextInputDemo shows the
// widget in a realistic form; this one exists to make the widget's INTERNALS
// visible -- caret position, selection range, scroll offset -- so a bug in any
// of them shows up in a screenshot instead of only under a debugger.
//
// The second field is pre-filled with text far wider than its box. Whether that
// text stays inside the box is the whole point of it.
struct TextInputLab : ScreenSystem<UIContext<InputAction>> {
  std::string value = "hello world";
  std::string overflow_value = "the quick brown fox jumps over the lazy dog and keeps on running";
  bool initial_focus = true;
  std::string previous_text = value;
  size_t previous_cursor = 11, previous_start = 0, previous_end = 11;
  bool previous_focus = false;
  std::array<std::string, 3> history{"Initial fixture: hello world", "", ""};

  void record(std::string action) {
    if (action == history[0]) return;
    history[2] = history[1];
    history[1] = history[0];
    history[0] = std::move(action);
  }

  static size_t codepoints(const std::string &text) {
    size_t count = 0;
    for (size_t i = 0; i < text.size(); i += afterhours::text_input::utf8_char_length(text, i)) ++count;
    return count;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * scale) / 2;
    const float top = (context.screen_height - 720 * scale) / 2;
    const afterhours::Color white{235, 240, 250, 255}, muted{176, 193, 217, 255};
    const afterhours::Color panel{28, 40, 58, 255}, accent{127, 218, 218, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background({14, 21, 34, 255}).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * scale), pixels(720 * scale)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true).with_debug_name("lab_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string label, float x, float y, float w, float h, float size,
                    afterhours::Color color, const char *debug = "", bool bold = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(label)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * scale))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Text input inspector", 48, 20, 1184, 44, 36, white, "lab_title", true);
    text(4, "Observe caret, selection and horizontal scrolling in native fields.", 48, 70, 1184, 28, 22, muted);
    text(5, "Selection and caret specimen", 48, 110, 560, 28, 23, white, "", true);
    auto field = text_input(context, mk(root.ent(), 1), value, box(48, 148, 360, 40)
        .with_font("AtkinsonMock", pixels(20 * scale)).with_custom_background(panel)
        .with_border(muted, 1.f).with_debug_name("lab_input"));
    bool reset = button(context, mk(root.ent(), 6), box(426, 148, 182, 40)
        .with_label("Reset fixture").with_font("AtkinsonMock", pixels(20 * scale))
        .with_custom_background(accent).with_custom_text_color({14, 21, 34, 255})
        .with_corner_radius(6 * scale).with_debug_name("lab_reset"));
    const bool load_unicode = button(context, mk(root.ent(), 70), box(910, 148, 322, 40)
        .with_label("Primary: load café").with_font("AtkinsonMock", pixels(20 * scale))
        .with_custom_background(accent).with_custom_text_color({14, 21, 34, 255})
        .with_corner_radius(6 * scale).with_debug_name("lab_unicode"));
    if (load_unicode) {
      value = "café";
      record("Loaded Unicode sample: café");
    }
    if (reset) {
      value = "hello world";
      overflow_value = "the quick brown fox jumps over the lazy dog and keeps on running";
      record("Reset fixture: all text selected");
    }
    auto &state = field.ent().get<afterhours::text_input::HasTextInputState>();
    if (reset || load_unicode || initial_focus) {
      if (reset || load_unicode) {
        state.storage.clear();
        state.storage.insert(0, value);
        state.undo_stack.clear();
        state.undo_index = 0;
        state.scroll_offset_x = 0;
      }
      state.cursor_position = value.size();
      state.selection_anchor = 0;
      for (const auto child_id : field.cmp().children) {
        const auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child || !child.asE().has<InFocusCluster>()) continue;
        context.set_focus(child_id);
        break;
      }
      initial_focus = false;
    }
    if (!reset && !load_unicode) {
      if (state.text() != previous_text) record("Primary text edited");
      else if (state.has_selection() && (state.selection_start() != previous_start || state.selection_end() != previous_end))
        record(fmt::format("Selection bytes {} to {}", state.selection_start(), state.selection_end()));
      else if (state.cursor_position != previous_cursor) record(fmt::format("Caret moved to byte {}", state.cursor_position));
      else if (previous_start != previous_end && !state.has_selection()) record("Selection cleared");
      else if (state.is_focused != previous_focus) record(state.is_focused ? "Primary field focused" : "Primary field left focus");
    }
    previous_text = state.text();
    previous_cursor = state.cursor_position;
    previous_start = state.selection_start();
    previous_end = state.selection_end();
    previous_focus = state.is_focused;
    div(context, mk(root.ent(), 7), box(48, 218, 560, 350).with_custom_background(panel).with_corner_radius(10 * scale));
    text(8, "Inspector / primary field", 68, 232, 380, 32, 25, white, "", true);
    text(9, state.is_focused ? "Focused" : "Unfocused", 454, 235, 134, 27, 19, state.is_focused ? accent : muted, "lab_focus");
    const std::array<std::string, 6> labels{"Caret byte index", "Selection start byte", "Selection end byte", "Selected code points", "Horizontal scroll", "Text bytes / code points"};
    const std::array<std::string, 6> values{
        std::to_string(state.cursor_position), std::to_string(state.selection_start()),
        std::to_string(state.selection_end()), std::to_string(codepoints(state.selected_text())),
        fmt::format("{:.1f} px", state.scroll_offset_x), fmt::format("{} / {}", state.text_size(), codepoints(state.text()))};
    for (size_t i = 0; i < labels.size(); ++i) {
      text(10 + static_cast<int>(i), labels[i], 68, 282 + static_cast<float>(i) * 40, 320, 28, 19, muted);
      text(20 + static_cast<int>(i), values[i], 414, 282 + static_cast<float>(i) * 40, 174, 28, 22, white,
           i == 0 ? "lab_caret" : i == 1 ? "lab_selection_start" : i == 2 ? "lab_selection_end" : i == 3 ? "lab_selected_count" : i == 4 ? "lab_scroll" : "lab_text_count", true);
    }
    text(30, "Indices start at 0; selection end is exclusive.", 68, 531, 520, 26, 18, muted);
    text(31, "Initial/reset state: hello world is fully selected.", 48, 589, 560, 26, 19, muted);
    text(32, "UTF-8 byte indices differ from Unicode code points.", 48, 625, 560, 26, 18, muted);
    text(33, "A code point is not always a complete visible character.", 48, 657, 560, 26, 18, muted);
    // Text wider than the field. It must not paint outside the box.
    text(40, "Overflow specimen", 648, 110, 584, 28, 23, white, "lab_overflow_caption", true);
    auto overflow = text_input(context, mk(root.ent(), 3), overflow_value, box(648, 148, 240, 40)
        .with_font("AtkinsonMock", pixels(20 * scale)).with_custom_background(panel)
        .with_border(muted, 1.f).with_debug_name("lab_overflow_input"));
    if (reset) {
      auto &overflow_state = overflow.ent().get<afterhours::text_input::HasTextInputState>();
      overflow_state.scroll_offset_x = 0;
      overflow_state.clear_selection();
    }
    for (int i = 0; i < 2; ++i) div(context, mk(root.ent(), 41 + i), box(i == 0 ? 646 : 889, 142, 1, 52).with_custom_background(accent));
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const float text_width = fonts ? afterhours::measure_text(fonts->get_font("AtkinsonMock"), overflow_value.c_str(), 20 * scale, 1.f).x : 0;
    text(43, fmt::format("Field {:.0f} px / full text {:.0f} px", overflow.cmp().rect().width, text_width), 648, 207, 584, 27, 19, muted, "lab_overflow_size");
    text(44, "Full source / overflow field", 648, 246, 584, 26, 19, accent);
    text_area(context, mk(root.ent(), 45), overflow_value, box(648, 278, 584, 84)
        .with_font("AtkinsonMock", pixels(18 * scale)).with_line_height(pixels(23 * scale))
        .with_custom_text_color(white).with_custom_background(panel).with_readonly()
        .with_word_wrap(true).with_debug_name("lab_overflow_source"));
    text(46, "Try it", 648, 379, 584, 30, 25, white, "", true);
    text(47, "Ctrl/Cmd+A selects all. Shift+Right extends a selection.", 648, 420, 584, 28, 18, muted);
    text(48, "Left/Right moves the caret. End reveals the overflow tail.", 648, 455, 584, 28, 18, muted);
    text(49, "The cyan ticks mark the overflow field's outside edges.", 648, 490, 584, 28, 18, muted);
    text(50, "Last observed actions / primary field", 648, 535, 584, 30, 23, white, "", true);
    for (size_t i = 0; i < history.size(); ++i) text(60 + static_cast<int>(i), history[i], 648, 576 + static_cast<float>(i) * 35, 584, 28, 19, i == 0 ? accent : muted, i == 0 ? "lab_last_action" : "");
  }
};

REGISTER_EXAMPLE_SCREEN(text_input_lab, "System Demos",
                        "Single text input with a live state readout",
                        TextInputLab)
