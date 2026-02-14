# Composite Cleanup Plan

Date: 2026-02-13

## Goal

Reduce code duplication and improve readability in `imm_components.h` by:
1. Adding small primitives (`hstack`, `vstack`, `spacer`)
2. Removing deprecated wrappers
3. Using new primitives inside composites where they reduce code

---

## New Primitives

### `hstack` (primitive) — DONE
- `div` + `FlexDirection::Row` + default `percent(1.0f) x children()`
- Used in: all screens (109 conversions), plus icon_row and stepper_labels inside composites

### `vstack` (primitive) — DONE
- `div` + `FlexDirection::Column` + default `percent(1.0f) x children()`
- Used in: all screens (109 conversions), plus tab_container tab_wrapper inside composites

### `spacer` (primitive) — DONE
- Invisible `expand()` div with `Theme::Usage::None` and `skip_tabbing`
- Available for future use in flex layouts to push siblings apart

---

## Composition Patterns — Analysis

### `labeled_row` — DEFERRED

Pattern: `[label] [control]` with focus cluster, color resolution, corner splitting.

Shared by checkbox, toggle_switch, slider, dropdown (~10-15 lines of shared
boilerplate each). After analysis, extraction was **deferred** because:

- **checkbox** and **dropdown** share the closest pattern (half-width label,
  corner splitting, InFocusCluster) but differ in color resolution: checkbox
  uses a custom `apply_color` lambda while dropdown uses `Theme::Usage::Primary`
  directly.
- **toggle_switch** uses `expand()` label width (not half-width) and
  `Theme::Usage::None` — a fundamentally different layout strategy.
- **slider** saves/restores `color_usage`, uses `init_component` with
  `is_interactive=true`, and manually calls `set_desired_width` on the label
  entity — unique among the four.
- A shared helper would need parameters for: label sizing mode, color
  handling, corner behavior, init_component flags, and post-init hooks.
  The result would be harder to understand than the current inline code.

### `arrow_nav` — DEFERRED

Pattern: `[<] [content] [>]` with index cycling and keyboard listener.

Shared by stepper, navigation_bar, pagination. After analysis, extraction
was **deferred** because:

- **Arrow configs differ**: stepper uses `pixels(24)` with `Usage::None`;
  navigation_bar uses `percent(0.20)` with rounded corners; pagination uses
  `pixels(default_component_size.x / 4)` with symbol font.
- **Center content differs completely**: stepper renders N visible labels
  with neighbor styling; navigation_bar shows a single label; pagination
  shows a row of option buttons.
- **Click handlers differ**: stepper updates `HasStepperState` directly;
  navigation_bar updates `HasNavigationBarState`; pagination calls
  `on_option_click` with different index math.
- **Keyboard support varies**: only stepper has `HasLeftRightListener`.
- A callback-based helper would just wrap two button calls while requiring
  callers to provide arrow configs, click handlers, and center content —
  adding indirection without meaningful deduplication.

---

## Composite-by-Composite Review

| Composite | Status | Action |
|---|---|---|
| `checkbox_no_label` | **Removed** | Callers migrated to `primitive::toggle_button`. |
| `checkbox` | Has labeled_row pattern | Pattern documented, left inline (see analysis). |
| `checkbox_group` | Clean | No changes needed. |
| `radio_group` | Unique rendering | **Left as-is.** Unique circle+dot rendering. |
| `toggle_switch` | Expand-label pattern | Different from labeled_row; left inline. |
| `slider` | Complex | Unique drag/keyboard/label logic; left inline. |
| `pagination` | Has arrow_nav pattern | Pattern documented, left inline (see analysis). |
| `dropdown` | Has labeled_row pattern | Pattern documented, left inline (see analysis). |
| `navigation_bar` | Has arrow_nav pattern | Pattern documented, left inline (see analysis). |
| `tab_container` | **Updated** | Tab wrappers now use `vstack`. |
| `progress_bar` | Clean | Unique overlay pattern. Left as-is. |
| `decorative_frame` | ~235 lines | Unique absolute layers. **Left as-is.** |
| `stepper` | **Updated** | Label container now uses `hstack`. |
| `button_group` | Clean | No changes needed. |
| `icon_row` | **Updated** | Now uses `hstack`. |

---

## Implementation Order

1. [x] `primitive::toggle_button`
2. [x] `primitive::hstack` — added + 109 screen conversions + HStackShowcase
3. [x] `primitive::vstack` — added + 109 screen conversions + VStackShowcase
4. [x] `primitive::spacer` — added
5. [x] Remove `checkbox_no_label` — removed, callers migrated to toggle_button
6. [x] Analyze `labeled_row` — deferred (insufficient commonality)
7. [x] Analyze `arrow_nav` — deferred (insufficient commonality)
8. [x] Update composites — icon_row, tab_container, stepper use hstack/vstack

---

## Notes

- `hstack`/`vstack`/`spacer` are non-breaking additions
- Removing `checkbox_no_label` is a breaking change (deprecated, callers should use `primitive::toggle_button`)
- Moving `div`/`button`/`image`/`sprite` into `primitive::` is a separate breaking change (tracked in existing TODO in imm_components.h)
- The `labeled_row` and `arrow_nav` patterns are real but the implementations vary enough across composites that extraction would add complexity. If future composites repeat these patterns more closely, reconsider extraction.
