#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

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
  std::string overflow_value =
      "the quick brown fox jumps over the lazy dog and keeps on running";

  static std::string describe_selection(const afterhours::text_input::HasTextInputState &s) {
    if (!s.has_selection())
      return "none";
    return fmt::format("{}..{}", s.selection_start(), s.selection_end());
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{255, 255, 255, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const float row_h = 26.f;
    float y = 96.f;

    auto line = [&](int id, const std::string &text, afterhours::Color color) {
      div(context, mk(entity, id),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(560), pixels(row_h)})
              .with_absolute_position(48.f, y)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(color)
              .with_debug_name(fmt::format("lab_line_{}", id)));
      y += row_h;
    };

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(600), pixels(40)})
            .with_absolute_position(48.f, 32.f)
            .with_label("Text Input Lab")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("lab_title"));

    auto field = text_input(context, mk(entity, 1), value,
                            ComponentConfig{}
                                .with_size(ComponentSize{pixels(360),
                                                         pixels(40)})
                                .with_absolute_position(48.f, 84.f)
                                .with_debug_name("lab_input"));

    y = 148.f;
    if (field.ent().has<afterhours::text_input::HasTextInputState>()) {
      const auto &s = field.ent().get<afterhours::text_input::HasTextInputState>();
      line(10, fmt::format("text:      \"{}\"", s.text()), white);
      line(11, fmt::format("cursor:    {}", s.cursor_position), muted);
      line(12, fmt::format("selection: {}", describe_selection(s)), muted);
      line(13, fmt::format("focused:   {}", s.is_focused ? "yes" : "no"), muted);
      line(14, fmt::format("scroll_x:  {:.1f}", s.scroll_offset_x), muted);
    }

    // Text wider than the field. It must not paint outside the box.
    y += row_h;
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(560), pixels(row_h)})
            .with_absolute_position(48.f, y)
            .with_label("overflow field - text must stay inside the box:")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_debug_name("lab_overflow_caption"));

    text_input(context, mk(entity, 3), overflow_value,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(240), pixels(40)})
                   .with_absolute_position(48.f, y + row_h + 8.f)
                   .with_debug_name("lab_overflow_input"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_input_lab, "System Demos",
                        "Single text input with a live state readout",
                        TextInputLab)
