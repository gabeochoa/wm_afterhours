# Debug Tooling

**Status:** Partially implemented  
**Priority:** Medium  
**Source:** break_ross migration experience

---

## Current State

- `ToggleUILayoutDebug` exists but has limited functionality
- Basic position/size logging available but incomplete
- No visual hierarchy viewer
- No performance profiling

---

## Error Messages

### Problem

When something goes wrong (e.g., entity ID conflict, invisible element), error messages could be more helpful.

### Current Issues
- Errors often lack context about which element caused the issue
- No suggestions for how to fix common problems
- Stack traces don't point to user code

### Suggested Improvements

```cpp
// Context-rich error messages:
void log_ui_error(const UIComponent& widget, const std::string& issue) {
    log_error("UI Error in '{}' (entity {}):\n"
              "  Issue: {}\n"
              "  Position: ({}, {})\n"
              "  Size: {}x{}\n"
              "  Parent: {}\n"
              "  Created at: {}:{}\n",
              widget.debug_name,
              widget.entity_id,
              issue,
              widget.position.x, widget.position.y,
              widget.computed[Axis::X], widget.computed[Axis::Y],
              widget.parent_name,
              widget.source_file, widget.source_line);
}

// Source location tracking (C++20):
template<typename... Args>
void mk_debug(EntityParent parent, int index,
              std::source_location loc = std::source_location::current()) {
    auto ep = mk(parent, index);
    ep.set_debug_source(loc.file_name(), loc.line());
    return ep;
}
```

---

## Visual Debug Overlay

### Problem

While `ToggleUILayoutDebug` exists, it would be helpful to have more debugging tools.

### Suggested Features

```cpp
namespace ui::debug {
    // Visual overlay showing component bounds with labels
    void show_bounds_overlay(bool enabled);
    
    // Highlight specific elements
    void highlight_element(Entity e, Color color = RED);
    void highlight_by_name(const std::string& name_pattern);
    
    // Show element info on hover
    void enable_hover_inspector(bool enabled);
    
    // Color-code by state
    struct OverlayColors {
        Color focused = YELLOW;
        Color hovered = CYAN;
        Color disabled = GRAY;
        Color error = RED;       // Negative size, etc.
        Color absolute = ORANGE; // Absolute positioned
    };
    void set_overlay_colors(const OverlayColors& colors);
}
```

### Hover Inspector Display

When hovering over an element with inspector enabled:

```
┌─────────────────────────────────┐
│ Element: "settings_panel"       │
│ Entity: 12345                   │
│ ───────────────────────────────│
│ Position: (100, 50)             │
│ Size: 400 x 300                 │
│ Computed: 398 x 296             │
│ ───────────────────────────────│
│ Padding: 4, 4, 4, 4             │
│ Margin: 2, 2, 2, 2              │
│ ───────────────────────────────│
│ Absolute: No                    │
│ Focused: Yes                    │
│ Hovered: Yes                    │
│ ───────────────────────────────│
│ Parent: "main_container"        │
│ Children: 5                     │
│ ───────────────────────────────│
│ Source: settings.cpp:142        │
└─────────────────────────────────┘
```

---

## Component Hierarchy Viewer

### Problem

No way to visualize the full UI tree structure at runtime.

### Suggested Implementation

```cpp
namespace ui::debug {
    // Print hierarchy to console
    void print_hierarchy(Entity root = {});
    
    // Get hierarchy as structured data
    struct HierarchyNode {
        Entity entity;
        std::string name;
        Rectangle bounds;
        std::vector<HierarchyNode> children;
    };
    HierarchyNode get_hierarchy(Entity root = {});
    
    // Interactive hierarchy viewer (renders as UI)
    void show_hierarchy_window(bool* open);
}

// Console output example:
// [ROOT]
// ├─ main_container (800x600)
// │  ├─ header (800x60)
// │  │  ├─ title_text (200x30)
// │  │  └─ close_button (40x40)
// │  ├─ content (800x480)
// │  │  ├─ sidebar (200x480)
// │  │  └─ main_panel (600x480)
// │  └─ footer (800x60)
// └─ tooltip_layer [ABSOLUTE]
//    └─ tooltip (150x40)
```

---

## Performance Profiling

### Problem

No built-in performance profiling for UI rendering.

### Suggested Implementation

```cpp
namespace ui::debug {
    struct FrameStats {
        int elements_created;
        int elements_rendered;
        int layout_passes;
        float layout_time_ms;
        float render_time_ms;
        int draw_calls;
        int vertices;
    };
    
    FrameStats get_frame_stats();
    
    // Per-element profiling
    struct ElementProfile {
        std::string name;
        float layout_time_us;
        float render_time_us;
        int child_count;
    };
    std::vector<ElementProfile> get_slow_elements(float threshold_us = 100);
    
    // Show profiler overlay
    void show_profiler_overlay(bool* open);
}

// Profiler overlay display:
// ┌─────────────────────────────────┐
// │ UI Performance                  │
// │ ─────────────────────────────── │
// │ Elements: 127 / 89 rendered     │
// │ Layout: 0.42ms (2 passes)       │
// │ Render: 1.23ms                  │
// │ Draw calls: 45                  │
// │ ─────────────────────────────── │
// │ Slowest elements:               │
// │  • chat_list: 0.31ms            │
// │  • settings_grid: 0.18ms        │
// │  • inventory_slots: 0.12ms      │
// └─────────────────────────────────┘
```

---

## Integration with Existing Debug Systems

```cpp
// Keyboard shortcuts for debug tools
namespace ui::debug {
    void register_debug_shortcuts() {
        // F1: Toggle bounds overlay
        // F2: Toggle hover inspector
        // F3: Toggle hierarchy viewer
        // F4: Toggle profiler
        // F5: Print hierarchy to console
    }
}

// Compile-time enable/disable
#ifdef UI_DEBUG_ENABLED
    #define UI_DEBUG_INIT() ui::debug::register_debug_shortcuts()
    #define UI_DEBUG_OVERLAY() ui::debug::render_overlays()
#else
    #define UI_DEBUG_INIT()
    #define UI_DEBUG_OVERLAY()
#endif
```

