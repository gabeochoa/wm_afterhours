# A11y Project Checklist → afterhours Validation Mapping

Source: https://www.a11yproject.com/checklist/
Based on WCAG 2.2 (A and AA criteria)

This document maps each A11y Project checklist item to either:
- An **existing** afterhours validation
- A **proposed** new validation flag for `validation_config.h`
- **N/A** (web-specific, not applicable to native UI)

---

## Current Validation Coverage

What `validation_config.h` already checks:

| Flag | Checklist Category |
|---|---|
| `enforce_contrast_ratio` | Color Contrast |
| `enforce_min_font_size` | Appearance (text scaling) |
| `enforce_screen_bounds` | Mobile/Touch (safe areas) |
| `enforce_child_containment` | Appearance (layout integrity) |
| `enforce_resolution_independence` | Appearance (text scaling) |
| `enforce_spacing_rhythm` | Appearance (consistent layout) |
| `enforce_pixel_alignment` | Appearance (rendering quality) |
| `enforce_zero_size_detection` | Controls (invisible elements) |
| `enforce_absolute_margin_conflict` | Appearance (layout bugs) |
| `enforce_label_has_font` | Content (text rendering) |

---

## Full Checklist Mapping

### Content

| Checklist Item | Applicable? | Existing | Proposed Validation |
|---|---|---|---|
| Use plain language, avoid idioms | N/A | — | Not automatable (content review) |
| Button/link/label content is unique and descriptive | **Yes** | — | `enforce_unique_interactive_labels` |
| Left-aligned text for LTR, right-aligned for RTL | **Yes** | — | `enforce_text_alignment_direction` |

**`enforce_unique_interactive_labels`** — Flag when multiple interactive elements
(buttons, tabs, menu items) share the same label text within one screen.
Duplicate labels make it impossible to distinguish controls when navigating
by name.

```cpp
// Proposed: walk the tree, collect all HasLabel + HasClickListener pairs,
// flag duplicates
bool enforce_unique_interactive_labels = false;
```

**`enforce_text_alignment_direction`** — Flag when text alignment contradicts
the text direction (e.g., right-aligned text in a left-to-right context).
This is relevant once RTL language support is added.

```cpp
bool enforce_text_alignment_direction = false;
```

---

### Global Code

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Validate your HTML | N/A | — | Web-only |
| Use lang attribute | N/A | — | Web-only (but afterhours has `translation::Language`) |
| Unique title per page/view | **Yes** | — | `enforce_unique_screen_titles` |
| Viewport zoom not disabled | N/A | — | Web-only |
| Landmark elements for content regions | N/A | — | ARIA-specific |
| Linear content flow | **Yes** | — | (See tab order below) |
| Avoid autofocus attribute | N/A | — | Web-specific |
| Allow extending session timeouts | **Yes** | — | Design guidance, not automatable |
| Remove title attribute tooltips | N/A | — | Web-specific |

---

### Keyboard

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Visible focus style for keyboard-navigated elements | **Yes** | Partial (theme has `focus_ring_thickness`, `focus_ring_offset`) | `enforce_focus_ring_visibility` |
| Focus order matches visual layout | **Yes** | — | `enforce_focus_order_matches_layout` |
| Remove invisible focusable elements | **Yes** | — | `enforce_no_hidden_focusables` |

**`enforce_focus_ring_visibility`** — Verify that the theme's focus ring has
sufficient contrast against the background and meets minimum size
requirements per WCAG 2.4.13 (Focus Appearance):

```cpp
bool enforce_focus_ring_visibility = false;
// WCAG 2.4.13: Focus indicator must have:
//   - At least 2px thick perimeter
//   - At least 3:1 contrast against adjacent colors
float min_focus_ring_thickness = 2.0f;
float min_focus_ring_contrast = 3.0f;
```

Validation logic:
1. Check `theme.focus_ring_thickness >= min_focus_ring_thickness`
2. Check `colors::contrast_ratio(theme.focus, element_background) >= min_focus_ring_contrast`
3. Check that focused elements actually render a visible focus indicator

**`enforce_focus_order_matches_layout`** — Walk the tab order and verify it
follows a logical reading path (top-to-bottom, left-to-right for LTR).
Flag cases where the tab-order position of an element is dramatically
different from its visual position.

```cpp
bool enforce_focus_order_matches_layout = false;
// Maximum allowed deviation between visual position order and tab order
// (in number of positions)
int max_tab_order_deviation = 3;
```

Validation logic:
1. Collect all focusable elements
2. Sort by tab order (the order `Tab` visits them)
3. Sort by visual position (top-left to bottom-right, row by row)
4. Compare the two orderings — flag elements where the positions differ
   by more than `max_tab_order_deviation`

**`enforce_no_hidden_focusables`** — Flag elements that are in the tab order
but not visible (hidden, zero-size, or fully transparent).

```cpp
bool enforce_no_hidden_focusables = false;
```

Validation logic:
1. Walk the tab order
2. For each focusable element, check: `should_hide == false`,
   `rect().width > 0`, `rect().height > 0`, `opacity > 0`
3. Flag any that are focusable but invisible

---

### Images

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| All images have alt text | Partial | — | `enforce_images_have_description` |
| Decorative images use null alt | N/A | — | Web-specific |
| Text alternative for complex images | Partial | — | Same flag |
| Alt includes image text | N/A | — | Not automatable |

**`enforce_images_have_description`** — Flag `HasImage` components that
don't have a corresponding `HasLabel` or a dedicated description component.
In game UI, images (icons, avatars) should have labels for players who
rely on non-visual cues (colorblindness, small screens, etc.).

```cpp
bool enforce_images_have_description = false;
// Allow images below this size to skip description (decorative icons)
float decorative_image_max_size = 16.0f;
```

---

### Headings

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Use headings to introduce content | N/A | — | No heading hierarchy in native UI |
| Only one h1 per page | N/A | — | Web-specific |
| Logical heading sequence | N/A | — | Web-specific |
| Don't skip heading levels | N/A | — | Web-specific |

Not applicable — afterhours doesn't have a heading hierarchy.  The
equivalent concern (clear visual hierarchy) is already addressed by
`enforce_min_font_size` and the font sizing system.

---

### Lists

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Use list elements for list content | N/A | — | Web-specific |

---

### Controls

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Links are recognizable | N/A | — | Web-specific (no links in game UI) |
| Controls have :focus states | **Yes** | Partial | `enforce_focus_ring_visibility` (above) |
| Use button element for buttons | N/A | — | Web-specific |
| Skip link visible when focused | N/A | — | Web-specific |
| Identify links opening new tabs | N/A | — | Web-specific |

---

### Tables

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Use table for tabular data | N/A | — | Web-specific |
| th for headers with scope | N/A | — | Web-specific |
| caption for table title | N/A | — | Web-specific |

---

### Forms

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Inputs associated with labels | **Yes** | — | `enforce_inputs_have_labels` |
| Fieldset and legend where appropriate | N/A | — | Web-specific |
| Inputs use autocomplete | N/A | — | Web-specific |
| Error list above form | N/A | — | Web-specific pattern |
| Error messaging associated with input | **Yes** | — | `enforce_error_near_source` |
| Error/warning/success not just color | **Yes** | — | `enforce_status_not_color_only` |

**`enforce_inputs_have_labels`** — Flag interactive input components
(text_input, slider, checkbox, radio_group, dropdown) that don't have a
visible label (either via `HasLabel` on the element itself or a sibling
label element).

```cpp
bool enforce_inputs_have_labels = false;
```

Validation logic:
1. Find all entities with input state components (`HasSliderState`,
   `HasCheckboxState`, `HasToggleSwitchState`, text input markers)
2. Check if the entity itself has a `HasLabel` with non-empty text
3. If not, check if a sibling or parent contains a label that describes
   this input
4. Flag if no label found

**`enforce_status_not_color_only`** — Flag when a component changes only
its color to indicate a state (error, success, warning) without also
providing text, an icon, or a shape change.  Color-only status is
invisible to colorblind users.

```cpp
bool enforce_status_not_color_only = false;
```

This is harder to automate.  A practical approach:
1. Detect elements using error/success/warning theme colors
2. Check if they also have a `HasLabel` with status text, or an icon
3. Flag if color is the only differentiator

---

### Media

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Media does not autoplay | **Yes** | — | Design guidance |
| Media controls use appropriate markup | N/A | — | Web-specific |
| All media can be paused | **Yes** | — | Design guidance |

---

### Video / Audio

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Captions present | Partial | — | Design guidance (subtitles) |
| Remove seizure triggers | **Yes** | — | `enforce_flash_rate_limit` |
| Transcripts available | N/A | — | Not applicable to game UI |

**`enforce_flash_rate_limit`** — Flag animations or color changes that flash
more than 3 times per second, per WCAG 2.3.1 (Three Flashes or Below
Threshold).  Excessive flashing can trigger seizures.

```cpp
bool enforce_flash_rate_limit = false;
float max_flashes_per_second = 3.0f;
```

This requires tracking color changes over time per element.  A practical
approach:
1. Track background color changes per entity per frame
2. Count transitions between bright/dark states within a rolling 1-second
   window
3. Flag if count exceeds threshold

---

### Appearance

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Content works in specialized browsing modes | N/A | — | Web-specific |
| Text scales to 200% | **Yes** | Existing: `enforce_resolution_independence` | Already covered |
| Good proximity between content | **Yes** | Existing: `enforce_spacing_rhythm` | Already covered |
| Color not the only way info is conveyed | **Yes** | — | `enforce_status_not_color_only` (above) |
| Instructions not visual/audio-only | **Yes** | — | Design guidance |
| Simple, consistent layout | **Yes** | — | Design guidance (audit skill) |

---

### Animation

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Animations subtle, don't flash too much | **Yes** | — | `enforce_flash_rate_limit` (above) |
| Mechanism to pause background video | **Yes** | — | Design guidance |
| Respect prefers-reduced-motion | **Yes** | — | `enforce_reduced_motion_support` |

**`enforce_reduced_motion_support`** — Verify that a reduced-motion
preference is available and respected.  In game UI, this means:
- A setting to disable/reduce non-essential animations
- The system checks if this setting exists

```cpp
bool enforce_reduced_motion_support = false;
```

This is more of a design-time check than a runtime validation.  Practical
approach: check that the application has registered a "reduced motion"
preference flag and that animated components reference it.

---

### Color Contrast

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Contrast for normal text | **Yes** | **Existing:** `enforce_contrast_ratio` (4.5:1) | — |
| Contrast for large text | **Yes** | Partial | `enforce_large_text_contrast` |
| Contrast for icons | **Yes** | — | `enforce_icon_contrast` |
| Contrast for input borders | **Yes** | — | `enforce_input_border_contrast` |
| Text overlapping images/video | **Yes** | — | Design guidance (use text_stroke) |
| Custom selection colors | N/A | — | Web-specific |

**`enforce_large_text_contrast`** — WCAG allows a lower contrast ratio
(3:1 instead of 4.5:1) for "large text" (18pt+ or 14pt+ bold).
Currently `enforce_contrast_ratio` uses one threshold for everything.

```cpp
bool enforce_large_text_contrast = false;
float large_text_min_contrast = 3.0f;
float large_text_threshold_size = 24.0f;    // 18pt ≈ 24px
float large_bold_text_threshold_size = 18.7f; // 14pt bold ≈ 18.7px
```

Validation logic:
1. For elements with `HasLabel`, check if font size ≥ large text threshold
2. If large: use `large_text_min_contrast` (3:1)
3. If normal: use `min_contrast_ratio` (4.5:1) from existing check
4. Also check if font weight is bold — bold text at 14pt+ qualifies as large

**`enforce_icon_contrast`** — Check that icons (elements with `HasImage` that
are likely icons based on size) have sufficient contrast against their
background.  WCAG 1.4.11 requires 3:1 for non-text UI components.

```cpp
bool enforce_icon_contrast = false;
float min_non_text_contrast = 3.0f;
```

**`enforce_input_border_contrast`** — Check that input fields (text_input,
dropdown, etc.) have borders with at least 3:1 contrast against the
surrounding background, per WCAG 1.4.11.

```cpp
bool enforce_input_border_contrast = false;
float min_input_border_contrast = 3.0f;
```

---

### Mobile and Touch

| Checklist Item | Applicable? | Existing | Proposed |
|---|---|---|---|
| Site can rotate to any orientation | Partial | — | Design guidance |
| Remove horizontal scrolling | Partial | `enforce_screen_bounds` | Partial coverage |
| Button/link icons activatable with ease | **Yes** | — | `enforce_min_touch_target` |
| Sufficient space between interactive items | **Yes** | — | `enforce_touch_target_spacing` |

**`enforce_min_touch_target`** — Ensure all interactive elements meet
minimum touch target size.  WCAG 2.5.8 (Target Size Minimum) requires
at least 24×24 CSS pixels.  Apple HIG recommends 44×44.

afterhours already defines `MIN_TOUCH_TARGET = 44.0f` in
`styling_defaults.h` and uses it in radio groups, but doesn't validate it
globally.

```cpp
bool enforce_min_touch_target = false;
float min_touch_target_size = 44.0f;  // WCAG: 24, Apple: 44, Android: 48
```

Validation logic:
1. Find all entities with `HasClickListener` or input state components
2. Check that `rect().width >= min_touch_target_size` and
   `rect().height >= min_touch_target_size`
3. Flag any interactive element that's too small

**`enforce_touch_target_spacing`** — Ensure adequate spacing between
adjacent interactive elements so users don't accidentally tap the wrong
one.  WCAG 2.5.8 allows smaller targets if sufficient spacing exists.

```cpp
bool enforce_touch_target_spacing = false;
float min_touch_target_gap = 8.0f;  // Minimum gap between clickable elements
```

Validation logic:
1. Collect all interactive elements' rects
2. For each pair of adjacent interactive elements, compute the gap
3. Flag if gap < `min_touch_target_gap`

---

## Proposed New Flags Summary

### High Priority (common issues, straightforward to implement)

| Flag | Category | WCAG | Severity |
|---|---|---|---|
| `enforce_min_touch_target` | Mobile/Touch | 2.5.8 | 0.8 |
| `enforce_focus_ring_visibility` | Keyboard | 2.4.7, 2.4.13 | 0.9 |
| `enforce_inputs_have_labels` | Forms | 1.3.1, 3.3.2 | 0.8 |
| `enforce_large_text_contrast` | Contrast | 1.4.3 | 0.7 |
| `enforce_no_hidden_focusables` | Keyboard | 2.4.3 | 0.7 |
| `enforce_unique_interactive_labels` | Content | 2.4.6 | 0.6 |

### Medium Priority (useful, more complex to implement)

| Flag | Category | WCAG | Severity |
|---|---|---|---|
| `enforce_focus_order_matches_layout` | Keyboard | 2.4.3 | 0.7 |
| `enforce_touch_target_spacing` | Mobile/Touch | 2.5.8 | 0.6 |
| `enforce_input_border_contrast` | Contrast | 1.4.11 | 0.6 |
| `enforce_icon_contrast` | Contrast | 1.4.11 | 0.5 |
| `enforce_status_not_color_only` | Forms/Appearance | 1.4.1 | 0.7 |
| `enforce_images_have_description` | Images | 1.1.1 | 0.5 |

### Low Priority (hard to automate, design-guidance-level)

| Flag | Category | WCAG | Severity |
|---|---|---|---|
| `enforce_flash_rate_limit` | Animation | 2.3.1 | 0.9 |
| `enforce_reduced_motion_support` | Animation | 2.3.3 | 0.5 |
| `enforce_text_alignment_direction` | Content | 1.3.2 | 0.3 |

---

## Implementation Plan

### Phase 1: Quick Wins (extend existing systems)

These build on infrastructure already in place:

**1a. `enforce_min_touch_target`**

Add to `ValidationConfig`. New system `ValidateMinTouchTarget` queries
`HasClickListener` + `UIComponent`, checks rect size.

```cpp
struct ValidateMinTouchTarget : System<UIComponent, HasClickListener> {
    virtual void for_each_with(Entity &entity, UIComponent &cmp,
                               HasClickListener &, float) override {
        // ... check cmp.rect().width >= config.min_touch_target_size
        // ... check cmp.rect().height >= config.min_touch_target_size
    }
};
```

Should also check `HasSliderState`, `HasCheckboxState`,
`HasToggleSwitchState` entities.

**1b. `enforce_focus_ring_visibility`**

Validate at theme level + per-element:
- Theme check: `theme.focus_ring_thickness >= 2.0f`
- Per-element: `contrast_ratio(theme.focus, element_bg) >= 3.0f`

New system `ValidateFocusRingVisibility` queries `UIComponent` + `HasColor`
for focused elements.

**1c. `enforce_inputs_have_labels`**

New system that queries for input components and checks for `HasLabel`:

```cpp
struct ValidateInputsHaveLabels : System<UIComponent, HasSliderState> {
    // ... also need variants for HasCheckboxState, HasToggleSwitchState
};
```

### Phase 2: Contrast Extensions

**2a. `enforce_large_text_contrast`**

Modify `ValidateComponentContrast` to check font size and use the
appropriate threshold:

```cpp
float threshold = config.min_contrast_ratio;  // 4.5 default
float font_size = resolve_to_pixels(cmp.font_size, screen_height);
if (font_size >= config.large_text_threshold_size) {
    threshold = config.large_text_min_contrast;  // 3.0
}
```

**2b. `enforce_input_border_contrast`**

New system `ValidateInputBorderContrast` queries `UIComponent` +
`HasBorder`, computes contrast between border color and the parent's
background color.

**2c. `enforce_icon_contrast`**

New system for `HasImage` elements that aren't decorative, checking
contrast of the icon's dominant color against its background.

### Phase 3: Keyboard Navigation

**3a. `enforce_no_hidden_focusables`**

Walk tab order, check each element's visibility state.

**3b. `enforce_focus_order_matches_layout`**

Collect focusable elements, sort by tab order vs. visual position, compare.

**3c. `enforce_unique_interactive_labels`**

Collect all `HasClickListener` + `HasLabel` pairs, detect duplicate labels.

### Phase 4: Advanced

**4a. `enforce_status_not_color_only`**

Requires detecting semantic color usage (error/success/warning colors)
and checking for accompanying non-color indicators.

**4b. `enforce_flash_rate_limit`**

Requires per-element color history tracking over time.

**4c. `enforce_touch_target_spacing`**

Requires spatial query between adjacent interactive elements.

---

## Updated `ValidationConfig` (proposed additions)

```cpp
struct ValidationConfig {
    // ... existing flags ...

    // === Accessibility: Keyboard (Section K) ===
    bool enforce_focus_ring_visibility = false;
    float min_focus_ring_thickness = 2.0f;
    float min_focus_ring_contrast = 3.0f;

    bool enforce_focus_order_matches_layout = false;
    int max_tab_order_deviation = 3;

    bool enforce_no_hidden_focusables = false;

    // === Accessibility: Touch Targets (Section T) ===
    bool enforce_min_touch_target = false;
    float min_touch_target_size = 44.0f;

    bool enforce_touch_target_spacing = false;
    float min_touch_target_gap = 8.0f;

    // === Accessibility: Labels & Content (Section L) ===
    bool enforce_inputs_have_labels = false;
    bool enforce_unique_interactive_labels = false;
    bool enforce_images_have_description = false;
    float decorative_image_max_size = 16.0f;

    // === Accessibility: Contrast Extensions (Section CE) ===
    bool enforce_large_text_contrast = false;
    float large_text_min_contrast = 3.0f;
    float large_text_threshold_size = 24.0f;
    float large_bold_text_threshold_size = 18.7f;

    bool enforce_icon_contrast = false;
    float min_non_text_contrast = 3.0f;

    bool enforce_input_border_contrast = false;
    float min_input_border_contrast = 3.0f;

    // === Accessibility: Status & Feedback (Section S) ===
    bool enforce_status_not_color_only = false;

    // === Accessibility: Animation Safety (Section A) ===
    bool enforce_flash_rate_limit = false;
    float max_flashes_per_second = 3.0f;

    bool enforce_reduced_motion_support = false;

    // === Accessibility: RTL / Direction (Section D) ===
    bool enforce_text_alignment_direction = false;

    // Updated preset
    ValidationConfig &enable_a11y_mode() {
        mode = ValidationMode::Warn;
        // Existing
        enforce_contrast_ratio = true;
        enforce_min_font_size = true;
        // New
        enforce_focus_ring_visibility = true;
        enforce_min_touch_target = true;
        enforce_inputs_have_labels = true;
        enforce_no_hidden_focusables = true;
        enforce_large_text_contrast = true;
        enforce_input_border_contrast = true;
        enforce_status_not_color_only = true;
        return *this;
    }
};
```

---

## Coverage Summary

| A11y Checklist Category | Total Items | Existing Coverage | Proposed Coverage | N/A (Web-Only) |
|---|---|---|---|---|
| **Content** | 3 | 0 | 2 | 1 |
| **Global Code** | 9 | 0 | 0 | 9 |
| **Keyboard** | 3 | 1 (partial) | 3 | 0 |
| **Images** | 4 | 0 | 1 | 3 |
| **Headings** | 4 | 0 | 0 | 4 |
| **Lists** | 1 | 0 | 0 | 1 |
| **Controls** | 6 | 1 (partial) | 1 | 4 |
| **Tables** | 3 | 0 | 0 | 3 |
| **Forms** | 6 | 0 | 3 | 3 |
| **Media** | 3 | 0 | 0 | 3 (design guidance) |
| **Video** | 2 | 0 | 1 | 1 |
| **Audio** | 1 | 0 | 0 | 1 |
| **Appearance** | 6 | 3 | 1 | 2 |
| **Animation** | 3 | 0 | 2 | 1 |
| **Color Contrast** | 6 | 1 | 3 | 2 |
| **Mobile/Touch** | 4 | 1 (partial) | 2 | 1 |
| **TOTAL** | 64 | **6** | **+15 new** | **38** |

Of the 64 checklist items:
- **38** are web-specific (HTML, ARIA, CSS) → N/A
- **26** are applicable to native UI
- **6** are already covered by existing validations
- **15** are proposed as new validation flags
- **5** are design guidance (not automatable)

After implementing all proposed flags, afterhours would cover **21 of 26**
applicable accessibility criteria through automated validation.

---

## Addendum: Ariakit Accessibility Patterns

Source: https://ariakit.org + source code analysis of `@ariakit/react-core`

Ariakit is the most accessibility-focused component library surveyed.
Several of its patterns suggest new validations or refinements for
afterhours.

### A1. `accessibleWhenDisabled` — Disabled Elements Stay Focusable

Ariakit's `Focusable` component keeps disabled elements in the tab order
using `aria-disabled` (instead of the native `disabled` attribute which
removes them from focus entirely). Events are intercepted and suppressed
so the element can receive focus but can't be activated.

Rationale: Users should be able to *discover* what controls exist, even
when they're currently unavailable (e.g., a "Paste" button when the
clipboard is empty).

**Proposed validation:**

```cpp
bool enforce_disabled_remain_focusable = false;
```

Validation logic:
1. Find all entities with a "disabled" state
2. Check that they are NOT removed from the tab order (i.e., they don't
   have `SkipWhenTabbing` set solely because they're disabled)
3. Check that they still have focus ring styling when focused

Severity: 0.5 (medium) — this is a best practice, not WCAG-required.

### A2. Focus-Visible vs Focus — Input Modality Tracking

Ariakit tracks whether the current interaction is keyboard or mouse
using global event listeners:
- `mousedown` → `isKeyboardModality = false`
- `keydown` (non-modifier, non-meta) → `isKeyboardModality = true`

Focus rings are only shown during keyboard navigation (`data-focus-visible`).
Mouse-clicked elements receive focus but no visible ring.

This is more reliable than CSS `:focus-visible` because it also applies
to virtual focus in composite widgets.

**Proposed validation:**

```cpp
bool enforce_focus_visible_distinction = false;
```

Validation logic:
1. Check that the theme/system has a mechanism to distinguish keyboard
   focus from mouse focus
2. Verify that focus ring rendering is conditional on input modality
3. Flag if focus rings appear for mouse-initiated focus (cosmetic noise)

Severity: 0.4 — UX quality improvement, not strictly a11y compliance.

### A3. Modal Dialog Must Have Dismissable Path

Ariakit's Dialog component always ensures a screen-reader-accessible
dismiss mechanism exists:
- If no `DialogDismiss` component is found inside the dialog, a
  **visually-hidden dismiss button** is prepended automatically
- `hideOnEscape` is `true` by default
- `hideOnInteractOutside` is `true` by default

**Proposed validation:**

```cpp
bool enforce_modal_has_dismiss = false;
```

Validation logic:
1. Find all entities that represent modal dialogs (visible, modal flag)
2. Check that they contain at least one child with a dismiss/close action
3. OR check that Escape key handling is registered
4. Flag modal dialogs with no dismissal mechanism

Severity: 0.8 — directly related to WCAG 2.1.2 (No Keyboard Trap).

### A4. Focus Restoration on Dialog Close

When a dialog closes, Ariakit restores focus to the element that was
focused before the dialog opened (the "disclosure element"), unless
another element was explicitly focused during the close action.

Props: `autoFocusOnHide`, `finalFocus`

**Proposed validation:**

```cpp
bool enforce_focus_restoration = false;
```

Validation logic:
1. Track which element had focus before a modal/popup opened
2. On close, verify that focus returns to that element (or a valid
   alternative)
3. Flag if focus is lost to the root/body after closing a dialog

Severity: 0.7 — WCAG 2.4.3 (Focus Order) compliance.

### A5. Composite Widgets: Single Tab Stop + Arrow Navigation

Ariakit's `Composite` component is the base for Menu, Select, Combobox,
Tab, Toolbar, and Radio. It enforces:
- The entire widget is ONE tab stop
- Arrow keys navigate between items
- Home/End jump to first/last item
- Items not in DOM order are reordered to match visual layout

**Proposed validation:**

```cpp
bool enforce_composite_keyboard_contract = false;
```

Validation logic:
1. Identify compound widget entities (dropdown, tabs, radio_group)
2. Verify that the container is a single tab stop (only the container
   or active item has `tabindex=0`, all others have `tabindex=-1`)
3. Verify that arrow keys move the active item
4. Flag compound widgets where Tab key moves between items instead of
   arrow keys (wrong behavior per WAI-ARIA)

Severity: 0.6 — WAI-ARIA pattern compliance.

### A6. Nested Dialog Stack — Only Topmost Traps Focus

Ariakit tracks nested dialogs with `useNestedDialogs()`. When multiple
dialogs are stacked, only the topmost one traps focus. Lower dialogs
release their focus trap when a child dialog opens.

**afterhours implication:** The dialog focus-trapping system should be
stack-aware. If dialog B opens on top of dialog A, pressing Tab should
cycle within B, not A. When B closes, A's trap should re-engage.

This is a behavioral requirement, not easily validated statically. Add
to design guidance rather than an automated flag.

### A7. Non-Modal Dialogs Preserve Tab Order

Ariakit's Dialog with `modal={false}` uses `preserveTabOrder` — the
dialog content is inserted into the document's natural tab flow at the
position of its portal. This means Tab from inside the dialog continues
to the next document element, not back to the dialog start.

**afterhours implication:** Non-modal popups (tooltips, hovercards,
non-modal popover) should participate in the tab order naturally, not
create an isolated focus scope.

### Summary of New Validations from Ariakit

| Flag | Priority | WCAG | Severity |
|---|---|---|---|
| `enforce_modal_has_dismiss` | **High** | 2.1.2 | 0.8 |
| `enforce_focus_restoration` | **High** | 2.4.3 | 0.7 |
| `enforce_composite_keyboard_contract` | **Medium** | APG | 0.6 |
| `enforce_disabled_remain_focusable` | **Low** | Best practice | 0.5 |
| `enforce_focus_visible_distinction` | **Low** | UX quality | 0.4 |

**Updated `enable_a11y_mode()` additions:**

```cpp
ValidationConfig &enable_a11y_mode() {
    // ... existing flags ...
    // From Ariakit analysis:
    enforce_modal_has_dismiss = true;
    enforce_focus_restoration = true;
    return *this;
}
```

**Updated totals:** With these 5 additional flags (2 high, 1 medium,
2 low), the total proposed validation flags increases from 15 to 20.
After full implementation, afterhours would cover **24 of 29**
applicable accessibility criteria through automated validation (adding
3 new applicable criteria from Ariakit patterns).
