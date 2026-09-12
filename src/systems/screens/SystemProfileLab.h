#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/ui/profiler.h>

struct SystemProfileLab : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  afterhours::ui::imm::ProfilerState state;
  bool load = false;
  bool panel_visible = true;
  double result = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    context.theme = theme_presets::neon_dark();
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20));
    auto &collector = afterhours::profiling::default_collector();
    if (load) {
      for (int i = 0; i < 40000; ++i) result = std::sin(result + i * .0001);
    }
    AFTERHOURS_PROFILE_COUNTER(collector, "Demo load", "iterations", load ? 40000 : 0);
    auto root = vstack(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background).with_corner_radius(0)
        .with_padding(Padding::all(w1280(20))).with_gap(h720(8))
        .with_debug_name("spl_root"));
    auto heading = hstack(context, mk(root.ent(), 0), ComponentConfig{}
        .with_size({percent(1), h720(40)}).with_gap(w1280(12)).with_no_wrap());
    div(context, mk(heading.ent(), 0), ComponentConfig{}
        .with_label("Profiler").with_720p_size(260, 36).with_font_size(h720(26)));
    if (button(context, mk(heading.ent(), 1), ComponentConfig{}
        .with_label(load ? "Remove load" : "Add load").with_720p_size(180, 36)
        .with_debug_name("spl_load"))) load = !load;
    if (button(context, mk(heading.ent(), 2), ComponentConfig{}
        .with_label(panel_visible ? "Hide panel" : "Show panel").with_720p_size(180, 36)
        .with_debug_name("spl_visibility"))) panel_visible = !panel_visible;
    div(context, mk(root.ent(), 1), ComponentConfig{}
        .with_label(!afterhours::profiling::available ? "Profiling compiled out" :
                    collector.recording() ? "Recording in background" : "Recording stopped")
        .with_size({percent(1), h720(28)}).with_font_size(h720(16))
        .with_debug_name("spl_state"));
    if (!panel_visible) return;
    auto &fonts = *afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto previous_font = fonts.active_font;
    fonts.set_active("AtkinsonMock");
    ProfilerOptions options;
    options.font_size = 20;
    options.max_rows = 5;
    profiler_panel(context, mk(root.ent(), 2), collector, state, options,
        ComponentConfig{}.with_size({percent(1), percent(.86f)}));
    fonts.set_active(previous_font);
  }
};

REGISTER_EXAMPLE_SCREEN(system_profile_lab, "System Demos", "Frame and system profiling", SystemProfileLab)
