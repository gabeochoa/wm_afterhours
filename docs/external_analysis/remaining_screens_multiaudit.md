# Consolidated Design Audit: Remaining Screens (Cross-Cutting Themes)

**Screens covered:** accessibility, aim_chat, animation_basic, animation_declarative, animation_interactive, animation_looping, animation_spring, auto_text_color, bevel_borders, circular_progress, colors, decorative_frame, decorators, example_borders, flex_alignment, images, language_demo, layout, meters_gauges, navigation_bar_demo, nine_slice_borders, pagination, radio_buttons, scroll_view, self_align, separators, setting_row_showcase, simple_button, tab_container, tabbing, text, text_input, text_overflow, text_shadow, text_stroke, themes, toasts, toggle_switches

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

These screens are component galleries, system demos, and animation showcases. They share common patterns. Rather than repeat the same issues 38 times, here are the cross-cutting themes ranked by frequency.

---

## Agreed by 4/4 auditors (appears in nearly all screens)

### 1. Section headers are consistently too small across all gallery screens
**Problem:** Every gallery screen uses small all-caps text for section headers (BASIC MODALS, SLIDERS, etc.) that fails to create visual hierarchy. Headers blend with content text.
**Flagged by:** Apple, Google, Sun, Microsoft — across all gallery screens
**Fix:** Establish a global heading style for gallery screens — 20px+ bold text with clear spacing above/below. Apply consistently to every gallery.

### 2. Disabled states are consistently too subtle
**Problem:** Every component that shows a disabled variant (buttons, checkboxes, toggles, sliders, text inputs) uses a barely-perceptible styling change. Disabled elements look nearly identical to their enabled counterparts.
**Flagged by:** Apple, Google, Sun, Microsoft — across buttons, checkboxes, forms, toggle_switches
**Fix:** Define a global disabled style — 50% opacity + "not-allowed" cursor + grayed background. Apply consistently to all component types.

---

## Agreed by 3/4 auditors (appears in most screens)

### 3. Gallery screens don't demonstrate hover/focus/pressed states
**Problem:** Most component galleries show rest states only. These screens are meant to be reference implementations but don't show the full interaction lifecycle. Users can't see what happens on hover or keyboard focus without interacting.
**Flagged by:** Apple, Google, Sun — across buttons, cards, checkboxes, forms, toggle_switches
**Fix:** Add an "Interaction States" section to key gallery screens showing hover, focus, and pressed states side-by-side (using forced states or annotations).

### 4. Status/counter text at bottom of demos is too small and looks like debug output
**Problem:** "Total clicks: 0", "Selected: Group 2/4", "BG Clicks: 0" — these status lines use small monospace text that looks like debug output rather than user-facing UI.
**Flagged by:** Google, Sun, Microsoft — across buttons, checkboxes, forms, modals
**Fix:** Style status bars as proper UI elements — larger text, themed background, clear labels. Or move to a dedicated "Output" section with proper framing.

---

## Agreed by 2/4 auditors (appears in some screens)

### 5. Dark theme screens have low contrast on secondary/muted text
**Problem:** Several screens using dark themes (neon_dark, etc.) have muted text that falls below 4.5:1 contrast. This includes helper text, secondary labels, and descriptions.
**Flagged by:** Apple, Google — across toggle_switches, animation demos, text demos
**Fix:** Audit all muted text colors against their backgrounds. Ensure minimum 4.5:1 for body text and 3:1 for large text, even for secondary/muted styled text.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |

---

## Grand Total Across All Screens

The most common issues across ALL 62 screens, ranked:

| Rank | Issue | Screens Affected | Agreement |
|------|-------|-----------------|-----------|
| 1 | **Color as sole state indicator** (toggles, checkmarks, progress) | 14 | 4/4 |
| 2 | **Small/low-contrast text** (labels, descriptions, footers) | 20+ | 4/4 |
| 3 | **Disabled states too subtle** | 10+ | 4/4 |
| 4 | **Inconsistent element sizing** within same hierarchy level | 12 | 4/4 |
| 5 | **Section headers too small** in gallery screens | 15+ | 4/4 |
| 6 | **Missing hover/focus state demonstrations** | 10+ | 3/4 |
| 7 | **Weak active/selected tab indication** | 8 | 3/4 |
| 8 | **Small touch/click targets** (arrows, close buttons, icons) | 10 | 3/4 |
| 9 | **Debug-like status text** instead of styled output | 8 | 3/4 |
| 10 | **Cryptic icons/abbreviations** without labels | 6 | 3/4 |
