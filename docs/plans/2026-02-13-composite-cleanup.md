# Composite Cleanup Plan

Date: 2026-02-13

## Goal

Reduce code duplication and improve readability in `imm_components.h` by:
1. Adding small primitives (`hstack`, `vstack`, `spacer`)
2. Extracting shared composition patterns (`labeled_row`, `arrow_nav`)
3. Removing deprecated wrappers

---

## New Primitives to Add

### `hstack` (primitive)
- `div` + `FlexDirection::Row` + default `children() x children()`
- Used in: checkbox, toggle_switch, slider, dropdown, navigation_bar, pagination, stepper, icon_row, tab_container, radio_group, checkbox_group, stepper labels
- ~14 call sites inside composites alone

### `vstack` (primitive)
- `div` + `FlexDirection::Column` + default `children() x children()`
- Used in: dropdown button_group, tab_container wrapper

### `spacer` (primitive)
- Invisible `expand()` div with `Theme::Usage::None` and `skip_tabbing`
- Used in: toggle_switch label (pushes toggle right), common flex pattern

---

## Shared Composition Patterns to Extract

### `labeled_row`
Pattern: `[label] [control]` with focus cluster, color resolution, corner splitting.

Shared by:
- `checkbox` (~40 lines)
- `toggle_switch` (~30 lines)
- `slider` (~30 lines)
- `dropdown` (~25 lines)

Common logic:
1. Strip `config.label`, save as `label`
2. Set row layout (flex_direction=Row, align_items=Center)
3. `init_component` on container
4. Add `FocusClusterRoot`
5. If label not empty: create label div with `scale_x(0.5f)`, right-sharp corners, `InFocusCluster`
6. Create control with left-sharp corners (if label present)

### `arrow_nav`
Pattern: `[<] [content] [>]` with index cycling and keyboard listener.

Shared by:
- `stepper` (~130 lines)
- `navigation_bar` (~90 lines)
- `pagination` (~95 lines)

Common logic:
1. Row layout container
2. Left arrow button -> `prev_index()`
3. Center content area (caller-provided)
4. Right arrow button -> `next_index()`
5. `HasLeftRightListener` for keyboard

---

## Composite-by-Composite Review

| Composite | Lines | Status | Action |
|---|---|---|---|
| `checkbox_no_label` | 488-507 | Deprecated | **Remove.** 1:1 wrapper around `primitive::toggle_button`. |
| `checkbox` | 509-620 | Has `labeled_row` pattern | Extract shared label+control row setup. |
| `checkbox_group` | 622-675 | Clean | Use `vstack` for container. |
| `radio_group` | 679-767 | Hardcoded pixel sizes | **Leave as-is.** Unique circle+dot rendering, not shared. |
| `toggle_switch` | 773-876 | Has `labeled_row` pattern | Extract shared label+control row setup. Pill+knob is unique/clean. |
| `slider` | 934-1120 | Has `labeled_row` pattern, longest composite | Extract label row. Drag/keyboard is inherent complexity. |
| `pagination` | 1122-1217 | Has `arrow_nav` pattern | Extract shared arrow+content+arrow structure. |
| `dropdown` | 1219-1360 | Has `labeled_row` pattern | Extract label side. Dropdown menu (abs button_group) is unique. |
| `navigation_bar` | 1363-1451 | Has `arrow_nav` pattern | Nearly identical to stepper minus multi-visible labels. |
| `tab_container` | 1480-1563 | Fairly clean | Use `hstack`/`vstack` for readability. |
| `progress_bar` | 1575-1665 | Clean | Unique overlay pattern. Leave as-is. |
| `decorative_frame` | 1784-2020 | ~235 lines, all absolute layers | Unique. **Leave as-is.** |
| `stepper` | 2044-2173 | Has `arrow_nav` pattern | Extract shared arrow structure. Multi-visible labels unique to stepper. |
| `button_group` | 430-483 | Clean | Minor: use `hstack`/`vstack`. |
| `icon_row` | 338-370 | Clean | Minor: use `hstack`. |

---

## Implementation Order

1. [x] `primitive::toggle_button` (done)
2. [ ] `primitive::hstack`
3. [ ] `primitive::vstack`
4. [ ] `primitive::spacer`
5. [ ] Remove `checkbox_no_label` (deprecated)
6. [ ] Extract `labeled_row` composition helper
7. [ ] Extract `arrow_nav` composition helper
8. [ ] Update composites to use new primitives/helpers

---

## Notes

- `hstack`/`vstack`/`spacer` are non-breaking additions (new functions in `primitive::` namespace)
- `labeled_row` and `arrow_nav` are internal refactors (composites still have same public API)
- Removing `checkbox_no_label` is a breaking change (deprecated, callers should use `primitive::toggle_button`)
- Moving `div`/`button`/`image`/`sprite` into `primitive::` is a separate breaking change (tracked in existing TODO in imm_components.h)
