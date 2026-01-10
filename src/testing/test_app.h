#pragma once

#include "../components.h"
#include "../input_mapping.h"
#include "../rl.h"
#include "../settings.h"
#include "test_feedback.h"
#include "test_input.h"
#include "test_snapshot.h"
#include <afterhours/ah.h>
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
    int wait_until_frame = 0;  // If > 0, don't continue until frame_counter >= this

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

  static afterhours::Entity *
  find_ui_element_by_label(const std::string &label) {
    afterhours::OptEntity opt = afterhours::EntityQuery()
                                    .whereHasComponent<afterhours::ui::HasLabel>()
                                    .whereLambda([&label](const afterhours::Entity &e) {
                                      return e.get<afterhours::ui::HasLabel>().label == label;
                                    })
                                    .gen_first();
    return opt.has_value() ? &opt.asE() : nullptr;
  }

  static std::optional<afterhours::EntityHandle>
  find_ui_element_handle_by_label(const std::string &label) {
    return afterhours::EntityQuery()
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

  // Schedule a mouse release for the next frame (call after click_ui_element and waiting)
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
      // Set the wait_until_frame in the promise so TestSystem knows not to continue
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
    auto opt_entity =
        afterhours::EntityHelper::getEntityForID(context->focus_id);
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
