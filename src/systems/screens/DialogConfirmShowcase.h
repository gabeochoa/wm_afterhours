#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

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

    // Base content so the modal backdrop dim is visible over something.
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("dialog_bg"));

    afterhours::modal::confirm(
        context, mk(entity, 1), open, "Apply changes?",
        "This will apply your new display settings and restart the renderer. "
        "You can revert from the settings menu afterwards.",
        "Apply", "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_confirm, "Component Galleries",
                        "Confirm dialog (shown open)", DialogConfirmShowcase)
