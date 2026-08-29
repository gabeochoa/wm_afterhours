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

// modal::fyi shown open: three-button dialog (e.g. Save / Don't Save / Cancel).
struct DialogFyiShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw(context, entity, 100, "Harbour Studio", "Editor");

    afterhours::modal::fyi(context, mk(entity, 1), open, "Unsaved changes",
                           "You have unsaved changes to this document. What "
                           "would you like to do?",
                           "Save", "Cancel", "Don't Save");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_fyi, "Component Galleries",
                        "Fyi (three-button) dialog (shown open)",
                        DialogFyiShowcase)
