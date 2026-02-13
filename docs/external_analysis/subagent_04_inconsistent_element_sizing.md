# Subagent 4: Inconsistent Element Sizing Within Same Hierarchy

**Severity:** Critical (4/4 auditor agreement)
**Affected screens:** 12
**Staleness review (2026-02-10):** RALPH_TASK modified KirbyOptions, DeadSpaceSettings, and EmpireTycoon but only fixed tab/positioning issues — element sizing consistency was NOT changed. **All findings remain valid.**

---

## Problem

Elements at the same hierarchy level (buttons in a group, menu items in a list, setting labels in a column) have inconsistent sizes, creating visual noise and false hierarchy. This makes screens feel unpolished and confuses users about which elements are more important.

## Affected Screens & Specific Locations

| Screen | Element Group | Problem | Fix |
|--------|--------------|---------|-----|
| `AngryBirdsSettings.h` | Left/right column buttons | Different widths, don't align to grid | Set both columns to equal width using `percent(0.45f)` |
| `CasualSettings.h` | Left/right column buttons | Same as AngryBirds — different widths | Same fix |
| `PowerWashSettings.h` | Setting labels (Vsync, SSAO vs others) | Different font sizes for same-level labels | Use consistent `h720(16.0f)` for all labels |
| `KirbyOptions.h` | Category icons (Controls, Display, Accessibility) | Dramatically different sizes and opacities | Set all icons to same `ComponentSize` |
| `DeadSpaceSettings.h` | Sidebar menu items | "Inverted Camera" and "Show Content Warning" shrink | Use consistent font size for all items |
| `FighterMenu.h` | F-key menu items | All same size — OK | No fix needed |
| `EmpireTycoon.h` | Bottom action buttons (Speed Up, Prioritize, New Project) | Different styles (yellow filled vs white outline) | Use consistent button variant |
| `RubberBanditsMenu.h` | Menu items | All same size but same color as bg | Fix contrast (see Subagent 2) |
| `SportsSettings.h` | Setting labels | Mostly consistent | Minor — low priority |
| `FlightOptions.h` | Sidebar category items | Consistent with dash prefix | OK except "FLIGHT SYSTEM" is boxed |
| `MiniMotorwaysSettings.h` | Sidebar categories | Consistent sizing | OK |
| `ButtonVariantsShowcase.h` | Size demos (Small/Medium/Large) | Intentionally different — showcase | No fix needed |

## Vendor-Level Fix Opportunities

### 1. Layout constraint helpers

Could add layout helpers that enforce uniform sizing within a group:

```cpp
// Proposed API
auto group = ui::button_group({
    {"Speed Up", accent},
    {"Prioritize", accent},
    {"New Project", secondary}
}, GroupConfig().with_uniform_width());
```

This is a significant API addition — **mark as future enhancement**.

### 2. Flex layout improvements

Current flex layout should handle equal-width distribution with `flex_grow`. If screens aren't using this, it's a usage issue, not a vendor issue.

## Screen-Level Fixes

| Screen | Change | Lines |
|--------|--------|-------|
| `AngryBirdsSettings.h` | Set both button columns to `percent(0.45f)` | ~4 |
| `CasualSettings.h` | Same as above | ~4 |
| `PowerWashSettings.h` | Normalize label font sizes | ~6 |
| `KirbyOptions.h` | Set all category icons to same size | ~3 |
| `DeadSpaceSettings.h` | Set consistent font size for all sidebar items | ~4 |
| `EmpireTycoon.h` | Use same button variant for all 3 action buttons | ~3 |

## Recommended Priority

| Priority | Action | Screens |
|----------|--------|---------|
| **P1** | Fix KirbyOptions icon sizes (most jarring) | KirbyOptions |
| **P1** | Fix PowerWash label sizes | PowerWashSettings |
| **P1** | Fix DeadSpace sidebar consistency | DeadSpaceSettings |
| **P2** | Fix AngryBirds/Casual column widths | AngryBirds, Casual |
| **P2** | Fix EmpireTycoon button consistency | EmpireTycoon |

## Estimated Effort

- **Total: ~25-35 lines across 6 screens**
- No vendor changes needed — all screen-level fixes
