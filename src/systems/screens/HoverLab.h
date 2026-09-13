#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

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

  static void draw_star(RectangleType rect, raylib::Color color) {
    const raylib::Vector2 center{rect.x + rect.width / 2, rect.y + rect.height / 2};
    std::array<raylib::Vector2, 10> points;
    for (size_t i = 0; i < points.size(); ++i) {
      const float angle = -3.14159265f / 2 + static_cast<float>(i) * 3.14159265f / 5;
      const float radius = std::min(rect.width, rect.height) * (i % 2 == 0 ? .4f : .18f);
      points[i] = {center.x + std::cos(angle) * radius,
                   center.y + std::sin(angle) * radius};
    }
    for (size_t i = 0; i < points.size(); ++i) {
      raylib::DrawTriangle(center, points[(i + 1) % points.size()], points[i], color);
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float ox = (context.screen_width - 1280 * s) / 2;
    const float oy = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{167, 182, 206, 255};
    const auto base = afterhours::Color{48, 56, 78, 255};
    const auto wash = afterhours::Color{88, 108, 155, 255};
    const auto at = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(ox + x * s, oy + y * s).with_corner_radius(0);
    };
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &name) {
      div(context, mk(entity, id), at(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({16, 22, 33, 255}).with_corner_radius(0)
        .with_debug_name("hv_canvas"));
    text(1, "Hover Lab", 48, 20, 946, 44, 34, white, "hv_title");
    text(2, "Compare direct hover, subtree hover and pointer passthrough.",
         48, 68, 1184, 28, 21, muted, "hv_summary");
    text(3, "Move across each row and star. Click the stars: 1 and 2 count; 3 passes pointer clicks to its row.",
         48, 99, 1184, 25, 18, muted, "hv_hint");
    if (button(context, mk(entity, 4), at(1032, 26, 200, 40)
        .with_label("Reset click counts").with_font("AtkinsonMock", pixels(19 * s))
        .with_custom_background({47, 68, 97, 255}).with_custom_text_color(white)
        .with_corner_radius(8 * s).with_debug_name("hv_reset"))) {
      std::fill(std::begin(star_clicks), std::end(star_clicks), 0);
    }
    const std::array<std::string, 3> names{"Direct hover", "Subtree hover", "Pointer passthrough"};
    const std::array<std::string, 3> explanations{
        "Entering the star removes the row highlight.",
        "Row stays highlighted; the star still receives clicks.",
        "The star ignores the pointer. The row receives the hit."};
    const std::array<std::string, 3> api{
        "was_hot(row)", "mouse_was_in_subtree(row)", "was_hot(row) + ignore_pointer_events"};
    const std::array<bool, 3> lit{
        row_id[0] >= 0 && context.was_hot(row_id[0]),
        row_id[1] >= 0 && context.mouse_was_in_subtree(row_id[1]),
        row_id[2] >= 0 && context.was_hot(row_id[2])};
    for (int n = 0; n < 3; ++n) {
      const float y = 128 + static_cast<float>(n) * 194;
      const bool star_hot = star_id[n] >= 0 && context.was_hot(star_id[n]);
      const int id = 100 + n * 30;
      div(context, mk(entity, id), at(48, y, 648, 184)
          .with_custom_background({25, 34, 50, 255}).with_corner_radius(12 * s)
          .with_debug_name(fmt::format("hv_case_{}", n)));
      text(id + 1, fmt::format("{}. {}", n + 1, names[n]), 68, y + 10, 608, 29,
           24, white, fmt::format("hv_case_title_{}", n));
      text(id + 2, explanations[n], 68, y + 42, 608, 24, 18, muted,
           fmt::format("hv_cap_{}", n));
      auto row = div(context, mk(entity, id + 3), at(68, y + 70, ROW_W, ROW_H)
          .with_label(names[n]).with_font("AtkinsonMock", pixels(21 * s))
          .with_custom_text_color(white).with_text_inset(12 * s)
          .with_custom_background(lit[n] ? wash : base)
          .with_debug_name(fmt::format("hv_row_{}", n)));
      // A click listener is what makes something hit-testable.
      row.ent().addComponentIfMissing<afterhours::ui::HasClickListener>(
          [](afterhours::Entity &) {});
      row_id[n] = row.ent().id;
      auto star = div(context, mk(row.ent(), 0), ComponentConfig{}
          .with_size({pixels(STAR * s), pixels(STAR * s)})
          .with_absolute_position((ROW_W - STAR - 6) * s, 6 * s)
          .with_custom_background(star_hot ? afterhours::Color{110, 125, 162, 255}
                                           : afterhours::Color{66, 78, 109, 255})
          // Row 3: star stops taking the hit, so the row keeps it.
          .with_ignore_pointer_events(n == 2).with_corner_radius(0)
          .with_on_draw_fg([](RectangleType r) { draw_star(r, {245, 210, 124, 255}); })
          .with_debug_name(fmt::format("hv_star_{}", n)));
      star.ent().addComponentIfMissing<afterhours::ui::HasClickListener>(
          [this, n](afterhours::Entity &) { star_clicks[n]++; });
      star_id[n] = star.ent().id;
      text(id + 4, n == 2 ? "Pointer\npassthrough" : "Interactive\nstar", 508, y + 70,
           168, 48, 18, n == 2 ? afterhours::Color{182, 212, 216, 255} : muted,
           fmt::format("hv_star_kind_{}", n));
      // What each row believes vs what the hit test chose.
      text(id + 5, "Case / live state", 68, y + 123, 192, 20, 15, muted, "");
      text(id + 6, "Row lit", 278, y + 123, 96, 20, 15, muted, "");
      text(id + 7, "Star hot", 400, y + 123, 96, 20, 15, muted, "");
      text(id + 8, "Star clicks", 534, y + 123, 134, 20, 15, muted, "");
      text(id + 9, fmt::format("{} / pointer state", n + 1), 68, y + 145,
           192, 25, 18, white, fmt::format("hv_state_case_{}", n));
      const auto badge = [&](int child, float x, const std::string &value,
                             bool active, const std::string &name) {
        div(context, mk(entity, child), at(x, y + 145, 66, 25)
            .with_label(value).with_font("AtkinsonMock", pixels(19 * s))
            .with_alignment(TextAlignment::Center).with_custom_text_color(white)
            .with_custom_background(active ? afterhours::Color{46, 96, 108, 255}
                                            : afterhours::Color{43, 53, 72, 255})
            .with_corner_radius(5 * s).with_ignore_pointer_events().with_debug_name(name));
      };
      badge(id + 10, 278, lit[n] ? "Yes" : "No", lit[n], fmt::format("hv_lit_{}", n));
      badge(id + 11, 400, star_hot ? "Yes" : "No", star_hot, fmt::format("hv_hot_{}", n));
      badge(id + 12, 534, std::to_string(star_clicks[n]), star_clicks[n] > 0,
            fmt::format("hv_clicks_{}", n));
    }
    div(context, mk(entity, 300), at(720, 128, 512, 414)
        .with_custom_background({25, 34, 50, 255}).with_corner_radius(12 * s)
        .with_debug_name("hv_reference"));
    text(301, "Pointer over the star", 740, 144, 472, 31, 25, white, "hv_expected_title");
    text(302, "Expected states / reference diagrams, not controls", 740, 181,
         472, 24, 17, muted, "hv_expected_note");
    text(303, "Case", 740, 218, 184, 22, 16, muted, "");
    text(304, "Row lit", 952, 218, 76, 22, 16, muted, "");
    text(305, "Star hot", 1040, 218, 76, 22, 16, muted, "");
    text(306, "Clicks", 1128, 218, 84, 22, 15, muted, "");
    for (int n = 0; n < 3; ++n) {
      const int id = 320 + n * 10;
      const float y = 252 + static_cast<float>(n) * 92;
      text(id, fmt::format("{}. {}", n + 1, names[n]), 740, y, 202, 23,
           18, white, "");
      div(context, mk(entity, id + 1), at(740, y + 30, 184, 36)
          .with_custom_background(n == 0 ? base : wash).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            const float unit = r.height / 36;
            const RectangleType child{r.x + r.width - 34 * unit, r.y + 4 * unit, 28 * unit, 28 * unit};
            raylib::DrawRectangleRec(child, n == 2 ? raylib::Color{66, 78, 109, 255}
                                                   : raylib::Color{110, 125, 162, 255});
            draw_star(child, {245, 210, 124, 255});
            const raylib::Vector2 tip{child.x + 14 * unit, child.y + 14 * unit};
            raylib::DrawTriangle(tip, {tip.x + 3 * unit, tip.y + 17 * unit},
                                 {tip.x + 12 * unit, tip.y + 12 * unit}, {248, 249, 253, 255});
          }).with_debug_name(fmt::format("hv_expected_{}", n)));
      text(id + 2, n == 0 ? "No" : "Yes", 952, y + 30, 76, 30, 21, white, "");
      text(id + 3, n == 2 ? "No" : "Yes", 1040, y + 30, 76, 30, 21, white, "");
      text(id + 4, n == 2 ? "No" : "Yes", 1128, y + 30, 84, 30, 21, white, "");
    }
    div(context, mk(entity, 370), at(720, 562, 512, 138)
        .with_custom_background({25, 34, 50, 255}).with_corner_radius(12 * s)
        .with_debug_name("hv_api_panel"));
    text(371, "Hover queries used by these rows", 740, 572, 472, 27,
         22, white, "hv_api_heading");
    for (int n = 0; n < 3; ++n) {
      text(372 + n, fmt::format("{}. {}", n + 1, api[n]), 740,
           604 + static_cast<float>(n) * 28, 472, 24, 17, muted,
           fmt::format("hv_api_{}", n));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(hover_lab, "System Demos",
                        "Subtree hover and ignoring pointer events",
                        HoverLab)
