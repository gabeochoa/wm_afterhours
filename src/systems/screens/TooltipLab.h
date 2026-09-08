#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// with_tooltip. Triggers sit in the corners and along the edges on purpose:
// placement goes through the same flipping a dropdown uses, so the interesting
// case is the one with no room below.
struct TooltipLab : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    auto root = div(context, mk(entity),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                        .with_background(Theme::Usage::Background)
                        .with_padding(Padding::all(w1280(16)))
                        .with_debug_name("ttl_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Hover a button. Corner ones flip rather than leaving the screen.")
            .with_size(ComponentSize{percent(1.f), h720(36)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.f))
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{.left = w1280(12)})
            .with_debug_name("ttl_title"));

    struct Spot {
      const char *label;
      const char *tip;
      float x, y; // fractions of the screen
    };
    static const Spot spots[] = {
        {"top left", "Plenty of room below this one", 0.04f, 0.16f},
        {"top right", "Clamped so it stays on screen", 0.80f, 0.16f},
        {"middle", "A longer tooltip, to show the box grows with the text",
         0.38f, 0.46f},
        {"bottom left", "No room below, so this one flips above", 0.04f, 0.90f},
        {"bottom right", "Flipped and clamped at once", 0.80f, 0.90f},
        {"instant", "delay 0, shows the moment you arrive", 0.38f, 0.70f},
    };

    int idx = 0;
    for (const auto &spot : spots) {
      auto cfg = ComponentConfig{}
                     .with_label(spot.label)
                     .with_720p_size(160, 40)
                     .with_absolute_position(spot.x * 1280.f *
                                                 (context.screen_width / 1280.f),
                                             spot.y * context.screen_height)
                     .with_tooltip(spot.tip, idx == 5 ? 0.f : 0.4f)
                     .with_debug_name(fmt::format("ttl_btn_{}", idx));
      button(context, mk(root.ent(), 10 + idx), cfg);
      idx++;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(tooltip_lab, "System Demos",
                        "with_tooltip, including edge flipping", TooltipLab)
