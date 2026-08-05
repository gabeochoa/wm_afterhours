#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_area.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// text_area's multiline features (D26): word wrap, auto-grow, Enter vs
// Shift+Enter, and clipping. Type past the right edge of each field to see it
// wrap; press Enter in the composer to send and Shift+Enter for a new line.
struct ComposerLab : ScreenSystem<UIContext<InputAction>> {
  // Long enough to start at three rows, so the box is visibly taller than the
  // one row it was configured with and auto-grow shows without typing.
  std::string composer =
      "Enter sends this and clears the box. Shift+Enter adds a line instead. "
      "The field starts one row tall and grows with the text, up to five.";
  std::string plain = "Enter adds a line here instead. Type past the edge to "
                      "watch this paragraph wrap onto more rows.";
  std::string nowrap = "wrapping is off on this one so this long line just "
                       "runs on and gets clipped at the edge";
  std::string clipped = "one\ntwo\nthree\nfour\nfive\nsix\nseven\neight";

  int sends = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const float W = 420.f;

    int id = 1;
    auto caption = [&](float y, const std::string &text) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(720), pixels(24)})
              .with_absolute_position(48.f, y)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font(UIComponent::DEFAULT_FONT, pixels(13.f))
              .with_debug_name(fmt::format("cl_cap_{}", id)));
    };

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(760), pixels(36)})
            .with_absolute_position(48.f, 24.f)
            .with_label("Composer Lab - wrap, auto-grow, Enter vs Shift+Enter")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("cl_title"));

    // 1. The chat composer: grows as you type, capped at 5 rows, Enter sends.
    caption(68.f, fmt::format("1. auto-grow + submit_on_enter (capped at 5 "
                              "rows) - sends: {}",
                              sends));
    auto sent = text_area(context, mk(entity, 100), composer,
                          ComponentConfig{}
                              .with_size(ComponentSize{pixels(W), pixels(30)})
                              .with_absolute_position(48.f, 94.f)
                              .with_line_height(pixels(22.f))
                              .with_font(UIComponent::DEFAULT_FONT, pixels(16.f))
                              .with_auto_grow()
                              .with_max_lines(5)
                              .with_submit_on_enter()
                              .with_debug_name("cl_composer"));
    (void)sent;
    // on_submit is what submit_on_enter fires; without a listener Enter would
    // just do nothing, which would read as the feature being broken.
    sent.ent().addComponentIfMissing<afterhours::text_input::HasTextInputListener>();
    sent.ent().get<afterhours::text_input::HasTextInputListener>().on_submit =
        [this](afterhours::Entity &) {
          sends++;
          composer.clear();
        };

    // 2. Fixed box, wrapping on. Enter breaks the line, the long-standing
    //    default.
    caption(250.f, "2. word wrap on, Enter inserts a newline (the default)");
    text_area(context, mk(entity, 200), plain,
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(W), pixels(96)})
                  .with_absolute_position(48.f, 276.f)
                  .with_line_height(pixels(22.f))
                  .with_font(UIComponent::DEFAULT_FONT, pixels(16.f))
                  .with_debug_name("cl_plain"));

    // 3. The control for panel 2: same width, wrapping off.
    caption(384.f, "3. word wrap OFF - one row per source line, clipped");
    text_area(context, mk(entity, 300), nowrap,
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(W), pixels(48)})
                  .with_absolute_position(48.f, 410.f)
                  .with_line_height(pixels(22.f))
                  .with_font(UIComponent::DEFAULT_FONT, pixels(16.f))
                  .with_word_wrap(false)
                  .with_debug_name("cl_nowrap"));

    // 4. More rows than fit. They must stop at the bottom edge rather than
    //    painting over the readout below.
    caption(474.f, "4. eight lines in a three-line box - must clip, not spill");
    text_area(context, mk(entity, 400), clipped,
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(W), pixels(70)})
                  .with_absolute_position(48.f, 500.f)
                  .with_line_height(pixels(22.f))
                  .with_font(UIComponent::DEFAULT_FONT, pixels(16.f))
                  .with_debug_name("cl_clipped"));

    // What the composer's wrap actually resolved to, so a mis-mapped caret or
    // a wrong row count shows up in a screenshot rather than only by eye.
    float ry = 94.f;
    auto say = [&](const std::string &text) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(280), pixels(22)})
              .with_absolute_position(520.f, ry)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font(UIComponent::DEFAULT_FONT, pixels(13.f))
              .with_debug_name(fmt::format("cl_say_{}", id)));
      ry += 22.f;
    };
    if (sent.ent().has<afterhours::text_input::HasTextAreaState>()) {
      const auto &s =
          sent.ent().get<afterhours::text_input::HasTextAreaState>();
      const auto &cache = s.layout_cache;
      say("composer state");
      say(fmt::format("rows:      {}", cache.line_count()));
      say(fmt::format("cursor:    {}", s.cursor_position));
      say(fmt::format("row/col:   {}/{}",
                      cache.line_at_offset(s.cursor_position),
                      cache.column_at_offset(s.cursor_position)));
      say(fmt::format("focused:   {}", s.is_focused ? "yes" : "no"));
      say(fmt::format("scroll_y:  {:.1f}", s.scroll_offset_y));
      say(fmt::format("widest:    {:.0f}px", cache.max_width()));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(composer_lab, "System Demos",
                        "text_area wrap, auto-grow and Enter vs Shift+Enter",
                        ComposerLab)
