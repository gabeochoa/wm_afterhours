# Layout / focus audit devtools plugin (WM-local)

Batch 3 Lane G — pluginization extraction. Behavior-preserving relocation
only: no query, snapshot, geometry, focus-ring, JSON output, CLI flag, or
screen behavior changes. No `vendor/afterhours` edits were made, and none
are needed to keep this plugin WM-local.

**Owner: WM-local.** Not claimed Afterhours-upstream. Do not promote until
the promotion gate below is met (AH layout-intent / focus-audit API only).

## Purpose

Named WM-local home for the three layout/focus devtools helpers that used
to live scattered under `src/testing/`:

- `layout_summary` — flat named-element bounds dump (headless, stdout or
  file) used by `--layout-summary` and screenshot-free layout QA.
- `ui_tree_dump` — JSON dump of the live UI tree with **layout intent**
  (size specs, flex direction, justify/align, gap, font resolution), so a
  second layout engine can solve the same input independently.
- `focus_audit` — focus-ring / focusability snapshot (JSON + optional
  cropped PNG), remote `audit_ui` E2E command, and `register_systems()`.

## Canonical files (in this directory)

| File | Role |
|---|---|
| `layout_summary.h` / `layout_summary.cpp` | Canonical flat layout summary. `namespace layout_summary`: `write(std::ostream&)`, `write_to_file(path)`. |
| `ui_tree_dump.h` | Canonical header-only UI tree dump. `namespace ui_tree_dump`: `size_json(size)`, `build(entity, cmp)`, `build_all()`. |
| `focus_audit.h` / `focus_audit.cpp` | Canonical focus audit. `namespace focus_audit`: `snapshot(bool)`, `write(path, data)`, `capture_crop(path, sample)`, `register_systems(systems)`; E2E command `audit_ui`. |

Relative includes were adjusted for the deeper location only
(`../../font_config.h`, `../../input_mapping.h`, `../../game.h`,
`../../settings.h`, `../../testing/screenshot_validation.h`,
`../../../vendor/magic_enum/magic_enum.hpp`). Canonical headers are
otherwise byte-identical to the pre-move files; canonical `.cpp` files are
byte-identical except for those include lines. No logic, JSON shapes, or
output formats changed.

### Build note — why transport TUs exist

`makefile` `MAIN_SRC` globs only `src/*.cpp`, `src/components/*.cpp`,
`src/systems/*.cpp`, `src/ui/*.cpp`, `src/testing/*.cpp`, `src/engine/*.cpp`.
A `.cpp` under `src/devtools/**` is **not** compiled. To keep the canonical
source as the single source of truth with zero build-system risk and no
makefile change, the old `src/testing/layout_summary.cpp` and
`src/testing/focus_audit.cpp` are transport TUs that
`#include` the canonical header + canonical `.cpp`. The canonical `.cpp`
files are compiled exactly once through those translation units. Adding a
`src/devtools/**/*.cpp` glob later would double-compile these units — if
that glob is ever added, delete the transport TUs in the same commit.

### Compatibility (old locations are shims, not removals)

| Old path (shim) | Canonical path | How existing code resolves |
|---|---|---|
| `src/testing/layout_summary.h` | `src/devtools/layout_audit/layout_summary.h` | `#include "testing/layout_summary.h"` / `"layout_summary.h"` forwards to canonical. |
| `src/testing/ui_tree_dump.h` | `src/devtools/layout_audit/ui_tree_dump.h` | Same; header-only, no transport TU needed. |
| `src/testing/focus_audit.h` | `src/devtools/layout_audit/focus_audit.h` | Same. |

`src/testing/layout_summary.cpp` and `src/testing/focus_audit.cpp` are
transport TUs (see Build note). `src/devtools/wm_devtools.h` and all other
call sites are untouched — they keep working through the shims.

## Public APIs

```cpp
#include "devtools/layout_audit/layout_summary.h" // or "testing/layout_summary.h"
namespace layout_summary {
  bool write(std::ostream &out);                    // flat name x y w h lines
  bool write_to_file(const std::string &path);      // creates parent dirs
}

#include "devtools/layout_audit/ui_tree_dump.h"     // or "testing/ui_tree_dump.h"
namespace ui_tree_dump {
  inline nlohmann::json size_json(const afterhours::ui::Size &s);
  inline nlohmann::json build(afterhours::Entity &entity,
                              afterhours::ui::UIComponent &cmp);
  inline nlohmann::json build_all(); // {"tree": [...]} for every AutoLayoutRoot
}

#include "devtools/layout_audit/focus_audit.h"      // or "testing/focus_audit.h"
namespace focus_audit {
  nlohmann::json snapshot(bool include_elements = false);
  void write(const std::filesystem::path &path, const nlohmann::json &data);
  bool capture_crop(const std::filesystem::path &path,
                    const nlohmann::json &sample);
  void register_systems(afterhours::SystemManager &systems);
}
```

Consumers (unchanged — they include the old paths, which resolve via the
shims):

- `src/headless_screenshots.cpp` — `layout_summary::write/write_to_file`
  (`run_layout_summary` / `--layout-summary <screen>`; defaults 1280x720,
  120 frames), `ui_tree_dump::build_all` (`--dump-ui-json <dir>`),
  `focus_audit::snapshot/write/capture_crop` (focus-ring test with
  `--focus-test --focus-audit`).
- `src/game.cpp` — `ui_tree_dump::build_all().dump(2)` (dump IPC),
  `focus_audit::register_systems(systems)`.
- `src/devtools/wm_devtools.h` — aggregate include (old paths).
- `src/testing/tests/DevtoolsManifestTest.h` — `layout_summary`,
  `ui_tree_dump`, `focus_audit` descriptor presence.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --layout-summary <screen>   # e.g. --layout-summary buttons
./output/ui_tester.exe --headless-screenshots <dir> # one headless screenshot smoke
```

`LayoutAuditTest.h` (`src/testing/tests/`) proves the canonical headers
compile, the old shim headers resolve to the same declarations, and the
`layout_summary` / `ui_tree_dump` / `focus_audit` entry points exist
unchanged. It is registered via `src/testing/tests/all_tests.h` and run by
`--run-all-tests`.

## Promotion gate

**Do not promote until a future AH supported layout-intent dump / focus
audit API exists upstream.** Specifically:

- Layout: AH must expose the intent query (`ui_tree_dump` already emits:
  desired/min/max specs, flex direction, justify/align, gap, font
  resolution, roundness/clip/scroll) through an AH query rather than by
  reading `UIComponent` fields directly. Geometry-only replay is not
  sufficient.
- Focus: AH must expose the focus-audit surface (`snapshot`, crop capture,
  `audit_ui`-equivalent E2E command, focus-ring geometry) as a supported
  API, not just the underlying `can_be_focused` / `focus_ring_for`
  internals WM currently calls.

Until then this plugin is pinned WM-local: the canonical headers are the
contract, and `LayoutAuditTest.h` plus `--run-all-tests` and the layout /
screenshot smokes above pin it.
