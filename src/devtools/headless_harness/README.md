# Headless screenshot + baseline validation harness devtools plugin (WM-local)

Batch 4 Lane K — pluginization extraction (deferred Batch 3 item).
Behavior-preserving relocation only: no shot, threshold, log line, CLI
flag, output path, or API name changed. No `vendor/afterhours` edits were
made, and none are needed to keep this plugin WM-local.

**Owner: WM-local.** Not claimed Afterhours-upstream. Do not promote until
the promotion gate below is met (AH capture-stability / tolerance /
baseline-compare API only).

## Purpose

Named WM-local home for the two headless capture helpers that used to
live scattered (`src/headless_screenshots.*` in `src/`,
`src/testing/screenshot_validation.*` under `src/testing/`):

- `headless_screenshots` — offscreen screenshot generation + the headless
  test runner: `run_headless_screenshots()` (all screens, each
  `g_headless_resolutions` entry, `{screen}_{label}.png` to
  `g_headless_output_dir`), `run_all_tests_headless()` (all registered
  coroutine tests, returns failure count), `run_focus_ring_test()`
  (tab-through PNGs under `focus_test/{screen}/`), and
  `run_layout_summary()` (flat layout dump for one screen, stdout or file).
- `screenshot_validation` — baseline visual-regression compare:
  `calculate_image_diff_percentage(path1, path2)` (share of visibly-changed
  pixels, `0.0` identical / `100.0` different or unreadable),
  `save_screenshot_to(path)` (render-texture grab), and
  `validate_screen_against_baseline(screen)` (≤1% threshold; honours the
  `set_update_baselines()` refresh mode).

## Canonical files (in this directory)

| File | Role |
|---|---|
| `headless_screenshots.h` / `headless_screenshots.cpp` | Canonical headless capture + headless test runner. Byte-identical to the pre-move file except relative includes (`../../preload.h`, `../../font_config.h`, `../../game.h`, `../../input_mapping.h`, `../../log.h`, `../../settings.h`, `../../systems/*`, `../../testing/*`). Globals owned by `main.cpp`/`game.cpp` are unchanged (`g_headless_mode`, `g_headless_output_dir`, `g_headless_resolutions`, `g_headless_screen_filter`, `g_headless_capture`, `mainRT`/`screenRT`/`uiFont`/`g_current_screen`). |
| `screenshot_validation.h` / `screenshot_validation.cpp` | Canonical baseline compare. Header byte-identical; `.cpp` byte-identical except relative includes (`../../log.h`, `../../rl.h`). Tolerance, diff metric, and baseline paths unchanged. |

### Build note — why transport TUs exist

`makefile` `MAIN_SRC` globs only `src/*.cpp`, `src/components/*.cpp`,
`src/systems/*.cpp`, `src/ui/*.cpp`, `src/testing/*.cpp`, `src/engine/*.cpp`.
A `.cpp` under `src/devtools/**` is **not** compiled. To keep the canonical
source as the single source of truth with zero build-system risk and no
makefile change, the old `src/headless_screenshots.cpp` and
`src/testing/screenshot_validation.cpp` are transport TUs that `#include`
the canonical header + canonical `.cpp`. The canonical `.cpp` files are
compiled exactly once through those translation units. Adding a
`src/devtools/**/*.cpp` glob later would double-compile these units — if
that glob is ever added, delete the transport TUs in the same commit.

### Compatibility (old locations are shims, not removals)

| Old path (shim) | Canonical path | How existing code resolves |
|---|---|---|
| `src/headless_screenshots.h` | `src/devtools/headless_harness/headless_screenshots.h` | `#include "headless_screenshots.h"` / `"devtools/headless_harness/headless_screenshots.h"` forwards to canonical. |
| `src/testing/screenshot_validation.h` | `src/devtools/headless_harness/screenshot_validation.h` | `#include "testing/screenshot_validation.h"` forwards to canonical. |

`src/headless_screenshots.cpp` and `src/testing/screenshot_validation.cpp`
are transport TUs (see Build note). `src/main.cpp`, `src/game.cpp`,
`src/devtools/wm_devtools.h`, and `src/devtools/devtools_manifest.h`
(all consumers / registry) are untouched — they keep working through the
shims. The manifest still names the old `src/` paths by design; the
canonical paths above are the source of truth.

## Public APIs

```cpp
#include "headless_screenshots.h"  // or "devtools/headless_harness/headless_screenshots.h"

extern bool g_headless_mode;                       // set by main.cpp
extern std::string g_headless_output_dir;          // default "output/"
struct HeadlessResolution { int width, height; std::string label; };
extern std::vector<HeadlessResolution> g_headless_resolutions;  // empty → 720p only
extern std::string g_headless_screen_filter;       // --screen filter
struct HeadlessCaptureOptions { int frames=120; float dt=1.0f/60.0f; bool instant_animations=false; };
extern HeadlessCaptureOptions g_headless_capture;

void run_headless_screenshots();
int  run_all_tests_headless();  // 0 = all passed
void run_focus_ring_test(const std::string &screen_filter, int max_tabs, bool audit=false);
int  run_layout_summary(const std::string &screen_name, int width, int height,
                        int frame_count, const std::string &output_path);

#include "testing/screenshot_validation.h"  // or "devtools/headless_harness/screenshot_validation.h"
namespace screenshot_validation {
  void set_update_baselines(bool update);
  bool is_update_baselines();
  float calculate_image_diff_percentage(const std::string &path1, const std::string &path2);
  bool save_screenshot_to(const std::string &path);
  bool validate_screen_against_baseline(const std::string &screen_name);  // ≤1% passes
}
```

Consumers (unchanged — they include the old paths, which resolve via the
shims):

- `src/main.cpp` — `--headless-screenshots [dir]`, `--image-backend`,
  `--resolution`, `--capture-frames`, `--capture-dt`,
  `--instant-animations`, `--screen`, `--run-all-tests`,
  `--layout-summary`, `--focus-test`, `--update-baselines`.
- `src/game.cpp` — `screenshot_validation::set_update_baselines()`,
  `save_screenshot_to()`, `validate_screen_against_baseline()`
  (`validate_screen` E2E command).
- `src/devtools/wm_devtools.h` — aggregate include (old paths);
  `src/devtools/devtools_manifest.h` — `headless_screenshots` +
  `screenshot_validation` descriptors (read-only in this batch).
- `src/devtools/layout_audit/focus_audit.cpp` —
  `screenshot_validation::save_screenshot_to()` (focus-audit crop).
- `src/testing/tests/DevtoolsManifestTest.h` — descriptor presence.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --headless-screenshots <dir> --screen buttons
./output/ui_tester.exe --layout-summary buttons
```

`--validate-screenshots` / E2E `validate_screen` runs the full baseline
compare via `screenshot_validation`. Any pre-existing drift on this
batch's base must be reproduced on clean `origin/main` rather than
refreshing baselines here — zero baseline changes in this PR.

## Promotion gate — AH gap candidates left

**Do not promote until a future AH supported headless-capture API
exists upstream.** Specifically:

- Capture stability: a deterministic offscreen settle loop (frames →
  still image) with font-atlas headless loading, ECS singleton setup, and
  multi-resolution (`HeadlessResolution`) support, exposed as a supported
  AH testing API rather than WM's private `g_headless_*` globals.
- Tolerance: the ≤1% visibly-changed-pixel diff metric and its
  unreadable/size-mismatch → 100% contract, with a configurable threshold
  plumbed through an AH query/command rather than compiled into
  `validate_screen_against_baseline`.
- Baseline-compare API: baseline directory layout, `--update-baselines`
  refresh mode, and the `validate_screen` E2E surface as supported AH
  test infrastructure.

Until then this plugin is pinned WM-local: the canonical headers are the
contract, and `make -j4`, `--run-all-tests`, and the headless screenshot
smoke above pin it.
