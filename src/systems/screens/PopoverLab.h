#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// popover(): a menu holds items, a popover holds whatever the caller builds,
// and it survives focus landing on one of those controls. floatinghotel wants
// it for the commit amend/fixup panel and the branch selector.
struct PopoverLab : ScreenSystem<UIContext<InputAction>> {
  // Closed, like a real trigger. The initially open branch panel below keeps
  // the baseline showing what a popover looks like.
  bool below_open = false;
  bool flip_open = false;
  bool pinned_open = true;
  bool place_open[3] = {false, false, false};
  bool amend = false;
  bool sign_off = true;
  int current_branch = 0;
  std::string status = "No action yet. Branch opens initially for comparison.";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = {19, 24, 32, 255};
    theme.surface = {36, 43, 55, 255};
    theme.primary = {56, 80, 113, 255};
    theme.secondary = {46, 55, 71, 255};
    theme.accent = {125, 177, 244, 255};
    theme.font = {237, 242, 249, 255};
    theme.font_muted = {175, 187, 204, 255};
    theme.focus = {165, 201, 248, 255};
    context.set_theme(theme);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{174, 188, 211, 255};
    const auto panel = theme.surface;
    const auto cyan = afterhours::Color{154, 192, 245, 255};
    const auto ink = afterhours::Color{15, 22, 36, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0)
        .with_ignore_pointer_events());
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("pv_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(6 * s).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w,
                    float h, float size, afterhours::Color color,
                    const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    auto control = [&](float x, float y, float w, float h) {
      return box(x, y, w, h).with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background(cyan).with_custom_text_color(ink);
    };
    auto floating = [&](float w, float h, const std::string &debug) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_custom_background(panel).with_border({79, 94, 117, 255}, s)
          .with_corner_radius(8 * s).with_skip_grid_snap(true)
          .with_debug_name(debug);
    };
    auto row = [&](float h) {
      return ComponentConfig{}.with_size({percent(1.f), pixels(h * s)})
          .with_font("AtkinsonMock", pixels(20 * s)).with_skip_grid_snap(true);
    };
    text(0, "Popovers", 48, 16, 550, 46, 38, white, "pv_title", true);
    text(1, "Anchored panels containing controls", 48, 62, 680, 28, 21, muted, "pv_subtitle");
    text(6, "Amend and sign-off form", 48, 146, 330, 27, 19, muted);
    text(7, "Branch opens initially", 430, 64, 340, 26, 18, muted, "pv_initial");
    text(8, "Trigger", 942, 24, 100, 28, 18, cyan);
    text(9, "Popover", 1100, 24, 132, 28, 18, white);
    div(context, mk(root.ent(), 80), box(918, 31, 16, 16).with_custom_background(cyan));
    div(context, mk(root.ent(), 81), box(1076, 31, 16, 16).with_custom_background(panel));

    auto trigger = button(context, mk(root.ent(), 2), control(48, 96, 240, 36)
        .with_label("Commit options").with_debug_name("pv_trigger"));
    if (trigger) below_open = !below_open;
    const RectangleType anchor = trigger.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(root.ent(), 3), anchor, below_open,
                           overlay::Placement::Below, floating(320, 216, "pv_panel"))) {
      pop.ent().get<UIComponentDebug>().set("pv_live_commit");
      // A vstack inside, for the padding: the panel itself has none.
      auto body = vstack(context, mk(pop.ent(), 0), ComponentConfig{}
          .with_size({percent(1.f), percent(1.f)}).with_padding(Padding::all(pixels(12 * s)))
          .with_transparent_bg().with_debug_name("pv_body"));
      div(context, mk(body.ent(), 0), row(34).with_label("Commit preferences")
          .with_alignment(TextAlignment::Left).with_custom_text_color(white)
          .with_transparent_bg().with_debug_name("pv_caption"));
      // A checkbox inside is the point: focusing it must NOT dismiss the panel,
      // which is what separates popover from menu_list.
      const auto option = [&](int id, bool &value, const char *label, const char *debug) {
        auto checkbox_row = checkbox(context, mk(body.ent(), id), value,
            row(40).with_label("").with_checkbox_indicators("", "")
                .with_custom_background(afterhours::Color{48, 65, 87, 255})
                .with_corner_radius(6 * s).with_margin(Margin{.bottom = pixels(4 * s)})
                .with_debug_name(debug));
        div(context, mk(checkbox_row.ent(), 1), box(10, 11, 18, 18)
            .with_ignore_pointer_events().with_on_draw_fg([s, value, white](RectangleType mark) {
              raylib::DrawRectangleLinesEx(mark, 1.5f * s, white);
              if (!value) return;
              raylib::DrawLineEx({mark.x + 3 * s, mark.y + 9 * s}, {mark.x + 7 * s, mark.y + 13 * s}, 2 * s, white);
              raylib::DrawLineEx({mark.x + 7 * s, mark.y + 13 * s}, {mark.x + 15 * s, mark.y + 4 * s}, 2 * s, white);
            }));
        div(context, mk(checkbox_row.ent(), 0), box(38, 0, 246, 40).with_label(label)
            .with_font("AtkinsonMock", pixels(19 * s)).with_alignment(TextAlignment::Left)
            .with_custom_text_color(white).with_ignore_pointer_events());
      };
      option(1, amend, "Amend previous commit", "pv_amend");
      option(2, sign_off, "Include sign-off", "pv_signoff");
      if (button(context, mk(body.ent(), 3), row(44).with_label("Commit demo")
          .with_custom_background(cyan).with_custom_text_color(ink)
          .with_margin(Margin{.top = pixels(8 * s)}).with_debug_name("pv_commit"))) {
        status = fmt::format("Demo commit: {} / sign-off {}", amend ? "amend previous" : "new commit", sign_off ? "included" : "omitted");
        below_open = false;
      }
    }

    // Initially open, so the screen shows a populated panel at rest without
    // the primary trigger having to lie about its own state.
    static constexpr const char *BRANCHES[4] = {"main", "release/2.4", "fix/tab-focus", "spike/indexing"};
    auto pinned = button(context, mk(root.ent(), 10), control(430, 96, 320, 36)
        .with_label(fmt::format("Branch: {}", BRANCHES[current_branch])).with_debug_name("pv_trigger_pinned"));
    if (pinned) pinned_open = !pinned_open;
    const RectangleType pin_anchor = pinned.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(root.ent(), 11), pin_anchor, pinned_open,
                           overlay::Placement::Below, floating(320, 224, "pv_panel_pinned"))) {
      pop.ent().get<UIComponentDebug>().set("pv_live_branch");
      auto body = vstack(context, mk(pop.ent(), 0), ComponentConfig{}
          .with_size({percent(1.f), percent(1.f)}).with_padding(Padding::all(pixels(12 * s)))
          .with_transparent_bg().with_debug_name("pv_pin_body"));
      div(context, mk(body.ent(), 0), row(34).with_label("Switch branch")
          .with_alignment(TextAlignment::Left).with_custom_text_color(white)
          .with_transparent_bg().with_debug_name("pv_pin_caption"));
      for (int b = 0; b < 4; b++) {
        if (button(context, mk(body.ent(), 1 + b), row(36)
            .with_label(fmt::format("  {}{}", BRANCHES[b], b == current_branch ? "  Current" : ""))
            .with_alignment(TextAlignment::Left).with_font("AtkinsonMock", pixels(18 * s))
            .with_custom_background(b == current_branch ? afterhours::Color{54, 76, 105, 255} : panel)
            .with_custom_text_color(white).with_custom_hover_bg({61, 76, 100, 255}).with_corner_radius(4 * s)
            // Without this the rounded corners of adjacent rows
            // touch and the column reads as one scalloped block.
            .with_margin(Margin{.bottom = pixels(4 * s)})
            .with_debug_name(fmt::format("pv_branch_{}", b)))) {
          current_branch = b;
          status = fmt::format("Demo branch switched to {}", BRANCHES[b]);
          pinned_open = false;
        }
      }
    }

    div(context, mk(root.ent(), 20), box(812, 96, 420, 260).with_custom_background(panel).with_debug_name("pv_notes"));
    text(21, "Use controls inside", 828, 108, 388, 34, 27, white, "pv_note_0", true);
    text(22, "Change options without dismissing", 828, 152, 388, 27, 20, muted);
    text(23, "the panel. Move focus outside to close.", 828, 181, 388, 27, 20, muted);
    text(24, "Menu: choose an item", 828, 229, 360, 26, 19, cyan);
    text(25, "Open   Save   Close", 844, 257, 352, 26, 19, white);
    text(26, "Form: edit several options", 828, 295, 372, 26, 19, cyan);
    text(27, "[ ] Amend    [x] Sign off", 844, 323, 352, 26, 18, white);

    // Placement row: the same panel asked to sit on three different sides.
    text(30, "Placement and edge flipping", 48, 370, 680, 34, 27, white, "pv_place_head", true);
    text(31, "Open each example to compare", 812, 375, 420, 26, 18, muted);
    struct Place { const char *name; overlay::Placement placement; float x; };
    // Spread so the Right panel and the Left panel do not land on the same
    // strip of screen; at 430 and 812 they both sat over x=630..820.
    const Place places[3] = {{"Above", overlay::Placement::Above, 48.f},
                            {"Right", overlay::Placement::Right, 380.f},
                            {"Left", overlay::Placement::Left, 1032.f}};
    const std::array<RectangleType, 3> regions{{{36, 414, 224, 158}, {368, 414, 410, 170}, {806, 414, 438, 170}}};
    for (int i = 0; i < 3; i++) {
      const auto r = regions[static_cast<size_t>(i)];
      div(context, mk(root.ent(), 90 + i), box(r.x, r.y, r.width, r.height)
          .with_custom_background(afterhours::Color{21, 31, 47, 255}).with_ignore_pointer_events());
      text(94 + i, fmt::format("{} placement", places[i].name), r.x + 8, i == 0 ? 540 : 418, r.width - 16, 26, 18, muted);
      auto t = button(context, mk(root.ent(), 40 + i), control(places[i].x, 500, 200, 36)
          .with_label(places[i].name).with_debug_name(fmt::format("pv_place_{}", i)));
      if (t) place_open[i] = !place_open[i];
      const RectangleType a = t.ent().get<UIComponent>().rect();
      if (!place_open[i]) {
        const float ghost_x = i == 0 ? 48.f : i == 1 ? 580.f : 842.f;
        const float ghost_y = i == 0 ? 428.f : 500.f;
        div(context, mk(root.ent(), 120 + i), box(ghost_x, ghost_y, 190, 72)
            .with_label("Panel appears here").with_alignment(TextAlignment::Center)
            .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(muted)
            .with_border(afterhours::Color{67, 88, 112, 255}, 1.f)
            .with_ignore_pointer_events());
      }
      if (auto pop = popover(context, mk(root.ent(), 50 + i), a, place_open[i], places[i].placement,
                            floating(190, 72, fmt::format("pv_placepanel_{}", i)))) {
        pop.ent().get<UIComponentDebug>().set(fmt::format("pv_live_place_{}", i));
        div(context, mk(pop.ent(), 0), ComponentConfig{}
            .with_size({percent(1.f), percent(1.f)})
            .with_label(fmt::format("Panel {}", places[i].name)).with_alignment(TextAlignment::Center)
            .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(white)
            .with_transparent_bg().with_debug_name(fmt::format("pv_placetext_{}", i)));
      }
      div(context, mk(root.ent(), 110 + i), box(i == 0 ? 264 : i == 1 ? 582 : 996, i == 0 ? 458 : 468, 30, 26)
          .with_ignore_pointer_events().with_on_draw_fg([s, i, cyan](RectangleType r) {
            const raylib::Vector2 start{r.x + (i == 2 ? 27 : 3) * s, r.y + (i == 0 ? 23 : 13) * s};
            const raylib::Vector2 end{r.x + (i == 0 ? 3 : i == 1 ? 27 : 3) * s, r.y + (i == 0 ? 3 : 13) * s};
            raylib::DrawLineEx(start, end, 2 * s, cyan);
            const float dx = i == 0 ? 5.f : i == 1 ? -6.f : 6.f;
            raylib::DrawLineEx(end, {end.x + dx * s, end.y + (i == 0 ? 6 : -5) * s}, 2 * s, cyan);
            raylib::DrawLineEx(end, {end.x + (i == 0 ? -5 : dx) * s, end.y + (i == 0 ? 6 : 5) * s}, 2 * s, cyan);
          }));
    }
    text(60, "Last action", 48, 596, 138, 28, 19, cyan);
    text(61, status, 194, 596, 1038, 28, 20, white, "pv_status");
    text(62, "Below requested / Above when the bottom edge leaves no room", 384, 650, 848, 30, 20, muted);
    auto low = button(context, mk(entity, 904), control(48, 648, 250, 36)
        .with_absolute_position(left + 48 * s, context.screen_height - 72 * s)
        .with_label("Bottom-edge flip").with_debug_name("pv_trigger_low"));
    if (low) flip_open = !flip_open;
    const RectangleType low_anchor = low.ent().get<UIComponent>().rect();
    if (auto pop = popover(context, mk(root.ent(), 5), low_anchor, flip_open,
                           overlay::Placement::Below, floating(320, 120, "pv_panel_low"))) {
      pop.ent().get<UIComponentDebug>().set("pv_live_low");
      div(context, mk(pop.ent(), 0), ComponentConfig{}
          .with_size({percent(1.f), percent(1.f)})
          .with_label("Below requested\nFlipped above the trigger")
          .with_alignment(TextAlignment::Center).with_custom_text_color(white)
          .with_font("AtkinsonMock", pixels(20 * s)).with_transparent_bg()
          .with_debug_name("pv_low_caption"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(popover_lab, "System Demos",
                        "popover() with controls inside, and the edge flip",
                        PopoverLab)
