# Cross-project afterhours review

Review date: 2026-09-12. Findings are in [AFTERHOURS_GAPS.md](AFTERHOURS_GAPS.md#cross-project-upstream-review-2026-09-12), UP-01 through UP-10. Three smaller opportunities are explicitly deferred there.

## Scope and method

All 38 other top-level project directories under `/Users/gabeochoa/p` were inventoried and assigned to one of three reviewers. Their inventories contained 2,648 code-like paths. That count includes tests, scripts and some generated/exported HTML later excluded from source review; it is not a count of files read line by line. The review used broad source searches followed by focused reads of implementations and callers. Large game, API and rendering files were sampled. It does not establish complete control-flow coverage or runtime correctness.

Vendor/dependency implementations, generated output, raw datasets, user documents, messages, personal configuration and secrets were outside the review. An HTML file encountered in the website inventory proved to be exported conversation data; it was excluded and supplies no finding. Empty and developer-tooling projects are accounted for below instead of inventing library requirements from them. Hidden `.claude` and `.codex` directories are tool configuration, not additional application projects.

The owned nested project `armchair_coach/puzzle` received a separate follow-up review, recorded below. Dependency submodules and alternate worktrees were excluded; the non-vendor Git links also include two cartographer worktrees.

All review processing used `nice -n 10`. No applications, builds or tests were run, no external services queried, and no library, consumer code or submodule pins changed. Only the two wm review documents were edited. The audio, settings and capture-format source findings received a second independent review. Other recommendations remain design candidates with explicit validation scenarios.

## Library availability

- Standalone `~/p/afterhours`: `19d6c9708a7624f81644a222dbefd02aed298069`, dated August 26.
- Newer wm vendor: `e3f13a7827e333460d38078fde716e1e4a5a68ef`, dated September 12.
- The standalone commit is an ancestor of the vendored commit. Both trees were inspected. Features present in the newer tree are already implemented, even if consumers or the standalone checkout have not adopted them. No remote publication state was inferred.

## Project coverage

HEAD and dirty state are local review snapshots. Existing modifications were preserved. Floatinghotel changed concurrently; its new reading-navigation work was not reviewed. Its cited watcher files were stable during the review. Source line references elsewhere may drift as those projects continue changing.

| Project | HEAD / state | Inventoried code paths | Source areas examined | Result / limits |
|---|---|---:|---|---|
| `MyNameChef` | 0a33b544; dirty (2 entries) | 333 | Queries, RNG, audio/settings, UI sounds, rendering and component/system declarations | UP-01; feedback helper deferred; query/RNG adoption |
| `afterhours-template` | c49f7051; dirty (2 entries) | 69 | Audio/settings, letterbox and rendering helpers | UP-01; sound/viewport adoption |
| `armchair_coach` | d3801b95; dirty (4 entries) | 166 | C++ skeleton; browser ECS, input, movement, camera and UI | Existing facilities cover generic needs; baseball logic remains local |
| `armchair_coach-navi` | 0b028c41; clean | 161 | Compared C++ skeleton; separately searched browser branch, AI and formation code | Branch differences accounted for; no new shared API justified |
| `break-ross` | 15e449c4; dirty (1 entries) | 60 | Camera, input, letterbox, rendering phases and maze/reveal helpers | Viewport/input adoption; game mechanics excluded |
| `cartographer` | f63bd979; dirty (5 entries) | 77 | Input prompts, editor/gallery UI, profiling, text/atlas and E2E helpers | UP-06; existing get_bindings, profiling and diagnostics adoption |
| `cross-profile-url` | 305f2916; clean | 3 | Content/background interception and native-message routing | Browser profile launcher; outside afterhours scope |
| `df` | ff950df1; clean | 9 | React App and UI/helper structure | Tabs/progress/controls already supported; economy excluded |
| `dotfiles` | 7b137fa1; dirty (3 entries) | 4 | Inventory and installer helpers | Personal configuration excluded; no product UI candidate |
| `endless-dance-chaos` | 0a335d7e; dirty (7 entries) | 40 | Loop/substeps, RNG, audio, crowd/schedule, render/save/test helpers | UP-01; periodic timer deferred; fixed-step/input support exists |
| `floatinghotel` | 4ef6c4ed; dirty, changed during review | 300 | Watcher, settings, frame activity and UI/utility source; selected old gap claims | UP-02 adoption and UP-09; concurrent edits limit snapshot coverage |
| `food` | fb744f81; clean | 12 | ECS, rendering, input, buttons and entity creation | Existing input/camera/UI support; p5 dependency excluded |
| `gabeochoa.github.com` | e3fb8e46; dirty (5 entries) | 56 | Site handlers, sand pixel rendering and presentation/chart structure | UP-08; generated/exported content excluded |
| `hanabi` | 7d167069; clean | 298 | API/UI declarations, text selection, clipboard/E2E, native dialogs, frame activity | UP-03/04; large systems sampled, no stored messages or service calls |
| `jkubkrehel-skills` | 79a09456; clean | 0 | Instructions, README and inventory | Documentation/skills only; no app capability |
| `kart-afterhours` | fd5a77f9; clean | 69 | Settings/audio, UI sound wiring, old gap claims, viewport and cooldowns | UP-01/02; stale rendering complaints resolved in newer library |
| `laf` | non-git directory | 0 | Directory inventory | No source available |
| `last_mile` | 24b16643; clean | 21 | Frontend/simulation structure; dense grid, upgrades, audio/settings | UP-08; existing render-target caching; game rules excluded |
| `mailboy` | 627dc968; dirty (4 entries) | 28 | Cart/spring/rail/camera components, rendering, loop and profiler | Profiling adoption; gameplay spring utility not justified yet |
| `mine` | c078afcd; clean | 9 | ECS, sketch/render/control source, setup and buttons | Existing UI/input; mining rules and p5 dependency excluded |
| `pharmasea` | fec888b3; clean | 451 | Engine, audio/settings, keymap/preload, save replacement, timers/pathfinding | UP-01/02/05; network and save-schema internals not fully traced |
| `pong-afterhours` | f02de58a; dirty (2 entries) | 3 | Small main/input/movement/bounce sample | Existing ECS/input/window support |
| `ponytail` | 45f7d2f8; clean | 31 | Instructions, README and source inventory | Agent/developer tooling; no app capability inferred |
| `prime_pressure` | 895f968c; dirty (1 entries) | 73 | Input/typing, letterbox/rendering, settings and order-system source | Existing viewport/input/timer adoption; gameplay typing remains local |
| `scrubdaddy` | 056ff5f2; dirty (2 entries) | 53 | UI/render/input structure; GameCanvas, upgrade controls, settings and sparklines | UP-07; optional sparkline; economy/mockups/data excluded |
| `skillz` | 656af3a0; dirty (6 entries) | 22 | README and installer/checker/orchestration inventory | Developer tooling and skills; no new app API |
| `smooth_scanner` | unborn git repository; clean | 0 | Directory and Git inventory | Unborn repository; no source available |
| `supermarket-engine` | 2c03fcb5; dirty (1 entries) | 65 | Key persistence, resource/timing/profiling, renderer/entity/window source | UP-05; old renderer and examples sampled, not run |
| `supermarket-react` | bec818e7; clean | 10 | Frontend structure, App, columns and tabs | Grid/tabs/progress already available |
| `synthwave-drive` | non-git directory | 1 | Python/ModernGL context, audio-envelope and render pipeline | Offline film renderer; no afterhours API requirement justified |
| `template` | non-git directory | 25 | Loop, input/E2E, settings and system source | Existing run/backend/test support; no Git metadata |
| `tetr-afterhours` | 387a0d4a; dirty (1 entries) | 8 | Drop/movement timers, collision filters and components | Timer policy deferred; game geometry remains local |
| `ui-afterhours` | 4989adad; dirty (2 entries) | 20 | Playback/injection, UI dumps, router/state, input and styling | Existing E2E scripted playback, injection and dumps |
| `watching-a-movie-a-day-presentation` | f0ae518b; clean | 32 | Presentation/chart code and template wiring | Sparkline/chart opportunity deferred; data ingestion excluded |
| `webhelper` | f0e33345; clean | 21 | Helper/task implementation, rendering, input/settings and loop | Existing facilities; task flow remains local |
| `wordproc` | c70ee2e7; dirty (1 entries) | 114 | Editor/text declarations, input/menu/toolbar, clipboard, dialogs/E2E | UP-03/04/06; serialization/export internals sampled |
| `workchat-pip` | 25349c2c; clean | 1 | Userscript source structure | Browser scraping/storage and platform selectors excluded |
| `ws-ios` | b25fb8ff; clean | 3 | Client UI/renderer and source structure | Basic controls supported; WebSocket protocol excluded |

### Nested project follow-up

`armchair_coach/puzzle` is an owned submodule at `42759cd`, clean at review time.
Its 196 tracked C++ source/header files were searched, with focused reads of
canvas deletion, child components, E2E orphan checks, GIF export, image sampling
and texture generation. These files are additional to the top-level inventory
count. The pass updates the existing image-wrapper and relationship-integrity
gaps and adds UP-10 for inconsistent capture formats across backends. The current deletion sweep includes all four
known child-control types. A future ownership API must address adding new child
types, rather than claiming the old omission remains unfixed. The separate
cascade-delete plan was not started.

## Implemented capabilities and adoption work

These source patterns do not justify new APIs. Adoption can still require consumer-specific validation; this review did not change pins or delete their helpers.

| Existing capability | Consumer evidence | Available implementation / remaining decision |
|---|---|---|
| Current bindings lookup | `cartographer/src/input_action.h:593` walks maps | `ProvidesLayeredInputMapping::get_bindings` already exists; UP-06 covers display/device preference only. |
| Atomic file replacement | Direct settings writes in floatinghotel and pharmasea | `files::write_string_atomic` exists. UP-02 requests integration into the shared settings plugin and correct caller failure handling. |
| Profiling hooks | Cartographer, mailboy and armchair system wrappers | Newer `core/system.h` has `SystemProfileHook`; `e2e_testing/perf_commands.h` has built-in profiling. |
| Grids, tabs, progress bars, tooltips and toasts | supermarket-react, df, scrubdaddy and last_mile | Existing UI components cover these controls. Do not add duplicate widget implementations. |
| Render-target readback | Puzzle GIF export and node pixel sampling | `capture_render_texture_to_memory` exists, but raylib returns PNG and sokol/Metal returns RGBA. UP-10 records the contract defect; the image-wrapper gap asks for portable raw RGBA readback. |
| Render-target caching | `last_mile/src/Grid.tsx:1023` caches its static layer | Existing render-texture operations cover caching; UP-08 is specifically mutable CPU pixel upload. |
| Letterbox coordinate transforms | MyNameChef, prime_pressure, break-ross, template and kart copies | Newer `window_manager.h` supplies `Viewport`, `content_viewport` and forward/inverse mapping. |
| Opacity, square focus rings, gap sizing and style forwarding | Kart historical gap file | Fixed in newer vendor; direct current implementation reads rejected these stale reports. |
| Timers, seeded RNG and pathfinding workers | Pharmasea, MyNameChef and endless helpers | Existing plugins cover generic needs. Replay behavior and game-specific movement/collision remain caller-owned. |
| Read-only text and selection geometry | Hanabi text selection/find helpers | `with_readonly`, `TextLayoutCache`, `offset_at_point` and `selection_rects` exist; no blanket missing-selection request. |
| Right-click E2E and UI tree/draw diagnostics | Wordproc and ui-afterhours local commands | Newer E2E handlers and dumps exist; comments claiming their absence are stale. |
| Custom toolbar icons | Wordproc overlay renderer | `with_texture` and custom draw callbacks exist. Artwork and styling remain app content. |
| Fixed-step/headless execution and instant animation | Endless loop and existing UI demos | Newer run configuration and existing animation controls cover these mechanisms; no replacement scheduler proposed. |

## Deliberately excluded from upstream proposals

Game rules, save schemas, economies, combat events, baseball simulation, terrain/maze behavior, Git refresh classification, browser profile routing, chat transport, service APIs and agent orchestration remain application responsibilities. Direct graphics calls alone were not treated as proof of a missing library API. Platform picker/watcher/accessibility requests are optional boundaries, not proposals to absorb their applications.

No tests were rerun for this documentation-only change. Checks covered project assignment completeness, source-reference validity, duplicate/implemented-feature filtering, and Markdown diff consistency. Implementation validation scenarios live beside each gap.
