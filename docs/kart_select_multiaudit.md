# Consolidated Design Audit: kart_select

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Stat bars (SPD/ACC/HND/WGT) use color as sole differentiator with no numeric values
**Problem:** The four stat bars use red, orange, green, and blue fills but show no numeric values. Users can't precisely compare characters — is SPD 60% or 70%? Color-blind users may struggle to distinguish bars.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Add numeric values (e.g., "SPD 7/10") or percentage labels beside each bar. Use patterns or shapes in addition to color to differentiate stats.

### 2. Character grid selection has weak focus/selected state
**Problem:** The selected character (Drift) has a subtle border but the visual distinction from unselected characters is minimal. The yellow highlight on Drift's grid cell is present but not dramatically different from the default blue circles.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Make the selected character dramatically different — larger scale, glow effect, stronger border (3-4px), or elevated/popped-out styling. Clearly differentiate from hover vs selected.

---

## Agreed by 3/4 auditors

### 3. Vehicle list lacks visual preview
**Problem:** The VEHICLE section lists names (Standard, Speedster, Off-Road, Classic) as text-only buttons. There's no visual preview of what each kart looks like, making the selection feel abstract.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Add small vehicle thumbnails or icons beside each name. Even simple silhouettes would help users differentiate options.

### 4. Bottom bar has three different button styles
**Problem:** "< Back" is white outlined, "READY!" is green filled, and the center text "Drift + Standard" is informational. Three different visual treatments at the same level create inconsistency.
**Flagged by:** Apple (#4), Google (#4), Microsoft (#4)
**Fix:** Use a consistent button family — outlined for secondary actions (Back) and filled for primary (READY!). Make the center text clearly non-interactive (different font weight, no button-like styling).

---

## Agreed by 2/4 auditors

### 5. Controller prompts at bottom are very small
**Problem:** "A: Select  B: Back  L/R: Switch Kart" text at the very bottom is rendered at a very small size and low contrast against the dark background.
**Flagged by:** Sun (#5), Microsoft (#5)
**Fix:** Increase font size to minimum 12px and improve contrast. Consider using icon+label button prompt style consistent with other screens.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
