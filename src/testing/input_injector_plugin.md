# WM local testing plugin: scheduled synthetic input injector

Local plugin (keep-as-local), not vendored afterhours. Implementation:
`src/engine/input_injector.h` / `src/engine/input_injector.cpp`.
Consumers: `src/testing/test_input.cpp` (delegates mouse/button queries),
`src/game.cpp` (MCP cursor + key hooks, per-frame hold/reset).

## Unique surface (why it is not retired)

- `schedule_mouse_click_at(const raylib::Rectangle &)` — scheduled clicks are
  rect-centred. Vendored `testing::input_injector::schedule_click_at` takes
  `(x, y, w, h)` floats instead.
- Mouse API is int-button + `set_mouse_position(int, int)` / `vec2
  get_mouse_position()` with raylib `*_Real` fallback when no synthetic
  mouse is active. Vendored uses `Position` and no-arg button functions
  (`is_mouse_button_pressed()` etc.).
- `consume_synthetic_press(key)` decrements a per-key press count after a
  1-frame delay; vendored `consume_press` leaves the count readable until
  `reset_frame()`. `src/testing/test_input.cpp::is_key_pressed` is written
  against the decrementing semantics.

The vendored injector (`vendor/afterhours/src/plugins/e2e_testing/
input_injector.h` + `test_input.h`) covers gamepads, wheel, pinch, and
`simulate_click(x, y)` — it is not a drop-in for the rect/int-button/consume
surface above, and `src/headless_screenshots.cpp` already calls the vendored
injector directly for the held shift key. Retiring this shim would require a
caller migration in `test_input.cpp` + `game.cpp` plus behavior re-validation.
Not done in this lane (batch P-B).

## Manifest

See `input_injector_manifest.json` in this directory for the machine-readable
inventory (files, public API, upstream comparison, disposition).
