#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScrollViewShowcase : ScreenSystem<UIContext<InputAction>> {
  // Persistent state for the invert scroll toggle
  bool invert_scroll = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Light theme for visibility
    Theme theme;
    theme.background = {241, 245, 249, 255};
    theme.surface = {255, 255, 255, 255};
    theme.primary = {46, 100, 146, 255};
    theme.secondary = {215, 225, 234, 255};
    theme.accent = {0, 101, 185, 255};
    theme.font = {25, 42, 60, 255};
    theme.font_muted = {73, 91, 109, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("scroll_bg"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &value,
                           float size, const std::string &name = "", bool muted = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * scale))
          .with_custom_text_color(muted ? theme.font_muted : theme.font)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 124).with_custom_background(theme.surface).with_corner_radius(10 * scale));
    label(1, 16, 34, 1112, 45, "Scroll View Demo", 35);
    checkbox(context, mk(root.ent(), 2), invert_scroll, box(16, 95, 36, 36)
        .with_custom_background(theme.secondary).with_checkbox_indicators("", "").with_debug_name("scroll_invert")
        .with_on_draw_fg([value = invert_scroll, color = theme.font, scale](RectangleType r) {
          afterhours::draw_rectangle_outline({r.x + 7 * scale, r.y + 7 * scale, r.width - 14 * scale, r.height - 14 * scale}, color, 2 * scale);
          if (!value) return;
          afterhours::draw_line_ex({r.x + 11 * scale, r.y + 18 * scale}, {r.x + 16 * scale, r.y + 23 * scale}, 2.5f * scale, color);
          afterhours::draw_line_ex({r.x + 16 * scale, r.y + 23 * scale}, {r.x + 26 * scale, r.y + 12 * scale}, 2.5f * scale, color);
        }));
    label(3, 61, 97, 479, 33, std::string("Invert both scroll axes: ") + (invert_scroll ? "On" : "Off"), 23, "scroll_invert_state");
    int preset = -1;
    const auto action = [&](int id, float x, float w, const std::string &value, const std::string &name) {
      return button(context, mk(root.ent(), id), box(x, 94, w, 39).with_label(value)
          .with_font("AtkinsonMock", pixels(22 * scale)).with_custom_text_color(theme.font)
          .with_custom_background(theme.secondary).with_corner_radius(6 * scale).with_debug_name(name));
    };
    if (action(4, 610, 239, "Reset position", "scroll_reset")) preset = 0;
    if (action(5, 865, 263, "Preview halfway", "scroll_halfway")) preset = 1;
    for (int i = 0; i < 2; ++i)
      div(context, mk(root.ent(), 10 + i), box(static_cast<float>(i) * 592, 165, 552, 509)
          .with_custom_background(theme.surface).with_border({179, 195, 210, 255}, 1)
          .with_corner_radius(10 * scale).with_debug_name(i == 0 ? "vert_section" : "horiz_section"));
    label(20, 20, 177, 512, 36, "Vertical Scroll / 20 items", 27);
    label(21, 20, 222, 512, 31, "Mouse wheel or vertical trackpad swipe", 21, "", true);
    label(30, 612, 177, 512, 36, "Horizontal Scroll / 15 items", 27);
    label(31, 612, 222, 512, 31, "Horizontal swipe or drag the bottom scrollbar", 21, "", true);
    auto vertical = vstack(context, mk(root.ent(), 22), box(20, 266, 512, 300)
        .with_custom_background({233, 240, 246, 255}).with_border(theme.font_muted, 1)
        .with_overflow(Overflow::Scroll, Axis::Y).with_padding(Padding::all(pixels(6 * scale)))
        .with_debug_name("vert_scroll"));
    auto horizontal = hstack(context, mk(root.ent(), 32), box(612, 266, 512, 110)
        .with_custom_background({233, 240, 246, 255}).with_border(theme.font_muted, 1)
        .with_overflow(Overflow::Scroll, Axis::X).with_no_wrap().with_padding(Padding::all(pixels(6 * scale)))
        .with_debug_name("horiz_scroll"));
    auto configure = [&](ElementResult &view, bool x_axis) {
      auto &state = view.ent().get<HasScrollView>();
      state.invert_scroll = invert_scroll;
      state.scrollbar_thickness = pixels(8 * scale);
      state.scrollbar_track_color = afterhours::Color{192, 205, 218, 255};
      state.scrollbar_thumb_color = theme.accent;
      if (preset < 0) return;
      const auto extent = state.viewport_or_zero();
      const float maximum = x_axis ? state.content_size.x - extent.x : state.content_size.y - extent.y;
      const float offset = preset == 0 ? 0 : std::max(0.f, maximum) / 2;
      state.scroll_offset = x_axis ? afterhours::Vector2Type{offset, 0} : afterhours::Vector2Type{0, offset};
      state.scroll_target = state.scroll_offset;
      state.last_eased_offset = state.scroll_offset;
      state.clamp_scroll();
    };
    configure(vertical, false);
    configure(horizontal, true);
    std::array<afterhours::Entity *, 20> vertical_items{};
    std::array<afterhours::Entity *, 15> horizontal_items{};
    for (int i = 0; i < 20; ++i) {
      auto item = div(context, mk(vertical.ent(), i), ComponentConfig{}
          .with_size({percent(.95f), pixels(44 * scale)})
          .with_label(fmt::format("Item {}", i + 1)).with_custom_background(theme.primary)
          .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_text_color({255, 255, 255, 255})
          .with_alignment(TextAlignment::Left).with_text_inset(0)
          .with_margin(Margin{.top = pixels(3 * scale), .bottom = pixels(3 * scale)})
          .with_corner_radius(5 * scale).with_debug_name(fmt::format("vert_item_{}", i)));
      item.ent().get<HasLabel>().text_x_offset = 12 * scale;
      vertical_items[static_cast<size_t>(i)] = &item.ent();
    }
    for (int i = 0; i < 15; ++i) {
      auto item = div(context, mk(horizontal.ent(), i), ComponentConfig{}
          .with_size({pixels(70 * scale), pixels(70 * scale)}).with_label(std::to_string(i + 1))
          .with_custom_background(theme.accent).with_custom_text_color({255, 255, 255, 255})
          .with_font("AtkinsonMock", pixels(27 * scale)).with_alignment(TextAlignment::Center)
          .with_margin(Margin{.left = pixels(4 * scale), .right = pixels(4 * scale)})
          .with_corner_radius(7 * scale).with_debug_name(fmt::format("horiz_item_{}", i)));
      horizontal_items[static_cast<size_t>(i)] = &item.ent();
    }
    const auto range = [](ElementResult &view, const auto &items, bool x_axis) {
      const auto &state = view.ent().template get<HasScrollView>();
      const auto viewport = view.cmp().rect();
      const float near = x_axis ? viewport.x : viewport.y;
      const float far = near + (x_axis ? viewport.width : viewport.height);
      const float offset = x_axis ? state.scroll_offset.x : state.scroll_offset.y;
      int first = 0;
      int last = 0;
      for (size_t i = 0; i < items.size(); ++i) {
        const auto rect = items[i]->template get<UIComponent>().rect();
        const float start = (x_axis ? rect.x : rect.y) - offset;
        const float end = start + (x_axis ? rect.width : rect.height);
        if (end <= near || start >= far) continue;
        if (first == 0) first = static_cast<int>(i) + 1;
        last = static_cast<int>(i) + 1;
      }
      return fmt::format("Visible items {}-{} of {}", first, last, items.size());
    };
    const auto position = [](ElementResult &view, bool x_axis) {
      const auto &state = view.ent().get<HasScrollView>();
      const auto extent = state.viewport_or_zero();
      const float maximum = std::max(0.f, x_axis ? state.content_size.x - extent.x : state.content_size.y - extent.y);
      const float offset = x_axis ? state.scroll_offset.x : state.scroll_offset.y;
      return fmt::format("{} position: {:.0f}px / {:.0f}%", x_axis ? "Horizontal" : "Vertical", offset, maximum > 0 ? offset / maximum * 100 : 0);
    };
    label(23, 20, 579, 512, 31, range(vertical, vertical_items, false), 23, "scroll_vertical_range");
    label(24, 20, 615, 512, 28, position(vertical, false), 21, "scroll_vertical_position", true);
    label(25, 20, 647, 512, 22, "Partial rows at the boundary are intentional.", 18, "", true);
    label(33, 612, 389, 512, 31, range(horizontal, horizontal_items, true), 23, "scroll_horizontal_range");
    label(34, 612, 426, 512, 29, position(horizontal, true), 21, "scroll_horizontal_position", true);
    label(35, 612, 477, 512, 34, "Reading the native scrollbar", 25);
    label(36, 612, 518, 512, 58, "Thumb length shows the visible fraction. Thumb travel shows position through the content.", 22, "", true);
    label(37, 612, 589, 512, 62, "The clipped card at the right edge continues offscreen. Scroll to reveal all 15 cards.", 22, "", true);
    label(40, 0, 690, 1144, 24, "Reset restores both offsets. Inversion changes wheel direction on both axes; it keeps the current positions.", 19, "", true);
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_view, "Component Galleries",
                        "Scrollable container demo", ScrollViewShowcase)
