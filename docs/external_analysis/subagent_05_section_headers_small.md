# Subagent 5: Section Headers Too Small in Gallery Screens

**Severity:** Critical (4/4 auditor agreement)
**Affected screens:** 15+ gallery/showcase screens
**Staleness review (2026-02-10):** RALPH_TASK did not change any section header sizes. **All findings remain valid.**

---

## Problem

Every gallery/showcase screen uses small all-caps text for section headers that fails to create visual hierarchy. Headers blend with content text, making it hard to scan the screen and understand the structure.

## Affected Screens

All gallery screens follow the same pattern. Here's the full list:

| Screen | Header Examples | Current Style |
|--------|----------------|---------------|
| `Buttons.h` | "States:", "Sizes:", "Group:", "Custom:" | Inline label, same size as content |
| `ButtonVariantsShowcase.h` | "Filled", "Outline", "Ghost", "Sizes", "Themes" | Inline label, no section separation |
| `CheckboxShowcase.h` | "With Label", "Box Only", "Disabled", "Multi-Select" | Small section headers |
| `Forms.h` | "Sliders", "Progress Bars", "Checkboxes" | Small teal headers |
| `Cards.h` | "Styles:", "Nested:", "Theme Colors:" | Tiny labels |
| `ModalShowcase.h` | "BASIC MODALS", "HELPER FUNCTIONS", "MODAL STACKING" | Small all-caps |
| `ToggleSwitchShowcase.h` | "Basic Toggles", "Custom Styled" | Section headers |
| `RadioGroupShowcase.h` | Section labels | Small text |
| `PaginationShowcase.h` | Section labels | Small text |
| `CircularProgressShowcase.h` | Section labels | Small text |
| `MetersGaugesShowcase.h` | Section labels | Small text |
| `ExampleSeparators.h` | Section labels | Small text |
| `SelfAlignShowcase.h` | Section labels | Small text |
| `NavigationBarShowcase.h` | Section labels | Small text |
| `SettingRowShowcase.h` | Section labels | Small text |

## Vendor-Level Fix Opportunity

### Gallery Section Header Helper

Create a reusable section header component for gallery screens:

```cpp
// Proposed helper in a shared gallery utilities header
inline void gallery_section_header(const std::string& title) {
    ui::div(ComponentConfig()
        .with_size(ComponentSize{percent(1.0f), h720(32.0f)})
        .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
        .with_label(title)
        .with_custom_background(Color(40, 60, 80, 200))
        .with_padding(Padding{.left = h720(12.0f)})
        .with_margin(Margin{.top = h720(8.0f), .bottom = h720(4.0f)}));
}

inline void gallery_title(const std::string& title) {
    ui::div(ComponentConfig()
        .with_size(ComponentSize{percent(1.0f), h720(48.0f)})
        .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
        .with_label(title)
        .with_custom_background(Color(0, 120, 130, 255))
        .with_padding(Padding{.left = h720(16.0f)}));
}
```

### Benefits
- Consistent heading style across all galleries
- Single place to update if design changes
- Reduces per-screen code duplication
- Establishes clear visual hierarchy

## Screen-Level Fixes (without vendor helper)

Each screen would need to manually increase header font size and add styling. This is ~3-5 lines per header × ~3-5 headers per screen × 15 screens = ~150-300 lines of repetitive changes.

**Strongly recommend the vendor helper approach instead.**

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P0** | Create shared gallery header helpers | Enables all fixes below |
| **P1** | Apply to all 15+ gallery screens | Consistent hierarchy everywhere |

## Estimated Effort

- Vendor helper: ~20 lines (new shared header)
- Per-screen adoption: ~2-3 line changes per header, ~5 headers per screen × 15 screens = ~150 line changes
- **Total: ~170 lines, but mechanical/safe changes**
