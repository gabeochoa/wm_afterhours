# Color Picker

**Status:** Not implemented  
**Priority:** Low

## Problem

No way for users to select colors.

## Suggested Implementation

```cpp
ElementResult color_picker(HasUIContext auto &ctx, EntityParent ep_pair,
                           Color &value,
                           ComponentConfig config = ComponentConfig());
```

## Features Needed
- HSV color wheel or square
- RGB/HSV sliders
- Alpha channel support
- Hex input
- Color preview (old vs new)

