#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// context_menu() at a point, which MenuShowcase claimed to cover but never
// called. floatinghotel wants this for right-click on files and commits.
struct ContextMenuLab : ScreenSystem<UIContext<InputAction>> {
  // Open by default: a screenshot baseline cannot right-click, and a closed
  // menu is an empty screen. Only the first -- menus dismiss on focus loss, so
  // two open at once is a frame no interactive run reaches.
  bool mid_open = true;
  bool corner_open = false;
  std::string status = "menu anchored at a fixed point";

  static std::vector<MenuItem> file_items() {
    return {
        MenuItem{"Stage file", "", false, false},
        MenuItem{"Unstage", "", false, false},
        MenuItem::sep(),
        MenuItem{"Copy path", "Cmd+C", false, false},
        MenuItem{"Discard changes", "", false, true}, // disabled
    };
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const float sw = (float)Settings::get().get_screen_width();
    const float sh = (float)Settings::get().get_screen_height();
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label("Context Menu - anchored at a point, not to a widget")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("cm_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 50.f)
            .with_label(status)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("cm_status"));

    // Buttons stand in for the right-click, so the screen is drivable from a
    // keyboard-only e2e script as well as a mouse.
    if (button(context, mk(entity, 2),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(200), pixels(34)})
                   .with_absolute_position(48.f, 88.f)
                   .with_label("open at centre")
                   .with_debug_name("cm_open_mid")))
      mid_open = true;

    if (button(context, mk(entity, 3),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(200), pixels(34)})
                   .with_absolute_position(264.f, 88.f)
                   .with_label("open near corner")
                   .with_debug_name("cm_open_corner")))
      corner_open = true;

    const Vector2Type mid{sw * 0.30f, sh * 0.34f};
    int picked = context_menu(context, mk(entity, 4), file_items(), mid,
                              mid_open,
                              ComponentConfig{}
                                  .with_size(ComponentSize{pixels(230),
                                                           pixels(32)})
                                  .with_debug_name("cm_mid"));
    if (picked != kNoMenuSelection)
      status = fmt::format("centre menu chose #{}", picked);

    // Near the bottom-right so there is no room below: the menu has to flip up
    // and back, which is the whole reason placement is shared with popover.
    const Vector2Type corner{sw - 90.f, sh - 60.f};
    picked = context_menu(context, mk(entity, 5), file_items(), corner,
                          corner_open,
                          ComponentConfig{}
                              .with_size(ComponentSize{pixels(230), pixels(32)})
                              .with_debug_name("cm_corner"));
    if (picked != kNoMenuSelection)
      status = fmt::format("corner menu chose #{}", picked);

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, sh - 96.f)
            .with_label("the corner menu has no room below it and flips up")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("cm_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(context_menu_lab, "System Demos",
                        "context_menu() at a point, including the edge flip",
                        ContextMenuLab)
