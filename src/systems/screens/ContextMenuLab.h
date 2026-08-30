#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// context_menu() opened by an actual right-click, which is the one way users
// expect it. floatinghotel wants this for right-click on files and commits.
struct ContextMenuLab : ScreenSystem<UIContext<InputAction>> {
  // Open by default: a screenshot baseline cannot right-click, and a closed
  // menu is an empty screen. Only the first -- menus dismiss on focus loss, so
  // two open at once is a frame no interactive run reaches.
  bool mid_open = true;
  bool corner_open = false;
  // Where the last right-click landed. The menu anchors here, so it opens
  // under the cursor rather than at some fixed spot.
  // Just inside the first row, so the menu reads as having come from it.
  Vector2Type mid_at{176.f, 150.f};
  std::string status = "right-click either row";

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

    // A file list, because right-clicking a row is the real shape of this
    // feature and a screen with one lone button showed none of it.
    static constexpr const char *FILES[6] = {
        "src/renderer.cpp", "src/renderer.h",  "src/scheduler.cpp",
        "assets/atlas.png", "tests/layout.cpp", "README.md"};
    static constexpr const char *STATE[6] = {"modified", "staged", "modified",
                                             "untracked", "staged", ""};

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(340)})
            .with_absolute_position(48.f, 92.f)
            .with_custom_background(afterhours::Color{26, 31, 44, 255})
            .with_roundness(0.05f)
            .with_debug_name("cm_filepanel"));

    for (int f = 0; f < 6; f++) {
      const float ry = 104.f + (float)f * 52.f;
      auto row = button(context, mk(entity, 21 + f),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(492), pixels(44)})
                            .with_absolute_position(62.f, ry)
                            .with_label(FILES[f])
                            .with_alignment(TextAlignment::Left)
                            .with_font_size(16.f)
                            .with_custom_background(
                                afterhours::Color{34, 40, 56, 255})
                            .with_roundness(0.16f)
                            .with_debug_name(fmt::format("cm_file_{}", f)));
      if (context.is_right_click(row.ent().id)) {
        mid_at = context.mouse.pos;
        mid_open = true;
        status = fmt::format("menu opened on {}", FILES[f]);
      }
      if (STATE[f][0] != '\0')
        div(context, mk(entity, 30 + f),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(140), pixels(44)})
                .with_absolute_position(400.f, ry)
                .with_label(STATE[f])
                .with_alignment(TextAlignment::Right)
                .with_font_size(14.f)
                .with_custom_text_color(muted)
                .with_debug_name(fmt::format("cm_state_{}", f)));
    }

    auto target = button(context, mk(entity, 2),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(420), pixels(44)})
                             .with_absolute_position(48.f, 452.f)
                             .with_label("right-click me")
                             .with_debug_name("cm_target"));
    if (context.is_right_click(target.ent().id)) {
      mid_at = context.mouse.pos;
      mid_open = true;
      status = fmt::format("opened at {:.0f},{:.0f}", mid_at.x, mid_at.y);
    }

    auto corner_target =
        button(context, mk(entity, 3),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(420), pixels(44)})
                   .with_absolute_position(sw - 468.f, sh - 108.f)
                   .with_label("right-click me too (no room below)")
                   .with_debug_name("cm_target_corner"));
    if (context.is_right_click(corner_target.ent().id)) {
      corner_open = true;
      status = "corner menu opened";
    }

    const Vector2Type mid = mid_at;
    int picked = context_menu(context, mk(entity, 4), file_items(), mid,
                              mid_open,
                              ComponentConfig{}
                                  .with_size(ComponentSize{pixels(230),
                                                           pixels(32)})
                                  // The shortcut gutter is sized from the row
                                  // height, which under-measures at 20px.
                                  .with_font_size(pixels(16.f))
                                  // A floating panel needs an edge, or it
                                  // reads as painted onto the background.
                                  .with_border(afterhours::Color{92, 104, 132,
                                                                 255},
                                               1.0f)
                                  .with_roundness(0.14f)
                                  .with_debug_name("cm_mid"));
    if (picked != kNoMenuSelection)
      status = fmt::format("centre menu chose #{}", picked);

    // Near the bottom so there is no room below: the menu has to flip up,
    // which is the whole reason placement is shared with popover.
    const Vector2Type corner{sw - 260.f, sh - 76.f};
    picked = context_menu(context, mk(entity, 5), file_items(), corner,
                          corner_open,
                          ComponentConfig{}
                              .with_size(ComponentSize{pixels(230), pixels(32)})
                              .with_font_size(pixels(16.f))
                              .with_border(afterhours::Color{92, 104, 132, 255},
                                           1.0f)
                              .with_roundness(0.14f)
                              .with_debug_name("cm_corner"));
    if (picked != kNoMenuSelection)
      status = fmt::format("corner menu chose #{}", picked);

    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(600), pixels(340)})
            .with_absolute_position(632.f, 92.f)
            .with_custom_background(afterhours::Color{26, 31, 44, 255})
            .with_roundness(0.05f)
            .with_debug_name("cm_notes"));

    static constexpr const char *NOTES[6] = {
        "How this differs from a menu bar",
        "A context menu opens at a point, not under a widget,",
        "so it follows the cursor rather than an anchor rect.",
        "It reuses popover's placement, which is why the one",
        "near the bottom edge flips up instead of running off.",
        "Right-click any row on the left to move it.",
    };
    for (int n = 0; n < 6; n++) {
      div(context, mk(entity, 41 + n),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(566), pixels(30)})
              .with_absolute_position(650.f, 112.f + (float)n * 34.f)
              .with_label(NOTES[n])
              .with_alignment(TextAlignment::Left)
              .with_font_size(n == 0 ? 17.f : 14.f)
              .with_custom_text_color(n == 0 ? white : muted)
              .with_debug_name(fmt::format("cm_note_{}", n)));
    }

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, sh - 96.f)
            .with_label("secondary click; the lower menu has no room and flips up")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("cm_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(context_menu_lab, "System Demos",
                        "context_menu() at a point, including the edge flip",
                        ContextMenuLab)
