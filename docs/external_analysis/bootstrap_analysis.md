# Bootstrap Analysis

What Bootstrap provides, how it approaches component design and styling, and
what afterhours can learn from the most widely-deployed UI framework on the web.

Repository: https://github.com/twbs/bootstrap
Docs: https://getbootstrap.com/docs/5.3/

---

## 1. What Bootstrap Is

Bootstrap is an **opinionated, styled** component framework.  Unlike Radix
(unstyled primitives) or Base UI (unstyled with hooks), Bootstrap ships with
complete visual designs out of the box.  It's CSS-first — the component
library is primarily a CSS framework with a thin JavaScript layer for
interactive behavior.

This makes it a useful comparison point: while Radix/Base UI show how to build
*customizable primitives*, Bootstrap shows what a **complete, ready-to-use
design system** looks like when fully realized.

---

## 2. Architecture

### 2.1 CSS-First, JS-Light

Bootstrap's components are defined primarily in SCSS.  Only 14 components
have JavaScript behavior:

| JS Component | Behavior |
|---|---|
| Alert | Dismissible (close button) |
| Button | Toggle state |
| Carousel | Slide/fade between panels |
| Collapse | Expand/collapse content |
| Dropdown | Toggle menu, keyboard nav, Popper positioning |
| Modal | Open/close, backdrop, focus trap, escape key |
| Offcanvas | Slide-in panel (drawer), backdrop, focus trap |
| Popover | Positioned floating content, hover/click triggers |
| Scrollspy | Track scroll position, highlight nav |
| Tab | Switch tab panels |
| Toast | Auto-dismiss notification |
| Tooltip | Positioned hover hint |

Everything else (Badge, Breadcrumb, Card, List Group, Pagination, Progress,
Spinner, etc.) is pure CSS — no JavaScript required.

### 2.2 BaseComponent Class

All JS components inherit from `BaseComponent`:

```javascript
class BaseComponent extends Config {
    constructor(element, config) {
        this._element = element;
        this._config = this._getConfig(config);
        Data.set(this._element, this.constructor.DATA_KEY, this);
    }
    dispose() { /* cleanup */ }
    static getInstance(element) { /* lookup by element */ }
    static getOrCreateInstance(element, config) { /* singleton per element */ }
}
```

Key pattern: **one component instance per DOM element**, looked up via a data
store keyed by element.  This is analogous to afterhours's ECS — one set of
components per entity.

### 2.3 Event Lifecycle

Every interactive component follows the same event pattern:

```
show → shown → hide → hidden
```

Events are cancellable — calling `event.preventDefault()` on `show` or `hide`
stops the state change.  The `shown`/`hidden` events fire after transitions
complete.

```javascript
EVENT_SHOW    = 'show.bs.modal'     // Before opening (cancellable)
EVENT_SHOWN   = 'shown.bs.modal'    // After open animation completes
EVENT_HIDE    = 'hide.bs.modal'     // Before closing (cancellable)
EVENT_HIDDEN  = 'hidden.bs.modal'   // After close animation completes
```

**afterhours equivalent:** No standard lifecycle.  `HasClickListener` fires
immediately.  The proposed `EventDetails` struct from
`base_ui_architecture_analysis.md` would bring cancellable events.  The
`show/shown/hide/hidden` pattern maps to the animated presence lifecycle
identified in `radix_ui_analysis.md`.

### 2.4 Data Attributes for Configuration

Bootstrap uses `data-bs-*` attributes for declarative configuration:

```html
<button data-bs-toggle="modal" data-bs-target="#myModal">Open</button>
<div data-bs-spy="scroll" data-bs-target="#navbar">...</div>
```

No JavaScript needed for basic usage — Bootstrap auto-discovers elements
with `data-bs-toggle` on page load.

### 2.5 Popper.js Integration

Dropdown, Tooltip, and Popover all use Popper.js (the predecessor to
Floating UI) for positioning.  This confirms the pattern seen across all
libraries: **floating element positioning is a solved problem that every
framework delegates to a positioning engine.**

---

## 3. Full Component Inventory

### Components (24)

| Bootstrap Component | Type | afterhours Status | Notes |
|---|---|---|---|
| **Accordion** | Interactive (Collapse) | Doc: `12_accordion.md` | Collapse with accordion parent constraint |
| **Alerts** | CSS + dismiss | Gap (partial) | Callout/banner with close button. Toast covers similar need. |
| **Badge** | CSS only | Gap | Small status label ("New", "3", "Sale") |
| **Breadcrumb** | CSS only | Gap | Navigation trail (Home > Category > Page) |
| **Buttons** | CSS + toggle | Has: `button()` | Solid, outline, link, sizes, disabled, active |
| **Button Group** | CSS only | Has: `button_group()` | Horizontal/vertical groups, toolbar |
| **Card** | CSS only | Has: `div()` | Header, body, footer, image, overlays |
| **Carousel** | Interactive | Gap | Slide/crossfade between panels (splash screens, tutorials) |
| **Close Button** | CSS only | Has (inline) | X button — built into modal |
| **Collapse** | Interactive | Doc: `12_accordion.md` | Show/hide content with animation |
| **Dropdowns** | Interactive | Has: `dropdown()` | Uses Popper. Up/down/start/end directions. |
| **List Group** | CSS + interactive | Doc: `25_list_table.md` | Styled list with badges, active/disabled states |
| **Modal** | Interactive | Has: `modal.h` | Static/scroll/centered, backdrop, focus trap |
| **Navbar** | CSS + collapse | Has: `navigation_bar()` | Responsive nav with collapse at breakpoints |
| **Navs & Tabs** | CSS + tab | Has: `tab_container()` | Tabs, pills, vertical, fill/justify |
| **Offcanvas** | Interactive | Gap | Slide-in drawer from any edge. Has backdrop + focus trap. |
| **Pagination** | CSS only | Has: `pagination()` | Page navigation |
| **Placeholders** | CSS only | Doc: `32_loading_states.md` | Skeleton/shimmer loading state |
| **Popovers** | Interactive | Gap | Floating content panel (uses Popper) |
| **Progress** | CSS only | Has: `progress_bar()` | Bar, striped, animated, stacked |
| **Scrollspy** | Interactive | Gap | Scroll-aware nav highlighting |
| **Spinners** | CSS only | Gap | Loading indicator (border/grow variants) |
| **Toasts** | Interactive | Has: `toast.h` | Dismissible, auto-hide, stacking |
| **Tooltips** | Interactive | Doc: `10_tooltip.md` | Hover hint (uses Popper) |

### Forms (9 categories)

| Bootstrap Form | afterhours Status | Notes |
|---|---|---|
| **Form Control** (text, email, password, etc.) | Has: `text_input()` | Size variants, readonly, disabled, plaintext |
| **Select** | Has: `dropdown()` | Native select with Bootstrap styling |
| **Checks & Radios** | Has: `checkbox()`, `radio_group()` | Checkbox, radio, switch, inline |
| **Range** (slider) | Has: `slider()` | Min/max/step, horizontal only |
| **Input Group** | Gap | Prepend/append text, buttons, or dropdowns to inputs |
| **Floating Labels** | Gap | Labels that float above input on focus |
| **Form Layout** | Has: `div()` with flex | Grid-based form layout |
| **Validation** | Has: `validation_config.h` | Valid/invalid states, custom messages |
| **File Input** | N/A | Not applicable to native UI |

### Layout (8 concepts)

| Bootstrap Layout | afterhours Status | Notes |
|---|---|---|
| **Breakpoints** | N/A (uses resolution scaling) | xs/sm/md/lg/xl/xxl (576-1400px) |
| **Containers** | Has: `div()` | Fixed/fluid width containers |
| **Grid** (12-column) | Has: `div()` with flex | Row/column system |
| **Columns** | Has: percent sizing | Auto, equal, variable width |
| **Gutters** | Has: margins/padding | Horizontal/vertical gap |
| **Z-index** | Has: render layers | Layered z-index system |
| **CSS Grid** | N/A | CSS Grid layout (native CSS feature) |
| **Stacks** | Has: `FlexDirection` | Vertical/horizontal stack helpers |

### Utilities (20 categories)

| Bootstrap Utility | afterhours Equivalent | Notes |
|---|---|---|
| Background | `with_background()`, `with_custom_background()` | Color, gradient, opacity |
| Borders | `with_border()` | Color, width, radius, individual sides |
| Colors | Theme colors | Text/background semantic colors |
| Display | `ShouldHide` | Show/hide, responsive visibility |
| Flex | `FlexDirection`, `JustifyContent`, `AlignItems` | Full flex utilities |
| Float | N/A | CSS float (web-specific) |
| Interactions | `SkipWhenTabbing`, `disabled` | Pointer-events, user-select |
| Opacity | `HasOpacity` | 0/25/50/75/100% |
| Overflow | `Overflow` enum | Visible/hidden/scroll per axis |
| Position | `is_absolute`, `with_translate()` | Static/relative/absolute/fixed/sticky |
| Shadows | `Shadow` struct | None/sm/default/lg |
| Sizing | `ComponentSize` | Width/height (25/50/75/100/auto) |
| Spacing | `Padding`, `Margin` | Margin/padding (0-5 scale, auto) |
| Text | `TextAlignment`, `HasLabel` | Alignment, wrapping, transform, size |
| Vertical Align | `AlignItems` | Top/middle/bottom/baseline |
| Visibility | `ShouldHide` | Visible/invisible (different from display) |

### Helpers (12)

| Bootstrap Helper | afterhours Equivalent | Notes |
|---|---|---|
| **Clearfix** | N/A | CSS float clearing (web-specific) |
| **Color & Background** | Theme color system | Combined color+bg utilities |
| **Colored Links** | N/A | Link color variants |
| **Focus Ring** | Focus ring in `theme.h` | Configurable focus indicator |
| **Icon Link** | `image_button()` | Icon + text combination |
| **Position** | `with_absolute_position()` | Positioning helpers (center, fixed-bottom, etc.) |
| **Ratio** | Gap | Aspect ratio (16x9, 4x3, 1x1, 21x9) |
| **Stacks** | `FlexDirection::Row/Column` | Shorthand for flex stacks |
| **Stretched Link** | N/A | Make parent clickable (web-specific) |
| **Text Truncation** | Gap | Ellipsis overflow |
| **Vertical Rule** | `separator(Vertical)` | Vertical divider |
| **Visually Hidden** | N/A | Screen reader only content |

---

## 4. Design Token System

### 4.1 Color System

Bootstrap generates a 9-step shade for each base color using
`tint-color()` (lighten) and `shade-color()` (darken):

```scss
$blue-100: tint-color($blue, 80%);  // lightest
$blue-200: tint-color($blue, 60%);
$blue-300: tint-color($blue, 40%);
$blue-400: tint-color($blue, 20%);
$blue-500: $blue;                    // base
$blue-600: shade-color($blue, 20%);
$blue-700: shade-color($blue, 40%);
$blue-800: shade-color($blue, 60%);
$blue-900: shade-color($blue, 80%);  // darkest
```

This is applied to: blue, indigo, purple, pink, red, orange, yellow, green,
teal, cyan — 10 hues x 9 steps = 90 color values.

On top of these, 8 **semantic theme colors** are defined:

| Theme Color | Role | afterhours Equivalent |
|---|---|---|
| `primary` | Main action color | `Theme::primary` |
| `secondary` | Less prominent | `Theme::secondary` |
| `success` | Positive/confirmation | Gap |
| `info` | Informational | Gap |
| `warning` | Caution | Gap |
| `danger` | Error/destructive | `Theme::error` |
| `light` | Light background | `Theme::surface` |
| `dark` | Dark background | `Theme::background` |

Each theme color generates 3 derived variants:
- `*-text-emphasis` — darkened for text
- `*-bg-subtle` — lightened for backgrounds
- `*-border-subtle` — lightened for borders

**Key insight:** Bootstrap's auto-derived variants (emphasis, subtle-bg,
subtle-border) solve the same problem as Radix's 12-step scale and the
proposed `ColorScale::from_seed()`.  The pattern is universal: **one seed
color → multiple derived variants for different UI roles**.

### 4.2 Spacing Scale

```scss
$spacer: 1rem;  // 16px base
$spacers: (
  0: 0,           // 0px
  1: $spacer * .25,  // 4px
  2: $spacer * .5,   // 8px
  3: $spacer,        // 16px
  4: $spacer * 1.5,  // 24px
  5: $spacer * 3,    // 48px
);
```

**afterhours equivalent:** `DefaultSpacing` (tiny=8, small=16, medium=24,
large=32, xlarge=48, container=64).  Similar scale but with different step
progression.

### 4.3 Breakpoints

```scss
$grid-breakpoints: (
  xs: 0, sm: 576px, md: 768px, lg: 992px, xl: 1200px, xxl: 1400px
);
```

**afterhours equivalent:** Resolution-independent sizing via `h720()` / `w1280()`
and `ScreenPercent`.  No breakpoint system — afterhours scales continuously
rather than snapping at thresholds.

### 4.4 Border Radius Scale

```scss
$border-radius:      .375rem;   // 6px
$border-radius-sm:   .25rem;    // 4px
$border-radius-lg:   .5rem;     // 8px
$border-radius-xl:   1rem;      // 16px
$border-radius-xxl:  2rem;      // 32px
$border-radius-pill: 50rem;     // fully round
```

**afterhours equivalent:** `Theme::roundness` (single float 0-1) +
`Theme::segments`.  Bootstrap's multiple radius tiers is more granular.

### 4.5 Shadow Scale

```
none → sm → default → lg → inset
```

**afterhours equivalent:** `Shadow` struct per component.  No shared scale.

### 4.6 Dark Mode

Bootstrap 5.3 uses `data-bs-theme="dark"` on `<html>` to switch color
mode.  All CSS variables have dark-mode overrides:

```scss
[data-bs-theme="dark"] {
  --bs-body-color: #{$body-color-dark};
  --bs-body-bg: #{$body-bg-dark};
  // ... all theme colors get dark variants
}
```

**afterhours equivalent:** Create a second `Theme` and switch between them.
No built-in dark mode toggle, but the theme system supports it.

### 4.7 Contrast Ratio

Bootstrap enforces WCAG contrast at build time:

```scss
$min-contrast-ratio: 4.5;  // WCAG AA for normal text
```

The `color-contrast()` function auto-selects light/dark text for any
background:

```scss
@function color-contrast($background) {
    // Returns $color-contrast-light or $color-contrast-dark
    // based on which meets $min-contrast-ratio
}
```

**afterhours equivalent:** `colors::auto_text_color()` and
`Theme::validate_accessibility()`.  Same approach — automatically pick
the best text color for any background.

---

## 5. What afterhours Can Learn

### 5.1 Semantic Color Roles: Success, Info, Warning

Bootstrap defines `success`, `info`, `warning` in addition to
primary/secondary/error.  These are universally useful:

| Role | Bootstrap Color | Use in Game UI |
|---|---|---|
| `success` | Green | Quest complete, item acquired, save successful |
| `info` | Cyan | Tutorial hints, passive information |
| `warning` | Yellow/amber | Low health, running out of resources |

**Recommendation:** Add `success`, `info`, `warning` to `Theme::Usage`:

```cpp
enum struct Usage {
    // ... existing ...
    Success,   // Green — positive outcomes
    Info,      // Cyan — informational
    Warning,   // Amber — caution
};
```

### 5.2 Auto-Derived Color Variants

Bootstrap's `*-text-emphasis`, `*-bg-subtle`, `*-border-subtle` pattern
generates useful variants from a single color.  This is the third library
(after Radix's 12-step scale and USWDS magic numbers) to confirm this need.

**Recommendation:** For each theme color, auto-derive:

```cpp
struct ColorVariants {
    Color solid;          // The base color (button fills)
    Color solid_hover;    // Slightly darker (hover state)
    Color text;           // Darkened (for text on light bg)
    Color bg_subtle;      // Very light tint (background highlight)
    Color border_subtle;  // Light tint (borders)

    static ColorVariants from_seed(Color base) {
        return {
            .solid       = base,
            .solid_hover = colors::darken(base, 0.15f),
            .text        = colors::darken(base, 0.40f),
            .bg_subtle   = colors::lighten(base, 0.80f),
            .border_subtle = colors::lighten(base, 0.60f),
        };
    }
};
```

This replaces ad-hoc `darken()`/`lighten()` calls throughout component code.

### 5.3 Event Lifecycle: show/shown/hide/hidden

The 4-phase lifecycle is the third independent confirmation:
- Bootstrap: `show` → `shown` → `hide` → `hidden`
- Radix: `Presence` state machine (`mounted` → `unmountSuspended` → `unmounted`)
- Base UI: `useTransitionStatus` (`unmounted` → `initial` → `open` → `close`)

**Recommendation:** This is now confirmed across all four researched libraries.
Implement a standard lifecycle for floating/toggled elements:

```cpp
enum class ElementLifecycle {
    Hidden,       // Not visible, not in layout
    Appearing,    // Animating in (show → shown)
    Visible,      // Fully visible
    Disappearing, // Animating out (hide → hidden)
};
```

### 5.4 Offcanvas / Drawer

Bootstrap's Offcanvas is a drawer from any edge with backdrop and focus trap.
This was also identified as a gap in `base_ui_architecture_analysis.md`.
Bootstrap's implementation confirms the needed features:

- Slide from top/bottom/start/end
- Optional backdrop (click to dismiss)
- Optional scroll lock on body
- Focus trapping
- Escape key to close
- Responsive: can convert to always-visible at breakpoints

### 5.5 Input Groups

Bootstrap's Input Group (prepend/append to an input) is not present in
Radix or Base UI but is widely used:

```
[Icon] [Text Input] [Button]
[$] [Amount Input] [.00]
[Search Input] [Go]
```

**afterhours equivalent:** Could be built with `div(Row)` + `text_input()` +
`button()`, but a dedicated `input_group()` would handle border radius merging
(sharp internal edges, rounded external edges).

### 5.6 Breadcrumb

A navigation trail showing hierarchy:

```
Home > Settings > Audio > Volume
```

Useful for: game settings, inventory categories, file browsers, menu
navigation history.  Simple to implement as a horizontal div with separators.

### 5.7 Scrollspy

Scroll-aware navigation highlighting — as you scroll content, the
corresponding nav item highlights.  Useful for:
- Long settings pages with section navigation
- Encyclopedia/codex entries
- Tutorial/help pages

### 5.8 Carousel

Image/panel slideshow with:
- Slide or crossfade transitions
- Previous/next controls
- Indicator dots
- Auto-play with interval
- Touch/swipe support

Useful for: splash screens, tutorials, character/skin showcases, item
galleries, news feeds.

---

## 6. Components Unique to Bootstrap (Not in Radix or Base UI)

| Component | What It Is | Priority for afterhours |
|---|---|---|
| **Breadcrumb** | Navigation trail | Medium — useful for deep settings |
| **Carousel** | Slideshow | Medium — tutorials, showcases |
| **Input Group** | Decorated inputs | Medium — compound input widgets |
| **Scrollspy** | Scroll-aware nav | Low — niche for long scrolling content |
| **Placeholders/Skeleton** | Loading shimmer | Medium — tracked in `32_loading_states.md` |
| **Floating Labels** | Animated input labels | Low — form UX polish |
| **Offcanvas** | Edge drawer | Medium — identified as Drawer in base UI analysis |

---

## 7. Updated Cross-Library Component Matrix

| Component | Bootstrap | Base UI | Radix | afterhours |
|---|---|---|---|---|
| **Accordion** | Yes | Yes | Yes | Doc |
| **Alert/Callout** | Yes | - | Yes (Themes) | Gap |
| **Avatar** | - | Yes | Yes | Doc |
| **Badge** | Yes | - | Yes (Themes) | Gap |
| **Breadcrumb** | Yes | - | - | Gap |
| **Button** | Yes | Yes | Yes | Has |
| **Button Group** | Yes | Yes (Toggle Group) | Yes | Has |
| **Card** | Yes | - | Yes (Themes) | Has (div) |
| **Carousel** | Yes | - | - | Gap |
| **Checkbox** | Yes | Yes | Yes | Has |
| **Collapse/Collapsible** | Yes | Yes | Yes | Doc |
| **Combobox/Autocomplete** | - | Yes | - | Gap |
| **Context Menu** | - | Yes | Yes | Doc |
| **Dialog/Modal** | Yes | Yes | Yes | Has |
| **Drawer/Offcanvas** | Yes | Yes (Drawer) | - | Gap |
| **Dropdown/Select** | Yes | Yes | Yes | Has |
| **Form/Field** | Yes | Yes | Yes | Gap |
| **Hover Card** | - | - | Yes | Gap |
| **Input Group** | Yes | - | - | Gap |
| **Label** | Yes | - | Yes | Has (inline) |
| **List Group** | Yes | - | - | Doc |
| **Menubar** | - | Yes | Yes | Gap |
| **Navigation** | Yes | Yes | Yes | Has |
| **Number Field** | - | Yes | - | Doc |
| **Pagination** | Yes | - | - | Has |
| **Popover** | Yes | Yes | Yes | Gap |
| **Progress** | Yes | Yes | Yes | Has |
| **Radio Group** | Yes | Yes | Yes | Has |
| **Scroll Area** | - | Yes | Yes | Has |
| **Separator** | - | Yes | Yes | Has |
| **Skeleton/Placeholder** | Yes | - | Yes (Themes) | Doc |
| **Slider** | Yes | Yes | Yes | Has |
| **Spinner** | Yes | - | Yes (Themes) | Gap |
| **Switch/Toggle** | Yes | Yes | Yes | Has |
| **Tabs** | Yes | Yes | Yes | Has |
| **Toast** | Yes | Yes | Yes | Has |
| **Toolbar** | - | Yes | Yes | Gap |
| **Tooltip** | Yes | Yes | Yes | Doc |

### Consensus Components (present in 3+ libraries)

These appear in at least 3 of the 4 researched libraries, confirming
they're essential for any complete UI system:

Already implemented (14): Button, Button Group, Checkbox, Dialog/Modal,
Dropdown/Select, Navigation, Progress, Radio Group, Scroll Area, Separator,
Slider, Switch/Toggle, Tabs, Toast

Tracked but not built (4): Accordion/Collapse, Tooltip, Skeleton, List

Gaps still needing docs (5): Popover, Toolbar, Drawer/Offcanvas, Form/Field,
Spinner

---

## 8. Token System Comparison

| Token | Bootstrap | Radix Themes | afterhours |
|---|---|---|---|
| **Color steps** | 9 per hue (tint/shade) | 12 per hue | 1 per role |
| **Semantic colors** | 8 (primary→dark) | Accent + gray | 10 (Theme::Usage) |
| **Color variants** | 3 auto-derived per theme color | Via scale steps | None (manual darken/lighten) |
| **Spacing** | 6 steps (0-5), base=16px | 9 steps | 6 named (tiny→container) |
| **Radius** | 6 levels (sm→pill) | 6 levels | 1 float (roundness) |
| **Shadows** | 4 levels (none→lg) | 6 levels | Per-component struct |
| **Font sizes** | 6 named | 9 steps | 4 tiers (sm/md/lg/xl) |
| **Breakpoints** | 6 (xs→xxl) | 6 (initial→xl) | None (continuous scaling) |
| **Dark mode** | data-bs-theme attribute | Inherits from theme provider | Theme swap |
| **Contrast** | Build-time WCAG check | N/A | Runtime auto_text_color |

### What This Tells Us

afterhours has the right *categories* of tokens (colors, spacing, radius,
font sizes) but needs more *granularity*:

1. **Color variants** — Need auto-derived hover/subtle/emphasis variants
2. **Radius scale** — Need multiple levels instead of one float
3. **Shadow scale** — Need shared levels instead of per-component config
4. **Semantic roles** — Need success/info/warning in addition to current set

---

## Summary

Bootstrap's contribution to the analysis is primarily about **completeness** —
it shows what a mature design system looks like after 13 years of iteration:

1. **Event lifecycle** (`show`/`shown`/`hide`/`hidden`) — the third
   confirmation that afterhours needs animated presence management

2. **Derived color variants** — auto-generating text-emphasis, bg-subtle,
   and border-subtle from each theme color eliminates manual color math

3. **Semantic color roles** — `success`/`info`/`warning` alongside
   primary/secondary/error covers the full range of UI feedback states

4. **Unique components** — Breadcrumb, Carousel, Input Group, and Scrollspy
   are Bootstrap-specific additions to the gap list

5. **Token granularity** — Multiple levels for radius, shadow, and font size
   (instead of afterhours' single values) enable finer design control

The biggest architectural difference from Radix/Base UI: Bootstrap is
**opinionated by default** — it ships ready-to-use styles.  This is actually
the correct model for afterhours' `Theme` system: provide good defaults that
work out of the box, while allowing full override via the theme builder.
