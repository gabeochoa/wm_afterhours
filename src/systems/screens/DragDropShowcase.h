#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include "HorizontalDragShowcase.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DragDropShowcase : ScreenSystem<UIContext<InputAction>> {
  HorizontalDragShowcase lanes_demo;
  bool show_lanes = false;

  // Kanban column data
  struct Card {
    int id;
    std::string title;
    std::string type;
    std::string priority;
    std::string assignee;
    afterhours::Color color;
  };

  std::vector<Card> todo_items = {
      {1, "Design mockups", "Design", "Medium", "AL", {100, 149, 237, 255}}, // Cornflower blue
      {2, "Write tests", "Quality", "Medium", "JM", {144, 190, 109, 255}},    // Soft green
      {3, "Fix login bug", "Bug", "High", "RK", {255, 140, 105, 255}},  // Salmon
  };

  std::vector<Card> progress_items = {
      {4, "API endpoints", "Backend", "High", "AL", {100, 200, 200, 255}}, // Teal
      {5, "DB schema", "Backend", "Medium", "JM", {240, 163, 100, 255}},     // Sandy orange
      {6, "Code review", "Review", "Low", "RK", {180, 150, 210, 255}},   // Lavender
  };

  std::vector<Card> done_items = {
      {7, "Setup CI", "Tooling", "Medium", "AL", {100, 200, 150, 255}},   // Mint
      {8, "Update docs", "Docs", "Low", "JM", {255, 200, 87, 255}}, // Gold
  };

  // Map from drag_group entity ID -> column index (populated each frame)
  std::map<afterhours::EntityID, int> group_to_column;
  const std::array<std::string, 3> column_names{"To do", "In progress", "Done"};
  int selected_task = 1;
  std::string last_move = "Board ready. Drag a card or use Move task below.";
  std::array<int, 3> first_visible{};
  std::map<afterhours::EntityID, int> group_to_first;
  static constexpr int visible_rows = 3;

  std::vector<Card> &column_data(int col) {
    if (col == 1) return progress_items;
    if (col == 2) return done_items;
    return todo_items;
  }

  void move_card(int source, int index, int target, int insert_at) {
    auto &src = column_data(source);
    if (index < 0 || index >= static_cast<int>(src.size())) return;
    auto &dst = column_data(target);
    Card card = src[index];
    src.erase(src.begin() + index);
    insert_at = std::clamp(insert_at, 0, static_cast<int>(dst.size()));
    dst.insert(dst.begin() + insert_at, card);
    selected_task = card.id;
    first_visible[target] = std::clamp(first_visible[target],
        std::max(0, insert_at - visible_rows + 1), insert_at);
    last_move = fmt::format("SPR-{:02d} / {} moved to {} / position {}.",
                            card.id, card.title, column_names[target], insert_at + 1);
  }

  void reset_board() {
    std::vector<Card> cards;
    for (int col = 0; col < 3; ++col) {
      auto &items = column_data(col);
      cards.insert(cards.end(), items.begin(), items.end());
      items.clear();
    }
    std::sort(cards.begin(), cards.end(), [](const Card &a, const Card &b) { return a.id < b.id; });
    for (const auto &card : cards) column_data(card.id <= 3 ? 0 : card.id <= 6 ? 1 : 2).push_back(card);
    selected_task = 1;
    last_move = "Board reset. Original task order restored.";
    first_visible.fill(0);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    if (show_lanes) lanes_demo.for_each_with(entity, context, dt);
    else draw_board(entity, context);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1160 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const auto *drag = afterhours::EntityHelper::get_singleton_cmp<DragGroupState>();
    const std::array<const char *, 2> modes{"Board", "Lanes"};
    for (size_t mode = 0; mode < modes.size(); ++mode) {
      const bool active = show_lanes == (mode == 1);
      if (button(context, mk(entity, 1000 + static_cast<int>(mode)), ComponentConfig{}
          .with_size({pixels(118 * s), pixels(38 * s)})
          .with_absolute_position(left + (700 + static_cast<float>(mode) * 126) * s, top + 25 * s)
          .with_label(modes[mode]).with_font("AtkinsonMock", pixels(18 * s))
          .with_custom_background(active ? afterhours::Color{72, 111, 176, 255}
                                         : afterhours::Color{42, 52, 69, 255})
          .with_custom_text_color({237, 243, 251, 255}).with_corner_radius(8 * s)
          .with_disabled(drag && drag->dragging).with_render_layer(10)
          .with_debug_name(mode == 0 ? "drag_mode_board" : "drag_mode_lanes")))
        show_lanes = mode == 1;
    }
  }

  void draw_board(afterhours::Entity &entity, UIContext<InputAction> &context) {
    // --- Consume drag-and-drop events first ---
    auto *drag_state = afterhours::EntityHelper::get_singleton_cmp<DragGroupState>();
    if (drag_state) {
      for (const auto &evt : drag_state->events) {
        const auto src = group_to_column.find(evt.source_group);
        const auto dst = group_to_column.find(evt.target_group);
        if (src == group_to_column.end() || dst == group_to_column.end()) continue;
        move_card(src->second, group_to_first.at(evt.source_group) + evt.source_index,
                  dst->second, group_to_first.at(evt.target_group) + evt.target_index);
      }
      drag_state->events.clear();
    }
    const bool dragging = drag_state && drag_state->dragging;

    // --- Build UI ---
    group_to_column.clear();
    group_to_first.clear();
    Theme theme;
    theme.background = {20, 25, 35, 255};
    theme.surface = {29, 36, 49, 255};
    theme.primary = {72, 111, 176, 255};
    theme.secondary = {42, 52, 69, 255};
    theme.accent = {126, 209, 166, 255};
    theme.font = {237, 243, 251, 255};
    theme.font_muted = {170, 185, 206, 255};
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1160 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_background(Theme::Usage::None).with_skip_grid_snap(true);
    };
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bg"));
    // Root container
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1160, 720)
        .with_absolute_position(left, top).with_debug_name("drag_drop_root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float font_size = 18.f,
                           bool strong = false, const std::string &name = "") {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(font_size * s))
          .with_custom_text_color(strong ? theme.font : theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto action = [&](int id, const std::string &value, float x, float y, float w,
                            const std::string &name, bool disabled = false, float height = 38.f) {
      return button(context, mk(root.ent(), id), box(x, y, w, height).with_label(value)
          .with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
          .with_background(Theme::Usage::Secondary).with_custom_text_color(theme.font)
          .with_corner_radius(8 * s).with_disabled(disabled).with_debug_name(name));
    };
    // Title bar
    label(root.ent(), 1, "Sprint board", 0, 20, 700, 43, 34, true);
    label(root.ent(), 2, "Sprint 12 / 07-18 September 2026", 0, 67, 750, 27, 20);
    label(root.ent(), 3, "Drag visible cards. Use the wheel or column arrows to page through additional rows.",
          0, 102, 1136, 25, 18);
    if (action(4, "Reset board", 1000, 25, 160, "board_reset", dragging)) reset_board();
    label(root.ent(), 5, fmt::format("{} of 8 completed", done_items.size()),
          868, 69, 292, 26, 20, true, "board_progress");

    int selected_column = 0;
    int selected_index = 0;
    std::string selected_title;
    for (int col = 0; col < 3; ++col) {
      const auto &items = column_data(col);
      for (int index = 0; index < static_cast<int>(items.size()); ++index) {
        if (items[index].id != selected_task) continue;
        selected_column = col;
        selected_index = index;
        selected_title = items[index].title;
      }
    }
    const std::array<afterhours::Color, 3> accents{{{136, 173, 233, 255}, {238, 191, 116, 255}, {126, 209, 166, 255}}};
    const auto wheel = afterhours::input::get_mouse_wheel_move_v();
    // Columns container
    auto columns = div(context, mk(root.ent(), 6), box(0, 142, 1160, 422).with_debug_name("columns_container"));
    for (int col = 0; col < 3; ++col) {
      const auto &items = column_data(col);
      const float x = col * 392.f;
      const int last_start = std::max(0, static_cast<int>(items.size()) - visible_rows);
      first_visible[col] = std::clamp(first_visible[col], 0, last_start);
      const auto mouse = context.mouse.pos;
      const float vx = left + (x + 14) * s;
      const float vy = top + 200 * s;
      const bool inside = mouse.x >= vx && mouse.x <= vx + 348 * s &&
                          mouse.y >= vy && mouse.y <= vy + 328 * s;
      if (!dragging && inside && std::abs(wheel.y) > 0.f) {
        const int rows = std::max(1, static_cast<int>(std::ceil(std::abs(wheel.y))));
        first_visible[col] = std::clamp(first_visible[col] + (wheel.y < 0 ? rows : -rows), 0, last_start);
      }
      // Column container
      auto column = div(context, mk(columns.ent(), col), box(x, 0, 376, 422)
          .with_custom_background(theme.surface).with_corner_radius(12 * s)
          .with_border({66, 80, 103, 255}, 1.f).with_debug_name("column_" + std::to_string(col)));
      div(context, mk(column.ent(), 0), box(16, 17, 4, 24).with_custom_background(accents[col]));
      label(column.ent(), 1, column_names[col], 28, 12, 258, 34, 25, true);
      div(context, mk(column.ent(), 2), box(318, 14, 40, 30).with_label(std::to_string(items.size()))
          .with_alignment(TextAlignment::Center).with_font("AtkinsonMock", pixels(19 * s))
          .with_custom_background(theme.secondary).with_custom_text_color(accents[col])
          .with_corner_radius(8 * s).with_debug_name("count_" + std::to_string(col)));
      const auto page_button = [&](int id, const std::string &text, float bx,
                                   bool disabled, const std::string &name) {
        return button(context, mk(column.ent(), id), box(bx, 388, 60, 28).with_label(text)
            .with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
            .with_background(Theme::Usage::Secondary).with_custom_text_color(theme.font)
            .with_corner_radius(6 * s).with_disabled(dragging || disabled).with_debug_name(name));
      };
      if (page_button(5, "Up", 232, first_visible[col] == 0, "page_up_" + std::to_string(col)))
        --first_visible[col];
      if (page_button(6, "Down", 298, first_visible[col] == last_start, "page_down_" + std::to_string(col)))
        ++first_visible[col];
      auto cards_area = vstack(context, mk(column.ent(), 3), box(14, 58, 348, 328)
          .with_draggable_children().with_gap(pixels(12 * s)).with_no_wrap().with_clip_children(true)
          .with_debug_name("cards_" + std::to_string(col)));
      if (!dragging && !inside)
        cards_area.ent().disableTag(DragTag::Group);
      // Register entity ID -> column mapping for event dispatch
      group_to_column[cards_area.ent().id] = col;
      group_to_first[cards_area.ent().id] = first_visible[col];
      const int visible_end = std::min(first_visible[col] + visible_rows, static_cast<int>(items.size()));
      for (int index = first_visible[col]; index < visible_end; ++index) {
        const auto &card = items[index];
        // Card with left color accent border
        auto card_ui = div(context, mk(cards_area.ent(), card.id), ComponentConfig{}
            .with_size({pixels(334 * s), pixels(96 * s)}).with_skip_grid_snap(true)
            .with_custom_background({40, 49, 66, 255}).with_corner_radius(8 * s)
            .with_border({77, 93, 118, 255}, 1.f).with_label(card.title)
            .with_font("AtkinsonMock", pixels(23 * s)).with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center).with_skip_tabbing(true)
            .with_debug_name("task_" + std::to_string(card.id)));
        div(context, mk(card_ui.ent(), 0), box(0, 12, 3, 72).with_custom_background(card.color).with_ignore_pointer_events());
        label(card_ui.ent(), 1, fmt::format("SPR-{:02d} / {}", card.id, card.type), 16, 8, 228, 23, 16);
        label(card_ui.ent(), 2, col == 2 ? "Complete" : card.priority + " priority", 16, 68, 215, 21, 16);
        div(context, mk(card_ui.ent(), 3), box(281, 62, 34, 26).with_label(card.assignee)
            .with_font("AtkinsonMock", pixels(16 * s)).with_alignment(TextAlignment::Center)
            .with_custom_background(theme.secondary).with_custom_text_color(theme.font)
            .with_corner_radius(7 * s).with_ignore_pointer_events());
        div(context, mk(card_ui.ent(), 4), box(14, 40, 14, 17).with_ignore_pointer_events()
            .with_on_draw_fg([s](RectangleType r) {
              for (int row = 0; row < 3; ++row)
                for (int dot = 0; dot < 2; ++dot)
                  afterhours::draw_rectangle({r.x + dot * 6 * s, r.y + row * 6 * s, 2 * s, 2 * s}, {152, 171, 198, 255});
            }));
      }
      const float cue_y = items.empty() ? 88.f : 392.f;
      label(column.ent(), 4, items.empty() ? "Drop a card into this column" :
          fmt::format("Rows {}-{} of {}", first_visible[col] + 1, visible_end, items.size()),
          26, cue_y, items.empty() ? 322.f : 202.f, 22, 16, false, "visible_rows_" + std::to_string(col));
    }
    div(context, mk(root.ent(), 7), box(0, 580, 1160, 76).with_custom_background(theme.surface).with_corner_radius(12 * s));
    label(root.ent(), 8, "Move task", 12, 585, 152, 26, 18, true);
    label(root.ent(), 13, fmt::format("Row {} of {}", selected_index + 1, column_data(selected_column).size()),
          152, 585, 132, 26, 16, false, "selected_position");
    if (action(14, "Earlier", 292, 582, 82, "task_earlier", dragging || selected_index == 0, 28))
      move_card(selected_column, selected_index, selected_column, selected_index - 1);
    if (action(15, "Later", 382, 582, 82, "task_later", dragging || selected_index + 1 >= static_cast<int>(column_data(selected_column).size()), 28))
      move_card(selected_column, selected_index, selected_column, selected_index + 1);
    label(root.ent(), 9, fmt::format("SPR-{:02d} / {}", selected_task, selected_title), 12, 613, 450, 32, 20, true, "selected_task");
    if (action(10, "<", 468, 604, 42, "previous_task", dragging)) selected_task = selected_task == 1 ? 8 : selected_task - 1;
    if (action(11, ">", 518, 604, 42, "next_task", dragging)) selected_task = selected_task == 8 ? 1 : selected_task + 1;
    label(root.ent(), 12, "Move to", 580, 586, 180, 23, 16);
    for (int col = 0; col < 3; ++col) {
      if (!action(20 + col, column_names[col], 586 + col * 184.f, 609, 172,
                  "move_to_" + std::to_string(col), dragging || selected_column == col)) continue;
      move_card(selected_column, selected_index, col, static_cast<int>(column_data(col).size()));
    }
    std::string status = last_move;
    if (dragging) {
      status = "Dragging / release in a column to move. The gap marks the insertion position.";
      const auto hover = find_drag_tagged(DragTag::HoverGroup);
      if (hover && group_to_column.contains(hover.asE().id))
        status = fmt::format("Drop in {} / position {}. Release to move.",
                            column_names[group_to_column.at(hover.asE().id)],
                            group_to_first.at(hover.asE().id) + drag_state->hover_index + 1);
    }
    label(root.ent(), 30, status, 0, 666, 1160, 26, 18, false, "board_status");
    label(root.ent(), 31, "Keyboard: Tab and Enter activate controls. Earlier / Later reorder the selected task across pages.",
          0, 696, 1160, 20, 16);
  }
};

REGISTER_EXAMPLE_SCREEN(drag_drop, "Component Galleries",
                        "Drag and drop in boards and horizontal priority lanes",
                        DragDropShowcase)
