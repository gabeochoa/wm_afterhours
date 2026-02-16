#pragma once

#include "../components.h"
#include "../input_mapping.h"
#include "../rl.h"
#include "../settings.h"
#include "test_feedback.h"
#include "test_input.h"
#include "test_snapshot.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/ui_collection.h>
#include <coroutine>
#include <functional>
#include <string>
#include <vector>

namespace test_app {
extern int frame_counter;
}

struct TestApp {
  struct promise_type {
    std::string error_message;
    bool done = false;
    int wait_until_frame =
        0; // If > 0, don't continue until frame_counter >= this

    TestApp get_return_object() {
      return TestApp{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {
      try {
        std::rethrow_exception(std::current_exception());
      } catch (const std::exception &e) {
        error_message = e.what();
      }
      done = true;
    }
    void return_void() { done = true; }

    bool should_continue() const {
      if (wait_until_frame > 0 && test_app::frame_counter < wait_until_frame) {
        return false;
      }
      return true;
    }
  };

  std::coroutine_handle<promise_type> handle;

  TestApp(std::coroutine_handle<promise_type> h) : handle(h) {}

  TestApp(const TestApp &) = delete;
  TestApp &operator=(const TestApp &) = delete;

  TestApp(TestApp &&other) noexcept : handle(other.handle) {
    other.handle = {};
  }

  TestApp &operator=(TestApp &&other) noexcept {
    if (this != &other) {
      if (handle) {
        handle.destroy();
      }
      handle = other.handle;
      other.handle = {};
    }
    return *this;
  }

  ~TestApp() {
    if (handle) {
      handle.destroy();
    }
  }

  bool is_done() const {
    if (!handle)
      return true;
    return handle.promise().done;
  }

  std::string get_error() const {
    if (!handle)
      return "";
    return handle.promise().error_message;
  }

  void resume() {
    if (handle && !handle.done()) {
      handle.resume();
    }
  }

  static void simulate_key(int key) { test_input::push_key(key); }

  static void simulate_char(char c) { test_input::push_char(c); }

  static void simulate_typing(const std::string &text) {
    for (char c : text) {
      test_input::push_char(c);
    }
  }

  static void simulate_tab() { test_input::simulate_tab(); }

  static void simulate_shift_tab() { test_input::simulate_shift_tab(); }

  static void simulate_arrow_key(int arrow_key) {
    test_input::simulate_arrow_key(arrow_key);
  }

  static void simulate_enter() { test_input::simulate_enter(); }

  static void simulate_escape() { test_input::simulate_escape(); }

  /// Simulate a window resize by updating the ProvidesCurrentResolution
  /// singleton and Settings. The UI layout system will pick up the new
  /// dimensions on the next frame's BeforeUIUpdates pass.
  static void simulate_window_resize(int width, int height) {
    // Update the ECS resolution singleton that the UI system reads
    auto *pcr = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    if (pcr) {
      pcr->current_resolution.width = width;
      pcr->current_resolution.height = height;
      pcr->should_refetch = false; // Prevent CollectCurrentResolution overwrite
    }
    // Update Settings so screens that read from Settings directly also see it
    Settings::get().update_resolution(
        afterhours::window_manager::Resolution{.width = width,
                                               .height = height});
  }

  /// Check that the current screen resolution matches expected values.
  static void expect_screen_size(int expected_w, int expected_h) {
    int actual_w = Settings::get().get_screen_width();
    int actual_h = Settings::get().get_screen_height();
    if (actual_w != expected_w || actual_h != expected_h) {
      throw std::runtime_error(
          "Expected screen size " + std::to_string(expected_w) + "x" +
          std::to_string(expected_h) + ", but got " +
          std::to_string(actual_w) + "x" + std::to_string(actual_h));
    }
  }

  static afterhours::Entity *
  find_ui_element_by_label(const std::string &label) {
    // Search UI collection where immediate-mode UI elements live.
    // Use force_merge to include recently-created temp entities.
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    afterhours::OptEntity opt =
        afterhours::EntityQuery(
            ui_coll,
            afterhours::EntityQuery<>::QueryOptions{.force_merge = true})
            .whereHasComponent<afterhours::ui::HasLabel>()
            .whereLambda([&label](const afterhours::Entity &e) {
              return e.get<afterhours::ui::HasLabel>().label == label;
            })
            .gen_first();
    return opt.has_value() ? &opt.asE() : nullptr;
  }

  static std::optional<afterhours::EntityHandle>
  find_ui_element_handle_by_label(const std::string &label) {
    // Search UI collection where immediate-mode UI elements live.
    // Use force_merge to include recently-created temp entities.
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    return afterhours::EntityQuery(
               ui_coll,
               afterhours::EntityQuery<>::QueryOptions{.force_merge = true})
        .whereHasComponent<afterhours::ui::HasLabel>()
        .whereLambda([&label](const afterhours::Entity &e) {
          return e.get<afterhours::ui::HasLabel>().label == label;
        })
        .gen_first_handle();
  }

  static void click_ui_element(afterhours::Entity &entity) {
    if (!entity.has<afterhours::ui::UIComponent>()) {
      throw std::runtime_error("Entity does not have UIComponent");
    }

    const afterhours::ui::UIComponent &ui_comp =
        entity.get<afterhours::ui::UIComponent>();

    // Get the base rect
    RectangleType rect = ui_comp.rect();

    // Apply translation modifiers if present (from with_translate)
    if (entity.has<afterhours::ui::HasUIModifiers>()) {
      rect = entity.get<afterhours::ui::HasUIModifiers>().apply_modifier(rect);
    }

    float center_x = rect.x + rect.width / 2.0f;
    float center_y = rect.y + rect.height / 2.0f;

    // Set mouse position and press button
    test_input::set_mouse_position(vec2{center_x, center_y});
    test_input::simulate_mouse_button_press(raylib::MOUSE_BUTTON_LEFT);
  }

  // Schedule a mouse release for the next frame (call after click_ui_element
  // and waiting)
  static void release_mouse_button() {
    test_input::simulate_mouse_button_release(raylib::MOUSE_BUTTON_LEFT);
  }

  static void click_button(const std::string &label) {
    test_feedback::show_toast("Clicking button: " + label);
    afterhours::Entity *element = find_ui_element_by_label(label);
    if (!element) {
      test_feedback::show_toast("Button not found: " + label, "error");
      throw std::runtime_error("UI element not found: " + label);
    }
    click_ui_element(*element);
  }

  static void expect_ui_exists(const std::string &label) {
    afterhours::Entity *element = find_ui_element_by_label(label);
    if (!element) {
      throw std::runtime_error("Expected UI element to exist: " + label);
    }
  }

  static void expect_ui_not_exists(const std::string &label) {
    afterhours::Entity *element = find_ui_element_by_label(label);
    if (element) {
      throw std::runtime_error("Expected UI element to not exist: " + label);
    }
  }

  struct WaitFrames {
    int target_frame;
    int start_frame;
    int slow_multiplier;

    WaitFrames(int frames)
        : target_frame(test_app::frame_counter + frames),
          start_frame(test_app::frame_counter),
          slow_multiplier(test_input::slow_test_mode ? 500 : 1) {
      if (test_input::slow_test_mode) {
        target_frame = test_app::frame_counter + (frames * slow_multiplier);
      }
    }

    bool await_ready() const { return test_app::frame_counter >= target_frame; }
    void await_suspend(std::coroutine_handle<promise_type> h) {
      // Set the wait_until_frame in the promise so TestSystem knows not to
      // continue
      h.promise().wait_until_frame = target_frame;
    }
    void await_resume() {}
  };

  static WaitFrames wait_for_frames(int frames) { return WaitFrames{frames}; }

  template <typename Func> struct WaitCondition {
    Func condition;
    int max_frames;
    int start_frame;

    WaitCondition(Func cond, int max)
        : condition(cond), max_frames(max),
          start_frame(test_app::frame_counter) {}

    bool await_ready() const {
      if (condition()) {
        return true;
      }
      if (test_app::frame_counter - start_frame >= max_frames) {
        return true;
      }
      return false;
    }
    void await_suspend(std::coroutine_handle<promise_type>) {}
    bool await_resume() {
      bool result = condition();
      if (!result && (test_app::frame_counter - start_frame >= max_frames)) {
        throw std::runtime_error("Condition not met within max frames");
      }
      return result;
    }
  };

  template <typename Func>
  static WaitCondition<Func> wait_for_condition(Func condition,
                                                int max_frames = 300) {
    return WaitCondition<Func>{condition, max_frames};
  }

  static auto wait_for_ui_exists(const std::string &label,
                                 int max_frames = 300) {
    return wait_for_condition(
        [label]() { return find_ui_element_by_label(label) != nullptr; },
        max_frames);
  }

  static afterhours::Entity *get_focused_element() {
    auto *context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (!context) {
      return nullptr;
    }
    if (context->focus_id == context->ROOT) {
      return nullptr;
    }
    // Search both UI and default collections for the focused entity
    auto opt_entity =
        afterhours::ui::UICollectionHolder::getEntityForID(context->focus_id);
    if (!opt_entity.has_value()) {
      return nullptr;
    }
    return &opt_entity.asE();
  }

  static void expect_focus(const std::string &label) {
    afterhours::Entity *focused = get_focused_element();
    if (!focused) {
      throw std::runtime_error(
          "Expected element to have focus, but no element is focused");
    }
    if (!focused->has<afterhours::ui::HasLabel>()) {
      throw std::runtime_error("Focused element does not have a label");
    }
    const afterhours::ui::HasLabel &has_label =
        focused->get<afterhours::ui::HasLabel>();
    if (has_label.label != label) {
      throw std::runtime_error("Expected focus on element with label '" +
                               label + "', but focused element has label '" +
                               has_label.label + "'");
    }
  }

  static void expect_no_focus() {
    afterhours::Entity *focused = get_focused_element();
    if (focused) {
      std::string label = "unknown";
      if (focused->has<afterhours::ui::HasLabel>()) {
        label = focused->get<afterhours::ui::HasLabel>().label;
      }
      throw std::runtime_error("Expected no focus, but element with label '" +
                               label + "' is focused");
    }
  }

  /// Check that the visual focus ring is drawn on the element with the given
  /// label. This differs from expect_focus() which checks the logical focus
  /// (e.g. the tray root). Visual focus follows visual_focus_id which, for
  /// trays, points to the selected child.
  static void expect_visual_focus(const std::string &label) {
    auto *context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (!context) {
      throw std::runtime_error("No UIContext singleton found");
    }
    if (context->visual_focus_id == context->ROOT) {
      throw std::runtime_error(
          "Expected visual focus on '" + label + "', but visual_focus_id is ROOT");
    }
    auto opt_entity =
        afterhours::ui::UICollectionHolder::getEntityForID(context->visual_focus_id);
    if (!opt_entity.has_value()) {
      throw std::runtime_error(
          "Expected visual focus on '" + label +
          "', but visual_focus_id entity not found");
    }
    afterhours::Entity &ve = opt_entity.asE();
    if (!ve.has<afterhours::ui::HasLabel>()) {
      throw std::runtime_error(
          "Visual focus element does not have a label (expected '" + label + "')");
    }
    const std::string &actual = ve.get<afterhours::ui::HasLabel>().label;
    if (actual != label) {
      throw std::runtime_error("Expected visual focus on '" + label +
                               "', but visual focus is on '" + actual + "'");
    }
  }

  /// Check that the element with visual focus has a specific debug name.
  /// Useful for elements without labels (e.g. radio row buttons).
  static void expect_visual_focus_debug_name(const std::string &debug_name) {
    auto *context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (!context) {
      throw std::runtime_error("No UIContext singleton found");
    }
    if (context->visual_focus_id == context->ROOT) {
      throw std::runtime_error(
          "Expected visual focus on debug_name '" + debug_name +
          "', but visual_focus_id is ROOT");
    }
    auto opt_entity =
        afterhours::ui::UICollectionHolder::getEntityForID(context->visual_focus_id);
    if (!opt_entity.has_value()) {
      throw std::runtime_error(
          "Expected visual focus on debug_name '" + debug_name +
          "', but visual_focus_id entity not found");
    }
    afterhours::Entity &ve = opt_entity.asE();
    if (!ve.has<afterhours::ui::UIComponentDebug>()) {
      throw std::runtime_error(
          "Visual focus element does not have UIComponentDebug (expected '" +
          debug_name + "')");
    }
    const std::string &actual = ve.get<afterhours::ui::UIComponentDebug>().name_value;
    if (actual != debug_name) {
      throw std::runtime_error("Expected visual focus on debug_name '" +
                               debug_name + "', but visual focus is on '" +
                               actual + "'");
    }
  }

  // ========== Responsive Layout Validation ==========

  struct OverflowViolation {
    std::string name;  // debug name or label
    float x, y, w, h; // computed rect
  };

  /// Walk all rendered UI elements and return any whose rect OR text content
  /// extends outside the viewport [0,0,viewport_w,viewport_h].
  static std::vector<OverflowViolation>
  check_viewport_overflow(int viewport_w, int viewport_h) {
    std::vector<OverflowViolation> violations;
    // Don't use force_merge: we only want entities that completed the layout
    // pipeline (force_merge includes freshly-created entities at position 0,0)
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    auto all_ui = afterhours::EntityQuery(ui_coll)
                      .whereHasComponent<afterhours::ui::UIComponent>()
                      .gen();

    auto *font_mgr = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::FontManager>();

    float vw = static_cast<float>(viewport_w);
    float vh = static_cast<float>(viewport_h);
    constexpr float TOLERANCE = 2.0f;

    for (afterhours::Entity &entity : all_ui) {
      auto &cmp = entity.get<afterhours::ui::UIComponent>();
      if (!cmp.was_rendered_to_screen || cmp.should_hide)
        continue;

      auto rect = cmp.rect();
      if (rect.width < 1.0f || rect.height < 1.0f)
        continue;
      // Skip elements that haven't been laid out yet (default computed values)
      if (cmp.computed[afterhours::ui::Axis::X] < 0 ||
          cmp.computed[afterhours::ui::Axis::Y] < 0)
        continue;

      // Check 1: element rect outside viewport
      bool rect_out = (rect.x + rect.width > vw + TOLERANCE) ||
                      (rect.y + rect.height > vh + TOLERANCE) ||
                      (rect.x < -TOLERANCE) ||
                      (rect.y < -TOLERANCE);

      // Check 2: text truncation (text wider than its container element)
      // Skip text check for elements at origin (0,0) - likely stale/unprocessed
      bool text_truncated = false;
      if (entity.has<afterhours::ui::HasLabel>() && font_mgr &&
          (rect.x > 0.5f || rect.y > 0.5f)) {
        auto &label = entity.get<afterhours::ui::HasLabel>();
        if (!label.label.empty() && label.label.length() > 1) {
          std::string fname = cmp.font_name;
          if (fname == afterhours::ui::UIComponent::UNSET_FONT ||
              fname == afterhours::ui::UIComponent::DEFAULT_FONT) {
            fname = font_mgr->active_font;
          }
          if (font_mgr->fonts.contains(fname)) {
            afterhours::Font font = font_mgr->fonts.at(fname);
            float font_size = cmp.font_size.value;
            if (font_size < 1.0f) font_size = 14.0f;
            auto text_sz = afterhours::measure_text(
                font, label.label.c_str(), font_size, 1.0f);
            if (text_sz.x > rect.width + 4.0f) text_truncated = true;
          }
        }
      }

      if (!rect_out && !text_truncated)
        continue;

      std::string name;
      if (entity.has<afterhours::ui::UIComponentDebug>()) {
        name = entity.get<afterhours::ui::UIComponentDebug>().name();
      } else if (entity.has<afterhours::ui::HasLabel>()) {
        name = "\"" + entity.get<afterhours::ui::HasLabel>().label + "\"";
      } else {
        name = "entity_" + std::to_string(entity.id);
      }
      violations.push_back(
          {name, rect.x, rect.y, rect.width, rect.height});
    }
    return violations;
  }

  /// Assert no UI elements overflow the viewport at the given resolution.
  /// Throws with a detailed message listing all violating elements.
  static void assert_no_viewport_overflow(int viewport_w, int viewport_h) {
    auto violations = check_viewport_overflow(viewport_w, viewport_h);
    if (!violations.empty()) {
      std::string msg = "Responsive layout check FAILED at " +
                        std::to_string(viewport_w) + "x" +
                        std::to_string(viewport_h) + ": " +
                        std::to_string(violations.size()) +
                        " element(s) overflow the viewport:\n";
      for (auto &v : violations) {
        msg += "  - " + v.name + " at (" + std::to_string((int)v.x) + "," +
               std::to_string((int)v.y) + ") size " +
               std::to_string((int)v.w) + "x" + std::to_string((int)v.h) +
               "\n";
      }
      throw std::runtime_error(msg);
    }
  }

  static test_snapshot::SnapshotResult
  capture_snapshot(const std::string &name) {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();
    return test_snapshot::capture_snapshot(name, screen_width, screen_height);
  }

  static test_snapshot::SnapshotResult
  compare_snapshot(const std::string &name, float tolerance = 0.01f) {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();
    return test_snapshot::compare_snapshot(name, screen_width, screen_height,
                                           tolerance);
  }
};
