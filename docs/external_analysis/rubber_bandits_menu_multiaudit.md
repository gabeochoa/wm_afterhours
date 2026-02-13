# Consolidated Design Audit: rubber_bandits_menu

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Extremely low contrast throughout — yellow text on yellow background
**Problem:** The entire screen uses a saturated yellow background with slightly darker yellow and white text. The title "RUBBER BANDIT$" uses green-on-yellow and gold-on-yellow which is nearly invisible. Multiple elements fail the 4.5:1 contrast minimum.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Dramatically increase contrast. Use a darker background or much darker text. The title needs to be clearly legible — consider dark text on the yellow background or a contrasting banner.

### 2. Menu items have no visible focus/hover state differentiation
**Problem:** The ">" indicator next to "OPTIONS" suggests it's selected, but the visual difference between selected and unselected items is minimal. All items are the same color and size with no highlight.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Add a clear selected state — background highlight, bold text, larger size, or left-edge accent. The ">" arrow alone is insufficient.

---

## Agreed by 3/4 auditors

### 3. "Supporter pack available" notification is easy to miss
**Problem:** The small white notification box at the right ("Supporter pack available X / More characters in shop") uses tiny text on a small white card. It blends with the yellow background.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Increase size and contrast. Add a border or shadow to lift the notification visually. Use a more attention-grabbing color if this is an important promotional element.

### 4. Status text at bottom-left is illegible
**Problem:** "Change the game's settings" helper text at the bottom-left uses light green text on the yellow background. It's nearly invisible at normal viewing distance.
**Flagged by:** Apple (#4), Google (#4), Microsoft (#4)
**Fix:** Increase contrast substantially — use dark text or add a semi-transparent dark background behind the helper text.

---

## Agreed by 2/4 auditors

### 5. Character selector (LB/RB EARL THE BIRD) is disconnected from the menu
**Problem:** The character selector at the bottom center feels like a separate component from the main menu. Its dark background makes it visually inconsistent with the bright yellow theme.
**Flagged by:** Sun (#5), Microsoft (#5)
**Fix:** Integrate the character selector more cohesively with the overall theme. Use consistent background treatment, or position it where it clearly relates to gameplay context.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
