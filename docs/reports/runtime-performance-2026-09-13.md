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

## Change and remaining verification

Both startup paths previously rasterized all registered CJK fonts before choosing a screen. The WM font definitions now declare their consuming screens, and the screen registry loads each deferred font once, immediately before constructing its first consumer. Existing Korean and Japanese full repertoires remain available to their consumers. The Language demo's new bold Japanese face uses 133 codepoints covering its fixed copy and ASCII.

Consumer mapping covers Language, Forms, Sync Scroll, Mini Motorways settings and Localized Input Prompts. Font loading is shared by windowed and headless initialization. Timing output names every loaded font and its glyph count.

After compiling, repeat the command against the changed binary and run all five consumer screens, including repeat visits. Confirm that first-screen startup contains no CJK atlas loads, each required font loads once on demand, and Japanese text renders without missing glyphs. Controlled cold starts, windowed frame measurements and retained GPU texture accounting remain separate checks.
