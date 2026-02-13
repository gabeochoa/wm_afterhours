# Consolidated Design Audit: cards

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. "Styles:" label is tiny and barely readable
**Problem:** The "Styles:" and "Nested:" section labels are rendered in extremely small text with a dark background. They're hard to read and don't serve as effective section headers.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Increase section label font size to match heading hierarchy. Use a clear header style — larger text, different weight, or color accent.

### 2. Cards have very similar styling making variants hard to compare
**Problem:** Surface, Primary, Sharp, and Custom cards all look similar — light backgrounds with subtle borders. The visual differences between card types are too subtle to demonstrate the variant system effectively.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Exaggerate the differences for showcase purposes — Sharp should have clearly squared corners, Custom should use a noticeably different color, Primary should be visibly darker or have an accent.

---

## Agreed by 3/4 auditors

### 3. Semi-Transparent card description text is very small
**Problem:** "This panel uses opacity to create a translucent overlay effect." is rendered in tiny text inside the Semi-Transparent card. The text is important for understanding the demo but hard to read.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Increase description text size. The card description is the key content — make it readable.

### 4. Theme Colors row at bottom is cramped
**Problem:** The "Theme Colors:" row packs six color swatches (Primary, Secondary, Accent, Surface, Background) tightly together. The swatches are small and labels overlap.
**Flagged by:** Apple (#4), Sun (#4), Microsoft (#4)
**Fix:** Increase spacing between swatches. Use larger sample rectangles with labels clearly below or beside each.

---

## Agreed by 2/4 auditors

### 5. No interactive demonstration of card behaviors
**Problem:** Cards are shown as static elements. A gallery should demonstrate clickable cards, expandable cards, or cards with hover states.
**Flagged by:** Google (#5), Sun (#5)
**Fix:** Add at least one interactive card that responds to clicks or hover to demonstrate the component's interactive capabilities.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
