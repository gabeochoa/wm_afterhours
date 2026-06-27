# Plan: UI Plugin Refactoring + File Tree Component

## Phase 1: Move internal functions into `detail::` namespaces

Follow the existing pattern in `e2e_testing/input_injector.h`. Remove the TODO comment in `imm_components.h:3-6` since this work addresses it.

### 1A. `layout_types.h` (smallest, do first)

Rename member functions of `ComponentSize`:
- `_scale_x()` -> `scale_x()`
- `_scale_y()` -> `scale_y()`

Update callers in `imm_components.h` and `text_input/component.h`.

### 1B. `component_init.h`

Wrap in `imm::detail::`:
- `_overwrite_defaults`, `_add_missing_components`, `_validate_config`
- All `apply_*` functions: `apply_flags`, `apply_layout`, `apply_label`, `apply_texture`, `apply_shadow`, `apply_border`, `apply_bevel`, `apply_render_layer`, `apply_nine_slice`, `apply_visuals`, `apply_animations`

Promote to public API (drop underscore):
- `_init_component()` -> `init_component()`
- `_init_state()` -> `init_state()`

Update internal calls within `init_component` to use `detail::` prefix.

### 1C. `rendering.h`

Wrap in `ui::detail::`:
- `_compute_effective_opacity`, `_find_clip_ancestor`, `_find_scroll_view_ancestor`
- `_get_scroll_offset`, `_get_scroll_scissor_rect`
- `_fix_scroll_view_child_positions`, `_update_scroll_view_content_size`
- `_draw_text_at_position`

Update ~18 internal call sites within `rendering.h`.

### 1D. `imm_components.h`

Wrap in `imm::detail::`:
- `prev_index`, `next_index`
- `generate_label_text`, `update_label_entity`, `update_handle_label`, `update_main_label`

Update ~7 internal call sites. Rename ~24 calls from `_init_component`/`_init_state` to `init_component`/`init_state`.

### 1E. Other files (1-2 changes each)

- `setting_row.h`: `_init_component` -> `init_component`
- `text_input/component.h`: `_init_state` + `_init_component` -> drop underscore
- `text_input/text_area.h`: `_init_state` + `_init_component` -> drop underscore
- `modal.h`: `_init_component` -> `init_component`

### Phase 1 order

1. `layout_types.h` (simplest)
2. `component_init.h` (defines the functions others depend on)
3. `imm_components.h` (most call sites)
4. `rendering.h` (self-contained)
5. `setting_row.h`, `text_input/component.h`, `text_input/text_area.h`, `modal.h`
6. `make` after each file

No external consumer files in `src/` need changes (confirmed: no showcase screen calls any `_` function directly).

---

## Phase 2: Audit composites for public API compliance

Review each of the 14 composite components to confirm they compose using only:
- Public primitives: `div`, `button`, `image`, `sprite`, `image_button`, `checkbox_no_label`, `circular_progress`
- Public init: `init_component()`, `init_state()`
- Direct ECS access (acceptable for custom interaction logic)

**Pre-audit result**: All composites already pass. No code changes needed. The output of this phase is a documentation comment in `imm_components.h` listing the public API surface for custom component authors.

---

## Phase 3: File tree component

### New files

1. `vendor/afterhours/src/plugins/ui/tree_view.h` — Generic tree view component
2. `src/systems/screens/FileTreeShowcase.h` — Showcase screen

### Modified files

3. `vendor/afterhours/src/plugins/ui/immediate.h` — Add `#include "tree_view.h"`

### Tree view design

**Data model** (generic over `T`):
```cpp
template <typename T>
struct TreeNode {
  T data;
  std::vector<TreeNode<T>> children;
  bool is_leaf = false;
};
```

**State** (ECS component on root entity):
```cpp
struct HasTreeViewState : BaseComponent {
  std::unordered_set<std::string> expanded_nodes;
  std::string selected_node_id;
  bool changed_since = false;
};
```

**Visual composition** (primitives only):
```
tree_view (init_component for root container)
  div (scrollable, overflow_y=Scroll)
    per visible node:
      button (row, full width, click to select)
        div (indent spacer, width = depth * indent_px)
        div (expand arrow: ">" or "v", clickable)
        div (label)
```

**Config**:
```cpp
struct TreeViewConfig {
  float indent_width = 20.0f;
  float row_height = 28.0f;
  std::function<std::string(const T&)> get_label;
  std::function<std::string(const T&)> get_id;
  std::function<bool(const T&)> is_expandable;
};
```

**Public API**: `imm::tree_view(ctx, ep_pair, roots, view_config, config)`

### File tree showcase

- Uses `std::filesystem` to scan directories
- Lazy loading: children read on expand
- Default root: project directory
- Toolbar: Home, Up, Refresh buttons
- Status bar: selected file path + size

---

## Verification

```bash
# After each phase:
make

# Phase 1 — confirm no stale references:
grep -rn '_init_component\|_init_state' vendor/afterhours/src/plugins/ui/ --include='*.h'
# Should only find definitions in detail:: or the renamed public versions

# Phase 3 — run the showcase:
./output/ui_tester --screen=file_tree
```

All 65+ existing screens must continue to compile and render correctly.
