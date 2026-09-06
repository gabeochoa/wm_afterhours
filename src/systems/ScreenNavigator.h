#pragma once

#include "../input_mapping.h"
#include "../settings.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Sidebar for jumping between the example screens, since , and . are painful
// at 100+. Registered only by the interactive demo, never by the screenshot or
// e2e paths, so it cannot reach a baseline.
struct ScreenNavigator : afterhours::System<UIContext<InputAction>> {
  struct Row {
    bool is_header = false;
    std::string label;
    int screen_index = -1;
  };

  static constexpr float WIDTH = 300.f;
  static constexpr float ROW_H = 26.f;

  std::vector<Row> all_rows;
  std::vector<int> shown; // indices into all_rows
  std::string filter;
  bool visible = true;
  int current_index = 0;

  // Deferred: swapping mid-iteration frees what the cycler still points at.
  std::function<void(int)> on_pick;

  void build_rows(const std::vector<std::string> &names) {
    auto &reg = ExampleScreenRegistry::get();
    std::vector<std::string> order = {"Game Mockups", "Component Galleries",
                                      "System Demos", "Tools"};
    for (const auto &n : names) {
      std::string c = reg.get_screen_category(n);
      if (std::find(order.begin(), order.end(), c) == order.end())
        order.push_back(c);
    }

    all_rows.clear();
    for (const auto &cat : order) {
      bool wrote_header = false;
      for (size_t i = 0; i < names.size(); i++) {
        if (reg.get_screen_category(names[i]) != cat)
          continue;
        if (!wrote_header) {
          all_rows.push_back({true, cat.empty() ? "Uncategorised" : cat, -1});
          wrote_header = true;
        }
        all_rows.push_back({false, names[i], static_cast<int>(i)});
      }
    }
  }

  size_t n_screens() const {
    size_t n = 0;
    for (int i : shown)
      if (!all_rows[static_cast<size_t>(i)].is_header)
        n++;
    return n;
  }

  static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void apply_filter() {
    shown.clear();
    const std::string needle = lower(filter);
    if (needle.empty()) {
      for (size_t i = 0; i < all_rows.size(); i++)
        shown.push_back(static_cast<int>(i));
      return;
    }
    // Headers only earn a place when something under them survived.
    for (size_t i = 0; i < all_rows.size(); i++) {
      if (all_rows[i].is_header)
        continue;
      if (lower(all_rows[i].label).find(needle) == std::string::npos)
        continue;
      size_t h = i;
      while (h > 0 && !all_rows[h].is_header)
        h--;
      if (all_rows[h].is_header &&
          (shown.empty() || shown.back() != static_cast<int>(h)))
        shown.push_back(static_cast<int>(h));
      shown.push_back(static_cast<int>(i));
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    if (!visible || all_rows.empty())
      return;

    apply_filter();

    const int layer = 2000; // above modals, which sit at 1000
    const auto panel_bg = afterhours::Color{22, 25, 32, 255};
    const auto header_col = afterhours::Color{130, 145, 175, 255};
    const auto row_col = afterhours::Color{210, 218, 232, 255};
    const auto sel_col = afterhours::Color{15, 18, 24, 255};
    const auto sel_bg = afterhours::Color{110, 170, 230, 255};

    const float h = static_cast<float>(Settings::get().get_screen_height());

    auto panel = vstack(context, mk(entity, 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(WIDTH), pixels(h)})
                            .with_absolute_position(0.f, 0.f)
                            .with_custom_background(panel_bg)
                            .with_padding(Spacing::sm)
                            .with_render_layer(layer)
                            .with_no_wrap()
                            .with_debug_name("nav_panel"));

    div(context, mk(panel.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format("Screens ({})", n_screens()))
            .with_size(ComponentSize{percent(1.f), pixels(28)})
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(row_col)
            .with_render_layer(layer)
            .with_debug_name("nav_title"));

    text_input(context, mk(panel.ent(), 1), filter,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(32)})
                   .with_placeholder("filter")
                   // Otherwise it takes the current screen's theme and the
                   // sidebar changes colour every time you switch.
                   .with_custom_background(afterhours::Color{38, 43, 54, 255})
                   .with_auto_text_color(false)
                   .with_custom_text_color(row_col)
                   .with_render_layer(layer)
                   .with_debug_name("nav_filter"));

    div(context, mk(panel.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(8)})
            .with_render_layer(layer)
            .with_debug_name("nav_gap"));

    virtual_list(
        context, mk(panel.ent(), 3), shown.size(), ROW_H,
        [&](size_t i, afterhours::Entity &row) {
          const Row &r = all_rows[static_cast<size_t>(shown[i])];
          if (r.is_header) {
            div(context, mk(row, 0),
                ComponentConfig{}
                    .with_label(r.label)
                    .with_size(ComponentSize{percent(1.f), pixels(ROW_H)})
                    .with_alignment(TextAlignment::Left)
                    .with_custom_text_color(header_col)
                    .with_font_size(13.f)
                    .with_render_layer(layer)
                    .with_skip_tabbing(true));
            return;
          }
          const bool sel = r.screen_index == current_index;
          auto cfg = ComponentConfig{}
                         .with_label(r.label)
                         .with_size(ComponentSize{percent(1.f), pixels(ROW_H)})
                         .with_alignment(TextAlignment::Left)
                         .with_font_size(14.f)
                         .with_render_layer(layer);
          if (sel) {
            cfg.with_custom_background(sel_bg)
                .with_auto_text_color(false)
                .with_custom_text_color(sel_col);
          } else {
            cfg.with_transparent_bg()
                .with_auto_text_color(false)
                .with_custom_text_color(row_col);
          }
          if (button(context, mk(row, 1), cfg)) {
            if (on_pick) {
              auto cb = on_pick;
              const int idx = r.screen_index;
              context.defer([cb, idx]() { cb(idx); });
            }
          }
        },
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), pixels(h - 100.f)})
            .with_render_layer(layer)
            .with_debug_name("nav_list"));
  }
};
