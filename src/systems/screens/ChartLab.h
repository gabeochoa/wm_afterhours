#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/ui/line_chart.h>
#include <cmath>

struct ChartLab : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int dataset = 4;
  bool live = false;
  double time = 0;
  std::size_t selected = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    context.theme = theme_presets::neon_dark();
    if (live) time += dt;
    auto root = vstack(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background).with_corner_radius(0)
        .with_padding(Padding::all(w1280(24))).with_gap(h720(12))
        .with_debug_name("chart_root"));
    div(context, mk(root.ent(), 0), ComponentConfig{}
        .with_label("Charts / frame timings").with_size({percent(1), h720(40)})
        .with_font_size(h720(26)));
    auto controls = hstack(context, mk(root.ent(), 1), ComponentConfig{}
        .with_size({percent(1), h720(40)}).with_gap(w1280(8)).with_no_wrap());
    constexpr const char *labels[] = {"Empty", "Single", "Constant", "Negative", "Multiple"};
    for (int i = 0; i < 5; ++i) {
      if (button(context, mk(controls.ent(), i), ComponentConfig{}
          .with_label(labels[i]).with_720p_size(135, 36)
          .with_debug_name(fmt::format("chart_case_{}", i)))) {
        dataset = i;
        selected = 0;
      }
    }
    if (button(context, mk(controls.ent(), 5), ComponentConfig{}
        .with_label(live ? "Pause live" : "Start live").with_720p_size(150, 36)
        .with_debug_name("chart_live"))) live = !live;
    std::vector<ChartSeries> series;
    ChartSeries update{"Update", {}, afterhours::Color{94, 225, 212, 255}};
    ChartSeries render{"Render", {}, afterhours::Color{255, 193, 107, 255}};
    const int count = dataset == 0 ? 0 : dataset == 1 ? 1 : 120;
    for (int i = 0; i < count; ++i) {
      const double x = i / 60.;
      const double y = dataset == 2 ? 8 : dataset == 3 ? std::sin(x * 5 + time) * 8 : 8 + std::sin(x * 5 + time) * 3;
      update.points.push_back({x, y});
      render.points.push_back({x, 4 + std::cos(x * 7 + time) * 2});
    }
    series.push_back(std::move(update));
    if (dataset == 4) series.push_back(std::move(render));
    line_chart(context, mk(root.ent(), 2), std::move(series), {"ms", selected},
        ComponentConfig{}.with_size({percent(1), h720(430)})
        .with_background(Theme::Usage::Surface).with_debug_name("chart_plot"));
    auto samples = hstack(context, mk(root.ent(), 3), ComponentConfig{}
        .with_size({percent(1), h720(40)}).with_gap(w1280(8)).with_no_wrap());
    if (button(context, mk(samples.ent(), 0), ComponentConfig{}
        .with_label("Previous sample").with_720p_size(190, 36)
        .with_debug_name("chart_previous")) && selected > 0) --selected;
    if (button(context, mk(samples.ent(), 1), ComponentConfig{}
        .with_label("Next sample").with_720p_size(190, 36)
        .with_debug_name("chart_next")) && selected + 1 < static_cast<std::size_t>(count)) ++selected;
    div(context, mk(samples.ent(), 2), ComponentConfig{}
        .with_label(count == 0 ? "No samples" : fmt::format("Sample {} of {}", selected + 1, count))
        .with_720p_size(240, 36).with_debug_name("chart_selection"));
    div(context, mk(root.ent(), 4), ComponentConfig{}
        .with_label("Hover for values. Tab through controls; Enter selects. X: seconds / Y: milliseconds.")
        .with_size({percent(1), h720(32)}).with_font_size(h720(16)));
  }
};

REGISTER_EXAMPLE_SCREEN(chart_lab, "System Demos", "Interactive timing charts", ChartLab)
