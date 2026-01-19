# Text Input

**Status:** Not implemented  
**Priority:** Medium

## Problem

No way to accept text input from users. Fundamental widget for forms, search boxes, chat, and settings.

## Suggested Implementation

```cpp
ElementResult text_input(HasUIContext auto &ctx, EntityParent ep_pair,
                         std::string &value,
                         ComponentConfig config = ComponentConfig());
```

## Features Needed
- Single-line text entry
- Cursor position tracking
- Text selection (click-drag, Shift+Arrow)
- Copy/paste support (Ctrl+C/V)
- Keyboard navigation (Home, End, Arrow keys)
- Placeholder text support

## Related
- See `03_text_editing_widget.md` for composable text editing primitives
- Text Area / Text Editor extends this with multiline support

