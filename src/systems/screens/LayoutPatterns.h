#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Minimal fixed-size layout for agent E2E assert_ui checks.
struct LayoutPatterns : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    auto root =
        vstack(context, mk(entity, 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(afterhours::Color{30, 34, 44, 255})
                   .with_padding(Padding{.top = pixels(40),
                                         .left = pixels(40),
                                         .bottom = pixels(40),
                                         .right = pixels(40)})
                   .with_debug_name("pattern_root"));

    // 416 inner width fits three 120px boxes + 12px gaps + 16px padding/side.
    auto row = hstack(context, mk(root.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(416), pixels(100)})
                          .with_custom_background(afterhours::Color{50, 60, 80, 255})
                          .with_padding(Padding::all(pixels(16)))
                          .with_gap(pixels(12))
                          .with_align_items(AlignItems::Center)
                          .with_debug_name("pattern_row"));

    const afterhours::Color box_colors[] = {
        {0, 180, 220, 255}, {220, 60, 120, 255}, {50, 210, 130, 255}};

    for (int i = 0; i < 3; ++i) {
      div(context, mk(row.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(120), pixels(80)})
              .with_custom_background(box_colors[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(24.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("pattern_box_{}", i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(layout_patterns, "Tools",
                        "Minimal layout patterns for agent E2E",
                        LayoutPatterns)
