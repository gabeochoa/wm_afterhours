# egui UI Library Comparison

Analysis of [egui](https://github.com/emilk/egui) demo library
(`crates/egui_demo_lib/src/demo/`) and what afterhours can learn from it.

## egui Component Inventory

### Primitives (built-in to `egui::Ui`)

| Widget | Description |
|---|---|
| Label | Static text display |
| Hyperlink | Clickable URL link |
| Button | Standard click button |
| Link | Text-styled clickable link |
| ImageButton | Button with image |
| Checkbox | Boolean toggle with label |
| RadioButton | Exclusive selection |
| SelectableLabel | Toggle-style selectable text |
| TextEdit | Single-line and multi-line text input |
| Slider | Value slider (horizontal/vertical, logarithmic, stepped) |
| DragValue | Drag-to-adjust numeric input |
| ProgressBar | Animated progress display |
| Spinner | Loading indicator |
| Separator | Visual divider |
| Image | Texture display |
| ComboBox | Dropdown select |
| ColorPicker | Color selection widget |
| CollapsingHeader | Accordion/tree node |
| RichText | Styled text (size, color, font) |

### Layout Containers

| Container | Description |
|---|---|
| `horizontal` / `vertical` | Basic directional layout |
| `horizontal_wrapped` | Wrapping horizontal flow |
| `columns` | Equal-width column split |
| `Grid` | Auto-layout grid with striped rows, column count |
| `ScrollArea` | Scrollable region (vertical, horizontal, both) with virtualization |
| `Panel` | Resizable docking panels (top, bottom, left, right, central) |
| `Window` | Floating, resizable, draggable windows |
| `Frame` | Visual container with margin, padding, shadow, rounding, fill, stroke |
| `Resize` | Manually resizable region |
| `Modal` | Backdrop overlay with focus trapping |
| `Popup` | Anchored popup with alignment control |
| `Tooltip` | Hover-triggered popup |
| `Sides` | Left/right justified content within a row |

### Extras (`egui_extras` crate)

| Widget | Description |
|---|---|
| `TableBuilder` | Virtualized table with resizable columns, headers, row selection, sorting |
| `StripBuilder` | Flexbox-like layout strips with exact/remainder/relative sizing |
| `DatePickerButton` | Calendar date picker |

### Demo-only Custom Widgets

| Widget | Description |
|---|---|
| Toggle switch | iOS-style animated toggle (~15 lines) |
| Password field | Text input with show/hide toggle using per-widget state |

---

## Afterhours Component Inventory (for reference)

### Primitives

div, hstack, vstack, spacer, separator, button, image, sprite, image_button,
circular_progress

### Composites

icon_row, button_group, checkbox, checkbox_group, radio_group, toggle_switch,
slider, stepper, pagination, dropdown, navigation_bar, tab_container,
progress_bar, decorative_frame, setting_row, tree_view

---

## How egui Showcases Its Demos (egui.rs)

The [egui.rs](https://www.egui.rs/) website is not a documentation site with
screenshots -- it's the actual egui demo app compiled to WebAssembly and running
inside a `<canvas>` element. The entire page is one HTML file that loads
`egui_demo_app.js` + `egui_demo_app_bg.wasm`. Every widget is live and
interactive.

### Architecture

The demo app (`WrapApp`) orchestrates everything:

1. **Top tab bar** — Selectable labels switch between major demo apps: Demos,
   EasyMark editor, HTTP, Fractal Clock, 3D painting, Rendering test. Each is a
   separate `DemoApp` implementation. A global theme switch (dark/light) is the
   first item.

2. **Right-side demo index panel** — A fixed 160px `Panel::right` listing every
   demo as a toggle button. Clicking toggles the demo's window open/closed.
   Demos are grouped: About, then a separator, then all demos (Bézier Curve
   through Window Options), then another separator, then test demos (Clipboard
   Test through Window Resize Test). An "Organize windows" button at the bottom
   auto-arranges all open windows.

3. **Floating window system** — Each demo opens in its own draggable, resizable
   `Window`. Multiple demos can be open simultaneously for side-by-side
   comparison. Window state (position, size, open/closed) is tracked in a
   `BTreeSet<String>`.

4. **Three default windows on load** — About egui (explains what it is), Code
   Example (shows source alongside rendered widgets), Widget Gallery (every
   widget type in one panel). This immediately answers: what is this? how do I
   use it? what can it do?

### Key Showcase Patterns

**Widget Gallery** — A single `Grid` with two columns (label | widget) showing
every built-in widget. Includes `Visible`, `Interactive`, and `Opacity` controls
at the bottom for testing disabled/hidden/semi-transparent states. Striped rows
for readability.

**Code Example** — Interleaves source code strings with rendered widgets in a
Grid. The code IS the documentation:

```rust
show_code(ui, r#"ui.heading("Example");"#);  // display as syntax-highlighted text
ui.heading("Example");                         // render the actual widget
ui.end_row();
```

Every line of code shown is also executed, so the output always matches.

**Demo/View trait separation** — Two traits keep content reusable:

```rust
trait Demo {
    fn name(&self) -> &'static str;
    fn show(&mut self, ui: &mut Ui, open: &mut bool);  // wraps in Window
}
trait View {
    fn ui(&mut self, ui: &mut Ui);  // pure content, no container
}
```

The same `View::ui` works inside a floating window, a resizable panel, a test
harness, or the mobile layout. Demos implement both; tests only call `View::ui`.

**Automatic snapshot testing** — `demos_should_match_snapshot()` iterates every
registered demo, renders it into a 2048x2048 harness, auto-sizes to fit, and
snapshots. Adding a new demo automatically gets it tested.

**Mobile-responsive layout** — `is_mobile(ctx)` switches from sidebar + windows
to a hamburger menu + full-screen about page with a "Continue to the demo!"
button.

**State persistence** — All demo state (widget values, window positions,
open/closed set) is serializable via serde and persists across page reloads.

**Source code links** — Every demo ends with `egui_github_link_file!()`, a macro
that generates a clickable link to the file on GitHub. One line per demo.

### Comparison with Afterhours

| Aspect | egui.rs | afterhours |
|---|---|---|
| Platform | WebAssembly in browser | Native C++/OpenGL, CLI-driven |
| Navigation | Toggle buttons in sidebar, multiple open | `--screen=<name>`, `,`/`.` keys, one at a time |
| Simultaneous views | Multiple windows open at once | One screen active at a time |
| Code alongside UI | Grid interleaving code + widget | Not present |
| Widget gallery | Single panel with every widget | Separate showcase per widget type |
| State controls | Visible/Interactive/Opacity toggles | Not present |
| Snapshot testing | Auto-iterates all demos | Headless screenshots + e2e scripts |
| Mobile layout | Auto-detects, different UI | Not applicable (native app) |
| State persistence | serde serialization | Not present for demo state |
| Categories | Flat list with separators | Structured (Game Mockups, Component Galleries, etc.) |
| Screen count | ~30 demos + ~11 tests | 77 screens |

### Ideas for Afterhours

1. **Unified Widget Gallery screen** — A single showcase screen rendering every
   `imm::` primitive and composite in a grid layout (label | widget), similar
   to egui's Widget Gallery. Currently each widget has its own showcase screen,
   which is great for detail but makes it hard to see everything at a glance.

2. **Code snippets in showcases** — Show the `ComponentConfig` builder code next
   to each rendered widget. For immediate-mode components, display the `imm::`
   call. This makes showcases self-documenting.

3. **Widget state controls** — Add visible/disabled/opacity toggles to showcase
   screens so developers can test how components look in different states without
   writing separate screens.

4. **Web demo via Emscripten** — The afterhours renderer uses OpenGL, which can
   compile to WebGL via Emscripten. A live web demo would be a powerful
   marketing/documentation tool. (Effort: high, but high impact.)

5. **Auto-snapshot all screens** — The `run_headless_screenshots()` function
   already exists. Making it part of CI with per-screen regression comparison
   would match egui's testing thoroughness.

6. **Source code links in HUD** — Show the file path of the current showcase
   screen in the HUD (e.g., `src/systems/screens/CheckboxShowcase.h`), making
   it easy to find and modify the source.

---

## What egui Does Well

### 1. Virtualized Scrolling

egui's `ScrollArea` supports three modes, from simple to fully manual:

```rust
// Simple: lay out everything (slow for big lists)
ScrollArea::vertical().show(ui, |ui| { ... });

// Uniform rows: only lay out visible rows (O(visible) not O(total))
ScrollArea::vertical().show_rows(ui, row_height, 10_000, |ui, row_range| {
    for row in row_range {
        ui.label(format!("Row {row}"));
    }
});

// Full manual: get viewport rect, paint only visible items
ScrollArea::vertical().show_viewport(ui, |ui, viewport| {
    ui.set_height(row_height * num_rows as f32);
    let first = (viewport.min.y / row_height).floor() as usize;
    let last = (viewport.max.y / row_height).ceil() as usize;
    // only paint first..last
});
```

The table demo also supports heterogeneous row heights via
`body.heterogeneous_rows()`.

**Gap in afterhours:** All scroll children are laid out every frame regardless
of visibility. A `show_rows`-style API for uniform-height lists would be a
significant performance win for long scrollable content.

### 2. Scroll-To API

egui provides rich programmatic scroll control:

- `scroll_to_row(index, alignment)` — jump to a specific row
- `scroll_to_cursor(Align::TOP | BOTTOM)` — jump to top/bottom
- `scroll_with_delta(Vec2)` — scroll by pixel amount
- `stick_to_bottom(true)` — auto-scroll to bottom as content grows (chat/log)
- `vertical_scroll_offset(pixels)` — set exact offset

**Gap in afterhours:** No programmatic scroll-to API. Building chat windows,
log viewers, or "jump to item" UIs requires manual offset management.

### 3. Built-in Animation Interpolation

egui tracks boolean state transitions per-widget and provides a smooth 0.0–1.0
interpolant:

```rust
let how_on = ui.ctx().animate_bool_responsive(response.id, *on);
let circle_x = egui::lerp(left..=right, how_on);
```

One line gives smooth animated transitions for any widget. The toggle switch
demo uses this for the sliding circle.

**Afterhours comparison:** Has a more comprehensive animation system
(`animation_config.h`, spring/declarative/looping), but lacks an equivalent
one-liner for simple state transitions.

### 4. Response Chaining / Tooltip System

Every egui widget returns a `Response` that supports chained decoration:

```rust
ui.label("Hover me")
    .on_hover_text("Simple tooltip")
    .on_hover_ui(|ui| {
        ui.label("Rich tooltip with widgets");
        ui.hyperlink("https://example.com");
    })
    .on_disabled_hover_text("Different tooltip when disabled");
```

Tooltips can be interactive (selectable text, links), positioned at the cursor,
and work on any widget without modifying the widget itself.

**Gap in afterhours:** No per-widget tooltip support. Adding tooltips requires
building custom hover detection and overlay logic.

### 5. Demo / View Trait Separation

egui separates window framing from content:

```rust
trait Demo {
    fn name(&self) -> &'static str;
    fn show(&mut self, ui: &mut Ui, open: &mut bool);
}

trait View {
    fn ui(&mut self, ui: &mut Ui);
}
```

The same `View::ui` implementation works inside a window, a panel, or a test
harness. This makes snapshot testing trivial.

**Afterhours comparison:** Uses `ScreenSystem` which couples the screen
lifecycle to the ECS system. More powerful for game UIs, but harder to reuse
widget code across different containers.

### 6. Visual Snapshot Testing

egui has `egui_kittest` for automated visual regression testing:

```rust
let mut harness = Harness::builder()
    .with_pixels_per_point(2.0)
    .with_theme(egui::Theme::Dark)
    .with_size(Vec2::new(380.0, 550.0))
    .build_ui(|ui| { demo.ui(ui); });

harness.fit_contents();
harness.snapshot("widget_gallery_dark_x2");
```

Tests cover multiple themes (light/dark), pixel densities (1x/2x), and
multi-step interaction sequences (click -> snapshot -> click -> snapshot).

**Afterhours comparison:** Has e2e tests with baselines, but the egui approach
is more structured — every demo has a snapshot test, and interaction sequences
are tested as part of the visual regression suite.

### 7. Popup Positioning / Alignment

The `Popup` system has precise anchor control:

```rust
Popup::menu(&response)
    .align(RectAlign::BOTTOM)
    .gap(4.0)
    .close_behavior(PopupCloseBehavior::CloseOnClickOutside)
    .show(|ui| { /* popup content */ });
```

`RectAlign` combines a parent anchor point with a child anchor point, giving
fine control over where the popup appears relative to its trigger.

**Gap in afterhours:** Dropdown positioning is handled internally. No
general-purpose popup positioning API for custom overlays.

### 8. Resizable Panels (Docking Layout)

```rust
Panel::left("nav").resizable(true).size_range(80.0..=200.0).show_inside(ui, |ui| { ... });
Panel::right("props").resizable(true).default_size(150.0).show_inside(ui, |ui| { ... });
CentralPanel::default().show_inside(ui, |ui| { ... });
```

Panels claim space in order of declaration. Each can be resized with drag
handles. This gives IDE-style split-pane layouts.

**Gap in afterhours:** No resizable panel splitting. Flexbox layout handles
proportional sizing but not user-driven resize.

### 9. Custom Widget Protocol

egui's custom widget recipe is minimal:

```rust
fn toggle_ui(ui: &mut egui::Ui, on: &mut bool) -> egui::Response {
    let desired_size = ui.spacing().interact_size.y * egui::vec2(2.0, 1.0);
    let (rect, mut response) = ui.allocate_exact_size(desired_size, egui::Sense::click());
    if response.clicked() { *on = !*on; response.mark_changed(); }
    if ui.is_rect_visible(rect) {
        let how_on = ui.ctx().animate_bool_responsive(response.id, *on);
        let visuals = ui.style().interact_selectable(&response, *on);
        // ... paint with rect, visuals, how_on ...
    }
    response
}

// Wrapper for ui.add(toggle(&mut val)) syntax
fn toggle(on: &mut bool) -> impl egui::Widget + '_ {
    move |ui: &mut egui::Ui| toggle_ui(ui, on)
}
```

Four steps: decide size, allocate space, handle interaction, paint. The
visibility check (`is_rect_visible`) is a free optimization — off-screen
widgets skip painting entirely.

**Afterhours comparison:** Custom widgets use `init_component()` + ECS entity
creation, which is more powerful but more boilerplate. The visibility-gated
paint pattern is something afterhours could adopt in its rendering pass.

### 10. Undo/Redo Utility

```rust
let undoer: Undoer<MyState> = Undoer::default();
undoer.feed_state(time, &current_state);
if let Some(prev) = undoer.undo(&current_state) { current_state = prev.clone(); }
if let Some(next) = undoer.redo(&current_state) { current_state = next.clone(); }
```

Generic undo/redo stack that works with any `Clone + PartialEq` state. Useful
for settings screens, editors, form flows.

**Gap in afterhours:** No built-in undo/redo utility.

---

## What Afterhours Does Better

| Area | Afterhours Advantage |
|---|---|
| Component richness | More composite widgets out of the box: stepper, pagination, navigation_bar, tab_container, decorative_frame, circular_progress, setting_row, tree_view |
| Theming | Full `Theme` struct with preset themes (`neon_dark`, etc.) and `auto_text_color` for contrast accessibility. egui has `Visuals` but no preset themes |
| Game input | Gamepad/keyboard navigation, tabbing system, controller-aware input mapping |
| Flex layout | Full CSS-like flexbox: justify-content, align-items, flex-wrap, flex-gap, self-align. egui layout is simpler (horizontal/vertical/columns) |
| Drag & drop | `ComponentConfig{}.draggable_children(true)` enables reorder. egui requires manual `dnd_drag_source`/`dnd_drop_zone` wiring |
| Showcase breadth | 77 example screens vs egui's ~30 demos |
| Adaptive scaling | Built-in `ScalingMode::Adaptive` for different screen sizes |
| Validation | Built-in form validation system (`validation_config.h`, `validation_systems.h`) |
| Text input | Full text area with selection, line indexing, multi-line support |

---

## Feature Gap Summary (Prioritized)

| Feature | egui Has It | Impact | Effort |
|---|---|---|---|
| Virtualized scroll (`show_rows`) | Yes | High — fixes perf for long lists | Medium |
| Scroll-to API | Yes | Medium — enables chat/log/jump-to | Low-Medium |
| Stick-to-bottom scroll | Yes | Medium — chat/log pattern | Low |
| Per-widget tooltips | Yes | Medium — composability | Medium |
| Collapsing/accordion sections | Yes | Medium — settings/tree UIs | Low |
| Grid layout (auto-columns, striped) | Yes | Medium — data display | Medium |
| Resizable panels | Yes | Low-Medium — editor/tool UIs | Medium |
| Popup alignment API | Yes | Low — custom overlays | Low |
| Undo/redo utility | Yes | Low — editor/forms | Low |
| Visual snapshot test framework | Yes | Low — CI quality | Medium |
