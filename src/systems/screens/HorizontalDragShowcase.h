#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct HorizontalDragShowcase : ScreenSystem<UIContext<InputAction>> {
  inline static const std::array<std::vector<std::string>, 3> initial_lanes{{
      {"Security patch", "Data loss fix"},
      {"Refactor authentication", "Update dependencies", "Add logging"},
      {"README typo", "Rename variable"}}};
  std::array<std::vector<std::string>, 3> lanes = initial_lanes;
  std::map<afterhours::EntityID, size_t> group_to_row;
  std::string status = "Ready / 7 tags across 3 priority lanes";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    static constexpr std::array<const char *, 3> names{"High priority", "Medium priority", "Low priority"};
    static constexpr std::array<afterhours::Color, 3> accents{{
        {237, 135, 135, 255}, {232, 193, 111, 255}, {116, 202, 176, 255}}};
    auto *drag = afterhours::EntityHelper::get_singleton_cmp<DragGroupState>();
    if (drag) {
      for (const auto &event : drag->events) {
        const auto source = group_to_row.find(event.source_group);
        const auto target = group_to_row.find(event.target_group);
        if (source == group_to_row.end() || target == group_to_row.end()) continue;
        auto &from = lanes[source->second];
        auto &to = lanes[target->second];
        if (event.source_index < 0 || event.source_index >= static_cast<int>(from.size())) continue;
        auto tag = from[static_cast<size_t>(event.source_index)];
        from.erase(from.begin() + event.source_index);
        const int index = std::clamp(event.target_index, 0, static_cast<int>(to.size()));
        to.insert(to.begin() + index, tag);
        status = fmt::format("Last moved: {} / {} / position {}", tag, names[target->second], index + 1);
      }
      drag->events.clear();
    }
    group_to_row.clear();
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.background = {17, 23, 34, 255};
    context.theme.surface = {30, 40, 56, 255};
    context.theme.font = {229, 235, 245, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(1160 * s), pixels(648 * s)})
        .with_absolute_position((context.screen_width - 1160 * s) / 2,
                                (context.screen_height - 648 * s) / 2)
        .with_background(Theme::Usage::None).with_debug_name("hdrag_root"));
    const auto text = [&](int id, const std::string &label, float x, float y,
                          float w, float h, float size, afterhours::Color color) {
      return div(context, mk(root.ent(), id), box(x, y, w, h)
          .with_label(label).with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_background(Theme::Usage::None)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    };
    const afterhours::Color ink{229, 235, 245, 255}, muted{175, 191, 212, 255};
    text(0, "Horizontal Drag & Drop", 0, 0, 840, 42, 30, ink);
    text(1, "Drag between priority lanes or reorder within a lane.", 0, 50, 930, 28, 20, muted);
    text(2, "Neutral tags / lane markers show priority", 0, 82, 930, 24, 17, muted);
    if (button(context, mk(root.ent(), 3), box(964, 4, 196, 40)
        .with_label("Reset example").with_font("AtkinsonMock", pixels(20 * s))
        .with_custom_background({45, 67, 95, 255}).with_custom_text_color(ink)
        .with_corner_radius(8 * s).with_debug_name("hdrag_reset"))) {
      lanes = initial_lanes;
      status = "Reset / 7 tags restored to their starting lanes";
    }
    for (size_t lane = 0; lane < lanes.size(); ++lane) {
      const float top = 118 + static_cast<float>(lane) * 148;
      auto panel = div(context, mk(root.ent(), 10 + static_cast<int>(lane)), box(0, top, 1160, 136)
          .with_background(Theme::Usage::Surface).with_border({74, 92, 117, 255}, 1)
          .with_corner_radius(10 * s).with_debug_name("hdrag_lane_" + std::to_string(lane)));
      div(context, mk(panel.ent(), 0), box(16, 13, 4, 22).with_custom_background(accents[lane]));
      div(context, mk(panel.ent(), 1), box(30, 10, 264, 30)
          .with_label(names[lane]).with_font("AtkinsonMockBold", pixels(24 * s))
          .with_custom_text_color(ink).with_background(Theme::Usage::None));
      div(context, mk(panel.ent(), 2), box(300, 12, 42, 26)
          .with_label(std::to_string(lanes[lane].size())).with_font("AtkinsonMock", pixels(18 * s))
          .with_custom_background({47, 60, 80, 255}).with_custom_text_color(accents[lane])
          .with_corner_radius(5 * s).with_alignment(TextAlignment::Center));
      auto group = hstack(context, mk(panel.ent(), 3), box(16, 48, 1128, 76)
          .with_align_items(AlignItems::FlexStart).with_flex_wrap(FlexWrap::Wrap)
          .with_draggable_children().with_gap(pixels(8 * s))
          .with_background(Theme::Usage::None).with_debug_name("hdrag_group_" + std::to_string(lane)));
      group_to_row[group.ent().id] = lane;
      for (size_t i = 0; i < lanes[lane].size(); ++i) {
        const auto &name = lanes[lane][i];
        const float width = static_cast<float>(name.size()) * 10 + 38;
        auto tag = button(context, mk(group.ent(), static_cast<int>(i)), ComponentConfig{}
            .with_size({pixels(width * s), pixels(34 * s)}).with_label(name)
            .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(ink)
            .with_custom_background({47, 62, 82, 255}).with_border({88, 107, 132, 255}, 1)
            .with_corner_radius(6 * s).with_debug_name("tag_" + name)
            .with_on_draw_fg([s, muted](RectangleType r) {
              for (int dy = -1; dy <= 1; ++dy)
                for (int dx = 0; dx < 2; ++dx)
                  raylib::DrawCircleV({r.x + (10 + 4 * static_cast<float>(dx)) * s,
                                      r.y + r.height / 2 + 4 * static_cast<float>(dy) * s}, s, muted);
            }));
        tag.ent().get<HasLabel>().text_x_offset = 10 * s;
      }
      div(context, mk(panel.ent(), 4), box(778, 12, 366, 24)
          .with_label(lanes[lane].empty() ? "Empty lane / drop a tag here" : "Drop here or between tags")
          .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(muted)
          .with_alignment(TextAlignment::Right).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    }
    text(5, status, 0, 578, 1160, 30, 20, ink).ent()
        .addComponentIfMissing<UIComponentDebug>("hdrag_status").set("hdrag_status");
    text(6, "Fixed 8px gaps at 720p / release a tag inside a lane to move it", 0, 618, 1160, 26, 17, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(horizontal_drag, "Component Galleries",
                        "Horizontal drag and drop between priority lanes",
                        HorizontalDragShowcase)
