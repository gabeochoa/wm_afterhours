# Consolidated Design Audit: casual_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Toggle states rely solely on color with no secondary indicator
**Problem:** Music/Sound/Vibrate toggles use green for ON and brown/gray for OFF. Color is the only way to distinguish states. No icon, label suffix, or shape change reinforces the state.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Add text labels directly on each toggle showing "ON"/"OFF", or use a toggle switch component with a clear sliding indicator. Ensure 4.5:1 contrast on all text.

### 2. Close button (X) overlaps panel boundary
**Problem:** The red circular X button extends beyond the panel's top-right corner, floating partially outside the cream panel. This breaks containment and looks unintentional.
**Flagged by:** Apple (#2), Google (#2), Sun (#3), Microsoft (#2)
**Fix:** Place the close button fully inside the panel header with consistent padding, or make it a clear part of the header bar.

---

## Agreed by 3/4 auditors

### 3. Two-column button layout has inconsistent widths
**Problem:** Left buttons (Notifications, Language) are narrower than right buttons (Credits, Support, Terms and Privacy). The columns don't align to a shared grid.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Use consistent button widths across both columns. Align to a grid system with equal column widths.

### 4. WiFi icon button is too small and cryptic
**Problem:** The small icon button next to "Save/Load Progress" uses a WiFi-like symbol with no label. Its function is unclear and the touch target is small.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Add a text label or tooltip ("Sync"), increase size to minimum 44px, or combine with Save/Load into a single clearly-labeled button.

---

## Agreed by 2/4 auditors

### 5. "About" button is visually orphaned at the bottom-left
**Problem:** The About button sits alone at the bottom-left with the version text, disconnected from the main button groups. It uses a different style (outline vs filled).
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Group About with the other secondary buttons (Credits, Support) or place it in a consistent footer row.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
