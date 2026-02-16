# Framework Gaps Design

Addresses 8 gaps identified by a developer trying to match an HTML/CSS mockup in afterhours. Two gaps (font weight, rich text) are deferred pending further design.

Source: `~/p/floatinghotel/docs/afterhours_gaps.md`

---

## Gap 1: Custom Hover Background

**Problem:** Hover forces `Theme::Usage::Background` globally. No per-component hover color.

**Change:**

Add `std::optional<Color> hover_color` to `HasColor` in `color.h`.

Add `with_custom_hover_bg(Color)` to `ComponentConfig`. Wire through `apply_visuals()` in `component_init.h`.

In `rendering.h` (RenderImm line ~1161, RenderBatched line ~1693), change:

```cpp
col = context.theme.from_usage(Theme::Usage::Background);
```

to:

```cpp
col = entity.template get<HasColor>().hover_color.value_or(
    context.theme.from_usage(Theme::Usage::Background));
```

**Files:** `color.h`, `imm_components.h`, `component_init.h`, `rendering.h`

---

## Gap 7: Default Child Backgrounds

**Problem:** Child divs without explicit background keep stale `HasColor` from previous frames, causing unexpected colored boxes.

**Change:**

In `component_init.h` `apply_visuals()`, remove the `!config.label.empty()` guard on the `Theme::Usage::Default` transparent fallback. Change:

```cpp
} else if (config.color_usage == Theme::Usage::Default &&
           !config.label.empty()) {
```

to:

```cpp
} else if (config.color_usage == Theme::Usage::Default) {
```

All elements with default color usage now get transparent background. Elements that want a visible background already use `with_background()` or `with_custom_background()`.

**Files:** `component_init.h`

---

## Gap 6: Per-Side Border

**Problem:** `with_border(Color, thickness)` draws all four sides. No way to do just a bottom border.

**Change:**

In `components.h`, add `BorderSide` and restructure `Border`:

```cpp
struct BorderSide {
  Color color = Color{0, 0, 0, 0};
  Size thickness = pixels(0.0f);
  bool has_border() const { return thickness.value > 0.0f && color.a > 0; }
};

struct Border {
  BorderSide top, right, bottom, left;

  static Border all(Color color, Size thickness) {
    BorderSide s{color, thickness};
    return {s, s, s, s};
  }

  bool has_border() const {
    return top.has_border() || right.has_border() ||
           bottom.has_border() || left.has_border();
  }

  bool is_uniform() const {
    return top.color == right.color && right.color == bottom.color &&
           bottom.color == left.color &&
           top.thickness.value == right.thickness.value &&
           right.thickness.value == bottom.thickness.value &&
           bottom.thickness.value == left.thickness.value;
  }
};
```

Existing `with_border(Color, Size)` calls `Border::all()` — fully backwards compatible.

Add `with_border_top/right/bottom/left(Color, Size)` to `ComponentConfig`.

In `rendering.h`, border rendering: if `is_uniform()`, use `draw_rectangle_rounded_lines()` as before. Otherwise, draw individual sides as thin filled rectangles.

**Files:** `components.h`, `imm_components.h`, `component_init.h`, `rendering.h`

---

## Gap 4: Flex Gap

**Problem:** No CSS `gap` equivalent. Must manually add margins to every child.

**Change:**

Add `float gap = 0.f` to `UIComponent` in `ui_core_components.h`.

Add `with_gap(Size)` to `ComponentConfig`. Resolve to pixels in `component_init.h`.

In `autolayout.h`, the layout loop at line ~979 initializes `float gap = 0.f` from justify-content distribution. Change to start from the widget's explicit gap value. The remaining-space calculation for `SpaceBetween`/`SpaceAround` subtracts `gap * (num_children - 1)` first, then distributes leftover space on top.

For `expand()` children, remaining space calculation also accounts for gap: `remaining = total - fixed_children_total - gap * (num_children - 1)`.

**Tests (in `autolayout_test.cpp`):**

- `gap_column_basic` — 3 children with 10px gap, verify offsets 0/60/120
- `gap_row_basic` — 2 children with 8px gap in Row, verify offsets 0/108
- `gap_with_expand` — fixed + expand with 20px gap, expand gets 300-50-20=230
- `gap_single_child` — gap has no effect with one child

**Files:** `ui_core_components.h`, `imm_components.h`, `component_init.h`, `autolayout.h`, `autolayout_test.cpp`

---

## Gap 3: Text Overflow Ellipsis

**Problem:** Text wider than container is clipped silently. No automatic truncation with "...".

**Change:**

Add enum and config method:

```cpp
enum struct TextOverflow { Clip, Ellipsis };
```

Add `TextOverflow text_overflow = TextOverflow::Clip` to `HasLabel`. Add `with_text_overflow(TextOverflow)` to `ComponentConfig`.

In `rendering.h` `draw_text_in_rect()`, when `text_overflow == Ellipsis` and `position_text_ex()` returns `text_fits == false`:

1. Measure "..." at the resolved font size
2. Binary search for longest prefix where `prefix_width + ellipsis_width <= container_width`
3. Draw `prefix + "..."` instead of original text

Default is `Clip` — no change for existing users.

When combined with styled spans (future rich text), ellipsis truncates across span boundaries.

**Files:** `ui_core_components.h`, `imm_components.h`, `component_init.h`, `rendering.h`

---

## Gap 8: Absolute Child Positioning (Investigation)

**Problem (reported):** Children of absolute-positioned elements render at screen (0,0).

**Finding:** The layout engine already handles this correctly in `compute_rect_bounds()` (line 1286-1311) — it accumulates parent offset including absolute parents. The `component_init.h` code (line 371-379) correctly moves translate values to `absolute_pos_x/y` for absolute elements and zeros out `HasUIModifiers` translate.

**Action:** Write a test to verify absolute parent with flow children inherits position correctly. If it passes, the issue is on the developer's side (likely setting translate after init, or not using `with_absolute_position()` + `with_translate()` together). If it fails, fix the layout engine.

**Test:**

```cpp
TEST(absolute_parent_children_inherit_position) {
  TestLayout t;
  auto &root = t.make_ui(pixels(800), pixels(600));

  auto &abs_parent = t.make_ui(pixels(200), pixels(200));
  t.ui(abs_parent).make_absolute();
  t.ui(abs_parent).absolute_pos_x = 100.f;
  t.ui(abs_parent).absolute_pos_y = 50.f;

  auto &child = t.make_ui(pixels(80), pixels(40));
  t.add_child(root, abs_parent);
  t.add_child(abs_parent, child);
  t.run(root);

  // Child should be at parent's position (100, 50) + child's own offset (0, 0)
  CHECK_APPROX(t.ui(child).computed_rel[Axis::X], 100.f);
  CHECK_APPROX(t.ui(child).computed_rel[Axis::Y], 50.f);
}
```

**Files:** `autolayout_test.cpp`

---

## Gap 9: Cursor Change on Hover

**Problem:** No API to change mouse cursor on interactive elements.

**Change:**

Add enum and component in `components.h`:

```cpp
enum struct CursorType { Default, Pointer, ResizeH, ResizeV, Text };

struct HasCursor : BaseComponent {
  CursorType cursor = CursorType::Default;
  explicit HasCursor(CursorType c = CursorType::Default) : cursor(c) {}
};
```

Add `with_cursor(CursorType)` to `ComponentConfig`. Wire through `component_init.h`.

In the render system, after hit-testing determines `is_hot`, check for `HasCursor` and call platform cursor API:

```cpp
if (context.is_hot(entity.id) && entity.has<HasCursor>()) {
    SetMouseCursor(to_raylib_cursor(entity.get<HasCursor>().cursor));
}
```

Reset to `MOUSE_CURSOR_DEFAULT` when nothing with a cursor is hot.

**Files:** `components.h`, `imm_components.h`, `component_init.h`, `rendering.h`

---

## Gap 10: Letter Spacing

**Problem:** No `letter-spacing` equivalent. Section headers look cramped.

**Change:**

Add `float letter_spacing = 0.f` to `HasLabel`. Add `with_letter_spacing(float)` to `ComponentConfig`.

Thread the value through text measurement and drawing calls in `rendering.h` — `measure_text()` and `draw_text_at_position()` already accept a spacing parameter.

**Files:** `ui_core_components.h`, `imm_components.h`, `component_init.h`, `rendering.h`

---

## Deferred

| Gap | Reason |
|-----|--------|
| Font weight (#2) | Needs font family registry infrastructure |
| Rich text (#5) | Implementation approach TBD |

---

## Testing & Demo Plan

**Autolayout unit tests (`autolayout_test.cpp`):**
- Gap 4 (Flex gap): `gap_column_basic`, `gap_row_basic`, `gap_with_expand`, `gap_single_child`
- Gap 8 (Absolute positioning): `absolute_parent_children_inherit_position`

**Visual demos in existing screens:**
- Gap 6 (Per-side border) → `ExampleBorders.h`
- Gap 3 (Text overflow ellipsis) → `ExampleTextOverflow.h`
- Gap 10 (Letter spacing) → `ExampleText.h`
- Gap 1 (Custom hover bg) → `Buttons.h`
- Gap 4 (Flex gap) → `ExampleFlexAlignment.h`

**Regression check:**
- Gap 7 (Default child backgrounds) → run `--focus-test` across all screens, spot-check for visual regressions

---

## Implementation Order

1. **Gap 7** — Default child backgrounds (1 line change, high impact)
2. **Gap 1** — Custom hover background (low difficulty, high impact)
3. **Gap 6** — Per-side border (moderate, high reuse)
4. **Gap 4** — Flex gap (tests first, then layout engine)
5. **Gap 8** — Absolute positioning test (investigation)
6. **Gap 3** — Text overflow ellipsis
7. **Gap 10** — Letter spacing
8. **Gap 9** — Cursor on hover
