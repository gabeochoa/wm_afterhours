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
  bool show_guides = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float offset_x = (context.screen_width - 1280 * s) / 2;
    const float offset_y = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 241, 250, 255};
    const auto muted = afterhours::Color{172, 187, 210, 255};
    const auto incoming = afterhours::Color{47, 57, 76, 255};
    const auto outgoing = afterhours::Color{45, 79, 129, 255};
    const auto at = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(offset_x + x * s, offset_y + y * s)
          .with_corner_radius(0);
    };
    const auto label = [&](int id, const std::string &value, float x, float y,
                           float w, float h, float size, afterhours::Color color,
                           const std::string &name) {
      div(context, mk(entity, id), at(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    const std::vector<std::pair<const char *, bool>> messages = {
        {"hi", false},
        {"you around?", true},
        {"yeah, one sec", false},
        {"the layout pass was measuring children before they snapped, so a "
         "row of eight buttons ended up fifteen pixels wider than the bar "
         "holding them", true},
        {"ah", false},
        {"anyway it is fixed", true},
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({17, 23, 34, 255}).with_corner_radius(0)
        .with_debug_name("fc_root"));
    label(1, "Fit-content bubbles", 48, 20, 916, 44, 34, white, "fc_title");
    label(2, "Same text, different maximum width.", 48, 70, 1184, 28, 21, muted, "fc_subtitle");
    if (button(context, mk(entity, 3), at(984, 26, 248, 42)
        .with_label(show_guides ? "Hide measurement guides" : "Show measurement guides")
        .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(white)
        .with_custom_background({43, 70, 108, 255}).with_corner_radius(8 * s)
        .with_debug_name("fc_guides"))) show_guides = !show_guides;
    div(context, mk(entity, 4), at(244, 111, 18, 18).with_custom_background(incoming)
        .with_corner_radius(5 * s));
    label(5, "Incoming / left", 270, 105, 210, 30, 18, white, "fc_incoming_legend");
    div(context, mk(entity, 6), at(486, 111, 18, 18).with_custom_background(outgoing)
        .with_corner_radius(5 * s));
    label(7, "You / right", 512, 105, 187, 30, 18, white, "fc_outgoing_legend");
    label(8, "4 = sample layout story", 742, 105, 290, 30, 18, muted, "fc_sample_note");
    div(context, mk(entity, 9), at(240, 144, 800, 1)
        .with_custom_background({58, 75, 99, 255}));

    // Two caps, same content: the bubbles are sized by what is in them.
    const struct {
      const char *name;
      float x;
      float width;
      float cap;
    } columns_spec[] = {{"fc_wide", 240, 464, 380},
                        {"fc_narrow", 736, 304, 200}};
    int index = 0;
    for (const auto &spec : columns_spec) {
      const int id = 100 + index++ * 100;
      div(context, mk(entity, id), at(spec.x, 156, spec.width, 480)
          .with_custom_background({25, 34, 49, 255}).with_corner_radius(12 * s)
          .with_debug_name(std::string(spec.name) + "_frame"));
      label(id + 1, fmt::format("Maximum {:.0f} px", spec.cap * s), spec.x + 20,
            171, spec.width - 40, 31, 22, white, std::string(spec.name) + "_cap");
      if (show_guides) {
        div(context, mk(entity, id + 2), at(spec.x + 48, 211, spec.cap, 8)
            .with_background(Theme::Usage::None).with_ignore_pointer_events()
            .with_on_draw_fg([=](RectangleType r) {
              const raylib::Color color{135, 183, 239, 255};
              raylib::DrawLineEx({r.x, r.y + r.height / 2},
                                 {r.x + r.width, r.y + r.height / 2}, s, color);
              raylib::DrawLineEx({r.x, r.y}, {r.x, r.y + r.height}, s, color);
              raylib::DrawLineEx({r.x + r.width, r.y},
                                 {r.x + r.width, r.y + r.height}, s, color);
            }).with_debug_name(std::string(spec.name) + "_width_guide"));
      }
      auto column = vstack(context, mk(entity, id + 3),
          at(spec.x + 20, 230, spec.cap + 28, 0)
              .with_size({pixels((spec.cap + 28) * s), children()})
              .with_gap(pixels(10 * s)).with_background(Theme::Usage::None)
              .with_debug_name(spec.name));
      int message_index = 0;
      for (const auto &[message, is_me] : messages) {
        ++message_index;
        auto row = hstack(context, mk(column.ent(), message_index),
            ComponentConfig{}.with_size({percent(1), children()})
                .with_gap(pixels(8 * s)).with_background(Theme::Usage::None)
                .with_debug_name(std::string(spec.name) + "_row_" + std::to_string(message_index)));
        div(context, mk(row.ent(), 1), ComponentConfig{}
            .with_size({pixels(20 * s), pixels(22 * s)})
            .with_margin(Margin{.top = pixels(6 * s)})
            .with_label(std::to_string(message_index)).with_font("AtkinsonMock", pixels(16 * s))
            .with_custom_text_color(muted).with_background(Theme::Usage::None));
        // A row, so the bubble can sit left or right without stretching: the
        // bubble hugs its text and the row takes the leftover width.
        auto message_row = hstack(context, mk(row.ent(), 2), ComponentConfig{}
            .with_size({pixels(spec.cap * s), children()})
            .with_justify_content(is_me ? JustifyContent::FlexEnd : JustifyContent::FlexStart)
            .with_background(Theme::Usage::None));
        auto bubble = vstack(context, mk(message_row.ent(), 0), ComponentConfig{}
            .with_size({children(), children()})
            .with_max_width(pixels(spec.cap * s))
            .with_padding(Padding{.top = pixels(8 * s), .left = pixels(12 * s),
                                  .bottom = pixels(8 * s), .right = pixels(12 * s)})
            .with_custom_background(is_me ? outgoing : incoming)
            .with_corner_radius(10 * s)
            .with_debug_name(std::string(spec.name) + "_msg_" + std::to_string(message_index)));
        auto content = div(context, mk(bubble.ent(), 0), ComponentConfig{}
            .with_label(message).with_fit_content(pixels((spec.cap - 24) * s), pixels(18 * s))
            .with_font("AtkinsonMock", pixels(18 * s)).with_text_inset(0)
            .with_custom_text_color(white).with_alignment(TextAlignment::Left)
            .with_background(Theme::Usage::None)
            .with_debug_name(std::string(spec.name) + "_text_" + std::to_string(message_index)));
        if (message_index != 4) continue;
        const auto bounds = bubble.cmp().rect();
        const auto text_bounds = content.cmp().rect();
        auto *cache = afterhours::EntityHelper::get_singleton_cmp<TextMeasureCache>();
        size_t lines = 0;
        if (cache && text_bounds.width > 0) {
          lines = afterhours::ui::detail::wrap_text_to_width(
              message, text_bounds.width, [&](const std::string &line) {
                return cache->measure(line, "AtkinsonMock", 18 * s, 1).x;
              }).size();
        }
        label(id + 4, fmt::format("Message 4 / {} lines / {:.0f} px high", lines, bounds.height),
              spec.x, 644, spec.width, 29, 18, muted, std::string(spec.name) + "_metrics");
        if (!show_guides || index != 2 || bounds.height <= 0) continue;
        div(context, mk(entity, id + 5), ComponentConfig{}
            .with_size({pixels(8 * s), pixels(bounds.height)})
            .with_absolute_position(bounds.x + bounds.width + 12 * s, bounds.y)
            .with_background(Theme::Usage::None).with_ignore_pointer_events()
            .with_on_draw_fg([=](RectangleType r) {
              const raylib::Color color{135, 183, 239, 255};
              raylib::DrawLineEx({r.x + r.width, r.y}, {r.x + r.width, r.y + r.height}, s, color);
              raylib::DrawLineEx({r.x, r.y}, {r.x + r.width, r.y}, s, color);
              raylib::DrawLineEx({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, s, color);
            }).with_debug_name("fc_narrow_height_guide"));
      }
    }
    label(10, "with_fit_content(max_width, font_size) / short text sets the width; long text wraps at the cap.",
          48, 686, 1184, 27, 18, white, "fc_sizing_rule");
  }
};

REGISTER_EXAMPLE_SCREEN(fit_content_lab, "System Demos",
                        "with_fit_content(): boxes that hug their own text",
                        FitContentLab)
