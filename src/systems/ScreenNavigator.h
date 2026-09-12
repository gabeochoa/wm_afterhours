#pragma once

#include "../input_mapping.h"
#include "../settings.h"
#include "ExampleScreenRegistry.h"
#include "ScreenTree.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScreenNavigator : afterhours::System<UIContext<InputAction>> {
  static constexpr float WIDTH = 430.f;
  static constexpr float ROW_H = 32.f;
  ScreenTree tree;
  bool visible = false;
  bool show_launcher = false;
  bool focus_filter = false;
  bool reveal_cursor = false;
  int current_index = 0;
  // Deferred: swapping mid-iteration frees what the cycler still points at.
  std::function<void(int)> on_pick;

  void build_rows(const std::vector<std::string> &names) {
    tree.entries.clear();
    auto &registry = ExampleScreenRegistry::get();
    for (std::size_t i = 0; i < names.size(); ++i)
      tree.entries.push_back({names[i], registry.get_screen_category(names[i]),
                             registry.get_screen_description(names[i]), static_cast<int>(i)});
    tree.rebuild();
  }
  void open() {
    visible = focus_filter = reveal_cursor = true;
    tree.reveal(current_index);
  }
  void activate(UIContext<InputAction> &context, int row) {
    if (row < 0 || row >= static_cast<int>(tree.rows.size())) return;
    if (tree.rows[row].header()) { tree.toggle(row); reveal_cursor = true; return; }
    const auto index = tree.entries[tree.rows[row].entry].screen_index;
    visible = false;
    if (!on_pick) return;
    auto callback = on_pick;
    context.defer([callback, index] { callback(index); });
  }
  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    const int layer = 2000;
    if (!visible) {
      if (!show_launcher) return;
      if (button(context, mk(entity, 100), ComponentConfig{}
          .with_label("Browse screens (`)").with_size({pixels(200), pixels(32)})
          .with_absolute_position(static_cast<float>(Settings::get().get_screen_width()) - 212, 8)
          .with_custom_background({22, 25, 32, 255}).with_custom_text_color({220, 228, 240, 255})
          .with_auto_text_color(false).with_render_layer(layer).with_debug_name("nav_open"))) open();
      return;
    }
    const float height = static_cast<float>(Settings::get().get_screen_height());
    const float row_height = ROW_H * height / 720.f;
    div(context, mk(entity, 90), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_absolute_position(0, 0)
        .with_custom_background({13, 16, 22, 255}).with_corner_radius(0)
        .with_render_layer(layer).with_debug_name("nav_backdrop"));
    auto panel = vstack(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(WIDTH), pixels(height)})
        .with_absolute_position(0, 0).with_custom_background({22, 25, 32, 255})
        .with_corner_radius(0).with_padding(Padding::all(pixels(12)))
        .with_render_layer(layer).with_no_wrap().with_debug_name("nav_panel"));
    auto label = [&](int id, const std::string &text, float h) {
      div(context, mk(panel.ent(), id), ComponentConfig{}
          .with_label(text).with_size({percent(1), pixels(h)})
          .with_font_size(17).with_custom_text_color({220, 228, 240, 255})
          .with_render_layer(layer));
    };
    label(0, "Screens / search and browse", 32);
    if (button(context, mk(entity, 92), ComponentConfig{}
        .with_label("Close").with_size({pixels(64), pixels(28)})
        .with_absolute_position(WIDTH - 76, 12).with_font_size(16)
        .with_custom_background({38, 43, 54, 255}).with_custom_text_color({220, 228, 240, 255})
        .with_auto_text_color(false).with_render_layer(layer).with_debug_name("nav_close"))) {
      visible = false;
      return;
    }
    const auto old_query = tree.query;
    auto filter = text_input(context, mk(panel.ent(), 1), tree.query, ComponentConfig{}
        .with_size({percent(1), pixels(36)}).with_placeholder("Search names, categories, descriptions")
        .with_custom_background({38, 43, 54, 255}).with_auto_text_color(false)
        .with_custom_text_color({220, 228, 240, 255}).with_font_size(16)
        .with_render_layer(layer).with_debug_name("nav_filter"));
    if (focus_filter) { context.set_focus(filter.ent().id); focus_filter = false; }
    if (tree.query != old_query) { tree.cursor = 0; tree.rebuild(); reveal_cursor = true; }
    label(2, "Arrows browse / Enter opens / Esc closes", 30);
    if (afterhours::input::is_key_pressed(afterhours::keys::ESCAPE)) { visible = false; return; }
    bool moved = false;
    if (afterhours::input::is_key_pressed(afterhours::keys::DOWN)) { tree.move(1); moved = true; }
    if (afterhours::input::is_key_pressed(afterhours::keys::UP)) { tree.move(-1); moved = true; }
    if (!context.has_focus(filter.ent().id)) {
      if (afterhours::input::is_key_pressed(afterhours::keys::LEFT)) { tree.left(); moved = true; }
      if (afterhours::input::is_key_pressed(afterhours::keys::RIGHT)) { tree.right(); moved = true; }
    }
    if (moved) { context.set_focus(panel.ent().id); reveal_cursor = true; }
    if (afterhours::input::is_key_pressed(afterhours::keys::ENTER)) activate(context, tree.cursor);
    if (tree.rows.empty()) { label(3, "No matching screens", 40); return; }
    const auto &selected = tree.rows[tree.cursor];
    const std::string detail = selected.header() ? selected.label : tree.entries[selected.entry].description;
    div(context, mk(entity, 91), ComponentConfig{}
        .with_label(selected.label + "\n\n" + detail + "\n\nEnter opens / current screen marked *")
        .with_size({pixels(std::max(0.f, static_cast<float>(Settings::get().get_screen_width()) - WIDTH - 48)), pixels(250)})
        .with_absolute_position(WIDTH + 24, 80).with_font_size(24)
        .with_custom_text_color({220, 228, 240, 255}).with_render_layer(layer)
        .with_debug_name("nav_details"));
    auto list = virtual_list(context, mk(panel.ent(), 4), tree.rows.size(), row_height,
        [&](std::size_t i, afterhours::Entity &row_entity) {
          const auto row = tree.rows[i];
          const bool current = !row.header() && tree.entries[row.entry].screen_index == current_index;
          const bool cursor = static_cast<int>(i) == tree.cursor;
          const std::string label = row.header()
              ? std::string(tree.query.empty() && !tree.expanded.contains(row.label) ? "+ " : "- ") + row.label
              : std::string(current ? "* " : "    ") + row.label;
          auto debug_label = row.label;
          std::replace(debug_label.begin(), debug_label.end(), ' ', '_');
          auto config = ComponentConfig{}.with_label(label)
              .with_size({percent(1), pixels(row_height)}).with_alignment(TextAlignment::Left)
              .with_font_size(16).with_render_layer(layer).with_auto_text_color(false)
              .with_custom_text_color(cursor ? afterhours::Color{20, 24, 32, 255} : afterhours::Color{220, 228, 240, 255})
              .with_custom_background(cursor ? afterhours::Color{230, 235, 244, 255} :
                  current ? afterhours::Color{64, 52, 24, 255} : afterhours::Color{22, 25, 32, 255})
              .with_debug_name(row.header() ? "nav_category_" + debug_label : "nav_screen_" + debug_label)
              .with_skip_tabbing(true);
          if (button(context, mk(row_entity, 0), config))
            context.defer([this, &context, i] { activate(context, static_cast<int>(i)); });
        }, ComponentConfig{}.with_size({percent(1), pixels(height - 130)})
            .with_render_layer(layer).with_debug_name("nav_list"));
    if (!reveal_cursor || !list.ent().has<HasScrollView>()) return;
    auto &scroll = list.ent().get<HasScrollView>();
    const float y = tree.cursor * row_height;
    const float view = height - 130;
    if (y < scroll.scroll_target.y) scroll.scroll_target.y = y;
    if (y + row_height > scroll.scroll_target.y + view) scroll.scroll_target.y = y + row_height - view;
    scroll.scroll_offset.y = scroll.scroll_target.y;
    reveal_cursor = false;
  }
};
