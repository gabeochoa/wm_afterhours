#include "test_input.h"
#include "../rl.h"

// Undefine macros to avoid recursion and allow implementation
#undef IsMouseButtonPressed
#undef IsMouseButtonDown
#undef IsMouseButtonReleased
#undef IsMouseButtonUp
#undef GetCharPressed
#undef IsKeyPressed
#undef GetMousePosition

namespace test_input {
std::queue<KeyPress> input_queue;
bool test_mode = false;
bool slow_test_mode = false;
MouseState mouse_state;

void push_key(int key) {
  KeyPress press;
  press.key = key;
  press.is_char = false;
  input_queue.push(press);
}

void push_char(char c) {
  KeyPress press;
  press.key = 0;
  press.is_char = true;
  press.char_value = c;
  input_queue.push(press);
}

void clear_queue() {
  while (!input_queue.empty()) {
    input_queue.pop();
  }
}

bool key_consumed_this_frame = false;
bool char_consumed_this_frame = false;

bool is_key_pressed(int key) {
  if (!test_mode || input_queue.empty() || key_consumed_this_frame) {
    return raylib::IsKeyPressed_Real(key);
  }

  if (!input_queue.front().is_char && input_queue.front().key == key) {
    input_queue.pop();
    key_consumed_this_frame = true;
    return true;
  }

  return raylib::IsKeyPressed_Real(key);
}

int get_char_pressed() {
  if (!test_mode || input_queue.empty() || char_consumed_this_frame) {
    return raylib::GetCharPressed_Real();
  }

  if (input_queue.front().is_char) {
    char c = input_queue.front().char_value;
    input_queue.pop();
    char_consumed_this_frame = true;
    return static_cast<int>(c);
  }

  return raylib::GetCharPressed_Real();
}

void reset_frame() {
  key_consumed_this_frame = false;
  char_consumed_this_frame = false;
  mouse_state.left_button_pressed_this_frame = false;
  mouse_state.left_button_released_this_frame = false;
}

void set_mouse_position(vec2 pos) {
  mouse_state.position = pos;
  mouse_state.simulation_active = true;
  // Also update real mouse position for things that bypass our hooks
  raylib::SetMousePosition(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void simulate_mouse_button_press(int button) {
  if (button == raylib::MOUSE_BUTTON_LEFT) {
    mouse_state.left_button_held = true;
    mouse_state.left_button_pressed_this_frame = true;
    mouse_state.simulation_active = true;
  }
}

void simulate_mouse_button_release(int button) {
  if (button == raylib::MOUSE_BUTTON_LEFT) {
    mouse_state.left_button_held = false;
    mouse_state.left_button_released_this_frame = true;
    mouse_state.simulation_active = true;
  }
}

void clear_mouse_simulation() { mouse_state = MouseState{}; }

vec2 get_mouse_position() {
  if (test_mode && mouse_state.simulation_active &&
      mouse_state.position.has_value()) {
    return mouse_state.position.value();
  }
  return raylib::GetMousePosition_Real();
}

bool is_mouse_button_pressed(int button) {
  if (test_mode && mouse_state.simulation_active &&
      button == raylib::MOUSE_BUTTON_LEFT) {
    return mouse_state.left_button_pressed_this_frame;
  }
  return raylib::IsMouseButtonPressed_Real(button);
}

bool is_mouse_button_down(int button) {
  if (test_mode && mouse_state.simulation_active &&
      button == raylib::MOUSE_BUTTON_LEFT) {
    return mouse_state.left_button_held;
  }
  return raylib::IsMouseButtonDown_Real(button);
}

bool is_mouse_button_released(int button) {
  if (test_mode && mouse_state.simulation_active &&
      button == raylib::MOUSE_BUTTON_LEFT) {
    return mouse_state.left_button_released_this_frame;
  }
  return raylib::IsMouseButtonReleased_Real(button);
}

bool is_mouse_button_up(int button) {
  if (test_mode && mouse_state.simulation_active &&
      button == raylib::MOUSE_BUTTON_LEFT) {
    return !mouse_state.left_button_held;
  }
  return raylib::IsMouseButtonUp_Real(button);
}

void simulate_tab() { push_key(raylib::KEY_TAB); }

void simulate_shift_tab() {
  push_key(raylib::KEY_LEFT_SHIFT);
  push_key(raylib::KEY_TAB);
}

void simulate_arrow_key(int arrow_key) { push_key(arrow_key); }

void simulate_enter() { push_key(raylib::KEY_ENTER); }

void simulate_escape() { push_key(raylib::KEY_ESCAPE); }
} // namespace test_input
