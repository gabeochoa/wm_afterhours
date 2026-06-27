# Consolidated Design Audit: deadspace_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Low contrast text throughout — teal on dark gray is hard to read
**Problem:** The main settings list uses teal/cyan text on a near-black background. While atmospheric, the contrast ratio is borderline, especially for smaller text like "Inverted Camera (Y-Axis)" and "Show Content Warning" in the sidebar which are noticeably dimmer.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Increase text luminance to achieve at least 4.5:1 contrast ratio. The sidebar dim text is particularly bad — either brighten it or use the same color as other sidebar items.

### 2. Sidebar items have inconsistent text sizing suggesting broken hierarchy
**Problem:** "Resume Game" through "Select Difficulty" use a consistent size, but "Inverted Camera (Y-Axis)" and "Show Content Warning" suddenly shrink to a much smaller font. This looks like a bug rather than intentional hierarchy.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#3)
**Fix:** Use consistent font size for all sidebar menu items. If some are secondary, use indentation or grouping rather than shrinking text.

---

## Agreed by 3/4 auditors

### 3. No visible focus indicator on the main settings list
**Problem:** The right-side settings categories (Controls, Gameplay, etc.) appear as flat teal bars with no visible hover/focus/selected state differentiation. It's unclear which item is currently selected or how to interact.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#2)
**Fix:** Add a clear focus indicator — brighter border, background highlight, or left-edge accent bar on the focused item. Show hover states on mouse-over.

### 4. Sidebar and main panel have no clear visual separation
**Problem:** The PAUSE MENU sidebar and the // SETTINGS panel sit next to each other with only a subtle border between them. The relationship (sidebar selects, main panel shows content) isn't visually reinforced.
**Flagged by:** Google (#4), Sun (#3), Microsoft (#4)
**Fix:** Add a stronger visual divider — a brighter vertical line, different background shade, or clear panel border to establish the master-detail layout.

---

## Agreed by 2/4 auditors

### 5. Bottom bar button prompts are small and inconsistently styled
**Problem:** The "[<-] SELECT" and "Esc BACK" prompts at the bottom are small and use different bracket styles. The "SELECT" label appears twice (once centered, once in the button bar).
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Consolidate to a single button prompt bar with consistent styling. Remove the duplicate "SELECT" label.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
