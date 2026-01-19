# List Box and Table

**Status:** Not implemented  
**Priority:** Low

---

## List Box

### Problem

No scrollable list with selection. Dropdown works for small option sets but not for large lists.

### Suggested Implementation

```cpp
template <typename Container>
ElementResult list_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const Container &items,
                       size_t &selected_index,
                       ComponentConfig config = ComponentConfig());
```

---

## Table / Data Grid

### Problem

No structured table layout with columns.

### Suggested Implementation

```cpp
ElementResult table(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::vector<std::string> &headers,
                    int num_columns,
                    ComponentConfig config = ComponentConfig());
```

### Features Needed
- Column headers
- Resizable columns
- Sortable columns (click header)
- Row selection
- Scrollable body with fixed header

