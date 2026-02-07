# Bugs & Polish Batch — Design

**Date:** 2026-02-06
**Goal:** Fix root-cause bugs in the Afterhours library and resolve all P0/P1 visual issues in the showcase before adding new features.

**Ordering:** Bottom-up. Library fixes first, then showcase visual fixes.

---

## 1. Render System Const Constraint (#05)

**Problem:** `SystemManager::render()` casts everything to `const`, preventing render systems from modifying component state. This breaks the immediate-mode UI pattern where rendering and state updates are interleaved.

**Fix:** Remove the const path from `render()` entirely. Make it work the same way as `tick()` — no const casting, no separate dispatch paths. There is no practical benefit to const render systems in an immediate-mode UI framework.

**Changes:**
- `vendor/afterhours/src/core/system.h`: In `render()`, remove the `const SystemBase&` and `const Entity&` casts. Use the same non-const dispatch as `tick()`.
- `render_all()`: Change from `get_entities()` to `get_entities_for_mod()`.
- Remove existing `const_cast` workarounds from render systems.
- Remove unnecessary `mutable` member annotations from `RenderImm`, `RenderSprites`, etc.

---

## 2. Absolute Positioning Fix (#52)

**Problem:** Margins on absolute-positioned elements cause negative sizes because the autolayout subtracts margins from element size. Elements silently become invisible.

**Fix:** CSS-like behavior — margins on absolute elements become position offsets only, they do not affect size.

**Changes:**
- `vendor/afterhours/src/plugins/ui/ui_core_components.h`, `rect()` method (lines 111-119): When `absolute == true`, skip the margin subtraction from width/height. Margins still offset position (left/top).
- Add debug warning when any element's computed size is negative (catch remaining edge cases).
- Include translate values in debug position logging so absolute elements show their actual rendered position.

---

## 3. Component Config Issues (#55)

### 3a. Text elements needing background colors

**Problem:** Text-only elements may not render correctly without an explicit background color.

**Fix:** In the text rendering path, if an element has a label but no `HasColor` component, treat it as having a transparent background automatically. Users should not need `.with_custom_color(Color{0,0,0,0})`.

### 3b. Disabled button state

**Problem:** `.with_disabled()` exists but visual behavior is unclear.

**Fix:** Ensure disabled buttons automatically get grayed-out appearance (alpha multiplier or theme disabled colors). Verify disabled buttons don't trigger click handlers.

### 3c. Invalid property combination warnings

**Problem:** Some property combinations silently produce broken UI.

**Fix:** Add debug-mode validation warnings for:
- `with_margin()` + `with_absolute_position()` (covered by #2 fix, but still warn)
- `fill_parent()` with a parent that has no explicit size
- Text element with label but no font specified

---

## 4. P0: Text Clipping Root Cause

**Problem:** Text clips at screen edges even after container padding adjustments. Root cause is a hardcoded `TEXT_LEFT_BEARING_OFFSET = 8.0f` pixel approximation that doesn't match actual glyph metrics.

**Fix:** Replace the hardcoded offset with actual per-character bearing from Raylib's `GetGlyphInfo()` API. Query the first character's `offsetX` for accurate left-side bearing.

**Changes (3 locations):**
- `vendor/afterhours/src/plugins/ui/rendering.h`, `position_text_ex()` left-aligned path (~line 376)
- `vendor/afterhours/src/plugins/ui/rendering.h`, `position_text_ex()` center-aligned path (~line 384)
- `vendor/afterhours/src/plugins/ui/render_primitives.h`, batch rendering (~line 615)

---

## 5. P1: Toggle Switch Visibility

**Problem:** Toggle track color matches surface color, making tracks nearly invisible.

**Fix:** Adjust toggle track color to use a distinct background that contrasts with the surface. Check both on/off states.

---

## 6. P1: Checkbox Panel Overflow

**Problem:** Right panel "Multi-Select" and "MinMax" boxes overflow container bounds.

**Fix:** Adjust checkbox showcase panels to use proper sizing (percentage or fill_parent instead of pixel values that exceed container).

---

## 7. P1: Missing Focus States

**Problem:** Some components show focus rings (buttons), others don't (checkboxes, toggles).

**Fix:** Audit interactive component rendering and ensure all focusable components render a visible focus indicator when focused.

---

## 8. P1: Theme Switcher Contrast

**Problem:** "Cozy Kraft" button text is yellow/gold on yellow background when selected.

**Fix:** Use the existing auto-text-color system to ensure selected button text contrasts with its background.

---

## Implementation Order

1. **Render const fix** (#05) — unblocks everything, simplest change
2. **Absolute positioning fix** (#52) — fixes silent layout bugs
3. **Text clipping root cause** (P0) — fixes the most visible issue
4. **Component config issues** (#55) — API/DX improvements
5. **P1 visual fixes** (toggle, checkbox, focus, theme) — polish pass

---

## Verification

After all fixes:
- Build compiles without warnings
- All 60+ showcase screens render without text clipping
- No elements overflow containers
- All interactive elements show focus states
- Contrast meets WCAG AA (4.5:1 text, 3:1 UI)
- Existing tests pass
- Take new baseline screenshots
