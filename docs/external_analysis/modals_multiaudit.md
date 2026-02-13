# Consolidated Design Audit: modals

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Section headers (BASIC MODALS, HELPER FUNCTIONS, etc.) are too small
**Problem:** The all-caps section headers use very small text on teal bars. They're hard to read and don't create strong visual hierarchy — the section headers should be the most prominent non-title elements.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Increase section header font size. Use a clear heading style — larger, bolder, or with more padding in the header bar.

### 2. Showcase only shows trigger buttons, not the actual modals
**Problem:** The screen shows buttons to open modals but doesn't show what the modals look like. As a gallery showcase, users need to see the modal designs without clicking — consider showing a preview or inline example.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Show at least one modal inline (always open) as a visual preview. Or include annotated screenshots of each modal type within the gallery.

---

## Agreed by 3/4 auditors

### 3. "Confirmation" button uses a distinctly different color (red/pink) without explanation
**Problem:** Among the Helper Functions, "Confirmation" is styled in pink/red while "Info Dialog" is teal and "Notice" is purple. The color choices aren't explained — does red mean destructive? Warning?
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Add color annotations or labels explaining the semantic meaning ("destructive", "informational", "notice"). Or use consistent color for all trigger buttons.

### 4. Results bar at bottom is dense and hard to parse
**Problem:** "Confirm: Pending | FYI: Pending | Confirms: 0  Cancels: 0" packs too much state information into one line with pipe separators. It looks like debug output.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Break into multiple lines or use a small table/grid format. Label each metric clearly and use color coding for status.

---

## Agreed by 2/4 auditors

### 5. "BG Clicks: 0" counter is cryptic
**Problem:** "BG Clicks: 0" presumably tracks background clicks for the input blocking test, but "BG" is an abbreviation that isn't self-explanatory.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Use full text: "Background Clicks: 0" or add a description of what this counter demonstrates.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
