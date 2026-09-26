#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// The small imm helpers: settings_list/settings_row with built-in dividers
// and label/value alignment, value_pill readouts paired with the controls
// they describe, and divider_line as a static rule (divider() without the
// drag behaviour).
struct SmallComponentsLab : ScreenSystem<UIContext<InputAction>> {
  float volume = 0.8f;
  float brightness = 0.4f;
  bool notifications = true;
  size_t quality = 2;
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
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("scl_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    text(0, "Small components lab", 16, 0, 700, 44, 30);
    text(1, "settings_list + settings_row, value_pill, divider_line", 16, 48, 900, 28, 19);

    text(2, "Settings list (dividers and alignment built in)", 32, 108, 560, 30, 21);
    auto list = settings_list(context, mk(root.ent(), 3), ComponentConfig{}
        .with_size({pixels(580 * scale), children()})
        .with_absolute_position(32 * scale, 146 * scale)
        .with_debug_name("scl_list"));
    auto pct = [](float v) {
      return std::to_string(static_cast<int>(v * 100.f + 0.5f)) + "%";
    };
    auto row_volume = settings_row(context, mk(list.ent(), 0), "Volume");
    slider(context, mk(row_volume.ent(), 0), volume, ComponentConfig{}
        .with_size({pixels(200 * scale), pixels(28 * scale)}));
    value_pill(context, mk(row_volume.ent(), 1), pct(volume), ComponentConfig{}
        .with_size({pixels(80 * scale), pixels(28 * scale)})
        .with_margin(Margin::Left(pixels(12 * scale))));
    auto row_bright = settings_row(context, mk(list.ent(), 1), "Brightness");
    slider(context, mk(row_bright.ent(), 0), brightness, ComponentConfig{}
        .with_size({pixels(200 * scale), pixels(28 * scale)}));
    value_pill(context, mk(row_bright.ent(), 1), pct(brightness), ComponentConfig{}
        .with_size({pixels(80 * scale), pixels(28 * scale)})
        .with_margin(Margin::Left(pixels(12 * scale))));
    static const std::vector<std::string> qualities = {"Low", "Medium", "High", "Ultra"};
    auto row_quality = settings_row(context, mk(list.ent(), 2), "Quality");
    stepper(context, mk(row_quality.ent(), 0), qualities, quality, ComponentConfig{}
        .with_size({pixels(200 * scale), pixels(32 * scale)}));
    value_pill(context, mk(row_quality.ent(), 1), qualities[quality], ComponentConfig{}
        .with_size({pixels(96 * scale), pixels(28 * scale)})
        .with_margin(Margin::Left(pixels(12 * scale))));
    auto row_notif = settings_row(context, mk(list.ent(), 3), "Notifications");
    toggle_switch(context, mk(row_notif.ent(), 0), notifications);
    value_pill(context, mk(row_notif.ent(), 1), notifications ? "On" : "Off",
               ComponentConfig{}
                   .with_size({pixels(64 * scale), pixels(28 * scale)})
                   .with_margin(Margin::Left(pixels(12 * scale))));

    text(4, "divider_line, horizontal", 660, 108, 560, 30, 21);
    text(5, "Text above the rule.", 660, 150, 560, 26, 17);
    divider_line(context, mk(root.ent(), 6), Axis::X, ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(1)})
        .with_absolute_position(660 * scale, 186 * scale));
    text(7, "Text below the rule.", 660, 198, 560, 26, 17);

    text(8, "divider_line, vertical", 660, 262, 560, 30, 21);
    auto panes = div(context, mk(root.ent(), 9), ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(150 * scale)})
        .with_absolute_position(660 * scale, 300 * scale)
        .with_flex_direction(FlexDirection::Row));
    div(context, mk(panes.ent(), 0), ComponentConfig{}
        .with_size({expand(), percent(1.f)}).with_label("Left pane")
        .with_background(Theme::Usage::Surface)
        .with_padding(Padding::all(pixels(16 * scale)))
        .with_alignment(TextAlignment::Center)
        .with_align_items(AlignItems::Center).with_justify_content(JustifyContent::Center));
    divider_line(context, mk(panes.ent(), 1), Axis::Y);
    div(context, mk(panes.ent(), 2), ComponentConfig{}
        .with_size({expand(), percent(1.f)}).with_label("Right pane")
        .with_background(Theme::Usage::Surface)
        .with_padding(Padding::all(pixels(16 * scale)))
        .with_alignment(TextAlignment::Center)
        .with_align_items(AlignItems::Center).with_justify_content(JustifyContent::Center));

    text(10, "Helpers: imm::settings_list, imm::settings_row, imm::value_pill, imm::divider_line.",
         32, 640, 1100, 28, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(small_components_lab, "System Demos",
                        "settings list, value pill, divider line", SmallComponentsLab)
