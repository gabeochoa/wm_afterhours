#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/clipboard.h>
#include <array>

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
  bool show_anchor = true;
  bool anchor_initialized = false;
  // Where the last right-click landed. The menu anchors here, so it opens
  // under the cursor rather than at some fixed spot.
  // Just inside the first row, so the menu reads as having come from it.
  Vector2Type mid_at{};
  Vector2Type corner_at{};
  size_t target_file = 0;
  std::array<bool, 6> staged{false, true, false, false, true, false};
  std::string status = "Right-click any file row";
  static constexpr std::array<const char *, 6> files{
      "src/renderer.cpp", "src/renderer.h", "src/scheduler.cpp",
      "assets/atlas.png", "tests/layout.cpp", "README.md"};

  std::string file_state(size_t index) const {
    if (staged[index]) return "Staged";
    if (index == 5) return "Clean";
    if (index == 3) return "Untracked";
    return "Modified";
  }

  std::vector<MenuItem> file_items() const {
    return {
        MenuItem{files[target_file], "", false, true},
        MenuItem::sep(),
        MenuItem{"Stage file", "", false, staged[target_file] || target_file == 5},
        MenuItem{"Unstage", "", false, !staged[target_file]},
        MenuItem::sep(),
        MenuItem{"Copy path", "Cmd+C", false, false},
        MenuItem{"Discard changes", "", false, true}, // disabled
    };
  }

  void apply_action(int picked) {
    if (picked == 2) {
      staged[target_file] = true;
      status = "Staged " + std::string(files[target_file]);
      return;
    }
    if (picked == 3) {
      staged[target_file] = false;
      status = "Unstaged " + std::string(files[target_file]);
      return;
    }
    if (picked != 5) return;
    afterhours::clipboard::set_text(files[target_file]);
    status = "Copied path: " + std::string(files[target_file]);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    bool reopen_mid = false;
    bool reopen_corner = false;
    if (context.mouse.just_pressed) {
      const auto outside = [&](Vector2Type at) {
        const auto placed = overlay::place({at.x, at.y, 0, 0}, 340 * s, 242 * s,
                                            context.screen_width, context.screen_height,
                                            overlay::Placement::Below);
        return !is_mouse_inside(context.mouse.pos, RectangleType{placed.x, placed.y, 340 * s, 246 * s});
      };
      if (mid_open && outside(mid_at)) mid_open = false;
      if (corner_open && outside(corner_at)) corner_open = false;
    }
    const auto white = afterhours::Color{240, 245, 253, 255};
    const auto muted = afterhours::Color{181, 195, 216, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    if (!anchor_initialized) {
      mid_at = {(left + 130) * s, (top + 222) * s};
      anchor_initialized = true;
    }
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({18, 23, 33, 255}).with_corner_radius(0).with_debug_name("cm_root"));
    int id = 0;
    const auto label = [&](const std::string &value, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(value)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto control = [&](float x, float y, float w, float h, const std::string &value, const std::string &name) {
      return box(left + x, y, w, h).with_label(value).with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background({49, 67, 94, 255}).with_custom_text_color(white)
          .with_corner_radius(7 * s).with_debug_name(name);
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 90)
        .with_custom_background({34, 42, 57, 255}).with_corner_radius(12 * s));
    label("Context menus", 20, 34, 1104, 42, 34, white, "cm_title");
    label("Right-click any file row. The menu opens at the pointer and adapts to the file state.",
          20, 79, 1104, 27, 21, muted);

    // A file list, because right-clicking a row is the real shape of this
    // feature and a screen with one lone button showed none of it.
    div(context, mk(root.ent(), id++), box(left, 132, 604, 390)
        .with_custom_background({26, 33, 46, 255}).with_corner_radius(12 * s).with_debug_name("cm_filepanel"));
    label("File", 30, 150, 390, 30, 23, white);
    label("State", 448, 150, 126, 30, 23, white);
    for (size_t f = 0; f < files.size(); ++f) {
      const float y = 194 + static_cast<float>(f) * 50;
      const bool target = target_file == f;
      auto config = box(left + 16, y, 572, 44)
          .with_custom_background(target ? afterhours::Color{47, 66, 94, 255} : afterhours::Color{33, 41, 56, 255})
          .with_corner_radius(4 * s).with_debug_name(fmt::format("cm_file_{}", f));
      if (target) config.with_border({151, 182, 224, 255}, s);
      auto row = button(context, mk(root.ent(), id++), config);
      if (context.is_right_click(row.ent().id)) {
        target_file = f;
        mid_at = context.mouse.pos;
        mid_open = true;
        reopen_mid = true;
        corner_open = false;
        status = "menu opened on " + std::string(files[f]);
      }
      label(files[f], 30, y + 5, 390, 34, 18, white, fmt::format("cm_filename_{}", f), true);
      const auto state = file_state(f);
      const auto state_color = state == "Staged" ? afterhours::Color{68, 99, 74, 255}
                             : state == "Modified" ? afterhours::Color{91, 75, 45, 255}
                             : state == "Untracked" ? afterhours::Color{62, 77, 112, 255}
                                                     : afterhours::Color{58, 68, 82, 255};
      div(context, mk(root.ent(), id++), box(left + 448, y + 6, 126, 32)
          .with_custom_background(state_color).with_corner_radius(6 * s).with_ignore_pointer_events());
      label(state, 454, y + 8, 114, 28, 18, white, fmt::format("cm_state_{}", f));
    }
    label("Discard is disabled: this fixture never deletes files.", 20, 493, 568, 24, 18, muted, "cm_discard_reason");

    auto target = button(context, mk(root.ent(), id++), control(20, 550, 380, 44, "Open at pointer", "cm_target"));
    if (target || context.is_right_click(target.ent().id)) {
      mid_at = context.mouse.pos;
      mid_open = true;
      reopen_mid = true;
      corner_open = false;
      status = fmt::format("opened at {:.0f},{:.0f}", mid_at.x, mid_at.y);
    }
    label("Right-click or activate; uses the selected file.", 20, 601, 584, 25, 18, muted);

    div(context, mk(root.ent(), id++), box(left + 624, 132, 520, 390)
        .with_custom_background({26, 33, 46, 255}).with_corner_radius(12 * s).with_debug_name("cm_notes"));
    label("Point anchoring", 644, 148, 480, 32, 27, white);
    label("The click position is the anchor.", 644, 191, 480, 27, 21, muted);
    label("The menu prefers below that point.", 644, 222, 480, 27, 21, muted);
    label("Near an edge, it flips or clamps to fit.", 644, 253, 480, 27, 21, muted);
    for (int side = 0; side < 2; ++side) {
      div(context, mk(root.ent(), id++), box(left + 648 + side * 244, 302, 226, 120)
          .with_custom_background({20, 26, 37, 255}).with_border({79, 95, 119, 255}, s)
          .with_ignore_pointer_events().with_on_draw_fg([side, s](RectangleType r) {
            const float anchor_y = r.y + (side == 0 ? 22.f : 100.f) * s;
            const float anchor_x = r.x + 30 * s;
            afterhours::draw_rectangle({anchor_x, anchor_y + (side == 0 ? 7.f : -77.f) * s, 140 * s, 70 * s}, {67, 87, 119, 255});
            for (int row = 0; row < 3; ++row)
              afterhours::draw_rectangle({anchor_x + 12 * s, anchor_y + (side == 0 ? 22.f : -62.f) * s + row * 18 * s,
                                         114 * s, 2 * s}, {160, 180, 208, 255});
            afterhours::draw_line_ex({anchor_x - 8 * s, anchor_y}, {anchor_x + 8 * s, anchor_y}, 2 * s, {255, 206, 113, 255});
            afterhours::draw_line_ex({anchor_x, anchor_y - 8 * s}, {anchor_x, anchor_y + 8 * s}, 2 * s, {255, 206, 113, 255});
          }));
      label(side == 0 ? "Room below" : "Flipped above", 648 + side * 244, 428, 226, 27, 20, muted);
    }
    if (button(context, mk(root.ent(), id++), control(644, 474, 230, 32,
        show_anchor ? "Anchor crosshair: on" : "Anchor crosshair: off", "cm_anchor_toggle"))) show_anchor = !show_anchor;
    label("+ invocation point", 890, 477, 234, 27, 19, muted);

    label("No room below. Expect a flip above.", 724, 610, 400, 28, 19, muted, "cm_hint");
    auto corner_target = button(context, mk(root.ent(), id++), control(784, 650, 340, 44,
        "Bottom-edge flip test", "cm_target_corner"));
    if (corner_target || context.is_right_click(corner_target.ent().id)) {
      corner_at = context.mouse.pos;
      corner_open = true;
      reopen_corner = true;
      mid_open = false;
      status = "corner menu opened";
    }
    label(status, 20, 646, 730, 28, 20, white, "cm_status");
    label("Target: " + std::string(files[target_file]) + " / " + file_state(target_file),
          20, 679, 730, 25, 18, muted, "cm_target_status");

    const auto menu_config = ComponentConfig{}.with_size({pixels(340 * s), pixels(44 * s)})
        .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color({255, 255, 255, 255})
        .with_border({138, 158, 188, 255}, 2 * s).with_render_layer(20);
    const auto items = file_items();
    const auto render_menu = [&](Vector2Type at, bool &open, bool reopened, const std::string &name) {
      auto pair = mk(root.ent(), id++);
      auto &holder = deref(pair).first;
      if (reopened && holder.has<HasMenuState>()) holder.get<HasMenuState>().was_open_last_frame = false;
      const int picked = context_menu(context, pair, items, at, open,
                                      ComponentConfig(menu_config).with_debug_name(name));
      if (!holder.has<UIComponent>() || !open) return picked;
      const auto list_ids = holder.get<UIComponent>().children;
      for (const auto list_id : list_ids) {
        auto list = UICollectionHolder::getEntityForID(list_id);
        if (!list.valid() || !list.asE().has<UIComponent>()) continue;
        for (const auto child_id : list.asE().get<UIComponent>().children) {
          auto child = UICollectionHolder::getEntityForID(child_id);
          if (!child.valid() || !child.asE().has<HasLabel>()) continue;
          auto &row = child.asE();
          auto &text = row.get<HasLabel>();
          const bool shortcut = row.has<UIComponentDebug>() &&
                                row.get<UIComponentDebug>().name_value == "menu_shortcut";
          if (shortcut) {
            text.is_disabled = false;
            text.set_explicit_text_color({207, 219, 238, 255});
            row.get<UIComponent>().desired[Axis::X].value -= 7 * s;
            row.addComponentIfMissing<IgnorePointerEvents>();
            continue;
          }
          text.text_x_offset = 12 * s;
          if (text.is_disabled) row.removeComponentIfExists<HasClickListener>();
        }
        const auto placed = overlay::place({at.x, at.y, 0, 0}, 340 * s, 242 * s,
                                            context.screen_width, context.screen_height,
                                            overlay::Placement::Below);
        const auto holder_rect = holder.get<UIComponent>().rect();
        float row_y = 2 * s;
        for (size_t index = 0; index < items.size(); ++index) {
          const auto &item = items[index];
          const float height = (item.separator ? 11.f : 44.f) * s;
          if (item.disabled && !item.separator) {
            auto shield = button(context, mk(holder, 50000 + static_cast<int>(index)),
                ComponentConfig{}.with_size({pixels(336 * s), pixels(height)})
                    .with_absolute_position(placed.x - holder_rect.x + 2 * s,
                                            placed.y - holder_rect.y + row_y)
                    .with_custom_background({0, 0, 0, 0}).with_custom_hover_bg({0, 0, 0, 0}).with_corner_radius(0)
                    .with_skip_tabbing(true).with_render_layer(23));
            shield.ent().get<HasClickListener>().cb = [&context, list_id](afterhours::Entity &) {
              context.set_focus(list_id);
            };
          }
          row_y += height;
        }
      }
      return picked;
    };
    apply_action(render_menu(mid_at, mid_open, reopen_mid, "cm_mid"));
    // Near the bottom so there is no room below: the menu has to flip up,
    // which is the whole reason placement is shared with popover.
    apply_action(render_menu(corner_at, corner_open, reopen_corner, "cm_corner"));
    if (!show_anchor || (!mid_open && !corner_open)) return;
    const auto at = corner_open ? corner_at : mid_at;
    div(context, mk(root.ent(), id++), ComponentConfig{}.with_size({pixels(20 * s), pixels(20 * s)})
        .with_absolute_position(at.x - 10 * s, at.y - 10 * s).with_background(Theme::Usage::None)
        .with_ignore_pointer_events().with_render_layer(30).with_debug_name("cm_anchor")
        .with_on_draw_fg([s](RectangleType r) {
          const float x = r.x + r.width / 2;
          const float y = r.y + r.height / 2;
          afterhours::draw_line_ex({r.x, y}, {r.x + r.width, y}, s, {255, 210, 126, 255});
          afterhours::draw_line_ex({x, r.y}, {x, r.y + r.height}, s, {255, 210, 126, 255});
        }));
  }
};

REGISTER_EXAMPLE_SCREEN(context_menu_lab, "System Demos",
                        "context_menu() at a point, including the edge flip",
                        ContextMenuLab)
