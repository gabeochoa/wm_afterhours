# Accordion

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

## Description

An accordion is a vertical stack of interactive headings used to toggle the display of further information. Each item can be 'collapsed' (showing just a short label) or 'expanded' (showing full content).

**Also known as:** Collapse, Collapsible sections, Details, Disclosure, Expandable, Expander

## Use Cases in Game UI
- Settings categories that can be expanded/collapsed
- FAQ or help sections
- Inventory categories
- Quest log with expandable quest details
- Character stats/skills with expandable descriptions

## Suggested Implementation

```cpp
struct AccordionConfig {
    bool allow_multiple_open = false;  // Only one section open at a time
    bool animate_transition = true;
    float animation_duration = 0.2f;
};

ElementResult accordion(HasUIContext auto &ctx, EntityParent ep_pair,
                        AccordionConfig config = AccordionConfig());

ElementResult accordion_item(HasUIContext auto &ctx, EntityParent ep_pair,
                             const std::string &header,
                             bool &is_expanded,
                             ComponentConfig config = ComponentConfig());

// Usage:
accordion(ctx, mk(parent, 0)) {
    accordion_item(ctx, mk(current, 0), "Graphics Settings", graphics_expanded) {
        // Content when expanded
        slider(ctx, mk(current, 0), "Brightness", brightness);
        toggle(ctx, mk(current, 1), "Fullscreen", fullscreen);
    }
    accordion_item(ctx, mk(current, 1), "Audio Settings", audio_expanded) {
        slider(ctx, mk(current, 0), "Master Volume", master_vol);
        slider(ctx, mk(current, 1), "Music Volume", music_vol);
    }
}
```

## Features Needed
- Expand/collapse toggle (click header or chevron icon)
- Animated height transition
- Chevron/arrow indicator showing state
- Optional: only one section open at a time
- Keyboard navigation (Enter/Space to toggle)
- ARIA attributes for accessibility

## Visual States
- Collapsed: Shows header with closed chevron (▶ or ▼)
- Expanded: Shows header with open chevron (▼ or ▲) + content
- Hover: Highlight on header
- Focus: Focus ring on header

## Related
- See `25_tree_node.md` for simpler single-level collapsible headers

---

## Example Screen: AccordionShowcase

**File:** `src/systems/screens/AccordionShowcase.h`
**CLI:** `--screen=accordion`
**Category:** Widgets

### Layout

A settings-style screen using accordions:

1. **Single-Open Accordion** — An accordion with `allow_multiple_open = false`. Three sections: "Graphics" (slider + toggle), "Audio" (3 volume sliders), "Controls" (key binding display). Opening one closes the others.

2. **Multi-Open Accordion** — Same sections but `allow_multiple_open = true`. Multiple sections can be expanded simultaneously.

3. **Animated Transition** — An accordion with `animate_transition = true`. Expanding/collapsing smoothly animates the height of the content area.

4. **Nested Accordion** — An accordion item containing another accordion: "Advanced Settings > Graphics > Shader Quality / Shadow Resolution".

5. **Keyboard Navigation** — Tab to focus accordion headers, Enter/Space to toggle. Visual focus ring on the active header.

### Features Exercised

- `accordion()` and `accordion_item()` with `bool &is_expanded`
- `allow_multiple_open` exclusive mode
- Animated height transition
- Chevron indicator state (▶ collapsed, ▼ expanded)
- Nested accordions
- Keyboard toggle (Enter/Space)

### Verification

- Single-open: expanding "Audio" collapses "Graphics"
- Multi-open: both "Audio" and "Graphics" can be open
- Animation: content smoothly slides open, not instant
- Chevron rotates on expand/collapse
- Keyboard Enter toggles the focused accordion header

### E2E Test Plan

**Test file:** `src/testing/tests/AccordionTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `expect_ui_not_exists`, `simulate_tab`, `simulate_enter`, `capture_snapshot`.

#### Screenshots

1. `accordion_initial` — all sections collapsed
2. `accordion_graphics_open` — Graphics section expanded, others collapsed
3. `accordion_multi_open` — multiple sections expanded simultaneously (multi-open mode)
4. `accordion_animated` — mid-animation during expand (captured at transition frame)
5. `accordion_nested` — nested accordion expanded

#### Test Script

```cpp
TEST(accordion_single_open) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("accordion_initial");

  // Open Graphics
  TestApp::click_button("Graphics");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Resolution");  // content of Graphics section
  auto snap = TestApp::capture_snapshot("accordion_graphics_open");

  // Open Audio (should close Graphics in single-open mode)
  TestApp::click_button("Audio");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Master Volume");
  TestApp::expect_ui_not_exists("Resolution");  // Graphics should be closed
}

TEST(accordion_multi_open) {
  co_await TestApp::wait_for_frames(5);

  // In multi-open section, open two items
  TestApp::click_button("Multi Graphics");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Multi Audio");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Both should be visible
  auto snap = TestApp::capture_snapshot("accordion_multi_open");
}

TEST(accordion_keyboard) {
  co_await TestApp::wait_for_frames(5);

  // Tab to focus first accordion header
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Graphics");

  // Enter to toggle
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Resolution");

  // Enter again to collapse
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_not_exists("Resolution");
}
```

