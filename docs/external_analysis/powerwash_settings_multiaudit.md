# Consolidated Design Audit: powerwash_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Setting labels have inconsistent font sizes creating false hierarchy
**Problem:** "Vsync" and "SSAO" are displayed in much larger text than other settings (Screen Resolution, Window Mode, Anti-Aliasing). These are all settings at the same level, but the size differences suggest some are more important.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Use consistent font size for all setting labels. If the label text is shorter, it shouldn't be rendered larger — maintain uniform sizing across the column.

### 2. Left/right arrow selectors (<, >) are small and hard to target
**Problem:** The < and > buttons for each setting are small and closely spaced. On the highlighted "Anti-Aliasing" row, the > arrow appears green/highlighted while others are plain. Touch/click targets appear below 44px.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Increase arrow button sizes to at least 44px. Add visible button boundaries or backgrounds. Consider using a slider or dropdown for settings with many options.

---

## Agreed by 3/4 auditors

### 3. Help panel on the right provides useful but redundant information
**Problem:** The "Help: Anti-Aliasing" panel shows a description and "Current: 2x" which duplicates the main panel's value. The help panel is valuable but its content could be more actionable (e.g., performance impact).
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Add unique value to the help panel — performance impact estimates, visual comparison descriptions, or recommended settings. Remove the redundant "Current: 2x" since it's already visible in the main list.

### 4. Tab bar (GENERAL/GAMEPLAY/VIDEO/AUDIO/CONTROLS) has inconsistent styling
**Problem:** "VIDEO" is highlighted as the active tab with larger text and a box, while other tabs use smaller text. The size difference between active and inactive tabs is jarring — it shifts the layout.
**Flagged by:** Apple (#4), Google (#4), Microsoft (#4)
**Fix:** Use a consistent tab size for all tabs. Indicate the active tab with color/underline/background rather than size changes that shift layout.

---

## Agreed by 2/4 auditors

### 5. Bottom button prompts (X Close, O Reset, [] Select) are cryptic
**Problem:** The button prompt bar uses single letters/symbols (X, O, []) that map to controller buttons, but without icons they're ambiguous. "O Reset" could mean the letter O or a circle button.
**Flagged by:** Sun (#5), Microsoft (#5)
**Fix:** Use proper controller button icons rather than text letters. Or use text labels like "Close (X)" with the button icon in a distinct style.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
