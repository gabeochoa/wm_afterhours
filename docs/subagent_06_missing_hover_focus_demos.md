# Subagent 6: Missing Hover/Focus/Pressed State Demonstrations

**Severity:** High (3/4 auditor agreement)
**Affected screens:** 10+

---

## Problem

Gallery/showcase screens only display rest states. These screens are meant to be role-model reference implementations but don't show what components look like on hover, keyboard focus, or pressed. Users of the library can't evaluate interaction design without manually clicking through every element.

## Affected Screens

| Screen | Components Shown | States Demonstrated | Missing |
|--------|-----------------|--------------------:|---------|
| `Buttons.h` | Normal, Accent, Secondary, Disabled | Rest only | Hover, Focus, Pressed |
| `ButtonVariantsShowcase.h` | Filled, Outline, Ghost × 4 states | Rest only | Hover, Focus, Pressed |
| `Cards.h` | Surface, Primary, Sharp, Custom | Rest only | Hover, Elevated |
| `CheckboxShowcase.h` | Primary, Secondary, Accent, Disabled | Rest + Checked | Hover, Focus ring |
| `Forms.h` | Sliders, Checkboxes, Progress | Rest only | Drag, Focus ring |
| `ToggleSwitchShowcase.h` | Basic, Custom styled | Rest + Toggled | Hover glow, Focus ring |
| `RadioGroupShowcase.h` | Radio options | Rest + Selected | Hover, Focus ring |
| `PaginationShowcase.h` | Page buttons | Rest + Active | Hover |
| `TabContainerShowcase.h` | Tabs | Rest + Selected | Hover |
| `NavigationBarShowcase.h` | Nav items | Rest + Active | Hover |

## Vendor-Level Fix Opportunities

### 1. Forced-state API (HIGH IMPACT)

Add a way to force a component into a specific interaction state for demonstration purposes:

```cpp
// Proposed API
auto config = ComponentConfig()
    .with_label("Hover State")
    .with_forced_state(UIState::Hovered);  // Renders as if hovered

auto config2 = ComponentConfig()
    .with_label("Focused State")
    .with_forced_state(UIState::Focused);  // Shows focus ring
```

This would allow gallery screens to display all states simultaneously without user interaction.

### 2. State annotation helper

A lighter approach — add a "state preview" section that programmatically triggers visual states:

```cpp
// Simpler approach: just apply the hover/focus colors directly
auto hover_config = ComponentConfig()
    .with_label("Hover")
    .with_custom_background(theme.accent_hover_color);
```

## Screen-Level Fixes (without vendor change)

Each gallery would add a new section showing forced states by manually applying hover/focus colors:

```cpp
// Example section for Buttons.h
gallery_section_header("Interaction States");

// Simulate hover by using hover color directly
ui::button(ComponentConfig()
    .with_label("Hovered")
    .with_custom_background(theme.primary_hover));

ui::button(ComponentConfig()
    .with_label("Pressed")
    .with_custom_background(theme.primary_pressed));

ui::div(ComponentConfig()
    .with_label("Focused")
    .with_border(focus_ring_color, h720(2.0f)));
```

## Recommended Priority

| Priority | Action | Impact |
|----------|--------|--------|
| **P1** | Add "Interaction States" section to Buttons + ButtonVariants | Highest-traffic gallery screens |
| **P2** | Add forced hover/focus to CheckboxShowcase, ToggleSwitchShowcase | Next tier |
| **P3** | Vendor: Add `with_forced_state()` API | Long-term, enables all galleries |

## Estimated Effort

- Per-screen manual state section: ~15-25 lines per screen
- 6 priority screens: ~120 lines total
- Vendor forced-state API: ~30-50 lines
- **Recommend: manual approach first (~120 lines), vendor API later**
