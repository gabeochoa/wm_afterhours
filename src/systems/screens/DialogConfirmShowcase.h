#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

#include "DialogBackdrop.h"
#include "DialogPresentation.h"
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Renders modal::confirm OPEN by default so `make screenshots` captures the
// dialog for visual review + baseline regression. Long message exercises wrap.
struct DialogConfirmShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;
  bool preview = false;
  bool applied = false;
  float remaining = 15;
  std::string status = "Two pending display changes";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.surface = {30, 40, 57, 255};
    context.theme.primary = {44, 93, 155, 255};
    context.scaling_mode = ScalingMode::Proportional;
    if (preview) {
      remaining = std::max(0.f, remaining - dt);
      if (remaining == 0) {
        preview = false;
        status = "Preview expired. Previous display settings restored.";
      }
    }
    const bool changed = preview || applied;
    dialog_backdrop::draw_context(context, entity, "Display",
        {{"Resolution", changed ? "2560 x 1440 / active" : "1920 x 1080 -> 2560 x 1440 / pending", !applied},
         {"Refresh rate", changed ? "120 Hz / active" : "60 Hz -> 120 Hz / pending", !applied},
         {"Window mode", "Borderless"}, {"Vertical sync", "On"},
         {"Render scale", "100%"}, {"Colour profile", "sRGB"}}, status);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    if (button(context, mk(entity, 20), ComponentConfig{}
        .with_size({pixels(270 * s), pixels(44 * s)}).with_absolute_position(942 * s, 92 * s)
        .with_label(applied ? "Reset display demo" : "Review display changes").with_font("AtkinsonMock", pixels(21 * s))
        .with_debug_name("dc_review"))) {
      applied = false;
      open = true;
      status = "Two pending display changes";
    }
    const bool confirming = open;
    auto confirmation = afterhours::modal::confirm(
        context, mk(entity, 1), open, "Apply display changes?",
        "Resolution: 1920 x 1080  ->  2560 x 1440\n"
        "Refresh rate: 60 Hz  ->  120 Hz\n"
        "The display may go dark briefly. Confirm within 15 seconds,\n"
        "or the previous settings return. You can change them in Display.\n"
        "Demo preview only; your actual display stays unchanged.",
        "Apply display settings", "Keep current settings");
    constexpr std::array<float, 2> widths{282, 282};
    if (confirmation) dialog_presentation::style(context, confirmation.ent(), 760, 144, widths);
    if (confirming && confirmation.confirmed()) {
      preview = true;
      remaining = 15;
      status = "Previewing 2560 x 1440 at 120 Hz";
    }
    if (confirming && (confirmation.cancelled() || confirmation.dismissed())) status = "Kept 1920 x 1080 at 60 Hz";
    const bool previewing = preview;
    auto keep = afterhours::modal::confirm(
        context, mk(entity, 2), preview, "Keep these display settings?",
        fmt::format("Preview: 2560 x 1440 at 120 Hz\nPrevious settings return in {} seconds.\n"
                    "Choose Keep to finish, or Revert to restore them now.", static_cast<int>(std::ceil(remaining))),
        "Keep display settings", "Revert now");
    if (keep) dialog_presentation::style(context, keep.ent(), 760, 104, widths);
    if (previewing && keep.confirmed()) {
      applied = true;
      status = "Applied 2560 x 1440 at 120 Hz";
    }
    if (previewing && (keep.cancelled() || keep.dismissed())) status = "Reverted to 1920 x 1080 at 60 Hz";
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_confirm, "Component Galleries",
                        "Confirm dialog (shown open)", DialogConfirmShowcase)
