#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/ui/line_chart.h>
#include <array>
#include <cmath>

struct ChartLab : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int dataset = 4;
  bool live = false;
  bool budget_visible = false;
  double time = 0;
  std::size_t selected = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    namespace charts = afterhours::charts;
    context.theme = theme_presets::neon_dark();
    context.theme.secondary = {67, 78, 94, 255};
    context.scaling_mode = ScalingMode::Proportional;
    if (live) time += dt;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const auto white = afterhours::Color{238, 244, 252, 255};
    const auto muted = afterhours::Color{179, 193, 214, 255};
    const auto update_color = afterhours::Color{94, 225, 212, 255};
    const auto render_color = afterhours::Color{255, 193, 107, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)}).with_corner_radius(0)
        .with_custom_background({19, 24, 34, 255}).with_debug_name("chart_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto control = [&](float x, float y, float width, const std::string &text, const std::string &name) {
      return box(left + x, y, width, 36).with_label(text).with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background({49, 64, 88, 255}).with_custom_text_color(white)
          .with_corner_radius(7 * s).with_debug_name(name);
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 94)
        .with_custom_background({35, 43, 57, 255}).with_corner_radius(12 * s));
    label("Frame timing chart", 20, 34, 1104, 42, 34, white);
    label("Synthetic demo data. Compare samples, edge cases, and two timing series.", 20, 79, 1104, 28, 21, muted);
    label("Dataset", 20, 134, 104, 29, 22, white);
    constexpr std::array<const char *, 5> labels{"Empty", "Single sample", "Constant", "Negative-value test", "Multiple series"};
    constexpr std::array<float, 5> widths{100, 150, 150, 242, 198};
    float control_x = 130;
    for (int i = 0; i < 5; ++i) {
      auto config = control(control_x, 131, widths[static_cast<size_t>(i)], labels[static_cast<size_t>(i)], fmt::format("chart_case_{}", i));
      if (dataset == i) config.with_custom_background({58, 99, 156, 255}).with_border({176, 211, 255, 255}, 2 * s);
      if (button(context, mk(root.ent(), id++), config)) {
        dataset = i;
        selected = 0;
        live = false;
        time = 0;
      }
      control_x += widths[static_cast<size_t>(i)] + 12;
    }
    if (button(context, mk(root.ent(), id++), control(20, 184, 160, live ? "Pause live" : "Start live", "chart_live"))) live = !live;
    label(live ? "Live on" : "Live off", 198, 188, 160, 28, 22, live ? update_color : muted, "chart_live_status");
    label("Preset: " + std::string(labels[static_cast<size_t>(dataset)]), 378, 188, 430, 28, 20, white, "chart_preset");
    if (button(context, mk(root.ent(), id++), control(846, 184, 278,
        budget_visible ? "120 fps budget: on" : "120 fps budget: off", "chart_budget"))) budget_visible = !budget_visible;

    std::vector<ChartSeries> series;
    ChartSeries update{"Update", {}, update_color};
    ChartSeries render{"Render", {}, render_color};
    const int count = dataset == 0 ? 0 : dataset == 1 ? 1 : 120;
    for (int i = 0; i < count; ++i) {
      const double x = i / 60.;
      const double y = dataset == 2 ? 8 : dataset == 3 ? std::sin(x * 5 + time) * 8 : 8 + std::sin(x * 5 + time) * 3;
      update.points.push_back({x, y});
      render.points.push_back({x, 4 + std::cos(x * 7 + time) * 2});
    }
    series.push_back(std::move(update));
    if (dataset == 4) series.push_back(std::move(render));
    std::optional<charts::Bounds> extent;
    for (const auto &line : series) {
      const auto bounds = charts::bounds(line.points);
      if (!bounds) continue;
      if (!extent) {
        extent = bounds;
        continue;
      }
      extent->min_x = std::min(extent->min_x, bounds->min_x);
      extent->max_x = std::max(extent->max_x, bounds->max_x);
      extent->min_y = std::min(extent->min_y, bounds->min_y);
      extent->max_y = std::max(extent->max_y, bounds->max_y);
    }
    const float label_size = 20 * s;
    const float gutter = std::max(58.f, label_size * 4.f + 10.f);
    const float vertical = std::max(28.f, label_size + 12.f);
    const RectangleType plot{left * s + gutter, (244 + top) * s + vertical,
                             1144 * s - gutter - 16, 312 * s - vertical * 2};
    const auto mouse = context.mouse.pos;
    const bool hovering = extent && is_mouse_inside(mouse, plot);
    std::optional<size_t> preview;
    if (hovering) {
      const double x = std::lerp(extent->min_x, extent->max_x, static_cast<double>((mouse.x - plot.x) / plot.width));
      preview = charts::nearest_x(series.front().points, x);
    }
    if (button(context, mk(root.ent(), id++), control(20, 579, 42, "<", "chart_previous").with_disabled(selected == 0))) --selected;
    if (button(context, mk(root.ent(), id++), control(156, 579, 42, ">", "chart_next")
        .with_disabled(count == 0 || selected + 1 >= static_cast<size_t>(count)))) ++selected;
    const auto displayed_index = preview.value_or(selected);
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const std::string previous_font = fonts ? fonts->active_font : std::string{};
    if (fonts) fonts->set_active("AtkinsonMock");
    line_chart(context, mk(root.ent(), id++), series, {"ms", std::nullopt, label_size},
        box(left, 244, 1144, 312).with_custom_background({27, 34, 46, 255})
            .with_corner_radius(10 * s).with_debug_name("chart_plot"));
    if (fonts) fonts->set_active(previous_font);
    label("ms", 4, 239, 68, 25, 20, muted);
    label("seconds", 568, 550, 124, 25, 19, muted);
    if (extent) {
      const auto bounds = *extent;
      const auto position = [plot, bounds](charts::Point point) -> Vector2Type {
        return {plot.x + static_cast<float>(charts::fraction(point.x, bounds.min_x, bounds.max_x)) * plot.width,
                plot.y + (1.f - static_cast<float>(charts::fraction(point.y, bounds.min_y, bounds.max_y))) * plot.height};
      };
      for (int tick = 1; tick < 4; ++tick) {
        const double fraction = tick / 4.;
        if (bounds.min_y != bounds.max_y)
          label(fmt::format("{:.2f}", std::lerp(bounds.max_y, bounds.min_y, fraction)),
                4, plot.y / s - top + plot.height / s * static_cast<float>(fraction) - 13, gutter / s - 12, 26, 19, muted);
        if (bounds.min_x != bounds.max_x)
          label(fmt::format("{:.2f}", std::lerp(bounds.min_x, bounds.max_x, fraction)),
                plot.x / s - left + plot.width / s * static_cast<float>(fraction) - 30,
                (plot.y + plot.height) / s - top + 4, 82, 25, 19, muted);
      }
      div(context, mk(root.ent(), id++), box(left, 244, 1144, 312)
          .with_ignore_pointer_events().with_on_draw_fg([series, displayed_index, position, plot, s, label_size,
                                                         bounds, budget = budget_visible, update_color, render_color](RectangleType rect) {
            const float legend_y = rect.y + 16 * s;
            const float legend_x = plot.x + 82 * s;
            afterhours::draw_line_ex({legend_x, legend_y}, {legend_x + 32 * s, legend_y}, 2 * s, update_color);
            afterhours::draw_circle(static_cast<int>(legend_x + 16 * s), static_cast<int>(legend_y), 3 * s, update_color);
            if (series.size() == 2) {
              const float second = legend_x + std::max(130.f, label_size * 10.f);
              afterhours::draw_line_ex({second, legend_y}, {second + 32 * s, legend_y}, 2 * s, render_color);
              afterhours::draw_rectangle({second + 12 * s, legend_y - 4 * s, 8 * s, 8 * s}, render_color);
              for (size_t i = 0; i < series[1].points.size(); i += 12) {
                const auto point = position(series[1].points[i]);
                afterhours::draw_rectangle({point.x - 3 * s, point.y - 3 * s, 6 * s, 6 * s}, render_color);
              }
            }
            if (displayed_index < series.front().points.size()) {
              const float x = position(series.front().points[displayed_index]).x;
              for (float y = plot.y; y < plot.y + plot.height; y += 10 * s)
                afterhours::draw_line_ex({x, y}, {x, std::min(y + 5 * s, plot.y + plot.height)}, s, {186, 201, 223, 255});
              for (size_t i = 0; i < series.size(); ++i) {
                const auto point = position(series[i].points[displayed_index]);
                if (i == 0) afterhours::draw_circle(static_cast<int>(point.x), static_cast<int>(point.y), 5 * s, update_color);
                if (i == 1) afterhours::draw_rectangle({point.x - 5 * s, point.y - 5 * s, 10 * s, 10 * s}, render_color);
              }
            }
            constexpr double budget_ms = 1000. / 120.;
            if (!budget || budget_ms < bounds.min_y || budget_ms > bounds.max_y) return;
            const float y = position({bounds.min_x, budget_ms}).y;
            for (float x = plot.x; x < plot.x + plot.width; x += 14 * s)
              afterhours::draw_line_ex({x, y}, {std::min(x + 8 * s, plot.x + plot.width), y}, 2 * s, {166, 180, 208, 255});
          }));
      if (budget_visible) {
        constexpr double budget_ms = 1000. / 120.;
        const bool in_range = budget_ms >= bounds.min_y && budget_ms <= bounds.max_y;
        label(in_range ? "Dashed line: 8.33 ms / 120 fps budget"
                       : fmt::format("8.33 ms budget is {} this plot's range", budget_ms > bounds.max_y ? "above" : "below"),
              594, 218, 530, 25, 18, muted, "chart_budget_status");
      }
    }
    label("Sample", 74, 583, 78, 27, 20, white);
    const std::string selection = count == 0 ? "No samples" : fmt::format("Sample {} of {} / {:.3f} s", displayed_index + 1, count, series.front().points[displayed_index].x);
    label(selection, 224, 580, 396, 30, 22, white, "chart_selection");
    label(count == 0 ? "No selection" : preview ? "Hover preview" : selected == 0 ? "First sample" : selected + 1 == static_cast<size_t>(count) ? "Last sample" : "Selected sample",
          224, 610, 396, 23, 17, muted, "chart_selection_mode");
    label(count == 0 ? "Update: --" : fmt::format("Update: {:.2f} ms", series.front().points[displayed_index].y),
          646, 580, 220, 30, 22, update_color, "chart_update_value");
    label(series.size() < 2 || count == 0 ? "Render: --" : fmt::format("Render: {:.2f} ms", series[1].points[displayed_index].y),
          888, 580, 236, 30, 22, render_color, "chart_render_value");
    label("Move across the plot to preview values. Tab through controls; Enter activates. Use < and > to select a sample.",
          20, 645, 1104, 27, 19, muted);
    label(dataset == 3 ? "Negative-value test data exercises the axis. These synthetic values are not real frame durations."
                      : fmt::format("{} samples / {} series / 60 samples per second / synthetic values / x: seconds / y: milliseconds",
                                    count, series.size()),
          20, 678, 1104, 27, 18, muted, "chart_metadata");
  }
};

REGISTER_EXAMPLE_SCREEN(chart_lab, "System Demos", "Interactive timing charts", ChartLab)
