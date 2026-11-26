#pragma once
// WM-local plugin: ThemeBuilder devtool kit model.
//
// Non-UI state/model seam extracted from
// src/systems/screens/ThemeBuilderLab.h (Batch 2, Lane D — pluginization).
// Everything here is pure model code: no UIContext, no widgets, no ECS,
// no rendering, no afterhours/vendor edits. ThemeBuilderLab.h is the sole
// consumer and keeps every UI/rendering/popover behavior exactly as before.
//
// Model seams:
//   * token table (kTokens, order is the table order)
//   * token link graph (can_link cycle prevention + link_root resolution)
//   * resolved-theme computation (links flattened into a plain Theme)
//   * shape scalar conversion (0..1 slider <-> px, 24px / 6px scales)
//   * export path resolution (absolute as typed, relative vs save dir)
//   * snapshot/history (capture/restore, gesture coalescing, undo/redo)
// Public API lives in namespace wm::devtools::theme_builder. Header-only;
// there is no .cpp to link. See README.md in this directory for the
// plugin card, consumer, and promotion gate.

#include <afterhours/src/plugins/ui/theme.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace wm::devtools::theme_builder {

using afterhours::ui::Theme;

// ------------------------------------------------------------------
// Token table. Order is the table order; the category header prints
// on change. Mirrors the rows long served by ThemeBuilderLab.
// ------------------------------------------------------------------
struct TokenRow {
  Theme::Usage usage;
  const char *name;
  const char *debug;
  const char *category;
};

inline constexpr TokenRow kTokens[] = {
    {Theme::Usage::Font, "Text", "text", "Text"},
    {Theme::Usage::DarkFont, "Dark font", "darkfont", "Text"},
    {Theme::Usage::FontMuted, "Muted text", "muted", "Text"},
    {Theme::Usage::Background, "Background", "background", "Surfaces"},
    {Theme::Usage::Surface, "Surface", "surface", "Surfaces"},
    {Theme::Usage::Primary, "Primary", "primary", "Brand"},
    {Theme::Usage::Secondary, "Secondary", "secondary", "Brand"},
    {Theme::Usage::Accent, "Accent", "accent", "Brand"},
    {Theme::Usage::Error, "Error", "error", "Status"},
    {Theme::Usage::Success, "Success", "success", "Status"},
    {Theme::Usage::Warning, "Warning", "warning", "Status"},
    {Theme::Usage::Focus, "Focus ring", "focus", "Focus"},
};
inline constexpr int kTokenCount = 12;
static_assert(std::size(kTokens) == static_cast<std::size_t>(kTokenCount),
              "token table must list exactly kTokenCount rows");

// ------------------------------------------------------------------
// Shape scalars. Sliders are 0..1 fractions; the readouts and the
// theme are both derived from the fraction, so they can never
// disagree. 0.5 -> 12px panel radius, 3px focus ring.
// ------------------------------------------------------------------
inline constexpr float kPanelRadiusMaxPx = 24.f;
inline constexpr float kFocusRingMaxPx = 6.f;
inline constexpr std::size_t kHistoryCap = 100;

inline float clamp01(float v) { return std::clamp(v, 0.f, 1.f); }
inline float radius_px(float radius01) { return radius01 * kPanelRadiusMaxPx; }
inline float ring_px(float ring01) { return ring01 * kFocusRingMaxPx; }
// Inverses for theme-file import, clamped into the slider range.
inline float radius01_for(float radius_px_value) {
  return clamp01(radius_px_value / kPanelRadiusMaxPx);
}
inline float ring01_for(float thickness) {
  return clamp01(thickness / kFocusRingMaxPx);
}

// ------------------------------------------------------------------
// Token link graph.
//   LinkTable[i] = token index token i follows, or nothing for an
//   independent color. The swatch edits the independent color; the
//   resolved color is what the preview shows, so unlinking restores
//   the token's own color.
// ------------------------------------------------------------------
using LinkTable = std::array<std::optional<int>, kTokenCount>;

// Linking from -> to cycles if to's chain reaches from.
inline bool can_link(const LinkTable &link, int from, int to) {
  bool seen[kTokenCount] = {};
  int cur = to;
  while (cur >= 0 && cur < kTokenCount && !seen[cur]) {
    if (cur == from)
      return false;
    seen[cur] = true;
    cur = link[static_cast<std::size_t>(cur)].value_or(-1);
  }
  return true;
}

// Follow token's link chain to its root (the independent token, or
// the token itself when unlinked). Cycle-safe: a pre-existing cycle
// stops at the first repeated token, same as the in-screen walk.
inline int link_root(const LinkTable &link, int token) {
  int cur = token;
  bool seen[kTokenCount] = {};
  while (cur >= 0 && cur < kTokenCount && link[static_cast<std::size_t>(cur)] &&
         !seen[cur]) {
    seen[cur] = true;
    cur = *link[static_cast<std::size_t>(cur)];
  }
  return cur;
}

// The theme the preview renders and the theme file exports: the
// edited theme with links flattened in (a linked token takes the
// color of its root) and the shape scalars applied. The input theme
// is left untouched.
inline Theme resolve_theme(const Theme &edited, const LinkTable &link,
                           float radius01, float ring01) {
  Theme resolved = edited;
  resolved.panel_corner_radius = radius_px(radius01);
  resolved.focus_ring_thickness = ring_px(ring01);
  for (int i = 0; i < kTokenCount; i++) {
    if (link[static_cast<std::size_t>(i)]) {
      const int root = link_root(link, i);
      resolved.color_ref(kTokens[static_cast<std::size_t>(i)].usage) =
          edited.color_ref(kTokens[static_cast<std::size_t>(root)].usage);
    }
  }
  return resolved;
}

// ------------------------------------------------------------------
// Export path: absolute as typed, relative against the save dir.
// (The default is a bare filename, so it lands in the save dir.)
// The caller supplies the save dir (afterhours::files::get_save_path());
// an empty dir returns the path as typed.
// ------------------------------------------------------------------
inline std::string resolve_export_path(std::string_view export_path,
                                       const std::filesystem::path &save_dir) {
  const std::filesystem::path p(export_path);
  if (p.is_absolute())
    return std::string(export_path);
  return save_dir.empty() ? std::string(export_path)
                          : (save_dir / p).string();
}

// ------------------------------------------------------------------
// Snapshot / history.
// ------------------------------------------------------------------
struct Snapshot {
  std::array<afterhours::Color, kTokenCount> color{};
  float radius01 = 0.5f;
  float ring01 = 0.5f;
  LinkTable link{};
};

inline bool snapshots_equal(const Snapshot &a, const Snapshot &b) {
  if (a.radius01 != b.radius01 || a.ring01 != b.ring01)
    return false;
  for (int i = 0; i < kTokenCount; i++) {
    const auto &x = a.color[static_cast<std::size_t>(i)];
    const auto &y = b.color[static_cast<std::size_t>(i)];
    if (x.r != y.r || x.g != y.g || x.b != y.b || x.a != y.a)
      return false;
    if (a.link[static_cast<std::size_t>(i)] !=
        b.link[static_cast<std::size_t>(i)])
      return false;
  }
  return true;
}

inline Snapshot capture_snapshot(const Theme &edited, const LinkTable &link,
                                 float radius01, float ring01) {
  Snapshot s{};
  s.radius01 = radius01;
  s.ring01 = ring01;
  for (int i = 0; i < kTokenCount; i++) {
    s.color[static_cast<std::size_t>(i)] =
        edited.color_ref(kTokens[static_cast<std::size_t>(i)].usage);
    s.link[static_cast<std::size_t>(i)] =
        link[static_cast<std::size_t>(i)];
  }
  return s;
}

inline void restore_snapshot(const Snapshot &s, Theme &edited, LinkTable &link,
                             float &radius01, float &ring01) {
  for (int i = 0; i < kTokenCount; i++) {
    edited.color_ref(kTokens[static_cast<std::size_t>(i)].usage) =
        s.color[static_cast<std::size_t>(i)];
    link[static_cast<std::size_t>(i)] = s.link[static_cast<std::size_t>(i)];
  }
  radius01 = s.radius01;
  ring01 = s.ring01;
}

// Undo/redo history over snapshots. Edits on consecutive frames are
// one gesture (a slider drag, a typed hex), so one undo step reverts
// the whole gesture.
struct History {
  std::vector<Snapshot> undo_stack;
  std::vector<Snapshot> redo_stack;
  bool gesture_open = false;

  bool can_undo() const { return !undo_stack.empty(); }
  bool can_redo() const { return !redo_stack.empty(); }

  // The per-frame coalescing step: push the frame-start snapshot on
  // the first changed frame of a gesture, keep pushing nothing while
  // the gesture continues, and close the gesture on an unchanged
  // frame. A suppressed frame (undo/redo/import bookkeeping) does
  // not touch the history at all.
  void record_edit(const Snapshot &frame_start, const Snapshot &frame_end,
                   bool suppressed) {
    if (suppressed)
      return;
    if (!snapshots_equal(frame_start, frame_end)) {
      if (!gesture_open) {
        undo_stack.push_back(frame_start);
        if (undo_stack.size() > kHistoryCap)
          undo_stack.erase(undo_stack.begin());
        redo_stack.clear();
        gesture_open = true;
      }
    } else {
      gesture_open = false;
    }
  }

  // Pop one undo step into current (the pre-undo state moves to the
  // redo stack). Callers must check can_undo() first.
  void undo_to(Snapshot &current) {
    redo_stack.push_back(current);
    current = undo_stack.back();
    undo_stack.pop_back();
    gesture_open = false;
  }

  void redo_to(Snapshot &current) {
    undo_stack.push_back(current);
    current = redo_stack.back();
    redo_stack.pop_back();
    gesture_open = false;
  }
};

} // namespace wm::devtools::theme_builder
