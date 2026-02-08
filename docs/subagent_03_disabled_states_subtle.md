# Subagent 3: Disabled States Too Subtle

**Severity:** Critical (4/4 auditor agreement)
**Affected screens:** 10+

---

## Problem

Components showing disabled variants (buttons, checkboxes, toggles, sliders, text inputs) use barely-perceptible styling changes. Users can't quickly distinguish disabled elements from enabled ones, leading to confusion and wasted interaction attempts.

## Affected Screens & Specific Locations

| Screen | Element | Current Disabled Style | Fix |
|--------|---------|----------------------|-----|
| `ButtonVariantsShowcase.h` | Filled Disabled | Grayed fill, text readable | Reduce opacity to 50%; gray text |
| `ButtonVariantsShowcase.h` | Outline Disabled | Faded text, no border change | Add dashed/dimmer border; 50% opacity |
| `ButtonVariantsShowcase.h` | Ghost Disabled | Italic text | Not sufficient — use 50% opacity + gray |
| `Buttons.h` | "Disabled" button | Slightly gray | More aggressively gray; 50% opacity |
| `CheckboxShowcase.h` | Disabled ON / Disabled OFF | Slightly lighter, similar to active | 50% opacity; gray checkmark |
| `Forms.h` | Disabled checkbox | Lighter gray, looks active | 50% opacity; cursor change |
| `FlightOptions.h` | "Vibration (Unavailable)" | Slightly dimmer text | Add lock icon; reduce to 50% opacity; strikethrough |
| `PaginationShowcase.h` | Disabled page buttons | Subtle gray | 50% opacity |

## Vendor-Level Fix Opportunities

### 1. Global disabled opacity (HIGH IMPACT)

Currently `with_disabled()` in `component_config.h` sets a flag but doesn't enforce visual styling. The rendering is screen-specific.

**Proposed vendor change:** Add a global disabled opacity multiplier in the rendering pipeline. When `is_disabled` is true, automatically apply 50% opacity to the entire component subtree.

```cpp
// In component_config.h or rendering
if (config.is_disabled) {
    // Apply 50% opacity overlay
    // Optionally add "strikethrough" or "grayed" filter
}
```

### 2. Disabled cursor style

Currently there's no cursor change for disabled elements. If the UI supports cursors, disabled elements should show `not-allowed`.

### 3. Disabled interaction blocking

Verify that `with_disabled()` actually blocks clicks/hovers in all component types, not just buttons.

## Screen-Level Fixes (if vendor change isn't done)

Each screen would need to manually set reduced opacity or distinct colors for disabled elements:

```cpp
// Example per-screen fix
auto disabled_config = ComponentConfig()
    .with_disabled()
    .with_custom_background(Color(128, 128, 128, 128))  // 50% gray
    .with_font_color(Color(180, 180, 180, 180));         // Dimmed text
```

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P0** | Vendor: Add automatic 50% opacity for disabled components | Fixes ALL screens at once |
| **P1** | FlightOptions: Add lock icon + stronger disabled style | Single screen, high visibility |
| **P2** | Showcase screens: Ensure disabled examples are clearly different | Gallery quality |

## Estimated Effort

- Vendor opacity change: ~10-15 lines in rendering code
- Per-screen fixes (if no vendor change): ~3-5 lines each × 8 screens = ~30 lines
- **Recommended: vendor change only (~15 lines), fixes everything**
