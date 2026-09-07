#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// blend::Mode, blend_scope, and the transition counter.
//
// The bottom two rows draw the same eight blits: one scope each, then one
// scope round the batch. The count in the title is the difference.
struct BlendModeLab : ScreenSystem<UIContext<InputAction>> {
  struct ModeSwatch {
    const char *caption;
    afterhours::blend::Mode mode;
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    namespace blend = afterhours::blend;

    // Read before this frame's own drawing so the number is last frame's, which
    // is the whole frame rather than a partial one.
    const size_t last_frame_transitions = blend::transitions_this_frame();
    blend::reset_transition_count();

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_padding(Padding::all(w1280(16)))
                        .with_debug_name("bml_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format(
                "Blend modes. Real transitions last frame: {}",
                last_frame_transitions))
            .with_size(ComponentSize{percent(1.f), h720(36)})
            .with_font(UIComponent::DEFAULT_FONT, h720(17.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("bml_title"));

    static const ModeSwatch swatches[] = {
        {"Alpha", blend::Mode::Alpha},
        {"Additive", blend::Mode::Additive},
        {"Multiplied", blend::Mode::Multiplied},
        {"AlphaPremultiply", blend::Mode::AlphaPremultiply},
    };

    auto row = hstack(context, mk(root.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(260)})
                          .with_gap(w1280(12))
                          .with_margin(Margin{.top = h720(10)})
                          .with_no_wrap()
                          .with_debug_name("bml_row"));

    int idx = 0;
    for (const auto &sw : swatches) {
      auto col = vstack(context, mk(row.ent(), idx),
                        ComponentConfig{}
                            .with_size(ComponentSize{expand(), percent(1.f)})
                            .with_debug_name(fmt::format("bml_col_{}", idx)));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(sw.caption)
              .with_size(ComponentSize{percent(1.f), h720(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
              .with_background(Theme::Usage::None)
              .with_debug_name(fmt::format("bml_cap_{}", idx)));

      // Three overlapping discs, drawn in this swatch's mode. Overlap is what
      // makes the modes tell each other apart.
      const auto mode = sw.mode;
      div(context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), h720(220)})
              .with_custom_background(afterhours::Color{28, 30, 40, 255})
              .with_on_draw_fg([mode](RectangleType rr) {
                afterhours::blend_scope scope(mode);
                const float r = rr.height * 0.26f;
                const float cx = rr.x + rr.width * 0.5f;
                const float cy = rr.y + rr.height * 0.5f;
                const auto disc = [&](float dx, float dy,
                                      afterhours::Color c) {
                  afterhours::draw_circle(static_cast<int>(cx + dx),
                                          static_cast<int>(cy + dy), r, c);
                };
                disc(-r * 0.6f, -r * 0.35f,
                     afterhours::Color{220, 70, 70, 170});
                disc(r * 0.6f, -r * 0.35f,
                     afterhours::Color{70, 200, 120, 170});
                disc(0.f, r * 0.55f, afterhours::Color{80, 130, 235, 170});
              })
              .with_debug_name(fmt::format("bml_swatch_{}", idx)));
      idx++;
    }

    // Same eight blits twice: once wrapped individually, once under one scope.
    // The captions say what each costs.
    auto compare = hstack(context, mk(root.ent(), 2),
                          ComponentConfig{}
                              .with_size(ComponentSize{percent(1.f), h720(150)})
                              .with_gap(w1280(12))
                              .with_margin(Margin{.top = h720(10)})
                              .with_no_wrap()
                              .with_debug_name("bml_compare"));

    const char *captions[] = {"one scope per blit: 2 transitions each",
                              "one scope round the batch: 2 total"};
    for (int which = 0; which < 2; which++) {
      auto col = vstack(context, mk(compare.ent(), which),
                        ComponentConfig{}
                            .with_size(ComponentSize{expand(), percent(1.f)})
                            .with_debug_name(fmt::format("bml_cmp_{}", which)));

      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(captions[which])
              .with_size(ComponentSize{percent(1.f), h720(24)})
              .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
              .with_background(Theme::Usage::None)
              .with_debug_name(fmt::format("bml_cmpcap_{}", which)));

      const bool hoisted = (which == 1);
      div(context, mk(col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), h720(110)})
              .with_custom_background(afterhours::Color{28, 30, 40, 255})
              .with_on_draw_fg([hoisted](RectangleType rr) {
                const auto draw_one = [&](int i) {
                  const float w = rr.width / 10.f;
                  afterhours::draw_rectangle(
                      RectangleType{rr.x + w * (i + 1), rr.y + rr.height * 0.3f,
                                    w * 0.8f, rr.height * 0.4f},
                      afterhours::Color{120, 200, 255, 150});
                };
                if (hoisted) {
                  afterhours::blend_scope outer(
                      afterhours::blend::Mode::Additive);
                  for (int i = 0; i < 8; i++) {
                    afterhours::blend_scope inner(
                        afterhours::blend::Mode::Additive);
                    draw_one(i);
                  }
                } else {
                  for (int i = 0; i < 8; i++) {
                    afterhours::blend_scope each(
                        afterhours::blend::Mode::Additive);
                    draw_one(i);
                  }
                }
              })
              .with_debug_name(fmt::format("bml_cmpbox_{}", which)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(blend_mode_lab, "System Demos",
                        "blend::Mode, blend_scope and the transition counter",
                        BlendModeLab)
