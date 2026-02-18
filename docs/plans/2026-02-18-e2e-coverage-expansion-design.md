# E2E Test Coverage Expansion

## Goal

Add E2E test scripts for 7 high-interaction screens that currently have zero test coverage. These screens all have buttons, tabs, selection, or drag interactions that should be exercised.

## Screens

### Batch 1: Game Mockups

**1. potion_crafting** (`tests/e2e_scripts/40_potion_crafting.e2e`)
- Verify initial state: title "Alchemist's Workshop", tab "Recipes" selected, recipe "Mana Elixir" selected (index 1), brew progress "65% Brewed"
- Tab switching: click "Inventory" tab, click "Journal" tab, click "Recipes" tab
- Recipe selection: click "Healing Potion" row, verify "Brewing: Healing Potion" header updates
- Click "Brew!" button
- Screenshot after each state change

**2. shop_interface** (`tests/e2e_scripts/41_shop_interface.e2e`)
- Verify initial state: "General Store" title, "G 3,750" gold display, "Buy" tab selected, "Health Potion" selected (index 1), cart shows "5 items"
- Tab switching: click "Sell" tab, click "Buy" tab
- Item selection: click "Iron Sword" row, verify detail panel shows "Iron Sword" name and "250 G" price
- Click "Add to Cart" button
- Click "Purchase" button
- Screenshot after each state change

**3. kart_select** (`tests/e2e_scripts/42_kart_select.e2e`)
- Verify initial state: "SELECT YOUR RACER" title, "Drift" selected (index 2), "Standard" kart selected, bottom bar shows "Drift  +  Standard"
- Character selection: click "Turbo" cell, verify stat bars update, bottom bar shows "Turbo  +  Standard"
- Kart selection: click "Speedster", verify bottom bar shows "Turbo  +  Speedster"
- Click "READY!" button
- Click "< Back" button
- Screenshot after each state change

### Batch 2: Component Galleries

**4. toasts** (`tests/e2e_scripts/43_toasts.e2e`)
- Verify initial state: "Toast Notifications" title, sections visible
- Click "Info Toast" button, verify counter increments in toast message
- Click "Success Toast" button
- Click "Warning Toast" button
- Click "Error Toast" button
- Click "Quick (displays 1s)" button
- Click "Long (displays 10s)" button
- Click "Spam x5 (!)" button
- Click "With Undo Action" button, verify "Undos: 1" counter
- Screenshot after key interactions

**5. file_tree** (`tests/e2e_scripts/44_file_tree.e2e`)
- Verify initial state: "File Tree" title, current path displayed, directory listing visible
- Click "Home" button
- Click "Up" button, verify path changes
- Click "Refresh" button
- Tab to tree and expand a directory
- Screenshot after each navigation

**6. horizontal_drag** (`tests/e2e_scripts/45_horizontal_drag.e2e`)
- Verify initial state: "Horizontal Drag & Drop" title, 3 priority lanes with correct counts: "High Priority (2)", "Medium Priority (3)", "Low Priority (2)"
- Verify tag text: "Security patch", "Data loss fix" in high; "Refactor auth", "Update deps", "Add logging" in medium
- Drag a tag from High to Medium using mouse_down/mouse_move/mouse_up
- Verify counts updated: "High Priority (1)", "Medium Priority (4)"
- Screenshot after drag

**7. stepper_showcase** (`tests/e2e_scripts/46_stepper_showcase.e2e`)
- Verify initial state: "Stepper Showcase" title, initial values (Resolution: "2560x1440", Quality: "Medium", Language: "English", Difficulty: "Normal", Framerate: "60")
- Tab to Resolution stepper, press Right arrow to advance to "3840x2160"
- Tab to Quality stepper, press Right to advance to "High"
- Tab to card selector stepper, press Right to switch from "Warrior" to "Mage", verify card description changes
- Screenshot after changes

## E2E Command Reference

Available commands (from existing scripts):
- `goto_screen <name>` - navigate to screen
- `wait <seconds>` - pause
- `expect_text "<text>"` - assert text visible
- `key <KEY>` - TAB, ENTER, ESCAPE, RIGHT, LEFT, UP, DOWN, PAGEDOWN, PAGEUP
- `screenshot <name>` - capture screenshot
- `focus_component <debug_name>` - focus element by debug_name
- `click <x> <y>` or `click <x%> <y%>` - click at coordinates
- `mouse_down <x%> <y%>` - press mouse button at position
- `mouse_move <x%> <y%>` - move mouse
- `mouse_up` - release mouse button
- `validate_screen <name>` - compare against baseline

## Conventions

Follow existing test file patterns:
- Header comment explaining what the test covers
- Numbered test sections with `# =========` separators
- `wait` after interactions for frame processing (0.2-0.5s typical)
- `expect_text` to verify state, not just screenshots
- Screenshots named `<screen>_<state>` (e.g. `potion_crafting_recipe_selected`)
