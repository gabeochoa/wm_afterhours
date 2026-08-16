#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// popover(): a menu holds items, a popover holds whatever the caller builds,
// and it survives focus landing on one of those controls. floatinghotel wants
// it for the commit amend/fixup panel and the branch selector.
struct PopoverLab : ScreenSystem<UIContext<InputAction>> {
  // Open by default so a baseline captures the panel; see ContextMenuLab.
  bool below_open = true;
  bool flip_open = false;
  bool amend = false;
  std::string status = "popover holds arbitrary controls";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const float sh = (float)Settings::get().get_screen_height();
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto panel = afterhours::Color{34, 41, 60, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label("Popover - anchored panel with real controls inside")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("pv_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 50.f)
            .with_label(status)
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("pv_status"));

    auto trigger = button(context, mk(entity, 2),
                          ComponentConfig{}
                              .with_size(ComponentSize{pixels(200), pixels(36)})
                              .with_absolute_position(48.f, 96.f)
                              .with_label("Commit options")
                              .with_debug_name("pv_trigger"));
    if (trigger)
      below_open = !below_open;

    const RectangleType anchor = trigger.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(entity, 3), anchor, below_open,
                           overlay::Placement::Below,
                           ComponentConfig{}
                               .with_size(ComponentSize{pixels(260),
                                                        pixels(150)})
                               .with_custom_background(panel)
                               .with_debug_name("pv_panel"))) {
      div(context, mk(pop.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), pixels(28)})
              .with_label("Amend the previous commit?")
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_transparent_bg()
              .with_debug_name("pv_caption"));

      // A checkbox inside is the point: focusing it must NOT dismiss the panel,
      // which is what separates popover from menu_list.
      checkbox(context, mk(pop.ent(), 1), amend,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(34)})
                   .with_label("amend")
                   .with_debug_name("pv_amend"));

      if (button(context, mk(pop.ent(), 2),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(34)})
                     .with_label("Commit")
                     .with_debug_name("pv_commit"))) {
        status = amend ? "committed (amended)" : "committed";
        below_open = false;
      }
    }

    auto low = button(context, mk(entity, 4),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(200), pixels(36)})
                          .with_absolute_position(48.f, sh - 72.f)
                          .with_label("near the bottom")
                          .with_debug_name("pv_trigger_low"));
    if (low)
      flip_open = !flip_open;

    const RectangleType low_anchor = low.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(entity, 5), low_anchor, flip_open,
                           overlay::Placement::Below,
                           ComponentConfig{}
                               .with_size(ComponentSize{pixels(260),
                                                        pixels(120)})
                               .with_custom_background(panel)
                               .with_debug_name("pv_panel_low"))) {
      div(context, mk(pop.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), pixels(28)})
              .with_label("asked for Below, no room, flipped")
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(white)
              .with_font_size(13.f)
              .with_transparent_bg()
              .with_debug_name("pv_low_caption"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(popover_lab, "System Demos",
                        "popover() with controls inside, and the edge flip",
                        PopoverLab)
