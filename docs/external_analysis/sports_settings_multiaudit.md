# Consolidated Design Audit: sports_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Preview bars (teal mini-bars next to each setting) lack clear meaning
**Problem:** Each setting row has small teal bar segments to the right of the value, but their meaning is unclear. Do they represent impact? Range position? Quality tier? There's no legend or tooltip explaining them.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Either add a legend explaining what the bars represent, or remove them if they're decorative. If they show performance impact, label them as such.

### 2. Left/right arrow selectors are small and have weak active state
**Problem:** The < and > arrows for each setting are small and their active/hover state (green) is subtle. The arrows are positioned close to the value text, making precise clicking difficult.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Increase arrow button size to at least 44px touch targets. Add visible button backgrounds. Increase spacing between arrows and values.

---

## Agreed by 3/4 auditors

### 3. "Anti-Aliasing method" value text is truncated and small
**Problem:** The Anti-Aliasing method shows "TSR (Temporal Super Resolution)" which is truncated or very small compared to other value labels. This setting has the longest label but gets squeezed.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Allow the value area to expand for longer text, or use abbreviations with a help panel explanation. Ensure all value text meets minimum readable size.

### 4. Help panel on the right side has limited useful information
**Problem:** The "Window mode" help panel shows a title, generic description ("Adjust this setting to customize your gaming experience"), and current value. The description is too generic to be helpful.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Provide setting-specific descriptions — for Window Mode: explain Fullscreen vs Borderless vs Windowed differences. For graphics settings, mention performance impact.

---

## Agreed by 2/4 auditors

### 5. Bottom button prompts (Y Reset to default, B Back) are small
**Problem:** The controller prompts at the bottom-right are small with low contrast. "Reset to default" is an important action but gets minimal visual weight.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Increase prompt text size and use proper button icons. Consider making "Reset to default" a more prominent button if it's a common action.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
