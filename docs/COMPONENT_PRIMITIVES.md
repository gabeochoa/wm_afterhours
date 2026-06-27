# Component Primitives

Consolidated reference for all UI components in the afterhours framework -- what exists, what's planned, and how they compose. This document covers the existing component inventory, new primitives needed, component configuration, and composition patterns.

---

## 1. Existing Components

These are already implemented in afterhours and should be built on, not duplicated.

### 1.1 Layout

| Component | Function | Description |
|-----------|----------|-------------|
| Container | `div()` | Flexbox container (Row/Column, justify, align, wrap) |
| Scroll View | `scroll_view()` | Scrollable container with `HasScrollView`, `HasClipChildren` |
| Separator | `separator()` | Visual divider between content sections |

### 1.2 Buttons & Toggles

| Component | Function | Description |
|-----------|----------|-------------|
| Button | `button()` | Standard clickable button |
| Button Group | `button_group()` | Row of connected buttons |
| Image Button | `image_button()` | Button with image/sprite content |
| Checkbox | `checkbox()` | Boolean toggle with check indicator |
| Radio Group | `radio_group()` | Mutually exclusive option selection |
| Toggle Switch | `toggle_switch()` | On/off sliding toggle |

### 1.3 Input

| Component | Function | Description |
|-----------|----------|-------------|
| Text Input | `text_input()` | Single-line text entry |
| Text Area | `text_area()` | Multi-line text entry |
| Slider | `slider()` | Value selection via draggable handle |
| Dropdown | `dropdown()` | Select from a list of options |

### 1.4 Navigation

| Component | Function | Description |
|-----------|----------|-------------|
| Tab Container | `tab_container()` | Tabbed content switcher |
| Navigation Bar | `navigation_bar()` | Top-level navigation |
| Pagination | `pagination()` | Page navigation controls |

### 1.5 Display

| Component | Function | Description |
|-----------|----------|-------------|
| Progress Bar | `progress_bar()` | Linear progress indicator |
| Circular Progress | `circular_progress()` | Radial progress indicator |
| Image | `image()` | Image display |
| Sprite | `sprite()` | Sprite sheet frame display |
| Icon Row | `icon_row()` | Horizontal row of icons |

### 1.6 Composite

| Component | Function | Description |
|-----------|----------|-------------|
| Setting Row | `setting_row()` | Label + control layout for settings screens |

### 1.7 Systems

| System | Description |
|--------|-------------|
| Modal Plugin | Stacking modals, focus trapping, dialog results |
| Toast Plugin | Auto-dismiss notifications with severity levels |
| Animations | Declarative per-component (on_click, on_appear, on_hover, loop) |
| Theming | `Theme`, `ComponentConfig`, color utilities |

### 1.8 Interaction Components

| Component | Description |
|-----------|-------------|
| `HasClickListener` | Click/tap event handling |
| `HasDragListener` | Drag gesture handling |
| `HasLeftRightListener` | Left/right arrow key input |

---

## 2. Planned Components

### 2.1 Button Variants

**Status:** Partially implemented (basic button exists, variants not yet)

```cpp
enum struct ButtonVariant { Filled, Outline, Ghost, Icon };

ComponentConfig{}.with_button_variant(ButtonVariant::Outline);
```

| Variant | Appearance | Use |
|---------|-----------|-----|
| Filled | Solid background, contrast text | Primary actions |
| Outline | Transparent background, colored border | Secondary actions |
| Ghost | No background until hover | Tertiary/inline actions |
| Icon | Icon-only (requires text label via tooltip) | Compact toolbar actions |

**Icon + Text Buttons:**

```cpp
ComponentConfig{}
    .with_label("Settings")
    .with_icon(gear_texture, gear_rect)
    .with_icon_position(IconPosition::Left);  // Left, Right, Top, Bottom
```

### 2.2 Stepper / Selector

**Status:** Not implemented (reimplemented per-screen currently)

The `< value >` pattern with left/right arrows to cycle through discrete options.

```cpp
// String options
stepper(ctx, ep, options, option_index, config);

// Numeric variants
stepper_int(ctx, ep, value, min, max, step, config);
stepper_float(ctx, ep, value, min, max, step, config);
```

**Behavior:**
- [-] and [+] buttons on sides, current value in center
- Long-press for rapid increment
- Keyboard: left/right arrows when focused
- Clamps at min/max

### 2.3 Tooltip

**Status:** Not implemented

```cpp
ComponentConfig{}.with_tooltip("Help text here");
```

**Behavior:**
- 0.5s hover delay before showing
- Position near cursor or element, auto-flip at screen edges
- Auto-hide on mouse move away
- Shows on keyboard focus (not just hover)
- Escape dismisses

### 2.4 Popover

**Status:** Not implemented

Click-triggered interactive floating panel anchored to a trigger element.

```cpp
bool begin_popover(ctx, ep, trigger_entity, is_open, PopoverConfig{});
// ... interactive content (buttons, forms) ...
end_popover();
```

**Behavior:**
- Positioned relative to trigger, auto-flip at edges
- Optional arrow pointing to trigger
- Close on outside click or Escape
- Focus trap within popover content
- Keyboard navigation

| | Tooltip | Popover |
|---|---------|---------|
| Trigger | Hover (with delay) | Click |
| Content | Text (or rich text) | Interactive (buttons, forms) |
| Dismissal | Mouse leaves | Outside click / Escape |
| Focus | No focus trap | Focus trapped |

### 2.5 Context Menu / Dropdown Menu

**Status:** Not implemented

Shared `MenuItem` structure for both right-click context menus and click-triggered dropdown menus.

```cpp
struct MenuItem {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<std::string> shortcut;  // "Ctrl+C"
    std::function<void()> on_select;
    bool disabled = false;
    bool is_separator = false;
    std::vector<MenuItem> submenu;
};
```

**Context Menu:** Right-click triggered, positioned at cursor.
**Dropdown Menu:** Click triggered, positioned below trigger button.

**Shared behavior:**
- Click-outside-to-close
- Keyboard navigation (arrows, Enter, Escape)
- Nested submenus (hover or arrow-key to open)
- Dividers, icons, disabled items, shortcut display

### 2.6 Accordion

**Status:** Not implemented

Vertical stack of collapsible sections with headers.

```cpp
accordion(ctx, ep, AccordionConfig{ .allow_multiple_open = false });
accordion_item(ctx, ep, "Graphics Settings", is_expanded, config);
```

**Behavior:**
- Click header or chevron to expand/collapse
- Animated height transition (configurable)
- Optional single-open mode (opening one closes others)
- Keyboard: Enter/Space to toggle, arrows between headers

### 2.7 Combobox / Autocomplete

**Status:** Not implemented

Text input + filtered dropdown list.

```cpp
combobox(ctx, ep, options, selected_value, search_text, config);
```

**Behavior:**
- Text input for filtering, filtered dropdown list
- Highlight matching text, clear button
- Empty state when no matches

### 2.8 Segmented Control

**Status:** Not implemented

Connected button group for switching between options (hybrid of button group, radio, and tabs).

```cpp
segmented_control(ctx, ep, options, selected_value, config);
```

**Behavior:**
- Connected button appearance with active segment highlight
- Slide animation between segments
- Icon-only mode supported

### 2.9 Rating

**Status:** Not implemented

Star rating display and input.

```cpp
rating(ctx, ep, value, RatingConfig{ .max_stars = 5, .half_stars = true });
rating_display(ctx, ep, value, config);  // Read-only
```

### 2.10 Form Helpers

**Status:** Not implemented

```cpp
// Fieldset -- groups related form fields with a legend
fieldset(ctx, ep, "Audio Settings", config);

// Form Label -- accessible label with click-to-focus
form_label(ctx, ep, "Email", for_input_entity, config);
```

---

## 3. Structural Primitives

Higher-level building blocks for complex layouts, primarily needed for editor/tooling UIs.

### 3.1 Draggable Divider

Resize handle between two regions. Core building block for split panes and resizable panels.

**Inputs:** Orientation, minimum sizes per side, current split position.
**Outputs:** New split position on drag.
**Behavior:** Thin bar (or invisible hit zone), resize cursor on hover, clamps to minimums.

### 3.2 Dockable Panel

Panel that docks to a parent edge with a draggable divider for resizing.

**Inputs:** Dock edge (left/right/top/bottom), initial size, min size, visible/hidden.
**Composition:** `div()` + Draggable Divider + `HasClipChildren`

### 3.3 Split Pane

Two content regions separated by a draggable divider.

**Inputs:** Orientation, initial split ratio, minimum pane size.
**Behavior:** Click-to-focus model (one pane active at a time). Nestable for N-way splits.
**Composition:** `div()` + `div()` + Draggable Divider

### 3.4 Tree Node

Collapsible node in a hierarchical tree. Domain-agnostic -- the app provides content.

**Inputs:** Depth level, expanded/collapsed, has-children flag, app-provided content.
**Outputs:** Expand/collapse toggle, selection state, drag-and-drop events.
**Behavior:** Depth-based indentation, expand/collapse arrow, multi-select (Shift/Cmd+click), draggable with drop indicators, keyboard nav (arrows to move, Enter to expand, Space to select).
**Composition:** `div()` with depth padding + `HasClickListener` + optional `HasDragListener`

### 3.5 Anchored Popup

Floating container positioned relative to an anchor point. Shared positioning engine for tooltips, popovers, context menus, autocomplete.

**Inputs:** Anchor position, preferred placement, content.
**Behavior:** Z-ordered above other content, repositions on scroll, flips placement at screen edges, optional dismiss on click-outside or Escape.
**Composition:** `div()` with absolute positioning + z-ordering + screen bounds clamping

### 3.6 Command Palette

Centered overlay with text input and fuzzy-filtered results list.

**Inputs:** List of items (label, category, shortcut, callback), trigger shortcut.
**Behavior:** Centered pop-in, fuzzy matching, category grouping, keyboard nav (Up/Down/Enter/Escape), shortcut hints.
**Composition:** Anchored Popup (centered) + `text_input()` + `scroll_view()` + filtered results

### 3.7 Tab Strip

Horizontal row of tab entities with ordering, drag-and-drop reorder, overflow handling, and active state.

**Inputs:** Tab entities (app creates with content components), active tab ID.
**Outputs:** Tab selected/closed/reorder/detach events.
**Behavior:** Click to select, close button (visible on active/hover on others), DnD reorder, dirty indicator.
**Composition:** `div()` (Row) + tab entity children with `HasClickListener` + `HasDragListener`

### 3.8 Breadcrumb Bar

Horizontal clickable segments with separators and per-segment dropdowns.

**Inputs:** Ordered segments (label, dropdown items callback).
**Behavior:** Click segment for dropdown of siblings, collapse middle into "..." on overflow.
**Composition:** `div()` (Row) + per-segment `button()` + Anchored Popup

### 3.9 Minimap

Scaled-down document rendering in a narrow vertical strip with viewport rectangle.

**Inputs:** Document content (render callback), viewport position/size.
**Behavior:** Click to jump, drag to scrub, configurable position (right edge default).
**Composition:** `div()` with custom render callback + `HasDragListener` + viewport overlay

### 3.10 Gutter

Vertical strip alongside scrollable content with per-line decoration slots.

**Inputs:** Line count, line height (must match content), slot content per line.
**Behavior:** Synced to content scroll position, auto-sizes width, click handling per slot.
**Composition:** `div()` (Column) synced to scroll + per-line `div()` rows

### 3.11 Scroll Decoration Layer

Colored marks in the scrollbar track showing positions of interest.

**Inputs:** List of marks (position fraction, color).
**Behavior:** Small colored rectangles at proportional positions, display only.
**Composition:** Extension to `scroll_view()` render pass

### 3.12 Collapsible Region

Content area that collapses to a summary line. Click or arrow to expand/collapse.

**Inputs:** Summary content, expanded content, initial state.
**Behavior:** Arrow indicator rotates, optional height animation, nestable.
**Composition:** `div()` + `HasClickListener` on summary + conditional child rendering

### 3.13 Fuzzy Matcher (Utility)

Not a UI component -- an algorithm for fuzzy string matching with ranked results and match highlights.

**Inputs:** Query string, candidate list.
**Outputs:** Ranked matches with match character indices.
**Requirements:** < 1ms for thousands of candidates.

---

## 4. Component Configuration

### 4.1 ComponentConfig Builder

All components use the `ComponentConfig` builder pattern for configuration:

```cpp
ComponentConfig{}
    .with_label("Save")
    .with_size(ComponentSize{pixels(200), pixels(50)})
    .with_color_usage(Theme::Usage::Primary)
    .with_padding(Padding{8, 8, 8, 8})
    .with_rounded_corners(RoundedCorners())
    .with_roundness(0.08f)
    .with_tooltip("Save the current file")
    .with_disabled(false)
```

### 4.2 Config Presets

For patterns that repeat 3+ times across 2+ screens, use named preset functions:

```cpp
// src/ui_helpers.h
inline ComponentConfig row_bg(float w = 600, float h = 52, Color bg = {30, 30, 40, 255}) {
    return ComponentConfig{}
        .with_background(bg)
        .with_rounded_corners(RoundedCorners())
        .with_roundness(0.08f)
        .with_size(ComponentSize{pixels(w), pixels(h)});
}

inline ComponentConfig screen_title(const std::string& label, float font_size = 36.0f) {
    return ComponentConfig{}
        .with_label(label)
        .with_font("EqProRounded", font_size)
        .with_custom_text_color(Color{255, 255, 255, 255});
}

// Usage:
div(ctx, mk(e, 10), row_bg());
```

Presets should be screen-agnostic with sensible defaults. Screens can override individual properties via chaining after the preset call.

### 4.3 Config Validation

Runtime validators catch common configuration errors:

| Validator | Flag | What It Checks |
|-----------|------|---------------|
| ValidateZeroSize | `enforce_zero_size_detection` | Elements resolving to 0 width/height |
| ValidateAbsoluteMarginConflict | `enforce_absolute_margin_conflict` | Absolute-positioned elements with margins |
| ValidateLabelHasFont | `enforce_label_has_font` | Labels without a font set |
| ValidateSpacingRhythm | `enforce_spacing_rhythm` | Margins/padding on 4px grid |
| ValidatePixelAlignment | `enforce_pixel_alignment` | Fractional positions causing blur |

**Modes:**
- Development mode: enables config-conflict validators (ZeroSize, AbsoluteMargin, LabelHasFont)
- Strict mode: enables all 5 validators

---

## 5. Composition Map

How primitives compose into features:

```
Split-Screen Editing
  +-- Split Pane
       +-- Draggable Divider
       +-- Content Pane (div)
       +-- Content Pane (div)

Sidebar (Nav Pane)
  +-- Dockable Panel (left)
       +-- Draggable Divider (right edge)
       +-- Tab Strip (panel switcher)
       +-- Panel Content (scroll_view)
            +-- File Tree --> Tree Node (recursive)
            +-- Outline  --> Tree Node (recursive)
            +-- Search   --> Tree Node (grouped results)

Bottom Panel
  +-- Dockable Panel (bottom)
       +-- Draggable Divider (top edge)
       +-- Tab Strip (terminal/output switcher)
       +-- Panel Content

Tab Bar
  +-- Tab Strip
       +-- Tab entities (app-provided)

Command Palette
  +-- Anchored Popup (centered)
       +-- text_input() + Fuzzy Matcher
       +-- scroll_view() + filtered results

Autocomplete
  +-- Anchored Popup (below cursor)
       +-- scroll_view() + filtered results
       +-- Optional docs pane (div)

Context Menus
  +-- Anchored Popup (at cursor)
       +-- Menu items (recursive for submenus)

Breadcrumbs
  +-- Breadcrumb Bar
       +-- Anchored Popup (per segment dropdown)

Diff Viewer
  +-- Split Pane
       +-- Editor (read-only, left)
       +-- Editor (editable, right)
       + Gutter decorations for change markers

Code Folding
  +-- Collapsible Region (per fold range)
       + Gutter slot (fold arrow)

Settings Editor
  +-- scroll_view()
       +-- text_input() (search) + Fuzzy Matcher
       +-- setting_row() entries
```

---

## 6. Implementation Priority

Ordered by dependency chain (each primitive unlocks the features listed):

1. **Draggable Divider** -- unlocks Split Pane, Dockable Panel
2. **Dockable Panel** -- unlocks sidebar, bottom panel
3. **Tree Node** -- unlocks file tree, outline with DnD
4. **Tab Strip** -- unlocks tab bar, panel switchers
5. **Anchored Popup** -- unlocks autocomplete, hover cards, context menus, command palette
6. **Fuzzy Matcher** -- unlocks command palette, file tree filter, settings search
7. **Stepper** -- unlocks all settings screens with `< value >` pattern
8. **Context Menu / Dropdown Menu** -- unlocks right-click menus, menu bars
9. **Tooltip / Popover** -- unlocks contextual help, click-triggered panels
10. **Command Palette** -- unlocks Cmd+Shift+P
11. **Accordion** -- unlocks collapsible settings categories
12. **Collapsible Region** -- unlocks code folding
13. **Breadcrumb Bar** -- unlocks path navigation
14. **Gutter** -- unlocks line numbers, fold markers, breakpoints
15. **Scroll Decoration Layer** -- unlocks scrollbar marks
16. **Minimap** -- unlocks document overview
17. **Combobox** -- unlocks search/filter selects
18. **Segmented Control** -- unlocks view mode toggles
19. **Rating** -- unlocks star rating displays
20. **Form Helpers** (Fieldset, Form Label) -- unlocks accessible form grouping

---

## 7. Known Vendor Issues

Components that have known issues in the `vendor/afterhours` library:

| Component | Issue | Workaround |
|-----------|-------|------------|
| `tab_container()` | Tab strip renders at screen-absolute position, ignoring parent bounds | Build manual tab buttons in a row |
| `toggle_switch()` | Creates sibling entities consuming extra layout space | Use `with_no_wrap()`, increase container height |
| `checkbox` / `toggle_switch` | Focus ring visibility unverified on these types | Needs manual keyboard testing |
