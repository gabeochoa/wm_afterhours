# WM devtools plugin package (Batch P-C)

Additive manifest/registry layer over the existing WM-local devtools
surfaces. Nothing here moves implementation, rewires a call site, edits
`vendor/afterhours`, or changes screenshot baselines. The registry is
`devtools_manifest.h` (`namespace wm::devtools`); this file mirrors it
for humans. If they ever disagree, the header wins and
`DevtoolsManifestTest.h` fails until the README is re-aligned.

## Files

| File | Role |
|---|---|
| `src/devtools/devtools_manifest.h` | Stdlib-only registry: `PluginDescriptor[]`, `find()`, `count_by_owner()`, `descriptors_complete()`. No side effects. |
| `src/devtools/wm_devtools.h` | Aggregate compatibility include: manifest + the 11 existing public headers, verbatim. No moved implementation. |
| `src/testing/tests/DevtoolsManifestTest.h` | Compile + registry smoke test, run by `./output/ui_tester.exe --run-all-tests`. |

## Manifest (11 plugins)

| Id | Header | Owner | Public entry points | Promotion gate | Test entry |
|---|---|---|---|---|---|
| headless_screenshots | `src/headless_screenshots.h` | WM-local | `run_headless_screenshots`, `run_all_tests_headless`, `run_focus_ring_test`, `run_layout_summary` | Multi-resolution stability: 480p/720p/1080p byte-stable across 3 consecutive headless runs | `--headless-screenshots`; `--run-all-tests` |
| screenshot_validation | `src/testing/screenshot_validation.h` | WM-local | `set_update_baselines`, `calculate_image_diff_percentage`, `validate_screen_against_baseline` | Threshold parity (`DIFF_THRESHOLD_PCT` 0.05%, >8/255 delta) with AH `screenshot_baseline.h` + drift policy | `--run-all-tests`; `make validate-screenshots` |
| layout_summary | `src/testing/layout_summary.h` | WM-local | `write`, `write_to_file` | DO NOT promote until layout-intent API upstream | `--layout-summary <screen>` |
| ui_tree_dump | `src/testing/ui_tree_dump.h` | WM-local | `build`, `size_json` | DO NOT promote until layout-intent API upstream (desired/min/max, flex, gap query) | `--layout-summary <screen>`; `--dump-ui-json` |
| focus_audit | `src/testing/focus_audit.h` | WM-local | `snapshot`, `write`, `capture_crop`, `register_systems`, cmd `audit_ui` | E2E runner owns audit flag | `--focus-test --focus-audit`; `--run-all-tests` |
| frame_stats | `src/frame_stats.h` | WM-local | `FrameStats::push` | AH `Snapshot` percentile parity with 120-ring / every-15-frames math | `--run-all-tests` (manifest test) |
| profiler_overlay | `src/systems/ProfilerOverlay.h` | WM-local | `ProfilerOverlay` system | AH chrome parity: `wm_profiler` input-gate, refresh-rate cycle, shortcut hint (panel + collector already upstream) | `--profile` (F3); `--run-all-tests` |
| e2e_integration | `src/testing/e2e_integration.h` | WM-local (shim) | `register_e2e_systems`, `configure_runner`, re-exported arg parsing | AH app-command registration hook | `make test` (`.e2e` headless) |
| e2e_commands | `src/testing/e2e_commands.h` | WM-local | `HandleOpen/Next/Prev/Goto/ValidateScreenCommand` | AH `screen_commands.h` covers WM screen registry (app-specific; may stay WM-local) | `make test` |
| apply_blur_pass | `src/systems/ApplyBlurPass.h` | WM-local | `ApplyBlurPass::once` | PRECISE: AH `BlurPass` must expose direct-to-window scale path (GetRenderWidth/GetScreenWidth) + mainRT path parity + `blur_requests()` ownership; pixel-match <=0.05% before call-site moves. Manifest-only. | Headless screenshot (blur screen); `make validate-screenshots` |
| font_config | `src/font_config.h` | WM-local | `get_all_fonts`, `get_*_codepoints`, `aliases`, `FontDef` | PRECISE: AH `FontCatalog` must accept raster_size + codepoint callbacks, alias/@bold convention, per-screen scoping; KR/JP/fullwidth ranges round-trip without '?' fallback. Manifest-only. | `FontConfigTest.h`; `--run-all-tests` |

All 11 are owner `wm::devtools::Owner::WmLocal`. None is claimed as
Afterhours-upstream; the upstream column in `notes` names the vendored
counterpart already present (e.g. `profiling.h`, `ui/profiler.h`,
`e2e_testing/*`, `font_catalog.h`, `font_coverage.h`) so a future
promotion can diff against it.

ApplyBlurPass and font_config are included with precise promotion
criteria (above) and explicitly do not refactor their call sites in
this batch.

## Deferred items (explicitly not in this batch)

- **Headless multi-resolution stability**: 480p/720p/1080p byte-stability
  across repeated runs is unverified; headless_screenshots stays WM-local
  behind that gate. No baseline changes were needed or made.
- **Layout-intent API upstream**: `layout_summary` and `ui_tree_dump`
  remain WM-local until AH exposes an intent query; geometry-only replay
  is not promotable.
- **Font registry upstream**: `font_config` remains WM-local until the
  FontCatalog gates above (raster_size + codepoint callbacks, alias/@bold,
  per-screen scoping, CJK round-trip) are met.

## How to use

```cpp
#include "devtools/devtools_manifest.h" // registry only (stdlib-only)
auto *d = wm::devtools::find("apply_blur_pass");
```

For the implementation headers, include `devtools/wm_devtools.h`
instead. Both are header-only; there is no `.cpp` to link until a
future batch introduces one.
