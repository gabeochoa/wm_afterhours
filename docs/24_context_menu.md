# Context Menu / Popup

**Status:** Not implemented  
**Priority:** Medium

## Problem

No right-click menus or popup menus.

## Suggested Implementation

```cpp
ElementResult context_menu(HasUIContext auto &ctx, EntityParent ep_pair,
                           const std::vector<std::string> &options,
                           ComponentConfig config = ComponentConfig());
```

## Features Needed
- Trigger on right-click
- Position near cursor
- Click-outside-to-close
- Keyboard navigation
- Nested submenus

