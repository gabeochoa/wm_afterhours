#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "../settings.h"
#include "RenderScreenHUD.h"
#include <afterhours/src/plugins/input_prompts.h>
#include <afterhours/src/plugins/ui/profiler.h>

struct ProfilerOverlay : afterhours::System<afterhours::ui::UIContext<InputAction>> {
  afterhours::ui::imm::ProfilerState state;
  std::function<bool()> navigation_open;
  bool visible = false;
  bool show_launcher = false;
  bool browser_was_open = false;
  std::size_t refresh_rate_index = 0;
  static constexpr std::array<int, 4> refresh_rates{0, 120, 60, 30};

  explicit ProfilerOverlay(bool start_recording = false)
      : show_launcher(start_recording) {
    if (start_recording) afterhours::profiling::default_collector().start();
  }

  std::string shortcut() const {
    using namespace afterhours;
    const auto *mapping = EntityHelper::get_singleton_cmp<input::ProvidesInputMapping>();
    const auto *input = EntityHelper::get_singleton_cmp<input::InputCollector>();
    if (!mapping || !input) return {};
    const auto binding = mapping->mapping.find(to_int(InputAction::ToggleProfiler));
    if (binding == mapping->mapping.end()) return {};
    auto prompt = input_prompts::prompt_for(binding->second, input->device_activity.preferred());
    if (!prompt) prompt = input_prompts::prompt_for(binding->second, input_prompts::Device::KeyboardMouse);
    return prompt ? prompt->label : std::string{};
  }

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context, float) override {
    using namespace afterhours;
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    if constexpr (!profiling::available) return;
    if (context.pressed(InputAction::ToggleProfiler)) {
      visible = !visible;
      show_launcher = true;
    }
    const bool browser_open = navigation_open && navigation_open();
    if (visible && !browser_open && !browser_was_open &&
        input::is_key_pressed(keys::ESCAPE)) visible = false;
    browser_was_open = browser_open;
    context.remove_input_gate("wm_profiler");
    if (browser_open) return;
    if (!visible && !show_launcher) return;
    const auto hint = shortcut();
    const auto suffix = hint.empty() ? std::string{} : " (" + hint + ")";
    const float width = static_cast<float>(Settings::get().get_screen_width());
    const float height = static_cast<float>(Settings::get().get_screen_height());
    if (!visible) {
      if (button(context, mk(entity, 7100), ComponentConfig{}
          .with_label("Profiler" + suffix).with_size({pixels(200), pixels(32)})
          .with_absolute_position(width - 424, 8).with_render_layer(1500)
          .with_custom_background({22, 25, 32, 255}).with_custom_text_color({220, 228, 240, 255})
          .with_auto_text_color(false).with_font("AtkinsonMock", 18).with_debug_name("wm_profiler_open"))) visible = true;
      return;
    }
    auto &styling = UIStylingDefaults::get();
    const auto previous_font = styling.default_font_name;
    styling.default_font_name = "AtkinsonMock";
    const float panel_height = std::min(720.f, height - 80.f);
    auto panel = vstack(context, mk(entity, 7101), ComponentConfig{}
        .with_size({pixels(std::min(1120.f, width - 32.f)), pixels(panel_height)})
        .with_absolute_position(16, 48).with_render_layer(1500)
        .with_background(Theme::Usage::Background).with_corner_radius(12)
        .with_padding(Padding::all(pixels(12))).with_gap(pixels(8))
        .with_no_wrap().with_debug_name("wm_profiler_panel"));
    context.add_input_gate("wm_profiler", [&context, root = panel.ent().id](EntityID id) {
      if (context.contains_in_subtree(root, id)) return true;
      auto target = UICollectionHolder::getEntityForID(id);
      return target.valid() && target.asE().has<UIComponent>() &&
             target.asE().get<UIComponent>().render_layer >= 2000;
    });
    auto header = hstack(context, mk(panel.ent(), 0), ComponentConfig{}
        .with_size({percent(1), pixels(36)}).with_no_wrap().with_gap(pixels(12)));
    div(context, mk(header.ent(), 0), ComponentConfig{}
        .with_label("Live profiler / " + ScreenHUDState::current_screen_name)
        .with_size({expand(), pixels(36)}).with_font_size(24));
    const auto refresh_label = refresh_rates[refresh_rate_index] == 0
        ? std::string("Refresh: every frame")
        : fmt::format("Refresh: {} Hz", refresh_rates[refresh_rate_index]);
    if (button(context, mk(header.ent(), 2), ComponentConfig{}
        .with_label(refresh_label).with_size({pixels(240), pixels(32)})
        .with_font_size(18).with_debug_name("wm_profiler_refresh"))) {
      refresh_rate_index = (refresh_rate_index + 1) % refresh_rates.size();
    }
    if (button(context, mk(header.ent(), 1), ComponentConfig{}
        .with_label("Hide" + suffix).with_size({pixels(160), pixels(32)})
        .with_font_size(18).with_debug_name("wm_profiler_hide"))) {
      visible = false;
      context.remove_input_gate("wm_profiler");
    }
    auto &collector = profiling::default_collector();
    div(context, mk(panel.ent(), 1), ComponentConfig{}
        .with_label(collector.recording() ? "Recording across screens" : "Recording stopped")
        .with_size({percent(1), pixels(24)}).with_font_size(18));
    ProfilerOptions options;
    options.refresh_hz = refresh_rates[refresh_rate_index];
    options.font_size = 20;
    options.show_chart = panel_height >= 600;
    options.max_rows = static_cast<std::size_t>(std::clamp(
        (panel_height - (options.show_chart ? 450.f : 260.f)) / 36.f, 1.f, 6.f));
    auto &fonts = *EntityHelper::get_singleton_cmp<FontManager>();
    const auto previous_active_font = fonts.active_font;
    fonts.set_active("AtkinsonMock");
    profiler_panel(context, mk(panel.ent(), 2), collector, state, options,
        ComponentConfig{}.with_size({percent(1), expand()}));
    fonts.set_active(previous_active_font);
    styling.default_font_name = previous_font;
  }
};
