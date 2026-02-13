# DaisyUI Analysis

What DaisyUI provides, how it approaches theming and component design, and
what afterhours can learn from the most popular Tailwind CSS component library.

Repository: https://github.com/saadeghi/daisyui
Docs: https://daisyui.com/components/

---

## 1. What DaisyUI Is

DaisyUI is a **CSS-only component library** built on Tailwind CSS.  It provides
58 components and 35 pre-built themes — all implemented purely in CSS with
zero JavaScript.  Interactive behaviors (modal open/close, dropdown toggle,
drawer slide) are achieved via CSS `:focus-within`, `:checked`, `:target`,
`<details>`, and the new `[popover]` API.

This makes DaisyUI fundamentally different from every other library analyzed:
- **Base UI / Radix:** Behavior-first, unstyled
- **Bootstrap:** CSS + thin JS layer
- **DaisyUI:** CSS-only, maximally themed, zero JS

Why study it:
- 35 themes demonstrate how a **single token system can express wildly
  different visual identities** — exactly what afterhours needs for plugin themes
- Several unique components not found in any other analyzed library
- The token system is small and well-designed (only ~10 variables per theme)
- Shows what's possible with CSS-only interactivity (relevant to immediate-mode
  UI where state management is the framework's job)

---

## 2. Theme System

### 2.1 Theme Token Set

Every DaisyUI theme defines exactly the same variables.  The entire visual
identity is controlled by just **10 color tokens + 6 design tokens**:

**Color Tokens (10 pairs = 20 values):**

| Token | Role | `*-content` variant |
|---|---|---|
| `--color-base-100` | Page background | `--color-base-content` |
| `--color-base-200` | Slightly darker surface | (inherits base-content) |
| `--color-base-300` | Card/section borders | (inherits base-content) |
| `--color-primary` | Main action color | `--color-primary-content` |
| `--color-secondary` | Secondary action | `--color-secondary-content` |
| `--color-accent` | Highlight / emphasis | `--color-accent-content` |
| `--color-neutral` | Dark neutral surfaces | `--color-neutral-content` |
| `--color-info` | Informational | `--color-info-content` |
| `--color-success` | Positive / confirm | `--color-success-content` |
| `--color-warning` | Caution / attention | `--color-warning-content` |
| `--color-error` | Error / destructive | `--color-error-content` |

**Design Tokens (6):**

| Token | Purpose | Example Values |
|---|---|---|
| `--radius-selector` | Radius for toggles/radios | 0–0.5rem |
| `--radius-field` | Radius for inputs/buttons | 0–0.5rem |
| `--radius-box` | Radius for cards/modals | 0–1rem |
| `--size-selector` | Size unit for toggles | 0.25rem |
| `--size-field` | Size unit for inputs/buttons | 0.25rem |
| `--border` | Border width | 0–2px |
| `--depth` | 3D depth (shadows, bevels) | 0–1 |
| `--noise` | Noise texture overlay | 0–1 |

**Key insight:** With only ~26 CSS variables, DaisyUI creates 35 themes that
look completely different — from cyberpunk neon to retro paper to wireframe
sketches.  This proves that a small, well-chosen token set is sufficient
for extreme visual variety.

### 2.2 OKLCH Color Space

DaisyUI v5 uses **OKLCH** for all color values:

```css
--color-primary: oklch(45% 0.24 277.023);
--color-primary-content: oklch(93% 0.034 272.788);
```

OKLCH advantages:
- Perceptually uniform (unlike HSL)
- Predictable lightness manipulation
- Hue interpolation that doesn't pass through gray
- `color-mix(in oklab, ...)` produces visually correct blends

DaisyUI derives hover/active/border states using `color-mix()`:

```css
--btn-bg: color-mix(in oklab, var(--btn-color), #000 7%);      /* hover */
--btn-border: color-mix(in oklab, var(--btn-bg), #000 calc(var(--depth) * 5%));
```

**afterhours adoption:** Consider using OKLCH internally for color
manipulation.  The `color-mix()` pattern with a `--depth` multiplier
elegantly scales from flat (depth=0) to skeuomorphic (depth=1).

### 2.3 Content Color Pairing

Every color has a paired `*-content` color — the text/icon color guaranteed
to be readable on that background:

```css
--color-primary: oklch(45% 0.24 277);        /* blue background */
--color-primary-content: oklch(93% 0.034 272); /* light text on blue */
```

Components reference both:

```css
.btn-primary {
    --btn-color: var(--color-primary);
    --btn-fg: var(--color-primary-content);
}
```

**afterhours equivalent:** This is exactly the `auto_text_color()` concept,
but pre-computed per theme.  Pre-pairing is faster at runtime since no
contrast calculation is needed.

### 2.4 Depth and Noise Variables

Unique to DaisyUI: `--depth` and `--noise` control the visual "dimensionality":

- `--depth: 0` → flat design (no shadows, no borders, no bevels)
- `--depth: 1` → full depth (shadows, text-shadow, border darkening, inset highlights)
- `--noise: 0` → clean surfaces
- `--noise: 1` → grain/texture overlay on buttons and surfaces

These are **multiplied** into every shadow/border calculation:

```css
box-shadow: 0 0.5px 0 0.5px oklch(100% 0 0 / calc(var(--depth) * 6%)) inset;
text-shadow: 0 0.5px oklch(100% 0 0 / calc(var(--depth) * 0.15));
border-color: color-mix(in oklab, var(--btn-bg), #000 calc(var(--depth) * 5%));
```

**afterhours adoption:** A single `depth` float (0 to 1) that scales all
shadow, border, and bevel effects simultaneously.  This lets themes range
from flat to skeuomorphic with one variable:

```cpp
struct ThemeTokens {
    float depth = 0.5f;    // 0 = flat, 1 = full 3D
    float noise = 0.0f;    // 0 = clean, 1 = textured
};
```

### 2.5 Theme Examples

| Theme | Radius | Border | Depth | Noise | Visual Style |
|---|---|---|---|---|---|
| light | 0.25–0.5rem | 1px | 1 | 0 | Clean modern |
| dark | 0.25–0.5rem | 1px | 1 | 0 | Dark modern |
| cyberpunk | 0 | 1px | 0 | 0 | Sharp edges, neon, flat |
| retro | 0.25–0.5rem | 1px | 0 | 0 | Warm paper, muted |
| wireframe | 0 | 2px | 0 | 0 | Black & white sketch |
| luxury | 0 | 1px | 1 | 0 | Dark gold, elegant |
| valentine | 0.5–1rem | 1px | 1 | 0 | Pink, rounded, soft |

This demonstrates that just 6 design tokens create vastly different aesthetics.

---

## 3. Full Component Inventory (58 components)

### Actions (6)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Button** | 10 color variants, 8 style variants (ghost, link, outline, dash, soft), 5 sizes | Has: `button()` |
| **Dropdown** | CSS-only toggle via focus-within, popover API, 8 directions | Has: `dropdown()` |
| **Modal** | CSS-only dialog (checkbox/target/open attr), 5 positions (top/middle/bottom/start/end) | Has: `modal.h` |
| **Swap** | Toggle between two states with rotate/flip animation | Gap |
| **Filter** | Radio button group that collapses unselected options | Gap |
| **FAB** | Floating action button with expandable sub-actions (fan/flower layout) | Gap |

### Data Display (14)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Avatar** | User image with status indicator, group stacking | Doc |
| **Badge** | Small status/count label | Gap |
| **Calendar** | Month grid calendar display | Gap |
| **Card** | Container with header/body/actions, 5 sizes, side layout | Has (div) |
| **Carousel** | CSS-only snap scroll (horizontal/vertical) | Gap |
| **Chat** | Chat bubble layout (start/end) with avatar, header, footer | Gap |
| **Collapse** | Expandable content section | Doc |
| **Countdown** | Animated number ticker (CSS-only, no JS) | Gap |
| **Diff** | Before/after image comparison slider (CSS-only) | Gap |
| **Kbd** | Keyboard shortcut display (like `Ctrl+C`) | Gap |
| **Stat** | Statistics display (title, value, description, figure) | Gap |
| **Table** | Styled data table | Doc |
| **Timeline** | Vertical/horizontal event timeline | Gap |
| **Hover 3D** | Perspective tilt card with shine effect on hover | Gap |

### Navigation (7)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Breadcrumbs** | Navigation trail | Gap |
| **Dock** | Fixed bottom bar with active indicator (mobile tab bar) | Gap |
| **Link** | Styled anchor with color variants | Has (inline) |
| **Menu** | Vertical/horizontal menu list | Has: `navigation_bar()` |
| **Navbar** | Top navigation bar | Has: `navigation_bar()` |
| **Steps** | Multi-step progress indicator (horizontal/vertical) | Gap |
| **Tab** | Tab navigation with content switching | Has: `tab_container()` |

### Feedback (5)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Alert** | Feedback banner with icon and color | Gap |
| **Loading** | Spinner/dots/ring/ball/bars/infinity animations | Gap |
| **Progress** | Linear progress bar | Has: `progress_bar()` |
| **Radial Progress** | Circular progress (CSS-only conic gradient) | Gap |
| **Skeleton** | Loading placeholder | Doc |
| **Status** | Small status dot indicator | Gap |
| **Toast** | Positioned notification container | Has: `toast.h` |
| **Tooltip** | Positioned hover hint (top/bottom/left/right) | Doc |
| **Validator** | Form field validation feedback | Has: `validation_config.h` |

### Data Input (9)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Checkbox** | Checkbox with color/size variants | Has: `checkbox()` |
| **File Input** | File upload field | N/A |
| **Fieldset** | Form field group with legend | Gap |
| **Input** | Text input with sizes/colors | Has: `text_input()` |
| **Label** | Form label with optional description | Has (inline) |
| **Radio** | Radio button with colors | Has: `radio_group()` |
| **Range** | Slider with step marks | Has: `slider()` |
| **Rating** | Star rating (half-star support) | Gap |
| **Select** | Dropdown select | Has: `dropdown()` |
| **Textarea** | Multi-line text input | Has: `text_input()` |
| **Toggle** | Switch/toggle | Has: `toggle()` |
| **Text Rotate** | Auto-rotating text (CSS-only animation) | Gap |

### Layout (8)

| Component | What It Does | afterhours Status |
|---|---|---|
| **Divider** | Horizontal/vertical separator with optional text | Has: `separator()` |
| **Drawer** | Slide-in side panel (CSS-only via checkbox) | Gap |
| **Footer** | Page footer layout | Has (div) |
| **Hero** | Full-width hero section with overlay | Gap |
| **Indicator** | Positioned badge overlay on any element (notifications, status) | Gap |
| **Join** | Merge adjacent items (shared borders/radius) | Gap |
| **Mask** | SVG mask shapes (squircle, diamond, hexagon, star, heart, etc.) | Gap |
| **Stack** | Stacked elements with depth offset | Gap |

### Utility (4)

| Component | Purpose |
|---|---|
| **Glass** | Frosted glass background effect |
| **Join** | Border-radius merging for adjacent items |
| **Radius** | Shared radius tokens |
| **Typography** | Prose/content styling |

---

## 4. Unique Components (Not in Bootstrap, Base UI, or Radix)

These are components that DaisyUI provides that no other analyzed library has:

| Component | What It Does | Game UI Relevance |
|---|---|---|
| **Chat** | Message bubble layout with avatar | **High** — in-game chat, NPC dialogue, quest logs |
| **Countdown** | Animated number rolling | **High** — timers, score displays, cooldowns |
| **Diff** | Before/after comparison slider | Low — debug/editor tool |
| **Dock** | Fixed bottom tab bar | **High** — mobile-style bottom navigation |
| **FAB** | Floating action button with fan-out | **Medium** — quick action radial menu |
| **Filter** | Collapsing radio filter | Medium — inventory/shop filtering |
| **Hover 3D** | Perspective tilt with shine | **High** — card/item showcase, loot reveals |
| **Indicator** | Notification overlay on any element | **High** — unread counts, new item badges |
| **Kbd** | Keyboard shortcut display | **High** — keybinding hints, tutorial UI |
| **Mask** | Shape clipping (squircle, hex, star) | **High** — avatar frames, icon shapes, badges |
| **Rating** | Star rating input | **Medium** — reviews, difficulty display |
| **Stack** | Stacked cards with depth | **Medium** — card piles, deck display |
| **Stat** | Stats display (value + trend) | **High** — player stats, dashboard, HUD |
| **Steps** | Multi-step progress | **High** — quest progress, tutorials, onboarding |
| **Swap** | Toggle between two visual states | **High** — play/pause, menu/close, day/night |
| **Text Rotate** | Auto-cycling text | **Medium** — title screens, rotating tips |
| **Timeline** | Event timeline | **Medium** — quest log, history, changelog |

---

## 5. Component Styling Patterns

### 5.1 CSS Variable Indirection

Every component uses CSS custom properties for internal state, not hard-coded
colors.  For example, `.btn` defines:

```css
--btn-bg: var(--btn-color, var(--color-base-200));
--btn-fg: var(--color-base-content);
--btn-border: color-mix(in oklab, var(--btn-bg), #000 calc(var(--depth) * 5%));
--btn-shadow: 0 3px 2px -2px color-mix(in oklab, var(--btn-bg) calc(var(--depth) * 30%), #0000);
```

Then color variants simply override the seed:

```css
.btn-primary { --btn-color: var(--color-primary); --btn-fg: var(--color-primary-content); }
.btn-success { --btn-color: var(--color-success); --btn-fg: var(--color-success-content); }
```

All derived values (hover, active, border, shadow) are computed from
`--btn-color` automatically.

**afterhours adoption:** This pattern solves a common problem: when adding a
new color variant, you only set one variable and all states derive correctly.
The equivalent for afterhours:

```cpp
struct ButtonStyle {
    Color seed_color;       // Set this...
    Color text_color;       // ...and this
    // Everything else derives automatically:
    Color hover_bg()    { return colors::darken(seed_color, 0.07f); }
    Color active_bg()   { return colors::darken(seed_color, 0.05f); }
    Color border()      { return colors::darken(seed_color, 0.05f * theme.depth); }
    Shadow shadow()     { return Shadow::from(seed_color, 0.30f * theme.depth); }
};
```

### 5.2 Size Scale via Multiplied Base Unit

All sizing derives from a base unit:

```css
--size: calc(var(--size-field, 0.25rem) * 10);  /* md button */
--size: calc(var(--size-field, 0.25rem) * 6);   /* xs button */
--size: calc(var(--size-field, 0.25rem) * 14);  /* xl button */
```

The multipliers: xs=6, sm=8, md=10, lg=12, xl=14.

**afterhours adoption:** A consistent size scale where all component sizes
are multiples of a single base unit:

```cpp
constexpr float size_unit = 4.0f;  // Base unit (pixels at 1x)
constexpr float size_xs = size_unit * 6;   // 24px
constexpr float size_sm = size_unit * 8;   // 32px
constexpr float size_md = size_unit * 10;  // 40px
constexpr float size_lg = size_unit * 12;  // 48px
constexpr float size_xl = size_unit * 14;  // 56px
```

### 5.3 Three Radius Tiers

Instead of one radius value, DaisyUI uses three:

| Token | Used By | Range |
|---|---|---|
| `--radius-selector` | Toggles, checkboxes, radios, chips | 0–0.5rem |
| `--radius-field` | Buttons, inputs, badges | 0–0.5rem |
| `--radius-box` | Cards, modals, dropdowns, drawers | 0–1rem |

**afterhours adoption:** Replace single `Theme::roundness` with 3 tiers:

```cpp
struct RadiusTiers {
    float selector;  // Small interactive controls
    float field;     // Input fields, buttons
    float container; // Cards, modals, panels
};
```

### 5.4 CSS Layer Ordering

DaisyUI uses `@layer daisyui.l1.l2.l3` for CSS specificity control:

- `l3` = Base component styles (lowest specificity)
- `l2` = Variant styles (color, size)
- `l1` = Override styles (active, disabled)

This means variants always override base, and state overrides always win —
without specificity wars.  The equivalent for afterhours is ensuring that
style resolution order is: base → variant → state → user override.

### 5.5 Button Variant Matrix

DaisyUI's button has the richest variant system of any library analyzed:

**10 color variants:** primary, secondary, accent, neutral, info, success,
warning, error, ghost, link

**8 style variants:**
- Default (solid fill)
- `ghost` — transparent until hover
- `link` — underlined text, no background
- `outline` — border only, fills on hover
- `dash` — dashed border, fills on hover
- `soft` — 8% tinted background (subtle fill)
- `active` — pressed appearance
- `disabled` — grayed out

**5 sizes:** xs, sm, md, lg, xl

**4 shapes:** default, square, circle, wide, block

Total combinations: 10 × 8 × 5 × 4 = **1,600 button variants** from
a single component definition.

**afterhours adoption:** The current button has far fewer variants.  The
`ghost`, `soft`, and `dash` styles are particularly useful for game UIs:
- `ghost` — toolbar/menu buttons that fade in on hover
- `soft` — subtle category tabs, secondary actions
- `dash` — "add new item" placeholder buttons

---

## 6. CSS-Only Interactivity Patterns

DaisyUI achieves modal, dropdown, drawer, and collapse without JavaScript
using several CSS-only patterns:

### 6.1 Checkbox Toggle

```html
<input type="checkbox" class="drawer-toggle" />
<div class="drawer-side">...</div>
```
```css
.drawer-toggle:checked ~ .drawer-side {
    visibility: visible;
    opacity: 1;
}
```

### 6.2 Details/Summary

```html
<details class="dropdown">
    <summary>Click me</summary>
    <div class="dropdown-content">...</div>
</details>
```

### 6.3 Focus-Within

```css
.dropdown:focus-within .dropdown-content {
    opacity: 1;
    scale: 100%;
}
```

### 6.4 Dialog / Popover API

```css
.modal[open] { visibility: visible; }
.dropdown[popover]:popover-open { display: block; }
```

**afterhours relevance:** In immediate-mode UI, the framework already
manages state — so these CSS-only patterns aren't needed.  But the
**transition animations** DaisyUI applies (scale 95%→100%, opacity
0→1, translate slide-in) are directly applicable.

---

## 7. What afterhours Should Adopt

### 7.1 Minimal Theme Token Set (High Priority)

DaisyUI proves that **~26 variables** are sufficient for a complete
theme system.  afterhours should aim for a similar-size token set:

```cpp
struct ThemeTokens {
    // Surfaces (3)
    Color base_100;  // Page/window background
    Color base_200;  // Elevated surface
    Color base_300;  // Borders, dividers

    // Semantic colors (8 pairs = 16)
    ColorPair primary;    // Main actions
    ColorPair secondary;  // Secondary actions
    ColorPair accent;     // Highlights
    ColorPair neutral;    // Dark surfaces
    ColorPair info;       // Informational
    ColorPair success;    // Positive
    ColorPair warning;    // Caution
    ColorPair error;      // Error

    // Design tokens (6)
    float radius_selector;  // Toggle/radio corner radius
    float radius_field;     // Button/input corner radius
    float radius_box;       // Card/modal corner radius
    float border_width;     // Border thickness
    float depth;            // 0=flat, 1=3D (shadows, bevels)
    float noise;            // 0=clean, 1=textured
};
```

This is the **convergence point** across all analyzed libraries:
- Bootstrap: 8 theme colors
- Radix: accent + gray + 12-step scales
- DaisyUI: 10 color + 6 design tokens
- All agree on: primary, secondary, success, info, warning, error

### 7.2 Depth Variable (High Priority)

The `--depth` multiplier that scales all shadow/border effects from 0 to 1
is DaisyUI's most original contribution.  One float controls the entire
visual "flatness to skeuomorphism" spectrum:

```cpp
// In shadow/border calculations:
float border_darken = 0.05f * theme.depth;
float shadow_opacity = 0.30f * theme.depth;
float inset_highlight = 0.06f * theme.depth;
float text_shadow_alpha = 0.15f * theme.depth;
```

### 7.3 Swap Component (High Priority)

Toggle between two visual states with animated transition.  Essential for
game UIs:

```cpp
swap({
    .animation = SwapAnimation::Rotate,  // or Flip, Fade
    .state = is_playing,
}, [&]() {
    // on state
    icon(Icon::Pause);
}, [&]() {
    // off state
    icon(Icon::Play);
});
```

Use cases: play/pause, menu/close, mute/unmute, day/night toggle,
expand/collapse.

### 7.4 Chat Bubble (High Priority)

Game-essential chat/dialogue layout:

```cpp
chat_bubble({
    .side = ChatSide::Start,  // or End
    .color = ChatColor::Primary,
    .avatar = player_avatar,
}, [&]() {
    chat_header("Player Name", "2 min ago");
    chat_message("Hello world!");
    chat_footer("Delivered");
});
```

Use cases: in-game chat, NPC dialogue, quest descriptions, tutorial popups.

### 7.5 Stat Display (High Priority)

Statistics/dashboard readout:

```cpp
stat({
    .title = "Score",
    .value = "31,459",
    .description = "+21% from last game",
    .figure = icon(Icon::Trophy),
});
```

Use cases: HUD stats, player profiles, end-of-match screens, shop prices.

### 7.6 Steps / Progress Steps (High Priority)

Multi-step progress indicator:

```cpp
steps({
    .direction = Direction::Horizontal,
    .items = {
        { "Create Character", StepState::Complete },
        { "Choose Class", StepState::Complete },
        { "Select Skills", StepState::Active },
        { "Confirm", StepState::Pending },
    },
});
```

Use cases: tutorials, quest progress, crafting steps, setup wizards.

### 7.7 Indicator / Badge Overlay (High Priority)

Notification badge positioned on any element:

```cpp
indicator([&]() {
    indicator_item({ .position = {Top, End} }, [&]() {
        badge({ .color = BadgeColor::Error }, "3");
    });
    icon_button(Icon::Inbox);
});
```

Use cases: unread message count, new item notification, shop sale badge,
quest available marker.

### 7.8 Keyboard Shortcut Display (Medium Priority)

```cpp
kbd("Ctrl"); text("+"); kbd("S");
```

Use cases: keybinding settings, control overlay, tutorial hints.

### 7.9 Mask / Shape Clipping (Medium Priority)

SVG mask shapes for avatars and icons:

```cpp
mask(MaskShape::Squircle, [&]() { image(avatar_texture); });
mask(MaskShape::Hexagon, [&]() { image(badge_icon); });
mask(MaskShape::Star, [&]() { image(rating_star); });
```

Shapes available: squircle, heart, hexagon, hexagon-2, decagon, pentagon,
diamond, square, circle, parallelogram, star, star-2, triangle, triangle-2,
triangle-3, triangle-4.

Use cases: avatar frames, achievement shapes, rarity borders, faction emblems.

### 7.10 Countdown / Number Ticker (Medium Priority)

Animated rolling number display:

```cpp
countdown({ .value = seconds_remaining, .digits = 2 });
```

Use cases: match timer, cooldown display, score counter, auction timer.

### 7.11 Rating / Star Rating (Medium Priority)

```cpp
rating({
    .value = 3.5f,
    .max = 5,
    .half_star = true,
    .size = Size::MD,
});
```

Use cases: difficulty rating, item quality, player review.

### 7.12 Hover 3D (Low Priority)

Perspective tilt with shine effect on mouse hover:

```cpp
hover_3d([&]() {
    card([&]() {
        item_image(legendary_sword);
        text("Excalibur");
    });
});
```

Uses CSS perspective + `rotate3d()` driven by mouse position within a 3x3
grid.  Creates a convincing physical-card-in-hand feel.

Use cases: loot reveals, card games, item inspection, achievement showcase.

---

## 8. Updated Cross-Library Component Matrix

Components newly identified from DaisyUI (not in any prior analysis):

| New Component | DaisyUI | Game UI Priority |
|---|---|---|
| **Chat Bubble** | Yes | High |
| **Countdown/Ticker** | Yes | High |
| **Dock (Bottom Tab Bar)** | Yes | High |
| **FAB (Floating Action)** | Yes | Medium |
| **Indicator (Badge Overlay)** | Yes | High |
| **Kbd (Key Display)** | Yes | High |
| **Mask (Shape Clip)** | Yes | High |
| **Rating (Stars)** | Yes | Medium |
| **Stat Display** | Yes | High |
| **Steps (Multi-Step)** | Yes | High |
| **Swap (Toggle Animation)** | Yes | High |
| **Text Rotate** | Yes | Low |
| **Timeline** | Yes | Medium |
| **Hover 3D / Tilt Card** | Yes | Low |
| **Filter (Collapse Radio)** | Yes | Medium |
| **Diff (Comparison Slider)** | Yes | Low |

---

## 9. Token System Comparison (All Libraries)

| Token | Bootstrap | Radix | DaisyUI | afterhours Current |
|---|---|---|---|---|
| **Surface colors** | 2 (body, secondary) | 12-step gray | 3 (base 100/200/300) | ~3 (bg, surface, surface_alt) |
| **Semantic pairs** | 8 (no content) | accent + gray | 10 (with content) | ~6 (no info/success/warning) |
| **Radius tiers** | 6 | 6 | 3 (selector/field/box) | 1 (roundness float) |
| **Size scale** | 6 | 9 | 5 (xs-xl) | 4 (sm/md/lg/xl) |
| **Depth/shadow** | 4 fixed levels | 6 fixed levels | 1 float (depth 0-1) | Per-component |
| **Dark mode** | `data-bs-theme` | Theme provider | `data-theme` | Theme swap |
| **Theme count** | 1 + dark | ~20 accent options | 35 pre-built | Arbitrary |
| **Color space** | Hex/RGB | N/A (CSS) | OKLCH | Linear RGB |
| **Total tokens** | ~50+ | ~100+ | ~26 | ~20 |

### Convergence: The Universal Token Set

Across all 5 analyzed libraries, these tokens appear universally:

**Must-have colors (7 semantic roles):**
primary, secondary, accent/neutral, info, success, warning, error

**Must-have design tokens:**
radius (2-3 tiers), border width, shadow/depth scale, font size scale

**DaisyUI's contribution:** Proving that 26 tokens is sufficient, the
`depth` float, the `*-content` pairing pattern, and OKLCH as color space.

---

## Summary

DaisyUI's main contributions to the afterhours analysis:

1. **Minimal theme tokens** — 26 variables create 35 wildly different themes.
   This is the target: a small, well-chosen token set that enables maximum
   visual variety.

2. **Depth variable** — A single `float depth` (0-1) scales all shadows,
   borders, bevels, and text-shadows. This is DaisyUI's most original idea.

3. **Content color pairing** — Every semantic color has a pre-computed
   `*-content` counterpart, eliminating runtime contrast calculation.

4. **OKLCH color space** — Perceptually uniform color manipulation via
   `color-mix(in oklab, ...)` produces visually correct derived states.

5. **16 unique components** — Chat bubbles, countdown tickers, docks, FABs,
   indicators, kbd displays, shape masks, ratings, stat displays, steps,
   swap toggles, and hover 3D effects are all absent from prior analyses
   and directly useful for game UIs.

6. **Button variant exhaustiveness** — 10 colors × 8 styles × 5 sizes × 4
   shapes = 1,600 button variants from one definition, demonstrating the
   power of CSS variable indirection for style composition.

7. **3-tier radius** — selector / field / box radius tiers are more useful
   than one global roundness or 6 arbitrary levels.
