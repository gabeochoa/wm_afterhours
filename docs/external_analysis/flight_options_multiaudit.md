# Consolidated Design Audit: flight_options

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Disabled "Vibration (Unavailable)" item lacks clear disabled styling
**Problem:** The Vibration setting appears grayed out with "Requires compatible controller" text, but its styling is only slightly dimmer than active items. The disabled state isn't immediately obvious — it could be mistaken for a less-important setting.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Use a clearly distinct disabled style — strikethrough text, reduced opacity to 50%, or a different background color. Add an icon indicating unavailability (lock icon or similar).

### 2. Left sidebar categories have inconsistent text sizing with active item
**Problem:** "FLIGHT SYSTEM" is highlighted with a border and appears selected, but the other category items (CONTROLS, KEYBOARD, etc.) use the same font size with dash prefixes. The hierarchy between selected and unselected isn't strong enough.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Make the selected category visually dominant — bold text, brighter color, left accent bar, and slightly larger or different weight. Dim unselected items more.

---

## Agreed by 3/4 auditors

### 3. Decorative horizontal line at top has no clear purpose
**Problem:** There's a thin blue line with a dot at the top-left, between "OPTIONS" and the category list. It appears decorative but breaks the visual flow without serving a structural purpose.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Remove the decorative line or repurpose it as a meaningful progress/section indicator. If it's a breadcrumb, make that function clear.

### 4. Right-side settings buttons lack visible boundaries
**Problem:** The flight system settings (Control Type, High-G Turn, etc.) are styled as filled bars but blend together with minimal spacing. It's hard to tell where one button ends and the next begins.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Add more vertical spacing between items (8px gaps), or use subtle borders/dividers to clearly separate each setting row.

---

## Agreed by 2/4 auditors

### 5. Bottom button prompt bar is sparse and left-aligned
**Problem:** "Enter OK" and "Esc BACK" are left-aligned at the bottom with a lot of empty space to the right. The help text above ("Choose between standard or expert control schemes") is also sparse.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Center the button prompts or right-align them consistently. Add more descriptive help text for the selected setting.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
