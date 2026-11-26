#pragma once
// Unit tests for the ThemeBuilder plugin model
// (src/devtools/theme_builder/theme_builder_model.h).
//
// Pure model checks only: no UIContext, no ECS, no raylib, no
// filesystem, so the tests are deterministic in headless
// --run-all-tests. Registered via testing/tests/all_tests.h.

#include "../../devtools/theme_builder/theme_builder_model.h"
#include "../test_macros.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace theme_builder_model_test {
using namespace wm::devtools::theme_builder;

inline void tb_assert(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

inline bool tb_color_eq(afterhours::Color a, afterhours::Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

// Token table indices (dialog order, see kTokens).
constexpr int kFont = 0;
constexpr int kPrimary = 5;
constexpr int kAccent = 7;
constexpr int kError = 8;

} // namespace theme_builder_model_test

// The table is the contract: order, names, debug labels and categories
// all reach the screen verbatim, and the column split reads them by
// index, so pin them here.
TEST(theme_builder_token_table) {
  using namespace theme_builder_model_test;

  tb_assert(kTokenCount == 12, "token table must hold 12 rows");
  tb_assert(kTokens[kFont].usage == Theme::Usage::Font &&
                std::string(kTokens[kFont].name) == "Text" &&
                std::string(kTokens[kFont].debug) == "text" &&
                std::string(kTokens[kFont].category) == "Text",
            "row 0 is the Text token");
  tb_assert(kTokens[kPrimary].usage == Theme::Usage::Primary &&
                std::string(kTokens[kPrimary].name) == "Primary" &&
                std::string(kTokens[kPrimary].category) == "Brand",
            "row 5 is the Primary token");
  tb_assert(kTokens[11].usage == Theme::Usage::Focus &&
                std::string(kTokens[11].name) == "Focus ring" &&
                std::string(kTokens[11].debug) == "focus" &&
                std::string(kTokens[11].category) == "Focus",
            "row 11 is the Focus token");
  for (int i = 0; i < kTokenCount; i++)
    tb_assert(Theme::is_valid(kTokens[i].usage),
              "every table row names a valid usage");
  co_return;
}

TEST(theme_builder_link_cycles) {
  using namespace theme_builder_model_test;

  LinkTable link{};
  link.fill(std::nullopt);
  // No links at all: anything can follow anything (except itself).
  tb_assert(can_link(link, kPrimary, kAccent), "empty table allows any link");
  tb_assert(can_link(link, kFont, kFont) == false,
            "a token cannot follow itself");

  // Primary follows Accent: Accent must not be able to follow Primary.
  link[kPrimary] = kAccent;
  tb_assert(can_link(link, kPrimary, kAccent),
            "Primary may follow Accent");
  tb_assert(can_link(link, kAccent, kPrimary) == false,
            "Accent cannot follow Primary (cycle)");

  // Three-deep chain 0 -> 1 -> 2: the root may not close the loop.
  link.fill(std::nullopt);
  link[0] = 1;
  link[1] = 2;
  tb_assert(can_link(link, 0, 2), "chain head may point at the root");
  tb_assert(can_link(link, 2, 0) == false,
            "chain root cannot point back at the head (cycle)");
  tb_assert(can_link(link, 1, 0) == false,
            "middle cannot point at the head (cycle)");
  co_return;
}

TEST(theme_builder_link_root) {
  using namespace theme_builder_model_test;

  LinkTable link{};
  link.fill(std::nullopt);
  tb_assert(link_root(link, 4) == 4, "an unlinked token is its own root");

  link[kPrimary] = kAccent;
  tb_assert(link_root(link, kPrimary) == kAccent,
            "direct link resolves to its target");

  link.fill(std::nullopt);
  link[0] = 1;
  link[1] = 2;
  tb_assert(link_root(link, 0) == 2, "chain resolves to the deepest token");
  tb_assert(link_root(link, 1) == 2, "chain middle resolves to the root");

  // A pre-existing cycle (unreachable through can_link) stops at the
  // first repeated token instead of walking forever.
  link.fill(std::nullopt);
  link[0] = 1;
  link[1] = 0;
  tb_assert(link_root(link, 0) == 0, "cycle stops at the first repeat");
  tb_assert(link_root(link, 1) == 1, "cycle stops at the first repeat");
  co_return;
}

TEST(theme_builder_resolved_theme) {
  using namespace theme_builder_model_test;

  Theme edited;
  edited.primary = afterhours::Color{1, 2, 3, 255};
  edited.accent = afterhours::Color{9, 8, 7, 255};
  edited.error = afterhours::Color{50, 60, 70, 255};

  LinkTable link{};
  link.fill(std::nullopt);
  link[kPrimary] = kAccent;

  const Theme resolved = resolve_theme(edited, link, 1.f, 0.f);
  tb_assert(resolved.panel_corner_radius == 24.f,
            "radius 1.0 flattens to 24px");
  tb_assert(resolved.focus_ring_thickness == 0.f,
            "ring 0.0 flattens to 0px");
  tb_assert(tb_color_eq(resolved.color_ref(Theme::Usage::Primary),
                        afterhours::Color{9, 8, 7, 255}),
            "a linked token resolves to its root's color");
  tb_assert(tb_color_eq(resolved.color_ref(Theme::Usage::Error),
                        afterhours::Color{50, 60, 70, 255}),
            "an unlinked token keeps its own color");
  tb_assert(tb_color_eq(edited.color_ref(Theme::Usage::Primary),
                        afterhours::Color{1, 2, 3, 255}),
            "resolve leaves the edited theme untouched");

  // Chain: Primary -> Secondary -> Accent resolves through both hops.
  link.fill(std::nullopt);
  link[kPrimary] = 6;
  link[6] = kAccent;
  const Theme chained = resolve_theme(edited, link, 0.5f, 0.5f);
  tb_assert(tb_color_eq(chained.color_ref(Theme::Usage::Primary),
                        afterhours::Color{9, 8, 7, 255}),
            "chained links resolve to the deepest root");
  tb_assert(chained.panel_corner_radius == 12.f &&
                chained.focus_ring_thickness == 3.f,
            "0.5 fractions flatten to 12px and 3px");

  // Scalar conversions round-trip, with clamping on import.
  tb_assert(radius_px(0.5f) == 12.f && ring_px(0.5f) == 3.f,
            "slider fractions become 12px and 3px at 0.5");
  tb_assert(radius01_for(24.f) == 1.f && radius01_for(12.f) == 0.5f,
            "radius import inverts the scale");
  tb_assert(radius01_for(999.f) == 1.f && radius01_for(-5.f) == 0.f &&
                ring01_for(6.f) == 1.f && ring01_for(3.f) == 0.5f,
            "import clamps into the slider range");
  co_return;
}

TEST(theme_builder_export_path) {
  using namespace theme_builder_model_test;

  tb_assert(resolve_export_path("/tmp/x.theme", "/save") == "/tmp/x.theme",
            "absolute paths pass through as typed");
  tb_assert(resolve_export_path("theme_builder_export.theme", "/save") ==
                "/save/theme_builder_export.theme",
            "relative paths resolve against the save dir");
  tb_assert(resolve_export_path("theme_builder_export.theme",
                                std::filesystem::path{}) ==
                "theme_builder_export.theme",
            "an empty save dir leaves the path as typed");
  tb_assert(resolve_export_path("sub/x.theme", "/save") ==
                "/save/sub/x.theme",
            "nested relative paths still resolve against the save dir");
  co_return;
}

TEST(theme_builder_history) {
  using namespace theme_builder_model_test;

  Theme edited;
  edited.primary = afterhours::Color{10, 20, 30, 255};
  edited.accent = afterhours::Color{40, 50, 60, 255};
  LinkTable link{};
  link.fill(std::nullopt);
  link[kPrimary] = kAccent;

  const Snapshot snap = capture_snapshot(edited, link, 0.25f, 0.75f);

  // Mutate everything, restore from the snapshot, and expect an
  // identical capture back.
  edited.primary = afterhours::Color{99, 99, 99, 255};
  link.fill(std::nullopt);
  float scratch_radius = 0.9f, scratch_ring = 0.1f;
  restore_snapshot(snap, edited, link, scratch_radius, scratch_ring);
  tb_assert(tb_color_eq(edited.color_ref(Theme::Usage::Primary),
                        afterhours::Color{10, 20, 30, 255}),
            "restore brings the edited colors back");
  tb_assert(link[kPrimary] == kAccent, "restore brings the links back");
  tb_assert(snapshots_equal(capture_snapshot(edited, link, 0.25f, 0.75f),
                            snap),
            "capture/restore round-trips");

  Snapshot other = snap;
  other.color[static_cast<std::size_t>(kFont)].r = 255;
  tb_assert(!snapshots_equal(snap, other), "a color change is a change");
  other = snap;
  other.link[static_cast<std::size_t>(kFont)] = kError;
  tb_assert(!snapshots_equal(snap, other), "a link change is a change");
  other = snap;
  other.ring01 = 0.1f;
  tb_assert(!snapshots_equal(snap, other), "a scalar change is a change");

  // Gesture coalescing: a run of changed frames pushes once.
  History history;
  Snapshot s0{}, s1{}, s2{}, s3{};
  s0.radius01 = 0.f;
  s1.radius01 = 0.1f;
  s2.radius01 = 0.2f;
  s3.radius01 = 0.3f;
  history.record_edit(s0, s1, false);
  tb_assert(history.can_undo() && history.undo_stack.size() == 1 &&
                history.undo_stack.front().radius01 == 0.f &&
                history.gesture_open,
            "first changed frame opens a gesture and pushes once");
  history.record_edit(s1, s2, false);
  tb_assert(history.undo_stack.size() == 1,
            "consecutive changed frames stay one gesture");
  history.record_edit(s2, s2, false);
  tb_assert(!history.gesture_open, "an unchanged frame closes the gesture");
  history.record_edit(s2, s3, false);
  tb_assert(history.undo_stack.size() == 2 &&
                history.undo_stack.back().radius01 == 0.2f,
            "the next change pushes a second step");
  history.record_edit(s3, s0, true);
  tb_assert(history.undo_stack.size() == 2 && history.gesture_open,
            "a suppressed frame leaves the gesture untouched");
  tb_assert(!history.can_redo(), "no redo yet");

  Snapshot current = s3;
  history.undo_to(current);
  tb_assert(current.radius01 == 0.2f && history.can_redo() &&
                history.undo_stack.size() == 1 && !history.gesture_open,
            "undo steps back to the previous frame");
  history.redo_to(current);
  tb_assert(current.radius01 == 0.3f && !history.can_redo() &&
                history.undo_stack.size() == 2,
            "redo reapplies the step");

  // Cap: 105 separate gestures keep only the newest 100 steps.
  History capped;
  Snapshot prev{}, next{};
  for (int i = 0; i < 105; i++) {
    prev.radius01 = static_cast<float>(i);
    next.radius01 = static_cast<float>(i + 1);
    capped.record_edit(prev, next, false);
    capped.record_edit(next, next, false);
  }
  tb_assert(capped.undo_stack.size() == wm::devtools::theme_builder::kHistoryCap,
            "history is capped at 100 steps");
  tb_assert(capped.undo_stack.front().radius01 == 5.f,
            "the oldest five steps are evicted first");
  co_return;
}
