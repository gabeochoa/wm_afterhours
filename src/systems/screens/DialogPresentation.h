#pragma once

// DialogPresentation.h -- the wm-side presentation layer for library widgets.
//
// The afterhours library builds its modal/stepper widgets with its own
// default styling; this app restyles them after build by walking the built
// entity tree (the library has no presentation-config hook for this yet --
// that half lives in a future afterhours lane and is out of scope here).
// This header is the ONLY place that tree-walk restyling pattern lives:
// screens call the helpers below instead of open-coding their own
// getEntityForIDEnforce walks. If you find yourself reaching for
// UICollectionHolder::getEntityForIDEnforce in a screen to restyle a
// library widget, add the walk here instead.

#include "../../external.h"
#include "../../input_mapping.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <span>
#include <string>

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

// Chevron geometry + palette for style_stepper_arrows, in design units
// (pre-scale). The defaults are the SettingRowShowcase look; StepperShowcase
// overrides line_width / tip_offset / half_height for its chunkier arrows.
struct StepperArrowStyle {
  float scale = 1.f;
  afterhours::Color ink{235, 241, 250, 255};
  afterhours::Color background{47, 64, 86, 255};
  float width = 44.f;
  float line_width = 2.f;
  float tip_offset = 4.f;
  float back_offset = 4.f;
  float half_height = 8.f;
};

// Restyle a built stepper's arrows: fixed width, no text label, a flat
// background, and a hand-drawn chevron (the library's "<" / ">" glyphs do
// not match the gallery's icon style). `row` may be the stepper itself
// (StepperShowcase: HasStepperState lives on the result entity, arrows are
// its direct clickable children) or a setting_row wrapping one
// (SettingRowShowcase: the stepper is a child control). Both shapes are
// recognised so callers do not re-implement the walk. Arrows are named
// `<name>_previous` / `<name>_next` in tree order -- e2e scripts click
// them by those debug names.
inline void style_stepper_arrows(imm::ElementResult row,
                                 const std::string &name,
                                 const StepperArrowStyle &style) {
  const float s = style.scale;
  int direction = 0;
  const auto restyle_arrow = [&](afterhours::Entity &arrow) {
    const bool previous = direction++ == 0;
    arrow.get<UIComponent>().set_desired_width(pixels(style.width * s));
    arrow.get<HasLabel>().label.clear();
    arrow.addComponentIfMissing<UIComponentDebug>(name)
        .set(name + (previous ? "_previous" : "_next"));
    arrow.addComponentIfMissing<afterhours::HasColor>(style.background)
        .set(style.background);
    const afterhours::Color ink = style.ink;
    arrow.addComponentIfMissing<HasOnDraw>().fg =
        [s, previous, ink, style](RectangleType r) {
          const float dir = previous ? -1.f : 1.f;
          const float cx = r.x + r.width / 2;
          const float cy = r.y + r.height / 2;
          afterhours::draw_line_ex(
              {cx - dir * style.back_offset * s, cy - style.half_height * s},
              {cx + dir * style.tip_offset * s, cy}, style.line_width * s,
              ink);
          afterhours::draw_line_ex(
              {cx + dir * style.tip_offset * s, cy},
              {cx - dir * style.back_offset * s, cy + style.half_height * s},
              style.line_width * s, ink);
        };
  };
  const auto scan = [&](afterhours::Entity &host) {
    for (const auto child_id : host.get<UIComponent>().children) {
      auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
      if (child.has<HasClickListener>()) restyle_arrow(child);
    }
  };
  if (row.ent().has<HasStepperState>()) {
    scan(row.ent());
    return;
  }
  for (const auto child_id : row.cmp().children) {
    auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
    if (child.has<HasStepperState>()) {
      scan(child);
    } else if (child.has<HasClickListener>()) {
      restyle_arrow(child);
    }
  }
}
} // namespace dialog_presentation
