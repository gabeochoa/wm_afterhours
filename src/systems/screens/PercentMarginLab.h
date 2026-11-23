#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PercentMarginLab : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));

    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("pml_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    auto track = [&](int id, float y, const std::string &name) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(760 * scale), pixels(48 * scale)})
          .with_absolute_position(32 * scale, y * scale)
          .with_background(Theme::Usage::Surface).with_corner_radius(4 * scale)
          .with_debug_name(name));
    };

    text(0, "Percent margin lab", 16, 0, 700, 44, 30, "pml_title");
    text(1, "A percent size is charged its own margin (#162): size + margin equals the percent share, so the box fits its track instead of overflowing by the margin.",
         16, 48, 1150, 52, 19);

    text(10, "percent(1.0) + 40px left margin: box shrinks to 720, footprint is the full 760 track", 32, 112, 900, 26, 19);
    auto track_a = track(11, 142, "pml_track_a");
    button(context, mk(track_a.ent(), 12), ComponentConfig{}
        .with_size({percent(1.0f), pixels(48 * scale)})
        .with_margin(Margin{.left = pixels(40 * scale)})
        .with_background(Theme::Usage::Primary).with_corner_radius(4 * scale)
        .with_label("percent(1.0) box").with_font("AtkinsonMock", pixels(17 * scale))
        .with_debug_name("pml_percent_box"));

    text(20, "pixels(760) + same 40px margin: box keeps 760 and overflows the track by 40", 32, 212, 900, 26, 19);
    auto track_b = track(21, 242, "pml_track_b");
    button(context, mk(track_b.ent(), 22), ComponentConfig{}
        .with_size({pixels(760 * scale), pixels(48 * scale)})
        .with_margin(Margin{.left = pixels(40 * scale)})
        .with_background(Theme::Usage::Secondary).with_corner_radius(4 * scale)
        .with_label("pixels(760) box").with_font("AtkinsonMock", pixels(17 * scale))
        .with_debug_name("pml_pixels_box"));

    text(30, "Two percent(0.5) rows with 10px margins each: each shrinks, together they fit", 32, 312, 900, 26, 19);
    auto track_c = div(context, mk(root.ent(), 31), ComponentConfig{}
        .with_size({pixels(760 * scale), pixels(48 * scale)})
        .with_absolute_position(32 * scale, 342 * scale)
        .with_background(Theme::Usage::Surface).with_corner_radius(4 * scale)
        .with_flex_direction(FlexDirection::Row).with_debug_name("pml_track_c"));
    for (int i = 0; i < 2; ++i) {
      button(context, mk(track_c.ent(), 32 + i), ComponentConfig{}
          .with_size({percent(0.5f), pixels(48 * scale)})
          .with_margin(Margin{.left = pixels(10 * scale), .right = pixels(10 * scale)})
          .with_background(Theme::Usage::Primary).with_corner_radius(4 * scale)
          .with_label(i == 0 ? "half A" : "half B").with_font("AtkinsonMock", pixels(17 * scale))
          .with_debug_name("pml_half_" + std::to_string(i)));
    }

    text(40, "percent(1.0) + percent margin left 0.25: margin is 190 (25% of the track), box is 570, footprint still 760", 32, 412, 1000, 26, 19);
    auto track_d = track(41, 442, "pml_track_d");
    button(context, mk(track_d.ent(), 42), ComponentConfig{}
        .with_size({percent(1.0f), pixels(48 * scale)})
        .with_margin(Margin{.left = percent(0.25f)})
        .with_background(Theme::Usage::Primary).with_corner_radius(4 * scale)
        .with_label("percent(1.0) box, percent margin").with_font("AtkinsonMock", pixels(17 * scale))
        .with_debug_name("pml_percent_margin_box"));

    text(50, "Give the parent padding instead if the old full-width box plus margin was intended.",
         32, 522, 1150, 28, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(percent_margin_lab, "System Demos",
                        "percent sizing charged its own margin (#162), px vs percent rows", PercentMarginLab)
