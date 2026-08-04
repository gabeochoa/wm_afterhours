#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Every way a label can end up on more than one line, side by side, so the
// screenshot shows whether they agree with each other.
//
// Each block draws a thin box at its laid-out size with the text inside it.
// That pairing is the point: layout and rendering compute the line count
// separately, and when they disagree the text spills past its own outline --
// visible here, invisible in a test that only checks one of the two.
struct MultiLineTextLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr float FS = 18.f;

  // Drag-to-resize panel, bottom of the screen. Wrapping only re-flows when
  // the width actually changes, so a box you can drag is the quickest way to
  // see whether the line count, the box height and the painted text stay in
  // agreement at every width -- not just the handful a fixed screen picks.
  float box_w = 300.f;
  float box_h = 120.f;
  static constexpr float BOX_X = 48.f;
  static constexpr float BOX_Y = 520.f;
  static constexpr float HANDLE = 28.f;

  // Previous frame's handle id. hot/active are resolved after the screen is
  // built, so this frame's colour has to come from last frame's state -- the
  // same reason was_hot() exists.
  afterhours::EntityID handle_id = -1;
  int drag_count = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{255, 255, 255, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto box = afterhours::Color{60, 70, 100, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(600), pixels(36)})
            .with_absolute_position(48.f, 24.f)
            .with_label("Multi-line Text Lab")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("ml_title"));

    int id = 1;
    auto caption = [&](float x, float y, const std::string &text,
                       float w = 300.f) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              // 28, not 22: the renderer insets 5px top and bottom, so a
              // 22px box leaves 12px for a 13px font and reports overflow.
              .with_size(ComponentSize{pixels(w), pixels(28)})
              .with_absolute_position(x, y)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_debug_name(fmt::format("ml_caption_{}", id)));
    };

    // --- Hard '\n', no wrap, no pinned size: the plainest multi-line label.
    caption(48.f, 76.f, "hard \\n, auto-fit size");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(96)})
            .with_absolute_position(48.f, 100.f)
            .with_label("first line\nsecond line\nthird line")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_hard_autofit"));

    // --- Same text, pinned font size: lines are 18px rather than fitted.
    caption(360.f, 76.f, "hard \\n, font_size 18");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(96)})
            .with_absolute_position(360.f, 100.f)
            .with_label("first line\nsecond line\nthird line")
            .with_alignment(TextAlignment::Left)
            .with_font_size(FS)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_hard_pinned"));

    // --- Soft wrap at a fixed width. Needs the pinned size to wrap at all.
    caption(48.f, 212.f, "wrap, fixed 280x110 box");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(110)})
            .with_absolute_position(48.f, 236.f)
            .with_label("the quick brown fox jumps over the lazy dog and then "
                        "keeps on running well past the edge")
            .with_text_overflow(TextOverflow::Wrap)
            .with_font_size(FS)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_wrap_fixed"));

    // --- The one that used to overflow: the box height comes from Dim::Text,
    // so it has to be as tall as however many lines the text wraps to.
    caption(360.f, 212.f, "wrap, height = Dim::Text (self-sizing)");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), Size{Dim::Text, 0.f, 1.f}})
            .with_absolute_position(360.f, 236.f)
            .with_label("the quick brown fox jumps over the lazy dog and then "
                        "keeps on running well past the edge")
            .with_text_overflow(TextOverflow::Wrap)
            .with_font_size(FS)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_wrap_selfsizing"));

    // --- Multi-colour runs across a hard break, the diff-viewer shape.
    caption(48.f, 372.f, "styled runs across a hard \\n");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(96)})
            .with_absolute_position(48.f, 396.f)
            .with_styled_label({
                TextSpan{"- removed line\n", afterhours::Color{230, 90, 90, 255}},
                TextSpan{"+ added line\n", afterhours::Color{90, 210, 120, 255}},
                TextSpan{"  context line", muted},
            })
            .with_alignment(TextAlignment::Left)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_styled_break"));

    // --- Blank line from '\n\n' has to keep its height.
    caption(360.f, 372.f, "blank line from \\n\\n is preserved");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(96)})
            .with_absolute_position(360.f, 396.f)
            .with_label("above the gap\n\nbelow the gap")
            .with_font_size(FS)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_blank_line"));

    // --- Drag the corner handle to resize; the text re-wraps live.
    caption(BOX_X, BOX_Y - 24.f,
            fmt::format("drag the corner handle  ({:.0f} x {:.0f})", box_w,
                        box_h));

    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_w), pixels(box_h)})
            .with_absolute_position(BOX_X, BOX_Y)
            .with_label("the quick brown fox jumps over the lazy dog and then "
                        "keeps on running well past the edge of the box")
            .with_text_overflow(TextOverflow::Wrap)
            .with_font_size(FS)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(box)
            .with_debug_name("ml_resizable"));

    // The handle only has to start the drag. HandleDrags keeps firing while
    // this entity is active, so the callback still runs once the cursor has
    // left the handle -- which is the whole point, since resizing moves the
    // corner out from under it.
    // Lights up when the hit test finds it. If dragging ever stops working,
    // this says whether the cursor is landing on the handle at all -- which
    // splits "hit test misses" from "the drag callback is wrong".
    const bool hot = handle_id >= 0 && context.was_hot(handle_id);
    const bool held = handle_id >= 0 && context.was_active(handle_id);
    const auto handle_col = held ? afterhours::Color{255, 230, 120, 255}
                            : hot ? afterhours::Color{190, 235, 255, 255}
                                  : afterhours::Color{120, 200, 255, 255};

    auto handle = div(context, mk(entity, id++),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(HANDLE),
                                                   pixels(HANDLE)})
                          .with_absolute_position(BOX_X + box_w - HANDLE,
                                                  BOX_Y + box_h - HANDLE)
                          .with_color_usage(Theme::Usage::Custom)
                          .with_custom_color(handle_col)
                          .with_debug_name("ml_resize_handle"));
    handle_id = handle.ent().id;

    handle.ent().addComponentIfMissing<afterhours::ui::HasDragListener>(
        [this](afterhours::Entity &) {
          const auto m = afterhours::input::get_mouse_position();
          // Track the corner, not the handle's own rect: the pointer is the
          // thing being dragged and the rect is a frame behind it.
          box_w = std::clamp(m.x - BOX_X, 80.f, 700.f);
          box_h = std::clamp(m.y - BOX_Y, 40.f, 160.f);
          drag_count++;
        });

    // Where the UI thinks the pointer is, versus where the handle is. If these
    // disagree the problem is coordinate mapping, not the drag wiring.
    const auto mp = afterhours::input::get_mouse_position();
    caption(BOX_X, BOX_Y + 176.f,
            fmt::format("mouse ({:.0f},{:.0f})   handle ({:.0f},{:.0f} {}x{})"
                        "   hot={} held={} drags={}",
                        mp.x, mp.y, BOX_X + box_w - HANDLE,
                        BOX_Y + box_h - HANDLE, (int)HANDLE, (int)HANDLE,
                        hot ? "Y" : "n", held ? "Y" : "n", drag_count),
            860.f);

    // Same text at the same dragged width, but the height is Dim::Text, so
    // layout has to arrive at the line count the renderer will paint. The box
    // above is told its height; this one has to work it out, and any
    // disagreement shows as text outside the outline at some width you drag
    // through. That is the case a fixed screen cannot cover.
    caption(BOX_X + 720.f, BOX_Y - 24.f, "same width, height = Dim::Text");
    div(context, mk(entity, id++),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_w), Size{Dim::Text, 0.f, 1.f}})
            .with_absolute_position(BOX_X + 720.f, BOX_Y)
            .with_label("the quick brown fox jumps over the lazy dog and then "
                        "keeps on running well past the edge of the box")
            .with_text_overflow(TextOverflow::Wrap)
            .with_font_size(FS)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_color_usage(Theme::Usage::Custom)
            .with_custom_color(afterhours::Color{70, 100, 80, 255})
            .with_debug_name("ml_resizable_selfsized"));
  }
};

REGISTER_EXAMPLE_SCREEN(multiline_text_lab, "System Demos",
                        "Hard breaks, wrapping, and self-sizing text blocks",
                        MultiLineTextLab)
