# CSS Grid Layout

**Status:** Not implemented  
**Priority:** Low

## Problem

Only Row/Column flex layout is available.

## Suggested Implementation

```cpp
ElementResult grid(HasUIContext auto &ctx, EntityParent ep_pair,
                   int columns, int rows = -1,  // -1 = auto rows
                   ComponentConfig config = ComponentConfig());

ComponentConfig{}
    .with_grid_gap(DefaultSpacing::small())
    .with_grid_columns(3)
    .with_grid_auto_rows(pixels(100));
```

