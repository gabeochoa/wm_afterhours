---
task: Fix UI review issues across showcase screens
test_command: "python3 screenshot_all_screens.py --quick"
---

# Task: UI Review Fixes

Fix outstanding UI issues from the UI review across showcase screens. Items are grouped by priority. Work through them in order (P0 → P1 → P2 → P3).

---

## Build & Run Reference

```bash
make                                          # Build the project
./output/ui_tester.exe                        # Run normally
./output/ui_tester.exe --headless-screenshots # Generate all screenshots to output/
./output/ui_tester.exe --list-screens         # List all registered screens
./output/ui_tester.exe --mcp                  # Start in MCP mode (for scripts)
./output/ui_tester.exe --mcp --screen=NAME    # Start MCP on a specific screen
```

## Validation Tools Reference

These are the tools available for validating your work. Use them liberally.

### 1. Single-Screen Screenshot
```bash
python3 screenshot_screen.py <screen_name> [output_path]
# Example: python3 screenshot_screen.py toggle_switch_showcase /tmp/toggle_fix.png
```

### 2. All-Screen Screenshots (with built-in validation)
```bash
python3 screenshot_all_screens.py --quick                    # 720p only (fast)
python3 screenshot_all_screens.py --resolution=720p,1080p    # Multi-res
python3 screenshot_all_screens.py --quick --with-hover       # Include hover states
```
This automatically captures UI validation warnings/errors from the engine (ScreenBounds, ChildContainment, ContrastRatio, MinFontSize) and prints a summary.

### 3. Layout Regression Testing
```bash
python3 scripts/layout_test.py --baseline                  # Generate baselines BEFORE changes
python3 scripts/layout_test.py                             # Compare current vs baseline
python3 scripts/layout_test.py --screen=toggle             # Test specific screen
python3 scripts/layout_test.py --threshold=2               # Allow 2% diff
```

### 4. Interactivity Testing
```bash
python3 scripts/interactivity_test.py                      # Test all defined screens
python3 scripts/interactivity_test.py --screen=NAME        # Test specific screen
python3 scripts/interactivity_test.py --save-screenshots   # Save before/after images
python3 scripts/interactivity_test.py --verbose            # Detailed output
```

### 5. MCP Tools (available when running with --mcp)
When writing custom validation, these MCP tools are available via JSON-RPC:
- `screenshot` — capture current frame as PNG
- `dump_ui_tree` — get full UI tree as JSON (element names, rects, visibility, clickable)
- `mouse_move`, `mouse_click`, `key_press` — simulate input
- `goto_screen` — navigate to a screen by name
- `list_screens` — enumerate available screens
- `ping` / `exit` — lifecycle management

### 6. Built-in Engine Validation
The engine runs these checks automatically during rendering (visible in stderr):
- **ScreenBounds** — elements outside screen edges (16px safe area)
- **ChildContainment** — children rendering outside parent bounds
- **ContrastRatio** — text contrast below 4.5:1 WCAG AA
- **MinFontSize** — fonts below 12px minimum

---

## Success Criteria

### P0: Text Clipping at Screen Edges

These are partially fixed but some clipping persists. The root cause may be in the text rendering layer.

1. [ ] **LanguageDemo text clipping** — `src/systems/screens/LanguageDemo.h`
   - Title "Language Demo" should not clip on the left edge
   - Validate: `python3 screenshot_screen.py language_demo /tmp/language_demo.png` — visually confirm no left-edge clipping on title
   - Validate: run `python3 screenshot_all_screens.py --quick` and confirm no `ScreenBounds` warnings for `language_demo`

2. [ ] **DeadSpaceSettings sidebar clipping** — `src/systems/screens/DeadSpaceSettings.h`
   - Sidebar items like "Show Content Warning" should not clip left
   - Validate: `python3 screenshot_screen.py dead_space_settings /tmp/deadspace.png` — confirm sidebar text fully visible

3. [ ] **ModalShowcase footer clipping** — `src/systems/screens/ModalShowcase.h`
   - Footer status "Confirm: Pending" should not clip left
   - Validate: `python3 screenshot_screen.py modal_showcase /tmp/modals.png` — confirm footer text fully visible

4. [ ] **EmpireTycoon currency clipping** — `src/systems/screens/EmpireTycoon.h`
   - "$1,250,980" should display fully, not clipped to "$250,980"
   - Validate: `python3 screenshot_screen.py empire_tycoon /tmp/empire.png` — confirm full currency value visible

### P1: High Priority

5. [ ] **Toggle switch visibility** — `src/systems/screens/ToggleSwitchShowcase.h`
   - Toggle tracks are nearly invisible because track color matches surface color
   - Fix: increase contrast between toggle track and background (e.g. darken off-state track, or add a border)
   - Validate: `python3 screenshot_screen.py toggle_switch_showcase /tmp/toggle.png` — track should be clearly distinguishable from background in both on and off states
   - Validate: no `ContrastRatio` warnings for this screen in `screenshot_all_screens.py --quick` output

6. [ ] **Checkbox panel overflow** — `src/systems/screens/CheckboxShowcase.h`
   - Right panel "Multi-Select" and "MinMax" boxes overflow their container on the right side
   - Fix: constrain panel width or reduce content width so nothing extends past container
   - Validate: `python3 screenshot_screen.py checkbox_showcase /tmp/checkbox.png` — no content visually spilling past right panel edge
   - Validate: no `ChildContainment` warnings for this screen

7. [ ] **Missing focus states** — Multiple component showcase files
   - Buttons show focus rings, but checkboxes and toggles don't
   - Fix: ensure checkboxes and toggle switches display a visible focus indicator when focused
   - Validate: use MCP interactivity to tab through elements and take screenshots:
     ```bash
     python3 scripts/interactivity_test.py --save-screenshots
     ```
     Inspect before/after images to confirm focus rings appear on checkboxes and toggles

8. [ ] **Theme switcher self-contrast** — `src/systems/screens/Themes.h`
   - "Cozy Kraft" button text is yellow/gold on yellow background when selected — unreadable
   - Fix: ensure selected button text contrasts with button background (e.g. use dark text on light backgrounds)
   - Validate: `python3 screenshot_screen.py themes /tmp/themes.png` — Cozy Kraft button text readable when selected
   - Validate: no `ContrastRatio` warnings for `themes` screen

### P2: Medium Priority

9. [ ] **Cards label alignment** — `src/systems/screens/Cards.h`
   - Inconsistent label-to-content spacing across card elements
   - Fix: standardize padding/margin between labels and their associated content
   - Validate: `python3 screenshot_screen.py cards /tmp/cards.png` — labels consistently spaced from content

10. [ ] **Navigation bar chevron separation** — `src/systems/screens/NavigationBarShowcase.h`
    - Chevrons are visually separated from their parent nav element
    - Fix: tighten spacing between chevrons and the element they belong to
    - Validate: `python3 screenshot_screen.py navigation_bar_showcase /tmp/navbar.png` — chevrons visually connected to nav items

11. [ ] **Kirby options icon grid alignment** — `src/systems/screens/KirbyOptions.h`
    - Bottom row of icon grid drifts left compared to rows above
    - Fix: align all grid rows to the same left edge
    - Validate: `python3 screenshot_screen.py kirby_options /tmp/kirby.png` — all icon rows start at the same x position

12. [ ] **Toggle switches empty space** — `src/systems/screens/ToggleSwitchShowcase.h`
    - Massive empty space below the toggle controls
    - Fix: reduce bottom padding or distribute toggles more evenly in available space
    - Validate: `python3 screenshot_screen.py toggle_switch_showcase /tmp/toggle_space.png` — content fills available area without large blank regions

13. [ ] **Toasts button grouping** — `src/systems/screens/ToastShowcase.h`
    - Buttons float without clear visual grouping
    - Fix: group related buttons together with consistent spacing, visual containers, or section labels
    - Validate: `python3 screenshot_screen.py toast_showcase /tmp/toasts.png` — buttons appear organized in logical groups

### P3: Low Priority / Polish

14. [ ] **Standardize spacing tokens** — All showcase screens
    - Spacing feels arbitrary across screens
    - Fix: audit and normalize to consistent spacing values (e.g. Spacing::xs, sm, md, lg)
    - Validate: run full screenshot capture at multiple resolutions:
      ```bash
      python3 screenshot_all_screens.py --resolution=720p,1080p
      ```
      Visually review output in `/tmp/ui_showcase_screenshots/` for consistent spacing

15. [ ] **Scroll view demo polish** — `src/systems/screens/ScrollViewShowcase.h`
    - Horizontal scroll clips numbers, scroll indicators are unclear
    - Fix: ensure scrolled content is fully visible within scroll area; make scroll indicators more prominent
    - Validate: `python3 screenshot_screen.py scroll_view_showcase /tmp/scroll.png` — numbers fully visible, scroll indicators clear

16. [ ] **Angry Birds settings icon clarity** — `src/systems/screens/AngryBirdsSettings.h`
    - Icon buttons use unclear symbols
    - Fix: replace or augment ambiguous icons with clearer alternatives or text labels
    - Validate: `python3 screenshot_screen.py angry_birds_settings /tmp/angry.png` — all icon buttons clearly indicate their function

### Bonus: Inline TODOs

17. [ ] **Font configuration TODO** — `Cards.h`, `Forms.h`, `Buttons.h`
    - Three screens have `// TODO: Add font configuration when fonts are selected`
    - Fix: wire up font configuration for these screens (they currently rely on theme defaults)
    - Validate: screenshots show correct themed fonts, no fallback/default font rendering

18. [ ] **IslandsTrains entity ID conflict** — `src/systems/screens/IslandsTrainsSettings.h`
    - Screen is commented out: `// TODO: Fix entity ID conflict in vertical_gradient before re-enabling`
    - Fix: resolve the entity ID conflict and re-enable the screen registration
    - Validate: `./output/ui_tester.exe --list-screens` includes `islands_trains_settings`
    - Validate: `python3 screenshot_screen.py islands_trains_settings /tmp/islands.png` — renders without errors

---

## Validation Checklist (run after all changes)

After implementing fixes, run these in order:

```bash
# 1. Build
make

# 2. Quick screenshot smoke test (captures all screens at 720p + validation)
python3 screenshot_all_screens.py --quick

# 3. Layout regression test (generates new baselines if none exist)
python3 scripts/layout_test.py --baseline
python3 scripts/layout_test.py

# 4. Interactivity smoke test
python3 scripts/interactivity_test.py

# 5. Multi-resolution validation (slow, but catches resolution-dependent bugs)
python3 screenshot_all_screens.py --resolution=480p,720p,1080p
```

**Pass criteria:**
- Build succeeds with no new warnings
- All screenshot captures succeed (no FAIL in output)
- Zero `ScreenBounds` validation errors in screenshot output
- Zero `ChildContainment` validation errors in screenshot output
- Zero `ContrastRatio` validation errors for screens you've modified
- Layout tests pass (< 1% diff from baseline for unchanged screens)
- Interactivity tests pass

---

## Ralph Instructions

1. Work on the next incomplete criterion (marked `[ ]`)
2. Before making changes, capture a baseline screenshot of the screen you're modifying
3. After making changes, capture a new screenshot and compare visually
4. Run the relevant validation commands listed under each criterion
5. Check off completed criteria (change `[ ]` to `[x]`)
6. Run `make` after every change to confirm it compiles
7. Commit your changes frequently with descriptive messages
8. When ALL criteria are `[x]`, output: `<ralph>COMPLETE</ralph>`
9. If stuck on the same issue 3+ times, output: `<ralph>GUTTER</ralph>`
