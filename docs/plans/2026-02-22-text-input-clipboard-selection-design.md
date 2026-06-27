# Text Input: Selection & Clipboard Support

**Date:** 2026-02-22
**Status:** Planned

## Goal

Add text selection and clipboard (Ctrl+C/V/X) support to the `text_input()` widget in afterhours. Requires adding KeyChord support to the input system so actions can be mapped to key+modifier combinations without manual modifier gating in handlers.

## Scope

### In scope
1. **KeyChord input system** -- key+modifier mapping with chord priority
2. Selection state in `HasTextInputState` (anchor + cursor)
3. Shift+Left/Right to extend selection character-by-character
4. Ctrl+A to select all
5. Ctrl+C to copy selection to clipboard
6. Ctrl+V to paste from clipboard (replaces selection if any)
7. Ctrl+X to cut selection to clipboard
8. Backspace/Delete deletes selection when active
9. Character input replaces selection when active
10. Visual selection highlight rendered behind text
11. Clicking collapses selection

### Out of scope (future)
- Word-level selection (Ctrl+Shift+Arrow)
- Double-click to select word
- Mouse drag selection
- Shift+Home/End selection

---

## Part 1: KeyChord Input System

### Problem

The current input system maps actions to bare keys. When two actions share the same key but differ by modifiers (e.g., LEFT vs Shift+LEFT), both fire and only the last one survives in `last_action`. Handlers must manually check `is_key_down(LEFT_CONTROL)` which is fragile, inconsistent, and prevents users from remapping modifier combos.

### Design

#### KeyChord struct (`input_system.h`)

```cpp
struct KeyChord {
  KeyCode key = 0;
  uint8_t required_modifiers = 0;
  bool has_explicit_modifiers = false;

  static constexpr uint8_t MOD_SHIFT = 1;
  static constexpr uint8_t MOD_CTRL  = 2;
  static constexpr uint8_t MOD_ALT   = 4;
  static constexpr uint8_t MOD_SUPER = 8;

  // Implicit from KeyCode -- backward compatible, permissive matching
  KeyChord(KeyCode k)
      : key(k), required_modifiers(0), has_explicit_modifiers(false) {}

  // Explicit chord -- exclusive matching
  KeyChord(KeyCode k, uint8_t mods)
      : key(k), required_modifiers(mods), has_explicit_modifiers(true) {}
};
```

Two matching modes:
- **Permissive** (`has_explicit_modifiers = false`): Fires when the key is pressed regardless of what modifiers are held. All existing mappings work this way via implicit conversion from `KeyCode`/`int`. **No breaking changes.**
- **Exclusive** (`has_explicit_modifiers = true`): Fires only when the key is pressed AND the required modifiers match. New opt-in behavior.

This is the permanent design, not a migration path. Permissive is the correct default for most actions (menu navigation doesn't care about modifiers). Exclusive is opt-in for key-chord-differentiated actions.

#### Variant change

```cpp
// Before:
using AnyInput = std::variant<KeyCode, GamepadAxisWithDir, GamepadButton>;

// After:
using AnyInput = std::variant<KeyChord, GamepadAxisWithDir, GamepadButton>;
```

Backward compatible because `KeyChord` is implicitly constructible from `KeyCode` (int).

#### Modifier detection helper

```cpp
static uint8_t get_current_modifiers() {
  uint8_t mods = 0;
  if (is_key_down(keys::LEFT_SHIFT) || is_key_down(keys::RIGHT_SHIFT))
    mods |= KeyChord::MOD_SHIFT;
  if (is_key_down(keys::LEFT_CONTROL) || is_key_down(keys::RIGHT_CONTROL))
    mods |= KeyChord::MOD_CTRL;
  if (is_key_down(keys::LEFT_ALT) || is_key_down(keys::RIGHT_ALT))
    mods |= KeyChord::MOD_ALT;
  if (is_key_down(keys::LEFT_SUPER) || is_key_down(keys::RIGHT_SUPER))
    mods |= KeyChord::MOD_SUPER;
  return mods;
}
```

#### Chord matching in `check_single_action_pressed` / `check_single_action_down`

For `KeyChord` inputs (variant index 0):

```cpp
if (input.index() == 0) {
  auto &chord = std::get<0>(input);
  temp_medium = DeviceMedium::Keyboard;
  bool key_match = is_key_pressed(chord.key);  // or is_key_down for _down variant
  if (key_match) {
    if (chord.has_explicit_modifiers) {
      uint8_t current = get_current_modifiers();
      temp = ((current & chord.required_modifiers) == chord.required_modifiers) ? 1.f : 0.f;
    } else {
      temp = 1.f;  // permissive: key matched, ignore modifiers
    }
  }
}
```

#### Chord priority (suppression)

In the input collection system (`InputCollectionSystem::for_each_with`), after building the pressed/down lists, suppress permissive bare-key actions when an exclusive chord for the same key also matched. This prevents both `WidgetLeft` and `TextSelectLeft` from firing when Shift+LEFT is pressed.

Implementation: after collecting all matching actions, iterate and remove any permissive action whose key matches a chord action that also fired.

#### `pressed_exact()` on UIContext

Safety check that validates current modifiers match what the action requires. Also stores `last_action_modifiers` alongside `last_action` so the check is against what was expected, not looked up from mappings.

```cpp
// In UIContext:
uint8_t last_action_modifiers = 0;

[[nodiscard]] bool pressed_exact(const InputAction &name) {
  if (last_action != name) return false;
  if (input::get_current_modifiers() != last_action_modifiers) return false;
  last_action = InputAction::None;
  return true;
}
```

### File Changes (Part 1)

| File | Change |
|------|--------|
| `vendor/afterhours/src/plugins/input_system.h` | Add `KeyChord`, `get_current_modifiers()`, update `AnyInput` variant, update `check_single_action_pressed`/`_down`, add chord suppression in collection loop |
| `vendor/afterhours/src/plugins/ui/context.h` | Add `last_action_modifiers`, add `pressed_exact()` |
| `vendor/afterhours/src/plugins/ui/systems.h` | Store modifiers when setting `last_action` |

---

## Part 2: Selection State

### Done

`selection_anchor` added to `HasTextInputStateT` as `std::optional<size_t>`:

```cpp
std::optional<size_t> selection_anchor;

bool has_selection() const;
size_t selection_start() const;
size_t selection_end() const;
std::string selected_text() const;
void clear_selection();
```

### New utility: `delete_selection()` (`utils.h`)

```cpp
inline bool delete_selection(AnyTextInputState auto &s) {
    if (!s.has_selection()) return false;
    size_t start = s.selection_start();
    size_t len = s.selection_end() - start;
    s.storage.erase(start, len);
    s.cursor_position = start;
    s.clear_selection();
    s.changed_since = true;
    return true;
}
```

---

## Part 3: New InputActions and Mappings

### New enum entries (`input_mapping.h`)

```cpp
TextSelectLeft,   // Shift+LEFT
TextSelectRight,  // Shift+RIGHT
TextCopy,         // Ctrl+C
TextCut,          // Ctrl+X
TextPaste,        // Ctrl+V
```

### Updated mappings

```cpp
// Existing -- updated from bare key to chord:
mapping[to_int(InputAction::TextSelectAll)] = {
    KeyChord{raylib::KEY_A, KeyChord::MOD_CTRL},
};

// New:
mapping[to_int(InputAction::TextSelectLeft)] = {
    KeyChord{raylib::KEY_LEFT, KeyChord::MOD_SHIFT},
};
mapping[to_int(InputAction::TextSelectRight)] = {
    KeyChord{raylib::KEY_RIGHT, KeyChord::MOD_SHIFT},
};
mapping[to_int(InputAction::TextCopy)] = {
    KeyChord{raylib::KEY_C, KeyChord::MOD_CTRL},
};
mapping[to_int(InputAction::TextCut)] = {
    KeyChord{raylib::KEY_X, KeyChord::MOD_CTRL},
};
mapping[to_int(InputAction::TextPaste)] = {
    KeyChord{raylib::KEY_V, KeyChord::MOD_CTRL},
};
```

All existing mappings (e.g., `WidgetLeft = { raylib::KEY_LEFT }`) are unchanged and work identically via implicit `KeyChord` conversion with permissive matching.

---

## Part 4: Input Handling (`component.h`)

Focused input block, in order:

```cpp
// 1. Clipboard (before char input so Ctrl+C/V/X don't insert characters)
if (ctx.pressed(InputAction::TextCopy)) {
    if (state.has_selection())
        clipboard::set_text(state.selected_text());
}
if (ctx.pressed(InputAction::TextCut)) {
    if (state.has_selection()) {
        clipboard::set_text(state.selected_text());
        delete_selection(state);
    }
}
if (ctx.pressed(InputAction::TextPaste)) {
    if (state.has_selection()) delete_selection(state);
    // insert clipboard text at cursor
}
if (ctx.pressed(InputAction::TextSelectAll)) {
    state.selection_anchor = 0;
    state.cursor_position = state.text_size();
}

// 2. Character input (replaces selection)
for (int key = input::get_char_pressed(); key > 0; ...) {
    if (state.has_selection()) delete_selection(state);
    insert_char(state, key);
    state.clear_selection();
}

// 3. Selection movement (Shift+Arrow)
if (ctx.pressed(InputAction::TextSelectLeft)) {
    if (!state.selection_anchor) state.selection_anchor = state.cursor_position;
    move_cursor_left(state);
}
if (ctx.pressed(InputAction::TextSelectRight)) {
    if (!state.selection_anchor) state.selection_anchor = state.cursor_position;
    move_cursor_right(state);
}

// 4. Plain movement (collapses selection) -- use pressed_exact for safety
if (ctx.pressed_exact(InputAction::WidgetLeft)) {
    if (state.has_selection()) {
        state.cursor_position = state.selection_start();
        state.clear_selection();
    } else {
        move_cursor_left(state);
    }
}
if (ctx.pressed_exact(InputAction::WidgetRight)) {
    // same pattern, collapse to selection_end()
}

// 5. Deletion (selection-aware)
if (ctx.pressed(InputAction::TextBackspace)) {
    if (state.has_selection()) delete_selection(state);
    else delete_before_cursor(state);
    state.clear_selection();
}
if (ctx.pressed(InputAction::TextDelete)) {
    if (state.has_selection()) delete_selection(state);
    else delete_at_cursor(state);
    state.clear_selection();
}

// 6. Home/End collapse selection, Enter/Submit unchanged
```

No manual `is_key_down(LEFT_CONTROL)` checks anywhere. The existing `TextSelectAll` handler loses its modifier gate entirely.

---

## Part 5: Selection Rendering (`component.h`)

When `has_selection()` is true and field is focused, render a colored rectangle behind the selected text. Uses the same font measurement logic as the cursor to calculate x positions for `selection_start()` and `selection_end()`.

Rendered as a div with `with_translate()` and `with_opacity()`, similar to the cursor overlay, but wider and using a semi-transparent accent color. Render layer between text and cursor.

---

## Part 6: E2E Testing

### New E2E Commands (done)

- `expect_input_text <name> "<expected>"` -- asserts text input content
- `expect_input_selection <name> <start> <end>` -- asserts selection range

Implemented in `ui_commands.h`, registered in `runner.h`.

### E2E Test Scripts (done)

- `55_text_selection.e2e` -- Shift+Arrow creates selection, typing replaces it
- `56_text_select_all.e2e` -- Ctrl+A selects all, typing replaces
- `57_text_copy_paste.e2e` -- Ctrl+C copies, Ctrl+V pastes to another field
- `58_text_cut.e2e` -- Ctrl+X removes and pastes elsewhere
- `59_text_selection_delete.e2e` -- Backspace deletes selection, arrow collapses without deleting

---

## Implementation Order

1. **KeyChord in input_system.h** -- struct, variant, matching, suppression, `get_current_modifiers()`
2. **UIContext changes** -- `last_action_modifiers`, `pressed_exact()`
3. **systems.h** -- store modifiers when setting `last_action`
4. **input_mapping.h** -- new enum entries + chord mappings
5. **utils.h** -- `delete_selection()`
6. **component.h** -- selection-aware input handling + selection rendering
7. **Run e2e tests 55-59** to validate

## Full File Change Summary

| File | Change |
|------|--------|
| `vendor/afterhours/src/plugins/input_system.h` | `KeyChord`, `get_current_modifiers()`, variant, matching, suppression |
| `vendor/afterhours/src/plugins/ui/context.h` | `last_action_modifiers`, `pressed_exact()` |
| `vendor/afterhours/src/plugins/ui/systems.h` | Store modifiers alongside `last_action` |
| `vendor/afterhours/src/plugins/ui/text_input/state.h` | `selection_anchor`, helpers (done) |
| `vendor/afterhours/src/plugins/ui/text_input/utils.h` | `delete_selection()` |
| `vendor/afterhours/src/plugins/ui/text_input/component.h` | Selection handling + rendering |
| `src/input_mapping.h` | New actions + chord mappings |
| `vendor/afterhours/src/plugins/e2e_testing/ui_commands.h` | `expect_input_text`, `expect_input_selection` (done) |
| `vendor/afterhours/src/plugins/e2e_testing/runner.h` | Command parsing (done) |
