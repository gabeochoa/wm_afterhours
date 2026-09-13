#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Repro & regression tests for nested clip container scissoring.
//
// Panel 1: CONTROL — normal rows in scroll view (no Overflow::Hidden)
// Panel 2: HasClipChildren rows inside scroll view (original bug)
// Panel 3: HasClipChildren wrapper around rows, inside scroll view (nested)
// Panel 4: Scroll view inside scroll view (nested scrolling)
//
// Every panel must clip at its scroll border. If any panel's
// rows paint outside that border, the fix is incomplete.

struct ScrollClipBugRepro : ScreenSystem<UIContext<InputAction>> {

  afterhours::Color bg{30, 30, 35, 255};
  afterhours::Color scroll_bg{40, 42, 50, 255};
  afterhours::Color row_blue{70, 130, 180, 255};
  afterhours::Color row_red{180, 70, 70, 255};
  afterhours::Color row_purple{130, 70, 180, 255};
  afterhours::Color row_green{70, 160, 100, 255};
  afterhours::Color white{255, 255, 255, 255};
  afterhours::Color muted{140, 140, 150, 255};
  afterhours::Color border_green{80, 200, 100, 255};
  afterhours::Color border_red{200, 80, 80, 255};
  afterhours::Color border_purple{160, 100, 220, 255};
  afterhours::Color border_teal{80, 200, 180, 255};

  static constexpr int NUM_ROWS = 20;
  bool original_fixture = false;
  bool nested_outer_target = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::midnight();

    const float sw = context.screen_width;
    const float sh = context.screen_height;
    const float scale = std::min(sw / 1280.f, sh / 720.f);
    const float left = (sw - 1280 * scale) / 2;
    const float top = (sh - 720 * scale) / 2;
    context.scaling_mode = ScalingMode::Proportional;
    div(context, mk(entity, 0), ComponentConfig{}.with_size({pixels(sw), pixels(sh)})
        .with_custom_background(bg).with_corner_radius(0).with_debug_name("bg"));
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(left + x * scale, top + y * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &value, float size,
                           const std::string &name = "") {
      return div(context, mk(entity, id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(white)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_render_layer(11).with_debug_name(name));
    };
    const auto action = [&](int id, float x, float y, float w, const std::string &value, const std::string &name) {
      return button(context, mk(entity, id), box(x, y, w, 38).with_label(value)
          .with_font("AtkinsonMock", pixels(20 * scale)).with_custom_text_color(white)
          .with_custom_background({61, 77, 102, 255}).with_corner_radius(6 * scale).with_debug_name(name));
    };
    int preset = -1;
    if (original_fixture) {
      div(context, mk(entity, 1), ComponentConfig{}.with_size({pixels(sw - 360), pixels(30)})
          .with_absolute_position(20, 8).with_label("Nested Clip Container Regression Tests")
          .with_font("AtkinsonMock", pixels(22)).with_custom_text_color(white));
      div(context, mk(entity, 2), ComponentConfig{}.with_size({pixels(sw - 40), pixels(20)})
          .with_absolute_position(20, 39).with_label("Original geometry: 40px rows / outer height = window height - 102px / inner = 1.5x outer")
          .with_font("AtkinsonMock", pixels(16)).with_custom_text_color({195, 205, 220, 255}));
    } else {
      label(1, 15, 20, 950, 43, "Nested Clip Container Regression Tests", 31);
      label(2, 15, 72, 1250, 29, "Colored guides mark the clip boundaries. Rows must never paint beyond them.", 22);
      if (action(3, 15, 111, 143, "Reset all", "clip_reset")) preset = 0;
      if (action(4, 173, 111, 240, "Compare at 120px", "clip_compare")) preset = 120;
      if (action(5, 428, 111, 281, nested_outer_target ? "Nested wheel: outer" : "Nested wheel: inner", "clip_nested_target"))
        nested_outer_target = !nested_outer_target;
      label(6, 15, 154, 1250, 44, "Wheel panel 4; switch Inner / Outer to choose its target.\nNested decoration clipping is pending: this view hides the inner bar and covers its leaked footer.", 18);
    }
    if (action(7, original_fixture ? (sw - 338 - left) / scale : 942.f,
               original_fixture ? (5 - top) / scale : 111.f, original_fixture ? 323 / scale : 323.f,
               original_fixture ? "Show annotated comparison" : "Show original fixture", "clip_fixture_mode")) {
      original_fixture = !original_fixture;
      preset = 0;
    }
    const float margin = original_fixture ? 15 : 15 * scale;
    const float col_w = original_fixture ? (sw - 75) / 4 : 301.25f * scale;
    const float label_y = original_fixture ? 62 : top + 206 * scale;
    const float scroll_y = original_fixture ? 82 : top + 260 * scale;
    const float scroll_h = original_fixture ? sh - 102 : std::min(390 * scale, 510.f);
    const float panel_left = original_fixture ? 0 : left;
    const float caption_size = original_fixture ? 16 : 21 * scale;

    // ── Panel 1: CONTROL ──
    float p1_x = panel_left + margin;
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("1: Control")
            .with_size(ComponentSize{pixels((int)col_w), pixels(original_fixture ? 19 : 29 * scale)})
            .with_absolute_position(p1_x, label_y)
            .with_font("AtkinsonMock", pixels(caption_size))
            .with_custom_text_color(white));

    auto scroll1 = vstack(
        context, mk(entity, 11),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_w), pixels(scroll_h)})
            .with_absolute_position(p1_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_green, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * scale)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_control"));

    for (int i = 0; i < NUM_ROWS; i++) {
      auto item = div(context, mk(scroll1.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("{:02d} / Ctrl {}", i + 1, i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_blue)
              .with_custom_text_color(white)
              .with_font("AtkinsonMock", pixels(original_fixture ? 19 : 21 * scale)).with_text_inset(0)
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(8.f * scale)
              .with_debug_name(fmt::format("ctrl_{}", i)));
      item.ent().get<HasLabel>().text_x_offset = 12;
    }

    // ── Panel 2: HasClipChildren rows in scroll view ──
    float p2_x = panel_left + margin * 2.0f + col_w;
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("2: ClipChild rows")
            .with_size(ComponentSize{pixels((int)col_w), pixels(original_fixture ? 19 : 29 * scale)})
            .with_absolute_position(p2_x, label_y)
            .with_font("AtkinsonMock", pixels(caption_size))
            .with_custom_text_color(white));

    auto scroll2 = vstack(
        context, mk(entity, 21),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_w), pixels(scroll_h)})
            .with_absolute_position(p2_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_red, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * scale)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_clip_rows"));

    for (int i = 0; i < NUM_ROWS; i++) {
      auto item = div(context, mk(scroll2.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("{:02d} / Clip {}", i + 1, i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_red)
              .with_custom_text_color(white)
              .with_font("AtkinsonMock", pixels(original_fixture ? 19 : 21 * scale)).with_text_inset(0)
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(8.f * scale)
              .with_overflow(Overflow::Hidden, Axis::X)
              .with_debug_name(fmt::format("clip_{}", i)));
      item.ent().get<HasLabel>().text_x_offset = 12;
    }

    // ── Panel 3: HasClipChildren wrapper around rows, in scroll view ──
    float p3_x = panel_left + margin * 3.0f + col_w * 2.0f;
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("3: Nested wrapper")
            .with_size(ComponentSize{pixels((int)col_w), pixels(original_fixture ? 19 : 29 * scale)})
            .with_absolute_position(p3_x, label_y)
            .with_font("AtkinsonMock", pixels(caption_size))
            .with_custom_text_color(white));

    auto scroll3 = vstack(
        context, mk(entity, 31),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_w), pixels(scroll_h)})
            .with_absolute_position(p3_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * scale)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_nested_wrapper"));

    // Wrapper div with Overflow::Hidden that contains all rows
    auto wrapper = vstack(
        context, mk(scroll3.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), children()})
            .with_overflow(Overflow::Hidden, Axis::X)
            .with_debug_name("clip_wrapper"));

    for (int i = 0; i < NUM_ROWS; i++) {
      auto item = div(context, mk(wrapper.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("{:02d} / Nest {}", i + 1, i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_purple)
              .with_custom_text_color(white)
              .with_font("AtkinsonMock", pixels(original_fixture ? 19 : 21 * scale)).with_text_inset(0)
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(8.f * scale)
              .with_debug_name(fmt::format("nest_{}", i)));
      item.ent().get<HasLabel>().text_x_offset = 12;
    }

    // ── Panel 4: Scroll view inside scroll view ──
    float p4_x = panel_left + margin * 4.0f + col_w * 3.0f;
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("4: Scroll in scroll")
            .with_size(ComponentSize{pixels((int)col_w), pixels(original_fixture ? 19 : 29 * scale)})
            .with_absolute_position(p4_x, label_y)
            .with_font("AtkinsonMock", pixels(caption_size))
            .with_custom_text_color(white));

    // Outer scroll view
    auto outer_scroll = vstack(
        context, mk(entity, 41),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_w), pixels(scroll_h)})
            .with_absolute_position(p4_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_teal, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * scale)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_outer"));

    // Inner scroll view (taller than outer, so outer scrolls it)
    auto inner_scroll = vstack(
        context, mk(outer_scroll.ent(), 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{percent(1.0f), pixels(scroll_h * 1.5f)})
            .with_custom_background(afterhours::Color{50, 55, 65, 255})
            .with_border(row_green, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_corner_radius(8.f * scale)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_inner"));

    for (int i = 0; i < NUM_ROWS; i++) {
      auto item = div(context, mk(inner_scroll.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("{:02d} / Inner {}", i + 1, i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_green)
              .with_custom_text_color(white)
              .with_font("AtkinsonMock", pixels(original_fixture ? 19 : 21 * scale)).with_text_inset(0)
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_corner_radius(8.f * scale)
              .with_debug_name(fmt::format("inner_{}", i)));
      item.ent().get<HasLabel>().text_x_offset = 12;
    }
    const std::array<afterhours::Entity *, 5> views{&scroll1.ent(), &scroll2.ent(), &scroll3.ent(), &outer_scroll.ent(), &inner_scroll.ent()};
    for (size_t i = 0; i < views.size(); ++i) {
      auto &state = views[i]->get<HasScrollView>();
      if (preset < 0) continue;
      state.scroll_offset = {0, static_cast<float>(i == 4 ? 0 : preset)};
      state.scroll_target = state.scroll_offset;
      state.last_eased_offset = state.scroll_offset;
      state.clamp_scroll();
    }
    outer_scroll.ent().get<HasScrollView>().vertical_enabled = original_fixture || nested_outer_target;
    auto &inner_state = inner_scroll.ent().get<HasScrollView>();
    inner_state.vertical_enabled = original_fixture || !nested_outer_target;
    inner_state.scrollbar_track_color = original_fixture ? std::nullopt : std::optional<afterhours::Color>{afterhours::Color{0, 0, 0, 0}};
    inner_state.scrollbar_thumb_color = original_fixture ? std::nullopt : std::optional<afterhours::Color>{afterhours::Color{0, 0, 0, 0}};
    if (original_fixture) return;
    const float footer_y = scroll_y + scroll_h + 2 * scale;
    div(context, mk(entity, 1000), ComponentConfig{}
        .with_size({pixels(sw), pixels(std::max(0.f, sh - footer_y))})
        .with_absolute_position(0, footer_y).with_custom_background(bg)
        .with_corner_radius(0).with_render_layer(10).with_ignore_pointer_events());
    const float boundary_y = (scroll_y + scroll_h - top) / scale;
    const std::array<const char *, 4> structures{"Normal rows / 20 rows", "Each row clips children / 20", "Scroll > clipped wrapper > 20 rows", "Outer > inner scroll > 20 rows"};
    const std::array<afterhours::Color, 4> colors{border_green, border_red, border_purple, border_teal};
    const std::array<const char *, 4> offset_names{"Control", "Row clip", "Wrapper", "Outer"};
    for (size_t i = 0; i < 4; ++i) {
      const float x = 15 + static_cast<float>(i) * 316.25f;
      div(context, mk(entity, 60 + static_cast<int>(i)), box(x, 202, 301.25f, 3).with_custom_background(colors[i]));
      label(70 + static_cast<int>(i), x, 236, 301.25f, 23, structures[i], 17);
      div(context, mk(entity, 80 + static_cast<int>(i)), box(x, boundary_y, 301.25f, 2).with_custom_background(colors[i]));
      const auto &state = views[i]->get<HasScrollView>();
      const auto offset = std::lround(state.scroll_offset.y);
      label(90 + static_cast<int>(i), x, boundary_y + 8, 301.25f, 25, fmt::format("{} offset: {}px", offset_names[i], offset), 20,
            "clip_offset_" + std::to_string(i));
      const auto inner_offset = std::lround(inner_scroll.ent().get<HasScrollView>().scroll_offset.y);
      label(100 + static_cast<int>(i), x, boundary_y + 38, 301.25f, 24,
            i == 3 ? fmt::format("Inner: {}px / height {:.0f}px", inner_offset, scroll_h * 1.5f)
                   : fmt::format("Viewport {:.0f}px / rows 40px", scroll_h), 17,
            "clip_extent_" + std::to_string(i));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_clip_bug, "Tools",
                        "Nested clip container regression tests",
                        ScrollClipBugRepro)
