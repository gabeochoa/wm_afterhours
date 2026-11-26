#pragma once

// A Trellis-style fractal dockable workspace for wm_afterhours, built as a
// proof of what afterhours needs for first-class tiling-WM support (see
// docs/TRELLIS_LEARNINGS.md). One layout tree (splits / tab groups / leaves)
// plus a floating layer, an animated camera that frames any subtree,
// drag-to-dock with live previews, divider resizing, semantic-zoom tiles,
// spatial history and a serializable document. Panel state lives in the
// app, keyed by panel id, so panels never lose state when they move.

#include "../../../external.h"
#include <afterhours/ah.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dock {

using afterhours::Color;
using afterhours::RectangleType;
using afterhours::Vector2Type;

struct Node {
  enum class Kind { Split, Tabs, Leaf } kind = Kind::Leaf;
  bool vertical = false;  // Split: false = columns side by side
  std::vector<std::unique_ptr<Node>> children;
  std::vector<float> weights;
  std::vector<std::string> panels;  // Tabs
  int active = 0;
  std::string panel;  // Leaf

  static std::unique_ptr<Node> leaf(std::string id) {
    auto n = std::make_unique<Node>();
    n->kind = Kind::Leaf;
    n->panel = std::move(id);
    return n;
  }
  static std::unique_ptr<Node> tabs(std::vector<std::string> ids, int act = 0) {
    auto n = std::make_unique<Node>();
    n->kind = Kind::Tabs;
    n->panels = std::move(ids);
    n->active = act;
    return n;
  }
  template <typename... Kids>
  static std::unique_ptr<Node> splitv(bool vertical, std::vector<float> w,
                                      Kids... kids) {
    std::vector<std::unique_ptr<Node>> v;
    (v.push_back(std::move(kids)), ...);
    return split(vertical, std::move(v), std::move(w));
  }
  static std::unique_ptr<Node> split(
      bool vertical, std::vector<std::unique_ptr<Node>> kids,
      std::vector<float> w = {}) {
    auto n = std::make_unique<Node>();
    n->kind = Kind::Split;
    n->vertical = vertical;
    n->children = std::move(kids);
    n->weights = std::move(w);
    if (n->weights.size() != n->children.size())
      n->weights.assign(n->children.size(), 1.f);
    return n;
  }
  std::string active_panel() const {
    if (kind == Kind::Leaf) return panel;
    if (kind == Kind::Tabs && !panels.empty())
      return panels[std::clamp(active, 0, (int)panels.size() - 1)];
    return "";
  }
  bool contains(const std::string &id) const {
    if (kind == Kind::Leaf) return panel == id;
    if (kind == Kind::Tabs)
      return std::find(panels.begin(), panels.end(), id) != panels.end();
    for (auto &c : children)
      if (c->contains(id)) return true;
    return false;
  }
};

struct FloatWin {
  std::string id;
  RectangleType rect{80, 80, 360, 260};
};

struct Doc {
  std::unique_ptr<Node> root;
  std::vector<FloatWin> floating;
};

inline void heal_weights(Node &n) {
  if (n.weights.size() != n.children.size())
    n.weights.assign(n.children.size(), 1.f);
  float sum = 0.f;
  for (float w : n.weights) sum += std::max(w, 0.05f);
  if (sum <= 0.f) return;
  for (float &w : n.weights) w = std::max(w, 0.05f) / sum;
}

// Remove a panel from the tree; collapses single-child splits and
// single-tab groups (to a leaf). Returns false if the subtree became empty
// and should itself be removed by the caller.
inline bool remove_panel_from(std::unique_ptr<Node> &node,
                              const std::string &id) {
  if (!node) return false;
  if (node->kind == Node::Kind::Leaf) return node->panel != id;
  if (node->kind == Node::Kind::Tabs) {
    auto it = std::find(node->panels.begin(), node->panels.end(), id);
    if (it != node->panels.end()) {
      node->panels.erase(it);
      node->active = std::clamp(node->active, 0,
                                 std::max(0, (int)node->panels.size() - 1));
    }
    if (node->panels.empty()) return false;
    if (node->panels.size() == 1) {
      std::string only = node->panels[0];
      node = Node::leaf(only);
    }
    return true;
  }
  std::vector<std::unique_ptr<Node>> kept;
  std::vector<float> kept_w;
  for (size_t i = 0; i < node->children.size(); ++i) {
    if (remove_panel_from(node->children[i], id)) {
      kept.push_back(std::move(node->children[i]));
      kept_w.push_back(i < node->weights.size() ? node->weights[i] : 1.f);
    }
  }
  node->children = std::move(kept);
  node->weights = std::move(kept_w);
  if (node->children.empty()) return false;
  if (node->children.size() == 1) {
    node = std::move(node->children[0]);
    return true;
  }
  heal_weights(*node);
  return true;
}

inline Node *find_host(Node *n, const std::string &panel_id) {
  if (!n) return nullptr;
  if (n->kind != Node::Kind::Split)
    return n->contains(panel_id) ? n : nullptr;
  for (auto &c : n->children)
    if (Node *h = find_host(c.get(), panel_id)) return h;
  return nullptr;
}

// Insert `moving` (a fresh leaf node) beside/into the node hosting target.
// edge: 0 left, 1 right, 2 top, 3 bottom, 4 center (tabify).
inline bool insert_beside(std::unique_ptr<Node> &slot,
                          const std::string &target,
                          std::unique_ptr<Node> moving, int edge) {
  if (!slot) {
    slot = std::move(moving);
    return true;
  }
  if (slot->kind == Node::Kind::Split) {
    for (auto &c : slot->children)
      if (c->contains(target))
        return insert_beside(c, target, std::move(moving), edge);
    return false;
  }
  if (!slot->contains(target)) return false;
  if (edge == 4) {  // tabify
    std::string moving_id = moving->active_panel();
    if (slot->kind == Node::Kind::Tabs) {
      slot->panels.push_back(moving_id);
      slot->active = (int)slot->panels.size() - 1;
    } else {
      std::string host_id = slot->panel;
      slot = Node::tabs({host_id, moving_id}, 1);
    }
    return true;
  }
  const bool want_vertical = edge >= 2;
  const bool before = edge == 0 || edge == 2;
  auto host = std::move(slot);
  std::vector<std::unique_ptr<Node>> kids;
  if (before) {
    kids.push_back(std::move(moving));
    kids.push_back(std::move(host));
  } else {
    kids.push_back(std::move(host));
    kids.push_back(std::move(moving));
  }
  slot = Node::split(want_vertical, std::move(kids));
  return true;
}

inline std::string serialize_node(const Node *n) {
  if (!n) return "_";
  if (n->kind == Node::Kind::Leaf) return "L(" + n->panel + ")";
  if (n->kind == Node::Kind::Tabs) {
    std::string s = "T" + std::to_string(n->active) + "(";
    for (size_t i = 0; i < n->panels.size(); ++i)
      s += (i ? "," : "") + n->panels[i];
    return s + ")";
  }
  std::string s = n->vertical ? "V(" : "H(";
  for (size_t i = 0; i < n->children.size(); ++i) {
    if (i) s += ",";
    s += std::to_string(n->weights[i]).substr(0, 5) + ":" +
         serialize_node(n->children[i].get());
  }
  return s + ")";
}

inline std::string serialize_doc(const Doc &d) {
  std::string s = serialize_node(d.root.get());
  for (const FloatWin &f : d.floating) {
    s += ";F(" + f.id + "," + std::to_string((int)f.rect.x) + "," +
         std::to_string((int)f.rect.y) + "," +
         std::to_string((int)f.rect.width) + "," +
         std::to_string((int)f.rect.height) + ")";
  }
  return s;
}

// --- Global chrome safe areas (gallery shell) ---
// ScreenNavigator's "Browse screens" launcher (layer 2000) sits at the
// top-right: x = screen_width - 212, y = 8, 200x32. Trellis toolbars that
// extend under it lose clicks to that layer. These helpers let Trellis
// chrome reserve that strip (Trellis-local; the shell itself is untouched).
inline RectangleType global_launcher_rect(float screen_w) {
  return {screen_w - 212.f, 8.f, 200.f, 32.f};
}
inline bool rects_intersect(RectangleType a, RectangleType b) {
  return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height &&
         a.y + a.height > b.y;
}
// Page-local x so a toolbar of `toolbar_w` ends 12px left of the launcher.
// `page_screen_x` is the page's screen-space origin.
inline float toolbar_x_ending_before_launcher(float screen_w,
                                              float page_screen_x,
                                              float toolbar_w) {
  const float launcher_left_page =
      global_launcher_rect(screen_w).x - page_screen_x;
  return std::max(launcher_left_page - 12.f - toolbar_w, 0.f);
}
// How many tab-strip pixels are usable before header controls (F/x) start.
inline constexpr float kHeaderControlsReserve = 80.f;

template <typename Ctx> struct PanelDef {
  std::string title;
  Color color{120, 140, 200, 255};
  // Draw panel content as children of `parent` inside `content` (screen
  // coords). `k` is the camera scale (1 = unzoomed). Use mk(parent, n) with
  // panel-local ids.
  std::function<void(Ctx &, afterhours::Entity &, RectangleType, float)> draw;
};

template <typename Ctx> struct Workspace {
  Doc doc;
  std::map<std::string, PanelDef<Ctx>> defs;
  std::string focused;

  RectangleType view{0, 0, 1280, 720};         // camera rect (workspace units)
  RectangleType view_target{0, 0, 1280, 720};
  RectangleType root_rect{0, 0, 1280, 720};    // workspace space size
  std::vector<RectangleType> history;
  int hist_at = -1;

  // solved layout, rebuilt each render
  std::map<std::string, RectangleType> rects;  // panel -> workspace rect
  std::map<std::string, Node *> hosts;         // panel -> host node
  struct Divider {
    Node *split;
    int index;
    RectangleType rect;  // screen coords
    bool vertical;
  };
  std::vector<Divider> dividers;

  // drag state
  bool dragging = false;
  std::string drag_panel;
  Vector2Type drag_grab{0, 0};
  Vector2Type drag_pos{0, 0};
  bool drag_moved = false;
  std::string drop_target;
  int drop_edge = -1;       // 0..3 edges, 4 center, 5 float, 6..9 root edges
  RectangleType drop_preview{0, 0, 0, 0};
  Node *drag_divider_split = nullptr;
  int drag_divider_index = -1;
  bool drag_float_move = false;
  float last_click_t = -10.f;
  std::string last_click_panel;
  float clock_s = 0.f;
  // Hysteresis: the last committed drop target; a new candidate only wins
  // once the pointer leaves the committed preview (inflated), so targets
  // don't flicker while dragging across a panel.
  int stable_drop_edge = -1;
  std::string stable_drop_target;
  RectangleType stable_drop_preview{0, 0, 0, 0};

  static constexpr float kToolbarWidth = 452.f;  // 5 buttons * 84 + 4 * 8

  // Full workspace reset: layout is the caller's job (each app rebuilds its
  // own tree); this clears everything the workspace itself remembers so a
  // full app reset truly returns to defaults.
  void reset_camera_history() {
    view = root_rect;
    view_target = root_rect;
    history.clear();
    hist_at = -1;
    focused.clear();
    dragging = false;
    drag_panel.clear();
    drag_moved = false;
    drop_target.clear();
    drop_edge = -1;
    drop_preview = {0, 0, 0, 0};
    stable_drop_edge = -1;
    stable_drop_target.clear();
    stable_drop_preview = {0, 0, 0, 0};
    drag_divider_split = nullptr;
    drag_divider_index = -1;
    drag_float_move = false;
    last_click_t = -10.f;
    last_click_panel.clear();
  }

  std::string drop_label() const {
    const char *edge = "Float";
    switch (drop_edge) {
      case 0: edge = "Split left"; break;
      case 1: edge = "Split right"; break;
      case 2: edge = "Split top"; break;
      case 3: edge = "Split bottom"; break;
      case 4: edge = "Tab"; break;
      case 5: edge = "Float"; break;
      case 6: edge = "Dock left"; break;
      case 7: edge = "Dock right"; break;
      case 8: edge = "Dock top"; break;
      case 9: edge = "Dock bottom"; break;
      default: break;
    }
    std::string label = edge;
    if (!drop_target.empty()) label += " onto " + drop_target;
    return label;
  }

  void solve(Node *n, RectangleType r) {
    if (!n) return;
    if (n->kind == Node::Kind::Leaf) {
      rects[n->panel] = r;
      hosts[n->panel] = n;
      return;
    }
    if (n->kind == Node::Kind::Tabs) {
      if (!n->panels.empty()) {
        const std::string &a =
            n->panels[std::clamp(n->active, 0, (int)n->panels.size() - 1)];
        rects[a] = r;
        hosts[a] = n;
        for (auto &p : n->panels) hosts[p] = n;
      }
      return;
    }
    heal_weights(*n);
    const float gap = 6.f;
    float total = n->vertical ? r.height : r.width;
    float avail = total - gap * float(n->children.size() - 1);
    float pos = n->vertical ? r.y : r.x;
    for (size_t i = 0; i < n->children.size(); ++i) {
      float span = avail * n->weights[i];
      RectangleType cr = n->vertical
                             ? RectangleType{r.x, pos, r.width, span}
                             : RectangleType{pos, r.y, span, r.height};
      solve(n->children[i].get(), cr);
      pos += span + gap;
    }
  }

  float cam_k(RectangleType area) const {
    return std::min(area.width / view.width, area.height / view.height);
  }
  RectangleType to_screen(RectangleType area, RectangleType ws) const {
    const float k = cam_k(area);
    const float ox = area.x + (area.width - view.width * k) / 2.f;
    const float oy = area.y + (area.height - view.height * k) / 2.f;
    return {ox + (ws.x - view.x) * k, oy + (ws.y - view.y) * k, ws.width * k,
            ws.height * k};
  }
  Vector2Type to_ws(RectangleType area, Vector2Type screen) const {
    const float k = cam_k(area);
    const float ox = area.x + (area.width - view.width * k) / 2.f;
    const float oy = area.y + (area.height - view.height * k) / 2.f;
    return {(screen.x - ox) / k + view.x, (screen.y - oy) / k + view.y};
  }

  void push_history() {
    if (hist_at >= 0 && history[static_cast<size_t>(hist_at)].x == view_target.x &&
        history[static_cast<size_t>(hist_at)].width == view_target.width)
      return;
    history.erase(history.begin() + (hist_at + 1), history.end());
    history.push_back(view_target);
    hist_at = (int)history.size() - 1;
  }
  void frame_rect(RectangleType r, bool record = true) {
    view_target = r;
    if (record) push_history();
  }
  void frame_all() { frame_rect(root_rect); }
  void frame_panel(const std::string &id) {
    auto it = rects.find(id);
    if (it != rects.end()) frame_rect(it->second);
  }
  void step_out() {
    // grow view to the parent framing: double around center, clamp to root
    RectangleType v = view_target;
    float cx = v.x + v.width / 2.f, cy = v.y + v.height / 2.f;
    float w = std::min(v.width * 1.7f, root_rect.width);
    float h = std::min(v.height * 1.7f, root_rect.height);
    RectangleType nv{cx - w / 2.f, cy - h / 2.f, w, h};
    nv.x = std::clamp(nv.x, root_rect.x, root_rect.x + root_rect.width - w);
    nv.y = std::clamp(nv.y, root_rect.y, root_rect.y + root_rect.height - h);
    if (w >= root_rect.width * 0.99f && h >= root_rect.height * 0.99f)
      nv = root_rect;
    frame_rect(nv);
  }
  void nav_back() {
    if (hist_at > 0) view_target = history[static_cast<size_t>(--hist_at)];
  }
  void nav_forward() {
    if (hist_at + 1 < (int)history.size())
      view_target = history[static_cast<size_t>(++hist_at)];
  }

  bool is_floating(const std::string &id) const {
    for (auto &f : doc.floating)
      if (f.id == id) return true;
    return false;
  }
  void float_panel(const std::string &id, RectangleType ws_rect) {
    if (doc.root) {
      if (!remove_panel_from(doc.root, id)) doc.root.reset();
    }
    std::erase_if(doc.floating,
                  [&](const FloatWin &f) { return f.id == id; });
    // Never lose a window: keep floats fully inside the workspace.
    ws_rect.width = std::min(ws_rect.width, root_rect.width);
    ws_rect.height = std::min(ws_rect.height, root_rect.height);
    ws_rect.x = std::clamp(ws_rect.x, root_rect.x,
                           root_rect.x + root_rect.width - ws_rect.width);
    ws_rect.y = std::clamp(ws_rect.y, root_rect.y,
                           root_rect.y + root_rect.height - ws_rect.height);
    doc.floating.push_back({id, ws_rect});
  }
  void close_panel(const std::string &id) {
    std::erase_if(doc.floating,
                  [&](const FloatWin &f) { return f.id == id; });
    if (doc.root && !remove_panel_from(doc.root, id)) doc.root.reset();
    if (focused == id) focused = "";
  }
  void raise_float(const std::string &id) {
    auto it = std::find_if(doc.floating.begin(), doc.floating.end(),
                           [&](const FloatWin &f) { return f.id == id; });
    if (it != doc.floating.end()) {
      FloatWin f = *it;
      doc.floating.erase(it);
      doc.floating.push_back(f);
    }
  }

  // Render the whole workspace into `area` (screen coords) as children of
  // `parent`. `ui_scale` converts workspace design px for chrome text.
  void render(Ctx &ctx, afterhours::Entity &parent, RectangleType area,
              float dt) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    clock_s += dt;
    root_rect = {0, 0, area.width, area.height};

    // camera animation (one house motion: fast exp settle ~460ms feel)
    const float blend = 1.f - std::exp(-dt * 9.f);
    view.x += (view_target.x - view.x) * blend;
    view.y += (view_target.y - view.y) * blend;
    view.width += (view_target.width - view.width) * blend;
    view.height += (view_target.height - view.height) * blend;
    if (std::abs(view_target.x - view.x) < 0.5f &&
        std::abs(view_target.width - view.width) < 0.5f)
      view = view_target;

    rects.clear();
    hosts.clear();
    if (doc.root) solve(doc.root.get(), root_rect);
    for (auto &f : doc.floating) rects[f.id] = f.rect;

    const float k = cam_k(area);
    const float header_h = std::clamp(34.f * k, 24.f, 40.f);
    const Vector2Type mpos = ctx.mouse.pos;
    const Vector2Type mws = to_ws(area, mpos);

    // --- input: dividers, headers, floating windows (manual hit tests) ---
    handle_input(ctx, area, mpos, mws, k, header_h);

    // --- draw tiled panels ---
    std::vector<std::string> order;
    std::function<void(Node *)> collect = [&](Node *n) {
      if (!n) return;
      if (n->kind == Node::Kind::Split) {
        for (auto &c : n->children) collect(c.get());
      } else if (!n->active_panel().empty()) {
        order.push_back(n->active_panel());
      }
    };
    collect(doc.root.get());
    int seq = 0;
    for (auto &id : order) draw_panel(ctx, parent, id, area, k, header_h, seq);
    for (auto &f : doc.floating)
      draw_panel(ctx, parent, f.id, area, k, header_h, seq);

    // --- dividers (recomputed in screen space for hit tests next frame) ---
    dividers.clear();
    collect_dividers(doc.root.get(), area);

    // --- drop preview + drag ghost ---
    if (dragging && drop_edge >= 0) {
      div(ctx, mk(parent, 9000),
          ComponentConfig{}
              .with_absolute_position(drop_preview.x, drop_preview.y)
              .with_size({pixels(drop_preview.width),
                          pixels(drop_preview.height)})
              .with_custom_background(Color{50, 120, 255, 110})
              .with_border(Color{30, 100, 240, 255}, 3.f)
              .with_corner_radius(6.f)
              .with_ignore_pointer_events()
              .with_debug_name("dock_preview"));
      // Readable target label, centered in the preview.
      const float label_w = std::min(drop_preview.width, 300.f);
      div(ctx, mk(parent, 9002),
          ComponentConfig{}
              .with_absolute_position(
                  drop_preview.x + (drop_preview.width - label_w) / 2.f,
                  drop_preview.y + std::max(drop_preview.height / 2.f - 16.f, 4.f))
              .with_size({pixels(label_w), pixels(32.f)})
              .with_custom_background(Color{20, 28, 45, 245})
              .with_border(Color{120, 180, 255, 255}, 1.5f)
              .with_corner_radius(8.f)
              .with_label(drop_label())
              .with_font("FredokaMockBold", pixels(14.f))
              .with_custom_text_color(Color{255, 255, 255, 255})
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events()
              .with_debug_name("dock_preview_label"));
    }
    if (dragging && drag_moved) {
      auto it = defs.find(drag_panel);
      const std::string title = it != defs.end() ? it->second.title : drag_panel;
      const float ghost_w =
          std::clamp(drop_preview.width, 200.f, 340.f);
      div(ctx, mk(parent, 9001),
          ComponentConfig{}
              .with_absolute_position(drag_pos.x - drag_grab.x,
                                      drag_pos.y - drag_grab.y)
              .with_size({pixels(ghost_w), pixels(48.f)})
              .with_custom_background(Color{25, 32, 48, 245})
              .with_border(Color{120, 180, 255, 255}, 2.f)
              .with_corner_radius(8.f)
              .with_label(title + " \u2014 " + drop_label())
              .with_font("FredokaMockBold", pixels(14.f))
              .with_custom_text_color(Color{255, 255, 255, 255})
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events()
              .with_debug_name("dock_ghost"));
    }
  }

  void collect_dividers(Node *n, RectangleType area) {
    if (!n || n->kind != Node::Kind::Split) return;
    // recompute child rects the same way solve() does
    // (walk again with stored rects of first/last leaves is complex; instead
    // derive divider lines from child subtree bounds)
    std::vector<RectangleType> bounds;
    for (auto &c : n->children) bounds.push_back(subtree_bounds(c.get()));
    for (size_t i = 0; i + 1 < bounds.size(); ++i) {
      RectangleType a = bounds[i], b = bounds[i + 1];
      RectangleType gap = n->vertical
                              ? RectangleType{a.x, a.y + a.height, a.width,
                                              b.y - (a.y + a.height)}
                              : RectangleType{a.x + a.width, a.y,
                                              b.x - (a.x + a.width), a.height};
      RectangleType srect = to_screen(area, gap);
      // widen hit area (20px: the 6px gap plus a generous grab margin)
      if (n->vertical) {
        srect.y -= 7;
        srect.height += 14;
      } else {
        srect.x -= 7;
        srect.width += 14;
      }
      dividers.push_back({n, (int)i, srect, n->vertical});
    }
    for (auto &c : n->children) collect_dividers(c.get(), area);
  }

  RectangleType subtree_bounds(Node *n) {
    if (!n) return {0, 0, 0, 0};
    if (n->kind != Node::Kind::Split) {
      auto it = rects.find(n->active_panel());
      return it != rects.end() ? it->second : RectangleType{0, 0, 0, 0};
    }
    RectangleType u{1e9f, 1e9f, -1e9f, -1e9f};
    for (auto &c : n->children) {
      RectangleType b = subtree_bounds(c.get());
      u.x = std::min(u.x, b.x);
      u.y = std::min(u.y, b.y);
      u.width = std::max(u.width, b.x + b.width);
      u.height = std::max(u.height, b.y + b.height);
    }
    u.width -= u.x;
    u.height -= u.y;
    return u;
  }

  static bool inside(Vector2Type p, RectangleType r) {
    return p.x >= r.x && p.x <= r.x + r.width && p.y >= r.y &&
           p.y <= r.y + r.height;
  }

  void handle_input(Ctx &ctx, RectangleType area, Vector2Type mpos,
                    Vector2Type mws, float k, float header_h) {
    // Esc: cancel drag or step out
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      if (dragging) {
        dragging = false;
        drag_moved = false;
        drop_edge = -1;
        stable_drop_edge = -1;
        drop_target.clear();
      } else {
        step_out();
      }
    }
    // ctrl+wheel zoom to pointer, plain wheel over stage edge also zooms
    const float wheel = raylib::GetMouseWheelMove();
    if (wheel != 0.f &&
        (raylib::IsKeyDown(raylib::KEY_LEFT_CONTROL) ||
         raylib::IsKeyDown(raylib::KEY_LEFT_SUPER))) {
      const float f = wheel > 0 ? 0.82f : 1.22f;
      RectangleType v = view_target;
      float nw = std::clamp(v.width * f, 140.f, root_rect.width);
      float nh = std::clamp(v.height * f, 90.f, root_rect.height);
      float fx = (mws.x - v.x) / std::max(v.width, 1.f);
      float fy = (mws.y - v.y) / std::max(v.height, 1.f);
      v.x = std::clamp(mws.x - fx * nw, root_rect.x,
                       root_rect.x + root_rect.width - nw);
      v.y = std::clamp(mws.y - fy * nh, root_rect.y,
                       root_rect.y + root_rect.height - nh);
      v.width = nw;
      v.height = nh;
      if (nw >= root_rect.width * 0.99f) v = root_rect;
      view_target = v;
    }

    if (drag_divider_split) {
      if (!ctx.mouse.left_down) {
        drag_divider_split = nullptr;
      } else {
        Node *sp = drag_divider_split;
        // recompute weights from pointer along split axis
        // (bounds from subtree rects)
        std::vector<RectangleType> bounds;
        for (auto &c : sp->children) bounds.push_back(subtree_bounds(c.get()));
        if (drag_divider_index + 1 < (int)bounds.size()) {
          RectangleType all = subtree_bounds(sp);
          float gap = 6.f;
          if (sp->vertical) {
            float avail = all.height - gap * float(sp->children.size() - 1);
            float before = 0.f;
            for (int i = 0; i < drag_divider_index; ++i)
              before += avail * sp->weights[static_cast<size_t>(i)] + gap;
            float pair = avail * (sp->weights[drag_divider_index] +
                                  sp->weights[drag_divider_index + 1]);
            float want = std::clamp(mws.y - all.y - before, 40.f, pair - 40.f);
            sp->weights[drag_divider_index] = want / avail;
            sp->weights[drag_divider_index + 1] = (pair - want) / avail;
          } else {
            float avail = all.width - gap * float(sp->children.size() - 1);
            float before = 0.f;
            for (int i = 0; i < drag_divider_index; ++i)
              before += avail * sp->weights[static_cast<size_t>(i)] + gap;
            float pair = avail * (sp->weights[drag_divider_index] +
                                  sp->weights[drag_divider_index + 1]);
            float want = std::clamp(mws.x - all.x - before, 40.f, pair - 40.f);
            sp->weights[drag_divider_index] = want / avail;
            sp->weights[drag_divider_index + 1] = (pair - want) / avail;
          }
        }
      }
      return;
    }

    if (dragging) {
      drag_pos = mpos;
      drag_moved = drag_moved || std::abs(ctx.mouse.delta.x) + std::abs(ctx.mouse.delta.y) > 0.f;
      update_drop_target(area, mpos, k);
      if (ctx.mouse.just_released) {
        apply_drop(area);
        dragging = false;
        drag_moved = false;
        drop_edge = -1;
        stable_drop_edge = -1;
      }
      return;
    }

    if (!ctx.mouse.just_pressed) return;

    // divider grab?
    for (auto &d : dividers) {
      if (inside(mpos, d.rect)) {
        drag_divider_split = d.split;
        drag_divider_index = d.index;
        return;
      }
    }

    // header grab? floating first (topmost last)
    std::vector<std::string> try_order;
    for (auto it = doc.floating.rbegin(); it != doc.floating.rend(); ++it)
      try_order.push_back(it->id);
    for (auto &[id, r] : rects)
      if (!is_floating(id)) try_order.push_back(id);

    for (auto &id : try_order) {
      auto it = rects.find(id);
      if (it == rects.end()) continue;
      RectangleType sr = to_screen(area, it->second);
      RectangleType header{sr.x, sr.y, sr.width, header_h};
      if (!inside(mpos, header)) continue;
      // let header buttons (right side) have their clicks
      if (mpos.x > sr.x + sr.width - kHeaderControlsReserve) continue;
      focused = id;
      if (is_floating(id)) raise_float(id);

      // double-click = frame / step back
      if (last_click_panel == id && clock_s - last_click_t < 0.35f) {
        RectangleType wr = it->second;
        const bool framed =
            std::abs(view_target.x - wr.x) < 2.f &&
            std::abs(view_target.width - wr.width) < 2.f;
        if (framed)
          step_out();
        else
          frame_rect(wr);
        last_click_t = -10.f;
        return;
      }
      last_click_t = clock_s;
      last_click_panel = id;

      dragging = true;
      drag_panel = id;
      drag_moved = false;
      drag_float_move = is_floating(id);
      drag_grab = {mpos.x - sr.x, mpos.y - sr.y};
      drag_pos = mpos;
      return;
    }
  }

  void update_drop_target(RectangleType area, Vector2Type mpos, float k) {
    const int prev_edge = drop_edge;
    const std::string prev_target = drop_target;
    const RectangleType prev_preview = drop_preview;
    drop_edge = 5;  // float
    drop_target = "";
    drop_preview = {mpos.x - drag_grab.x, mpos.y - drag_grab.y, 220.f, 40.f};

    // workspace edges -> dock to root (44px bands: easy to hit at 720p)
    const float edge_band = 44.f;
    if (mpos.x < area.x + edge_band) { drop_edge = 6; drop_preview = {area.x, area.y, area.width * 0.3f, area.height}; }
    else if (mpos.x > area.x + area.width - edge_band) { drop_edge = 7; drop_preview = {area.x + area.width * 0.7f, area.y, area.width * 0.3f, area.height}; }
    else if (mpos.y < area.y + edge_band) { drop_edge = 8; drop_preview = {area.x, area.y, area.width, area.height * 0.3f}; }
    else if (mpos.y > area.y + area.height - edge_band) { drop_edge = 9; drop_preview = {area.x, area.y + area.height * 0.7f, area.width, area.height * 0.3f}; }
    else {
      for (auto &[id, wr] : rects) {
        if (id == drag_panel || is_floating(id)) continue;
        RectangleType sr = to_screen(area, wr);
        if (!inside(mpos, sr)) continue;
        const float fx = (mpos.x - sr.x) / std::max(sr.width, 1.f);
        const float fy = (mpos.y - sr.y) / std::max(sr.height, 1.f);
        const float dmin = std::min({fx, 1.f - fx, fy, 1.f - fy});
        drop_target = id;
        // Wide center zone (34%): tabify is the default drop, edges split.
        if (dmin > 0.34f) {
          drop_edge = 4;
          drop_preview = sr;
        } else if (fx == dmin) {
          drop_edge = 0;
          drop_preview = {sr.x, sr.y, sr.width / 2.f, sr.height};
        } else if (1.f - fx == dmin) {
          drop_edge = 1;
          drop_preview = {sr.x + sr.width / 2.f, sr.y, sr.width / 2.f, sr.height};
        } else if (fy == dmin) {
          drop_edge = 2;
          drop_preview = {sr.x, sr.y, sr.width, sr.height / 2.f};
        } else {
          drop_edge = 3;
          drop_preview = {sr.x, sr.y + sr.height / 2.f, sr.width, sr.height / 2.f};
        }
        break;
      }
    }
    (void)k;
    // Hysteresis: while the pointer stays inside the committed preview
    // (inflated 14px), keep that target even if raw geometry now prefers a
    // neighbour — no twitchy switching near boundaries.
    if (prev_edge >= 0 && prev_preview.width > 0.f &&
        (drop_edge != prev_edge || drop_target != prev_target)) {
      const RectangleType inflated{prev_preview.x - 14.f, prev_preview.y - 14.f,
                                   prev_preview.width + 28.f,
                                   prev_preview.height + 28.f};
      if (inside(mpos, inflated)) {
        drop_edge = prev_edge;
        drop_target = prev_target;
        drop_preview = prev_preview;
      }
    }
    stable_drop_edge = drop_edge;
    stable_drop_target = drop_target;
    stable_drop_preview = drop_preview;
  }

  void apply_drop(RectangleType area) {
    const std::string id = drag_panel;
    RectangleType old_screen{0, 0, 360, 260};
    if (auto it = rects.find(id); it != rects.end())
      old_screen = to_screen(area, it->second);
    RectangleType float_ws = to_ws_rect(area, drop_preview);
    if (float_ws.width < 40.f) float_ws.width = 360.f;
    if (float_ws.height < 40.f) float_ws.height = 260.f;

    if (drop_edge == 5) {  // float (or move float)
      RectangleType wr = float_ws;
      wr.width = std::max(old_screen.width / std::max(cam_k(area), 0.05f), 200.f);
      wr.height = std::max(old_screen.height / std::max(cam_k(area), 0.05f), 150.f);
      float_panel(id, wr);
      return;
    }
    // remove from current home first
    std::erase_if(doc.floating, [&](const FloatWin &f) { return f.id == id; });
    if (doc.root && !remove_panel_from(doc.root, id)) doc.root.reset();
    auto leaf = Node::leaf(id);
    if (drop_edge >= 6) {  // root edges
      const int edge = drop_edge - 6;  // 0 left 1 right 2 top 3 bottom
      if (!doc.root) {
        doc.root = std::move(leaf);
        return;
      }
      const bool vert = edge >= 2;
      const bool before = edge == 0 || edge == 2;
      std::vector<std::unique_ptr<Node>> kids;
      if (before) {
        kids.push_back(std::move(leaf));
        kids.push_back(std::move(doc.root));
      } else {
        kids.push_back(std::move(doc.root));
        kids.push_back(std::move(leaf));
      }
      doc.root = Node::split(vert, std::move(kids),
                             before ? std::vector<float>{0.3f, 0.7f}
                                    : std::vector<float>{0.7f, 0.3f});
      return;
    }
    if (!drop_target.empty()) {
      insert_beside(doc.root, drop_target, std::move(leaf), drop_edge);
    } else if (!doc.root) {
      doc.root = std::move(leaf);
    }
  }

  RectangleType to_ws_rect(RectangleType area, RectangleType sr) {
    Vector2Type tl = to_ws(area, {sr.x, sr.y});
    Vector2Type br = to_ws(area, {sr.x + sr.width, sr.y + sr.height});
    return {tl.x, tl.y, br.x - tl.x, br.y - tl.y};
  }


  // Small chrome row: title-agnostic navigation for the workspace.
  // Safe-area: the clamped x keeps the whole row left of the global
  // "Browse screens" launcher (layer 2000, top-right) so Reset & friends
  // are never hidden under it.
  void toolbar(Ctx &ctx, afterhours::Entity &parent, float x, float y,
               const std::function<void()> &on_reset,
               float page_screen_x = 0.f, float screen_width = 1280.f) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float safe_x = toolbar_x_ending_before_launcher(
        screen_width, page_screen_x, kToolbarWidth);
    const float bx0 = std::min(x, safe_x);
    auto mkbtn = [&](int id, float bx, const char *label, const char *name) {
      return button(ctx, mk(parent, 8000 + id),
                    ComponentConfig{}
                        .with_absolute_position(bx, y)
                        .with_size({pixels(84.f), pixels(30.f)})
                        .with_label(label)
                        .with_font("FredokaMockBold", pixels(12.5f))
                        .with_custom_text_color(Color{28, 36, 54, 255})
                        .with_custom_background(Color{255, 255, 255, 245})
                        .with_border(Color{105, 118, 145, 255}, 1.5f)
                        .with_corner_radius(8.f)
                        .with_debug_name(name));
    };
    if (mkbtn(0, bx0, "Overview", "dock_overview")) frame_all();
    if (mkbtn(1, bx0 + 92.f, "< Back", "dock_back")) nav_back();
    if (mkbtn(2, bx0 + 184.f, "Forward >", "dock_forward")) nav_forward();
    if (mkbtn(3, bx0 + 276.f, "Step out", "dock_step_out")) step_out();
    if (on_reset && mkbtn(4, bx0 + 368.f, "Reset", "dock_reset")) on_reset();
  }

  void draw_panel(Ctx &ctx, afterhours::Entity &parent, const std::string &id,
                  RectangleType area, float k, float header_h, int &seq) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    auto dit = defs.find(id);
    auto rit = rects.find(id);
    if (dit == defs.end() || rit == rects.end()) return;
    const PanelDef<Ctx> &def = dit->second;
    RectangleType sr = to_screen(area, rit->second);
    if (sr.x + sr.width < area.x || sr.y + sr.height < area.y ||
        sr.x > area.x + area.width || sr.y > area.y + area.height)
      return;
    const int base = 100 + seq * 40;
    seq++;

    const bool tiny = sr.width < 110.f || sr.height < 84.f;
    const bool is_focused = focused == id;

    auto frame = div(
        ctx, mk(parent, base),
        ComponentConfig{}
            .with_absolute_position(sr.x, sr.y)
            .with_size({pixels(std::max(sr.width, 8.f)),
                        pixels(std::max(sr.height, 8.f))})
            .with_custom_background(Color{250, 250, 252, 255})
            .with_border(is_focused ? Color{40, 110, 230, 255}
                                    : Color{110, 122, 148, 255},
                         is_focused ? 2.5f : 1.5f)
            .with_corner_radius(8.f)
            .with_debug_name("dock_" + id));

    if (tiny) {
      auto tile = button(
          ctx, mk(frame.ent(), 1),
          ComponentConfig{}
              .with_size({percent(1.f), percent(1.f)})
              .with_custom_background(def.color)
              .with_label(def.title)
              .with_font("FredokaMockBold",
                         pixels(std::clamp(sr.width * 0.16f, 9.f, 15.f)))
              .with_custom_text_color(Color{255, 255, 255, 255})
              .with_alignment(TextAlignment::Center)
              .with_corner_radius(8.f)
              .with_debug_name("dock_tile_" + id));
      if (tile) {
        focused = id;
        frame_rect(rit->second);
      }
      return;
    }

    Node *host = hosts.count(id) ? hosts[id] : nullptr;
    const bool tabbed = host && host->kind == Node::Kind::Tabs &&
                        host->panels.size() > 1;

    // header strip
    div(ctx, mk(frame.ent(), 2),
        ComponentConfig{}
            .with_absolute_position(0.f, 0.f)
            .with_size({pixels(sr.width), pixels(header_h)})
            .with_custom_background(is_focused ? Color{198, 218, 248, 255}
                                               : Color{218, 226, 242, 255})
            .with_corner_radius(8.f)
            .with_ignore_pointer_events()
            .with_debug_name("dock_header_" + id));

    if (tabbed) {
      float tx = 6.f;
      int ti = 0;
      // Truncate the strip before the header controls (F/x, 80px reserve)
      // so tabs never run under them; remaining tabs stay reachable by
      // dragging the panel wider or tabifying elsewhere.
      for (const std::string &pid : host->panels) {
        auto pd = defs.find(pid);
        const std::string title = pd != defs.end() ? pd->second.title : pid;
        float tw = 26.f + (float)title.size() * 7.5f * std::min(k, 1.f) + 18.f;
        const float avail = sr.width - kHeaderControlsReserve - tx;
        if (avail < 40.f) break;
        tw = std::min(tw, avail);
        const bool active_tab = pid == id;
        auto tab = button(
            ctx, mk(frame.ent(), 10 + ti),
            ComponentConfig{}
                .with_absolute_position(tx, 4.f)
                .with_size({pixels(tw), pixels(header_h - 8.f)})
                .with_label(title)
                .with_font("FredokaMockBold", pixels(12.f))
                .with_custom_text_color(active_tab ? Color{20, 28, 44, 255}
                                                   : Color{62, 72, 92, 255})
                .with_custom_background(active_tab ? Color{255, 255, 255, 255}
                                                   : Color{198, 208, 228, 255})
                .with_border(Color{110, 122, 148, 255}, 1.f)
                .with_corner_radius(6.f)
                .with_debug_name("dock_tab_" + pid));
        if (tab) {
          host->active = ti;
          focused = pid;
        }
        tx += tw + 4.f;
        ++ti;
      }
    } else {
      div(ctx, mk(frame.ent(), 3),
          ComponentConfig{}
              .with_absolute_position(10.f, 0.f)
              .with_size({pixels(std::max(sr.width - 90.f, 20.f)),
                          pixels(header_h)})
              .with_label(def.title)
              .with_font("FredokaMockBold", pixels(13.f))
              .with_custom_text_color(Color{28, 36, 54, 255})
              .with_alignment(TextAlignment::Left)
              .with_transparent_bg()
              .with_ignore_pointer_events()
              .with_debug_name("dock_title_" + id));
    }

    // header buttons: float/dock toggle + close
    auto float_btn = button(
        ctx, mk(frame.ent(), 30),
        ComponentConfig{}
            .with_absolute_position(sr.width - 68.f, 4.f)
            .with_size({pixels(30.f), pixels(header_h - 8.f)})
            .with_label(is_floating(id) ? "D" : "F")
            .with_font("FredokaMockBold", pixels(12.f))
            .with_custom_text_color(Color{45, 55, 75, 255})
            .with_custom_background(Color{255, 255, 255, 255})
            .with_border(Color{110, 122, 148, 255}, 1.f)
            .with_corner_radius(6.f)
            .with_debug_name("dock_float_" + id));
    if (float_btn) {
      if (is_floating(id)) {
        std::erase_if(doc.floating,
                      [&](const FloatWin &f) { return f.id == id; });
        if (doc.root) {
          // dock to the right of root
          std::vector<std::unique_ptr<Node>> kids;
          kids.push_back(std::move(doc.root));
          kids.push_back(Node::leaf(id));
          doc.root = Node::split(false, std::move(kids), {0.68f, 0.32f});
        } else {
          doc.root = Node::leaf(id);
        }
      } else {
        float_panel(id, rit->second);
      }
    }
    auto close_btn = button(
        ctx, mk(frame.ent(), 31),
        ComponentConfig{}
            .with_absolute_position(sr.width - 34.f, 4.f)
            .with_size({pixels(30.f), pixels(header_h - 8.f)})
            .with_label("x")
            .with_font("FredokaMockBold", pixels(12.f))
            .with_custom_text_color(Color{180, 50, 60, 255})
            .with_custom_background(Color{255, 255, 255, 255})
            .with_border(Color{110, 122, 148, 255}, 1.f)
            .with_corner_radius(6.f)
            .with_debug_name("dock_close_" + id));
    if (close_btn) close_panel(id);

    // content
    RectangleType content{sr.x + 6.f, sr.y + header_h + 4.f,
                          std::max(sr.width - 12.f, 10.f),
                          std::max(sr.height - header_h - 10.f, 10.f)};
    auto body = div(ctx, mk(frame.ent(), 4),
                    ComponentConfig{}
                        .with_absolute_position(6.f, header_h + 4.f)
                        .with_size({pixels(content.width),
                                    pixels(content.height)})
                        .with_transparent_bg()
                        .with_debug_name("dock_body_" + id));
    if (def.draw) def.draw(ctx, body.ent(), content, k);
  }
};

}  // namespace dock
