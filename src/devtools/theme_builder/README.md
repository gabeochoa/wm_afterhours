# ThemeBuilder devtool kit model (WM-local plugin)

Batch 2, Lane D — pluginization extraction. This plugin is the **non-UI
model** of the ThemeBuilder devtool lab: the state/logic seams that do
not need a `UIContext`, widgets, ECS, or rendering. It is the only file
of its kind in this directory — all UI, rendering, popover, and inspector
behavior stays in the screen, which is the single consumer.

**Owner: WM-local.** No Afterhours-upstream claim. No `vendor/afterhours`
edits were made for this extraction, and none are needed to keep it local.

## Files

| File | Role |
|---|---|
| `theme_builder_model.h` | Header-only model (`namespace wm::devtools::theme_builder`). No `.cpp` to link. |
| `../theme_builder/README.md` (this file) | Plugin card: purpose, public API, consumer, promotion gate. |
| `src/testing/tests/ThemeBuilderModelTest.h` | Pure unit tests (`--run-all-tests`). |

## What was extracted (and what did not move)

| Model piece | Consumed by |
|---|---|
| `TokenRow`, `kTokens[12]`, `kTokenCount` | `ThemeBuilderLab` table, columns, picker, inspector, history |
| `LinkTable`, `can_link()`, `link_root()` | Link picker cycle prevention + resolved-color walk |
| `resolve_theme()` | Live-preview theme; identical fn flattens the exported `.theme` file |
| `radius_px()` / `ring_px()` / `radius01_for()` / `ring01_for()` | Shape readouts, preview derivation, import inverses |
| `resolve_export_path()` | Theme-file export/import path |
| `Snapshot`, `capture_snapshot()`, `restore_snapshot()`, `snapshots_equal()` | Undo/redo frames |
| `History` (`record_edit`, `undo_to`, `redo_to`) | Gesture coalescing and undo/redo step semantics |

All UI remains in `src/systems/screens/ThemeBuilderLab.h`: the token
table layout, swatches, link-picker popover, shape sliders, preview
page, inspector hit-testing, and export/import buttons are unchanged,
as are every `tb_*` debug label the E2E script drives. The refactor is
behavior-preserving: same tokens, same link rules, same colors, same
history cap (100 steps, one gesture per undo step), same file paths.

## Public API

```cpp
#include "devtools/theme_builder/theme_builder_model.h"
using namespace wm::devtools::theme_builder;

Theme resolved = resolve_theme(edited, link, radius01, ring01);
bool ok = can_link(link, from, to);          // cycle prevention
int root = link_root(link, token);           // follow to the root
std::string p = resolve_export_path(path, save_dir);
Snapshot snap = capture_snapshot(edited, link, radius01, ring01);
restore_snapshot(snap, edited, link, radius01, ring01);
history.record_edit(frame_start, frame_end, suppressed);
```

## Consumer

- `ThemeBuilderLab` (`src/systems/screens/ThemeBuilderLab.h`) — the
  only consumer. It keeps the fields and the whole `for_each_with`
  render; only the model calls above replace inline logic.

## Promotion gate

**Do not promote until the lab's remaining WM-local dependencies have
upstream primitives.** A future Afterhours candidate may take this model
only when AH ships the missing primitives the lab currently works around
inline (all observable in `ThemeBuilderLab.h`, none of them edited here):

- **Gap inheritance**: the lab stacks columns with explicit gaps because
  `hstack`/`vstack` gaps do not inherit to child rows; AH needs an
  inheritable gap/justify/align intent query before the geometry stays
  upstreamable.
- **Text-inset margin**: AH labels apply no inset by default
  (`AFTERHOURS_DEFAULT_TEXT_INSET`); the lab's readable labels rely on
  explicit padding. Upstream needs an inset convention that matches the
  lab's rendering byte-for-byte.
- **Shared-popover suppression**: the lab keeps exactly one link-picker
  popover open at a time and closes swatch palettes when it opens; AH
  needs shared-popover suppression/ownership before the popover logic
  moves. Do not edit AH to get there — the plugin stays WM-local until
  those primitives exist.

Until then the plugin is pinned WM-local: the model header is the
contract, and `ThemeBuilderModelTest.h` plus E2E
`tests/e2e_scripts/368_theme_builder.e2e` pin it.

## How to use

```cpp
#include "devtools/theme_builder/theme_builder_model.h"
auto resolved = wm::devtools::theme_builder::resolve_theme(
    edited, link, radius01, ring01);
```
