# Gap design decisions

Started 2026-09-12. The user requested a design discussion before implementation
so the remaining approved gaps can be worked through without repeated questions.
All five design questions below are settled. No implementation started during
this discussion. Future question-tool options should be numbered, as requested
by the user.

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

## Accepted design decisions

These rows record the user's answers. The implementation notes below supply
defaults and verification requirements for working through the approved gaps.

| Decision | Affected work | Decision | Status |
|---|---|---|---|
| D-01 Platform coverage | UP-04 dialogs; UP-09 file watching | Implement macOS first behind one platform-independent public API. Callers use the same types, calls and result handling everywhere, without platform conditionals. Other backends initially return the shared unsupported result. | Accepted |
| D-02 Chart scope | UP-12 | Build the charts needed by the profiling UI first and exercise them in the wm test screen. Keep the broader chart set and additional interactions as TODOs; they do not block the profiling UI. | Accepted |
| D-03 Profiling experience | UP-11 | Support continuous bounded recording independently of panel visibility, code controls and a compile-time switch to remove the profiler. Measure overhead before claiming it is suitable to leave on. | Accepted |
| D-04 Input prompts | UP-06 | Automatically switch on deliberate keyboard/gamepad input, ignore stick noise and incidental pointer motion, and allow callers to pin a device. | Accepted |
| D-05 Screen discovery | wm TODO | Use a searchable tree with collapsible categories, keyboard navigation and a clearly marked current screen. Preserve comma/period cycling without duplicate destinations. | Accepted |

### D-01: One public API, macOS implementation first

The user chose macOS first and required that library users should not need to
know which platform implementation they are calling. Keep platform headers,
native handles and backend selection inside the library. Public requests,
owned results, lifecycle and completion rules must be the same on every
platform. Adding Windows or Linux support must not require caller changes.

Before a backend exists, return the common unsupported outcome through that
same API. Do not pretend success or report cancellation. This keeps error
handling portable without claiming identical availability. Test responses must
also work through the common API on unsupported hosts.

### D-02: Profiling determines the first chart scope

The user chose to focus on charts needed for the profiling UI and leave the
rest as TODOs. Start with frame-time history; reuse the same time-series
rendering for other profiling histories where useful. Choose further chart
types only when the profiling panel needs them. Preserve live updates, value
inspection and the interactive wm test screen for the charts actually built.

The previously proposed five-chart set is no longer a first-release requirement.
Area, bar, scatter and sparkline variants that the profiler does not need remain
follow-up work. Pie/donut, stacked charts, histograms and pan/zoom were offered
as options, not individually approved requirements. Reassess them when extending
the chart set. This decision does not defer unrelated approved upstream gaps.

### D-03: Cheap background recording with runtime and build controls

The user wants profiling cheap enough to leave recording, accepts continuous
recording, and wants control from code and a compile-time switch to remove it
when unused. Recording must be independent of panel visibility. A profiling
build can retain bounded recent history while the panel is hidden; callers can
start and stop collection without changing whether the panel is shown.

Implementation direction: keep collection small, reuse bounded storage, and
avoid per-sample allocation after initialization. Do not format labels, build
widgets or sort display tables while the panel is hidden. Sample process
CPU/memory at a lower cadence than frame/system timings. Pausing the displayed
snapshot and stopping collection are separate operations with clear labels.
Retain the planned overlay and embeddable panel with customization.

Provide a compile-time option that removes this profiler's instrumentation,
collection, storage and UI. Disabled instrumentation must not evaluate custom
sample expressions or leave timing calls in the frame loop. Preserve existing
consumer profiling integrations; compiling out the new profiler must not
silently disable an app's independently installed hook.

Compare the same repeatable workload with the profiler compiled out, compiled
in but stopped, recording with the panel hidden, and recording with it visible.
Report frame-time distribution, CPU, memory and steady-state allocations, and
check sustained recording for memory growth. Use `nice -n 10` consistently.
No numeric overhead budget has been agreed and no overhead claim is measured
yet. If recording materially distorts the workload, optimize or reduce sampling
before recommending that consumers leave it on.

### D-04: Automatic input hints with an app override

The user accepted automatic keyboard/controller hint switching. Use deliberate
input to update device preference; stick drift and incidental mouse movement
must not switch the hints. Allow applications to pin a device instead of using
automatic selection. This preference controls prompt presentation, not which
devices may supply input.

Use current bindings, including remaps and modifiers, when formatting hints.
Verify deliberate device changes, noisy idle input, the pinned override and
returning to automatic selection. Controller artwork remains caller-owned.

### D-05: Searchable screen tree

The user chose option 1: a searchable tree with collapsible categories, such as
imitation screens, widgets and diagnostics. Include keyboard navigation and
clearly mark the current screen. Show each intended destination once; give
intentional variants distinct names. Comma/period cycling must not repeat a
destination because it has multiple registrations or aliases.

Implementation direction: choose category names from the actual screen
inventory. Search should find screens across categories, including collapsed
ones, and show enough category context to distinguish results. Keep navigation
in the tree and activation of a screen distinct so keyboard browsing does not
reload screens at every focus change. Reuse existing screen-switch scheduling
outside ECS iteration.

Verify pointer and keyboard search, expand/collapse, direct activation, the
active-screen marker, no-match results and clearing search. Cycle through the
full inventory in both directions and check that only intentional wraparound
revisits a destination. This is wm work; it does not request a new library tree
widget unless existing support proves insufficient.

## Implementation handoff

The five decisions above and the established correctness requirements are
sufficient to begin the approved work. Resolve routine implementation choices
from source and validation rather than reopening these product questions.
Charts needed by UP-11 come before the broader UP-12 TODOs. Keep changes in
reviewable local commits and retain separate commits for individual screen
updates. Nothing is pushed as part of this work.

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
The profiler's additional performance requirement is recorded in D-03 above.
