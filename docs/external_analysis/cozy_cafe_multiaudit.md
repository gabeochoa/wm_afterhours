# Consolidated Design Audit: cozy_cafe

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Patience bars and time badges rely on color alone and are nearly illegible
**Problem:** Customer patience is shown as a tiny green/empty bar with a percentage badge (60%, 0%). The badges are extremely small and use low-contrast text on colored backgrounds. Time remaining badges (2m, 5m) are similarly tiny. Color is the sole state indicator.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Enlarge badges to readable size (min 12px text). Add a secondary encoding beyond color (pattern, shape, or large numeric display). Ensure 4.5:1 contrast ratio on all badge text.

### 2. Star rating is unreadable at current size
**Problem:** The rating display in the header shows stars and "Customers Served: 25" but both are rendered at such a small scale that individual stars are indistinguishable. The subtitle text is extremely small.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Enlarge the star rating area substantially. Use clearly defined, larger star icons. Increase the subtitle text to minimum body-text size.

---

## Agreed by 3/4 auditors

### 3. Inconsistent panel header styling (brown vs green)
**Problem:** "Today's Specials" uses a dark brown header while "Customers" uses a sage green header. These are sibling panels at the same level but have completely different header colors, suggesting different importance.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Use a single consistent header color for same-level panels. Subtle shade variations are acceptable, but not entirely different hues.

### 4. Bottom icon buttons (Inventory/Research/Crafting) lack labels at first glance
**Problem:** The three icon buttons in the bottom-right use abstract icons with small text labels underneath. The badge notifications (red dots with numbers) are small. At a glance, the icons alone don't clearly convey function.
**Flagged by:** Apple (#4), Google (#4), Microsoft (#4)
**Fix:** Increase icon size, ensure labels are always visible (not just on hover), and make notification badges larger and higher contrast.

---

## Agreed by 2/4 auditors

### 5. Music slider lacks value label and clear track/handle contrast
**Problem:** The music slider shows "Music: 70%" text but the slider itself uses a muted green track with a slightly different shade handle. The handle doesn't stand out clearly from the track.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Increase handle-to-track contrast. Consider adding tick marks or making the handle a distinctly different color/shape from the track.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
