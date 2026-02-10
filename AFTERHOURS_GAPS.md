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

## Gaps

### 1. tab_container tab strip renders outside parent bounds

- **Issue**: When `tab_container()` is used with `with_absolute_position()`, the tab strip (the row of tab buttons) renders at the top of the screen instead of relative to the container's position. Moving the container's y-position has no effect on the tab strip position.
- **Affected screens**: `DeadSpaceSettings.h` — the 8 settings category tabs clip at the top screen edge
- **Workaround**: Replace `tab_container()` with manually-created tab buttons in a row, giving us full positioning control. See `DeadSpaceSettings.h` "MANUAL TAB BUTTONS" section.
- **Ideal fix**: `tab_container()` should respect the parent's absolute position and constrain its tab strip within its declared bounds. The tab strip should be a child of the container, not rendered at a screen-relative position.

### 2. toggle_switch creates sibling entities that consume parent layout space

- **Issue**: `toggle_switch()` creates internal entities (track, thumb, label) as siblings in the parent's entity namespace rather than children of the toggle entity. This means each toggle_switch consumes more vertical space in a flex column than its declared height (e.g., a 42px toggle actually takes ~60px of layout space). When a flex column doesn't use `with_no_wrap()`, excess content wraps to a new column (right side), causing elements to render outside their expected container.
- **Affected screens**: `ToggleSwitchShowcase.h` — disabled toggles and status bar were rendering at the top-right corner of the screen, outside the card
- **Workaround**: (1) Add `with_no_wrap()` to the card to prevent wrapping, (2) increase card height to 95% of screen, (3) remove the status bar that couldn't fit due to the entity overhead
- **Ideal fix**: `toggle_switch()` should create its track/thumb/label as nested children of the toggle entity, not as siblings in the parent. This would keep layout height predictable at the declared size.

