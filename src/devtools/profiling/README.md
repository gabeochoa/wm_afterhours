# Profiling local plugin

WM-local plugin (Batch 3 Lane H). Not afterhours upstream. No
`vendor/afterhours` edits were made for this extraction, and none are
needed to keep it local.

## Purpose

Named WM-local home for WM's profiling chrome: the per-frame stats ring
that stress labs read, and the live profiler overlay that wraps
Afterhours' collector/panel in WM's input, navigation, and panel
chrome. Behavior-preserving relocation only: no overlay visual, layout,
label, debug name, input gate, refresh cycle, shortcut hint, font, or
`FrameStats` math changes.

Layout choice: `src/devtools/profiling/` (not `src/plugins/`) because
this is devtool chrome — it is already registered in the devtools
manifest (`frame_stats`, `profiler_overlay` in
`src/devtools/devtools_manifest.h`, read-only in this batch) and is
consumed through `src/devtools/wm_devtools.h`. The sibling blur pass
lives in `src/plugins/render_passes/` because it is a render pass, not
a devtool (see that README).

## Public API

Canonical headers (in this directory):

- `frame_stats.h` — `include "devtools/profiling/frame_stats.h"`
  - `FrameStats` — header-only, stdlib-only ring.
    - `CAPACITY = 120`, `ring`, `head`, `count`, `frame`
      (`unsigned long`), `fps`, `avgMs`, `p95Ms`, `maxMs`
    - `push(float dt)` — ignores `dt <= 0`; stores `dt * 1000` ms in the
      120-slot ring; recomputes `avgMs` / `p95Ms` / `maxMs` / `fps`
      only every 15 frames (`frame % 15`), by sorting a copy of the
      live ring. `p95Ms` is the `0.95 * (count - 1)` sorted index;
      `fps = 1000 / avgMs`. This math is unchanged by the move.
- `ProfilerOverlay.h` — `include "devtools/profiling/ProfilerOverlay.h"`
  - `ProfilerOverlay : afterhours::System<afterhours::ui::UIContext<InputAction>>`
    - `explicit ProfilerOverlay(bool start_recording = false)` — if
      `start_recording`, starts `afterhours::profiling::default_collector()`.
      Also sets `show_launcher`.
    - `state` (`afterhours::ui::imm::ProfilerState`), `navigation_open`
      (`std::function<bool()>`), `visible`, `show_launcher`,
      `browser_was_open`, `refresh_rate_index`,
      `refresh_rates{0, 120, 60, 30}` (0 = every frame).
    - `shortcut()` — ToggleProfiler binding label via
      `ProvidesLayeredInputMapping` / `InputCollector` / `input_prompts`.
    - `for_each_with(...)` — ToggleProfiler press toggles `visible` +
      `show_launcher`; ESC hides when visible (unless the screen
      browser is open); removes/applies the `wm_profiler` input gate
      (subtree + render-layer >= 2000 pass-through); launcher button
      `wm_profiler_open` ("Profiler (hint)", 200x32, render layer 1500,
      AtkinsonMock 18); panel `wm_profiler_panel` (<=1120 wide,
      `min(720, height-80)` high, at 16,48, layer 1500); header
      "Live profiler / <current screen>", refresh button
      `wm_profiler_refresh` cycling `refresh_rates`, hide button
      `wm_profiler_hide`; "Recording across screens" /
      "Recording stopped" line; `ProfilerOptions{refresh_hz,
      font_size=20, show_chart = panel_height >= 600, max_rows =
      clamp((panel_height - 450/260)/36, 1, 6)}`; renders
      `afterhours::ui::imm::profiler_panel(context, ..., collector,
      state, options, ...)` under AtkinsonMock.

Compatibility: `src/frame_stats.h` and `src/systems/ProfilerOverlay.h`
are shims including the canonical headers above. Every existing
include path and API is preserved: `AnimationStressLab` /
`CurvedTextStressLab` (`../../frame_stats.h`), `game.cpp`
(`systems/ProfilerOverlay.h`), and `src/devtools/wm_devtools.h`
(`../frame_stats.h`, `../systems/ProfilerOverlay.h`) compile
unmodified.

## Current consumers

- `AnimationStressLab` (`src/systems/screens/AnimationStressLab.h`) —
  `FrameStats stats; stats.push(dt)` when not paused; displays
  `fps`/`avgMs`/`p95Ms`/`maxMs` in `anim_stress_stats`.
- `CurvedTextStressLab` (`src/systems/screens/CurvedTextStressLab.h`) —
  `FrameStats stats; stats.push(dt)` when not paused; displays
  `fps`/`avgMs`/`p95Ms`/`maxMs` in `curved_stress_stats` alongside its
  own measure/layout/draw stage timings.
- `game.cpp` — constructs `ProfilerOverlay(profile)` twice (two
  construction sites, lines ~545 and ~814), wires `navigation_open`,
  registers the system.
- `src/devtools/wm_devtools.h` — aggregate include (via the shims);
  `DevtoolsManifestTest.h` exercises `FrameStats::push` through it.
- `SystemProfileLab` (`src/systems/screens/SystemProfileLab.h`) — does
  **not** consume this plugin; it uses Afterhours'
  `profiler_panel` + `default_collector()` directly with its own
  statics (`spl_*` labels). Listed here because it is the other
  profiling screen and the natural comparison point for the promotion
  gate below.

## What Afterhours already covers (read-only vendor comparison)

Read from `vendor/afterhours/` in this checkout; no vendor edits:

- `src/plugins/profiling.h` — `afterhours::profiling`: `Options`,
  `SystemSample`, `Counter`, `Spike`, `Snapshot` (`frame_ms`, fps /
  average / p50 / p95 / p99, spikes, systems, counters, frames,
  dropped samples, optional `cpu_percent`/`resident_mb`),
  `select_frames`, `Collector` (`start`, `recording`, per-system and
  counter recording), `default_collector()`,
  `AFTERHOURS_PROFILE_COUNTER`. `available` is compile-time
  (`AFTERHOURS_ENABLE_PROFILING`).
- `src/plugins/process_metrics.h` — `afterhours::profiling::
  ProcessMetrics` + `process_metrics()`: `cpu_seconds` /
  `resident_mb`, macOS (`getrusage` / `task_info`) only; empty
  elsewhere. The collector folds this into `Snapshot.cpu_percent` /
  `resident_mb` when `sample_process` is on.
- `src/plugins/ui/profiler.h` — `afterhours::ui::imm`:
  `ProfilerOptions` (`show_chart/systems/counters`, `max_rows`,
  `refresh_hz`, `font_size`), `ProfilerState` (paused, sort, snapshot,
  selection, chart rect/id, anchor/selecting), `profiler_view` and
  `profiler_panel` (FPS/avg/p50/p95/p99 summary, process CPU/memory,
  line chart, system table, counter table). This plugin does not
  reimplement the panel; `ProfilerOverlay` is the WM chrome around it.

What stays WM-local here: the `FrameStats` 120-ring / every-15-frames
math (AH `Snapshot` percentiles are over the collector's frame
history, not a 120-slot UI ring), and all of `ProfilerOverlay`'s
chrome (ToggleProfiler binding + shortcut hint, `wm_profiler` input
gate, ESC/browser interaction, launcher button, panel geometry,
refresh-rate cycle, AtkinsonMock pinning). `SystemProfileLab`'s
curated slot layout is likewise WM-local markup over the upstream
panel.

## Promotion gate

No promotion in this batch. A future Afterhours candidate may take
this plugin only when **all** of the following hold, proven by a
curated overlay snapshot and the scripts below — not by a passing
build alone:

- **Curated overlay snapshot**: `156_live_profiler.e2e` (F3 open,
  `wm_profiler_panel` 1120x640 at 1280x720 and 1120x720 at 1920x1080,
  refresh cycle every-frame -> 120 -> 60 -> 30 -> every-frame,
  record/stop, LIVE pause/resume, sort recent/overall, chart
  selection, ESC hide, browser-open suppression, launcher reopen)
  passes byte-identical against the pre-move baseline screenshots,
  with zero screenshot diffs. Any geometry, label, debug-name, or
  font change fails the gate.
- **FrameStats parity**: `snapshot`-equivalent check that the
  120-ring / every-15 / p95-index math matches the current header's
  outputs on a fixed `dt` sequence (the `DevtoolsManifestTest`
  sanity — `p95Ms == 0` after one push — is necessary but not
  sufficient; a promotion must pin fps/avg/p95/max over >= 30 pushes).
- **Collector/process parity**: no change to how
  `default_collector()` is started/stopped or how
  `process_metrics()` (macOS-only) surfaces in the panel; the
  `AFTERHOURS_ENABLE_PROFILING=0` compiled-out path (early return in
  `for_each_with`, "Profiling compiled out" in `SystemProfileLab`)
  must still compile and render.

Until then the canonical headers above are the contract, and
`--run-all-tests` plus `156_live_profiler` / `154_system_profile_lab`
/ `351_animation_stress_lab` / `352_curved_text_stress_lab` pin them.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/156_live_profiler.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/351_animation_stress_lab.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/352_curved_text_stress_lab.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/154_system_profile_lab.e2e --headless
```

No new unit test was needed: `DevtoolsManifestTest.h` already covers
`FrameStats::push` through the shim chain
(`wm_devtools.h` -> `src/frame_stats.h` -> canonical), and the four
e2e scripts above cover overlay, stress labs, and system profile.
