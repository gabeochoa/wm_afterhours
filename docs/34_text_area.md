# Text Area / Text Editor (Multiline)

**Status:** Not implemented  
**Priority:** Low (Very Large effort)

## Problem

No way to accept multi-line text input or full text editing (selection, undo/redo, layout).

## Working Implementation (wordproc)
- `src/editor/text_buffer.h/.cpp` (gap buffer + selection + undo)
- `src/editor/text_layout.h/.cpp` (layout + rendering)

## Additional Features Beyond Text Input
- Line wrapping
- Vertical scrolling
- Line numbers (optional)
- Tab handling
- Caret movement + selection
- Undo/redo
- Clipboard integration

## Related
- See `03_text_editing_widget.md` for composable primitives
- See `10_command_history.md` for undo/redo system

