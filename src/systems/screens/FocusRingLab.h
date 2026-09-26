#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Focus ring family: :focus-visible, per-widget ring offset, and the two
// highlight modes. Clicking a button gives it focus but, in Split mode,
// draws no ring -- the ring appears when focus arrives by Tab. The mode
// toggle shows FollowsMostRecentInput, where the mouse drives the ring on
// purpose and the :focus-visible gate does not apply.
struct FocusRingLab : ScreenSystem<UIContext<InputAction>> {
  bool follow_mouse = false;
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    theme.highlight_mode = follow_mouse ? HighlightMode::FollowsMostRecentInput
                                        : HighlightMode::Split;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("frl_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    text(0, "Focus ring lab", 16, 0, 630, 44, 30, "frl_title");
    text(1, "Tab / Shift+Tab moves focus. Click a button: it takes focus, but in Split mode draws no ring.",
         16, 48, 1150, 28, 19);

    struct Spot {
      const char *label;
      float x, y;
    };
    static const Spot spots[] = {
        {"First", 40, 130}, {"Second", 280, 130}, {"Third", 520, 130},
    };
    for (int i = 0; i < 3; ++i) {
      const auto &spot = spots[i];
      button(context, mk(root.ent(), 10 + i), ComponentConfig{}
          .with_label(spot.label).with_720p_size(200, 48)
          .with_absolute_position(spot.x * scale, spot.y * scale)
          .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
          .with_corner_radius(6 * scale).with_debug_name("frl_btn_" + std::to_string(i)));
    }
    text(20, "Per-widget ring offset (theme default is 4, an inset)", 40, 226, 700, 30, 20);
    struct OffsetSpot {
      const char *label;
      const char *caption;
      float offset;
      bool custom;
    };
    static const OffsetSpot offset_spots[] = {
        {"Outset", "offset -6: ring outside the box", -6.f, true},
        {"Default", "theme offset 4", 0.f, false},
        {"Deep inset", "offset 12: ring well inside", 12.f, true},
    };
    for (int i = 0; i < 3; ++i) {
      const auto &spot = offset_spots[i];
      auto config = ComponentConfig{}
          .with_label(spot.label).with_720p_size(200, 48)
          .with_absolute_position((40 + static_cast<float>(i) * 240) * scale, 268 * scale)
          .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
          .with_corner_radius(6 * scale).with_debug_name("frl_off_" + std::to_string(i));
      if (spot.custom)
        config.with_focus_ring_offset(spot.offset);
      button(context, mk(root.ent(), 30 + i), config);
      text(40 + i, spot.caption, 40 + static_cast<float>(i) * 240, 330, 230, 26, 16);
    }

    if (button(context, mk(root.ent(), 50), ComponentConfig{}
        .with_label(follow_mouse ? "Mode: FollowsMostRecentInput" : "Mode: Split")
        .with_720p_size(340, 44)
        .with_absolute_position(40 * scale, 400 * scale)
        .with_font("AtkinsonMock", pixels(18 * scale)).with_background(Theme::Usage::Surface)
        .with_debug_name("frl_mode")))
      follow_mouse = !follow_mouse;
    text(51, "In FollowsMostRecentInput the mouse drives the ring; the :focus-visible gate is off.",
         400, 408, 760, 28, 17);

    const char *modality = context.focus_modality == FocusSource::Pointer    ? "Pointer"
                           : context.focus_modality == FocusSource::Explicit ? "Explicit"
                                                                             : "Grab";
    text(60, "focus_id: " + std::to_string(context.focus_id) +
                 "   modality: " + modality +
                 "   focus_visible: " + (context.focus_visible() ? "yes" : "no") +
                 "   ring on: " + std::to_string(context.visual_focus_id),
         40, 480, 1100, 30, 19, "frl_readout");
    text(61, "Custom widgets resolve the same ring via ui::focus_ring_for(context, entity, component).",
         40, 524, 1100, 28, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(focus_ring_lab, "System Demos",
                        "focus ring: :focus-visible, offsets, highlight modes", FocusRingLab)
