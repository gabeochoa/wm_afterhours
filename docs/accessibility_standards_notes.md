# Accessibility Standards Research Notes

Research into WAI-ARIA keyboard patterns, WCAG focus guidelines, and APCA contrast — filtered for what's applicable to afterhours as a native UI framework (not a web framework).

Sources: Base UI (https://base-ui.com/llms.txt), WAI-ARIA APG, WCAG 2.2, APCA.

---

## What's generic vs what's web-only

Base UI is a React component library. A lot of what it does is web-specific plumbing (ARIA attributes, HTML semantics, screen reader live regions, CSS pseudo-classes). We don't need any of that.

What IS generic and applies to any UI system:

- **Keyboard behavior contracts per widget type** — which keys do what
- **Focus management rules** — where focus goes after user actions
- **Focus indicator requirements** — how visible the ring needs to be
- **Contrast algorithms** — APCA as a replacement for WCAG 2 ratio math

What is web-only and we should skip:

- ARIA roles/attributes (`role="dialog"`, `aria-label`, `aria-expanded`, etc.)
- HTML semantics (`<label>`, `<nav>`, `<main>`, `tabindex`)
- Screen reader announcements (live regions, `aria-live`)
- Skip navigation links
- CSS `:focus-visible` vs `:focus` distinction (we control our own focus ring)
- `<form>` element association magic

---

## 1. Keyboard Behavior Contracts

The WAI-ARIA APG defines keyboard interaction patterns for every common widget. These are platform-agnostic behavioral contracts — "when a user presses X key while Y widget is focused, Z happens." afterhours should enforce these.

### Buttons

| Key | Action |
|-----|--------|
| `Enter` or `Space` | Activate the button |

Simple. afterhours already does this.

### Checkbox / Switch / Toggle

| Key | Action |
|-----|--------|
| `Space` | Toggle the state |

Note: `Enter` does NOT toggle a checkbox (unlike buttons). This distinction matters.

### Radio Group

| Key | Action |
|-----|--------|
| Arrow keys (up/down or left/right) | Move selection to next/previous option and select it |
| `Tab` | Move focus OUT of the radio group (to the next widget) |

This is the "roving focus" pattern. Only one radio in the group is in the tab order. Arrows move within the group; Tab exits.

### Tabs

| Key | Action |
|-----|--------|
| Left/Right arrows (horizontal) | Move focus to previous/next tab |
| Up/Down arrows (vertical) | Move focus to previous/next tab |
| `Home` | Move focus to first tab |
| `End` | Move focus to last tab |
| `Tab` | Move focus to the active tab panel |

Same roving focus pattern as radio groups. afterhours has tabs — we should validate this keyboard model.

Base UI provides an `activationDirection` data attribute (`left`, `right`, `up`, `down`) which is useful for animating tab transitions based on direction.

### Slider

| Key | Action |
|-----|--------|
| Right/Up arrow | Increase value by one step |
| Left/Down arrow | Decrease value by one step |
| `Home` | Set to minimum |
| `End` | Set to maximum |
| `Page Up` | Increase by a large step (typically 10%) |
| `Page Down` | Decrease by a large step |

afterhours has sliders. The arrow key behavior probably exists but `Home`/`End`/`Page Up`/`Page Down` likely don't.

### Dialog (Modal)

| Key | Action |
|-----|--------|
| `Escape` | Close the dialog |
| `Tab` | Cycle focus within the dialog (focus trap) |
| `Shift+Tab` | Cycle focus backward within the dialog |

Focus rules:
- **On open**: Focus moves to the first focusable element inside the dialog (or a designated `initialFocus` element).
- **On close**: Focus returns to the element that triggered the dialog (`finalFocus` / focus return).
- **While open**: Tab key MUST NOT escape the dialog. Focus cycles within.

This is the biggest gap in most UI frameworks. Base UI provides `initialFocus` and `finalFocus` props on Dialog to configure this. afterhours has modals — we should enforce all three rules.

### Menu / Dropdown

| Key | Action |
|-----|--------|
| Arrow keys | Navigate between items |
| `Enter` | Activate selected item |
| `Escape` | Close the menu |
| Alphanumeric keys | Jump to item starting with typed character (type-ahead) |
| `Home` | First item |
| `End` | Last item |

Type-ahead is a nice-to-have that makes menus much more usable for keyboard users. afterhours probably doesn't have this yet.

### Accordion / Collapsible

| Key | Action |
|-----|--------|
| `Enter` or `Space` | Toggle the section |
| Arrow keys | Move between accordion headers |
| `Home` | First header |
| `End` | Last header |

### Toolbar

| Key | Action |
|-----|--------|
| Arrow keys | Move between tools |
| `Tab` | Exit the toolbar entirely |
| `Home` | First tool |
| `End` | Last tool |

Another roving focus pattern. The entire toolbar is one tab stop; arrows navigate within.

### Combobox / Autocomplete

| Key | Action |
|-----|--------|
| Arrow keys | Navigate options in dropdown |
| `Enter` | Select highlighted option |
| `Escape` | Close dropdown / clear |
| Typing | Filter options |

### Tooltip

| Key | Action |
|-----|--------|
| `Escape` | Dismiss the tooltip |

Tooltips appear on focus (not just hover). This is important — hover-only tooltips are inaccessible to keyboard users.

---

## 2. The Roving Focus Pattern

Several widgets share the same pattern: **the group is one tab stop, arrows move within, Tab exits**. These are called "composite widgets."

Afterhours widgets that should use this pattern:
- **Radio groups** — arrows select, Tab exits
- **Tab bars** — arrows switch tab focus, Tab goes to panel
- **Toolbars** — arrows move between tools, Tab exits
- **Menu items** — arrows navigate, Tab exits (or closes)

Widgets that should NOT use roving focus (each item is a separate tab stop):
- **Checkboxes** — each is independent, Tab moves between them
- **Buttons** — each is an independent tab stop
- **Form fields** — each is an independent tab stop

### What to validate

A new validation `enforce_roving_focus` could check:
- Tab groups tagged as "composite" (radio, tabs, toolbar) have exactly one child in the tab order
- Arrow key handlers are registered on composite groups
- Tab key moves focus out of the group, not to the next item within

---

## 3. Focus Management Rules

### Focus return

When a popup/modal/menu closes, focus MUST return to the element that opened it. Without this, keyboard users get lost.

Base UI implements this as `finalFocus` on Dialog. The concept is:
1. Before opening a popup, record `trigger_entity_id`
2. On close, set focus to `trigger_entity_id`

### Focus trapping

While a modal is open, Tab must cycle within the modal. Base UI does this automatically.

For afterhours:
1. When a modal is active, the tab order should be scoped to entities inside the modal
2. Tab on the last focusable element wraps to the first
3. Shift+Tab on the first wraps to the last

### Initial focus

When a dialog opens, focus should move to the first focusable element inside (or a designated element). Don't leave focus on the trigger behind the backdrop.

### What to validate

| Validation | What it checks |
|-----------|---------------|
| `enforce_focus_return` | When a modal/popup closes, focus returns to trigger |
| `enforce_focus_trap` | Tab cycles within active modal, doesn't escape |
| `enforce_initial_focus` | When modal opens, focus moves inside |

---

## 4. WCAG Focus Indicator Requirements

### 2.4.7 Focus Visible (Level AA)

The basic rule: when an element has keyboard focus, there MUST be a visible indicator. It must stay visible the entire time the element is focused (not time-limited).

### 2.4.13 Focus Appearance (Level AAA)

The strict version with measurable criteria:

**Size**: The indicator must be at least as large as a 2px thick perimeter around the element.
- For a 90×30px button: minimum indicator area = `4h + 4w` = `4(30) + 4(90)` = 480px²
- Simplest approach: solid 2px outline
- An inset indicator (not touching the outer edge) needs to be thicker to compensate

**Contrast**: At least 3:1 contrast ratio between the same pixels in focused and unfocused states.
- This measures the CHANGE, not the absolute contrast
- Example: white background → dark outline = measure white-to-dark contrast
- This is different from Non-text Contrast (1.4.11) which measures the indicator against adjacent colors

**Both matter**:
- Focus Appearance (2.4.13) = change-of-contrast between states
- Non-text Contrast (1.4.11) = adjacent-contrast of the indicator against surroundings
- An indicator can pass one but fail the other

**Best practice**: Two-color focus indicator (e.g., dark outline + light inner outline). Works against any background color without needing to compute contrast per-element.

### What to validate

| Validation | What it checks |
|-----------|---------------|
| `enforce_focus_indicator_exists` | Focused elements render a visible ring |
| `enforce_focus_indicator_thickness` | Focus ring is at least 2px |
| `enforce_focus_indicator_contrast` | Focus ring color has 3:1 change-of-contrast vs unfocused state |

afterhours already renders focus rings. The validation would check that the ring actually meets the specs.

---

## 5. APCA Contrast (Replacing WCAG 2 Ratio)

Base UI recommends APCA over WCAG 2: "Unless your application has strict requirements around compliance with current standards, consider adhering to APCA, which is slated to become the new standard in WCAG 3."

### Why WCAG 2 contrast ratio is broken

WCAG 2 uses a simple luminance ratio (4.5:1 for text). Problems:

1. **Overstates contrast for dark colors** — 4.5:1 near black can be functionally unreadable
2. **Can't guide dark mode** — the math is wrong for dark-on-dark, so WCAG 2 is useless for dark themes
3. **Ignores font size and weight** — only has two tiers (normal vs large text), no granularity
4. **86% of websites fail WCAG 2 contrast** — many failures aren't actually inaccessible, the math is just wrong

### How APCA works

APCA outputs a Lightness Contrast (Lc) value on a perceptually uniform scale.

- **Polarity-aware**: light-on-dark produces a different value than dark-on-light (because human perception is asymmetric)
- **Perceptually uniform**: Lc 60 means the same perceived contrast regardless of the absolute lightness of the colors
- **Halving Lc = halving perceived contrast**

### APCA thresholds

| Lc Value | Use Case | Min Font |
|----------|----------|----------|
| **Lc 90** | Preferred for body text columns | 14px / weight 400 |
| **Lc 75** | Minimum for body text columns | 18px / weight 400 |
| **Lc 60** | Minimum for readable content text | 24px/400 or 16px/700 |
| **Lc 45** | Minimum for headlines, large text | 36px/400 or 24px/700 |
| **Lc 30** | Absolute minimum for any text (placeholder, disabled) | Large only |
| **Lc 15** | Minimum for non-text that needs to be discernible | 5px+ smallest dimension |

Below Lc 15: treat as invisible.
For dark mode large fonts: keep contrast below Lc 90 (prevents halation/glare).
For AAA equivalent: add Lc 15 to each level.

### The algorithm

The core is ~50 lines of math:
1. Linearize sRGB: `R_lin = (R/255)^2.4`
2. Compute luminance Y: `Y = 0.2126729*R + 0.7151522*G + 0.0721750*B`
3. Soft clamp near black
4. Compute Lc with polarity-dependent exponents (different for text-lighter-than-bg vs text-darker-than-bg)
5. Scale and offset to final Lc value

### What to add to afterhours

afterhours currently has `colors::contrast_ratio()` using WCAG 2 math and `enforce_contrast_ratio` in validation config with a flat 4.5 threshold.

Proposed additions:

**New function**: `colors::apca_contrast(Color text, Color bg) -> float` — returns Lc value

**New validation config flags**:

```cpp
// === APCA Contrast (Design Rules Section E2) ===
bool enforce_apca_contrast = false;
// These thresholds are checked against font size:
float apca_body_text_min = 75.0f;     // Lc 75 for body text (<18px)
float apca_large_text_min = 60.0f;    // Lc 60 for large text (18px+)
float apca_headline_min = 45.0f;      // Lc 45 for headlines (24px+ bold or 36px+)
float apca_placeholder_min = 30.0f;   // Lc 30 for placeholder/disabled text
float apca_nontext_min = 30.0f;       // Lc 30 for non-text elements
float apca_dark_mode_max = 90.0f;     // Max Lc for large dark-mode text (prevent halation)
```

**Integration**: The existing `ValidateComponentContrast` system already iterates `UIComponent + HasColor + HasLabel` and knows the font size (from `ValidateMinFontSize`). Adding APCA would mean:
1. Compute `apca_contrast(text_color, bg_color)`
2. Look up the appropriate threshold based on font size
3. Report violation if below threshold
4. Optionally also flag dark-mode large text above the max

---

## 6. Summary: What to Build

### New validations for `validation_config.h`

| Flag | Priority | What |
|------|----------|------|
| `enforce_focus_indicator` | High | Focused elements render a visible ring |
| `enforce_focus_indicator_contrast` | High | Ring has 3:1 contrast change |
| `enforce_focus_indicator_thickness` | Medium | Ring is at least 2px thick |
| `enforce_apca_contrast` | High | Text meets Lc threshold for its font size |
| `enforce_focus_return` | High | Focus returns to trigger when modal closes |
| `enforce_focus_trap` | High | Tab cycles within active modal |
| `enforce_initial_focus` | Medium | Focus moves inside modal on open |
| `enforce_escape_dismiss` | Medium | Modals/popups close on Escape |
| `enforce_roving_focus` | Medium | Composite widgets (radio, tabs, toolbar) use arrow-key navigation |
| `enforce_accessible_names` | Medium | Every interactive element has a non-empty label |

### New utility function

`colors::apca_contrast(Color text, Color bg) -> float` — APCA Lc value

### New component behaviors to verify

| Widget | Expected Keyboard | Currently Implemented? |
|--------|-------------------|----------------------|
| Button | Enter/Space activate | Likely yes |
| Checkbox | Space toggles | Check |
| Radio group | Arrows select, Tab exits | Probably not roving |
| Tabs | Arrows switch, Tab to panel | Probably not roving |
| Slider | Arrows adjust, Home/End min/max | Arrows maybe, Home/End probably not |
| Modal | Escape closes, focus trapped, focus return | Escape maybe, trap/return probably not |
| Menu | Arrows navigate, Enter activates, Escape closes, type-ahead | Partial |
| Tooltip | Shows on focus, Escape dismisses | Check |

---

## References

- Base UI llms.txt: https://base-ui.com/llms.txt
- Base UI Accessibility: https://base-ui.com/react/overview/accessibility.md
- WAI-ARIA APG: https://www.w3.org/WAI/ARIA/apg/
- WAI-ARIA APG Patterns: https://www.w3.org/WAI/ARIA/apg/patterns/
- WCAG 2.4.7 Focus Visible: https://www.w3.org/WAI/WCAG22/Understanding/focus-visible.html
- WCAG 2.4.13 Focus Appearance: https://www.w3.org/WAI/WCAG22/Understanding/focus-appearance.html
- WCAG 1.4.11 Non-text Contrast: https://www.w3.org/WAI/WCAG22/Understanding/non-text-contrast.html
- APCA Calculator: https://apcacontrast.com
- APCA Why: https://git.apcacontrast.com/documentation/WhyAPCA
- APCA Readability Criterion: https://readtech.org/ARC/
