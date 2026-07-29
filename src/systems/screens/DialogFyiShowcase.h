#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::fyi shown open: three-button dialog (e.g. Save / Don't Save / Cancel).
struct DialogFyiShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Editor")
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("dialog_bg"));

    afterhours::modal::fyi(context, mk(entity, 1), open, "Unsaved changes",
                           "You have unsaved changes to this document. What "
                           "would you like to do?",
                           "Save", "Cancel", "Don't Save");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_fyi, "Component Galleries",
                        "Fyi (three-button) dialog (shown open)",
                        DialogFyiShowcase)
