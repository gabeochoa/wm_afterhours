# Sep 26 — ranked gap backlog

Demand = distinct projects asking. Sorted by demand, easiest
first within a tier. Full detail: todo.md "Gap intake 2026-09-23".
Items 1–16 of the original ranking (E2E addressing, tooltip config,
contrast helper, focus ring family, small imm components, validation
throttle, pointer parity, text_area parity, prepared text, virtual
list, kart VERIFYs, Margin/Padding ctors + settings-path override,
OS appearance query,
rendering-nits batch, #374 sokol
resize abort, #375 focus border top edge)
are done; their records live in todo.md.
The widget-lifetime parent cleanup is also
done (creation-time parent->children link,
subtree retired in one sweep); its remainders
are listed below. The text-editing action
surface is done too (line deletion, platform-
correct default keymap, EditCommand responder).
The frame/host-loop CRITs are done too
(request-frame, frame-wake, Metal target_fps);
the rest of that family is listed below.
context_menu closed as VERIFY (the primitive
already shipped and wordproc had adopted it),
plus its two named remainders, checked/radio
items and bounded long-menu scrolling.
Colour input is done too (three swatch shapes
in ui/color_swatch.h; WM colour swatch lab,
E2E 362-364).

| #  | Item                             | Want        | Effort |
|----|----------------------------------|-------------|--------|
| 1  | accessibility plugin             | hanabi      | L      |
| 2  | Two view trees in one window     | hanabi      | XL     |

cg = cartographer, wp = wordproc, fh = floatinghotel.

Still open from closed items:
* Frame/host loop beyond the CRITs: #543 frame
  phases, #544 input-activity snapshot, #545
  window exposure events, #547 timer deadlines,
  #548 dt-is-callback-time, #549 headless
  cadence harness, #580/#581/#587/#589 and the
  allocation/GPU accounting members (todo.md
  loose-items line).
* Widget lifetime: exit animations still have
  nothing to animate (no motion-aware hold on
  the retirement sweep), and the #171 family's
  consumer-visibility half (#146, #160, #162,
  #163, #525 specifics) is untouched.
* Pointer parity covered polling parity only. hanabi #405
  (trackpad vs wheel-detent distinction/smoothing — needs a
  platform delta kind the backends do not expose) and #406
  (hanabi ruled it app-side).
* Rendering-nits remainders: #88 baseline alignment (a
  feature, not a nit), #76 (needs hanabi's original repro —
  not provable from the one-line claim), and the todo.md
  line's non-ranked members (#92/#106, #93/#97/#222, #101).
* Prepared text covered the cache correctness, the #48 coverage
  query and #62 styled-run placement. Still open: the full
  PreparedText redesign (docs/architecture.md), #51 text-landing
  geometry and #286/#68 self-reported laid-out size.

Notes:
- REJECT? list (todo.md Unit G) is not in this ranking:
  G1 OS-integration family, G2 table component, G3 rich-text
  subsystem, G4 app E2E verbs, G5 negative results, G6
  comment-preserving JSON writes.
