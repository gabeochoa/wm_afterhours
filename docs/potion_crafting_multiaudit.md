# Consolidated Design Audit: potion_crafting

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Recipe list item descriptions are small and low-contrast
**Problem:** Each recipe shows description text ("Restores 50 HP", "Deals 25 fire damage") and duration ("30s", "45s") in small, muted text on a dark background. The information is important for decision-making but hard to read.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Increase description font size and contrast. Use a lighter text color or add a subtle background panel behind the text. Duration should be more prominent since it affects gameplay decisions.

### 2. Ingredient icons (S, *, M, F, etc.) are abstract single-letter codes
**Problem:** Ingredients in the "Required Ingredients" section and the right panel use single-letter colored circles (S, *, M, F, H, X). Without memorization, users can't tell what "S x3" means.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Show the full ingredient name beside or below the icon. Even abbreviations like "Sea" would be better than single letters. The right panel does show full names — make the Required Ingredients section match.

---

## Agreed by 3/4 auditors

### 3. Brewing progress bar lacks clear start/end indicators
**Problem:** The green progress bar under the bottle shows "65% Brewed" but has no markers for 0% or 100%. The bar blends with the dark background, making progress estimation imprecise.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Add a visible track/background to the progress bar showing the full range. Add tick marks at 25% intervals or at minimum show clear endpoints.

### 4. Tab bar (Recipes/Inventory/Journal) has weak selected state
**Problem:** "Recipes" has a border indicating selection, but the visual distinction from "Inventory" and "Journal" is subtle. The tabs blend together, especially on the dark background.
**Flagged by:** Apple (#4), Sun (#4), Microsoft (#4)
**Fix:** Make the selected tab more visually prominent — brighter background, underline accent, or filled vs outlined treatment. Dim unselected tabs.

---

## Agreed by 2/4 auditors

### 5. Status bar text at bottom is very small
**Problem:** "Alchemy Level: 12 | Recipes Known: 4 | Potions Brewed: 47" uses small text at the bottom of the screen. This progression info is valuable but easy to miss.
**Flagged by:** Google (#5), Sun (#5)
**Fix:** Increase font size or integrate key stats into the header area near "Gold: 2,450" where they'll get more attention.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
