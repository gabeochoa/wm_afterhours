# Screen command pack (WM-local E2E navigation)

WM-local testing plugin (Batch 4 Lane L). Not afterhours upstream.

Named home for the WM app-level E2E navigation command pack that
previously lived loose in `src/testing/e2e_commands.h` +
`src/testing/e2e_integration.h`. Behavior-preserving canonical move:
every command spelling, arg, and handler semantic is unchanged, and
the old headers are now shims (see "Layout" below). `src/game.cpp`
is untouched.

## Layout

- `e2e_commands.h` — canonical, namespace `e2e_commands`. The six
  app-screen handlers + `register_app_commands<ScreenManager>()`.
- `e2e_integration.h` — canonical, namespace `e2e`. `E2EConfig`,
  `register_e2e_systems()`, `configure_runner()`, and the re-exports
  of upstream arg parsing (`should_run_e2e`, `parse_e2e_args`,
  `count_e2e_scripts`, `screenshot_path`).
- `src/testing/e2e_commands.h` and `src/testing/e2e_integration.h`
  are compatibility shims that include these canonical headers, so
  `src/game.cpp`, `src/main.cpp`, and `src/devtools/wm_devtools.h`
  compile unmodified. No `game.cpp` change was needed.

## Commands (WM pack, canonical)

| Command | Args | Handler | Registered by |
|---|---|---|---|
| `open_screen` | `<name>` | `HandleOpenScreenCommand<ScreenManager>` (needs `screen_manager->open_screen(name)`) | **Nowhere** — dead today, kept for parity; no script uses it |
| `goto_screen` | `<name>` | `HandleGotoScreenCommand` (callback `goto_screen_fn` set by game.cpp) | `game.cpp` by hand |
| `next_screen` | — | `HandleNextScreenCommand` (pushes `PAGEDOWN`) | `game.cpp` by hand + `register_app_commands` |
| `prev_screen` | — | `HandlePrevScreenCommand` (pushes `PAGEUP`) | `game.cpp` by hand + `register_app_commands` |
| `validate_screen` | `<name>` | `HandleValidateScreenCommand` (callback into `screenshot_validation::validate_screen_against_baseline`, 1% threshold) | `game.cpp` by hand |
| `resize` | `<w> <h>` | `HandleResizeWithSettingsCommand` — **non-consuming** pre-hook that also syncs `Settings::update_resolution()`, then lets the vendor handler do the ECS update | `register_app_commands` |

`register_app_commands` registers the resize hook + next/prev only;
`open_screen` / `goto_screen` / `validate_screen` are wired by hand
in `game.cpp`. `register_e2e_systems` (canonical
`e2e_integration.h`) is likewise not called by `game.cpp`, which
hand-registers the same AH builtins + UI pack + screenshot/reset +
unknown/cleanup plus its own `focus_element` handler. Both paths
stay as-is; this move changes no registration.

## AH vs WM diff (`vendor/afterhours/src/plugins/e2e_testing/screen_commands.h`)

AH's `screen_commands.h` provides exactly one command:
`expect_screen_luma <min> <max>` (mean-luminance assertion via a
pluggable `LumaProvider`, retried via `retry_with`), plus
`register_screen_commands()`. Checked command-name by command-name
against WM's pack:

| WM command | In `screen_commands.h`? | Where AH covers it (if anywhere) |
|---|---|---|
| `open_screen` | No | Nowhere in the e2e_testing plugin (no screen registry exists upstream) |
| `goto_screen` | No | Nowhere upstream |
| `next_screen` | No | Nowhere upstream (AH `key PAGEDOWN` is manual) |
| `prev_screen` | No | Nowhere upstream (AH `key PAGEUP` is manual) |
| `validate_screen` | No | Partial: `screenshot_baseline.h` has `diff_images` / expect-screenshot plumbing (default 5% mismatch, channel tol 8) but no `validate_screen` command, and its threshold/drift semantics differ from WM's 1% `validate_screen_against_baseline` (keep-baseline-on-pass + `--update-baselines` refresh of failures only). Not a drop-in. |
| `resize` | No (`resize`/`resize_steps` live in `command_handlers.h`, registered as builtins) | WM does **not** duplicate the handler: `HandleResizeWithSettingsCommand` never consumes the command — it syncs WM `Settings` and returns so the vendor `HandleResizeCommand` performs the ECS/window update. Complementary glue, not a duplicate. |
| `expect_screen_luma` | n/a (AH-only) | WM does not use it; no binary or script depends on it. Not part of this pack. |

Registry / coupling diff (`e2e_integration.h` vs AH): AH has no file
equivalent to WM's `e2e_integration.h`. Upstream wiring lives in
`e2e_testing.h` (`register_builtin_handlers`, unknown-handler,
cleanup, shared-handler pack) plus `app.h` (`E2EApp::create`), which
registers builtins + screenshot/reset callbacks. WM's
`e2e::register_e2e_systems` is the WM analogue (builtins + UI pack
with `dump_callback` + screenshot/reset callbacks + app commands +
unknown/cleanup) and re-exports AH arg parsing, which `src/main.cpp`
consumes via `e2e::should_run_e2e` / `e2e::parse_e2e_args`.

## Duplicates vs WM-only glue

- **Duplicates of AH commands: none.** Zero command-name overlap with
  `screen_commands.h`, and the one name that touches an AH builtin
  (`resize`) is a non-consuming WM-Settings sync hook, so no handler
  can be removed in favour of delegation.
- **WM-only glue, all of it:** the whole pack. `goto_screen` depends
  on the WM screen-name→index table + `load_screen()` callback;
  `next_screen`/`prev_screen` encode the WM showcase PageUp/PageDown
  convention; `validate_screen` couples to WM
  `screenshot_validation` + `baseline_screenshots/`; the resize hook
  couples to WM `Settings`; the integration header wires WM
  callbacks (screenshot/reset/dump) into AH's runner. Delegation to
  AH types was therefore **not** done — canonical move only.

## The precise AH gap candidate (no upstream edit)

If AH ever grows a `screen_commands.h` navigation pack, the minimal
gap is a generic `register_screen_commands(sm, resolver)` where
`resolver` maps a screen name to a bool (the pattern AH already
uses for `LumaProvider` and `state_commands::register_state`), with
`next_screen`/`prev_screen` parameterized by the page-down/up keys
instead of hardcoding `PAGE_DOWN`/`PAGE_UP`. `validate_screen`
should only be promoted with the pass threshold and the
keep-baseline-on-pass / failure-only refresh policy beside it —
promoting AH's 5% default as-is would silently loosen WM visual
regression. Until that exists, this pack stays WM-local.

## Verification

- `make -j4` with `PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig` —
  clean.
- `./output/ui_tester --run-all-tests` — pass.
- `./output/ui_tester --test-script tests/e2e_scripts/05_custom_commands.e2e
  --headless --quiet --time-scale 4` (the only script using
  next/prev; also goto_screen) — pass.
- `01_basic_navigation` (goto_screen), `02_button_click`
  (goto_screen + screenshot), `08_checkbox_toggle` (validate_screen
  + resize 1280x720) — pass.
- `09_radio_select` (validate_screen + resize) — **fails identically
  on clean `origin/main`** (verified by stashing this change,
  rebuilding, and rerunning): a pre-existing failure, unchanged by
  this move.
- Zero baseline changes (`baseline_screenshots/` untouched).
