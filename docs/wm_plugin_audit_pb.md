# WM pluginization audit — batch P-B (dispositions)

Branch `wm/plugin-audit-dispositions`. Evidence-first; each disposition is
no-change unless the tiny safe fix is listed. Nothing in `vendor/`,
`todo.md`, `docs/AFTERHOURS_GAPS.md`, or `CORRECT.md` was edited.

## 1. LetterboxLayout duplicate audit — KEEP LOCAL + tiny guard fix

- Local: `src/systems/LetterboxLayout.h::compute_letterbox_layout(w, h,
  content_w, content_h)`, single caller `RenderRenderTexture` (draws
  `layout.dst` for the main render texture).
- Upstream: `window_manager::content_viewport` / `content_to_window` /
  `window_to_content` + `graphics::letterbox_content_size` +
  `window_centre_position` exist in vendored afterhours, and they guard
  `<= 0` sizes. But they letterbox against a fixed singleton resolution
  (`ProvidesCurrentResolution::current_resolution`), not caller-supplied
  render-texture dimensions, so they are not a drop-in for this helper.
  Disposition: keep local; any future migration is upstream-API-first, not
  in this lane (no broad caller migration was performed).
- Tiny fix (done): return an empty layout when any dimension is `<= 0`.
  Fixes the recorded 0x0 minimized-window defect (todo.md:444: division by
  content width/height produced inf/NaN rects; casting inf to int is UB).
  Valid-input behavior is unchanged; the empty `dst` (0x0 rect) is a no-op
  draw in `RenderRenderTexture`. Regression test: `tests/letterbox_test.cpp`
  (valid same-aspect / pillarbox / letterbox + all zero/negative
  combinations: empty layout, finite rects).

## 2. Runtime log-level override shim — KEEP AS LOCAL SHIM (comment only)

- Local: `src/log/log_runtime.h` (`g_runtime_log_level`,
  `effective_log_level`, `set/reset_runtime_log_level`) with real callers in
  `src/game.cpp` (`--quiet` E2E mode sets `LOG_NOTHING`, resets, then
  `LOG_WARN` on failure).
- Vendored afterhours logging (`vendor/afterhours/src/logging.h`) has no
  runtime level override at all (compile-time `AFTER_HOURS_LOG_LEVEL` only).
  Removal was not attempted: the `game.cpp` quiet-mode path depends on the
  shim, and no parity test exists.
- Exact unique semantics (documented in the header): `LOG_ALOG_` is the
  "use compile-time default" sentinel; `effective_log_level()` returns the
  runtime level only when it is at least `LOG_TRACE`, else the compile-time
  default. Gating is split: `AFTER_HOURS_LOG_AT` early-outs at compile time
  while `vlog()` re-checks at runtime; `log_once_per_impl` bypasses the
  runtime level entirely. Only a behavior-free documenting comment was added.

## 3. Scheduled synthetic input injector — KEEP AS LOCAL WM TESTING PLUGIN

- Local: `src/engine/input_injector.*` (`schedule_mouse_click_at(Rectangle)`,
  `inject/release_scheduled_click`, key hold/press helpers,
  `consume_synthetic_press`, int-button mouse API with raylib `*_Real`
  fallback). Callers: `src/testing/test_input.cpp`, `src/game.cpp`.
- Upstream: `afterhours::testing::input_injector` / `test_input` in vendored
  `e2e_testing/` covers more (gamepads, wheel, pinch, `simulate_click(x,y)`)
  but differs in spelling and consume semantics (rect API vs `(x,y,w,h)`;
  int-button/`vec2` vs `Position`/no-arg; decrementing `consume_synthetic_press`
  vs readable-until-reset `consume_press`). Parity is not proven by existing
  tests, so nothing was retired or migrated.
- Action (API/behavior preserved): header comment naming the plugin + local
  plugin README/manifest in `src/testing/` (`input_injector_plugin.md`,
  `input_injector_manifest.json`).

## 4. Settings foreign-file refusal — KEEP AS APP POLICY (no code change)

- `src/settings.cpp::settingsPlaces()` + `load_save_file()` implement the
  policy: `settings.json` in the launch directory / executable directory /
  save path are candidates only; a file with a `project` marker for another
  project is skipped (never parsed), unmarked files must parse the full WM
  schema, and a refused foreign/corrupt file is never overwritten by a later
  save (`save_blocked` / first non-existent candidate). This is safety-sensitive
  startup code on main; no refactor was made.
- Evidence: `tests/settings_save_test.cpp` already covers the refusal
  end-to-end (foreign `{"commit_log_ratio":...}` file refused and preserved;
  wrong-project `"cartographer"` marker refused even with a WM-shaped body;
  legacy unmarked WM file loads). No new test or helper was needed; the
  policy stays as app policy.

## 5. Frozen ui_workarounds — UNTOUCHED

- `src/ui_workarounds/GradientBackground.h` and `NotificationBadge.h` were
  not edited. Verified zero callers: no `#include` and no symbol use of
  `GradientBackground`/`NotificationBadge` anywhere in `src/` or `tests/`
  (only the files' own namespace definitions match). They remain frozen
  pending upstream primitives.

## 6. Audit note

- This file (`docs/wm_plugin_audit_pb.md`). `todo.md` and
  `docs/AFTERHOURS_GAPS.md` were not edited per lane instructions.
