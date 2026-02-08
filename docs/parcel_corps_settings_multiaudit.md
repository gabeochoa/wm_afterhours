# Consolidated Design Audit: parcel_corps_settings

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Settings panel is visually dense — too many control types in one panel
**Problem:** The phone-style settings panel mixes toggle switches, option selectors (</>), sliders, and text labels all in a narrow column. The density makes it hard to scan and the different control types are not visually distinct from each other.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Group related settings with section headers and spacing. Use consistent control types where possible. Add dividers between setting groups.

### 2. Icon legend at bottom is tiny and hard to read
**Problem:** The "Icon Legend" section at the bottom lists what each colored icon means (Language, Effects, Sync, Quality, Display, Text) but the icons and text are extremely small and low-contrast.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Either increase the legend size or remove it and use text labels beside each icon directly in the settings list. Inline labels are more discoverable than a separate legend.

---

## Agreed by 3/4 auditors

### 3. Right-side panels (chat, notifications) have unclear relationship to settings
**Problem:** The Twitch-style chat, live notification, and delivery app prompts on the right side create visual noise that competes with the settings panel. The relationship between the game HUD and settings overlay isn't clear.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Dim or blur the background HUD elements when the settings panel is active, or use a modal overlay. Settings should be the clear focus.

### 4. Speedometer (009 M/h) is redundant during settings
**Problem:** The large "009 M/h GAMON" speedometer in the bottom-right shows game data while the user is in settings. This is distracting and irrelevant to the settings task.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Hide or substantially dim the speedometer when the settings panel is open. Show only settings-relevant information.

---

## Agreed by 2/4 auditors

### 5. Resolution text "2560 x 1440" is inconsistently placed
**Problem:** The resolution value appears as small text above the Resolution row, not aligned with other setting values. Other settings show their values inline (e.g., "English UK" next to Language).
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Move the resolution value inline with the setting label, consistent with how other settings display their current values.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
