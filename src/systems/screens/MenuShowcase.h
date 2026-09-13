#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// dropdown_menu under a trigger, and the edge flip. The two bottom triggers are
// the interesting ones -- there is no room below them, so their menus open
// upward. context_menu and popover have their own screens.
struct MenuShowcase : ScreenSystem<UIContext<InputAction>> {
  // One menu open by default, because a screenshot baseline cannot click and
  // an open menu is the state worth capturing. Only one: menus dismiss on
  // focus loss, so opening a second closes the first, and defaulting several
  // open would bake a frame into the baseline that no interactive run reaches.
  // Bottom is the one to show -- it is the menu that flips.
  bool file_open = false;
  bool edit_open = false;
  bool bottom_open = true;
  bool corner_open = false;
  std::string status;

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

  struct PlacementSample {
    RectangleType anchor{};
    RectangleType popup{};
  };

  PlacementSample menu(UIContext<InputAction> &context, afterhours::Entity &parent,
                        int id, const std::string &label, const std::vector<MenuItem> &items,
                        bool &open, float x, float y, float trigger_width, float scale,
                        const std::string &name) {
    auto pair = mk(parent, id);
    const int chosen = dropdown_menu(context, pair, label, items, open,
        ComponentConfig{}.with_size({pixels(352 * scale), pixels(40 * scale)})
            .with_absolute_position(x, y).with_font("AtkinsonMock", pixels(20 * scale))
            .with_custom_text_color({238, 246, 250, 255}).with_letter_spacing(0)
            .with_render_layer(10).with_corner_radius(0));
    if (chosen >= 0) status = name + " item chosen: " + items[static_cast<size_t>(chosen)].label;
    auto &holder = deref(pair).first;
    auto &holder_cmp = holder.get<UIComponent>();
    holder_cmp.set_desired_width(pixels(trigger_width));
    holder.get<UIComponentDebug>().set("menu_" + name + "_holder");
    const RectangleType anchor{x, y, trigger_width, 40 * scale};
    float popup_height = 0;
    for (const auto &item : items) popup_height += (item.separator ? 10 : 40) * scale;
    const auto placed = overlay::place(anchor, 352 * scale, popup_height,
        context.screen_width, context.screen_height, overlay::Placement::Below);
    PlacementSample sample{anchor, {placed.x, placed.y, 352 * scale, popup_height}};
    for (const auto child_id : holder_cmp.children) {
      auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
      auto &cmp = child.get<UIComponent>();
      if (child.has<HasLabel>()) {
        child.get<UIComponentDebug>().set("menu_" + name + "_trigger");
        child.get<HasLabel>().text_inset = Vector2Type{12 * scale, 0};
        continue;
      }
      for (const auto list_id : cmp.children) {
        auto &list = UICollectionHolder::getEntityForIDEnforce(list_id);
        auto &list_cmp = list.get<UIComponent>();
        list.get<UIComponentDebug>().set("menu_" + name + "_popup");
        list.get<afterhours::HasColor>().set({20, 37, 52, 255});
        int shortcut_index = 0;
        for (const auto item_id : list_cmp.children) {
          auto &item = UICollectionHolder::getEntityForIDEnforce(item_id);
          auto &item_cmp = item.get<UIComponent>();
          auto &debug = item.get<UIComponentDebug>();
          const auto original_name = debug.name_value;
          if (!item.has<HasLabel>()) {
            if (original_name == "menu_row_fill")
              item.get<afterhours::HasColor>().set({39, 52, 68, 255});
            continue;
          }
          auto &item_label = item.get<HasLabel>();
          item_label.letter_spacing = 0;
          if (original_name == "menu_shortcut") {
            item_label.is_disabled = false;
            item_label.set_explicit_text_color({187, 211, 226, 255});
            item_label.text_inset = Vector2Type{0, 0};
            item_cmp.set_desired_width(pixels(130 * scale));
            item_cmp.absolute_pos_x = 206 * scale;
            if (item.has<afterhours::HasColor>())
              item.get<afterhours::HasColor>().set({0, 0, 0, 0});
            debug.set("menu_" + name + "_shortcut_" + std::to_string(shortcut_index++));
            continue;
          }
          const bool disabled = item_label.is_disabled;
          item_label.text_x_offset = 14 * scale;
          item_label.is_disabled = false;
          item_label.set_explicit_text_color(disabled ? afterhours::Color{167, 182, 196, 255}
                                                      : afterhours::Color{239, 246, 250, 255});
          if (!disabled) item.get<afterhours::HasColor>().set({29, 65, 81, 255});
          debug.set("menu_" + name + "_" + original_name);
        }
      }
    }
    return sample;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.primary = {35, 78, 94, 255};
    UIStylingDefaults::get().set_grid_snapping(false);
    const float sw = context.screen_width;
    const float sh = context.screen_height;
    const float s = std::min(sw / 1280.f, sh / 720.f);
    const float ox = (sw - 1280 * s) / 2;
    const float oy = (sh - 720 * s) / 2;
    const afterhours::Color white{235, 244, 249, 255};
    const afterhours::Color muted{171, 192, 209, 255};
    const auto at = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(ox + x * s, oy + y * s).with_corner_radius(0);
    };
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &name) {
      div(context, mk(entity, id), at(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(sw), pixels(sh)}).with_custom_background({14, 24, 36, 255})
        .with_corner_radius(0).with_debug_name("menu_canvas"));
    text(10, "Anchored menus", 48, 20, 1184, 44, 34, white, "menu_showcase_title");
    text(11, "Menus open below their trigger, flip at the bottom edge and clamp at the right edge.",
         48, 70, 1184, 27, 20, muted, "menu_subtitle");
    text(12, "Choose a menu, then a row. Shortcut labels are hints; this demo reports your selection.",
         48, 101, 1184, 25, 18, muted, "menu_instruction");
    div(context, mk(entity, 13), at(48, 184, 370, 64)
        .with_custom_background({26, 41, 57, 255}).with_corner_radius(8 * s)
        .with_debug_name("menu_bar"));
    text(14, "Live menu examples", 450, 188, 782, 30, 24, white, "menu_live_heading");
    text(15, "File and Edit have room to open downward.", 450, 220, 782, 25, 18, muted, "menu_top_caption");
    // Menu bar along the top: plenty of room, so these open downward.
    const auto file = menu(context, entity, 1, "File", file_items(), file_open,
                            ox + 64 * s, oy + 196 * s, 144 * s, s, "File");
    const auto edit = menu(context, entity, 2, "Edit", edit_items(), edit_open,
                            ox + 234 * s, oy + 196 * s, 144 * s, s, "Edit");
    // Near the bottom edge: overlay::place flips these above their triggers.
    const auto bottom = menu(context, entity, 3, "Bottom edge", file_items(), bottom_open,
                              48 * s, sh - 76 * s, 220 * s, s, "Bottom");
    // Bottom-right: flips vertically AND clamps horizontally.
    const auto corner = menu(context, entity, 4, "Bottom-right corner", edit_items(), corner_open,
                              sw - 300 * s, sh - 76 * s, 252 * s, s, "Corner");
    std::string state = "All menus closed";
    const std::array<PlacementSample, 4> samples{file, edit, bottom, corner};
    const std::array<bool, 4> open_states{file_open, edit_open, bottom_open, corner_open};
    const std::array<const char *, 4> names{"File", "Edit", "Bottom", "Corner"};
    for (size_t i = 0; i < samples.size(); ++i) {
      const auto &sample = samples[i];
      const auto anchor = sample.anchor;
      const auto popup = sample.popup;
      const bool above = i >= 2;
      if (anchor.width > 0) {
        div(context, mk(entity, 30 + static_cast<int>(i)), ComponentConfig{}
            .with_size({pixels(12 * s), pixels(8 * s)})
            .with_absolute_position(anchor.x + anchor.width - 22 * s, anchor.y + 16 * s)
            .with_background(Theme::Usage::None).with_ignore_pointer_events().with_render_layer(14)
            .with_on_draw_fg([=](RectangleType r) {
              const float tip = above ? r.y : r.y + r.height;
              const float edge = above ? r.y + r.height : r.y;
              raylib::DrawLineEx({r.x, edge}, {r.x + r.width / 2, tip}, 2 * s, {219, 237, 245, 255});
              raylib::DrawLineEx({r.x + r.width / 2, tip}, {r.x + r.width, edge}, 2 * s, {219, 237, 245, 255});
            }));
      }
      if (!open_states[i] || popup.height <= 0) continue;
      const bool placed_above = popup.y < anchor.y;
      const bool clamped = popup.x + s < anchor.x;
      state = std::string(names[i]) + " menu open / placed " + (placed_above ? "above" : "below") + " anchor";
      if (clamped) state += ", clamped left";
      div(context, mk(entity, 40 + static_cast<int>(i)), ComponentConfig{}
          .with_size({pixels(popup.width), pixels(popup.height)})
          .with_absolute_position(popup.x, popup.y)
          .with_background(Theme::Usage::None).with_border({118, 158, 179, 255}, s)
          .with_corner_radius(0).with_ignore_pointer_events().with_render_layer(15));
      if (!placed_above) continue;
      div(context, mk(entity, 50 + static_cast<int>(i)), ComponentConfig{}
          .with_size({pixels(12 * s), pixels(anchor.y + anchor.height / 2 - popup.y - popup.height / 2)})
          .with_absolute_position(popup.x - 22 * s, popup.y + popup.height / 2)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            const float x = r.x + r.width / 2;
            raylib::DrawLineEx({x, r.y}, {x, r.y + r.height}, 2 * s, {111, 180, 185, 255});
            raylib::DrawLineEx({x, r.y}, {r.x, r.y + 9 * s}, 2 * s, {111, 180, 185, 255});
            raylib::DrawLineEx({x, r.y}, {r.x + r.width, r.y + 9 * s}, 2 * s, {111, 180, 185, 255});
          }).with_debug_name("menu_upward_guide"));
    }
    text(16, state, 48, 137, 1184, 31, 22, white, "menu_showcase_status");
    text(17, status.empty() ? "No item selected yet" : status,
         450, 261, 782, 29, 19, muted, "menu_selection");
    text(60, "Placement reference", 428, 317, 804, 29, 24, white, "menu_reference_title");
    const std::array<const char *, 3> cases{"Top / below anchor", "Bottom / flip above", "Corner / clamp left"};
    for (int i = 0; i < 3; ++i) {
      const float x = 428 + static_cast<float>(i) * 272;
      text(61 + i, cases[i], x, 354, 260, 28, 18, muted, "");
      div(context, mk(entity, 70 + i), at(x, 393, 256, 142)
          .with_background(Theme::Usage::None).with_border({85, 113, 139, 255}, s)
          .with_ignore_pointer_events().with_on_draw_fg([=](RectangleType r) {
            const float unit = r.width / 256;
            const float ax = r.x + (i == 2 ? 154 : 24) * unit;
            const float ay = r.y + (i == 0 ? 14 : 112) * unit;
            const float px = r.x + (i == 2 ? 116 : 24) * unit;
            const float py = r.y + (i == 0 ? 38 : 34) * unit;
            raylib::DrawRectangleRec({ax, ay, 82 * unit, 18 * unit}, {61, 130, 144, 255});
            raylib::DrawRectangleRec({px, py, 140 * unit, 76 * unit}, {35, 59, 78, 255});
            raylib::DrawRectangleLinesEx({px, py, 140 * unit, 76 * unit}, unit, {164, 196, 214, 255});
            for (int line = 0; line < 3; ++line)
              raylib::DrawLineEx({px + 12 * unit, py + (18 + static_cast<float>(line) * 20) * unit},
                                 {px + 98 * unit, py + (18 + static_cast<float>(line) * 20) * unit},
                                 3 * unit, {162, 187, 207, 255});
          }).with_debug_name("menu_reference_" + std::to_string(i)));
    }
    text(80, "Diagrams show expected placement. Open the live triggers to test it.",
         428, 544, 804, 25, 17, muted, "menu_reference_note");
    text(81, "Revert is intentionally disabled to demonstrate an unavailable action.",
         428, 579, 804, 26, 18, muted, "menu_disabled_note");
    div(context, mk(entity, 82), ComponentConfig{}
        .with_size({pixels(sw - 96 * s), pixels(s)})
        .with_absolute_position(48 * s, sh - 24 * s)
        .with_custom_background({53, 86, 106, 255}).with_ignore_pointer_events()
        .with_debug_name("menu_anchor_baseline"));
    div(context, mk(entity, 83), ComponentConfig{}
        .with_size({pixels(sw), pixels(1)})
        .with_absolute_position(0, sh - 1).with_custom_background({77, 120, 138, 255})
        .with_ignore_pointer_events().with_debug_name("menu_viewport_edge"));
    div(context, mk(entity, 84), ComponentConfig{}
        .with_size({pixels(430 * s), pixels(28 * s)})
        .with_absolute_position((sw - 430 * s) / 2, sh - 67 * s)
        .with_label(fmt::format("Viewport bottom: y = {:.0f} px", sh))
        .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(muted)
        .with_background(Theme::Usage::None).with_ignore_pointer_events());
  }
};

REGISTER_EXAMPLE_SCREEN(menu_showcase, "System Demos",
                        "Anchored dropdown menus with edge flipping",
                        MenuShowcase)
