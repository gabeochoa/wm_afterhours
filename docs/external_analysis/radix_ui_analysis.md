# Radix UI Analysis

How Radix Primitives achieves unstyled, accessible components, how Radix Themes
layers styling on top, and what afterhours can learn from both.

Repos:
- https://github.com/radix-ui/primitives
- https://github.com/radix-ui/themes

---

## 1. Two-Layer Architecture

Radix has a clean two-layer split:

**Layer 1: Primitives** — Unstyled, accessible behavior-only components.
Zero CSS. They handle keyboard navigation, focus management, open/close state,
ARIA attributes, and positioning.  They ship no visual opinion.

**Layer 2: Themes** — A styled component library built on top of Primitives.
Adds colors, sizes, variants, spacing tokens, and responsive prop system.
Themes wraps each Primitive in a thin layer that applies CSS classes.

This is exactly the pattern identified in `base_ui_architecture_analysis.md`
as the ideal for afterhours: behavior primitives that any "theme" can wrap.

---

## 2. How Primitives Enable Customization

### 2.1 The `asChild` Prop (Element Delegation)

Every Radix primitive part accepts `asChild`.  When `true`, the primitive
does not render its own DOM element — instead it merges its props, event
handlers, and ref onto the child element you provide:

```tsx
// Default: Radix renders a <button>
<Dialog.Trigger>Open</Dialog.Trigger>

// asChild: Radix merges behavior onto YOUR element
<Dialog.Trigger asChild>
  <MyFancyButton variant="primary">Open</MyFancyButton>
</Dialog.Trigger>
```

Under the hood, this is powered by the `Slot` component which:
1. Takes all props from the Radix part (event handlers, data attributes, ref)
2. Takes all props from the child element
3. Merges them: event handlers are composed (both fire), className is joined,
   style is merged, refs are composed

This is Radix's version of Base UI's `render` prop, but arguably cleaner —
`asChild` is a single boolean, not a render function.

**afterhours equivalent:** The proposed `button_behavior(ctx, entity)` pattern
from `base_ui_architecture_analysis.md`.  In afterhours terms, `asChild` means
"attach this behavior to an existing entity instead of creating a new one."

### 2.2 Compound Component Parts

Every Radix component is decomposed into named parts:

```tsx
<Dialog.Root>           // State container (renders nothing)
  <Dialog.Trigger />    // Button that opens
  <Dialog.Portal>       // Portals content to body
    <Dialog.Overlay />  // Backdrop
    <Dialog.Content>    // The dialog panel
      <Dialog.Title />
      <Dialog.Description />
      <Dialog.Close />  // Button that closes
    </Dialog.Content>
  </Dialog.Portal>
</Dialog.Root>
```

Users compose only the parts they need.  No `variant="centered"` or
`showOverlay={true}` — you just include or exclude parts.

### 2.3 Data Attributes for State

Every part exposes its state via `data-state` and other data attributes:

```css
[data-state="open"] { ... }
[data-state="closed"] { ... }
[data-state="checked"] { ... }
[data-disabled] { ... }
[data-highlighted] { ... }
[data-orientation="horizontal"] { ... }
```

This lets any CSS solution (Tailwind, CSS Modules, vanilla) style states
without needing JavaScript callbacks.

### 2.4 Controlled & Uncontrolled State

Every stateful primitive supports both modes via `useControllableState`:

```tsx
// Uncontrolled (internal state)
<Dialog.Root defaultOpen={false}>

// Controlled (external state)
<Dialog.Root open={isOpen} onOpenChange={setIsOpen}>
```

### 2.5 Scoped Context (Nested Component Isolation)

Radix uses `createContextScope` so that nested instances of the same component
don't interfere.  A `Dialog` inside a `Dialog` each gets its own context.
This is critical for things like nested menus and dialog-in-dialog.

---

## 3. Internal Building Blocks

These are the shared utilities that all Radix primitives build on:

### 3.1 `Primitive` — Base Element

A map from HTML tag name → React component that supports `asChild`.
Every Radix part renders via `Primitive.button`, `Primitive.div`, etc.

### 3.2 `Collection` — Ordered Item Tracking

Tracks registered child items in DOM order.  Used by Slider (multiple thumbs),
Select (options), Menu (items), Tabs (tabs).  Similar to Base UI's
`CompositeList`.

**afterhours equivalent:** Not yet implemented.  This is the same need as the
proposed `composite_group` from the Base UI analysis.

### 3.3 `RovingFocusGroup` — Keyboard Navigation

Arrow-key navigation within a group.  Features:
- Orientation-aware (horizontal/vertical)
- Loop option (wrap around)
- Direction-aware (LTR/RTL)
- Tracks current tab stop
- Distinguishes mouse vs keyboard focus

Used by: Tabs, Toolbar, RadioGroup, ToggleGroup, Menubar.

**afterhours equivalent:** Per-widget inline arrow-key logic.  The proposed
`composite_group` would serve the same role.

### 3.4 `DismissableLayer` — Outside Interaction Detection

Detects:
- Escape key press
- Pointer down outside the layer
- Focus moving outside the layer

Supports a *layer stack* — multiple nested dismissable layers where only the
topmost one dismisses.  Critical for nested menus and dialogs.

**afterhours equivalent:** `modal.h` handles escape + backdrop click for
modals.  A generalized version is needed for non-modal floaters (dropdowns,
popovers, tooltips).

### 3.5 `FocusScope` — Focus Trapping

Two modes:
- **loop:** Tab wraps from last to first item
- **trapped:** Focus cannot leave the scope at all

Also handles:
- Auto-focus on mount
- Return focus on unmount

**afterhours equivalent:** Modal already does focus trapping via input gates.
The auto-focus-on-mount and return-focus-on-unmount behaviors should be
extracted as reusable utilities.

### 3.6 `Presence` — Mount/Unmount with Animation

A state machine with three states: `mounted`, `unmountSuspended`, `unmounted`.

When `present` goes from true → false, instead of immediately unmounting,
it enters `unmountSuspended` and waits for any CSS animation to complete.
This allows exit animations without manual state management.

**afterhours equivalent:** No equivalent.  Entity destruction is immediate.
For floating elements, we'd need a "pending destroy" state that lets the
exit animation complete before actual ECS entity removal.

### 3.7 `Popper` — Floating Positioning

Wraps Floating UI (`@floating-ui/react-dom`) with a Radix-friendly API:

```tsx
<Popper.Root>
  <Popper.Anchor />  // Reference element
  <Popper.Content    // Floating element
    side="bottom"
    sideOffset={8}
    align="center"
    avoidCollisions={true}
    collisionPadding={10}
  >
    <Popper.Arrow />
  </Popper.Content>
</Popper.Root>
```

Internally uses: `offset`, `flip`, `shift`, `limitShift`, `size`, `hide`,
and `arrow` from Floating UI — exactly what's documented in
`floating_ui_analysis.md`.

Used by: Tooltip, Popover, DropdownMenu, ContextMenu, Select, HoverCard,
NavigationMenu, Menubar.

---

## 4. Full Primitives Inventory

### User-Facing Components (30)

| Radix Primitive | Parts | afterhours Status | Notes |
|---|---|---|---|
| **Accordion** | Root, Item, Header, Trigger, Content | Doc: `12_accordion.md` | Collapsible sections |
| **Alert Dialog** | Root, Trigger, Portal, Overlay, Content, Title, Description, Action, Cancel | Has: `modal.h` (DialogType::Alert) | Blocking confirmation dialog |
| **Aspect Ratio** | Root | Gap | Constrains child to aspect ratio (16:9, 4:3, etc.) |
| **Avatar** | Root, Image, Fallback | Doc: `13_avatars_badges.md` | Image with fallback initials |
| **Checkbox** | Root, Indicator | Has: `checkbox()` | |
| **Collapsible** | Root, Trigger, Content | Doc: `12_accordion.md` | Single expand/collapse section |
| **Context Menu** | Root, Trigger, Portal, Content, Item, Group, Label, CheckboxItem, RadioGroup, RadioItem, Sub, SubTrigger, SubContent, Separator | Doc: `24_context_menu.md` | Right-click menu with submenus |
| **Dialog** | Root, Trigger, Portal, Overlay, Content, Title, Description, Close | Has: `modal.h` | Modal dialog |
| **Dropdown Menu** | Root, Trigger, Portal, Content, Item, Group, Label, CheckboxItem, RadioGroup, RadioItem, Sub, SubTrigger, SubContent, Separator | Has (partial): `dropdown()` | afterhours dropdown is select-only, not full menu |
| **Form** | Root, Field, Label, Control, Message, ValidityState, Submit | Gap | Client-side form validation |
| **Hover Card** | Root, Trigger, Portal, Content | Gap | Card preview on hover |
| **Label** | Root | Has (inline) | Labels are built into component configs |
| **Menubar** | Root, Menu, Trigger, Portal, Content, Item, ... (same as Menu) | Gap | Horizontal menu bar (File, Edit, etc.) |
| **Navigation Menu** | Root, List, Item, Trigger, Content, Link, Indicator, Viewport | Has: `navigation_bar()` | Nav with dropdowns |
| **OTP Field** | Root, Input, Separator | Gap | One-time password input |
| **Password Toggle Field** | Root, Input, Toggle, Slot | Gap | Password with show/hide toggle |
| **Popover** | Root, Trigger, Anchor, Portal, Content, Close, Arrow | Gap | Non-modal floating panel |
| **Progress** | Root, Indicator | Has: `progress_bar()`, `circular_progress()` | |
| **Radio Group** | Root, Item, Indicator | Has: `radio_group()` | |
| **Scroll Area** | Root, Viewport, Scrollbar, Thumb, Corner | Has: `overflow = Scroll` | Custom scrollbar styling |
| **Select** | Root, Trigger, Value, Icon, Portal, Content, Viewport, Item, ItemText, ItemIndicator, Group, Label, Separator, ScrollUpButton, ScrollDownButton | Has: `dropdown()` | Full select with groups/scroll |
| **Separator** | Root | Has: `separator()` | |
| **Slider** | Root, Track, Range, Thumb | Has: `slider()` | Note: Radix supports multi-thumb |
| **Switch** | Root, Thumb | Has: `toggle_switch()` | |
| **Tabs** | Root, List, Trigger, Content | Has: `tab_container()` | |
| **Toast** | Provider, Root, Title, Description, Action, Close, Viewport | Has: `toast.h` | |
| **Toggle** | Root | Has: `button()` (implicit) | Pressed/unpressed button |
| **Toggle Group** | Root, Item | Has: `button_group()` | Single/multi select |
| **Toolbar** | Root, Button, Link, Separator, ToggleGroup, ToggleItem | Gap | Toolbar with roving focus |
| **Tooltip** | Provider, Root, Trigger, Portal, Content, Arrow | Doc: `10_tooltip.md` | Hover hint |

### Internal Utilities (14)

| Utility | Purpose | afterhours Equivalent |
|---|---|---|
| `Primitive` | Base element with `asChild` | `_init_component()` |
| `Slot` / `Slottable` | Element merging for `asChild` | N/A — proposed behavior attachment |
| `Collection` | Ordered item tracking | Proposed `composite_group` |
| `RovingFocusGroup` | Arrow-key nav in groups | Proposed `composite_group` |
| `DismissableLayer` | Escape + outside click dismissal | `modal.h` (partial) |
| `FocusScope` | Focus trapping + auto-focus | `modal.h` input gates |
| `Presence` | Animated mount/unmount | No equivalent |
| `Popper` | Floating positioning (wraps Floating UI) | Proposed `floating.h` |
| `Portal` | Render to document.body | N/A (render layers) |
| `Direction` | RTL/LTR provider | N/A |
| `VisuallyHidden` | Accessible hidden content | N/A (no screen readers) |
| `FocusGuards` | Tab-trapping sentinel elements | N/A |
| `Arrow` | SVG arrow/caret element | Would be entity-based |
| `useControllableState` | Controlled/uncontrolled bridge | N/A (always controlled in IMGUI) |

---

## 5. How Radix Themes Adds Styling

The key insight: **Themes wraps Primitives, it doesn't modify them.**

### 5.1 The PropDef System

Themes defines a typed prop definition system for visual properties:

```typescript
const baseButtonPropDefs = {
  size:    { type: 'enum', values: ['1','2','3','4'], default: '2', responsive: true, className: 'rt-r-size' },
  variant: { type: 'enum', values: ['solid','soft','outline','ghost'], default: 'solid', className: 'rt-variant' },
  color:   { ... },
  radius:  { ... },
  loading: { type: 'boolean', className: 'rt-loading' },
};
```

Each prop def maps to a CSS class name.  The `extractProps()` helper reads
the component's props, converts them to `className` + `style`, and strips
them from the forwarded props.

**Result:** `<Button size="3" variant="soft" color="blue">` becomes
`<button class="rt-Button rt-r-size-3 rt-variant-soft" data-accent-color="blue">`.

### 5.2 Responsive Props

Any prop marked `responsive: true` accepts either a value or a breakpoint map:

```tsx
<Button size={{ initial: '2', md: '3', lg: '4' }}>
```

This generates classes like `rt-r-size-2 md:rt-r-size-3 lg:rt-r-size-4`.

**afterhours equivalent:** Resolution-independent sizing via `h720()`, `w1280()`,
`ScreenPercent`.  afterhours doesn't have breakpoint-based responsiveness, but
the resolution scaling achieves a similar goal for TV/console.

### 5.3 Accent Colors via Data Attributes

Themes uses `data-accent-color` attributes rather than inline styles:

```html
<button data-accent-color="blue" class="rt-Button rt-variant-solid">
```

CSS custom properties are then resolved per color:

```css
[data-accent-color="blue"] {
  --accent-1: var(--blue-1);
  --accent-2: var(--blue-2);
  ...
  --accent-12: var(--blue-12);
}
```

This means any element can override its accent color by setting
`data-accent-color`, and all children inherit it.

**afterhours equivalent:** `Theme::Usage` enum + `ComponentConfig::color_usage`.
The Radix approach of having a 12-step color scale per hue is more granular
than afterhours' current primary/secondary/accent system.

### 5.4 Token System

Themes defines design tokens as CSS custom properties:

| Token Category | Examples | afterhours Equivalent |
|---|---|---|
| Color scale | `--gray-1` through `--gray-12` per hue | `Theme` colors (flat, no scale) |
| Space | `--space-1` through `--space-9` | `DefaultSpacing::tiny()` through `::container()` |
| Radius | `--radius-1` through `--radius-6` | `Theme::roundness` (single float) |
| Font size | `--font-size-1` through `--font-size-9` | `FontSizing::small/medium/large/xl` |
| Font weight | `--font-weight-light/regular/medium/bold` | Not tokenized |
| Line height | `--line-height-1` through `--line-height-9` | `TypographyScale::compute_line_height()` |
| Letter spacing | `--letter-spacing-1` through `--letter-spacing-9` | Not tokenized |
| Shadows | `--shadow-1` through `--shadow-6` | `Shadow` struct (per-component) |

### 5.5 Component Variants

Themes provides multiple visual variants per component.  For buttons:

| Variant | Description |
|---|---|
| `classic` | Solid with subtle 3D effect |
| `solid` | Flat solid fill |
| `soft` | Light tinted background |
| `surface` | Translucent with border |
| `outline` | Border only, no fill |
| `ghost-offset` | No border, slight offset on hover |
| `ghost` | No border, no fill — just text |

**afterhours equivalent:** `ButtonVariant` enum (Filled, Outline, Ghost).
Could expand to match Radix's richer set.

---

## 6. Themes-Only Components (Not in Primitives)

These are additional components that Radix Themes adds beyond what Primitives
provides, since they're purely visual (no complex behavior):

| Theme Component | Description | afterhours Status |
|---|---|---|
| **Badge** | Small status label | Gap |
| **Callout** | Alert/info banner with icon | Gap |
| **Card** | Contained surface | Has: `div()` with surface color |
| **Checkbox Cards** | Checkbox styled as selectable cards | Gap |
| **Data List** | Key-value pair display | Gap |
| **Icon Button** | Button with only an icon | Has: `image_button()` |
| **Inset** | Negative margin content area | N/A (niche) |
| **Kbd** | Keyboard shortcut display | Gap |
| **Radio Cards** | Radio styled as selectable cards | Gap |
| **Segmented Control** | Toggle group styled as segments | Has: `button_group()` (close) |
| **Skeleton** | Loading placeholder | Doc: `32_loading_states.md` |
| **Spinner** | Loading animation | Gap |
| **Table** | Data table | Doc: `25_list_table.md` |
| **Tab Nav** | Tabs as navigation links | Has: `navigation_bar()` |
| **Text Field** | Styled text input | Has: `text_input()` |
| **Text Area** | Styled multiline input | Has: `text_area()` |
| **Box / Flex / Grid / Container / Section** | Layout primitives | Has: `div()` with flex |
| **Text / Heading / Blockquote / Code / Em / Strong / Link / Quote** | Typography primitives | Has: `div()` with labels |

---

## 7. What afterhours Should Adopt

### 7.1 Animated Presence (Mount/Unmount Lifecycle)

**Problem:** When a floating element (dropdown, tooltip, popover) closes,
afterhours immediately destroys or hides the entity.  There's no way to play
an exit animation before removal.

**Recommendation:** Add a `Presence`-like lifecycle to entities:

```cpp
enum class PresenceState { Mounted, Animating_Out, Unmounted };

// When hiding a floating element:
// 1. Set state to Animating_Out
// 2. Start exit animation
// 3. When animation completes, set state to Unmounted
// 4. Entity is actually removed/hidden
```

This would integrate with the existing `AnimTrigger` system — add an
`AnimTrigger::OnDismiss` that fires when an element starts closing.

### 7.2 Dismissable Layer Stack

**Problem:** If a popover opens a dialog which opens a tooltip, pressing Escape
should close only the topmost layer.  afterhours modal handles this for modals,
but there's no generalized layer stack for non-modal floaters.

**Recommendation:** Create a `DismissableLayer` utility:

```cpp
struct DismissableStack {
    // Stack of open dismissable elements (topmost = back)
    std::vector<EntityID> layers;

    void push(EntityID id);
    void remove(EntityID id);
    EntityID topmost() const;

    // Only the topmost layer receives dismiss events
    bool should_dismiss(EntityID id, DismissReason reason) const;
};

enum class DismissReason { EscapeKey, OutsideClick, FocusOutside };
```

### 7.3 Collection / Ordered Item Tracking

Both Radix and Base UI independently build this same utility.  It's clearly
essential for any compound widget system.

**Recommendation:** This reinforces the `composite_group` recommendation from
`base_ui_architecture_analysis.md`.  Two major libraries converging on the
same pattern is strong validation.

### 7.4 Richer Color Scale

**Problem:** afterhours has one color per semantic role (primary, secondary, etc.).
Themes has 12 steps per hue, enabling subtle state variations (hover slightly
lighter, active slightly darker, disabled very light, etc.)

**Recommendation:** Expand the theme to support a multi-step scale:

```cpp
struct ColorScale {
    // 12 steps from lightest to darkest
    // Steps 1-2: backgrounds
    // Steps 3-5: interactive backgrounds (hover, active)
    // Steps 6-8: borders
    // Steps 9: solid backgrounds (buttons)
    // Steps 10: solid hover
    // Steps 11-12: text
    std::array<Color, 12> steps;

    Color bg() const { return steps[0]; }
    Color bg_subtle() const { return steps[1]; }
    Color hover() const { return steps[3]; }
    Color active() const { return steps[4]; }
    Color border() const { return steps[6]; }
    Color solid() const { return steps[8]; }
    Color solid_hover() const { return steps[9]; }
    Color text() const { return steps[10]; }
    Color text_high() const { return steps[11]; }

    // Auto-generate from a single seed color
    static ColorScale from_seed(Color seed);
};
```

This would replace the current pattern of manually calling `colors::darken()`
and `colors::lighten()` scattered through component code.

### 7.5 Component Variant Expansion

Radix Themes offers 7 button variants vs afterhours' 3.  The additional
variants are useful:

| Variant | Use Case | Mapped to afterhours |
|---|---|---|
| Filled/Solid | Primary actions | Has: `ButtonVariant::Filled` |
| Outline | Secondary actions | Has: `ButtonVariant::Outline` |
| Ghost | Tertiary/inline actions | Has: `ButtonVariant::Ghost` |
| **Soft** | Subtle highlighted actions | Gap — would be tinted transparent bg |
| **Surface** | Card-like buttons | Gap — would be surface color + border |
| **Classic** | Slightly 3D/raised | Gap — could use `BevelBorder` |

### 7.6 PropDef-Style Typed Configuration

**Problem:** `ComponentConfig` is a single struct with ~50 fields.  Some fields
only apply to certain components (e.g., `mask_char` only for text input,
`checkbox_checked_indicator` only for checkbox).  There's already a TODO about
this in the source.

**Radix's approach:** Each component declares its own prop definitions with
specific types and defaults.  The `extractProps` function processes only
the props that are defined, with type safety.

**Recommendation:** This is already noted as a TODO in `component_config.h`.
The Radix PropDef pattern validates the approach — concept-constrained configs
per component type.

---

## 8. Updated Component Gap Analysis

Combining Radix's inventory with the Base UI analysis from
`base_ui_architecture_analysis.md`:

### New Components from Radix (not in Base UI list)

| Component | Priority | Notes |
|---|---|---|
| **Aspect Ratio** | Low | Simple — constrain entity to ratio |
| **Badge** | Medium | Status labels (common in game UIs: "NEW", "SALE", "RARE") |
| **Callout** | Low | Info/warning banner |
| **Card** | Low | Already achievable with `div()` + surface color |
| **Data List** | Medium | Key-value display (stats screens, inventory details) |
| **Kbd** | Low | Keyboard shortcut display |
| **OTP Field** | Low | Niche |
| **Password Toggle Field** | Low | Text input + show/hide already possible |
| **Skeleton** | Medium | Loading placeholders (tracked in `32_loading_states.md`) |
| **Spinner** | Medium | Loading animation |
| **Segmented Control** | Has | `button_group()` covers this |

### Updated Priority List (all sources combined)

**High:**
1. Combobox / Autocomplete (Base UI)
2. Popover (Base UI + Radix)
3. Toolbar (Base UI + Radix)
4. Floating positioning engine (Floating UI → enables Popover, Tooltip, etc.)

**Medium:**
5. Tooltip (Radix — already tracked in `10_tooltip.md`)
6. Number Field (Base UI)
7. Drawer (Base UI)
8. Badge (Radix Themes)
9. Menubar (Base UI + Radix)
10. Collapsible / Accordion (Radix — tracked in `12_accordion.md`)
11. Skeleton / Spinner (Radix Themes)
12. Context Menu (Radix — tracked in `24_context_menu.md`)

**Low:**
13. Field / Fieldset / Form (Base UI + Radix)
14. Hover Card (Radix)
15. Data List (Radix Themes)
16. Aspect Ratio, Kbd, Callout (Radix)

---

## 9. Key Takeaways: Radix vs Base UI vs afterhours

| Aspect | Radix | Base UI | afterhours |
|---|---|---|---|
| **Element delegation** | `asChild` prop (boolean) | `render` prop (element or function) | Proposed: `behavior(ctx, entity)` functions |
| **State exposure** | `data-state`, `data-*` attributes | `data-*` attributes + state callbacks | ECS components (structurally equivalent) |
| **Positioning** | Wraps Floating UI | Wraps Floating UI | Proposed: native port of Floating UI core |
| **Focus management** | `FocusScope` + `FocusGuards` | `FloatingFocusManager` | `modal.h` input gates |
| **Dismiss handling** | `DismissableLayer` (stacking) | `useDismiss` hook | `modal.h` (single layer) |
| **Item collection** | `Collection` utility | `CompositeList` | Proposed: `composite_group` |
| **Roving focus** | `RovingFocusGroup` | `Composite` | Proposed: `composite_group` |
| **Animated presence** | `Presence` state machine | `useTransitionStatus` | No equivalent — needs lifecycle |
| **Controlled/uncontrolled** | `useControllableState` | `useControlled` | Always controlled (IMGUI) |
| **Styling tokens** | 12-step color scales, space/radius/font tokens | Data attributes + CSS variables | `Theme` + `DefaultSpacing` + `TypographyScale` |
| **Component variants** | 7 button variants, responsive size props | Minimal styling (unstyled) | 3 button variants |

### Patterns Both Libraries Agree On (Strong Signal)

When two independent, widely-used libraries converge on the same pattern,
it's a strong signal that the pattern is correct:

1. **Compound decomposition into named parts** — Both do this for every component
2. **Ordered child collection tracking** — `Collection` (Radix) ≈ `CompositeList` (Base UI)
3. **Roving focus as a shared primitive** — `RovingFocusGroup` (Radix) ≈ `Composite` (Base UI)
4. **Dismissable layer with stacking** — `DismissableLayer` (Radix) ≈ `useDismiss` (Base UI)
5. **Floating UI for positioning** — Both use it as their positioning engine
6. **Element delegation** — `asChild` (Radix) ≈ `render` (Base UI)
7. **State via data attributes** — Both expose component state this way
8. **Animated mount/unmount lifecycle** — `Presence` (Radix) ≈ `useTransitionStatus` (Base UI)

These 8 patterns should be treated as the canonical requirements for any
serious component system.  afterhours currently implements #7 (via ECS) and
partially implements #4 (via modal).  The remaining 6 are the roadmap.

---

## Summary

Radix adds three insights beyond what we learned from Base UI:

1. **The two-layer split is validated** — Primitives (behavior) and Themes
   (styling) as separate packages confirms that afterhours should keep
   behavior primitives independent of any specific theme or game's visual style.

2. **Animated presence is essential** — Both Radix and Base UI solve animated
   mount/unmount, which afterhours currently lacks.  This is needed before
   floating elements (tooltips, popovers, dropdowns) can feel polished.

3. **Multi-step color scales** — Radix's 12-step color system is more
   practical than "one color per role" for building state-dependent UIs.
   A `ColorScale::from_seed()` generator would eliminate manual
   `darken()`/`lighten()` calls throughout component code.

The convergence of Radix and Base UI on the same 8 core patterns gives high
confidence in the roadmap outlined across these analysis docs.
