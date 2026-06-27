# Resizable Panels

**Status:** Not implemented  
**Priority:** Medium

---

## Problem

There's no way to create panels that the user can resize by dragging a handle. This is essential for:
- Split-pane editors (code left, preview right)
- Adjustable sidebars (inventory, chat)
- Resizable detail panels
- Debug tool windows

The existing layout system handles static sizing (`pixels`, `percent`, `expand`) but has no concept of user-driven resize at runtime. Child content should automatically reflow when the panel size changes — no manual re-layout needed.

## Suggested API

### Split Panel (Two Panes with Divider)

The most common pattern: two panels separated by a draggable divider.

```cpp
enum class SplitDirection { Horizontal, Vertical };

struct SplitPanelConfig {
    SplitDirection direction = SplitDirection::Horizontal;  // side-by-side
    float initial_ratio = 0.5f;      // Initial split position (0.0–1.0)
    float min_ratio = 0.1f;          // Minimum size for first panel
    float max_ratio = 0.9f;          // Maximum size for first panel
    float handle_size = 6.0f;        // Drag handle thickness in pixels
    Color handle_color = Theme::Usage::Border;
    Color handle_hover_color = Theme::Usage::Accent;
    bool collapsible = false;        // Double-click handle to collapse a panel
};

struct SplitPanelResult {
    Entity first;     // First panel entity — add children to this
    Entity second;    // Second panel entity — add children to this
    float ratio;      // Current split ratio (read back for persistence)
    bool dragging;    // True while user is dragging the handle
};

SplitPanelResult split_panel(HasUIContext auto &ctx, EntityParent ep_pair,
                             float &ratio,  // In/out — persisted by caller
                             SplitPanelConfig config = SplitPanelConfig());

// Usage:
static float split = 0.3f;
auto panels = split_panel(ctx, mk(parent), split,
    SplitPanelConfig{.direction = SplitDirection::Horizontal});

// Left panel: file tree
for (auto &file : files) {
    div(ctx, mk(panels.first, file.id),
        ComponentConfig{}.with_label(file.name)
                         .with_size({expand(), pixels(24)}));
}

// Right panel: editor content (auto-resizes when split changes)
text_area(ctx, mk(panels.second), editor_text,
    ComponentConfig{}.with_size({expand(), expand()}));
```

### Standalone Resizable Panel

A single panel with a drag handle on one or more edges.

```cpp
enum class ResizeEdge {
    Right   = 1 << 0,
    Bottom  = 1 << 1,
    Left    = 1 << 2,
    Top     = 1 << 3,
};

struct ResizablePanelConfig {
    int resize_edges = (int)ResizeEdge::Right;  // Bitmask of draggable edges
    float min_width = 100.0f;
    float min_height = 50.0f;
    float max_width = 0.0f;   // 0 = no max
    float max_height = 0.0f;  // 0 = no max
    float handle_size = 6.0f;
    Color handle_color = Theme::Usage::Border;
    Color handle_hover_color = Theme::Usage::Accent;
    bool show_resize_cursor = true;  // Change cursor on hover
};

struct ResizablePanelResult {
    ElementResult element;
    float width;    // Current width (read back for persistence)
    float height;   // Current height (read back for persistence)
    bool resizing;  // True while user is dragging
};

ResizablePanelResult resizable_panel(HasUIContext auto &ctx, EntityParent ep_pair,
                                     float &width, float &height,
                                     ResizablePanelConfig config = ResizablePanelConfig());

// Usage:
static float panel_w = 300.0f, panel_h = 400.0f;
auto panel = resizable_panel(ctx, mk(parent), panel_w, panel_h,
    ResizablePanelConfig{
        .resize_edges = (int)ResizeEdge::Right | (int)ResizeEdge::Bottom,
        .min_width = 150.0f,
        .min_height = 200.0f
    });

// Children use expand() / percent() — they auto-reflow
scroll_view(ctx, mk(panel.element.ent()), ComponentConfig{}
    .with_size({expand(), expand()}));
```

### Multi-Split (3+ Panels)

For editor-style layouts with multiple resizable sections:

```cpp
struct MultiSplitConfig {
    SplitDirection direction = SplitDirection::Horizontal;
    float handle_size = 6.0f;
    float min_panel_size = 50.0f;  // Minimum pixels per panel
};

struct MultiSplitResult {
    std::vector<Entity> panels;
    bool any_dragging;
};

MultiSplitResult multi_split(HasUIContext auto &ctx, EntityParent ep_pair,
                             std::vector<float> &ratios,  // N-1 ratios for N panels
                             int panel_count,
                             MultiSplitConfig config = MultiSplitConfig());

// Usage: 3-column layout
static std::vector<float> ratios = {0.25f, 0.5f};  // 25% | 50% | 25%
auto cols = multi_split(ctx, mk(parent), ratios, 3);

// Panel 0: sidebar
// Panel 1: main content
// Panel 2: properties
```

## Auto-Resize of Children

The key requirement: children inside a resizable panel should reflow automatically when the panel size changes. This works naturally with the existing sizing primitives:

| Child sizing | Behavior on panel resize |
|-------------|--------------------------|
| `expand()` / `flex_grow()` | Stretches/shrinks to fill available space |
| `percent(0.5f)` | Stays proportional to panel size |
| `children()` | Unaffected (content-driven) |
| `pixels(200)` | Fixed — may overflow if panel shrinks below 200px |
| `screen_pct(0.3f)` | Unaffected (relative to screen, not panel) |

No new layout logic is needed for children — the existing flex layout already handles dynamic parent sizes. The resizable panel just changes its own `pixels()` size each frame based on drag input.

## Implementation Sketch

### Drag Handle

The drag handle is a thin `div` between panels that listens for mouse drag:

```cpp
void render_drag_handle(HasUIContext auto &ctx, EntityParent ep_pair,
                        SplitDirection direction, float &ratio,
                        float total_size, float handle_size,
                        float min_ratio, float max_ratio,
                        bool &dragging) {
    auto handle = div(ctx, ep_pair, ComponentConfig{}
        .with_size(direction == SplitDirection::Horizontal
            ? ComponentSize{pixels(handle_size), expand()}
            : ComponentSize{expand(), pixels(handle_size)})
        .with_background(dragging ? handle_hover_color : handle_color));

    // Check for drag interaction
    auto &ui = handle.ent().get<UIComponent>();
    if (is_mouse_over(ui) && mouse_pressed()) {
        dragging = true;
    }
    if (dragging && mouse_released()) {
        dragging = false;
    }
    if (dragging) {
        float mouse_pos = direction == SplitDirection::Horizontal
            ? get_mouse_x() : get_mouse_y();
        float parent_start = direction == SplitDirection::Horizontal
            ? parent_rect.x : parent_rect.y;
        ratio = (mouse_pos - parent_start) / total_size;
        ratio = std::clamp(ratio, min_ratio, max_ratio);
    }
}
```

### Split Panel Layout

Each frame, the split panel computes pixel sizes from the ratio and sets them on the two child panels:

```cpp
SplitPanelResult split_panel(HasUIContext auto &ctx, EntityParent ep_pair,
                             float &ratio, SplitPanelConfig config) {
    // Container
    auto container = div(ctx, ep_pair, ComponentConfig{}
        .with_size({expand(), expand()})
        .with_flex_direction(config.direction == SplitDirection::Horizontal
            ? FlexDirection::Row : FlexDirection::Column));

    float total = config.direction == SplitDirection::Horizontal
        ? container_width : container_height;
    float handle_px = config.handle_size;
    float available = total - handle_px;
    float first_size = available * ratio;
    float second_size = available * (1.0f - ratio);

    // First panel
    auto first = div(ctx, mk(container.ent(), 0), ComponentConfig{}
        .with_size(config.direction == SplitDirection::Horizontal
            ? ComponentSize{pixels(first_size), expand()}
            : ComponentSize{expand(), pixels(first_size)}));

    // Drag handle
    bool dragging = false;
    render_drag_handle(ctx, mk(container.ent(), 1),
                       config.direction, ratio, total,
                       handle_px, config.min_ratio, config.max_ratio, dragging);

    // Second panel
    auto second = div(ctx, mk(container.ent(), 2), ComponentConfig{}
        .with_size(config.direction == SplitDirection::Horizontal
            ? ComponentSize{pixels(second_size), expand()}
            : ComponentSize{expand(), pixels(second_size)}));

    return {first.ent(), second.ent(), ratio, dragging};
}
```

## Drag Handle Visual Feedback

```cpp
// Default: thin line, changes color on hover
// Hover: accent color + resize cursor
// Dragging: accent color, slightly thicker or highlighted

ComponentConfig{}
    .with_background(handle_color)
    .with_hover_background(handle_hover_color)
    // Future: .with_cursor(CursorType::ResizeHorizontal)
```

### Collapsible Panels

When `collapsible = true`, double-clicking the handle collapses the smaller panel to zero (ratio → 0.0 or 1.0). Double-clicking again restores the previous ratio:

```cpp
if (config.collapsible && double_clicked_handle) {
    if (ratio > config.min_ratio) {
        saved_ratio = ratio;
        ratio = 0.0f;  // Collapse first panel
    } else {
        ratio = saved_ratio;  // Restore
    }
}
```

---

## Example Screen: ResizablePanelShowcase

**File:** `src/systems/screens/ResizablePanelShowcase.h`
**CLI:** `--screen=resizable_panels`
**Category:** Layout

### Layout

1. **Horizontal Split** — A two-pane split (50/50 default). Left pane has a list of 10 items. Right pane shows detail text for the selected item. Dragging the handle resizes both panes. The list items use `expand()` width so they auto-shrink.

2. **Vertical Split** — A top/bottom split (70/30 default). Top pane has a text area. Bottom pane has a console/log output. Demonstrates vertical drag direction.

3. **Three-Column Split** — A `multi_split` with 3 columns (25% | 50% | 25%). Left: file tree. Center: editor. Right: properties. Both handles are independently draggable.

4. **Constrained Split** — A split with `min_ratio = 0.2` and `max_ratio = 0.8`. The handle can't be dragged beyond these limits. Labels show the current ratio.

5. **Collapsible Panel** — A split with `collapsible = true`. Double-clicking the handle collapses the left panel completely. Double-clicking again restores it.

6. **Standalone Resizable** — A single `resizable_panel` with drag handles on the right and bottom edges. Contains a scroll view with content. Demonstrates corner resize (dragging the bottom-right corner resizes both axes).

### Features Exercised

- `split_panel()` horizontal and vertical
- `multi_split()` with 3 panels
- Drag handle hover color change
- `min_ratio` / `max_ratio` constraints
- `collapsible` double-click behavior
- `resizable_panel()` with edge handles
- Child content auto-reflow on resize

### Verification

- Dragging handle resizes both panels proportionally
- Children using `expand()` fill available space after resize
- Handle can't be dragged beyond min/max constraints
- Double-click collapses panel; double-click again restores
- Handle changes color on hover
- Three-column layout: each handle is independent
- Standalone panel: right edge and bottom edge both draggable

### E2E Test Plan

**Test file:** `src/testing/tests/ResizablePanelTest.h`

#### New Custom Commands Needed

- `drag_element(label, dx, dy)` — press on element center, move by (dx, dy), release. Needed for dragging the resize handle.

#### Screenshots

1. `resize_initial` — default 50/50 split
2. `resize_dragged_left` — split dragged to ~30/70
3. `resize_dragged_right` — split dragged to ~70/30
4. `resize_vertical` — vertical split with top/bottom panes
5. `resize_three_column` — three-column multi-split
6. `resize_collapsed` — left panel collapsed via double-click
7. `resize_restored` — left panel restored after second double-click
8. `resize_standalone` — standalone panel resized via edge handles

#### Test Script

```cpp
TEST(resize_split_panel_drag) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("resize_initial");

  // Drag the handle left (reduce left panel)
  drag_element("Split Handle", -200, 0);
  co_await TestApp::wait_for_frames(5);

  auto snap_left = TestApp::capture_snapshot("resize_dragged_left");

  // Drag back to the right
  drag_element("Split Handle", 400, 0);
  co_await TestApp::wait_for_frames(5);

  auto snap_right = TestApp::capture_snapshot("resize_dragged_right");
}

TEST(resize_constrained) {
  co_await TestApp::wait_for_frames(5);

  // Try dragging beyond min_ratio — should be clamped
  drag_element("Constrained Handle", -1000, 0);
  co_await TestApp::wait_for_frames(5);

  // Left panel should still be at min_ratio (20%), not collapsed
  TestApp::expect_ui_exists("20%");
}

TEST(resize_collapsible) {
  co_await TestApp::wait_for_frames(5);

  // Double-click handle to collapse
  TestApp::click_button("Collapsible Handle");
  co_await TestApp::wait_for_frames(3);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(1);
  TestApp::click_button("Collapsible Handle");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_collapsed = TestApp::capture_snapshot("resize_collapsed");

  // Double-click again to restore
  TestApp::click_button("Collapsible Handle");
  co_await TestApp::wait_for_frames(3);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(1);
  TestApp::click_button("Collapsible Handle");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_restored = TestApp::capture_snapshot("resize_restored");
}

TEST(resize_vertical_split) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("resize_vertical");

  // Drag vertical handle down
  drag_element("Vertical Handle", 0, 100);
  co_await TestApp::wait_for_frames(5);

  // Top panel should be larger now
}

TEST(resize_three_column) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("resize_three_column");
}

TEST(resize_standalone_panel) {
  co_await TestApp::wait_for_frames(5);

  // Drag right edge to widen
  drag_element("Right Edge Handle", 100, 0);
  co_await TestApp::wait_for_frames(5);

  // Drag bottom edge to heighten
  drag_element("Bottom Edge Handle", 0, 80);
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("resize_standalone");
}
```
