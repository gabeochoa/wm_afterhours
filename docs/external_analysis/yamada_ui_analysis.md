# Yamada UI Analysis for afterhours

## Overview

[Yamada UI](https://yamada-ui.com/) is a comprehensive React component library providing
100+ components, 7 chart components, 12 animation primitives, and 8 utility components.
It follows a consistent compound-component architecture (dot notation: `Component.Root`,
`Component.Item`, etc.) and offers both an import-from-module and a CLI-download model
that lets users customize component internals.

Yamada UI is relevant to `afterhours` because of its exceptionally broad component
catalog — it includes several components rarely found in other libraries (Reorder,
Resizable, Timeline, InfiniteScrollArea, Snacks, Editable, PinInput, Dropzone,
EmptyState, Indicator, CheckboxCard/RadioCard, ColorPicker, and planned Dockable/Tour/
Chat/Swipeable). Its compound-component pattern also validates the decomposition
approach recommended in the KivyMD and Base UI analyses.

**Source:** https://yamada-ui.com/docs/components and GitHub `yamada-ui/yamada-ui`
(MDX docs extracted from `www/contents/components/`)

---

## 1. Architecture

### 1.1 Compound Component Pattern (Dot Notation)

Every Yamada UI component uses a consistent dot-notation pattern:

```tsx
<Component.Root>
  <Component.Item>
    <Component.SubPart />
  </Component.Item>
</Component.Root>
```

Examples:
- `Reorder.Root` > `Reorder.Item`
- `Carousel.Root` > `Carousel.List` > `Carousel.Item` + `Carousel.PrevTrigger` + `Carousel.NextTrigger` + `Carousel.Indicators`
- `Timeline.Root` > `Timeline.Item` > `Timeline.Connector` > `Timeline.Indicator` + `Timeline.Content` > `Timeline.Title` + `Timeline.Description`
- `Stat.Root` > `Stat.Label` + `Stat.Value` + `Stat.HelperMessage` > `Stat.Icon`
- `EmptyState.Root` > `EmptyState.Indicator` + `EmptyState.Title` + `EmptyState.Description`

**afterhours relevance:** This is the same compound pattern seen in KivyMD (MDButton +
MDButtonIcon + MDButtonText) and Base UI, now with a third independent validation point.
The `.Root` / `.Item` / `.SubPart` naming convention is clean and should be adopted for
`afterhours`' complex components.

### 1.2 Variant + Size System

Every component supports `variant` and `size` props with consistent values:
- **Variants:** `outline`, `filled`, `flushed`, `plain`, `panel`, `solid`, `subtle`, `surface` (varies by component)
- **Sizes:** `xs`, `sm`, `md`, `lg`, `xl` (varies by component)

**afterhours relevance:** `afterhours` has partial variant support on buttons. A
universal variant + size system across all components would provide consistency.

### 1.3 CLI Download Model

Yamada UI offers `yamada-cli add <component>` to download component source code locally.
Users can then customize the variant styles, logic, or fix bugs directly. Updates can be
pulled from upstream with conflict resolution (like Git).

**afterhours relevance:** Not directly applicable (C++ vs. npm), but the idea of
"download component as source to customize" maps to `afterhours`' header-only approach
where users can modify component implementations directly.

---

## 2. Component Inventory

### 2.1 Full Component List (100+ components)

#### Layout (18)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **AspectRatio** | Maintains child's aspect ratio | Not planned |
| **Bleed** | Allows child to bleed into parent padding | Not planned |
| **Box** | Generic container (div equivalent) | Exists |
| **Center** | Centers child horizontally and vertically | Exists (alignment) |
| **Container** | Max-width container with horizontal centering | Exists |
| **Flex** | Flexbox container | Exists |
| **Float** | Anchors element to a corner of its container | Not planned |
| **Grid** | CSS grid layout | Exists |
| **Group** | Groups children with shared styling | Partial |
| **HStack** | Horizontal stack | Exists |
| **SimpleGrid** | Equal-column grid | Exists |
| **Spacer** | Flexible space (flex: 1) | Exists |
| **Stack** | Vertical or horizontal stack with spacing | Exists |
| **VStack** | Vertical stack | Exists |
| **Wrap** | Flex wrap layout | Exists |
| **ZStack** | Layered stack (children overlap in z-order) | Not planned |
| **ScrollArea** | Scrollable container | Exists |
| **Resizable** | Resizable split panels with drag handle | Planned (docs/37) |

#### Navigation (6)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Accordion** | Collapsible content panels | Planned (docs/12) |
| **Breadcrumb** | Navigation breadcrumb trail | Not planned |
| **Menu** | Dropdown menu with items | Exists |
| **Pagination** | Page navigation with prev/next | Planned |
| **Steps** | Multi-step progress indicator | Not planned |
| **Tabs** | Tabbed content panels | Exists |

#### Input (21)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Autocomplete** | Text input with suggestions dropdown | Not planned |
| **Button** | Standard button | Exists |
| **Checkbox** | Checkbox control | Exists |
| **CheckboxCard** | Selectable card (multi-select) | Not planned |
| **ColorPicker** | Color selection with picker popup | Planned (docs/26) |
| **ColorSelector** | Inline color selector (hue/saturation/alpha) | Planned (docs/26) |
| **DatePicker** | Date selection with calendar dropdown | Planned (docs/34) |
| **Dropzone** | File upload via drag and drop | Not planned |
| **Editable** | Click-to-edit inline text | Not planned |
| **FileButton** | Button that opens file picker | Not planned |
| **FileInput** | File input field | Not planned |
| **IconButton** | Icon-only button | Exists |
| **Input** | Text input field | Exists |
| **NativeSelect** | Native select dropdown | Exists |
| **NumberInput** | Numeric input with +/- controls | Planned (docs/09) |
| **PasswordInput** | Password field with show/hide toggle | Not planned |
| **PinInput** | PIN/OTP code entry (4-6 digit boxes) | Not planned |
| **Radio** | Radio button | Exists |
| **RadioCard** | Selectable card (single-select) | Not planned |
| **Rating** | Star rating input | Not planned |
| **SegmentedControl** | Single-select button group | Not planned |
| **Select** | Styled select dropdown | Exists |
| **Slider** | Range slider | Exists |
| **Switch** | Toggle switch | Exists |
| **Textarea** | Multi-line text input | Exists |
| **TimePicker** | Time selection | Planned (docs/34) |
| **Toggle** | Toggle button (pressed/unpressed) | Exists |

#### Display (18)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Alert** | Contextual feedback message (info/success/warning/error) | Planned (docs/14) |
| **Avatar** | User avatar (image or initials) | Not planned |
| **Badge** | Small status label | Planned (docs/13) |
| **Blockquote** | Styled quotation | Not planned |
| **Card** | Content container with header/body/footer | Exists |
| **CircleProgress** | Circular progress indicator | Planned (docs/32) |
| **Code** | Inline code display | Not planned |
| **DataList** | Term + description list | Not planned |
| **EmptyState** | "No data" placeholder (icon + title + description + CTA) | Not planned |
| **Heading** | Section heading | Exists |
| **Highlight** | Text with highlighted substring | Not planned |
| **Image** | Image display with fallback | Partial |
| **Indicator** | Badge overlay on avatar/icon corner | Not planned |
| **Kbd** | Keyboard shortcut display | Not planned |
| **List** | Ordered/unordered list | Exists |
| **Mark** | Highlighted text span | Not planned |
| **Separator** | Horizontal/vertical divider | Exists |
| **Stat** | Numeric statistic with label and trend icon | Not planned |
| **Status** | Colored dot + label status indicator | Not planned |
| **Tag** | Dismissible label tag | Not planned |
| **Text** | Text display | Exists |
| **Timeline** | Chronological event list | Not planned |
| **Tooltip** | Hover tooltip | Planned (docs/10) |

#### Feedback (6)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Drawer** | Side panel (modal or persistent) | Not planned |
| **Modal** | Dialog/modal overlay | Exists |
| **Popover** | Positioned popup content | Planned |
| **Progress** | Linear progress bar | Planned (docs/32) |
| **Snacks** | Stacking notification toasts in forms | Planned (docs/14) |
| **Tooltip** | Simple/rich tooltip | Planned (docs/10) |

#### Advanced (6)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Calendar** | Full calendar display/selection widget | Not planned |
| **Carousel** | Slideshow with prev/next + indicators | Not planned |
| **InfiniteScrollArea** | Auto-load content on scroll to bottom | Not planned |
| **Reorder** | Drag-and-drop list reordering | Exists |

#### Planned (not yet implemented in Yamada UI)

| Component | Description | afterhours Relevance |
|-----------|-------------|---------------------|
| **ActionBar** | Contextual action toolbar (appears on selection) | Medium — useful for multi-select actions |
| **Chat** | Chat/messaging component | Low — web-specific |
| **Dockable** | Dockable/undockable panels | High — game UI tool panels |
| **Form** | Form validation wrapper | Low — web-specific |
| **NativePopover** | Browser-native popover | N/A |
| **PhoneInput** | Phone number input | N/A |
| **QrCode** | QR code generator | N/A |
| **Swipeable** | Swipe gesture handler | Medium — mobile game UIs |
| **TimePicker** | Time selection | Planned (docs/34) |
| **Tip** | Inline contextual tip | Medium — onboarding |
| **Tour** | Step-by-step onboarding tour | High — game tutorials |
| **Tree** | Tree view | Exists |

#### Charts (7)

| Chart | Description | afterhours Relevance |
|-------|-------------|---------------------|
| AreaChart | Filled line chart | Medium — stats screens |
| BarChart | Vertical/horizontal bars | Medium — stats screens |
| DonutChart | Ring chart | Medium — stat displays |
| LineChart | Line graph | Medium — stat displays |
| PieChart | Pie/segment chart | Medium — stat displays |
| RadarChart | Spider/radar chart | High — RPG stat displays |
| RadialChart | Radial bar chart | Low |

#### Animations (12)

| Animation | Description | afterhours Relevance |
|-----------|-------------|---------------------|
| **Airy** | Floating/breathing animation | Medium — idle animations |
| **Collapse** | Height collapse/expand | High — accordion, panels |
| **Fade** | Opacity fade in/out | Exists |
| **FadeScale** | Fade + scale combined | Medium |
| **Flip** | 3D flip rotation | Medium — card flip |
| **Loading** | Loading spinner shapes | Medium |
| **Motion** | General-purpose motion wrapper | Exists |
| **Ripple** | Material Design ripple effect | Not yet |
| **Rotate** | Rotation animation | Exists |
| **Skeleton** | Loading skeleton placeholder | Not planned |
| **Slide** | Slide in from edge | Medium |
| **SlideFade** | Slide + fade combined | Medium |

#### Utilities (8)

| Utility | Description | afterhours Relevance |
|---------|-------------|---------------------|
| **ClientOnly** | Render only on client | N/A (web) |
| **FocusLock** | Trap focus in container | High — modal focus |
| **For** | Loop/iteration helper | N/A (JSX) |
| **Format** | Number/date formatting | Low |
| **Portal** | Render in different DOM location | Medium — overlays |
| **Show** | Conditional rendering | N/A (immediate mode) |
| **Slot** | Named content slot | Medium — composition |
| **VisuallyHidden** | Hidden but accessible content | Medium — a11y |

### 2.2 Component Count Summary

| Category | Count | afterhours Exists | afterhours Planned | New Gap |
|----------|-------|-------------------|-------------------|---------|
| Layout | 18 | 13 | 1 | 4 |
| Navigation | 6 | 3 | 2 | 1 |
| Input | 26 | 13 | 5 | 8 |
| Display | 22 | 8 | 4 | 10 |
| Feedback | 6 | 1 | 4 | 1 |
| Advanced | 4 | 1 | 0 | 3 |
| Charts | 7 | 0 | 0 | 7 |
| Animations | 12 | 4 | 0 | 8 |
| Utilities | 8 | 0 | 1 | 7 |
| Planned | 12 | 1 | 1 | 10 |
| **Total** | **121** | **44** | **18** | **59** |

---

## 3. Notable Components (Deep Dives)

### 3.1 Reorder

Drag-and-drop list reordering with compound parts:

```tsx
<Reorder.Root>
  <Reorder.Item value="item-1">Item 1</Reorder.Item>
  <Reorder.Item value="item-2">Item 2</Reorder.Item>
</Reorder.Root>
```

Supports:
- **5 variants:** panel, outline, solid, subtle, surface
- **5 sizes:** xs, sm, md, lg, xl
- **Horizontal + vertical** orientation
- **onChange** callback with new order
- **Items as data array** (pass `items` prop instead of manual children)

**afterhours relevance:** `afterhours` already has `with_draggable_children()` which
is equivalent. Yamada's variant system (panel/outline/solid) for reorderable items
is a nice touch — visual style of the drag items is configurable without custom code.

### 3.2 Timeline

Chronological event display with rich structure:

```tsx
<Timeline.Root>
  <Timeline.Item>
    <Timeline.Connector>
      <Timeline.Indicator />   {/* dot/icon on the line */}
    </Timeline.Connector>
    <Timeline.Content>
      <Timeline.Title>Event Title</Timeline.Title>
      <Timeline.Description>Event details...</Timeline.Description>
    </Timeline.Content>
  </Timeline.Item>
</Timeline.Root>
```

**afterhours relevance:** Timeline is valuable for game UIs — quest progression,
achievement history, changelog display, event logs. The compound structure
(Connector with Indicator line + Content with Title/Description) is a clean
decomposition that could map to `afterhours` as a vertical list with connected dots.

### 3.3 Steps

Multi-step process indicator:

```tsx
<Steps.Root items={stepsData}>
  <Steps.List />           {/* step indicators (1, 2, 3...) */}
  <Steps.Contents />       {/* content for current step */}
  <Steps.CompletedContent> {/* shown when all steps complete */}
    All done!
  </Steps.CompletedContent>
</Steps.Root>
```

Supports:
- Data-driven items (pass `items` array with title, description, content)
- Horizontal and vertical orientation
- Active step tracking
- Completed state content

**afterhours relevance:** Steps/stepper is recommended across multiple analyses
(Ant Design, DaisyUI, Materialize). Yamada's data-driven approach with
`Steps.CompletedContent` is the cleanest API seen so far.

### 3.4 EmptyState

Placeholder for empty content areas:

```tsx
<EmptyState.Root
  indicator={<ShoppingCartIcon />}
  title="Your cart is empty"
  description="Explore our products and add items to your cart"
/>
```

Compound parts: `EmptyState.Indicator` + `EmptyState.Title` + `EmptyState.Description`

**afterhours relevance:** Empty states are a common game UI need (empty inventory,
empty friends list, no search results). This is a simple component to implement and
provides a consistent pattern instead of ad-hoc "No items" text.

### 3.5 Editable

Inline click-to-edit text:

```tsx
<Editable.Root defaultValue="Click to edit">
  <Editable.Preview />  {/* shows text normally */}
  <Editable.Input />     {/* shows input on click */}
</Editable.Root>
```

Compound parts: Preview (display mode), Input (edit mode), Textarea (multiline edit),
Control (edit/submit/cancel triggers).

**afterhours relevance:** Editable text is useful for game UIs — renaming characters,
editing save file names, custom team names. The Preview/Input toggle pattern is clean
and maps to a state machine (display → editing → display).

### 3.6 Snacks

Notification system for form contexts:

```tsx
const { snack, snacks } = useSnacks()

snack({ title: "Saved", description: "Your changes were saved" })

<Snacks snacks={snacks} />
```

Different from a global toast system — Snacks are scoped to a specific UI region
(typically above a form), stacking vertically.

**afterhours relevance:** This is a localized toast/notification system. `afterhours`
could support both global toasts (screen-level) and scoped snacks (container-level)
notifications.

### 3.7 Stat

Numeric display with trend:

```tsx
<Stat.Root>
  <Stat.Label>Total Page Views</Stat.Label>
  <Stat.Value>1,993,818</Stat.Value>
  <Stat.HelperMessage>
    <Stat.Icon type="increase" />
    21% more than last month
  </Stat.HelperMessage>
</Stat.Root>
```

**afterhours relevance:** Stat displays are essential for game UIs — scores, player
stats, currency, damage numbers. The Label + Value + Helper structure is a useful
pattern. The `Stat.Icon` with `type="increase"` / `type="decrease"` automatically
shows up/down arrow + green/red coloring.

### 3.8 CheckboxCard / RadioCard

Selectable cards (visual alternative to checkboxes/radios):

```tsx
<RadioCardGroup.Root>
  <RadioCardGroup.Item.Root value="warrior">
    <RadioCardGroup.Item.Label>Warrior</RadioCardGroup.Item.Label>
    <RadioCardGroup.Item.Description>
      High HP, melee damage, heavy armor
    </RadioCardGroup.Item.Description>
  </RadioCardGroup.Item.Root>
  {/* more items... */}
</RadioCardGroup.Root>
```

**afterhours relevance:** This is exactly the pattern needed for character class
selection, difficulty selection, item comparison — anywhere the user picks from
visually rich options. Much better UX than plain radio buttons for game contexts.

### 3.9 Indicator

Badge overlay positioned at a corner of another element:

```tsx
<Indicator label="3" colorScheme="danger">
  <Avatar src="..." />
</Indicator>
```

Positions: top-left, top-right, bottom-left, bottom-right. Can show text label
or just a dot.

**afterhours relevance:** Notification badges on icons/avatars are useful for game
UIs — unread messages, new items, quest updates. Simple to implement as a positional
overlay.

### 3.10 InfiniteScrollArea

Auto-loads more content when scrolled to the bottom:

```tsx
<InfiniteScrollArea
  loading={<Spinner />}
  onLoad={({ finish, index }) => {
    loadMore()
    if (index >= maxPages) finish()
  }}
>
  {items.map(item => <Card key={item.id} />)}
</InfiniteScrollArea>
```

**afterhours relevance:** Infinite scroll is useful for large data sets in game UIs —
leaderboards, item catalogs, chat history. The `onLoad` / `finish()` callback pattern
is clean. Implementation requires detecting when scroll reaches the bottom of the
content and firing a load callback.

### 3.11 Resizable

Split panels with a draggable divider:

```tsx
<Resizable.Root>
  <Resizable.Item>Panel One</Resizable.Item>
  <Resizable.Trigger />  {/* the drag handle/divider */}
  <Resizable.Item>Panel Two</Resizable.Item>
</Resizable.Root>
```

Supports horizontal/vertical orientation, min/max sizes, keyboard resize.

**afterhours relevance:** Already planned in `docs/37_resizable_panels.md`. Yamada's
compound API (Root > Item + Trigger + Item) is a clean pattern to follow.

### 3.12 PinInput

OTP/PIN code entry (multiple single-character inputs):

```tsx
<PinInput.Root />  {/* renders 4 single-char input boxes by default */}
```

Supports: configurable field count, auto-focus-next on input, paste handling,
masked/password mode, variant/size system.

**afterhours relevance:** PIN input is useful for game UIs — room codes, unlock codes,
multiplayer lobby codes. The auto-advance-on-input behavior is the key UX feature.

### 3.13 Rating

Star rating input:

```tsx
<Rating defaultValue={3} />
```

Supports: custom icons (not just stars), half-star precision, read-only mode,
color scheme, 5 sizes.

**afterhours relevance:** Rating is useful for game UIs — difficulty rating, review
scores, item quality display. Already recommended in the DaisyUI analysis.

---

## 4. Animation Primitives

Yamada UI provides 12 named animation components:

| Animation | Effect | Duration Control | afterhours Equivalent |
|-----------|--------|-----------------|----------------------|
| **Airy** | Floating/breathing scale oscillation | Infinite loop | Not yet |
| **Collapse** | Height collapse to 0 / expand | Duration + easing | Partial |
| **Fade** | Opacity 0→1 / 1→0 | Duration + easing | Exists |
| **FadeScale** | Fade + scale from 0.8→1 | Duration + easing | Not yet |
| **Flip** | 3D rotation (X or Y axis) | Duration + easing | Not yet |
| **Loading** | Various spinner shapes | Infinite loop | Not yet |
| **Motion** | General-purpose animation wrapper | Spring/keyframe | Partial |
| **Ripple** | Material ripple from point | Duration | Not yet |
| **Rotate** | Rotation | Duration + easing | Exists |
| **Skeleton** | Loading placeholder shimmer | Infinite loop | Not yet |
| **Slide** | Slide from edge (top/right/bottom/left) | Duration + easing | Not yet |
| **SlideFade** | Slide + fade combined | Duration + easing | Not yet |

**afterhours relevance:** These are high-value animation presets. The most impactful
to add:
1. **Collapse** — essential for accordion/panel expand
2. **SlideFade** — common for modal/drawer entrance
3. **Flip** — card flip for game UIs
4. **Skeleton** — loading placeholder (shimmer effect)
5. **Airy** — idle "breathing" animation for game elements

---

## 5. Chart Components

Yamada UI includes 7 chart types built on [Recharts](https://recharts.org/):

| Chart | Description | Game UI Use Case |
|-------|-------------|-----------------|
| **AreaChart** | Filled area under line | Resource over time |
| **BarChart** | Vertical/horizontal bars | Stat comparison |
| **DonutChart** | Ring with center label | Completion percentage |
| **LineChart** | Line graph | Performance trend |
| **PieChart** | Pie segments | Resource distribution |
| **RadarChart** | Spider/radar polygon | RPG character stats |
| **RadialChart** | Radial progress bars | Multi-stat display |

**afterhours relevance:** Charts are a gap in `afterhours`. For game UIs, the most
valuable are:
1. **RadarChart** — RPG stat displays (STR/DEX/INT/WIS/CHA/CON)
2. **BarChart** — damage stats, leaderboard bars
3. **DonutChart** — completion rings, health/mana displays
4. **LineChart** — performance over time

Implementation would use `afterhours`' drawing primitives (lines, arcs, filled
polygons) rather than an external charting library.

---

## 6. Prioritized Recommendations for afterhours

### 6.1 High Priority (New Components)

| Component | Effort | Impact | Notes |
|-----------|--------|--------|-------|
| **Steps** | Medium | High | Multi-step progress; widely recommended |
| **EmptyState** | Low | High | Simple but universally needed |
| **Stat** | Low | High | Label + Value + Trend; essential for game UIs |
| **RadioCard / CheckboxCard** | Medium | High | Rich selection cards; character/item selection |
| **SegmentedControl** | Medium | High | Button group selector; validated by KivyMD too |
| **Editable** | Medium | Medium | Click-to-edit text; rename characters/saves |
| **Timeline** | Medium | Medium | Quest progression, event logs |

### 6.2 Medium Priority

| Component | Effort | Impact | Notes |
|-----------|--------|--------|-------|
| **Rating** | Low | Medium | Star rating; item quality, reviews |
| **Indicator** | Low | Medium | Corner badge; notification counts |
| **PinInput** | Medium | Medium | Code entry; room codes, unlock codes |
| **InfiniteScrollArea** | Medium | Medium | Auto-load on scroll; leaderboards |
| **Collapse animation** | Low | High | Height expand/collapse; accordion enabler |
| **RadarChart** | High | Medium | Spider chart; RPG stats |
| **Snacks (scoped toasts)** | Medium | Medium | Container-scoped notifications |
| **Flip animation** | Low | Medium | 3D card flip |

### 6.3 Lower Priority

| Component | Effort | Impact | Notes |
|-----------|--------|--------|-------|
| **Autocomplete** | High | Low | Text + suggestions dropdown |
| **PasswordInput** | Low | Low | Password with toggle |
| **DataList** | Low | Low | Term + description pairs |
| **Skeleton** | Medium | Low | Loading placeholder shimmer |
| **ZStack** | Low | Low | Layered children |
| **Carousel** | High | Medium | Already analyzed in Swiper |
| **Calendar** | High | Low | Full calendar widget |

---

## 7. Key Takeaways

### 7.1 Broadest Component Catalog Analyzed

At 121 components (including planned), Yamada UI is the largest library analyzed.
It validates that a mature UI system needs components beyond the basics — EmptyState,
Stat, Timeline, Rating, Indicator, and Editable are all "small but essential" components
that `afterhours` should add.

### 7.2 Compound Components Are Universal

This is now the fourth independent library (Base UI, KivyMD, Radix, Yamada UI) using
the compound component pattern. The dot-notation (`Component.Root` > `Component.Item`)
is the dominant API pattern. `afterhours` should adopt this for all complex components.

### 7.3 CheckboxCard / RadioCard Are High-Value

Rich selection cards (showing label + description + visual in a selectable card)
are a pattern that game UIs need frequently (class selection, item comparison, mode
selection). No previous analysis highlighted this as a standalone component — Yamada UI
makes it first-class.

### 7.4 The "Small Components" Gap

`afterhours` has a gap in small, simple components: EmptyState, Stat, Status, Indicator,
Tag, Kbd, Mark, Highlight. Each takes minimal effort to implement but collectively they
make a UI framework feel complete rather than barebones.

### 7.5 Animation Presets as Named Components

Yamada UI's approach of providing named animation components (Collapse, SlideFade, Flip,
Airy) rather than just a generic animation API makes common patterns trivially easy to
use. `afterhours` should offer similar named presets alongside its lower-level animation
system.
