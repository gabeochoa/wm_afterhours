#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/ui/profiler.h>
#include <array>

struct SystemProfileLab : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  afterhours::ui::imm::ProfilerState state;
  bool load = false;
  bool panel_visible = true;
  double result = 0;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const afterhours::Color ink{237, 244, 253, 255}, muted{180, 197, 220, 255};
    const afterhours::Color panel{25, 37, 56, 255}, field{40, 57, 80, 255};
    context.theme = theme_presets::neon_dark();
    context.theme.font = ink;
    context.theme.font_muted = muted;
    context.theme.secondary = {56, 74, 96, 255};
    context.theme.surface = panel;
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto &collector = afterhours::profiling::default_collector();
    if (load) {
      for (int i = 0; i < 40000; ++i) result = std::sin(result + i * .0001);
    }
    AFTERHOURS_PROFILE_COUNTER(collector, "Demo load", "iterations", load ? 40000 : 0);
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({13, 22, 36, 255}).with_corner_radius(0).with_debug_name("spl_root"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                           afterhours::Color color, const std::string &name = "", bool center = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(center ? TextAlignment::Center : TextAlignment::Left)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    label(0, 0, 20, 1144, 44, "System profiler", 35, ink);
    label(1, 0, 74, 184, 28, "Demo workload", 22, muted);
    const auto action = [&](int id, float x, float w, const std::string &text, const std::string &name) {
      return button(context, mk(root.ent(), id), box(x, 68, w, 42).with_label(text)
          .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_text_color(ink)
          .with_custom_background(field).with_corner_radius(6 * scale).with_debug_name(name));
    };
    if (action(2, 202, 312, load ? "Synthetic workload: On" : "Add synthetic workload", "spl_load")) load = !load;
    if (action(3, 530, 258, panel_visible ? "Hide profiler panel" : "Show profiler panel", "spl_visibility")) panel_visible = !panel_visible;
    label(4, 810, 74, 334, 29, load ? "40,000 sine iterations / frame" : "Synthetic workload: Off", 19, muted, "spl_workload");
    if (!panel_visible) {
      label(5, 0, 174, 1144, 62, collector.recording() ? "Panel hidden. Recording continues in the background." : "Panel hidden. Recording is stopped.", 27, ink, "spl_state");
      return;
    }
#if !AFTERHOURS_ENABLE_PROFILING
    label(5, 0, 174, 1144, 50, "Profiling compiled out", 28, ink, "spl_state");
    label(6, 0, 234, 1144, 50, "Build with AFTERHOURS_ENABLE_PROFILING=1 to collect timings.", 23, muted);
    return;
#else
    div(context, mk(root.ent(), 20), box(0, 496, 1144, 212).with_custom_background(panel).with_corner_radius(8 * scale));
    auto &fonts = *afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto previous_font = fonts.active_font;
    fonts.set_active("AtkinsonMock");
    ProfilerOptions options;
    options.font_size = 18 * scale;
    options.max_rows = 5;
    options.refresh_hz = 0;
    auto native_pair = mk(root.ent(), 21);
    profiler_panel(context, native_pair, collector, state, options, box(0, 126, 1144, 582));
    fonts.set_active(previous_font);
    auto &native = deref(native_pair).first;
    const auto place = [scale](afterhours::Entity &node, float x, float y, float w, float h) {
      auto &cmp = node.get<UIComponent>();
      cmp.absolute = true;
      cmp.absolute_pos_x = x * scale;
      cmp.absolute_pos_y = y * scale;
      cmp.desired[Axis::X] = pixels(w * scale);
      cmp.desired[Axis::Y] = pixels(h * scale);
      for (auto edge : {Axis::left, Axis::right, Axis::top, Axis::bottom}) cmp.desired_margin[edge] = pixels(0);
    };
    const auto style_text = [&](afterhours::Entity &node, float size, afterhours::Color color) {
      node.get<UIComponent>().enable_font("AtkinsonMock", pixels(size * scale), true);
      if (!node.has<HasLabel>()) return;
      auto &text = node.get<HasLabel>();
      text.font_name = "AtkinsonMock";
      text.explicit_text_color = color;
      text.text_inset = afterhours::Vector2Type{0, 0};
      text.text_x_offset = 8 * scale;
    };
    auto &controls = UICollectionHolder::getEntityForIDEnforce(native.get<UIComponent>().children[0]);
    auto &view = UICollectionHolder::getEntityForIDEnforce(native.get<UIComponent>().children[1]);
    place(controls, 0, 0, 1144, 44);
    place(view, 0, 60, 1144, 522);
    const std::array<float, 4> control_x{0, 336, 508, 696};
    const std::array<float, 4> control_w{200, 160, 176, 448};
    size_t control_index = 0;
    for (const auto child_id : controls.get<UIComponent>().children) {
      auto &control = UICollectionHolder::getEntityForIDEnforce(child_id);
      const size_t index = control_index++;
      if (index >= control_x.size()) continue;
      place(control, control_x[index], 0, control_w[index], 44);
      style_text(control, 20, ink);
      control.get<HasLabel>().text_x_offset = 0;
      control.addComponentIfMissing<afterhours::HasColor>(field).set(index == 0 ? afterhours::Color{27, 108, 161, 255} : field);
      if (index == 1 && state.snapshot.frame_ms.empty() && !state.paused) {
        control.removeComponentIfExists<HasClickListener>();
        control.get<HasLabel>().is_disabled = true;
      }
      if (index == 2) control.get<HasLabel>().label = "Clear samples";
      if (index == 3) control.get<HasLabel>().label = state.sort_by_overall ? "Sort: overall average, descending" : "Sort: recent average, descending";
    }
    const auto &snapshot = state.snapshot;
    const bool sampled = !snapshot.frame_ms.empty();
    div(context, mk(root.ent(), 22), box(212, 126, 112, 44).with_custom_background(collector.recording() ? afterhours::Color{28, 85, 64, 255} : field)
        .with_corner_radius(6 * scale));
    label(23, 216, 133, 104, 29, collector.recording() ? "Recording" : "Stopped", 18, ink, "spl_state", true);
    const std::array<std::string, 7> names{"FPS", "Avg / frame", "p50 / ms", "p95 / ms", "p99 / ms", "Process CPU", "Resident memory"};
    const std::array<std::string, 7> values{
        sampled ? fmt::format("{:.1f}", snapshot.fps) : "No data",
        sampled ? fmt::format("{:.2f} ms", snapshot.average_frame_ms) : "No data",
        sampled ? fmt::format("{:.2f} ms", snapshot.p50_ms) : "No data",
        sampled ? fmt::format("{:.2f} ms", snapshot.p95_ms) : "No data",
        sampled ? fmt::format("{:.2f} ms", snapshot.p99_ms) : "No data",
        snapshot.cpu_percent ? fmt::format("{:.1f}%", *snapshot.cpu_percent) : "Unavailable",
        snapshot.resident_mb ? fmt::format("{:.1f} MiB", *snapshot.resident_mb) : "Unavailable"};
    for (size_t i = 0; i < names.size(); ++i) {
      const float x = static_cast<float>(i) * 165;
      div(context, mk(root.ent(), 30 + static_cast<int>(i)), box(x, 186, 154, 77).with_custom_background(panel).with_corner_radius(7 * scale));
      label(40 + static_cast<int>(i), x + 7, 190, 140, 28, names[i], 16, muted);
      label(50 + static_cast<int>(i), x + 7, 222, 140, 32, values[i], 22, ink, "spl_metric_" + std::to_string(i));
    }
    label(60, 0, 316, 790, 28, "Frame time history / ms", 23, ink);
    label(61, 890, 317, 254, 26, "Frame number / increasing", 18, muted);
    size_t table_row = 0;
    for (size_t i = 0; i < view.get<UIComponent>().children.size(); ++i) {
      auto &child = UICollectionHolder::getEntityForIDEnforce(view.get<UIComponent>().children[i]);
      if (i == 0) {
        place(child, 0, 81, 1144, 24);
        child.get<HasLabel>().label = fmt::format("Sample window: last {} frames / {} recorded since clear / View {}", snapshot.frame_ms.size(), snapshot.frames, state.paused ? "paused" : "live");
        style_text(child, 19, muted);
        continue;
      }
      if (i == 1) {
        place(child, 0, 108, 1144, 20);
#ifdef __APPLE__
        child.get<HasLabel>().label = "macOS process samples every 0.5s; CPU requires two samples. Pause view freezes display, not recording.";
#else
        child.get<HasLabel>().label = "Process CPU and memory sampling currently supports macOS. Pause view freezes display, not recording.";
#endif
        style_text(child, 17, muted);
        continue;
      }
      if (i == 2) {
        place(child, 0, 164, 1144, 142);
        if (!sampled) child.get<HasOnDraw>().fg = {};
        continue;
      }
      if (child.has<HasLabel>()) {
        const bool empty_table = child.get<HasLabel>().label == "No system samples yet";
        place(child, empty_table ? 20 : 0, empty_table ? 382 : 498, empty_table ? 1104 : 1144, empty_table ? 60 : 24);
        if (empty_table) child.get<HasLabel>().label = "System timings appear after recording starts. Each row identifies one system and its update, render, or fixed phase.";
        child.get<HasLabel>().text_overflow = TextOverflow::Wrap;
        style_text(child, empty_table ? 22 : 18, muted);
        continue;
      }
      const bool header = table_row == 0;
      place(child, 0, header ? 316 : 348 + static_cast<float>(table_row - 1) * 30, 1144, header ? 30 : 28);
      const std::array<std::string, 4> headings{"System name (phase)", state.sort_by_overall ? "Recent avg / ms" : "Recent avg / ms v", state.sort_by_overall ? "Overall avg / ms v" : "Overall avg / ms", "Last frame / ms"};
      size_t column = 0;
      for (const auto cell_id : child.get<UIComponent>().children) {
        auto &cell = UICollectionHolder::getEntityForIDEnforce(cell_id);
        cell.get<UIComponent>().desired[Axis::Y] = pixels((header ? 30 : 28) * scale);
        style_text(cell, 18, header ? ink : muted);
        cell.get<HasLabel>().text_overflow = TextOverflow::Ellipsis;
        if (header && column < headings.size()) cell.get<HasLabel>().label = headings[column];
        ++column;
      }
      ++table_row;
    }
    if (!sampled) label(62, 128, 383, 888, 60, "Start recording to collect frame timings.", 27, ink, "spl_chart_empty", true);
#endif
  }
};

REGISTER_EXAMPLE_SCREEN(system_profile_lab, "System Demos", "Frame and system profiling", SystemProfileLab)
