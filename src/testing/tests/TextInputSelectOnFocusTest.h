#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

namespace text_input_test_helpers {

// Find the first entity with HasTextInputState whose debug name contains the
// given substring. Falls back to finding any entity with HasTextInputState.
inline afterhours::Entity *
find_text_input_by_debug_name(const std::string &name_substr) {
  auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;

  // First try: exact debug name match
  afterhours::OptEntity opt =
      afterhours::EntityQuery(
          ui_coll,
          afterhours::EntityQuery<>::QueryOptions{.force_merge = true})
          .whereHasComponent<afterhours::text_input::HasTextInputState>()
          .whereComponentAndLambda<afterhours::ui::UIComponentDebug>(
              [&name_substr](const afterhours::ui::UIComponentDebug &d) {
                return d.name_value.find(name_substr) != std::string::npos;
              })
          .gen_first();
  if (opt.has_value())
    return &opt.asE();

  // Fallback: find any entity with HasTextInputState (first one)
  afterhours::OptEntity fallback =
      afterhours::EntityQuery(
          ui_coll,
          afterhours::EntityQuery<>::QueryOptions{.force_merge = true})
          .whereHasComponent<afterhours::text_input::HasTextInputState>()
          .gen_first();
  return fallback.has_value() ? &fallback.asE() : nullptr;
}

inline void expect_text_input_value(const std::string &debug_name_substr,
                                    const std::string &expected_value) {
  auto *ent = find_text_input_by_debug_name(debug_name_substr);
  if (!ent) {
    throw std::runtime_error("Text input not found: " + debug_name_substr);
  }
  auto &state = ent->get<afterhours::text_input::HasTextInputState>();
  std::string actual = state.text();
  if (actual != expected_value) {
    throw std::runtime_error("Expected text input '" + debug_name_substr +
                             "' to have value '" + expected_value +
                             "', but got '" + actual + "'");
  }
}

inline void expect_text_input_has_selection(const std::string &debug_name_substr,
                                            bool should_have_selection) {
  auto *ent = find_text_input_by_debug_name(debug_name_substr);
  if (!ent) {
    throw std::runtime_error("Text input not found: " + debug_name_substr);
  }
  auto &state = ent->get<afterhours::text_input::HasTextInputState>();
  if (should_have_selection && !state.has_selection()) {
    throw std::runtime_error("Expected text input '" + debug_name_substr +
                             "' to have selection, but it does not");
  }
  if (!should_have_selection && state.has_selection()) {
    throw std::runtime_error("Expected text input '" + debug_name_substr +
                             "' to have no selection, but it has one (anchor=" +
                             std::to_string(*state.selection_anchor) +
                             " cursor=" +
                             std::to_string(state.cursor_position) + ")");
  }
}

inline void
expect_text_input_full_selection(const std::string &debug_name_substr) {
  auto *ent = find_text_input_by_debug_name(debug_name_substr);
  if (!ent) {
    throw std::runtime_error("Text input not found: " + debug_name_substr);
  }
  auto &state = ent->get<afterhours::text_input::HasTextInputState>();
  if (!state.has_selection()) {
    throw std::runtime_error("Expected text input '" + debug_name_substr +
                             "' to have full selection, but no selection");
  }
  size_t sel_start = state.selection_start();
  size_t sel_end = state.selection_end();
  if (sel_start != 0 || sel_end != state.text_size()) {
    throw std::runtime_error(
        "Expected text input '" + debug_name_substr +
        "' to have full selection (0-" + std::to_string(state.text_size()) +
        "), but selection is (" + std::to_string(sel_start) + "-" +
        std::to_string(sel_end) + ")");
  }
}

} // namespace text_input_test_helpers

TEST(text_input_select_on_focus) {
  using namespace text_input_test_helpers;

  co_await TestApp::wait_for_frames(5);

  // The Username field auto-focuses when the screen loads.
  // Verify it's focused before typing.
  {
    auto *ent = find_text_input_by_debug_name("Username_input");
    if (!ent) {
      throw std::runtime_error("Username_input not found");
    }
    auto &state = ent->get<afterhours::text_input::HasTextInputState>();
    if (!state.is_focused) {
      throw std::runtime_error("Username_input should be auto-focused");
    }
  }

  // Field is empty and focused, type "hello"
  // One character is consumed per frame, so wait enough frames
  TestApp::simulate_typing("hello");
  co_await TestApp::wait_for_frames(10);

  expect_text_input_value("Username_input", "hello");

  // Tab away from Username and cycle through all fields to come back,
  // triggering select-all-on-focus.
  bool refocused = false;
  for (int i = 0; i < 30 && !refocused; i++) {
    TestApp::simulate_tab();
    // Need 4 frames: HandleTabbing may use a ROOT intermediary when wrapping
    // (last→ROOT in frame 1, ROOT→first in frame 2, Screen updates in frame 3,
    //  TestSystem reads in frame 4).
    co_await TestApp::wait_for_frames(4);

    auto *ent = find_text_input_by_debug_name("Username_input");
    if (ent &&
        ent->get<afterhours::text_input::HasTextInputState>().is_focused) {
      refocused = true;
    }
  }
  if (!refocused) {
    throw std::runtime_error("Could not tab back to Username_input");
  }

  // Text should be fully selected after tabbing back
  expect_text_input_full_selection("Username_input");

  // Type "world" — should replace the selected "hello"
  TestApp::simulate_typing("world");
  co_await TestApp::wait_for_frames(10);

  expect_text_input_value("Username_input", "world");
}
