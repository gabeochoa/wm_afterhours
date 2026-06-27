# Ariakit Analysis

Source: https://ariakit.org
GitHub: https://github.com/ariakit/ariakit

Ariakit is a low-level, unstyled React component library designed as
the accessibility foundation for higher-level UI libraries and design
systems. It's created by Diego Haz and is the successor to Reakit.

---

## 1. Architecture Overview

Ariakit has a three-layer architecture:

```
  Hook Layer          Component Layer        Store Layer
  ─────────           ───────────────        ───────────
  useFocusable()  →   <Focusable>            (no store)
  useComposite()  →   <Composite>        ←   useCompositeStore()
  useDialog()     →   <Dialog>           ←   useDialogStore()
  useCombobox()   →   <Combobox>         ←   useComboboxStore()
```

- **Hook Layer** (`createHook`): Each component has a `use<Name>` hook
  that accepts props and returns HTML props. Hooks compose by chaining —
  `useComposite` calls `useFocusable` internally. This is the behavior
  layer.

- **Component Layer** (`createElement`): Thin wrappers that call the
  hook and render via `createElement()`. Handles the `render` prop for
  element delegation and `wrapElement` for wrapping.

- **Store Layer**: External state objects (created via `use<Name>Store`)
  that hold shared state between related components. Stores are
  framework-agnostic at the core (`@ariakit/core`) with React bindings
  on top (`@ariakit/react`).

### Key insight for afterhours

The hook/component split means behavior is reusable without being tied
to a specific rendering strategy. afterhours could adopt a similar
pattern where widget *behavior* (focus management, keyboard handling,
state) is separate from widget *rendering* (what gets drawn).

---

## 2. Composition Model: The `render` Prop

Ariakit's primary composition mechanism is the **`render` prop** (similar
to Radix UI's `asChild` but more flexible):

### 2a. Replace the underlying element

```jsx
// Render a Combobox as a <textarea> instead of <input>
<Combobox render={<textarea rows={5} />} />

// Render a Button as a <summary>
<Button render={<summary />}>Show modal</Button>

// Render a Tab as a Next.js Link
<Tab render={<Link href="/new" />}>New</Tab>
```

### 2b. Compose with third-party components

```jsx
// Render a Menu as a Framer Motion element
<Menu render={<motion.div animate={{ y: 100 }} />} />
```

### 2c. Explicit render function for full control

```jsx
// Full control over prop merging
<Button render={(htmlProps) => <summary {...htmlProps} />} />

// Inject wrapper elements
<Dialog render={(dialogProps) => (
    <div className="backdrop">
        <div {...dialogProps} />
    </div>
)} />
```

### Props merging rules

When using the element form of `render`:
- `style`, `className`, `ref`, and event props are **auto-merged**
- All other props from the original component pass through
- Rendered element props **override** original component props
- `children` passed to the rendered element override component children

When using the function form:
- **No automatic merging** — you control everything
- Use Ariakit's `Role` component for type-safe prop merging

### afterhours equivalent

afterhours doesn't have a JSX-like composition model, but the concept
maps to **render delegation**. The idea is that a widget's behavior
(focus, keyboard, ARIA state) can be applied to *any* renderable entity,
not just a predefined widget. This is already partially how afterhours
works — you compose `Has*` components onto entities. The Ariakit model
validates that this approach is correct.

**Concrete takeaway:** When building new widgets, separate the behavioral
components (`HasFocusable`, `HasComposite`, `HasDismissable`) from the
visual components (`HasColor`, `HasBorder`, `HasLabel`). Any entity can
mix-and-match behavioral and visual components.

---

## 3. Primitives / Abstract Components

Ariakit's component set is organized into two tiers:

### 3a. Abstract components (building blocks)

These are the reusable primitives that higher-level components build on:

| Primitive | What It Does | afterhours Equivalent |
|---|---|---|
| **Focusable** | Normalizes focus across elements; adds `data-focus-visible`, `accessibleWhenDisabled`, tab index management | Partial: `SkipWhenTabbing`, focus ring in theme |
| **Composite** | Single tab stop, arrow-key navigation among children (roving focus). Base for Menu, Select, Tab, Toolbar, Radio | Missing: no roving focus abstraction |
| **Collection** | Tracks DOM-order of items, observes DOM mutations for order changes | Missing: no ordered item tracking |
| **Command** | Click via mouse or keyboard (Enter/Space). Base for Button | Partial: `HasClickListener` |
| **Group** | Generic labeled container for related elements | Missing: no group concept |
| **Portal** | Renders element in a different DOM location | N/A (no DOM in native UI) |
| **Role** | Base primitive providing core Ariakit features to any element | Similar to `UIComponent` as base |
| **Separator** | Visual/semantic separator between sections or composite items | Missing |
| **VisuallyHidden** | Hidden visually but available to screen readers | N/A (web-specific) |
| **FocusTrap** | Sentinel element that redirects focus back into a container | Partial: dialog focus trapping |

### 3b. High-level components

| Component | WAI-ARIA Pattern | afterhours Status |
|---|---|---|
| **Button** | Button | Exists |
| **Checkbox** | Checkbox | Exists |
| **Combobox** | Combobox | Missing (planned as Autocomplete) |
| **Dialog** | Dialog (modal + non-modal) | Exists (modal only) |
| **Disclosure** | Disclosure (expand/collapse) | Missing |
| **Form** | Form (validation, error association) | Partial (text_input) |
| **Heading** | Auto-leveled headings | N/A |
| **Hovercard** | Popover on hover with safe triangle | Missing |
| **Menu** | Menu + Menu Button | Exists (dropdown) |
| **Menubar** | Menubar (persistent horizontal menu) | Missing |
| **Popover** | Positioned popup | Missing (planned) |
| **Radio** | Radio Group | Exists |
| **Select** | Combobox (listbox-style) | Exists (dropdown) |
| **Tab** | Tabs | Exists |
| **Tag** | Tag/chip input | Missing |
| **Toolbar** | Toolbar (grouped controls, roving focus) | Missing (planned) |
| **Tooltip** | Tooltip | Exists |

---

## 4. Styling Approach

Ariakit is **completely unstyled** — no default CSS, no opinions on
styling method. Components expose state through **data attributes** and
**CSS variables** that consumers can target.

### 4a. Data attributes for state

These are the **stable, public API** CSS selectors (others may change):

| Attribute | Applied When | Component(s) |
|---|---|---|
| `[data-focus-visible]` | Keyboard focus (not mouse) | Focusable + all descendants |
| `[data-active-item]` | Item has focus in composite widget | ComboboxItem, MenuItem, TabItem, etc. |
| `[data-active]` | Simulated `:active` for non-button elements | Command, Button |
| `[data-open]` | Content is shown (synchronous) | Disclosure, Dialog, Popover, Menu |
| `[data-enter]` | Content entering (delayed for CSS transitions) | Disclosure, Dialog, Popover |
| `[data-leave]` | Content exiting (for exit animations) | Disclosure, Dialog, Popover |
| `[data-backdrop]` | Dialog backdrop element | Dialog |
| `[data-autofill]` | Browser autofilled the value | Select (in forms) |
| `[data-user-value]` | Span matching user's input | ComboboxItemValue |
| `[aria-checked]` | Checkbox/toggle checked state | Checkbox |
| `[aria-disabled]` | Element is disabled | Focusable (all) |
| `[aria-expanded]` | Content element is shown | Disclosure, MenuButton, Select |
| `[aria-invalid]` | Form field has validation error | Form fields |

**Key warning from Ariakit:** Never use `role` or other ARIA attributes
as CSS selectors — they're internal implementation and may change. Only
use the attributes listed above.

### 4b. CSS variables for positioning

| Variable | Purpose |
|---|---|
| `--dialog-viewport-height` | Visual viewport height (for mobile keyboard) |
| `--popover-anchor-width` | Width of the anchor element |
| `--popover-available-height` | Available vertical space |
| `--popover-available-width` | Available horizontal space |
| `--popover-overflow-padding` | Padding from viewport edge |
| `--popover-transform-origin` | Origin point for animations |
| `--scrollbar-width` | Browser scrollbar width |

### 4c. Transition system

Ariakit has a built-in transition state machine for show/hide:

1. Element starts hidden
2. `data-open` applied synchronously on show
3. `data-enter` applied one frame later (allows CSS transitions to start)
4. On hide: `data-leave` applied (if enter transition was detected)
5. Element hidden after transition completes

This means you can use pure CSS transitions without JavaScript animation
libraries:

```css
.dialog {
    transform: scale(0.9);
    opacity: 0;
    transition: transform 200ms, opacity 200ms;
}
.dialog[data-enter] {
    transform: scale(1);
    opacity: 1;
}
```

Or CSS animations:

```css
.dialog[data-open] {
    animation: fade-in 200ms;
}
```

### afterhours equivalent

afterhours already uses a similar pattern with entity components exposing
state that the rendering system reads. The **data attribute** concept maps
to component presence — an entity "has" `[data-focus-visible]` if it has
a `HasFocusVisible` component.

**Concrete takeaway:** The transition state machine
(`open → enter → leave → hidden`) is a clean model afterhours should
adopt. Currently transitions are ad-hoc. A formal `HasPresence` component
with these four states would standardize animated show/hide across all
widgets.

---

## 5. Accessibility Patterns

This is Ariakit's strongest area. Key patterns they implement:

### 5a. `accessibleWhenDisabled`

Disabled elements stay **focusable** (with `aria-disabled` instead of
`disabled`). This lets users discover what controls exist even when
they're not currently usable.

Ariakit's `Focusable` component intercepts events and `stopPropagation`
+ `preventDefault` when disabled, so the element can receive focus but
can't be activated.

**afterhours validation relevance:** New flag:
`enforce_disabled_elements_remain_focusable` — check that disabled
interactive elements are still in the tab order (using `aria-disabled`
semantics rather than removing from focus entirely).

### 5b. Focus-visible tracking

Ariakit tracks **keyboard modality** globally:
- `mousedown` → `isKeyboardModality = false`
- `keydown` (non-modifier) → `isKeyboardModality = true`
- When an element gains focus while `isKeyboardModality` is true,
  `data-focus-visible` is applied

This is more reliable than `:focus-visible` because it also applies to
virtual focus in composite widgets (where the DOM focus stays on the
container but the "active item" changes via arrow keys).

**afterhours implication:** The distinction between "mouse/touch focus"
and "keyboard focus" should be tracked globally. Focus rings should only
be shown for keyboard navigation, not for mouse clicks.

### 5c. Composite / Roving Focus

The `Composite` component is the most architecturally significant
primitive. It handles:

1. **Single tab stop**: The entire widget is one tab stop. Tab moves
   past the widget entirely.
2. **Arrow key navigation**: Up/Down/Left/Right moves between items
3. **Grid layout**: Items can declare a `rowId` to enable 2D grid
   navigation
4. **Virtual focus**: DOM focus stays on the composite container; the
   active item is tracked in state and gets `data-active-item`
5. **Keyboard proxy**: Key events on the container are forwarded to the
   active item's element
6. **Home/End**: Jump to first/last item
7. **Type-ahead**: Printable keys search item labels

Components built on Composite:
- **Menu** (vertical, with submenu support)
- **Menubar** (horizontal)
- **Select** (listbox with trigger button)
- **Combobox** (input + filterable listbox)
- **Tab** (tab list + panels)
- **Toolbar** (horizontal/vertical grouped controls)
- **Radio** (radio group)

**afterhours implication:** A reusable `Composite` / roving focus
abstraction would eliminate duplicated keyboard navigation code across
dropdown, tabs, radio_group, etc. This is the single most impactful
primitive afterhours is missing.

### 5d. Dialog focus management

Ariakit's Dialog handles an extensive set of focus concerns:

| Concern | How Ariakit Handles It |
|---|---|
| **Initial focus** | `autoFocusOnShow` + `initialFocus` prop |
| **Focus trapping** | `FocusTrap` sentinel elements (not `tabindex=-1` on all outside elements) |
| **Focus restoration** | `autoFocusOnHide` + `finalFocus` prop, defaults to disclosure element |
| **Hidden dismiss** | `prependHiddenDismiss()` — adds a visually-hidden button so screen readers can always close |
| **Escape key** | `hideOnEscape` prop |
| **Click outside** | `hideOnInteractOutside` prop |
| **Nested dialogs** | `useNestedDialogs()` — tracks dialog stack, only outermost traps focus |
| **Body scroll lock** | `usePreventBodyScroll()` when modal |
| **Non-modal** | `preserveTabOrder` keeps dialog in document tab flow |
| **Viewport resize** | `--dialog-viewport-height` CSS var updates on visual viewport change (mobile keyboard) |
| **Tree disabling** | `disableTreeOutside()` — marks elements outside dialog as inert |

**afterhours implication:** The hidden dismiss button is a pattern we
should adopt for modal dialogs. Even if there's a visible close button,
there should always be a way to dismiss that's reachable from anywhere
in the focus order.

### 5e. Hovercard safe triangle

The `Hovercard` component implements a "safe polygon" to prevent the
hovercard from closing when the mouse moves from the anchor to the card:

- `disablePointerEventsOnApproach` — disables pointer events on elements
  between anchor and card during hover transition

This is the same concept as Floating UI's safe polygon, implemented
differently.

### 5f. Collection DOM-order tracking

The `Collection` component tracks items in their **rendered DOM order**,
not their React declaration order. It observes DOM mutations to keep the
order synchronized. This is critical for composite widgets where the
visual order determines the arrow key navigation order.

**afterhours implication:** When the tab/focus order of a composite
widget's children needs to match visual layout order, afterhours needs
a similar mechanism. The entity list ordering should reflect spatial
layout, not creation order.

### 5g. Form validation with error association

Ariakit's Form component:
- Associates error messages with inputs via `aria-invalid` + `aria-describedby`
- Supports browser built-in validation
- Error state exposed via `[aria-invalid]` for styling

**afterhours implication:** Input validation errors should be associated
with the specific input element, not just displayed in a generic area.

---

## 6. Store Architecture

Each complex component family has a **store** that holds shared state:

```jsx
const dialog = useDialogStore({ open: false });

// Multiple components share the store
<DialogDisclosure store={dialog}>Open</DialogDisclosure>
<Dialog store={dialog}>Content</Dialog>
```

Or via Provider pattern:

```jsx
<DialogProvider open={false}>
    <DialogDisclosure>Open</DialogDisclosure>
    <Dialog>Content</Dialog>
</DialogProvider>
```

Stores are split between:
- `@ariakit/core` — framework-agnostic state logic
- `@ariakit/react` — React bindings (hooks, context)

Store features:
- Controlled & uncontrolled state (like React's `value` vs `defaultValue`)
- Selective subscriptions (`store.useState("activeId")` — only re-renders
  when that specific field changes)
- Batched updates (multiple state changes produce one re-render)

### afterhours equivalent

afterhours already has something similar via ECS — the "store" is just
the entity's component data. The controlled/uncontrolled pattern doesn't
apply to ECS. But **selective subscriptions** are interesting — the idea
that a system should only run when the specific data it cares about has
changed. This ties into the EntityQuery change-tracking idea from the
ReactiveUI analysis.

---

## 7. What afterhours Should Adopt

### High Priority

1. **Composite/RovingFocus abstraction** — A reusable `HasComposite` +
   `HasCompositeItem` component pair that handles single-tab-stop +
   arrow-key navigation. All compound widgets (dropdown, tabs, radio,
   toolbar) would use this instead of each implementing their own.

2. **Presence state machine** — Formal `open → enter → leave → hidden`
   lifecycle for animated visibility. Standardize across all widgets
   that show/hide (dialog, dropdown, tooltip, popover).

3. **Focus-visible distinction** — Track input modality (keyboard vs
   mouse/touch) globally. Only show focus rings for keyboard navigation.

4. **`accessibleWhenDisabled` as default** — Disabled interactive
   elements should remain focusable. Add event interception so they
   can't be activated.

### Medium Priority

5. **Hidden dismiss in modals** — Always provide a mechanism to dismiss
   modal dialogs that's reachable from the focus order.

6. **Collection/item ordering** — Items in composite widgets should be
   ordered by spatial position, not creation order.

7. **Hovercard with safe polygon** — Implement the approach polygon
   for hover-triggered popups.

8. **Error-input association** — Validation errors should reference the
   specific input entity they relate to.

### Low Priority (future consideration)

9. **Behavior/render separation at API level** — Ariakit's hook layer
   means you can use the behavior without the component. afterhours
   could formalize this — systems that add behavior components
   independently from systems that render.

10. **Nested dialog focus scope** — Support dialog stacking with proper
    focus scope management (only the topmost dialog traps focus).

---

## 8. Component Gap Summary (Updated)

Components Ariakit has that afterhours doesn't:

| Missing Component | Priority | Notes |
|---|---|---|
| **Combobox** (autocomplete) | High | Input + filterable popup list |
| **Popover** (positioned popup) | High | Anchor-relative positioning |
| **Menubar** | Medium | Horizontal persistent menu |
| **Toolbar** | Medium | Grouped controls with roving focus |
| **Disclosure** (accordion) | Medium | Expand/collapse toggle |
| **Hovercard** | Medium | Hover-triggered rich tooltip |
| **Tag** (chip input) | Low | Multi-value tag input |
| **Separator** | Low | Visual divider component |

Components afterhours has that Ariakit doesn't (game-specific):
- Slider
- Toggle switch
- Progress bar
- Color picker
- Breadcrumb
- Carousel
