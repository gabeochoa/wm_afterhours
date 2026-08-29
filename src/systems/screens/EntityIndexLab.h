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
  int selected_node = 0;

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

    const auto ink = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto good = afterhours::Color{120, 220, 160, 255};
    const auto panel = afterhours::Color{26, 32, 46, 255};
    const auto rowbg = afterhours::Color{34, 41, 58, 255};
    const auto selbg = afterhours::Color{58, 96, 150, 255};

    static constexpr const char *DEVICE[kNodes] = {
        "Mixer A", "Preamp 1", "Tape Deck", "Reverb 2", "Patch 7", "Monitor"};
    static constexpr const char *PORT_NAME[kPortsPerNode] = {
        "Input L", "Input R", "Send", "Return"};
    static constexpr const char *PORT_STATE[kPortsPerNode] = {
        "connected", "connected", "idle", "connected"};

    auto &ec = afterhours::EntityHelper::get_default_collection();

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1184), pixels(42)})
            .with_absolute_position(48.f, 22.f)
            .with_label("Patch Bay")
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(26.f))
            .with_custom_text_color(ink)
            .with_debug_name("ei_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1184), pixels(30)})
            .with_absolute_position(48.f, 60.f)
            .with_label("Pick a device. Its ports come back through a secondary "
                        "index, not a scan of the collection.")
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(16.f))
            .with_custom_text_color(muted)
            .with_debug_name("ei_sub"));

    // ---- device list -------------------------------------------------------
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(360), pixels(504)})
            .with_absolute_position(48.f, 100.f)
            .with_custom_background(panel)
            .with_roundness(0.04f)
            .with_debug_name("ei_listpanel"));

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(328), pixels(30)})
            .with_absolute_position(64.f, 116.f)
            .with_label("DEVICES")
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(14.f))
            .with_custom_text_color(muted)
            .with_debug_name("ei_listhead"));

    for (int n = 0; n < (int)nodes.size(); n++) {
      const size_t count =
          afterhours::EntityQuery(ec)
              .whereIndexed<IsDemoPort>(nodes[(size_t)n])
              .gen()
              .size();
      const bool sel = (n == selected_node);

      const float ry = 148.f + (float)n * 60.f;
      if (button(context, mk(entity, 10 + n),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(328), pixels(54)})
                     .with_absolute_position(64.f, ry)
                     .with_label(DEVICE[n])
                     .with_alignment(TextAlignment::Left)
                     .with_font_size(pixels(17.f))
                     .with_custom_background(sel ? selbg : rowbg)
                     .with_custom_text_color(ink)
                     .with_roundness(0.18f)
                     .with_debug_name(fmt::format("ei_dev_{}", n)))) {
        selected_node = n;
      }

      // Its own right-aligned column; inline spaces left the counts ragged.
      div(context, mk(entity, 80 + n),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(120), pixels(54)})
              .with_absolute_position(256.f, ry)
              .with_label(fmt::format("{} ports", count))
              .with_alignment(TextAlignment::Right)
              .with_font_size(pixels(15.f))
              .with_custom_text_color(muted)
              .with_debug_name(fmt::format("ei_devcount_{}", n)));
    }

    // ---- port table for the selected device --------------------------------
    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(792), pixels(504)})
            .with_absolute_position(440.f, 100.f)
            .with_custom_background(panel)
            .with_roundness(0.04f)
            .with_debug_name("ei_detailpanel"));

    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(760), pixels(36)})
            .with_absolute_position(456.f, 116.f)
            .with_label(DEVICE[selected_node])
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(21.f))
            .with_custom_text_color(ink)
            .with_debug_name("ei_detail_title"));

    const char *cols[3] = {"SLOT", "PORT", "STATE"};
    const float col_x[3] = {456.f, 560.f, 900.f};
    for (int c = 0; c < 3; c++) {
      div(context, mk(entity, 20 + c),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(300), pixels(28)})
              .with_absolute_position(col_x[c], 156.f)
              .with_label(cols[c])
              .with_alignment(TextAlignment::Left)
              .with_font_size(pixels(14.f))
              .with_custom_text_color(muted)
              .with_debug_name(fmt::format("ei_col_{}", c)));
    }

    // The one indexed lookup the screen is actually about.
    const auto ports = afterhours::EntityQuery(ec)
                           .whereIndexed<IsDemoPort>(nodes[(size_t)selected_node])
                           .gen();

    int r = 0;
    for (const auto &pref : ports) {
      const int slot = pref.get().get<IsDemoPort>().slot;
      const float ry = 186.f + (float)r * 46.f;

      div(context, mk(entity, 30 + r),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(760), pixels(40)})
              .with_absolute_position(456.f, ry)
              .with_custom_background(rowbg)
              .with_roundness(0.12f)
              .with_debug_name(fmt::format("ei_portrow_{}", r)));

      div(context, mk(entity, 40 + r),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(90), pixels(40)})
              .with_absolute_position(472.f, ry)
              .with_label(fmt::format("{}", slot))
              .with_alignment(TextAlignment::Left)
              .with_font_size(pixels(16.f))
              .with_custom_text_color(muted)
              .with_debug_name(fmt::format("ei_slot_{}", r)));

      div(context, mk(entity, 50 + r),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(320), pixels(40)})
              .with_absolute_position(576.f, ry)
              .with_label(PORT_NAME[slot % kPortsPerNode])
              .with_alignment(TextAlignment::Left)
              .with_font_size(pixels(16.f))
              .with_custom_text_color(ink)
              .with_debug_name(fmt::format("ei_pname_{}", r)));

      const bool live = std::string(PORT_STATE[slot % kPortsPerNode]) ==
                        std::string("connected");
      div(context, mk(entity, 60 + r),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(300), pixels(40)})
              .with_absolute_position(916.f, ry)
              .with_label(PORT_STATE[slot % kPortsPerNode])
              .with_alignment(TextAlignment::Left)
              .with_font_size(pixels(16.f))
              .with_custom_text_color(live ? good : muted)
              .with_debug_name(fmt::format("ei_pstate_{}", r)));
      r++;
    }

    // ---- the measurement, kept but subordinate -----------------------------
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

    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1184), pixels(28)})
            .with_absolute_position(48.f, 620.f)
            .with_label(fmt::format(
                "Listing all {} ports touches {} entities by scan and {} by "
                "index, in a collection of {}.",
                indexed_found, scanned, resolves,
                kNodes + kNodes * kPortsPerNode + kBallast))
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(15.f))
            .with_custom_text_color(good)
            .with_debug_name("ei_counts"));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1184), pixels(28)})
            .with_absolute_position(48.f, 650.f)
            .with_label(fmt::format(
                "Both spellings return {}, which is the only reason the "
                "cheaper one is worth using.",
                scan_found))
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(15.f))
            .with_custom_text_color(muted)
            .with_debug_name("ei_agree"));
  }
};

REGISTER_EXAMPLE_SCREEN(entity_index_lab, "System Demos",
                        "parent to children through a secondary index",
                        EntityIndexLab)
