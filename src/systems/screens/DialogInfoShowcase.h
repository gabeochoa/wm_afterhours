#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

#include "DialogBackdrop.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::info shown open: single-acknowledge dialog.
struct DialogInfoShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw(context, entity, 100, "Harbour Studio", "Multiplayer");

    afterhours::modal::info(
        context, mk(entity, 1), open, "Connection lost",
        "You were disconnected from the host. Returning to the main menu.",
        "OK");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_info, "Component Galleries",
                        "Info dialog (shown open)", DialogInfoShowcase)
