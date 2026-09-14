# Performance evidence

September 13, 2026, shared Apple Silicon laptop, niceness 10. These are historical
samples, not guarantees. Other users, filesystem caches and memory compression were
uncontrolled. No build overlapped the final timing runs. Cold launch and an otherwise
idle machine remain unmeasured.

## Lazy fonts

CJK fonts load before their first consuming screen and stay resident. A repeat-visit
check loaded NotoSansKR, NotoSansJP, Sazanami and NotoSansKRInline once each. The fixed
Japanese specimen uses 133 codepoints; other consumers retain their repertoires.

| Same headless workload | Before | After |
|---|---:|---:|
| First observed ready upper bound | 4.797s | 1.123s |
| Warm ready bounds | 2.973 / 3.798s | 1.098 / 0.922s |
| Peak sampled RSS | 816.2MiB | 360.1MiB |
| Aggregate frame average | 7.18ms | 7.80ms |

This supports reduced startup work, not faster steady-state frames. Ready markers
include two frames plus audit/polling overhead; aggregate timing includes screenshots
and navigation. Raw reports: `output/wm-only-todos/runtime-matched-before/report.json`
and `runtime-final/report.json`. Short RSS samples cannot establish absence of leaks.

## Atlases and windowed frames

Cafe and Images each went from five textures to one padded, edge-extruded atlas.
Cafe's eight sampled artwork regions matched exactly. Images matched 14/15; one
scaled icon changed one red-channel level in one pixel. Storage grew by 16,080 and
4,080 pixels. Fewer textures did not reduce driver submissions:

| Screen | Before and after draws / binds per frame |
|---|---:|
| Cozy Cafe | 66 / 68 |
| Images | 59 / 61 |

Two repeats agreed. Frame medians ranged 8.187–8.864ms without an attributable gain.
The optional `mac_gl_probe.c` counts GL draw/bind calls and verifies swap interval 1
on every present. Bind counts include redundant binds and unbinds. Present timing
includes scheduling; it is not GPU duration. Preserve painter/clip order before
attempting batching changes.

## Process CPU

600-frame settled phases with verified vsync, excluding 0.5s around audit boundaries.
CPU uses differences in the app's cumulative `ps time`, excluding sampler CPU;
100% is one core. The counter has 0.01s precision.

| Phase | App CPU | Frame median / p95 |
|---|---:|---:|
| Buttons idle | 69.3% | 8.624 / 10.956ms |
| Chart Lab active | 53.8% | 8.833 / 11.489ms |
| Returned Buttons | 60.3% | 8.151 / 11.254ms |

Returned RSS was 371.9→371.8MiB over about four seconds. This identifies ongoing idle
work but does not attribute it to a library system. Raw data is in
`output/wm-remaining/phase-cpu/report.json`.

## Virtual-list clipping

Coalescing adjacent equal clip scopes reduced headless idle averages from 14.84→8.22ms
with the profiler hidden and 24.76→11.47ms shown. The list already built only 26–30 rows.
The initial jump baseline clicked the wrong control and was rejected. Shown-idle
before/after started at different rows, so this is not controlled profiler overhead.
Repeat with `scripts/verify_review_todos.py --benchmark-only`.

## Repeat a measurement

```sh
nice -n 10 clang -dynamiclib -Wno-deprecated-declarations scripts/mac_gl_probe.c -framework OpenGL -o output/mac_gl_probe.dylib
nice -n 10 python3 scripts/profile_runtime.py --binary output/ui_tester.exe --windowed --gl-probe output/mac_gl_probe.dylib --frames 600 --switch-cycles 3 --output output/windowed-measurement
nice -n 10 python3 scripts/measure_atlas.py --before /absolute/path/to/before/ui_tester.exe --after output/ui_tester.exe --probe output/mac_gl_probe.dylib --output output/atlas-measurement
```

The scripts isolate settings/resources and lower child priority. The probe must reach
the app directly; protected launcher utilities can strip DYLD variables. Both clocks
use CLOCK_MONOTONIC. Missing/invalid probe data fails the measurement. Without
`--windowed`, the uncapped headless workload is unsuitable for idle CPU comparisons.
See library [profiling measurements](../vendor/afterhours/docs/profiling-measurements.md)
and [atlas measurement](../vendor/afterhours/docs/font-atlas-measurement.md).
