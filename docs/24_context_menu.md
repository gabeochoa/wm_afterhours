# Context Menu / Dropdown Menu

**Status:** Not implemented  
**Priority:** Medium

## Problem

No right-click menus, popup menus, or click-triggered dropdown menus. These share the same underlying `MenuItem` structure, positioning, keyboard navigation, and submenu mechanics — only the trigger differs.

## Shared MenuItem Structure

```cpp
struct MenuItem {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<std::string> shortcut;  // "Ctrl+C"
    std::function<void()> on_select;
    bool disabled = false;
    bool is_separator = false;
    std::vector<MenuItem> submenu;  // For nested menus
};
```

## Context Menu (Right-Click)

```cpp
ElementResult context_menu(HasUIContext auto &ctx, EntityParent ep_pair,
                           const std::vector<MenuItem> &items,
                           bool &is_open);
```

- Triggered on right-click
- Positioned at cursor location
- Click-outside-to-close

## Dropdown Menu (Click)

```cpp
ElementResult dropdown_menu(HasUIContext auto &ctx, EntityParent ep_pair,
                            Entity trigger_button,
                            const std::vector<MenuItem> &items,
                            bool &is_open);

// Usage:
if (button(ctx, mk(parent, 0), ComponentConfig{}.with_label("File")).clicked) {
    file_menu_open = true;
}

dropdown_menu(ctx, mk(parent, 1), mk(parent, 0).entity, {
    {"New", new_icon, "Ctrl+N", []{ new_file(); }},
    {"Open", open_icon, "Ctrl+O", []{ open_file(); }},
    {"Save", save_icon, "Ctrl+S", []{ save_file(); }},
    {.is_separator = true},
    {"Exit", std::nullopt, std::nullopt, []{ exit(); }}
}, file_menu_open);
```

- Triggered on click
- Positioned below the trigger button
- Shortcut key labels right-aligned

## Shared Features Needed
- Click-outside-to-close
- Keyboard navigation (arrows, Enter, Escape)
- Nested submenus (hover-to-open or arrow-to-open)
- Menu item hover highlighting
- Dividers/separators
- Icons
- Disabled items
- Shortcut key display (dropdown)

---

## Example Screen: ContextMenuShowcase

**File:** `src/systems/screens/ContextMenuShowcase.h`
**CLI:** `--screen=context_menu`
**Category:** Overlay Components

### Layout

A screen demonstrating both context menus (right-click) and dropdown menus (click):

1. **Simple Context Menu** — A colored panel labeled "Right-click me". Right-clicking opens a menu with 4 options: Cut, Copy, Paste, Delete. Selecting an option appends to an action log.

2. **Nested Submenu** — A second panel where right-clicking shows: "Format >" (submenu with Bold, Italic, Underline), "Align >" (submenu with Left, Center, Right), and "Properties".

3. **Dynamic Context Menu** — A list of 5 items. Right-clicking each item opens a context menu customized to that item: "Rename [item name]", "Duplicate", "Delete". The menu title changes per item.

4. **Dropdown Menu** — A "File" button that opens a dropdown menu below it with items: New (Ctrl+N), Open (Ctrl+O), Save (Ctrl+S), separator, Exit. Shortcut text right-aligned. Hovering "Recent >" opens a nested submenu.

5. **Menu Bar** — A row of buttons: File, Edit, View. Clicking each opens its dropdown. Only one dropdown open at a time. Demonstrates switching between menus.

6. **Keyboard Navigation** — After opening any menu, arrow keys navigate options, Enter selects, Escape closes. Right-arrow opens submenus, Left-arrow closes them.

### Features Exercised

- `context_menu()` triggered on right-click
- `dropdown_menu()` triggered on click, anchored below trigger
- Shared `MenuItem` structure with icons, shortcuts, separators, submenus
- Positioning near cursor (context) / below trigger (dropdown) with screen-edge clamping
- Click-outside-to-close behavior
- Keyboard navigation (arrows, Enter, Escape)
- Nested submenus with hover-to-open
- Dynamic option generation per target element

### Verification

- Right-click opens menu at cursor position
- Click opens dropdown below the trigger button
- Clicking outside the menu closes it
- Arrow keys navigate menu items with visual highlight
- Submenu opens on hover/right-arrow, closes on left-arrow
- Escape closes the innermost open menu level
- Dropdown shortcut text (Ctrl+N) is right-aligned
- Only one dropdown open at a time in menu bar

### E2E Test Plan

**Test file:** `src/testing/tests/ContextMenuTest.h`

#### New Custom Commands Needed

- `right_click_element(label)` — right-click on an element by label. Positions mouse at element center and presses `MOUSE_BUTTON_RIGHT`.
- `hover_element(label)` — move mouse to element center without clicking. Needed for submenu hover-to-open.

#### Screenshots

1. `context_menu_initial` — panels and menu bar visible, no menus open
2. `context_menu_open` — context menu visible at cursor position after right-click
3. `context_menu_submenu` — nested submenu expanded from "Format >" item
4. `context_menu_keyboard_nav` — menu open with highlighted item via arrow key navigation
5. `dropdown_menu_open` — dropdown below "File" button with shortcuts visible
6. `dropdown_menu_submenu` — nested submenu expanded from "Recent >" item

#### Test Script

```cpp
TEST(context_menu_open_and_select) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("context_menu_initial");

  // Right-click the simple panel
  right_click_element("Right-click me");
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Cut");
  TestApp::expect_ui_exists("Copy");
  TestApp::expect_ui_exists("Paste");
  TestApp::expect_ui_exists("Delete");
  auto snap = TestApp::capture_snapshot("context_menu_open");

  // Click "Copy"
  TestApp::click_button("Copy");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Menu should close, action should be logged
  TestApp::expect_ui_not_exists("Cut");
}

TEST(context_menu_submenu) {
  co_await TestApp::wait_for_frames(5);

  right_click_element("Format Panel");
  co_await TestApp::wait_for_frames(5);

  // Hover over "Format >" to open submenu
  hover_element("Format");
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("Bold");
  TestApp::expect_ui_exists("Italic");
  auto snap = TestApp::capture_snapshot("context_menu_submenu");
}

TEST(context_menu_keyboard) {
  co_await TestApp::wait_for_frames(5);

  right_click_element("Right-click me");
  co_await TestApp::wait_for_frames(5);

  // Arrow down to navigate
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);

  auto snap = TestApp::capture_snapshot("context_menu_keyboard_nav");

  // Enter to select
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(3);

  // Menu should close
  TestApp::expect_ui_not_exists("Cut");
}

TEST(context_menu_escape) {
  co_await TestApp::wait_for_frames(5);

  right_click_element("Right-click me");
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_ui_exists("Cut");

  TestApp::simulate_escape();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_not_exists("Cut");
}

TEST(dropdown_menu_open) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("File");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("New");
  TestApp::expect_ui_exists("Ctrl+N");
  TestApp::expect_ui_exists("Save");
  auto snap = TestApp::capture_snapshot("dropdown_menu_open");

  // Hover "Recent >" to open submenu
  hover_element("Recent");
  co_await TestApp::wait_for_frames(10);

  auto snap2 = TestApp::capture_snapshot("dropdown_menu_submenu");
}

TEST(dropdown_menu_select) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("File");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Click "New"
  TestApp::click_button("New");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Menu should close
  TestApp::expect_ui_not_exists("Ctrl+N");
}
```

