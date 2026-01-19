# Tree Node / Collapsible Header

**Status:** Not implemented  
**Priority:** Medium

## Problem

No expandable/collapsible sections. Needed for hierarchical data and settings categories.

## Suggested Implementation

```cpp
ElementResult tree_node(HasUIContext auto &ctx, EntityParent ep_pair,
                        const std::string &label,
                        bool &expanded,
                        ComponentConfig config = ComponentConfig());
```

## Features Needed
- Expand/collapse toggle (click or keyboard)
- Expand/collapse icon (arrow or +/-)
- Indentation for nested nodes
- Optional selection state

