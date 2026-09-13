#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleLayout : ScreenSystem<UIContext<InputAction>> {
  int selected_item = -1;
  std::string last_layout;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply sage natural theme for this screen
    auto theme = afterhours::ui::theme_presets::sage_natural();
    context.theme = theme;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float ox = (context.screen_width - 1280 * s) / 2;
    const float oy = (context.screen_height - 720 * s) / 2;
    const auto ink = afterhours::Color{40, 61, 49, 255};
    const auto muted = afterhours::Color{78, 99, 83, 255};
    const std::array<afterhours::Color, 3> colors{theme.primary, theme.secondary, theme.accent};
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
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({184, 205, 178, 255}).with_corner_radius(0)
        .with_debug_name("layout_canvas"));
    div(context, mk(entity, 1), at(48, 24, 1184, 672)
        .with_custom_background(theme.background).with_corner_radius(12 * s)
        .with_debug_name("layout_main"));
    text(2, "Layout System Demo", 72, 34, 1136, 43, 32, ink, "layout_title");
    text(3, "Row and column layouts", 72, 82, 700, 27, 21, muted, "layout_subtitle");
    for (int i = 0; i < 3; ++i) {
      const float x = 842 + static_cast<float>(i) * 122;
      div(context, mk(entity, 10 + i), at(x, 88, 16, 16)
          .with_custom_background(colors[i]).with_corner_radius(3 * s));
      text(13 + i, fmt::format("Item {}", i + 1), x + 24, 82, 94, 27,
           18, muted, fmt::format("layout_key_{}", i));
    }
    text(20, "Row / centered children", 72, 121, 1136, 28, 23, ink, "layout_row_heading");
    // Row layout demo - JustifyContent::Center to keep panels within container
    auto row_container = hstack(context, mk(entity, 21), at(72, 156, 1136, 120)
        .with_custom_background(theme.surface).with_corner_radius(10 * s)
        .with_padding(Padding::all(pixels(12 * s)))
        .with_justify_content(JustifyContent::Center).with_align_items(AlignItems::Center)
        .with_debug_name("row_container"));
    std::array<RectangleType, 3> row_rects{};
    for (int i = 0; i < 3; ++i) {
      auto result = button(context, mk(row_container.ent(), i), ComponentConfig{}
          .with_size({pixels(180), pixels(80 * s)})
          .with_label(fmt::format("Item {}", i + 1)).with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background(colors[i]).with_custom_text_color({255, 255, 244, 255})
          .with_alignment(TextAlignment::Center).with_text_inset(12 * s)
          .with_corner_radius(8 * s)
          .with_border(selected_item == i ? ink : colors[i], 2 * s)
          .with_margin(Margin{.left = pixels(28 * s), .right = pixels(28 * s)})
          .with_debug_name(fmt::format("layout_row_item_{}", i)));
      row_rects[i] = result.cmp().rect();
      if (!result) continue;
      selected_item = i;
      last_layout = "Row";
    }
    const auto first = row_rects[0];
    const float margin = row_container.cmp().children.empty() ? 0.f :
        UICollectionHolder::getEntityForIDEnforce(row_container.cmp().children[0])
            .get<UIComponent>().computed_margin[Axis::left];
    if (first.width > 0) {
      div(context, mk(entity, 22), ComponentConfig{}
          .with_size({pixels(first.width + 2 * margin), pixels(first.height)})
          .with_absolute_position(first.x - margin, first.y)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            const raylib::Color tint{142, 170, 124, 74};
            raylib::DrawRectangleRec({r.x, r.y, margin, r.height}, tint);
            raylib::DrawRectangleRec({r.x + r.width - margin, r.y, margin, r.height}, tint);
            raylib::DrawLineEx({r.x, r.y + r.height / 2},
                               {r.x + margin, r.y + r.height / 2}, s, {83, 113, 66, 255});
            raylib::DrawLineEx({r.x + r.width - margin, r.y + r.height / 2},
                               {r.x + r.width, r.y + r.height / 2}, s, {83, 113, 66, 255});
          }).with_debug_name("layout_margin_guide"));
    }
    const float row_gap = row_rects[1].x - first.x - first.width;
    text(23, fmt::format("Row: left to right. Item width 180 px; {:.0f} px side margins make {:.0f} px gaps.", margin, row_gap),
         72, 282, 1136, 27, 18, muted, "layout_row_description");
    // Separator between row demo and column demo
    div(context, mk(entity, 24), at(72, 317, 1136, 1)
        .with_custom_background({144, 165, 136, 255}).with_debug_name("section_separator_1"));
    text(30, "Column / stacked children", 72, 330, 850, 28, 23, ink, "layout_column_heading");
    text(31, "Parent frame", 1004, 334, 204, 24, 18, muted, "layout_parent_label");
    // Column layout demo - stacked items demonstration
    auto col_container = hstack(context, mk(entity, 32), at(72, 364, 1136, 220)
        .with_custom_background(theme.surface).with_corner_radius(10 * s)
        .with_padding(Padding::all(pixels(20 * s)))
        .with_justify_content(JustifyContent::Center).with_debug_name("col_demo_container"));
    // Column with stacked items - centered within container
    auto column = vstack(context, mk(col_container.ent(), 0), ComponentConfig{}
        .with_size({percent(.60f), percent(1)})
        .with_custom_background({226, 232, 221, 255})
        .with_padding(Padding::all(pixels(8 * s))).with_corner_radius(8 * s)
        .with_align_items(AlignItems::Center).with_no_wrap().with_debug_name("stacked_column"));
    std::array<RectangleType, 3> column_rects{};
    // Stacked items - use percent of parent column
    for (int i = 0; i < 3; ++i) {
      auto result = button(context, mk(column.ent(), i), ComponentConfig{}
          .with_size({percent(.95f), pixels(40)})
          .with_label(fmt::format("Item {}", i + 1)).with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background(colors[i]).with_custom_text_color({255, 255, 244, 255})
          .with_alignment(TextAlignment::Center).with_text_inset(12 * s, 2 * s)
          .with_corner_radius(7 * s).with_border(selected_item == i ? ink : colors[i], 2 * s)
          .with_margin(Margin{.bottom = pixels(i == 2 ? 0 : 3)})
          .with_debug_name(fmt::format("layout_column_item_{}", i)));
      column_rects[i] = result.cmp().rect();
      if (!result) continue;
      selected_item = i;
      last_layout = "Column";
    }
    text(33, "Child stack", 84, 400, 190, 29, 20, ink, "layout_child_label");
    text(34, "60% of the\nparent content", 84, 436, 190, 54, 17, muted, "layout_child_width");
    const auto top = column_rects[0];
    const float column_gap = column_rects[1].y - top.y - top.height;
    const auto stack_bounds = column.cmp().rect();
    if (top.height > 0) {
      div(context, mk(entity, 35), ComponentConfig{}
          .with_size({pixels(12 * s), pixels(column_gap)})
          .with_absolute_position(stack_bounds.x + stack_bounds.width + 12 * s, top.y + top.height)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            const raylib::Color color{70, 103, 61, 255};
            raylib::DrawLineEx({r.x + r.width / 2, r.y},
                               {r.x + r.width / 2, r.y + r.height}, s, color);
            raylib::DrawLineEx({r.x, r.y}, {r.x + r.width, r.y}, s, color);
            raylib::DrawLineEx({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, s, color);
          }).with_debug_name("layout_column_gap_guide"));
    }
    text(36, fmt::format("{:.0f} px gap", column_gap), 1010, 425, 186, 28,
         19, ink, "layout_column_gap");
    text(37, "Column: top to bottom. Each item is 95% of the stack's content width and 40 px high.",
         72, 590, 1136, 27, 18, muted, "layout_column_description");
    // Separator between column demo and description
    div(context, mk(entity, 38), at(72, 624, 1136, 1)
        .with_custom_background({144, 165, 136, 255}).with_debug_name("section_separator_2"));
    text(40, fmt::format("Sage Natural theme  /  Viewport {:.0f} x {:.0f}  /  Parent widths: row {:.0f} px, column {:.0f} px",
         context.screen_width, context.screen_height, row_container.cmp().rect().width, col_container.cmp().rect().width),
         72, 633, 1136, 26, 18, muted, "layout_metadata");
    text(41, selected_item < 0 ? "Select any item to highlight its match in the other layout."
                             : fmt::format("Selected: Item {} from {}. Colors identify the same items in both arrangements.", selected_item + 1, last_layout),
         72, 663, 1136, 25, 18, ink, "layout_selection");
  }
};

REGISTER_EXAMPLE_SCREEN(layout, "System Demos",
                        "Layout system example with containers", ExampleLayout)
