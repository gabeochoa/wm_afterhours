# Focus Ring Audit - Consolidated Report

**Date:** 2026-02-15
**Screens audited:** 72
**Method:** Headless tab-through test (16 tab presses per screen, screenshots at each step)
**Screenshots:** `output/focus_test/{screen}/tab_{0..15}.png`

---

## Category 1: Working Well (31 screens)

Focus ring visible, progression correct, wrapping works.

| Screen | Notes |
|--------|-------|
| tabbing | Best implementation — bright yellow ring, excellent contrast |
| buttons | Clean progression through all 14 buttons, wraps correctly |
| toggle_switches | All 6 toggles traversed, disabled correctly skipped |
| checkboxes | All checkboxes traversed, disabled correctly skipped |
| forms | Sliders, toggles, buttons all reachable |
| text_input | Inputs + buttons, cursor feedback on text fields |
| stepper_showcase | All steppers reachable |
| pagination | Every arrow and page button individually focusable |
| fighter_menu | Logical order, wraps correctly |
| kirby_options | Clear ring on pastel background |
| angry_birds_settings | All settings items reachable |
| casual_settings | Focus stays within settings panel |
| flight_options | Tabs, content items, footer buttons all reachable |
| mini_motorways_settings | Tabs and settings all reachable |
| parcel_corps_settings | All interactive elements reachable |
| sports_settings | Excellent — row-level focus + dynamic help panel |
| kart_select | Selection state and focus state visually distinct |
| shop_interface | Tabs, items, action buttons all reachable |
| potion_crafting | Tabs, recipes, brew button all reachable |
| empire_tycoon | Sidebar, icons all reachable |
| race_results | 3 action buttons cycle cleanly |
| aim_chat | Input field + buttons reachable |
| toasts | All 9 trigger buttons reachable |
| navigation_bar_demo | All arrow buttons reachable |
| themes | Theme buttons + preview buttons reachable |
| auto_text_color | All 16 swatches traversed correctly |
| adaptive_scaling | 5 elements cycle cleanly |
| layout_bug_repros | Tabs, toggles, buttons all reachable |
| language_demo | Language buttons + menu buttons, good visibility |
| setting_row_showcase | Toggles, steppers, sliders all reachable |
| animation_declarative | 11 boxes cycle correctly |

---

## Category 2: Low Visibility (14 screens)

Focus ring works but is hard to see due to contrast, thinness, or theme conflicts.

| Screen | Issue |
|--------|-------|
| tab_container | Low contrast ring on light gray background |
| cozy_cafe | Ring matches card borders — nearly indistinguishable |
| rubber_bandits_menu | Light ring on bright yellow background |
| deadspace_settings | Too thin against near-black background |
| powerwash_settings | Thin ring, small arrow button targets hard to see |
| islands_trains_settings | Thin low-contrast ring on sage green background |
| accessibility | White ring on light-colored blocks has reduced contrast |
| animation_interactive | Thin dark outline, low contrast on dark bg |
| animation_spring | Ring invisible when elements at small animation sizes |
| animation_basic | Thin dark ring on teal button, low contrast (also only 1 element) |
| file_tree | Inconsistent styles between toolbar buttons and list items |
| simple_button | Dark outline blends with "Outline" button's own border |
| modals | Buttons clipped at right edge of viewport, ring partially hidden |
| button_variants | Some ghost/outline variants lack visible focus style |

---

## Category 3: Focus Progression Bugs (3 screens)

Focus ring exists but navigation is broken.

| Screen | Issue |
|--------|-------|
| **flex_alignment** | Focus stuck on "All" tab — Tab key never advances |
| **islands_trains_settings** | Focus trapped in slider bar segments (each notch is a tab stop, consumes all 16 tabs on 1 control) |
| **images** | Only 3 of 8+ interactive elements are in tab order |

---

## Category 4: No Focus Ring — Has Interactive Elements (11 screens)

These screens have buttons/controls that should be focusable but show no focus ring.

| Screen | Interactive Elements Present |
|--------|----------------------------|
| **radio_buttons** | 3 radio groups with individual radio options |
| **scroll_view** | "Invert Scroll" button |
| **neon_strike** | Ability buttons (UAV, Recon, Shield, Strike), objective button |
| **absolute_positioning** | Translate buttons, corner pins, flow items |
| **layout** | Panel buttons, stacked items |
| **self_align** | FlexStart/Center/FlexEnd buttons |
| **text_overflow** | OK button |
| **meters_gauges** | Interactive control bar (Value: 65%) |
| **nine_slice_borders** | "Click Me!" button |
| **example_borders** | "Interactive Button" element |
| **horizontal_drag** | Drag targets (tags) |

---

## Category 5: No Focus Ring — Display Only (13 screens)

These screens are purely presentational with no interactive elements. No focus ring is expected.

| Screen | Content Type |
|--------|-------------|
| colors | Color swatch display |
| text | Typography showcase |
| text_shadow | Shadow effect demo |
| text_stroke | Stroke effect demo |
| circular_progress | Progress indicator demo |
| decorators | Decorator showcase |
| decorative_frame | Frame style demo |
| bevel_borders | Border style showcase |
| animation_looping | Looping animation demo (Breathe, Glow, Bounce, Spin) |
| separators | Separator widget demo |
| cards | Card styles gallery (may be non-interactive by design) |
| drag_drop | Kanban board (drag-only, no tab support) |
| hstack_showcase | Layout demo (only 4 toolbar items focusable, rest display-only) |
| vstack_showcase | Layout demo (only 4 sidebar items focusable, rest display-only) |

---

## Priority Summary

| Priority | Category | Count | Action |
|----------|----------|-------|--------|
| P0 | Focus stuck / trapped | 2 | Fix flex_alignment and islands_trains_settings |
| P1 | No ring on interactive elements | 11 | Add focus support to these screens |
| P2 | Low visibility | 14 | Improve ring contrast/thickness for these themes |
| P3 | Limited tab coverage | 1 | images screen missing elements in tab order |
| -- | Working well | 31 | No action needed |
| -- | Display-only (no ring expected) | 13 | No action needed |

---

## Top Recommendations

1. **Fix focus stuck bugs** (flex_alignment, islands_trains_settings) — these completely block keyboard navigation
2. **Add `with_skip_tabbing(false)` or ensure focusability** on the 11 screens with interactive elements that don't receive focus (radio_buttons is the most critical since it's a core input component)
3. **Improve ring contrast** — the dual-color ring implementation helps but some themes (cozy_cafe, islands_trains_settings) still have rings that blend into card borders or backgrounds
4. **Consider per-theme focus ring color overrides** — the tabbing screen's bright yellow ring is the gold standard; screens with light backgrounds need darker or more prominent rings
