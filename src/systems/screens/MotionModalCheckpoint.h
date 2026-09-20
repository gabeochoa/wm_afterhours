#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MotionModalCheckpoint : ScreenSystem<UIContext<InputAction>> {
  bool open = false;
  int bg_clicks = 0;
  int inner_clicks = 0;
  int reopen_in = 0;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    namespace motion = afterhours::motion;
    context.theme = afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{55, 50, 60, 255}, coral{255, 115, 105, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    if (reopen_in > 0 && --reopen_in == 0) open = true;

    div(context, mk(entity, 0), ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({255, 245, 235, 255}).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, const std::string &text, float x, float y) {
      return div(context, mk(entity, id), box(x, y, 500, 30).with_label(text)
          .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(1, "Modal checkpoint: fade in, fade out, input blocked while leaving", 80, 60);
    auto open_btn = button(context, mk(entity, 2), box(80, 140, 200, 56).with_label("Open").with_custom_background(coral)
                   .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink).with_corner_radius(12 * s)
                   .with_debug_name("open_btn"));
    if (open_btn) open = true;
    auto bg_btn = button(context, mk(entity, 3), box(320, 140, 200, 56).with_label("Background").with_custom_background({170, 235, 200, 255})
                   .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink).with_corner_radius(12 * s)
                   .with_debug_name("bg_btn"));
    if (bg_btn) ++bg_clicks;

    auto dialog = afterhours::modal(context, mk(entity, 5), open,
                                    afterhours::ModalConfig{}
                                        .with_title("Fading dialog")
                                        .with_size(h720(420), h720(220))
                                        .with_motion(motion::Spring::gentle(), motion::Timeline{.keys = {{0.f, 0.f}, {1.5f, 1.f}}}));
    if (dialog) {
      if (button(context, mk(dialog.ent(), 1),
                 ComponentConfig{}.with_size({pixels(140 * s), pixels(44 * s)}).with_label("Inner")
                     .with_custom_background({200, 170, 220, 255}).with_font("AtkinsonMock", pixels(20 * s))
                     .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("inner_btn")))
        ++inner_clicks;
      if (button(context, mk(dialog.ent(), 2),
                 ComponentConfig{}.with_size({pixels(140 * s), pixels(44 * s)}).with_label("Close")
                     .with_custom_background(coral).with_font("AtkinsonMock", pixels(20 * s))
                     .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("close_btn")))
        open = false;
      if (button(context, mk(dialog.ent(), 3),
                 ComponentConfig{}.with_size({pixels(220 * s), pixels(44 * s)}).with_label("Close then reopen")
                     .with_custom_background({255, 215, 140, 255}).with_font("AtkinsonMock", pixels(20 * s))
                     .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("close_reopen_btn"))) {
        open = false;
        reopen_in = 3;
      }
    }

    const auto &m = dialog.ent().get<afterhours::modal::Modal>();
    const char *phase = m.phase == afterhours::modal::Modal::Phase::Hidden     ? "hidden"
                        : m.phase == afterhours::modal::Modal::Phase::Entering ? "entering"
                        : m.phase == afterhours::modal::Modal::Phase::Visible  ? "visible"
                                                                                : "exiting";
    label(10, std::string("phase: ") + phase, 80, 500);
    label(11, "bg clicks: " + std::to_string(bg_clicks), 80, 540);
    label(12, "inner clicks: " + std::to_string(inner_clicks), 80, 580);
    label(13, std::string("open: ") + (open ? "yes" : "no"), 80, 620);
    const char *focus = context.has_focus(open_btn.id()) ? "open_btn"
                        : context.has_focus(bg_btn.id())  ? "bg_btn"
                        : context.focus_id == context.ROOT ? "none"
                                                            : "other";
    label(14, std::string("focus: ") + focus, 80, 660);
  }
};

REGISTER_EXAMPLE_SCREEN(motion_modal, "Animations",
                        "Modal overlay contract: enter, visible, exit, hidden",
                        MotionModalCheckpoint)
