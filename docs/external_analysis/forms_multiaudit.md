# Consolidated Design Audit: forms

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Slider handles are barely visible against the track
**Problem:** The slider handles (Volume, Brightness, Difficulty) are small and use a slightly different shade from the track. The handle-to-track contrast is too low to easily grab and position.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Make slider handles larger (at least 20px), use a contrasting color (white or bright accent), and add a visible border or shadow for depth.

### 2. Checkbox rows (Music, SFX, etc.) don't align with slider column
**Problem:** The Sliders column and Checkboxes column have different widths and vertical alignment. The two sections look like independent panels rather than parts of a cohesive form.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Use consistent column widths and vertical alignment. Add a shared grid or align both columns to the same vertical start position.

---

## Agreed by 3/4 auditors

### 3. "Disabled" checkbox row is barely distinguishable
**Problem:** The disabled checkbox ("Disabled v") uses a slightly lighter gray but is otherwise identical to active checkboxes. Users may try to interact with it.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Use clearly reduced opacity (50%), strikethrough text, or a grayed-out background that's obviously non-interactive.

### 4. Clipboard Demo section at bottom is visually disconnected
**Problem:** The clipboard section (Copy Status, Paste, clipboard empty) sits below a status bar and uses different button styles (dark, red, label). It feels like a separate demo rather than part of the form gallery.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Either integrate it visually with the form (consistent styling, clear section header) or separate it more clearly with a visible divider and section title.

---

## Agreed by 2/4 auditors

### 5. Progress bars don't show how they differ from sliders
**Problem:** Progress Bars section shows "Audio Level: 75%" and "Level Progress: 75/100" but they look similar to the sliders above. A showcase should make the read-only nature of progress bars clear.
**Flagged by:** Apple (#5), Sun (#5)
**Fix:** Add a visual distinction — no handle, different color scheme, or a label indicating these are display-only. Animate them to show they change independently.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
