#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// with_fit_content(): a box that hugs its own text and wraps at a cap. The
// chat bubble, basically, and every consumer that wanted one measured the
// string in app code because the four settings it needs are easy to get three
// of -- miss the font size and the width still caps, so it looks like it
// worked right up until a long message runs off on one line.
//
// The right column is the same messages at a smaller cap, so the wrap point
// moves and the bubbles are visibly sized by content rather than by a number
// someone picked.
struct FitContentLab : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const std::vector<std::pair<const char *, bool>> messages = {
        {"hi", false},
        {"you around?", true},
        {"yeah, one sec", false},
        {"the layout pass was measuring children before they snapped, so a "
         "row of eight buttons ended up fifteen pixels wider than the bar "
         "holding them",
         true},
        {"ah", false},
        {"anyway it is fixed", true},
    };

    auto root =
        div(context, mk(entity),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                .with_background(Theme::Usage::Background)
                .with_padding(Padding::all(w1280(16)))
                .with_debug_name("fc_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("fit_content: the box is as wide as its text, up to a cap")
            .with_size(ComponentSize{percent(1.f), h720(34)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("fc_title"));

    auto columns = hstack(context, mk(root.ent(), 1),
                          ComponentConfig{}
                              .with_size(ComponentSize{percent(1.f), children()})
                              .with_gap(w1280(24))
                              .with_margin(Margin{.top = h720(12)})
                              .with_debug_name("fc_columns"));

    // Two caps, same content: the bubbles are sized by what is in them.
    const struct {
      const char *name;
      const char *caption;
      float cap;
    } columns_spec[] = {
        {"fc_wide", "cap 380px", 380.f},
        {"fc_narrow", "cap 200px", 200.f},
    };

    int col_idx = 0;
    for (const auto &spec : columns_spec) {
      auto col = vstack(context, mk(columns.ent(), col_idx++),
                        ComponentConfig{}
                            .with_size(ComponentSize{expand(), children()})
                            .with_debug_name(spec.name));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(spec.caption)
              .with_size(ComponentSize{percent(1.f), h720(22)})
              .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
              .with_background(Theme::Usage::None)
              .with_debug_name(std::string(spec.name) + "_cap"));

      int msg_idx = 1;
      for (const auto &[text, is_me] : messages) {
        // A row, so the bubble can sit left or right without stretching: the
        // bubble hugs its text and the row takes the leftover width.
        auto row =
            hstack(context, mk(col.ent(), msg_idx),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.f), children()})
                       .with_justify_content(is_me ? JustifyContent::FlexEnd
                                                   : JustifyContent::FlexStart)
                       .with_background(Theme::Usage::None)
                       .with_margin(Margin{.bottom = h720(4)})
                       .with_debug_name(std::string(spec.name) + "_row_" +
                                        std::to_string(msg_idx)));

        div(context, mk(row.ent(), 0),
            ComponentConfig{}
                .with_label(text)
                .with_fit_content(w1280(spec.cap), h720(14.f))
                .with_custom_background(is_me
                                            ? afterhours::Color{54, 84, 138, 255}
                                            : afterhours::Color{44, 48, 62, 255})
                .with_alignment(TextAlignment::Left)
                .with_debug_name(std::string(spec.name) + "_msg_" +
                                 std::to_string(msg_idx)));
        msg_idx++;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(fit_content_lab, "System Demos",
                        "with_fit_content(): boxes that hug their own text",
                        FitContentLab)
