# Afterhours Complaints & Gaps — Cross-Project Consolidated

Collected from all `~/p/` projects that use the afterhours framework.
De-duplicated and categorized. Items already tracked in other wm_afterhours docs
(46, 57, vendor_ui_sizing_issues) are noted but not repeated in detail.

**Sources:**
- `floatinghotel/docs/afterhours_gaps.md` (14 items)
- `floatinghotel/docs/afterhours_issues.md` (2 bugs)
- `floatinghotel/docs/afterhours-gaps.md` (6 primitives + 4 vendor bugs + 3 requests)
- `wordproc/docs/afterhours-feature-requests.md` (15 items)
- `kart-afterhours/docs/afterhours_gaps.md` (animation, e2e, UI, build, headless)
- `pharmasea/docs/` (ECS handle system)
- `endless_dance_chaos/docs/` (3D helpers, backend decoupling)

---

## BUGS (crash/incorrect behavior)

### B1. `to_ent()` crashes when widget entity is not in UI mapping
**Source:** floatinghotel
**Severity:** Critical
**Details:** `compute_size_for_child_expectation` calls `to_ent(widget.id)` which dereferences `mapping.end()` when the widget ID isn't in the AutoLayout mapping (e.g. entity 0 created before UI init).
**Fix:** Guard with `mapping.find()` check before dereference.
**Status:** Patched locally in floatinghotel vendor.

### B2. `simulate_click` doesn't auto-release mouse button
**Source:** floatinghotel
**Severity:** High
**Details:** `test_input::simulate_click()` sets `left_down = true` but never releases it. After the first click in an E2E test, all subsequent clicks see `prev_down = true` and never detect a new press transition.
**Fix:** Added `auto_release` field to MouseState; `reset_frame()` auto-releases when `press_frames` expires.
**Status:** Patched locally in floatinghotel vendor.

### B3. Text overflow ellipsis hangs with `expand()` sizing
**Source:** floatinghotel
**Severity:** High
**Details:** Using `with_text_overflow(Ellipsis)` on elements sized with `expand()` causes infinite hang. The binary search in truncation runs against a 0-width container before layout resolves.
**Status:** Known. Only safe with fixed pixel widths.

### B4. Toggle switch font size uses `h720().value` incorrectly
**Source:** wm_afterhours (vendor_ui_sizing_issues.md)
**Severity:** Medium
**Details:** `h720(12.0f).value` returns `0.0167` (screen_pct fraction), not 12px. Font system clamps to 12px minimum with warning.
**Status:** Fixed locally — changed to `12.0f`.

### B5. Negative container height when margins exceed component height
**Source:** wm_afterhours (vendor_ui_sizing_issues.md)
**Details:** `rect()` returns negative height when `computed_margin[Y] > computed[Y]`. Fixed with `fmaxf(0.f, ...)` clamp.
**Status:** Fixed locally.

### B6. Dropdown hardcoded 200x50 button group
**Source:** wm_afterhours (vendor_ui_sizing_issues.md)
**Details:** Dropdown falls back to `default_component_size` (200x50) when no explicit size is provided. Doesn't pass resolved size to button_group.
**Status:** Verified, not yet fixed upstream.

### B7. Slider handle overflow
**Source:** wm_afterhours (vendor_ui_sizing_issues.md)
**Details:** Handle height set to full slider config height, but background resolves slightly smaller. Changed to `percent(1.f)`.
**Status:** Fixed locally.

### B8. tab_container() position bug
**Source:** floatinghotel
**Details:** Tab strip renders at screen-absolute position, ignoring parent container bounds.
**Status:** Known upstream.

### B9. toggle_switch() layout issue
**Source:** floatinghotel
**Details:** Creates sibling entities that consume extra layout space, misaligning adjacent elements.
**Status:** Known upstream.

### B10. Checkbox internal layout overflow
**Source:** kart-afterhours
**Details:** `imm::checkbox` creates a `checkbox_row` with `NoWrap`, then adds children that are often taller than the row. Internal to the component — can't be fixed from caller side.
**Status:** Open.

### B11. `SINGLETON_FWD` macro fails inside struct/class scope
**Source:** kart-afterhours
**Details:** Expands to `inline std::shared_ptr<Type>` which is invalid inside a struct body (needs `static inline`). Affects `sound_system.h`.
**Status:** Workaround: manually expand with `static inline`.

### B12. `std::bad_variant_access` crash on shutdown (static destruction order)
**Source:** kart-afterhours
**Details:** Backend variant is destroyed before entity destruction runs, causing `std::visit` on dead variant to throw.
**Fix:** Explicitly call `delete_all_entities` and `graphics::shutdown()` before `main()` returns.
**Status:** Workaround applied. Needs engine-level cleanup API.

---

## MISSING UI PRIMITIVES

### P1. Draggable Divider / Split Pane
**Source:** floatinghotel
**Need:** Resizing adjacent panels (sidebar/main, side-by-side diff).
**Workaround:** Built app-local in `src/ui/split_panel.h`.
**Also tracked in:** 46_cross_project_feature_gaps.md (§37 resizable panels)

### P2. Tree Node
**Source:** floatinghotel
**Need:** Collapsible hierarchical file tree.
**Workaround:** Built app-local in `src/ui/tree_view.h`.

### P3. Dropdown Menu
**Source:** floatinghotel
**Need:** Click-to-open list below trigger element.
**Workaround:** Built app-local in `src/ui/menu_setup.h`.
**Also tracked in:** 46_cross_project_feature_gaps.md

### P4. Context Menu
**Source:** floatinghotel, wordproc
**Need:** Right-click popup at cursor position.
**Workaround:** App-local in both projects. ~50 lines of boilerplate each.
**Also tracked in:** 24_context_menu.md

### P5. Anchored Popup / Popover
**Source:** floatinghotel
**Need:** Popup relative to trigger element with auto-flip at edges.
**Workaround:** Reuse dropdown approach with manual position calc.

### P6. Tooltip Component
**Source:** wordproc, floatinghotel
**Need:** Hover delay, proper z-order, edge repositioning.
**Workaround:** Manual `is_hot()` + absolute div. No delay.
**Also tracked in:** 10_tooltip.md, 57_vendor_proposals.md §8

### P7. Menu Bar with Keyboard Accelerators
**Source:** wordproc
**Need:** Win95-style menu bar (File, Edit, View...) with nested dropdowns.
**Workaround:** Hand-built with `imm_menu.h`.
**Also tracked in:** 46_cross_project_feature_gaps.md §7

### P8. Status Bar Component
**Source:** wordproc
**Need:** Fixed-position bar with segmented info cells.
**Also tracked in:** 46_cross_project_feature_gaps.md §8

---

## MISSING RENDERING / VISUAL FEATURES

### R1. No Font Weight Support
**Source:** floatinghotel
**Need:** `with_font_weight(FontWeight::SemiBold)` for diff headers and status letters.
**Workaround:** Load separate bold font file and switch per-component.
**Also tracked in:** 17_font_loading.md

### R2. No Rich Text / Multi-Color Text in Single Label
**Source:** floatinghotel, wordproc
**Need:** Different colors within one label (e.g. status letter + filename, syntax highlighting).
**Workaround:** Bake into single string, sacrifice per-span coloring.
**Also tracked in:** 15_rich_text_label.md

### R3. Toolbar Icons / Bitmap Rendering
**Source:** wordproc
**Need:** Render 16x16 pixel-art icons inside buttons.
**Workaround:** Draw icons as raylib primitives in post-render overlay.

### R4. Per-Character Text Decoration (Access Keys / Mnemonics)
**Source:** wordproc
**Need:** Underline single character in label (e.g. "**F**ile" with F underlined).
**Workaround:** Draw underlines via raylib in overlay.

### R5. Custom Render Callback
**Source:** wordproc
**Need:** Draw arbitrary primitives inside a UI component's bounds, respecting render layers.
**Workaround:** Post-render overlay (causes z-order issues with popups).

### R6. Drop Shadows on Popup Elements
**Source:** wordproc
**Need:** Shadow on menus/popups for visual depth.
**Workaround:** None. Menus have no shadow.

### R7. Scrollbar Visual Customization
**Source:** wordproc
**Need:** Style scrollbar thumb/track (Win95 3D bevels, arrow buttons).
**Workaround:** Draw scrollbar manually with raw raylib.

### R8. Focus Indicators / Keyboard Focus Ring
**Source:** wordproc
**Need:** Visible dotted/solid focus rectangles on interactive elements (WCAG 2.4.7).
**Workaround:** None. Focus indicators are invisible.
**Also tracked in:** 16_focus_ring_styles.md

### R9. Animation / Motion Transitions
**Source:** wordproc
**Need:** Spring-based transitions for menus, toasts, state changes.
**Workaround:** None. Everything appears/disappears instantly.

### R10. Dropdown Triangle Glyph (▼)
**Source:** wordproc
**Need:** Render proper ▼ instead of appending " v" to label.
**Workaround:** Draw triangle programmatically in overlay.

---

## LAYOUT / SIZING GAPS

### L1. Row Flex Layout Broken with `expand()` Children
**Source:** floatinghotel
**Details:** Child with `expand()` in a Row flex consumes full parent width instead of remaining width after fixed siblings. CSS `flex: 1` behavior is expected.
**Workaround:** Bake all content into single label string on parent.
**Status:** Open.

### L2. Custom Colors Bypass Disabled Dimming
**Source:** floatinghotel
**Details:** `resolve_background_color()` returns custom colors as-is when `disabled=true`. Only `Theme::Usage`-based colors get dimming. Since most real apps use `with_custom_background()`, disabled state has no visual effect.
**Workaround:** Manually swap bg/text colors based on `enabled` parameter.
**Also tracked in:** 57_vendor_proposals.md §2 (now DONE via disabled_opacity)

### L3. `with_font_tier()` Only Supports `h720()` Scaling
**Source:** floatinghotel
**Details:** No way to use tier lookup with fixed `pixels()` sizing. Forces proportional scaling.
**Workaround:** Manual `with_font_size(pixels(theme.font_sizing.get(tier)))`.

### L4. Elements with opacity/translate as slide-in initial state
**Source:** kart-afterhours
**Details:** Common pattern of `with_opacity(0).with_translate(-2000, 0)` to hide elements before animation. If animation system doesn't run, elements are permanently hidden.
**Suggestion:** `with_slide_in()` config that layout system understands.

---

## E2E TESTING GAPS

### E1. text_input() requires InputAction enum values from host app
**Source:** floatinghotel
**Details:** `text_input::text_input()` expects `InputAction::TextBackspace`, `TextDelete`, `TextHome`, `TextEnd` which aren't part of afterhours and must be defined by the host app.
**Workaround:** Add required enum values to app's InputAction.

### E2. Clipboard shortcuts not wired in text_input
**Source:** floatinghotel
**Details:** Ctrl/Cmd+C/V/X not handled — requires manual action binding.
**Status:** Fixed in wm_afterhours (phases 1-9 implementation).

### E3. No right-click / multi-button mouse support in E2E testing
**Source:** wordproc
**Details:** `MouseState` only tracks left-click. `is_mouse_button_pressed(1)` returns false in test mode. No `simulate_right_click()` or `right_click` command.
**Workaround:** wordproc bypasses input simulation and directly sets context menu state.

### E4. No `disable_animations` / `enable_animations` built-in commands
**Source:** kart-afterhours
**Details:** Common need across kart, wm_afterhours, wordproc. Currently re-implemented per-project.

### E5. No `--screenshot-dir` CLI flag
**Source:** kart-afterhours
**Details:** Screenshots save to hardcoded `screenshots/` path. Need configurable output for baseline comparison workflows.

### E6. No `--e2e-speed` CLI flag
**Source:** kart-afterhours
**Details:** wordproc supports this to multiply wait durations for faster CI runs. Should be built into afterhours runner.

### E7. `expect_text` needs case-insensitive option
**Source:** kart-afterhours
**Details:** Game text casing varies by styling. Case-insensitive variant would reduce false failures.

---

## ANIMATION SYSTEM GAPS

### A1. No global disable/instant-complete flag
**Source:** kart-afterhours
**Details:** Need `animation::set_instant_mode(true)` for E2E testing, accessibility (reduce motion), and development iteration.
**Workaround:** Manual `animation_control::disabled` flag checked at every animation call site.

### A2. No way to clear/reset all animation tracks
**Source:** kart-afterhours
**Details:** Stale tracks from previous screens persist on screen switch. No `animation::clear_all()`.

---

## HEADLESS MODE GAPS

### H1. `ProvidesCurrentResolution` returns (0,0) in headless mode
**Source:** kart-afterhours
**Details:** `GetRenderWidth()`/`GetRenderHeight()` return 0 without a window, causing all layout to collapse.
**Workaround:** Custom system overrides resolution when zero detected.

### H2. `GetFontDefault()` returns invalid font in headless mode
**Source:** kart-afterhours
**Details:** Causes crashes in `DrawTextEx` and incorrect text measurements.
**Workaround:** Override `DEFAULT_FONT` and `UNSET_FONT` with fonts loaded via manual atlas generation.

---

## ECS / CORE ENGINE

### C1. EntityHandle system (slot + generation)
**Source:** pharmasea
**Details:** Need O(1) entity resolution, pointer-free serialization, stale reference detection. Full design doc exists in `pharmasea/docs/archive/afterhours_handle_system_migration.md` with 9-phase rollout plan.
**Status:** Design complete. Implementation not started.

### C2. 3D Drawing Helpers
**Source:** endless_dance_chaos
**Details:** Backend-agnostic 3D drawing primitives (draw_cube, draw_sphere, draw_plane, etc.) for both raylib and sokol backends. Full design in `endless_dance_chaos/docs/plans/2026-02-18-afterhours-3d-drawing-helpers-design.md`.
**Status:** Design complete. Implementation not started.

### C3. Backend Decoupling (raylib → afterhours abstractions)
**Source:** endless_dance_chaos
**Details:** ~360 direct `raylib::` calls in game code that should use afterhours wrappers. Full migration plan in `endless_dance_chaos/docs/plans/2026-02-18-backend-decoupling-design.md`.
**Status:** Design complete. Per-project migration needed.

---

## HOVER / INTERACTION GAPS

### I1. Hover State / Mouse-Over Callback
**Source:** wordproc
**Need:** `with_hover_background()` on ComponentConfig. Currently requires manual `is_hot()` check each frame.
**Status:** Partially resolved via `with_custom_hover_bg(Color)` in floatinghotel (commit bff4609). May not be propagated to all projects.

### I2. Cursor Change on Hover
**Source:** floatinghotel (resolved), wordproc
**Details:** `with_cursor(CursorType::Pointer)` etc.
**Status:** Resolved in floatinghotel (commit 27b535e). Available upstream.

---

## Summary by Severity

| Category | Count | Critical | High | Medium | Low |
|----------|-------|----------|------|--------|-----|
| Bugs | 12 | 2 | 3 | 4 | 3 |
| Missing Primitives | 8 | 0 | 3 | 4 | 1 |
| Rendering/Visual | 10 | 0 | 3 | 5 | 2 |
| Layout/Sizing | 4 | 0 | 1 | 2 | 1 |
| E2E Testing | 7 | 0 | 2 | 3 | 2 |
| Animation | 2 | 0 | 1 | 1 | 0 |
| Headless | 2 | 1 | 1 | 0 | 0 |
| ECS/Core | 3 | 0 | 0 | 2 | 1 |
| Interaction | 2 | 0 | 0 | 1 | 1 |
| **Total** | **50** | **3** | **14** | **22** | **11** |

## Items Already Fixed Upstream or Locally

| ID | Description | Where Fixed |
|----|-------------|-------------|
| floatinghotel §1 | Custom hover bg | afterhours bff4609 |
| floatinghotel §3 | Text overflow ellipsis | afterhours 120a9ed (but has expand() bug) |
| floatinghotel §4 | Flex gap | afterhours 37fe6f4 |
| floatinghotel §6 | Per-side border | afterhours 9eb0796 |
| floatinghotel §7 | Default transparent bg | afterhours 778f786 |
| floatinghotel §8 | Absolute child positioning | afterhours 1cb50a3 |
| floatinghotel §9 | Cursor on hover | afterhours 27b535e |
| floatinghotel §10 | Letter spacing | afterhours bff4609 |
| wordproc §7 | Sans-serif font | App-level (load Roboto) |
| wm_afterhours | Checkbox "v" → "X" | afterhours vendor |
| wm_afterhours | Auto disabled opacity | afterhours vendor (disabled_opacity) |
| wm_afterhours | Toggle font h720 fix | Local vendor patch |
| wm_afterhours | Negative rect height clamp | Local vendor patch |
| wm_afterhours | Slider handle overflow | Local vendor patch |
| wm_afterhours | Clipboard shortcuts in text_input | wm_afterhours phases 1-9 |
