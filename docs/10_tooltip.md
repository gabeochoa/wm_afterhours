# Tooltip

**Status:** Not implemented  
**Priority:** Medium

## Problem

No hover-based contextual help.

## Suggested Implementation

```cpp
void set_tooltip(HasUIContext auto &ctx, const std::string &text);

// Or as a modifier
ComponentConfig{}.with_tooltip("Help text here");
```

## Features Needed
- Delay before showing (e.g., 0.5s hover)
- Position near cursor or element
- Auto-hide on mouse move away
- Rich content support (not just text)

---

## Popover

An interactive floating panel anchored to a trigger element. Unlike tooltips, popovers are triggered via click, can contain buttons/forms, and have a focus trap.

### Use Cases in Game UI
- Item info popup (on click)
- Quick actions menu
- Emoji picker
- Color palette popup
- User profile preview

### Suggested Implementation

```cpp
enum class PopoverPlacement {
    Top, TopStart, TopEnd,
    Bottom, BottomStart, BottomEnd,
    Left, LeftStart, LeftEnd,
    Right, RightStart, RightEnd,
    Auto  // Automatically choose best placement
};

struct PopoverConfig {
    PopoverPlacement placement = PopoverPlacement::Auto;
    bool close_on_outside_click = true;
    bool show_arrow = true;
    float offset = 8.0f;  // Distance from trigger
};

bool begin_popover(HasUIContext auto &ctx, EntityParent ep_pair,
                   Entity trigger_element,
                   bool &is_open,
                   PopoverConfig config = PopoverConfig());
void end_popover();

// Usage:
if (button(ctx, mk(parent, 0), ComponentConfig{}.with_label("⋮")).clicked) {
    more_menu_open = !more_menu_open;
}

if (begin_popover(ctx, mk(parent, 1), mk(parent, 0).entity, more_menu_open)) {
    if (button(ctx, mk(current, 0), "Edit").clicked) { /* ... */ }
    if (button(ctx, mk(current, 1), "Delete").clicked) { /* ... */ }
    end_popover();
}
```

### Features Needed
- Positioning relative to trigger element (shares logic with tooltip positioning)
- Auto-flip when near screen edge
- Optional arrow/caret pointing to trigger
- Close on outside click
- Close on Escape
- Keyboard navigation within popover
- Focus trap

### Relationship to Tooltip

Tooltips and popovers share the same positioning/anchoring engine. The difference is:

| | Tooltip | Popover |
|---|---------|---------|
| Trigger | Hover (with delay) | Click |
| Content | Text only (or rich text) | Interactive (buttons, forms) |
| Dismissal | Mouse leaves | Outside click / Escape |
| Focus | No focus trap | Focus trapped inside |

Both should use a shared `AnchoredOverlay` positioning primitive internally.

---

## Example Screen: TooltipShowcase

**File:** `src/systems/screens/TooltipShowcase.h`
**CLI:** `--screen=tooltips`
**Category:** Overlay Components

### Layout

A grid of interactive elements, each with a tooltip:

1. **Basic Tooltips** — 4 buttons in a row, each with `with_tooltip("Help text")`. Hovering for 0.5s shows the tooltip near the cursor/element.

2. **Positioned Tooltips** — Buttons at screen edges to test auto-positioning: a button in the top-left corner (tooltip should appear below/right), bottom-right corner (tooltip above/left), etc.

3. **Rich Tooltip** — A button whose tooltip contains an icon + multiline text + a small color swatch. Demonstrates rich content beyond plain text.

4. **Disabled Element Tooltip** — A disabled button with a tooltip explaining why it's disabled: "Complete level 5 to unlock this feature."

5. **Long Tooltip** — A button with a very long tooltip string to test word wrapping and max-width behavior.

6. **Popover (Click)** — A "More ⋮" button. Clicking it opens a popover positioned below the button with 3 action items: Edit, Duplicate, Delete. The popover has an arrow pointing to the trigger button.

7. **Popover Auto-positioning** — A popover trigger button that the user can drag around the screen. The popover auto-flips placement (above/below/left/right) based on available space near screen edges. Demonstrates shared positioning logic with tooltips.

8. **Popover with Form** — A "Settings" button that opens a popover containing a slider and a checkbox. Demonstrates interactive content and focus trap inside the popover.

### Features Exercised

- `set_tooltip()` or `.with_tooltip()` on ComponentConfig
- Hover delay (configurable, default 0.5s)
- Auto-positioning near element (flipping when near edges)
- Auto-hide on mouse move away
- Rich content in tooltips (optional)
- `begin_popover()` / `end_popover()` with arrow and placement
- Popover close on outside click and Escape
- Popover focus trap with interactive content

### Verification

- Tooltip appears after ~0.5s hover delay, not immediately
- Tooltip disappears when mouse moves off the element
- Edge buttons: tooltip never renders off-screen
- Long tooltip text wraps within a reasonable max width
- Disabled button still shows its tooltip
- Popover arrow points to the trigger button
- Popover auto-flips when near screen edges
- Clicking outside popover closes it
- Tab key cycles within popover (focus trap)

### E2E Test Plan

**Test file:** `src/testing/tests/TooltipTest.h`

#### New Custom Commands Needed

- `hover_element(label)` — move mouse to element center without clicking. Needed to trigger hover-based tooltip display after the delay period.

#### Screenshots

1. `tooltip_initial` — buttons visible, no tooltips showing
2. `tooltip_visible` — tooltip appearing after hover delay (~0.5s / 30 frames)
3. `tooltip_edge_positioned` — tooltip on edge button showing it doesn't clip off-screen
4. `tooltip_long_wrapped` — long tooltip text with word wrapping visible
5. `popover_open` — popover with arrow pointing to trigger button
6. `popover_auto_flip` — popover flipped above trigger near bottom edge
7. `popover_form` — popover with interactive slider/checkbox inside

#### Test Script

```cpp
TEST(tooltip_hover_shows) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("tooltip_initial");

  // Hover over first button
  hover_element("Hover Me");
  co_await TestApp::wait_for_frames(5);  // before delay
  // Tooltip should NOT be visible yet (too early)

  co_await TestApp::wait_for_frames(30);  // after 0.5s delay
  // Tooltip should now be visible
  TestApp::expect_ui_exists("Help text here");
  auto snap = TestApp::capture_snapshot("tooltip_visible");

  // Move mouse away
  test_input::set_mouse_position({0, 0});
  co_await TestApp::wait_for_frames(5);

  // Tooltip should disappear
  TestApp::expect_ui_not_exists("Help text here");
}

TEST(tooltip_edge_position) {
  co_await TestApp::wait_for_frames(5);

  // Hover over top-left corner button
  hover_element("Top Left Button");
  co_await TestApp::wait_for_frames(35);

  auto snap = TestApp::capture_snapshot("tooltip_edge_positioned");
  // Visual verification: tooltip renders below/right, not clipped
}

TEST(tooltip_disabled_element) {
  co_await TestApp::wait_for_frames(5);

  hover_element("Disabled Button");
  co_await TestApp::wait_for_frames(35);

  // Disabled button's tooltip should still show
  TestApp::expect_ui_exists("Complete level 5 to unlock");
}

TEST(popover_open_and_close) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("More");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Edit");
  TestApp::expect_ui_exists("Duplicate");
  TestApp::expect_ui_exists("Delete");
  auto snap = TestApp::capture_snapshot("popover_open");

  // Click Edit — popover should close
  TestApp::click_button("Edit");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_not_exists("Duplicate");
}

TEST(popover_escape_closes) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("More");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Edit");

  TestApp::simulate_escape();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_not_exists("Edit");
}
```

