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

// Renders modal::confirm OPEN by default so `make screenshots` captures the
// dialog for visual review + baseline regression. Long message exercises wrap.
struct DialogConfirmShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw(context, entity, 100, "Harbour Studio", "Display");

    afterhours::modal::confirm(
        context, mk(entity, 1), open, "Apply changes?",
        "This will apply your new display settings and restart the renderer. "
        "You can revert from the settings menu afterwards.",
        "Apply", "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_confirm, "Component Galleries",
                        "Confirm dialog (shown open)", DialogConfirmShowcase)
