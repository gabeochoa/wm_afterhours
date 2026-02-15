# Afterhours Library Gaps & Workarounds

Issues encountered in the `vendor/afterhours` library during UI work. We do NOT modify vendor code — instead we document gaps here and add workarounds in our source.

---

## Format

Each entry should include:
- **Issue**: What's wrong or missing
- **Affected screens**: Which screens hit this
- **Workaround**: What we did instead (with file + line references)
- **Ideal fix**: What afterhours should provide

---

## See Also

- `docs/vendor_ui_sizing_issues.md` — additional verified vendor bugs (dropdown sizing, toggle font size, slider handle overflow) with debug logging details and applied fixes

---

## Gaps

### 1. ~~tab_container tab strip renders outside parent bounds~~ (FIXED)

- **Issue**: When `tab_container()` is used with `with_absolute_position()`, the tab strip (the row of tab buttons) renders at the top of the screen instead of relative to the container's position. Moving the container's y-position has no effect on the tab strip position.
- **Root cause**: `with_absolute_position(x,y)` stored the position as a translate in `HasUIModifiers` (render-time only), but `compute_rect_bounds` used `parent.computed_rel` (which was 0,0) for child offsets.
- **Fix**: Added `absolute_pos_x/y` fields to `UIComponent`, set during `apply_visuals` in component_init.h, used in `compute_relative_positions` in autolayout.h.

### 2. ~~toggle_switch creates sibling entities that consume parent layout space~~ (NOT A BUG)

- **Original report**: `toggle_switch()` creates internal entities as siblings in the parent's entity namespace, consuming more vertical space than declared.
- **Investigation**: The internal entities (label, track, knob) ARE children of the toggle entity, not siblings. `mk(entity)` correctly creates them as children. The ToggleSwitchShowcase overflow was caused by the total content (title + sections + separators + toggles + checkboxes + disabled section) exceeding the card height, not by individual toggles taking extra space. A repro with 3 toggles at 42px in a 150px container confirmed they fit correctly.

### 3. Focus ring visibility on checkbox and toggle_switch (unverified)

- **Issue**: The UI review flagged that checkboxes and toggle switches don't show focus rings when tabbed to, while buttons do. The afterhours rendering code has focus ring support for multiple element types, so the rings may actually render — this could not be verified via static screenshots (no element is focused during headless capture).
- **Affected screens**: `CheckboxShowcase.h`, `ToggleSwitchShowcase.h`
- **Workaround**: None needed if focus rings render correctly. Needs manual verification with keyboard tabbing.
- **Ideal fix**: If focus rings don't render on checkbox/toggle, the rendering pipeline should treat them the same as buttons for focus ring drawing.

