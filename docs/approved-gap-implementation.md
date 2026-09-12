# Approved gap implementation

The user requested all approved gaps, each in its own local commit. No pushes.
Apply review feedback by rewriting the original relevant commit, preserving
later changes. Add no code comments; leave existing comments unchanged unless
they become false. Prefer early returns. Run work with `nice -n 10`, builds
with `-j2`.

Each library change has its own afterhours commit and a wm tracking commit.
Application integration is recorded separately below.

| Work | Status |
|---|---|
| UP-01 Audio gains | Complete, afterhours `1fce0d9` |
| UP-02 Settings saves | Complete, afterhours `325caee` |
| UP-03 Scoped test clipboard | Complete, afterhours `b81173b`; 15/15 checks including both text widgets |
| UP-04 Native dialogs, portable API, macOS first | Complete, afterhours `544ea06`; queue and real macOS cancellation tests pass |
| UP-05 Complete binding persistence | Complete, afterhours `00a6745`; codec checks pass with no backend and raylib |
| UP-06 Automatic binding prompts with device override | Complete, afterhours `bcefe22`; modifiers, axis direction, device switching, noise, override and remapping checks pass |
| UP-09 Filesystem watcher, portable API, macOS first | Complete, afterhours `f4de4d3`; real filesystem lifecycle and overflow checks pass |
| UP-10 Explicit capture formats | Complete, afterhours `6eea5bd`; raylib/Metal raw and PNG pixel checks pass |
| UP-11 Default profiling UI and compile-out support | Complete, afterhours `5a8e3be`; bounded collector, independent hooks, compile-out and real panel checks pass; rendering and recording measurements documented |
| UP-12 Charts required by profiler, interactive wm test screen | Complete, afterhours `dff9c96`; data checks, wm pointer interactions and visual capture pass |
| wm searchable screen tree | Complete; pure model checks, pointer/keyboard E2E, 720p/1080p, current-screen marker and unique runtime cycle pass |

UP-07 and UP-08 remain deferred. Sound-feedback and periodic timers remain
skipped. The wider chart set remains TODO. Performance investigations and
older visual gaps retain their separate recorded scope.

The plugin architecture proposal is in `docs/plugin-architecture-proposal.md`.
That work is a plan only, not a library migration.

Verification before the live wm integration on September 12: the full afterhours suite exited 0, and
wm screenshot validation passed 117/117. The full wm E2E run passed 154/155;
its only failure was the profiler's focus-state baseline. After correcting that
test's setup, it passed alone and in a two-script batch after keyboard input,
with a 0.0000% screenshot difference in both runs. The other scripts and
application code did not change after that full run. The complete suite was
not repeated after the test-only correction.

The screen-tree model checks pass. Runtime cycles in both directions visit
all 117 registered screens once before wrapping. Profiling-disabled wm syntax
checks pass; collector and real-renderer benchmark results and limits are in
`vendor/afterhours/docs/profiling-measurements.md`.


## Live wm integration

The normal application now uses the shared audio gains, atomic settings saves,
explicit PNG capture and binding-prompt APIs. Audio preferences remain independent
when master volume changes. Failed settings writes preserve the previous file and
return failure. Capture failures are reported, and resize updates both render
targets and the resolution used by wm screens, including in headless tests.

`make run` enables profiling, starts collection and opens the library profiler
panel over the current screen. F3 hides or shows it. Recording continues across
screen changes and while hidden. The panel uses the library timing chart and
reports process CPU, resident memory, frame history and system timings. Build
configuration changes invalidate objects when profiling is enabled or compiled
out. Usage is in `docs/live-profiling.md`.

Audio preference integration and failed-save preservation checks pass. A native
`make run` session verified recording, live CPU/RAM values and F3 visibility.
The new overlay E2E covers input blocking, recording controls, browser navigation,
screen changes and 720p/1080p layout. PNG captures were inspected at their actual
1280×720 and 1920×1080 dimensions. Profiling-disabled overlay syntax and build
configuration invalidation checks pass.

The full integration E2E run passed 154/156. Its two failures exposed wm resize
bugs in the absolute-positioning and nested-scroll demos. After fixing those
screens, both complete parent-containment scripts and the live-profiler script
passed together (3/3). The full 156-script suite was not repeated after those
two screen-only fixes.

Screenshot validation rendered all 117 screens. The nested-scroll screen's
baseline was refreshed after visual inspection: its viewport now uses actual
pixel dimensions instead of scaling them again. The other 116 baselines passed
unchanged.


Profiler review replaced lifetime totals and per-call means in the panel with
recent and overall average milliseconds per completed frame. Recent averages
use the configured history, 600 frames by default; skipped frames count as zero
and repeated calls are combined. Sorting defaults to recent average cost.
Collector tests pass 61/61 checks, compile-out checks pass, and profiler E2E
scripts 39, 154 and 156 pass together. The updated 720p baselines and the live
720p/1080p panel were inspected. Recording still performs zero steady-state C++
allocations in the three-run benchmark; measurements and limits are recorded
in `vendor/afterhours/docs/profiling-measurements.md`.


Profiler readability and refresh review enlarged the overlay and lab text,
including chart labels, and replaced the fixed 4 Hz refresh with every-frame
updates by default. The wm header cycles through every frame, 120, 60 and
30 Hz. Library callers can set refresh rate and font size in `ProfilerOptions`.
The recent averages now use running sums to avoid rescanning system histories
on refresh. Collector and refresh tests pass 67/67 checks; compile-out and
chart checks pass. Profiler/chart E2E scripts 39, 153, 154 and 156 pass together,
including rate selection and 720p/1080p containment. The chart baseline remains
byte-identical; the profiler baselines were inspected and refreshed. Both layout
and real-renderer refresh benchmarks are documented in afterhours. The underlying
grid/content-height discrepancy is recorded in `docs/AFTERHOURS_GAPS.md`.
