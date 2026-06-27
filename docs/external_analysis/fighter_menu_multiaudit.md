# Consolidated Design Audit: fighter_menu

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Function key labels (F1-F6) are inaccessible and non-standard
**Problem:** Menu items use "F1 System Options", "F2 Game Options", etc. Function keys are keyboard shortcuts shown as primary labels, which is confusing — they look like part of the menu item name rather than shortcut indicators.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Move shortcut indicators to the right side of the menu item (standard convention) or display them as subtle secondary text. The menu item label should be the readable name only.

### 2. Character preview area is mostly empty gray space
**Problem:** The center "Offline Mode" panel shows a small placeholder face icon with "Select Character" text, but the vast majority of the space is unused gray. This wastes prime screen real estate and looks unfinished.
**Flagged by:** Apple (#2), Google (#2), Sun (#3), Microsoft (#2)
**Fix:** Show an actual character preview or illustration. If no character is selected, show a more engaging prompt with a clear call-to-action.

---

## Agreed by 3/4 auditors

### 3. Tab bar (Offline/Online/Customize/Options) has unclear selected state
**Problem:** The "Options" tab appears highlighted in green/yellow, but the other tabs use similar styling. The L/R bumper indicators are small. The selected tab isn't visually distinct enough from unselected tabs.
**Flagged by:** Apple (#3), Google (#3), Sun (#2)
**Fix:** Make the selected tab clearly distinct — stronger background color, underline indicator, or different elevation. Dim unselected tabs more aggressively.

### 4. Right panel info card has weak visual connection to the selected menu item
**Problem:** The System Options description card on the right shows an icon and description, but there's no visual line or arrow connecting it to the selected "F1 System Options" item on the left. The relationship is implied only by position.
**Flagged by:** Apple (#4), Google (#4), Microsoft (#4)
**Fix:** Add a subtle connector (line, arrow, or shared background highlight) between the selected menu item and its detail card. Or use a slide-in animation when selection changes.

---

## Agreed by 2/4 auditors

### 5. Bottom button bar prompt text is small and dense
**Problem:** The controller prompt bar at the bottom shows multiple button mappings (+:Select, A:Confirm, B:Return, L R:Change Entry) in small text. The information density is high for a quick-reference bar.
**Flagged by:** Sun (#5), Microsoft (#5)
**Fix:** Increase font size, add more spacing between prompts, and use consistent icon+label formatting for each button mapping.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
