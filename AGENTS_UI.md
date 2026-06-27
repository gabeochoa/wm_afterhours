# Agent UI Development Guide

Headless CLI + E2E only. **Do not use MCP** (`--mcp`, `layout_test.py`, `screenshot_all_screens.py`, etc.).

## Quick commands

```bash
make                          # build ui_tester
make test                     # all E2E scripts, headless + quiet
make test-visible             # E2E with visible window (debug)
make run-all-tests            # FontConfig coroutine tests only (UI → E2E)
make test-layout              # Catch2 autolayout (zero GPU)
make validate-screenshots     # PNG regression vs baselines
make ci                       # build + validate + coroutine + E2E
```

Single script:

```bash
./output/ui_tester.exe --test-script tests/e2e_scripts/101_layout_patterns.e2e --headless --quiet
```

Layout bounds (named elements only, not full tree):

```bash
./output/ui_tester.exe --layout-summary layout_patterns
```

## Token budget (best → worst)

1. **Exit code** — `make test` pass/fail
2. **`assert_ui` failure line** — primary debug signal (~1 line)
3. **Catch2** — layout math, zero GPU (`vendor/afterhours/example/ui_layout/`)
4. **E2E summary** — `E2E passed 91/91` with `--quiet`
5. **`--layout-summary`** — one line per named element when asserts aren't enough
6. **`validate-screenshots`** — pixel diff count, not full image
7. **Screenshot PNG** — last resort

**Never** use `dump_ui` or full UI trees in routine agent loops.

## E2E script rules

- Use `assert_ui` / `assert_ui_text` for every layout claim
- Use `goto_screen <name>` then `wait 0.5` before asserts
- Prefer `validate_screen` over raw `screenshot` when a baseline exists
- `dump_ui` only in dedicated regression scripts (e.g. `100_dump_simple_button.e2e`)

## Adding a new screen or E2E test

1. Add screen under `src/systems/screens/` with stable `with_debug_name(...)` on elements you will assert
2. **Build:** `make`
3. **Visual check (required for new tests):** capture a render and inspect it before committing

```bash
# One-off visual capture
printf 'goto_screen my_screen\nwait 0.5\nscreenshot my_screen_review\n' > /tmp/review.e2e
./output/ui_tester.exe --test-script /tmp/review.e2e --headless
open /tmp/e2e_screenshot_my_screen_review.png   # macOS
```

Or headless batch for all screens:

```bash
make screenshots   # output/{screen}_720p.png
```

4. Write `.e2e` script with `assert_ui` one-liners; add `screenshot` line for review artifacts in `/tmp/`
5. Run: `./output/ui_tester.exe --test-script tests/e2e_scripts/NN_my_test.e2e --headless --quiet`
6. If visual baselines are needed: `make update-baselines` then use `validate_screen` in the script

## Mac notes

- Default to headless (`make test`) — avoids WindowServer RAM from many windows
- Use `make test-visible` only when debugging interactively
- Single-process batch: one `ui_tester` run per `make test`, not one window per script

## Animation

Use `--time-scale` for faster E2E waits. Capture one screenshot per keyframe; do not dump per-frame JSON.
