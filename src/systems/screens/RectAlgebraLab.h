#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// rect::cut_*, pad, expand and align_*, drawn directly.
//
// Left: a window manager tiling a screen, which is cut_left and cut_right and
// nothing else. Right: the repro from the gap report -- take a header strip,
// inset the rest, centre a box in it.
struct RectAlgebraLab : ScreenSystem<UIContext<InputAction>> {
  bool center_guides = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float x0 = (context.screen_width - 1280 * s) / 2;
    const float y0 = (context.screen_height - 720 * s) / 2;
    const afterhours::Color ink{16, 23, 36, 255};
    const afterhours::Color white{232, 240, 250, 255};
    const afterhours::Color muted{173, 190, 213, 255};
    const afterhours::Color blue{137, 183, 242, 255};
    const afterhours::Color purple{180, 180, 221, 255};
    const afterhours::Color orange{245, 183, 111, 255};
    const afterhours::Color green{146, 219, 181, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(x0, y0).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("ral_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w, float h,
                    float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Rectangle algebra", 32, 18, 720, 46, 38, white, "ral_title", true);
    text(1, "Static geometry demonstration / cut, pad, expand and align", 32, 67, 1190, 28, 21, muted);
    text(2, "Window tiling", 32, 112, 596, 36, 28, white, "ral_tiling_title", true);
    text(3, "Cut header, pad body, center box", 652, 112, 596, 36, 28, white, "ral_repro_title", true);
    text(4, "Sidebar", 42, 155, 138, 27, 18, blue);
    text(5, "Canvas", 248, 155, 220, 27, 18, blue);
    text(6, "Inspector", 496, 155, 130, 27, 18, blue);
    text(7, "Header / cut_top 22 px", 672, 155, 380, 27, 18, blue);
    // ---- Tiling, the way a WM does it -------------------------------------
    div(context, mk(root.ent(), 10), box(32, 190, 596, 280)
        .with_custom_background(afterhours::Color{24, 27, 36, 255})
            .with_on_draw_fg([](RectangleType rr) {
              namespace r = afterhours::rect;
              const afterhours::Color frame{70, 120, 190, 255};
              const afterhours::Color fill{40, 70, 110, 200};

              auto pane = [&](RectangleType box, const char *) {
                afterhours::draw_rectangle(r::pad(box, 3.f), fill);
                afterhours::draw_rectangle_outline(r::pad(box, 3.f), frame);
              };

              RectangleType screen = r::pad(rr, 10.f);

              // A status strip off the bottom, then split the rest.
              auto [work, status] = r::cut_bottom(screen, 26.f);
              afterhours::draw_rectangle(
                  r::pad(status, 3.f),
                  afterhours::Color{50, 55, 70, 255});

              auto [sidebar, main_area] = r::cut_left(work, 90.f);
              pane(sidebar, "sidebar");

              auto [editor, inspector] = r::cut_right(main_area, 110.f);
              pane(inspector, "inspector");

              // The editor splits again, so the nesting is visible.
              auto [tabs, canvas] = r::cut_top(editor, 24.f);
              afterhours::draw_rectangle(
                  r::pad(tabs, 3.f), afterhours::Color{55, 60, 80, 255});
              pane(canvas, "canvas");
            })
        .with_debug_name("ral_tiling"));
    // ---- The report's repro ------------------------------------------------
    div(context, mk(root.ent(), 11), box(652, 190, 596, 280)
        .with_custom_background(afterhours::Color{24, 27, 36, 255})
            .with_on_draw_fg([guides = center_guides](RectangleType rr) {
              namespace r = afterhours::rect;
              RectangleType panel = r::pad(rr, 20.f);

              // "take a 22px header off this panel, inset the remainder by 6,
              // and centre a 64x36 box in it"
              auto [header, body] = r::cut_top(panel, 22.f);
              afterhours::draw_rectangle(
                  header, afterhours::Color{60, 90, 140, 255});

              const auto inner = r::pad(body, 6.f);
              afterhours::draw_rectangle_outline(
                  inner, afterhours::Color{90, 100, 130, 255});

              if (guides) {
                const afterhours::Color guide{77, 88, 112, 255};
                afterhours::draw_rectangle({inner.x + inner.width / 2.f, inner.y, 1, inner.height}, guide);
                afterhours::draw_rectangle({inner.x, inner.y + inner.height / 2.f, inner.width, 1}, guide);
              }
              afterhours::draw_rectangle(
                  r::align_center(inner, 64, 36),
                  afterhours::Color{220, 150, 70, 255});

              // expand() goes the other way: a hit target round the box.
              afterhours::draw_rectangle_outline(
                  r::expand(r::align_center(inner, 64, 36), 12.f),
                  afterhours::Color{220, 150, 70, 120});

              // align_* on each edge of the inner box.
              const afterhours::Color tick{120, 200, 160, 255};
              afterhours::draw_rectangle(r::align_top(inner, 30, 8), tick);
              afterhours::draw_rectangle(r::align_bottom(inner, 30, 8), tick);
              afterhours::draw_rectangle(r::align_left(inner, 8, 30), tick);
              afterhours::draw_rectangle(r::align_right(inner, 8, 30), tick);
            })
        .with_debug_name("ral_repro"));
    text(20, "1  pad(screen, 10 px)", 32, 486, 580, 28, 20, muted);
    text(21, "2  Status strip / cut_bottom 26 px", 32, 518, 580, 28, 20, muted);
    text(22, "3  Sidebar / cut_left 90 px", 32, 550, 580, 28, 20, blue);
    text(23, "4  Inspector / cut_right 110 px", 32, 582, 580, 28, 20, blue);
    text(24, "5  Tabs / cut_top 24 px; Canvas uses the rest", 32, 614, 596, 28, 20, muted);
    text(25, "Pane borders are inset 3 px from each cut.", 32, 658, 596, 28, 18, muted);
    text(30, "1  Outer pad 20 px; header cut 22 px", 652, 486, 596, 28, 20, blue);
    text(31, "2  Inner outline / pad(body, 6 px)", 652, 518, 596, 28, 20, purple);
    text(32, "3  Center box / 64 × 36 px", 652, 550, 596, 28, 20, orange);
    text(33, "4  Hit target / expand 12 px = 88 × 60 px", 652, 582, 596, 28, 20, orange);
    text(34, "5  align_top / bottom / left / right", 652, 614, 596, 28, 20, green);
    text(35, "All operation values are screen px.", 652, 658, 380, 28, 18, muted);
    if (button(context, mk(root.ent(), 40), box(1032, 656, 216, 36)
        .with_label(center_guides ? "Center guides: on" : "Center guides: off")
        .with_font("AtkinsonMock", pixels(18 * s)).with_custom_background(purple)
        .with_custom_text_color(ink).with_corner_radius(6 * s).with_debug_name("ral_guides")))
      center_guides = !center_guides;
  }
};

REGISTER_EXAMPLE_SCREEN(rect_algebra_lab, "System Demos",
                        "rect::cut, pad, expand and align",
                        RectAlgebraLab)
