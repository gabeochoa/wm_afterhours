#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// HasScrollView's colour knobs: thickness used to be the only one. Four panes
// -- the default, a theme usage, explicit colours, and a thick bar.
struct ScrollbarStyleLab : ScreenSystem<UIContext<InputAction>> {
  struct Pane {
    const char *caption;
    // Which knob this pane demonstrates. Applied to the live component after
    // the view exists, the same way sync_group and show_scrollbar are.
    void (*style)(HasScrollView &);
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_debug_name("sbs_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Scrollbar styling: track and thumb, by usage or by colour")
            .with_size(ComponentSize{percent(1.f), h720(40)})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.f))
            .with_padding(Padding{.left = w1280(16)})
            .with_background(Theme::Usage::Surface)
            .with_debug_name("sbs_title"));

    static const Pane panes[] = {
        {"default", [](HasScrollView &) {}},
        {"usage: Accent thumb",
         [](HasScrollView &sv) {
           sv.scrollbar_thumb_usage = Theme::Usage::Accent;
         }},
        {"explicit colours",
         [](HasScrollView &sv) {
           sv.scrollbar_track_color = afterhours::Color{60, 30, 30, 255};
           sv.scrollbar_thumb_color = afterhours::Color{235, 120, 90, 255};
         }},
        {"thicker, no track",
         [](HasScrollView &sv) {
           sv.scrollbar_thickness = h720(14.f);
           sv.scrollbar_track_color = afterhours::Color{0, 0, 0, 0};
           sv.scrollbar_thumb_usage = Theme::Usage::Primary;
         }},
    };

    auto row = hstack(context, mk(root.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(420)})
                          .with_gap(w1280(16))
                          .with_padding(Padding::all(w1280(16)))
                          .with_no_wrap()
                          .with_debug_name("sbs_row"));

    int idx = 0;
    for (const auto &pane : panes) {
      auto col = vstack(context, mk(row.ent(), idx),
                        ComponentConfig{}
                            .with_size(ComponentSize{expand(), percent(1.f)})
                            .with_debug_name(fmt::format("sbs_col_{}", idx)));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(pane.caption)
              .with_size(ComponentSize{percent(1.f), h720(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(14.f))
              .with_background(Theme::Usage::None)
              .with_debug_name(fmt::format("sbs_caption_{}", idx)));

      auto view =
          div(context, mk(col.ent(), 1),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(1.f), h720(340)})
                  .with_background(Theme::Usage::Surface)
                  .with_overflow(Overflow::Scroll, Axis::Y)
                  .with_padding(Padding::all(w1280(6)))
                  .with_debug_name(fmt::format("sbs_view_{}", idx)));

      if (view.ent().has<HasScrollView>()) {
        auto &sv = view.ent().get<HasScrollView>();
        sv.show_scrollbar = true;
        pane.style(sv);
      }

      // Enough rows to overflow, so every pane actually has a bar to look at.
      for (int r = 0; r < 24; r++)
        div(context, mk(view.ent(), r),
            ComponentConfig{}
                .with_label(fmt::format("row {:02d}", r))
                .with_size(ComponentSize{percent(1.f), h720(24)})
                .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
                .with_background(Theme::Usage::None)
                .with_debug_name(fmt::format("sbs_{}_{}", idx, r)));

      idx++;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(scrollbar_style_lab, "System Demos",
                        "HasScrollView track and thumb colours",
                        ScrollbarStyleLab)
