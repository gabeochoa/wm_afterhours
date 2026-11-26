# Settings policy local plugin

WM-local plugin (Batch 6 Lane S). Not afterhours upstream.

## Purpose

Pure classification layer for the settings.json foreign/corrupt-file
refusal policy, extracted behavior-preservingly from
`src/settings.cpp`. It owns only the decisions — the project marker
check, legacy unmarked-schema acceptance, and the refused-file
write-fallback rule — so the policy is named, testable, and reusable.
`src/settings.cpp` keeps all filesystem IO, candidate discovery,
parsing, and every log line's text and order. Refusal behavior is not
weakened and the settings.json format is unchanged.

## Public API

Canonical header (in this directory):

- `settings_file_policy.h` — `include "plugins/settings_policy/settings_file_policy.h"`
  - `kProjectMarker` — `"wm_afterhours"`, the marker `to_json` writes
    and `load_save_file` requires.
  - `Marker { Match, Foreign, Invalid, Absent }` —
    `classify_marker(json)`; the marker-only decision `settings.cpp`
    uses before attempting the schema parse.
  - `FileClass { AcceptMarked, AcceptLegacyUnmarked,
    RefuseForeignProject, RefuseInvalidProjectMarker,
    RefuseSchemaMismatch }` — `classify(json, schema_ok)`; the full
    decision. Marker refusal wins regardless of `schema_ok`,
    mirroring the load loop's check order. Unmarked legacy files are
    accepted only when the full schema parses; partial/corrupt
    unmarked files are `RefuseSchemaMismatch`.
  - `accepts_file(FileClass)` — true for exactly the two accept classes.
  - `FallbackTarget { has_target, index }` —
    `first_missing_candidate(candidate_count, exists_flags)` returns
    the first nonexistent app-owned candidate at index >= 1 (index 0,
    the launch-directory candidate, is never chosen, so a refused
    foreign/corrupt file is never overwritten);
    `blocks_save(saw_unusable, target)` is true only when something
    was refused and no fallback candidate exists.
  - Plugin facade: `plugin::descriptor` (`name`, `version`,
    `project_marker`).

Callers must include nothing beyond `<nlohmann/json.hpp>`-compatible
setup (settings.cpp already does). Header-only; no `.cpp` to link.

## Consumer

- `src/settings.cpp` — the only consumer. It keeps
  `settings_places()` candidate order (`WM_SETTINGS_PATH` override,
  launch dir, executable dir, parent dir, save path), every
  `log_warn` / `log_info` string, the
  `save_blocked` write guard, and the atomic write. Its
  `kSettingsProject` is now an alias for `kProjectMarker`.

## Tests

New standalone unit test `tests/settings_policy_test.cpp` covering
marked-match, marked-foreign, non-string project, unmarked
full-schema, unmarked partial/corrupt, schema-mismatch-on-marked,
and the fallback selection (first-missing, all-exist blocked,
single-candidate, no-refusal). There is no makefile target for
the standalone tests (mirrors `settings_save_test`); build from the
repo root:

```bash
clang++ -std=c++23 -I vendor -Isrc tests/settings_policy_test.cpp \
  -o /tmp/settings_policy_test && /tmp/settings_policy_test
```


```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
```

Previously existing behavior suites stay green:
`settings_save_test` 5/5 (via `tests/settings_save_test.cpp` +
`src/settings.cpp` + the vendored files TU), `settings_audio_test`
3/3.

## Afterhours promotion gate (future candidate only)

No afterhours/vendor edits in this lane. Promotion candidate only
after this plugin holds stable across the full local suite and the
settings save/audio suites; any promotion is a separate proposal with
its own review. Not claimed upstream-ready.
