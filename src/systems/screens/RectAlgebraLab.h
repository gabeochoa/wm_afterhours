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
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_padding(Padding::all(w1280(16)))
                        .with_debug_name("ral_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("rect algebra: cut, pad, align")
            .with_size(ComponentSize{percent(1.f), h720(36)})
            .with_font(UIComponent::DEFAULT_FONT, h720(17.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("ral_title"));

    auto row = hstack(context, mk(root.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(560)})
                          .with_gap(w1280(16))
                          .with_margin(Margin{.top = h720(10)})
                          .with_no_wrap()
                          .with_debug_name("ral_row"));

    // ---- Tiling, the way a WM does it -------------------------------------
    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.f)})
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
    div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.f)})
            .with_custom_background(afterhours::Color{24, 27, 36, 255})
            .with_on_draw_fg([](RectangleType rr) {
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
  }
};

REGISTER_EXAMPLE_SCREEN(rect_algebra_lab, "System Demos",
                        "rect::cut, pad, expand and align",
                        RectAlgebraLab)
