# ComponentConfig Issues

**Status:** Partially implemented  
**Priority:** High  
**Source:** break_ross migration experience

---

## Completed

### Config Conflict Detection Validators (Section A)

Three new runtime validators added to `validation_systems.h`, each with a corresponding
`enforce_*` flag in `ValidationConfig` (off by default, enabled in dev/strict presets):

1. **ValidateZeroSize** — Flags elements that resolve to 0 width or height
   (common with `percent(1.0)` when parent has no explicit size, or `children()` with no children).
   Flag: `enforce_zero_size_detection`. Severity: 0.8.

2. **ValidateAbsoluteMarginConflict** — Flags absolute-positioned elements with non-zero margins.
   On absolute elements, margins are position offsets (not spacing), which is often unintended.
   Flag: `enforce_absolute_margin_conflict`. Severity: 0.6.

3. **ValidateLabelHasFont** — Flags elements with a `HasLabel` component but `font_name == UNSET_FONT`.
   These elements may not render text. Flag: `enforce_label_has_font`. Severity: 0.9.

### Missing Validators for Existing Flags (Section B)

Two validators implemented for flags that already existed in `ValidationConfig` but had
no corresponding system:

4. **ValidateSpacingRhythm** — Checks that computed margins and padding are on a 4px rhythm
   grid (multiples of 4). Flag: `enforce_spacing_rhythm`. Severity: 0.4.

5. **ValidatePixelAlignment** — Checks that computed element positions are pixel-aligned
   (no fractional coordinates that cause blurry rendering). Flag: `enforce_pixel_alignment`.
   Severity: 0.3.

### Registration & Configuration

- All 5 validators registered in both `register_update_systems()` and `register_all_systems()`.
- `enable_development_mode()` enables the 3 config-conflict validators.
- `enable_strict_mode()` enables all 5 validators.
- `game.cpp` enables the 3 config-conflict validators in its validation setup.

---

## Remaining

### Text Elements Requiring Background Colors

Text elements with `.with_label()` may not render correctly without an explicit background
color. The rendering system checks for `HasColor` OR `HasLabel`, but text elements sometimes
need a background color. Consider automatically adding a transparent background to text-only
elements, or documenting the requirement clearly.

### Button Disabled State Documentation

`.with_disabled()` works but behavior isn't well-documented:
- Disabled buttons use `Theme::Usage::DisabledForeground` and `DisabledBackground`
- Disabled buttons don't respond to hover or click
- Focus can still move to disabled buttons (for accessibility)
- Consider adding `with_disabled_style()` for customization

### Example Screen: ConfigValidationShowcase

A showcase screen (`--screen=config_validation`) could demonstrate edge cases:
- Text without background rendering
- Disabled button states and interaction blocking
- Invalid config combos triggering validation warnings
- Helper functions producing warning-free elements
- Scrollable validation log at the bottom

---
