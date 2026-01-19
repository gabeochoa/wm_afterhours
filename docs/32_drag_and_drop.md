# Drag and Drop

**Status:** Not implemented  
**Priority:** Low

## Problem

No way to drag items between containers.

## Suggested Implementation

```cpp
ComponentConfig{}.with_draggable(true);
ComponentConfig{}.with_drop_target(true);
// Plus callbacks for drag start, over, drop
```

