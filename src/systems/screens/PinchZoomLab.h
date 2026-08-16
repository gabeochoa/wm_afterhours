#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Trackpad pinch. The gesture cannot be produced headlessly, so this screen
// exists to prove the plumbing: the e2e `pinch` command drives the same
// get_pinch_delta() a real trackpad does.
struct PinchZoomLab : ScreenSystem<UIContext<InputAction>> {
  float zoom = 1.f;
  int events = 0;

  static constexpr float kMinZoom = 0.25f;
  static constexpr float kMaxZoom = 4.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};

    // The whole integration: a delta composes, so there is no gesture-start
    // zoom to latch.
    const float pinch = afterhours::input::get_pinch_delta();
    if (pinch != 0.f) {
      zoom = std::clamp(zoom * (1.f + pinch), kMinZoom, kMaxZoom);
      events++;
    }

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label("Pinch Zoom - trackpad magnification")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("pz_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 50.f)
            .with_label(fmt::format("zoom {:.2f}x   events {}   pinching {}",
                                    zoom, events,
                                    afterhours::input::is_pinching() ? "yes"
                                                                     : "no"))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("pz_readout"));

    // A square that scales, so the zoom is visible rather than just numeric.
    const float side = 220.f * zoom;
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(side), pixels(side)})
            .with_absolute_position(640.f - side / 2.f, 380.f - side / 2.f)
            .with_custom_background(afterhours::Color{70, 110, 180, 255})
            .with_label(fmt::format("{:.2f}x", zoom))
            .with_custom_text_color(white)
            .with_font_size(20.f)
            .with_debug_name("pz_box"));

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 660.f)
            .with_label("needs -fblocks -framework AppKit and "
                        "AFTER_HOURS_ENABLE_MACOS_GESTURES; reads 0 otherwise")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("pz_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(pinch_zoom_lab, "System Demos",
                        "Trackpad pinch magnification driving a zoom",
                        PinchZoomLab)
