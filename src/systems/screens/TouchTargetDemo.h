#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/ui/validation_systems.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TouchTargetDemo : ScreenSystem<UIContext<InputAction>> {
  bool enabled = true;
  bool enlarged = false;
  int minimum = 44;
  int clicks = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const auto muted = afterhours::Color{176, 190, 210, 255};
    const float width = std::min(context.screen_width - 64.f, 1120.f);
    const float card_width = (width - 24.f) / 2.f;
    auto base = ComponentConfig{}.with_font("AtkinsonMock", pixels(20))
        .with_custom_text_color(theme.font).with_skip_grid_snap();
    auto root = div(context, mk(entity), ComponentConfig{base}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0));
    auto content = div(context, mk(root.ent(), 0), ComponentConfig{base}
        .with_size({pixels(width), pixels(660)})
        .with_absolute_position((context.screen_width - width) / 2, 24)
        .with_background(Theme::Usage::None));
    auto label = [&](afterhours::Entity &parent, int id, const std::string &text,
                     float x, float y, float w, const std::string &name = "") {
      return div(context, mk(parent, id), ComponentConfig{base}
          .with_size({pixels(w), pixels(28)}).with_absolute_position(x, y)
          .with_label(text).with_background(Theme::Usage::None)
          .with_custom_text_color(muted).with_debug_name(name));
    };
    div(context, mk(content.ent(), 0), ComponentConfig{base}
        .with_size({pixels(width), pixels(44)}).with_label("Touch targets")
        .with_font("AtkinsonMock", pixels(32)).with_background(Theme::Usage::None));
    label(content.ent(), 1, "Red outlines mark clickable areas below the minimum size.",
          0, 48, width);
    auto control = [&](int id, float x, float w, const std::string &text,
                       const std::string &name) {
      return button(context, mk(content.ent(), id), ComponentConfig{base}
          .with_size({pixels(w), pixels(48)}).with_absolute_position(x, 96)
          .with_label(text).with_custom_background(theme.primary)
          .with_alignment(TextAlignment::Center).with_corner_radius(8)
          .with_debug_name(name));
    };
    if (control(2, 0, 224, enabled ? "Validation: on" : "Validation: off", "touch_enabled"))
      enabled = !enabled;
    if (control(3, 240, 224, std::format("Minimum: {} px", minimum), "touch_minimum"))
      minimum = minimum == 44 ? 48 : minimum == 48 ? 32 : 44;
    if (control(4, 480, 240, enlarged ? "Restore 32 px" : "Enlarge to 48 px", "touch_resize"))
      enlarged = !enlarged;

    auto &validation = UIStylingDefaults::get().get_validation_config_mut();
    validation = {};
    validation.mode = ValidationMode::Warn;
    validation.enforce_min_touch_target = enabled;
    validation.min_touch_target_size = static_cast<float>(minimum);
    validation.highlight_violations = true;

    const std::array<std::string, 4> titles{
        "Compact button", "44 px button", "Small icon, larger target", "Disabled button"};
    const std::array<std::string, 4> descriptions{
        "Enlarge the target to clear its warning.", "Meets the default 44 px minimum.",
        "The clickable area is 48 px, the icon 20 px.", "Disabled controls are excluded."};
    for (int i = 0; i < 4; ++i) {
      auto card = div(context, mk(content.ent(), 10 + i), ComponentConfig{base}
          .with_size({pixels(card_width), pixels(200)})
          .with_absolute_position((i % 2) * (card_width + 24), 176.f + (i / 2) * 224.f)
          .with_custom_background(theme.surface).with_corner_radius(12));
      label(card.ent(), 0, titles[i], 24, 16, card_width - 48);
      label(card.ent(), 1, descriptions[i], 24, 52, card_width - 48);
      const float size = i == 0 ? (enlarged ? 48.f : 32.f) : i == 1 ? 44.f : i == 2 ? 48.f : 28.f;
      auto sample = button(context, mk(card.ent(), 2), ComponentConfig{base}
          .with_size({pixels(size), pixels(size)}).with_absolute_position(24, 112)
          .with_padding(Padding::all(pixels(0)))
          .with_label(i == 2 ? "" : "A").with_disabled(i == 3)
          .with_custom_background(theme.primary).with_alignment(TextAlignment::Center)
          .with_corner_radius(8).with_debug_name(std::format("touch_sample_{}", i)));
      if (sample) ++clicks;
      if (i == 2)
        div(context, mk(sample.ent(), 0), ComponentConfig{base}
            .with_size({pixels(20), pixels(20)}).with_absolute_position(14, 14)
            .with_custom_background(theme.font).with_corner_radius(4)
            .with_ignore_pointer_events());
      const bool failed = sample.ent().has<ValidationViolation>();
      const auto status = !enabled ? "Validation off" : i == 3 ? "Skipped" : failed ? "Too small" : "Pass";
      label(card.ent(), 3, std::format("{} x {} px", static_cast<int>(size), static_cast<int>(size)),
            96, 108, 160);
      label(card.ent(), 4, status, 96, 140, 160, std::format("touch_status_{}", i));
    }
    label(content.ent(), 20, std::format("Button presses: {}", clicks), 0, 624, width, "touch_clicks");
  }
};

REGISTER_EXAMPLE_SCREEN(touch_targets, "Tools", "Minimum clickable size validation", TouchTargetDemo)
