#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleFlexAlignment : ScreenSystem<UIContext<InputAction>> {
  size_t active_category = 0;
  std::array<std::string_view, 6> category_labels = {
      "Overview", "Vertical", "Horizontal", "Cross-axis", "Self align", "Gap"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.background = {23, 29, 41, 255};
    theme.surface = {36, 45, 62, 255};
    theme.font = {239, 244, 250, 255};
    theme.font_muted = {175, 192, 215, 255};
    theme.accent = {122, 218, 224, 255};
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
    // Full screen root
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1160, 720)
        .with_absolute_position(left, top).with_debug_name("root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size = 18.f,
                           bool strong = false, const std::string &name = "") {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(strong ? theme.font : theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    // Title row
    label(root.ent(), 0, "Flexbox alignment", 0, 18, 1160, 44, 34, true);
    label(root.ent(), 1, "Within each set, only the layout rule changes. Dimensions use 720p design pixels.",
          0, 70, 1160, 27, 20);
    // Use tab_container for category selection
    tab_container(context, mk(root.ent(), 2), category_labels, active_category,
        box(0, 111, 1160, 38).with_font("AtkinsonMock", pixels(20 * s))
            .with_alignment(TextAlignment::Center).with_debug_name("category_tabs"));
    const std::array<JustifyContent, 5> justify{JustifyContent::FlexStart, JustifyContent::FlexEnd,
        JustifyContent::Center, JustifyContent::SpaceBetween, JustifyContent::SpaceAround};
    const std::array<std::string, 5> justify_names{"Start", "End", "Center", "Space between", "Space around"};
    const std::array<AlignItems, 3> aligns{AlignItems::FlexStart, AlignItems::Center, AlignItems::FlexEnd};
    const std::array<std::string, 3> align_names{"Start", "Center", "End"};
    const auto section = [&](int id, const std::string &title, const std::string &api,
                             float x, float y, float w) {
      label(root.ent(), id, title, x, y, w, 31, 25, true);
      label(root.ent(), id + 1, api, x, y + 35, w, 24, 17);
    };
    const auto axis = [&](int id, float x, float y, float w, float h, bool vertical) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_ignore_pointer_events()
          .with_on_draw_fg([vertical, s](RectangleType r) {
            const raylib::Vector2 from{r.x + 3 * s, r.y + 3 * s};
            const raylib::Vector2 to{vertical ? from.x : r.x + r.width - 3 * s,
                                    vertical ? r.y + r.height - 3 * s : from.y};
            const afterhours::Color ink{122, 218, 224, 220};
            raylib::DrawLineEx(from, to, s, ink);
            raylib::DrawLineEx(to, {to.x - 4 * s, to.y - 4 * s}, s, ink);
            raylib::DrawLineEx(to, {to.x + (vertical ? 4 : -4) * s, to.y + (vertical ? -4 : 4) * s}, s, ink);
          }));
    };
    const auto make_justify = [&](int id, float x, float y, float w, float h,
                                  bool vertical, int mode) {
      const std::string name = std::string(vertical ? "vertical_" : "horizontal_") + std::to_string(mode);
      auto config = box(x, y, w, h).with_custom_background({43, 54, 74, 255})
          .with_padding(Padding::all(pixels((vertical ? 8 : 4) * s)))
          .with_justify_content(justify[mode]).with_gap(pixels((vertical ? 10 : 2) * s))
          .with_no_wrap().with_debug_name(name);
      auto frame = vertical ? vstack(context, mk(root.ent(), id), config)
                            : hstack(context, mk(root.ent(), id), config);
      for (int i = 0; i < 3; ++i) {
        div(context, mk(frame.ent(), i), ComponentConfig{}
            .with_size(vertical ? ComponentSize{percent(.85f), pixels(30 * s)}
                                : ComponentSize{pixels(20 * s), percent(.70f)})
            .with_label(std::to_string(i + 1)).with_alignment(TextAlignment::Center)
            .with_font("AtkinsonMock", pixels((vertical ? 18 : 14) * s)).with_skip_grid_snap(true)
            .with_custom_background(vertical ? afterhours::Color{0, 110, 140, 255} : afterhours::Color{180, 50, 80, 255})
            .with_custom_text_color({255, 255, 255, 255}).with_corner_radius(4 * s)
            .with_debug_name(name + "_" + std::to_string(i)));
      }
      div(context, mk(root.ent(), id + 20), box(x, y, w, h).with_ignore_pointer_events()
          .with_on_draw_fg([s, vertical, mode, eid = frame.ent().id](RectangleType r) {
            const auto parent = UICollectionHolder::getEntityForID(eid);
            if (!parent) return;
            const auto &cmp = parent.asE().get<UIComponent>();
            if (cmp.children.size() < 3) return;
            const auto first = UICollectionHolder::getEntityForID(cmp.children.front());
            const auto last = UICollectionHolder::getEntityForID(cmp.children.back());
            if (!first || !last) return;
            const auto a = first.asE().get<UIComponent>().rect();
            const auto b = last.asE().get<UIComponent>().rect();
            const float inset = (vertical ? 8 : 4) * s;
            const float begin = vertical ? r.y + inset : r.x + inset;
            const float end = vertical ? r.y + r.height - inset : r.x + r.width - inset;
            const auto marker = [&](float lo, float hi) {
              if (hi - lo < s) return;
              const auto point = [&](float value) {
                return vertical ? raylib::Vector2{r.x + r.width - 3 * s, value}
                                : raylib::Vector2{value, r.y + r.height - 3 * s};
              };
              const afterhours::Color color{156, 186, 218, 180};
              raylib::DrawLineEx(point(lo), point(hi), s, color);
              for (float value : {lo, hi}) {
                auto pt = point(value);
                auto tip = pt;
                if (vertical) tip.x -= 4 * s;
                else tip.y -= 4 * s;
                raylib::DrawLineEx(pt, tip, s, color);
              }
            };
            if (mode == 0) marker(vertical ? b.y + b.height : b.x + b.width, end);
            if (mode == 1) marker(begin, vertical ? a.y : a.x);
            if (mode == 2) {
              const auto p1 = vertical ? raylib::Vector2{r.x, r.y + r.height / 2} : raylib::Vector2{r.x + r.width / 2, r.y};
              const auto p2 = vertical ? raylib::Vector2{r.x + r.width, p1.y} : raylib::Vector2{p1.x, r.y + r.height};
              raylib::DrawLineEx(p1, p2, s, {181, 201, 224, 80});
            }
            if (mode < 3) return;
            for (size_t i = 1; i < cmp.children.size(); ++i) {
              const auto prev = UICollectionHolder::getEntityForID(cmp.children[i - 1]);
              const auto next = UICollectionHolder::getEntityForID(cmp.children[i]);
              if (!prev || !next) continue;
              const auto p = prev.asE().get<UIComponent>().rect();
              const auto n = next.asE().get<UIComponent>().rect();
              marker(vertical ? p.y + p.height : p.x + p.width, vertical ? n.y : n.x);
            }
            if (mode == 4) {
              marker(begin, vertical ? a.y : a.x);
              marker(vertical ? b.y + b.height : b.x + b.width, end);
            }
          }));
    };
    const auto make_align = [&](int id, float x, float y, float w, float h, int mode) {
      const std::string name = "cross_" + std::to_string(mode);
      auto frame = hstack(context, mk(root.ent(), id), box(x, y, w, h)
          .with_custom_background({43, 54, 74, 255}).with_align_items(aligns[mode])
          .with_padding(Padding{.top = pixels(6 * s), .left = pixels(8 * s), .bottom = pixels(6 * s), .right = pixels(8 * s)})
          .with_gap(pixels(8 * s)).with_no_wrap().with_debug_name(name)
          .with_on_draw_fg([mode, s](RectangleType r) {
            const float yline = mode == 0 ? r.y + 6 * s : mode == 2 ? r.y + r.height - 6 * s : r.y + r.height / 2;
            raylib::DrawLineEx({r.x, yline}, {r.x + r.width, yline}, s, {175, 205, 233, 160});
          }));
      const std::array<float, 3> heights{.50f, .30f, .40f};
      for (int i = 0; i < 3; ++i)
        div(context, mk(frame.ent(), i), ComponentConfig{}.with_size({pixels(40 * s), percent(heights[i])})
            .with_label(std::to_string(i + 1)).with_font("AtkinsonMock", pixels(18 * s))
            .with_alignment(TextAlignment::Center).with_skip_grid_snap(true).with_corner_radius(4 * s)
            .with_custom_background({180, 50, 80, 255}).with_custom_text_color({255, 255, 255, 255})
            .with_debug_name(name + "_" + std::to_string(i)));
    };
    // Container with AlignItems::Center
    const auto make_self = [&](int id, float x, float y, float w, float h) {
      auto frame = hstack(context, mk(root.ent(), id), box(x, y, w, h)
          .with_custom_background({43, 54, 74, 255}).with_align_items(AlignItems::Center)
          .with_justify_content(JustifyContent::SpaceAround).with_no_wrap()
          .with_padding(Padding{.top = pixels(6 * s), .left = pixels(10 * s), .bottom = pixels(6 * s), .right = pixels(10 * s)})
          .with_debug_name("self_align_container"));
      const std::array<SelfAlign, 4> self{SelfAlign::FlexStart, SelfAlign::Auto, SelfAlign::Center, SelfAlign::FlexEnd};
      const std::array<std::string, 4> names{"Start", "Auto", "Center", "End"};
      const std::array<float, 4> widths{70, 60, 70, 60};
      const std::array<afterhours::Color, 4> colors{{{0, 110, 140, 255}, {70, 50, 130, 255}, {180, 50, 80, 255}, {200, 75, 100, 255}}};
      for (int i = 0; i < 4; ++i)
        div(context, mk(frame.ent(), i), ComponentConfig{}.with_size({pixels(widths[i] * s), percent(.25f)})
            .with_label(names[i]).with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
            .with_custom_background(colors[i]).with_custom_text_color({255, 255, 255, 255})
            .with_corner_radius(4 * s).with_skip_grid_snap(true).with_self_align(self[i]).with_debug_name("self_" + std::to_string(i)));
    };
    if (active_category == 0) {
      section(10, "Vertical spacing", "JustifyContent / Column / main axis down", 0, 170, 568);
      section(12, "Horizontal spacing", "JustifyContent / Row / main axis right", 592, 170, 568);
      for (int i = 0; i < 5; ++i) {
        const float x = i * 114.f;
        label(root.ent(), 30 + i, justify_names[i], x, 236, 112, 26, i < 3 ? 18.f : 15.f);
        make_justify(100 + i, x, 270, 104, 168, true, i);
        label(root.ent(), 40 + i, justify_names[i], 592 + x, 236, 112, 26, i < 3 ? 18.f : 15.f);
        make_justify(150 + i, 592 + x, 270, 104, 76, false, i);
      }
      axis(20, 578, 279, 8, 145, true);
      axis(21, 605, 358, 536, 10, false);
      label(root.ent(), 22, "Brackets mark unused space or the gaps between items.", 592, 377, 568, 24, 18);
      label(root.ent(), 23, "Before snapping: inner gap = 2 x edge gap + base gap.", 592, 408, 568, 24, 18);
      section(14, "Cross-axis alignment", "AlignItems / heights: 50%, 30%, 40% of content height", 0, 467, 568);
      section(16, "Individual overrides", "SelfAlign / parent AlignItems::Center", 592, 467, 568);
      for (int i = 0; i < 3; ++i) {
        label(root.ent(), 50 + i, align_names[i], i * 196.f, 537, 176, 27, 20);
        make_align(200 + i, i * 196.f, 572, 176, 108, i);
      }
      axis(24, 578, 578, 8, 96, true);
      label(root.ent(), 25, "Teal Start / purple Auto / pink Center / rose End", 592, 538, 568, 25, 17);
      make_self(210, 592, 572, 568, 108);
      label(root.ent(), 26, "Overview covers alignment. Auto inherits the parent; Gap has its own comparison tab.", 0, 695, 1160, 22, 17);
      return;
    }
    if (active_category <= 2) {
      const bool vertical = active_category == 1;
      section(10, vertical ? "Vertical spacing" : "Horizontal spacing",
              vertical ? "JustifyContent / Column / three 30px-high children / base gap 10px"
                       : "JustifyContent / Row / three 20px-wide children / base gap 2px", 0, 173, 1160);
      for (int i = 0; i < 5; ++i) {
        label(root.ent(), 30 + i, justify_names[i], i * 236.f, 250, 216, 28, 22, true);
        make_justify(100 + i, i * 236.f, 290, 208, vertical ? 290 : 140, vertical, i);
      }
      axis(20, vertical ? 1154 : 8, vertical ? 300 : 443, vertical ? 6 : 1130, vertical ? 267 : 10, vertical);
      label(root.ent(), 21, vertical ? "Main axis points down." : "Main axis points right.", 0, 605, 1160, 27, 20, true);
      label(root.ent(), 22, "Start and End bracket the unused region. Center marks the midpoint. Between marks the inner gaps.", 0, 642, 1160, 26, 18);
      label(root.ent(), 23, "Around adds equal space around each item; the configured base gap is added only between items.", 0, 680, 1160, 26, 18);
      return;
    }
    if (active_category == 3) {
      section(10, "Cross-axis alignment", "AlignItems / row direction / cross axis points down", 0, 173, 1160);
      for (int i = 0; i < 3; ++i) {
        label(root.ent(), 30 + i, align_names[i], i * 396.f, 246, 368, 29, 22, true);
        make_align(200 + i, i * 396.f, 288, 368, 294, i);
      }
      axis(20, 1154, 302, 6, 264, true);
      label(root.ent(), 21, "Item widths stay at 40px. Heights are 50%, 30% and 40% of the padded content height.", 0, 612, 1160, 28, 20);
      label(root.ent(), 22, "Reference lines mark the top edge, vertical midpoint and bottom edge.", 0, 652, 1160, 28, 20);
      return;
    }
    if (active_category == 4) {
      section(10, "Individual overrides", "SelfAlign / parent AlignItems::Center / children are 25% of content height", 0, 173, 1160);
      label(root.ent(), 20, "Teal Start / purple Auto / pink Center / rose End", 0, 247, 1160, 28, 21);
      make_self(210, 0, 291, 1160, 282);
      label(root.ent(), 21, "Auto inherits the parent's Center alignment, so Auto and Center share a midpoint.", 0, 610, 1160, 29, 21);
      label(root.ent(), 22, "Start overrides to the top edge; End overrides to the bottom. Horizontal spacing stays SpaceAround.", 0, 654, 1160, 28, 19);
      return;
    }
    // Gap demo
    section(10, "Gap comparison", "Uniform spacing between children, without per-child margins", 0, 173, 1160);
    const std::array<std::string, 4> gap_names{"Column / no gap", "Column / gap 10px", "Row / no gap", "Row / gap 10px"};
    for (int group = 0; group < 4; ++group) {
      const float x = group * 296.f;
      const bool vertical = group < 2;
      label(root.ent(), 30 + group, gap_names[group], x, 250, 272, 29, 22, true);
      auto config = box(x, 294, 272, vertical ? 294 : 146).with_custom_background({43, 54, 74, 255})
          .with_padding(Padding::all(pixels(8 * s))).with_no_wrap().with_gap(pixels((group % 2 == 0 ? 0 : 10) * s))
          .with_debug_name("gap_" + std::to_string(group));
      auto frame = vertical ? vstack(context, mk(root.ent(), 300 + group), config)
                            : hstack(context, mk(root.ent(), 300 + group), config);
      for (int i = 0; i < 4; ++i)
        div(context, mk(frame.ent(), i), ComponentConfig{}
            .with_size(vertical ? ComponentSize{percent(.85f), percent(.18f)} : ComponentSize{percent(.20f), percent(.80f)})
            .with_label(std::to_string(i + 1)).with_font("AtkinsonMock", pixels(18 * s))
            .with_alignment(TextAlignment::Center).with_custom_text_color({255, 255, 255, 255})
            .with_custom_background(vertical ? afterhours::Color{0, 110, 140, 255} : afterhours::Color{180, 50, 80, 255})
            .with_corner_radius(4 * s).with_skip_grid_snap(true).with_debug_name("gap_" + std::to_string(group) + "_" + std::to_string(i)));
    }
    label(root.ent(), 20, "Both column fixtures use the same child heights; both row fixtures use the same child widths.", 0, 620, 1160, 29, 20);
    label(root.ent(), 21, "Only the gap changes. All dimensions shown use 720p design pixels.", 0, 664, 1160, 29, 20);
  }
};

REGISTER_EXAMPLE_SCREEN(flex_alignment, "System Demos",
                        "Flexbox justify and align properties",
                        ExampleFlexAlignment)
