# afterhours gap work: status

Snapshot 2026-11-16, before a machine restart. PRs are open against gabeochoa/afterhours, and nothing is merged. The ranked gap list is in [PRIORITIZED.md](PRIORITIZED.md).

## Review order

1. **Stacked PRs: merge the base first.** #86 needs #85; #104 needs #88; #121 needs #92; #125 needs #95; #127 needs #94; #138 needs #79. Each says so in its first line.
2. **Visible change:** #119 (colors) moves `UI_RED`/`UI_GREEN`/`UI_BLUE` to raylib's values on sokol, so floatinghotel and hanabi pixels change. `UI_WHITE`/`UI_PINK` are still per-backend and flagged in the PR.
3. **Expect CHANGELOG.md conflicts:** almost every PR adds a line at the top. When merging in sequence, keep both sides.
4. **Full suite:** `make test` was run once with #78 to #86 combined. The only failures were `settings_save_test` (fixed in #82) and `config_size_test` (already failing on main, fixed in #87). Later PRs ran only their own tests and the existing tests for the code they touch. Run the full suite before merging a batch.
5. **Your unpushed local commit** `eab28aa` (DPI mouse fix) on ~/p/afterhours main is not in any PR. Every branch is based on origin/main.

## Open PRs (72)

| PR | Branch | Base | Title |
|---|---|---|---|
| #78 | `gap/jobs` | main | jobs: worker pool that delivers results on the owner thread |
| #79 | `gap/wraptext` | main | text: wrap_lines returns byte ranges and widths per wrapped line |
| #80 | `gap/e2e-waits` | main | e2e: wait_wall and wait_until_wall wait in real time |
| #81 | `gap/image` | main | image: public CPU decode, size query, and RGBA texture upload |
| #82 | `gap/atomic` | main | files: durable atomic writes with a temp per write |
| #83 | `gap/logging` | main | docs: spell out the replaced-logging host contract |
| #84 | `gap/validate-enum` | main | ui: validate_enum_has_value expands to one statement |
| #85 | `gap/e2e-keys` | main | e2e: graphics key reads go through the injector |
| #86 | `gap/e2e-gamepad` | **gap/e2e-keys** | e2e: gamepad injection for buttons, axes and pad availability |
| #87 | `gap/config-size` | main | ui: merge render_layer_offset in apply_overrides |
| #88 | `gap/textfit` | main | text: ellipsize_text and fit_text_prefix, shared with the renderer |
| #89 | `gap/axis-edges` | main | input: gamepad axes report a pressed edge, not every held frame |
| #90 | `gap/controlled-widgets` | main | ui: slider and toggle_switch follow the caller's value |
| #91 | `gap/test-file-roots` | main | files: TestRoots redirects save, config and temp for a test |
| #92 | `gap/glyph-placeholder` | main | text: draw a visible replacement for codepoints no face covers |
| #93 | `gap/menu-rows` | main | menu: rows take padding, theme usage and hover from the menu config |
| #94 | `gap/font-points` | main | text: font_size_per_point converts point sizes to font sizes |
| #95 | `gap/e2e-command-shape` | main | e2e: register_command with typed arguments, and expect() |
| #96 | `gap/focus-ring-contrast` | main | ui: Theme::focus_ring_contrast turns off the ring's contrast edges |
| #97 | `gap/focus-text-input` | main | ui: set_focus on a text input's returned entity focuses its field |
| #98 | `gap/layout-inspect` | main | e2e: dump_ui writes resolved padding, margin and gap |
| #99 | `gap/hover-tint` | main | ui: with_hoverable and with_skip_hover_override config setters |
| #100 | `gap/drag-hit` | main | ui: drag groups hit-test the scrolled, clipped rect |
| #101 | `gap/grid-snap-extents` | main | autolayout: children() sizes and scroll extents follow snapped placement |
| #102 | `gap/tree-row-renderer` | main | tree_view: render_row callback builds custom row contents |
| #103 | `gap/e2e-type-codepoints` | main | e2e: type queues Unicode codepoints, not UTF-8 bytes |
| #104 | `gap/graphemes` | **gap/textfit** | text: step carets and deletes by grapheme cluster |
| #105 | `gap/font-headless-guard` | main | fonts: raylib file loaders no longer crash without a window |
| #106 | `gap/catalog-builds` | main | examples: catalog makefiles name the none backend |
| #107 | `gap/translation-cstdlib` | main | translation: include <cstdlib> before fmt |
| #108 | `gap/fixed-timestep` | main | core: SystemManager fixed tick rate and catch-up cap are settable |
| #109 | `gap/key-repeat` | main | input: Repeat paces a held input with a delay and a rate |
| #110 | `gap/host-types` | main | docs: host type macros |
| #111 | `gap/none-measure` | main | text: NONE backend estimates text width instead of zero |
| #112 | `gap/e2e-results` | main | e2e: results(), --results-json, single-script verdict line |
| #113 | `gap/rng-streams` | main | random: RngStream, an owned seeded generator that saves and resumes |
| #114 | `gap/enum-actions` | main | input: is_held / is_pressed and an enum-keyed mapping |
| #115 | `gap/cache-dir` | main | files: get_cache_path for regenerable data, apart from config |
| #116 | `gap/exact-mods` | main | input: the more specific chord wins when two match one key |
| #117 | `gap/e2e-screenshot-fail` | main | e2e: a screenshot callback returning false fails the script |
| #118 | `gap/measure-inputs` | main | autolayout: measure text with the drawn weight and letter spacing |
| #119 | `gap/colors` | main | colors: raylib's palette names on every backend |
| #120 | `gap/local-time` | main | time: local_time_now returns the wall-clock local date and time |
| #121 | `gap/raylib-fallback` | **gap/glyph-placeholder** | text: raylib falls back across loaded faces per codepoint |
| #122 | `gap/frame-rate-stats` | main | stats: FrameRateStats averages fps over a window |
| #123 | `gap/value-types` | main | developer: Vector4Type, MatrixType and graphics::Camera2D |
| #124 | `gap/disabled` | main | ui: disabled state no longer depends on a HasLabel |
| #125 | `gap/e2e-shadowed-command` | **gap/e2e-command-shape** | e2e: app commands registered first win; shadowed ones warn |
| #126 | `gap/crash-reentry` | main | crash: report without log_error, and only once |
| #127 | `gap/font-memory` | **gap/font-points** | fonts: load_font_from_memory, and sokol loads each face once |
| #128 | `gap/initial-focus` | main | ui: grab_initial_focus, and an automatic grab keeps the modality |
| #129 | `gap/e2e-defer-unknown` | main | e2e: register_unknown_handler can name commands another state handles |
| #130 | `gap/wrappers` | main | backends: draw_fps, get_key_pressed, unload_font, NULL_KEY, texture wrap |
| #131 | `gap/e2e-is-running` | main | e2e: is_e2e_running() for hiding nondeterministic HUD readouts |
| #132 | `gap/cxxabi-has-include` | main | core: include cxxabi.h only when the header exists |
| #133 | `gap/default-font-leftovers` | main | fonts: debug overlay and load errors use the set default font |
| #134 | `gap/concept-declval` | main | graphics: concepts use declval, not C compound literals |
| #135 | `gap/frame-dt` | main | sokol: get_frame_time is the time since the last admitted frame |
| #136 | `gap/e2e-hold-key` | main | E2E: hold_key / release_key builtins |
| #137 | `gap/wheel-input` | main | ui: wheel hit-tests the drawn scroll view; Shift+wheel goes sideways |
| #138 | `gap/wrap-hard-breaks` | **gap/wraptext** | text: wrap breaks words wider than the line between graphemes |
| #139 | `gap/seed-from-id` | main | query: seed_from_id outside the SKIP_ENTITY_QUERY_MODIFICATIONS split |
| #140 | `gap/e2e-ui-gestures` | main | e2e: mouse_down_ui, mouse_move_ui and scroll_wheel_ui by element name |
| #141 | `gap/sokol-headless-run` | main | sokol: run() honours DisplayMode::Headless |
| #142 | `gap/middle-click` | main | ui: middle button in MousePointerState and is_middle_click |
| #143 | `gap/catalog-fmt-include` | main | examples: catalog common.mk finds Homebrew headers |
| #144 | `gap/sound-wrappers` | main | audio: stop, pause, resume, pitch, pan and device wrappers |
| #145 | `gap/inert` | main | ui: with_inert keeps a subtree from taking clicks or focus |
| #146 | `gap/audio-lifecycle` | main | graphics: Config/RunConfig audio flag opens the audio device |
| #147 | `gap/takes-focus` | main | ui: with_takes_focus(false) clicks without moving keyboard focus |
| #148 | `gap/consumer-builds` | main | docs: minimal consumer builds and which one a test suite wants |
| #149 | `gap/field-arrows` | main | text_input: the field owns Up and Down while focused |

## Waiting on a decision (19)

Each note in [decisions/](decisions/) has the problem, options, a recommendation and a size estimate. Pick an option and it becomes a PR.

| Gap | Recommendation (short) |
|---|---|
| [accessibility](decisions/decide-accessibility.md) | Recommendation  Option 1. It's the testable half, it fixes the "a space is the only label" problem for every consumer's tests and tooling, and it's th… |
| [distance-sq](decisions/decide-distance-sq.md) | Recommendation  Option 1 if Gabe is fine with three projects deleting their copies on bump (the error is immediate and the fix is obvious). Otherwise … |
| [e2e-live-resize](decisions/decide-e2e-live-resize.md) | Recommendation C, done as two PRs. B first (small, testable headless, useful to both projects), then the #595 hook as its own sokol/macOS PR, verified… |
| [e2e-name-lookup](decisions/decide-e2e-name-lookup.md) | Recommendation C. B is about 120 lines: one `resolve_ui(name_or_text, scope)` used by every handler, an `under=` key parsed from the args the way `ass… |
| [e2e-one-call-setup](decisions/decide-e2e-one-call-setup.md) | Recommendation B. pong-gaps is already the answer for this gap and for parts of #98 and gap/e2e-hold-key. Splitting it lets the e2e piece land without… |
| [focus-shortcuts](decisions/decide-focus-shortcuts.md) | Recommended.** `ComponentConfig::with_shortcut(action, fn)` stores handlers in a `HasShortcuts` component. A `DispatchShortcuts` system in the post-up… |
| [input-collector-base](decisions/decide-input-collector-base.md) | Recommended.** gap/enum-actions (`input::is_pressed(Action)` / `is_held(Action)`) already removes the collector from `ImpulseBall`: its `any_of` over … |
| [macos-desktop](decisions/decide-macos-desktop.md) | Recommendation  Option 1 now, and option 3 if a third desktop app shows up. Only the two desktop-style apps want this, and both already ship working a… |
| [memory-budgets](decisions/decide-memory-budgets.md) | Recommendation  Do option 1 now as its own small PR; it's concrete and two caches move onto it. Option 2 next, if Gabe agrees afterhours should own an… |
| [scrollbar-policy](decisions/decide-scrollbar-policy.md) | Recommendation  Option 1 now, then auto-hide (the visibility half of option 2) as a follow-up PR. The gutter is small and opt-in. Auto-hide needs Gabe… |
| [selectable-text](decisions/decide-selectable-text.md) | Recommendation  **A**, after gap/wraptext lands, limited to per-element selection. Fold the `text_area` readonly wiring from B into it as a separate s… |
| [slider-parts](decisions/decide-slider-parts.md) | Recommendation  Option 1. It's opt-in, so no baselines move. It matches how toggle_switch and checkbox are styled, and it lets cartographer delete its… |
| [sokol-antialiasing](decisions/decide-sokol-antialiasing.md) | Recommendation  Option 2, in two PRs: (a) the MSAA render target + resolve with a headless coverage test, then (b) the windowed flag. That keeps E2E c… |
| [sokol-audio](decisions/decide-sokol-audio.md) | Recommendation  Option 3 now (tiny, removes the silent failure), and option 1 when the first sokol game needs sound. miniaudio is the same engine rayl… |
| [sokol-spacing](decisions/decide-sokol-spacing.md) | Recommendation  **B, unclamped.** It is about 10 lines plus a sokol CPU-fontstash test like `font_atlas_measure_test`: spacing widens measure by `(n-1… |
| [sokol-window-resize](decisions/decide-sokol-window-resize.md) | Recommendation  Option 1, ported by whoever can run floatinghotel's native resize probe (Metal changes have to be tested from floatinghotel). The code… |
| [storage-codecs](decisions/decide-storage-codecs.md) | recommended).**    - `struct Codec<T> { int version; std::function<std::string(const T&)> encode; std::function<std::expected<T, DecodeError>(std::str… |
| [tab-order](decisions/decide-tab-order.md) | Recommended.** Each frame, build the ordered list of focusable ids instead of streaming. Tab and Shift+Tab move to the next or previous id within the … |
| [transform-velocity](decisions/decide-transform-velocity.md) | recommendation). Revisit when a second 2D game wants the same `{pos, prev}` + `{vel}` shape. 2. **Add a minimal 2D pair.** `afterhours::Position2D { v… |

## In progress

- `gap/input-remap`: **WIP, pushed, no PR.** Remapping (`set_binding`/`clear_binding`) for the non-layered input map. Stopped mid-item and untested.
- Input triage items marked "pr" but not started: the click-count (double-click) gesture.

## Tier 4 triage

Only input has been triaged: [triage/triage-input.md](triage/triage-input.md). Of 25 gaps, 8 are worth a PR, 7 need a decision, 8 are app-specific and 2 are already fixed. #137, #142, #145, #147 and #149 came out of it. ui-widgets (39), ui-layout (26), rendering (25), text (18), ecs-core (13), e2e (12), networking (11), platform (10) and the smaller areas haven't been triaged.

## Not started

- Tier 2 Metal gaps: live resize painting stale frames, per-frame autorelease pool, GPU object retirement timing, non-rectangular child clipping. Test these from floatinghotel.
- Audit follow-ups ([../RAYLIB_USAGE_AUDIT.md](../RAYLIB_USAGE_AUDIT.md)): sokol audio (miniaudio), sokol gamepads, custom shaders on both backends, runtime window/DPI queries, CPU image flip/export/readback.
- Recommended: split the unmerged `pong-gaps` branch into themed PRs. It already solves one-call E2E setup and overlaps #98 and #136.

## Notes for whoever resumes

- After the reboot, run `git -C ~/p/afterhours worktree prune`: the `/tmp/ah-gap-*` worktrees will be gone, but their branches are in ~/p/afterhours/.git.
- [pr-tooling/](pr-tooling/) has the agent rules (RULES.md) and the scripts used to review and open each PR: `review_open.sh` flags new comments and TODOs, and requires an example.
- Never use `git stash` across agents. The stash is shared by every worktree of a repo, and it swapped work between agents twice; both swaps were caught and repaired. The rescue patches are in pr-tooling/.
- Found while working, also fixed in PRs: a runner bug where `register_before_builtins` never overrode a builtin (#125, #129), a headless raylib font-load segfault (#105), `seed_from_id` missing in one build branch (#139), 17 catalog examples that didn't build (#106, #143), and `render_layer_offset` dropped by `apply_overrides` (#87).
