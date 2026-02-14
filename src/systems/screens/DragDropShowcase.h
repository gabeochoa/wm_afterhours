#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DragDropShowcase : ScreenSystem<UIContext<InputAction>> {
  // Kanban column data
  struct Card {
    std::string title;
    afterhours::Color color;
  };

  std::vector<Card> todo_items = {
      {"Design mockups", {100, 149, 237, 255}},    // Cornflower blue
      {"Write tests", {144, 190, 109, 255}},        // Soft green
      {"Fix login bug", {255, 140, 105, 255}},      // Salmon
  };

  std::vector<Card> progress_items = {
      {"API endpoints", {100, 200, 200, 255}},      // Teal
      {"DB schema", {240, 163, 100, 255}},             // Sandy orange
      {"Code review", {180, 150, 210, 255}},         // Lavender
  };

  std::vector<Card> done_items = {
      {"Setup CI", {100, 200, 150, 255}},            // Mint
      {"Update docs", {255, 200, 87, 255}},          // Gold
  };

  void render_column_header(UIContext<InputAction> &context,
                            afterhours::Entity &parent, int id,
                            const std::string &title, int count,
                            const Theme &theme) {
    // Column title with inline count
    div(context, mk(parent, id),
        ComponentConfig{}
            .with_label(fmt::format("{} ({})", title, count))
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, h720(40.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name(title + "_header"));
  }

  void render_card(UIContext<InputAction> &context,
                   afterhours::Entity &parent, int id, const Card &card,
                   const Theme &theme) {
    // Card with left color accent border
    div(context, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(70)})
            .with_custom_background(
                afterhours::Color{62, 65, 76, 255})
            .with_roundness(0.08f)
            .with_padding(Padding{
                .left = DefaultSpacing::small(),
                .right = DefaultSpacing::small()})
            .with_border(afterhours::Color{75, 78, 90, 255}, 1.0f)
            .with_label(card.title)
            .with_custom_text_color(afterhours::Color{235, 235, 240, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(34.0f))
            .with_skip_tabbing(true)
            .with_debug_name("card_" + card.title));
  }

  // Map from drag_group entity ID -> column index (populated each frame)
  std::map<afterhours::EntityID, int> group_to_column;

  std::vector<Card> &column_data(int col) {
    switch (col) {
    case 0: return todo_items;
    case 1: return progress_items;
    case 2: return done_items;
    default: return todo_items;
    }
  }

  void render_column(UIContext<InputAction> &context,
                     afterhours::Entity &parent, int id,
                     const std::string &title,
                     std::vector<Card> &items, const Theme &theme,
                     int column_index) {
    // Column container
    auto column =
        div(context, mk(parent, id),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.30f), percent(1.0f)})
                .with_custom_background(
                    afterhours::Color{42, 44, 54, 255})
                .with_roundness(0.04f)
                .with_border(afterhours::Color{58, 60, 72, 255}, 1.0f)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("column_" + title));

    render_column_header(context, column.ent(), 0, title,
                         static_cast<int>(items.size()), theme);

    // Cards area — draggable children enables drag-and-drop reordering
    auto cards_area =
        div(context, mk(column.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), children()})
                .with_flex_direction(FlexDirection::Column)
                .with_draggable_children()
                .with_debug_name("cards_" + title));

    // Register entity ID -> column mapping for event dispatch
    group_to_column[cards_area.ent().id] = column_index;

    for (int i = 0; i < static_cast<int>(items.size()); i++) {
      render_card(context, cards_area.ent(), i, items[i], theme);
    }

    // Empty state placeholder - uses same height as a card
    if (items.empty()) {
      auto empty_box = div(context, mk(cards_area.ent(), 999),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(60)})
              .with_custom_background(
                  afterhours::Color{45, 47, 55, 255})
              .with_roundness(0.06f)
              .with_border(afterhours::Color{70, 72, 80, 100}, 1.0f)
              .with_flex_direction(FlexDirection::Row)
              .with_align_items(AlignItems::Center)
              .with_justify_content(JustifyContent::Center)
              .with_skip_tabbing(true));

      div(context, mk(empty_box.ent(), 0),
          ComponentConfig{}
              .with_label("Drop items here")
              .with_size(ComponentSize{children(), pixels(48)})
              .with_custom_text_color(
                  afterhours::Color{120, 120, 135, 255})
              .with_font(UIComponent::DEFAULT_FONT, h720(30.0f))
              .with_skip_tabbing(true));
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // --- Consume drag-and-drop events first ---
    auto *drag_state =
        afterhours::EntityHelper::get_singleton_cmp<DragGroupState>();
    if (drag_state) {
      for (auto &evt : drag_state->events) {
        auto src_it = group_to_column.find(evt.source_group);
        auto tgt_it = group_to_column.find(evt.target_group);
        if (src_it == group_to_column.end() ||
            tgt_it == group_to_column.end())
          continue;

        auto &src = column_data(src_it->second);
        auto &tgt = column_data(tgt_it->second);

        if (evt.source_index < 0 ||
            evt.source_index >= static_cast<int>(src.size()))
          continue;

        Card card = src[evt.source_index];
        src.erase(src.begin() + evt.source_index);

        int insert_at = evt.target_index;
        // When moving within the same column, account for removal offset.
        if (src_it->second == tgt_it->second &&
            evt.source_index < insert_at) {
          insert_at--;
        }
        insert_at = std::clamp(insert_at, 0, static_cast<int>(tgt.size()));
        tgt.insert(tgt.begin() + insert_at, card);
      }
      drag_state->events.clear();
    }

    // --- Build UI ---
    group_to_column.clear();

    Theme theme;
    theme.background = afterhours::Color{24, 26, 32, 255};
    theme.surface = afterhours::Color{35, 37, 44, 255};
    theme.primary = afterhours::Color{90, 120, 200, 255};
    theme.secondary = afterhours::Color{55, 57, 65, 255};
    theme.accent = afterhours::Color{100, 200, 150, 255};
    theme.font = afterhours::Color{230, 230, 235, 255};
    theme.font_muted = afterhours::Color{140, 140, 150, 255};
    context.theme = theme;

    // Root container
    auto root =
        div(context, mk(entity, 100),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.02f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("drag_drop_root"));

    // Title bar
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
            .with_label("Drag & Drop Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, h720(48.0f))
            .with_skip_tabbing(true));

    div(context, mk(title_bar.ent(), 1),
        ComponentConfig{}
            .with_label("Drag cards between columns to organize")
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_custom_text_color(
                afterhours::Color{185, 185, 195, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_skip_tabbing(true));

    // Columns container
    auto columns =
        div(context, mk(root.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.85f)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_no_wrap()
                .with_debug_name("columns_container"));

    render_column(context, columns.ent(), 0, "To Do", todo_items, theme, 0);
    render_column(context, columns.ent(), 1, "In Progress", progress_items,
                  theme, 1);
    render_column(context, columns.ent(), 2, "Done", done_items, theme, 2);
  }
};

REGISTER_EXAMPLE_SCREEN(drag_drop, "Component Galleries",
                        "Drag and drop between columns with reordering",
                        DragDropShowcase)
