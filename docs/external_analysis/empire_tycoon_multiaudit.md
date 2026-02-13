# Consolidated Design Audit: empire_tycoon

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Left sidebar icons lack clear labels and consistent sizing
**Problem:** The four sidebar buttons (Rides, Food, Upgrades, Finance) use abstract icons with tiny text labels below. The icons are different visual styles and the "Upgrades" button has a notification badge that's easy to miss. The icon-to-label relationship is weak.
**Flagged by:** Apple (#1), Google (#1), Sun (#2), Microsoft (#1)
**Fix:** Increase icon size and label font. Use a consistent icon style. Make the notification badge larger and higher contrast. Consider icon+text buttons where the label is beside the icon.

### 2. Production Overview and Current Projects panels lack visual hierarchy
**Problem:** Both panels use identical white backgrounds with small gray headers. The data inside (Toys: 5200/min, etc.) is readable but the "^" trend indicators are cryptic. The "^ = Trending Up" legend in the top-right is easy to miss.
**Flagged by:** Apple (#2), Google (#2), Sun (#1), Microsoft (#2)
**Fix:** Use color or icons for trend indicators (green arrow up, red arrow down). Move the legend closer to the data it explains, or replace the "^" with proper arrow icons inline.

---

## Agreed by 3/4 auditors

### 3. Three action buttons at bottom use inconsistent styles
**Problem:** "Speed Up" and "Prioritize" use yellow filled style while "New Project" uses a white/outline style. All three are primary actions at the same level but have different visual weights.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Use a consistent button style for same-level actions. If one is primary, make the others secondary using a coherent hierarchy (filled vs outlined).

### 4. Bottom-right icon buttons (Shop/Settings/Leaderboard) are too small
**Problem:** The three circular gold buttons at the bottom-right are small with abstract icons. Their function isn't immediately clear without hovering. Touch targets appear below 44px.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Increase button size to at least 44px. Add text labels below or beside icons. Use a more visible background color that contrasts with the toolbar area.

---

## Agreed by 2/4 auditors

### 5. Happiness meter is small and disconnected from context
**Problem:** The "Happiness 85%" meter in the top-right uses a small green bar with a smiley face. It's isolated from other metrics and easy to overlook despite being a key game metric.
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Increase the meter size or integrate it with other key metrics in a unified HUD bar. Use a larger, more visible progress bar.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
