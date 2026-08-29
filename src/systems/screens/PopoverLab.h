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
  // Closed, like a real trigger. The pinned panel below keeps the baseline
  // showing what a popover looks like.
  bool below_open = false;
  bool flip_open = false;
  bool pinned_open = true;
  bool place_open[3] = {true, true, true};
  bool amend = false;
  bool sign_off = true;
  std::string status = "Click a trigger to open its panel";

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
      // A vstack inside: the popover does not lay its own flow children out in
      // a column, so caption, checkbox and button drew on top of each other.
      auto body = vstack(context, mk(pop.ent(), 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f),
                                                      percent(1.f)})
                             .with_padding(Spacing::sm)
                             .with_transparent_bg()
                             .with_debug_name("pv_body"));

      div(context, mk(body.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), pixels(26)})
              .with_label("Amend the previous commit?")
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_transparent_bg()
              .with_debug_name("pv_caption"));

      // A checkbox inside is the point: focusing it must NOT dismiss the panel,
      // which is what separates popover from menu_list.
      checkbox(context, mk(body.ent(), 1), amend,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(32)})
                   .with_label("amend")
                   .with_debug_name("pv_amend"));

      checkbox(context, mk(body.ent(), 2), sign_off,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(32)})
                   .with_label("sign off")
                   .with_debug_name("pv_signoff"));

      if (button(context, mk(body.ent(), 3),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(34)})
                     .with_label("Commit")
                     .with_margin(Margin{.top = DefaultSpacing::tiny()})
                     .with_debug_name("pv_commit"))) {
        status = amend ? "Committed, amending the previous commit"
                       : "Committed";
        below_open = false;
      }
    }

    // Pinned open, so the screen shows a populated panel at rest without the
    // primary trigger having to lie about its own state.
    auto pinned = button(context, mk(entity, 10),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(200), pixels(36)})
                             .with_absolute_position(430.f, 96.f)
                             .with_label("Branch")
                             .with_debug_name("pv_trigger_pinned"));
    if (pinned)
      pinned_open = !pinned_open;

    const RectangleType pin_anchor = pinned.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(entity, 11), pin_anchor, pinned_open,
                           overlay::Placement::Below,
                           ComponentConfig{}
                               .with_size(
                                   ComponentSize{pixels(260), pixels(212)})
                               .with_custom_background(panel)
                               .with_debug_name("pv_panel_pinned"))) {
      auto body = vstack(context, mk(pop.ent(), 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f),
                                                      percent(1.f)})
                             .with_padding(Spacing::sm)
                             .with_transparent_bg()
                             .with_debug_name("pv_pin_body"));

      div(context, mk(body.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), pixels(26)})
              .with_label("Switch to")
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(muted)
              .with_font_size(13.f)
              .with_transparent_bg()
              .with_debug_name("pv_pin_caption"));

      static constexpr const char *BRANCHES[4] = {"main", "release/2.4",
                                                  "fix/tab-focus",
                                                  "spike/indexing"};
      for (int b = 0; b < 4; b++) {
        if (button(context, mk(body.ent(), 1 + b),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.f), pixels(30)})
                       .with_label(BRANCHES[b])
                       .with_alignment(TextAlignment::Left)
                       .with_font_size(14.f)
                       // Without this the rounded corners of adjacent rows
                       // touch and the column reads as one scalloped block.
                       .with_margin(Margin{.bottom = pixels(4)})
                       .with_debug_name(fmt::format("pv_branch_{}", b)))) {
          status = fmt::format("Switched to {}", BRANCHES[b]);
          pinned_open = false;
        }
      }
    }

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(420), pixels(196)})
            .with_absolute_position(812.f, 96.f)
            .with_custom_background(panel)
            .with_roundness(0.06f)
            .with_debug_name("pv_notes"));

    static constexpr const char *NOTES[5] = {
        "What a popover is for",
        "A menu holds items you pick from.",
        "A popover holds whatever you build:",
        "checkboxes, inputs, a nested list.",
        "Focus landing inside must not close it.",
    };
    for (int i = 0; i < 5; i++) {
      div(context, mk(entity, 21 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(388), pixels(26)})
              .with_absolute_position(828.f, 114.f + (float)i * 30.f)
              .with_label(NOTES[i])
              .with_alignment(TextAlignment::Left)
              .with_font_size(i == 0 ? 16.f : 14.f)
              .with_custom_text_color(i == 0 ? white : muted)
              .with_debug_name(fmt::format("pv_note_{}", i)));
    }

    // Placement row: the same panel asked to sit on three different sides.
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(48.f, 368.f)
            .with_label("Placement, and what happens when there is no room")
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(white)
            .with_debug_name("pv_place_head"));

    struct Place {
      const char *name;
      overlay::Placement placement;
      float x;
    };
    // Spread so the Right panel and the Left panel do not land on the same
    // strip of screen; at 430 and 812 they both sat over x=630..820.
    const Place places[3] = {
        {"Above", overlay::Placement::Above, 48.f},
        {"Right", overlay::Placement::Right, 380.f},
        {"Left", overlay::Placement::Left, 1032.f},
    };
    for (int i = 0; i < 3; i++) {
      auto t = button(context, mk(entity, 40 + i),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(200), pixels(36)})
                          .with_absolute_position(places[i].x, 500.f)
                          .with_label(places[i].name)
                          .with_debug_name(fmt::format("pv_place_{}", i)));
      if (t)
        place_open[i] = !place_open[i];

      const RectangleType a = t.ent().get<UIComponent>().rect();
      if (auto pop =
              popover(context, mk(entity, 50 + i), a, place_open[i],
                      places[i].placement,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(190), pixels(72)})
                          .with_custom_background(panel)
                          .with_debug_name(fmt::format("pv_placepanel_{}", i)))) {
        div(context, mk(pop.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.f), percent(1.f)})
                .with_label(fmt::format("anchored {}", places[i].name))
                .with_font_size(14.f)
                .with_custom_text_color(muted)
                .with_transparent_bg()
                .with_debug_name(fmt::format("pv_placetext_{}", i)));
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
