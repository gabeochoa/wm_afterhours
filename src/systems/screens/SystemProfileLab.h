#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/e2e_testing/perf_commands.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// SystemManager::set_profile_hook, read back through PerfProvider.
//
// The point is the names in the table: every one belongs to a system this
// screen did not write, and none of them could appear in a profile before.
struct SystemProfileLab : ScreenSystem<UIContext<InputAction>> {
  bool profiling = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    namespace perf = afterhours::testing::perf_commands;

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_padding(Padding::all(w1280(16)))
                        .with_debug_name("spl_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Per-system timings, including the library's own")
            .with_size(ComponentSize{percent(1.f), h720(36)})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("spl_title"));

    auto controls = hstack(context, mk(root.ent(), 1),
                           ComponentConfig{}
                               .with_size(ComponentSize{percent(1.f), h720(44)})
                               .with_gap(w1280(8))
                               .with_margin(Margin{.top = h720(8)})
                               .with_no_wrap()
                               .with_debug_name("spl_controls"));

    if (button(context, mk(controls.ent(), 0),
               ComponentConfig{}
                   .with_label(profiling ? "stop" : "start")
                   .with_720p_size(120, 36)
                   .with_debug_name("spl_toggle"))) {
      profiling = !profiling;
      if (profiling)
        perf::builtin_profile::enable();
      else
        perf::builtin_profile::disable();
    }

    div(context, mk(controls.ent(), 1),
        ComponentConfig{}
            .with_label(profiling ? "collecting" : "stopped")
            .with_720p_size(200, 36)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.f))
            .with_background(Theme::Usage::None)
            .with_debug_name("spl_state"));

    auto table = vstack(context, mk(root.ent(), 2),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(520)})
                            .with_margin(Margin{.top = h720(8)})
                            .with_background(Theme::Usage::Surface)
                            .with_padding(Padding::all(w1280(8)))
                            .with_debug_name("spl_table"));

    // Empty until started, which is itself the thing worth showing: nothing
    // here is instrumented by this screen.
    const auto entries =
        perf::provider().top_entries ? perf::provider().top_entries(14)
                                     : std::vector<perf::PerfEntry>{};

    if (entries.empty()) {
      div(context, mk(table.ent(), 0),
          ComponentConfig{}
              .with_label("press start, then watch systems appear")
              .with_size(ComponentSize{percent(1.f), h720(28)})
              .with_font(UIComponent::DEFAULT_FONT, h720(14.f))
              .with_background(Theme::Usage::None)
              .with_debug_name("spl_empty"));
      return;
    }

    int row = 0;
    for (const auto &e : entries) {
      // Trim the namespace so the table is readable; the full name is what
      // dump_profile prints.
      std::string shown = e.name;
      const size_t last = shown.rfind("::");
      if (last != std::string::npos)
        shown = shown.substr(last + 2);

      div(context, mk(table.ent(), row),
          ComponentConfig{}
              .with_label(fmt::format("{:<44} {:>8.3f} ms  x{}", shown, e.ms,
                                      e.entity_count.value_or(0)))
              .with_size(ComponentSize{percent(1.f), h720(28)})
              .with_font(UIComponent::DEFAULT_FONT, h720(13.f))
              .with_background(Theme::Usage::None)
              .with_debug_name(fmt::format("spl_row_{}", row)));
      row++;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(system_profile_lab, "System Demos",
                        "Per-system timings via SystemManager's profile hook",
                        SystemProfileLab)
