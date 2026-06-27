# KivyMD Analysis for afterhours

## Overview

[KivyMD](https://kivymd.readthedocs.io/en/latest/) (v2.0) is a collection of Material
Design widgets for the [Kivy](https://kivy.org/) Python framework. Unlike web-based
component libraries, KivyMD targets **native cross-platform apps** (Android, iOS,
Windows, macOS, Linux) using Python and a custom declarative language (KV). This makes
it one of the closest analogues to `afterhours` among all libraries analyzed so far,
since both are non-web, rendering-engine-based UI toolkits implementing Material Design.

KivyMD follows **Material Design 3** (M3) specifications and provides 50+ components,
15 behavior mixins, a dynamic color system, and screen transition animations — all
rendered through an OpenGL-based graphics engine rather than a browser DOM.

**Source:** https://kivymd.readthedocs.io/en/latest/components/ and GitHub source at
`kivymd/KivyMD` (docstrings extracted directly from component source files)

---

## 1. Architecture

### 1.1 Key Architectural Patterns

KivyMD uses a **mixin-based composition** architecture:

- **Widgets** are concrete UI components (MDButton, MDCard, MDDialog, etc.)
- **Behaviors** are mixins that add capabilities to any widget (Elevation, Ripple,
  Hover, Touch, Focus, Scale, Rotate, Toggle, Magic)
- **Theme** is a centralized singleton (`theme_cls`) that all widgets reference
- **Declarative KV** language separates UI structure from Python logic

This is analogous to `afterhours`' ECS approach where:
- **Entities** = widgets
- **Components** = behaviors/mixins
- **Systems** = rendering and input processing

### 1.2 Declarative vs. Imperative Styles

KivyMD supports two UI construction styles:

**Declarative KV** (preferred):
```
MDButton:
    style: "elevated"
    MDButtonIcon:
        icon: "plus"
    MDButtonText:
        text: "Elevated"
```

**Imperative Python**:
```python
MDButton(
    MDButtonIcon(icon="plus"),
    MDButtonText(text="Elevated"),
    style="elevated",
)
```

**afterhours relevance:** `afterhours` uses an imperative immediate-mode style. The
KivyMD declarative approach with nested child composition (icon + text as children of
button) is worth studying. The compound component pattern (MDButton containing
MDButtonIcon + MDButtonText as separate entities) maps well to ECS child relationships.

### 1.3 Theme System

KivyMD's theming follows M3's dynamic color system:
- `theme_cls.primary_palette` — sets the primary hue (e.g., "Green", "Blue")
- `theme_cls.theme_style` — "Light" or "Dark"
- `theme_cls.surfaceColor`, `theme_cls.backgroundColor`, etc. — computed from palette
- **Dynamic color** — generates color schemes from arbitrary source colors or
  wallpaper extraction (Android 12+ style)
- All widgets automatically update when theme changes

**afterhours relevance:** `afterhours` has a Theme struct but components don't
auto-update when it changes (immediate-mode rebuilds each frame anyway). The dynamic
color generation (deriving a full palette from a single seed color) is the key
takeaway — this would let `afterhours` users set one primary color and have the
entire UI palette derived automatically.

---

## 2. Component Inventory

### 2.1 Full Component List

KivyMD provides **52 components** organized by category:

#### Layout Components (11)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **AnchorLayout** | Positions children at anchor points | Exists (alignment) |
| **BoxLayout** | Horizontal/vertical linear layout | Exists |
| **CircularLayout** | Arranges children in a circle | Not planned |
| **FloatLayout** | Free-positioning layout | Exists (absolute) |
| **GridLayout** | Row/column grid | Exists |
| **RelativeLayout** | Children positioned relative to parent | Exists |
| **ResponsiveLayout** | Switches between mobile/tablet/desktop views | Planned (docs/29) |
| **StackLayout** | Flow layout, wraps children | Partial |
| **Screen** | Single screen/page container | Exists |
| **ScreenManager** | Manages screen transitions | Partial |
| **ScrollView** | Scrollable content area | Exists |

#### Navigation Components (4)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Appbar** (Top + Bottom) | App bar with title, leading/trailing actions; 4 types: center-aligned, small, medium, large | Exists (header bar) |
| **Navigation bar** | Bottom navigation for 3-5 primary destinations | Planned (docs/21) |
| **NavigationDrawer** | Side drawer (standard or modal) for 5+ destinations | Planned (docs/21) |
| **NavigationRail** | Compact side nav for mid-sized screens; 3-7 destinations + optional FAB | Not planned |

#### Input Components (7)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Button** | 9 variants: elevated, filled, tonal, outlined, text, icon, segmented, FAB, extended FAB | Exists (partial variants) |
| **Chip** | 4 types: assist, filter, input, suggestion | Not planned |
| **SelectionControls** | Checkbox + Switch | Exists |
| **SegmentedButton** | Single-select or multi-select group; 2-5 items | Not planned |
| **Slider** | Continuous or discrete range selection | Exists |
| **Text fields** | Filled + outlined; with leading/trailing icons, hint text, helper text, max length, error state | Exists |
| **DropdownItem** | Dropdown trigger that opens a menu | Exists (dropdown) |

#### Display Components (8)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Card** | 3 types: elevated, filled, outlined; supports swipe-to-dismiss (MDCardSwipe) | Exists (basic) |
| **Badge** | Notification badge overlay | Planned (docs/13) |
| **Divider** | Horizontal + vertical thin lines | Exists |
| **ImageList** | Grid of images with overlay content (MDSmartTile) | Not planned |
| **Label** | Text rendering with theme integration | Exists |
| **List** | 1-3 line items with leading icon/avatar, trailing icon/checkbox | Exists (basic) |
| **FitImage** | Aspect-ratio-preserving image display | Not planned |
| **LoadingIndicator** | M3 morphing shape animations (indeterminate) | Not planned |

#### Feedback Components (5)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Dialog** | Basic dialog with icon, headline, supporting text, buttons, custom content | Exists (modal) |
| **Snackbar** | Bottom notification with optional action + dismiss icon | Planned (docs/14) |
| **Tooltip** | Plain (text only) + Rich (title, description, link, buttons) | Planned (docs/10) |
| **ProgressIndicator** | Linear + circular, determinate + indeterminate | Planned (docs/32) |
| **BottomSheet** | Standard (persistent) + modal; anchored to bottom | Not planned |

#### Picker Components (2)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **DatePicker** | 3 types: docked, modal, modal input; supports date ranges | Planned (docs/34) |
| **TimePicker** | 3 types: vertical dial, horizontal dial, input | Planned (docs/34) |

#### Advanced Components (7)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Hero** | Shared element transition between screens | Not planned |
| **Transition** | Screen transition animations (fade-slide, etc.) | Partial |
| **Swiper** | Touch-swipeable card carousel | Not planned |
| **SliverAppbar** | Collapsible/scrollable app bar with parallax header | Not planned |
| **ExpansionPanel** | Collapsible content panel (accordion) | Planned |
| **Menu** | Dropdown menu with positioned items | Exists |
| **RefreshLayout** | Pull-to-refresh scroll container | Not planned |

#### Utility Components (3)

| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **FileManager** | Directory/file browser with preview mode | Not planned |
| **RecycleView** / **RecycleGridLayout** / **MDRecycleBoxLayout** | Virtualized lists for large datasets | Partial (virtual scroll) |
| **Widget** | Base widget class | Exists |

### 2.2 Component Count Summary

| Category | KivyMD Count | afterhours Exists | afterhours Planned | Gap |
|----------|-------------|-------------------|-------------------|-----|
| Layout | 11 | 8 | 1 | 2 |
| Navigation | 4 | 1 | 2 | 1 |
| Input | 7 | 5 | 0 | 2 |
| Display | 8 | 4 | 1 | 3 |
| Feedback | 5 | 1 | 3 | 1 |
| Pickers | 2 | 0 | 2 | 0 |
| Advanced | 7 | 2 | 1 | 4 |
| Utility | 3 | 1 | 0 | 2 |
| **Total** | **47** | **22** | **10** | **15** |

---

## 3. Behavior Mixins (Deep Dive)

KivyMD's behavior system is its most architecturally interesting feature. Behaviors
are mixins that add cross-cutting concerns to any widget. This maps directly to
ECS components in `afterhours`.

### 3.1 Elevation Behavior

```python
class CommonElevationBehavior:
    elevation_level = 0          # 0-5
    shadow_offset = (0, -6)      # x, y offset
    shadow_color = (0, 0, 0, .6) # shadow RGBA
    shadow_softness = 12         # blur radius
```

- Applied as a mixin to any widget
- Supports custom elevation levels and shadow parameters
- Follows M3 elevation spec

**afterhours relevance:** This validates the elevation system proposed in multiple
previous analyses. The mixin approach maps to an `HasElevation` component in ECS:

```cpp
struct HasElevation {
    int level = 0;           // 0-5
    vec2 shadow_offset = {0, -6};
    Color shadow_color = {0, 0, 0, 0.6f};
    float shadow_softness = 12.f;
};
```

### 3.2 Ripple Behavior

Two variants:
- **CircularRippleBehavior** — circular ripple emanating from touch point
- **RectangularRippleBehavior** — rectangular ripple constrained to widget bounds

Properties:
- `ripple_scale` — maximum scale of the ripple circle (default varies)
- `ripple_color` — RGBA of the ripple
- `ripple_duration_in_fast` / `ripple_duration_in_slow` — animation timing

**afterhours relevance:** Ripple feedback has been recommended in the Materialize and
M2 analyses. KivyMD's approach of having two ripple shapes (circular for round buttons,
rectangular for cards/lists) is a practical implementation detail.

### 3.3 State Layer Behavior

Implements M3's state overlay system — semi-transparent overlays for hover, focus,
pressed, and dragged states. Works in conjunction with the Ripple behavior.

**afterhours relevance:** Directly implements the overlay opacity system described
in the M2 analysis (4%/8%/12% overlays).

### 3.4 Touch Behavior

Adds three touch events to any widget:
- `on_long_touch` — fired after a held press
- `on_double_tap` — fired on double-tap
- `on_triple_tap` — fired on triple-tap

**afterhours relevance:** `afterhours` currently only has `on_click`. Long press,
double-tap, and triple-tap are missing input events that should be added to the
input system. Long press is especially useful for game UIs (inspect, charge, context
menu).

### 3.5 Hover Behavior

Fires `on_enter` and `on_leave` events when the mouse enters/leaves a widget. Has
a `detect_visible` property that checks if the widget is actually visible (not
covered by a modal, not in a hidden tab, etc.) before firing events.

**afterhours relevance:** The `detect_visible` check is a smart detail. `afterhours`
should also skip hover events for widgets that are obscured by overlays or in
inactive tabs. This requires a z-order / visibility check in the hover system.

### 3.6 Focus Behavior

Changes background color when the widget has keyboard/tab focus. Fires `on_enter`
and `on_leave` based on focus state.

### 3.7 Magic Behavior

Provides five playful animation effects for buttons:
- **wobble** — oscillating rotation
- **grow** — scale up from press point
- **shake** — horizontal shake
- **twist** — full rotation
- **shrink** — scale down

**afterhours relevance:** These are "juice" effects for game UIs. Simple to implement
as animation presets but high-impact for making interfaces feel alive. Could be
added as `AnimPreset::Wobble`, `AnimPreset::Shake`, etc.

### 3.8 Rotate / Scale Behaviors

Provide transform controls (angle, scale x/y/z, origin) that integrate with the
Kivy canvas graphics pipeline. These are lower-level building blocks for the
Magic behavior and custom animations.

### 3.9 Toggle Behavior

Adds radio-group toggling to any button. Buttons with the same `group` string
are mutually exclusive — selecting one deselects the others.

**afterhours relevance:** `afterhours` has radio button support but the toggle
behavior as a reusable mixin that can be applied to *any* widget (buttons, cards,
chips, segments) is a cleaner pattern.

### 3.10 Motion Behavior

Implements display/dismiss animations for transient widgets (dialogs, menus,
snackbars, etc.). Follows M3 motion guidelines.

### 3.11 Declarative Behavior

Enables the imperative Python style of UI construction (nesting widgets as
constructor arguments) alongside the KV declarative style:

```python
MDScreen(
    MDButton(
        MDButtonText(text="Hello"),
        style="filled",
    ),
)
```

**afterhours relevance:** This is the closest analogue to `afterhours`' immediate-mode
builder pattern. The compound nesting of children as constructor arguments is a
retained-mode version of `afterhours`' `begin()/end()` scoping.

---

## 4. Compound Component Pattern

A key KivyMD pattern is **compound components** — complex widgets decomposed into
named sub-parts that the user assembles:

### 4.1 Examples

**Button** = MDButton + MDButtonIcon + MDButtonText
```
MDButton:
    style: "elevated"
    MDButtonIcon:
        icon: "plus"
    MDButtonText:
        text: "Elevated"
```

**List Item** = MDListItem + MDListItemLeadingIcon + MDListItemHeadlineText + MDListItemSupportingText + MDListItemTertiaryText + MDListItemTrailingIcon
```
MDListItem:
    MDListItemLeadingIcon:
        icon: "account"
    MDListItemHeadlineText:
        text: "Primary text"
    MDListItemSupportingText:
        text: "Secondary text"
    MDListItemTrailingIcon:
        icon: "chevron-right"
```

**Dialog** = MDDialog + MDDialogIcon + MDDialogHeadlineText + MDDialogSupportingText + MDDialogButtonContainer + MDDialogContentContainer

**Navigation Bar** = MDNavigationBar + MDNavigationItem + MDNavigationItemIcon + MDNavigationItemLabel

**Text Field** = MDTextField + MDTextFieldLeadingIcon + MDTextFieldHintText + MDTextFieldHelperText + MDTextFieldTrailingIcon + MDTextFieldMaxLengthText

**Slider** = MDSlider + MDSliderHandle + MDSliderValueLabel

**Chip** = MDChip + MDChipLeadingAvatar / MDChipLeadingIcon + MDChipText + MDChipTrailingIcon

### 4.2 Pattern Analysis

Every compound component follows the same structure:
1. **Container** — the main widget (MDButton, MDListItem, MDDialog)
2. **Named slots** — typed children with explicit roles (LeadingIcon, HeadlineText, TrailingIcon)
3. **Optional parts** — omitting a child removes that feature (no icon = no icon displayed)
4. **Type safety** — each slot is a distinct class, so misplacing a TrailingIcon where a LeadingIcon belongs is a visible error

**afterhours relevance:** This is the strongest pattern for `afterhours` to adopt.
Currently, `afterhours` configures complex widgets through a single `ComponentConfig`
with many optional fields. The compound approach would instead have:

```cpp
// Instead of:
config.with_icon("plus").with_text("Elevated").with_style(ButtonStyle::Elevated)

// Compound approach:
Button(ButtonStyle::Elevated)
    .with(ButtonIcon("plus"))
    .with(ButtonText("Elevated"))
```

This makes the widget tree inspectable, the parts individually styleable, and the
API self-documenting.

---

## 5. Unique Components Worth Adopting

### 5.1 Hero (Shared Element Transition)

KivyMD's Hero implements shared element transitions between screens:
- Place `MDHeroFrom` on screen A with a tag
- Place `MDHeroTo` on screen B with the same tag
- When navigating A→B, the hero widget animates from its position in A to its
  position in B, morphing shape along the way

This is the same pattern used in Flutter, Android, and iOS navigation. It's
the most impactful navigation animation for creating a sense of spatial
continuity.

**afterhours relevance:** Hero/shared element transitions are not currently planned
but would be valuable for game UIs (e.g., selecting an item in a grid → item flies
to a detail view). Implementation requires:
1. Tagging widgets that should participate in hero transitions
2. Capturing the source widget's screen position and size
3. Animating position + size + shape to the target
4. Hiding the original and target during the animation

### 5.2 SliverAppbar (Collapsible Header)

A scrollable app bar that:
- Starts expanded (showing a large image/header)
- Collapses to a compact app bar on scroll
- Parallax effect on the header image during collapse

**afterhours relevance:** Collapsible headers are useful in game UIs for
inventory/shop screens where a banner image collapses into a toolbar on scroll.
Requires coordination between a scroll view and a header component.

### 5.3 Card Swipe (MDCardSwipe)

Cards that can be swiped left/right to reveal actions underneath (like iOS
swipe-to-delete). Properties include swipe direction, opening point, and
swipe distance.

**afterhours relevance:** Swipe-to-reveal on cards and list items is a common
mobile pattern. For `afterhours`, this could be implemented as a behavior/mixin
on any rectangular widget.

### 5.4 Loading Indicator (Shape Morphing)

M3's loading indicator animates between predefined geometric shapes (circle,
square, triangle, etc.) using morphing animations. It's indeterminate (no
progress value) and purely visual, used for background loading states.

**afterhours relevance:** Shape morphing is a visually distinctive loading
pattern. Could be implemented using the bezier curve capabilities proposed in
the OpenFL analysis.

### 5.5 Segmented Button

A group of 2-5 mutually exclusive (single-select) or multi-select buttons:
- Can contain icon, text, or both
- Visual selection indicator (checkmark + fill)
- Horizontal row with connected borders

**afterhours relevance:** Segmented buttons are a common game UI pattern
(difficulty selection, mode selection, filter toggles). Should be added to
`afterhours` as a first-class component.

### 5.6 Bottom Sheet

A surface anchored to the bottom of the screen:
- **Standard**: persistent, co-exists with main content
- **Modal**: blocks interaction with main content, dimmed scrim

**afterhours relevance:** Bottom sheets are useful for mobile-style game UIs
(character stats, item details, action menus). Implementation is a panel that
slides up from the bottom with configurable height and drag-to-dismiss.

---

## 6. Button Taxonomy (9 Variants)

KivyMD implements the full M3 button taxonomy, which is the most comprehensive
button system analyzed across all libraries:

| # | Variant | Visual | Emphasis | Use Case |
|---|---------|--------|----------|----------|
| 1 | **Elevated** | Shadow + filled surface | Medium-high | Important but not primary |
| 2 | **Filled** | Solid primary color fill | Highest | Primary action (1 per screen) |
| 3 | **Filled Tonal** | Tonal color fill (lighter) | Medium | Secondary but still prominent |
| 4 | **Outlined** | Border only, no fill | Medium-low | Alternative/cancel actions |
| 5 | **Text** | No border, no fill | Lowest | Tertiary actions, inline links |
| 6 | **Icon** | Icon only, no text | Varies | Toolbar actions, toggles |
| 7 | **Segmented** | Connected group | Varies | Multi-option selection |
| 8 | **FAB** | Large, circular/pill | High | Primary floating action |
| 9 | **Extended FAB** | FAB with text label | High | Primary action with description |

**afterhours relevance:** `afterhours` currently supports basic buttons with
`ButtonStyle` variants. Expanding to the full M3 taxonomy would give users a
principled way to express emphasis hierarchy. The most impactful additions are:
- **Tonal** (between filled and outlined)
- **FAB / Extended FAB** (floating primary action)
- **Segmented** (group selection)

---

## 7. List Item Anatomy

KivyMD's list items are the most richly structured of any library analyzed:

```
┌─────────────────────────────────────────────────────┐
│ [Leading]  Headline Text          [Trailing]        │
│            Supporting Text                          │
│            Tertiary Text                            │
└─────────────────────────────────────────────────────┘
```

Slot types:
- **Leading**: Icon (`MDListItemLeadingIcon`) or Avatar (`MDListItemLeadingAvatar`)
- **Headline**: Primary text (required)
- **Supporting**: Secondary text (optional, makes it 2-line)
- **Tertiary**: Third line (optional, makes it 3-line)
- **Trailing**: Icon (`MDListItemTrailingIcon`) or Checkbox (`MDListItemTrailingCheckbox`)

Three density levels: one-line (48dp), two-line (64dp), three-line (88dp).

**afterhours relevance:** `afterhours` list items are simple text rows. Adopting the
KivyMD anatomy (leading slot, 1-3 text lines, trailing slot) would make lists
dramatically more useful for game UIs (inventory with icons, settings with toggles,
leaderboards with avatars).

---

## 8. Text Field Anatomy

KivyMD text fields have the richest sub-component structure:

```
┌─────────────────────────────────────────────────────┐
│ [Leading Icon]  Hint Text / Value    [Trailing Icon]│
│                                                     │
│ Helper Text                        Max Length (0/10)│
└─────────────────────────────────────────────────────┘
```

Sub-components:
- `MDTextFieldLeadingIcon` — icon before the input (e.g., magnifying glass)
- `MDTextFieldHintText` — placeholder that animates to a floating label on focus
- `MDTextFieldHelperText` — helper text below the field (persistent or on-focus)
- `MDTextFieldTrailingIcon` — icon after the input (e.g., clear button, info)
- `MDTextFieldMaxLengthText` — character counter (e.g., "3/10")

Two visual modes:
- **Filled** — grey background with bottom border
- **Outlined** — full border, floating label at top

States: empty → focused (label floats up) → filled → error → disabled

**afterhours relevance:** `afterhours` text fields are basic input boxes. Adding:
1. Floating label animation (label moves from inside to above the field on focus)
2. Helper text slot (persistent or on-error)
3. Character counter
4. Leading/trailing icon slots

would significantly improve the text input UX.

---

## 9. Comparison with afterhours' Existing Architecture

### 9.1 Mixin Behaviors vs. ECS Components

| KivyMD Behavior | afterhours Equivalent | Status |
|-----------------|----------------------|--------|
| Elevation | `HasElevation` (proposed) | Not yet |
| Ripple | `HasRipple` (proposed) | Not yet |
| StateLayer | State overlay in render | Partial |
| Touch | Input events | Partial (no long-press, double-tap) |
| Hover | `HasHover` | Exists |
| Focus | `HasFocus` (proposed) | Partial |
| Toggle | Radio group logic | Exists |
| Rotate | Transform component | Exists |
| Scale | Transform component | Exists |
| Magic (wobble/shake/grow) | Animation presets | Not yet |
| Motion (enter/exit) | Animation definitions | Partial |
| Declarative | Immediate-mode builder | Different paradigm |

### 9.2 KivyMD as a Native Framework

KivyMD is uniquely relevant because:
1. **Not web-based** — renders through OpenGL, like `afterhours`
2. **Python + KV** — has both imperative and declarative styles
3. **Material Design 3** — implements the latest spec version
4. **Cross-platform** — same code on all platforms, like `afterhours`
5. **Retained mode** — but with a declarative KV language that makes it feel reactive

The biggest difference: KivyMD is **retained mode** (widget tree persists and updates),
while `afterhours` is **immediate mode** (UI rebuilt each frame). This means KivyMD's
animation and state management patterns need translation for `afterhours`.

---

## 10. Prioritized Recommendations for afterhours

### 10.1 High Priority

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Compound component pattern** | Medium | High | Decompose complex widgets into named sub-parts |
| **Rich list items** | Medium | High | Leading/trailing slots, 1-3 line density |
| **Segmented button** | Medium | High | 2-5 item single/multi-select group |
| **Touch events (long press, double-tap)** | Low | High | Add to input system |
| **Magic effects (wobble, shake, grow)** | Low | Medium | Animation presets for game UI juice |
| **Toggle behavior as mixin** | Low | Medium | Reusable radio-group logic for any widget |

### 10.2 Medium Priority

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Bottom sheet** | Medium | Medium | Slide-up panel for mobile-style UIs |
| **Hero/shared element transition** | High | Medium | Widget flies between screens |
| **Text field floating label** | Medium | Medium | Label animates from placeholder to floating |
| **Collapsible app bar (sliver)** | Medium | Medium | Parallax scroll-collapse header |
| **Card swipe-to-reveal** | Medium | Medium | Swipe to show actions underneath |
| **Hover visibility check** | Low | Medium | Skip hover events for obscured widgets |
| **Dynamic color generation** | Medium | Medium | Derive full palette from seed color |

### 10.3 Lower Priority

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Circular layout** | Low | Low | Arrange children in a circle |
| **Image list (smart tile)** | Medium | Low | Grid with overlay content |
| **Loading indicator (shape morph)** | High | Low | Morphing geometric shapes |
| **File manager** | High | Low | Directory/file browser |
| **Pull-to-refresh** | Medium | Low | Drag-down to refresh content |

---

## 11. Key Takeaways

### 11.1 Compound Components Are the Right Pattern

KivyMD's most impactful pattern is decomposing complex widgets into **named, typed
sub-parts** (MDButtonIcon, MDListItemHeadlineText, MDDialogSupportingText). This makes
the API self-documenting, each part independently styleable, and the widget tree
inspectable. `afterhours` should adopt this for buttons, list items, text fields,
dialogs, and cards.

### 11.2 Behaviors as Reusable Mixins

KivyMD's behavior system (Elevation, Ripple, Hover, Touch, Toggle, Magic) maps
perfectly to ECS components. Each behavior is a standalone concern that can be
composed onto any widget. `afterhours` should formalize its existing behaviors
(hover, focus) and add missing ones (elevation, ripple, long-press, magic effects)
as reusable ECS components.

### 11.3 The Touch Event Gap

KivyMD's `TouchBehavior` exposes `on_long_touch`, `on_double_tap`, and
`on_triple_tap` — all missing from `afterhours`. Long press is critical for game
UIs (hold-to-inspect, hold-to-charge, context menus).

### 11.4 Button Emphasis Hierarchy

The 9-variant button taxonomy (elevated → filled → tonal → outlined → text → icon →
segmented → FAB → extended FAB) provides a principled way to express emphasis. At
minimum, `afterhours` should add **tonal** (medium emphasis) and **FAB** (floating
action) variants.

### 11.5 Native Framework Validation

KivyMD validates that Material Design can be fully implemented in a non-web,
OpenGL-based rendering framework — the same architecture as `afterhours`. This
means all M3 components, behaviors, and animations are achievable in `afterhours`
without web-specific dependencies.
