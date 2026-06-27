# Consolidated Design Audit: shop_interface

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Item icon symbols (/, +, -, #, *, !, O, =) are cryptic
**Problem:** Each inventory item has a single-character icon in a colored circle. These characters (/, +, -, #, *, !, O, =) have no obvious connection to the items they represent (Iron Sword, Health Potion, etc.). Users must memorize the mapping.
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Use recognizable icons — a sword for weapons, a potion bottle for consumables, a shield for armor. Even simple silhouettes would be far more readable than abstract single characters.

### 2. "Stock" column values with low-stock highlighting rely on color alone
**Problem:** Items with low stock (Fire Scroll: 3, Steel Shield: 2) show the number in red/pink. Color is the only indicator that stock is low. The threshold for "low" isn't communicated.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Add a warning icon or text suffix ("Low!" or "2 left") beside low-stock values. Use color as reinforcement, not the sole indicator.

---

## Agreed by 3/4 auditors

### 3. Buy/Sell tab styling doesn't clearly show which is active
**Problem:** "Buy" has a border and "Sell" appears slightly different, but the active tab indication is subtle. Both tabs use similar colors and sizes.
**Flagged by:** Apple (#3), Google (#3), Microsoft (#3)
**Fix:** Make the active tab visually dominant — filled background on active, outlined/dimmed on inactive. Use a clear underline or top-border accent.

### 4. Cart section lacks visual emphasis for the "Purchase" action
**Problem:** The cart shows items and total (180 G), but the "Purchase" button uses a muted yellow that doesn't strongly call to action. For a primary e-commerce action, it should be the most prominent element.
**Flagged by:** Apple (#4), Google (#4), Sun (#4)
**Fix:** Make "Purchase" a high-contrast primary button — larger, brighter, with clear hover/pressed states. Consider adding a confirmation step for expensive purchases.

---

## Agreed by 2/4 auditors

### 5. Bottom button prompt bar is small and easy to miss
**Problem:** "A: Select  B: Back  X: Add to Cart  Y: Remove" at the bottom uses small text. These are critical interaction hints but positioned where they get least attention.
**Flagged by:** Sun (#5), Microsoft (#5)
**Fix:** Increase prompt text size and add proper controller button icons. Position closer to the interactive area or make them contextual (show near the cursor).

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
