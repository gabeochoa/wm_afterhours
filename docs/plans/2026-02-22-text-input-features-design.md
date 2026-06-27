# Text Input Features Design

Based on HTML spec audit, wordproc reference implementation, and 17 failing e2e tests.

## Architecture Decision: Shift-as-Modifier

**Chosen approach:** Check Shift state as a raw modifier (like wordproc does) instead of creating separate SelectLeft/SelectRight/SelectHome/SelectEnd InputActions.

**Why:** wordproc's `navigateWithSelection` pattern halves the action count. One `MoveLeft` action handles both plain movement and selection depending on whether Shift is held. This also automatically gives us Shift+Home, Shift+End, Shift+Ctrl+Arrow etc. for free.

**Impact on existing code:** `TextSelectLeft` and `TextSelectRight` actions become redundant. We keep them for backward compat but the new shift-check path supersedes them.

## Implementation Order (easiest to hardest)

### Phase 1: Key Repeat

**Files:** `input_system.h`, `context.h`, `component.h`

Add `is_key_pressed_repeat()` to the input wrapper (backends already expose it). Add `inputs_pressed_repeat` vector to `InputCollector`. Add `ctx.pressed_or_repeat()` to `UIContext`. Change text_input `component.h` to use `pressed_or_repeat()` for: arrows, backspace, delete, home, end, and all new movement actions.

Reference: wordproc's `isKeyPressedRepeatHelper` in `action_map.cpp:28-35`.

### Phase 2: Shift-for-Selection + Word Ops + Shift+Home/End

**Files:** `input_mapping.h`, `component.h`

New InputActions:
- `TextWordLeft` — Ctrl+Left (Win/Linux), Alt+Left (Mac)
- `TextWordRight` — Ctrl+Right (Win/Linux), Alt+Right (Mac)
- `TextDeleteWordBack` — Ctrl+Backspace (Win/Linux), Alt+Backspace (Mac)
- `TextDeleteWordForward` — Ctrl+Delete (Win/Linux), Alt+Delete (Mac)

In `component.h`, add a shift-detection block before movement handling:

```cpp
bool shift_held = input::is_key_down(keys::LEFT_SHIFT) ||
                  input::is_key_down(keys::RIGHT_SHIFT);
auto nav = [&](auto move_fn) {
    if (shift_held && !state.selection_anchor)
        state.selection_anchor = state.cursor_position;
    if (!shift_held) state.clear_selection();
    move_fn();
    reset_blink(state);
};
```

Then all movement becomes: `nav([&]{ move_cursor_left(state); })` etc.

Shift+Home/End come free — the existing `TextHome`/`TextEnd` handlers just use `nav()` and shift automatically extends selection.

Utility functions `find_word_start`, `find_word_end`, `select_word_at` already exist in `utils.h`.

Reference: wordproc's `navigateWithSelection` in `input_system.h:358-371`.

### Phase 3: Escape to Blur

**Files:** `component.h`

~3 lines inside the `is_focused` block:

```cpp
if (ctx.pressed(InputAction::MenuBack)) {
    state.clear_selection();
    ctx.clear_focus();
}
```

### Phase 4: Click-to-Position Cursor

**Files:** `component.h`

Replace the click handler lambda. On click: get mouse position relative to field rect, iterate through text measuring character widths to find the nearest byte offset, set `cursor_position`. The font and `measure_text` are already available in the rendering block.

Key function needed: `pixel_to_char_offset(font, text, font_size, local_x)` — binary search or linear scan measuring substrings.

### Phase 5: Click-Drag Select + Shift+Click

**Files:** `component.h`, possibly `state.h`

- **Shift+Click:** On click, if Shift held, set `selection_anchor` (if not already set) and move cursor to click position.
- **Drag:** On mouse-down, set anchor. On mouse-move while held, update cursor position. Needs tracking drag state (bool `is_dragging` in state or local to the handler).

### Phase 6: Double-Click Word + Triple-Click Select All

**Files:** `component.h`, `state.h`

Add to state: `float last_click_time`, `size_t last_click_pos`, `int click_count`.

On click: if within 300ms and near same position, increment click_count. On double: call `select_word_at()`. On triple: select all.

### Phase 7: Horizontal Scrolling

**Files:** `component.h`, `state.h`

Add `float scroll_offset_x` to state. After cursor position changes, ensure cursor is visible:

```
if (cursor_x - scroll_offset_x > field_width) scroll_offset_x = cursor_x - field_width + margin;
if (cursor_x - scroll_offset_x < 0) scroll_offset_x = cursor_x - margin;
```

Apply scroll_offset_x when rendering text, cursor, and selection. Add clip rect to field.

### Phase 8: Undo/Redo

**Files:** `state.h`, `component.h`, `input_mapping.h`

New InputActions: `TextUndo` (Ctrl+Z / Cmd+Z), `TextRedo` (Ctrl+Shift+Z / Ctrl+Y / Cmd+Shift+Z).

Simple snapshot approach: store `{text, cursor_position}` snapshots on each mutation. Cap at ~50 entries. Merge consecutive single-char inserts into one entry (like wordproc's `can_merge_with`).

Reference: wordproc's `CommandHistory` in `extracted/command_history.h`.

### Phase 9: Readonly + Disabled

**Files:** `state.h`, `component.h`

Add `bool readonly = false` and `bool disabled = false` to state.

- **Readonly:** Guard all mutation paths (insert_char, delete, paste, cut). Allow focus, selection, copy.
- **Disabled:** Skip focus entirely, render with reduced opacity. Guard at top of `text_input()` function.

## Future Refactors

### Replace `#ifdef AFTER_HOURS_ENABLE_E2E_TESTING` with concept/interface swap

`input_system.h` has 18 `#ifdef AFTER_HOURS_ENABLE_E2E_TESTING` blocks — nearly every input function has an e2e branch that routes to `test_input::*` and an `#else` that calls the real backend. This is brittle (easy to forget the ifdef when adding new functions) and clutters the code.

**Proposed approach:** Define a concept (or abstract interface) for the input backend, then swap the implementation at compile time via a single template parameter or type alias instead of per-function ifdefs.

```cpp
// Concept for input backend
template <typename T>
concept InputBackend = requires(T t, int key, int btn) {
  { T::is_key_pressed(key) } -> std::same_as<bool>;
  { T::is_key_down(key) } -> std::same_as<bool>;
  { T::is_key_pressed_repeat(key) } -> std::same_as<bool>;
  { T::get_char_pressed() } -> std::same_as<int>;
  { T::is_mouse_button_pressed(btn) } -> std::same_as<bool>;
  { T::is_mouse_button_down(btn) } -> std::same_as<bool>;
  { T::get_mouse_position() };  // returns MousePosition
  { T::get_mouse_wheel_move() } -> std::same_as<float>;
};

// Real backend
struct RaylibInputBackend { /* wraps raylib calls */ };

// Test backend
struct E2EInputBackend { /* wraps test_input calls */ };

// Single swap point
#ifdef AFTER_HOURS_ENABLE_E2E_TESTING
using ActiveInputBackend = E2EInputBackend;
#else
using ActiveInputBackend = RaylibInputBackend;
#endif

// Then input struct just delegates:
static bool is_key_pressed(KeyCode k) { return ActiveInputBackend::is_key_pressed(k); }
```

This reduces 18 ifdefs to 1, and makes adding new input functions impossible to forget the test path for.

**Files affected:** `input_system.h`, `e2e_testing/test_input.h`
**Risk:** Low — purely mechanical refactor, no behavior change.

### Scale hardcoded pixel constants relative to field height

Several pixel values in `component.h` are hardcoded and won't look correct at different resolutions or field sizes. The proportional padding logic (lines 180-188) already derives values from `field_h` — these remaining constants should follow the same pattern.

**Values that need scaling:**

| Constant | Line | Current | Should be |
|----------|------|---------|-----------|
| `TEXT_MARGIN` | 243 | `5.f` px | `field_h * 0.07f` (~5px at 36px field) |
| `SCROLL_MARGIN` | 285 | `4.f` px | `field_h * 0.06f` (~4px at 36px field) — or derive from `pad_w` |
| `CURSOR_WIDTH` | 349 | `2.0f` px | `field_h * 0.055f` (~2px at 36px, ~3px at 54px) |
| `sel_width` min | 333 | `2.f` px | Same as cursor width |
| Focus border | 213 | `pixels(2.0f)` | `field_h * 0.055f` |
| `pad_w - 5.f` | 197 | Hardcoded `5.f` | The `5.f` is `draw_text_in_rect`'s internal margin. If that function's margin also doesn't scale, this is a deeper issue in `rendering.h`. |

**The `720.f` fallbacks** (lines 225, 388) are safe — they're only used when `ProvidesCurrentResolution` isn't available, which shouldn't happen in practice.

**The `0.5f` disabled opacity** (line 90) is intentionally resolution-independent.

**Approach:** Compute all these once from `field_h` at the top of the focused block (after line 221) and use named locals:

```cpp
float field_h = field_cmp.computed[Axis::Y];
float cursor_w  = std::max(field_h * 0.055f, 1.f);
float text_marg = field_h * 0.07f;
float scroll_marg = field_h * 0.06f;
```

**Dependency:** The `5.f` in `draw_text_in_rect` (`rendering.h`) is its own hardcoded margin. That function either needs a parameter for margin or needs to scale it internally. Until that's fixed, `text_x_offset = pad_w - 5.f` will be slightly off at non-default sizes.

### Replace FAKE focus sentinel with `suppress_auto_focus` flag

The escape-to-blur feature uses `ctx.set_focus(ctx.FAKE)` to prevent `try_to_grab` from immediately re-focusing the first widget. This requires special-case `|| focus_id == FAKE` checks in `EndUIContextManager` and `ComputeVisualFocusId`.

A cleaner approach: add a `bool suppress_auto_focus` flag to UIContext. The escape handler sets it, `try_to_grab` checks it, and clicking/tabbing clears it. No sentinel needed.

## E2E Test Mapping

| Phase | Tests that should pass after |
|-------|------------------------------|
| 1 | Key repeat (no e2e — manual feel) |
| 2 | 70, 71, 72, 76, 86 |
| 3 | 88 |
| 4 | 67, 85 |
| 5 | 68, 82 |
| 6 | 69, 90 |
| 7 | 66 |
| 8 | 79 |
| 9 | 80, 81 |
