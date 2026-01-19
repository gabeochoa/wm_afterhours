# Entity ID Management with mk()

**Status:** Not implemented  
**Priority:** High  
**Source:** break_ross migration experience

---

## Problem

The `mk(parent, index)` pattern requires manual index management, which is error-prone and makes refactoring difficult.

### Current Issues

1. **Manual Index Management**: Must provide unique indices to `mk()` to avoid entity ID conflicts
2. **Refactoring Hazard**: Adding/removing elements requires updating many index values
3. **Silent Conflicts**: Duplicate indices may cause subtle bugs that are hard to diagnose
4. **No Compile-Time Checking**: Index conflicts only detected at runtime (if at all)

### Example of the Problem

```cpp
// Adding a new element requires updating all subsequent indices
imm::text(ctx, mk(parent, 0), ...);  // ID 0
imm::text(ctx, mk(parent, 1), ...);  // ID 1
// Want to add element here - must renumber everything below!
imm::text(ctx, mk(parent, 2), ...);  // Was 1, now 2
imm::text(ctx, mk(parent, 3), ...);  // Was 2, now 3
// ... and so on
```

---

## Suggested Solutions

### Option 1: Auto-Incrementing IDs

```cpp
// Automatic sequential ID generation per parent
auto mk_next(EntityParent parent) -> EntityParent;

// Usage:
imm::text(ctx, mk_next(parent), ...);  // Auto-assigned ID
imm::text(ctx, mk_next(parent), ...);  // Next sequential ID
imm::button(ctx, mk_next(parent), ...); // And so on

// Implementation sketch:
struct AutoIdState {
    std::unordered_map<Entity, int> next_id;
};

EntityParent mk_next(EntityParent parent) {
    static AutoIdState state;
    int id = state.next_id[parent.entity]++;
    return mk(parent, id);
}

// Reset at frame start:
void reset_auto_ids() {
    AutoIdState::instance().next_id.clear();
}
```

### Option 2: String-Based IDs

```cpp
// More stable, readable IDs using strings
auto mk_id(EntityParent parent, const char* id) -> EntityParent;

// Usage:
imm::text(ctx, mk_id(parent, "title"), ...);
imm::text(ctx, mk_id(parent, "subtitle"), ...);
imm::button(ctx, mk_id(parent, "confirm_btn"), ...);

// Benefits:
// - IDs are stable across refactoring
// - Self-documenting code
// - Easy to find specific elements in logs/debugging

// Implementation uses string hashing:
EntityParent mk_id(EntityParent parent, const char* id) {
    return mk(parent, static_cast<int>(std::hash<std::string_view>{}(id)));
}
```

### Option 3: Scoped ID Blocks

```cpp
// Scoped ID generation within a block
{
    auto scope = IdScope(parent);
    imm::text(ctx, scope.next(), ...);   // 0
    imm::text(ctx, scope.next(), ...);   // 1
    imm::button(ctx, scope.next(), ...); // 2
}

// Nested scopes
{
    auto outer = IdScope(parent);
    imm::div(ctx, outer.next(), [&](auto& container) {
        auto inner = IdScope(container);
        imm::text(ctx, inner.next(), ...);  // Separate ID space
    });
}
```

---

## Better Error Messages

### Current Behavior

When index conflicts occur, errors are cryptic or non-existent.

### Suggested Improvements

```cpp
// In debug builds, track ID usage and warn on conflicts:
EntityParent mk(EntityParent parent, int index) {
#ifdef DEBUG
    static std::unordered_set<std::pair<Entity, int>> used_ids;
    auto key = {parent.entity, index};
    if (used_ids.contains(key)) {
        log_error("Entity ID conflict: parent={}, index={}\n"
                  "  Previous usage: {}\n"
                  "  Current usage: {}\n"
                  "  Suggestion: Use mk_next() for auto-incrementing IDs,\n"
                  "              or mk_id() for string-based IDs.",
                  parent.entity, index,
                  /* source location of first use */,
                  /* source location of current use */);
    }
    used_ids.insert(key);
#endif
    return {/* actual implementation */};
}
```

---

## Compile-Time Checking (Advanced)

For constexpr contexts, detect duplicate indices at compile time:

```cpp
// Using C++20 concepts and consteval
template<int... Indices>
consteval bool unique_indices() {
    std::array<int, sizeof...(Indices)> arr{Indices...};
    std::sort(arr.begin(), arr.end());
    return std::adjacent_find(arr.begin(), arr.end()) == arr.end();
}

// Usage with static_assert:
static_assert(unique_indices<0, 1, 2, 3>(), "Duplicate indices!");
```

---

## Migration Path

1. **Phase 1**: Add `mk_next()` as opt-in alternative
2. **Phase 2**: Add debug warnings for manual `mk()` with conflict detection
3. **Phase 3**: Encourage `mk_next()` or `mk_id()` in documentation
4. **Phase 4**: Consider deprecating manual index management

