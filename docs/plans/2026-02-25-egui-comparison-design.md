# egui vs afterhours UI — Comparison & Opportunities

Analysis of [egui's demo library](https://github.com/emilk/egui/tree/main/crates/egui_demo_lib/src/demo) compared to `vendor/afterhours/src/plugins/ui/`.

---

## 1. Missing Widget Types

### High Value

| Widget | egui | afterhours | Notes |
|--------|------|------------|-------|
| **DragValue** | `egui::DragValue::new(&mut val).speed(1.0)` | — | Numeric field you drag to change (like Blender). Supports speed, ranges, suffixes ("45°"), click-to-type. Huge for game settings UIs. |
| **CollapsingHeader** | `ui.collapsing("Title", \|ui\| { ... })` | — | Expandable/collapsible content sections. egui uses these everywhere. `tree_view.h` exists but no general collapsible section primitive. |
| **Tooltips** | `.on_hover_text("hint")` on any widget | — | Zero tooltip support in afterhours. egui chains tooltips on every single response. This is the biggest UX gap. |
| **Color Picker** | `ui.color_edit_button_srgba(&mut color)` | — | Full color editor with alpha, sRGB/linear modes. Very useful for dev tools and game editors. |
| **Spinner** | `egui::Spinner::new()` | — | Animated loading indicator. Simple but missing. |

### Medium Value

| Widget | egui | afterhours | Notes |
|--------|------|------------|-------|
| **Selectable labels** | `ui.selectable_value(&mut val, opt, "label")` | — | Labels that toggle on click (tag pills). egui uses these as an alternative to radio buttons. |
| **Password field** | `TextEdit::singleline(s).password(true)` | `text_input` exists | Text input with show/hide toggle. afterhours has text_input but no password masking mode. |
| **Indeterminate checkbox** | `Checkbox::new(&mut v, "All").indeterminate(flag)` | — | Tri-state checkbox (checked/unchecked/partial). Useful for "select all" patterns with partial selection. |
| **Context menus** | `Popup::context_menu(&response)` | — | Right-click popup menus with nested submenus. Full menu system. |
| **Resizable containers** | `Resize::default().show(ui, \|ui\| { ... })` | — | User-draggable resize handles on panels/containers. |
| **Grid layout** | `Grid::new("id").num_columns(2).striped(true)` | — | Auto-columned grid with striping, spacing control. Different from flex — true label:value alignment. |
| **Table (virtual)** | `TableBuilder` with 100k rows | — | Full table component with headers, resizable columns, row selection, virtual scrolling. |

---

## 2. API Ergonomics

### Patterns egui does well that afterhours could learn from

#### 2a. Response chaining / Tooltip system

egui's killer API feature: every widget returns a `Response`, and you chain behaviors onto it.

```rust
// egui
ui.button("Click me!")
    .on_hover_text("Does a thing")
    .on_disabled_hover_text("Enable this first");

ui.checkbox(&mut val, "Option")
    .on_hover_ui(|ui| {
        ui.label("Rich tooltip with widgets inside!");
        ui.hyperlink("docs.rs");
    });
```

afterhours has `ElementResult` which is close, but it only carries `{changed, entity, value}`. There's no mechanism to attach hover behavior, tooltips, or post-interaction decoration.

**Opportunity:** Extend `ElementResult` with tooltip/hover-text chaining. Even a simple version would be transformative:
```cpp
button(ctx, mk(parent), config.with_label("Save"))
    .on_hover_text("Save your progress");
```

#### 2b. Widget-as-value pattern

egui treats widgets as objects you configure then add:

```rust
// egui: widget is a configured object
ui.add(Slider::new(&mut val, 0.0..=100.0)
    .logarithmic(true)
    .suffix("°")
    .step_by(5.0)
    .trailing_fill(true));
```

afterhours uses function calls with a monolithic `ComponentConfig`:

```cpp
// afterhours: everything goes through ComponentConfig
slider(ctx, mk(parent), value,
    ComponentConfig{}.with_label("Angle").with_size(...));
```

The afterhours approach isn't bad, but `ComponentConfig` is a god-object (the todo.md even flags this). Widget-specific config structs (e.g., `SliderConfig` with `.logarithmic()`, `.suffix()`, `.step()`) would make the API self-documenting.

#### 2c. Inline layout closures

egui's layout API is extremely ergonomic:

```rust
ui.horizontal(|ui| {
    ui.label("Name:");
    ui.text_edit_singleline(&mut name);
});
```

afterhours requires explicit entity creation:
```cpp
auto row = hstack(ctx, mk(parent));
div(ctx, mk(row.ent()), config.with_label("Name:"));
text_input(ctx, mk(row.ent()), name, config);
```

Not necessarily worse — the ECS approach has benefits — but higher ceremony for simple layouts.

#### 2d. Built-in disabled/enabled states

egui has `ui.add_enabled(condition, widget)` and `ui.add_enabled_ui(condition, |ui| { ... })` which dims everything inside when disabled. afterhours has `config.disabled` but the ergonomics of wrapping a whole section in disabled state is clunkier.

#### 2e. Reset button

egui has a one-liner: `egui::reset_button(ui, self, "Reset")` which resets any struct to its `Default`. Small but shows the "batteries included" philosophy.

---

## 3. Performance Techniques

### 3a. Virtual scrolling (the big one)

egui's `ScrollArea::show_rows()` only layouts and paints visible rows. Their table demo handles **100,000 rows** at 60fps:

```rust
ScrollArea::vertical().show_rows(ui, row_height, 10_000, |ui, row_range| {
    for row in row_range {
        ui.label(format!("Row {}", row));
    }
});
```

Only ~20-30 rows are ever laid out or painted per frame. The rest are skipped entirely.

afterhours' `HasScrollView` clips children visually but still **creates entities and runs layout for all of them**. With 100+ items, this becomes the bottleneck.

**Opportunity:** Add a virtual scroll mode to `HasScrollView` that:
1. Takes a row count + row height
2. Only creates entities for visible rows (range-based callback)
3. Sets total content height for correct scrollbar sizing

This is likely the single biggest performance win available.

### 3b. Visibility culling at paint time

egui wraps painting in `if ui.is_rect_visible(rect)` — elements outside the viewport skip their paint calls entirely. The toggle switch demo shows this:

```rust
if ui.is_rect_visible(rect) {
    // all painting code here
}
```

afterhours' rendering walks every entity. Even with scissor clipping, it still processes draw calls for off-screen elements.

**Opportunity:** In the render loop, check if an entity's computed rect intersects the viewport before issuing draw calls. This is a cheap AABB test that can skip significant work in scroll views.

### 3c. ID-based animation (no per-widget allocation)

egui's animation system is elegant: `ui.ctx().animate_bool_responsive(response.id, *on)` — the context tracks animation state keyed by widget ID in a global hashmap. No component allocation, no entity, no animation track.

afterhours uses `HasToggleSwitchState::animation_progress` and inline lerp, or the full `animation::AnimationManager<Key>` system. Both work but involve per-entity state components or explicit key management.

**Opportunity:** A lightweight `UIContext::animate_bool(entity_id, target)` that returns a 0-1 float, storing state internally. Would simplify toggle/checkbox/hover animations significantly.

### 3d. Text layout caching

egui caches shaped text ("galleys") and reuses them across frames. Text shaping is expensive — measuring string widths, breaking lines, computing glyph positions.

afterhours has `TextCache` (`core/text_cache.h`) but the extent of caching in the UI render path is unclear.

**Opportunity:** Ensure text measurement results are cached per-string per-font-size and invalidated only when the string or container width changes.

### 3e. Repaint-on-demand

egui only repaints when something changes. Idle UI = zero CPU. Widgets call `ui.request_repaint()` when they need animation updates.

afterhours repaints every frame (game loop). This is fine for games, but for tool UIs or settings screens, skipping frames when nothing changed would save power/thermals.

**Opportunity:** Not urgent for a game UI, but worth noting. A dirty flag on UIContext that skips relayout when no input occurred and no animations are running.

---

## 4. Behavioral Polish

### 4a. Slider improvements

egui's slider has features that make it feel professional:

| Feature | egui | afterhours |
|---------|------|------------|
| Click numeric value to type | Yes | No |
| Logarithmic scale | Yes | No |
| Smart aim (snaps to round numbers) | Yes | No |
| Step increments | Yes (`step_by(5.0)`) | Fixed 0.01 step |
| Suffix/prefix display | Yes (`"45°"`) | No |
| Vertical orientation | Yes | No |
| Trailing fill color | Yes (optional) | No |
| Custom handle shapes | Circle, Rect | Fixed rectangle |
| Range clamping modes | Never/Edits/Always | Always |

**Opportunity:** Most of these are config additions to the existing `slider()`. Logarithmic + smart aim + click-to-edit would make the biggest difference in feel.

### 4b. Smooth animations everywhere

egui's `animate_bool_responsive(id, value)` is used for:
- Toggle switch knob sliding
- Hover state transitions (bg color fades)
- Collapsing header open/close
- Focus ring appearance

afterhours has animations for toggle switch (`animation_progress` + inline lerp) but hover states and focus appear to be instant. The `animation.h` plugin exists but isn't integrated into the core UI render path for transitions.

**Opportunity:** Add automatic hover/focus transition animations in the rendering system. When `HasColor` changes due to hover, lerp over 2-3 frames instead of snapping. This is what makes egui feel "smooth."

### 4c. Disabled state rendering

egui multiplies opacity for the entire disabled subtree. afterhours has `config.disabled` which prevents interaction but the visual treatment is less consistent.

**Opportunity:** When a widget is disabled, automatically reduce opacity (e.g., 0.5) and skip hover color changes. Should be a rendering-level concern, not per-widget.

### 4d. Text handling

egui has rich text features afterhours lacks:
- **Text truncation with ellipsis** on labels (afterhours has `TextOverflow::Ellipsis` — good)
- **Rich text** with mixed colors, fonts, sizes, underlines, strikethrough in a single label
- **Selectable text** in labels and tooltips
- **Hint text** (placeholder) in text inputs
- **Text wrapping modes** (Truncate, Wrap, Extend)

---

## 5. Architectural Patterns Worth Stealing

### 5a. Custom widget protocol

egui's custom widget pattern is brilliantly simple (toggle_switch.rs is 15 lines):

1. Decide a size
2. Allocate space (`ui.allocate_exact_size`)
3. Handle interaction (check `response.clicked()`)
4. Paint (draw to `ui.painter()`)

afterhours' widget creation requires understanding ECS entities, `init_component()`, `ComponentConfig`, the entity parent system, etc. The barrier to creating a custom widget is much higher.

**Opportunity:** Document a "custom widget in 20 lines" pattern for afterhours. Even if the underlying system is more complex, a clear recipe lowers the barrier.

### 5b. Scene / pan-zoom container

egui's `Scene` widget wraps any UI in a pannable, zoomable viewport. The demo literally embeds the entire widget gallery inside a Scene. This is useful for:
- Map editors
- Node graph editors
- Zoomable game UIs

afterhours has no equivalent.

### 5c. Undo/Redo utility

egui provides `Undoer<State>` — a generic undo/redo stack that works with any Clone+PartialEq type. You feed it state each frame, and it tracks changes with debouncing.

afterhours has `command_history.h` but it's a different pattern (command objects vs state snapshots).

---

## 6. Priority Recommendations

### Tier 1 — High Impact, Moderate Effort

#### 1. Tooltips

**What:** Any widget can show a text tooltip on hover. Chaining syntax on `ElementResult`.

**Why:** The single biggest UX gap. egui chains `.on_hover_text()` on every widget. afterhours has zero tooltip support — no component, no rendering, no API.

**How it works in egui:**
```rust
ui.button("Save").on_hover_text("Save your progress");
ui.checkbox(&mut val, "Option").on_hover_ui(|ui| {
    ui.label("Rich tooltip content");
    ui.hyperlink("docs.rs");
});
```

**Implementation approach:**
- Add a `HasTooltip` component: `{ std::string text; float delay_seconds = 0.4f; }`
- Extend `ElementResult` with `.on_hover_text(string)` that attaches `HasTooltip` to the entity
- In the UI render systems, when an entity is hot and has been hot for > delay, render a floating div at the mouse position with the tooltip text
- The tooltip div should be rendered at the highest render layer, with a slight offset from the cursor
- Position clamping: keep tooltip within screen bounds (flip if near edge)
- Simple version first: text-only. Rich tooltips (with nested widgets) can come later.

**Touches:** `ElementResult`, `rendering.h` (new tooltip render pass), new `HasTooltip` component in `components.h`

**Effort:** ~200 lines. 1-2 sessions.

---

#### 2. Virtual Scrolling

**What:** Scroll views that only create/layout/render visible items. Handles 10,000+ items at 60fps.

**Why:** The single biggest performance gap. afterhours creates entities for ALL children in a scroll view, runs layout on all of them, then clips visually. egui only creates entities for visible rows.

**How it works in egui:**
```rust
ScrollArea::vertical().show_rows(ui, row_height, 10_000, |ui, row_range| {
    // row_range is e.g. 45..65 — only 20 rows created
    for row in row_range {
        ui.label(format!("Row {}", row));
    }
});
```

**Implementation approach:**
- Add a new imm function: `virtual_scroll(ctx, ep_pair, item_count, item_height, callback)` where callback receives `(ctx, parent_entity, visible_range)`
- The function creates a scroll container with `HasScrollView` and computes which items are visible based on `scroll_offset / item_height`
- Total content height is set to `item_count * item_height` for correct scrollbar behavior
- Only calls the callback for the visible range (plus 1-2 items of buffer above/below for smooth scrolling)
- Heterogeneous row heights: harder. Could support a height-callback variant later, or require uniform height for v1.

**Touches:** New function in `imm_components.h`, minor additions to `HasScrollView` for virtual mode, autolayout needs to respect virtual content height.

**Effort:** ~300-400 lines. 2-3 sessions. Significant but well-scoped.

---

#### 3. Hover/Focus Transition Animations

**What:** When a widget's hover or focus state changes, the visual properties (background color, border) transition smoothly over 2-3 frames instead of snapping instantly.

**Why:** This is the #1 reason egui "feels smooth." It's subtle but your brain notices the difference. afterhours snaps colors instantly on hover/focus changes.

**How it works in egui:**
```rust
let visuals = ui.style().interact_selectable(&response, *on);
// visuals.bg_fill is already interpolated based on hover/press state
// egui's style system returns smoothly-transitioning colors
```

**Implementation approach:**
- In `rendering.h`, where `HasColor` is resolved for drawing, instead of using the target color directly, lerp from the previous frame's color
- Add a small per-entity cache: `{ Color previous_bg; Color previous_border; }` — could be a lightweight component `HasColorTransition` or stored in `HasColor` directly
- Each frame: `drawn_color = lerp(previous_color, target_color, 0.15f)` (or use the existing animation easing system)
- When `|previous - target| < epsilon`, snap to target and stop lerping
- This covers: button hover, button press, focus ring appearance, toggle state changes
- Could be opt-out via a flag for elements that need instant color changes

**Touches:** `rendering.h` (color resolution path), possibly a new small component or fields on `HasColor`

**Effort:** ~100-150 lines. 1 session. High reward for low effort.

---

#### 4. CollapsingHeader

**What:** A clickable header that expands/collapses a content section with animation.

**Why:** Essential for organizing dense UIs. Settings screens, debug panels, inspector windows all need collapsible sections. egui uses these everywhere.

**How it works in egui:**
```rust
ui.collapsing("Advanced Settings", |ui| {
    ui.checkbox(&mut self.debug, "Debug mode");
    ui.slider(&mut self.quality, 0..=100);
});

// Custom header variant:
CollapsingState::load_with_default_open(ctx, id, false)
    .show_header(ui, |ui| { /* custom header widgets */ })
    .body(|ui| { /* content */ });
```

**Implementation approach:**
- New imm function: `collapsing_header(ctx, ep_pair, label, &open, config)` returning `ElementResult`
- Internally: creates a button for the header (with a "▶"/"▼" indicator), and a div for the body
- When `open` is false, the body div gets `ShouldHide` or is simply not created
- Animate the open/close: use the animation system to lerp body height from 0 to content height (or simpler: just animate an opacity/translate for a slide-down effect)
- The `open` state is a `bool&` owned by the caller (same pattern as checkbox/toggle)
- Nested collapsing headers should work naturally since they're just divs inside divs

**Touches:** New function in `imm_components.h`, new `HasCollapsingState` component in `components.h`

**Effort:** ~150-200 lines. 1-2 sessions.

---

### Tier 2 — High Impact, Higher Effort

#### 5. DragValue

**What:** A numeric input field that you can click-drag to change the value (like Blender/Unity number inputs). Click to type a precise value.

**Why:** More useful than sliders for many game dev scenarios — doesn't need a defined range, supports any numeric precision, compact.

**How it works in egui:**
```rust
ui.add(DragValue::new(&mut scalar).speed(1.0));
ui.add(DragValue::new(&mut opacity).speed(0.01).range(0.0..=1.0));
ui.add(DragValue::new(&mut angle).suffix("°").speed(0.5));
```

**Implementation approach:**
- New imm function: `drag_value(ctx, ep_pair, &value, DragValueConfig{})`
- `DragValueConfig`: speed (pixels-per-unit), optional range, optional suffix/prefix, optional step
- Two modes: **display mode** (shows formatted value, draggable) and **edit mode** (text input for typing)
- Display mode: renders as a div with label. On drag (`HasDragListener`), multiply mouse delta.x by speed and add to value
- Edit mode: on click (without drag), switch to a text_input. On Enter/blur, parse and apply
- Visual: cursor changes to `ResizeH` on hover. Subtle underline or dotted-border to hint it's interactive
- Support int and float via template or overloads

**Touches:** New function in `imm_components.h`, new `HasDragValueState` component, new `DragValueConfig` (or extend `ComponentConfig`)

**Effort:** ~300 lines. 2 sessions.

---

#### 6. Slider Improvements

**What:** Bring afterhours' slider up to egui's feature level.

**Why:** Sliders are one of the most-used widgets. egui's slider feels professional; afterhours' feels basic.

**Features to add (in priority order):**

1. **Configurable step size** — current hardcoded `0.01f` step becomes configurable. Add `with_slider_step(float)` to ComponentConfig or a `SliderConfig`.

2. **Suffix/prefix** — display "45°" or "$100" instead of just the number. Add `with_slider_suffix(string)` / `with_slider_prefix(string)`.

3. **Trailing fill** — colored fill from the left edge to the handle position. Currently the handle floats on a plain track. Add a fill div that stretches from 0 to handle position.

4. **Value range display** — show min/max labels at the ends of the slider track.

5. **Click-to-edit** — clicking the displayed value opens a text input for precise entry. Requires integration with text_input.

6. **Logarithmic scale** — map the slider position through `log/exp` so huge ranges (0.001 to 1000) are usable. Add `with_slider_logarithmic(bool)`.

7. **Smart aim** — when dragging near a round number (50, 100, 250), snap to it with a small dead zone. Makes it easy to hit common values.

8. **Vertical orientation** — rotate the slider 90°. Useful for audio mixers, color channels.

**Implementation approach:** Most are additions to the existing `slider()` function and `ComponentConfig`/`SliderConfig`. Logarithmic + smart aim change the value mapping math. Click-to-edit requires toggling between slider and text_input mode.

**Effort:** ~400 lines total across all features. 2-3 sessions. Can be done incrementally.

---

#### 7. Visibility Culling in Renderer

**What:** Skip draw calls for entities whose computed rect is entirely outside the viewport.

**Why:** In a scroll view with 50+ items, half might be off-screen but still processed for rendering. An AABB check costs ~5 nanoseconds per entity and can skip expensive draw calls (text shaping, texture sampling, rounded corner tessellation).

**How it works in egui:**
```rust
if ui.is_rect_visible(rect) {
    // all painting code here — skipped entirely if off-screen
}
```

**Implementation approach:**
- At the top of the render loop (in `rendering.h`), before drawing any entity, check if `entity.rect()` intersects the viewport rect (or the nearest clip ancestor's rect)
- `find_accumulated_clip_rect()` already exists — use its result
- If no intersection, skip the entity's draw calls entirely (background, border, shadow, text, image)
- Children of a fully-clipped parent can also be skipped (early-out the subtree)
- Add a debug counter to track how many entities were culled per frame

**Touches:** `rendering.h` render loop

**Effort:** ~50-80 lines. Half a session. Very high reward-to-effort ratio.

---

#### 8. ID-Based Animation Helper

**What:** A lightweight `ctx.animate_bool(entity_id, target_bool)` that returns a smoothly-interpolated 0.0-1.0 float, with state stored internally in the UIContext.

**Why:** egui uses `animate_bool_responsive(id, value)` for toggle transitions, collapsing headers, hover fades — everything. No component allocation, no animation key enum, no explicit track management. Just call it and get a smooth value.

**How it works in egui:**
```rust
let how_on = ui.ctx().animate_bool_responsive(response.id, *on);
// Returns 0.0..1.0, smoothly animated
let circle_x = egui::lerp(left..=right, how_on);
```

**Implementation approach:**
- Add to `UIContext`: `std::unordered_map<EntityID, float> bool_animations;`
- New method: `float animate_bool(EntityID id, bool target, float speed = 0.15f)`
  - Looks up current value (default 0.0 for false, 1.0 for true on first call)
  - Lerps toward target (0.0 or 1.0) by speed factor
  - Snaps when within epsilon
  - Returns current interpolated value
- Cleanup: remove entries for entities that no longer exist (check at end of frame)
- Usage in toggle_switch, checkbox, collapsing_header — replace inline lerp code

**Touches:** `context.h` (new map + method)

**Effort:** ~60 lines. Half a session.

---

### Tier 3 — Nice to Have

#### 9. Grid Layout

**What:** A layout mode where children are arranged in a grid with aligned columns, like an HTML table. Not the same as flex-wrap — columns align vertically across rows.

**Why:** Settings screens often have label:value pairs. With flex, you manually size labels to align. A grid auto-aligns columns. egui's `Grid::new("id").num_columns(2).striped(true)` makes settings panels trivial.

**Implementation approach:**
- New imm function: `grid(ctx, ep_pair, num_columns, config)` returning a container
- Children added to the grid are placed left-to-right, wrapping at `num_columns`
- Column widths are computed as `max(child widths in that column)` — requires a two-pass layout (measure pass, then position pass)
- Optional striping: alternate row backgrounds
- This is architecturally harder because afterhours' autolayout is single-pass. May need a "grid layout mode" in the layout system or a post-layout fixup pass.

**Effort:** ~400-500 lines. 3+ sessions. Significant layout system work.

---

#### 10. Color Picker

**What:** An interactive color editor with a hue wheel/square, alpha slider, and numeric inputs for RGB/HSV values.

**Why:** Essential for game editors, level designers, theme customization tools. Not needed for player-facing game UI typically.

**Implementation approach:**
- New composite widget built from existing primitives: a hue bar (slider), a saturation/value square (custom painted area with drag), alpha slider, and text inputs for numeric values
- The SV square requires custom rendering — a gradient-filled quad with drag interaction
- Could start with a simpler version: three sliders (H, S, V) + preview swatch
- Store as `Color` or provide HSV/RGB conversion utilities

**Effort:** ~500+ lines for a full picker. 3+ sessions. Could start with a "three sliders" MVP.

---

#### 11. Indeterminate Checkbox

**What:** A checkbox with three visual states: checked (✓), unchecked (✗), and indeterminate (—). The indeterminate state is display-only — clicking always toggles to checked/unchecked.

**Why:** "Select all" patterns where some items are selected. Common in list management UIs.

**Implementation approach:**
- Add `bool indeterminate` parameter to `checkbox()` (or a `with_indeterminate(bool)` on config)
- When indeterminate is true and the checkbox is unchecked, render a "—" indicator instead of "✗"
- Clicking an indeterminate checkbox sets it to checked (standard UX convention)
- No state change needed — indeterminate is a display hint, not a third bool state

**Effort:** ~30 lines. Trivial addition to existing checkbox.

---

#### 12. Context Menus / Popups

**What:** Right-click (or long-press) on an element to show a floating menu. Supports nested submenus.

**Why:** Standard interaction pattern for any editor-like UI. Useful for item context actions, node graph editors, etc.

**Implementation approach:**
- New component: `HasContextMenu` with a callback that builds menu content
- On right-click (or designated input action), create a floating div at the click position
- Menu items are buttons in a vertical stack
- Submenus: hovering a "submenu" item opens a child menu to the right
- Click outside or Escape closes the menu
- Needs `InputExclusivity` tag (already exists) to block clicks through the menu
- The floating menu should render at the highest render layer with absolute positioning

**Effort:** ~400 lines. 2-3 sessions. The nested submenu handling adds complexity.

---

#### 13. Password Field Mode

**What:** A text input mode that masks characters with dots/asterisks, with an optional show/hide toggle button.

**Why:** Login screens, settings with sensitive values. Small addition to existing text_input.

**Implementation approach:**
- Add `bool password_mode = false` to text input config/state
- When password mode is on, the rendering path displays `•` for each character instead of the actual text
- The underlying string is unchanged — only the display is masked
- Optional: add a "👁" toggle button beside the input (like egui's password demo) using the existing button primitive
- Cursor positioning still works on the real string; only the rendered glyphs change

**Touches:** `text_input/` rendering, `ComponentConfig` or `TextInputConfig`

**Effort:** ~50-80 lines. 1 session.

---

#### 14. Selectable Labels

**What:** A label that acts like a toggle button — clicking it selects/deselects it. Highlighted when selected.

**Why:** Tag filters, category selectors, toolbar items. egui uses `selectable_value()` and `selectable_label()` heavily as alternatives to radio buttons.

**Implementation approach:**
- New imm function: `selectable_label(ctx, ep_pair, &selected, label, config)`
- Internally: a button with a transparent background when unselected, and `Theme::Usage::Primary` when selected
- Returns `ElementResult` with `changed` true when clicked
- `selectable_value` variant: `selectable_value(ctx, ep_pair, &current, match_value, label)` — sets `current = match_value` on click, highlights when `current == match_value`
- Essentially a styled button — very simple to build from existing primitives

**Effort:** ~40 lines. Trivial.

---

#### 15. Spinner

**What:** An animated circular loading indicator.

**Why:** Async operations, loading states, "please wait" feedback. Simple but missing.

**Implementation approach:**
- New imm function: `spinner(ctx, ep_pair, config)`
- Render as a partial arc (e.g., 270° of a circle) that rotates continuously
- Use the animation system to drive rotation: `animation::anim(SpinnerKey).from(0).to(360).loop()`
- Or simpler: use frame time directly to compute rotation angle (`fmod(time * speed, 360)`)
- Rendering: use the existing circular progress ring drawing path (`HasCircularProgressState`) with a rotating start angle
- Size defaults to match text height

**Effort:** ~60 lines. 1 session.

---

#### 16. Disabled Opacity Cascade

**What:** When a widget is disabled, automatically reduce opacity for the entire subtree and skip hover color changes.

**Why:** Consistent disabled appearance without per-widget styling. egui does `ui.multiply_opacity(0.5)` for disabled scopes. afterhours has `config.disabled` but disabled elements can still look fully opaque and interactive.

**Implementation approach:**
- In `rendering.h`, when resolving colors for an entity, check if any ancestor has `config.disabled` set
- If disabled, multiply the effective opacity by 0.5 (or a configurable value)
- `HasOpacity` already exists and `compute_effective_opacity()` already walks ancestors — extend this to also check for disabled state
- Skip hover color overrides for disabled entities (check `HasColor::skip_hover_override`)
- Cursor should not change to pointer for disabled interactive elements

**Touches:** `rendering.h` opacity/color resolution, `systems.h` hover handling

**Effort:** ~40-60 lines. Small change with broad visual improvement.

---

#### 17. Scene / Pan-Zoom Container

**What:** A container that supports panning (scroll/drag) and zooming (pinch/ctrl+scroll) of its content. Content is rendered in a transformed coordinate space.

**Why:** Map editors, node-based editors, zoomable game UIs, debug visualizations. egui's Scene demo embeds the entire widget gallery in a pannable/zoomable viewport.

**Implementation approach:**
- New imm function: `scene(ctx, ep_pair, &scene_rect, config)` where `scene_rect` tracks the current view rectangle
- Internally: a clipped container with custom input handling for pan (drag) and zoom (scroll wheel)
- Content is rendered with a scale transform applied: `HasUIModifiers::scale` and `translate_x/y`
- Zoom range limits (e.g., 0.1x to 5.0x)
- Double-click to reset view
- All child widgets still work — their positions/sizes are just scaled
- This is architecturally challenging because afterhours' layout system doesn't naturally support arbitrary scale transforms on subtrees. May need a special render-time transform pass.

**Effort:** ~500+ lines. 3+ sessions. Architecturally complex — layout system needs to handle scaled coordinate spaces.

---

## Chosen Approach

Work in three waves. Each wave delivers visible improvement before moving on. All changes are in `vendor/afterhours/` since that's the UI library.

### Wave 1 — "Make it feel smooth" (3-4 sessions)

These are small changes that improve the feel of every existing widget without adding new ones.

**1a. Visibility culling in renderer** (~50 lines, `rendering.h`)

Do this first because it's the easiest win and helps with profiling the rest.

```
In the main render loop, before drawing any entity:

1. Get entity's computed rect from UIComponent
2. Get the accumulated clip rect (find_accumulated_clip_rect already exists)
3. If no intersection → skip all draw calls for this entity
4. If entity has children and its rect is fully outside → skip the subtree

Add a debug counter: "entities drawn: 47/312" visible in the debug overlay.
```

Files: `rendering.h` (render loop, ~line 337 area where the TODO about caching already lives)

**1b. Hover/focus transition animations** (~120 lines, `rendering.h` + `components.h`)

```
Add to HasColor:
    Color rendered_bg = {0,0,0,0};     // last frame's actual drawn color
    Color rendered_border = {0,0,0,0}; // last frame's actual drawn border
    bool has_rendered = false;          // false until first frame

In the render path where background color is resolved:
    Color target = resolve_hover_color(entity, theme, ...);
    if (!has_rendered) {
        rendered_bg = target;
        has_rendered = true;
    } else {
        rendered_bg = colors::lerp(rendered_bg, target, 0.18f);
        if (colors::approx_equal(rendered_bg, target)) rendered_bg = target;
    }
    // use rendered_bg instead of target for drawing
```

Same for border color. The 0.18 factor gives a ~3-frame transition at 60fps. Instant when needed (first frame, or when `skip_hover_override` is set).

Files: `components.h` (add fields to `HasColor`), `rendering.h` (color resolution path)

**1c. ID-based animation helper** (~60 lines, `context.h`)

```
Add to UIContext:
    std::unordered_map<EntityID, float> anim_values_;

    float animate_bool(EntityID id, bool target, float speed = 0.18f) {
        auto it = anim_values_.find(id);
        float current = (it != anim_values_.end()) ? it->second : (target ? 1.0f : 0.0f);
        float goal = target ? 1.0f : 0.0f;
        current += (goal - current) * speed;
        if (std::abs(current - goal) < 0.005f) current = goal;
        anim_values_[id] = current;
        return current;
    }

    // Call at end of frame to prune stale entries
    void cleanup_animations(const std::vector<Entity*>& live_entities);
```

Then refactor `toggle_switch` to use it — replace the inline `animation_progress += (target - animation_progress) * 0.2f` with `ctx.animate_bool(entity.id, state.on)`. This also makes it available for collapsing headers, future widgets, etc.

Files: `context.h`

**1d. Disabled opacity cascade** (~40 lines, `rendering.h`)

```
In compute_effective_opacity(), after checking HasOpacity, also check:
    if (cur.has<UIComponent>() && cur.get<UIComponent>().disabled) {
        result *= 0.45f;
    }

In the hover system (systems.h), skip hover color changes for disabled entities.
Set cursor to Default (not Pointer) for disabled interactive elements.
```

Files: `rendering.h` (`compute_effective_opacity`), `systems.h` (hover handling)

---

### Wave 2 — "Add the missing essentials" (5-6 sessions)

New widgets and the big performance feature.

**2a. Tooltips** (~200 lines)

New component in `components.h`:
```cpp
struct HasTooltip : BaseComponent {
    std::string text;
    float delay = 0.4f;        // seconds before showing
    float hover_time = 0.0f;   // accumulated hover time
    bool showing = false;
};
```

New method on `ElementResult`:
```cpp
ElementResult& on_hover_text(const std::string& text) {
    entity_ref.addComponentIfMissing<HasTooltip>().text = text;
    return *this;
}
```

New render pass at the end of `rendering.h` (after all other rendering):
```
1. Find the hot entity (ctx.hot_id)
2. If it has HasTooltip and hover_time > delay:
   a. Measure text width/height
   b. Position a rect near the mouse (offset by 16px down-right)
   c. Clamp to screen bounds (flip if near edge)
   d. Draw background rect (theme.surface with border)
   e. Draw text
3. If hot entity changed, reset hover_time on the old one
```

Update `systems.h` to accumulate `hover_time` each frame when entity is hot, reset when not.

Files: `components.h`, `element_result.h`, `rendering.h`, `systems.h`

**2b. CollapsingHeader** (~180 lines)

New component:
```cpp
struct HasCollapsingState : BaseComponent {
    bool open = false;
    float anim_progress = 0.0f; // 0 = collapsed, 1 = expanded
};
```

New imm function:
```cpp
ElementResult collapsing_header(ctx, ep_pair, label, &open, config) {
    // Create header button with ▶/▼ indicator
    auto header = button(ctx, mk(entity),
        config.with_label(fmt::format("{} {}", open ? "▼" : "▶", label))
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Left));

    if (header) open = !open;

    // Animate open/close
    float t = ctx.animate_bool(entity.id, open);

    // Body container — only create children when t > 0
    if (t > 0.0f) {
        auto body = div(ctx, mk(entity),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), children()})
                .with_opacity(t));

        // Clip height: multiply computed height by t
        // This requires setting max-height or using scale
        body.ent().addComponentIfMissing<HasOpacity>(t);

        return {header.changed(), body.ent(), open};
    }

    return {header.changed(), entity, open};
}
```

For the collapse animation, the simplest approach is opacity fade. A height animation (slide) is nicer but requires the layout system to support animated max-height constraints — do opacity first, upgrade to height later.

Files: `imm_components.h`, `components.h`

**2c. Virtual scrolling** (~350 lines)

New imm function:
```cpp
template<typename Callback>
ElementResult virtual_scroll(
    HasUIContext auto& ctx, EntityParent ep_pair,
    size_t item_count, float item_height,
    Callback&& build_item,   // void(ctx, parent, size_t index)
    ComponentConfig config = ComponentConfig()
) {
    auto [entity, parent] = deref(ep_pair);

    // Create scroll container
    config.with_overflow_y(Overflow::Scroll);
    init_component(ctx, ep_pair, config, ComponentType::Div, false, "virtual_scroll");

    auto& scroll = entity.addComponentIfMissing<HasScrollView>();

    // Compute visible range
    float viewport_h = entity.get<UIComponent>().computed[Axis::Y];
    if (viewport_h <= 0) viewport_h = 600.0f; // first frame fallback

    float total_h = item_count * item_height;
    scroll.content_size.y = total_h;
    scroll.viewport_size.y = viewport_h;

    size_t first = static_cast<size_t>(scroll.scroll_offset.y / item_height);
    size_t visible_count = static_cast<size_t>(viewport_h / item_height) + 2;
    size_t last = std::min(first + visible_count, item_count);

    // Top spacer — pushes visible items to correct position
    if (first > 0) {
        div(ctx, mk(entity, -1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(first * item_height)})
                .with_background(Theme::Usage::None)
                .with_skip_tabbing(true));
    }

    // Build only visible items
    for (size_t i = first; i < last; ++i) {
        build_item(ctx, mk(entity, static_cast<int>(i)), i);
    }

    // Bottom spacer — maintains total scroll height
    float bottom_space = (item_count - last) * item_height;
    if (bottom_space > 0) {
        div(ctx, mk(entity, -2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(bottom_space)})
                .with_background(Theme::Usage::None)
                .with_skip_tabbing(true));
    }

    return {false, entity};
}
```

Usage:
```cpp
virtual_scroll(ctx, mk(parent), 10000, 32.0f,
    [](auto& ctx, auto ep, size_t i) {
        div(ctx, ep, ComponentConfig{}.with_label(fmt::format("Row {}", i)));
    },
    ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(400)}));
```

Files: `imm_components.h`, minor `HasScrollView` additions

**2d. Selectable labels** (~40 lines)

```cpp
ElementResult selectable_label(HasUIContext auto& ctx, EntityParent ep_pair,
                                bool selected, const std::string& label,
                                ComponentConfig config = ComponentConfig()) {
    auto bg = selected ? Theme::Usage::Primary : Theme::Usage::None;
    config.with_label(label).with_background(bg);
    return button(ctx, ep_pair, config);
}

template<typename T>
ElementResult selectable_value(HasUIContext auto& ctx, EntityParent ep_pair,
                                T& current, T value, const std::string& label,
                                ComponentConfig config = ComponentConfig()) {
    bool selected = (current == value);
    auto result = selectable_label(ctx, ep_pair, selected, label, config);
    if (result) current = value;
    return result;
}
```

Files: `imm_components.h`

**2e. Indeterminate checkbox** (~30 lines)

Add to `ComponentConfig`:
```cpp
bool checkbox_indeterminate = false;
auto& with_indeterminate(bool v) { checkbox_indeterminate = v; return *this; }
```

In `checkbox()`, when resolving the indicator label:
```cpp
if (config.checkbox_indeterminate && !state.on) {
    toggle_config.label = "—";  // dash instead of X
} else {
    toggle_config.label = state.on ? checked_indicator : unchecked_indicator;
}
```

Files: `component_config.h`, `imm_components.h`

**2f. Spinner** (~70 lines)

```cpp
ElementResult spinner(HasUIContext auto& ctx, EntityParent ep_pair,
                       ComponentConfig config = ComponentConfig()) {
    auto [entity, parent] = deref(ep_pair);

    if (config.size.is_default) {
        Size sz = h720(24.0f);
        config.with_size(ComponentSize{sz, sz});
    }

    init_component(ctx, ep_pair, config, ComponentType::CircularProgress,
                   false, "spinner");

    // Use frame time for continuous rotation
    float time = /* get elapsed time from input or context */;
    float rotation_speed = 2.0f; // rotations per second
    float angle_degrees = std::fmod(time * rotation_speed * 360.0f, 360.0f);

    // Render as a partial arc (270° of a circle) at the current rotation angle
    auto& state = entity.addComponentIfMissing<HasCircularProgressState>(
        0.75f, 4.0f);
    state.set_value(0.75f)
         .set_thickness(4.0f)
         .set_start_angle(degrees(angle_degrees))
         .set_fill_color(ctx.theme.accent)
         .set_track_color(Color{0, 0, 0, 0}); // invisible track

    entity.removeComponentIfExists<HasColor>();
    entity.removeComponentIfExists<HasBorder>();

    return {false, entity};
}
```

Files: `imm_components.h`

---

### Wave 3 — "Power features" (6+ sessions, pick as needed)

These are larger features to tackle based on what the project actually needs.

**3a. DragValue** (~300 lines)

```
States:
  - Display mode: shows "42.0°", cursor is ResizeH on hover
  - Dragging mode: mouse delta * speed adjusts value
  - Edit mode: click (without drag) opens text_input, Enter/Escape commits/cancels

Components needed:
  - HasDragValueState { float value; bool editing; float drag_start_value; }
  - DragValueConfig or extend ComponentConfig with:
    - drag_speed (float, default 1.0)
    - range (optional min/max)
    - suffix/prefix (string)
    - step (optional snap increment)

Interaction flow:
  1. MouseDown → record start value, enter drag mode
  2. MouseMove while down → value += delta.x * speed
  3. MouseUp without significant movement → enter edit mode (text_input)
  4. MouseUp after drag → commit drag value
  5. In edit mode: Enter → parse text, commit. Escape → cancel, restore.

Rendering:
  - Display mode: div with formatted label ("42.0°"), subtle underline
  - Edit mode: text_input with the current value as initial text
  - On hover: cursor changes to ResizeH, slight bg highlight
```

Files: `imm_components.h`, `components.h`, `component_config.h`

**3b. Slider improvements** (~400 lines, incremental)

Each feature is independent and can be merged separately:

```
Step size:
  - Add with_slider_step(float) to config
  - In apply_slider_value: round to nearest step
  - In HasLeftRightListener: use step instead of hardcoded 0.01

Suffix/prefix:
  - Add with_slider_suffix(string) to config
  - In label rendering: append suffix to formatted value

Trailing fill:
  - Add a fill div between track left edge and handle
  - Absolute positioned, width = handle_left_position
  - Color: theme.accent with 0.6 opacity

Click-to-edit:
  - Add HasSliderEditMode state
  - Click on value label → replace with text_input
  - Enter/Escape → parse/cancel, return to slider mode
  - Similar to DragValue edit mode

Logarithmic:
  - Add with_slider_logarithmic(bool) to config
  - Map slider position through log/exp:
    position = log(value - min) / log(max - min)
    value = min + exp(position * log(max - min))
  - Handle zero/negative values (offset to avoid log(0))

Smart aim:
  - When dragging, detect "round" numbers near current value
  - If within a dead zone (e.g., 3% of range), snap to the round number
  - Round numbers: powers of 10, halves, quarters
  - egui's implementation is ~50 lines of logic

Vertical orientation:
  - Swap x/y in layout and drag handling
  - Rotate the handle's margin axis
  - Swap drag listener from delta.x to delta.y
```

**3c. Context menus** (~400 lines)

```
API:
  button(ctx, mk(parent), config.with_label("Item"))
      .on_context_menu([](auto& ctx, auto parent) {
          button(ctx, mk(parent), config.with_label("Edit"));
          button(ctx, mk(parent), config.with_label("Delete"));
          separator(ctx, mk(parent));
          button(ctx, mk(parent), config.with_label("Properties"));
      });

Components:
  - HasContextMenu { callback; bool open; Vector2Type position; }

System:
  - On right-click (or designated action) on entity with HasContextMenu:
    set open=true, record click position
  - When open: create a floating div at the recorded position,
    call the callback to populate it with menu items
  - Click outside or Escape → close
  - InputExclusivity tag blocks clicks through the menu
  - Nested submenus: menu items can themselves have HasContextMenu
    that opens on hover (with a delay)
```

**3d. Password field** (~60 lines)

```
Add to text input config:
  bool password_mode = false;
  bool password_visible = false;

In text rendering (text_input/text_layout.h):
  if (password_mode && !password_visible) {
      render "•" repeated text.length() times instead of actual text
  }

In text_input() or as a wrapper:
  password_input(ctx, ep_pair, &text, config) {
      auto row = hstack(ctx, ep_pair, ...);
      text_input(ctx, mk(row.ent()), text, config.with_password(true));
      auto toggle = button(ctx, mk(row.ent()), config.with_label("👁"));
      if (toggle) config.password_visible = !config.password_visible;
  }
```

**3e. Grid layout** (~500 lines)

```
This is the most architecturally complex item.

Approach A — Post-layout fixup:
  1. grid() creates a normal div with flex-wrap
  2. After layout, a system finds all grid containers and:
     - Groups children into rows of N
     - For each column, finds max width across all rows
     - Adjusts each child's computed width to match its column's max
  3. Requires a second layout pass or a fixup system

Approach B — Two-pass layout:
  1. First frame: create all children, measure natural sizes
  2. Second frame: apply column-aligned sizes
  3. Flickers on first frame unless we hide until measured

Approach C — Manual column widths:
  1. User specifies column widths (fixed pixels or proportions)
  2. grid() creates rows of hstacks with matching column sizes
  3. No auto-sizing, but works in single pass
  4. Start here — simplest and covers 80% of use cases

Striping:
  - Even rows get theme.surface background
  - Odd rows get theme.background
  - A flag on the grid config enables this
```

**3f. Color picker** (~500+ lines)

```
Build as a composite of existing widgets:

Simple version (3 sliders + swatch):
  - H slider (0-360, with rainbow gradient track)
  - S slider (0-100%)
  - V slider (0-100%)
  - A slider (0-100%, optional)
  - Preview swatch (div with the selected color as background)
  - Hex input (text_input that parses/formats #RRGGBB)

Full version (adds):
  - SV square: a 2D area where X=saturation, Y=value
    Requires custom rendering of a gradient quad
  - Hue bar: vertical rainbow strip
  - Color history (recent picks)

Utility functions needed:
  - RGB ↔ HSV conversion (add to color.h)
  - Color formatting (#RRGGBB string ↔ Color)
```

**3g. Scene / pan-zoom** (~500+ lines)

```
Most complex feature. Requires layout system support for scale transforms.

Simpler approach — render-time-only transform:
  1. scene() creates a clipped div
  2. Stores pan offset (x,y) and zoom level
  3. Scroll → adjust pan. Ctrl+scroll → adjust zoom.
  4. Before rendering children, push a transform matrix:
     translate(pan_x, pan_y) * scale(zoom)
  5. After rendering children, pop transform
  6. Input coordinates need inverse-transforming for hit testing

This works for rendering but breaks layout:
  - Children are laid out at 1:1 scale, then drawn scaled
  - Text sizes, padding, margins don't scale with zoom
  - Fine for "zoom a canvas" but not "zoom the whole UI"

For game use cases (map editor, node graph), the render-time
approach is usually sufficient since the "scene content" is
custom-drawn, not standard UI widgets.
```

---

### Implementation Order Summary

```
Wave 1 (feel):
  1a. Visibility culling         ~50 lines    0.5 session
  1b. Hover transition anims     ~120 lines   1 session
  1c. ID-based anim helper       ~60 lines    0.5 session
  1d. Disabled opacity cascade   ~40 lines    0.5 session
                                 ─────────    ──────────
                                 ~270 lines   2.5 sessions

Wave 2 (essentials):
  2a. Tooltips                   ~200 lines   2 sessions
  2b. CollapsingHeader           ~180 lines   1.5 sessions
  2c. Virtual scrolling          ~350 lines   2.5 sessions
  2d. Selectable labels          ~40 lines    0.5 session
  2e. Indeterminate checkbox     ~30 lines    0.5 session
  2f. Spinner                    ~70 lines    0.5 session
                                 ─────────    ──────────
                                 ~870 lines   7 sessions

Wave 3 (power, pick as needed):
  3a. DragValue                  ~300 lines   2 sessions
  3b. Slider improvements        ~400 lines   3 sessions
  3c. Context menus              ~400 lines   3 sessions
  3d. Password field             ~60 lines    0.5 session
  3e. Grid layout                ~500 lines   3+ sessions
  3f. Color picker               ~500+ lines  3+ sessions
  3g. Scene / pan-zoom           ~500+ lines  3+ sessions
                                 ──────────   ──────────
                                 ~2660 lines  17+ sessions

Total across all waves: ~3800 lines, ~26 sessions
```

Wave 1 should be done first regardless — it improves every existing widget for ~270 lines of code. Wave 2 adds the most-requested missing pieces. Wave 3 is a menu to pick from based on what the game actually needs.
