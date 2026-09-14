#pragma once

// JSON dump of the live UI tree: geometry AND the layout intent that produced
// it (size specs, flex direction, justify/align, gap).
//
// The intent half is the point. A dump of rects alone can only be replayed --
// it is afterhours' own answer, so comparing it to a screenshot catches
// rendering bugs and nothing else. Emitting the intent lets a second layout
// engine (the mock tool's CSS flexbox) solve the SAME input independently, and
// the disagreements are where afterhours' autolayout is worth looking at.

#include <afterhours/ah.h>
#include <nlohmann/json.hpp>
#include <string>
#include "../font_config.h"
#include "../input_mapping.h"

#if __has_include(<magic_enum/magic_enum.hpp>)
#include <magic_enum/magic_enum.hpp>
#else
#include "../../vendor/magic_enum/magic_enum.hpp"
#endif

namespace ui_tree_dump {

inline nlohmann::json size_json(const afterhours::ui::Size &s) {
  return {{"dim", std::string(magic_enum::enum_name(s.dim))},
          {"value", s.value},
          {"strictness", s.strictness},
          {"screen_reference", std::string(magic_enum::enum_name(s.screen_reference))}};
}

inline nlohmann::json build(afterhours::Entity &entity,
                            afterhours::ui::UIComponent &cmp) {
  using namespace afterhours::ui;

  nlohmann::json node;
  node["id"] = cmp.id;

  if (entity.has<UIComponentDebug>())
    node["name"] = entity.get<UIComponentDebug>().name();

  node["rect"] = {{"x", cmp.rect().x},
                  {"y", cmp.rect().y},
                  {"width", cmp.rect().width},
                  {"height", cmp.rect().height}};

  node["computed"] = {{"width", cmp.computed[Axis::X]},
                      {"height", cmp.computed[Axis::Y]}};

  node["relative_pos"] = {{"x", cmp.computed_rel[Axis::X]},
                          {"y", cmp.computed_rel[Axis::Y]}};

  node["padding"] = {{"left", cmp.computed_padd[Axis::left]},
                     {"top", cmp.computed_padd[Axis::top]},
                     {"right", cmp.computed_padd[Axis::right]},
                     {"bottom", cmp.computed_padd[Axis::bottom]}};

  node["margin"] = {{"left", cmp.computed_margin[Axis::left]},
                    {"top", cmp.computed_margin[Axis::top]},
                    {"right", cmp.computed_margin[Axis::right]},
                    {"bottom", cmp.computed_margin[Axis::bottom]}};

  // The unresolved specification -- what the caller asked for, before the
  // solver had an answer. `computed` above is the result; these two disagreeing
  // in an interesting way is the entire signal the mock tool is looking for.
  node["desired"] = {{"x", size_json(cmp.desired[Axis::X])},
                     {"y", size_json(cmp.desired[Axis::Y])}};
  node["min_size"] = {{"x", size_json(cmp.min_size[Axis::X])},
                      {"y", size_json(cmp.min_size[Axis::Y])}};
  node["max_size"] = {{"x", size_json(cmp.max_size[Axis::X])},
                      {"y", size_json(cmp.max_size[Axis::Y])}};

  node["flex_direction"] =
      std::string(magic_enum::enum_name(cmp.flex_direction));
  node["justify_content"] =
      std::string(magic_enum::enum_name(cmp.justify_content));
  node["align_items"] = std::string(magic_enum::enum_name(cmp.align_items));
  node["self_align"] = std::string(magic_enum::enum_name(cmp.self_align));
  node["flex_wrap"] = std::string(magic_enum::enum_name(cmp.flex_wrap));
  node["gap"] = cmp.gap;
  node["desired_gap"] = size_json(cmp.desired_gap);

  node["absolute"] = cmp.absolute;
  node["visible"] = cmp.was_rendered_to_screen;

  // A clipping or scrolling container is *supposed* to hold children larger
  // than itself, so overflow under one of these is not a layout fault.
  node["clips"] = entity.has<HasClipChildren>();
  node["scrolls"] = entity.has<HasScrollView>();

  // Corner rounding, so the mock's CSS pane looks like the render rather than
  // reporting a difference that is only skin. Matches rendering.h: no
  // HasRoundedCorners means no rounding at all, whatever the theme says.
  if (entity.has<HasRoundedCorners>()) {
    const HasRoundedCorners &rc = entity.get<HasRoundedCorners>();
    node["roundness"] = rc.roundness;
    if (rc.radius_px) node["corner_radius"] = *rc.radius_px;
    // bitset order is imm::CornerPosition: TL, TR, BL, BR.
    node["corners"] = nlohmann::json::array(
        {bool(rc.rounded_corners[0]), bool(rc.rounded_corners[1]),
         bool(rc.rounded_corners[2]), bool(rc.rounded_corners[3])});
  }

  // Only a node with a fill actually shows its corners on screen.
  node["paints"] = entity.has<afterhours::HasColor>();

  node["clickable"] = entity.has<HasClickListener>();
  node["draggable"] = entity.has<HasDragListener>();
  node["focusable"] = !entity.has<SkipWhenTabbing>();

  if (entity.has<HasLabel>()) {
    const HasLabel &lbl = entity.get<HasLabel>();
    node["label"] = lbl.label;
    auto *fm = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    auto *context = afterhours::EntityHelper::get_singleton_cmp<UIContext<InputAction>>();
    if (fm && context) {
      const auto name = fm->resolve_weighted(cmp.font_name, cmp.font_weight);
      const auto font_file = [](std::string source) {
        for (const auto &alias : font_config::aliases)
          if (source == alias.name) source = alias.source;
        for (const auto &definition : font_config::get_all_fonts())
          if (source == definition.name) return definition.filename;
        return std::string{};
      };
      const auto file = font_file(name);
      const float scale = imm::ThemeDefaults::get().theme.ui_scale;
      const float size = resolve_to_pixels(cmp.font_size, context->screen_height,
                                           cmp.resolved_scaling_mode, scale);
      const auto inset = resolve_text_inset(context->theme, lbl.text_inset);
      const auto font = fm->get_font(name);
      const float line_height = afterhours::measure_text(font, "Ag", size, 1.f).y;
      node["text"] = {{"font", name}, {"file", file}, {"size", size},
                      {"explicit_size", cmp.font_size_explicitly_set}, {"minimum_size", MIN_FONT_SIZE},
                      {"line_height", line_height}, {"spacing", 1.f + lbl.letter_spacing},
                      {"inset_x", inset.x}, {"inset_y", inset.y},
                      {"offset_x", lbl.text_x_offset}, {"offset_y", lbl.text_y_offset}};
      node["text"]["spans"] = nlohmann::json::array();
      for (const auto &span : lbl.spans) {
        const auto span_font = fm->resolve_weighted(cmp.font_name, span.weight);
        node["text"]["spans"].push_back({{"text", span.text}, {"font", span_font},
                                       {"file", font_file(span_font)}});
      }
    }
    node["text_alignment"] = std::string(magic_enum::enum_name(lbl.alignment));
    node["text_overflow"] =
        std::string(magic_enum::enum_name(lbl.text_overflow));
  }

  nlohmann::json children = nlohmann::json::array();
  for (afterhours::EntityID child_id : cmp.children) {
    try {
      children.push_back(build(AutoLayout::to_ent_static(child_id),
                               AutoLayout::to_cmp_static(child_id)));
    } catch (...) {
      // Skip invalid children
    }
  }
  node["children"] = children;

  return node;
}

/// Every AutoLayoutRoot in the live collection, as {"tree": [...]}.
inline nlohmann::json build_all() {
  nlohmann::json result;
  result["tree"] = nlohmann::json::array();

  auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
  ui_coll.merge_entity_arrays();
  auto roots = afterhours::EntityQuery(ui_coll, {.ignore_temp_warning = true})
                   .whereHasComponent<afterhours::ui::AutoLayoutRoot,
                                      afterhours::ui::UIComponent>()
                   .gen();

  for (auto &entity_ref : roots) {
    afterhours::Entity &entity = entity_ref.get();
    result["tree"].push_back(
        build(entity, entity.get<afterhours::ui::UIComponent>()));
  }

  return result;
}

} // namespace ui_tree_dump
