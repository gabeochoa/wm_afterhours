# RollingNumber local plugin

WM-local plugin (Batch P-A). Not afterhours upstream.

## Purpose

Six-digit rolling odometer: each digit rolls on an ease-out-cubic motion
timeline up or down to its target digit, preserving per-digit roll distance
(multiple of 10 targets roll a full turn). Reduced-motion aware: with
`reduced_motion=true`, `retarget`/`advance` settle immediately.

## Public API

Canonical header: `src/plugins/rolling_number/rolling_number.h` —
`include "plugins/rolling_number/rolling_number.h"`

- `rolling_number::Number` — six `afterhours::motion::Track<float>` digits,
  `position`/`from`/`to` arrays, `target`, `elapsed`.
- `rolling_number::retarget(Number&, int, bool reduced_motion=false)` —
  clamp to `[0, maximum=999999]`, start the roll (or settle if reduced).
- `rolling_number::advance(Number&, float dt, bool reduced_motion=false)` —
  step the tracks (negative `dt` clamped to 0).
- `rolling_number::settle(Number&)`, `rolling_number::sync(Number&)`,
  `rolling_number::wrap(double)`.
- `rolling_number::duration` (0.65s), `rolling_number::maximum` (999999).
- Plugin facade: `rolling_number::plugin::descriptor` (name, version,
  digit_count, duration, max, reduced-motion flag) plus the same operations
  re-exported under `rolling_number::plugin::`.

Compatibility: `src/rolling_number.h` is a shim including this header;
existing `#include "../../rolling_number.h"` call sites are unchanged.

## Current consumers

- `src/systems/screens/RollingNumberLab.h` (`rolling_number` screen) —
  digits rendering, `+1`/`-1`/`+25`/`Set 99`/`Set 999`/`Reset`, pause, blur smear.
- `tests/rolling_number_test.cpp` — behavior suite (unchanged, via the shim).
- `tests/rolling_number_plugin_test.cpp` — descriptor + facade hardening.

## Tests

```bash
clang++ -std=c++23 -I vendor -Isrc $(pkg-config --cflags raylib) \
  tests/rolling_number_test.cpp tests/rolling_number_plugin_test.cpp
```

Targeted e2e: `297_rolling_number.e2e`.

## Afterhours promotion gate (future candidate only)

No afterhours changes in this batch. Promotion candidate only after this
plugin holds stable across the full local suite, targeted e2e
(RollingNumberLab), and headless screenshot baselines. Not claimed
upstream-ready; any promotion is a separate proposal with its own review.
