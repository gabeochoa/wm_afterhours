# Tooltip

**Status:** Not implemented  
**Priority:** Medium

## Problem

No hover-based contextual help.

## Suggested Implementation

```cpp
void set_tooltip(HasUIContext auto &ctx, const std::string &text);

// Or as a modifier
ComponentConfig{}.with_tooltip("Help text here");
```

## Features Needed
- Delay before showing (e.g., 0.5s hover)
- Position near cursor or element
- Auto-hide on mouse move away
- Rich content support (not just text)

