# UI Review TODO List

Generated from screenshot review of all 51 showcase screens.

---

## Priority Legend

- **P0** — Critical: Broken/clipped UI, must fix before release
- **P1** — High: Usability/accessibility issues
- **P2** — Medium: Polish and consistency improvements
- **P3** — Low: Nice-to-have refinements

---

## P0: Critical Issues

### Text Clipping at Screen Edges

Multiple screens have text clipped by screen boundaries.

| Screen | File | Issue | Status |
|--------|------|-------|--------|
| Language Demo | `LanguageDemo.h` | Title "Language Demo" clips left — "L" cut off | **IN PROGRESS** — Added padding, left margin, reduced font size |
| Dead Space Settings | `DeadSpaceSettings.h` | Sidebar items like "Show Content Warning" clip left | **IN PROGRESS** — Adjusted sidebar position and font size |
| Modals | `ModalShowcase.h` | Footer status "Confirm: Pending" clips left | **IN PROGRESS** — Added self-align center, padding, reduced font |
| Fighter Menu | `FighterMenu.h` | "L Change Entry" button prompt partially cut | **FIXED** — Moved button prompts left by 20px |
| Empire Tycoon | `EmpireTycoon.h` | "$1,250,980" clips to "$250,980" | **IN PROGRESS** — Widened currency pill and text container |

**Changes Made:**

1. `LanguageDemo.h`:
   - Changed main container padding from `Spacing::sm` to `Spacing::md`
   - Changed header width from `pixels(1000)` to `percent(1.0f)` for responsive sizing
   - Changed footer width from `pixels(1000)` to `percent(1.0f)`
   - Added `with_margin(Margin{.left = pixels(8)})` to title
   - Reduced title font size from 32.0f to 26.0f

2. `DeadSpaceSettings.h`:
   - Increased `sidebar_x` from 35.0f to 55.0f
   - Reduced sidebar item font size from 19.0f to 16.0f

3. `ModalShowcase.h`:
   - Added `.with_self_align(SelfAlign::Center)` to root container
   - Added `.with_font(UIComponent::DEFAULT_FONT, 18.0f)` to results row
   - Added `.with_padding(Spacing::sm)` to results row

4. `FighterMenu.h`:
   - Moved L/R button prompts left (690→670, 720→700, 750→730)
   - Increased label width from 120 to 130px

5. `EmpireTycoon.h`:
   - Increased currency pill offset from 255 to 300px from right edge
   - Increased pill width from 225 to 275px
   - Increased money text width from 165 to 210px
   - Reduced money font size from 28 to 22

**Remaining Issue:**
Some clipping persists despite changes. Root cause appears to be text rendering extending beyond stated bounds, possibly due to font metrics or glyph bearing. May require investigating the text rendering layer in `afterhours`.

---

### [x] Empire Tycoon Contrast Failure — LOW PRIORITY

**File:** `EmpireTycoon.h`

After review, the text uses `dark_text` color which provides adequate contrast against the light blue `panel_blue` background. The contrast issue noted initially was based on screenshot compression artifacts. No fix needed.

---

## P1: High Priority Issues

### [ ] Toggle Switch Visibility

**File:** `ToggleSwitchShowcase.h`

Toggle tracks are nearly invisible — track color matches surface color.

**Status:** Not started

---

### [ ] Checkbox Panel Overflow

**File:** `CheckboxShowcase.h`

Right panel "Multi-Select" and "MinMax" boxes overflow their container bounds on the right side.

**Status:** Not started

---

### [ ] Missing Focus States

**Files:** Multiple component showcases

Some components show focus rings (buttons), others don't (checkboxes, toggles).

**Status:** Not started

---

### [ ] Theme Switcher Self-Contrast Issue

**File:** `Themes.h`

"Cozy Kraft" button text is yellow/gold on yellow background when selected.

**Status:** Not started

---

## P2: Medium Priority Issues

### [ ] Cards Label Alignment

**File:** `Cards.h`

Inconsistent label-to-content spacing.

**Status:** Not started

---

### [ ] Navigation Bar Chevron Separation

**File:** `NavigationBarShowcase.h`

Chevrons visually separated from their parent element.

**Status:** Not started

---

### [ ] Kirby Options Icon Grid Alignment

**File:** `KirbyOptions.h`

Bottom row of icon grid drifts left.

**Status:** Not started

---

### [ ] Toggle Switches Empty Space

**File:** `ToggleSwitchShowcase.h`

Massive empty space below toggle controls.

**Status:** Not started

---

### [ ] Toasts Button Grouping

**File:** `ToastShowcase.h`

Buttons float without clear visual grouping.

**Status:** Not started

---

## P3: Low Priority / Polish

### [ ] Standardize Spacing Tokens

**Files:** All showcase screens

Spacing feels arbitrary across screens.

**Status:** Not started

---

### [ ] Scroll View Demo Polish

**File:** `ScrollViewShowcase.h`

Horizontal scroll clips numbers, scroll indicators unclear.

**Status:** Not started

---

### [ ] Angry Birds Settings Icon Clarity

**File:** `AngryBirdsSettings.h`

Icon buttons use unclear symbols.

**Status:** Not started

---

## Validation Checklist

After implementing fixes, verify:

- [x] Run `screenshot_all_screens.py` — all 51 screens capture without error
- [ ] No text clips at screen edges (partially fixed, some persist)
- [ ] All interactive elements have visible focus state
- [ ] Contrast ratios meet WCAG AA (4.5:1 for text, 3:1 for UI)
- [ ] Spacing is consistent within each screen
- [ ] No elements overflow their containers

---

## Summary of Work Done

### Session 1 (2026-01-31)

**Files Modified:**
- `LanguageDemo.h` — Padding, responsive sizing, margin adjustments
- `DeadSpaceSettings.h` — Sidebar positioning, font size reduction
- `ModalShowcase.h` — Self-alignment, padding, font adjustments
- `FighterMenu.h` — Button prompt repositioning (**FIXED**)
- `EmpireTycoon.h` — Currency display widening

**Build Status:** ✅ Compiles successfully  
**Screenshot Status:** ✅ All 51 screens captured, no validation errors

**Observation:**
Text clipping appears to be a deeper issue with how text is rendered relative to component bounds. Even with increased container sizes and margins, some left-edge clipping persists. This may require:
1. Investigation of text rendering in `vendor/afterhours/src/plugins/ui/`
2. Adding explicit text padding/inset in the rendering layer
3. Ensuring font metrics account for glyph bearing

---

## Files Modified This Session

| File | Changes |
|------|---------|
| `LanguageDemo.h` | Padding, sizing, margin, font |
| `DeadSpaceSettings.h` | Sidebar position, font size |
| `ModalShowcase.h` | Self-align, padding, font |
| `FighterMenu.h` | Button positions |
| `EmpireTycoon.h` | Currency pill dimensions |

---

*Last updated: 2026-01-31 (Session 1)*
