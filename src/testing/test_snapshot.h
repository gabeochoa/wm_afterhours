#pragma once

// DEPRECATED: Use E2E scripts (make test) and validate_screen instead.
// See AGENTS_UI.md. Kept for legacy coroutine tests that still reference it.

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
    int entity_id = -1;
    std::string label;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool visible = false;
    bool has_focus = false;
    std::string debug_name;
    
    bool is_absolute = false;
    bool should_hide = false;
    int parent_id = -1;
    std::vector<int> children_ids;
    
    float computed_x = 0.0f;
    float computed_y = 0.0f;
    float computed_rel_x = 0.0f;
    float computed_rel_y = 0.0f;
    
    float margin_top = 0.0f;
    float margin_left = 0.0f;
    float margin_bottom = 0.0f;
    float margin_right = 0.0f;
    float margin_x = 0.0f;
    float margin_y = 0.0f;
    
    float padding_top = 0.0f;
    float padding_left = 0.0f;
    float padding_bottom = 0.0f;
    float padding_right = 0.0f;
    float padding_x = 0.0f;
    float padding_y = 0.0f;
    
    std::string flex_direction = "Column";
    
    float translate_x = 0.0f;
    float translate_y = 0.0f;
    float scale = 1.0f;
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
