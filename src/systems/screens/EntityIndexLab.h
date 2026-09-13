#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>

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
    if (!graph_is_alive()) build_graph();

    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float offset_x = (context.screen_width - 1280 * s) / 2;
    const float offset_y = (context.screen_height - 720 * s) / 2;
    const auto ink = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{168, 184, 208, 255};
    const auto good = afterhours::Color{120, 220, 160, 255};
    const auto panel = afterhours::Color{26, 35, 50, 255};
    const auto rowbg = afterhours::Color{34, 46, 65, 255};
    const auto selbg = afterhours::Color{47, 78, 119, 255};
    const auto box = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(offset_x + x * s, offset_y + y * s)
          .with_corner_radius(0);
    };
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &name, const char *font = "AtkinsonMock") {
      div(context, mk(entity, id), box(x, y, w, h).with_label(value)
          .with_font(font, pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    static constexpr const char *DEVICE[kNodes] = {
        "Mixer A", "Preamp 1", "Tape Deck", "Reverb 2", "Patch 7", "Monitor"};
    static constexpr const char *DEVICE_TYPE[kNodes] = {
        "Mixing console", "Microphone preamp", "Tape recorder",
        "Effects processor", "Patch panel", "Monitor output"};
    static constexpr const char *PORT_NAME[kPortsPerNode] = {
        "Input L", "Input R", "Send", "Return"};
    auto &ec = afterhours::EntityHelper::get_default_collection();

    div(context, mk(entity, 100), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({16, 22, 32, 255}).with_corner_radius(0)
        .with_debug_name("ei_canvas"));
    text(0, "Patch Bay", 48, 20, 220, 44, 34, ink, "ei_title");
    text(101, "Secondary index demo", 276, 29, 700, 31, 23, muted, "ei_demo_label");
    text(1, "Compare a full collection scan with indexed lookups for the same device ports.",
         48, 73, 1184, 29, 20, muted, "ei_sub");

    // ---- device list -------------------------------------------------------
    div(context, mk(entity, 2), box(48, 120, 344, 510)
        .with_custom_background(panel).with_corner_radius(12 * s)
        .with_debug_name("ei_listpanel"));
    text(3, fmt::format("Devices / {}", nodes.size()), 64, 135, 312, 33,
         25, ink, "ei_listhead");
    text(102, "Select a device to inspect its ports.", 64, 176, 312, 25,
         17, muted, "ei_select_instruction");
    size_t total_ports = 0;
    for (int n = 0; n < static_cast<int>(nodes.size()); ++n) {
      const size_t count = afterhours::EntityQuery(ec)
          .whereIndexed<IsDemoPort>(nodes[static_cast<size_t>(n)]).gen().size();
      total_ports += count;
      const bool selected = n == selected_node;
      const float y = 211.f + static_cast<float>(n) * 60.f;
      if (button(context, mk(entity, 10 + n), box(64, y, 312, 52)
          .with_custom_background(selected ? selbg : rowbg)
          .with_corner_radius(8 * s).with_debug_name(fmt::format("ei_dev_{}", n)))) {
        selected_node = n;
      }
      if (selected) {
        div(context, mk(entity, 110 + n), box(64, y + 7, 4, 38)
            .with_custom_background({154, 201, 252, 255}).with_corner_radius(2 * s)
            .with_ignore_pointer_events().with_debug_name("ei_selected_marker"));
      }
      text(120 + n, DEVICE[n], 80, y + 1, 219, 28, 21, ink,
           fmt::format("ei_device_name_{}", n));
      text(130 + n, DEVICE_TYPE[n], 80, y + 28, 219, 21, 16, muted,
           fmt::format("ei_device_type_{}", n));
      // Its own right-aligned column; inline spaces left the counts ragged.
      div(context, mk(entity, 80 + n), box(306, y + 12, 60, 28)
          .with_label(fmt::format("{} ports", count))
          .with_font("AtkinsonMock", pixels(15 * s))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Center)
          .with_custom_background({56, 74, 99, 255}).with_corner_radius(8 * s)
          .with_ignore_pointer_events().with_debug_name(fmt::format("ei_devcount_{}", n)));
    }
    text(103, fmt::format("{} ports across {} devices", total_ports, nodes.size()),
         64, 588, 312, 28, 19, ink, "ei_port_total");

    // ---- port table for the selected device --------------------------------
    div(context, mk(entity, 4), box(416, 120, 816, 334)
        .with_custom_background(panel).with_corner_radius(12 * s)
        .with_debug_name("ei_detailpanel"));
    text(104, "Selected device", 432, 133, 390, 25, 18, muted, "ei_selected_label");
    text(5, DEVICE[selected_node], 432, 161, 602, 39, 30, ink, "ei_detail_title");

    // The one indexed lookup the screen is actually about.
    auto ports = afterhours::EntityQuery(ec)
        .whereIndexed<IsDemoPort>(nodes[static_cast<size_t>(selected_node)]).gen();
    std::sort(ports.begin(), ports.end(), [](const auto &a, const auto &b) {
      return a.get().template get<IsDemoPort>().slot < b.get().template get<IsDemoPort>().slot;
    });
    text(105, fmt::format("{} ports", ports.size()), 1100, 164, 112, 33,
         23, ink, "ei_selected_count");
    text(20, "Slot index", 448, 210, 161, 30, 18, muted, "ei_col_0");
    text(21, "Port", 626, 210, 329, 30, 18, muted, "ei_col_1");
    text(22, "State", 974, 210, 226, 30, 18, muted, "ei_col_2");
    int row = 0;
    for (const auto &port : ports) {
      const int slot = port.get().get<IsDemoPort>().slot;
      const float y = 248.f + static_cast<float>(row) * 46.f;
      const bool connected = slot % kPortsPerNode != 2;
      div(context, mk(entity, 30 + row), box(432, y, 784, 40)
          .with_custom_background(rowbg).with_corner_radius(6 * s)
          .with_debug_name(fmt::format("ei_portrow_{}", row)));
      text(40 + row, std::to_string(slot), 448, y, 161, 40, 26, ink,
           fmt::format("ei_slot_{}", row), "AtkinsonMock");
      text(50 + row, PORT_NAME[slot % kPortsPerNode], 626, y, 329, 40,
           21, ink, fmt::format("ei_pname_{}", row));
      div(context, mk(entity, 140 + row), box(952, y + 15, 10, 10)
          .with_custom_background(connected ? good : rowbg)
          .with_corner_radius(5 * s).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            if (connected) return;
            raylib::DrawRing({r.x + r.width / 2, r.y + r.height / 2},
                            3 * s, 5 * s, 0, 360, 24, {168, 184, 208, 255});
          }).with_debug_name(connected ? "ei_connected_dot" : "ei_idle_dot"));
      if (!connected) {
        div(context, mk(entity, 150 + row), box(974, y + 5, 70, 30)
            .with_custom_background({50, 62, 81, 255}).with_corner_radius(8 * s)
            .with_ignore_pointer_events());
      }
      text(60 + row, connected ? "connected" : "idle", 974, y, 225, 40,
           20, connected ? good : ink, fmt::format("ei_pstate_{}", row));
      ++row;
    }
    text(106, "Slot indices start at 0. Port states are illustrative.",
         432, 427, 784, 22, 15, muted, "ei_table_note");

    // ---- the measurement, kept but subordinate -----------------------------
    const size_t resolves_before = afterhours::EntityCollection::stats().bucket_resolves;
    std::vector<int> indexed_ids;
    for (const auto &handle : nodes) {
      const auto found = afterhours::EntityQuery(ec).whereIndexed<IsDemoPort>(handle).gen();
      for (const auto &port : found) indexed_ids.push_back(port.get().id);
    }
    const size_t resolves = afterhours::EntityCollection::stats().bucket_resolves - resolves_before;
    size_t scanned = 0;
    std::vector<int> scanned_ids;
    for (const auto &handle : nodes) {
      const auto found = afterhours::EntityQuery(ec)
          .whereLambda([&](const afterhours::Entity &) { ++scanned; return true; })
          .whereHasComponent<IsDemoPort>()
          .whereLambda([&](const afterhours::Entity &value) {
            return value.get<IsDemoPort>().parent == handle;
          }).gen();
      for (const auto &port : found) scanned_ids.push_back(port.get().id);
    }
    std::sort(indexed_ids.begin(), indexed_ids.end());
    std::sort(scanned_ids.begin(), scanned_ids.end());
    const bool results_match = indexed_ids == scanned_ids;
    const size_t collection_size = static_cast<size_t>(std::count_if(
        ec.get_entities().begin(), ec.get_entities().end(),
        [](const auto &value) { return static_cast<bool>(value); }));
    div(context, mk(entity, 160), box(416, 478, 816, 152)
        .with_custom_background(panel).with_corner_radius(12 * s)
        .with_debug_name("ei_metrics_panel"));
    text(161, "Lookup work / 6 device queries / not elapsed time", 432, 488,
         784, 28, 20, ink, "ei_measurement_title");
    text(162, "Scan", 432, 524, 242, 24, 18, muted, "ei_scan_label");
    text(163, "Indexed lookup", 698, 524, 242, 24, 18, muted, "ei_index_label");
    text(164, "Live collection", 964, 524, 242, 24, 18, muted, "ei_collection_label");
    text(70, std::to_string(scanned), 432, 547, 242, 42, 38, ink, "ei_scan_count", "AtkinsonMock");
    text(72, std::to_string(resolves), 698, 547, 242, 42, 38, good, "ei_index_count", "AtkinsonMock");
    text(73, std::to_string(collection_size), 964, 547, 242, 42, 38,
         ink, "ei_collection_count", "AtkinsonMock");
    const float max_work = static_cast<float>(std::max(scanned, resolves));
    const auto work_bar = [&](int id, float x, size_t value, afterhours::Color color,
                              const char *name) {
      div(context, mk(entity, id), box(x, 592, 234, 7)
          .with_custom_background({46, 59, 78, 255}).with_debug_name(std::string(name) + "_track"));
      if (max_work <= 0 || value == 0) return;
      div(context, mk(entity, id + 1), box(x, 592, 234 * static_cast<float>(value) / max_work, 7)
          .with_custom_background(color).with_skip_grid_snap().with_debug_name(name));
    };
    work_bar(170, 432, scanned, {115, 158, 217, 255}, "ei_scan_bar");
    work_bar(172, 698, resolves, good, "ei_index_bar");
    text(174, "Entity checks / linear scale", 432, 603, 242, 22, 15, muted, "ei_scan_units");
    text(175, "Handle resolves / same scale", 698, 603, 242, 22, 15, muted, "ei_index_units");
    text(176, "Demo + app entities", 964, 603, 242, 22, 15, muted, "ei_collection_units");
    div(context, mk(entity, 180), box(48, 648, 1184, 44)
        .with_custom_background(results_match ? afterhours::Color{28, 57, 47, 255}
                                             : afterhours::Color{84, 41, 44, 255})
        .with_corner_radius(8 * s).with_debug_name("ei_result_banner"));
    text(71, results_match ? fmt::format("Results match: {}", indexed_ids.size())
                          : "Results differ", 64, 653, 296, 32, 23,
         results_match ? good : ink, "ei_agree");
    text(181, results_match ? "Scan and indexed lookup return the same port entity IDs."
                            : "Scan and indexed lookup returned different port entity IDs.",
         364, 653, 852, 32, 20, ink, "ei_result_explanation");
    text(182, "Lookup counts exclude index rebuild cost and other queries on this screen.",
         48, 694, 1184, 22, 16, muted, "ei_measurement_scope");
  }
};

REGISTER_EXAMPLE_SCREEN(entity_index_lab, "System Demos",
                        "parent to children through a secondary index",
                        EntityIndexLab)
