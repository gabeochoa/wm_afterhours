# Subagent 10: Cryptic Icons and Abbreviations Without Labels

**Severity:** Moderate (3/4 auditor agreement)
**Affected screens:** 6

---

## Problem

Several screens use abstract single-character icons, abbreviations, or symbol-only buttons that don't clearly communicate their function. Users must memorize icon meanings or hunt for legends that are small and easy to miss.

## Affected Screens & Specific Locations

| Screen | Element | Current | Problem | Fix |
|--------|---------|---------|---------|-----|
| `ShopInterface.h` | Item icons (/, +, -, #, *, !, O, =) | Single character in colored circle | No connection to item type | Use descriptive icons (sword, potion, shield) or item-type abbreviation |
| `PotionCrafting.h` | Ingredient icons (S, *, M, F, H, X) | Single letter in colored circle | Requires memorization | Show full name beside icon (already done in right panel — match in recipe area) |
| `KirbyOptions.h` | Tab bar icons (WiFi, Control, Home, etc.) | Small colored icons, most unlabeled | Function unclear | Add text labels below all tab icons |
| `EmpireTycoon.h` | Bottom icons (Shop, Settings, Leaderboard) | Abstract circular icons | Function unclear at first glance | Increase size; ensure labels always visible |
| `CozyCafe.h` | Bottom icons (Inventory, Research, Crafting) | Abstract icons with tiny labels | Labels too small to read quickly | Increase icon + label size |
| `ParcelCorpsSettings.h` | Settings category icons (colored dots) | Color-coded dots with separate legend | Legend is tiny and far from icons | Replace dots with inline text labels |
| `NeonStrike.h` | Left weapon/equipment icons | Abstract shapes | Function unclear | Add text labels beside icons |
| `ModalShowcase.h` | "BG Clicks" | Abbreviation | "BG" isn't obvious | Spell out "Background" |

## Vendor-Level Fix Opportunities

### 1. Icon + Label component

If not already available, create a combined icon-with-label component that always renders both:

```cpp
// Proposed API
ui::icon_button(IconConfig()
    .with_icon("shop_icon")
    .with_label("Shop")                    // Always visible
    .with_label_position(LabelPosition::Below)
    .with_min_size(h720(48.0f)));
```

### 2. Tooltip support

Add hover tooltips to icon-only elements as a fallback:

```cpp
auto config = ComponentConfig()
    .with_label("S")
    .with_tooltip("Sea Salt");  // Shows on hover
```

This is a significant feature addition — **mark as future enhancement**.

## Screen-Level Fixes

Most fixes are straightforward — add text beside or below icons:

| Screen | Change | Lines |
|--------|--------|-------|
| `ShopInterface.h` | Use 2-3 letter type abbreviations instead of symbols (SWD, POT, ARM, etc.) | ~8 |
| `PotionCrafting.h` | Add abbreviated names beside ingredient icons in recipe area | ~5 |
| `KirbyOptions.h` | Add text labels below all tab icons | ~7 |
| `EmpireTycoon.h` | Increase icon+label size for bottom buttons | ~3 |
| `CozyCafe.h` | Increase icon+label size for bottom buttons | ~3 |
| `ParcelCorpsSettings.h` | Replace legend with inline labels | ~10 |
| `NeonStrike.h` | Add text labels beside weapon icons | ~5 |
| `ModalShowcase.h` | Change "BG" to "Background" | ~1 |

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P1** | Fix ShopInterface item icons (most confusing) | High-visibility mockup |
| **P1** | Fix KirbyOptions tab labels | High-visibility mockup |
| **P1** | Fix PotionCrafting ingredient consistency | High-visibility mockup |
| **P2** | Increase icon sizes for EmpireTycoon, CozyCafe | Readability |
| **P2** | Fix ParcelCorps legend → inline labels | Readability |
| **P3** | Vendor: Tooltip support | Future enhancement |

## Estimated Effort

- Per-screen text/label additions: ~1-10 lines each × 8 screens = ~42 lines
- **Total: ~42 lines, all screen-level changes**
- Vendor tooltip feature: ~100+ lines (future)

---

# Grand Summary: All 10 Subagents

| # | Issue | Severity | Vendor Fix? | Screen Effort | Total Lines |
|---|-------|----------|-------------|---------------|-------------|
| 1 | Color as sole state indicator | Critical | Checkbox "✓" (1 line) | ~60-80 | ~80 |
| 2 | Small/low-contrast text | Critical | MinFontSize exists | ~60-80 | ~80 |
| 3 | Disabled states too subtle | Critical | Auto 50% opacity (~15 lines) | ~30 if no vendor | ~15-30 |
| 4 | Inconsistent element sizing | Critical | None needed | ~25-35 | ~35 |
| 5 | Section headers too small | Critical | Gallery helpers (~20 lines) | ~150 | ~170 |
| 6 | Missing hover/focus demos | High | `with_forced_state()` (future) | ~120 | ~120 |
| 7 | Weak tab indication | High | Tab defaults (~15 lines) | ~30 | ~45 |
| 8 | Small touch/click targets | High | ValidateMinTouchTarget (~20 lines) | ~25 | ~45 |
| 9 | Debug-like status text | Moderate | Status bar helper (~15 lines) | ~40 | ~55 |
| 10 | Cryptic icons/abbreviations | Moderate | Tooltip (future) | ~42 | ~42 |
| | **TOTALS** | | **~85 vendor lines** | **~600 screen lines** | **~700** |

## Recommended Execution Order

1. **Vendor fixes first** (~85 lines): disabled opacity, gallery helpers, tab defaults, touch target validator, checkbox ✓
2. **Game mockup screens** (~300 lines): highest visibility, role-model quality
3. **Gallery screens** (~250 lines): section headers + status bars + state demos
4. **Polish pass** (~150 lines): remaining contrast, sizing, label fixes
