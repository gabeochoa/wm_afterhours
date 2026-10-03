#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/frame_loop.h>

#include <atomic>
#include <chrono>
#include <thread>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

inline std::atomic<int> &host_loop_async_pending() {
  static std::atomic<int> n{0};
  return n;
}
inline std::atomic<int> &host_loop_async_done() {
  static std::atomic<int> n{0};
  return n;
}

struct HostLoopLab : ScreenSystem<UIContext<InputAction>> {
  bool started = false;
  int granted_frames = 0;
  float marker_x = 0.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    if (!started) {
      started = true;
      afterhours::frame_loop::set_mode(
          afterhours::frame_loop::Mode::Continuous);
      afterhours::frame_loop::reset_stats();
    }
    const bool on_demand = afterhours::frame_loop::mode() ==
                           afterhours::frame_loop::Mode::OnDemand;
    const bool granted =
        !on_demand || afterhours::frame_loop::consume_frame_request();
    granted_frames += granted ? 1 : 0;
    marker_x += granted ? dt * 120.f : 0.f;
    if (marker_x > 516.f)
      marker_x = 0.f;

    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f,
                                 context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock",
                                               pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16)))
        .with_debug_name("hl_root"));
    auto text = [&](int id, const std::string &label, float x, float y,
                    float w, float h, float size) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale))
          .with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    auto btn = [&](int id, const char *label, float x, const char *debug) {
      return button(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(240 * scale), pixels(44 * scale)})
          .with_absolute_position(x * scale, 120 * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(19 * scale))
          .with_custom_background({52, 74, 105, 255})
          .with_custom_text_color(theme.font).with_corner_radius(5 * scale)
          .with_debug_name(debug));
    };

    text(0, "Host loop lab", 16, 0, 700, 44, 30);
    text(1, "The marker moves on granted frames only.", 16, 48, 900, 28, 19);

    if (btn(2, on_demand ? "Use continuous" : "Use on-demand", 32, "hl_mode"))
      afterhours::frame_loop::set_mode(
          on_demand ? afterhours::frame_loop::Mode::Continuous
                    : afterhours::frame_loop::Mode::OnDemand);
    if (btn(3, "Request frame", 296, "hl_request"))
      afterhours::frame_loop::request_frame();
    if (btn(4, "Finish async work", 560, "hl_async")) {
      host_loop_async_pending()++;
      std::thread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        host_loop_async_pending()--;
        host_loop_async_done()++;
        afterhours::frame_loop::request_frame();
      }).detach();
    }
    if (btn(5, "Reset counters", 824, "hl_reset")) {
      afterhours::frame_loop::reset_stats();
      granted_frames = 0;
      host_loop_async_done() = 0;
    }

    div(context, mk(root.ent(), 6), ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(36 * scale)})
        .with_absolute_position(32 * scale, 200 * scale)
        .with_background(Theme::Usage::Surface).with_corner_radius(5 * scale)
        .with_ignore_pointer_events());
    div(context, mk(root.ent(), 7), ComponentConfig{}
        .with_size({pixels(36 * scale), pixels(36 * scale)})
        .with_absolute_position((36 + marker_x) * scale, 200 * scale)
        .with_background(Theme::Usage::Accent).with_corner_radius(5 * scale)
        .with_ignore_pointer_events());

    const auto stats = afterhours::frame_loop::stats();
    text(8, on_demand ? "mode on-demand" : "mode continuous",
         32, 280, 560, 30, 21);
    text(9, "requests " + std::to_string(stats.requests), 32, 320, 560, 30, 21);
    text(10, "granted frames " + std::to_string(granted_frames),
         32, 360, 560, 30, 21);
    text(11,
         "async pending " + std::to_string(host_loop_async_pending().load()),
         32, 400, 560, 30, 21);
    text(12, "async done " + std::to_string(host_loop_async_done().load()),
         32, 440, 560, 30, 21);

    text(13, "frame_loop::set_mode, frame_loop::request_frame, "
             "frame_loop::consume_frame_request",
         32, 640, 1100, 28, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(host_loop_lab, "System Demos",
                        "request-frame, frame-wake, target fps", HostLoopLab)
