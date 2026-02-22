#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Repro & regression tests for nested clip container scissoring.
//
// Panel 1: CONTROL — normal rows in scroll view (no Overflow::Hidden)
// Panel 2: HasClipChildren rows inside scroll view (original bug)
// Panel 3: HasClipChildren wrapper around rows, inside scroll view (nested)
// Panel 4: Scroll view inside scroll view (nested scrolling)
//
// All panels should clip identically when scrolled. If any panel's
// rows paint outside the scroll border, the fix is incomplete.

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

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::midnight();

    int sw = Settings::get().get_screen_width();
    int sh = Settings::get().get_screen_height();

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(sw), pixels(sh)})
            .with_custom_background(bg)
            .with_debug_name("bg"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Nested Clip Container Regression Tests")
            .with_size(ComponentSize{pixels(sw - 40), pixels(26)})
            .with_absolute_position(20.0f, 10.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(white));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("All 4 panels should clip identically when scrolled. "
                        "Rows must not paint outside the colored borders.")
            .with_size(ComponentSize{pixels(sw - 40), pixels(18)})
            .with_absolute_position(20.0f, 38.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted));

    float margin = 15.0f;
    float col_w = ((float)sw - margin * 5.0f) / 4.0f;
    float label_y = 62.0f;
    float scroll_y = 82.0f;
    float scroll_h = (float)sh - scroll_y - 20.0f;

    // ── Panel 1: CONTROL ──
    float p1_x = margin;
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("1: Control")
            .with_size(ComponentSize{pixels((int)col_w), pixels(16)})
            .with_absolute_position(p1_x, label_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(row_blue));

    auto scroll1 = vstack(
        context, mk(entity, 11),
        ComponentConfig{}
            .with_720p_size(col_w, scroll_h)
            .with_absolute_position(p1_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_green, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.04f)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_control"));

    for (int i = 0; i < NUM_ROWS; i++) {
      div(context, mk(scroll1.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("Ctrl {}", i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_blue)
              .with_custom_text_color(white)
              .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("ctrl_{}", i)));
    }

    // ── Panel 2: HasClipChildren rows in scroll view ──
    float p2_x = margin * 2.0f + col_w;
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("2: ClipChild rows")
            .with_size(ComponentSize{pixels((int)col_w), pixels(16)})
            .with_absolute_position(p2_x, label_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(row_red));

    auto scroll2 = vstack(
        context, mk(entity, 21),
        ComponentConfig{}
            .with_720p_size(col_w, scroll_h)
            .with_absolute_position(p2_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_red, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.04f)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_clip_rows"));

    for (int i = 0; i < NUM_ROWS; i++) {
      div(context, mk(scroll2.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("Clip {}", i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_red)
              .with_custom_text_color(white)
              .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_overflow(Overflow::Hidden, Axis::X)
              .with_debug_name(fmt::format("clip_{}", i)));
    }

    // ── Panel 3: HasClipChildren wrapper around rows, in scroll view ──
    float p3_x = margin * 3.0f + col_w * 2.0f;
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("3: Nested wrapper")
            .with_size(ComponentSize{pixels((int)col_w), pixels(16)})
            .with_absolute_position(p3_x, label_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(row_purple));

    auto scroll3 = vstack(
        context, mk(entity, 31),
        ComponentConfig{}
            .with_720p_size(col_w, scroll_h)
            .with_absolute_position(p3_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.04f)
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
      div(context, mk(wrapper.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("Nest {}", i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_purple)
              .with_custom_text_color(white)
              .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("nest_{}", i)));
    }

    // ── Panel 4: Scroll view inside scroll view ──
    float p4_x = margin * 4.0f + col_w * 3.0f;
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("4: Scroll in scroll")
            .with_size(ComponentSize{pixels((int)col_w), pixels(16)})
            .with_absolute_position(p4_x, label_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(row_green));

    // Outer scroll view (shorter to make nesting obvious)
    auto outer_scroll = vstack(
        context, mk(entity, 41),
        ComponentConfig{}
            .with_720p_size(col_w, scroll_h)
            .with_absolute_position(p4_x, scroll_y)
            .with_custom_background(scroll_bg)
            .with_border(border_teal, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.04f)
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
            .with_roundness(0.04f)
            .with_overflow(Overflow::Scroll, Axis::Y)
            .with_padding(Spacing::xs)
            .with_debug_name("scroll_inner"));

    for (int i = 0; i < NUM_ROWS; i++) {
      div(context, mk(inner_scroll.ent(), i),
          ComponentConfig{}
              .with_label(fmt::format("Inner {}", i + 1))
              .with_size(ComponentSize{percent(0.95f), pixels(40)})
              .with_custom_background(row_green)
              .with_custom_text_color(white)
              .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
              .with_margin(Margin{.bottom = pixels(3)})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("inner_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_clip_bug, "Tools",
                        "Nested clip container regression tests",
                        ScrollClipBugRepro)
