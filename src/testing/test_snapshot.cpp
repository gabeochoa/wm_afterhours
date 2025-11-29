#include "test_snapshot.h"
#include "../external.h"
#include "../game.h"
#include "../input_mapping.h"
#include <afterhours/ah.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
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
    element.entity_id = static_cast<int>(entity.id);
    element.x = ui_comp.x();
    element.y = ui_comp.y();
    element.width = ui_comp.width();
    element.height = ui_comp.height();
    element.visible = ui_comp.was_rendered_to_screen;
    element.has_focus = context->has_focus(entity.id);
    
    element.is_absolute = ui_comp.absolute;
    element.should_hide = ui_comp.should_hide;
    element.parent_id = static_cast<int>(ui_comp.parent);
    
    for (afterhours::EntityID child_id : ui_comp.children) {
      element.children_ids.push_back(static_cast<int>(child_id));
    }
    
    element.computed_x = ui_comp.computed[afterhours::ui::Axis::X];
    element.computed_y = ui_comp.computed[afterhours::ui::Axis::Y];
    element.computed_rel_x = ui_comp.computed_rel[afterhours::ui::Axis::X];
    element.computed_rel_y = ui_comp.computed_rel[afterhours::ui::Axis::Y];
    
    element.margin_top = ui_comp.computed_margin[afterhours::ui::Axis::top];
    element.margin_left = ui_comp.computed_margin[afterhours::ui::Axis::left];
    element.margin_bottom = ui_comp.computed_margin[afterhours::ui::Axis::bottom];
    element.margin_right = ui_comp.computed_margin[afterhours::ui::Axis::right];
    element.margin_x = ui_comp.computed_margin[afterhours::ui::Axis::X];
    element.margin_y = ui_comp.computed_margin[afterhours::ui::Axis::Y];
    
    element.padding_top = ui_comp.computed_padd[afterhours::ui::Axis::top];
    element.padding_left = ui_comp.computed_padd[afterhours::ui::Axis::left];
    element.padding_bottom = ui_comp.computed_padd[afterhours::ui::Axis::bottom];
    element.padding_right = ui_comp.computed_padd[afterhours::ui::Axis::right];
    element.padding_x = ui_comp.computed_padd[afterhours::ui::Axis::X];
    element.padding_y = ui_comp.computed_padd[afterhours::ui::Axis::Y];
    
    if (ui_comp.flex_direction & afterhours::ui::FlexDirection::Column) {
      element.flex_direction = "Column";
    } else if (ui_comp.flex_direction & afterhours::ui::FlexDirection::Row) {
      element.flex_direction = "Row";
    } else {
      element.flex_direction = "None";
    }
    
    if (entity.has<afterhours::ui::HasUIModifiers>()) {
      const auto &modifiers = entity.get<afterhours::ui::HasUIModifiers>();
      element.translate_x = modifiers.translate_x;
      element.translate_y = modifiers.translate_y;
      element.scale = modifiers.scale;
    }

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
      elem_json["entity_id"] = element.entity_id;
      elem_json["label"] = element.label;
      elem_json["x"] = element.x;
      elem_json["y"] = element.y;
      elem_json["width"] = element.width;
      elem_json["height"] = element.height;
      elem_json["visible"] = element.visible;
      elem_json["has_focus"] = element.has_focus;
      elem_json["debug_name"] = element.debug_name;
      
      elem_json["is_absolute"] = element.is_absolute;
      elem_json["should_hide"] = element.should_hide;
      elem_json["parent_id"] = element.parent_id;
      elem_json["children_ids"] = element.children_ids;
      
      elem_json["computed_x"] = element.computed_x;
      elem_json["computed_y"] = element.computed_y;
      elem_json["computed_rel_x"] = element.computed_rel_x;
      elem_json["computed_rel_y"] = element.computed_rel_y;
      
      elem_json["margin_top"] = element.margin_top;
      elem_json["margin_left"] = element.margin_left;
      elem_json["margin_bottom"] = element.margin_bottom;
      elem_json["margin_right"] = element.margin_right;
      elem_json["margin_x"] = element.margin_x;
      elem_json["margin_y"] = element.margin_y;
      
      elem_json["padding_top"] = element.padding_top;
      elem_json["padding_left"] = element.padding_left;
      elem_json["padding_bottom"] = element.padding_bottom;
      elem_json["padding_right"] = element.padding_right;
      elem_json["padding_x"] = element.padding_x;
      elem_json["padding_y"] = element.padding_y;
      
      elem_json["flex_direction"] = element.flex_direction;
      
      elem_json["translate_x"] = element.translate_x;
      elem_json["translate_y"] = element.translate_y;
      elem_json["scale"] = element.scale;
      
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
        if (elem_json.contains("entity_id")) {
          element.entity_id = elem_json["entity_id"].get<int>();
        }
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
        
        if (elem_json.contains("is_absolute")) {
          element.is_absolute = elem_json["is_absolute"].get<bool>();
        }
        if (elem_json.contains("should_hide")) {
          element.should_hide = elem_json["should_hide"].get<bool>();
        }
        if (elem_json.contains("parent_id")) {
          element.parent_id = elem_json["parent_id"].get<int>();
        }
        if (elem_json.contains("children_ids")) {
          element.children_ids = elem_json["children_ids"].get<std::vector<int>>();
        }
        
        if (elem_json.contains("computed_x")) {
          element.computed_x = elem_json["computed_x"].get<float>();
        }
        if (elem_json.contains("computed_y")) {
          element.computed_y = elem_json["computed_y"].get<float>();
        }
        if (elem_json.contains("computed_rel_x")) {
          element.computed_rel_x = elem_json["computed_rel_x"].get<float>();
        }
        if (elem_json.contains("computed_rel_y")) {
          element.computed_rel_y = elem_json["computed_rel_y"].get<float>();
        }
        
        if (elem_json.contains("margin_top")) {
          element.margin_top = elem_json["margin_top"].get<float>();
        }
        if (elem_json.contains("margin_left")) {
          element.margin_left = elem_json["margin_left"].get<float>();
        }
        if (elem_json.contains("margin_bottom")) {
          element.margin_bottom = elem_json["margin_bottom"].get<float>();
        }
        if (elem_json.contains("margin_right")) {
          element.margin_right = elem_json["margin_right"].get<float>();
        }
        if (elem_json.contains("margin_x")) {
          element.margin_x = elem_json["margin_x"].get<float>();
        }
        if (elem_json.contains("margin_y")) {
          element.margin_y = elem_json["margin_y"].get<float>();
        }
        
        if (elem_json.contains("padding_top")) {
          element.padding_top = elem_json["padding_top"].get<float>();
        }
        if (elem_json.contains("padding_left")) {
          element.padding_left = elem_json["padding_left"].get<float>();
        }
        if (elem_json.contains("padding_bottom")) {
          element.padding_bottom = elem_json["padding_bottom"].get<float>();
        }
        if (elem_json.contains("padding_right")) {
          element.padding_right = elem_json["padding_right"].get<float>();
        }
        if (elem_json.contains("padding_x")) {
          element.padding_x = elem_json["padding_x"].get<float>();
        }
        if (elem_json.contains("padding_y")) {
          element.padding_y = elem_json["padding_y"].get<float>();
        }
        
        if (elem_json.contains("flex_direction")) {
          element.flex_direction = elem_json["flex_direction"].get<std::string>();
        }
        
        if (elem_json.contains("translate_x")) {
          element.translate_x = elem_json["translate_x"].get<float>();
        }
        if (elem_json.contains("translate_y")) {
          element.translate_y = elem_json["translate_y"].get<float>();
        }
        if (elem_json.contains("scale")) {
          element.scale = elem_json["scale"].get<float>();
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

  const float tolerance = 0.1f;
  
  std::map<int, const UIState::Element*> expected_by_id;
  for (const auto &elem : expected.elements) {
    expected_by_id[elem.entity_id] = &elem;
  }
  
  std::map<int, const UIState::Element*> actual_by_id;
  for (const auto &elem : actual.elements) {
    actual_by_id[elem.entity_id] = &elem;
  }
  
  std::set<int> all_ids;
  for (const auto &pair : expected_by_id) {
    all_ids.insert(pair.first);
  }
  for (const auto &pair : actual_by_id) {
    all_ids.insert(pair.first);
  }
  
  for (int id : all_ids) {
    auto exp_it = expected_by_id.find(id);
    auto act_it = actual_by_id.find(id);
    
    if (exp_it == expected_by_id.end()) {
      diff << "Element with id " << id << " missing in expected\n";
      has_diffs = true;
      continue;
    }
    if (act_it == actual_by_id.end()) {
      diff << "Element with id " << id << " missing in actual\n";
      has_diffs = true;
      continue;
    }
    
    const auto &exp = *exp_it->second;
    const auto &act = *act_it->second;

    if (exp.label != act.label) {
      diff << "Element " << id << " label mismatch: expected '" << exp.label
           << "', got '" << act.label << "'\n";
      has_diffs = true;
    }

    if (std::abs(exp.x - act.x) > tolerance) {
      diff << "Element " << id << " x position mismatch: expected " << exp.x
           << ", got " << act.x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.y - act.y) > tolerance) {
      diff << "Element " << id << " y position mismatch: expected " << exp.y
           << ", got " << act.y << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.width - act.width) > tolerance) {
      diff << "Element " << id << " width mismatch: expected " << exp.width
           << ", got " << act.width << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.height - act.height) > tolerance) {
      diff << "Element " << id << " height mismatch: expected " << exp.height
           << ", got " << act.height << "\n";
      has_diffs = true;
    }
    if (exp.visible != act.visible) {
      diff << "Element " << id << " visibility mismatch: expected "
           << exp.visible << ", got " << act.visible << "\n";
      has_diffs = true;
    }
    if (exp.has_focus != act.has_focus) {
      diff << "Element " << id << " focus mismatch: expected " << exp.has_focus
           << ", got " << act.has_focus << "\n";
      has_diffs = true;
    }
    
    if (exp.is_absolute != act.is_absolute) {
      diff << "Element " << id << " is_absolute mismatch: expected "
           << exp.is_absolute << ", got " << act.is_absolute << "\n";
      has_diffs = true;
    }
    if (exp.should_hide != act.should_hide) {
      diff << "Element " << id << " should_hide mismatch: expected "
           << exp.should_hide << ", got " << act.should_hide << "\n";
      has_diffs = true;
    }
    if (exp.parent_id != act.parent_id) {
      diff << "Element " << id << " parent_id mismatch: expected "
           << exp.parent_id << ", got " << act.parent_id << "\n";
      has_diffs = true;
    }
    
    if (std::abs(exp.computed_x - act.computed_x) > tolerance) {
      diff << "Element " << id << " computed_x mismatch: expected "
           << exp.computed_x << ", got " << act.computed_x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.computed_y - act.computed_y) > tolerance) {
      diff << "Element " << id << " computed_y mismatch: expected "
           << exp.computed_y << ", got " << act.computed_y << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.computed_rel_x - act.computed_rel_x) > tolerance) {
      diff << "Element " << id << " computed_rel_x mismatch: expected "
           << exp.computed_rel_x << ", got " << act.computed_rel_x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.computed_rel_y - act.computed_rel_y) > tolerance) {
      diff << "Element " << id << " computed_rel_y mismatch: expected "
           << exp.computed_rel_y << ", got " << act.computed_rel_y << "\n";
      has_diffs = true;
    }
    
    if (std::abs(exp.margin_top - act.margin_top) > tolerance) {
      diff << "Element " << id << " margin_top mismatch: expected "
           << exp.margin_top << ", got " << act.margin_top << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.margin_left - act.margin_left) > tolerance) {
      diff << "Element " << id << " margin_left mismatch: expected "
           << exp.margin_left << ", got " << act.margin_left << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.margin_bottom - act.margin_bottom) > tolerance) {
      diff << "Element " << id << " margin_bottom mismatch: expected "
           << exp.margin_bottom << ", got " << act.margin_bottom << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.margin_right - act.margin_right) > tolerance) {
      diff << "Element " << id << " margin_right mismatch: expected "
           << exp.margin_right << ", got " << act.margin_right << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.margin_x - act.margin_x) > tolerance) {
      diff << "Element " << id << " margin_x mismatch: expected "
           << exp.margin_x << ", got " << act.margin_x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.margin_y - act.margin_y) > tolerance) {
      diff << "Element " << id << " margin_y mismatch: expected "
           << exp.margin_y << ", got " << act.margin_y << "\n";
      has_diffs = true;
    }
    
    if (std::abs(exp.padding_top - act.padding_top) > tolerance) {
      diff << "Element " << id << " padding_top mismatch: expected "
           << exp.padding_top << ", got " << act.padding_top << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.padding_left - act.padding_left) > tolerance) {
      diff << "Element " << id << " padding_left mismatch: expected "
           << exp.padding_left << ", got " << act.padding_left << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.padding_bottom - act.padding_bottom) > tolerance) {
      diff << "Element " << id << " padding_bottom mismatch: expected "
           << exp.padding_bottom << ", got " << act.padding_bottom << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.padding_right - act.padding_right) > tolerance) {
      diff << "Element " << id << " padding_right mismatch: expected "
           << exp.padding_right << ", got " << act.padding_right << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.padding_x - act.padding_x) > tolerance) {
      diff << "Element " << id << " padding_x mismatch: expected "
           << exp.padding_x << ", got " << act.padding_x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.padding_y - act.padding_y) > tolerance) {
      diff << "Element " << id << " padding_y mismatch: expected "
           << exp.padding_y << ", got " << act.padding_y << "\n";
      has_diffs = true;
    }
    
    if (exp.flex_direction != act.flex_direction) {
      diff << "Element " << id << " flex_direction mismatch: expected '"
           << exp.flex_direction << "', got '" << act.flex_direction << "'\n";
      has_diffs = true;
    }
    
    if (std::abs(exp.translate_x - act.translate_x) > tolerance) {
      diff << "Element " << id << " translate_x mismatch: expected "
           << exp.translate_x << ", got " << act.translate_x << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.translate_y - act.translate_y) > tolerance) {
      diff << "Element " << id << " translate_y mismatch: expected "
           << exp.translate_y << ", got " << act.translate_y << "\n";
      has_diffs = true;
    }
    if (std::abs(exp.scale - act.scale) > tolerance) {
      diff << "Element " << id << " scale mismatch: expected "
           << exp.scale << ", got " << act.scale << "\n";
      has_diffs = true;
    }
    
    if (exp.children_ids != act.children_ids) {
      diff << "Element " << id << " children_ids mismatch\n";
      has_diffs = true;
    }
  }

  diff_message = diff.str();
  return !has_diffs;
}

} // namespace test_snapshot
