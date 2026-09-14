# Runtime baseline, September 13, 2026

The existing macOS `output/ui_tester.exe` reached a usable headless Buttons screen in at most 6.806 seconds on the first observed launch, then 4.926 and 4.491 seconds on warm repeats. These launches did not purge filesystem caches, so the first result is not a controlled cold-start measurement. The marker follows two rendered frames and includes audit and polling overhead.

Run the same bounded workload against another binary with:

```sh
nice -n 10 python3 scripts/profile_runtime.py \
  --binary /absolute/path/to/ui_tester.exe \
  --output /tmp/wm-runtime-measurement \
  --frames 120 --switch-cycles 3 --frame-metrics
```

The script creates separate run directories, copies settings, and links resources. It launches the application at niceness 10, samples RSS and CPU, records a JSON report, and leaves the original settings untouched. `WM_PROFILE_STARTUP=1` also collects per-font timings in binaries with that instrumentation.

## Measurements

The unprofiled workload renders 120 idle Buttons frames, 120 live Chart Lab frames, three rounds of switching between those screens, then another 120 Buttons frames in one process.

| Measurement | Existing binary |
|---|---:|
| Workload wall time including startup and audits | 9.576 s |
| CPU time including sampler subprocesses | 6.673 s |
| Average CPU including sampler | 69.7% |
| Sampled peak RSS | 759.0 MiB |
| Initial idle RSS | 756.5 MiB |
| Returned idle RSS | 630.6 MiB |
| Buttons audit element count, each visit | 65 |
| Chart Lab audit element count, each visit | 33 |

RSS did not grow monotonically across these six switches. This short sample does not establish absence of leaks, and visible audit element counts do not cover every retained entity or texture. The three startup runs peaked at 669.0, 810.3 and 756.7 MiB, illustrating substantial run-to-run variation under machine load and memory compression.

An additional run with the existing profiler enabled recorded 544 frames, averaging 7.92 ms/frame, with p50 4.73 ms, p95 8.51 ms and p99 152.17 ms. These are aggregate workload timings including navigation and screenshot audits. The expensive audit renderer averaged 2.930 ms in the profiler, so they must not be presented as steady-state idle frame times. This run took 10.638 seconds, used 7.134 CPU seconds including the sampler, and peaked at 816.8 MiB RSS. The final profiler snapshot reported 100.9% process CPU and 521.7 MB resident memory.

Headless software rendering runs uncapped. Its CPU consumption does not predict a vsynced window's idle CPU use. A macOS `sample` attempt timed out after 15 seconds and produced no usable stack profile.

## Changed build and same-workload comparison

CJK fonts now load immediately before their first consuming screen, rather than
before every first screen. The shared loader covers Language, Forms, Sync Scroll,
Mini Motorways settings and Localized Input Prompts. Each font stays loaded for the
rest of the process; there is no eviction policy. The Japanese Language sample uses
133 codepoints covering its fixed copy and ASCII, while other consumers retain their
existing full repertoires.

A later pair of measurements used the preserved old binary and the changed binary,
with the same 120/120/120-frame workload, three switch cycles, profiler and sampler.
The changed binary ran first; neither run overlapped our build or tests. Other users
of the machine and filesystem cache state were not controlled.

| Measurement | Preserved old binary | Changed binary |
|---|---:|---:|
| First observed ready upper bound | 4.797 s | 1.123 s |
| Warm ready upper bounds | 2.973 / 3.798 s | 1.098 / 0.922 s |
| Workload wall time, including startup/audits | 7.616 s | 5.696 s |
| CPU seconds including sampler | 6.562 s | 4.764 s |
| Sampled peak RSS | 816.2 MiB | 360.1 MiB |
| Initial idle RSS | 815.6 MiB | 359.3 MiB |
| Returned idle RSS | 532.0 MiB | 359.9 MiB |
| Profiled frames | 544 | 544 |
| Aggregate frame average | 7.18 ms | 7.80 ms |
| Frame p50 / p95 / p99 | 4.26 / 6.45 / 149.69 ms | 4.25 / 6.25 / 143.68 ms |

The warm-start average fell about 70% and sampled peak RSS about 56%. These results
support deferring unused fonts. They do not demonstrate an improvement in
steady-state frame time: the aggregate average increased slightly and includes
expensive screenshot audits. Old RSS dropping late in the run also illustrates why
resident memory alone cannot establish object lifetime or absence of leaks.

Both runs reported stable per-screen audit counts across repeat visits, 66 Buttons
and 34 Chart Lab elements with profiling enabled. The changed process finished
within 0.6 MiB of its initial idle RSS. Its startup log contains no CJK font loads.
All five consuming screens passed their interaction tests, and the all-screen
capture exercised each deferred font in one process. A separate instrumented test visited all five consumers twice and recorded exactly
one load each for NotoSansKR, NotoSansJP, Sazanami and NotoSansKRInline. Results are
in `output/wm-only-todos/font-repeat/summary.json`. Fonts stay resident when returning
to Latin-only screens.

Raw local reports are in `output/wm-only-todos/runtime-matched-before/report.json`
and `output/wm-only-todos/runtime-final/report.json`. Controlled cold starts
and retained GPU texture accounting remain open. Windowed measurements follow below.

## Artwork atlas checks

Cafe artwork went from five source textures to one 404×2020 padded atlas; image
specimens went from five to one 104×520 atlas. Two-pixel edge extrusion preserves
bilinear filtering. The generator independently reconstructs both files and checks
their exact pixels. Atlas storage adds 16,080 and 4,080 pixels respectively.

All eight Cafe artwork rectangles are pixel-identical to pre-change native
captures. Fourteen of fifteen image-screen artwork rectangles are identical; the
remaining scaled icon differs by one red-channel level in one pixel. This is
consistent with texture-coordinate rounding. Labels describing atlas coordinates
were updated separately. Headless resource cleanup was corrected during review.

This verifies appearance and resource consolidation. The driver measurement below
found no draw-call or texture-bind reduction.

## Driver measurement of the artwork atlases

`scripts/mac_gl_probe.c` is a WM-owned macOS OpenGL interposer. It counts
`glDrawArrays`, `glDrawElements` and `glBindTexture`, then records those counters
at `CGLFlushDrawable`. It requests and records swap interval 1 on each present.
Texture-bind calls include unbinds and redundant binds; they are not a count of
unique resident textures. Frame times are present-to-present wall times, including
probe overhead and waiting, not GPU execution times.

The before binary was built from `f5338fdd`, with only `CozyCafe.h` and
`ImageShowcase.h` restored from `a5cae19c`, the pre-atlas versions. A temporary
`AtlasComparisonScreens.h` included those two screens for a smaller build. Both
binaries used afterhours `c5cfd359967c6bb165fd40b6a2bc14d5009adc2f` and the
same resources/settings. The comparison is between the original screen revisions
and their atlas revisions, including their accompanying caption changes.

`scripts/measure_atlas.py` renders 360 requested frames, excludes the first 100
and final 30 presents, and requires nonzero draw counts and swap interval 1.
Each run retained 235 frames. Two runs per screen/version reversed the order on
the second repeat. These were reduced-priority runs on the shared machine without
our builds running concurrently. The temporary before worktree is removed after
measurement; the recipe above and the script allow rebuilding it.

| Screen | Original draws / binds | Atlas draws / binds | Original median ms, repeats | Atlas median ms, repeats |
|---|---:|---:|---:|---:|
| Cozy Cafe | 66 / 68 | 66 / 68 | 8.306 / 8.798 | 8.417 / 8.380 |
| Images | 59 / 61 | 59 / 61 | 8.864 / 8.677 | 8.187 / 8.432 |

The counts matched in both repeats. Cafe p95 ranged from 11.628 to 16.008 ms
before and 11.604 to 12.486 ms after; Images ranged from 12.152 to 12.452 ms before
and 12.347 to 12.350 ms after. These timings do not demonstrate an attributable
performance gain. Keeping artwork in fewer texture resources did not merge these
screens' submitted draws. The gaps file records the need to attribute state
boundaries before attempting a library batching change.

Build and run the probe with:

```sh
nice -n 10 clang -dynamiclib -Wno-deprecated-declarations scripts/mac_gl_probe.c \
  -framework OpenGL -o output/mac_gl_probe.dylib
nice -n 10 python3 scripts/measure_atlas.py \
  --before /absolute/path/to/before/ui_tester.exe --after output/ui_tester.exe \
  --probe output/mac_gl_probe.dylib --output output/atlas-measurement
```

The launcher applies niceness in the child before executing the app directly.
A protected system utility in front of the app can strip the DYLD instrumentation
environment. Both measurement scripts reject missing probe data. The probe is
optional and is neither linked into WM nor compiled into afterhours.
Raw CSVs and summaries are in `output/wm-followthrough/atlas-measurement/`.

## Windowed workload with verified swap interval

The final windowed run used the current WM binary, niceness 10, the GL probe,
360 frames each of Buttons, Chart Lab and returned Buttons, plus three switch
cycles. The profiler overlay was disabled. The probe checked swap interval 1 on
every recorded present. Python and the C probe use `CLOCK_MONOTONIC` so phase
markers share the same clock on macOS. Samples within 0.5 seconds of a phase
boundary are excluded from the frame-time table.

| Phase | Settled frames | Frame median / p95 | Draw / bind calls |
|---|---:|---:|---:|
| Buttons idle | 277 | 8.830 / 13.897 ms | 54 / 56 |
| Chart Lab active | 264 | 8.437 / 12.528 ms | 41 / 43 |
| Returned Buttons | 261 | 8.339 / 12.820 ms | 54 / 56 |

Startup upper bounds were 1.244, 1.830 and 1.219 seconds. The first is another
observed launch, not a cold cache. The workload took 13.386 seconds, used 8.966
CPU seconds including the sampler, and averaged 67.0% CPU over startup, audits,
navigation and frames. This is not an isolated idle CPU measurement. Peak sampled
RSS was 425.2 MiB; RSS after the initial idle phase was 370.0 MiB and after the
returned idle phase 370.5 MiB. Audit counts stayed at 65 for Buttons and 33 for
Chart Lab across all visits. This short run cannot establish absence of leaks.

No build or other test of ours overlapped this run. Other users' machine load,
display scheduling, memory compression and filesystem caches were uncontrolled.
The controlled cold-start/idle-machine experiment remains open. Earlier probe
trials were discarded because the app reset vsync or the clocks were mismatched.

```sh
nice -n 10 python3 scripts/profile_runtime.py --binary output/ui_tester.exe \
  --windowed --gl-probe output/mac_gl_probe.dylib --frames 360 --switch-cycles 3 \
  --output output/windowed-measurement
```

Raw proof is in `output/wm-followthrough/windowed-verified/report.json` and each
run's `gl.csv`. Invalid or missing instrumentation now fails the measurement
instead of silently producing a supposedly vsynced result.

## Process-only CPU during settled phases

The sampler now also reads `ps time`, the app's cumulative process CPU time.
Subtracting samples within each phase excludes CPU spent by the sampler itself.
Intervals omit 0.5 seconds at each audit boundary. The `ps` counter has 0.01-second
precision; timestamps include polling overhead. This measures the app while idle,
not a guarantee that the shared machine is idle.

A follow-up run used 600 frames per phase, one switch cycle, niceness 10 and
verified swap interval 1. No build or other test of ours ran concurrently.

| Phase | Sample interval | App CPU seconds | App CPU | Frame median / p95 |
|---|---:|---:|---:|---:|
| Buttons idle | 4.344 s | 3.010 s | 69.3% | 8.624 / 10.956 ms |
| Chart Lab active | 4.166 s | 2.240 s | 53.8% | 8.833 / 11.489 ms |
| Returned Buttons | 4.099 s | 2.470 s | 60.3% | 8.151 / 11.254 ms |

100% means one core. Returned-idle RSS was 371.9 MiB at the start and 371.8 MiB
at the end. This small interval cannot establish absence of leaks. These results
show substantial ongoing work in the idle Buttons screen, but do not attribute
that cost to a particular library system or establish a regression. Raw samples
and phase summaries are in `output/wm-remaining/phase-cpu/report.json`.
Use the windowed command above with `--frames 600` to repeat this measurement.
