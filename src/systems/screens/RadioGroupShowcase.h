#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct RadioGroupShowcase : ScreenSystem<UIContext<InputAction>> {
  size_t fruit_idx = 1;

  std::array<std::string_view, 3> fruit_options = {"apple", "orange", "watermelon"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Light theme to match reference
    Theme theme;
    theme.background = afterhours::Color{245, 245, 245, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{100, 100, 100, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{158, 158, 158, 255};
    context.theme = theme;

    // Main container with column layout
    auto container = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(220), pixels(140)})
            .with_custom_background(theme.surface)
            .with_border(theme.font_muted, 1.0f)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.08f)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("container"));

    // Radio group - rows at 36px height each
    radio_group(context, mk(container.ent(), 0), fruit_options, fruit_idx,
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(36)})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("radios"));
  }
};

REGISTER_EXAMPLE_SCREEN(radio_buttons, "Component Galleries",
                        "Radio button group demo",
                        RadioGroupShowcase)
