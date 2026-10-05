# CORRECT — wm_afterhours
Run: `python3 scripts/check_correct.py`.

## Classes (happened 2+)
| # | Class | Evidence (2+) | Level + why |
|---|---|---|---|
| C1 | Consumer-side workaround instead of upstream primitive | `src/ui_workarounds/{GradientBackground,NotificationBadge}.h` still resident; removals d6d6911 `Transitions: use chevron primitive`, ec3cc7b `Drop marquee quantization workaround, bump afterhours`, ee28e5d transitions lessons | Lint freeze (allowlist) — architecture fix is upstream in afterhours, outside this repo; freezing the folder forces that conversation instead of a 3rd workaround copy |
| C2 | New Lab/screen registered but never visited by e2e (silent no-coverage) | Pattern commits 38a1403/8e3cf3b/3123e42/760cecd each `add E2E 36x` with the Lab; at HEAD 3 registered screens had no `goto_screen` anywhere: small_components_lab, focus_ring_lab, validation_throttle_lab | Behavior test + lint — added the 3 to `tests/e2e_scripts/99_check_all_screens.e2e`; checker derives flags from `REGISTER_EXAMPLE_SCREEN`, no hand list to drift |

## Commits (local only, one per class)
- 3da1ea23 C1: checker freezes `src/ui_workarounds/` to the 2 grandfathered files
- 457f0623 C2: checker C2 + add 3 missing screens to 99_check_all_screens.e2e

## Proof
Checker with a fixture `src/ui_workarounds/NewHack.h` + HEAD 99 script: 1×C1 + 3×C2 failures. On HEAD: OK. (E2E scripts not executed here — full `make`/e2e run is the repo's heavy suite; the added lines follow 99's exact `goto_screen`/`wait` pattern.)

## Rule table
| Rule | Do | Never |
|---|---|---|
| C1 | Fix/extend the primitive in afterhours, bump, delete workaround | Add a file to `src/ui_workarounds/` |
| C2 | Same commit as a screen: `goto_screen <flag>` in an `.e2e` (99 minimum) | Ship a `REGISTER_EXAMPLE_SCREEN` with no e2e visit |
