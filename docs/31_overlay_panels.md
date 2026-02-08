# Drawer / Flyout / Sheet

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

> **Popover** merged into `23_tooltip.md` (shared anchoring/positioning with tooltips).  
> **Dropdown Menu** merged into `24_context_menu.md` (shared MenuItem structure with context menus).

---

## Description

A panel that slides out from the edge of the screen, typically used for secondary navigation, detail views, or settings. Unlike modals, drawers don't center on screen — they attach to an edge and can remain open alongside the main content.

**Also known as:** Tray, Flyout, Sheet, Side panel

## Use Cases in Game UI
- Inventory sidebar
- Character equipment panel
- Quest details panel
- Settings panel (slides from right)
- Player stats panel
- Chat panel
- Mobile-style bottom sheet for action lists

## Suggested Implementation

```cpp
enum class DrawerEdge { Left, Right, Top, Bottom };

struct DrawerConfig {
    DrawerEdge edge = DrawerEdge::Right;
    float width_or_height = 0.3f;  // As screen percentage, or pixels if > 1
    bool close_on_outside_click = true;
    bool show_backdrop = true;
    float backdrop_opacity = 0.5f;
    bool animate = true;
    float animation_duration = 0.25f;
};

bool begin_drawer(HasUIContext auto &ctx, EntityParent ep_pair,
                  bool &is_open,
                  const std::string &title,
                  DrawerConfig config = DrawerConfig());
void end_drawer();

// Or immediate-mode style:
ElementResult drawer(HasUIContext auto &ctx, EntityParent ep_pair,
                     bool &is_open,
                     const std::string &title,
                     DrawerConfig config = DrawerConfig());

// Usage:
if (begin_drawer(ctx, mk(parent, 0), inventory_open, "Inventory",
                 DrawerConfig{.edge = DrawerEdge::Left})) {
    // Drawer content
    for (auto& item : items) {
        inventory_slot(ctx, mk(current, item.id), item);
    }
    end_drawer();
}
```

## Features Needed
- Slide in/out animation from edge
- Optional backdrop overlay (reuse modal backdrop)
- Close button in header
- Close on outside click
- Close on Escape key
- Scrollable content area
- Swipe to close (for touch, future)

---

## Example Screen: DrawerShowcase

**File:** `src/systems/screens/DrawerShowcase.h`
**CLI:** `--screen=drawer`
**Category:** Overlay Components

### Layout

A screen with buttons that open drawers from different edges:

1. **Drawer (Left)** — Button "Open Inventory" opens a drawer sliding from the left edge (30% width). Contains a scrollable list of 20 items. Closes via X button, Escape, or clicking outside the drawer.

2. **Drawer (Right)** — Button "Open Details" opens a drawer from the right (30% width). Contains a form with text inputs and sliders. Demonstrates interactive content inside a drawer.

3. **Drawer (Bottom)** — Button "Show Options" opens a bottom sheet (40% height). Mobile-style action list with 5 options. Demonstrates the vertical variant.

4. **Drawer (Top)** — Button "Notifications" opens a top sheet (200px). Shows a list of notification items. Demonstrates fixed-pixel sizing.

5. **No Backdrop Drawer** — Button "Open Chat" opens a right drawer without a backdrop, allowing interaction with the main content behind it. Demonstrates `show_backdrop = false`.

### Features Exercised

- `drawer()` with Left/Right/Top/Bottom edges
- Drawer backdrop overlay, outside-click-to-close, Escape to close
- Slide animation for drawer open/close
- Scrollable content area inside drawer
- Interactive content (form inputs) inside drawer
- No-backdrop mode

### Verification

- Left drawer slides in from left edge, backdrop appears behind it
- Clicking outside drawer closes it (with backdrop)
- Clicking outside drawer does NOT close it when `close_on_outside_click = false`
- Escape closes the drawer
- X button in header closes the drawer
- Scroll works inside drawer content area
- Bottom sheet takes 40% of screen height from the bottom edge
- No-backdrop drawer allows clicking main content behind it

### E2E Test Plan

**Test file:** `src/testing/tests/DrawerTest.h`

#### New Custom Commands Needed

- `expect_element_visible(label)` / `expect_element_hidden(label)` — check visibility. Needed to verify drawer open/close state.

#### Screenshots

1. `drawer_initial` — trigger buttons visible, no drawers open
2. `drawer_left_open` — left drawer slid in with backdrop
3. `drawer_right_form` — right drawer with form content
4. `drawer_bottom_sheet` — bottom sheet open (40% height)
5. `drawer_no_backdrop` — right drawer open without backdrop

#### Test Script

```cpp
TEST(drawer_left_open_close) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("drawer_initial");

  // Open left drawer
  TestApp::click_button("Open Inventory");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);  // wait for slide animation

  TestApp::expect_ui_exists("Inventory");
  auto snap = TestApp::capture_snapshot("drawer_left_open");

  // Press Escape to close
  TestApp::simulate_escape();
  co_await TestApp::wait_for_frames(10);
}

TEST(drawer_bottom_sheet) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Show Options");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);

  auto snap = TestApp::capture_snapshot("drawer_bottom_sheet");

  // Click outside to close
  test_input::set_mouse_position({640, 100});  // above the bottom sheet
  test_input::simulate_mouse_button_press(raylib::MOUSE_BUTTON_LEFT);
  co_await TestApp::wait_for_frames(1);
  test_input::simulate_mouse_button_release(raylib::MOUSE_BUTTON_LEFT);
  co_await TestApp::wait_for_frames(10);
}

TEST(drawer_right_form_interaction) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Open Details");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);

  auto snap = TestApp::capture_snapshot("drawer_right_form");

  // Close via X button
  TestApp::click_button("Close Drawer");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);
}

TEST(drawer_no_backdrop) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Open Chat");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);

  auto snap = TestApp::capture_snapshot("drawer_no_backdrop");

  // Main content should still be interactable
  TestApp::click_button("Open Inventory");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(10);

  // Both drawers should be open now
  TestApp::expect_ui_exists("Inventory");
}
```
