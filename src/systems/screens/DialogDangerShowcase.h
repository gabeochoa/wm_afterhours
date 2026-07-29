#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

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

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Save slots")
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("dialog_bg"));

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
