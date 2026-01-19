# Input Number (Int/Float)

**Status:** Not implemented  
**Priority:** Medium

## Problem

No specialized numeric input with validation, step buttons, or drag-to-adjust.

## Suggested Implementation

```cpp
ElementResult input_int(HasUIContext auto &ctx, EntityParent ep_pair,
                        int &value, int min, int max, int step = 1,
                        ComponentConfig config = ComponentConfig());

ElementResult input_float(HasUIContext auto &ctx, EntityParent ep_pair,
                          float &value, float min, float max, float step = 0.1f,
                          ComponentConfig config = ComponentConfig());
```

## Features Needed
- +/- buttons
- Direct text entry with validation
- Min/max clamping
- Optional drag-to-adjust

