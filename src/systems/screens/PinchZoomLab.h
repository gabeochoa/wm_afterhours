#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>

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
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280.f * s) / 2.f;
    const float top = (context.screen_height - 720.f * s) / 2.f;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{173, 188, 210, 255};
    const auto ink = afterhours::Color{14, 21, 35, 255};
    const auto panel = afterhours::Color{25, 37, 56, 255};
    const auto cyan = afterhours::Color{112, 218, 230, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));

    // The whole integration: a delta composes, so there is no gesture-start
    // zoom to latch.
    const float pinch = afterhours::input::get_pinch_delta();
    if (pinch != 0.f) {
      zoom = std::clamp(zoom * (1.f + pinch), kMinZoom, kMaxZoom);
      events++;
    }

    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0)
        .with_ignore_pointer_events());
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("pz_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w,
                    float h, float size, afterhours::Color color,
                    const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Pinch zoom", 48, 24, 700, 48, 38, white, "pz_title", true);
    text(1, "Pinch two fingers on the trackpad to resize the square.",
         48, 78, 1184, 30, 23, muted, "pz_instruction");
    if (button(context, mk(root.ent(), 2), box(1058, 132, 174, 44)
        .with_label("Reset to 1.00×").with_font("AtkinsonMock", pixels(20 * s))
        .with_custom_background(cyan).with_custom_text_color(ink)
        .with_corner_radius(6).with_debug_name("pz_reset"))) zoom = 1.f;
    text(3, "Scale", 48, 120, 180, 24, 17, muted);
    text(4, fmt::format("{:.2f}×", zoom), 48, 148, 180, 42, 32, white, "pz_readout", true);
    text(5, "Width / design px", 252, 120, 220, 24, 17, muted);
    text(6, fmt::format("{:.1f}", 220.f * zoom), 252, 148, 180, 42, 30, white, "pz_width", true);
    text(7, "Height / design px", 472, 120, 220, 24, 17, muted);
    text(8, fmt::format("{:.1f}", 220.f * zoom), 472, 148, 180, 42, 30, white, "pz_height", true);
    text(9, "Pinch updates received", 712, 120, 260, 24, 17, muted);
    text(10, std::to_string(events), 712, 148, 260, 42, 30, white, "pz_events", true);

    auto stage = div(context, mk(root.ent(), 11), box(48, 214, 816, 424)
        .with_custom_background(panel).with_clip_children(true).with_debug_name("pz_stage"));
    for (int i = 0; i < 37; ++i) {
      div(context, mk(stage.ent(), 20 + i), box(12.f + static_cast<float>(i) * 22, 0, 1, 424)
          .with_custom_background(afterhours::Color{40, 56, 77, 255}).with_ignore_pointer_events());
    }
    for (int i = 0; i < 19; ++i) {
      div(context, mk(stage.ent(), 60 + i), box(0, 14.f + static_cast<float>(i) * 22, 816, 1)
          .with_custom_background(afterhours::Color{40, 56, 77, 255}).with_ignore_pointer_events());
    }
    // A square that scales, so the zoom is visible rather than just numeric.
    const float side = 220.f * zoom;
    div(context, mk(stage.ent(), 2), box(408 - side / 2, 212 - side / 2, side, side)
        .with_custom_background(afterhours::Color{66, 111, 185, 255})
        .with_ignore_pointer_events().with_debug_name("pz_box"));
    const std::array<RectangleType, 4> reference{{
        {298, 102, 220, 1}, {298, 321, 220, 1}, {298, 102, 1, 220}, {517, 102, 1, 220}}};
    for (size_t i = 0; i < reference.size(); ++i) {
      const auto r = reference[i];
      div(context, mk(stage.ent(), 100 + static_cast<int>(i)), box(r.x, r.y, r.width, r.height)
          .with_custom_background(afterhours::Color{193, 219, 247, 170}).with_ignore_pointer_events());
    }
    div(context, mk(stage.ent(), 110), box(298, 194, 220, 36)
        .with_label(fmt::format("{:.2f}×", zoom)).with_alignment(TextAlignment::Center)
        .with_font("AtkinsonMockBold", pixels(26 * s)).with_custom_text_color(white)
        .with_ignore_pointer_events().with_debug_name("pz_box_label"));
    div(context, mk(root.ent(), 12), box(888, 214, 344, 424).with_custom_background(panel));
    const bool pinching = afterhours::input::is_pinching();
    div(context, mk(root.ent(), 13), box(908, 234, 144, 34)
        .with_label(pinching ? "Pinching" : "Idle").with_alignment(TextAlignment::Center)
        .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(pinching ? ink : white)
        .with_custom_background(pinching ? cyan : afterhours::Color{53, 70, 91, 255})
        .with_corner_radius(6).with_debug_name("pz_state"));
    text(14, "Demo prerequisites", 908, 288, 304, 34, 26, white, "pz_requirements", true);
#if defined(__APPLE__) && defined(AFTER_HOURS_ENABLE_MACOS_GESTURES)
    text(15, "Native backend compiled", 908, 332, 304, 30, 20, cyan, "pz_support");
#else
    text(15, "Unavailable in this build", 908, 332, 304, 30, 20, cyan, "pz_support");
#endif
    text(16, "Hardware availability is not", 908, 372, 304, 26, 19, muted);
    text(17, "reported by this API.", 908, 400, 304, 26, 19, muted);
    text(18, "macOS + AppKit + blocks", 908, 444, 304, 26, 19, white);
    text(19, "Gesture build flag required", 908, 474, 304, 26, 19, muted);
    text(20, "Injected updates use the", 908, 528, 304, 26, 19, muted);
    text(21, "same zoom calculation.", 908, 556, 304, 26, 19, muted);
    text(22, "Range 0.25× to 4.00× / outline 1.00× / grid 22 design px",
         48, 650, 1184, 26, 19, muted, "pz_range");
    text(23, fmt::format("Square {:.1f} × {:.1f} screen px. Large zoom is clipped to the preview.", side * s, side * s),
         48, 682, 1184, 26, 18, muted, "pz_hint");
  }
};

REGISTER_EXAMPLE_SCREEN(pinch_zoom_lab, "System Demos",
                        "Trackpad pinch magnification driving a zoom",
                        PinchZoomLab)
