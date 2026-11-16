# afterhours gaps — mailboy

## 2026-10-19 — ROOT-CAUSED: zig ReleaseSafe failure was an LLVM 21 miscompile of an `inline` constant table, not app UB (mailboy)
- Symptom: `zig build -Doptimize=ReleaseSafe` deterministically failed
  tests/upgrades.e2e (expect_day_duration 780 got 540) + tests/modifiers.e2e
  (first: expect_stamps 127 got 115), while make clang -O0/-O1/-O2 (Apple
  clang 17, system libc++) and zig Debug passed both. Every modifier
  effect read as the identity *except* Rush Hour, whose offer text / BPM /
  stamps kept working.
- Bisect (each variant: `zig build` at ReleaseSafe unless noted, then only
  tests/upgrades.e2e):
  - Baseline ReleaseSafe: FAIL. Baseline Debug: PASS.
  - `-fno-strict-aliasing`: FAIL (not aliasing).
  - `-fwrapv` + `-fno-strict-overflow`: FAIL (not signed overflow).
  - UBSan print mode (`-fsanitize=object-size`, no trap): FAIL, no
    diagnostic — the optimizer folds the check away.
  - Manual `zig c++ -O2` build (same compiler, same bundled libc++): PASS.
    Diffing the `zig build` clang line vs the manual one left only
    zig-added flags: `-fsanitize=undefined -fsanitize-trap=...`,
    `-D_FORTIFY_SOURCE=2`, `-fstack-protector-strong`, `NDEBUG` (manual
    only). Adding just the UBSan flags to the manual build flipped it to
    FAIL.
  - UBSan check groups: `object-size` FAIL, `pointer-overflow` FAIL, and
    `return` / `returns-nonnull-attribute` / `shift-base` / every other
    group PASS. Both `object-size` and `pointer-overflow` singles FAIL.
  - Minimal repro (60 lines, no game code): an `inline constexpr` table of
    `{enum id, string_view name, ...}` rows, searched by name then by id,
    compiled with `zig c++ -O2 -fsanitize=object-size
    -fsanitize-trap=object-size`, returns the TABLE[1] (Rush Hour) row for
    every id != 0/1. `static constexpr` / bare `constexpr` / `extern`
    tables read correctly; Apple clang 17 + UBSan reads the `inline`
    table correctly. So the trigger is vague linkage (`inline`,
    `linkonce_odr` in the IR) + LLVM 21 + UBSan object-size/pointer-overflow
    at -O2. No application UB was found: no UBSan diagnostic fires in any
    mode, and the same code is correct at -O0 / under Apple clang.
- Fix (code, not flags): `src/components.h` — `DAY_MODIFIER_TABLE` changed
  from `inline constexpr` to plain `constexpr` (internal linkage, one copy
  per TU; no code relies on the table having one address across TUs — all
  uses are local reads). Only this table has the search-by-name-then-by-id
  pattern; the other header tables are indexed directly. The flag was NOT
  kept as a workaround: no `-fno-...` was added. `build.zig` now defaults
  to ReleaseSafe (`b.option("optimize") orelse .ReleaseSafe`, so every
  `-Doptimize` value still works; `preferred_optimize_mode` was deliberately
  not used — it ignores `-Doptimize` when no `--release` flag is given).
- Verification: `zig build test` (default ReleaseSafe) 52 PASS / 0 FAIL;
  `zig build -Doptimize=Debug test` 52 PASS / 0 FAIL;
  `zig build -Doptimize=ReleaseFast` + upgrades.e2e PASS;
  `make test-headless` (make -O0, Homebrew pkg-config first on PATH)
  52 PASS / 0 FAIL + RENDER_TEST PASS. Existing tests/upgrades.e2e +
  tests/modifiers.e2e are the regression coverage for this (they failed
  under the bug, pass after the fix); no new test file was added.
- Note the earlier "latent UB" hypothesis in this file (bottom entry) was
  wrong: the code had no UB. The failure was a toolchain miscompile.

## 2026-10-18 — Zig 0.16.0 build (`build.zig`): afterhours compiles clean under zig's bundled libc++, but zig is NOT Apple clang + system libc++
- New `build.zig` (fresh file; no sibling repo has one) mirrors the
  Makefile: 15 `src/*.cpp` + `vendor/afterhours/src/plugins/files.cpp` +
  `vendor/afterhours/src/plugins/settings.cpp`, Makefile `FLAGS`/`NOFLAGS`
  verbatim, `-Ivendor/ -Isrc/`, `-framework OpenGL -framework
  CoreGraphics`, default build -> `zig-out/bin/mailboy`, `run` step
  (forwards args after `--`), `test` step (`--test --headless`, cwd = repo
  root, no timeout — wrap with `perl -e 'alarm 300; exec @ARGV'` if one is
  wanted). No `build.zig.zon` needed (verified: zig 0.16 builds without
  one). No `vendor/` edits, Makefile untouched and still working, nothing
  committed.
- No PCH in the zig build: the Makefile precompiles `src/pch.h` and injects
  it with `-include-pch`; no source `#include`s `pch.h`, they all rely on
  injection. Zig compiles with `-include <abs>/src/pch.h` normally instead.
  Verified via `zig build --verbose-cc`: the `-include .../src/pch.h` flag
  is present in every TU compile line. No afterhours change needed for
  this; the PCH is a Makefile-only mechanism.
- Friction 1 — zig's C++ is a different toolchain, not Apple clang.
  `zig build --verbose-cc` shows each TU compiled by zig's bundled clang
  with `-nostdinc -nostdinc++`, `-isystem <zig>/lib/libcxx/include` +
  `libcxxabi/include`, `-target aarch64-apple-macosx27.0.1`, and the
  bundled libc++ linked statically (`otool -L zig-out/bin/mailboy` shows no
  `/usr/lib/libc++.1.dylib`, unlike the make binary). Afterhours + mailboy
  compile and pass all tests under this libc++ with zero source changes,
  but it is a real toolchain divergence: ABI/behavior differences between
  zig's libc++ and Apple's libc++ would surface here first, and the zig
  binary is 33M vs the make binary's 6.4M (static libc++ + dwarf-4 debug
  info vs dynamic system libc++).
- Friction 2 — zig Debug adds UBSan to C++ TUs the Makefile never has.
  Verbose compile lines show `-fsanitize=undefined
  -fno-sanitize=vptr,function`, plus `-fstack-protector-strong` and
  `-fno-omit-frame-pointer`, on top of the shared `-O0`. Measured effect on
  the offline render (`--render-song /tmp/*.wav --song=2`): zig binary
  3.405s real vs make binary 1.015s real (~3.4x), with byte-identical WAV
  output (`cmp` clean) and identical render summaries (peak 0.891263,
  87.4593s duration). The UBSan instrumentation is the likely cause; it is
  default zig Debug behavior, not an afterhours problem.
- Friction 3 — zig 0.16 DOES support pkg-config
  (`linkSystemLibrary("raylib", .{})` defaults to `use_pkg_config = .yes`,
  via `runPkgConfig` in `std/Build/Step/Compile.zig`), but it resolves
  bare `pkg-config` from PATH and on any failure silently falls back to
  `-lraylib` by name — no error, no hint about which pkg-config ran. In
  this shell PATH puts MacPorts `/opt/local/bin/pkg-config` first, whose
  `pc_path` (`/opt/local/lib/pkgconfig:...`) has no raylib, so the
  pkg-config path misses and the fallback must save the build. `build.zig`
  therefore also adds the Homebrew Cellar dirs
  (`/Users/gabeochoa/homebrew/Cellar/raylib/5.5/include|lib`, guarded by an
  on-disk existence check) as `-isystem`/`-L`; the verbose log shows the
  Cellar include on all TU lines in this shell. With Homebrew's
  `pkg-config` first on PATH, pkg-config supplies the same dirs instead.
  Assumption documented in README "Building with Zig".
- Friction 4 (zig 0.16 stdlib API churn, not afterhours): `std.fs`
  filesystem helpers are gone in 0.16 — `std.fs.openFileAbsolute` does not
  exist. The existence check in `build.zig` uses
  `std.Io.Dir.accessAbsolute(b.graph.io, path, .{})` instead.
- Verification (2026-10-18): clean `rm -rf zig-out .zig-cache && zig build`
  green with 0 warnings on stderr. `zig build test` green: 52 `[PASS]`, 0
  `[FAIL]`, `E2E tests PASSED`. `make test-headless` (same tree) also
  green: 52 `[PASS]`, 0 `[FAIL]`, `E2E tests PASSED`, plus `RENDER_TEST
  PASS` lines — the zig `test` step deliberately runs only
  `--test --headless`, not `--render-test`. Note the brief's "51 files" was
  off by one: `tests/` holds 52 `.e2e` files and the runner reports 52
  passes under both builds. `zig build run -- --render-song ... --song=2`
  verified: forwards args, WAV byte-identical to make's.
- `zig build test` inherits the pre-existing runner behavior from the
  2026-10-16 screenshot entry: no self-timeout. The zig step adds none by
  design (per spec); use the perl alarm wrapper externally.

## 2026-10-18 — set_path_override is NOT missing at d1d7f0e: no upstream request, no app-side reimplementation
- Claim checked: "`src/main.cpp:274/281` call
  `afterhours::settings::set_path_override`, but that API only exists in a
  locally-updated `settings.h`; the submodule pointer `d1d7f0e` does not
  contain it, so fresh clones fail." The claim is FALSE. `git show
  d1d7f0e:src/plugins/settings.h` in `vendor/afterhours` contains it at
  line 107 (`static void set_path_override(const fs::path &path)`), with
  `path_override_`, `clear_path_override()`, `root_path()`, and
  `get_save_path()` alongside. It was added upstream in `a0dd57d`
  (2026-09-27, "Settings: code-overridable root path"), before the pinned
  commit. The submodule working tree was clean at `d1d7f0e` throughout.
- Consequence: the app-side reimplementation (options a/b/c) was NOT done,
  and neither `set_path_override` call site was deleted. Deleting working
  plugin calls to reimplement the same redirect app-side would be pure
  churn with real regression risk; the plugin API is the correct layer.
  Real-run settings behavior is unchanged (nothing in `src/` changed).
  There is NO upstream API request to file for this: the API already
  exists upstream. Do not re-file it.
- Fresh-clone proof (pristine clone, no local changes): `git clone
  https://github.com/gabeochoa/mailboy /tmp/mb-check`, then `git -c
  url.https://github.com/.insteadOf=git@github.com: submodule update --init
  --recursive` (see gotcha below), then `make -j4` + `make
  test-headless` passed: 49 e2e `[PASS]`, 0 `[FAIL]`, including
  `[PASS] meta_loop` (the persistence round-trip), plus `RENDER_TEST
  PASS`. Test-mode settings went to
  `/tmp/mailboy_e2e_settings/mailboy_settings.json`, not the real save.
- Fresh-clone gotcha (not a code bug): `git clone --recursive -c
  url.https://github.com/.insteadOf=git@github.com: ...` does NOT
  propagate the `-c` rewrite to the submodule clone — it still tried
  `ssh: connect to host github.com port 22: Operation not permitted` and
  aborted. The two-step form above works. The submodule URL in
  `.gitmodules` is still `git@github.com:gabeochoa/afterhours.git`; anyone
  cloning without SSH needs the two-step form or a global
  `url.https://github.com/.insteadOf` config.
- json note (mailboy-vendored, not afterhours): `vendor/nlohmann/json.hpp`
  bumped 3.11.2 -> 3.11.3 (see below). This supersedes the 2026-10-18 fmt
  entry's "left alone" note about `operator"" _json` warnings: 3.11.3
  adds the no-space `operator""_json` form, so the warnings are gone
  without any scoped `-Wno-deprecated-literal-operator`.

## 2026-10-18 — nlohmann json.hpp 3.11.2 -> 3.11.3 (mailboy-vendored)
- `/tmp/json.hpp` verified as 3.11.3: `NLOHMANN_JSON_VERSION_PATCH 3`
  (`vendor/nlohmann/json.hpp:70` after replacement). Copied byte-identical
  over `vendor/nlohmann/json.hpp` (`cmp` clean). No scoped `-Wno-...` or
  pragma wrapper was needed.
- Warning proof (Apple clang 17, `-Wdeprecated-literal-operator`, TU that
  only includes `<nlohmann/json.hpp>`): 3.11.2 emits 4 warnings per TU
  (`operator "" _json` / `operator "" _json_pointer` + the two `using`
  declarations); 3.11.3 emits 0. The default `make` build does not enable
  that warning, so the fresh-clone build on 3.11.2 showed no json warnings;
  the probe with the flag explicit is the discriminating check.
- Build/test proof on 3.11.3 in `~/p/mailboy`: clean rebuild `make -j4`
  succeeds (only pre-existing unrelated warnings: `-Wshadow` in
  afterhours `animation/track.h`, unused `sl0`/`sr0` in
  `src/render_systems.cpp`); `make test-headless` green: 49 e2e `[PASS]`,
  0 `[FAIL]`, `[PASS] meta_loop`, `RENDER_TEST PASS`. Uncommitted, like
  everything in this pass; the pristine fresh clone above still has 3.11.2
  and also passes.

## 2026-10-18 — fmt 8.0.1 -> 11.2.0 re-vendor: afterhours does NOT vendor fmt
- Afterhours itself vendors no copy of fmt: there is no `vendor/fmt` under
  `vendor/afterhours`. Its headers include `<fmt/...>` / `"fmt/format.h"`
  and rely on the HOST to supply fmt headers. Mailboy supplies them from
  `vendor/fmt/` via `-Ivendor/`; afterhours' own tests/examples Makefiles
  instead point at the Homebrew include dir ("fmt is header-only here; its
  headers live under the Homebrew include dir"). So afterhours is affected
  by the host's fmt version, not by any copy it controls.
- Afterhours' fmt usage is unaffected by the 8 -> 11 jump: `fmt::format`
  (~66 sites), plus `translation.h`'s `fmt::vformat` +
  `fmt::dynamic_format_arg_store<fmt::format_context>` + `fmt::arg`. All
  still exist in fmt 11.2.0. Verified compiling clean: full mailboy build
  on Apple clang 17, and `vendor/afterhours/src/plugins/settings.cpp`
  (which pulls in the fmt-using UI headers) on Homebrew clang 21.1.8.
  No afterhours edits were needed (and none were made).
- Latent gap (not fixed; do not edit `vendor/afterhours`): afterhours
  headers include fmt BEFORE any std header (`translation.h:3-4`
  includes `<fmt/args.h>` / `<fmt/format.h>` first; the UI headers use
  quoted `"fmt/format.h"`). fmt 11.2's `format.h` uses unqualified
  `::malloc` / `::free` without including `<cstdlib>`, so a TU that
  reaches fmt without a prior `<cstdlib>` / `<iostream>` fails on
  clang 21 (verified: `<string>`-only or `<cstdio>`-only first fails,
  `<cstdlib>`/`<iostream>`-first passes). Mailboy is safe because every
  TU loads the PCH first (`src/pch.h` includes `<iostream>` etc. before
  fmt). Afterhours standalone TUs that include `translation.h` first
  would break on newer toolchains. Upstream fix would be `<cstdlib>`
  before fmt in `translation.h` (or fmt upstream including `<cstdlib>`
  in `format.h`).
- Unrelated observation from the same clang 21 probe: `settings.cpp`
  emits deprecated-literal-operator warnings from `vendor/nlohmann/json.hpp`
  (`operator"" _json`), not from fmt. Pre-existing, different vendor lib,
  left alone.

## 2026-10-16 — submodule bump d537763 -> d1d7f0e (heads/main)
- **Breaking: host type macros.** New `src/developer.h` requires the host to define
  `ColorType` and `FontType` (and `Vector3Type`) macros before including afterhours,
  in addition to the previously-required `RectangleType`/`Vector2Type`/`TextureType`.
  Error was cryptic: `no type named 'col' in namespace 'afterhours::detail_rl_types'`.
  Fix was app-side (3 lines in `src/rl.h`), but afterhours should either provide
  raylib defaults itself or document the full required macro list in one place.
- Otherwise the bump was clean: no other API breakage found yet on this pass.
- **Breaking: `log_error` must be a single statement.** New `src/crash_handler.h`
  `report()` does `if (...) log_error(...) else log_error(...)`. Mailboy's
  `AFTER_HOURS_REPLACE_LOGGING` macro expanded to `if (...) log_me(...); assert(false)`,
  which mis-binds the `else` ("expected expression" at crash_handler.h:52).
  Host macros now need `do { ... } while (0)`. Undocumented; afterhours should
  document the statement-safety requirement for replacement logging macros.

## 2026-10-16 — screenshot.e2e hangs headless
- `mailboy.exe --test-file tests/screenshot.e2e` (no --test flag) ran 30+ min at
  high CPU without exiting (pid 60411, killed). The e2e runner needs a hard
  timeout wrapper (`perl -e 'alarm 300; exec @ARGV'`); single-test invocation
  without `--test` also does not complete as a single test (pre-existing arg
  handling). Runner should self-timeout and exit nonzero instead of spinning.

## Zig ReleaseSafe exposes a latent bug (2026-10-18, mailboy) — RESOLVED 2026-10-19, see top entry
`zig build -Doptimize=ReleaseSafe` binary (3.5M, render 0.47s vs make 1.0s,
WAV byte-identical) deterministically FAILS tests/upgrades.e2e:
expect_day_duration 780 got 540 — caffeine_level contributes 0 under
optimization (Debug zig + make clang -O0 both pass). Likely UB the optimizer
exploits (strict aliasing / signed overflow / uninit read in the upgrade
path). Zig default stays Debug until root-caused. Filed here because the
pattern (singleton state mutated via buy()) may live in shared helpers.
RESOLUTION: the "latent UB" hypothesis was wrong — no app UB was found.
Root cause was an LLVM 21 miscompile of the `inline` DAY_MODIFIER_TABLE
under zig's UBSan (see top entry); the fix was internal linkage for that
table, and `build.zig` now defaults to ReleaseSafe.

## Gamepad test injection missing (2026-10-19)
afterhours e2e test_input injects keys/mouse/wheel/pinch only; gamepad
buttons/axes bypass the injector (input_system reads raylib directly), so
pad input cannot be driven by the e2e runner. Mailboy works around it with
a game-side synthetic pad in src/game_input.h (same shim real hardware
reads through), but an upstream gamepad injector (queue_gamepad_press /
axis state in test_input) would let every game test pads for real.
