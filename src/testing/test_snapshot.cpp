#include "test_snapshot.h"
#include "../external.h"
#include "../game.h"
#include "../input_mapping.h"
#include <afterhours/ah.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>

namespace test_snapshot {

std::string get_snapshot_dir() { return "test_snapshots"; }

std::string get_snapshot_image_path(const std::string &name) {
  std::filesystem::path dir = get_snapshot_dir();
  std::filesystem::create_directories(dir);
  return (dir / (name + ".png")).string();
}

std::string get_snapshot_state_path(const std::string &name) {
  std::filesystem::path dir = get_snapshot_dir();
  std::filesystem::create_directories(dir);
  return (dir / (name + "_state.json")).string();
}

SnapshotResult capture_snapshot(const std::string &name, int /*screen_width*/,
                                int /*screen_height*/) {
  SnapshotResult result;
  result.snapshot_path = get_snapshot_image_path(name);

  if (mainRT.id == 0) {
    result.error_message = "Render texture not initialized";
    return result;
  }

  raylib::Image image = raylib::LoadImageFromTexture(mainRT.texture);
  if (image.data == nullptr) {
    result.error_message = "Failed to capture screenshot from render texture";
    return result;
  }

  raylib::ImageFlipVertical(&image);
  raylib::ExportImage(image, result.snapshot_path.c_str());
  raylib::UnloadImage(image);

  UIState state = capture_ui_state();
  std::string state_path = get_snapshot_state_path(name);
  if (!save_ui_state(state, state_path)) {
    result.error_message = "Failed to save UI state";
    return result;
  }

  result.success = true;
  return result;
}

SnapshotResult compare_snapshot(const std::string &name, int /*screen_width*/,
                                int /*screen_height*/, float tolerance) {
  SnapshotResult result;
  result.snapshot_path = get_snapshot_image_path(name);

  if (mainRT.id == 0) {
    result.error_message = "Render texture not initialized";
    return result;
  }

  raylib::Image current_image = raylib::LoadImageFromTexture(mainRT.texture);
  if (current_image.data == nullptr) {
    result.error_message =
        "Failed to capture current screenshot from render texture";
    return result;
  }
  raylib::ImageFlipVertical(&current_image);

  if (!std::filesystem::exists(result.snapshot_path)) {
    raylib::UnloadImage(current_image);
    result.error_message =
        "Snapshot file does not exist: " + result.snapshot_path;
    return result;
  }

  raylib::Image expected_image =
      raylib::LoadImage(result.snapshot_path.c_str());
  if (expected_image.data == nullptr) {
    raylib::UnloadImage(current_image);
    result.error_message =
        "Failed to load expected snapshot: " + result.snapshot_path;
    return result;
  }

  if (current_image.width != expected_image.width ||
      current_image.height != expected_image.height) {
    raylib::UnloadImage(current_image);
    raylib::UnloadImage(expected_image);
    result.error_message = "Image size mismatch: expected " +
                           std::to_string(expected_image.width) + "x" +
                           std::to_string(expected_image.height) + ", got " +
                           std::to_string(current_image.width) + "x" +
                           std::to_string(current_image.height);
    return result;
  }

  int differences = 0;
  float tolerance_pixels = tolerance * 255.0f;

  for (int y = 0; y < current_image.height; y++) {
    for (int x = 0; x < current_image.width; x++) {
      raylib::Color current_color = raylib::GetImageColor(current_image, x, y);
      raylib::Color expected_color =
          raylib::GetImageColor(expected_image, x, y);

      int r_diff = std::abs(static_cast<int>(current_color.r) -
                            static_cast<int>(expected_color.r));
      int g_diff = std::abs(static_cast<int>(current_color.g) -
                            static_cast<int>(expected_color.g));
      int b_diff = std::abs(static_cast<int>(current_color.b) -
                            static_cast<int>(expected_color.b));
      int a_diff = std::abs(static_cast<int>(current_color.a) -
                            static_cast<int>(expected_color.a));

      if (r_diff > tolerance_pixels || g_diff > tolerance_pixels ||
          b_diff > tolerance_pixels || a_diff > tolerance_pixels) {
        differences++;
      }
    }
  }

  result.pixel_differences = differences;

  if (differences > 0) {
    std::string diff_path = get_snapshot_dir() + "/" + name + "_diff.png";
    raylib::Image diff_image = raylib::ImageCopy(current_image);

    for (int y = 0; y < current_image.height; y++) {
      for (int x = 0; x < current_image.width; x++) {
        raylib::Color current_color =
            raylib::GetImageColor(current_image, x, y);
        raylib::Color expected_color =
            raylib::GetImageColor(expected_image, x, y);

        int r_diff = std::abs(static_cast<int>(current_color.r) -
                              static_cast<int>(expected_color.r));
        int g_diff = std::abs(static_cast<int>(current_color.g) -
                              static_cast<int>(expected_color.g));
        int b_diff = std::abs(static_cast<int>(current_color.b) -
                              static_cast<int>(expected_color.b));

        if (r_diff > tolerance_pixels || g_diff > tolerance_pixels ||
            b_diff > tolerance_pixels) {
          raylib::ImageDrawPixel(&diff_image, x, y, raylib::RED);
        } else {
          raylib::Color gray = {128, 128, 128, 255};
          raylib::ImageDrawPixel(&diff_image, x, y, gray);
        }
      }
    }

    raylib::ExportImage(diff_image, diff_path.c_str());
    raylib::UnloadImage(diff_image);
    result.diff_path = diff_path;
    result.error_message =
        "Snapshot comparison failed: " + std::to_string(differences) +
        " pixels differ (tolerance: " + std::to_string(tolerance) + ")";
  } else {
    result.success = true;
  }

  raylib::UnloadImage(current_image);
  raylib::UnloadImage(expected_image);

  std::string state_path = get_snapshot_state_path(name);
  UIState current_state = capture_ui_state();
  std::optional<UIState> expected_state = load_ui_state(state_path);

  if (expected_state.has_value()) {
    std::string state_diff;
    if (!compare_ui_states(expected_state.value(), current_state, state_diff)) {
      if (!result.success) {
        result.error_message += "\nUI State differences: " + state_diff;
      } else {
        result.error_message = "UI State differences: " + state_diff;
      }
      result.success = false;
    }
  }

  return result;
}

UIState capture_ui_state() {
  UIState state;

  auto *context = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  if (!context) {
    return state;
  }

  for (afterhours::Entity &entity :
       afterhours::EntityQuery()
           .whereHasComponent<afterhours::ui::UIComponent>()
           .gen()) {
    if (!entity.has<afterhours::ui::UIComponent>()) {
      continue;
    }

    const afterhours::ui::UIComponent &ui_comp =
        entity.get<afterhours::ui::UIComponent>();

    UIState::Element element;
    element.x = ui_comp.x();
    element.y = ui_comp.y();
    element.width = ui_comp.width();
    element.height = ui_comp.height();
    element.visible = ui_comp.was_rendered_to_screen;
    element.has_focus = context->has_focus(entity.id);

    if (entity.has<afterhours::ui::HasLabel>()) {
      element.label = entity.get<afterhours::ui::HasLabel>().label;
    }

    if (entity.has<afterhours::ui::UIComponentDebug>()) {
      const auto &debug = entity.get<afterhours::ui::UIComponentDebug>();
      element.debug_name = debug.name();
    }

    state.elements.push_back(element);
  }

  return state;
}

bool save_ui_state(const UIState &state, const std::string &path) {
  try {
    nlohmann::json json;
    json["elements"] = nlohmann::json::array();

    for (const auto &element : state.elements) {
      nlohmann::json elem_json;
      elem_json["label"] = element.label;
      elem_json["x"] = element.x;
      elem_json["y"] = element.y;
      elem_json["width"] = element.width;
      elem_json["height"] = element.height;
      elem_json["visible"] = element.visible;
      elem_json["has_focus"] = element.has_focus;
      elem_json["debug_name"] = element.debug_name;
      json["elements"].push_back(elem_json);
    }

    std::ofstream file(path);
    if (!file.is_open()) {
      return false;
    }
    file << json.dump(2);
    file.close();
    return true;
  } catch (...) {
    return false;
  }
}

std::optional<UIState> load_ui_state(const std::string &path) {
  if (!std::filesystem::exists(path)) {
    return std::nullopt;
  }

  try {
    std::ifstream file(path);
    if (!file.is_open()) {
      return std::nullopt;
    }

    nlohmann::json json;
    file >> json;
    file.close();

    UIState state;
    if (json.contains("elements") && json["elements"].is_array()) {
      for (const auto &elem_json : json["elements"]) {
        UIState::Element element;
        if (elem_json.contains("label")) {
          element.label = elem_json["label"].get<std::string>();
        }
        if (elem_json.contains("x")) {
          element.x = elem_json["x"].get<float>();
        }
        if (elem_json.contains("y")) {
          element.y = elem_json["y"].get<float>();
        }
        if (elem_json.contains("width")) {
          element.width = elem_json["width"].get<float>();
        }
        if (elem_json.contains("height")) {
          element.height = elem_json["height"].get<float>();
        }
        if (elem_json.contains("visible")) {
          element.visible = elem_json["visible"].get<bool>();
        }
        if (elem_json.contains("has_focus")) {
          element.has_focus = elem_json["has_focus"].get<bool>();
        }
        if (elem_json.contains("debug_name")) {
          element.debug_name = elem_json["debug_name"].get<std::string>();
        }
        state.elements.push_back(element);
      }
    }

    return state;
  } catch (...) {
    return std::nullopt;
  }
}

bool compare_ui_states(const UIState &expected, const UIState &actual,
                       std::string &diff_message) {
  std::ostringstream diff;
  bool has_diffs = false;

  if (expected.elements.size() != actual.elements.size()) {
    diff << "Element count mismatch: expected " << expected.elements.size()
         << ", got " << actual.elements.size() << "\n";
    has_diffs = true;
  }

  size_t min_size = std::min(expected.elements.size(), actual.elements.size());
  for (size_t i = 0; i < min_size; i++) {
    const auto &exp = expected.elements[i];
    const auto &act = actual.elements[i];

    if (exp.label != act.label) {
      diff << "Element " << i << " label mismatch: expected '" << exp.label
           << "', got '" << act.label << "'\n";
      has_diffs = true;
    }

    const float tolerance = 0.1f;
    if (std::abs(exp.x - act.x) > tolerance) {
      diff << "Element " << i << " x position mismatch: expected " << exp.x
           << ", got " << act.x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.y - act.y) > tolerance) {
      diff << "Element " << i << " y position mismatch: expected " << exp.y
           << ", got " << act.y << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.width - act.width) > tolerance) {
      diff << "Element " << i << " width mismatch: expected " << exp.width
           << ", got " << act.width << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.height - act.height) > tolerance) {
      diff << "Element " << i << " height mismatch: expected " << exp.height
           << ", got " << act.height << "\n";
      has_diffs = true;
    }
    if (exp.visible != act.visible) {
      diff << "Element " << i << " visibility mismatch: expected "
           << exp.visible << ", got " << act.visible << "\n";
      has_diffs = true;
    }
    if (exp.has_focus != act.has_focus) {
      diff << "Element " << i << " focus mismatch: expected " << exp.has_focus
           << ", got " << act.has_focus << "\n";
      has_diffs = true;
    }
  }

  diff_message = diff.str();
  return !has_diffs;
}

} // namespace test_snapshot
