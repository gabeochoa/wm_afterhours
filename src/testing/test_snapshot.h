#pragma once

#include "../rl.h"
#include <optional>
#include <string>
#include <vector>

namespace test_snapshot {

struct SnapshotResult {
  bool success = false;
  std::string error_message;
  std::string snapshot_path;
  std::string diff_path;
  int pixel_differences = 0;
};

struct UIState {
  struct Element {
    std::string label;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool visible = false;
    bool has_focus = false;
    std::string debug_name;
  };
  std::vector<Element> elements;
};

SnapshotResult capture_snapshot(const std::string &name, int screen_width,
                                int screen_height);
SnapshotResult compare_snapshot(const std::string &name, int screen_width,
                                int screen_height, float tolerance = 0.01f);
UIState capture_ui_state();
bool save_ui_state(const UIState &state, const std::string &path);
std::optional<UIState> load_ui_state(const std::string &path);
bool compare_ui_states(const UIState &expected, const UIState &actual,
                       std::string &diff_message);

} // namespace test_snapshot
