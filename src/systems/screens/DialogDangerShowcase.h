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

// modal::confirm_danger shown open: destructive confirm (Error-red action,
// Cancel as the calm default on the left).
struct DialogDangerShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw(context, entity, 100, "Harbour Studio", "Save slots");

    afterhours::modal::confirm_danger(
        context, mk(entity, 1), open, "Delete save?",
        "This permanently deletes \"Chapter 3 - Autosave\". This action cannot "
        "be undone.",
        "Delete", "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_danger, "Component Galleries",
                        "Danger confirm dialog (shown open)",
                        DialogDangerShowcase)
