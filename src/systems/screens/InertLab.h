#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct InertLab : ScreenSystem<UIContext<InputAction>> {
  int live_clicks = 0;
  int inert_clicks = 0;
  int nofocus_clicks = 0;
  int focusable_clicks = 0;
  bool live_checked = false;
  bool inert_checked = false;

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
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("inert_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };

    text(0, "Inert lab", 16, 0, 700, 44, 30, "inert_title");
    text(1, "The inert panel ignores pointer and keyboard: its button and toggle never change. The live panel is the control.",
         16, 48, 1150, 28, 19);

    text(10, "Live (control)", 32, 110, 300, 30, 21);
    if (button(context, mk(root.ent(), 11), ComponentConfig{}
            .with_label("Live button").with_720p_size(220, 48)
            .with_absolute_position(32 * scale, 150 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * scale).with_debug_name("inert_live_btn")))
      ++live_clicks;
    toggle_switch(context, mk(root.ent(), 12), live_checked, ComponentConfig{}
        .with_label("Live toggle").with_size({pixels(220 * scale), pixels(40 * scale)})
        .with_absolute_position(32 * scale, 214 * scale)
        .with_font("AtkinsonMock", pixels(18 * scale))
        .with_debug_name("inert_live_check"));
    text(13, "live clicks: " + std::to_string(live_clicks) +
                 "   toggle: " + (live_checked ? "on" : "off"),
         32, 270, 340, 28, 19, "inert_live_readout");

    auto inert_panel = div(context, mk(root.ent(), 20), ComponentConfig{}
        .with_size({pixels(340 * scale), pixels(190 * scale)})
        .with_absolute_position(420 * scale, 104 * scale)
        .with_background(Theme::Usage::Surface).with_corner_radius(8 * scale)
        .with_inert().with_debug_name("inert_panel"));
    text(21, "Inert subtree (with_inert)", 436, 110, 320, 30, 21);
    if (button(context, mk(inert_panel.ent(), 22), ComponentConfig{}
            .with_label("Inert button").with_720p_size(220, 48)
            .with_absolute_position(16 * scale, 46 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * scale).with_debug_name("inert_btn")))
      ++inert_clicks;
    toggle_switch(context, mk(inert_panel.ent(), 23), inert_checked, ComponentConfig{}
        .with_label("Inert toggle").with_size({pixels(220 * scale), pixels(40 * scale)})
        .with_absolute_position(16 * scale, 110 * scale)
        .with_font("AtkinsonMock", pixels(18 * scale))
        .with_debug_name("inert_check"));
    text(24, "inert clicks: " + std::to_string(inert_clicks) +
                 "   toggle: " + (inert_checked ? "on" : "off") + "  (stays 0 / off)",
         420, 310, 380, 28, 19, "inert_readout");
    text(25, "Tab order also skips the inert subtree.", 420, 344, 380, 26, 17);

    text(30, "takes_focus(false)", 32, 410, 400, 30, 21);
    text(31, "Click the no-focus button: it counts the click but keyboard focus does not move to it.",
         32, 444, 1150, 28, 19);
    if (button(context, mk(root.ent(), 32), ComponentConfig{}
            .with_label("Focusable button").with_720p_size(220, 48)
            .with_absolute_position(32 * scale, 486 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * scale).with_debug_name("inert_focusable_btn")))
      ++focusable_clicks;
    if (button(context, mk(root.ent(), 33), ComponentConfig{}
            .with_label("No-focus button").with_720p_size(220, 48)
            .with_absolute_position(280 * scale, 486 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Surface)
            .with_corner_radius(6 * scale).with_takes_focus(false).with_debug_name("inert_nofocus_btn")))
      ++nofocus_clicks;
    text(34, "focusable clicks: " + std::to_string(focusable_clicks) +
                 "   no-focus clicks: " + std::to_string(nofocus_clicks) +
                 "   focus_id: " + std::to_string(context.focus_id),
         32, 550, 1100, 30, 19, "inert_focus_readout");
  }
};

REGISTER_EXAMPLE_SCREEN(inert_lab, "System Demos",
                        "with_inert subtree + takes_focus(false): no pointer, keyboard or focus steal", InertLab)
