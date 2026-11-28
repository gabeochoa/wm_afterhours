# Collected afterhours gaps

Collected 2026-11-28 by copying every gap file found under `~/p/` verbatim
into this directory. No triage, dedup, or status update was done here; see
`STATUS.md`, `PRIORITIZED.md`, and `../AFTERHOURS_GAPS.md` for that. To
re-collect: copy each source over its file below and re-check the
exclusions. Files are byte-identical to their sources at collection time
(verified with `cmp`).

## Sources

| Collected file | Source under `~/p/` | Lines |
|---|---|---|
| `afterhours-todo.md` | `afterhours/todo.md` (library's own follow-ups) | 315 |
| `afterhours-online.md` | `afterhours-online/docs/gaps.md` | 356 |
| `armchair_coach-puzzle.md` | `armchair_coach/puzzle/afterhours_gaps.md` | 87 |
| `armchair_coach-puzzle-design-docs.md` | `armchair_coach/puzzle/design/docs/afterhours_gaps.md` | 1003 |
| `armchair_coach-puzzle-focus-gaps.md` | `armchair_coach/puzzle/design/docs/afterhours-focus-gaps.md` | 267 |
| `cartographer.md` | `cartographer/docs/engineering/afterhours_gaps.md` | 741 |
| `endless-dance-chaos.md` | `endless-dance-chaos/afterhours_gaps.md` | 260 |
| `floatinghotel.md` | `floatinghotel/docs/afterhours-gaps.md` | 2799 |
| `hanabi.md` | `hanabi/afterhours_gaps.md` | 14155 |
| `hanabi-index.md` | `hanabi/afterhours_gaps_index.md` | 907 |
| `kart-afterhours.md` | `kart-afterhours/docs/afterhours_gaps.md` | 295 |
| `mailboy.md` | `mailboy/afterhours_gaps.md` | 250 |
| `MyNameChef.md` | `MyNameChef/afterhours_gaps.md` | 289 |
| `pharmasea.md` | `pharmasea/afterhours_gaps.md` | 1202 |
| `pong-afterhours.md` | `pong-afterhours/afterhours_gaps.md` | 63 |
| `pumpkin-beachball.md` | `pumpkin-beachball/afterhours_gaps.md` | 890 |
| `pumpkin-beachball-online-docs.md` | `pumpkin-beachball/docs/afterhours-online-gaps.md` | 768 |
| `pumpkin-beachball-online-notes.md` | `pumpkin-beachball/notes/afterhours-online-gaps.md` | 618 |
| `voice_ah.md` | `voice_ah/afterhours_gaps.md` | 121 |
| `wordproc.md` | `wordproc/docs/afterhours-gaps.md` | 26 |

Total: 25,412 lines across 20 files. `cartographer-requests.md` (11 lines)
predates this collection and has no single source file; it was left as-is.

## What changed in this collection

- New: `afterhours-todo.md`, `afterhours-online.md`, the three
  `armchair_coach-puzzle*` files, and `pharmasea.md` — none were collected
  before.
- Refreshed: `cartographer.md`, `floatinghotel.md`, `kart-afterhours.md`,
  `mailboy.md` — the sources had moved on since the 2026-11-15 snapshot
  (new entries, closed gaps marked shipped, re-triages against newer pins).
- Unchanged: the other ten files were already byte-identical to their sources.

Warning: the current `mailboy/afterhours_gaps.md` is a rewrite about the
Web/Emscripten backend; it no longer contains the 2026-10-19 zig
ReleaseSafe root-cause entry. That version survives in this repo's git
history (the pre-refresh `mailboy.md`).

## Deliberately excluded

- `hanabi/puffin_gaps.md` — Hanabi-vs-Puffin feature gaps, not afterhours gaps.
- `kart-afterhours/.claude/worktrees/*/docs/afterhours_gaps.md` — four
  identical 135-line older snapshots of the kart file; the live file above
  supersedes them.
- Vendored copies (e.g. `afterhours-online` inside
  `pumpkin-beachball/vendor/`) — duplicates of a source already collected.
- Gap mentions scattered in other repos' `todo.md` files and in
  `afterhours/tests/downstream_gaps_test.cpp` — not standalone gap files;
  the armchair focus file above is the prose record for those D33–D38 tests.
- Repos with no gap file (`tetr-afterhours`, `prime_pressure`, `break-ross`,
  `pixels`, `webhelper`, `template`): nothing to collect.
