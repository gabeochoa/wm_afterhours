# Subagent 1: Color as Sole State Indicator

**Severity:** Critical (4/4 auditor agreement)
**Affected screens:** 14

---

## Problem

Toggles, checkboxes, and status indicators use color as the only way to differentiate states (ON vs OFF, selected vs unselected, low stock vs normal). This fails accessibility standards (WCAG 1.4.1 — Use of Color) and is problematic for color-blind users.

## Affected Screens & Specific Locations

### Game Mockups

| Screen | Element | Current | Fix |
|--------|---------|---------|-----|
| `AngryBirdsSettings.h` | Music/Sound/Vibrate toggles | Green "ON" / gray "OFF" pills, color only | Add checkmark icon in ON states; use ✓/✗ text |
| `CasualSettings.h` | Music/Sound/Vibrate toggles | Green/brown pills, color only | Same as above |
| `MiniMotorwaysSettings.h` | Night Mode/Vibration/etc circles | Filled vs empty circle | Add checkmark inside filled; add ON/OFF label beside |
| `CozyCafe.h` | Customer patience bars | Green fill vs empty, tiny percentage | Enlarge percentage text; add warning icon when low |
| `KartSelect.h` | SPD/ACC/HND/WGT stat bars | Red/orange/green/blue bars, no numbers | Add numeric values (e.g., "7/10") beside each bar |
| `ShopInterface.h` | Stock column low-stock highlight | Red text for low stock | Add "Low!" suffix or warning icon |
| `NeonStrike.h` | Health/armor bars | Cyan/blue fill only | Already has numeric values; ensure contrast |
| `RaceResults.h` | Point values (+15, +12) | Green text, same for all | Add bold/icon for player row specifically |
| `EmpireTycoon.h` | Trend indicators "^" | Text "^" meaning trend up | Replace with colored arrow icon ↑ |
| `SportsSettings.h` | Teal preview bars | Mini bars with unclear meaning | Add legend or tooltip |
| `ParcelCorpsSettings.h` | Icon legend colors | Colored dots for categories | Add text labels inline |

### Component Galleries

| Screen | Element | Current | Fix |
|--------|---------|---------|-----|
| `CheckboxShowcase.h` | Checked state | "v" text character | Use ✓ checkmark; add background highlight on selected rows |
| `Forms.h` | Checkbox checked state | "v" text, same bg for on/off | Add background color change for checked items |
| `ToggleSwitchShowcase.h` | Toggle states | Green/gray toggle, labeled | Good — already uses text labels. Low priority. |

## Vendor-Level Fix Opportunities

1. **Toggle switch component**: Already supports text labels. No vendor change needed.
2. **Checkbox component**: The "v" checkmark is set in `imm_components.h`. Could upgrade to proper "✓" character.
3. **Progress bar component**: Could add optional numeric label rendering beside/inside the bar.

## Recommended Priority

| Priority | Action | Screens |
|----------|--------|---------|
| **P0** | Change checkbox "v" to "✓" in vendor code | All checkbox screens |
| **P1** | Add numeric values to stat/progress bars | KartSelect, CozyCafe, NeonStrike |
| **P1** | Add secondary indicators to toggle states | AngryBirds, Casual, MiniMotorways |
| **P2** | Add low-stock icons/text | ShopInterface |
| **P2** | Replace "^" with arrow icons | EmpireTycoon |

## Estimated Effort

- Vendor checkbox fix: 1 line change
- Per-screen toggle improvements: ~5-10 lines each
- Stat bar numeric labels: ~3-5 lines each
- **Total: ~60-80 lines across 14 screens + 1 vendor change**
