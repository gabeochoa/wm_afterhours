#pragma once

namespace test_input {
  bool is_mouse_button_pressed(int button);
  bool is_mouse_button_down(int button);
  bool is_mouse_button_released(int button);
  bool is_mouse_button_up(int button);
  int get_char_pressed();
  bool is_key_pressed(int key);
}
