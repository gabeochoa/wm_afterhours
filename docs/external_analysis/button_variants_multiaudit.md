# Consolidated Design Audit: button_variants

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Outline and Ghost button rows lack visible button boundaries
**Problem:** The Outline row shows text-only buttons with no visible border or background. The Ghost row is even more minimal. Without hovering, users can't tell these are interactive buttons — they look like plain text labels.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Outline buttons should have a visible 1-2px border at rest (not just on hover). Ghost buttons should have at least a subtle underline or different text color to indicate interactivity.

### 2. Disabled states across all three variants are inconsistent
**Problem:** Filled Disabled uses a grayed-out filled style, Outline Disabled uses faded text, Ghost Disabled uses italic text. Three different approaches to the same concept.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Use a consistent disabled pattern across all variants — same opacity reduction (50%), same text color (gray), and same cursor behavior.

---

## Agreed by 3/4 auditors

### 3. "Lg Outline" size example is text-only with no visible button
**Problem:** In the Sizes row, "Lg Outline" appears as plain text with no border, background, or button styling. It looks like a label rather than a large outline button demonstration.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Add a visible outline border to make it clearly a button. It should match the Outline variant styling at the Large size.

### 4. Theme cards at bottom are cramped and hard to compare
**Problem:** The three theme preview cards (Neon Dark, Cozy Kraft, Sage) are small and tightly packed. The button samples inside each card are tiny and hard to evaluate visually.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Increase card size or spread them out. Show larger button samples so the theme differences are clearly visible.

---

## Agreed by 2/4 auditors

### 5. No hover/focus/pressed state demonstrations
**Problem:** The gallery shows rest states only. A button variants showcase should demonstrate all interaction states to serve as a proper reference.
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Add an interactive section or annotations showing hover, focus, and pressed states for each variant.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
