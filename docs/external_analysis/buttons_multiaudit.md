# Consolidated Design Audit: buttons

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Disabled button state is barely distinguishable from Normal
**Problem:** The "Disabled" button in the States row uses a slightly grayed-out style, but the difference from the "Normal" button is subtle — both have similar sizing, positioning, and text color. Users may try to click the disabled button.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Make disabled state more obvious — reduced opacity (50%), strikethrough, or grayed-out text with a distinctly different background. Add "not-allowed" cursor on hover.

### 2. Row labels (States, Sizes, Group, Custom) are left-aligned but buttons are inconsistently spaced
**Problem:** Each section row has a left label with buttons flowing to the right, but the spacing between buttons varies across rows. The "Group" row has even spacing while "Custom" has uneven gaps.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Use consistent gap spacing (8px or 16px) between all buttons within and across rows. Align to an 8px grid.

---

## Agreed by 3/4 auditors

### 3. "Total clicks: 0" counter has no clear visual connection to which buttons it tracks
**Problem:** The click counter at the bottom tracks all button clicks, but it's positioned below the entire gallery with no indication of which buttons are counted. It could track all or just some.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Add clarifying text ("Total clicks across all buttons: 0") or position the counter closer to the interactive buttons it tracks.

### 4. Custom color buttons don't demonstrate the full custom API
**Problem:** Coral, Teal, and Sharp show custom colors but don't demonstrate custom borders, shadows, or other styling options. As a gallery showcase, it could demonstrate more configuration.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Add more varied custom examples — outlined custom, shadow custom, gradient-like custom — to showcase the full configuration API.

---

## Agreed by 2/4 auditors

### 5. Section separators are just darker bars with no labels or visual meaning
**Problem:** Each section is separated by a dark horizontal bar, but these bars have inconsistent heights and don't strongly delineate sections.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Use consistent section dividers with more spacing, or add subtle horizontal rules between sections.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
