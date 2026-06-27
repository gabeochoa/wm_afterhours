# Materialize CSS Analysis for afterhours

## Overview

[Materialize](https://materializeweb.com/) is a responsive front-end CSS framework
based on Google's Material Design guidelines. It provides a comprehensive set of styled
components, a 12-column grid system, color palette, typography scale, shadow/elevation
system, and interactive JavaScript components — all following Material Design principles.

Materialize is relevant to afterhours because it provides a complete, opinionated design
system with well-defined component behaviors, visual hierarchy rules, and interaction
patterns that can be adopted for a native UI framework.

---

## 1. Design Principles (from Material Design)

Materialize implements these core Material Design principles:

### 1.1 Elevation and Shadow

Material Design treats UI surfaces as physical sheets of material at different elevations.
Materialize implements 6 shadow levels (z-depth 0–5):

| Level | Use Case | Shadow Intensity |
|-------|----------|-----------------|
| z-depth-0 | Flat / no shadow | None |
| z-depth-1 | Cards, raised elements at rest | Subtle |
| z-depth-2 | Cards on hover, raised buttons | Light |
| z-depth-3 | Navigation bars, app bars | Medium |
| z-depth-4 | Dialogs, modals | Strong |
| z-depth-5 | Maximum elevation (rare) | Heavy |

**afterhours relevance:** afterhours has no formalized elevation/shadow system. This
maps well to the `depth` variable concept from the DaisyUI analysis and the 4-layer
shadow system from Ant Design. A unified 5-level elevation system should be adopted.

### 1.2 Color System

Materialize provides a full Material Design color palette:
- **19 base hues** (red, pink, purple, deep-purple, indigo, blue, light-blue, cyan,
  teal, green, light-green, lime, yellow, amber, orange, deep-orange, brown, grey,
  blue-grey)
- **14 shades per hue**: base, lighten-1 through lighten-5, darken-1 through darken-4,
  accent-1 through accent-4
- **Semantic application**: background classes (`red lighten-2`) and text classes
  (`red-text text-darken-3`)

**afterhours relevance:** afterhours has 6 named colors and `darken()`/`lighten()`
utility functions. A more structured palette with numbered shade levels (like
Materialize's lighten-1 through darken-4) would provide:
- Consistent hover/active state derivations
- Predictable color hierarchy within a hue family
- Theme-level color scale configuration

### 1.3 Typography

Materialize uses Roboto font with a defined type scale:
- **6 heading levels** (h1–h6) with defined sizes
- **Flow text**: a responsive text system where font size and line height scale fluidly
  with viewport width, keeping line length between 45–80 characters
- **Blockquotes** with left-border accent styling

**afterhours relevance:** Flow text (responsive font sizing) is a unique concept. For
game UIs that need to work across different resolutions, a `flow_text` mode that
interpolates font size between a min and max based on container width would be valuable.

### 1.4 Waves Effect

Materialize implements a "waves" (ripple) feedback effect on interactive elements.
When a user clicks/taps a button, a circular wave animation expands from the click
point, providing immediate visual feedback that the interaction was registered.

Variants:
- `waves-effect` — default (dark ripple on light backgrounds)
- `waves-light` — light ripple on dark backgrounds
- `waves-red`, `waves-yellow`, `waves-orange`, `waves-purple`, `waves-green`,
  `waves-teal` — colored ripples

**afterhours relevance:** afterhours has no click feedback animation. A ripple/wave
effect on buttons and interactive elements would significantly improve perceived
responsiveness. This is implementable as a radial fill animation originating from the
click position, expanding over ~300ms with fade-out.

### 1.5 Pulse Effect

A pulsing circle animation on elements to draw attention (e.g., new features,
onboarding hints). The element radiates a colored ring outward repeatedly.

**afterhours relevance:** Useful for game UI tutorials, new feature indicators, and
notification badges. Implementable as a repeating ring animation around an element.

---

## 2. Component Inventory

### 2.1 Full Component List

Materialize provides these components, categorized:

#### Layout & Structure
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Grid** | 12-column responsive grid with breakpoints (s, m, l, xl) | Flex layout ✓ |
| **Container** | Centered content wrapper (~70% width) | Div with max-width ✓ |
| **Footer** | Page footer with sitemap links | Not applicable |

#### Navigation
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Navbar** | Top navigation bar with logo, links, search | `navigation_bar()` ✓ |
| **Sidenav** | Slide-out side navigation drawer | **Missing** |
| **Breadcrumbs** | Path-based navigation trail | **Missing** (identified in Bootstrap analysis) |
| **Tabs** | Tab bar with swipeable content panes | `tab_container()` ✓ |
| **Pagination** | Page number navigation | `pagination()` ✓ |
| **Scrollspy** | Highlight current section in navigation | **Missing** |
| **Pushpin** | Pin element at position during scroll | **Missing** (sticky positioning) |

#### Buttons & Actions
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Buttons** | 5 variants: Filled, Tonal, Outlined, Elevated, Text | `button()` with `ButtonVariant` (Filled, Outline, Ghost) |
| **Floating Action Button (FAB)** | Circular button with expandable sub-actions | **Missing** |
| **Icon Buttons** | Buttons with material icons | `button()` with icon ✓ |

#### Content Display
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Cards** | Content container with title, body, actions, image, reveal | **Missing** as formal component |
| **Collections** | Grouped list items with avatars, secondary content, actions | **Missing** |
| **Badges** | Small notification count/label overlays | **Missing** (identified in Ant Design analysis) |
| **Carousel** | Touch-enabled image/item slider with 3D perspective | **Missing** (identified in Swiper analysis) |
| **Parallax** | Background image moves at different scroll speed | **Missing** |
| **Media / Lightbox** | Click-to-expand image viewer (Material Box) | **Missing** |
| **Slider** | Full-width image slider with captions | **Missing** |

#### Feedback & Overlays
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Modals** | Dialog/confirmation overlays with actions | Modal plugin exists ✓ |
| **Toasts** | Temporary notification messages | Toast plugin exists ✓ |
| **Tooltips** | Hover/focus info popups (top, bottom, left, right) | Planned (`docs/10_tooltip.md`) |
| **Preloader (Linear)** | Determinate/indeterminate progress bar | `progress_bar()` ✓ |
| **Preloader (Circular)** | Spinning circle loader (3 sizes, 4 colors) | `circular_progress()` ✓ |
| **Feature Discovery** | Tap target / onboarding spotlight | **Missing** |

#### Form Controls
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Text Inputs** | Material-style input with floating label, validation | Text input exists ✓ |
| **Textarea** | Multi-line text input with auto-resize | Text area exists ✓ |
| **Select** | Styled dropdown select with search, multi-select | `dropdown()` ✓ |
| **Checkboxes** | Material-styled checkboxes | `checkbox()` ✓ |
| **Radio Buttons** | Material-styled radio groups | `radio_group()` ✓ |
| **Switches** | On/off toggle switches | `toggle_switch()` ✓ |
| **Range** | Slider input for numeric ranges | `slider()` ✓ |
| **Autocomplete** | Text input with suggestion dropdown | **Missing** |
| **Chips** | Tag-style removable tokens | **Missing** |
| **Date Picker** | Calendar-based date selection | **Missing** (identified in Bootstrap analysis) |
| **Time Picker** | Clock-based time selection | **Missing** |

#### Visual Effects
| Component | Description | afterhours Status |
|-----------|-------------|-------------------|
| **Waves** | Ripple click feedback animation | **Missing** |
| **Pulse** | Attention-drawing pulse animation | **Missing** |
| **Shadow** | 6-level elevation system | **Missing** as formal system |
| **Collapsible** | Accordion / expandable sections | **Missing** (identified as Accordion) |
| **Dropdown** | Generic dropdown menu | `dropdown()` ✓ |

---

## 3. Component Deep Dives

### 3.1 Button Variants (5 Types)

Materialize implements Material Design 3's button taxonomy:

| Variant | Emphasis | Visual | Use Case |
|---------|----------|--------|----------|
| **Filled** | High | Solid background + text color | Primary action (1 per screen) |
| **Tonal** | Medium | Muted background (lighter than filled) | Secondary actions |
| **Outlined** | Medium | Border + transparent background | Alternative secondary actions |
| **Elevated** | Medium | Tonal + shadow | Depth on flat pages |
| **Text** | Low | Text only, no background | Tertiary actions, in-card actions |

Plus **Floating** (FAB): circular, elevated, icon-only.

Size variants: Standard, Large, Small.

**afterhours has:** `ButtonVariant::Filled`, `ButtonVariant::Outline`, `ButtonVariant::Ghost`

**Gap:** Missing **Tonal** and **Elevated** variants. Tonal is particularly useful — it's
a middle ground between filled and outline that works well for secondary actions
(e.g., "Cancel" next to "Submit"). Elevated adds shadow to a tonal button.

**Recommendation:** Add `ButtonVariant::Tonal` and `ButtonVariant::Elevated`:

```cpp
enum struct ButtonVariant {
    Filled,    // High emphasis: solid bg
    Tonal,     // Medium emphasis: muted bg (lighten primary by 60%)
    Outlined,  // Medium emphasis: border, transparent bg (was "Outline")
    Elevated,  // Medium emphasis: tonal + shadow
    Text,      // Low emphasis: text only (was "Ghost")
};
```

### 3.2 Cards

Materialize cards are rich content containers with these sub-components:

| Card Part | Description |
|-----------|-------------|
| **card-image** | Top image with optional title overlay |
| **card-content** | Main text body |
| **card-action** | Bottom action links/buttons |
| **card-title** | Title text (in content or image overlay) |
| **card-reveal** | Hidden content that slides up on click (reveal pattern) |
| **card-tabs** | Tabs embedded in the card |
| **card-stacked** | Horizontal layout (image left, content right) |
| **FAB in card** | Floating action button positioned between image and content |
| **sticky-action** | Card-action remains visible even when reveal is open |

**afterhours relevance:** Cards are one of the most commonly used UI patterns in games
(item cards, character cards, quest cards, shop items). A formal card component would be
high value:

```cpp
struct CardConfig {
    std::optional<TextureConfig> image;      // Top image
    std::string title;                        // Card title
    std::string content;                      // Body text
    std::vector<std::string> actions;         // Action button labels
    bool horizontal = false;                  // Image left, content right
    bool has_reveal = false;                  // Click to reveal more content
    std::string reveal_content;               // Hidden reveal content
};
```

The **card-reveal** pattern (tap to flip/reveal more info) is especially useful for
game UIs — tap an item card to see its stats, tap a character to see their backstory.

### 3.3 Collections (List Items)

Materialize collections are structured list items with these configurations:

| Type | Content |
|------|---------|
| **Basic** | Simple text items |
| **Links** | Clickable items with hover state |
| **Headers** | Section headers within the list |
| **Secondary content** | Right-aligned icons/actions per item |
| **Avatar** | Circular image + title + description + action per item |

**afterhours relevance:** Collections map to a `list_item()` component with avatar,
primary text, secondary text, and trailing action icon. This is essential for:
- Inventory lists
- Player lists
- Settings menus
- Chat participant lists
- Leaderboards

### 3.4 Feature Discovery (Tap Target)

A unique Materialize component for onboarding/tutorials. It creates a large colored
circle behind a target element with a title and description, drawing the user's
attention to a specific UI element.

**afterhours relevance:** Directly applicable to game tutorials. A `feature_spotlight()`
component that highlights a specific UI element with an explanatory overlay:

```cpp
struct FeatureSpotlight {
    EntityID target_element;     // Element to spotlight
    std::string title;           // "Try this feature!"
    std::string description;     // Explanatory text
    Color spotlight_color;       // Background circle color
    std::function<void()> on_dismiss;
};
```

### 3.5 Chips

Small, removable tag/token elements used for:
- Tags/labels
- Contact chips (with avatar)
- Input chips (user types, chip is created)
- Filter chips (toggleable)

**afterhours relevance:** Chips are useful for:
- Inventory item tags (rarity, type, enchantment)
- Filter/sort criteria display
- Selected items in multi-select
- Player status tags

### 3.6 Waves (Ripple Effect)

Implementation details from Materialize:
- Triggered on mousedown/touchstart at the click position
- A `<div class="waves-ripple">` is created at the click point
- Expands via CSS transform: `scale()` from 0 to covering the element
- Duration: ~300ms for the expansion, ~300ms for the fade-out
- Color: semi-transparent (rgba) version of a base color
- Multiple ripples can coexist (rapid clicking creates overlapping ripples)

**afterhours recommendation:** Add a ripple feedback system:

```cpp
struct RippleEffect : BaseComponent {
    float center_x, center_y;  // Origin point (click position)
    float radius = 0.0f;       // Current expansion radius
    float max_radius;           // Final radius (diagonal of element)
    float alpha = 0.4f;         // Current opacity
    float expand_duration = 0.3f;
    float fade_duration = 0.3f;
    Color color;
    float elapsed = 0.0f;
};
```

Rendered as a filled circle with expanding radius and fading alpha, clipped to the
parent element bounds via scissor mode.

### 3.7 Collapsible (Accordion)

An accordion/expandable section component with options:
- **Accordion mode** — only one section open at a time
- **Expandable mode** — multiple sections can be open
- **Popout mode** — active section expands wider than inactive ones
- **Preselected** — specific section open by default

Each collapsible item has a header and a body. Clicking the header toggles the body
visibility with a slide animation.

**afterhours relevance:** Identified in multiple analyses. Accordion/collapsible is
one of the most-requested missing components, useful for:
- Settings categories
- FAQ sections
- Inventory categories
- Nested navigation

### 3.8 Autocomplete

Text input with dropdown suggestion list:
- Filters suggestions as user types
- Supports single-select and multi-select modes
- Each suggestion can have: `id`, `text`, `image`
- Dropdown appears on focus/input, dismisses on blur/selection

**afterhours relevance:** Autocomplete is useful for:
- Search bars
- Command palettes
- Item name input
- Chat commands (@mention, /slash commands)

---

## 4. Design Token System

### 4.1 Color Tokens

Materialize uses CSS custom properties for theming:

```css
:root {
    --md-sys-color-primary: #6750A4;
    --md-sys-color-on-primary: #FFFFFF;
    --md-sys-color-primary-container: #EADDFF;
    --md-sys-color-on-primary-container: #21005D;
    --md-sys-color-secondary: ...;
    --md-sys-color-surface: ...;
    --md-sys-color-on-surface: ...;
    --md-sys-color-error: ...;
    /* etc. */
}
```

Key Material Design 3 color roles:
- **Primary** — main brand color
- **Secondary** — accent/complement
- **Tertiary** — third brand color
- **Error** — error state
- **Surface** — background surfaces
- **On-\*** — text/icon color on each surface (e.g., on-primary)

**afterhours has:** `Theme::Usage` enum (Primary, Secondary, Background, Accent, Default)
and `custom_color`.

**Gap:** Missing the "on-\*" pattern (explicit text color for each surface) and
tertiary/surface roles. The `auto_text_color` feature partially addresses this, but
having explicit theme-level text color pairings is more predictable.

### 4.2 Shadow Tokens

6 discrete levels (z-depth-0 through z-depth-5). Each level uses a multi-layer
box-shadow for realism:

```css
.z-depth-1 { box-shadow: 0 2px 2px 0 rgba(0,0,0,0.14),
                          0 3px 1px -2px rgba(0,0,0,0.12),
                          0 1px 5px 0 rgba(0,0,0,0.2); }
```

Each shadow is composed of 3 layers: key light, ambient, and fill — matching real-world
lighting.

**afterhours recommendation:** Formalize shadow/elevation as theme tokens:

```cpp
struct ElevationLevel {
    float offset_y;     // Vertical offset
    float blur_radius;  // Blur amount
    float spread;       // Spread amount
    float opacity;      // Shadow opacity
};

// In Theme:
std::array<ElevationLevel, 6> elevation_levels = {{
    {0, 0, 0, 0.0f},        // Level 0: flat
    {2, 2, 0, 0.14f},       // Level 1: cards at rest
    {4, 5, 0, 0.14f},       // Level 2: cards on hover
    {6, 10, 0, 0.14f},      // Level 3: nav bars
    {8, 10, 1, 0.14f},      // Level 4: dialogs
    {12, 17, 2, 0.14f},     // Level 5: maximum
}};
```

### 4.3 Spacing

Materialize uses a consistent spacing system but doesn't formalize it into numbered
tokens like some other frameworks. It relies on component-specific padding and the
grid's gutter width.

### 4.4 Typography Scale

```
h1: 4.2rem  (light weight, 56px)
h2: 2.56rem (light weight, 34px)
h3: 1.92rem (regular, 26px)
h4: 1.28rem (regular, 17px)
h5: 1.18rem (regular, 16px)  — default paragraph
h6: 1rem    (regular, 13px)
```

Line height: 1.5 for body text.

**Flow text** formula: font size interpolates between 1.2rem (at 360px viewport) and
1.68rem (at 1200px viewport), with matching line height adjustment. This keeps
line length optimal across screen sizes.

---

## 5. Interaction Patterns

### 5.1 Auto Init

Materialize provides `M.AutoInit()` — a single call that automatically initializes
all components on the page by scanning for specific CSS classes. This is a convenience
for rapid prototyping.

**afterhours relevance:** Not directly applicable (immediate-mode handles this), but
the concept of "convention over configuration" — components that work with minimal
setup — is valuable. afterhours components should have sensible defaults that require
minimal `ComponentConfig` customization.

### 5.2 Plugin Instance Pattern

Every Materialize JavaScript component follows the same pattern:

```javascript
// Initialize
const instance = M.ComponentName.init(element, options);

// Access instance later
const instance = M.ComponentName.getInstance(element);

// Methods
instance.open();
instance.close();
instance.destroy();

// Properties
instance.el        // DOM element
instance.options   // Configuration
instance.isOpen    // State
```

**afterhours relevance:** This uniform API (init → use → destroy) with consistent
property naming is a good pattern. afterhours components should follow a similar
convention for stateful components (modals, dropdowns, tooltips).

### 5.3 Callback Hooks

Materialize components provide consistent callback options:

```javascript
{
    onOpenStart: function(el) {},   // Before open animation
    onOpenEnd: function(el) {},     // After open animation
    onCloseStart: function(el) {},  // Before close animation
    onCloseEnd: function(el) {},    // After close animation
}
```

This 4-callback pattern (before/after × open/close) appears across modals, sidenav,
dropdown, collapsible, materialbox, etc.

**afterhours relevance:** Standardize lifecycle callbacks for all animated/stateful
components. The `onOpenStart`/`onOpenEnd`/`onCloseStart`/`onCloseEnd` pattern aligns
with the lifecycle hooks recommended in the Riot.js analysis.

---

## 6. Cross-Reference with afterhours

### Components afterhours Already Has

| Materialize Component | afterhours Equivalent |
|----------------------|----------------------|
| Buttons (Filled, Outlined) | `button()` with `ButtonVariant` |
| Navbar | `navigation_bar()` |
| Tabs | `tab_container()` |
| Pagination | `pagination()` |
| Progress Bar (Linear) | `progress_bar()` |
| Progress Bar (Circular) | `circular_progress()` |
| Modal | Modal plugin |
| Toast | Toast plugin |
| Dropdown | `dropdown()` |
| Checkbox | `checkbox()` |
| Radio | `radio_group()` |
| Switch | `toggle_switch()` |
| Range/Slider | `slider()` |
| Text Input | Text input component |
| Textarea | Text area component |
| Grid | Flex layout system |
| Image | `image()` |

### Components afterhours Should Add

Prioritized by game UI relevance:

| Priority | Component | Game UI Use Case |
|----------|-----------|-----------------|
| **High** | **Card** | Item cards, character cards, quest cards, shop items |
| **High** | **Collapsible/Accordion** | Settings categories, inventory groups, FAQ |
| **High** | **Chips/Tags** | Item tags, filter criteria, status labels |
| **High** | **Waves (Ripple)** | Click feedback on all interactive elements |
| **High** | **Badge** | Notification counts, new item indicators |
| **High** | **Autocomplete** | Search, command palette, chat commands |
| **Medium** | **FAB** | Quick action menu (inventory shortcuts, build menu) |
| **Medium** | **Feature Discovery** | Tutorial spotlights, onboarding |
| **Medium** | **Collection/List Item** | Inventory lists, player lists, leaderboards |
| **Medium** | **Sidenav/Drawer** | Side menu, inventory panel |
| **Medium** | **Breadcrumbs** | Navigation trail in nested menus |
| **Medium** | **Carousel** | Item showcase, character selection |
| **Medium** | **Tonal + Elevated button variants** | Secondary actions, depth on flat UIs |
| **Low** | **Pulse effect** | Attention drawing, tutorial hints |
| **Low** | **Parallax** | Background decoration (lobby screens) |
| **Low** | **Scrollspy** | Scrolling content with section tracking |
| **Low** | **Pushpin/Sticky** | Pinned headers in scrollable content |
| **Low** | **Lightbox/MaterialBox** | Full-screen image viewing |

### Design System Gaps to Address

| Gap | Materialize Has | afterhours Has | Recommendation |
|-----|----------------|----------------|----------------|
| **Elevation system** | 6 levels (z-depth-0 to z-depth-5) | None formalized | Add 5-level elevation to Theme |
| **Color shade scale** | 14 shades per hue | `darken()`/`lighten()` functions | Add numbered shade levels |
| **On-surface colors** | Explicit text color per surface | `auto_text_color` (computed) | Add explicit pairings to Theme |
| **Flow text** | Responsive font sizing | Fixed font sizes | Add resolution-adaptive text |
| **Ripple feedback** | Waves effect on all interactive elements | None | Add ripple animation system |
| **Button taxonomy** | 5 variants (Filled/Tonal/Outlined/Elevated/Text) | 3 variants | Add Tonal + Elevated |
| **Pulse animation** | Attention-drawing pulse | None | Add pulse component/effect |

---

## 7. Key Takeaways

### 7.1 Material Design's Elevation Model

The single most impactful pattern from Materialize is the **elevation/shadow system**.
Every element has a defined z-depth, and shadows reinforce the visual hierarchy. This
is something afterhours should formalize in its Theme:

```cpp
// In Theme:
int elevation = 0;  // 0-5, applied to ComponentConfig

// Usage:
config.with_elevation(2)  // Card at rest
config.with_elevation(3)  // Card on hover (auto-elevated)
```

Components should have **default elevation levels**:
- Buttons: 1 (rest), 2 (hover), 0 (pressed)
- Cards: 1
- Modals/Dialogs: 4
- FAB: 2 (rest), 4 (hover)
- Navbar: 3
- Tooltips: 3
- Toast: 3

### 7.2 Ripple as Universal Feedback

Materialize applies waves/ripple to nearly every interactive element. This provides
consistent, immediate feedback across all interactions. afterhours should add a ripple
system that automatically applies to all clickable components (opt-out rather than
opt-in).

### 7.3 Card as First-Class Component

Cards are used everywhere in game UIs but afterhours has no formal card component.
A card with image, title, content, actions, and reveal should be a high-priority
addition. The "reveal" pattern (tap to see more) is especially valuable for
information-dense game UIs.

### 7.4 Material Button Taxonomy

The 5-button hierarchy (Filled → Tonal → Outlined → Elevated → Text) provides clear
visual communication of action importance. afterhours should adopt this taxonomy to
give developers a clear vocabulary for action emphasis.

### 7.5 Feature Discovery for Tutorials

Materialize's "tap target" feature discovery component is a unique and useful pattern
for game tutorials. Instead of building custom tutorial overlays, a standardized
spotlight component that highlights a target element with explanatory text would be
reusable across any game.

---

## 8. Priority Summary

| # | Feature | Effort | Impact | Description |
|---|---------|--------|--------|-------------|
| 1 | **Elevation/shadow system** | Low | High | 5-level theme-integrated shadow tokens |
| 2 | **Ripple/waves feedback** | Medium | High | Click feedback animation on all interactives |
| 3 | **Card component** | Medium | High | Image + title + content + actions + reveal |
| 4 | **Collapsible/accordion** | Medium | High | Expandable sections with animation |
| 5 | **Button taxonomy expansion** | Low | Medium | Add Tonal + Elevated variants |
| 6 | **Chips/tags** | Low | Medium | Removable token elements |
| 7 | **Autocomplete** | Medium | Medium | Text input with suggestion dropdown |
| 8 | **Badge** | Low | Medium | Notification count overlay |
| 9 | **Feature discovery** | Medium | Medium | Tutorial spotlight component |
| 10 | **Collection/list item** | Medium | Medium | Structured list with avatar + actions |
| 11 | **Flow text** | Low | Low | Resolution-adaptive font sizing |
| 12 | **Pulse effect** | Low | Low | Attention-drawing animation |
| 13 | **Color shade scale** | Medium | Medium | Numbered shade levels per hue |
| 14 | **FAB** | Low | Low | Circular expandable action button |
