# Small core and consumer-authored plugins

Proposal only, 2026-09-12. No library or application implementation is part of this document. The approved gap work can continue independently. API names marked proposed are sketches, not existing interfaces.

Keep the current ECS core and make the plugin boundaries easier to use and enforce. Let an application compose ordinary C++ functions, components and systems. Use an optional integration header when two independently useful plugins need repeated glue. Do not introduce a central plugin message bus or runtime discovery system for the current applications.

The single-include core already exists. `vendor/afterhours/ah.h:3` includes only `src/ecs.h`, which exposes entities, queries, handles and systems. The work is mostly about removing accidental dependencies around that core and clarifying ownership and registration.

## Evidence and limits

This proposal uses the earlier [cross-project review](afterhours-upstream-review.md) and focused reads of current afterhours, wm, Wordproc, Cartographer, Pong and Floatinghotel code. Puzzle supplied an additional example of handle-based application systems. It is not a fresh audit of every file under `~/p`, and no application or benchmark was run.

The library was changing concurrently during approved gap implementation. Its HEAD was `325caeece371697e38c14e8b1184eeb7cb135657` at the final source check. References describe the files read, and line numbers may shift.

| Observed source | What already exists | Implication |
|---|---|---|
| `vendor/afterhours/ah.h:3`, `src/ecs.h:3` | One public include for ECS, without plugin or graphics includes | Keep this entry point. A new umbrella header is unnecessary. |
| `vendor/afterhours/PLUGIN_API.md`, `check_plugin_boundaries.sh` | Documented public APIs and a source boundary check | Improve the existing contract and checker. Do not treat plugin authoring as a new capability. |
| `vendor/afterhours/src/developer.h:224` and `:278` | Static lifecycle methods and `PluginCore` concept | The concept checks callable signatures, not registration, dependencies, ownership or ordering. |
| `vendor/afterhours/src/developer.h:13` | Backend selection and graphics types share the plugin authoring header | Separate the plugin contract from backend type definitions. |
| `vendor/afterhours/src/plugins/color.h:7`, `:124`, `:504` | Color functions and `HasColor` live together and include the developer header | Split pure color operations from ECS attachment if standalone use justifies it. |
| `vendor/afterhours/src/plugins/animation.h:271`, `:331` | A static manager per key type and a non-templated no-op registration function | A satisfying concept can still produce no running animation. Typed registration must be explicit. |
| `vendor/afterhours/src/core/system.h:604`, `:628` | Systems append to ordered phase vectors and run in that order | Existing applications already have a straightforward composition mechanism. |
| `vendor/afterhours/src/core/system.h:561` | One process-wide profile hook shared by managers | Profiling observers need deliberate ownership; installing a plugin must not silently replace another observer. |
| `vendor/afterhours/src/plugins/ui/animation_config.h:267`, `:314` | UI has separate easing and spring code and reads the animation instant flag | There is already animation/UI composition, including duplicated algorithms and shared policy. |
| `vendor/afterhours/src/plugins/ui/imm_components.h:1373` | Switch rendering calls `colors::lerp` with animation progress | Ordinary value passing already solves the specific animation/color question. |
| `vendor/afterhours/src/plugins/ui/rendering.h:17`, `:716` | UI rendering includes E2E headers and reads `test_input::detail::test_mode` | Optional inspection should depend on public UI observations, not UI depending on E2E internals. |
| `vendor/afterhours/src/plugins/e2e_testing/perf_commands.h:44` | The built-in collector lives with E2E commands | The requested profiling collector belongs in an optional independent plugin, read by UI and E2E adapters. |

A small local include-graph check followed quoted includes and counted all conditional branches. It found 19 local headers reachable from `ah.h`, 18 from `color.h`, and 96 from `ui.h`. No plugin or graphics header was reachable from `ah.h`. These are source dependency counts, not preprocessor output or measured compile costs. Angle-bracket dependencies such as fmt are outside that count.

## What consumers suggest

Wordproc separates UI registration around its own widget creation in `~/p/wordproc/src/ui/ui_context.h:93`. Toast and modal setup are explicit, and modal rendering happens after ordinary UI rendering. A helper that registers every plugin's entire lifecycle in one opaque call would hide a real ordering requirement.

Cartographer's `~/p/cartographer/src/systems.cpp:2184` places library input systems beside its own map, character, rope and tool systems. Its render list at line 2221 surrounds application drawing with explicit rendering phases. Those application systems are already close to consumer-authored plugins. They do not need dynamic loading to become reusable modules.

Pong opts into input and window management beside `afterhours/ah.h` in `~/p/pong-afterhours/src/main.cpp:17`, then registers them at line 373. A small game should retain that experience without importing UI, networking, profiling or persistence.

Floatinghotel separates a watcher object from its ECS adapter in `~/p/floatinghotel/src/platform/file_watcher.h:40` and `src/ecs/file_watcher_system.h:13`. The platform object produces owned events. The app polls them, classifies paths and updates repository state. This is a useful model for other plugin boundaries. Git policy remains in Floatinghotel.

wm's animation demos sometimes advance their own managers directly, for example `src/systems/screens/AnimationBasicDemo.h:91`. Plugins should support plain use as well as an optional ECS registration helper. Requiring a singleton and three empty lifecycle methods for every color function, timer value or animation algorithm would make authoring harder.

Hypothesis: explicit composition with better package boundaries will meet these consumers' needs. The current reads do not establish demand for downloaded binary plugins, independent plugin ABI versions, hot unloading or automatic dependency discovery.

## Three possible designs

| Design | Author experience | Costs and fit |
|---|---|---|
| 1. Ordinary modules with explicit composition | Include selected headers, own state, register systems, pass typed values or references. Share small integration headers when needed. | Fits current callers and adds little machinery. The app remains responsible for visible registration order. Recommended. |
| 2. Typed plugin installation graph | Plugins declare required types and phase constraints. A host installs them, checks duplicates, sorts registration and owns teardown. | Can improve many-plugin startup errors, but requires a new host, lifecycle rules and dependency graph. Prototype only if explicit registration causes repeated observed failures. |
| 3. Runtime registry and event bus | Plugins discover named services and publish messages through a host. | Supports dynamically selected modules, but hides dependencies and adds dispatch, subscription lifetime and versioning rules. No reviewed consumer requires this. Defer. |

Design 1 does not prohibit a typed queue, a callback or an interface. It chooses those mechanisms for a particular boundary instead of making every interaction travel through a global registry.

## Recommended boundaries

| Location | Keep or move here | Reason |
|---|---|---|
| Core through `ah.h` | Entity and component lifetime, handles and resolution, collections, queries, system execution and phase ordering | Every ECS plugin needs these contracts. |
| Small core diagnostics contracts | Assertions, logging entry points, system identity and optional timing observation | Failures and instrumentation need a stable connection to execution. Rich implementations can remain optional. |
| Optional utility headers | Colors, geometry, interpolation, seeded RNG, timers | Pure calculations need no plugin installation, singleton or graphics window. Their names need not imply ECS ownership. |
| Optional stateful plugins | Animation scheduling, input, audio, files/settings, command history, pathfinding, collision, translation, telemetry | They have specific callers and dependencies, and mostly already live under `plugins/`. |
| Platform/backend packages | Window creation, rendering, textures/capture, clipboard, native dialogs and file watching | Implementation dependencies and platform code stay behind shared public types. macOS-first implementation still has one caller API. |
| UI package | Layout, styling, widgets, focus and rendering | This is a substantial opt-in feature. Autolayout belongs with UI or a separately useful layout package, not the ECS core. |
| Optional UI extras | Profiling panel, charts, inspectors, E2E adapters and optional theme persistence | A basic widget user should not automatically need test command processing or a profiler. Split where dependencies or cost justify it. |
| App-owned modules | Screens, game rules, document schemas, repository refresh rules, art and sound policy | Reuse should follow an actual second caller. No reason to upstream every application system. |

Do not move files just to make directory names look smaller. Existing optional plugins can stay physically in the afterhours repository. The useful guarantees are that consumers can omit them, their headers compile independently, and their public contracts do not expose unrelated internals.

The small concrete extraction candidates are:

1. A lightweight plugin authoring header separated from the backend types in `developer.h`.
2. Color value/math operations separated from `HasColor` and the ECS include chain, with existing `color.h` preserved as a convenience include during a deliberate migration.
3. Animation math/state separated from its static manager and registration helpers. UI can consume the same algorithms without importing a second scheduler.
4. A profiling collector separated from E2E commands, plus separate UI and E2E readers. This aligns with approved UP-11.
5. UI inspection records or callbacks separated from E2E test state. Retain test visibility and clipping information while reversing the dependency.
6. Crash report formatting, stack collection and signal-handler installation separated from the minimal current-system diagnostic connection in `SystemManager`.

Numbers 1 through 6 are planning recommendations, not approved implementation commitments. They should not expand the current gap implementation automatically.

## How plugins communicate

Use the narrowest public contract that fits the data.

| Interaction | Preferred contract | Ownership |
|---|---|---|
| Color interpolation driven by animation | Return a progress value and call a color function | Caller owns endpoints and destination. |
| A system changes an entity's visible state | Public component types, queried by the consuming system | Entity owns components; a named system owns each write phase. |
| A native operation completes later | Request handle and owned result, drained on the app thread | Provider owns in-flight work; caller consumes results. |
| One configurable action needs a callback | Typed callback supplied at construction or registration | Owner controls callback lifetime and cancels before destruction. |
| A profiler feeds UI, export and tests | Read-only snapshots from one collector | Collector owns bounded history; readers do not reinstall instrumentation. |
| An app-specific bridge combines two plugins | Header or system depending on both public APIs | Bridge owns the mapping, neither base plugin imports the other. |

A plugin may depend on another plugin. A UI package depending on color is reasonable. The rule is that dependency direction is explicit and acyclic. Independent base packages do not need mutual awareness.

For events, document whether delivery happens now or on the next drain, who can consume a record, and what happens on overflow. Floatinghotel's worker-to-main-thread queue is justified by a real thread boundary. Do not make an unbounded cross-plugin bus to deliver per-frame color changes.

## Worked example: a new animation feature and color

Today the composition can be just this expression. `eased_progress` comes from the animation chosen by the caller; the color operation already exists.

```cpp
const auto displayed = afterhours::colors::lerp(start, finish, eased_progress);
```

If a user adds a new easing curve, the curve produces progress and the same color function consumes it. `color.h` does not need registration, an animation include, a new virtual method or a message subscription.

Proposed value-oriented API sketch:

```cpp
#include <afterhours/src/plugins/animation_values.h>
#include <afterhours/src/plugins/color_values.h>

struct Fade {
    afterhours::animation_values::ProgressTrack track;
    afterhours::Color start;
    afterhours::Color finish;
};

afterhours::Color advance_fade(Fade& fade, float dt) {
    const float progress = afterhours::animation_values::advance(
        fade.track, dt, MyEaseCurve{});
    return afterhours::colors::lerp(fade.start, fade.finish, progress);
}
```

`animation_values.h`, `color_values.h`, `ProgressTrack` and `advance` are proposed names. `MyEaseCurve` is a consumer-owned callable. This sketch describes a timed progress track, not a spring simulation. Springs need their own velocity/state and must not be forced into a stateless easing interface.

The proposed progress API must return the settled value after completion. Today's `AnimationManager::get_value` returns no value for inactive tracks at `animation.h:172`; blindly mapping that to zero would reset a finished fade. Specify this before reusing the current manager as the implementation.

Overshoot and color space are separate choices. A spring can overshoot position while a color bridge clamps progress to a supported range. Existing `colors::lerp` operates on byte channels. It must not silently become linear-light interpolation as part of this refactor. A future color interpolation function can implement that policy and be passed by the caller without changing the animation plugin.

If several apps repeat entity binding, add an opt-in integration header, for example `plugins/integrations/animated_color.h`. It includes both public contracts and defines the binding component and system. That integration resolves a handle each tick, computes the value and writes `HasColor`. The animation package never needs to name `HasColor`.

The bridge should animate its own state and expose a result. Binding raw pointers to component fields across frames would make entity deletion unsafe. Start with one writer per animated property. Replace, reject or queue another request explicitly; do not let registration order accidentally pick a winner.

## Registration, ordering and teardown

Keep the existing `register_update_system` and `register_render_system` operations as the execution basis. Packages may offer registration helpers, but expose the phases an app needs to interleave. Wordproc's pre-UI, app UI, post-UI sequence is a concrete requirement, not a candidate for hiding behind dependency auto-installation.

Existing-style consumer-authored module sketch:

```cpp
namespace my_app::color_effects {

void register_update_systems(afterhours::SystemManager& systems) {
    systems.register_update_system(std::make_unique<AdvanceColorEffects>());
}

}

my_app::color_effects::register_update_systems(systems);
my_app::register_render_systems(systems);
```

`AdvanceColorEffects` is an app-owned system. This form already works without editing afterhours. A plain utility library does not need to satisfy `PluginCore`. A templated plugin should expose a helper whose type arguments are required, rather than a useful-looking no-op overload merely to satisfy a concept.

Lifecycle contract to document for new stateful plugins:

1. Construct configuration and owned state before registering systems that use it. Validate required services at registration, with a named error.
2. Register each instance once with its intended manager. Multiple screen managers may intentionally use the same plugin; a process-wide installed flag would reject valid use.
3. Run updates in visible order. Animation advancement must precede consumers of that frame's animation values. OS callbacks enqueue owned results and do not enter ECS while it is iterating.
4. On screen exit, cancel its outstanding requests and release its animation bindings. Destruction of systems must precede destruction of any state they reference.
5. On app shutdown, stop and join worker activity before releasing callback state or backend resources.

For duplicate registration, begin with an explicit precondition and a targeted diagnostic in stateful registration helpers. If callers repeatedly need install/remove operations, add manager-owned registration tokens with deterministic removal at a phase boundary. Do not invent hot unloading as a prerequisite for the first plugin.

The current process-global `EntityHelper` and static animation managers limit independent instances. This proposal does not claim they are already isolated worlds. New plugin state should be owned by a system, an explicit caller object, or a scoped component. Do not require a whole-ECS world migration to extract a color function. Expand world ownership only with a concrete multi-instance test and consumer.

Profiling is the strongest immediate shared-observer case. Prefer one collector with multiple readers. If another instrumentation provider must run concurrently, add scoped subscriptions to that specific hook and define teardown during a running system. A provider must not overwrite another provider's begin/end pair halfway through a scope. This does not require a general service locator.

## Public extension APIs, not access to internals

Consumer-authored plugins should use supported public APIs. Calling them internal while expecting independent consumers to rely on them would make ordinary library updates break plugins without warning.

The stable extension contract should cover components, handles/resolution, queries, system registration and lifetime. Each plugin publishes the types and operations other plugins may use. Backend adapters publish platform-neutral requests/results and capabilities. Implementation helpers stay under `detail` or private headers with no compatibility promise.

The existing `PLUGIN_API.md` currently permits broad collection access such as `get_entities_for_mod`. Inventory those callers before tightening it. Replace real needs with smaller operations, then migrate callers; a documentation ban alone would strand existing plugins.

Check contracts at compile time where signatures suffice. Check lifetime, ordering, unsupported operations and worker shutdown with runtime tests. `PluginCore` alone cannot prove any of those properties.

Use source compatibility as the first guarantee. No reviewed caller requires a binary plugin ABI. A small supported-version range in a plugin README and compile checks against supported afterhours versions are sufficient initially. A packaging target such as `afterhours::files` may carry platform link requirements, but the public header and result type remain identical across platforms.

## Validation before migration

- Compile a translation unit containing only `ah.h` without graphics, UI, native dialog, audio or test dependencies. Check its local include boundary automatically.
- Compile public utility and plugin headers independently and in different orders. Linking an unused optional plugin should not be necessary.
- Build a tiny external plugin outside `vendor/afterhours` using only documented headers. Include a plain utility, an ECS system and one integration between two plugins.
- Compare a timed fade and a spring-driven color change before and after extraction. Cover settled state, cancellation, zero duration, overshoot policy, instant mode and target deletion.
- Verify two screen managers, repeated screen entry/exit, shutdown with queued OS results, and one plugin missing an optional backend. No stale callbacks or double advancement.
- Verify profiling and E2E readers can coexist without duplicate instrumentation or replaced hooks. Verify the compiled-out configuration through generated symbols or assembly as well as behavior.
- Measure clean/incremental compile time, linked size and frame overhead before claiming the split improves them. The include counts above do not establish a speedup.
- Preserve wm keyboard/pointer flows and screenshot behavior, Wordproc's UI ordering, and a small game build. A change that only compiles in wm has not proved consumer plugin authoring.

## Suggested migration sequence

Each step is independently reviewable. Keep implementation commits separate from this proposal and incorporate later review fixes into their original local commits, as requested.

1. Add the external-plugin compile example and include-boundary checks against current behavior. Record actual build dependencies and costs.
2. Split the plugin authoring contract from backend types, preserving current include paths while migrating first-party plugin includes. Tighten the existing boundary checker to cover transitive dependencies and forbidden `detail` use.
3. Extract color calculations and animation value/state operations. Migrate one wm demo and the shared UI color transition. Add the integration header only if another concrete caller repeats the binding code.
4. Let approved profiling work produce an independent collector, UI reader and E2E reader. Keep diagnostic hooks in core small and compiled out when configured.
5. Move UI inspection and richer crash reporting behind their own opt-in adapters, preserving existing diagnostics and test observations.
6. Trial consumer-authored packages in Wordproc and one small game. Document the necessary phases and lifetime rules from those actual integrations.
7. Reassess installation helpers only after that trial. Add a typed installer if repeated setup mistakes justify it; retain explicit ordering where app work must interleave.

The initial design decision can be just this: plugins are normal opt-in C++ packages, with public dependencies and app-owned composition. Animation/color needs shared values and, sometimes, a small adapter. It does not need plugins to discover one another at runtime.
