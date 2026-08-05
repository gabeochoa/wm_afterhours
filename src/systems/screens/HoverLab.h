#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Subtree hover and ignoring pointer events (D25). Hover each row, then its star:
// row 1 loses its wash to the star, rows 2 and 3 fix that two different ways.
struct HoverLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr float ROW_W = 420.f;
  static constexpr float ROW_H = 48.f;
  static constexpr float STAR = 36.f;

  // Hover is queried while building, before this frame's hot_id resolves, so
  // the colour comes from last frame -- same reason was_hot() exists.
  afterhours::EntityID row_id[3] = {-1, -1, -1};
  afterhours::EntityID star_id[3] = {-1, -1, -1};
  int star_clicks[3] = {0, 0, 0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto base = afterhours::Color{48, 56, 78, 255};
    const auto wash = afterhours::Color{88, 108, 155, 255};
    const auto star_col = afterhours::Color{240, 200, 110, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(760), pixels(36)})
            .with_absolute_position(48.f, 24.f)
            .with_label("Hover Lab - subtree hover and ignoring pointer events")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("hv_title"));

    int id = 1;
    auto caption = [&](float y, const std::string &text) {
      div(context, mk(entity, id++),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(760), pixels(26)})
              .with_absolute_position(48.f, y)
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_debug_name(fmt::format("hv_cap_{}", id)));
    };

    // 0 = is_hot (the bug), 1 = mouse_in_subtree, 2 = star ignores pointer events
    auto build_row = [&](int n, float y, bool lit) {
      auto row = div(context, mk(entity, 100 + n),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(ROW_W), pixels(ROW_H)})
                         .with_absolute_position(48.f, y)
                         .with_label(fmt::format("  thread row {}", n + 1))
                         .with_alignment(TextAlignment::Left)
                         .with_font_size(18.f)
                         .with_custom_text_color(white)
                         .with_color_usage(Theme::Usage::Custom)
                         .with_custom_color(lit ? wash : base)
                         .disable_rounded_corners()
                         .with_debug_name(fmt::format("hv_row_{}", n)));
      // A click listener is what makes something hit-testable.
      row.ent().addComponentIfMissing<afterhours::ui::HasClickListener>(
          [](afterhours::Entity &) {});
      row_id[n] = row.ent().id;

      auto star = div(
          context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(STAR), pixels(STAR)})
              .with_absolute_position(ROW_W - STAR - 6.f, 6.f)
              .with_label("*")
              .with_alignment(TextAlignment::Center)
              .with_font_size(20.f)
              .with_custom_text_color(star_col)
              .with_color_usage(Theme::Usage::Custom)
              .with_custom_color(afterhours::Color{70, 82, 112, 255})
              // Row 3: star stops taking the hit, so the row keeps it.
              .with_ignore_pointer_events(n == 2)
              .disable_rounded_corners()
              .with_debug_name(fmt::format("hv_star_{}", n)));
      star.ent().addComponentIfMissing<afterhours::ui::HasClickListener>(
          [this, n](afterhours::Entity &) { star_clicks[n]++; });
      star_id[n] = star.ent().id;
    };

    const auto lit_is_hot =
        row_id[0] >= 0 && context.was_hot(row_id[0]);
    const auto lit_subtree =
        row_id[1] >= 0 && context.mouse_was_in_subtree(row_id[1]);
    const auto lit_ignored =
        row_id[2] >= 0 && context.was_hot(row_id[2]);

    caption(72.f, "1. is_hot(row) - the star steals hotness, wash drops out");
    build_row(0, 98.f, lit_is_hot);

    caption(160.f,
            "2. mouse_was_in_subtree(row) - stays lit across the whole row, "
            "star still clickable");
    build_row(1, 186.f, lit_subtree);

    caption(248.f,
            "3. star has with_ignore_pointer_events - row keeps the hit, star no "
            "longer clickable");
    build_row(2, 274.f, lit_ignored);

    // What each row believes vs what the hit test chose.
    const auto say = [&](int n, const char *how, bool lit) {
      div(context, mk(entity, 200 + n),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(700), pixels(24)})
              .with_absolute_position(48.f, 350.f + (float)n * 26.f)
              .with_label(fmt::format(
                  "row {}  {:<28} lit={}  star_hot={}  star_clicks={}", n + 1,
                  how, lit ? "Y" : "n",
                  star_id[n] >= 0 && context.was_hot(star_id[n]) ? "Y" : "n",
                  star_clicks[n]))
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_debug_name(fmt::format("hv_say_{}", n)));
    };
    say(0, "is_hot(row)", lit_is_hot);
    say(1, "mouse_was_in_subtree(row)", lit_subtree);
    say(2, "is_hot(row) + ignore_pointer", lit_ignored);

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(760), pixels(24)})
            .with_absolute_position(48.f, 440.f)
            .with_label("click each star: rows 1 and 2 count, row 3 does not")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("hv_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(hover_lab, "System Demos",
                        "Subtree hover and ignoring pointer events",
                        HoverLab)
