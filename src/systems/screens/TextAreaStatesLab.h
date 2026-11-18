#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// text_area in its three states (#158). The editable area takes typing; the
// readonly one focuses and selects but never edits; the disabled one takes
// neither focus nor edits and draws dimmed by the theme's disabled_opacity.
struct TextAreaStatesLab : ScreenSystem<UIContext<InputAction>> {
  std::string editable = "edit me";
  std::string readonly_text = "read only, select me";
  std::string disabled_text = "disabled, no focus";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{167, 182, 206, 255};
    const auto panel = afterhours::Color{29, 38, 56, 255};
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background).with_corner_radius(0)
        .with_debug_name("tas_root"));
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float size, afterhours::Color color,
                          const std::string &name) {
      div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(560 * s), pixels(26 * s)})
          .with_absolute_position(x * s, y * s).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    const auto area = [&](int id, std::string &value, float y,
                          ComponentConfig config, const std::string &name) {
      text_area(context, mk(root.ent(), id), value,
                ComponentConfig{config}
                    .with_size({pixels(560 * s), pixels(72 * s)})
                    .with_absolute_position(48 * s, y * s)
                    .with_font("AtkinsonMock", pixels(18 * s))
                    .with_custom_text_color(white)
                    .with_custom_background(panel)
                    .with_word_wrap(true)
                    .with_debug_name(name));
    };
    text(0, "Text area states", 48, 32, 26, white, "tas_title");
    text(1, "Editable", 48, 84, 19, muted, "");
    area(2, editable, 116, ComponentConfig{}, "tas_editable");
    text(3, "Readonly: focus and select work, typing does not", 48, 208, 19,
         muted, "");
    area(4, readonly_text, 240, ComponentConfig{}.with_readonly(),
         "tas_readonly");
    text(5, "Disabled: no focus, no edits, dimmed by theme.disabled_opacity",
         48, 332, 19, muted, "");
    area(6, disabled_text, 364, ComponentConfig{}.with_disabled(true),
         "tas_disabled");
  }
};

REGISTER_EXAMPLE_SCREEN(text_area_states_lab, "System Demos",
                        "text_area: editable, readonly and disabled states",
                        TextAreaStatesLab)
