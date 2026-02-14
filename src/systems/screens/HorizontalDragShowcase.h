#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct HorizontalDragShowcase : ScreenSystem<UIContext<InputAction>> {
  struct Tag {
    std::string label;
    afterhours::Color color;
  };

  std::vector<Tag> high_items = {
      {"Security patch", {220, 80, 80, 255}},
      {"Data loss fix", {200, 60, 100, 255}},
  };

  std::vector<Tag> medium_items = {
      {"Refactor auth", {240, 163, 100, 255}},
      {"Update deps", {200, 180, 80, 255}},
      {"Add logging", {180, 150, 210, 255}},
  };

  std::vector<Tag> low_items = {
      {"Readme typo", {100, 180, 160, 255}},
      {"Rename var", {130, 160, 200, 255}},
  };

  // Map from drag_group entity ID -> row index (populated each frame)
  std::map<afterhours::EntityID, int> group_to_row;

  std::vector<Tag> &row_data(int row) {
    switch (row) {
    case 0:
      return high_items;
    case 1:
      return medium_items;
    case 2:
      return low_items;
    default:
      return low_items;
    }
  }

  void render_tag(UIContext<InputAction> &context,
                  afterhours::Entity &parent, int id, const Tag &tag) {
    div(context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{children(), pixels(44)})
            .with_custom_background(tag.color)
            .with_roundness(0.3f)
            .with_padding(Padding{.left = DefaultSpacing::small(),
                                  .right = DefaultSpacing::small()})
            .with_margin(Margin{.right = DefaultSpacing::small()})
            .with_label(tag.label)
            .with_custom_text_color(afterhours::Color{20, 20, 25, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_skip_tabbing(true)
            .with_debug_name("tag_" + tag.label));
  }

  void render_row(UIContext<InputAction> &context,
                  afterhours::Entity &parent, int id,
                  const std::string &label, std::vector<Tag> &items,
                  afterhours::Color accent, const Theme &theme,
                  int row_index) {
    // Row container
    auto row = div(context, mk(parent, id),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.0f), children()})
                       .with_custom_background(afterhours::Color{38, 40, 50, 255})
                       .with_roundness(0.03f)
                       .with_border(afterhours::Color{55, 57, 68, 255}, 1.0f)
                       .with_padding(Spacing::sm)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_flex_direction(FlexDirection::Column)
                       .with_debug_name("row_" + label));

    // Row header
    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_label(
                fmt::format("{} ({})", label, static_cast<int>(items.size())))
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_text_color(accent)
            .with_font(UIComponent::DEFAULT_FONT, h720(36.0f))
            .with_skip_tabbing(true)
            .with_debug_name(label + "_header"));

    // Horizontal drag group for the tags
    auto tags_area =
        hstack(context, mk(row.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), children()})
                .with_align_items(AlignItems::Center)
                .with_flex_wrap(FlexWrap::Wrap)
                .with_draggable_children()
                .with_debug_name("tags_" + label));

    group_to_row[tags_area.ent().id] = row_index;

    for (int i = 0; i < static_cast<int>(items.size()); i++) {
      render_tag(context, tags_area.ent(), i, items[i]);
    }

    // Empty state
    if (items.empty()) {
      div(context, mk(tags_area.ent(), 999),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(200), pixels(44)})
              .with_custom_background(afterhours::Color{45, 47, 55, 255})
              .with_roundness(0.3f)
              .with_label("Drop tags here")
              .with_custom_text_color(afterhours::Color{100, 100, 115, 255})
              .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
              .with_skip_tabbing(true));
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // --- Consume drag events ---
    auto *drag_state =
        afterhours::EntityHelper::get_singleton_cmp<DragGroupState>();
    if (drag_state) {
      for (auto &evt : drag_state->events) {
        auto src_it = group_to_row.find(evt.source_group);
        auto tgt_it = group_to_row.find(evt.target_group);
        if (src_it == group_to_row.end() || tgt_it == group_to_row.end())
          continue;

        auto &src = row_data(src_it->second);
        auto &tgt = row_data(tgt_it->second);

        if (evt.source_index < 0 ||
            evt.source_index >= static_cast<int>(src.size()))
          continue;

        Tag tag = src[evt.source_index];
        src.erase(src.begin() + evt.source_index);

        int insert_at = evt.target_index;
        if (src_it->second == tgt_it->second &&
            evt.source_index < insert_at) {
          insert_at--;
        }
        insert_at = std::clamp(insert_at, 0, static_cast<int>(tgt.size()));
        tgt.insert(tgt.begin() + insert_at, tag);
      }
      drag_state->events.clear();
    }

    // --- Build UI ---
    group_to_row.clear();

    Theme theme;
    theme.background = afterhours::Color{24, 26, 32, 255};
    theme.surface = afterhours::Color{35, 37, 44, 255};
    theme.primary = afterhours::Color{90, 120, 200, 255};
    theme.secondary = afterhours::Color{55, 57, 65, 255};
    theme.accent = afterhours::Color{100, 200, 150, 255};
    theme.font = afterhours::Color{230, 230, 235, 255};
    theme.font_muted = afterhours::Color{140, 140, 150, 255};
    context.theme = theme;

    // Root
    auto root =
        div(context, mk(entity, 100),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.92f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.02f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("hdrag_root"));

    // Title
    auto title_bar =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(80)})
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(JustifyContent::Center)
                .with_margin(Margin{.bottom = DefaultSpacing::small()})
                .with_debug_name("title_bar"));

    div(context, mk(title_bar.ent(), 0),
        ComponentConfig{}
            .with_label("Horizontal Drag & Drop")
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, h720(48.0f))
            .with_skip_tabbing(true));

    div(context, mk(title_bar.ent(), 1),
        ComponentConfig{}
            .with_label("Drag tags between priority lanes")
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_custom_text_color(afterhours::Color{185, 185, 195, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_skip_tabbing(true));

    // Rows container
    auto rows =
        div(context, mk(root.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), children()})
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("rows_container"));

    render_row(context, rows.ent(), 0, "High Priority", high_items,
               afterhours::Color{220, 80, 80, 255}, theme, 0);
    render_row(context, rows.ent(), 1, "Medium Priority", medium_items,
               afterhours::Color{240, 180, 60, 255}, theme, 1);
    render_row(context, rows.ent(), 2, "Low Priority", low_items,
               afterhours::Color{100, 180, 160, 255}, theme, 2);
  }
};

REGISTER_EXAMPLE_SCREEN(horizontal_drag, "Component Galleries",
                        "Horizontal drag and drop between priority lanes",
                        HorizontalDragShowcase)
