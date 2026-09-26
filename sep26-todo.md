# Sep 26 — ranked gap backlog

Demand = distinct projects asking. Sorted by demand, easiest
first within a tier. Full detail: todo.md "Gap intake 2026-09-23".

| #  | Item                             | Want        | Effort |
|----|----------------------------------|-------------|--------|
| 1  | E2E addressing + command packs  | hanabi, cg, | DONE   |
|    |                                  | wordproc    |        |
| 2  | Tooltip config (font/pad/gap)   | WM, wordproc| DONE   |
| 3  | Public contrast-ratio helper    | cg (+WM)    | DONE*  |
| 4  | Focus ring family               | hanabi, wp  | DONE   |
| 5  | Small imm components            | cg, kart    | DONE   |
| 6  | Validation-log throttle         | cg, hanabi  | DONE   |
| 7  | Pointer parity (click/wheel)    | wp, hanabi  | DONE*  |
| 8  | text_area parity                | hanabi (+fh)| M      |
| 9  | Prepared text + label inset     | WM, hanabi  | L      |
| 10 | Virtual list variable heights   | hanabi, WM  | L      |
| 11 | kart VERIFYs (3)                | kart        | XS-S   |
| 12 | Margin/Padding ctors, set. path | cg          | XS     |
| 13 | OS appearance query             | hanabi      | S      |
| 14 | Rendering-nits batch (14)       | hanabi      | S      |
| 15 | #374 sokol resize abort         | hanabi      | M      |
| 16 | #375 focus border top edge      | hanabi      | M      |
| 17 | Widget-lifetime remainder       | hanabi      | M      |
| 18 | Text-editing action surface     | hanabi      | M      |
| 19 | Frame/host-loop family          | hanabi      | L      |
| 20 | context_menu plugin             | wordproc    | M/L    |
| 21 | Colour input                    | hanabi      | L      |
| 22 | accessibility plugin            | hanabi      | L      |
| 23 | Two view trees in one window    | hanabi      | XL     |

cg = cartographer, wp = wordproc, fh = floatinghotel.

* #3 was already public (`colors::contrast_ratio` in color.h);
  closed by pinning it with `color_contrast_test`, no new code.
* #7 covers polling parity only. Still open: hanabi #405
  (trackpad vs wheel-detent distinction/smoothing — needs a
  platform delta kind the backends do not expose) and #406
  (hanabi ruled it app-side).

Notes:
- #19 is single-project but holds the intake's only CRITs
  (#542 request-frame, #546 frame-wake); severity would put
  it at #3.
- #9 has the largest raw volume (~28 hanabi headings plus
  WM line 57); weighted by headings it would be #1.
- REJECT? list (todo.md Unit G) is not in this ranking:
  G1 OS-integration family, G2 table component, G3 rich-text
  subsystem, G4 app E2E verbs, G5 negative results, G6
  comment-preserving JSON writes.
