#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <array>

#include "DialogBackdrop.h"
#include "DialogPresentation.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::info shown open: single-acknowledge dialog.
struct DialogInfoShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;
  bool main_menu = false;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.surface = {30, 40, 57, 255};
    context.theme.primary = {44, 93, 155, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f,
                                 context.screen_height / 720.f);

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    if (main_menu) {
      dialog_backdrop::draw_context(
          context, entity, "Main menu",
          {{"Multiplayer", "No active session", true},
           {"Last demo lobby", "North pier co-op / host Alex"},
           {"Match state", "Never started"},
           {"Match progress", "None to recover"},
           {"Saved games", "None in this demo"}},
          "Returned to the main menu. Open the demo lobby to view it again.");
    } else {
      dialog_backdrop::draw_context(
          context, entity, "Multiplayer",
          {{"North pier co-op", "Host Alex / disconnected", true},
           {"Session mode", "Co-op lobby"},
           {"Match state", "Not started"},
           {"Match progress", "None"},
           {"Saved games", "None in this demo"},
           {"Connection", "Offline demo / reconnect unavailable"}},
          "The disconnected lobby remains here until you return to the menu.");
    }
    if (button(context, mk(entity, 20),
               ComponentConfig{}
                   .with_size({pixels(270 * scale), pixels(44 * scale)})
                   .with_absolute_position(942 * scale, 92 * scale)
                   .with_label(main_menu ? "Open demo lobby" : "View connection notice")
                   .with_font("AtkinsonMock", pixels(21 * scale))
                   .with_debug_name("di_open_lobby"))) {
      main_menu = false;
      open = true;
    }

    const bool was_open = open;
    auto notice = afterhours::modal::info(
        context, mk(entity, 1), open, "Connection lost",
        "Demo lobby: North pier co-op / host Alex\n"
        "This session cannot continue while disconnected.\n"
        "No match was started, so there is no progress to lose.\n"
        "Choose Return to main menu below, or press Esc, to leave.\n"
        "Reconnecting is not supported in this offline demo.",
        "Return to main menu");
    if (was_open && !open) main_menu = true;
    if (!notice) return;

    constexpr float width = 760;
    constexpr float body_height = 144;
    constexpr std::array<float, 1> action_widths{width - 48};
    dialog_presentation::style(context, notice.ent(), width, body_height,
                               action_widths);
    for (const auto child_id : notice.cmp().children) {
      auto child = UICollectionHolder::getEntityForID(child_id);
      if (!child.valid() || !child.asE().has<UIComponentDebug>() ||
          child.asE().get<UIComponentDebug>().name() != "dialog_buttons") continue;
      for (const auto button_id : child.asE().get<UIComponent>().children) {
        auto action = UICollectionHolder::getEntityForID(button_id);
        if (!action.valid()) continue;
        action.asE().addComponentIfMissing<UIComponentDebug>("di_return").set("di_return");
      }
    }
    div(context, mk(notice.ent(), 30),
        ComponentConfig{}
            .with_size({pixels(30 * scale), pixels(30 * scale)})
            .with_absolute_position((width - 83) * scale, 5 * scale)
            .with_background(Theme::Usage::None)
            .with_ignore_pointer_events()
            .with_render_layer(1001)
            .with_on_draw_fg([scale](RectangleType rect) {
              const auto point = [=](float x, float y) {
                return raylib::Vector2{rect.x + x * scale, rect.y + y * scale};
              };
              const raylib::Color cable{172, 190, 212, 255};
              const raylib::Color disconnected{244, 181, 120, 255};
              raylib::DrawLineEx(point(0, 15), point(9, 15), 3 * scale, cable);
              raylib::DrawLineEx(point(21, 15), point(30, 15), 3 * scale, cable);
              raylib::DrawLineEx(point(9, 9), point(9, 21), 3 * scale, cable);
              raylib::DrawLineEx(point(21, 9), point(21, 21), 3 * scale, cable);
              raylib::DrawLineEx(point(9, 11), point(13, 11), 2 * scale, cable);
              raylib::DrawLineEx(point(17, 19), point(21, 19), 2 * scale, cable);
              raylib::DrawLineEx(point(21, 3), point(9, 27), 3 * scale,
                                 disconnected);
            })
            .with_debug_name("di_disconnected_icon"));
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_info, "Component Galleries",
                        "Info dialog (shown open)", DialogInfoShowcase)
