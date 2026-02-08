# Consolidated Design Audit: kirby_options

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Option category icons have inconsistent sizes and visual weight
**Problem:** Controls (large green), Display (medium green), Accessibility (small, faded) are all at the same hierarchy level but have dramatically different sizes and opacities. Accessibility looks disabled or less important rather than equal.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Make all category icons the same size. Use color to differentiate but keep size and opacity consistent. Accessibility is critical and should never appear diminished.

### 2. Tab bar icons are cryptic and lack labels on most items
**Problem:** The top tab bar uses small colored icons (WiFi, Control, Home, Mail, Star, Gear) with L/R bumpers. Only "Gear" has a label underneath ("Options"). Most icons don't clearly convey their function.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Add text labels below all tab icons, or use more universally recognizable icons. Ensure each tab's function is clear without memorization.

---

## Agreed by 3/4 auditors

### 3. "Name" field with "@" icon has unclear purpose
**Problem:** The yellow "@ Name" field at the top of the options panel looks like a text input but its purpose isn't clear. Is it a username? A profile name? The "@" prefix adds confusion.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Add a descriptive label above the field ("Player Name" or "Profile"). Remove the "@" prefix or explain its meaning. Use standard text field styling with a visible border.

### 4. Edit/Erase buttons are small and isolated
**Problem:** The "Edit" and "Erase" buttons in the top-left are small, plain-styled, and disconnected from the content they affect. Their relationship to the Name field or options grid isn't obvious.
**Flagged by:** Apple (#4), Sun (#4), Microsoft (#4)
**Fix:** Position Edit/Erase closer to the element they control (the Name field). Use consistent button styling and increase size to meet touch targets.

---

## Agreed by 2/4 auditors

### 5. "Common" group label is ambiguous
**Problem:** The "Common" label above the first row of option icons (Controls, Display, Accessibility) is vague. Common to what? All games? Frequently used? The grouping logic isn't clear.
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Use a more descriptive group label like "General Settings" or "Frequently Used". Or remove the label if all items are at the same level.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
