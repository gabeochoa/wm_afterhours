# Text Editing — Remaining Work

**Status:** Almost complete  
**Priority:** Low  
**Depends on:** `06_action_binding_system.md`

## Implemented

All composable text editing primitives are now in afterhours:

- `TextSelection` → `ui/text_input/selection.h`
- `LineIndex` → `ui/text_input/line_index.h`
- `TextLayoutCache` → `ui/text_input/text_layout.h`
- `text_input()` → `ui/text_input/component.h`
- `text_area()` → `ui/text_input/text_area.h`
- `CommandHistory<T>` → `command_history.h`
- `clipboard::*` → `clipboard.h`
- Word/paragraph navigation → `ui/text_input/utils.h`

## Remaining: Clipboard Keyboard Shortcuts

The `text_input` widget doesn't wire up Ctrl+C/V/X to the clipboard. This requires the action binding system (06) for modifier key support.

### Proposed API

```cpp
// Opt-in clipboard shortcuts
text_input(ctx, mk(parent), text,
    ComponentConfig{}
        .with_size(...)
        .enable_keyboard_shortcuts<InputAction>()
);

// Requires these actions in the InputAction enum:
enum struct InputAction {
  // ... existing ...
  WidgetTextCopy,       // Ctrl+C / Cmd+C
  WidgetTextCut,        // Ctrl+X / Cmd+X
  WidgetTextPaste,      // Ctrl+V / Cmd+V
  WidgetTextSelectAll,  // Ctrl+A / Cmd+A
};
```

### Implementation Order

1. Implement `ActionMap` with modifier support (06)
2. Add `enable_keyboard_shortcuts()` config to `text_input`
3. Wire up clipboard actions when that config is set

---

## Remaining: Search Input

A specialized text input optimized for search / filter functionality. Built on top of `text_input` with additional affordances.

### Use Cases in Game UI
- Inventory search
- Player search
- Server browser filter
- Console command search

### Suggested Implementation

```cpp
struct SearchConfig {
    std::string placeholder = "Search...";
    bool show_clear_button = true;
    bool auto_focus = false;
    int debounce_ms = 300;
};

ElementResult search_input(HasUIContext auto &ctx, EntityParent ep_pair,
                           std::string &query,
                           std::function<void(const std::string&)> on_search,
                           SearchConfig config = SearchConfig());
```

### Features Needed
- Search icon (prefix)
- Clear button (X suffix) when query is non-empty
- Debounced callback (waits for typing to stop before calling `on_search`)
- Loading indicator while searching
- Composes `text_input` internally — benefits from clipboard shortcuts (above) once implemented
