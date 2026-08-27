#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// add_index / whereIndexed over a parent link stored in a component field,
// which is the exact shape puzzle asked for: a node graph where every port
// says which node it belongs to.
//
// The thing worth seeing on screen is the counter row. Asking for one node's
// ports touches that node's ports, not every entity in the collection, and
// that stays true as the collection grows.
namespace entity_index_lab {

struct IsDemoNode : afterhours::BaseComponent {
  int label = 0;
};

// The parent is a field on the child, not a link the ECS knows about. Nothing
// intercepts a write to it, which is why the index is rebuilt and not
// maintained.
struct IsDemoPort : afterhours::BaseComponent {
  afterhours::EntityHandle parent{afterhours::EntityHandle::invalid()};
  int slot = 0;
};

// Padding so the collection is meaningfully larger than any one answer. A
// lookup that is only faster on a graph of five is not evidence of anything.
struct IsBallast : afterhours::BaseComponent {};

} // namespace entity_index_lab

struct EntityIndexLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kNodes = 6;
  static constexpr int kPortsPerNode = 4;
  static constexpr int kBallast = 1000;

  std::vector<afterhours::EntityHandle> nodes;
  bool index_registered = false;

  bool graph_is_alive() const {
    if (nodes.empty())
      return false;
    for (const auto &h : nodes)
      if (!afterhours::EntityHelper::resolve(h))
        return false;
    return true;
  }

  // Rebuilt if anything cleared the collection out from under us, so the
  // screen survives a script reset or a return visit.
  void build_graph() {
    using namespace entity_index_lab;
    nodes.clear();
    afterhours::EntityHelper::merge_entity_arrays();

    for (int n = 0; n < kNodes; n++) {
      afterhours::Entity &node = afterhours::EntityHelper::createEntity();
      node.addComponent<IsDemoNode>().label = n;
      afterhours::EntityHelper::merge_entity_arrays();
      nodes.push_back(afterhours::EntityHelper::handle_for(node));
    }

    for (int n = 0; n < kNodes; n++) {
      for (int p = 0; p < kPortsPerNode; p++) {
        afterhours::Entity &port = afterhours::EntityHelper::createEntity();
        IsDemoPort &ip = port.addComponent<IsDemoPort>();
        ip.parent = nodes[(size_t)n];
        ip.slot = p;
      }
    }
    for (int i = 0; i < kBallast; i++)
      afterhours::EntityHelper::createEntity().addComponent<IsBallast>();
    afterhours::EntityHelper::merge_entity_arrays();

    if (!index_registered) {
      afterhours::EntityHelper::add_index<IsDemoPort>(
          [](const IsDemoPort &p) { return p.parent; });
      index_registered = true;
    }
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    using namespace entity_index_lab;
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.text_inset = {5.f, 5.f};

    if (!graph_is_alive())
      build_graph();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto good = afterhours::Color{120, 220, 160, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label(fmt::format("Entity Index - {} nodes x {} ports inside "
                                    "a {}-entity collection",
                                    kNodes, kPortsPerNode,
                                    kNodes + kNodes * kPortsPerNode + kBallast))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("ei_title"));

    // The measurement: how many entities each spelling touches to answer the
    // same question for every node.
    auto &ec = afterhours::EntityHelper::get_default_collection();
    const size_t resolves_before =
        afterhours::EntityCollection::stats().bucket_resolves;
    size_t indexed_found = 0;
    for (const auto &h : nodes)
      indexed_found +=
          afterhours::EntityQuery(ec).whereIndexed<IsDemoPort>(h).gen().size();
    const size_t resolves =
        afterhours::EntityCollection::stats().bucket_resolves - resolves_before;

    size_t scanned = 0;
    size_t scan_found = 0;
    for (const auto &h : nodes) {
      scan_found += afterhours::EntityQuery(ec)
                        .whereLambda([&](const afterhours::Entity &) {
                          scanned++;
                          return true;
                        })
                        .whereHasComponent<IsDemoPort>()
                        .whereLambda([&](const afterhours::Entity &e) {
                          return e.get<IsDemoPort>().parent == h;
                        })
                        .gen()
                        .size();
    }

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(28)})
            .with_absolute_position(48.f, 58.f)
            .with_label(fmt::format(
                "same answer ({} ports) - scanning touches {} entities, "
                "indexed touches {}",
                indexed_found, scanned, resolves))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(good)
            .with_debug_name("ei_counts"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 88.f)
            .with_label(fmt::format("scan found {} - the index is only worth "
                                    "anything if both agree",
                                    scan_found))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_debug_name("ei_agree"));

    // One row per node, its ports fetched through the index.
    float y = 130.f;
    for (int n = 0; n < (int)nodes.size(); n++) {
      const auto ports = afterhours::EntityQuery(ec)
                             .whereIndexed<IsDemoPort>(nodes[(size_t)n])
                             .gen();

      std::string slots;
      for (const auto &pref : ports) {
        if (!slots.empty())
          slots += ", ";
        slots += std::to_string(pref.get().get<IsDemoPort>().slot);
      }

      div(context, mk(entity, 3 + n),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1180), pixels(34)})
              .with_absolute_position(48.f, y)
              .with_label(fmt::format("node {} -> {} ports [{}]", n,
                                      ports.size(), slots))
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(white)
              .with_custom_background(afterhours::Color{
                  (unsigned char)(30 + (n % 2) * 8),
                  (unsigned char)(36 + (n % 2) * 10),
                  (unsigned char)(52 + (n % 2) * 14), 255})
              .with_debug_name(fmt::format("ei_node_{}", n)));
      y += 38.f;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(entity_index_lab, "System Demos",
                        "parent to children through a secondary index",
                        EntityIndexLab)
