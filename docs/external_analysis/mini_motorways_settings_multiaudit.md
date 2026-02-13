# Consolidated Design Audit: mini_motorways_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Toggle circles lack labels — state is only conveyed by filled vs empty
**Problem:** Night Mode, Vibration, Hold to Draw/Delete, and Colorblind Mode all use empty or filled circles as toggles. There's no text (ON/OFF) or checkmark — just empty vs filled. This is ambiguous, especially for "Hold to Draw/Delete" which is filled (on) while others are empty (off).
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Add "ON"/"OFF" labels beside each toggle, or use a standard toggle switch component. At minimum, add a checkmark inside the filled circle.

### 2. Back button "<-" is cryptic and small
**Problem:** The top-left back button shows only "<-" in a small yellow square. It's not immediately recognizable as a back/close button. The touch target appears small.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Use a standard back arrow icon or "< Back" text label. Increase size to minimum 44px. Use recognizable navigation iconography.

---

## Agreed by 3/4 auditors

### 3. Sidebar categories and right-side settings have no clear visual connection
**Problem:** The left sidebar (Game, Video, Audio, etc.) and the right settings area are separated by empty space. When "Game" is selected (highlighted yellow), the connection to the right-side toggles is implied only by position.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Add a visual container or shared background that groups the selected category with its settings. Use a connecting line or shared panel border.

### 4. "Controller Cursor Sensitivity" selector arrows are small
**Problem:** The < Default > selector for Controller Cursor Sensitivity uses small yellow arrow buttons. The arrows are the same size as the toggle circles but serve a different function, creating a visual conflict.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Differentiate the selector from toggles more clearly. Use larger, more distinct arrow buttons or a different control type (slider, dropdown).

---

## Agreed by 2/4 auditors

### 5. Version text and Tutorial button are visually disconnected
**Problem:** "Mini Motorways Version 1.10.2" sits at the bottom-left while "Tutorial ->" is at the bottom-right. They occupy the same footer row but have no visual relationship and different styles.
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Create a consistent footer bar that visually groups these elements, or separate them more deliberately with different vertical positions.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
