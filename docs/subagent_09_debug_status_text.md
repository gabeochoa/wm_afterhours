# Subagent 9: Debug-Like Status Text

**Severity:** Moderate (3/4 auditor agreement)
**Affected screens:** 8

---

## Problem

Status bars, counters, and result displays in gallery screens use small monospace text with pipe separators that looks like debug output rather than polished UI. These are meant to show users how to display dynamic state, but the presentation undermines the showcase quality.

## Affected Screens & Specific Locations

| Screen | Status Text | Current Style | Fix |
|--------|------------|---------------|-----|
| `Buttons.h` | "Total clicks: 0" | Small centered text | Style as a badge/chip with background |
| `ButtonVariantsShowcase.h` | "Total clicks: 0" | Small centered text | Same |
| `CheckboxShowcase.h` | "Selected: Group 2/4 \| MinMax 1/3" | Small text with pipe separator | Break into two styled badges |
| `Forms.h` | "Volume: 75% \| Resolution: 1920x1080 \| Quality: Medium" | Small status bar | Style as themed footer with sections |
| `ModalShowcase.h` | "Confirm: Pending \| FYI: Pending \| Confirms: 0 Cancels: 0" | Small text with pipes | Use a small card/table layout |
| `ModalShowcase.h` | "BG Clicks: 0" | Cryptic abbreviation | Spell out "Background Clicks: 0" |
| `PaginationShowcase.h` | Page state text | Small debug text | Style as info bar |
| `RadioGroupShowcase.h` | Selection state | Small debug text | Style as info bar |
| `AnimationInteractiveDemo.h` | Animation state values | Small text | Style as property inspector |

## Vendor-Level Fix Opportunity

### Status Bar Helper Component

Create a reusable "status bar" or "output panel" component for gallery screens:

```cpp
// Proposed helper
inline void gallery_status_bar(const std::string& text) {
    ui::div(ComponentConfig()
        .with_size(ComponentSize{percent(1.0f), h720(28.0f)})
        .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
        .with_label(text)
        .with_custom_background(Color(30, 40, 50, 200))
        .with_rounded_corners(RoundedCorners(h720(4.0f)))
        .with_padding(Padding{.left = h720(12.0f)})
        .with_margin(Margin{.top = h720(8.0f)}));
}

// Multi-value variant
inline void gallery_status_chips(
    const std::vector<std::pair<std::string, std::string>>& values) {
    // Renders as: [Volume: 75%] [Resolution: 1920x1080] [Quality: Medium]
    // Each in its own styled chip/badge
}
```

## Screen-Level Fixes (without vendor helper)

Each screen would restyle its status text:

| Screen | Change | Lines |
|--------|--------|-------|
| `Buttons.h` | Add background + padding to click counter | ~3 |
| `ButtonVariantsShowcase.h` | Same | ~3 |
| `CheckboxShowcase.h` | Split into two status elements with backgrounds | ~6 |
| `Forms.h` | Style as footer bar with sections | ~8 |
| `ModalShowcase.h` | Restyle both status lines + expand "BG" to "Background" | ~8 |
| `PaginationShowcase.h` | Add background styling | ~3 |
| `RadioGroupShowcase.h` | Add background styling | ~3 |

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P1** | Create shared `gallery_status_bar()` helper | Enables all fixes |
| **P1** | Apply to Forms, ModalShowcase (most visible) | High-traffic screens |
| **P2** | Apply to all remaining gallery screens | Consistency |

## Estimated Effort

- Shared helper: ~15 lines
- Per-screen adoption: ~3-8 lines × 8 screens = ~40 lines
- **Total: ~55 lines**
