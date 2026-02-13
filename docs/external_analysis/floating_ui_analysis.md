# Floating UI Analysis

What Floating UI provides, how its architecture works, and what afterhours
should adopt for popover/tooltip/dropdown positioning.

Repository: https://github.com/floating-ui/floating-ui

---

## 1. What Floating UI Is

Floating UI is a **positioning engine** for "floating" elements — any UI that
hovers near a reference element: tooltips, popovers, dropdowns, context menus,
autocomplete lists, date pickers, etc.

It is **not** a component library.  It provides the math to answer one question:

> Given a reference rectangle and a floating rectangle, what (x, y) should
> the floating element be placed at, and what adjustments are needed to keep
> it visible?

The library is split into layers:

| Package | Purpose |
|---|---|
| `@floating-ui/core` | Pure math — platform-agnostic positioning + middleware pipeline |
| `@floating-ui/utils` | Geometry helpers (placement types, rect math, side/alignment utilities) |
| `@floating-ui/dom` | DOM platform adapter (measures elements, finds clipping ancestors) |
| `@floating-ui/react-dom` | React bindings for DOM positioning |
| `@floating-ui/react` | Full interaction layer — hover, focus, dismiss, list navigation, transitions |
| `@floating-ui/react-native` | React Native platform adapter |

The key insight: **the core positioning math has zero platform dependencies**.
It works via a `Platform` interface that you implement for your environment.
React Native already does this.  afterhours can too.

---

## 2. Core Concepts

### 2.1 Placement

A placement is a side + optional alignment:

```
         top-start      top       top-end
              +------------------+
  left-start  |                  |  right-start
  left        |    reference     |  right
  left-end    |                  |  right-end
              +------------------+
       bottom-start   bottom   bottom-end
```

12 possible placements:
- 4 sides: `top`, `right`, `bottom`, `left`
- 8 aligned: `top-start`, `top-end`, `right-start`, `right-end`, etc.

### 2.2 The Middleware Pipeline

Positioning is a pipeline:

```
computePosition(reference, floating, {
    placement: 'bottom',
    middleware: [offset(8), flip(), shift(), arrow({element: arrowEl})],
})
→ { x, y, placement, middlewareData }
```

Each middleware receives the current state (`x`, `y`, `placement`, `rects`,
`platform`) and can:
1. **Adjust x/y** — nudge the position
2. **Change placement** — flip to a different side
3. **Return data** — pass info to later middleware or to rendering
4. **Reset the pipeline** — trigger a full recalculation (e.g., after flipping)

This is a composable, order-dependent chain.  Users pick only the behaviors
they need.

### 2.3 The Platform Interface

The `Platform` interface abstracts all environment-specific measurements:

```typescript
interface Platform {
  // Required:
  getElementRects(args)   → { reference: Rect, floating: Rect }
  getClippingRect(args)   → Rect  (the visible viewport/scroll area)
  getDimensions(element)  → { width, height }

  // Optional:
  getOffsetParent(element) → Element
  isElement(value)         → boolean
  isRTL(element)           → boolean
  getScale(element)        → { x, y }
}
```

**This is how afterhours would integrate** — implement `Platform` using the
ECS layout system's computed rectangles instead of DOM measurements.

---

## 3. Middleware Catalog

### 3.1 `offset` — Gap Between Reference and Floating

Adds distance (gutter) between the reference and floating element.

```
offset(8)            // 8px gap on main axis
offset({             // Detailed control:
  mainAxis: 8,       //   gap along the side
  crossAxis: 0,      //   shift along the alignment
  alignmentAxis: 4,  //   alignment-aware shift
})
```

**afterhours need:** Essential.  Every tooltip/popover needs a configurable gap
so the floating element doesn't touch the reference.

### 3.2 `flip` — Change Side When Overflowing

If the floating element overflows the viewport on the preferred side, flip it
to the opposite side (or try a sequence of fallback placements).

```
flip()                              // Flip to opposite side
flip({
  fallbackPlacements: ['top', 'left'],  // Try these in order
  fallbackStrategy: 'bestFit',          // or 'initialPlacement'
})
```

Algorithm:
1. Detect overflow on current placement
2. Try opposite placement
3. Try expanded placements (with alignment variations)
4. Fall back to best-fit or initial placement

**afterhours need:** Critical.  Game UIs often have elements near screen edges
(HUDs, status bars).  Dropdowns that open downward need to flip upward when
near the bottom of the screen.  afterhours `dropdown()` currently always opens
downward with `with_translate(pixels(0), config.size.y_axis)` — no overflow
detection.

### 3.3 `shift` — Slide Along Axis to Stay Visible

If the floating element overflows the viewport on the cross axis, slide it
along that axis to stay in view (without changing the side).

```
shift()                    // Shift on main axis
shift({
  crossAxis: true,         // Also shift on cross axis
  limiter: limitShift(),   // Prevent detachment from reference
})
```

`limitShift()` prevents the floating element from sliding so far that it
no longer visually relates to its reference.

**afterhours need:** Important.  A tooltip near the left edge should slide
right to stay visible, while still pointing at the reference.

### 3.4 `size` — Resize Floating Element to Fit

Provides `availableWidth` and `availableHeight` so the floating element can
shrink to fit within the viewport.

```
size({
  apply({ availableWidth, availableHeight, elements }) {
    elements.floating.style.maxHeight = `${availableHeight}px`;
    elements.floating.style.maxWidth = `${availableWidth}px`;
  }
})
```

**afterhours need:** Very useful.  A dropdown with many options near the bottom
of the screen should shrink its max height rather than overflow off-screen.
This is exactly the behavior needed for combobox, select, and context menu.

### 3.5 `arrow` — Position a Pointer Arrow

Calculates the (x, y) position for a triangular arrow/caret element that
points from the floating element to the reference.

```
arrow({ element: arrowEl, padding: 8 })
// Returns: middlewareData.arrow = { x, y, centerOffset }
```

The arrow stays centered on the reference, clamped within the floating
element's bounds (respecting padding for rounded corners).

**afterhours need:** Nice-to-have for tooltips and popovers.  Would require
rendering a small triangle entity at the computed position.

### 3.6 `autoPlacement` — Find Best Side Automatically

Instead of specifying a preferred side and flipping, let the algorithm pick
the side with the most available space.

```
autoPlacement()                          // Try all 12 placements
autoPlacement({
  allowedPlacements: ['top', 'bottom'],  // Restrict to vertical
})
```

**afterhours need:** Low priority.  `flip()` with fallbacks covers most cases.
`autoPlacement` is useful for context menus where there's no "preferred" side.

### 3.7 `hide` — Detect When Floating Should Be Hidden

Detects two conditions:
- **referenceHidden:** The reference element has scrolled out of view
- **escaped:** The floating element has overflowed its clipping boundary

Returns boolean flags so the UI can hide or fade the floating element.

```
hide({ strategy: 'referenceHidden' })
// Returns: middlewareData.hide.referenceHidden = true/false
```

**afterhours need:** Useful for scroll views.  If a dropdown's anchor scrolls
out of a `HasScrollView` container, the dropdown should auto-close or hide.

### 3.8 `inline` — Handle Multi-Line References

For references that span multiple lines (e.g., hyperlinks wrapping), picks the
most relevant client rect.

**afterhours need:** Not applicable.  afterhours elements are always single
rectangles.

---

## 4. Interaction Layer (React Package)

Beyond positioning, Floating UI's React package provides interaction behaviors.
These are the "how does the floating element open/close/navigate" pieces:

### 4.1 Interaction Hooks

| Hook | What It Does | afterhours Relevance |
|---|---|---|
| `useHover` | Open on hover with configurable delay, close delay, and "move" behavior | Medium — game UIs can use hover for tooltips |
| `useFocus` | Open on focus, close on blur | Medium — keyboard-driven tooltip/popover |
| `useClick` | Toggle open/close on click | High — dropdown/popover triggers |
| `useDismiss` | Close on Escape key or outside click/press | High — essential for any popup; afterhours modal already does this |
| `useListNavigation` | Arrow-key navigation within a floating list (menu, select, combobox) | High — this is the dropdown/menu keyboard nav |
| `useTypeahead` | Type-ahead character matching in a list | Medium — useful for long dropdown lists |
| `useRole` | Sets ARIA role attributes | Not applicable — no DOM/ARIA |
| `useClientPoint` | Position floating element at mouse cursor | Medium — context menu positioning |
| `useTransition` | Open/close transition states (mounted → initial → open → close → unmounted) | Medium — animation lifecycle |

### 4.2 Components

| Component | What It Does | afterhours Relevance |
|---|---|---|
| `FloatingFocusManager` | Focus trapping + initial focus + return focus | High — afterhours modal already implements this; could be extracted |
| `FloatingOverlay` | Backdrop/overlay that blocks interaction | Has — `modal.h` backdrop |
| `FloatingPortal` | Renders to document.body (escapes parent clipping) | N/A — afterhours uses render layers |
| `FloatingArrow` | Renders the arrow/caret element | Medium — if tooltips get arrows |
| `FloatingList` | Manages ordered list registration | High — maps to proposed `composite_group` |
| `FloatingTree` | Manages nested floating element hierarchy (submenus) | Medium — needed for nested menus/submenus |
| `FloatingDelayGroup` | Shares hover delay across grouped tooltips | Low — UX polish |
| `Composite` | Roving tabindex composite widget | High — same as Base UI's Composite |
| `FocusGuard` | Invisible elements that trap tab focus | N/A — afterhours uses input actions not tab trapping |

### 4.3 Safe Polygon (Safe Triangle)

When hovering from a trigger to a floating menu, the user's cursor may briefly
leave both elements.  `safePolygon()` constructs a triangular "safe zone"
between the trigger and the floating element — if the cursor stays inside this
polygon, the floating element stays open.

Algorithm:
1. Get the cursor's current position
2. Get the floating element's rectangle
3. Construct a polygon from the cursor point to two corners of the floating
   rect (forming a triangle/trapezoid)
4. On `mousemove`, check if the cursor is inside the polygon
5. If outside, close the floating element

**afterhours need:** Medium.  Relevant if implementing hover-triggered submenus
or hover-popovers.  For gamepad-only UIs, not needed.

---

## 5. What afterhours Should Adopt

### 5.1 Core Positioning Engine (High Priority)

Create `ui/floating.h` — a pure C++ port of Floating UI's core concepts:

```cpp
namespace afterhours::ui::floating {

// Placement types
enum class Side { Top, Right, Bottom, Left };
enum class Alignment { Start, Center, End };
struct Placement { Side side; Alignment alignment = Alignment::Center; };

// The core function
struct PositionResult {
    float x, y;
    Placement final_placement;
    // Per-middleware data
    struct { float x, y; float center_offset; } arrow;
    struct { bool reference_hidden; bool escaped; } hide;
    float available_width, available_height;
};

// Platform adapter — afterhours implements this using UIComponent rects
struct Platform {
    // Get reference and floating element rects from the layout system
    RectangleType get_rect(Entity& entity);
    // Get the visible viewport/clipping rect
    RectangleType get_clipping_rect(Entity& entity);
};

// Middleware interface
struct Middleware {
    virtual void apply(PositionState& state) = 0;
};

// Built-in middleware
struct Offset : Middleware { float main_axis; float cross_axis; };
struct Flip : Middleware { std::vector<Placement> fallbacks; };
struct Shift : Middleware { bool main_axis; bool cross_axis; };
struct Size : Middleware { /* provides available_width/height */ };
struct Arrow : Middleware { Entity* arrow_entity; float padding; };
struct Hide : Middleware { /* detects reference_hidden / escaped */ };

// Compute position
PositionResult compute_position(
    Platform& platform,
    Entity& reference,
    Entity& floating,
    Placement preferred,
    std::vector<Middleware*> middleware
);

} // namespace
```

This is ~300-500 lines of pure math, no dependencies beyond rectangles.  The
React Native platform adapter shows this is straightforward for non-DOM
environments.

### 5.2 Integrate with Existing Components (High Priority)

Wire the positioning engine into existing and future components:

| Component | Current Positioning | With Floating Engine |
|---|---|---|
| `dropdown()` | `with_translate(0, height)` — always below | `compute_position` with `flip()` + `size()` — opens above if no room below, shrinks if needed |
| `tooltip` (planned) | N/A | `compute_position` with `offset(8)` + `flip()` + `shift()` |
| `popover` (planned) | N/A | `compute_position` with `offset()` + `flip()` + `shift()` + `size()` |
| `context_menu` (planned) | N/A | `compute_position` with `autoPlacement()` positioned at cursor |
| `combobox` (planned) | N/A | Same as dropdown but with text input trigger |

### 5.3 Overflow Detection Utility (High Priority)

Port `detectOverflow()` — this is useful beyond just floating elements:

```cpp
// Returns how much an element overflows its clipping boundary on each side
// Positive = overflowing, negative = room left, 0 = flush
struct OverflowSides { float top, right, bottom, left; };

OverflowSides detect_overflow(
    RectangleType element_rect,
    RectangleType clipping_rect,
    Padding padding = {}
);
```

This function is also useful for:
- Validation (warn when elements overflow their parent)
- Scroll view indicators (show scroll arrows when content overflows)
- Layout debugging (highlight overflowing elements)

### 5.4 Placement Computation Helpers (High Priority)

Port `computeCoordsFromPlacement()` — pure geometry:

```cpp
// Given two rects and a placement, compute where the floating rect should go
Vector2Type compute_coords(
    RectangleType reference,
    RectangleType floating,
    Placement placement
);
```

This is ~50 lines and immediately useful for positioning any element relative
to any other element.

### 5.5 Dismiss Behavior (Medium Priority)

Extract a reusable "dismiss" behavior for floating elements:

```cpp
struct DismissBehavior {
    bool escape_key = true;        // Close on Escape
    bool outside_click = true;     // Close on click outside
    bool reference_scroll = false; // Close when reference scrolls away

    // Check if dismiss should fire this frame
    bool should_dismiss(UIContext& ctx, Entity& floating, Entity& reference);
};
```

afterhours `modal.h` already implements escape + backdrop click dismissal.
This would extract it into a reusable utility for non-modal floaters.

### 5.6 Hover Intent / Safe Polygon (Low Priority)

For mouse-driven UIs with hover menus, implement the safe polygon algorithm:

```cpp
// Given cursor position, reference rect, and floating rect,
// returns true if cursor is in the safe zone between them
bool is_in_safe_polygon(
    Vector2Type cursor,
    RectangleType reference,
    RectangleType floating,
    float buffer = 0.5f
);
```

This prevents menus from closing when the user moves diagonally from the
trigger to the floating panel.

### 5.7 Transition Lifecycle (Low Priority)

Floating UI provides a 4-state transition lifecycle:

```
unmounted → initial → open → close → unmounted
```

afterhours already has `AnimTrigger::OnAppear` and animation support.
The addition would be an `on_dismiss` animation trigger so floating elements
can animate out before being destroyed.

---

## 6. Implementation Phases

### Phase 1: Core Positioning (can land immediately)

1. Port `computeCoordsFromPlacement()` — 50 lines
2. Port `detectOverflow()` — 30 lines
3. Implement `offset` middleware — 40 lines
4. Implement `flip` middleware — 100 lines
5. Implement `shift` middleware — 50 lines
6. Wire into a `compute_position()` pipeline — 50 lines
7. **Total: ~320 lines of header-only C++**

### Phase 2: Integration

1. Create `floating_element()` component function that wraps `compute_position`
2. Refactor `dropdown()` to use floating positioning instead of hardcoded translate
3. Build `tooltip()` using the floating engine (covers `docs/10_tooltip.md`)
4. Build `popover()` using the floating engine

### Phase 3: Advanced Middleware

1. Implement `size` middleware (constrain floating element dimensions)
2. Implement `arrow` middleware (position pointer arrow)
3. Implement `hide` middleware (detect reference scrolled away)

### Phase 4: Interaction Behaviors

1. Extract dismiss behavior from `modal.h` into reusable utility
2. Add hover intent / safe polygon for mouse-driven menus
3. Add transition lifecycle (animate-out before destroy)

---

## 7. What NOT to Port

| Feature | Why Skip |
|---|---|
| DOM platform adapter | afterhours has its own layout system — we implement Platform directly |
| `inline` middleware | For multi-line text selections; afterhours elements are always single rects |
| `useRole` hook | ARIA attributes — not applicable to native UI |
| `FloatingPortal` | DOM portal to escape clipping contexts — afterhours uses render layers |
| `FocusGuard` | Invisible DOM elements for tab trapping — afterhours uses input actions |
| `getOverflowAncestors` | DOM-specific scroll ancestor detection — afterhours has `HasScrollView` |
| RTL support | Could add later, but not a priority for most game UIs |

---

## 8. Comparison: Current dropdown vs. Floating-Aware dropdown

### Current (hardcoded positioning):

```cpp
// From imm_components.h line 1341:
.with_absolute_position()
.with_translate(pixels(0), config.size.y_axis)  // Always below
.with_hidden(config.hidden || !dropdownState.on)
.with_render_layer(config.render_layer + 1)
```

Problems:
- Opens below even if there's no room (overflows off-screen)
- No shrinking if the list is too tall
- No repositioning if reference is near edge

### Proposed (floating-aware):

```cpp
auto pos = floating::compute_position(
    platform,
    trigger_entity,          // The dropdown button
    list_entity,             // The options list
    Placement{Side::Bottom}, // Prefer below
    {
        floating::offset(4),    // 4px gap
        floating::flip(),       // Flip above if no room below
        floating::size(),       // Shrink max-height to fit
        floating::shift(),      // Slide horizontally if needed
    }
);

// Apply computed position
list_entity.get<UIComponent>().set_absolute_position(pos.x, pos.y);
if (pos.available_height > 0) {
    list_entity.get<UIComponent>().set_max_height(pos.available_height);
}
```

---

## 9. Cross-Reference with Existing Docs

| Doc | How Floating UI Helps |
|---|---|
| `docs/10_tooltip.md` | Tooltip is the primary use case — `offset` + `flip` + `shift` |
| `docs/24_context_menu.md` | Context menu positioned at cursor — `autoPlacement` or `flip` + `shift` |
| `docs/31_overlay_panels.md` | Overlay positioning relative to triggers |
| `docs/base_ui_architecture_analysis.md` | Popover and Combobox both need floating positioning |
| `docs/12_accordion.md` | Not directly related, but collapsible content affects available space for floaters above |

---

## Summary

Floating UI solves a universal problem: **positioning element B relative to
element A while keeping B visible on screen**.  Its architecture is:

1. **Platform-agnostic core** — pure rect math with a `Platform` interface
2. **Composable middleware** — pick only the behaviors you need
3. **Interaction layer** — hover/focus/dismiss/navigation (React-specific, but
   the behaviors are generic)

For afterhours, the highest-value work is:
- **Port the ~320 lines of core positioning math** (placement computation,
  overflow detection, offset/flip/shift middleware)
- **Wire it into `dropdown()`** to fix the "always opens below" problem
- **Use it as the foundation for tooltip, popover, context menu, and combobox**

The middleware pipeline pattern is particularly elegant — rather than one giant
function with many boolean flags, each positioning behavior is an independent,
composable unit.
