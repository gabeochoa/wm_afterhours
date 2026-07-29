#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::info shown open: single-acknowledge dialog.
struct DialogInfoShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Multiplayer")
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("dialog_bg"));

    afterhours::modal::info(
        context, mk(entity, 1), open, "Connection lost",
        "You were disconnected from the host. Returning to the main menu.",
        "OK");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_info, "Component Galleries",
                        "Info dialog (shown open)", DialogInfoShowcase)
