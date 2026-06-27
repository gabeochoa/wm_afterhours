# Consolidated Design Audit: Full UI Showcase

**Screens audited:** 20 representative screens (of 76 total)
**Screenshots reviewed:** buttons, forms, text_input, checkboxes, toggle_switches, radio_buttons, dropdowns, modals, cards, navigation_bar_demo, tab_container, toasts, colors, layout, cozy_cafe, scroll_view, themes, text, shop_interface, fighter_menu
**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95, Muskox Craft, Afterhours Style, Accessibility, Atlassian Design

---

## Triage Status

| # | Issue | Triage | Notes |
|---|-------|--------|-------|
| 1 | Checkmarks as "V" | **Yes — fix** | Need real icon glyphs built into afterhours |
| 2 | Text contrast on dark backgrounds | **Investigate** | May be intentional per-theme but unclear |
| 3 | Text truncation/overflow | **Yes — fix** | Real bugs |
| 4 | Corner radii inconsistent within single screens | **Yes — fix** | Cross-theme variation is fine; within-screen mismatches are not |
| 5 | Spacing not on consistent grid | **Mostly fix** | Some screens may be intentional, most should follow grid |
| 6 | Type scale inconsistency | **Think about** | Worth defining a clearer system |
| 7 | Touch target validation | **Add validation** | Not fixing now, but add `enforce_min_touch_target` for future |

Items removed from previous report (intentional/not-issues): focus indicators (skip for now, likely just not triggered in screenshots), button hierarchy in showcase screens (intentional demos), centered form labels (intentional), disabled states (per-screen designer choice), elevation/shadows (theme-dependent), container nesting (depends on screen), color-only meaning (fine), different themes across screens (on purpose), handwritten/monospace fonts (on purpose), ALL CAPS headers (on purpose).

---

## 1. Checkmarks rendered as "V" instead of proper glyphs

**Triage: Yes — fix**

**Problem:** Across checkboxes, toggle switches, and dropdowns, the selected/checked state is indicated by a capital letter "V" rather than a proper checkmark character (✓). This appears in checkboxes_720p, toggle_switches_720p, forms_720p, and dropdowns_720p. Dropdown indicators also use "v" instead of a chevron/caret.

**Screens affected:** checkboxes, toggle_switches, forms, dropdowns (systemic — any component using a check/arrow indicator)

**Fix:** Build real icon glyphs into afterhours — checkmark (✓), chevron down (▾), chevron left/right (‹ ›), radio dot, etc. Either embed vector glyphs or ship a minimal icon font. This is a framework-level fix, not per-screen.

---

## 2. Text contrast unclear on some dark-themed screens

**Triage: Investigate**

**Problem:** Some dark-themed screens have text that may have insufficient contrast. In buttons_720p, dark blue text labels on dark navy backgrounds are hard to read. In forms_720p, teal headers on dark backgrounds have marginal contrast. The concern is whether specific text/background combinations actually fail WCAG AA thresholds (4.5:1 body, 3:1 large text).

**Screens affected:** buttons, forms, modals, navigation_bar_demo

**Next step:** Run the existing `enforce_contrast_ratio` validation on these screens programmatically to get actual contrast ratios rather than eyeballing. If specific combinations fail, fix them. If they pass, this is fine.

---

## 3. Text truncation and overflow

**Triage: Yes — fix**

**Problem:** Text is clipped or overflows its container in multiple screens:
- **cozy_cafe_720p:** "Patience:" text overlaps with progress bar and percentage for both customers
- **fighter_menu_720p:** "Configure vibration, save data, and..." description cut off
- **cards_720p:** "This panel uses opacity to create a translucen..." truncated
- **text_720p:** "Body text for readable content that flows natu..." cut off
- **forms_720p:** clipping at right edge of settings column

**Fix:** Per-screen fixes — widen containers, enable word wrap, or use proper ellipsis handling. The cozy_cafe patience overlap is the most visible real bug. Run `enforce_child_containment` validation to catch more instances across all 76 screens.

---

## 4. Corner radii inconsistent within single screens

**Triage: Yes — fix (within-screen only)**

**Problem:** Within a single screen, components at the same hierarchy level use different radii. Cross-theme variation is fine (each theme can define its own radius style), but within one screen the system should be consistent.

**Examples of within-screen mismatches:**
- **cards_720p:** Outer card has ~16px radius, child elements have sharp corners or inconsistent small radii
- **dropdowns_720p:** Some dropdown triggers are rounded, others within the same grid are sharper
- **tab_container_720p:** Tab panel has very large radius but tabs themselves have smaller/different radius

**Fix:** Each theme should define a radius scale, and the showcase screens for that theme should apply it consistently. Audit each screen individually against its own theme's radius values. Apply the inset formula (inner = outer - padding) for nested rounded corners.

---

## 5. Spacing not on consistent grid

**Triage: Mostly fix**

**Problem:** Gaps between similar elements vary within individual screens:
- **buttons_720p:** Gaps between button rows are unequal
- **checkboxes_720p:** Spacing between items differs left vs right column
- **text_input_720p:** Vertical spacing between label/field pairs varies
- **radio_buttons_720p:** Internal spacing differs between the three groups

**Fix:** For screens where spacing inconsistency is clearly unintentional, normalize to the nearest value on the 4/8px grid. Some screens (particularly game UIs like cozy_cafe, shop_interface) may have intentional custom spacing that shouldn't be normalized. Focus on the component showcase screens first.

---

## 6. Type scale — worth defining more clearly

**Triage: Think about**

**Problem:** Font sizes don't follow a clear mathematical progression. Same logical hierarchy level (e.g., section heading) appears at different sizes across screens, even within the same theme. In shop_interface, item names/categories/prices are at roughly the same size, flattening the hierarchy.

**Next step:** Not an immediate fix. Consider defining a formal type scale per theme (e.g., minor third 1.2× ratio) and documenting expected sizes for each semantic level. This would make it easier for screen designers to pick correct sizes.

---

## 7. Touch target size — add validation for the future

**Triage: Add validation, don't fix now**

**Problem:** Some interactive elements (radio button circles, small checkbox squares, dropdown arrows) appear to have visual sizes below 44×44px. The hit area may be larger than the visual, but there's no way to verify from screenshots alone.

**Next step:** Implement `enforce_min_touch_target` in `validation_config.h` so this can be checked programmatically. Flag interactive elements where the actual hit rect is smaller than 44×44px. This was already proposed in the a11y checklist mapping document.

---

## 8. Text input cursor positioning

**Triage: Investigate / fix**

**Problem:** Cursor (caret) positioning in text input fields may be incorrect. Visible on the text_input_720p screen. The cursor position calculation in `text_input/component.h` relies on `position_text_ex` for font measurement and uses a hardcoded `TEXT_MARGIN` of 5px. Potential issues include cursor drift when typing, incorrect initial position, or misalignment between where the cursor renders and where text actually appears.

**Screens affected:** text_input (and any screen using `text_input()`)

**Next step:** Investigate the cursor positioning code in `src/plugins/ui/text_input/component.h` lines 152-221. Test by typing in text fields and checking whether the cursor tracks correctly with the text. Known code concerns: 8px cursor width may be too thick at some resolutions, `screen_height` defaults to 720 if singleton not found, `position_text_ex` font size auto-shrinking may desync from cursor measurement.

---

## Summary

| Priority | Count | Items |
|----------|-------|-------|
| **Fix** | 3 | #1 (checkmark glyphs), #3 (text overflow), #4 (within-screen radii) |
| **Mostly fix** | 1 | #5 (spacing grid) |
| **Investigate** | 2 | #2 (contrast on dark themes), #8 (text input cursor positioning) |
| **Think about** | 1 | #6 (type scale) |
| **Add validation** | 1 | #7 (touch targets) |
