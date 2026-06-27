# Material Design 2 Analysis for afterhours

## Overview

[Material Design 2](https://m2.material.io/) (codename "Quantum Paper") is a comprehensive
design language developed by Google, first announced in 2014 and overhauled in 2018.
It synthesizes classic design principles with technology innovation: material surfaces
inspired by paper and ink, a physics-based motion system, and a principled approach
to color, typography, shape, and interaction.

Material Design 2 is structured as three layers:
1. **Material System** — Core philosophy and material studies
2. **Material Foundation** — Environment, layout, navigation, color, typography, sound, iconography, shape, motion, interaction, communication
3. **Material Guidelines** — Theming, usability, platform guidance

This document covers the full M2 spec as relevant to `afterhours`, analyzing principles,
design tokens, component patterns, and identifying what to adopt.

**Source:** https://m2.material.io/design/introduction (JS-rendered; content extracted
from site metadata API, navigation structure, and cached references)

---

## 1. Core Principles

Material Design 2 rests on three foundational principles:

### 1.1 Material Is the Metaphor

> A material metaphor is the unifying theory of a rationalized space and a system of
> motion. The material is grounded in tactile reality, inspired by the study of paper
> and ink, yet technologically advanced and open to imagination and magic.

Key ideas:
- **Surfaces and edges** provide visual cues grounded in reality
- **Familiar tactile attributes** help users quickly understand affordances
- **Flexibility** of the material creates new affordances that supersede the physical world, without breaking the rules of physics
- **Fundamentals of light, surface, and movement** convey how objects move, interact, and exist in space

**afterhours relevance:** This maps directly to the "material as surface" model. In
`afterhours`, every UI component is conceptually a rectangular surface with position,
size, and depth. The metaphor reinforces that `afterhours` should formalize:
- Surface layering (z-depth / elevation)
- Edge visibility (borders, shadows)
- Physical response to interaction (ripple, press, lift)

### 1.2 Bold, Graphic, Intentional

Material Design draws from print design: typography, grids, space, scale, color, and
imagery to create hierarchy, meaning, and focus. These elements guide the eye and
communicate structure without relying on decoration.

Key ideas:
- **Typography** creates structure and meaning
- **Color** is purposeful, reinforcing brand and hierarchy
- **Imagery** is intentional, not decorative
- **White space** is a design element, not unused space

**afterhours relevance:** Reinforces that `afterhours` should have:
- A structured type scale (not ad-hoc font sizes)
- Semantic color roles with clear hierarchy
- Deliberate spacing/padding system (not arbitrary pixel values)

### 1.3 Motion Provides Meaning

Motion in Material Design is not decorative — it focuses attention, maintains continuity,
and provides feedback about spatial relationships.

Key ideas:
- Objects **transform and reorganize** to provide continuity
- Motion **focuses attention** and guides the user
- Transitions maintain **spatial relationships**
- Feedback is **immediate** and confirms actions

**afterhours relevance:** Maps to the animation system analysis in `react_spring_analysis.md`.
Motion should be physics-based, interruptible, and purposeful — not just "ease-in-out
everything".

---

## 2. Material Foundation

### 2.1 Environment

The Material Design environment is a 3D space with light sources, surfaces at varying
depths, and shadows that communicate hierarchy.

#### 2.1.1 Surfaces

Material surfaces are opaque, rectangular sheets that:
- Occupy a position in 3D space (x, y, z)
- Have uniform thickness (1dp)
- Can split, join, move, and resize
- Cannot pass through other surfaces
- Are rendered at discrete elevation levels

Surface properties:
| Property | Behavior |
|----------|----------|
| Opaque | Content is not visible through material |
| Rectangular | All surfaces are rectangular with optional rounded corners |
| Uniform thickness | 1dp thick regardless of size |
| Shadow-casting | Surfaces at higher elevation cast shadows on surfaces below |
| Clipping | Content is clipped to the surface boundary |

**afterhours relevance:** `afterhours` already treats components as rectangular surfaces
with clipping. Missing: formal 1dp thickness concept, surface-splitting animations, and
join/separate transitions.

#### 2.1.2 Elevation

Elevation is the relative depth (z-position) of two surfaces, measured in dp.
Material Design defines **24 elevation levels** (0dp–24dp), though most UI uses
a practical subset:

| Elevation (dp) | Component Example |
|----------------|-------------------|
| 0 | Background, flat surface |
| 1 | Card at rest, search bar, switch |
| 2 | Raised button at rest |
| 3 | Refresh indicator, search bar (scrolled) |
| 4 | App bar, FAB at rest |
| 6 | Snackbar, FAB (pressed) |
| 8 | Bottom sheet, navigation drawer, menu |
| 12 | Floating action button (pressed) |
| 16 | Navigation drawer (modal), bottom sheet (modal) |
| 24 | Dialog |

Elevation behaviors:
- **Resting elevation**: Default state elevation (e.g., card = 1dp)
- **Dynamic elevation offset**: Change in elevation from resting (e.g., card pressed = +6dp)
- **Elevation interference**: Higher surfaces are always in front of lower surfaces within the same parent
- Shadow is cast by the ambient light (overhead) and a key light (angled top-left)

**afterhours relevance:** `afterhours` has no formalized elevation system. This is the
most impactful gap identified across multiple analyses (Materialize, DaisyUI, Ant Design,
Bootstrap). Recommendation:

```cpp
// In Theme:
struct ElevationLevel {
    float shadow_offset_x;
    float shadow_offset_y;
    float shadow_blur;
    float shadow_spread;
    Color shadow_color;
};

// Standard 6-level system (condensed from M2's 24):
// Level 0: flat (no shadow)
// Level 1: cards at rest (1dp)
// Level 2: raised buttons, cards on hover (2-3dp)
// Level 3: app bars, FABs at rest (4-6dp)
// Level 4: bottom sheets, drawers (8dp)
// Level 5: dialogs, modals (16-24dp)
```

#### 2.1.3 Light and Shadows

Material Design uses two light sources:
1. **Key light** — directional, creates sharp shadows. Positioned above and slightly
   to the left (mimicking top-left sunlight)
2. **Ambient light** — omnidirectional, creates soft shadows all around

This dual-shadow system creates realistic depth perception. Each elevation level has
a pair of shadows: one sharper (key) and one softer (ambient).

**afterhours relevance:** `afterhours` currently supports a single `TextShadow`. The
dual-shadow concept could be adopted for component surfaces where each elevation level
produces two shadow layers (key + ambient) rendered together.

### 2.2 Layout

#### 2.2.1 Responsive Layout Grid

Material Design uses an **8dp grid system** with three layout elements:
1. **Columns** — content areas (4/8/12 columns depending on breakpoint)
2. **Gutters** — spaces between columns (typically 16dp or 24dp)
3. **Margins** — space between content and screen edge

Breakpoints:
| Breakpoint | Range | Columns | Margins/Gutters |
|------------|-------|---------|----------------|
| xs | 0–599dp | 4 | 16dp |
| sm | 600–904dp | 8 | 32dp |
| md | 905–1239dp | 12 | 32dp |
| lg | 1240–1439dp | 12 | 32dp |
| xl | 1440dp+ | 12 | 32dp |

#### 2.2.2 Spacing Methods

Material Design uses three spacing approaches:
- **Padding** — space within a component (8dp increments)
- **Dimensions** — component width/height (8dp increments, 4dp for dense UIs)
- **Alignment** — keylines and grid alignment

The **4dp baseline grid** is used for typography and iconography, while the **8dp grid**
is used for layout. All dimensions should be multiples of 8dp, with 4dp allowed for
small elements like icons and type.

**afterhours relevance:** `afterhours` already uses an 8px spacing system in its Theme
(`padding`, `margin` fields). The formal breakpoint system should be adopted for
responsive layout (see `docs/29_responsive_design.md`).

#### 2.2.3 Density

Material Design 2 introduced a **density system** that allows components to be made
more compact. Density levels are measured as offsets from the default:

| Density | Offset | Use Case |
|---------|--------|----------|
| Default | 0 | Touch-friendly, mobile-first |
| Comfortable | -1 | Desktop, keyboard-focused UIs |
| Compact | -2 | Dense data tables, toolbars |
| Very compact | -3 | Extreme density (rare) |

Each density level reduces vertical padding by 4dp per step. For example, a button
with 36dp default height becomes 32dp at -1, 28dp at -2.

Components that support density: buttons, text fields, tables, toolbars, lists,
form controls, chips, tabs, app bars.

**afterhours relevance:** This is a powerful concept for game UIs that need to adapt
between controller/touch mode (larger targets) and mouse/keyboard mode (denser layouts).
Could be implemented as a Theme-level `density_offset` that automatically adjusts
component heights and padding.

```cpp
// In Theme:
int density_offset = 0;  // 0 = default, -1 = comfortable, -2 = compact

// Usage in component layout:
float effective_height = base_height + (density_offset * 4.0f);
float effective_padding = base_padding + (density_offset * 2.0f);
```

### 2.3 Navigation

Material Design defines a navigation hierarchy:

1. **Lateral navigation** — moving between screens at the same level (tabs, bottom nav, drawer)
2. **Forward navigation** — moving deeper into content (lists → detail, expanding content)
3. **Reverse navigation** — moving back (back button, close, cancel)

Navigation patterns:
| Pattern | Best For | Components |
|---------|----------|------------|
| Bottom navigation | 3–5 top-level destinations | Bottom nav bar |
| Navigation drawer | 5+ destinations, complex hierarchy | Side drawer |
| Tabs | 2–3 categories of equal importance | Tab bar |
| Navigation rail | Compact lateral navigation (tablet) | Rail |

Transition types:
- **Parent-to-child**: Container transform (surface morphs into new view)
- **Peer-to-peer**: Shared axis (slide along direction of navigation)
- **Top-level**: Fade through (cross-fade with scale)

**afterhours relevance:** Navigation patterns map to `docs/21_navigation.md` planned
features. The transition types (container transform, shared axis, fade through) are
specific, actionable animation patterns that `afterhours` should implement.

### 2.4 Color System

Material Design 2 uses a structured color system with **semantic color roles**:

#### 2.4.1 Theme Colors

| Role | Purpose | Surface Usage |
|------|---------|--------------|
| **Primary** | Brand color, key UI elements | App bar, FAB, prominent buttons |
| **Primary Variant** | Darker/lighter primary for contrast | Status bar, links |
| **Secondary** | Accent for less prominent elements | FABs, selection controls, highlights |
| **Secondary Variant** | Accent variant | Pressed state, linked text |
| **Background** | Screen background | Entire screen backdrop |
| **Surface** | Component background | Cards, sheets, menus, dialogs |
| **Error** | Error indication | Error text, error fields |

#### 2.4.2 On-Colors

Every surface color has a corresponding **"on" color** for text/icons that appear on it:

| Surface Color | On-Color | Example |
|---------------|----------|---------|
| Primary | On Primary | White text on blue button |
| Secondary | On Secondary | Black text on yellow accent |
| Background | On Background | Dark text on light background |
| Surface | On Surface | Dark text on white card |
| Error | On Error | White text on red error |

This pairing system guarantees accessible contrast ratios.

#### 2.4.3 Color Palette Generation

Material Design generates palettes from a single hue:
- **10-shade scale**: 50, 100, 200, 300, 400, 500, 600, 700, 800, 900
- **4 accent shades**: A100, A200, A400, A700
- Primary color is typically the 500 shade
- Lighter shades (50–300) for backgrounds
- Darker shades (700–900) for text emphasis

#### 2.4.4 Dark Theme

Material Design 2 defines specific dark theme rules:
- **Surface** color is dark grey (`#121212`), NOT pure black
- **Elevation overlay**: Higher surfaces are lighter (white overlay at increasing opacity)
  - 0dp: 0% overlay
  - 1dp: 5% overlay
  - 2dp: 7% overlay
  - 3dp: 8% overlay
  - 4dp: 9% overlay
  - 6dp: 11% overlay
  - 8dp: 12% overlay
  - 12dp: 14% overlay
  - 16dp: 15% overlay
  - 24dp: 16% overlay
- **Text** uses white at varying opacities:
  - High emphasis: 87% opacity
  - Medium emphasis: 60% opacity
  - Disabled: 38% opacity
- **Primary color** is desaturated (200 shade instead of 500)

**afterhours relevance:** The dark theme elevation overlay system is a critical insight.
Instead of just swapping colors for dark mode, higher surfaces should be progressively
lighter to maintain visual hierarchy. This directly impacts the elevation system
recommendation. The text opacity tiers (87%/60%/38%) should be formalized as
text emphasis levels in Theme.

```cpp
// In Theme:
struct TextEmphasis {
    float high   = 0.87f;  // Primary text, headings
    float medium = 0.60f;  // Secondary text, labels
    float disabled = 0.38f; // Disabled, hint text
};

// Dark theme elevation overlay:
float elevation_overlay_opacity(int elevation_dp) {
    // Approximate the M2 formula
    if (elevation_dp == 0) return 0.0f;
    return 0.0f + (elevation_dp * 0.5f + 4.5f) / 100.0f;
    // 1dp≈5%, 2dp≈7%, 4dp≈9%, 8dp≈12%, 16dp≈15%, 24dp≈16%
}
```

### 2.5 Typography

Material Design 2 defines a **type scale** with 13 named styles:

| Category | Size/Weight | Use |
|----------|------------|-----|
| H1 | 96sp / Light (300) | Hero text |
| H2 | 60sp / Light (300) | Page title |
| H3 | 48sp / Regular (400) | Section header |
| H4 | 34sp / Regular (400) | Sub-section |
| H5 | 24sp / Regular (400) | Card title |
| H6 | 20sp / Medium (500) | App bar title |
| Subtitle 1 | 16sp / Regular (400) | List primary |
| Subtitle 2 | 14sp / Medium (500) | Tab label |
| Body 1 | 16sp / Regular (400) | Paragraph text |
| Body 2 | 14sp / Regular (400) | Secondary text |
| Button | 14sp / Medium (500) | Button labels (ALL CAPS) |
| Caption | 12sp / Regular (400) | Timestamps, annotations |
| Overline | 10sp / Regular (400) | Category labels (ALL CAPS) |

Key typographic principles:
- **Roboto** is the default typeface (or Noto for non-Latin scripts)
- **Line height** is typically 1.2x–1.5x the font size
- **Letter spacing** varies: tight for large text, loose for small caps
- Type should be set on a **4dp baseline grid**
- Maximum line length: 40–60 characters for readability

**afterhours relevance:** `afterhours` currently uses ad-hoc font sizes in ComponentConfig.
A formalized type scale would provide:
- Named font size tokens (`FontSize::H1`, `FontSize::Body1`, `FontSize::Caption`)
- Automatic weight pairing
- Baseline grid alignment
- Consistent letter spacing per style

### 2.6 Shape

Material Design 2 treats **shape** as a first-class design attribute alongside color
and typography. Shapes communicate:
- **Brand identity** (rounded = friendly, angular = sharp/precise)
- **Component hierarchy** (small components get small shape, large get large)
- **State changes** (shape can morph during transitions)

Shape categories:
| Category | Corner Radius | Components |
|----------|--------------|------------|
| Small | 4dp | Buttons, text fields, chips, cards |
| Medium | 8dp–16dp | Dialogs, sheets, menus |
| Large | 24dp+ | Bottom sheets, navigation drawers |

Shape can be:
- **Rounded** (default) — rounded corners
- **Cut** — angled/chamfered corners
- **Mixed** — different corners on the same component

**afterhours relevance:** `afterhours` supports per-corner radius via `ComponentConfig`.
Missing: Named shape categories (small/medium/large) as Theme-level tokens, and the
ability to apply cut (chamfered) corners alongside rounded ones.

```cpp
// In Theme:
struct ShapeScheme {
    float small  = 4.0f;   // Buttons, chips, text fields
    float medium = 12.0f;  // Cards, dialogs
    float large  = 24.0f;  // Bottom sheets, nav drawers
    // Corner type could be extended:
    // enum CornerType { Rounded, Cut };
};
```

### 2.7 Motion System

Material Design 2 defines a complete motion system with four principles:

#### 2.7.1 Informative

Motion tells a story about:
- **Spatial relationships** — where an element came from and where it's going
- **Functionality** — what an element does
- **Feedback** — confirming user actions

#### 2.7.2 Focused

Motion directs attention to what matters:
- **Single focal point** — one primary animation at a time
- **Orchestrated** — secondary animations support the primary
- **Brief** — avoid long, distracting animations

#### 2.7.3 Expressive

Motion reinforces the feel of the brand:
- **Easing curves** define personality (standard, decelerate, accelerate)
- **Character** of motion reflects the app's tone

#### 2.7.4 Speed

Material Design defines specific duration ranges:
| Transition Type | Duration |
|----------------|----------|
| Small (icon, FAB) | 100ms |
| Medium (card expand) | 250ms |
| Large (full-screen) | 300ms |
| Complex/sequential | 375ms+ |

Easing curves:
| Curve | When | CSS Equivalent |
|-------|------|----------------|
| Standard | Moving between visible states | cubic-bezier(0.4, 0.0, 0.2, 1) |
| Decelerate | Entering the screen | cubic-bezier(0.0, 0.0, 0.2, 1) |
| Accelerate | Leaving the screen | cubic-bezier(0.4, 0.0, 1, 1) |
| Sharp | Temporarily leaving | cubic-bezier(0.4, 0.0, 0.6, 1) |

Transition patterns:
| Pattern | Description | Use |
|---------|-------------|-----|
| Container Transform | Source morphs into target | List → Detail |
| Shared Axis | Elements slide along a shared axis | Onboarding steps |
| Fade Through | Outgoing fades, incoming fades | Tab switch |
| Fade | Simple opacity change | Tooltip, snackbar |

**afterhours relevance:** The duration/easing catalog is directly actionable. `afterhours`
should define these as animation presets:

```cpp
namespace AnimPresets {
    // Durations
    constexpr float SMALL  = 100.0f;  // ms
    constexpr float MEDIUM = 250.0f;
    constexpr float LARGE  = 300.0f;
    constexpr float COMPLEX = 375.0f;

    // Easing (as cubic bezier control points)
    constexpr auto STANDARD   = CubicBezier{0.4f, 0.0f, 0.2f, 1.0f};
    constexpr auto DECELERATE = CubicBezier{0.0f, 0.0f, 0.2f, 1.0f};
    constexpr auto ACCELERATE = CubicBezier{0.4f, 0.0f, 1.0f, 1.0f};
    constexpr auto SHARP      = CubicBezier{0.4f, 0.0f, 0.6f, 1.0f};
}
```

### 2.8 Interaction

#### 2.8.1 States

Material Design defines **7 interactive states**, each with specific visual feedback:

| State | Overlay Opacity | Description |
|-------|----------------|-------------|
| Enabled | 0% | Default resting state |
| Disabled | 38% content, 12% container | Cannot be interacted with |
| Hovered | 4% overlay of on-color | Mouse is over the element |
| Focused | 12% overlay of on-color | Keyboard/screen-reader focus |
| Selected | 8% overlay of primary | Item is selected (checkbox, chip) |
| Activated | 12% overlay of primary | Currently active (nav item, tab) |
| Pressed | 12% overlay + ripple | Being pressed/clicked |
| Dragged | 8% overlay + elevation +8dp | Being dragged |

State overlay system:
- States are represented as **semi-transparent overlays** on top of the component surface
- The overlay color is typically the "on" color for that surface (e.g., white overlay on primary-colored button)
- States can **combine**: hover + selected, focused + activated, etc.
- **Ripple** provides immediate tactile feedback on press

**afterhours relevance:** This is one of the most actionable insights from M2. `afterhours`
currently has basic hover/active states. The overlay opacity system provides a principled
approach:

```cpp
// In Theme:
struct StateOverlay {
    float hover     = 0.04f;  // 4% overlay
    float focus     = 0.12f;  // 12% overlay
    float selected  = 0.08f;  // 8% overlay
    float activated = 0.12f;  // 12% overlay
    float pressed   = 0.12f;  // 12% overlay + ripple
    float dragged   = 0.08f;  // 8% overlay + elevation boost

    Color overlay_color_for(Color surface_color, Color on_color) {
        // Overlay is always the on-color at the specified opacity
        return Color::with_alpha(on_color, hover);
    }
};
```

#### 2.8.2 Gestures

Material Design categorizes gestures:

| Gesture | Action | Components |
|---------|--------|------------|
| Tap | Select, activate | Buttons, list items, chips |
| Long press | Select, context menu | List items, text |
| Swipe | Dismiss, navigate | Cards, lists, drawers |
| Scroll | Browse content | Lists, pages |
| Pinch | Zoom | Images, maps |
| Drag | Move, reorder | Lists, sliders, sheets |

**afterhours relevance:** Gestures map to input handling. `afterhours` supports tap, drag,
and scroll. Missing: long press (hold-to-activate), swipe-to-dismiss, and pinch-to-zoom.
Long press is particularly important for game UIs (hold-to-inspect, hold-to-charge).

#### 2.8.3 Selection

Selection states follow a hierarchy:
1. **None** — no selection
2. **Single** — one item selected (radio behavior)
3. **Multiple** — many items selected (checkbox behavior)
4. **Range** — contiguous selection (shift+click)

Selection feedback: checkmark icon + 8% primary overlay + optional toolbar change.

### 2.9 Iconography

#### 2.9.1 System Icons

- Size: **24dp** (default), with 20dp and 18dp variants
- Grid: designed on a **24x24dp** grid with **2dp padding** (20dp live area)
- Style: 2dp stroke weight, rounded caps/joins
- States: active (87% opacity), inactive (54% opacity), disabled (38% opacity)

#### 2.9.2 Product Icons

- Size: **48dp** (with keyline shapes: circle=40dp, square=36dp)
- Layering: tinted background + foreground icon + finish layer
- Consistent keylines across all icons

**afterhours relevance:** Icon sizing/opacity conventions should be formalized. The
standard 24dp grid with 2dp padding is a good default for `afterhours` icon rendering.

### 2.10 Sound

Material Design treats sound as a design element:
- Sound reinforces UI actions (haptic + visual + audio = multisensory feedback)
- Hierarchy: primary sounds (notifications) > secondary (button press) > ambient
- Sound attributes: frequency, volume, duration, timbre
- Sound choreography: sync with visual transitions

**afterhours relevance:** `afterhours` is a game UI framework, so sound integration is
highly relevant. M2's approach of pairing sound with UI state changes (button press,
modal open, error) could be formalized in the animation/event system.

### 2.11 Communication

Material Design defines patterns for user communication:

| Pattern | Component | Behavior |
|---------|-----------|----------|
| **Confirmation** | Dialog, snackbar | Explicit before destructive actions |
| **Acknowledgement** | Snackbar, toast | Passive confirmation of completed action |
| **Empty States** | Illustration + text + CTA | Content area has no data |
| **Errors** | Inline text, snackbar, dialog | Graded by severity |
| **Onboarding** | Feature highlight, coach mark | First-time user guidance |
| **Loading** | Linear/circular progress, skeleton | Content is loading |
| **Offline** | Banner, snackbar | No network connection |

Empty state best practices:
- Include a **descriptive illustration** or icon
- Provide a **short headline** explaining the state
- Include a **call-to-action** to resolve the state
- Vary messaging between first-empty and subsequent-empty

**afterhours relevance:** Empty states and onboarding patterns are directly relevant to
game UIs (empty inventory, empty leaderboard, first-time tooltip). These should be
supported as first-class component patterns.

---

## 3. Material Theming

Material Design 2 introduced **Material Theming** — the ability to systematically
customize Material Design to match a brand. A theme consists of:

### 3.1 Theme Attributes

| Attribute | Controls | Customizable? |
|-----------|----------|--------------|
| **Color** | Primary, secondary, surface, background, error + on-colors | Full palette |
| **Typography** | Font family, weight, size for all 13 type scale entries | Full scale |
| **Shape** | Corner radius/style for small, medium, large categories | Per category |
| **Iconography** | Icon style (filled, outlined, rounded, two-tone, sharp) | Style set |

### 3.2 Theme Implementation

Material Theming works through a **token system**:
- Design tokens define every visual property
- Tokens cascade: global → category → component → state
- Components read tokens from the current theme
- Themes can be swapped at runtime

**afterhours relevance:** `afterhours` already has a Theme struct with color, sizing,
and padding fields. The M2 token cascade (global → category → component → state) is
a more sophisticated system that would allow:
- Component categories to inherit from global tokens
- Individual components to override category defaults
- States to further modify component tokens

This aligns with the `ComponentConfig` system but is more structured.

---

## 4. Component Inventory

Material Design 2 specifies **30+ components** across these categories:

### 4.1 Full Component List

| M2 Component | afterhours Status | Priority |
|-------------|-------------------|----------|
| **App bars: top** | Exists (header bar) | -- |
| **App bars: bottom** | Not planned | Low |
| **Backdrop** | Not planned | Medium |
| **Banners** | Not planned | Medium |
| **Bottom navigation** | Planned (docs/21) | High |
| **Buttons** | Exists | -- |
| **Buttons: FAB** | Planned (docs/03) | High |
| **Cards** | Exists (basic) | -- |
| **Checkboxes** | Exists | -- |
| **Chips** | Planned (Materialize analysis) | High |
| **Data tables** | Planned (docs/25) | High |
| **Date pickers** | Planned (docs/34) | Medium |
| **Dialogs** | Exists (modal) | -- |
| **Dividers** | Exists (horizontal rule) | -- |
| **Image lists** | Not planned | Low |
| **Lists** | Exists (basic) | -- |
| **Menus** | Exists (dropdown) | -- |
| **Navigation drawer** | Planned (docs/21) | High |
| **Navigation rail** | Not planned | Medium |
| **Progress indicators** | Planned (docs/32) | High |
| **Radio buttons** | Exists | -- |
| **Selection controls** | Exists (partial) | -- |
| **Bottom sheets** | Not planned | Medium |
| **Side sheets** | Not planned | Low |
| **Sliders** | Exists | -- |
| **Snackbars** | Planned (docs/14) | High |
| **Switches** | Exists (toggle) | -- |
| **Tabs** | Exists | -- |
| **Text fields** | Exists | -- |
| **Time pickers** | Planned (docs/34) | Low |
| **Tooltips** | Planned (docs/10) | High |

### 4.2 Component-Specific Insights

#### Buttons (4 types in M2)

| Type | Elevation | Fill | Use |
|------|-----------|------|-----|
| Contained | 2dp (rest), 8dp (pressed) | Filled with primary | High emphasis |
| Outlined | 0dp | Stroke only | Medium emphasis |
| Text | 0dp | No fill or stroke | Low emphasis |
| Toggle | 0dp | Fill when active | Selection |

M2 also specifies **icon buttons** and **FABs** (3 sizes: mini=40dp, default=56dp,
extended=48dp height).

#### Cards (3 types)

| Type | Elevation | Border | Use |
|------|-----------|--------|-----|
| Elevated | 1dp (rest) | None | Default card |
| Outlined | 0dp | 1dp stroke | Grouped content |
| Filled | 0dp | None, filled surface | Container variant |

Card anatomy: media → header → supporting text → actions → overflow menu.

#### Text Fields (2 types)

| Type | Visual | Use |
|------|--------|-----|
| Filled | Grey background, bottom border | Default (higher affordance) |
| Outlined | Full border, floating label | Alternative (cleaner) |

Text field states: empty → focused (label floats up) → filled → error → disabled.

#### Chips (4 types)

| Type | Behavior | Use |
|------|----------|-----|
| Input | Represents user input, removable | Tags, recipients |
| Choice | Single-select from group | Filters |
| Filter | Multi-select from group | Search refinement |
| Action | Triggers an action | Quick actions |

#### Snackbars

- Position: bottom center (mobile), bottom left (desktop)
- Duration: 4–10 seconds, auto-dismiss
- Max 1 snackbar visible at a time (queue subsequent)
- Optional action button (single, text-style)
- Optional dismiss icon (X)
- Max 2 lines of text

**afterhours relevance:** Snackbar queue management (only 1 visible, queue rest) is a
specific behavior that should be built into the `afterhours` notification/toast system.

---

## 5. Material Studies

Material Design 2 includes **8 case studies** showing how different apps customize the
same Material components:

| Study | Brand Character | Key Customization |
|-------|----------------|-------------------|
| **Basil** | Organic, culinary | Green palette, serif type |
| **Crane** | Travel, luxury | Purple/crimson, geometric shapes |
| **Fortnightly** | News, editorial | High contrast, strong typography |
| **Owl** | Education, playful | Yellow/blue, rounded shapes |
| **Rally** | Finance, data-heavy | Dark theme, monospace numbers |
| **Reply** | Email, productive | Neutral palette, dense layout |
| **Shrine** | Retail, boutique | Pink/brown, cut corners |

**afterhours relevance:** These studies demonstrate that the same component system can
produce dramatically different visual identities through theme customization alone.
This validates `afterhours`' approach of separating structure (components) from
presentation (Theme). The most relevant study for game UIs is **Rally** (data-dense,
dark theme) and **Owl** (playful, rounded).

---

## 6. Usability

### 6.1 Accessibility

Material Design mandates:
- **Touch targets**: Minimum 48dp x 48dp (components can be smaller if touch area is padded)
- **Color contrast**: WCAG AA minimum (4.5:1 for normal text, 3:1 for large text)
- **Focus indicators**: Visible focus ring for keyboard navigation
- **Screen reader support**: Semantic labels for all interactive elements
- **Motion sensitivity**: Provide reduced-motion option

### 6.2 Bidirectionality

Material Design supports RTL (right-to-left) layouts:
- **Mirrored layout**: Columns, navigation, icons flip
- **Non-mirrored content**: Numbers, media playback controls, clocks don't flip
- **Contextual icons**: Back arrow flips, but "undo" arrow doesn't

**afterhours relevance:** RTL support is a consideration for internationalization.
Currently not supported in `afterhours` but worth noting as a long-term goal.

---

## 7. Comparison with Existing Analyses

### 7.1 M2 vs. Materialize (docs/materialize_analysis.md)

Materialize is a CSS implementation of M2's design principles. M2 is the source spec.
The Materialize analysis correctly captured elevation, color, and typography. M2 adds:
- Formal density system (not in Materialize)
- Shape as a first-class theming attribute
- State overlay opacity system
- Navigation transition patterns
- Comprehensive motion system with named easing curves

### 7.2 M2 vs. Ant Design (docs/antd_analysis.md)

Both are comprehensive design systems. Differences:
- **M2**: Material metaphor (paper/ink), elevation-first, ripple feedback
- **Ant Design**: Enterprise-focused, 10 interaction principles, richer feedback taxonomy
- **Overlap**: Both have semantic color systems, type scales, motion catalogs, density concepts

### 7.3 M2 vs. Bootstrap (docs/bootstrap_analysis.md)

- **M2**: Principled design language with formal philosophy
- **Bootstrap**: Pragmatic CSS framework with less design theory
- **M2 adds**: Elevation, shape, density, material metaphor, formal state system

### 7.4 M2 vs. DaisyUI (docs/daisyui_analysis.md)

- **DaisyUI**: Minimal tokens, CSS-only, OKLCH
- **M2**: Comprehensive tokens, behavior-inclusive, sRGB-based
- **Shared insight**: Both use "depth" as a design variable (M2's elevation ≈ DaisyUI's depth)

---

## 8. Prioritized Recommendations for afterhours

### 8.1 High Priority (adopt from M2)

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Elevation system** | Low | High | 6-level shadow + dark overlay |
| **State overlay system** | Low | High | 4%/8%/12% opacity overlays for hover/focus/press |
| **Density system** | Medium | High | Theme-level density offset for all components |
| **Named easing curves** | Low | Medium | 4 standard cubic-bezier presets |
| **Duration catalog** | Low | Medium | Small/medium/large/complex presets |
| **Dark theme elevation overlay** | Low | High | White overlay at increasing opacity per elevation |
| **Text emphasis levels** | Low | Medium | 87%/60%/38% opacity tiers |

### 8.2 Medium Priority

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Shape scheme** | Low | Medium | Small/medium/large corner radius tokens |
| **Type scale** | Medium | Medium | 13 named styles with size/weight/spacing |
| **Navigation transitions** | Medium | High | Container transform, shared axis, fade through |
| **On-color pairing** | Low | Medium | Every surface color has a paired text color |
| **Snackbar queue** | Medium | Medium | Single-visible, auto-dismiss, action button |

### 8.3 Lower Priority

| Feature | Effort | Impact | Notes |
|---------|--------|--------|-------|
| **Backdrop component** | Medium | Low | Persistent back layer + front content |
| **Chips (4 types)** | Medium | Medium | Input, choice, filter, action |
| **Navigation rail** | Low | Low | Compact side navigation |
| **Sound choreography** | Medium | Medium | Sound synced to UI transitions |
| **RTL/bidirectionality** | High | Low | Mirror layout for RTL languages |

---

## 9. Key Takeaways

### 9.1 Material as Surface

The single most fundamental idea in M2 is that **every UI element is a surface** with
position, depth, and physical properties. This maps perfectly to `afterhours`' ECS
where each component is an entity with transform (position), rendering (surface), and
interaction (states) components.

### 9.2 The State Overlay System

M2's approach of representing states as **semi-transparent overlays** is elegant and
universal. Instead of defining unique hover/focus/press colors per component,
`afterhours` can apply a standard overlay opacity to any surface. This dramatically
reduces the number of color tokens needed while ensuring consistent feedback.

### 9.3 Density Is a First-Class Concern

M2 is the only design system analyzed so far that formalizes **density as a theme attribute**.
For game UIs that must work with both controllers (large targets) and mouse (dense
layouts), this is a critical insight. A single `density_offset` in Theme could
automatically adjust all component heights and spacing.

### 9.4 Dark Theme ≠ Color Swap

M2's dark theme is not simply "swap light for dark colors." It introduces **elevation
overlays** where higher surfaces are lighter, maintaining visual hierarchy. This is
more sophisticated than a simple color palette inversion and should be adopted.

### 9.5 Motion Is a System, Not an Afterthought

M2 defines specific duration ranges, easing curves, and transition patterns with
clear names and use cases. `afterhours` should adopt this catalog rather than using
ad-hoc animation parameters.
