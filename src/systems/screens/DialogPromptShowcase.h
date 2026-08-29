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

// modal::prompt shown open: message + text input + OK/Cancel.
struct DialogPromptShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;
  std::string value = "untitled_map";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw(context, entity, 100, "Harbour Studio", "Files");

    afterhours::modal::prompt(context, mk(entity, 1), open, value, "Rename file",
                              "Enter a new name for this file:", "Rename",
                              "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_prompt, "Component Galleries",
                        "Prompt (input) dialog (shown open)",
                        DialogPromptShowcase)
