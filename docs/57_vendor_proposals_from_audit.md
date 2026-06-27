# Vendor-Level Proposals from Multi-Audit

These are vendor/afterhours-level changes identified by the 4-auditor design review. Each would fix the issue globally rather than per-screen.

---

## 1. Checkbox "v" → "X" (P0, ~1 line) — DONE

**Source:** Subagent 1 — Color as Sole State Indicator

The checkbox component in `imm_components.h` used the ASCII character `"v"` for the checked state. This was ambiguous — it could be confused with the letter V.

**Fix applied:** Changed default to `"X"` in both `checkbox_no_label()` and toggle switch. Also fixed toggle switch unchecked indicator from `"x"` to `" "` for consistency.

**Impact:** All checkbox screens automatically improve.

---

## 2. Auto 50% Opacity for Disabled Components (P0, ~15 lines)

**Source:** Subagent 3 — Disabled States Too Subtle

Currently `with_disabled()` sets a flag but doesn't enforce visual styling. Each screen must manually style disabled elements. This leads to inconsistency — some screens barely dim disabled elements.

**Proposed fix:** In the rendering pipeline, when `is_disabled` is true, automatically apply a 50% opacity multiplier to the component's background and text colors.

```cpp
// In rendering code (e.g. component_render.h)
if (config.is_disabled) {
    bg_color.a = static_cast<unsigned char>(bg_color.a * 0.5f);
    text_color.a = static_cast<unsigned char>(text_color.a * 0.5f);
}
```

**Impact:** Fixes 10+ screens at once. No per-screen code needed.

---

## 3. Gallery Section Header Helpers (P1, ~20 lines)

**Source:** Subagent 5 — Section Headers Too Small in Gallery Screens

Every gallery/showcase screen uses small inline text for section headers. There's no shared helper, so each screen has a slightly different style.

**Proposed fix:** Create a shared `gallery_helpers.h` with reusable section header and title components:

```cpp
inline auto gallery_section_header(UIContext& ctx, EntityID parent, int id,
                                   const std::string& title) {
    return div(ctx, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(32.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_label(title)
            .with_custom_background(Color(40, 60, 80, 200))
            .with_auto_text_color(true)
            .with_padding(Padding{.left = h720(12.0f)})
            .with_margin(Margin{.top = h720(8.0f), .bottom = h720(4.0f)}));
}

inline auto gallery_title(UIContext& ctx, EntityID parent, int id,
                          const std::string& title) {
    return div(ctx, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(48.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_label(title)
            .with_custom_background(Color(0, 120, 130, 255))
            .with_auto_text_color(true)
            .with_padding(Padding{.left = h720(16.0f)}));
}
```

**Impact:** 15+ gallery screens could adopt. Consistent visual hierarchy. Single place to update.

---

## 4. Gallery Status Bar Helper (P1, ~15 lines)

**Source:** Subagent 9 — Debug-Like Status Text

Gallery screens display dynamic state as small debug-looking text ("Total clicks: 0", "Selected: Group 2/4 | MinMax 1/3"). Already partially addressed in screen-level fixes, but a shared helper would ensure consistency.

**Proposed fix:** Add to the same `gallery_helpers.h`:

```cpp
inline auto gallery_status_bar(UIContext& ctx, EntityID parent, int id,
                               const std::string& text) {
    return div(ctx, mk(parent, id),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(36.0f)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_label(text)
            .with_custom_background(colors::lighten(ctx.theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_padding(Padding{.left = h720(12.0f)}));
}
```

**Impact:** 8+ gallery screens. Eliminates debug-style text.

---

## 5. Tab Component Default Styling Improvements (P1, ~15 lines)

**Source:** Subagent 7 — Weak Active/Selected Tab Indication

Tab containers don't have a strong enough default selected-tab style. Active tabs should have:
- Filled accent background or thick accent bottom bar (3-4px)
- Higher contrast text color
- No size changes (size-based selection shifts layout — anti-pattern)

**Proposed fix:** Update tab container defaults:

```cpp
// Active tab
active_tab_config
    .with_custom_background(theme.accent)
    .with_auto_text_color(true)
    // Bottom accent bar, 3px
    ;

// Inactive tab
inactive_tab_config
    .with_custom_background(Color::transparent())
    .with_custom_text_color(theme.font_muted);
```

**Impact:** All screens using tab container component. Individual game mockup screens use custom tabs, so this mainly affects gallery screens.

---

## 6. ValidateMinTouchTarget Validator (P1, ~20 lines)

**Source:** Subagent 8 — Small Touch/Click Targets

Add a validation rule that warns when interactive components (buttons) are below 44px minimum touch target size at 720p.

**Proposed fix:**

```cpp
struct ValidateMinTouchTarget : System<> {
    void for_each_with(Entity& entity, UIComponent& comp, IsButton&) {
        float min_w = h720_value(44.0f);
        float min_h = h720_value(44.0f);
        if (comp.computed_size.width < min_w || comp.computed_size.height < min_h) {
            report_violation("TouchTarget", entity,
                fmt::format("Button {}x{} below 44px minimum",
                    comp.computed_size.width, comp.computed_size.height));
        }
    }
};
```

**Impact:** Catches all future violations automatically during `--validate` runs. Currently all screens pass manually, but this prevents regressions.

---

## 7. `with_forced_state()` API (P3 / Future)

**Source:** Subagent 6 — Missing Hover/Focus/Pressed State Demonstrations

Gallery screens only show rest states. A `with_forced_state()` API would allow rendering components as if hovered/focused/pressed without user interaction:

```cpp
auto config = ComponentConfig()
    .with_label("Hover State")
    .with_forced_state(UIState::Hovered);
```

**Complexity:** Medium. Requires plumbing through the rendering pipeline to skip actual hover detection and apply forced state visuals.

**Alternative (simpler):** Screens can manually apply hover colors as custom backgrounds today. This works for demonstrating visual states but doesn't exercise the actual hover logic.

---

## 8. Tooltip Support (P3 / Future)

**Source:** Subagent 10 — Cryptic Icons and Abbreviations Without Labels

Add hover tooltips for icon-only elements as a fallback:

```cpp
auto config = ComponentConfig()
    .with_label("S")
    .with_tooltip("Sea Salt");  // Shows on hover after delay
```

**Complexity:** High. Requires:
- Tooltip render layer (always on top)
- Hover delay tracking
- Positioning logic (don't go off screen)
- ~100+ lines

**Alternative:** Use inline text labels (already done in screen-level fixes).

---

## Summary

| # | Proposal | Priority | Effort | Status |
|---|----------|----------|--------|--------|
| 1 | Checkbox "v" → "X" | P0 | 1 line | **DONE** |
| 2 | Auto disabled opacity | P0 | ~15 lines | **DONE** (theme.h disabled_opacity) |
| 3 | Gallery section header helpers | P1 | ~20 lines | Ready |
| 4 | Gallery status bar helper | P1 | ~15 lines | Ready |
| 5 | Tab default styling | P1 | ~15 lines | Ready |
| 6 | ValidateMinTouchTarget | P1 | ~20 lines | Ready |
| 7 | `with_forced_state()` | P3 | ~50 lines | Future |
| 8 | Tooltip support | P3 | ~100+ lines | Future |

**Total "Ready" vendor work: ~85 lines across 6 changes.**
