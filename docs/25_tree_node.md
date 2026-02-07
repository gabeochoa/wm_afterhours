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

---

## Example Screen: TreeNodeShowcase

**File:** `src/systems/screens/TreeNodeShowcase.h`
**CLI:** `--screen=tree_node`
**Category:** Widgets

### Layout

A file-browser-style tree demonstrating collapsible hierarchy:

1. **File Tree** — A tree with 3 levels: `Project > src > main.cpp, utils.h, editor > buffer.cpp, layout.cpp` and `Project > assets > textures > icon.png, bg.png`. Clicking a node header toggles expand/collapse. Arrow icons (▶/▼) indicate state.

2. **Selection** — Clicking a leaf node (file) highlights it. A label below shows "Selected: src/main.cpp". Only one node selectable at a time.

3. **Programmatic Control** — "Expand All" and "Collapse All" buttons that set all `expanded` bools. "Expand to: editor/buffer.cpp" button that expands the path to a specific node.

4. **Deep Nesting** — A 6-level deep tree to test indentation scaling and scrollability.

### Features Exercised

- `tree_node()` with `bool &expanded`
- Recursive nesting with proper indentation
- Expand/collapse toggle via click and keyboard (Enter/Space)
- Selection state tracking
- Expand/collapse icons (animated rotation if animation system available)

### Verification

- Clicking a collapsed node expands it, showing children
- Clicking an expanded node collapses it, hiding children
- Indentation increases visibly at each nesting level
- "Expand All" makes every node visible
- Keyboard Enter/Space toggles the focused node

### E2E Test Plan

**Test file:** `src/testing/tests/TreeNodeTest.h`

#### New Custom Commands Needed

- `expect_element_count(label_prefix, expected)` — count entities whose label starts with a prefix. Needed to verify how many visible nodes are shown after expand/collapse.

#### Screenshots

1. `tree_initial` — tree collapsed to top-level nodes only
2. `tree_expanded` — "src" node expanded showing child files
3. `tree_fully_expanded` — after "Expand All", all nodes visible
4. `tree_selected` — leaf node selected with highlight
5. `tree_deep_nesting` — 6-level deep tree with visible indentation

#### Test Script

```cpp
TEST(tree_node_expand_collapse) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("tree_initial");

  // Click "src" to expand
  TestApp::click_button("src");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("main.cpp");
  TestApp::expect_ui_exists("utils.h");
  auto snap = TestApp::capture_snapshot("tree_expanded");

  // Click "src" again to collapse
  TestApp::click_button("src");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_not_exists("main.cpp");
}

TEST(tree_node_select_leaf) {
  co_await TestApp::wait_for_frames(5);

  // Expand src
  TestApp::click_button("src");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Select main.cpp
  TestApp::click_button("main.cpp");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Selected: src/main.cpp");
  auto snap = TestApp::capture_snapshot("tree_selected");
}

TEST(tree_node_expand_all) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Expand All");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // All leaves should be visible
  TestApp::expect_ui_exists("main.cpp");
  TestApp::expect_ui_exists("icon.png");
  TestApp::expect_ui_exists("bg.png");
  auto snap = TestApp::capture_snapshot("tree_fully_expanded");
}

TEST(tree_node_keyboard) {
  co_await TestApp::wait_for_frames(5);

  // Tab to focus first node
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Enter to toggle expand
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("main.cpp");
}
```

