# Subagent 2: Small / Low-Contrast Text

**Severity:** Critical (4/4 auditor agreement)
**Affected screens:** 20+
**Staleness review (2026-02-10):** RALPH_TASK modified DeadSpaceSettings and EmpireTycoon but only fixed layout/positioning — font sizes and contrast were NOT changed. Full `screenshot_all_screens.py --quick` validation passes with 0 ContrastRatio and 0 MinFontSize engine warnings, but the engine thresholds are more lenient than the recommendations below. **All findings remain valid.**

---

## Problem

Many screens use text that is too small to read comfortably (below 12px at 720p) or has insufficient contrast against its background (below WCAG 4.5:1 ratio). This includes labels, descriptions, footer text, helper text, and status bars.

## Affected Screens & Specific Locations

### Critical (text < 12px or very low contrast)

| Screen | Element | Current Size | Fix |
|--------|---------|-------------|-----|
| `ParcelCorpsSettings.h` | Icon legend text | `h720(12.0f)` | Borderline — increase to `h720(14.0f)` |
| `ParcelCorpsSettings.h` | Resolution subtitle | `h720(12.0f)` | Increase to `h720(14.0f)` |
| `ParcelCorpsSettings.h` | Settings category icons | `h720(12.0f)` | Increase to `h720(14.0f)` |
| `CozyCafe.h` | Badge notification text | `h720(12.0f)` | Increase to `h720(14.0f)` |
| `CozyCafe.h` | "Customers Served: 25" | `h720(13.0f)` | Increase to `h720(14.0f)` |
| `CozyCafe.h` | Patience percentage | `h720(14.0f)` | OK size but low contrast on colored bg |
| `PotionCrafting.h` | Recipe descriptions | `h720(14.0f)` default | OK but low contrast on dark bg |
| `RaceResults.h` | Column headers | `h720(16.0f)` | OK size but very low contrast (gray on dark) |
| `RaceResults.h` | Controller prompts | `h720(14.0f)` | Increase to `h720(16.0f)` |
| `KartSelect.h` | Controller prompts | `h720(14.0f)` | Increase to `h720(16.0f)` |
| `DeadSpaceSettings.h` | Small sidebar items | Default | Increase to match other sidebar items |
| `RubberBanditsMenu.h` | Helper text + title | Default | Increase contrast dramatically |
| `NeonStrike.h` | Kill feed text | `h720(15.0f)` | Add background panel; increase size |

### Moderate (text 12-14px, could be larger)

| Screen | Element | Current Size | Fix |
|--------|---------|-------------|-----|
| `AngryBirdsSettings.h` | Build/Version/Player text | `h720(14.0f)` | Increase to `h720(16.0f)` or improve contrast |
| `PowerWashSettings.h` | Bottom prompts | `h720(14.0f)` | Increase to `h720(16.0f)` |
| `SportsSettings.h` | Anti-aliasing method value | `h720(16.0f)` | OK but truncated — allow wrapping |
| `FighterMenu.h` | Bottom prompt bar | Default | Increase size and spacing |
| `ShopInterface.h` | Bottom prompts | `h720(14.0f)` | Increase to `h720(16.0f)` |
| `EmpireTycoon.h` | Chat/notification text | `h720(14.0f)` | Increase to `h720(16.0f)` |

### Gallery Screens (section headers — see also Subagent 5)

All gallery screens use section header text that's too small. This is covered in detail by Subagent 5.

## Vendor-Level Fix Opportunities

1. **Global minimum font size enforcement**: Already have `ValidateMinFontSize` validator. Could lower threshold or make it configurable.
2. **Default font size floor**: Could set a minimum in `set_default_font()` that warns if below 12px.
3. **Contrast checker**: `ValidateComponentContrast` exists but may not catch text-on-similar-background cases.

## Recommended Priority

| Priority | Action | Screens |
|----------|--------|---------|
| **P0** | Fix all `h720(12.0f)` instances in non-decorative text | ParcelCorps, CozyCafe, DecoratorShowcase |
| **P1** | Increase controller prompt text to `h720(16.0f)` | RaceResults, KartSelect, ShopInterface, PowerWash |
| **P1** | Add contrast to DeadSpace sidebar small text | DeadSpaceSettings |
| **P1** | Fix RubberBandits contrast (yellow on yellow) | RubberBanditsMenu |
| **P2** | Add background panels behind NeonStrike kill feed | NeonStrike |

## Estimated Effort

- Font size bumps: 1 line each, ~20 changes
- Contrast fixes: 2-5 lines each, ~10 changes  
- Background panel additions: ~10 lines each, 2-3 screens
- **Total: ~60-80 lines across 20+ screens**
