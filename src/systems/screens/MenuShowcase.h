#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Anchored menus: dropdown_menu under a trigger, context_menu at a point, and
// the edge-flip that both share. The two bottom triggers are the interesting
// ones -- there is no room below them, so their menus open upward.
struct MenuShowcase : ScreenSystem<UIContext<InputAction>> {
  // Open by default: the interesting state is the open menu, and a screenshot
  // baseline cannot click.
  bool file_open = true;
  bool edit_open = true;
  bool bottom_open = true;
  bool corner_open = true;
  std::string status = "click a menu";

  static std::vector<MenuItem> file_items() {
    return {
        MenuItem{"New", "Cmd+N", false, false},
        MenuItem{"Open...", "Cmd+O", false, false},
        MenuItem{"Save", "Cmd+S", false, false},
        MenuItem::sep(),
        MenuItem{"Revert", "", false, true}, // disabled
    };
  }

  static std::vector<MenuItem> edit_items() {
    return {
        MenuItem{"Undo", "Cmd+Z", false, false},
        MenuItem{"Redo", "Shift+Cmd+Z", false, false},
        MenuItem::sep(),
        MenuItem{"Cut", "Cmd+X", false, false},
        MenuItem{"Copy", "Cmd+C", false, false},
        MenuItem{"Paste", "Cmd+V", false, false},
    };
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const float sw = (float)Settings::get().get_screen_width();
    const float sh = (float)Settings::get().get_screen_height();
    const auto white = afterhours::Color{255, 255, 255, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(sw * 0.5f), pixels(40)})
            .with_absolute_position(24.f, 16.f)
            .with_label("Anchored menus - the bottom two open upward")
            .with_custom_text_color(white)
            .with_debug_name("menu_showcase_title"));

    // Menu bar along the top: plenty of room, so these open downward.
    if (dropdown_menu(context, mk(entity, 1), "File", file_items(), file_open,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(150), pixels(32)})
                          .with_absolute_position(24.f, 64.f)) >= 0)
      status = "File item chosen";

    if (dropdown_menu(context, mk(entity, 2), "Edit", edit_items(), edit_open,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(150), pixels(32)})
                          .with_absolute_position(190.f, 64.f)) >= 0)
      status = "Edit item chosen";

    // Near the bottom edge: overlay::place flips these above their triggers.
    if (dropdown_menu(context, mk(entity, 3), "Bottom", file_items(),
                      bottom_open,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(150), pixels(32)})
                          .with_absolute_position(24.f, sh - 60.f)) >= 0)
      status = "Bottom item chosen";

    // Bottom-right: flips vertically AND clamps horizontally.
    if (dropdown_menu(context, mk(entity, 4), "Corner", edit_items(),
                      corner_open,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(150), pixels(32)})
                          .with_absolute_position(sw - 170.f, sh - 60.f)) >= 0)
      status = "Corner item chosen";

    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(360), pixels(32)})
            .with_absolute_position(24.f, sh * 0.5f)
            .with_label(status)
            .with_custom_text_color(white)
            .with_debug_name("menu_showcase_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(menu_showcase, "System Demos",
                        "Anchored dropdown menus with edge flipping",
                        MenuShowcase)
