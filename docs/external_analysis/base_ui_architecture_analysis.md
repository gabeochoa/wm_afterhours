# Base UI Architecture Analysis

How Base UI (MUI) achieves "unstyled, maximally customizable" components, and
what afterhours can adopt from the same playbook.

## 1. Core Architecture Patterns in Base UI

### 1.1 Compound Component Decomposition

Every Base UI component is split into small, independently renderable **parts**.
A Dialog isn't one monolithic widget — it's:

```
Dialog.Root        (state container, renders nothing)
Dialog.Trigger     (<button>)
Dialog.Portal      (teleports to document.body)
Dialog.Backdrop    (overlay)
Dialog.Popup       (floating panel)
Dialog.Title       (<h2>)
Dialog.Description (<p>)
Dialog.Close       (<button>)
Dialog.Viewport    (scroll container)
```

Each part is its own React component with its own file, props interface, and
state mapping.  Users compose only the parts they need.  There is no "variant"
prop that toggles between built-in layouts — layout is expressed by composition.

**How this differs from afterhours today:**

afterhours `slider()` and `tab_container()` are single functions that internally
create all child entities (label, track, thumb, tab bar, tab buttons, underline).
The user can influence the result via `ComponentConfig` fields but cannot replace
or rearrange the internal structure.

### 1.2 Behavior Hooks Separated from Rendering

For each "part", Base UI has a `use<Part>` hook that returns:
- DOM props (event handlers, ARIA attributes, data attributes)
- Refs
- State values

The actual element rendered is delegated to `useRenderElement()`.

```
useDialogRoot()     → floating-UI context, open/close logic
useButton()         → keyboard/click handling, disabled state
useCompositeRoot()  → arrow-key navigation, highlight tracking
useCompositeItem()  → registration, highlight state for one item
```

This means a library consumer could call `useDialogRoot()` without rendering
any DOM — they get pure behavior.  The rendering step is entirely pluggable.

**afterhours equivalent:** `_init_component()` is the closest analog.  It calls
`_overwrite_defaults()`, `_validate_config()`, and `_add_missing_components()`.
But behavior (click handling, slider drag, tab switching) is wired up
*inside* the component function (`slider()`, `tab_container()`, etc.) and
cannot be consumed separately.

### 1.3 The `render` Prop (Element Delegation)

Every Base UI part accepts a `render` prop:

```tsx
<Dialog.Trigger render={<MyCustomButton />}>
  Open
</Dialog.Trigger>

// or a render function:
<Switch.Thumb render={(props, state) => (
  <span {...props}>{state.checked ? <On/> : <Off/>}</span>
)} />
```

This lets users swap the underlying HTML element *or* compose multiple
Base UI parts onto one DOM node — without the library needing to anticipate
every possible use case.

**afterhours equivalent:** The `decorate()` pattern on `ElementResult` is the
closest thing, but it only adds *children*, it doesn't replace the element
itself.  There's no way to say "use my entity instead of the one the library
would create."

### 1.4 State Exposed via Data Attributes

Components expose state as `data-*` attributes (`data-checked`,
`data-highlighted`, `data-open`, `data-disabled`, etc.) so CSS can target
states without needing JavaScript callbacks.  These are driven by a
`stateAttributesMapping` table that converts component state → attributes.

**afterhours equivalent:** The ECS already stores state as components
(`HasSliderState`, `HasCheckboxState`, etc.) which the rendering system reads.
The pattern is structurally identical — state is separate from rendering — just
expressed in ECS terms rather than DOM attributes.

### 1.5 Controlled vs. Uncontrolled State

Every stateful component supports both:
- **Uncontrolled**: internal state with optional `defaultValue`
- **Controlled**: external state passed via `value` + `onValueChange`

The `useControlled()` hook bridges both modes.

**afterhours equivalent:** Most components take a mutable reference (`float&`,
`size_t&`, `bool&`) which is the external state.  There's no "uncontrolled"
mode — the caller always owns the state.  This is actually *fine* for
an immediate-mode UI since the caller re-renders every frame anyway.

### 1.6 Composite (Roving Focus) Abstraction

`CompositeRoot` + `CompositeItem` is an internal building block that provides:
- Registration of child items via `CompositeList`
- Arrow-key navigation between items
- Highlighted-index tracking
- Home/End key support
- Orientation-aware (horizontal, vertical, grid)

Tabs, Menu, Select, RadioGroup, Toolbar — all built on top of Composite.

**afterhours equivalent:** Tab/focus navigation is handled by the `UIContext`
`process_tabbing()` function and `WidgetNext`/`WidgetBack` actions.  There is
no reusable "composite" building block.  Each compound widget (tab_container,
checkbox_group, dropdown) implements its own arrow-key logic inline.

---

## 2. What afterhours Already Does Well

| Capability | afterhours approach | Notes |
|---|---|---|
| **Theming** | `Theme` + `Theme::Builder` + `ThemeDefaults` singleton | Excellent — covers colors, fonts, sizing, roundness. Auto-generates missing palette entries. |
| **Styling defaults per component** | `UIStylingDefaults::set_component_config()` | Allows registering default configs per ComponentType. |
| **Decorator composition** | `ElementResult::decorate(fn)` | Clean chainable pattern for adding visual decorations. |
| **Config inheritance** | `ComponentConfig::inherit_from()` | Internal sub-elements inherit relevant parent settings. |
| **Validation at config time** | `_validate_config()` + `ValidationConfig` | Catches misconfiguration at build time — Base UI has nothing like this. |
| **Resolution independence** | `h720()`, `w1280()`, `ScreenPercent` sizing | Designed for TV/console — Base UI relies on CSS rem/px. |

---

## 3. Where afterhours Can Learn From Base UI

### 3.1 Decompose Compound Widgets into Primitives

**Problem:**  `slider()` is ~130 lines that creates a label div, a background
div, a handle div, wires up drag callbacks, left/right listeners, and value
update logic — all in one function.  If a game wants a slider with no label,
a vertical slider, a range slider, or a slider with tick marks, they have to
either fight `ComponentConfig` flags or rewrite from scratch.

**Recommendation:** Factor compound widgets into composable primitives:

```cpp
// Low-level primitives (each one is independently usable):
slider_root(ctx, mk(parent), config);         // Container + state
slider_track(ctx, mk(slider_root_ent));       // Background track
slider_range(ctx, mk(slider_root_ent));       // Filled range portion
slider_thumb(ctx, mk(slider_root_ent));       // Draggable handle
slider_label(ctx, mk(slider_root_ent));       // Optional label

// High-level convenience (composes the primitives):
slider(ctx, mk(parent), value, config);       // Today's API, unchanged
```

The existing `slider()` becomes a convenience wrapper that calls the
primitives.  Power users compose primitives directly.

**Priority:** High.  This affects slider, tab_container, dropdown, checkbox_group,
pagination, navigation_bar, and button_group.

### 3.2 Extract a Reusable Composite/Roving-Focus Primitive

**Problem:** Arrow-key navigation within a group of items is reimplemented for
each compound widget (tabs, dropdown, checkbox_group, button_group).

**Recommendation:** Create a `CompositeGroup` concept:

```cpp
// composite_group creates a container where:
// - Tab enters/exits the group as a whole
// - Arrow keys move between items
// - Home/End jump to first/last
// - Orientation controls which arrow keys are active
auto group = composite_group(ctx, mk(parent), {
    .orientation = Orientation::Horizontal,
    .loop = true,
    .highlighted_index = &current_idx,
});

// Each item in the group:
composite_item(ctx, mk(group.ent()), config);
```

This is the single highest-leverage internal refactor — it eliminates
duplicated navigation logic and makes it easy to build new compound widgets
(toolbar, menu bar, radio group, segmented control).

### 3.3 Support Element Replacement (Render Delegation)

**Problem:** `button()` always creates its own entity.  If a game wants a
`Dialog.Trigger` that *is* a button (one entity, two behaviors), it can't merge
them.

**Recommendation:** Allow components to target an existing entity:

```cpp
// Option A: "adopt" pattern — use an existing entity as the button
auto my_entity = div(ctx, mk(parent), my_config);
button_behavior(ctx, my_entity.ent());  // Adds click listener + focus to existing entity

// Option B: "render_into" config flag
button(ctx, mk(parent), config.render_into(existing_entity));
```

This is the afterhours equivalent of Base UI's `render` prop.  It's not about
swapping HTML tags (afterhours has no tags) — it's about letting multiple
"roles" coexist on one entity.

**Priority:** Medium.  Most useful for toolbar items, list items that are also
buttons, and trigger elements.

### 3.4 Richer State Exposure for Theming/Styling

**Problem:** The `ComponentConfig` exposes `color_usage` and `custom_color`
for background, but there's no built-in way to say "when hovered, use this
color" or "when active, use this other color".  Each component handles this
internally (slider handle changes on drag, button darkens on press).

**Recommendation:** Add state-dependent color slots to `ComponentConfig`:

```cpp
config
  .with_background(Theme::Usage::Primary)
  .with_hover_background(Theme::Usage::Accent)
  .with_active_background(colors::darken(theme.primary, 0.15f))
  .with_focused_background(Theme::Usage::Primary);  // or no change
```

The rendering system already knows `was_hot()`, `was_active()`,
`has_focus()` — it just needs a mapping from state → color instead of
hardcoded per-component logic.

Base UI achieves this via data attributes + CSS.  afterhours would achieve it
via state-color mappings in ComponentConfig + the existing rendering pipeline.

**Priority:** Medium-high.  Eliminates a lot of per-component styling code
and makes themes feel more complete.

### 3.5 Event Detail / Cancel Pattern

**Problem:** `HasClickListener` takes a `std::function<void(Entity&)>` — the
callback fires and that's it.  There's no way to cancel the action (e.g.,
prevent a tab switch based on validation) or know *why* the event fired
(click vs. keyboard activation vs. programmatic).

**Recommendation:** Introduce an `EventDetails` struct:

```cpp
struct EventDetails {
    enum Reason { Click, KeyboardActivate, Programmatic };
    Reason reason;
    bool cancelled = false;
    void cancel() { cancelled = true; }
};

// Change handlers become:
using ChangeHandler = std::function<void(Entity&, EventDetails&)>;
```

**Priority:** Low-medium.  Primarily useful for forms, validation, and
undo-able state changes.

---

## 4. Concrete Refactoring Plan

### Phase 1: Composite Primitive (highest leverage)

1. Create `composite_group()` and `composite_item()` in a new
   `ui/composite.h`.
2. Refactor `tab_container()` to use it internally.
3. Refactor `dropdown()` and `checkbox_group()` to use it.
4. Refactor `button_group()` and `pagination()` to use it.
5. Each refactor is backward-compatible — existing APIs still work.

### Phase 2: Decomposed Slider

1. Factor `slider()` into `slider_root`, `slider_track`, `slider_thumb`,
   `slider_label`.
2. `slider()` becomes a convenience wrapper calling the primitives.
3. Add `slider_range()` (filled portion — currently inlined).
4. Support vertical slider via `slider_root` orientation.

### Phase 3: State-Dependent Styling

1. Add `StateColors` struct to `ComponentConfig` (hover, active, focused,
   disabled color overrides).
2. Wire rendering system to read StateColors when the entity's context state
   matches.
3. Remove hardcoded hover/active color logic from individual components.

### Phase 4: Behavior Attachment

1. Extract `button_behavior()` that adds click + focus to any entity.
2. Extract `drag_behavior()` for slider-like dragging.
3. Extract `toggle_behavior()` for checkbox/switch toggling.
4. These are the "hooks" equivalent — pure behavior, no entity creation.

---

## 5. What NOT to Copy from Base UI

| Pattern | Why it doesn't apply to afterhours |
|---|---|
| **Portal / teleport to body** | afterhours has no DOM. Render layers handle z-ordering. |
| **ARIA attributes** | Not a web runtime. The accessibility_standards_notes.md covers what keyboard behaviors to adopt instead. |
| **CSS-in-JS / className callbacks** | No CSS engine. Theming is already handled via `Theme` + `ComponentConfig`. |
| **Controlled vs. uncontrolled** | IMGUI pattern means the caller always owns state — no need for `useControlled()`. |
| **React-specific hooks (useRef, useMemo, useCallback)** | ECS components serve the same role as React state/refs. |
| **Floating UI integration** | afterhours uses its own layout system (`autolayout.h`). Popover positioning would be a separate feature. |

---

## 6. Mapping: Base UI Concepts → afterhours Equivalents

| Base UI | afterhours today | afterhours proposed |
|---|---|---|
| `<Component.Root>` | `component(ctx, mk(parent), config)` | Same, but roots become state-only containers |
| `<Component.Part>` | Internal entity created inside component fn | Exposed as `component_part(ctx, mk(root))` |
| `render` prop | N/A | `behavior(ctx, existing_entity)` functions |
| `useRenderElement()` | `_init_component()` + `_add_missing_components()` | Same — already the render delegation layer |
| `useCompositeRoot()` | Per-widget arrow-key code | `composite_group()` + `composite_item()` |
| `data-*` attributes | ECS components (`HasSliderState`, etc.) | Same — already state-attribute separated |
| `stateAttributesMapping` | Rendering reads ECS components | Add `StateColors` for styling per state |
| `className` callback | N/A | `ComponentConfig` callbacks for dynamic config |
| `eventDetails.cancel()` | N/A | `EventDetails` struct on change handlers |
| `CompositeList` | N/A | New: ordered child tracking in `composite_group` |

---

## 7. Quick Wins (can do now, no architecture change)

1. **Document the primitive decomposition** for `slider`, `tab_container`,
   and `dropdown` in their respective doc files (02-series).

2. **Add `with_hover_color()` / `with_active_color()`** to ComponentConfig.
   The rendering system already detects hover/active — just needs a color
   lookup table per entity.

3. **Extract `prev_index()` / `next_index()`** from `imm_components.h` into
   a shared utility — these are already used by multiple components and are
   the seed of the composite primitive.

4. **Add orientation to slider** — the track/thumb logic is already
   axis-generic in the layout system; slider just needs to support
   `FlexDirection::Column` as an option.

---

## 8. Component Coverage: Base UI vs afterhours

Full inventory of every Base UI component mapped against what afterhours has
today, what's already tracked in a docs/ file, and what's new/missing.

### Legend

- **Has** = afterhours has this component today
- **Doc** = a docs/ file exists tracking this as a future feature
- **Gap** = not implemented and not yet tracked anywhere

### Interactive Controls

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Button** | Has: `button()` | Supports Filled/Outline/Ghost variants, icon+text |
| **Toggle** | Has: `toggle_switch()` | iOS-style toggle; also `checkbox()` for non-toggle |
| **Toggle Group** | Has: `button_group()` | Single/multi select button group |
| **Checkbox** | Has: `checkbox()`, `checkbox_no_label()` | Full support including `checkbox_group()` |
| **Checkbox Group** | Has: `checkbox_group()` | Built into imm_components |
| **Radio** / **Radio Group** | Has: `radio_group()` | Full support |
| **Switch** | Has: `toggle_switch()` | Same as Toggle above |
| **Slider** | Has: `slider()` | Single-value, horizontal only. No range slider or vertical. |
| **Number Field** | Gap | Numeric input with increment/decrement buttons. See also `docs/09_input_number.md` |
| **Input** | Has: `text_input()` | Single-line text. Supports mask chars. |
| **Select** | Has: `dropdown()` | Single-select dropdown with options list |
| **Combobox** | Gap | Text input + dropdown suggestions (typeahead/autocomplete). Different from Select — user can type. |
| **Autocomplete** | Gap | Search-as-you-type filtering. Shares internals with Combobox. |

### Navigation & Structure

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Tabs** | Has: `tab_container()` | Horizontal tab bar with content switching |
| **Accordion** | Doc: `docs/12_accordion.md` | Collapsible sections — tracked, not yet built |
| **Collapsible** | Doc: `docs/12_accordion.md` | Single collapsible panel — accordion's building block |
| **Navigation Menu** | Has: `navigation_bar()` | Horizontal nav bar. No submenu/flyout support. |
| **Menubar** | Gap | Horizontal menu bar with dropdown menus (File, Edit, etc.) |
| **Menu** | Doc: `docs/24_context_menu.md` | Click-triggered popup menu — tracked as context menu |
| **Context Menu** | Doc: `docs/24_context_menu.md` | Right-click triggered menu — tracked |
| **Toolbar** | Gap | Horizontal row of buttons/controls with roving focus. Conceptually close to `button_group` but semantically different. |
| **Separator** | Has: `separator()` | Horizontal/vertical, with optional label |
| **Pagination** | Has: `pagination()` | Page navigation with prev/next |

### Overlays & Popups

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Dialog** | Has: `modal.h` | Full modal dialog with confirm/cancel/dismiss, backdrop, focus trap, closedBy modes |
| **Alert Dialog** | Has: `modal.h` | `DialogType::Alert` — blocking dialog, no outside dismiss |
| **Drawer** | Gap | Slide-in panel from edge (side sheet). Could be built on Dialog + animation. |
| **Popover** | Gap | Non-modal floating panel anchored to a trigger element. Needs positioning logic. |
| **Tooltip** | Doc: `docs/10_tooltip.md` | Hover hint — tracked, not yet built |
| **Preview Card** | Gap | Hover/focus card preview (like link previews). Popover variant. |
| **Toast** | Has: `toast.h` | Full support: info/success/warning/error/custom, auto-dismiss, stacking |

### Forms & Validation

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Field** | Gap | Wraps a control with label + description + error message. afterhours labels are per-component, no unified field wrapper. |
| **Fieldset** | Gap | Groups related fields with a legend. |
| **Form** | Gap | Form container with validation, submission, error collection. afterhours has no form abstraction. |

### Data Display

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Progress** | Has: `progress_bar()`, `circular_progress()` | Linear and circular variants |
| **Meter** | Doc: `docs/04_meters_gauges.md` | Bounded value display (health bar, disk usage). Close to progress but semantically "of a known range". |
| **Avatar** | Doc: `docs/13_avatars_badges.md` | User image/initials in a circle. Tracked. |

### Layout & Containers

| Base UI Component | afterhours Status | Notes |
|---|---|---|
| **Scroll Area** | Has: `overflow_y = Scroll` | Built into ComponentConfig overflow settings |
| **Div** | Has: `div()` | Generic container — the fundamental building block |

### Utilities (Base UI internal, not components)

| Base UI Utility | afterhours Equivalent | Notes |
|---|---|---|
| **Composite** | Gap (proposed `composite_group`) | Roving focus primitive — see Section 3.2 above |
| **Direction Provider** | N/A | RTL/LTR — not needed for most game UIs |
| **CSP Provider** | N/A | Web security policy — not applicable |
| **Floating UI** | N/A | Popover positioning library — afterhours uses autolayout |
| **use-button** | Built into `button()` | Click/keyboard activation behavior |
| **use-render** | Built into `_init_component()` | Element creation/rendering delegation |
| **merge-props** | N/A | React prop merging — not applicable to ECS |

### Summary Count

| Category | Base UI | afterhours Has | Tracked in Docs | Gap (New) |
|---|---|---|---|---|
| Interactive Controls | 13 | 9 | 1 | 3 (Combobox, Autocomplete, Number Field) |
| Navigation & Structure | 10 | 5 | 2 | 3 (Menubar, Toolbar, Collapsible as standalone) |
| Overlays & Popups | 6 | 3 | 1 | 2 (Drawer, Popover) |
| Forms & Validation | 3 | 0 | 0 | 3 (Field, Fieldset, Form) |
| Data Display | 3 | 2 | 1 | 0 |
| Layout & Containers | 2 | 2 | 0 | 0 |
| **Total** | **37** | **21** | **5** | **11** |

### Priority Ranking for Missing Components

**High — Common in game UIs:**
1. **Combobox / Autocomplete** — Server browsers, item search, command palettes
2. **Popover** — Player cards, item details, contextual info panels
3. **Toolbar** — Editor tools, in-game build bars (reusable composite pattern)
4. **Number Field** — Settings (volume, sensitivity), spin boxes

**Medium — Useful for polished UIs:**
5. **Drawer** — Inventory panels, side menus, chat drawers
6. **Menubar** — Level editors, dev tools, map editors
7. **Field** — Any form-heavy screen (account creation, settings)
8. **Collapsible** (standalone) — FAQ, nested settings, tree views

**Low — Nice-to-have:**
9. **Fieldset** — Grouping within forms
10. **Form** — Full form validation flow
11. **Preview Card** — Link/player hover previews

### Cross-reference with Existing Doc Files

These existing docs already cover related future work:

| Doc File | Covers |
|---|---|
| `docs/09_input_number.md` | Number Field / Stepper selector |
| `docs/10_tooltip.md` | Tooltip |
| `docs/12_accordion.md` | Accordion / Collapsible |
| `docs/13_avatars_badges.md` | Avatar |
| `docs/24_context_menu.md` | Menu / Context Menu |
| `docs/04_meters_gauges.md` | Meter |
| `docs/22_form_controls.md` | Form-related controls |
| `docs/25_list_table.md` | List / Table (not in Base UI) |
| `docs/11_tree_node.md` | Tree View (not in Base UI) |
| `docs/37_resizable_panels.md` | Resizable Panels (not in Base UI) |
| `docs/31_overlay_panels.md` | Overlay / Popover concepts |

---

## Summary

Base UI's power comes from three ideas:
1. **Compound decomposition** — every widget is N composable parts
2. **Behavior/render separation** — hooks provide behavior, rendering is pluggable
3. **State exposure** — components expose state, styling consumes it

afterhours already has (3) via ECS.  The biggest gains come from adopting (1)
and (2): decomposing compound widgets into primitives and extracting reusable
behavior functions.  This would let any game build arbitrary UI designs using
the same base primitives, without being constrained by the specific layouts
that `slider()` or `tab_container()` happen to bake in today.
