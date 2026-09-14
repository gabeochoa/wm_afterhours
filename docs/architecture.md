# Architecture and decisions

## Accepted behavior

| Work | Decision |
|---|---|
| Audio | Master multiplies independent music/effects preferences, including late loads. |
| Settings | Serialize before atomic replacement; preserve the last good file and report failure. |
| Clipboard | Scoped test provider covers app and widget calls; restore it and distinguish fresh writes. |
| Native dialogs / watcher | macOS first, one portable API; unsupported is distinct from cancellation. |
| Bindings | Preserve alternatives/modifiers; decode atomically; format integration stays optional. |
| Input prompts | Use current bindings and deliberate input, ignore stick noise, permit a pinned device. |
| Capture | Explicit owned RGBA and PNG, consistent dimensions/orientation across backends. |
| Profiling | Bounded recording independent of visibility; runtime controls and compile-out support. |
| Charts | Build profiler-required charts first; wider types remain follow-ups. |
| Screen browser | Search collapsed categories, separate cursor from activation, unique cycle destinations. |

These decisions are implemented as summarized in [history](history.md). UP-07/08
await consumer-led upstreaming; sound feedback and periodic timers were skipped.
No general plugin migration or cascade-delete implementation was approved here.

## Small core and external plugins

Proposal from the September consumer review. Use ordinary opt-in C++ packages,
public dependencies and app-owned composition. No reviewed caller needs runtime
plugin discovery, a binary ABI or a global message bus.

| Boundary | Responsibility |
|---|---|
| Core / `ah.h` | Entities, handles, collections, queries, systems, lifetime and small diagnostics hooks |
| Utility headers | Color, geometry and interpolation without ECS installation |
| Stateful plugins | Input, animation, audio, files/settings, history, pathfinding and collision |
| Platform adapters | Rendering, windows, textures, clipboard, dialogs and watcher behind portable types |
| UI | Layout, style, widgets, focus, paint; optional charts, profiler and test adapters |
| App | Screens, artwork, document/game rules and refresh policy |

Plugins communicate through shared value types, components, explicit calls and
specific queues/callbacks. Animation produces a value; color interpolation consumes
it. A reusable animation/color binding can live in an integration header depending
on both packages. Color itself need not know the animation scheduler. Define
overshoot, cancellation and target deletion at that binding.

Keep registration phases visible so apps can interleave pre-UI, app UI and post-UI
work. State must outlive registered systems; stop/join workers before releasing
callbacks or graphics resources. OS callbacks enqueue owned results outside ECS
iteration. Register each intended instance once per manager, not once per process.
Current global EntityHelper/static managers are not isolated worlds.

Expose supported authoring APIs, not private internals. Inventory callers before
narrowing existing `PLUGIN_API.md` collection access. Try a typed installer only if
explicit registration repeatedly fails in real consumers.

Suggested order: external-plugin/include-boundary checks; separate authoring from
backend headers; extract color/animation calculations; separate optional UI/E2E
readers; trial Wordproc and a small game. Check standalone headers, two managers,
reentry/teardown, missing backends and observer coexistence. Measure compile time,
binary size and frame cost before claiming gains. Chart extraction remains a todo.

## Prepared text layout

Proposal inspired by [Pretext](https://github.com/chenglou/pretext/tree/37d4b741a67e55ce9d1172c82fc41cffa22d18bf).
Prepare source and metrics when text/style changes; reflow numeric data when width
changes. Share lines/source ranges across sizing, drawing, carets, selection and hit
testing. Normal widget callers should not manage preparation themselves.

Source review at afterhours `3a5f2444` found duplicate whole-string caches, repeated
prefix measurement, wrap keys missing font/size/spacing, unconditional text-area
rebuilds and font replacement without generation invalidation. These are investigation
leads, not measured speedups. Atlas-independent Sokol advances are a separate completed fix.

- Own source storage in immutable `PreparedText`; retain source-byte and cluster mappings.
  `LayoutBuffer` reuses output capacity. Metrics-only layout shares the same break walker.
- Resolve actual font resource, weight, size, spacing and supported break policy once.
  Font reload/fallback/backend changes invalidate by generation, not texture ID alone.
- Width changes reflow; paint-only changes repaint. Size/metric changes reprepare.
  Avoid lossy key quantization near break boundaries and verify hash equality.
- Start with per-widget retention, bounded in bytes as well as entries. Replace redundant
  caches rather than adding another global map; count live retained memory after eviction.
- Preserve whitespace, hard breaks, source bytes and current long-word behavior initially.
  Independent word widths can lose kerning/spacing. Preparing runs must match drawing.
- Keep Unicode segmentation/shaping optional. UAX #29, #14 and #9 are different contracts.
  HarfBuzz alone supplies neither fallback nor paragraph bidi. Measuring CoreText while
  drawing original strings through DrawTextEx would recreate a measurement/render split.
- Keep text layout out of ECS core. Begin on the UI thread; later pure reflow can use
  immutable preparation with caller-owned buffers.

Implement in separate verified steps: instrument; unify resolved inputs/invalidation;
prepare legacy-compatible runs; share layout/edit geometry; retire duplicate paths;
then evaluate Unicode providers. Test break boundaries, fractional sizes, blank lines,
CRLF/tabs, long words, styled runs, reloads, fallback, copy bytes and caret round trips.
Classify unsupported emoji/Thai/Arabic/bidi honestly.

Benchmark cold preparation, warm idle, width changes, edits and unique-text churn
separately. Use the multiline/styled/language/input/SyncScroll/virtual-list/profiler
screens. Record time, allocations, copied bytes, retained memory and frame contribution.
Warm fonts separately; alternate before/after runs without overlapping builds. No
numeric budget or speedup is established yet. Native rendering is the first oracle;
HTML parity needs matching fonts/policies and a live browser.
