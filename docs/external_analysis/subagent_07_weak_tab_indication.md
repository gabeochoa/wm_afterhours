# Subagent 7: Weak Active/Selected Tab Indication

**Severity:** High (3/4 auditor agreement)
**Affected screens:** 8
**Staleness review (2026-02-10):** RALPH_TASK replaced `tab_container()` with manual tab buttons in KirbyOptions and DeadSpaceSettings (afterhours gap #1 workaround). The manual buttons have explicit active/inactive color styling, which **partially addresses** tab indication for those two screens. Other affected screens (PotionCrafting, ShopInterface, PowerWash, Sports, Fighter, TabContainer) were NOT modified. **6 of 8 findings remain valid; 2 partially addressed.**

---

## Problem

Screens with tab bars don't make the active tab visually distinct enough from inactive tabs. Users have to study the UI to determine which tab is currently selected. Some tabs use size changes (which shifts layout), some use subtle color changes, and some use borders that are hard to see.

## Affected Screens & Specific Locations

| Screen | Tab Bar | Active Indicator | Problem |
|--------|---------|-----------------|---------|
| `PotionCrafting.h` | Recipes / Inventory / Journal | Subtle border | Border too thin; bg unchanged |
| `ShopInterface.h` | Buy / Sell | Subtle border | Minimal visual difference |
| `PowerWashSettings.h` | GENERAL / GAMEPLAY / VIDEO / AUDIO / CONTROLS | Active tab is LARGER text | Size change shifts layout; jarring |
| `SportsSettings.h` | LB GAMEPLAY / VIDEO / AUDIO / CONTROLS RB | Active underline | Underline is subtle |
| `FighterMenu.h` | Offline / Online / Customize / Options | Green/yellow highlight | Highlight is subtle on dark bg |
| `KirbyOptions.h` | L WiFi Control Home Mail Star Gear R | Yellow bg on active | ~~OK but icons themselves unclear~~ **Partially addressed** — manual tab buttons with explicit active/inactive colors (RALPH_TASK) |
| `RaceResults.h` | N/A (no tabs) | — | Not applicable |
| `TabContainerShowcase.h` | Demo tabs | Showcase — shows selected state | Underline is thin |

## Vendor-Level Fix Opportunities

### 1. Tab component styling defaults

The tab container component likely has a default selected-tab style. Improving the default would fix all screens at once.

**Proposed changes to tab defaults:**
- Active tab: filled background (accent color), bold text, bottom accent bar (3px)
- Inactive tab: transparent background, regular weight text, no accent bar
- Hover: subtle background fill (accent at 20% opacity)

```cpp
// In tab container component defaults
active_tab_config
    .with_custom_background(theme.accent)
    .with_font_color(theme.on_accent)
    .with_border(Border{.bottom = h720(3.0f)}, theme.accent);

inactive_tab_config
    .with_custom_background(Color::transparent())
    .with_font_color(theme.on_surface.with_alpha(180));
```

### 2. Avoid size-based tab selection

The PowerWash approach of making the active tab larger is an anti-pattern — it shifts layout and creates visual instability. The vendor should enforce same-size tabs with color/weight differentiation only.

## Screen-Level Fixes

| Screen | Change | Lines |
|--------|--------|-------|
| `PotionCrafting.h` | Thicker border (3px) + bg color change on active | ~5 |
| `ShopInterface.h` | Add bg fill on active tab | ~3 |
| `PowerWashSettings.h` | Remove size change; use color/border instead | ~8 |
| `SportsSettings.h` | Thicker underline + bg subtle fill | ~4 |
| `FighterMenu.h` | Brighter highlight + stronger contrast | ~3 |
| `TabContainerShowcase.h` | Thicker underline in demo | ~2 |

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P0** | Fix PowerWash tab sizing anti-pattern | Most jarring issue |
| **P1** | Vendor: Improve tab default selected style | Fixes all at once |
| **P1** | Strengthen active indicator in PotionCrafting, ShopInterface | High-visibility mockups |
| **P2** | FighterMenu, SportsSettings tab contrast | Secondary |

## Estimated Effort

- Vendor tab defaults: ~15 lines
- Per-screen fixes: ~3-8 lines each × 6 screens = ~30 lines
- **Total: ~45 lines**
