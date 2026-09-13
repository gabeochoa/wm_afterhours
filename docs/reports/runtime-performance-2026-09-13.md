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
and `output/wm-only-todos/runtime-final/report.json`. Controlled cold starts,
windowed CPU/frame measurements and retained GPU texture accounting remain open.

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

This verifies appearance and resource consolidation. Driver texture-bind/draw-call
counts and attributable frame-time gains are not measured, and remain in `todo.md`.
