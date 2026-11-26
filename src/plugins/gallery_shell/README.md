# Gallery Shell local plugin

WM-local plugin (Batch 2 Lane E). Not afterhours upstream.

## Purpose

Named home for the example-gallery shell: the registry, navigator,
tree model, and HUD that let WM browse, list, and create its 150+
example screens. Behavior-preserving package of the four headers
formerly under `src/systems/`; no screen edits, no afterhours/vendor
edits.

## Public API

Canonical headers (in this directory):

- `ExampleScreenRegistry.h` — `include "plugins/gallery_shell/ExampleScreenRegistry.h"`
  - `ExampleScreen { name, category, description, create_system }` —
    one registered screen.
  - `ExampleScreenRegistry::get()` — singleton.
    - `register_screen(flag_name, category, description, create_func)`
    - `has_screen(flag_name)`
    - `create_screen(flag_name)` — loads UI fonts for the screen, then
      returns its system.
    - `get_screen_description(flag_name)`, `get_screen_category(flag_name)`
    - `list_screens()` — grouped `--list-screens` output (categories:
      Game Mockups, Component Galleries, System Demos, Tools, plus any
      unknown categories appended).
    - `get_screen_names()` — flat name list (insertion-sorted via `std::map`).
  - `REGISTER_EXAMPLE_SCREEN(flag_name, category, description, system_type)`
    — static registrar macro; one line per screen header.
  - `ScreenSystem<Components...>` — `afterhours::System` base that only
    runs while `g_current_screen == this`; screens inherit from this.
  - `g_current_screen` — global pointer to the active screen system,
    set by the screen cycler.
- `ScreenNavigator.h` — `include "plugins/gallery_shell/ScreenNavigator.h"`
  - `ScreenNavigator : System<UIContext<InputAction>>` — browse/search
    overlay (` ` to open, arrows browse, Enter opens, Esc closes).
    - `build_rows(names)`, `open()`, `activate(context, row)`,
      `for_each_with(...)` (panel + filter + virtual list + details pane).
    - `WIDTH`/`ROW_H` layout constants; `tree`, `visible`,
      `show_launcher`, `current_index`, deferred `on_pick` callback.
- `ScreenTree.h` — `include "plugins/gallery_shell/ScreenTree.h"`
  - `ScreenTree` — pure model (no UI): `entries`, `rows`, `expanded`,
    `query`, `cursor`, `rebuild()`, `move()`, `toggle()`, `left()`,
    `right()`, `reveal(screen_index)`; `category()` maps legacy labels
    (Components/Widgets/Text/Text Editing → Component Galleries,
    Bug Reports → System Demos, empty → Uncategorised).
- `RenderScreenHUD.h` — `include "plugins/gallery_shell/RenderScreenHUD.h"`
  - `ScreenHUDState` — current name/description/index/total statics.
  - `RenderScreenHUD : System<>` — bottom-left HUD (`--screen=<name>`,
    nav hint, description, `n/total` index); skips headless mode.

Compatibility: `src/systems/ExampleScreenRegistry.h`,
`src/systems/ScreenNavigator.h`, `src/systems/ScreenTree.h`, and
`src/systems/RenderScreenHUD.h` are shims including these headers;
all existing include paths (screens, `game`, `main`,
`headless_screenshots`, `ProfilerOverlay`, `e2e_commands`,
`tests/screen_tree_test.cpp`) are unchanged.

## How a consumer registers / lists / creates screens

```cpp
#include "plugins/gallery_shell/ExampleScreenRegistry.h"

// Register (typically at the bottom of the screen's header):
REGISTER_EXAMPLE_SCREEN(my_screen, "Tools", "What this screen shows",
                        MyScreenSystem);

// List (what --list-screens does):
ExampleScreenRegistry::get().list_screens();

// Create + switch (what the cycler/headless harness does):
auto screen = ExampleScreenRegistry::get().create_screen("my_screen");
g_current_screen = screen.get();
```

Screen headers are pulled into `main.cpp` via the Makefile-generated
`screen_includes.gen` (one `#include "systems/screens/<Name>.h"` per
`src/systems/screens/*.h`); the registrar macro self-registers on
static init, so no central list to edit.

## Current consumers

- `src/main.cpp` — `--list-screens` via `list_screens()`; all screens
  via `screen_includes.gen`.
- `src/game.cpp` — registers `RenderScreenHUD` + `ScreenNavigator`
  systems (via the `src/systems/*.h` shims).
- `src/headless_screenshots.cpp` — `create_screen`, `get_screen_names`,
  `has_screen` for headless capture; registers `RenderScreenHUD`.
- `src/systems/ProfilerOverlay.h` — includes `RenderScreenHUD` shim.
- 150+ screens under `src/systems/screens/*.h` — each includes
  `../ExampleScreenRegistry.h` and calls `REGISTER_EXAMPLE_SCREEN`.
- `src/testing/e2e_commands.h` — includes the registry shim.
- `tests/screen_tree_test.cpp` — includes the `ScreenTree` shim;
  exercises rebuild/toggle/move/reveal/search.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --list-screens          # same count/order/categories/descriptions
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/99_check_all_screens.e2e --headless
clang++ -std=c++23 -I vendor -Isrc tests/screen_tree_test.cpp -o /tmp/screen_tree_test && /tmp/screen_tree_test
```

Targeted e2e: `99_check_all_screens`, `155_screen_tree`.

## Afterhours promotion gate (future candidate only)

No afterhours/vendor changes in this batch. Future candidate only
for an afterhours examples/devtools harness — i.e. a small gallery
shell (registry + browser + HUD) a consumer harness can reuse — and
only after scaffold PR #3 (`screen_scaffold` `begin_screen` helper)
has landed and this plugin has held stable across the full local
suite, `99_check_all_screens` + `155_screen_tree`, and headless
screenshot baselines. Align any promotion with the scaffold
abstraction rather than shipping two competing screen helpers. Not
claimed upstream-ready; any promotion is a separate proposal with
its own review.
