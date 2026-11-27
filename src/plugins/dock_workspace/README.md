# Dock Workspace local plugin

WM-local plugin. Not afterhours upstream.

## Purpose

A Trellis-style fractal dockable workspace: one layout tree (splits, tab
groups, leaves) plus a floating layer, an animated camera that frames any
subtree, drag-to-dock with live previews, divider resizing, semantic-zoom
tiles, spatial history, and a serializable document. It is the working
prototype for first-class tiling-WM support in afterhours (see
`docs/TRELLIS_LEARNINGS.md`) and is intended to make docked-tool apps such
as hanabi, wordproc, and floatinghotel easier to build.

## Public API

Canonical header: `src/plugins/dock_workspace/DockWorkspace.h`
(`#include "../../plugins/dock_workspace/DockWorkspace.h"` from screens).

- `dock::Node` — layout tree (`leaf`, `tabs`, `splitv`/`split`), plus
  `remove_panel_from`, `insert_beside`, `find_host`, `move_tab_to`,
  `serialize_doc`.
- `dock::PanelDef<Ctx>` — per-panel title, semantic-tile color, draw
  callback, and `min_w`/`min_h`: below that screen size the panel renders
  as a semantic-zoom tile instead of fixed-size content.
- `dock::Workspace<Ctx>` — owns the `Doc`, camera/history, solver, input
  (divider drag, header/tab drag, double-click frame, Esc step-out,
  ctrl/cmd-wheel zoom), drop previews, and rendering.
  - `render(ctx, parent, area, dt)` adopts `area` as the workspace root
    on first use and rescales workspace-space state on area changes, so
    the overview camera matches the root instead of squeezing panels.
  - `toolbar(...)` renders Overview/Back/Forward/Step out/Reset clamped
    left of the gallery shell's Browse-screens launcher.
  - `reset_camera_history()` clears camera/history/focus/drag state;
    apps pair it with their own layout/value reset for a full reset.
- Tabs are draggable: grab any visible tab, reorder inside its strip
  (insertion line + tab-sized ghost), drag out to split/float/dock, or
  drop on another panel's header to insert at that tab position.

Compatibility: `src/systems/screens/trellis/DockWorkspace.h` is a shim
including this header, so the old include path keeps working.

## Consumers

- `TrellisPaint` (`trellis_paint`) — palettes around a canvas stage.
- `TrellisIde` (`trellis_ide`) — explorer/editor tabs/terminal/preview.
- `TrellisDesktop` (`trellis_desktop`) — floating fractal desktop.
- `TrellisOps` (`trellis_ops`) — streaming chart/metrics/logs/runbook.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
clang++ -std=c++23 -DAFTER_HOURS_USE_RAYLIB \
  -DAFTER_HOURS_ENABLE_E2E_TESTING -I vendor -Isrc \
  -I$HOME/homebrew/include tests/dock_workspace_test.cpp \
  -o /tmp/dock_workspace_test -L$HOME/homebrew/lib -lraylib \
  -framework CoreFoundation -framework OpenGL -framework AppKit \
  -framework IOKit && /tmp/dock_workspace_test
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/trellis_paint.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/trellis_ide.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/trellis_ide_tab_drag.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/trellis_desktop.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/trellis_ops.e2e --headless
```

## Afterhours promotion gate

Future candidate only. Upstreaming would mean an opt-in afterhours
tiling/workspace plugin (layout tree + solver + camera + drag/drop) that
other consumers can drive without WM's gallery-shell assumptions
(launcher safe-area, HUD reserve, Fredoka chrome). Promote only after a
second consumer outside these example screens wants the same workspace
semantics, and file the gap with this header as the ready prototype.
Do not edit afterhours/vendor for this plugin without separate review.
