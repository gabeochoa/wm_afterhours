# Subagent 8: Small Touch/Click Targets

**Severity:** High (3/4 auditor agreement)
**Affected screens:** 10

---

## Problem

Arrow buttons, close buttons, icon-only buttons, and small interactive elements have touch/click targets below the recommended 44×44px minimum (Apple HIG) / 48×48dp (Material). This makes precise interaction difficult, especially on touch devices or with motor impairments.

## Affected Screens & Specific Locations

| Screen | Element | Estimated Size | Minimum | Fix |
|--------|---------|---------------|---------|-----|
| `AngryBirdsSettings.h` | Red X close button | ~30px | 44px | Enlarge to 44px; move inside panel |
| `CasualSettings.h` | Red X close button | ~30px | 44px | Same as above |
| `CasualSettings.h` | WiFi/Sync icon button | ~24px | 44px | Enlarge; add label |
| `MiniMotorwaysSettings.h` | "<-" back button | ~32px | 44px | Enlarge to 44px |
| `MiniMotorwaysSettings.h` | "<" ">" sensitivity arrows | ~28px | 44px | Enlarge arrows |
| `PowerWashSettings.h` | "<" ">" selector arrows | ~28px | 44px | Enlarge arrows |
| `SportsSettings.h` | "<" ">" selector arrows | ~28px | 44px | Enlarge arrows |
| `ParcelCorpsSettings.h` | "<" ">" language arrows | ~24px | 44px | Enlarge arrows |
| `EmpireTycoon.h` | Bottom-right icon buttons (Shop/Settings/Leaderboard) | ~36px | 44px | Enlarge to 44px |
| `KirbyOptions.h` | Edit/Erase buttons | ~32px | 44px | Enlarge |
| `CozyCafe.h` | Patience badges | ~20px | 44px | Enlarge significantly |
| `NeonStrike.h` | Left-side weapon icons | ~36px | 44px | Enlarge to 44px |

## Vendor-Level Fix Opportunities

### 1. Minimum hit area enforcement (HIGH IMPACT)

Add a validation rule that warns when interactive components (buttons) are below minimum touch target size:

```cpp
// Proposed: ValidateMinTouchTarget
struct ValidateMinTouchTarget : System<> {
    void for_each_with(Entity& entity, UIComponent& comp, IsButton&) {
        float min_target = h720(44.0f);  // 44px at 720p
        if (comp.size.width < min_target || comp.size.height < min_target) {
            report_violation("TouchTarget", entity,
                "Button smaller than 44px minimum touch target");
        }
    }
};
```

This would catch all small buttons automatically during validation runs.

### 2. Hit area expansion

Some design systems allow a visual element to be smaller than its hit area. Could add:

```cpp
auto config = ComponentConfig()
    .with_size(ComponentSize{h720(24.0f), h720(24.0f)})  // Visual: 24px
    .with_min_hit_area(h720(44.0f));  // Clickable: 44px
```

This is a significant vendor feature — **mark as future enhancement**.

## Screen-Level Fixes

The most common pattern is `<` `>` arrow buttons for option selectors. These appear in 4 screens:

| Screen | Change | Lines |
|--------|--------|-------|
| `PowerWashSettings.h` | Increase arrow button size to `h720(44.0f)` | ~2 |
| `SportsSettings.h` | Same | ~2 |
| `ParcelCorpsSettings.h` | Same | ~2 |
| `MiniMotorwaysSettings.h` | Same for both back and arrows | ~3 |
| `AngryBirdsSettings.h` | Enlarge close button to `h720(44.0f)` | ~2 |
| `CasualSettings.h` | Same + enlarge sync button | ~3 |
| `EmpireTycoon.h` | Enlarge bottom icon buttons | ~2 |
| `KirbyOptions.h` | Enlarge Edit/Erase buttons | ~2 |
| `CozyCafe.h` | Enlarge patience badges | ~3 |

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P0** | Vendor: Add ValidateMinTouchTarget validator | Catches all future violations |
| **P1** | Fix close buttons (AngryBirds, Casual) | High-visibility, common pattern |
| **P1** | Fix arrow selectors (PowerWash, Sports, Parcel, MiniMotorways) | 4 screens, same fix |
| **P2** | Fix icon buttons (Empire, Kirby, CozyCafe) | Secondary |

## Estimated Effort

- Vendor validator: ~20 lines
- Per-screen fixes: ~2-3 lines each × 10 screens = ~25 lines
- **Total: ~45 lines**
