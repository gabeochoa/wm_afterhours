#pragma once

// Local WM testing plugin: scheduled synthetic input injector.
// Kept local (not retired to vendored afterhours e2e_testing:
// testing::input_injector / test_input) because parity is not proven:
//   * Rectangle-centred scheduled clicks (schedule_mouse_click_at takes a
//     raylib::Rectangle; vendored schedule_click_at takes x/y/w/h) --
//     game.cpp MCP cursor wraps the rect spelling.
//   * int-button mouse queries and int/vec2 positions with raylib
//     *_Real fallback (vendored uses Position + no-arg button functions).
//   * consume_synthetic_press decrements a per-key count with a 1-frame
//     delay (vendored consume_press reads until reset_frame; vendored
//     test_input routing differs again) -- see src/testing/test_input.cpp.
//   * test_input.cpp delegates mouse/button queries here; headless_screenshots
//     already uses the vendored injector for the shift key, so the two
//     coexist by call site, not by duplication.
// See src/testing/input_injector_plugin.md for the plugin README/manifest.
// API and behavior are frozen by this comment: no renames, no migration here.
#include "../rl.h"

namespace input_injector {

void schedule_mouse_click_at(const raylib::Rectangle &rect);
void inject_scheduled_click();
void release_scheduled_click();
void inject_key_press(int keycode);
void hold_key_for_duration(int keycode, float duration);
void set_key_down(int keycode);
void set_key_up(int keycode);
bool consume_synthetic_press(int keycode);
void update_key_hold(float dt);
bool is_key_synthetically_down(int keycode);

void set_mouse_position(int x, int y);
vec2 get_mouse_position();
bool is_mouse_button_down(int button);
bool is_mouse_button_pressed(int button);
bool is_mouse_button_released(int button);

void reset_frame();

} // namespace input_injector
