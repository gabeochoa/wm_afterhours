# Intent UI Analysis

## 1. Overview

**Intent UI** (https://intentui.com) is an open-source design system and component collection built on top of **React Aria Components** (Adobe) and styled with **Tailwind CSS v4**. Created by Irsyad A., it uses a "copy and paste" model — you own the source code rather than depending on a package.

- **GitHub:** intentui/intentui — 1,878 stars
- **License:** MIT
- **Foundation:** React Aria Components (accessibility), Tailwind CSS v4 (styling), Tailwind Variants (variant management)
- **Distribution:** shadcn CLI (`npx shadcn@latest add @intentui/<component>`)
- **Component count:** ~80 components across 11 categories
- **Notable:** Provides `llms.txt` for AI documentation consumption — each component has a `.md` endpoint

---

## 2. Architecture & Design Principles

### 2.1 Three-Layer Stack

Intent UI has a clean, well-separated architecture:

```
Layer 1: React Aria Components  → Behavior, accessibility, keyboard, ARIA
Layer 2: Tailwind Variants (tv) → Variant-based styling with compound variants  
Layer 3: Intent UI components   → Composition, defaults, theming
```

**afterhours relevance:** This is the cleanest separation of behavior/styling/composition we've seen. The key insight is that the styling layer (`tv()`) is its own independent concern, separate from both the behavior engine and the component composition.

### 2.2 Tailwind Variants (`tv`) Pattern

Every component uses `tailwind-variants` to define a style object with variants:

```typescript
const buttonStyles = tv({
  base: ["relative isolate inline-flex items-center ..."],
  variants: {
    intent: {
      primary: "[--btn-bg:var(--color-primary)] ...",
      secondary: "[--btn-bg:var(--color-secondary)] ...",
      danger: "[--btn-bg:var(--color-danger)] ...",
      warning: "[--btn-bg:var(--color-warning)] ...",
      outline: "...",
      plain: "...",
    },
    size: {
      xs: "min-h-8 ...",
      sm: "min-h-9 ...",
      md: "min-h-10 ...",
      lg: "min-h-11 ...",
      "sq-xs": "size-8 ...",  // square icon-only buttons
      "sq-sm": "size-10 ...",
      "sq-md": "size-11 ...",
      "sq-lg": "size-12 ...",
    },
    isCircle: { true: "rounded-full", false: "rounded-lg" },
    isDisabled: { true: "opacity-50" },
    isPending: { true: "opacity-50" },
  },
  defaultVariants: { intent: "primary", size: "md", isCircle: false },
})
```

**Key patterns:**
- **CSS custom property indirection** — Buttons set `[--btn-bg:var(--color-primary)]` then reference `bg-(--btn-bg)`, allowing theme-level overrides
- **Compound variants** — Style combinations like `{ gap: 0, columns: 1, className: "..." }` for complex interactions
- **Square size variants** — Separate `sq-xs`/`sq-sm`/`sq-md`/`sq-lg` for icon-only buttons (distinct from text button sizes)

**afterhours relevance:** The `tv()` pattern is essentially a style-variant lookup table. `afterhours` could adopt a similar pattern where `ComponentConfig` maps `{intent, size}` pairs to concrete styling properties at render time, rather than requiring explicit style overrides.

### 2.3 Compound Component Pattern (Dot Notation)

Intent UI uses the compound component pattern extensively:

```typescript
// Modal compound
Modal.Trigger = ModalTrigger
Modal.Header = ModalHeader
Modal.Title = ModalTitle
Modal.Description = ModalDescription
Modal.Body = ModalBody
Modal.Footer = ModalFooter
Modal.Close = ModalClose
Modal.Content = ModalContent

// Choicebox compound
Choicebox.Item = ChoiceboxItem
Choicebox.Label = ChoiceboxLabel
Choicebox.Description = ChoiceboxDescription

// InputOTP compound
InputOTP.Group = InputOTPGroup
InputOTP.Slot = InputOTPSlot
InputOTP.Separator = InputOTPSeparator

// DescriptionList compound
DescriptionList.Term = DescriptionTerm
DescriptionList.Details = DescriptionDetails
```

**afterhours relevance:** Yet another validation of compound components (also seen in Yamada UI, KivyMD, Radix). The dot-notation pattern is the dominant approach across modern UI libraries.

### 2.4 React Aria Foundation

Every interactive component delegates to a React Aria primitive for behavior and accessibility:

| Intent UI Component | React Aria Primitive | Behavior Provided |
|---|---|---|
| Button | `ButtonPrimitive` | Press events, disabled state, focus |
| Dialog | `DialogPrimitive` | ARIA dialog role, focus trapping |
| Modal | `ModalOverlay` + `ModalPrimitive` | Overlay dismiss, animation states |
| Menu | `MenuPrimitive` | Keyboard navigation, selection |
| ComboBox | `ComboBox` | Autocomplete, filtering |
| Tree | `TreePrimitive` | Expand/collapse, keyboard navigation |
| GridList | `GridList` | Multi-select, keyboard grid nav |

**afterhours relevance:** This validates the idea that accessibility behavior should be a separate, reusable layer. `afterhours` should extract keyboard navigation, focus management, and ARIA-equivalent behaviors into shared primitives rather than implementing them per-component.

---

## 3. Color & Theme System

### 3.1 Semantic Color Tokens (OKLCH)

Intent UI uses a **semantic token system** with OKLCH color values:

| Token | Purpose | Light Value (OKLCH) | Dark Value |
|-------|---------|---------------------|------------|
| `--bg` / `--fg` | Background / foreground | `oklch(1 0 0)` / `oklch(0.141 ...)` | `oklch(0.091 ...)` / `oklch(0.985 ...)` |
| `--primary` / `--primary-fg` | Primary actions | `oklch(0.546 0.245 262.881)` | Same |
| `--secondary` / `--secondary-fg` | Secondary UI | `oklch(0.92 ...)` | `oklch(0.244 ...)` |
| `--success` / `--success-fg` | Success state | `oklch(0.596 0.145 163.225)` | Same |
| `--warning` / `--warning-fg` | Warning state | `oklch(0.828 0.189 84.429)` | Same |
| `--danger` / `--danger-fg` | Destructive actions | `oklch(0.577 0.245 27.325)` | Same |
| `--muted` / `--muted-fg` | De-emphasized content | `oklch(0.967 ...)` | `oklch(0.21 ...)` |
| `--overlay` / `--overlay-fg` | Modal overlays | White / dark fg | Dark / light fg |
| `--border` | Borders | `oklch(0.911 ...)` | `oklch(0.270 ...)` |
| `--input` | Input borders | `oklch(0.871 ...)` | `oklch(0.290 ...)` |
| `--ring` | Focus ring | Same as primary | Same as primary |
| `--navbar` / `--navbar-fg` | Navigation bar | Near-white | Near-black |
| `--sidebar` / `--sidebar-fg` | Sidebar | Near-white | Near-black |

**Key observations:**
- **Content color pairing** — Every semantic color has a `-fg` counterpart (seen also in DaisyUI). This ensures text always has appropriate contrast against its background.
- **OKLCH throughout** — Perceptually uniform color space for consistent lightness across hues.
- **Chart colors** — 5 dedicated chart tokens (`--chart-1` through `--chart-5`) as a ramp from the primary color.
- **Surface specialization** — Separate tokens for `overlay`, `navbar`, and `sidebar` — these are not just `bg/fg`, they're specialized surface tokens.

### 3.2 Radius System

Intent UI uses a **single base radius** with computed derivatives:

```css
--radius-lg: 0.5rem;
--radius-xs: calc(var(--radius-lg) * 0.5);   /* 0.25rem */
--radius-sm: calc(var(--radius-lg) * 0.75);  /* 0.375rem */
--radius-md: calc(var(--radius-lg) * 0.9);   /* 0.45rem */
--radius-xl: calc(var(--radius-lg) * 1.25);  /* 0.625rem */
--radius-2xl: calc(var(--radius-lg) * 1.5);  /* 0.75rem */
--radius-3xl: calc(var(--radius-lg) * 2);    /* 1rem */
--radius-4xl: calc(var(--radius-lg) * 3);    /* 1.5rem */
```

**afterhours relevance:** A single `radius_base` theme property that all other radii derive from via multipliers. Changing one value updates the entire system. This is more maintainable than defining 8 independent radius values.

### 3.3 Theme Presets

Intent UI ships with 12+ theme presets (default, blue, sky, indigo, emerald, teal, purple, pink, rose, green, lime, cyan) plus grey-scale variants (slate, gray, zinc, neutral, stone). Each theme is a CSS file that redefines the semantic tokens.

The zinc theme example shows how simple theme swapping is — only the semantic token values change:

```css
/* Zinc theme */
--primary: var(--color-blue-600);
--secondary: var(--color-zinc-100);
--muted: var(--color-zinc-100);
--border: var(--color-zinc-200);
```

**afterhours relevance:** Theme presets with named identities ("zinc", "slate", "emerald") are more user-friendly than raw color values. `afterhours` `Theme` could ship named presets that users select.

### 3.4 CSS Custom Property Indirection in Components

Components use a two-layer CSS variable system:

```css
/* Button sets local variables */
[--btn-bg:var(--color-primary)]
[--btn-fg:var(--color-primary-fg)]
[--btn-overlay:color-mix(in_oklab,var(--color-primary-fg)_10%,var(--color-primary)_90%)]

/* Then references them */
bg-(--btn-bg) text-(--btn-fg) hover:bg-(--btn-overlay)
```

**Key insight:** The `color-mix()` function computes hover/pressed states algorithmically:
- **Hover overlay:** `color-mix(in_oklab, fg 10%, bg 90%)` — lightens the background by mixing 10% foreground
- This means hover states are **derived, not hardcoded**. One primary color definition generates all interactive states automatically.

**afterhours relevance:** `afterhours` currently requires explicit hover/pressed colors. Adopting `color-mix` logic (mix a percentage of white or foreground into the base color) would eliminate the need to define 3 colors per button variant.

---

## 4. Component Inventory

### 4.1 Full Catalog

| Category | Components | afterhours Status |
|----------|-----------|------------------|
| **Buttons** | Button, Button Group, File Trigger, Toggle Group, Toggle | Button exists; Toggle, Toggle Group, File Trigger missing |
| **Collections** | Choicebox, Dropdown, Grid List, List Box, Menu, Table, Tag Group, Tree | Dropdown exists; Choicebox, Grid List, Tree, Tag Group missing |
| **Colors** | Color Area, Color Field, Color Picker, Color Slider, Color Swatch Picker, Color Swatch, Color Thumb, Color Wheel | None (8 missing) |
| **Controls** | Command Menu, Context Menu, Keyboard, Scroll Area, Slider, Switch, Toolbar | Slider exists; Command Menu, Context Menu, Keyboard missing |
| **Date & Time** | Calendar, Date Field, Date Picker, Date Range Picker, Range Calendar, Time Field | None (6 missing) |
| **Drag & Drop** | Drop Zone | Drag exists; Drop Zone concept missing |
| **Forms** | Checkbox Group, Checkbox, Field, Input OTP, Input, Number Field, Radio Group, Search Field, Tag Field, Text Field, Textarea | Checkbox, Input, Radio exist; OTP, Number Field, Search Field, Tag Field missing |
| **Layouts** | Container, Navbar, Sidebar | None (3 missing) |
| **Media** | Avatar, Carousel | None (2 missing) |
| **Navigation** | Breadcrumbs, Disclosure Group, Disclosure, Link, Pagination, Snippet, Tabs | Tabs exist; Breadcrumbs, Disclosure, Pagination, Snippet missing |
| **Overlays** | Dialog, Drawer, Modal, Popover, Sheet, Tooltip | Popover exists; Dialog, Drawer, Modal, Sheet missing |
| **Pickers** | Combo Box, Multiple Select, Select | Dropdown exists; Combo Box, Multiple Select missing |
| **Statuses** | Badge, Loader, Meter, Note, Progress Bar, Progress Circle, Skeleton, Toast | Progress Bar exists; Badge, Loader, Meter, Note, Skeleton, Toast missing |
| **Surfaces** | Card, Description List, Heading, Separator, Show More, Text | Separator exists; Card, Description List, Show More missing |
| **Visualizations** | Area Chart, Bar Chart, Bar List, Chart, Leaderboard, Line Chart, Pie Chart, Tracker | None (8 missing) |

**Total: ~80 components. afterhours has ~12 equivalents. ~68 gaps.**

### 4.2 Notable / Unique Components

#### Choicebox (Rich Selection Card)
A large-format selection component — like RadioCard/CheckboxCard from Yamada UI. Grid-based, with label + description, built on GridList. Supports `columns` (1-6) and `gap` variants. Selection mode: single or multiple.

**afterhours relevance:** High. This is the "RadioCard" pattern identified in multiple libraries. Essential for game UIs (class selection, inventory categories, plan selection).

#### Command Menu
A searchable command palette (like VS Code's Ctrl+K). Built on React Aria `Autocomplete` + `Modal`. Features: keyboard shortcut registration, fuzzy filtering, sections with labels, keyboard shortcuts display, avatar support in items, loading state.

**afterhours relevance:** Medium-high. A command palette is useful for debug/developer tooling in game UIs. The keyboard shortcut system is independently useful.

#### Snippet
A tabbed code display component with copy-to-clipboard. Built on React Aria `Tabs`.

**afterhours relevance:** Low for games, but the "tabbed content with copy" pattern is useful for settings/configuration displays.

#### Tracker
A visual timeline of deployment/status outcomes — a row of colored bars representing success/warning/error states over time.

**afterhours relevance:** High for game UIs. Could visualize server health, match history, daily login streaks, quest completion timelines, or achievement progress.

#### Leaderboard
A ranked list component with proportional progress bars. Each item shows a label, value, and a bar scaled to the max value.

**afterhours relevance:** Very high. Leaderboards are core to game UIs. The proportional bar visualization is immediately useful.

#### Note (Status Banner)
A colored banner component with 5 intents (default, info, warning, danger, success), automatic icon selection, and support for embedded links and bold text.

**afterhours relevance:** High. This is the "alert/notification banner" pattern. Essential for in-game notifications, tutorial tips, and error messages.

#### Meter
A progress-bar-like component that changes color based on value thresholds (<30% primary, <50% success, <70% yellow, <80% warning, >80% danger). Uses Framer Motion for animated transitions.

**afterhours relevance:** High. Health bars, resource meters, reputation meters — the auto-coloring by percentage is directly applicable to game UIs.

#### Show More
A separator with an embedded toggle button — "show more" divider that reveals hidden content.

**afterhours relevance:** Medium. Useful for collapsible sections in settings, inventory overflow, or long lists.

#### Description List
A key-value pair display component using `<dl>/<dt>/<dd>` semantics, displayed as a responsive 2-column grid.

**afterhours relevance:** Medium. Item stats, character attributes, settings summary displays.

#### Drawer (Motion-Based)
A slide-in panel using **Framer Motion** (`motion/react`) for spring-based physics animations. Supports 4 sides (top/bottom/left/right) with a "float" option (inset with rounded corners). Uses `AnimatePresence` for mount/unmount animation lifecycle.

**afterhours relevance:** High. Side panels for inventory, chat, settings. The float variant (rounded, inset from edges) is a nice premium touch.

#### Multiple Select
A combo box that supports multiple selections, displaying selected items as tags. Combines Input, ComboBox, ListBox, Popover, and TagGroup.

**afterhours relevance:** Medium. Tag-based multi-select is useful for filter UIs, team composition, equipment loadouts.

#### Tree
Hierarchical list with expand/collapse, keyboard navigation, and indent levels. Uses CSS variable `--tree-item-level` for nested indentation.

**afterhours relevance:** Medium. File browsers, skill trees, nested menu systems.

---

## 5. Overlay Architecture

Intent UI has a comprehensive overlay system with clear distinctions between overlay types:

| Overlay | Motion | Dismiss | Layout | Use Case |
|---------|--------|---------|--------|----------|
| **Modal** | Scale-in + fade | Click outside (unless alertdialog) | Centered, sized (xs-5xl) | Confirmations, forms |
| **Sheet** | Slide from edge | Click outside | Fixed to edge (top/right/bottom/left) | Filters, navigation |
| **Drawer** | Spring physics (Framer) | Click outside | Fixed to edge, optional float | Mobile-friendly panels |
| **Dialog** | Inherited from parent | Varies | Content container inside Modal/Sheet | Reusable content structure |
| **Popover** | Fade | Click outside | Floating near trigger | Menus, tooltips |
| **Tooltip** | Fade | Hover out | Small floating | Help text |

**Key architectural pattern:** `Dialog` is the content structure (header, title, description, body, footer, close). `Modal`, `Sheet`, and `Drawer` are the **container** patterns that wrap a Dialog. This separates "what's in the overlay" from "how the overlay appears."

```
Modal.Content → ModalOverlay → ModalPrimitive → Dialog → [Header, Body, Footer]
Sheet.Content → SheetOverlay → SheetPrimitive → Dialog → [Header, Body, Footer]  
Drawer.Content → DrawerOverlay → DrawerRoot → Dialog → [Header, Body, Footer]
```

**afterhours relevance:** This is the cleanest overlay decomposition we've seen. `afterhours` should adopt this pattern:
- A `DialogContent` component (header/title/body/footer structure)
- Container variants: `Modal` (centered), `Sheet` (edge-anchored), `Drawer` (edge + spring physics)
- The Dialog content is identical regardless of container

### 5.1 Modal Animation Details

**Entering (200ms, ease-out):**
- Overlay: fade-in
- Content (mobile): slide-in-from-bottom
- Content (desktop): zoom-in-95% + fade-in

**Exiting (150ms, ease-in):**
- Overlay: fade-out
- Content (mobile): slide-out-to-bottom
- Content (desktop): zoom-out-95% + fade-out

**Responsive behavior:** On mobile, modals slide up from the bottom (sheet-like). On desktop, they scale in at the center. This adaptive animation is a UX best practice.

### 5.2 Sheet Side Variants with Float

The Sheet has a `isFloat` option that insets the panel from edges with rounded corners and a ring border:

```
Normal:  Panel fills edge completely, border on opposing side
Float:   Panel inset 2 units from all edges, rounded corners, ring border
```

**afterhours relevance:** The float variant is a premium touch that makes side panels feel less intrusive. Easy to implement — just add padding + border-radius when a "float" flag is set.

---

## 6. Focus & Accessibility Patterns

### 6.1 Focus Ring System

Intent UI defines a reusable `focusRing` TV variant:

```typescript
const focusRing = tv({
  variants: {
    isFocused: { true: "ring-4 ring-ring/20 data-invalid:ring-danger/20" },
    isFocusVisible: { true: "ring-4 ring-ring/20" },
    isInvalid: { true: "ring-4 ring-danger/20" },
  },
})

const focusStyles = tv({
  extend: focusRing,
  variants: {
    isFocused: { true: "border-ring/70" },
    isInvalid: { true: "border-danger/70" },
  },
})
```

**Key detail:** Focus rings are `ring-4 ring-ring/20` — a **4px ring at 20% opacity** of the ring color. This is subtle enough to not be visually aggressive but visible enough for keyboard users.

**afterhours relevance:** `afterhours` should adopt a similar 2-layer focus system:
- `focusRing`: The outer glow (always applied when focused)
- `focusStyles`: Ring + border color change (for input-like elements)

### 6.2 Forced Colors Support

Every component includes `forced-colors:` overrides for Windows High Contrast mode:

```css
forced-colors:outline-[Highlight]
forced-colors:[--btn-icon:ButtonText]
forced-colors:text-[GrayText]  /* disabled */
```

**afterhours relevance:** While web-specific, the principle of having a "high contrast" fallback mode is relevant. `afterhours` could support a high-contrast theme that overrides all colors to maximally contrastive values.

### 6.3 Mobile Focus Handling

The Dialog close button has special mobile focus logic:

```typescript
const isMobile = useMediaQuery("(max-width: 600px)")
useEffect(() => {
  if (isMobile && buttonRef.current) {
    buttonRef.current.focus()
  }
}, [isMobile])
```

On mobile, the close button auto-focuses immediately. On desktop, it does not (to avoid stealing focus from the dialog content).

---

## 7. Animation Patterns

### 7.1 Framer Motion Integration (Drawer)

The Drawer component uses `motion/react` for physics-based animations:

```typescript
const DrawerRoot = motion.create(ModalPrimitive)
const DrawerOverlay = motion.create(ModalOverlay)

// Overlay animation
animate={{ backgroundColor: "rgba(0, 0, 0, 0.2)" }}
exit={{ backgroundColor: "rgba(0, 0, 0, 0)" }}

// Content uses spring physics for slide-in
```

### 7.2 Meter Animated Progress

The Meter component uses Framer Motion for smooth value transitions:

```typescript
<motion.div
  initial={{ width: "0%", backgroundColor: getColor(0) }}
  animate={{ width: `${percentage}%`, backgroundColor: getColor(percentage) }}
  transition={{ duration: 0.5 }}
/>
```

Both the width and color animate simultaneously — as the value increases, the bar grows and its color transitions through the threshold palette.

**afterhours relevance:** The Meter's animated color-by-threshold pattern is directly applicable. `afterhours` health bars should smoothly transition both width and color as values change.

### 7.3 CSS-Based Animations (Modals)

Most overlays use CSS animations via `tw-animate-css`:

```css
data-entering:fade-in animate-in duration-200 ease-out
data-exiting:fade-out animate-out duration-150 ease-in
data-entering:slide-in-from-bottom
data-entering:zoom-in-95
```

**Pattern:** Enter animations are **slower** (200-300ms) with **ease-out** (decelerate to rest). Exit animations are **faster** (150-200ms) with **ease-in** (accelerate away). This asymmetry makes entering feel welcoming and exiting feel snappy.

---

## 8. Recommendations for afterhours

### 8.1 High Priority

| Feature | Source Pattern | Implementation Notes |
|---------|---------------|---------------------|
| **Semantic color token system** | 10 semantic color pairs (primary/fg, secondary/fg, success/fg, warning/fg, danger/fg, muted/fg, overlay/fg, + surface specializations) | Add `overlay`, `muted`, `navbar`, `sidebar` token pairs to Theme |
| **Derived hover states** | `color-mix(fg 10%, bg 90%)` for hover | Compute hover/pressed colors from base, eliminate manual color specification |
| **Derived radius system** | Single `radius_base` with multiplier-derived variants | Replace 8 independent radius values with 1 base + multipliers |
| **Overlay architecture** | Dialog (content) + Modal/Sheet/Drawer (container) separation | Build Dialog content structure once, reuse across overlay types |
| **Note/Banner component** | Intent-colored banners (info/warning/danger/success) with auto-icons | Essential for in-game notifications, tutorials, error displays |
| **Meter with auto-color** | Progress bar that changes color by value threshold | Health bars, resource meters, XP bars |
| **Leaderboard component** | Ranked list with proportional bars | Core game UI component |
| **Choicebox / Rich Selection** | Large-format selection cards with label + description | Class selection, plan selection, equipment categories |

### 8.2 Medium Priority

| Feature | Source Pattern | Implementation Notes |
|---------|---------------|---------------------|
| **Tracker** | Status timeline visualization (colored bar row) | Match history, quest logs, daily streaks |
| **Command Menu** | Searchable command palette with keyboard shortcuts | Debug/dev tooling, power-user features |
| **Drawer (spring physics)** | Slide-in panel with spring animation + float variant | Inventory panels, chat, settings |
| **Sheet (4-side)** | Edge-anchored overlay (top/right/bottom/left) + float option | Navigation, filters, detail views |
| **Description List** | Key-value grid display | Item stats, character attributes |
| **Show More** | Collapsible separator | Long lists, settings sections |
| **Button square variants** | Separate square icon-only sizes (`sq-xs` through `sq-lg`) | Icon toolbar buttons, action bars |
| **Multiple Select** | Tag-based multi-selection combo box | Equipment loadouts, team filters |

### 8.3 Lower Priority

| Feature | Source Pattern | Implementation Notes |
|---------|---------------|---------------------|
| **Color Picker suite** | Area, Slider, Wheel, Swatch Picker, Field (8 components) | Character customization, UI theming |
| **Tree** | Hierarchical expandable list | Skill trees, file browsers |
| **Input OTP** | Multi-slot PIN/code entry | Account verification flows |
| **Snippet** | Tabbed code display with copy | Configuration, debug output |
| **Chart components** | Area, Bar, Line, Pie, Bar List charts | Stats screens, analytics |
| **Theme presets** | Named theme identities (zinc, slate, emerald, etc.) | Pre-built visual themes users can select |
| **Date/Time suite** | Calendar, Date Picker, Time Field, Range Picker | Scheduling features if needed |

---

## 9. Key Takeaways

### 9.1 Separation of Content from Container (Overlays)

The most architecturally significant pattern is the **Dialog as content, overlay as container** separation. In Intent UI, a Dialog (header/title/body/footer/close) is a single reusable structure that can appear inside a Modal (centered popup), Sheet (edge panel), or Drawer (spring-animated panel). This means the content layout is defined once and the presentation/animation is chosen independently.

### 9.2 Derived, Not Defined

Intent UI derives as much as possible:
- **Hover colors** from base colors via `color-mix()`
- **All radius values** from a single `--radius-lg` base
- **Chart palette** from the primary color
- **Icon colors** from button foreground at different opacities

This "derive from one source" approach drastically reduces the number of theme tokens that need to be defined while maintaining consistency.

### 9.3 Surface Token Specialization

Beyond just `bg/fg`, Intent UI defines specialized surface tokens: `overlay/overlay-fg`, `navbar/navbar-fg`, `sidebar/sidebar-fg`. This acknowledges that different UI surfaces have different visual requirements — a sidebar might be slightly darker than the main background, overlays might need distinct treatment in dark mode, etc. This is more nuanced than a single background color.

### 9.4 Asymmetric Animation Timing

Enter: 200-300ms with ease-out (decelerate into view). Exit: 150-200ms with ease-in (accelerate out of view). The asymmetry feels natural — things arriving should feel welcoming, things leaving should feel quick and decisive.

### 9.5 Intent as Vocabulary

Intent UI names its semantic variants `intent` rather than `variant` or `color`:
- `intent="primary"` / `intent="danger"` / `intent="warning"`

This naming choice emphasizes that the visual style communicates **purpose**, not just appearance. A danger button isn't just "red" — it communicates a destructive intent. `afterhours` should adopt this naming convention for semantic color application.
