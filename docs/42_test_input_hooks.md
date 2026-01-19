# Test Input Hooks

**Status:** Not implemented  
**Priority:** Medium

## Problem

No first-class way to inject input events for automated tests.

## Working Implementation (wordproc)
- `src/testing/test_input.h/.cpp` (high-level input queue)
- `src/testing/input_injector.h/.cpp` (low-level raylib injection)
- `src/testing/test_input_provider.h` (UIContext integration)
- `src/external.h` (macro wrappers)

## Suggested Implementation

```cpp
namespace afterhours::ui::test {
  void push_key(int keycode);
  void push_char(char32_t ch);
  void set_mouse_position(float x, float y);
  void press_mouse_button(int button);  // one-frame press
  void hold_mouse_button(int button);
  void release_mouse_button(int button);
  void scroll_wheel(float delta);
  void advance_frame();
  void clear_all();
}

// Optional: provider interface for per-context input
struct InputProvider {
  virtual ~InputProvider() = default;
  virtual Vector2 get_mouse_position() = 0;
  virtual bool is_mouse_button_pressed(int button) = 0;
  virtual bool is_mouse_button_down(int button) = 0;
  virtual bool is_key_pressed(int key) = 0;
  virtual int get_char_pressed() = 0;
};
```

## Notes
- Pressed input must be visible for exactly one frame (matches hardware behavior).
- Avoid macro overrides by letting `UIContext` consume a configurable provider.

## Related
- See `43_e2e_testing.md` for the E2E testing framework that uses this

