#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include <afterhours/src/plugins/modal.h>
#include <span>

namespace dialog_presentation {
using namespace afterhours::ui;

inline void style_title(afterhours::Entity &panel) {
  const auto font = imm::UIStylingDefaults::get().default_font_name;
  for (const auto id : panel.get<UIComponent>().children) {
    auto &header = UICollectionHolder::getEntityForIDEnforce(id);
    if (!header.has<UIComponentDebug>() || header.get<UIComponentDebug>().name() != "modal_header") continue;
    for (const auto child_id : header.get<UIComponent>().children) {
      auto &title = UICollectionHolder::getEntityForIDEnforce(child_id);
      if (!title.has<UIComponentDebug>() || title.get<UIComponentDebug>().name() != "modal_title") continue;
      title.get<UIComponent>().font_name = font;
      title.get<HasLabel>().font_name = font;
      return;
    }
    return;
  }
}

inline void style(UIContext<InputAction> &context, afterhours::Entity &root,
                  float width, float body_height, std::span<const float> action_widths) {
  const auto font = imm::UIStylingDefaults::get().default_font_name;
  const auto heading_font = font == "AtkinsonMock" ? std::string("AtkinsonMockBold") : font;
  const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
  auto &panel = root.get<UIComponent>();
  const float height = body_height + 160;
  panel.desired[Axis::X] = pixels(width * scale);
  panel.desired[Axis::Y] = pixels(height * scale);
  panel.absolute_pos_x = (context.screen_width - width * scale) / 2;
  panel.absolute_pos_y = (context.screen_height - height * scale) / 2;
  for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom})
    panel.desired_padding[axis] = pixels(24 * scale);
  root.addComponentIfMissing<HasRoundedCorners>().set_radius_px(12 * scale);
  root.addComponentIfMissing<HasBorder>().border = Border::all(afterhours::Color{98, 116, 144, 255}, pixels(1));
  const auto visit = [&](auto &&self, afterhours::Entity &node) -> void {
    auto &cmp = node.get<UIComponent>();
    cmp.skip_grid_snap = true;
    cmp.enable_font(font, pixels(22 * scale), true);
    for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom})
      cmp.desired_margin[axis] = pixels(0);
    std::string name;
    if (node.has<UIComponentDebug>()) name = node.get<UIComponentDebug>().name();
    if (node.has<HasLabel>()) {
      auto &label = node.get<HasLabel>();
      label.font_name = font;
      label.explicit_text_color = afterhours::Color{235, 241, 250, 255};
      if (name == "modal_title") {
        cmp.enable_font(heading_font, pixels(32 * scale), true);
        label.font_name = heading_font;
      }
    }
    if (name == "modal_header") {
      cmp.desired[Axis::Y] = pixels(40 * scale);
      cmp.desired_margin[Axis::bottom] = pixels(8 * scale);
    }
    if (name == "dialog_message") {
      cmp.desired[Axis::Y] = pixels(body_height * scale);
      for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom})
        cmp.desired_padding[axis] = pixels(0);
    }
    if (name == "dialog_buttons") {
      cmp.desired[Axis::Y] = pixels(56 * scale);
      cmp.desired_gap = pixels(12 * scale);
      for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom})
        cmp.desired_padding[axis] = pixels(0);
    }
    size_t index = 0;
    for (const auto id : cmp.children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (!child.valid() || !child.asE().has<UIComponent>()) continue;
      self(self, child.asE());
      if (name == "dialog_buttons" && index < action_widths.size()) {
        auto &button = child.asE().get<UIComponent>();
        button.desired[Axis::X] = pixels(action_widths[index++] * scale);
        button.desired[Axis::Y] = pixels(44 * scale);
      }
    }
  };
  visit(visit, root);
}
}
