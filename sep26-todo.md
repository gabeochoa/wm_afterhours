# Sep 26 — ranked gap backlog

Demand = distinct projects asking. Sorted by demand, easiest
first within a tier. Full detail: todo.md "Gap intake 2026-09-23".
Items 1–13 of the original ranking (E2E addressing, tooltip config,
contrast helper, focus ring family, small imm components, validation
throttle, pointer parity, text_area parity, prepared text, virtual
list, kart VERIFYs, Margin/Padding ctors + settings-path override,
OS appearance query)
are done; their records live in todo.md.

| #  | Item                             | Want        | Effort |
|----|----------------------------------|-------------|--------|
| 1  | Rendering-nits batch (14)        | hanabi      | S      |
| 2  | #374 sokol resize abort          | hanabi      | M      |
| 3  | #375 focus border top edge       | hanabi      | M      |
| 4  | Widget-lifetime remainder        | hanabi      | M      |
| 5  | Text-editing action surface      | hanabi      | M      |
| 6  | Frame/host-loop family           | hanabi      | L      |
| 7  | context_menu plugin              | wordproc    | M/L    |
| 8  | Colour input                     | hanabi      | L      |
| 9  | accessibility plugin             | hanabi      | L      |
| 10 | Two view trees in one window     | hanabi      | XL     |

cg = cartographer, wp = wordproc, fh = floatinghotel.

Still open from closed items:
* Pointer parity covered polling parity only. hanabi #405
  (trackpad vs wheel-detent distinction/smoothing — needs a
  platform delta kind the backends do not expose) and #406
  (hanabi ruled it app-side).
* Prepared text covered the cache correctness, the #48 coverage
  query and #62 styled-run placement. Still open: the full
  PreparedText redesign (docs/architecture.md), #51 text-landing
  geometry and #286/#68 self-reported laid-out size.

Notes:
- #6 is single-project but holds the intake's only CRITs
  (#542 request-frame, #546 frame-wake); severity would put
  it at #3 of the original ranking.
- REJECT? list (todo.md Unit G) is not in this ranking:
  G1 OS-integration family, G2 table component, G3 rich-text
  subsystem, G4 app E2E verbs, G5 negative results, G6
  comment-preserving JSON writes.
