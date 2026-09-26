#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <cstring>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

namespace {
// The demo counts what the log actually receives. The sink is additive
// (stdout still prints), so installing it changes nothing else.
inline int g_layout_wrap_lines = 0;
inline int g_layout_overflow_lines = 0;
inline void count_layout_warnings(const char *, const char *message) {
  if (std::strstr(message, "Layout wrap:"))
    ++g_layout_wrap_lines;
  if (std::strstr(message, "Layout overflow:"))
    ++g_layout_overflow_lines;
}
} // namespace

// Validation-log throttle: the two bars below overflow their NoWrap rows
// every frame, the hanabi #53 scenario. Autolayout corrects them (watch
// the children shrink to fit) and used to log two warnings per bar per
// frame -- 570 lines in a 285-frame run. Each distinct (child, parent,
// reason) now warns exactly once via warn_once, so the counters on the
// right stop almost immediately while the frame count keeps climbing.
struct ValidationThrottleLab : ScreenSystem<UIContext<InputAction>> {
  int frames = 0;
  bool sink_installed = false;
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    if (!sink_installed) {
      log_sink_fn = &count_layout_warnings;
      sink_installed = true;
    }
    ++frames;
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("vtl_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    text(0, "Validation log throttle lab", 16, 0, 800, 44, 30);
    text(1, "Both bars overflow a NoWrap row every frame. Autolayout shrinks the children to fit -- and now logs it once per (child, parent, reason), not once per frame.",
         16, 48, 1150, 52, 19);

    auto bar = [&](int id, float y, const std::string &name,
                   const std::vector<std::string> &kids) {
      auto b = div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(460 * scale), pixels(44 * scale)})
          .with_absolute_position(32 * scale, y * scale)
          .with_flex_direction(FlexDirection::Row)
          .with_background(Theme::Usage::Surface)
          .with_corner_radius(6 * scale).with_debug_name(name));
      for (size_t i = 0; i < kids.size(); ++i)
        div(context, mk(b.ent(), static_cast<int>(i)), ComponentConfig{}
            .with_size({pixels(140 * scale), pixels(44 * scale)})
            .with_label(kids[i])
            .with_font("AtkinsonMock", pixels(17 * scale))
            .with_background(Theme::Usage::Primary)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::Center)
            .with_debug_name(kids[i]));
    };
    bar(10, 150, "find_bar", {"find_prev", "find_close", "find_next", "find_all"});
    text(20, "find_bar: 4 x 140px in a 460px NoWrap row", 32, 206, 700, 26, 17);
    bar(30, 260, "input_bar", {"text_input_field", "clear_button", "go_button", "pin_button"});
    text(40, "input_bar: a second (child, parent) pair -- its warning still fires", 32, 316, 800, 26, 17);

    text(50, "Frames this screen: " + std::to_string(frames), 32, 390, 700, 30, 21);
    text(51, "\"Layout wrap\" lines logged: " + std::to_string(g_layout_wrap_lines),
         32, 428, 700, 30, 21);
    text(52, "\"Layout overflow\" lines logged: " + std::to_string(g_layout_overflow_lines),
         32, 466, 700, 30, 21);
    text(53, "Before the fix the two counters would match the frame count. Now they stop at one per distinct violation.",
         32, 520, 1100, 52, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(validation_throttle_lab, "System Demos",
                        "layout warnings warn once, not per frame", ValidationThrottleLab)
