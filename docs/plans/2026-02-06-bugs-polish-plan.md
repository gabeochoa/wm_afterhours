# Bugs & Polish Batch — Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Fix root-cause bugs in the Afterhours library and resolve all P0/P1 visual issues before adding new features.

**Architecture:** Bottom-up — fix library internals first (render const, absolute positioning, text bearing), then fix showcase visual bugs. All changes are in header files (C++23 header-only library).

**Tech Stack:** C++23, Raylib (graphics), Afterhours ECS (custom header-only library)

**Build command:** `make` from project root. Executable: `output/ui_tester`

**Run command:** `./output/ui_tester` (launches UI showcase). Use `--screen=<name>` to test specific screens.

---

### Task 1: Remove const constraint from render systems

The render system forces all render systems to be const, which breaks the immediate-mode UI pattern. Remove the const path entirely so render works like tick.

**Files:**
- Modify: `vendor/afterhours/src/core/system.h:484-519`

**Step 1: Modify `render()` to match `tick()` dispatch**

In `vendor/afterhours/src/core/system.h`, replace the `render()` method (lines 484-501) with a non-const version that matches `tick()`:

```cpp
void render(Entities &entities, const float dt) {
    for (auto &system : render_systems_) {
        if (!system->should_run(dt)) continue;
        system->once(dt);
        system->on_iteration_begin(dt);
        for (std::shared_ptr<Entity> entity : entities) {
            if (!entity) continue;
            if (system->include_derived_children)
                system->for_each_derived(*entity, dt);
            else
                system->for_each(*entity, dt);
        }
        system->on_iteration_end(dt);
        system->after(dt);
    }
}
```

Key changes from the original:
- Parameter: `const Entities &entities` → `Entities &entities`
- Remove: `const SystemBase &sys = *system;` (use `system->` directly)
- Remove: `const Entity &e = *entity;` (use `*entity` directly)
- Use non-const `for_each` / `for_each_derived` (calls mutable `for_each_with`)

**Step 2: Update `render_all()` to use mutable entities**

In the same file, change `render_all()` (lines 516-519):

```cpp
void render_all(const float dt) {
    auto &entities = EntityHelper::get_entities_for_mod();
    render(entities, dt);
}
```

Changed: `const auto &entities = EntityHelper::get_entities();` → `auto &entities = EntityHelper::get_entities_for_mod();`

**Step 3: Build and verify**

```bash
make -j8
```

Expected: Compiles successfully. May produce warnings about unused const overloads — that's fine, we'll clean those up in the next step.

---

### Task 2: Remove const_cast and mutable workarounds from render systems

Now that render systems are non-const, remove all the workarounds that existed because of the const constraint.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/rendering.h`
- Modify: `vendor/afterhours/src/plugins/texture_manager.h`
- Modify: `vendor/afterhours/src/plugins/ui/layout_inspector.h`
- Modify: `vendor/afterhours/src/plugins/ui/debug_overlay.h`
- Modify: `vendor/afterhours/src/plugins/ui/context.h`

**Step 1: Fix `rendering.h` const_cast usages**

In `vendor/afterhours/src/plugins/ui/rendering.h`:

Line 1267: Change `const_cast<FontManager &>(font_manager).set_active(cmp.font_name)` to `font_manager.set_active(cmp.font_name)` — update the enclosing `for_each_with_derived` signature from const to non-const.

Line 1287: Change `_update_scroll_view_content_size(const_cast<Entity &>(entity))` to `_update_scroll_view_content_size(entity)` — update parameter from `const Entity &` to `Entity &`.

Lines 1791, 1812, 1872: Same pattern — remove `const_cast` and change the enclosing method signatures from const to non-const.

For the `RenderDebugAutoLayoutRoots` struct (lines 643-651), remove `mutable` from all member variables:
- `mutable UIContext<InputAction> *context` → `UIContext<InputAction> *context`
- `mutable int level = 0` → `int level = 0`
- `mutable int indent = 0` → `int indent = 0`
- `mutable EntityID isolated_id = -1` → `EntityID isolated_id = -1`
- `mutable bool isolate_enabled = false` → `bool isolate_enabled = false`
- `mutable IsolationMode isolation_mode` → `IsolationMode isolation_mode`
- `mutable UIEntityMappingCache *cache` → `UIEntityMappingCache *cache`

Also update all `const` method overrides in this struct to non-const (e.g., `void once(float) const override` → `void once(float) override`).

For the `for_each_with_derived` overrides at lines 810, 1309, 1829: change signatures from `const` to non-const and update parameter types accordingly.

**Step 2: Fix `texture_manager.h` mutable workarounds**

In `vendor/afterhours/src/plugins/texture_manager.h`:

Line 205: `mutable Texture sheet` → `Texture sheet` (in `RenderSprites`)
Line 221: `mutable Texture sheet` → `Texture sheet` (in `RenderAnimation`)

Update the `for_each_with` methods in these structs from const to non-const.

**Step 3: Fix other mutable workarounds**

In `vendor/afterhours/src/plugins/ui/layout_inspector.h`:
- Lines 32-34: Remove `mutable` from `context`, `selected_id`, `panel_hovered`
- Update const method overrides to non-const

In `vendor/afterhours/src/plugins/ui/debug_overlay.h`:
- Line 34: Remove `mutable` from `context`
- Update const method overrides to non-const

In `vendor/afterhours/src/plugins/ui/context.h`:
- Line 223: `mutable std::vector<RenderInfo> render_cmds` → `std::vector<RenderInfo> render_cmds`

**Step 4: Build and verify**

```bash
make -j8
```

Expected: Compiles successfully with no `const_cast` remaining in render systems.

**Step 5: Run the app to verify**

```bash
./output/ui_tester --screen=buttons
```

Expected: UI renders correctly, buttons are clickable, no crashes.

---

### Task 3: Fix absolute positioning margin behavior

Margins on absolute elements should be position-only (CSS-like), not reduce element size.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/ui_core_components.h:111-119`

**Step 1: Update `rect()` to skip margin subtraction for absolute elements**

In `vendor/afterhours/src/plugins/ui/ui_core_components.h`, replace the `rect()` method (lines 111-119):

```cpp
Rectangle rect() const {
    if (absolute) {
        // Absolute positioning: margins are position offsets only, don't shrink size
        return Rectangle{
            .x = computed_rel[Axis::X] + computed_margin[Axis::left],
            .y = computed_rel[Axis::Y] + computed_margin[Axis::top],
            .width = fmaxf(0.f, computed[Axis::X]),
            .height = fmaxf(0.f, computed[Axis::Y]),
        };
    }
    // Flow layout: margins reduce available space (standard CSS content-box)
    return Rectangle{
        .x = computed_rel[Axis::X] + computed_margin[Axis::left],
        .y = computed_rel[Axis::Y] + computed_margin[Axis::top],
        .width = fmaxf(0.f, computed[Axis::X] - computed_margin[Axis::X]),
        .height = fmaxf(0.f, computed[Axis::Y] - computed_margin[Axis::Y]),
    };
};
```

**Step 2: Build and verify**

```bash
make -j8
```

Expected: Compiles. Any showcase screens using absolute positioning with margins should now render correctly instead of being invisible.

---

### Task 4: Fix text clipping — replace hardcoded bearing offset with actual glyph metrics

The hardcoded `TEXT_LEFT_BEARING_OFFSET = 8.0f` doesn't match actual font metrics, causing text to clip at container edges.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/rendering.h:33-54, 370-400`
- Modify: `vendor/afterhours/src/plugins/ui/render_primitives.h:612-626`
- Modify: `vendor/afterhours/src/font_helper.h` (add helper function)

**Step 1: Add a glyph bearing helper function**

In `vendor/afterhours/src/font_helper.h`, add this function in the `#ifdef AFTER_HOURS_USE_RAYLIB` block, after `measure_text_utf8()` (before the `#else`):

```cpp
// Get the left-side bearing (offsetX) for the first glyph in a string.
// Returns 0 if the string is empty or the glyph is not found in the font.
inline float get_first_glyph_bearing(const raylib::Font font, const char *text) {
  if (!text || text[0] == '\0') return 0.0f;
  int bytesProcessed = 0;
  int codepoint = raylib::GetCodepoint(text, &bytesProcessed);
  int glyphIndex = raylib::GetGlyphIndex(font, codepoint);
  if (glyphIndex < 0 || glyphIndex >= font.glyphCount) return 0.0f;
  return static_cast<float>(font.glyphs[glyphIndex].offsetX);
}
```

Also add a non-raylib stub in the `#else` block:

```cpp
inline float get_first_glyph_bearing(const Font, const char *) {
  return 0.0f;
}
```

**Step 2: Update `position_text_ex()` to use actual glyph bearing**

In `vendor/afterhours/src/plugins/ui/rendering.h`:

First, remove the hardcoded constant and TODO comment (lines 33-54). Replace with:

```cpp
// Left-side bearing is now calculated per-string using get_first_glyph_bearing()
// in font_helper.h. No more hardcoded offset.
```

Then in `position_text_ex()`, after `Font font = fm.get_active_font();` (line 295), add:

```cpp
float bearing = get_first_glyph_bearing(font, text.c_str());
```

Update the Left-aligned case (lines 374-380):

```cpp
case TextAlignment::Left:
    position = Vector2Type{
        .x = container.x + margin_px.x + bearing,
        .y = container.y + margin_px.y +
             (container.height - 2 * margin_px.y - text_size.y) / 2,
    };
    break;
```

Update the Center-aligned case (lines 381-392):

```cpp
case TextAlignment::Center: {
    float centered_offset = (container.width - 2 * margin_px.x - text_size.x) / 2;
    float text_x = container.x + margin_px.x + centered_offset;
    // Clamp so text never starts before container left edge
    text_x = std::max(container.x + margin_px.x, text_x);
    position = Vector2Type{
        .x = text_x,
        .y = container.y + margin_px.y +
             (container.height - 2 * margin_px.y - text_size.y) / 2,
    };
    break;
}
```

Note: Center-aligned text no longer adds bearing offset — centering should be symmetric. The old `(TEXT_LEFT_BEARING_OFFSET / 2)` was an incorrect half-correction.

**Step 3: Update batched render text**

In `vendor/afterhours/src/plugins/ui/render_primitives.h`, in the `render_text()` method (around line 615), the alignment handling doesn't need the bearing offset either — the `position_text_ex()` already computed the correct position. No changes needed here unless the batched renderer overrides the position. Verify the center alignment code at line 618 doesn't re-add an offset:

```cpp
if (cmd.data.text.alignment == TextAlignment::Center) {
    Vector2Type textSize = measure_text_utf8(font, cmd.data.text.text, fontSize, spacing);
    float centered_x = cmd.data.text.rect.x + (cmd.data.text.rect.width - textSize.x) / 2.0f;
    startPos.x = std::max(cmd.data.text.rect.x, centered_x);
    startPos.y = cmd.data.text.rect.y + (cmd.data.text.rect.height - textSize.y) / 2.0f;
}
```

This looks correct — no bearing offset applied. Leave as-is.

**Step 4: Build and verify**

```bash
make -j8
```

Expected: Compiles. Text should no longer clip at left edges on any screen.

**Step 5: Visual verification**

```bash
./output/ui_tester --screen=language_demo
./output/ui_tester --screen=dead_space_settings
./output/ui_tester --screen=modals
./output/ui_tester --screen=empire_tycoon
```

Expected: No text clipping at left edges. Title text, sidebar items, and currency displays should be fully visible.

---

### Task 5: Fix text rendering for label-only elements (no background color required)

Text elements shouldn't need an explicit background color to render.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/rendering.h:1290-1297`

**Step 1: Update render condition to include label-only elements**

In `vendor/afterhours/src/plugins/ui/rendering.h`, check the render gate at line 1290. It currently reads:

```cpp
if (entity.has<HasColor>() || entity.has<HasLabel>() ||
    entity.has<ui::HasImage>() ||
    entity.has<texture_manager::HasTexture>() ||
    entity.has<FocusClusterRoot>() ||
    entity.has<HasCircularProgressState>() ||
    entity.has<HasScrollView>()) {
  render_me(context, font_manager, entity);
}
```

This already includes `entity.has<HasLabel>()`, so label-only elements DO enter the render path. The issue may be elsewhere — check the `render_me()` function to see if it skips drawing when there's no `HasColor`. Look at where background is drawn and ensure it gracefully handles missing `HasColor` (just skip the background draw, still draw text).

If the background drawing code requires `HasColor` and crashes/skips when it's missing, add a guard. The actual rendering code in `render_me()` around lines 1130-1165 likely draws background only if `entity.has<HasColor>()`, so label-only elements should just skip the background and draw text. Verify this is the case — if it already works, mark this task as done.

**Step 2: Build and verify**

```bash
make -j8
```

---

### Task 6: Add debug validation warnings for invalid property combinations

Warn developers when they use property combinations that produce broken UI.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/component_config.h`

**Step 1: Add validation warning in `with_absolute_position()`**

In `vendor/afterhours/src/plugins/ui/component_config.h`, update `with_absolute_position()` (lines 372-375) to check for margins:

```cpp
ComponentConfig &with_absolute_position() {
    is_absolute = true;
    // Warn if margins are already set — they become position offsets for absolute elements
    if (has_margin_set()) {
        log_warn("with_absolute_position() used with margins. "
                 "For absolute elements, margins are position offsets only "
                 "(they don't shrink the element). Consider using "
                 "with_translate() for clearer intent.");
    }
    return *this;
}
```

Check if `has_margin_set()` exists. If not, add a simple check: any non-default margin values indicate margins were set. Alternatively, just add a `bool margin_was_set = false;` flag that `with_margin()` sets to true.

**Step 2: Build and verify**

```bash
make -j8
```

Expected: Compiles. May see new warnings in the console output for screens that combine margins with absolute positioning — this is expected and useful.

---

### Task 7: Fix toggle switch track visibility

Toggle track color blends into the surface, making it nearly invisible.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/imm_components.h:743-745`

**Step 1: Improve track color contrast**

In `vendor/afterhours/src/plugins/ui/imm_components.h`, update the track color calculation (lines 743-745):

Current:
```cpp
Color track_off = colors::lighten(theme.secondary, 0.5f);
Color track_on = colors::lighten(theme.accent, 0.5f);
```

Replace with a darker off-state that's clearly visible against any surface:
```cpp
Color track_off = colors::darken(theme.secondary, 0.3f);
Color track_on = colors::lighten(theme.accent, 0.3f);
```

The off-state track needs to be DARKER than the surface to stand out, not lighter. The on-state can remain lighter since the accent color typically contrasts well.

**Step 2: Build and verify**

```bash
make -j8 && ./output/ui_tester --screen=toggle_switch_showcase
```

Expected: Toggle tracks are clearly visible in both on/off states across all themes.

---

### Task 8: Fix checkbox panel overflow

Right panel in checkbox showcase overflows its container.

**Files:**
- Modify: `src/systems/screens/CheckboxShowcase.h:256-263`

**Step 1: Read the full file to understand the layout**

Read `src/systems/screens/CheckboxShowcase.h` to understand the container sizing and how the right column's children are sized. The issue is likely that the container card height is too small for the number of checkboxes at their current sizes, or the column doesn't clip/scroll.

**Step 2: Fix by adding scroll or reducing content size**

The most appropriate fix depends on the exact layout. Options:
1. Add `.with_clip_children(true)` to the right column to clip overflow
2. Reduce the height of individual checkboxes in the right panel
3. Increase the card container height to fit all content

Choose the simplest fix that resolves the overflow without breaking the demo's visual purpose.

**Step 3: Build and verify**

```bash
make -j8 && ./output/ui_tester --screen=checkbox_showcase
```

Expected: No content overflows the right panel boundary.

---

### Task 9: Fix theme switcher "Cozy Kraft" contrast

Selected theme button uses yellow/gold text on yellow background.

**Files:**
- Modify: `src/systems/screens/Themes.h:148-162`

**Step 1: Investigate the actual contrast issue**

The code at line 155 already uses `.with_auto_text_color(true)` which should pick a contrasting text color. The issue may be that when "Cozy Kraft" is selected, the theme is applied and the Accent color ({225, 165, 165}) is used as the button background, but `auto_text_color` is comparing against the WRONG background.

Check how `with_auto_text_color(true)` sets the `background_hint` — it should use the button's actual background color, not the theme primary. If the `background_hint` is being set incorrectly (using primary instead of accent), fix the hint to use the button's actual rendered background color.

The fix may be to explicitly set the text color for the selected state button:

```cpp
if (selected) {
    // When selected, use accent background — ensure text contrasts with it
    Color accent = context.theme.accent;
    btn_config.with_text_color(
        colors::auto_text_color(accent, context.theme.font, context.theme.darkfont));
}
```

**Step 2: Build and verify**

```bash
make -j8 && ./output/ui_tester --screen=themes
```

Expected: "Cozy Kraft" selected button has readable text that contrasts with its background. Switch between all themes to verify none have contrast issues.

---

### Task 10: Audit and fix missing focus states

Ensure all interactive components render focus rings when focused.

**Files:**
- Modify: `vendor/afterhours/src/plugins/ui/imm_components.h` (if any components are missing FocusClusterRoot)

**Step 1: Audit which components have focus support**

Based on the code exploration, these components already have focus support:
- Checkbox rows (line 411: `FocusClusterRoot`)
- Toggle switch rows (line 671: `FocusClusterRoot`)
- Slider containers (line 1029: `FocusClusterRoot`)
- Dropdown/combo boxes (line 1245: `FocusClusterRoot`)
- Buttons — check if they use `visual_focus_id` matching

Verify by running the app and tabbing through components:

```bash
./output/ui_tester --screen=toggle_switch_showcase
```

Tab through all toggle switches and verify each one gets a visible focus ring.

**Step 2: Fix any components missing focus support**

If any interactive component doesn't show a focus ring when tabbed to, add `FocusClusterRoot` to its container entity and `InFocusCluster` to its children, following the same pattern as checkbox (imm_components.h line 411).

**Step 3: Build and verify**

```bash
make -j8
```

Expected: All interactive components show focus rings when navigated to via Tab/Shift+Tab.

---

### Task 11: Final verification

**Step 1: Full build**

```bash
make -j8
```

Expected: Compiles with no errors. Any new warnings should be intentional (debug validation warnings).

**Step 2: Run full app and spot-check screens**

```bash
./output/ui_tester
```

Navigate through several screens checking:
- No text clipping at edges
- Toggle switches are visible
- Checkbox panels don't overflow
- Focus rings appear on all interactive elements
- Theme switching shows readable text on all themes

**Step 3: Run existing tests**

```bash
./output/ui_tester --run-test SimpleButtonClickTest
./output/ui_tester --run-test TabbingTest
./output/ui_tester --run-test FontConfigTest
```

Expected: All tests pass.
