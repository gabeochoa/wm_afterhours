# Gap design decisions

Started 2026-09-12. The user requested a design discussion before implementation
so the remaining approved gaps can be worked through without repeated questions.
This document separates accepted decisions from recommendations awaiting answers.
No implementation started during this discussion.

Source inventory: [AFTERHOURS_GAPS.md](AFTERHOURS_GAPS.md#user-decisions).

## Enough direction to implement

These approved items have clear behavior and validation requirements in the gap
file. Resolve routine API and implementation details from the existing code.

| Item | Established behavior |
|---|---|
| UP-01 Audio | Master multiplies the independent music/effects preferences. Loading later applies current gains. |
| UP-02 Settings | Serialize before replacing the file, preserve the last good file on failure, and return truthful results. |
| UP-03 Clipboard | A scoped test provider covers apps and built-in widgets, restores the previous provider, and distinguishes fresh writes. |
| UP-05 Binding persistence | Preserve every binding alternative and modifier; reject invalid input without partially replacing mappings. Keep format integration optional. |
| UP-10 Capture | Explicit encoded PNG and owned raw RGBA results, consistent across backends, with defined dimensions and orientation. Inspect and migrate affected callers. |

Implementation readiness does not establish that a fix has passed verification.
Recheck source revisions and callers before editing. Existing gap entries specify
the necessary failure cases and runtime checks.

## Decisions to discuss

Each row remains pending until the user answers. Recommendations are proposals.

| Decision | Affected work | Recommendation / choice to settle | Status |
|---|---|---|---|
| D-01 Platform coverage | UP-04 dialogs; UP-09 file watching | Reuse macOS implementations first and explicitly report unsupported platforms. Alternatively require Windows/Linux implementations in the first release, or choose different coverage for each feature. | Asked; awaiting answer |
| D-02 Chart scope | UP-12 | Choose the initial chart types and useful interactions. Proposed foundation is line, area, bar, scatter and sparkline, with hover values, live data and a wm test screen. Decide whether other types or pan/zoom are needed initially. | Pending |
| D-03 Profiling experience | UP-11 | Choose default presentation and collection behavior: an opt-in overlay with an embeddable panel, collection only while enabled, bounded history, pause/reset and custom counters. Confirm whether background recording or export is needed initially. | Pending |
| D-04 Input prompts | UP-06 | Choose automatic switching behavior. Proposed default switches on deliberate keyboard/gamepad input, ignores stick noise and incidental pointer motion, and allows callers to pin a device. | Pending |
| D-05 Screen discovery | wm TODO | Choose searchable categories/tree versus a flat searchable list. Preserve comma/period cycling without duplicate destinations and keep the active screen visible. | Pending |

## Work outside this decision batch

UP-07 screen-reader support and UP-08 mutable RGBA textures remain deferred until
a consumer implements them and requests upstreaming. Sound-feedback hooks and
the periodic timer remain skipped.

The older visual, layout, slider, focus-order and relationship gaps retain their
recorded status. They are not all covered by the cross-project approvals. The
separate cascade-delete plan remains unstarted. Visual targets are already set
by the web mocks; investigation must establish which mismatches need library
changes and which can be fixed in wm.

Performance follow-ups start with measurement. Atlas layout, lazy loading and
memory optimizations should follow evidence rather than a design questionnaire.
No additional performance requirement has been inferred here.
